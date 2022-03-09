/*!
 * \file TraderFemas.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderFemas.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSContractInfo.hpp"

#include "../Share/ModuleHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"

#include <boost/filesystem.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	std::string s = std::move(fmt::sprintf(format, args...));
	strcpy(buffer, s.c_str());

	sink->handleTraderLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderFemas *instance = new TraderFemas();
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
};

inline uint32_t strToTime(const char* strTime)
{
	std::string str;
	const char *pos = strTime;
	while(strlen(pos) > 0)
	{
		if(pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	return strtoul(str.c_str(), NULL, 10);
}

TraderFemas::TraderFemas()
	: m_pUserAPI(NULL)
	, m_ayPosition(NULL)
	, m_ayOrders(NULL)
	, m_ayTrades(NULL)
	, m_ayPosDetail(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_uLastQryTime(0)
	, m_iRequestID(0)
	, m_bQuickStart(false)
	, m_bStopped(false)
	, m_bQryOnline(false)
	, m_uLocalOrdID(1)
	, m_bInQuery(false)
	, m_strandIO(NULL)
	, m_lastQryTime(0)
{
}


TraderFemas::~TraderFemas()
{
}

bool TraderFemas::init(WTSVariant* params)
{
	m_strFront = params->getCString("front");
	m_strBroker = params->getCString("broker");
	m_strUser = params->getCString("user");
	m_strPass = params->getCString("pass");

	m_strAppID = params->getCString("appid");
	m_strAuthCode = params->getCString("authcode");

	m_bQuickStart = params->getBoolean("quick");

	m_strQryFront = params->getCString("query");

	if (m_strQryFront.empty())
		m_bQryOnline = true;

	m_strFlowDir = params->getCString("flowdir");

	if (m_strFlowDir.empty())
		m_strFlowDir = "FemasTDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	WTSVariant* param = params->get("ctpmodule");
	if (param != NULL)
		m_strModule = getBinDir() + DLLHelper::wrap_module(param->asCString(), "");
	else
		m_strModule = DLLHelper::wrap_module("USTPtraderapiAF", "lib");

	m_hInstCTP = DLLHelper::load_library(m_strModule.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateFtdcTraderApi@CUstpFtdcTraderApi@@SAPEAV1@PEBD@Z";
#	else
	const char* creatorName = "?CreateFtdcTraderApi@CUstpFtdcTraderApi@@SAPAV1@PBD@Z";
#	endif	
#else
	const char* creatorName = "_ZN18CUstpFtdcTraderApi19CreateFtdcTraderApiEPKc";
#endif
	m_funcCreator = (APICreator)DLLHelper::get_symbol(m_hInstCTP, creatorName);

	return true;
}

void TraderFemas::release()
{
	m_bStopped = true;

	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	if (m_thrdWorker)
	{
		m_asyncIO.stop();
	}

	if(m_ayOrders)
		m_ayOrders->clear();

	if(m_ayPosDetail)
		m_ayPosDetail->clear();

	if(m_ayPosition)
		m_ayPosition->clear();

	if(m_ayTrades)
		m_ayTrades->clear();
}

void TraderFemas::connect()
{
	std::stringstream ss;
	ss << m_strFlowDir << "flows/" << m_strBroker << "/" << m_strUser << "/";
	std::string path = ss.str();
	boost::filesystem::create_directories(path.c_str());
	m_pUserAPI = m_funcCreator(path.c_str());
	m_pUserAPI->RegisterSpi(this);
	if(m_bQuickStart)
	{
		m_pUserAPI->SubscribePublicTopic(USTP_TERT_QUICK);				// 注册公有流
		m_pUserAPI->SubscribePrivateTopic(USTP_TERT_QUICK);				// 注册私有流
	}
	else
	{
		m_pUserAPI->SubscribePublicTopic(USTP_TERT_RESUME);				// 注册公有流
		m_pUserAPI->SubscribePrivateTopic(USTP_TERT_RESUME);			// 注册私有流
	}

	m_pUserAPI->RegisterFront((char*)m_strFront.c_str());
	m_pUserAPI->RegisterQryFront((char*)m_strQryFront.c_str());
	m_pUserAPI->SetHeartbeatTimeout(60);

	std::string logfile = path + "femas_req.log";
	m_pUserAPI->OpenRequestLog(logfile.c_str());
	logfile = path + "femas_res.log";
	m_pUserAPI->OpenResponseLog(logfile.c_str());

	m_pUserAPI->Init();

	if (m_thrdWorker == NULL)
	{
		m_strandIO = new boost::asio::io_service::strand(m_asyncIO);
		boost::asio::io_service::work work(m_asyncIO);
		m_thrdWorker.reset(new StdThread([this](){
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				m_asyncIO.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderFemas::disconnect()
{
	m_asyncIO.post([this](){
		release();
	});
}

bool TraderFemas::makeEntrustID(char* buffer, int length)
{
	if(buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t orderref = genLocalOrdID();
		sprintf(buffer, "%s%012d", m_strSessionID.c_str(), orderref);
		return true;
	}
	catch(...)
	{

	}

	return false;
}

void TraderFemas::registerSpi( ITraderSpi *listener )
{
	m_sink = listener;
	if(m_sink)
	{
		m_bdMgr = listener->getBaseDataMgr();
	}
}

uint32_t TraderFemas::genRequestID()
{
	return m_iRequestID.fetch_add(1) + 1;
}

uint32_t TraderFemas::genLocalOrdID()
{
	return m_uLocalOrdID.fetch_add(1) + 1;
}

int TraderFemas::authenticate()
{
	CUstpFtdcDSUserInfoField sDSUserInfo;
	strcpy(sDSUserInfo.AppID, m_strAppID.c_str());
	strcpy(sDSUserInfo.AuthCode, m_strAuthCode.c_str());
	sDSUserInfo.EncryptType = '1';
	m_pUserAPI->ReqDSUserCertification(&sDSUserInfo, genRequestID());

	return 0;
}

int TraderFemas::doLogin()
{
	CUstpFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	strcpy(req.Password, m_strPass.c_str());
	//strcpy(req.UserProductInfo, productInfo);
	int iResult = m_pUserAPI->ReqUserLogin(&req, genRequestID());
	if (iResult != 0)
	{
		write_log(m_sink,LL_ERROR, "[TraderFemas] Sending login request failed: %d", iResult);
	}

	return 0;
}

int TraderFemas::login( const char* user, const char* pass, const char* productInfo )
{
	m_strUser = user;
	m_strPass = pass;

	if(m_pUserAPI == NULL)
	{
		return -1;
	}

	m_wrapperState = WS_LOGINING;
	authenticate();

	return 0;
}

int TraderFemas::logout()
{
	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	CUstpFtdcReqUserLogoutField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	int iResult = m_pUserAPI->ReqUserLogout(&req, genRequestID());
	if (iResult != 0)
	{
		write_log(m_sink,LL_ERROR, "[TraderFemas] Sending logout request failed: %d", iResult);
	}

	return 0;
}

int TraderFemas::orderInsert(WTSEntrust* entrust)
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CUstpFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_strBroker.c_str());
	///投资者代码
	strcpy(req.UserID, m_strUser.c_str());
	strcpy(req.InvestorID, m_strUser.c_str());
	///合约代码
	strcpy(req.InstrumentID, entrust->getCode());

	WTSContractInfo* ct = entrust->getContractInfo();
	if (ct == NULL)
		return -1;

	strcpy(req.ExchangeID, wrapExchg(ct->getExchg()));

	if(strlen(entrust->getUserTag()) == 0)
	{
		///报单引用
		sprintf(req.UserOrderLocalID, "%s%012u", m_strSessionID.c_str(), genLocalOrdID());
	}
	else
	{
		strcpy(req.UserOrderLocalID, entrust->getUserTag());
	}
	//生成本地委托单号
	//entrust->setEntrustID(req.UserOrderLocalID);

	req.OrderPriceType = wrapPriceType(entrust->getPriceType(), strcmp(ct->getExchg(), "CFFEX") == 0);
	req.Direction = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.OffsetFlag = wrapOffsetType(entrust->getOffsetType());
	req.HedgeFlag = USTP_FTDC_CHF_Speculation;
	req.LimitPrice = entrust->getPrice();
	req.Volume = (int)entrust->getVolume();

	if (entrust->getOrderFlag() == WOF_NOR)
	{
		req.TimeCondition = USTP_FTDC_TC_GFD;
		req.VolumeCondition = USTP_FTDC_VC_AV;
	}
	else if (entrust->getOrderFlag() == WOF_FAK)
	{
		req.TimeCondition = USTP_FTDC_TC_IOC;
		req.VolumeCondition = USTP_FTDC_VC_AV;
	}
	else if (entrust->getOrderFlag() == WOF_FOK)
	{
		req.TimeCondition = USTP_FTDC_TC_IOC;
		req.VolumeCondition = USTP_FTDC_VC_CV;
	}

	req.ForceCloseReason = USTP_FTDC_FCR_NotForceClose;

	int iResult = m_pUserAPI->ReqOrderInsert(&req, genRequestID());
	if(iResult != 0)
	{
		write_log(m_sink,LL_ERROR, "[TraderFemas] Order inserting failed: %d", iResult);
	}

	return 0;
}

int TraderFemas::orderAction( WTSEntrustAction* action )
{
	if(m_wrapperState != WS_ALLREADY)
		return -1;

	//uint32_t frontid, sessionid, orderref;
	//if(!extractEntrustID(action->getEntrustID(), frontid, sessionid, orderref))
	//	return -1;

	CUstpFtdcOrderActionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.InvestorID, m_strUser.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	strcpy(req.UserOrderLocalID, action->getEntrustID());
	sprintf(req.UserOrderActionLocalID, "%s%012u", m_strSessionID.c_str(), genLocalOrdID());
	req.ActionFlag = wrapActionFlag(action->getActionFlag());
	req.LimitPrice = action->getPrice();

	req.VolumeChange = (int32_t)action->getVolume();

	strcpy(req.OrderSysID, action->getOrderID());
	strcpy(req.ExchangeID, wrapExchg(action->getExchg()));

	int iResult = m_pUserAPI->ReqOrderAction(&req, genRequestID());
	if(iResult != 0)
	{
		write_log(m_sink,LL_ERROR, "[TraderFemas] Sending cancel request failed: %d", iResult);
	}

	return 0;	
}

int TraderFemas::queryAccount()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CUstpFtdcQryInvestorAccountField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());
		strcpy(req.UserID, m_strUser.c_str());
		m_pUserAPI->ReqQryInvestorAccount(&req, genRequestID());
	});

	triggerQuery();

	return 0;
}

int TraderFemas::queryPositions()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CUstpFtdcQryInvestorPositionField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.UserID, m_strUser.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());
		m_pUserAPI->ReqQryInvestorPosition(&req, genRequestID());
	});

	return 0;
}

int TraderFemas::queryOrders()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CUstpFtdcQryOrderField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.UserID, m_strUser.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());

		m_pUserAPI->ReqQryOrder(&req, genRequestID());
	});

	return 0;
}

int TraderFemas::queryTrades()
{
	if(m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CUstpFtdcQryTradeField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.UserID, m_strUser.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());

		m_pUserAPI->ReqQryTrade(&req, genRequestID());
	});

	return 0;
}

void TraderFemas::OnFrontConnected()
{
	if(m_sink)
		m_sink->handleEvent(WTE_Connect, 0);
}

void TraderFemas::OnQryFrontConnected()
{
	//m_bQryOnline = true;

	//if (m_wrapperState == WS_ALLREADY)
	//{
	//	if (m_sink)
	//		m_sink->onLoginResult(true, "", m_lDate);
	//}
}

void TraderFemas::OnFrontDisconnected( int nReason )
{
	m_bQryOnline = false;
	m_wrapperState = WS_NOTLOGIN;
	if(m_sink)
		m_sink->handleEvent(WTE_Close, nReason);
}

void TraderFemas::OnQryFrontDisconnected(int nReason)
{
	m_bQryOnline = false;
}

void TraderFemas::OnHeartBeatWarning( int nTimeLapse )
{
	write_log(m_sink,LL_DEBUG, "[TraderFemas][%s-%s] Heartbeating...", m_strBroker.c_str(), m_strUser.c_str());
}

void TraderFemas::OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_bQryOnline = true;

	if (m_wrapperState == WS_LOGINED)
		onInitialized();
}

void TraderFemas::OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID == 0)
	{
		doLogin();
	}
	else
	{
		write_log(m_sink,LL_ERROR, "[TraderFemas][%s-%s] Authentiation failed: %s", m_strBroker.c_str(), m_strUser.c_str(), pRspInfo->ErrorMsg);
		m_wrapperState = WS_LOGINFAILED;

		if (m_sink)
			m_sink->onLoginResult(false, pRspInfo->ErrorMsg, 0);
	}
}

void TraderFemas::OnRspUserLogin( CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		m_wrapperState = WS_LOGINED;

		// 保存会话参数
		m_strSessionID = pRspUserLogin->MaxOrderLocalID;
		StrUtil::trim(m_strSessionID, "0", false, true);
		
		///获取当前交易日
		m_lDate = atoi(m_pUserAPI->GetTradingDay());

		write_log(m_sink,LL_INFO,"[TraderFemas][%s-%s] Login succeed...", m_strBroker.c_str(), m_strUser.c_str());

		//据说飞马不支持结算,所以查不到结算单
		write_log(m_sink,LL_INFO, "[TraderFemas][%s-%s] Querying confirming state of settlement data...", m_strBroker.c_str(), m_strUser.c_str());
		if (m_bQryOnline)
			onInitialized();
	}
	else
	{
		write_log(m_sink,LL_ERROR,"[TraderFemas][%s-%s] Login failed: %s", m_strBroker.c_str(), m_strUser.c_str(), pRspInfo->ErrorMsg);
		m_wrapperState = WS_LOGINFAILED;

		if(m_sink)
			m_sink->onLoginResult( false, pRspInfo->ErrorMsg, 0);
	}
}

void TraderFemas::OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_wrapperState = WS_NOTLOGIN;
	if(m_sink)
		m_sink->handleEvent(WTE_Logout, 0);
}

void TraderFemas::OnRspOrderInsert( CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	WTSEntrust* entrust = makeEntrust(pInputOrder);
	if(entrust)
	{
		WTSError *err = makeError(pRspInfo);
		//g_orderMgr.onRspEntrust(entrust, err);
		if(m_sink)
			m_sink->onRspEntrust(entrust, err);
		entrust->release();

		if(err)
			err->release();
	}
}

void TraderFemas::OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{

	}
	else
	{
		WTSError* error = WTSError::create(WEC_ORDERCANCEL, pRspInfo->ErrorMsg);
		if(m_sink)
			m_sink->onTraderError(error);
	}
}

void TraderFemas::OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setDescription(StrUtil::printf("%s-%s", m_strBroker.c_str(), m_strUser.c_str()).c_str());
		//accountInfo->setUsername(m_strUserName.c_str());
		accountInfo->setPreBalance(pRspInvestorAccount->PreBalance);
		accountInfo->setCloseProfit(pRspInvestorAccount->CloseProfit);
		accountInfo->setDynProfit(pRspInvestorAccount->PositionProfit);
		accountInfo->setMargin(pRspInvestorAccount->Margin);
		accountInfo->setAvailable(pRspInvestorAccount->Available);
		accountInfo->setCommission(pRspInvestorAccount->Fee);
		accountInfo->setFrozenMargin(pRspInvestorAccount->FrozenMargin);
		accountInfo->setFrozenCommission(pRspInvestorAccount->FrozenFee);
		accountInfo->setDeposit(pRspInvestorAccount->Deposit);
		accountInfo->setWithdraw(pRspInvestorAccount->Withdraw);
		accountInfo->setBalance(accountInfo->getPreBalance() + accountInfo->getCloseProfit() - accountInfo->getCommission() + accountInfo->getDeposit() - accountInfo->getWithdraw());
		accountInfo->setCurrency("CNY");

		WTSArray * ay = WTSArray::create();
		ay->append(accountInfo, false);
		if(m_sink)
			m_sink->onRspAccount(ay);

		ay->release();
	}
}

void TraderFemas::OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (!IsErrorRspInfo(pRspInfo) && pRspInvestorPosition)
	{
		if(NULL == m_ayPosition)
			m_ayPosition = WTSArray::create();

		WTSContractInfo* contract = m_bdMgr->getContract(pRspInvestorPosition->InstrumentID, pRspInvestorPosition->ExchangeID);
		if (contract)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			WTSPositionItem *pos = WTSPositionItem::create(pRspInvestorPosition->InstrumentID, commInfo->getCurrency(), commInfo->getExchg());
			pos->setContractInfo(contract);
			pos->setDirection(wrapPosDirection(pRspInvestorPosition->Direction));
			pos->setNewPosition(pRspInvestorPosition->Position - pRspInvestorPosition->YdPosition);
			pos->setPrePosition(pRspInvestorPosition->YdPosition);

			pos->setMargin(pRspInvestorPosition->UsedMargin);
			pos->setDynProfit(0.0);
			pos->setPositionCost(pRspInvestorPosition->PositionCost);

			if (pos->getTotalPosition() != 0)
			{
				pos->setAvgPrice(pos->getPositionCost() / pos->getTotalPosition() / commInfo->getVolScale());
			}
			else
			{
				pos->setAvgPrice(0);
			}

			if (commInfo->getCategoty() != CC_Combination)
			{
				int availNew = pRspInvestorPosition->Position - pRspInvestorPosition->YdPosition;
				availNew -= pRspInvestorPosition->FrozenPosition;

				if (availNew < 0)
					availNew = 0;
				pos->setAvailNewPos(availNew);

				double availPre = pos->getNewPosition() + pos->getPrePosition()
					- pRspInvestorPosition->FrozenPosition - pos->getAvailNewPos();
				pos->setAvailPrePos(availPre);
			}
			else
			{

			}

			//if (pos->getTotalPosition() > 0 && pos->getMargin() == 0)
			if (decimal::lt(pos->getTotalPosition(), 0.0) && decimal::eq(pos->getMargin(), 0.0))
			{
				//有仓位,但是保证金为0,则说明是套利合约,单个合约的可用持仓全部置为0
				pos->setAvailNewPos(0);
				pos->setAvailPrePos(0);
			}
		}
	}
	
	if(bIsLast)
	{
		if(m_sink)
			m_sink->onRspPosition(m_ayPosition);

		if(m_ayPosition)
		{
			m_ayPosition->release();
			m_ayPosition = NULL;
		}
	}
}

void TraderFemas::OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if(!IsErrorRspInfo(pRspInfo) && pTrade)
	{
		if(NULL == m_ayTrades)
			m_ayTrades = WTSArray::create();

		WTSTradeInfo* trade = makeTradeRecord(pTrade);
		if(trade)
		{
			m_ayTrades->append(trade, false);
		}
	}

	if(bIsLast)
	{
		if(m_sink)
			m_sink->onRspTrades(m_ayTrades);

		if(NULL != m_ayTrades)
			m_ayTrades->clear();
	}
}

void TraderFemas::OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if(!IsErrorRspInfo(pRspInfo) && pOrder)
	{
		if(NULL == m_ayOrders)
			m_ayOrders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(pOrder);
		if(orderInfo)
		{
			m_ayOrders->append(orderInfo, false);
		}
	}

	if(bIsLast)
	{
		if(m_sink)
			m_sink->onRspOrders(m_ayOrders);

		if(m_ayOrders)
			m_ayOrders->clear();
	}
}

void TraderFemas::onInitialized()
{
	write_log(m_sink,LL_INFO, "[TraderFemas][%s-%s] Trading channel initialized...", m_strBroker.c_str(), m_strUser.c_str());
	m_wrapperState = WS_ALLREADY;
	if (m_sink)
		m_sink->onLoginResult(true, "", m_lDate);
}

void TraderFemas::OnRspError( CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if (m_sink)
		write_log(m_sink,LL_ERROR, "[TraderFemas][TraderFemas] Error occured: %s, request id: %d", pRspInfo->ErrorMsg, nRequestID);
}

void TraderFemas::OnRtnOrder( CUstpFtdcOrderField *pOrder )
{
	WTSOrderInfo *orderInfo = makeOrderInfo(pOrder);
	if(orderInfo)
	{
		if(m_sink)
			m_sink->onPushOrder(orderInfo);

		orderInfo->release();
	}
}

void TraderFemas::OnRtnTrade( CUstpFtdcTradeField *pTrade )
{
	WTSTradeInfo *tRecord = makeTradeRecord(pTrade);
	if(tRecord)
	{
		if(m_sink)
			m_sink->onPushTrade(tRecord);

		tRecord->release();
	}
}

int TraderFemas::wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if( WDT_LONG == dirType)
		if(offsetType == WOT_OPEN)
			return USTP_FTDC_D_Buy;
		else
			return USTP_FTDC_D_Sell;
	else
		if(offsetType == WOT_OPEN)
			return USTP_FTDC_D_Sell;
		else
			return USTP_FTDC_D_Buy;
}

WTSDirectionType TraderFemas::wrapDirectionType(TUstpFtdcDirectionType dirType, TUstpFtdcOffsetFlagType offsetType)
{
	if( USTP_FTDC_D_Buy == dirType)
		if(offsetType == USTP_FTDC_OF_Open)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if(offsetType == USTP_FTDC_OF_Open)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

WTSDirectionType TraderFemas::wrapPosDirection(TUstpFtdcDirectionType dirType)
{
	if (USTP_FTDC_D_Buy == dirType)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

int TraderFemas::wrapOffsetType(WTSOffsetType offType)
{
	if( WOT_OPEN == offType)
		return USTP_FTDC_OF_Open;
	else if( WOT_CLOSE == offType)
		return USTP_FTDC_OF_Close;
	else if( WOT_CLOSETODAY == offType)
		return USTP_FTDC_OF_CloseToday;
	else if( WOT_CLOSEYESTERDAY == offType)
		return USTP_FTDC_OF_Close;
	else
		return USTP_FTDC_OF_ForceClose;
}

WTSOffsetType TraderFemas::wrapOffsetType(TUstpFtdcOffsetFlagType offType)
{
	if( USTP_FTDC_OF_Open == offType)
		return WOT_OPEN;
	else if( USTP_FTDC_OF_Close == offType)
		return WOT_CLOSE;
	else if( USTP_FTDC_OF_CloseToday == offType)
		return WOT_CLOSETODAY;
	else
		return WOT_FORCECLOSE;
}

int TraderFemas::wrapPriceType(WTSPriceType priceType, bool isCFFEX /* = false */)
{
	if(WPT_ANYPRICE == priceType)
		return isCFFEX ? USTP_FTDC_OPT_BestPrice : USTP_FTDC_OPT_AnyPrice;
	else if(WPT_LIMITPRICE == priceType)
		return USTP_FTDC_OPT_LimitPrice;
	else if(WPT_BESTPRICE == priceType)
		return USTP_FTDC_OPT_BestPrice;
	else
		return USTP_FTDC_OPT_BestPrice;
}

WTSPriceType TraderFemas::wrapPriceType(TUstpFtdcPriceType priceType)
{
	if (USTP_FTDC_OPT_AnyPrice == priceType || USTP_FTDC_OPT_BestPrice == priceType)
		return WPT_ANYPRICE;
	else if(USTP_FTDC_OPT_LimitPrice == priceType)
		return WPT_LIMITPRICE;
	else
		return WPT_LASTPRICE;
}

int TraderFemas::wrapTimeCondition(WTSTimeCondition timeCond)
{
	if(WTC_IOC == timeCond)
		return USTP_FTDC_TC_IOC;
	else if(WTC_GFD == timeCond)
		return USTP_FTDC_TC_GFD;
	else
		return USTP_FTDC_TC_GFS;
}

WTSTimeCondition TraderFemas::wrapTimeCondition(TUstpFtdcTimeConditionType timeCond)
{
	if(USTP_FTDC_TC_IOC == timeCond)
		return WTC_IOC;
	else if(USTP_FTDC_TC_GFD == timeCond)
		return WTC_GFD;
	else
		return WTC_GFS;
}

WTSOrderState TraderFemas::wrapOrderState(TUstpFtdcOrderStatusType orderState)
{
	if (orderState < USTP_FTDC_OS_AcceptedNoReply)
		return (WTSOrderState)orderState;
	else
		return WOS_Submitting;
}

int TraderFemas::wrapActionFlag(WTSActionFlag actionFlag)
{
	if(WAF_CANCEL == actionFlag)
		return USTP_FTDC_AF_Delete;
	else
		return USTP_FTDC_AF_Modify;
}


WTSOrderInfo* TraderFemas::makeOrderInfo(CUstpFtdcOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->InstrumentID, orderField->ExchangeID);
	if(contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(orderField->LimitPrice);
	pRet->setVolume(orderField->Volume);
	pRet->setDirection(wrapDirectionType(orderField->Direction, orderField->OffsetFlag));
	pRet->setPriceType(wrapPriceType(orderField->OrderPriceType));
	pRet->setOffsetType(wrapOffsetType(orderField->OffsetFlag));
	pRet->setContractInfo(contract);

	if (orderField->TimeCondition == USTP_FTDC_TC_GFD)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (orderField->TimeCondition == USTP_FTDC_TC_IOC)
	{
		if (orderField->VolumeCondition == USTP_FTDC_VC_AV || orderField->VolumeCondition == USTP_FTDC_VC_MV)
			pRet->setOrderFlag(WOF_FAK);
		else
			pRet->setOrderFlag(WOF_FOK);
	}

	pRet->setVolTraded(orderField->VolumeTraded);
	pRet->setVolLeft(orderField->VolumeRemain);

	pRet->setCode(orderField->InstrumentID);

	pRet->setOrderDate(strtoul(orderField->TradingDay, NULL, 10));
	std::string strTime = orderField->InsertTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10)*1000));
	pRet->setOrderState(wrapOrderState(orderField->OrderStatus));

	if (atoi(orderField->UserOrderLocalID) != 0)
		pRet->setEntrustID(orderField->UserOrderLocalID);
	else
		pRet->setEntrustID(orderField->OrderLocalID);
	pRet->setOrderID(orderField->OrderSysID);

	//pRet->setStateMsg(orderField->StatusMsg);

	pRet->setUserTag(orderField->UserOrderLocalID);

	return pRet;
}

WTSEntrust* TraderFemas::makeEntrust(CUstpFtdcInputOrderField *entrustField)
{
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->InstrumentID, entrustField->ExchangeID);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create( 
		entrustField->InstrumentID, 
		entrustField->Volume, 
		entrustField->LimitPrice);

	pRet->setContractInfo(ct);

	pRet->setDirection(wrapDirectionType(entrustField->Direction, entrustField->OffsetFlag));
	pRet->setPriceType(wrapPriceType(entrustField->OrderPriceType));
	pRet->setOffsetType(wrapOffsetType(entrustField->OffsetFlag));
	
	if (entrustField->TimeCondition == USTP_FTDC_TC_GFD)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (entrustField->TimeCondition == USTP_FTDC_TC_IOC)
	{
		if (entrustField->VolumeCondition == USTP_FTDC_VC_AV || entrustField->VolumeCondition == USTP_FTDC_VC_MV)
			pRet->setOrderFlag(WOF_FAK);
		else
			pRet->setOrderFlag(WOF_FOK);
	}

	pRet->setEntrustID(entrustField->UserOrderLocalID);
	pRet->setUserTag(entrustField->UserOrderLocalID);

	return pRet;
}

WTSError* TraderFemas::makeError(CUstpFtdcRspInfoField* rspInfo)
{
	if (rspInfo == NULL || rspInfo->ErrorID == 0)
		return NULL;

	WTSError* pRet = WTSError::create((WTSErroCode)rspInfo->ErrorID, rspInfo->ErrorMsg);
	return pRet;
}

WTSTradeInfo* TraderFemas::makeTradeRecord(CUstpFtdcTradeField *tradeField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->InstrumentID, tradeField->ExchangeID);
	if(contract == NULL)
		return NULL;

	WTSCommodityInfo* mInfo = contract->getCommInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(tradeField->InstrumentID);
	pRet->setVolume(tradeField->TradeVolume);
	pRet->setPrice(tradeField->TradePrice);
	pRet->setTradeID(tradeField->TradeID);
	pRet->setContractInfo(contract);

	std::string strTime = tradeField->TradeTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	uint32_t uDate = strtoul(tradeField->TradingDay, NULL, 10);
	//if(uDate == m_pContractMgr->getTradingDate())
	//{
	//	//如果当前日期和交易日一致,且时间大于21点,说明是夜盘,也就是实际日期要单独计算
	//	if (uTime / 10000 >= 21)
	//	{
	//		uDate = m_pMarketMgr->getPrevTDate(mInfo->getExchange(), uDate, 1);
	//	}
	//	else if(uTime <= 3)
	//	{
	//		//如果在3点以内,就要先获取上一个交易日,再获取下一个自然日
	//		//这样做的目的是,遇到周五晚上的情况,可以处理过来
	//		uDate = m_pMarketMgr->getPrevTDate(mInfo->getExchange(), uDate, 1);
	//		uDate = TimeUtils::getNextDate(uDate);
	//	}
	//}

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime*1000));

	WTSDirectionType dType = wrapDirectionType(tradeField->Direction, tradeField->OffsetFlag);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(tradeField->OffsetFlag));
	pRet->setRefOrder(tradeField->OrderSysID);
	pRet->setTradeType(WTT_Common);

	double amount = mInfo->getVolScale()*tradeField->TradeVolume*pRet->getPrice();
	pRet->setAmount(amount);
	
	StringMap::iterator it = m_mapOrderTag.find(pRet->getRefOrder());
	if(it != m_mapOrderTag.end())
	{
		pRet->setUserTag(it->second.c_str());
	}

	return pRet;
}

bool TraderFemas::IsErrorRspInfo(CUstpFtdcRspInfoField *pRspInfo)
{
	if(pRspInfo && pRspInfo->ErrorID!=0)
		return true;

	return false;
}

void TraderFemas::OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo)
{

}

void TraderFemas::OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo)
{

}

bool TraderFemas::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}

const char* TraderFemas::wrapExchg(const char* exchg)
{
	if (strcmp(exchg, "CZCE") == 0)
		return "ZCE";
	else
		return exchg;
}

void TraderFemas::triggerQuery()
{
	m_strandIO->post([this](){
		if (m_queQuery.empty() || m_bInQuery)
			return;

		uint64_t curTime = TimeUtils::getLocalTimeNow();
		if (curTime - m_lastQryTime < 1000)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			m_strandIO->post([this](){
				triggerQuery();
			});
			return;
		}


		m_bInQuery = true;
		CommonExecuter& handler = m_queQuery.front();
		handler();

		{
			StdUniqueLock lock(m_mtxQuery);
			m_queQuery.pop();
		}

		m_lastQryTime = TimeUtils::getLocalTimeNow();
	});
}