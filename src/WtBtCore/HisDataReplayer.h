/*!
 * \file HisDataReplayer.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include "HisDataMgr.h"
#include "../WtDataStorage/DataDefine.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSTypes.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_WTP_BEGIN
class WTSTickData;
class WTSVariant;
class WTSKlineSlice;
class WTSTickSlice;
class WTSOrdDtlSlice;
class WTSOrdQueSlice;
class WTSTransSlice;
class WTSSessionInfo;
class WTSCommodityInfo;

class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;

class EventNotifier;
NS_WTP_END

USING_NS_WTP;

class IDataSink
{
public:
	virtual void	handle_tick(const char* stdCode, WTSTickData* curTick) = 0;
	virtual void	handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue) {};
	virtual void	handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl) {};
	virtual void	handle_transaction(const char* stdCode, WTSTransData* curTrans) {};
	virtual void	handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual void	handle_schedule(uint32_t uDate, uint32_t uTime) = 0;

	virtual void	handle_init() = 0;
	virtual void	handle_session_begin(uint32_t curTDate) = 0;
	virtual void	handle_session_end(uint32_t curTDate) = 0;
	virtual void	handle_replay_done() {}
};

/*
 *	历史数据加载器的回调函数
 *	@obj	回传用的，原样返回即可
 *	@key	数据缓存的key
 *	@bars	K线数据
 *	@count	K线条数
 *	@factor	复权因子，最新的复权因子，如果是后复权，则factor不为1.0，如果是前复权，则factor必须为1.0
 */
typedef void(*FuncReadBars)(void* obj, WTSBarStruct* firstBar, uint32_t count);

/*
 *	加载复权因子回调
 *	@obj	回传用的，原样返回即可
 *	@stdCode	合约代码
 *	@dates
 */
typedef void(*FuncReadFactors)(void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count);

typedef void(*FuncReadTicks)(void* obj, WTSTickStruct* firstTick, uint32_t count);

class IBtDataLoader
{
public:
	/*
	 *	加载最终历史K线数据
	 *	和loadRawHisBars的区别在于，loadFinalHisBars，系统认为是最终所需数据，不在进行加工，例如复权数据、主力合约数据
	 *	loadRawHisBars是加载未加工的原始数据的接口
	 *
	 *	@obj	回传用的，原样返回即可
	 *	@stdCode	合约代码
	 *	@period	K线周期
	 *	@cb		回调函数
	 */
	virtual bool loadFinalHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	加载原始历史K线数据
	 *
	 *	@obj	回传用的，原样返回即可
	 *	@stdCode	合约代码
	 *	@period	K线周期
	 *	@cb		回调函数
	 */
	virtual bool loadRawHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	加载全部除权因子
	 */
	virtual bool loadAllAdjFactors(void* obj, FuncReadFactors cb) = 0;

	/*
	 *	根据合约加载除权因子
	 *
	 *	@stdCode	合约代码
	 */
	virtual bool loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb) = 0;

	/*
	 *	加载历史Tick数据
	 */
	virtual bool loadRawHisTicks(void* obj, const char* stdCode, uint32_t uDate, FuncReadTicks cb) = 0;

	virtual bool isAutoTrans() { return true; }
};

class HisDataReplayer
{

private:
	template <typename T>
	class HftDataList
	{
	public:
		std::string		_code;
		uint32_t		_date;
		uint32_t		_cursor;
		uint32_t		_count;

		std::vector<T> _items;

		HftDataList() :_cursor(UINT_MAX), _count(0), _date(0){}
	};

	typedef faster_hashmap<std::string, HftDataList<WTSTickStruct>>		TickCache;
	typedef faster_hashmap<std::string, HftDataList<WTSOrdDtlStruct>>	OrdDtlCache;
	typedef faster_hashmap<std::string, HftDataList<WTSOrdQueStruct>>	OrdQueCache;
	typedef faster_hashmap<std::string, HftDataList<WTSTransStruct>>	TransCache;


	typedef struct _BarsList
	{
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_cursor;
		uint32_t		_count;
		uint32_t		_times;

		std::vector<WTSBarStruct>	_bars;
		double			_factor;	//最后一条复权因子

		_BarsList() :_cursor(UINT_MAX), _count(0), _times(1), _factor(1){}
	} BarsList;

	/*
	 *	By Wesley @ 2022.03.13
	 *	这里把缓存改成智能指针
	 *	因为有用户发现如果在oncalc的时候获取未在oninit中订阅的K线的时候
	 *	因为使用BarList的引用，当K线缓存的map重新插入新的K线以后
	 *	引用的地方失效了，会引用到错误地址
	 *	我怀疑这里有可能是重新拷贝了一下数据
	 *	这里改成智能指针就能避免这个问题，因为不管map自己的内存如何组织
	 *	智能指针指向的地址都是不会变的
	 */
	typedef std::shared_ptr<BarsList> BarsListPtr;
	typedef faster_hashmap<std::string, BarsListPtr>	BarsCache;

	typedef enum tagTaskPeriodType
	{
		TPT_None,		//不重复
		TPT_Minute = 4,	//分钟线周期
		TPT_Daily = 8,	//每个交易日
		TPT_Weekly,		//每周,遇到节假日的话要顺延
		TPT_Monthly,	//每月,遇到节假日顺延
		TPT_Yearly		//每年,遇到节假日顺延
	}TaskPeriodType;

	typedef struct _TaskInfo
	{
		uint32_t	_id;
		char		_name[16];		//任务名
		char		_trdtpl[16];	//交易日模板
		char		_session[16];	//交易时间模板
		uint32_t	_day;			//日期,根据周期变化,每日为0,每周为0~6,对应周日到周六,每月为1~31,每年为0101~1231
		uint32_t	_time;			//时间,精确到分钟
		bool		_strict_time;	//是否是严格时间,严格时间即只有时间相等才会执行,不是严格时间,则大于等于触发时间都会执行

		uint64_t	_last_exe_time;	//上次执行时间,主要为了防止重复执行

		TaskPeriodType	_period;	//任务周期
	} TaskInfo;

	typedef std::shared_ptr<TaskInfo> TaskInfoPtr;



public:
	HisDataReplayer();
	~HisDataReplayer();

private:
	/*
	 *	从自定义数据文件缓存历史数据
	 */
	bool		cacheRawBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	从csv文件缓存历史数据
	 */
	bool		cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	从自定义数据文件缓存历史tick数据
	 */
	bool		cacheRawTicksFromBin(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	从csv文件缓存历史tick数据
	 */
	bool		cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	从外部加载器缓存历史数据
	 */
	bool		cacheFinalBarsFromLoader(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	从外部加载器缓存历史tick数据
	 */
	bool		cacheRawTicksFromLoader(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	缓存整合的期货合约历史K线（针对.HOT//2ND）
	 */
	bool		cacheIntegratedFutBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	缓存复权股票K线数据
	 */
	bool		cacheAdjustedStkBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	void		onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0, bool tickSimulated = true);

	void		loadFees(const char* filename);

	bool		replayHftDatas(uint64_t stime, uint64_t etime);

	uint64_t	replayHftDatasByDay(uint32_t curTDate);

	void		replayUnbars(uint64_t stime, uint64_t etime, uint32_t endTDate = 0);

	inline bool		checkTicks(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderDetails(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderQueues(const char* stdCode, uint32_t uDate);

	inline bool		checkTransactions(const char* stdCode, uint32_t uDate);

	void		checkUnbars();

	bool		loadStkAdjFactorsFromFile(const char* adjfile);

	bool		loadStkAdjFactorsFromLoader();

	bool		checkAllTicks(uint32_t uDate);

	inline	uint64_t	getNextTickTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdQueTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdDtlTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextTransTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);

	void		reset();


	void		dump_btstate(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime, double progress, int64_t elapse);
	void		notify_state(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime, double progress);

	uint32_t	locate_barindex(const std::string& key, uint64_t curTime, bool bUpperBound = false);

	/*
	 *	按照K线进行回测
	 *
	 *	@bNeedDump	是否将回测进度落地到文件中
	 */
	void	run_by_bars(bool bNeedDump = false);

	/*
	 *	按照定时任务进行回测
	 *
	 *	@bNeedDump	是否将回测进度落地到文件中
	 */
	void	run_by_tasks(bool bNeedDump = false);

	/*
	 *	按照tick进行回测
	 *
	 *	@bNeedDump	是否将回测进度落地到文件中
	 */
	void	run_by_ticks(bool bNeedDump = false);

public:
	bool init(WTSVariant* cfg, EventNotifier* notifier = NULL, IBtDataLoader* dataLoader = NULL);

	bool prepare();

	/*
	 *	运行回测
	 *
	 *	@bNeedDump	是否将回测进度落地到文件中
	 */
	void run(bool bNeedDump = false);
	
	void stop();

	void clear_cache();

	inline void set_time_range(uint64_t stime, uint64_t etime)
	{
		_begin_time = stime;
		_end_time = etime;
	}

	inline void enable_tick(bool bEnabled = true)
	{
		_tick_enabled = bEnabled;
	}

	inline void register_sink(IDataSink* listener, const char* sinkName) 
	{
		_listener = listener; 
		_stra_name = sinkName;
	}

	void register_task(uint32_t taskid, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	WTSKlineSlice* get_kline_slice(const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, bool isMain = false);

	WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);

	WTSTickData* get_last_tick(const char* stdCode);

	uint32_t get_date() const{ return _cur_date; }
	uint32_t get_min_time() const{ return _cur_time; }
	uint32_t get_raw_time() const{ return _cur_time; }
	uint32_t get_secs() const{ return _cur_secs; }
	uint32_t get_trading_date() const{ return _cur_tdate; }

	double calc_fee(const char* stdCode, double price, double qty, uint32_t offset);
	WTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	WTSCommodityInfo*	get_commodity_info(const char* stdCode);
	double get_cur_price(const char* stdCode);

	void sub_tick(uint32_t sid, const char* stdCode);
	void sub_order_queue(uint32_t sid, const char* stdCode);
	void sub_order_detail(uint32_t sid, const char* stdCode);
	void sub_transaction(uint32_t sid, const char* stdCode);

	inline bool	is_tick_enabled() const{ return _tick_enabled; }

	inline bool	is_tick_simulated() const { return _tick_simulated; }

	inline void update_price(const char* stdCode, double price)
	{
		_price_map[stdCode] = price;
	}

private:
	IDataSink*		_listener;
	IBtDataLoader*	_bt_loader;
	std::string		_stra_name;

	TickCache		_ticks_cache;	//tick缓存
	OrdDtlCache		_orddtl_cache;	//order detail缓存
	OrdQueCache		_ordque_cache;	//order queue缓存
	TransCache		_trans_cache;	//transaction缓存

	BarsCache		_bars_cache;	//K线缓存
	BarsCache		_unbars_cache;	//未订阅的K线缓存

	TaskInfoPtr		_task;

	std::string		_main_key;
	std::string		_min_period;	//最小K线周期,这个主要用于未订阅品种的信号处理上
	bool			_tick_enabled;	//是否开启了tick回测
	bool			_tick_simulated;	//是否需要模拟tick
	std::map<std::string, WTSTickStruct>	_day_cache;	//每日Tick缓存,当tick回放未开放时,会用到该缓存
	std::map<std::string, std::string>		_ticker_keys;

	uint32_t		_cur_date;
	uint32_t		_cur_time;
	uint32_t		_cur_secs;
	uint32_t		_cur_tdate;
	uint32_t		_closed_tdate;
	uint32_t		_opened_tdate;

	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;

	std::string		_base_dir;
	std::string		_mode;
	uint64_t		_begin_time;
	uint64_t		_end_time;

	bool			_running;
	bool			_terminated;
	//////////////////////////////////////////////////////////////////////////
	//手续费模板
	typedef struct _FeeItem
	{
		double	_open;
		double	_close;
		double	_close_today;
		bool	_by_volume;

		_FeeItem()
		{
			memset(this, 0, sizeof(_FeeItem));
		}
	} FeeItem;
	typedef faster_hashmap<std::string, FeeItem>	FeeMap;
	FeeMap		_fee_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef faster_hashmap<std::string, double> PriceMap;
	PriceMap		_price_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef faster_hashset<uint32_t> SIDSet;
	typedef faster_hashmap<std::string, SIDSet>	StraSubMap;
	StraSubMap		_tick_sub_map;		//tick数据订阅表
	StraSubMap		_ordque_sub_map;	//orderqueue数据订阅表
	StraSubMap		_orddtl_sub_map;	//orderdetail数据订阅表
	StraSubMap		_trans_sub_map;		//transaction数据订阅表

	//除权因子
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef faster_hashmap<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	const AdjFactorList& getAdjFactors(const char* code, const char* exchg, const char* pid);

	EventNotifier*	_notifier;

	HisDataMgr		_his_dt_mgr;
};

