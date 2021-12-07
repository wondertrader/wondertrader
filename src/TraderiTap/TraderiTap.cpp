#include "TraderiTap.h"
#include "HttpClient.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSParams.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/ModuleHelper.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"

#include "../API/iTap9.3.3.3/iTapAPIError.h"

#include <boost/filesystem.hpp>

uint32_t strToTime(const char* strTime)
{
	std::string str = strTime;
	StrUtil::replace(str, ":", "");
	StrUtil::replace(str, ".", "");
	StrUtil::replace(str, "-", "");

	return strtoul(str.c_str(), NULL, 10);
}

uint64_t extractOrdTime(const char* dateTime, uint32_t& curDate)
{
	std::string sDtTm = dateTime;
	if(sDtTm.find('-') == -1)
	{
		const StringVector &ay = StrUtil::split(dateTime, ".");

		std::string sDate = "20";
		sDate += ay[0].substr(0, 6);
		curDate = strtoul(sDate.c_str(), NULL, 10);

		std::string sTime = ay[0].substr(6) + ay[1].substr(0, 3);

		return TimeUtils::makeTime(curDate, strtoul(sTime.c_str(), NULL, 10));
	}
	else
	{
		StringVector ay = StrUtil::split(dateTime, " ");
		StrUtil::replace(ay[0], "-", "");
		curDate = strtoul(ay[0].c_str(), NULL, 10);
		return TimeUtils::makeTime(curDate, strToTime(ay[1].c_str()));
	}
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderiTap *instance = new TraderiTap();
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


TraderiTap::TraderiTap()
	: m_pUserAPI(NULL)
	, m_ayOrders(NULL)
	, m_ayTrades(NULL)
	, m_mapFunds(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_iRequestID(0)
	, m_bStopped(false)
	, m_bReconnect(false)
{
	memset(m_strContract, 0, 41);
	m_orderId = 0;
}


TraderiTap::~TraderiTap()
{
}

uint32_t* TraderiTap::makeRequestID()
{
	m_iRequestID++;
	return &m_iRequestID;
}


void TAP_CDECL TraderiTap::OnRspQrySpotLock(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPISpotLockDataRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnSpotLock(const ITapTrade::TapAPISpotLockDataRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspSubmitUserLoginInfo(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPISubmitUserLoginRspInfo * info)
{
	
}


void TAP_CDECL TraderiTap::OnRspSpecialOrderAction(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPISpecialOrderInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnSpecialOrder(const ITapTrade::TapAPISpecialOrderInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQrySpecialOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPISpecialOrderInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryCombinePosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICombinePositionInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnCombinePosition(const ITapTrade::TapAPICombinePositionInfo *info)
{
	
}

void TAP_CDECL TraderiTap::OnRspQryTradingDate(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, const ITapTrade::TapAPITradingCalendarQryRsp *info)
{
}

void TAP_CDECL TraderiTap::OnRspQryOrderProcess(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIOrderInfo *info)
{

}

void TAP_CDECL TraderiTap::OnRspQryExchange(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIExchangeInfo *info)
{

}

void TAP_CDECL TraderiTap::OnRspQryCurrency(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICurrencyInfo *rsp)
{

}

void TAP_CDECL TraderiTap::OnRspQryPositionSummary(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIPositionSummary *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryTradeMessage(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPITradeMessage *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryHisOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisOrderQryRsp *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryHisOrderProcess(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisOrderProcessQryRsp *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryHisMatch(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisMatchQryRsp *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryHisPosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisPositionQryRsp *info)
{
}


void TAP_CDECL TraderiTap::OnRspQryHisDelivery(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIHisDeliveryQryRsp *info)
{
}

void TraderiTap::reconnect()
{
	if (m_pUserAPI)
	{
		m_pUserAPI->SetAPINotify(NULL);
		m_funcDeleter(m_pUserAPI);
		m_pUserAPI = NULL;
	}

	TAPIINT32 CreateErrorCode = TAPIERROR_SUCCEED;
	TapAPIApplicationInfo stAppInfo;
	strcpy(stAppInfo.AuthCode, m_strAppKey.c_str());
	if (!boost::filesystem::exists(".\\ESFLogs\\"))
        boost::filesystem::create_directory(".\\ESFLogs\\");
	strcpy(stAppInfo.KeyOperationLogPath, ".\\ESFLogs\\");
	m_pUserAPI = m_funcCreator(&stAppInfo, CreateErrorCode);
	if (m_pUserAPI == NULL)
	{
		if (m_sink)
			m_sink->handleEvent(WTE_Connect, -1);
		m_sink->handleTraderLog(LL_ERROR, "[TraderiTap] Trading module initializing failed: %s...", GetErrcodeDesc(CreateErrorCode));

		StdThreadPtr thrd(new StdThread([this](){
			std::this_thread::sleep_for(std::chrono::seconds(2));
			m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
			reconnect();
		}));
		return;
	}

	m_pUserAPI->SetAPINotify(this);

	if (m_pUserAPI)
	{

		TAPIINT32 iErr = m_pUserAPI->SetHostAddress(m_strHost.c_str(), m_nPort);
		if (iErr != TAPIERROR_SUCCEED)
		{
			if (m_sink)
				m_sink->handleEvent(WTE_Connect, -1);

			//如果自动重连,则要继续自动重连
			if (m_bReconnect)
			{
				//这里丢到线程里去处理,让reconnect可以马上返回
				StdThreadPtr thrd(new StdThread([this](){
					std::this_thread::sleep_for(std::chrono::seconds(2));
					m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
					reconnect();
				}));
			}
		}
		else
		{
			if (m_sink)
				m_sink->handleEvent(WTE_Connect, 0);
		}
	}
}

bool TraderiTap::init(WTSParams* params)
{
	m_strHost = params->getCString("host");
	m_nPort = params->getInt32("port");

	m_strUser = params->getCString("user");
	m_strPass = params->getCString("pass");

	m_strAppKey = params->getCString("authkey");

	m_strCodePath = params->getCString("codepath");


	WTSParams* param = params->get("tapmodule");
	if (param != NULL)
		m_strModule = getBinDir() + DLLHelper::wrap_module(param->asCString(),"lib");
	else
		m_strModule = getBinDir() + DLLHelper::wrap_module("iTapTradeAPI", "lib");

	m_hInstESF = DLLHelper::load_library(m_strModule.c_str());
	m_funcCreator = (ESFCreator)DLLHelper::get_symbol(m_hInstESF, "CreateITapTradeAPI");
	m_funcDeleter = (ESFDeleter)DLLHelper::get_symbol(m_hInstESF, "FreeITapTradeAPI");
	m_funcGetErrorDesc = (ESFGetErrorDesc)DLLHelper::get_symbol(m_hInstESF, "GetITapErrorDescribe");

	return true;
}

void TraderiTap::release()
{
	m_bStopped = true;

	if(m_pUserAPI)
	{
		m_pUserAPI->SetAPINotify(NULL);
		m_funcDeleter(m_pUserAPI);
		m_pUserAPI = NULL;
	}

	if(m_ayOrders)
		m_ayOrders->clear();


	if(m_ayTrades)
		m_ayTrades->clear();
}

void TraderiTap::connect()
{
	m_bStopped = false;

	reconnect();
}

void TraderiTap::disconnect()
{
	release();
}

bool TraderiTap::makeEntrustID(char* buffer, int length)
{
	if(buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t thisid = m_orderId.fetch_add(1) + 1;
		sprintf(buffer, "#%s#.%s.%06u", m_strUser.c_str(), StrUtil::fmtUInt64(m_uLoginTime).c_str(), thisid);
		return true;
	}
	catch(...)
	{

	}

	return false;
}

void TraderiTap::registerSpi( ITraderSpi *listener )
{
	m_sink = listener;
	m_bdMgr = m_sink->getBaseDataMgr();
}

int TraderiTap::login( const char* user, const char* pass, const char* productInfo )
{
	m_strUser = user;
	m_strPass = pass;

	if(m_pUserAPI == NULL)
	{
		return -1;
	}

	m_wrapperState = WS_LOGINING;

	TapAPITradeLoginAuth stLoginAuth;
	memset(&stLoginAuth, 0, sizeof(stLoginAuth));
	strcpy(stLoginAuth.UserNo, m_strUser.c_str());
	strcpy(stLoginAuth.Password, m_strPass.c_str());
	stLoginAuth.ISModifyPassword = APIYNFLAG_NO;	

	uint32_t iResult = m_pUserAPI->Login(&stLoginAuth);
	if (iResult != TAPIERROR_SUCCEED)
	{
		m_sink->handleTraderLog(LL_ERROR, "[TraderiTap] Sending login request failed: %s", GetErrcodeDesc(iResult));
	}
	return 0;
}

int TraderiTap::logout()
{
	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	m_pUserAPI->Disconnect();
	return 0;
}

int TraderiTap::orderInsert( WTSEntrust* entrust )
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	//if(entrust->getPriceType() == WPT_ANYPRICE)
	{
		entrust->setTimeCondition(WTC_GFD);
	}

	WTSContractInfo* ct = m_bdMgr->getContract(entrust->getCode(), entrust->getExchg());
	if (ct == NULL)
	{
		m_sink->handleTraderLog(LL_ERROR, "Instrument %s not exists, not exchangable", ct->getFullCode());
		return -1;
	}
	char* code = (char*)entrust->getCode();
	code += strlen(ct->getProduct());

	TapAPINewOrder req;

	strcpy(req.AccountNo, m_strUser.c_str());
	strcpy(req.ExchangeNo, ct->getExchg());
	req.CommodityType = TAPI_COMMODITY_TYPE_FUTURES;
	strcpy(req.CommodityNo, productI2O(ct->getProduct()));
	strcpy(req.ContractNo, code);
	req.OrderType = wrapPriceType(entrust->getPriceType());
	req.OrderSource = TAPI_ORDER_SOURCE_PROGRAM;
	req.TimeInForce = wrapTimeCondition(entrust->getTimeCondition());
	req.OrderSide = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.OrderPrice = entrust->getPrice();
	req.OrderQty = (int)entrust->getVolume();
	strcpy(req.RefString, entrust->getUserTag());

	int iResult = m_pUserAPI->InsertOrder(makeRequestID(), NULL, &req);
	if(iResult != 0)
	{
		m_sink->handleTraderLog(LL_ERROR, "[TraderiTap] Order inserting failed: %s", GetErrcodeDesc(iResult));
		return iResult;
	}

	return 0;
}

int TraderiTap::orderAction( WTSEntrustAction* action )
{
	if(m_wrapperState != WS_ALLREADY)
		return -1;

	std::string strOrd = action->getOrderID();
	strOrd = strOrd.substr(m_strUser.size() + 5);

	TapAPIOrderCancelReq req;
	//memset(&req, 0, sizeof(req));
	strcpy(req.OrderNo, strOrd.c_str());

	int iResult = m_pUserAPI->CancelOrder(makeRequestID(), &req);
	if(iResult != 0)
	{
		m_sink->handleTraderLog(LL_ERROR, "[TraderiTap] Sending cancel request failed: %d", iResult);
		return iResult;
	}

	return 0;
}

int TraderiTap::queryCommodity()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_APIINITED)
	{
		return -1;
	}

	return m_pUserAPI->QryCommodity(makeRequestID());
}

int TraderiTap::queryContracts()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	int32_t iResult = m_pUserAPI->QryCommodity(makeRequestID());
	if(iResult != 0)
	{
		m_sink->handleTraderLog(LL_ERROR, "[ESFTrader-%s] Sending query of contracts failed: %d", m_strUser.c_str(), iResult);
		return -1;
	}

	return 0;
}


int TraderiTap::queryAccount()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	TapAPIFundReq req;
	memset(&req, 0, sizeof(req));
	strcpy(req.AccountNo, m_strUser.c_str());
	int iResult = m_pUserAPI->QryFund(makeRequestID(), &req);
	if (iResult != 0)
	{
		if (m_sink)
			m_sink->handleTraderLog(LL_ERROR, "[TraderiTap] Sending query of account failed: %d", iResult);
		return -1;
	}

	return 0;
}

int TraderiTap::queryPositions()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	TapAPIPositionQryReq req;
	//memset(&req, 0, sizeof(req));
	strcpy(req.AccountNo, m_strUser.c_str());
	int iResult = m_pUserAPI->QryPosition(makeRequestID(), &req);
	if (iResult != 0)
		return -1;

	return 0;
}

int TraderiTap::queryOrders()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	TapAPIOrderQryReq req;
	//memset(&req, 0, sizeof(req));
	strcpy(req.AccountNo, m_strUser.c_str());

	m_pUserAPI->QryOrder(makeRequestID(), &req);

	return 0;
}

int TraderiTap::queryTrades()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	TapAPIFillQryReq req;
	//memset(&req, 0, sizeof(req));
	strcpy(req.AccountNo, m_strUser.c_str());

	m_pUserAPI->QryFill(makeRequestID(), &req);

	return 0;
}


void TAP_CDECL TraderiTap::OnRspQryFill(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIFillInfo *rsp)
{
	if (errCode == 0 && rsp)
	{
		if (NULL == m_ayTrades)
			m_ayTrades = WTSArray::create();

		WTSTradeInfo* trade = makeTradeRecord(rsp);
		if (trade)
		{
			m_ayTrades->append(trade, false);
		}
	}

	if (isLast == APIYNFLAG_YES)
	{
		if (m_sink)
			m_sink->onRspTrades(m_ayTrades);

		if (NULL != m_ayTrades)
			m_ayTrades->clear();
	}
}


void TAP_CDECL TraderiTap::OnRspQryFund(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIFundData *rsp)
{
	if (errCode == 0 && rsp)
	{
		const char* currency = currencyO2I(rsp->CurrencyNo);

		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setDescription(StrUtil::printf("%s", m_strUser.c_str()).c_str());
		//accountInfo->setUsername(m_strUserName.c_str());
		accountInfo->setPreBalance(rsp->PreBalance);
		accountInfo->setCloseProfit(rsp->CloseProfit);
		accountInfo->setDynProfit(rsp->PositionProfit);
		accountInfo->setMargin(rsp->FrozenDeposit);
		//accountInfo->setAvailable((rsp->TAvailable));
		accountInfo->setCommission(rsp->AccountFee);
		accountInfo->setFrozenMargin(rsp->FrozenDeposit);
		accountInfo->setFrozenCommission(rsp->FrozenFee);
		accountInfo->setDeposit(rsp->CashInValue);
		accountInfo->setWithdraw(rsp->CashOutValue);
		accountInfo->setBalance(accountInfo->getPreBalance() + accountInfo->getCloseProfit() - accountInfo->getCommission() + accountInfo->getDeposit() - accountInfo->getWithdraw());
		accountInfo->setAvailable(accountInfo->getBalance() - accountInfo->getFrozenCommission() - accountInfo->getFrozenMargin() - accountInfo->getMargin());
		accountInfo->setCurrency(currency);

		if (m_mapFunds == NULL)
			m_mapFunds = WTSStringMap::create();

		m_mapFunds->add(currency, accountInfo, false);
	}

	if (isLast == APIYNFLAG_YES)
	{
		WTSArray* ay = WTSArray::create();
		if (m_mapFunds)
		{
			for (auto it = m_mapFunds->begin(); it != m_mapFunds->end(); it++)
			{
				WTSAccountInfo* accDes = (WTSAccountInfo*)it->second;
				ay->append(accDes, true);
			}
		}


		if (m_sink)
			m_sink->onRspAccount(ay);

		ay->release();

		if (m_mapFunds)
		{
			m_mapFunds->release();
			m_mapFunds = NULL;
		}
	}
}


void TAP_CDECL TraderiTap::OnRspQryOrder(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIOrderInfo *rsp)
{
	if (errCode == 0 && rsp)
	{
		if (NULL == m_ayOrders)
			m_ayOrders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(rsp);
		if (orderInfo)
		{
			m_ayOrders->append(orderInfo, false);
		}
	}

	if (isLast == APIYNFLAG_YES)
	{
		if (m_sink)
			m_sink->onRspOrders(m_ayOrders);

		if (m_ayOrders)
			m_ayOrders->clear();
	}
}

void TAP_CDECL TraderiTap::OnRspQryPosition(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIPositionInfo *info)
{
	if (errCode == 0 && info)
	{
		if (NULL == m_mapPosition)
			m_mapPosition = PositionMap::create();

		std::string code = StrUtil::printf("%s%s", productO2I(info->CommodityNo), info->ContractNo);
		const char* exchg = exchgO2I(info->ExchangeNo);
		WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg);
		if (contract)
		{
			WTSCommodityInfo* commInfo = m_bdMgr->getCommodity(contract);
			std::string key = StrUtil::printf("%s-%d", code.c_str(), info->MatchSide);
			WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(code.c_str(), commInfo->getCurrency(), exchg);
				m_mapPosition->add(key, pos, false);
			}

			pos->setDirection(wrapPosDirection(info->MatchSide));
			if (!info->IsHistory)
				pos->setNewPosition(info->PositionQty);
			else
				pos->setPrePosition(info->PositionQty);

			pos->setMargin(pos->getMargin() + info->AccountMaintenanceMargin);
			pos->setDynProfit(pos->getDynProfit() + info->PositionProfit);
			pos->setPositionCost(pos->getPositionCost() + info->PositionPrice*commInfo->getVolScale()*info->PositionQty);

			if (pos->getTotalPosition() != 0)
			{
				pos->setAvgPrice(info->PositionPrice);
			}
			else
			{
				pos->setAvgPrice(0);
			}

			pos->setAvailNewPos(pos->getNewPosition());
			pos->setAvailPrePos(pos->getPrePosition());
		}
	}

	if (isLast == APIYNFLAG_YES)
	{
		WTSArray* ayPos = WTSArray::create();

		if (m_mapPosition && m_mapPosition->size() > 0)
		{
			for (auto it = m_mapPosition->begin(); it != m_mapPosition->end(); it++)
			{
				ayPos->append(it->second, true);
			}
		}

		if (m_sink)
			m_sink->onRspPosition(ayPos);

		if (m_mapPosition)
		{
			m_mapPosition->release();
			m_mapPosition = NULL;
		}

		ayPos->release();
	}
}


int TraderiTap::wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if( WDT_LONG == dirType)
		if(offsetType == WOT_OPEN)
			return TAPI_SIDE_BUY;
		else
			return TAPI_SIDE_SELL;
	else
		if(offsetType == WOT_OPEN)
			return TAPI_SIDE_SELL;
		else
			return TAPI_SIDE_BUY;
}

WTSDirectionType TraderiTap::wrapDirectionType(TAPISideType dirType, TAPIPositionEffectType offType)
{
	if (TAPI_SIDE_BUY == dirType)
		if (offType == TAPI_PositionEffect_OPEN)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if (offType == TAPI_PositionEffect_OPEN)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

WTSDirectionType TraderiTap::wrapPosDirection(TAPISideType dirType)
{
	if (TAPI_SIDE_BUY == dirType)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

int TraderiTap::wrapOffsetType(WTSOffsetType offType)
{
	if( WOT_OPEN == offType)
		return TAPI_PositionEffect_OPEN;
	else if( WOT_CLOSE == offType)
		return TAPI_PositionEffect_COVER;
	else if( WOT_CLOSETODAY == offType)
		return TAPI_PositionEffect_COVER_TODAY;
	else if( WOT_CLOSEYESTERDAY == offType)
		return TAPI_PositionEffect_COVER;
	else
		return TAPI_PositionEffect_OPEN;
}

WTSOffsetType TraderiTap::wrapOffsetType(TAPIPositionEffectType offType)
{
	if (TAPI_PositionEffect_OPEN == offType)
		return WOT_OPEN;
	else if (TAPI_PositionEffect_COVER == offType)
		return WOT_CLOSE;
	else if (TAPI_PositionEffect_COVER_TODAY == offType)
		return WOT_CLOSETODAY;
	else
		return WOT_FORCECLOSE;
}

int TraderiTap::wrapPriceType(WTSPriceType priceType)
{
	if(WPT_ANYPRICE == priceType)
		return TAPI_ORDER_TYPE_MARKET;
	else
		return TAPI_ORDER_TYPE_LIMIT;
}

WTSPriceType TraderiTap::wrapPriceType(TAPIOrderTypeType priceType)
{
	if (TAPI_ORDER_TYPE_MARKET == priceType)
		return WPT_ANYPRICE;
	else if (TAPI_ORDER_TYPE_LIMIT == priceType)
		return WPT_LIMITPRICE;
	else
		return WPT_ANYPRICE;
}

int TraderiTap::wrapTimeCondition(WTSTimeCondition timeCond)
{
	if(WTC_IOC == timeCond)
		return TAPI_ORDER_TIMEINFORCE_FAK;
	else if(WTC_GFD == timeCond)
		return TAPI_ORDER_TIMEINFORCE_GFD;
	else
		return TAPI_ORDER_TIMEINFORCE_FOK;
}

WTSTimeCondition TraderiTap::wrapTimeCondition(TAPITimeInForceType timeCond)
{
	if (TAPI_ORDER_TIMEINFORCE_FAK == timeCond)
		return WTC_IOC;
	else if (TAPI_ORDER_TIMEINFORCE_GFD == timeCond)
		return WTC_GFD;
	else
		return WTC_GFS;
}

WTSOrderState TraderiTap::wrapOrderState(TAPIOrderStateType orderState)
{
	switch(orderState)
	{
	case TAPI_ORDER_STATE_ACCEPT:
		return WOS_NotTraded_NotQueuing;
	case TAPI_ORDER_STATE_QUEUED:
		return WOS_NotTraded_Queuing;
	case TAPI_ORDER_STATE_PARTFINISHED:
		return WOS_PartTraded_Queuing;
	case TAPI_ORDER_STATE_FINISHED:
		return WOS_AllTraded;
	case TAPI_ORDER_STATE_LEFTDELETED:
	case TAPI_ORDER_STATE_DELETED:
	case TAPI_ORDER_STATE_FAIL:
	case TAPI_ORDER_STATE_CANCELED:
		return WOS_Canceled;
	default:
		return WOS_Submitting;
	}
}


WTSOrderInfo* TraderiTap::makeOrderInfo(const TapAPIOrderInfo* orderField)
{
	std::string code = StrUtil::printf("%s%s", productO2I(orderField->CommodityNo), orderField->ContractNo);
	const char* exchg = exchgO2I(orderField->ExchangeNo);
	WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg);
	if(contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(orderField->OrderPrice);
	pRet->setVolume(orderField->OrderQty);
	pRet->setDirection(wrapDirectionType(orderField->OrderSide, orderField->PositionEffect));
	pRet->setPriceType(wrapPriceType(orderField->OrderType));
	pRet->setTimeCondition(wrapTimeCondition(orderField->TimeInForce));
	pRet->setOffsetType(wrapOffsetType(orderField->PositionEffect));

	pRet->setVolTraded(orderField->OrderMatchQty);
	pRet->setVolLeft(orderField->IsDeleted ? 0 : orderField->OrderQty - orderField->OrderMatchQty);

	pRet->setCode(code.c_str());
	pRet->setExchg(contract->getExchg());

	uint32_t curDate = m_uDate;
	pRet->setOrderTime(extractOrdTime(orderField->OrderInsertTime, curDate));
	pRet->setOrderDate(curDate);

	pRet->setOrderState(wrapOrderState(orderField->OrderState));

	pRet->setEntrustID(StrUtil::printf("%s.Ord.%s", m_strUser.c_str(), orderField->OrderNo).c_str());
	pRet->setOrderID(pRet->getEntrustID());

	pRet->setUserTag(orderField->RefString);
	pRet->setStateMsg(orderField->ErrorText);

	return pRet;
}

const char* TraderiTap::GetErrcodeDesc(int ec)
{
	static std::string curErr = "未知错误";
	if (m_pUserAPI)
		curErr = m_funcGetErrorDesc(ec);

	if(curErr.compare("未知错误") == 0)
	{
		curErr = StrUtil::printf("未知错误: %d", ec);
	}
	return curErr.c_str();
}

WTSOrderInfo* TraderiTap::makeOrderInfo(const TapAPIOrderInfoNotice* noticeField)
{
	if (noticeField == NULL)
		return NULL;

	return makeOrderInfo(noticeField->OrderInfo);
}


WTSEntrust* TraderiTap::makeEntrust(const TapAPIOrderLocalInputRsp *entrustField)
{
	std::string code = StrUtil::printf("%s%s", productO2I(entrustField->CommodityNo), entrustField->ContractNo);
	const char* exchg = exchgO2I(entrustField->ExchangeNo);

	WTSEntrust* pRet = WTSEntrust::create(code.c_str(),entrustField->OrderQty, entrustField->OrderPrice, exchg);

	pRet->setDirection(wrapDirectionType(entrustField->OrderSide, entrustField->PositionEffect));
	pRet->setPriceType(wrapPriceType(entrustField->OrderType));
	pRet->setOffsetType(wrapOffsetType(entrustField->PositionEffect));
	pRet->setTimeCondition(wrapTimeCondition(entrustField->TimeInForce));

	pRet->setUserTag(entrustField->RefString);

	return pRet;
}

WTSError* TraderiTap::makeErrorInfo(WTSErroCode errCode, const char* msg)
{
	WTSError* pRet = WTSError::create(errCode, msg);
	return pRet;
}

WTSTradeInfo* TraderiTap::makeTradeRecord(const TapAPIFillInfo* tradeField)
{
	std::string code = StrUtil::printf("%s%s", productO2I(tradeField->CommodityNo), tradeField->ContractNo);
	const char* exchg = exchgO2I(tradeField->ExchangeNo);
	WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg);
	if(contract == NULL)
		return NULL;

	WTSTradeInfo *pRet = WTSTradeInfo::create(code.c_str());
	pRet->setVolume(tradeField->MatchQty);
	pRet->setPrice(tradeField->MatchPrice);
	pRet->setTradeID(StrUtil::printf("%s.Trd.%s", m_strUser.c_str(), tradeField->MatchNo).c_str());

	StringVector ay = StrUtil::split(tradeField->MatchDateTime, " ");
	StrUtil::replace(ay[0], "-", "");
	pRet->setTradeDate(strtoul(ay[0].c_str(), NULL, 10));
	pRet->setTradeTime(TimeUtils::makeTime(pRet->getTradeDate(), strToTime(ay[1].c_str())*1000));

	WTSDirectionType dType = wrapDirectionType(tradeField->MatchSide, tradeField->PositionEffect);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(tradeField->PositionEffect));
	//pRet->setRefOrder(StrUtil::printf("%u", tradeField->StateData.OrderId).c_str());
	pRet->setRefOrder(StrUtil::printf("%s.Ord.%s", m_strUser.c_str(), tradeField->OrderNo).c_str());
	pRet->setTradeType(WTT_Common);
	
	pRet->setUserTag("");

	return pRet;
}

bool TraderiTap::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}

const char* TraderiTap::exchgO2I(const char* exchg)
{
	auto it = m_mapExchgOToI.find(exchg);
	if(it != m_mapExchgOToI.end())
		return it->second.c_str();

	return exchg;
}

const char* TraderiTap::exchgI2O(const char* exchg)
{
	auto it = m_mapExchgIToO.find(exchg);
	if(it != m_mapExchgIToO.end())
		return it->second.c_str();

	return exchg;
}

const char* TraderiTap::productO2I(const char* pid)
{
	if (pid == NULL)
		return "";

	auto it = m_mapProductOToI.find(pid);
	if(it != m_mapProductOToI.end())
		return it->second.c_str();

	return pid;
}

const char* TraderiTap::productI2O(const char* pid)
{
	auto it = m_mapProductIToO.find(pid);
	if(it != m_mapProductIToO.end())
		return it->second.c_str();

	return pid;
}

const char* TraderiTap::currencyO2I(const char* currency)
{
	auto it = m_mapCurrencyOToI.find(currency);
	if (it != m_mapCurrencyOToI.end())
		return it->second.c_str();

	return currency;
}


void TAP_CDECL TraderiTap::OnAPIReady(ITapTrade::TAPIINT32 errCode)
{
	if(errCode == 0)
	{
		//m_wrapperState = WS_APIINITED;

		m_wrapperState = WS_ALLREADY;
		m_sink->handleTraderLog(LL_INFO, "[TraderiTap-%s] Trading channeld initialized...", m_strUser.c_str());

		if (m_sink)
			m_sink->onLoginResult(true, "", 0);

	}
	else
	{

	}
}


void TAP_CDECL TraderiTap::OnConnect()
{
	//if (m_sink)
	//	m_sink->handleEvent(WTE_Connect, 0);
	int x = 0;
}


void TAP_CDECL TraderiTap::OnDisconnect(ITapTrade::TAPIINT32 reasonCode)
{
	m_wrapperState = WS_NOTLOGIN;
	if (m_sink)
		m_sink->handleEvent(WTE_Close, 0);

	if (!m_bStopped)
	{
		if (!m_bReconnect)
		{
			m_bReconnect = true;
			//这里丢到线程里去处理,让OnClose可以马上返回
			StdThreadPtr thrd(new StdThread([this](){
				std::this_thread::sleep_for(std::chrono::seconds(2));
				m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
				reconnect();
			}));
		}
	}
}


void TAP_CDECL TraderiTap::OnExpriationDate(ITapTrade::TAPIDATE date, int days)
{
	
}


void TAP_CDECL TraderiTap::OnHKMarketQuoteNotice(const ITapTrade::TapAPIOrderQuoteMarketNotice *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspAuthPassword(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode)
{
	
}


void TAP_CDECL TraderiTap::OnRspChangePassword(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode)
{
	
}


void TAP_CDECL TraderiTap::OnRspFillLocalInput(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIFillLocalInputRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspFillLocalRemove(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIFillLocalRemoveRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspHKMarketOrderDelete(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderMarketDeleteRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspHKMarketOrderInsert(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderMarketInsertRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspLogin(ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPITradeLoginRspInfo *loginRspInfo)
{
	if (errCode == 0)
	{
		m_wrapperState = WS_LOGINED;
		static uint32_t esfinstance = 0;
		m_uLoginTime = TimeUtils::getLocalTimeNow() * 100 + esfinstance++;
		m_uDate = strToTime(loginRspInfo->TradeDate);

		m_sink->handleTraderLog(LL_INFO, "[TraderiTap-%s] Login succeed...", m_strUser.c_str());

		m_bReconnect = false;
	}
	else if (errCode == TAPIERROR_LOGIN_SECCERTIFI || errCode == TAPIERROR_LOGIN_DDA)
	{
		if (m_strCodePath.empty())
		{
			m_wrapperState = WS_LOGINFAILED;

			if (m_sink)
				m_sink->onLoginResult(false, "Second authentiation needed, but no authcode server setup", 0);

			if (m_bReconnect)
			{
				//这里丢到线程里去处理,让onclose可以马上返回
				StdThreadPtr thrd(new StdThread([this](){
					std::this_thread::sleep_for(std::chrono::seconds(2));
					m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
					reconnect();
				}));
			}
		}
		else
		{
			m_wrapperState = WS_DYNAUTH;
			if (strlen(m_strContract) > 0)
				m_pUserAPI->RequestVertificateCode(makeRequestID(), m_strContract);
		}
	}
	else
	{
		m_wrapperState = WS_LOGINFAILED;

		if (m_sink)
			m_sink->onLoginResult(false, GetErrcodeDesc(errCode), 0);

		if (m_bReconnect)
		{
			//这里丢到线程里去处理,让onclose可以马上返回
			StdThreadPtr thrd(new StdThread([this](){
				std::this_thread::sleep_for(std::chrono::seconds(2));
				m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
				reconnect();
			}));
		}
	}
}


void TAP_CDECL TraderiTap::OnRspOrderAction(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderActionRsp *info)
{
	if(info->ActionType == APIORDER_INSERT)
	{
		if (info == NULL)
		{
			WTSError *error = WTSError::create(WEC_ORDERINSERT, GetErrcodeDesc(errCode));
			if (m_sink)
				m_sink->onTraderError(error);
			error->release();
			return;
		}

		WTSEntrust* entrust = makeEntrust(info->OrderInfo);
		if (entrust)
		{
			WTSError *err = errCode == 0 ? NULL : makeErrorInfo(WEC_ORDERINSERT, GetErrcodeDesc(errCode));
			//g_orderMgr.onRspEntrust(entrust, err);
			if (m_sink)
				m_sink->onRspEntrust(entrust, err);
			entrust->release();
			if (err) err->release();
		}

		if (errCode == 0)
		{
			WTSOrderInfo* order = makeOrderInfo(info->OrderInfo);
			if (order)
			{
				if (m_sink)
					m_sink->onPushOrder(order);
				order->release();
			}
		}
	}
	else if (info->ActionType == APIORDER_DELETE)
	{
		if (errCode != 0)
		{
			WTSError* error = WTSError::create(WEC_ORDERCANCEL, GetErrcodeDesc(errCode));
			if (m_sink)
				m_sink->onTraderError(error);
		}
	}
}


void TAP_CDECL TraderiTap::OnRspOrderLocalInput(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderLocalInputRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspOrderLocalModify(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderLocalModifyRsp *info)
{

}


void TAP_CDECL TraderiTap::OnRspOrderLocalRemove(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderLocalRemoveRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspOrderLocalTransfer(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TapAPIOrderLocalTransferRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryAccount(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIUINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryAccountCashAdjust(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountCashAdjustQryRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryAccountFeeRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountFeeRentQryRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryAccountMarginRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountMarginRentQryRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryBill(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIBillQryRsp *info)
{
	
}


void TAP_CDECL TraderiTap::OnRspQryCommodity(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPICommodityInfo *rsp)
{
	if (rsp && rsp->CommodityType == TAPI_COMMODITY_TYPE_FUTURES)
	{
		m_setContracts[productO2I(rsp->CommodityNo)] = exchgO2I(rsp->ExchangeNo);
	}

	if (isLast == APIYNFLAG_YES)
	{
		//m_wrapperState = WS_COMMQRYED;

		m_wrapperState = WS_ALLREADY;
		m_sink->handleTraderLog(LL_INFO, "[TraderiTap-%s]账户数据初始化完成...", m_strUser.c_str());

		if (m_sink)
			m_sink->onLoginResult(true, "", 0);
	}
}


void TAP_CDECL TraderiTap::OnRspQryContract(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPITradeContractInfo *info)
{
	
}

void TAP_CDECL TraderiTap::OnRspRequestVertificateCode(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const TapAPIRequestVertificateCodeRsp *rsp)
{
	if(errCode != 0)
	{
		if (m_sink)
			m_sink->onLoginResult(false, "Requesting for sending second authcode failed: %d", errCode);
		return;
	}

	CHttpClient hClient;
	std::string strRes;
	int retCode = hClient.Get(m_strCodePath, strRes);
	if (retCode != 0)
	{
		m_wrapperState = WS_LOGINFAILED;

		if (m_sink)
			m_sink->onLoginResult(false, "Fetching second authcode failed", 0);

		if (m_bReconnect)
		{
			//这里丢到线程里去处理,让onclose可以马上返回
			StdThreadPtr thrd(new StdThread([this](){
				std::this_thread::sleep_for(std::chrono::seconds(2));
				m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
				reconnect();
			}));
		}
	}
	else
	{
		printf(strRes.c_str());
		StringVector ay = StrUtil::split(strRes, ",");
		int ec = atoi(ay[0].c_str());
		if (ec == 0)
		{

			TapAPISecondCertificationReq req = { 0 };
			req.LoginType = TAPI_LOGINTYPE_NORMAL;
			strcpy(req.VertificateCode, ay[1].c_str());
			m_pUserAPI->SetVertificateCode(makeRequestID(), &req);
			m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s submitted second authcode", m_strUser.c_str());
		}
		else
		{
			m_wrapperState = WS_LOGINFAILED;

			if (m_sink)
				m_sink->onLoginResult(false, StrUtil::printf("Fetching second authcode failed: %s", ay[1].c_str()).c_str(), 0);

			if (m_bReconnect)
			{
				//这里丢到线程里去处理,让onclose可以马上返回
				StdThreadPtr thrd(new StdThread([this](){
					std::this_thread::sleep_for(std::chrono::seconds(2));
					m_sink->handleTraderLog(LL_WARN, "[TraderiTap] %s reconnecting...", m_strUser.c_str());
					reconnect();
				}));
			}
		}
	}
}


void TAP_CDECL TraderiTap::OnRspSetReservedInfo(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errCode, const ITapTrade::TAPISTR_50 info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnContactInfo(ITapTrade::TAPIINT32 errCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TAPISTR_40 ContactInfo)
{
	if (errCode == 0 && isLast == APIYNFLAG_YES)
	{
		strcpy(m_strContract, ContactInfo);

		if (WS_DYNAUTH == m_wrapperState)
			m_pUserAPI->RequestVertificateCode(makeRequestID(), m_strContract);
	}
}


void TAP_CDECL TraderiTap::OnRtnContract(const ITapTrade::TapAPITradeContractInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnFill(const ITapTrade::TapAPIFillInfo *rsp)
{
	WTSTradeInfo *tRecord = makeTradeRecord(rsp);
	if (tRecord)
	{
		if (m_sink)
			m_sink->onPushTrade(tRecord);

		tRecord->release();
	}
}


void TAP_CDECL TraderiTap::OnRtnFund(const ITapTrade::TapAPIFundData *rsp)
{
	if (rsp == NULL)
		return;

	if (m_sink)
		m_sink->handleTraderLog(LL_INFO, "[TraderiTap] account data updated: %s", rsp->CurrencyNo);

	const char* currency = currencyO2I(rsp->CurrencyNo);

	WTSAccountInfo* accountInfo = WTSAccountInfo::create();
	accountInfo->setDescription(StrUtil::printf("%s", m_strUser.c_str()).c_str());
	accountInfo->setPreBalance((rsp->PreBalance));
	accountInfo->setCloseProfit((rsp->CloseProfit));
	accountInfo->setDynProfit((rsp->PositionProfit));
	accountInfo->setMargin((rsp->FrozenDeposit));
	accountInfo->setCommission((rsp->AccountFee));
	accountInfo->setFrozenMargin((rsp->FrozenDeposit));
	accountInfo->setFrozenCommission((rsp->FrozenFee));
	accountInfo->setDeposit((rsp->CashInValue));
	accountInfo->setWithdraw((rsp->CashOutValue));
	accountInfo->setBalance(accountInfo->getPreBalance() + accountInfo->getCloseProfit() - accountInfo->getCommission() + accountInfo->getDeposit() - accountInfo->getWithdraw());
	accountInfo->setAvailable(accountInfo->getBalance() - accountInfo->getFrozenCommission() - accountInfo->getFrozenMargin() - accountInfo->getMargin());
	accountInfo->setCurrency(currency);

	WTSArray* ay = WTSArray::create();
	ay->append(accountInfo, false);

	if (m_sink)
		m_sink->onRspAccount(ay);

	ay->release();
}


void TAP_CDECL TraderiTap::OnRtnOrder(const ITapTrade::TapAPIOrderInfoNotice *rsp)
{
	WTSOrderInfo *orderInfo = makeOrderInfo(rsp);
	if (orderInfo)
	{
		if (m_sink)
			m_sink->onPushOrder(orderInfo);

		orderInfo->release();
	}
}


void TAP_CDECL TraderiTap::OnRtnPosition(const ITapTrade::TapAPIPositionInfo *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnPositionProfit(const ITapTrade::TapAPIPositionProfitNotice *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnPositionSummary(const TapAPIPositionSummary *info)
{
	
}


void TAP_CDECL TraderiTap::OnRtnTradeMessage(const ITapTrade::TapAPITradeMessage *info)
{
	if(info)
	{
		if (m_sink)
			m_sink->handleTraderLog(LL_INFO, info->TMsgContent);
	}
}
