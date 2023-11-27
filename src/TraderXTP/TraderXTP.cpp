/*!
 * \file TraderXTP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderXTP.h"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
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
		TraderXTP *instance = new TraderXTP();
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

inline bool IsErrorInfo(XTPRI *error_info)
{
	if (error_info && error_info->error_id != 0)
		return true;

	return false;
}

inline WTSDirectionType wrapPosDirection(XTP_POSITION_DIRECTION_TYPE dirType)
{
	switch (dirType)
	{
	case XTP_POSITION_DIRECTION_SHORT: return WDT_SHORT;
	default:
		return WDT_LONG;
	}
}

inline XTP_SIDE_TYPE wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return XTP_SIDE_BUY;
		else
			return XTP_SIDE_SELL;
	else
		if (offsetType == WOT_OPEN)
			return XTP_SIDE_SELL;
		else
			return XTP_SIDE_BUY;
}

inline WTSDirectionType wrapDirectionType(XTP_SIDE_TYPE side, XTP_POSITION_EFFECT_TYPE pe = 0)
{
	if (XTP_SIDE_BUY == side)
		if (pe == XTP_POSITION_EFFECT_OPEN)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if (pe == XTP_POSITION_EFFECT_OPEN)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

inline XTP_POSITION_EFFECT_TYPE wrapOffsetType(WTSOffsetType offType)
{
	if (WOT_OPEN == offType)
		return XTP_POSITION_EFFECT_OPEN;
	else if (WOT_CLOSE == offType)
		return XTP_POSITION_EFFECT_CLOSE;
	else if (WOT_CLOSETODAY == offType)
		return XTP_POSITION_EFFECT_CLOSETODAY;
	else if (WOT_CLOSEYESTERDAY == offType)
		return XTP_POSITION_EFFECT_CLOSE;
	else
		return XTP_POSITION_EFFECT_FORCECLOSE;
}

inline WTSOffsetType wrapOffsetType(XTP_SIDE_TYPE side, XTP_POSITION_EFFECT_TYPE offType = 0)
{
	if (XTP_POSITION_EFFECT_OPEN == offType)
		return WOT_OPEN;
	else if (XTP_POSITION_EFFECT_CLOSE == offType)
		return WOT_CLOSE;
	else if (XTP_POSITION_EFFECT_CLOSETODAY == offType)
		return WOT_CLOSETODAY;
	else if (XTP_POSITION_EFFECT_CLOSE == offType)
		return WOT_CLOSEYESTERDAY;
	else
		return WOT_FORCECLOSE;
}

inline WTSPriceType wrapPriceType(XTP_PRICE_TYPE priceType)
{
	if (XTP_PRICE_LIMIT == priceType)
		return WPT_LIMITPRICE;
	else 
		return WPT_ANYPRICE;
}

inline WTSOrderState wrapOrderState(XTP_ORDER_STATUS_TYPE orderState)
{
	switch (orderState)
	{
	case XTP_ORDER_STATUS_INIT:
		return WOS_NotTraded_NotQueuing;
	case XTP_ORDER_STATUS_ALLTRADED:
		return WOS_AllTraded;
	case XTP_ORDER_STATUS_PARTTRADEDQUEUEING:
		return WOS_PartTraded_Queuing;
	case XTP_ORDER_STATUS_NOTRADEQUEUEING:
		return WOS_NotTraded_Queuing;
	case XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING:
	case XTP_ORDER_STATUS_CANCELED:
		return WOS_Canceled;
	default:
		return WOS_Nottouched;
	}
}

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if(auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}


TraderXTP::TraderXTP()
	: _api(NULL)
	, _sink(NULL)
	, _ordref(makeRefID())
	, _reqid(1)
	, _orders(NULL)
	, _trades(NULL)
	, _positions(NULL)
	, _bd_mgr(NULL)
	, _tradingday(0)
	, _inited(false)
{
}


TraderXTP::~TraderXTP()
{
}

#pragma region "XTP::API::TraderSpi"

WTSEntrust* TraderXTP::makeEntrust(XTPOrderInfo* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = ct->getCommInfo();

	WTSEntrust* pRet = WTSEntrust::create(
		code.c_str(),
		(uint32_t)order_info->quantity,
		order_info->price,
		ct->getExchg());
	pRet->setContractInfo(ct);
	if (commInfo->isStock())
		pRet->setDirection(WDT_LONG);
	else
		pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	if (!commInfo->isOption())
		pRet->setOffsetType((order_info->side == XTP_SIDE_BUY)? WOT_OPEN : WOT_CLOSE);
	else
		pRet->setOffsetType(wrapOffsetType(order_info->side, order_info->position_effect));
	pRet->setOrderFlag(WOF_NOR);

	genEntrustID(pRet->getEntrustID(), order_info->order_client_id);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSOrderInfo* TraderXTP::makeOrderInfo(XTPQueryOrderRsp* order_info)
{
	std::string code, exchg;
	if (order_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = order_info->ticker;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = ct->getCommInfo();

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(ct);
	pRet->setPrice(order_info->price);
	pRet->setVolume((uint32_t)order_info->quantity);
	pRet->setPriceType(wrapPriceType(order_info->price_type));
	pRet->setOrderFlag(WOF_NOR);

	if (commInfo->isStock())
		pRet->setDirection(WDT_LONG);
	else
		pRet->setDirection(wrapDirectionType(order_info->side, order_info->position_effect));
	if (!commInfo->isOption())
		pRet->setOffsetType((order_info->side == XTP_SIDE_BUY) ? WOT_OPEN : WOT_CLOSE);
	else
		pRet->setOffsetType(wrapOffsetType(order_info->side, order_info->position_effect));

	pRet->setVolTraded((uint32_t)order_info->qty_traded);
	pRet->setVolLeft((uint32_t)order_info->qty_left);

	pRet->setCode(code.c_str());
	pRet->setExchange(ct->getExchg());

	pRet->setOrderDate((uint32_t)(order_info->insert_time / 1000000000));
	uint32_t uTime = order_info->insert_time % 1000000000;
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderState(order_info->order_status));
	if (order_info->order_status >= XTP_ORDER_STATUS_REJECTED)
		pRet->setError(true);

	genEntrustID(pRet->getEntrustID(), order_info->order_client_id);
	fmtutil::format_to(pRet->getOrderID(), "{}", order_info->order_xtp_id);

	pRet->setStateMsg("");

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) == 0)
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag);

		if (strlen(pRet->getOrderID()) > 0)
		{
			m_oidCache.put(StrUtil::trim(pRet->getOrderID()).c_str(), usertag, 0, [this](const char* message) {
				write_log(_sink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSTradeInfo* TraderXTP::makeTradeInfo(XTPQueryTradeRsp* trade_info)
{
	std::string code, exchg;
	if (trade_info->market == XTP_MKT_SH_A)
		exchg = "SSE";
	else
		exchg = "SZSE";
	code = trade_info->ticker;
	WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
	if (ct == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = ct->getCommInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	pRet->setVolume((uint32_t)trade_info->quantity);
	pRet->setPrice(trade_info->price);
	pRet->setTradeID(trade_info->exec_id);
	pRet->setContractInfo(ct);

	uint32_t uTime = (uint32_t)(trade_info->trade_time % 1000000000);
	uint32_t uDate = (uint32_t)(trade_info->trade_time / 1000000000);

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime));

	if (commInfo->isStock())
		pRet->setDirection(WDT_LONG);
	else
		pRet->setDirection(wrapDirectionType(trade_info->side, trade_info->position_effect));
	if (!commInfo->isOption())
		pRet->setOffsetType((trade_info->side == XTP_SIDE_BUY) ? WOT_OPEN : WOT_CLOSE);
	else
		pRet->setOffsetType(wrapOffsetType(trade_info->side, trade_info->position_effect));

	fmtutil::format_to(pRet->getRefOrder(), "{}", trade_info->order_xtp_id);
	pRet->setTradeType(WTT_Common);

	double amount = trade_info->quantity*pRet->getPrice();
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

void TraderXTP::OnDisconnected(uint64_t session_id, int reason)
{
	if (_sink)
		_sink->handleEvent(WTE_Close, reason);

	_asyncio.post([this](){
		write_log(_sink, LL_WARN, "[TraderrXTP] Connection lost, relogin in 2 seconds...");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		doLogin();
	});
}

void TraderXTP::OnError(XTPRI *error_info)
{
	if (_sink && error_info)
		write_log(_sink,LL_ERROR, error_info->error_msg);
}

void TraderXTP::OnOrderEvent(XTPOrderInfo *order_info, XTPRI *error_info, uint64_t session_id)
{
	if(IsErrorInfo(error_info))
	{
		WTSEntrust* entrust = makeEntrust(order_info);

		WTSError* error = WTSError::create(WEC_ORDERINSERT, error_info->error_msg);
		_sink->onRspEntrust(entrust, error);
		error->release();

		entrust->release();
	}
	else
	{
		WTSOrderInfo *orderInfo = makeOrderInfo(order_info);
		if (orderInfo)
		{
			if (_sink)
				_sink->onPushOrder(orderInfo);

			orderInfo->release();
		}
	}
}

void TraderXTP::OnTradeEvent(XTPTradeReport *trade_info, uint64_t session_id)
{
	WTSTradeInfo *trdInfo = makeTradeInfo(trade_info);
	if (trdInfo)
	{
		if (_sink)
			_sink->onPushTrade(trdInfo);

		trdInfo->release();
	}
}

void TraderXTP::OnCancelOrderError(XTPOrderCancelInfo *cancel_info, XTPRI *error_info, uint64_t session_id)
{
	if (IsErrorInfo(error_info))
	{
		WTSError* error = WTSError::create(WEC_ORDERCANCEL, error_info->error_msg);
		_sink->onTraderError(error);
		error->release();
	}

}

void TraderXTP::OnQueryOrder(XTPQueryOrderRsp *order_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (!IsErrorInfo(error_info) && order_info)
	{
		if (NULL == _orders)
			_orders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(order_info);
		if (orderInfo)
		{
			_orders->append(orderInfo, false);
		}
	}

	if (is_last)
	{
		if (_sink)
			_sink->onRspOrders(_orders);

		if (_orders)
			_orders->clear();
	}
}

void TraderXTP::OnQueryTrade(XTPQueryTradeRsp *trade_info, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (!IsErrorInfo(error_info) && trade_info)
	{
		if (NULL == _trades)
			_trades = WTSArray::create();

		WTSTradeInfo* trdInfo = makeTradeInfo(trade_info);
		if (trdInfo)
		{
			_trades->append(trdInfo, false);
		}
	}

	if (is_last)
	{
		if (_sink)
			_sink->onRspTrades(_trades);

		if (_trades)
			_trades->clear();
	}
}

void TraderXTP::OnQueryPosition(XTPQueryStkPositionRsp *position, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (!IsErrorInfo(error_info) && position )
	{
		if (NULL == _positions)
			_positions = PositionMap::create();

		std::string code, exchg;
		if (position->market == XTP_MKT_SH_A)
			exchg = "SSE";
		else
			exchg = "SZSE";
		code += position->ticker;
		WTSContractInfo* contract = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if (contract)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			std::string key = fmt::format("{}-{}", code.c_str(), position->position_direction);
			WTSPositionItem* pos = (WTSPositionItem*)_positions->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(code.c_str(), commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				_positions->add(key, pos, false);
			}
			pos->setDirection(wrapPosDirection(position->position_direction));

			pos->setNewPosition((double)(position->total_qty - position->yesterday_position));
			pos->setPrePosition((double)position->yesterday_position);

			pos->setMargin(position->total_qty*position->avg_price);
			pos->setDynProfit(0);
			pos->setPositionCost(position->total_qty*position->avg_price);

			pos->setAvgPrice(position->avg_price);

			pos->setAvailNewPos(0);
			pos->setAvailPrePos((double)position->sellable_qty);
		}
	}

	if (is_last)
	{

		WTSArray* ayPos = WTSArray::create();

		if (_positions && _positions->size() > 0)
		{
			for (auto it = _positions->begin(); it != _positions->end(); it++)
			{
				ayPos->append(it->second, true);
			}
		}

		if (_sink)
			_sink->onRspPosition(ayPos);

		if (_positions)
		{
			_positions->release();
			_positions = NULL;
		}

		ayPos->release();
	}
}

void TraderXTP::OnQueryAsset(XTPQueryAssetRsp *asset, XTPRI *error_info, int request_id, bool is_last, uint64_t session_id)
{
	//if (is_last)
	//{
	//	m_bInQuery = false;
	//	triggerQuery();
	//}

	if (is_last && !IsErrorInfo(error_info) && asset)
	{
		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setPreBalance(asset->orig_banlance);
		accountInfo->setCloseProfit(0);
		accountInfo->setDynProfit(0);
		accountInfo->setMargin(0);
		accountInfo->setAvailable(asset->buying_power);
		accountInfo->setCommission(asset->fund_sell_fee);
		accountInfo->setFrozenMargin(asset->withholding_amount);
		accountInfo->setFrozenCommission(0);
		if (asset->deposit_withdraw > 0)
			accountInfo->setDeposit(asset->deposit_withdraw);
		else if (asset->deposit_withdraw < 0)
			accountInfo->setWithdraw(0);
		accountInfo->setBalance(asset->total_asset);
		accountInfo->setCurrency("CNY");

		WTSArray * ay = WTSArray::create();
		ay->append(accountInfo, false);
		if (_sink)
			_sink->onRspAccount(ay);

		ay->release();
	}
}

#pragma endregion "XTP::API:TraderSpi"

#pragma region "ITraderApi"
bool TraderXTP::init(WTSVariant *params)
{
	_user = params->getCString("user");
	_pass = params->getCString("pass");
	_acckey = params->getCString("acckey");

	_host = params->getCString("host");
	_port = params->getInt32("port");

	_client = params->getInt32("client");

	_quick = params->getBoolean("quick");	

	_flowdir = params->getCString("flowdir");

	_hbInterval = params->getUInt32("hbinterval");

	if (_flowdir.empty())
		_flowdir = "XTPTDFlow";

	_flowdir = StrUtil::standardisePath(_flowdir);

	std::string module = params->getCString("xtpmodule");
	if (module.empty()) module = "xtptraderapi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;

	m_hInstXTP = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateTraderApi@TraderApi@API@XTP@@SAPEAV123@EPEBDW4XTP_LOG_LEVEL@@@Z";
#	else
	const char* creatorName = "?CreateTraderApi@TraderApi@API@XTP@@SAPAV123@EPBDW4XTP_LOG_LEVEL@@@Z";
#	endif
#else
	const char* creatorName = "_ZN3XTP3API9TraderApi15CreateTraderApiEhPKc13XTP_LOG_LEVEL";
#endif
	m_funcCreator = (XTPCreator)DLLHelper::get_symbol(m_hInstXTP, creatorName);

	return true;
}

void TraderXTP::release()
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

void TraderXTP::registerSpi(ITraderSpi *listener)
{
	_sink = listener;
	if (_sink)
	{
		_bd_mgr = listener->getBaseDataMgr();
	}
}

void TraderXTP::reconnect()
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
	_api = m_funcCreator(_client, ss.str().c_str(), XTP_LOG_LEVEL_DEBUG);			// 创建UserApi
	if (_api == NULL)
	{
		if (_sink)
			_sink->handleEvent(WTE_Connect, -1);
		write_log(_sink,LL_ERROR, "[TraderrXTP] Module initializing failed");

		StdThreadPtr thrd(new StdThread([this](){
			std::this_thread::sleep_for(std::chrono::seconds(2));
			write_log(_sink,LL_WARN, "[TraderrXTP] {} reconnecting...", _user.c_str());
			reconnect();
		}));
		return;
	}

	_api->SubscribePublicTopic(_quick ? XTP_TERT_QUICK : XTP_TERT_RESUME);
	_api->SetSoftwareVersion("1.0.0"); //设定此软件的开发版本号,用户自定义
	_api->SetSoftwareKey(_acckey.c_str());//设定用户的开发代码,在XTP申请开户时,由xtp人员提供
	_api->SetHeartBeatInterval(15);//设定交易服务器超时时间,单位为秒,此为1.1.16新增接口
	_api->RegisterSpi(this);						// 注册事件

	if (_sink)
		_sink->handleEvent(WTE_Connect, 0);
}

void TraderXTP::connect()
{
	reconnect();

	if (_thrd_worker == NULL)
	{
		//boost::asio::io_service::work work(_asyncio);
		_worker.reset(new boost::asio::io_service::work(_asyncio));
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

void TraderXTP::disconnect()
{
	release();
}

bool TraderXTP::isConnected()
{
	return (_state == TS_ALLREADY);
}

void TraderXTP::genEntrustID(char* buffer, uint32_t orderRef)
{
	//这里不再使用sessionid，因为每次登陆会不同，如果使用的话，可能会造成不唯一的情况
	fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderRef);
}

bool TraderXTP::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	auto idx = StrUtil::findLast(entrustid, '#');
	if (idx == std::string::npos)
		return false;

	orderRef = strtoul(entrustid + idx + 1, NULL, 10);

	return true;
}

bool TraderXTP::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		uint32_t orderref = _ordref.fetch_add(1) + 1;
		fmtutil::format_to(buffer, "{}#{}#{}", _user, _tradingday, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderXTP::doLogin()
{
	_state = TS_LOGINING;

	_api->SetHeartBeatInterval(_hbInterval);
	uint64_t iResult = _api->Login(_host.c_str(), _port, _user.c_str(), _pass.c_str(), XTP_PROTOCOL_TCP);
	if (iResult == 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink, LL_ERROR, "[TraderXTP] Login failed: {}", error_info->error_msg);
		std::string msg = error_info->error_msg;
		_state = TS_LOGINFAILED;
		_asyncio.post([this, msg] {
			_sink->onLoginResult(false, msg.c_str(), 0);
		});
	}
	else
	{
		_sessionid = iResult;
		if (!_inited)
		{
			_tradingday = strtoul(_api->GetTradingDay(), NULL, 10);

			{
				//初始化委托单缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_eid.sc";
				m_eidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			{
				//初始化订单标记缓存器
				std::stringstream ss;
				ss << "./xtpdata/local/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << _user << "_oid.sc";
				m_oidCache.init(ss.str().c_str(), _tradingday, [this](const char* message) {
					write_log(_sink, LL_WARN, message);
				});
			}

			write_log(_sink, LL_INFO, "[TraderXTP] [{}] Login succeed, trading date: {}...", _user.c_str(), _tradingday);

			_state = TS_LOGINED;
			_inited = true;
			_asyncio.post([this] {
				_state = TS_ALLREADY;
				_sink->onLoginResult(true, 0, _tradingday);				
			});
		}
		else
		{
			write_log(_sink, LL_INFO, "[TraderXTP] [{}] Connection recovered", _user.c_str());
			_state = TS_ALLREADY;
		}
	}
}

int TraderXTP::login(const char* user, const char* pass, const char* productInfo)
{
	_user = user;
	_pass = pass;

	if (_api == NULL)
	{
		return -1;
	}

	doLogin();
	return 0;
}

int TraderXTP::logout()
{
	if (_api == NULL)
		return -1;

	_api->Logout(_sessionid);
	return 0;
}

int TraderXTP::orderInsert(WTSEntrust* entrust)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	XTPOrderInsertInfo req;
	memset(&req, 0, sizeof(req));
	
	uint32_t orderref;
	extractEntrustID(entrust->getEntrustID(), orderref);
	req.order_client_id = orderref;
	strcpy(req.ticker, entrust->getCode());
	req.market = wt_stricmp(entrust->getExchg(), "SSE") == 0 ? XTP_MKT_SH_A : XTP_MKT_SZ_A;
	req.price = entrust->getPrice();
	req.quantity = (int64_t)entrust->getVolume();
	req.price_type = XTP_PRICE_LIMIT;
	req.side = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.business_type = XTP_BUSINESS_TYPE_CASH;
	req.position_effect = wrapOffsetType(entrust->getOffsetType());

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag(), 0, [this](const char* message) {
			write_log(_sink, LL_WARN, message);
		});
	}

	uint64_t iResult = _api->InsertOrder(&req, _sessionid);
	if (iResult == 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Order inserting failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTP::orderAction(WTSEntrustAction* action)
{
	if (_api == NULL || _state != TS_ALLREADY)
	{
		return -1;
	}

	uint64_t iResult = _api->CancelOrder(strtoull(action->getOrderID(), NULL, 10), _sessionid);
	if (iResult == 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Order cancelling failed: {}", error_info->error_msg);
	}

	return 0;
}

uint32_t TraderXTP::genRequestID()
{
	return _reqid.fetch_add(1) + 1;
}

int TraderXTP::queryAccount()
{
	int iResult = _api->QueryAsset(_sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Account querying failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTP::queryPositions()
{
	int iResult = _api->QueryPosition("", _sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Positions querying failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTP::queryOrders()
{
	XTPQueryOrderReq req;
	memset(&req, 0, sizeof(XTPQueryOrderReq));
	int iResult = _api->QueryOrders(&req, _sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Orders querying failed: {}", error_info->error_msg);
	}

	return 0;
}

int TraderXTP::queryTrades()
{
	XTPQueryTraderReq req;
	memset(&req, 0, sizeof(XTPQueryTraderReq));
	int iResult = _api->QueryTrades(&req, _sessionid, genRequestID());
	if (iResult != 0)
	{
		auto error_info = _api->GetApiLastError();
		write_log(_sink,LL_ERROR, "[TraderXTP] Trades querying failed: {}", error_info->error_msg);
	}

	return 0;
}
#pragma endregion "ITraderApi"