/*!
 * \file TraderAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/FasterDefs.h"
#include "../Includes/ITraderApi.h"
#include "../Share/BoostFile.hpp"
#include "../Share/StdUtils.hpp"
#include "../Includes/WTSCollection.hpp"
#include "../Share/SpinMutex.hpp"

NS_WTP_BEGIN
class WTSVariant;
class WTSContractInfo;
class WTSCommodityInfo;
class ITrdNotifySink;
class ActionPolicyMgr;

typedef std::vector<uint32_t> OrderIDs;
typedef WTSMap<uint32_t> OrderMap;

class TraderAdapter : public ITraderSpi
{
public:
	TraderAdapter();
	~TraderAdapter();

	typedef enum tagAdapterState
	{
		AS_NOTLOGIN,		//未登录
		AS_LOGINING,		//正在登录
		AS_LOGINED,			//已登录
		AS_LOGINFAILED,		//登录失败
		AS_POSITION_QRYED,	//仓位已查
		AS_ORDERS_QRYED,	//订单已查
		AS_TRADES_QRYED,	//成交已查
		AS_ALLREADY			//全部就绪
	} AdapterState;

	typedef struct _PosItem
	{
		//多仓数据
		double	l_newvol;
		double	l_newavail;
		double	l_prevol;
		double	l_preavail;

		//空仓数据
		double	s_newvol;
		double	s_newavail;
		double	s_prevol;
		double	s_preavail;

		_PosItem()
		{
			memset(this, 0, sizeof(_PosItem));
		}

		double total_pos(bool isLong = true) const
		{
			if (isLong)
				return l_newvol + l_prevol;
			else
				return s_newvol + s_prevol;
		}

		double avail_pos(bool isLong = true) const
		{
			if (isLong)
				return l_newavail + l_preavail;
			else
				return s_newavail + s_preavail;
		}

	} PosItem;

	typedef struct _RiskParams
	{
		uint32_t	_order_times_boundary;
		uint32_t	_order_stat_timespan;
		uint32_t	_order_total_limits;

		uint32_t	_cancel_times_boundary;
		uint32_t	_cancel_stat_timespan;
		uint32_t	_cancel_total_limits;

		_RiskParams()
		{
			memset(this, 0, sizeof(_RiskParams));
		}
	} RiskParams;

public:
	bool init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr);

	bool initExt(const char* id, ITraderApi* api, IBaseDataMgr* bdMgr, ActionPolicyMgr* policyMgr);

	void release();

	bool run();

	inline const char* id() const{ return _id.c_str(); }

	AdapterState state() const{ return _state; }

	void addSink(ITrdNotifySink* sink)
	{
		_sinks.insert(sink);
	}

private:
	uint32_t doEntrust(WTSEntrust* entrust);
	bool	doCancel(WTSOrderInfo* ordInfo);

	inline void	printPosition(const char* stdCode, const PosItem& pItem);

	inline WTSContractInfo* getContract(const char* stdCode);

	inline void updateUndone(const char* stdCode, double qty);

	const RiskParams* getRiskParams(const char* stdCode);

public:
	double	getPosition(const char* stdCode, bool bValidOnly, int32_t flag = 3);
	double	enumPosition(const char* stdCode = "");
	OrderMap* getOrders(const char* stdCode);
	inline double getUndoneQty(const char* stdCode)
	{
		auto it = _undone_qty.find(stdCode);
		if (it != _undone_qty.end())
			return it->second;

		return 0;
	}

	uint32_t getInfos(const char* stdCode);

	OrderIDs buy(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo = NULL);
	OrderIDs sell(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo = NULL);

	/*
	 *	下单接口: 开多
	 *	
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok
	 */
	uint32_t openLong(const char* stdCode, double price, double qty, int flag);

	/*
	 *	下单接口: 开空
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok
	 */
	uint32_t openShort(const char* stdCode, double price, double qty, int flag);

	/*
	 *	下单接口: 平多
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@isToday	是否今仓，默认false
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	uint32_t closeLong(const char* stdCode, double price, double qty, bool isToday, int flag);

	/*
	 *	下单接口: 平空
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@isToday	是否今仓，默认false
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	uint32_t closeShort(const char* stdCode, double price, double qty, bool isToday, int flag);
	
	bool	cancel(uint32_t localid);
	OrderIDs cancelAll(const char* stdCode);

	inline bool	isTradeEnabled(const char* stdCode) const;

	bool	checkCancelLimits(const char* stdCode);
	bool	checkOrderLimits(const char* stdCode);

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderSpi接口
	virtual void handleEvent(WTSTraderEvent e, int32_t ec) override;

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) override;

	virtual void onLogout() override;

	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err) override;

	virtual void onRspAccount(WTSArray* ayAccounts) override;

	virtual void onRspPosition(const WTSArray* ayPositions) override;

	virtual void onRspOrders(const WTSArray* ayOrders) override;

	virtual void onRspTrades(const WTSArray* ayTrades) override;

	virtual void onPushOrder(WTSOrderInfo* orderInfo) override;

	virtual void onPushTrade(WTSTradeInfo* tradeRecord) override;

	virtual void onTraderError(WTSError* err, void* pData = NULL) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

	virtual void handleTraderLog(WTSLogLevel ll, const char* message) override;

private:
	WTSVariant*			_cfg;
	std::string			_id;
	std::string			_order_pattern;

	uint32_t			_trading_day;

	ITraderApi*			_trader_api;
	FuncDeleteTrader	_remover;
	AdapterState		_state;

	wt_hashset<ITrdNotifySink*>	_sinks;

	IBaseDataMgr*		_bd_mgr;
	ActionPolicyMgr*	_policy_mgr;

	wt_hashmap<std::string, PosItem> _positions;

	SpinMutex	_mtx_orders;
	OrderMap*	_orders;
	wt_hashset<std::string> _orderids;	//主要用于标记有没有处理过该订单

	wt_hashmap<std::string, double> _undone_qty;	//未完成数量

	typedef WTSHashMap<std::string>	TradeStatMap;
	TradeStatMap*	_stat_map;	//统计数据

	//这两个缓存时间内的容器,主要是为了控制瞬间流量而设置的
	typedef std::vector<uint64_t> TimeCacheList;
	typedef wt_hashmap<std::string, TimeCacheList> CodeTimeCacheMap;
	CodeTimeCacheMap	_order_time_cache;	//下单时间缓存
	CodeTimeCacheMap	_cancel_time_cache;	//撤单时间缓存

	//如果被风控了,就会进入到排除队列
	wt_hashset<std::string>	_exclude_codes;

	typedef wt_hashmap<std::string, RiskParams>	RiskParamsMap;
	RiskParamsMap	_risk_params_map;
	bool			_risk_mon_enabled;
};

typedef std::shared_ptr<TraderAdapter>					TraderAdapterPtr;
typedef wt_hashmap<std::string, TraderAdapterPtr>	TraderAdapterMap;


//////////////////////////////////////////////////////////////////////////
//TraderAdapterMgr
class TraderAdapterMgr
{
public:
	void	release();

	void	run();

	const TraderAdapterMap& getAdapters() const { return _adapters; }

	TraderAdapterPtr getAdapter(const char* tname);

	bool	addAdapter(const char* tname, TraderAdapterPtr& adapter);

private:
	TraderAdapterMap	_adapters;
};

NS_WTP_END
