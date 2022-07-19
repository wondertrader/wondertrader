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
		AS_NOTLOGIN,		//δ��¼
		AS_LOGINING,		//���ڵ�¼
		AS_LOGINED,			//�ѵ�¼
		AS_LOGINFAILED,		//��¼ʧ��
		AS_POSITION_QRYED,	//��λ�Ѳ�
		AS_ORDERS_QRYED,	//�����Ѳ�
		AS_TRADES_QRYED,	//�ɽ��Ѳ�
		AS_ALLREADY			//ȫ������
	} AdapterState;

	typedef struct _PosItem
	{
		//�������
		double	l_newvol;
		double	l_newavail;
		double	l_prevol;
		double	l_preavail;

		//�ղ�����
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

	inline void updateUndone(const char* stdCode, double qty, bool bOuput = true);

	const RiskParams* getRiskParams(const char* stdCode);

public:
	double	getPosition(const char* stdCode, bool bValidOnly, int32_t flag = 3);
	double	enumPosition(const char* stdCode = "");
	OrderMap* getOrders(const char* stdCode);
	double getUndoneQty(const char* stdCode)
	{
		auto it = _undone_qty.find(stdCode);
		if (it != _undone_qty.end())
			return it->second;

		return 0;
	}

	OrderIDs buy(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo = NULL);
	OrderIDs sell(const char* stdCode, double price, double qty, int flag, bool bForceClose, WTSContractInfo* cInfo = NULL);

	/*
	 *	�µ��ӿ�: ����
	 *	
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok
	 */
	uint32_t openLong(const char* stdCode, double price, double qty, int flag);

	/*
	 *	�µ��ӿ�: ����
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok
	 */
	uint32_t openShort(const char* stdCode, double price, double qty, int flag);

	/*
	 *	�µ��ӿ�: ƽ��
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@isToday	�Ƿ��֣�Ĭ��false
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 */
	uint32_t closeLong(const char* stdCode, double price, double qty, bool isToday, int flag);

	/*
	 *	�µ��ӿ�: ƽ��
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@isToday	�Ƿ��֣�Ĭ��false
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 */
	uint32_t closeShort(const char* stdCode, double price, double qty, bool isToday, int flag);
	
	bool	cancel(uint32_t localid);
	OrderIDs cancelAll(const char* stdCode);

	inline bool	isTradeEnabled(const char* stdCode) const;

	bool	checkCancelLimits(const char* stdCode);
	bool	checkOrderLimits(const char* stdCode);

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderSpi�ӿ�
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

	virtual void handleTraderLog(WTSLogLevel ll, const char* message) override;

private:
	WTSVariant*			_cfg;
	std::string			_id;
	std::string			_order_pattern;

	uint32_t			_trading_day;

	ITraderApi*			_trader_api;
	FuncDeleteTrader	_remover;
	AdapterState		_state;

	faster_hashset<ITrdNotifySink*>	_sinks;

	IBaseDataMgr*		_bd_mgr;
	ActionPolicyMgr*	_policy_mgr;

	faster_hashmap<std::string, PosItem> _positions;

	StdUniqueMutex _mtx_orders;
	OrderMap*		_orders;
	faster_hashset<std::string> _orderids;	//��Ҫ���ڱ����û�д�����ö���

	faster_hashmap<LongKey, double> _undone_qty;	//δ�������

	typedef WTSHashMap<LongKey>	TradeStatMap;
	TradeStatMap*	_stat_map;	//ͳ������

	//����������ʱ���ڵ�����,��Ҫ��Ϊ�˿���˲�����������õ�
	typedef std::vector<uint64_t> TimeCacheList;
	typedef faster_hashmap<LongKey, TimeCacheList> CodeTimeCacheMap;
	CodeTimeCacheMap	_order_time_cache;	//�µ�ʱ�仺��
	CodeTimeCacheMap	_cancel_time_cache;	//����ʱ�仺��

	//����������,�ͻ���뵽�ų�����
	faster_hashset<LongKey>	_exclude_codes;

	typedef faster_hashmap<LongKey, RiskParams>	RiskParamsMap;
	RiskParamsMap	_risk_params_map;
	bool			_risk_mon_enabled;
};

typedef std::shared_ptr<TraderAdapter>				TraderAdapterPtr;
typedef faster_hashmap<std::string, TraderAdapterPtr>	TraderAdapterMap;


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
