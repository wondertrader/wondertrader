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
#include <unordered_map>
#include "DataDefine.h"
#include "../WtDataWriter/MysqlDB.hpp"

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSTypes.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_OTP_BEGIN
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
NS_OTP_END

typedef std::shared_ptr<MysqlDb>	MysqlDbPtr;

USING_NS_OTP;

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

	typedef std::unordered_map<std::string, HftDataList<WTSTickStruct>>		TickCache;
	typedef std::unordered_map<std::string, HftDataList<WTSOrdDtlStruct>>	OrdDtlCache;
	typedef std::unordered_map<std::string, HftDataList<WTSOrdQueStruct>>	OrdQueCache;
	typedef std::unordered_map<std::string, HftDataList<WTSTransStruct>>	TransCache;


	typedef struct _BarsList
	{
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_cursor;
		uint32_t		_count;
		uint32_t		_times;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_cursor(UINT_MAX), _count(0), _times(1){}
	} BarsList;

	typedef std::unordered_map<std::string, BarsList>	BarsCache;

	typedef enum tagTaskPeriodType
	{
		TPT_None,		//不重复
		TPT_Minute = 4,	//分钟线周期
		TPT_Daily = 8,	//每个交易日
		TPT_Weekly,		//每周，遇到节假日的话要顺延
		TPT_Monthly,	//每月，遇到节假日顺延
		TPT_Yearly		//每年，遇到节假日顺延
	}TaskPeriodType;

	typedef struct _TaskInfo
	{
		uint32_t	_id;
		char		_name[16];		//任务名
		char		_trdtpl[16];	//交易日模板
		char		_session[16];	//交易时间模板
		uint32_t	_day;			//日期，根据周期变化，每日为0，每周为0~6，对应周日到周六，每月为1~31，每年为0101~1231
		uint32_t	_time;			//时间，精确到分钟
		bool		_strict_time;	//是否是严格时间，严格时间即只有时间相等才会执行，不是严格时间，则大于等于触发时间都会执行

		uint64_t	_last_exe_time;	//上次执行时间，主要为了防止重复执行

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
	bool		cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	从数据库缓存历史数据
	 */
	bool		cacheRawBarsFromDB(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	从自定义数据文件缓存历史tick数据
	 */
	bool		cacheRawTicksFromBin(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	从csv文件缓存历史tick数据
	 */
	bool		cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate);

	void		onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0);

	void		loadFees(const char* filename);

	void		replayHftDatas(uint64_t stime, uint64_t etime);

	uint64_t	replayHftDatasByDay(uint32_t curTDate);

	void		replayUnbars(uint64_t stime, uint64_t etime, uint32_t endTDate = 0);

	inline bool		checkTicks(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderDetails(const char* stdCode, uint32_t uDate);

	inline bool		checkOrderQueues(const char* stdCode, uint32_t uDate);

	inline bool		checkTransactions(const char* stdCode, uint32_t uDate);

	void		checkUnbars();

	bool		loadStkAdjFactors(const char* adjfile);

	bool		loadStkAdjFactorsFromDB();

	void		initDB();

	bool		checkAllTicks(uint32_t uDate);

	inline	uint64_t	getNextTickTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdQueTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextOrdDtlTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);
	inline	uint64_t	getNextTransTime(uint32_t curTDate, uint64_t stime = UINT64_MAX);

public:
	bool init(WTSVariant* cfg);

	void run();

	void register_sink(IDataSink* listener){ _listener = listener; }

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

	bool	is_tick_enabled() const{ return _tick_enabled; }

private:
	IDataSink*		_listener;

	TickCache		_ticks_cache;	//tick缓存
	OrdDtlCache		_orddtl_cache;	//order detail缓存
	OrdQueCache		_ordque_cache;	//order queue缓存
	TransCache		_trans_cache;	//transaction缓存

	BarsCache		_bars_cache;	//K线缓存
	BarsCache		_unbars_cache;	//未订阅的K线缓存

	TaskInfoPtr		_task;

	std::string		_main_key;
	std::string		_min_period;	//最小K线周期，这个主要用于未订阅品种的信号处理上
	bool			_tick_enabled;
	std::map<std::string, WTSTickStruct>	_day_cache;	//每日Tick缓存,当tick回放未开放时，会用到该缓存
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
	typedef std::unordered_map<std::string, FeeItem>	FeeMap;
	FeeMap		_fee_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_map<std::string, double> PriceMap;
	PriceMap		_price_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_set<uint32_t> SIDSet;
	typedef std::unordered_map<std::string, SIDSet>	StraSubMap;
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
	typedef std::unordered_map<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	inline const AdjFactorList& getAdjFactors(const char* code, const char* exchg)
	{
		char key[20] = { 0 };
		sprintf(key, "%s.%s", exchg, code);
		return _adj_factors[key];
	}

	typedef struct _DBConfig
	{
		bool	_active;
		char	_host[64];
		int32_t	_port;
		char	_dbname[32];
		char	_user[32];
		char	_pass[32];

		_DBConfig() { memset(this, 0, sizeof(_DBConfig)); }
	} DBConfig;

	DBConfig	_db_conf;
	MysqlDbPtr	_db_conn;
};

