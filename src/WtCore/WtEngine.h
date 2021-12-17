/*!
 * \file WtEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <queue>
#include <functional>
#include <stdint.h>

#include "ParserAdapter.h"
#include "WtFilterMgr.h"


#include "../Includes/FasterDefs.h"
#include "../Includes/RiskMonDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

#include "../Share/BoostFile.hpp"


NS_OTP_BEGIN
class WTSSessionInfo;
class WTSCommodityInfo;
class WTSContractInfo;

class IBaseDataMgr;
class IHotMgr;

class WTSVariant;

class WTSTickData;
struct WTSBarStruct;
class WTSTickSlice;
class WTSKlineSlice;
class WTSPortFundInfo;

class WtDataManager;
class TraderAdapterMgr;

class EventNotifier;

typedef std::function<void()>	TaskItem;

class WtRiskMonWrapper
{
public:
	WtRiskMonWrapper(WtRiskMonitor* mon, IRiskMonitorFact* fact) :_mon(mon), _fact(fact){}
	~WtRiskMonWrapper()
	{
		if (_mon)
		{
			_fact->deleteRiskMonotor(_mon);
		}
	}

	WtRiskMonitor* self(){ return _mon; }


private:
	WtRiskMonitor*		_mon;
	IRiskMonitorFact*	_fact;
};
typedef std::shared_ptr<WtRiskMonWrapper>	WtRiskMonPtr;

class IEngineEvtListener
{
public:
	virtual void on_initialize_event() {}
	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) {}
	virtual void on_session_event(uint32_t uDate, bool isBegin = true) {}
};

class WtEngine : public WtPortContext, public IParserStub
{
public:
	WtEngine();

	inline void set_adapter_mgr(TraderAdapterMgr* mgr) { _adapter_mgr = mgr; }

	void set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs = 0, uint32_t rawTime = 0);

	void set_trading_date(uint32_t curTDate);

	inline uint32_t get_date() { return _cur_date; }
	inline uint32_t get_min_time() { return _cur_time; }
	inline uint32_t get_raw_time() { return _cur_raw_time; }
	inline uint32_t get_secs() { return _cur_secs; }
	inline uint32_t get_trading_date() { return _cur_tdate; }

	inline IBaseDataMgr*		get_basedata_mgr(){ return _base_data_mgr; }
	inline IHotMgr*				get_hot_mgr() { return _hot_mgr; }
	WTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	WTSCommodityInfo*	get_commodity_info(const char* stdCode);
	WTSContractInfo*	get_contract_info(const char* stdCode);

	WTSTickData*	get_last_tick(uint32_t sid, const char* stdCode);
	WTSTickSlice*	get_tick_slice(uint32_t sid, const char* stdCode, uint32_t count);
	WTSKlineSlice*	get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, uint64_t etime = 0);

	void sub_tick(uint32_t sid, const char* code);

	double get_cur_price(const char* stdCode);

	double calc_fee(const char* stdCode, double price, double qty, uint32_t offset);

	inline void setRiskMonitor(WtRiskMonPtr& monitor)
	{
		_risk_mon = monitor;
	}

	inline void regEventListener(IEngineEvtListener* listener)
	{
		_evt_listener = listener;
	}

	//////////////////////////////////////////////////////////////////////////
	//WtPortContext接口
	virtual WTSPortFundInfo* getFundInfo() override;

	virtual void setVolScale(double scale) override;

	virtual bool isInTrading() override;

	virtual void writeRiskLog(const char* fmt, ...) override;

	virtual uint32_t	getCurDate() override;
	virtual uint32_t	getCurTime() override;
	virtual uint32_t	getTradingDate() override;
	virtual uint32_t	transTimeToMin(uint32_t uTime) override{ return 0; }

	//////////////////////////////////////////////////////////////////////////
	/// IParserStub接口
	virtual void handle_push_quote(WTSTickData* newTick, uint32_t hotFlag) override;

public:
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier);

	virtual void run(bool bAsync = false) = 0;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick);

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;

	virtual void on_init(){}
	virtual void on_session_begin();
	virtual void on_session_end();


protected:
	void		load_fees(const char* filename);

	void		load_datas();

	void		save_datas();

	void		append_signal(const char* stdCode, double qty, bool bStandBy);

	void		do_set_position(const char* stdCode, double qty, double curPx = -1);

	void		task_loop();

	void		push_task(TaskItem task);

	void		update_fund_dynprofit();

	bool		init_riskmon(WTSVariant* cfg);

private:
	void		init_outputs();
	inline void	log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee = 0.0);
	inline void	log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
		double profit, double totalprofit = 0);


protected:
	uint32_t		_cur_date;	//当前日期
	uint32_t		_cur_time;		//当前时间, 是1分钟线时间, 比如0900, 这个时候的1分钟线是0901, _cur_time也就是0901, 这个是为了CTA里面方便
	uint32_t		_cur_raw_time;	//当前真实时间
	uint32_t		_cur_secs;	//当前秒数, 包含毫秒
	uint32_t		_cur_tdate;	//当前交易日

	IBaseDataMgr*	_base_data_mgr;	//基础数据管理器
	IHotMgr*		_hot_mgr;		//主力管理器
	WtDataManager*	_data_mgr;		//数据管理器
	IEngineEvtListener*	_evt_listener;

	typedef faster_hashset<uint32_t> SIDSet;
	typedef faster_hashmap<std::string, SIDSet>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StraSubMap		_bar_sub_map;	//K线数据订阅表

	faster_hashset<std::string>		_ticksubed_raw_codes;	//tick订阅表（真实代码模式）
	

	//////////////////////////////////////////////////////////////////////////
	//
	typedef struct _SigInfo
	{
		double		_volume;
		uint64_t	_gentime;

		_SigInfo()
		{
			_volume = 0;
			_gentime = 0;
		}
	}SigInfo;
	typedef faster_hashmap<std::string, SigInfo>	SignalMap;
	SignalMap		_sig_map;

	//////////////////////////////////////////////////////////////////////////
	//信号过滤器
	WtFilterMgr		_filter_mgr;
	EventNotifier*	_notifier;

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
	

	WTSPortFundInfo*	_port_fund;

	//////////////////////////////////////////////////////////////////////////
	//持仓数据
	typedef struct _DetailInfo
	{
		bool		_long;
		double		_price;
		double		_volume;
		uint64_t	_opentime;
		uint32_t	_opentdate;
		double		_profit;

		_DetailInfo()
		{
			memset(this, 0, sizeof(_DetailInfo));
		}
	} DetailInfo;

	typedef struct _PosInfo
	{
		double		_volume;
		double		_closeprofit;
		double		_dynprofit;

		std::vector<DetailInfo> _details;

		_PosInfo()
		{
			_volume = 0;
			_closeprofit = 0;
			_dynprofit = 0;
		}
	} PosInfo;
	typedef faster_hashmap<std::string, PosInfo> PositionMap;
	PositionMap		_pos_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef faster_hashmap<std::string, double> PriceMap;
	PriceMap		_price_map;

	//后台任务线程, 把风控和资金, 持仓更新都放到这个线程里去
	typedef std::queue<TaskItem>	TaskQueue;
	StdThreadPtr	_thrd_task;
	TaskQueue		_task_queue;
	StdUniqueMutex	_mtx_task;
	StdCondVariable		_cond_task;
	bool			_terminated;

	typedef struct _RiskMonFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IRiskMonitorFact*	_fact;
		FuncCreateRiskMonFact	_creator;
		FuncDeleteRiskMonFact	_remover;
	} RiskMonFactInfo;
	RiskMonFactInfo	_risk_fact;
	WtRiskMonPtr	_risk_mon;
	double			_risk_volscale;
	uint32_t		_risk_date;

	TraderAdapterMgr*	_adapter_mgr;

	BoostFilePtr	_trade_logs;
	BoostFilePtr	_close_logs;
};
NS_OTP_END