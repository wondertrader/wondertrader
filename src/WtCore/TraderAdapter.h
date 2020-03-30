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
#include <unordered_map>
#include <unordered_set>

#include "ExecuteDefs.h"

#include "../Share/ITraderApi.h"
#include "../Share/BoostDefine.h"

NS_OTP_BEGIN
class WTSVariant;
class ActionPolicyMgr;
class WTSContractInfo;
class WTSCommodityInfo;
class WtExecuter;

class ITrdNotifySink;

class TraderAdapter : public ITraderApiListener
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
		AS_ALLREADY			//全部就绪
	} AdapterState;

	typedef struct _PosItem
	{
		//多仓数据
		uint32_t	l_newvol;
		uint32_t	l_newavail;
		uint32_t	l_prevol;
		uint32_t	l_preavail;

		//空仓数据
		uint32_t	s_newvol;
		uint32_t	s_newavail;
		uint32_t	s_prevol;
		uint32_t	s_preavail;

		_PosItem()
		{
			memset(this, 0, sizeof(_PosItem));
		}

		uint32_t total_pos(bool isLong = true) const
		{
			if (isLong)
				return l_newvol + l_prevol;
			else
				return s_newvol + s_prevol;
		}

		uint32_t avail_pos(bool isLong = true) const
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

	void release();

	bool run();

	const char* id() const{ return _id.c_str(); }

	AdapterState state() const{ return _state; }

	void addSink(ITrdNotifySink* sink)
	{
		_sinks.insert(sink);
	}

private:
	uint32_t doEntrust(WTSEntrust* entrust);
	bool	doCancel(WTSOrderInfo* ordInfo);

	uint32_t openLong(const char* stdCode, double price, uint32_t qty);
	uint32_t openShort(const char* stdCode, double price, uint32_t qty);
	uint32_t closeLong(const char* stdCode, double price, uint32_t qty, bool isToday = false);
	uint32_t closeShort(const char* stdCode, double price, uint32_t qty, bool isToday = false);

	inline void	printPosition(const char* stdCode, const PosItem& pItem);

	WTSContractInfo* getContract(const char* stdCode);
	WTSCommodityInfo* getCommodify(const char* stdCommID);


	const RiskParams* getRiskParams(const char* stdCode);

public:
	int32_t getPosition(const char* stdCode, int32_t flag = 3);
	OrderMap* getOrders(const char* stdCode);
	int32_t getUndoneQty(const char* stdCode)
	{
		auto it = _undone_qty.find(stdCode);
		if (it != _undone_qty.end())
			return it->second;

		return 0;
	}
	
	OrderIDs buy(const char* stdCode, double price, uint32_t qty);
	OrderIDs sell(const char* stdCode, double price, uint32_t qty);
	bool	cancel(uint32_t localid);
	OrderIDs cancel(const char* stdCode, bool isBuy, uint32_t qty = 0);

	bool	isTradeEnabled(const char* stdCode) const;

	bool	checkCancelLimits(const char* stdCode);
	bool	checkOrderLimits(const char* stdCode);

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderApiListener接口
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

	virtual void onTraderError(WTSError* err) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

	virtual void handleTraderLog(WTSLogLevel ll, const char* format, ...) override;

private:
	WTSVariant*			_cfg;
	std::string			_id;
	std::string			_order_pattern;

	ITraderApi*			_trader_api;
	FuncDeleteTrader	_remover;
	AdapterState		_state;

	std::unordered_set<ITrdNotifySink*>	_sinks;

	IBaseDataMgr*		_bd_mgr;
	ActionPolicyMgr*	_policy_mgr;

	std::unordered_map<std::string, PosItem> _positions;

	BoostUniqueMutex _mtx_orders;
	OrderMap*		_orders;
	std::unordered_set<std::string> _orderids;	//主要用于标记有没有处理过该订单

	std::unordered_map<std::string, int32_t> _undone_qty;	//未完成手数

	typedef WTSHashMap<std::string>	TradeStatMap;
	TradeStatMap*	_stat_map;	//统计数据

	typedef std::vector<uint64_t> TimeCacheList;
	typedef std::unordered_map<std::string, TimeCacheList> CodeTimeCacheMap;
	CodeTimeCacheMap	_order_time_cache;	//下单时间缓存
	CodeTimeCacheMap	_cancel_time_cache;	//撤单时间缓存

	std::unordered_set<std::string>	_exclude_codes;
	
	typedef std::unordered_map<std::string, RiskParams>	RiskParamsMap;
	RiskParamsMap	_risk_params_map;
	bool			_risk_mon_enabled;
};

typedef std::shared_ptr<TraderAdapter>				TraderAdapterPtr;
typedef std::unordered_map<std::string, TraderAdapterPtr>	TraderAdapterMap;


//////////////////////////////////////////////////////////////////////////
//TraderAdapterMgr
class TraderAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();


	TraderAdapterPtr getAdapter(const char* tname);

	bool	addAdapter(const char* tname, TraderAdapterPtr& adapter);

private:
	TraderAdapterMap	_adapters;
};

NS_OTP_END
