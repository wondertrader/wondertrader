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
#include <atomic>
#include <unordered_map>
#include <boost/noncopyable.hpp>

#include "../Includes/ExecuteDefs.h"

#include "../Includes/ITraderApi.h"

NS_WTP_BEGIN
class WTSVariant;
class ActionPolicyMgr;
class WTSContractInfo;
class WTSCommodityInfo;
class WtExecuter;
class EventNotifier;

class ITrdNotifySink;

class TraderAdapterMgr;

class TraderAdapter : public ITraderSpi
{
public:
	TraderAdapter(TraderAdapterMgr* mgr);
	~TraderAdapter();

public:
	bool init(const char* id, WTSVariant* params, IBaseDataMgr* bdMgr);

	void release();

	bool run();

	inline const char* id() const{ return _id.c_str(); }

	bool isDone() const { return _done; }

	void queryFund();
	void queryPosition();

public:
	//////////////////////////////////////////////////////////////////////////
	//ITraderSpi½Ó¿Ú
	virtual void handleEvent(WTSTraderEvent e, int32_t ec) override;

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate) override;

	virtual void onLogout() override;

	virtual void onRspAccount(WTSArray* ayAccounts) override;

	virtual void onRspPosition(const WTSArray* ayPositions) override;

	virtual void onRspTrades(const WTSArray* ayTrades) override;

	virtual void onRspOrders(const WTSArray* ayOrders) override;

	virtual void onPushTrade(WTSTradeInfo* tradeRecord) override;

	virtual void onPushOrder(WTSOrderInfo* orderInfo) override;

	virtual void onTraderError(WTSError* err, void* pData = NULL) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

	virtual void handleTraderLog(WTSLogLevel ll, const char* message) override;

private:
	TraderAdapterMgr*	_mgr;
	WTSVariant*			_cfg;
	std::string			_id;

	ITraderApi*			_trader_api;
	FuncDeleteTrader	_remover;

	IBaseDataMgr*		_bd_mgr;
	uint32_t			_date;

	bool				_done;
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

	const TraderAdapterMap& getAdapters() const { return _adapters; }

	TraderAdapterPtr getAdapter(const char* tname);

	bool	addAdapter(const char* tname, TraderAdapterPtr& adapter);

	bool	isAnyAlive() const {
		return _live_cnt != 0;
	}

	std::size_t size() const { return _adapters.size(); }

	void decAlive();

	void refresh();

private:
	TraderAdapterMap		_adapters;
	std::mutex				_mutex;
	std::atomic<uint32_t>	_live_cnt;
};

NS_WTP_END
