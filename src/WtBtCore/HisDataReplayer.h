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
#include <set>
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
	virtual void	handle_tick(const char* stdCode, WTSTickData* curTick, bool isBarEnd = true) = 0;
	virtual void	handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue) {};
	virtual void	handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl) {};
	virtual void	handle_transaction(const char* stdCode, WTSTransData* curTrans) {};
	virtual void	handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual void	handle_schedule(uint32_t uDate, uint32_t uTime) = 0;

	virtual void	handle_init() = 0;
	virtual void	handle_session_begin(uint32_t curTDate) = 0;
	virtual void	handle_session_end(uint32_t curTDate) = 0;
	virtual void	handle_replay_done() {}

	virtual void	handle_section_end(uint32_t curTDate, uint32_t curTime) {}
};

/*
 *	��ʷ���ݼ������Ļص�����
 *	@obj	�ش��õģ�ԭ�����ؼ���
 *	@key	���ݻ����key
 *	@bars	K������
 *	@count	K������
 *	@factor	��Ȩ���ӣ����µĸ�Ȩ���ӣ�����Ǻ�Ȩ����factor��Ϊ1.0�������ǰ��Ȩ����factor����Ϊ1.0
 */
typedef void(*FuncReadBars)(void* obj, WTSBarStruct* firstBar, uint32_t count);

/*
 *	���ظ�Ȩ���ӻص�
 *	@obj	�ش��õģ�ԭ�����ؼ���
 *	@stdCode	��Լ����
 *	@dates
 */
typedef void(*FuncReadFactors)(void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count);

typedef void(*FuncReadTicks)(void* obj, WTSTickStruct* firstTick, uint32_t count);

class IBtDataLoader
{
public:
	/*
	 *	����������ʷK������
	 *	��loadRawHisBars���������ڣ�loadFinalHisBars��ϵͳ��Ϊ�������������ݣ����ڽ��мӹ������縴Ȩ���ݡ�������Լ����
	 *	loadRawHisBars�Ǽ���δ�ӹ���ԭʼ���ݵĽӿ�
	 *
	 *	@obj	�ش��õģ�ԭ�����ؼ���
	 *	@stdCode	��Լ����
	 *	@period	K������
	 *	@cb		�ص�����
	 */
	virtual bool loadFinalHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	����ԭʼ��ʷK������
	 *
	 *	@obj	�ش��õģ�ԭ�����ؼ���
	 *	@stdCode	��Լ����
	 *	@period	K������
	 *	@cb		�ص�����
	 */
	virtual bool loadRawHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	����ȫ����Ȩ����
	 */
	virtual bool loadAllAdjFactors(void* obj, FuncReadFactors cb) = 0;

	/*
	 *	���ݺ�Լ���س�Ȩ����
	 *
	 *	@stdCode	��Լ����
	 */
	virtual bool loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb) = 0;

	/*
	 *	������ʷTick����
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
		/*
		 * By Wesley @ 2022.03.21
		 * �α꣬���ڱ����һ�����ݵ�λ�ã�����˵�Ѿ��طŹ�������
		 * δ��ʼ��ʱ���α�ΪUINT_MAX��һ����ʼ�����α��Ȼ�Ǵ���0��
		 */
		std::size_t		_cursor;
		std::size_t		_count;

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
		/*
		 * By Wesley @ 2022.03.21
		 * �α꣬���ڱ����һ�����ݵ�λ�ã�����˵�Ѿ��طŹ�������
		 * δ��ʼ��ʱ���α�ΪUINT_MAX��һ����ʼ�����α��Ȼ�Ǵ���0��
		 */
		uint32_t		_cursor;
		uint32_t		_count;
		uint32_t		_times;

		std::vector<WTSBarStruct>	_bars;
		double			_factor;	//���һ����Ȩ����

		_BarsList() :_cursor(UINT_MAX), _count(0), _times(1), _factor(1){}
	} BarsList;

	/*
	 *	By Wesley @ 2022.03.13
	 *	����ѻ���ĳ�����ָ��
	 *	��Ϊ���û����������oncalc��ʱ���ȡδ��oninit�ж��ĵ�K�ߵ�ʱ��
	 *	��Ϊʹ��BarList�����ã���K�߻����map���²����µ�K���Ժ�
	 *	���õĵط�ʧЧ�ˣ������õ������ַ
	 *	�һ��������п��������¿�����һ������
	 *	����ĳ�����ָ����ܱ���������⣬��Ϊ����map�Լ����ڴ������֯
	 *	����ָ��ָ��ĵ�ַ���ǲ�����
	 */
	typedef std::shared_ptr<BarsList> BarsListPtr;
	typedef faster_hashmap<std::string, BarsListPtr>	BarsCache;

	typedef enum tagTaskPeriodType
	{
		TPT_None,		//���ظ�
		TPT_Minute = 4,	//����������
		TPT_Daily = 8,	//ÿ��������
		TPT_Weekly,		//ÿ��,�����ڼ��յĻ�Ҫ˳��
		TPT_Monthly,	//ÿ��,�����ڼ���˳��
		TPT_Yearly		//ÿ��,�����ڼ���˳��
	}TaskPeriodType;

	typedef struct _TaskInfo
	{
		uint32_t	_id;
		char		_name[16];		//������
		char		_trdtpl[16];	//������ģ��
		char		_session[16];	//����ʱ��ģ��
		uint32_t	_day;			//����,�������ڱ仯,ÿ��Ϊ0,ÿ��Ϊ0~6,��Ӧ���յ�����,ÿ��Ϊ1~31,ÿ��Ϊ0101~1231
		uint32_t	_time;			//ʱ��,��ȷ������
		bool		_strict_time;	//�Ƿ����ϸ�ʱ��,�ϸ�ʱ�伴ֻ��ʱ����ȲŻ�ִ��,�����ϸ�ʱ��,����ڵ��ڴ���ʱ�䶼��ִ��

		uint64_t	_last_exe_time;	//�ϴ�ִ��ʱ��,��ҪΪ�˷�ֹ�ظ�ִ��

		TaskPeriodType	_period;	//��������
	} TaskInfo;

	typedef std::shared_ptr<TaskInfo> TaskInfoPtr;



public:
	HisDataReplayer();
	~HisDataReplayer();

private:
	/*
	 *	���Զ��������ļ�������ʷ����
	 */
	bool		cacheRawBarsFromBin(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bForBars = true);

	/*
	 *	��csv�ļ�������ʷ����
	 */
	bool		cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	���Զ��������ļ�������ʷtick����
	 */
	bool		cacheRawTicksFromBin(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	��csv�ļ�������ʷtick����
	 */
	bool		cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	���ⲿ������������ʷ����
	 */
	bool		cacheFinalBarsFromLoader(const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	���ⲿ������������ʷtick����
	 */
	bool		cacheRawTicksFromLoader(const std::string& key, const char* stdCode, uint32_t uDate);

	/*
	 *	�������ϵ��ڻ���Լ��ʷK�ߣ����.HOT//2ND��
	 */
	bool		cacheIntegratedFutBarsFromBin(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	/*
	 *	���渴Ȩ��ƱK������
	 */
	bool		cacheAdjustedStkBarsFromBin(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period, bool bSubbed = true);

	void		onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0, bool tickSimulated = true);

	void		loadFees(const char* filename);

	bool		replayHftDatas(uint64_t stime, uint64_t etime);

	uint64_t	replayHftDatasByDay(uint32_t curTDate);

	void		simTickWithUnsubBars(uint64_t stime, uint64_t etime, uint32_t endTDate = 0, int pxType = 0);

	void		simTicks(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0, int pxType = 0);

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
	 *	����K�߽��лز�
	 *
	 *	@bNeedDump	�Ƿ񽫻ز������ص��ļ���
	 */
	void	run_by_bars(bool bNeedDump = false);

	/*
	 *	���ն�ʱ������лز�
	 *
	 *	@bNeedDump	�Ƿ񽫻ز������ص��ļ���
	 */
	void	run_by_tasks(bool bNeedDump = false);

	/*
	 *	����tick���лز�
	 *
	 *	@bNeedDump	�Ƿ񽫻ز������ص��ļ���
	 */
	void	run_by_ticks(bool bNeedDump = false);

public:
	bool init(WTSVariant* cfg, EventNotifier* notifier = NULL, IBtDataLoader* dataLoader = NULL);

	bool prepare();

	/*
	 *	���лز�
	 *
	 *	@bNeedDump	�Ƿ񽫻ز������ص��ļ���
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
	double get_day_price(const char* stdCode, int flag = 0);

	std::string get_rawcode(const char* stdCode);

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

	inline IHotMgr*	get_hot_mgr() { return &_hot_mgr; }

private:
	IDataSink*		_listener;
	IBtDataLoader*	_bt_loader;
	std::string		_stra_name;

	TickCache		_ticks_cache;	//tick����
	OrdDtlCache		_orddtl_cache;	//order detail����
	OrdQueCache		_ordque_cache;	//order queue����
	TransCache		_trans_cache;	//transaction����

	BarsCache		_bars_cache;	//K�߻���
	BarsCache		_unbars_cache;	//δ���ĵ�K�߻���

	TaskInfoPtr		_task;

	std::string		_main_key;
	std::string		_min_period;	//��СK������,�����Ҫ����δ����Ʒ�ֵ��źŴ�����
	std::string		_main_period;	//������
	bool			_tick_enabled;	//�Ƿ�����tick�ز�
	bool			_tick_simulated;	//�Ƿ���Ҫģ��tick
	std::map<std::string, WTSTickStruct>	_day_cache;	//ÿ��Tick����,��tick�ط�δ����ʱ,���õ��û���
	std::map<std::string, std::string>		_ticker_keys;

	//By Wesley @ 2022.06.01
	std::set<std::string>		_unsubbed_in_need;	//δ���ĵ���Ҫ��K��

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
	//������ģ��
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
	//By Wesley @ 2022.02.07
	//tick���ݶ����first��contextid��second�Ƕ���ѡ�0-ԭʼ���ģ�1-ǰ��Ȩ��2-��Ȩ
	typedef std::pair<uint32_t, uint32_t> SubOpt;
	typedef faster_hashmap<uint32_t, SubOpt> SubList;
	typedef faster_hashmap<LongKey, SubList>	StraSubMap;
	StraSubMap		_tick_sub_map;		//tick���ݶ��ı�
	StraSubMap		_ordque_sub_map;	//orderqueue���ݶ��ı�
	StraSubMap		_orddtl_sub_map;	//orderdetail���ݶ��ı�
	StraSubMap		_trans_sub_map;		//transaction���ݶ��ı�

	//��Ȩ����
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

