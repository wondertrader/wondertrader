/*!
 * \file TraderHuaX.CPP
 * \project	WonderTrader
 *
 * \author HeJ
 * \date 2022/07/31
 *
 * \brief
 */
#include "TraderHuaX.h"


template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;
	const char* buffer = fmtutil::format(format, args...);
	sink->handleTraderLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderHuaX *instance = new TraderHuaX();
		return instance;
	}

	EXPORT_FLAG void deleteTrader(ITraderApi* &trader)
	{
		if (NULL != trader)
		{
			delete trader;
			trader = NULL;
		}
	}
}

const char* ENTRUST_SECTION = "entrusts";
const char* ORDER_SECTION = "orders";

inline bool IsErrorInfo(CTORATstpRspInfoField* pRspInfo)
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
		return true;

	return false;
}

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if(auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}


TraderHuaX::TraderHuaX()
	: _api(NULL)
	, _sink(NULL)
	, _ordref(makeRefID())
	, _reqid(1)
	, _accounts(NULL)
	, _orders(NULL)
	, _trades(NULL)
	, _positions(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _inited(false)
	,_terminal("PC")
	, _encrypt(false)
	, _flowdir("HuaXTDFlow")
{
}

TraderHuaX::~TraderHuaX()
{
}

#pragma region "Huax::API::TraderSpi"

WTSEntrust* TraderHuaX::makeEntrust(CTORATstpInputOrderField* entrust_info)
{
	std::string code, exchg;
	if (entrust_info->ExchangeID == TORA_TSTP_EXD_SSE)
		exchg = WT_MKT_SH_A;
	else if(entrust_info->ExchangeID == TORA_TSTP_EXD_SZSE)
		exchg = WT_MKT_SZ_A;
	else
	{
		write_log(_sink, LL_WARN, "[TraderHuaX] unsupport exchange {}", entrust_info->ExchangeID);
		return nullptr;
	}

	code = entrust_info->SecurityID;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)entrust_info->VolumeTotalOriginal,
		entrust_info->OrderPriceType,
		ct->getExchg());
	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType(entrust_info->Direction));
	pRet->setPriceType(wrapPriceType(entrust_info->OrderPriceType));
	if (pRet->getDirection() == WDT_LONG)
		pRet->setOffsetType(WOT_OPEN);
	else if (pRet->getDirection() == WDT_SHORT)
		pRet->setOffsetType(WOT_CLOSEYESTERDAY);

	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), entrust_info->OrderRef);

	const char* usertag = _eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderHuaX::makeOrderInfo(CTORATstpOrderField* order_info)
{
	std::string code, exchg;
	if (order_info->ExchangeID == TORA_TSTP_EXD_SSE)
		exchg = WT_MKT_SH_A;
	else
		exchg = WT_MKT_SZ_A;
	code = order_info->SecurityID;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(order_info->LimitPrice);
	pRet->setVolume((uint32_t)order_info->VolumeTotalOriginal);
	pRet->setDirection(wrapDirectionType(order_info->Direction));
	pRet->setPriceType(wrapPriceType(order_info->OrderPriceType));
	pRet->setOrderFlag(WOF_NOR);
	if (pRet->getDirection() == WDT_LONG)
		pRet->setOffsetType(WOT_OPEN);
	else if (pRet->getDirection() == WDT_SHORT)
		pRet->setOffsetType(WOT_CLOSEYESTERDAY);

	pRet->setVolTraded((uint32_t)order_info->VolumeTraded);
	pRet->setVolLeft((uint32_t)order_info->VolumeTotalOriginal - order_info->VolumeTraded);

	pRet->setCode(code.c_str());
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate((uint32_t)std::stoi(order_info->InsertDate));
	auto time_vector = StrUtil::split(order_info->InsertTime,":");
	uint32_t uTime{ 0 };
	for (std::string time_str : time_vector)
	{
		uTime = uTime * 100;
		uTime += std::stoi(time_str);
	}
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime*1000));

	pRet->setOrderState(wrapOrderState(order_info->OrderStatus));
	if (order_info->OrderStatus == TORA_TSTP_OST_Rejected)
		pRet->setError(true);

	genEntrustID(pRet->getEntrustID(), order_info->OrderRef);
	pRet->setOrderID(order_info->OrderSysID);

	pRet->setStateMsg("");

	const char* usertag = _eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) == 0)
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag);

		if (strlen(pRet->getOrderID()) > 0)
		{
			_oidCache.put(StrUtil::trim(order_info->OrderSysID).c_str(), usertag, 0, [this](const char* message) {
				write_log(_sink, LL_ERROR, message);
			});
		}
	}
	return pRet;
}

WTSTradeInfo* TraderHuaX::makeTradeInfo(CTORATstpTradeField* trade_info)
{
	std::string code, exchg;
	if (trade_info->ExchangeID == TORA_TSTP_EXD_SSE)
		exchg = WT_MKT_SH_A;
	else if (trade_info->ExchangeID == TORA_TSTP_EXD_SZSE)
		exchg = WT_MKT_SZ_A;
	code = trade_info->SecurityID;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)trade_info->Volume);
	pRet->setPrice(trade_info->Price);
	pRet->setTradeID(trade_info->TradeID);
	pRet->setContractInfo(contract);

	pRet->setTradeDate((uint32_t)std::stoi(trade_info->TradeDate));
	auto time_vector = StrUtil::split(trade_info->TradeTime, ":");
	uint32_t uTime{ 0 };
	for (std::string time_str : time_vector)
	{
		uTime = uTime * 100;
		uTime += std::stoi(time_str);
	}
	pRet->setTradeTime(TimeUtils::makeTime(pRet->getTradeDate(), uTime * 1000));

	pRet->setDirection(wrapDirectionType(trade_info->Direction));
	if (pRet->getDirection() == WDT_LONG)
		pRet->setOffsetType(WOT_OPEN);
	else if (pRet->getDirection() == WDT_SHORT)
		pRet->setOffsetType(WOT_CLOSEYESTERDAY);

	genEntrustID(pRet->getRefOrder(), trade_info->OrderRef);

	pRet->setTradeType(WTT_Common);

	double amount = pRet->getVolume() * pRet->getPrice();
	pRet->setAmount(amount);

	const char* usertag = _oidCache.get(StrUtil::trim(trade_info->OrderSysID).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

inline WTSPositionItem* TraderHuaX::makePositionItem(CTORATstpPositionField* position_info)
{
	std::string code, exchg;
	if (position_info->ExchangeID == TORA_TSTP_EXD_SSE)
		exchg = WT_MKT_SH_A;
	else if (position_info->ExchangeID == TORA_TSTP_EXD_SZSE)
		exchg = WT_MKT_SZ_A;
	code = position_info->SecurityID;
	WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (contract)
	{
		WTSCommodityInfo* commInfo = contract->getCommInfo();
		std::string key = code;
		WTSPositionItem* pos = WTSPositionItem::create(code.c_str(), commInfo->getCurrency(), commInfo->getExchg());
		pos->setContractInfo(contract);
		pos->setDirection(WDT_LONG);

		pos->setNewPosition((double)position_info->TodayBSPos);
		pos->setPrePosition((double)position_info->HistoryPos);
		pos->setMargin(position_info->TotalPosCost);
		pos->setDynProfit(0);
		pos->setPositionCost(position_info->TotalPosCost);

		pos->setAvgPrice(position_info->HistoryPosPrice);

		pos->setAvailNewPos(0);
		pos->setAvailPrePos((double)position_info->AvailablePosition);
		return pos;
	}
	return nullptr;
}

inline WTSAccountInfo* TraderHuaX::makeAccountInfo(CTORATstpTradingAccountField* accountField)
{
	WTSAccountInfo* accountInfo = WTSAccountInfo::create();
	accountInfo->setPreBalance(accountField->PreDeposit);
	accountInfo->setCloseProfit(0);
	accountInfo->setDynProfit(0);
	accountInfo->setMargin(0);
	accountInfo->setAvailable(accountField->UsefulMoney);
	accountInfo->setCommission(accountField->Commission);
	accountInfo->setFrozenMargin(accountField->FrozenCash);
	accountInfo->setFrozenCommission(accountField->FrozenCommission);
	accountInfo->setDeposit(accountField->Deposit);
	accountInfo->setDeposit(accountField->Withdraw);
	accountInfo->setBalance(accountField->PreDeposit);
	switch (accountField->CurrencyID)
	{
	case TORA_TSTP_CID_CNY:
		accountInfo->setCurrency("CNY");
		break;
	case TORA_TSTP_CID_HKD:
		accountInfo->setCurrency("HKD");
		break;
	case TORA_TSTP_CID_USD:
		accountInfo->setCurrency("USD");
		break;
	default:
		break;
	}
	return accountInfo;
}

void TraderHuaX::OnFrontDisconnected(int nReason)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, nReason);

	_state = TS_NOTLOGIN;
	//_asyncio.post([this](){
	//	write_log(_sink, LL_WARN, "[TraderHuaX] Connection lost, relogin in 2 seconds...");
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//	reconnect();
	//});
}

void TraderHuaX::OnRspError(CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (_sink && pRspInfoField)
		write_log(_sink,LL_ERROR, "[TraderHuaX] ErrorID:{} ErrorMsg:{}", pRspInfoField->ErrorID, pRspInfoField->ErrorMsg);
}

void TraderHuaX::OnRspOrderInsert(CTORATstpInputOrderField* pInputOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID)
{
	WTSError* error = nullptr;
	WTSEntrust* entrust = nullptr;
	if (IsErrorInfo(pRspInfoField))
	{
		error = WTSError::create(WEC_ORDERINSERT, pRspInfoField->ErrorMsg);
	}
	else
	{
		error = WTSError::create(WEC_NONE, pRspInfoField->ErrorMsg);
	}

	entrust = makeEntrust(pInputOrderField);
	if (error && entrust)
	{
		if (_sink)
			_sink->onRspEntrust(entrust, error);
		error->release();
		entrust->release();
	}
}

void TraderHuaX::OnRtnOrder(CTORATstpOrderField* pOrderField)
{
	WTSOrderInfo* orderInfo = makeOrderInfo(pOrderField);
	if (orderInfo)
	{
		if (_sink)
			_sink->onPushOrder(orderInfo);
		orderInfo->release();
	}
}

void TraderHuaX::OnErrRtnOrderInsert(CTORATstpInputOrderField* pInputOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID)
{
	WTSEntrust* entrust = makeEntrust(pInputOrderField);
	WTSError* error = WTSError::create(WEC_ORDERINSERT, pRspInfoField->ErrorMsg);

	if (error && entrust)
	{
		if (_sink)
			_sink->onRspEntrust(entrust, error);
		error->release();
		entrust->release();
	}
}

void TraderHuaX::OnRtnTrade(CTORATstpTradeField* pTradeField)
{
	WTSTradeInfo* trdInfo = makeTradeInfo(pTradeField);
	if (trdInfo)
	{
		if (_sink)
			_sink->onPushTrade(trdInfo);

		trdInfo->release();
	}
}

void TraderHuaX::OnErrRtnOrderAction(CTORATstpInputOrderActionField* pInputOrderActionField, CTORATstpRspInfoField* pRspInfoField, int nRequestID)
{
	if (IsErrorInfo(pRspInfoField))
	{
		WTSError* error = WTSError::create(WEC_ORDERCANCEL, pRspInfoField->ErrorMsg);
		_sink->onTraderError(error);
		error->release();
	}
}

void TraderHuaX::OnRspQryOrder(CTORATstpOrderField* pOrderField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (!IsErrorInfo(pRspInfoField) && pOrderField)
	{
		if (NULL == _orders)
			_orders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(pOrderField);
		if (orderInfo)
		{
			_orders->append(orderInfo, false);
		}
	}

	if (bIsLast)
	{
		if (_sink)
			_sink->onRspOrders(_orders);

		if (_orders)
			_orders->clear();
	}
}

void TraderHuaX::OnRspQryTrade(CTORATstpTradeField* pTradeField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (!IsErrorInfo(pRspInfoField) && pTradeField)
	{
		if (NULL == _trades)
			_trades = WTSArray::create();

		WTSTradeInfo* trdInfo = makeTradeInfo(pTradeField);
		if (trdInfo)
		{
			_trades->append(trdInfo, false);
		}
	}

	if (bIsLast)
	{
		if (_sink)
			_sink->onRspTrades(_trades);

		if (_trades)
			_trades->clear();
	}
}

void TraderHuaX::OnRspQryPosition(CTORATstpPositionField* pPositionField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (!IsErrorInfo(pRspInfoField) && pPositionField)
	{
		if (NULL == _positions)
			_positions = WTSArray::create();

		// 华鑫里面持仓没有买卖方向，用证券代码做key
		std::string key = pPositionField->SecurityID;
		WTSPositionItem* pos = makePositionItem(pPositionField);
		if (pos)
		{
			_positions->append(pos, false);
		}
	}

	if (bIsLast)
	{
		if (_sink)
			_sink->onRspPosition(_positions);

		if (_positions)
		{
			_positions->clear();
		}
	}
}

void TraderHuaX::OnRspQryTradingAccount(CTORATstpTradingAccountField* pTradingAccountField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (!IsErrorInfo(pRspInfoField) && pTradingAccountField)
	{
		if (NULL == _accounts)
			_accounts = WTSArray::create();
		WTSAccountInfo* accountInfo = makeAccountInfo(pTradingAccountField);

		if (accountInfo)
		{
			_accounts->append(accountInfo, false);
		}
	}
	
	if (bIsLast)
	{
		if (_sink)
			_sink->onRspAccount(_accounts);

		if (_accounts)
			_accounts->clear();
	}
}

void TraderHuaX::OnRspUserLogin(CTORATstpRspUserLoginField* pRspUserLoginField, CTORATstpRspInfoField* pRspInfoField, int nRequestID)
{
	if (pRspInfoField->ErrorID != 0)
	{
		write_log(_sink, LL_INFO, "[TraderHuaX] [{}] Login faild, erroMsg: {}...", _user.c_str(), pRspInfoField->ErrorMsg);
		if (_sink)
			_sink->handleEvent(WTE_Logout, -1);
		StdThreadPtr thrd(new StdThread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink, LL_WARN, "[TraderHuaX] {} relogin...", _user.c_str());
			reconnect();
		}));
		return;
	}
	else
	{
		_state = TS_LOGINED;
		CTORATstpQryShareholderAccountField field;
		memset(&field, 0, sizeof(CTORATstpQryShareholderAccountField));
		strcpy(field.InvestorID, _user.c_str());
		int ret = _api->ReqQryShareholderAccount(&field, genRequestID());
		if (ret != 0)
		{
			write_log(_sink, LL_ERROR, "[TraderHuaX] ReqGetConnectionInfo fail, ret{}", ret);
		}
	}
}

void TraderHuaX::OnRspGetConnectionInfo(CTORATstpConnectionInfoField* pConnectionInfoField, CTORATstpRspInfoField* pRspInfo, int nRequestID)
{	
	if (pRspInfo->ErrorID == 0)
	{
		write_log(_sink, LL_INFO, "inner_ip_address{}\n"
			"inner_port{}\n"
			"outer_ip_address{}\n"
			"outer_port{}\n"
			"mac_address{}\n",
			pConnectionInfoField->InnerIPAddress,
			pConnectionInfoField->InnerPort,
			pConnectionInfoField->OuterIPAddress,
			pConnectionInfoField->OuterPort,
			pConnectionInfoField->MacAddress);
		if (_sink)
			_sink->handleEvent(WTE_Connect, 0);
	}
	else
	{
		write_log(_sink, LL_ERROR, "get connection info fail! error_id[{}] error_msg[{}]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
}

void TraderHuaX::OnRspQryShareholderAccount(CTORATstpShareholderAccountField* pShareholderAccountField, CTORATstpRspInfoField* pRspInfoField, int nRequestID, bool bIsLast)
{
	if (!IsErrorInfo(pRspInfoField) && pShareholderAccountField)
	{
		if (pShareholderAccountField->MarketID == TORA_TSTP_MKD_SHA)
			_sse_share_hold_id = pShareholderAccountField->ShareholderID;
		else if (pShareholderAccountField->MarketID == TORA_TSTP_MKD_SZA)
			_szse_share_hold_id = pShareholderAccountField->ShareholderID;
	}
	if (bIsLast)
	{
		if (!_inited)
		{
			_tradingday = TimeUtils::getCurDate();
			{
				//初始化委托单缓存器
				std::stringstream ss;
				ss << "./huaxdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_eid.sc";
				_eidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			{
				//初始化订单标记缓存器
				std::stringstream ss;
				ss << "./huaxdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderHuaX] [{}] Login succeed, trading date: {}...", _user.c_str(), _tradingday);

			_inited = true;
			_asyncio.post([this] {
				_sink->onLoginResult(true, 0, _tradingday);
				_state = TS_ALLREADY;
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderHuaX] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}
	}
}

void TraderHuaX::OnFrontConnected()
{
	write_log(_sink, LL_INFO, "[TraderHuaX] Connection success");
	// 获取终端信息
	int ret = _api->ReqGetConnectionInfo(genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqGetConnectionInfo fail, ret{}", ret);
	}
}

#pragma endregion "Huax::API:TraderSpi"

#pragma region "ITraderApi"
bool TraderHuaX::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_appid = params->getCString("appid");

	_front = params->getCString("front");

	_quick = params->getBoolean("quick");	

	if (params->has("flowdir"))
		_flowdir = params->getBoolean("flowdir");

	_encrypt = params->getBoolean("encrypt");

	//_sse_share_hold_id = params->getString("sse_id");
	//_szse_share_hold_id = params->getString("szse_id");

	if (params->has("terminal"))
		_terminal = params->getString("terminal");
	_pub_ip = params->getString("pub_ip");
	_pub_port = params->getString("pub_port");
	_trade_ip = params->getString("trade_ip");
	_mac = params->getString("mac");
	_hard_disk = params->getString("hard_disk");

	if (_flowdir.empty())
		_flowdir = "HXTDFlow";

	_flowdir = StrUtil::standardisePath(_flowdir);
	
	std::string module = params->getString("huaxmodule");
	if (module.empty())
		module = "fasttraderapi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");

	_hInstHuaX = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateTstpTraderApi@CTORATstpTraderApi@TORASTOCKAPI@@SAPEAV12@PEBD_N@Z";
#	else
	const char* creatorName = "?CreateTstpTraderApi@CTORATstpTraderApi@TORASTOCKAPI@@SAPAV12@PBD_N@Z";
#	endif
#else
	const char* creatorName = "_ZN12TORASTOCKAPI18CTORATstpTraderApi19CreateTstpTraderApiEPKcb";
#endif
	_funcCreator = (HuaXCreator)DLLHelper::get_symbol(_hInstHuaX, creatorName);

	return true;
}

void TraderHuaX::release()
{
	if (_api)
	{
		_api->RegisterSpi(NULL);
		_api->Release();
		_api = NULL;
	}

	if (_orders)
		_orders->clear();

	if (_positions)
		_positions->clear();

	if (_trades)
		_trades->clear();
}

void TraderHuaX::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderHuaX::reconnect()
{
	if (_api)
	{
		_api->RegisterSpi(NULL);
		_api->Release();
		_api = NULL;
	}

	std::stringstream ss;
	ss << _flowdir << "flows/" << _user << "/";
	boost::filesystem::create_directories(ss.str().c_str());
	_api = _funcCreator(ss.str().c_str(), _encrypt);			// 创建UserApi
	if (_api == NULL)
	{
		if (_sink)
			_sink->handleEvent(WTE_Connect, -1);
		write_log(_sink,LL_ERROR, "[TraderHuaX] Module initializing failed");

		StdThreadPtr thrd(new StdThread([this](){
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink,LL_WARN, "[TraderHuaX] {} reconnecting...", _user.c_str());
			reconnect();
		}));
		return;
	}

	_api->RegisterSpi(this);						// 注册事件
	_api->RegisterFront((char*)_front.c_str());						// 注册事件
	// 订阅公有流和私有流
	_api->SubscribePublicTopic(_quick ? TORA_TERT_QUICK : TORA_TERT_RESUME);
	_api->SubscribePrivateTopic(_quick ? TORA_TERT_QUICK : TORA_TERT_RESUME);

	// 启动
	_api->Init();
	//_api->Join();
}

void TraderHuaX::connect()
{
	reconnect();

	if (_thrd_worker == NULL)
	{
		boost::asio::io_service::work work(_asyncio);
		_thrd_worker.reset(new StdThread([this](){
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_asyncio.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderHuaX::disconnect()
{
	release();
}

bool TraderHuaX::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderHuaX::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderHuaX::extractEntrustID(const char* entrustid, int &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderHuaX::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		int orderref = _ordref.fetch_add(1);
		fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderHuaX::doLogin()
{
	_state = TS_LOGINING;

	CTORATstpReqUserLoginField field;
	memset(&field, 0, sizeof(CTORATstpReqUserLoginField));
	//以用户代码方式登录
	strcpy_s(field.LogInAccount, _user.c_str());
	field.LogInAccountType = TORA_TSTP_LACT_UserID;
	strcpy_s(field.Password, _pass.c_str());
	// 终端采集  信息
	strcpy_s(field.UserProductInfo, _productInfo.c_str());
	// 按照监管要求填写终端信息
	const char* terminalInfo = fmtutil::format("{};IIP={};IPORT={};LIP={};MAC={};HD={}", _terminal, _pub_ip, _pub_port, _trade_ip, _mac, _hard_disk);
	strcpy_s(field.TerminalInfo, terminalInfo);

	int ret = _api->ReqUserLogin(&field, genRequestID());
	if (ret != 0)
	{
		std::string erro_code = std::to_string(ret);
		write_log(_sink, LL_ERROR, "[TraderHuaX] Login failed: error code {}", erro_code);
		
		_state = TS_LOGINFAILED;
		_asyncio.post([this, erro_code]{
			_sink->onLoginResult(false, erro_code.c_str(), 0);
		});
	}
}

int TraderHuaX::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;
	_productInfo = productInfo;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin();
	return 0;
}

int TraderHuaX::logout()
{
	if (_api == NULL)
		return -1;
	CTORATstpUserLogoutField field;
	strcpy_s(field.UserID, _user.c_str());
	int ret = _api->ReqUserLogout(&field, genRequestID());
	return 0;
}

int TraderHuaX::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	// 请求报单
	CTORATstpInputOrderField field;
	memset(&field, 0, sizeof(CTORATstpInputOrderField));
	int orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);
	strcpy(field.SecurityID, entrust->getCode());
	if (std::strcmp(entrust->getExchg(), WT_MKT_SH_A)==0)
	{
		strcpy(field.ShareholderID, _sse_share_hold_id.c_str());
		field.ExchangeID = TORA_TSTP_EXD_SSE;
	}
	else
	{
		strcpy(field.ShareholderID, _szse_share_hold_id.c_str());
		field.ExchangeID = TORA_TSTP_EXD_SZSE;
	}

	field.Direction = wrapDirectionType(entrust->getDirection());
	field.VolumeTotalOriginal = (int64_t)entrust->getVolume();
	// 上交所支持限价指令和最优五档剩撤、最优五档剩转限两种市价指令，对于科创板额外支持本方最优和对手方最优两种市价指令和盘后固定价格申报指令
	// 深交所支持限价指令和立即成交剩余撤销、全额成交或撤销、本方最优、对手方最优和最优五档剩撤五种市价指令
	// 限价指令和上交所科创板盘后固定价格申报指令需填写报单价格，其它市价指令无需填写报单价格
	// 以下以上交所限价指令为例，其它指令参考开发指南相关说明填写OrderPriceType、TimeCondition和VolumeCondition三个字段:
	field.LimitPrice = entrust->getPrice();
	field.OrderPriceType = TORA_TSTP_OPT_LimitPrice;
	field.TimeCondition = TORA_TSTP_TC_GFD;
	field.VolumeCondition = TORA_TSTP_VC_AV;
	// OrderRef为报单引用，类型为整型，该字段报单时为选填
	// 若不填写，则系统会为每笔报单自动分配一个报单引用
	// 若填写，则需保证同一个TCP会话下报单引用严格单调递增，不要求连续递增，至少需从1开始编号
	field.OrderRef = orderref;
	//InvestorID为选填，若填写则需保证填写正确
	//Operway为委托方式，根据券商要求填写，无特殊说明置空即可

	// 终端自定义字段，终端可根据需要填写如下字段的值，该字段值不会被柜台系统修改，在报单回报和查询报单时返回给终端
	//strcpy(field.SInfo, entrust->getUserTag());
	//field.IInfo = orderref;

	//其它字段置空
	if (strlen(entrust->getUserTag()) > 0)
	{
		_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}
	int ret = _api->ReqOrderInsert(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqOrderInsert fail, ret{}", ret);
	}
	return 0;
}

int TraderHuaX::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	CTORATstpInputOrderActionField field;
	memset(&field, 0, sizeof(CTORATstpInputOrderActionField));
	if (std::strcmp(action->getExchg(), WT_MKT_SH_A)==0)
	{
		field.ExchangeID = TORA_TSTP_EXD_SSE;
	}
	else
	{
		field.ExchangeID = TORA_TSTP_EXD_SZSE;
	}

	field.ActionFlag = TORA_TSTP_AF_Delete;
	// 撤单支持以下两种方式定位原始报单：
	// （1）报单引用方式
	//field.OrderRef = 1;
	//field.FrontID = m_front_id;
	//field.SessionID = m_session_id;
	// （2）系统报单编号方式
	strcpy(field.OrderSysID, action->getOrderID());
	// OrderActionRef报单操作引用，用法同报单引用，可根据需要选填

	// 终端自定义字段，终端可根据需要填写如下字段的值，该字段值不会被柜台系统修改，在查询撤单时返回给终端
	//strcpy(field.SInfo, "sinfo");

	// 委托方式字段根据券商要求填写，无特殊说明置空即可
	//Operway

	int ret = _api->ReqOrderAction(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqOrderAction fail, ret[{}]", ret);
	}
	return 0;
}

uint32_t TraderHuaX::genRequestID()
{
	return _reqid.fetch_add(1);
}

int TraderHuaX::queryAccount()
{
	// 查询资金账户
	CTORATstpQryTradingAccountField field;
	memset(&field, 0, sizeof(field));
	int ret = _api->ReqQryTradingAccount(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqQryTradingAccount fail, ret[{}]\n", ret);
	}
	return 0;
}

int TraderHuaX::queryPositions()
{
	// 查询持仓
	CTORATstpQryPositionField field;
	memset(&field, 0, sizeof(CTORATstpQryPositionField));

	// 以下字段不填表示不设过滤条件，即查询所有持仓
	//strcpy(field.SecurityID, "600000");

	int ret = _api->ReqQryPosition(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqQryPosition fail, ret[{}]\n", ret);
	}
	return 0;
}

int TraderHuaX::queryOrders()
{
	// 查询报单
	CTORATstpQryOrderField field;
	memset(&field, 0, sizeof(field));

	// 以下字段不填表示不设过滤条件，即查询所有报单
	//strcpy(field.SecurityID, "600000");
	//strcpy(field.InsertTimeStart, "09:35:00");
	//strcpy(field.InsertTimeEnd, "10:30:00");

	// IsCancel字段填1表示只查询可撤报单
	//field.IsCancel = 1;

	int ret = _api->ReqQryOrder(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqQryOrder fail, ret[{}]\n", ret);
	}
	return 0;
}

int TraderHuaX::queryTrades()
{
	// 查询成交
	CTORATstpQryTradeField field;
	memset(&field, 0, sizeof(field));

	int ret = _api->ReqQryTrade(&field, genRequestID());
	if (ret != 0)
	{
		write_log(_sink, LL_ERROR, "[TraderHuaX] ReqQryTrade fail, ret[{}]\n", ret);
	}
	return 0;
}
#pragma endregion "ITraderApi"

#pragma region "Huax::API::warp"
inline WTSDirectionType TraderHuaX::wrapDirectionType(TTORATstpDirectionType dirType)
{
	switch (dirType)
	{
	case TORA_TSTP_D_Buy:
		return WDT_LONG;
	case TORA_TSTP_D_Sell:
		return WDT_SHORT;
	case TORA_TSTP_D_ETFPur:
		return WDT_LONG;
	case TORA_TSTP_D_ETFRed:
		return WDT_SHORT;
	case TORA_TSTP_D_Repurchase:
		return WDT_LONG;
	case TORA_TSTP_D_ReverseRepur:
		return WDT_SHORT;
	case TORA_TSTP_D_OeFundPur:
		return WDT_LONG;
	case TORA_TSTP_D_OeFundRed:
		return WDT_SHORT;
	case TORA_TSTP_D_CreditBuy:
		return WDT_LONG;
	case TORA_TSTP_D_CreditSell:
		return WDT_SHORT;
	default:
		write_log(_sink, LL_WARN, "[TraderHuaX] unsupport DirectionType {}", dirType);
		return WDT_NET;
	}
}

inline WTSPriceType TraderHuaX::wrapPriceType(TTORATstpDirectionType priceType)
{
	if (TORA_TSTP_OPT_AnyPrice == priceType)
		return WPT_ANYPRICE;
	else if(TORA_TSTP_OPT_LimitPrice == priceType)
		return WPT_LIMITPRICE;
	else if (TORA_TSTP_OPT_BestPrice == priceType)
		return WPT_BESTPRICE;
	else
		write_log(_sink, LL_WARN, "[TraderHuaX] unsupport priceType {}", priceType);
}

inline WTSOrderState TraderHuaX::wrapOrderState(TTORATstpOrderStatusType orderState)
{
	switch (orderState)
	{
	case TORA_TSTP_OST_Unknown:
		return WOS_NotTraded_NotQueuing;
	case TORA_TSTP_OST_AllTraded:
		return WOS_AllTraded;
	case TORA_TSTP_OST_PartTraded:
		return WOS_PartTraded_Queuing;
	case TORA_TSTP_OST_Accepted:
		return WOS_NotTraded_Queuing;
	case TORA_TSTP_OST_PartTradeCanceled:
	case TORA_TSTP_OST_AllCanceled:
	case TORA_TSTP_OST_Rejected:
		return WOS_Canceled;
	default:
		return WOS_Nottouched;
	}
}
#pragma endregion "Huax::API::warp"