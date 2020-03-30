#include "TraderCTPMini.h"
#include "..\Share\WTSError.hpp"
#include "..\Share\WTSContractInfo.hpp"
#include "..\Share\WTSSessionInfo.hpp"
#include "..\Share\WTSTradeDef.hpp"
#include "..\Share\WTSDataDef.hpp"
#include "..\Share\WTSParams.hpp"
#include "..\Share\StdUtils.hpp"
#include "..\Share\TimeUtils.hpp"
#include "..\Share\IBaseDataMgr.h"


#ifdef _WIN64
#pragma comment(lib, "./ThostTraderApi/thosttraderapi.lib")
#else
#pragma comment(lib, "./ThostTraderApi/thosttraderapi32.lib")
#endif

uint32_t strToTime(const char* strTime)
{
	std::string str;
	const char *pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	return strtoul(str.c_str(), NULL, 10);
}

ITraderApi* createTrader()
{
	TraderCTPMini *instance = new TraderCTPMini();
	return instance;
}

void deleteTrader(ITraderApi* &trader)
{
	if (NULL != trader)
	{
		delete trader;
		trader = NULL;
	}
}

TraderCTPMini::TraderCTPMini()
	: m_pUserAPI(NULL)
	, m_ayPosition(NULL)
	, m_ayOrders(NULL)
	, m_ayTrades(NULL)
	, m_ayPosDetail(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_uLastQryTime(0)
	, m_iRequestID(0)
	, m_bQuickStart(false)
	, m_hInstCTP(NULL)
	, m_bInQuery(false)
	, m_strandIO(NULL)
	, m_lastQryTime(0)
{
}


TraderCTPMini::~TraderCTPMini()
{
}

bool TraderCTPMini::init(WTSParams* params)
{
	m_strFront = params->get("front")->asCString();
	m_strBroker = params->get("broker")->asCString();
	m_strUser = params->get("user")->asCString();
	m_strPass = params->get("pass")->asCString();

	m_strAppID = params->getCString("appid");
	m_strAuthCode = params->getCString("authcode");

	WTSParams* param = params->get("module");
	if (param != NULL)
		m_strModule = param->asCString();
	else
		m_strModule = "thosttraderapi_se.dll";

	m_hInstCTP = LoadLibrary(m_strModule.c_str());
	if (m_hInstCTP == NULL)
		return false;

	m_funcCreator = (CTPCreator)GetProcAddress(m_hInstCTP, "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPAV1@PBD@Z");

	m_bQuickStart = params->getBoolean("quick");

	return true;
}

void TraderCTPMini::release()
{
	if (m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	if (m_ayOrders)
		m_ayOrders->clear();

	if (m_ayPosDetail)
		m_ayPosDetail->clear();

	if (m_ayPosition)
		m_ayPosition->clear();

	if (m_ayTrades)
		m_ayTrades->clear();
}

void TraderCTPMini::connect()
{
	std::string path = m_strBroker;
	path += "/";
	path += m_strUser;
	path += "/";
	StdFile::create_directories(path.c_str());
	m_pUserAPI = m_funcCreator(path.c_str());
	m_pUserAPI->RegisterSpi(this);
	if (m_bQuickStart)
	{
		m_pUserAPI->SubscribePublicTopic(THOST_TERT_QUICK);			// 注册公有流
		m_pUserAPI->SubscribePrivateTopic(THOST_TERT_QUICK);		// 注册私有流
	}
	else
	{
		m_pUserAPI->SubscribePublicTopic(THOST_TERT_RESUME);		// 注册公有流
		m_pUserAPI->SubscribePrivateTopic(THOST_TERT_RESUME);		// 注册私有流
	}

	m_pUserAPI->RegisterFront((char*)m_strFront.c_str());

	if (m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	if (m_thrdWorker == NULL)
	{
		m_strandIO = new boost::asio::io_service::strand(m_asyncIO);
		boost::asio::io_service::work work(m_asyncIO);
		m_thrdWorker.reset(new BoostThread([this](){
			while (true)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(2));
				m_asyncIO.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderCTPMini::disconnect()
{
	m_asyncIO.post([this](){
		release();
	});

	if (m_thrdWorker)
	{
		m_asyncIO.stop();
		m_thrdWorker->join();
		m_thrdWorker = NULL;

		delete m_strandIO;
		m_strandIO = NULL;
	}
}

bool TraderCTPMini::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t orderref = InterlockedIncrement(&m_orderRef);
		sprintf(buffer, "%06u.%010u.%06u", m_frontID, m_sessionID, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderCTPMini::registerListener(ITraderApiListener *listener)
{
	m_traderSink = listener;
	if (m_traderSink)
	{
		m_bdMgr = listener->getBaseDataMgr();
	}
}

uint32_t TraderCTPMini::genRequestID()
{
	return m_iRequestID.fetch_add(1) + 1;
}

int TraderCTPMini::login(const char* user, const char* pass, const char* productInfo)
{
	m_strUser = user;
	m_strPass = pass;

	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	m_wrapperState = WS_LOGINING;
	authenticate();

	return 0;
}

int TraderCTPMini::doLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	strcpy(req.Password, m_strPass.c_str());
	strcpy(req.UserProductInfo, m_strProdInfo.c_str());
	int iResult = m_pUserAPI->ReqUserLogin(&req, genRequestID());
	if (iResult != 0)
	{
		m_traderSink->handleTraderLog(LL_ERROR, "[CTPTrader]登录请求发送失败, 错误码:%d", iResult);
	}

	return 0;
}

int TraderCTPMini::logout()
{
	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	CThostFtdcUserLogoutField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	int iResult = m_pUserAPI->ReqUserLogout(&req, genRequestID());
	if (iResult != 0)
	{
		m_traderSink->handleTraderLog(LL_ERROR, "[CTPTrader]注销请求发送失败, 错误码:%d", iResult);
	}

	return 0;
}

int TraderCTPMini::orderInsert(WTSEntrust* entrust)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_strBroker.c_str());
	///投资者代码
	strcpy(req.InvestorID, m_strUser.c_str());
	///合约代码
	strcpy(req.InstrumentID, entrust->getCode());

	if (strlen(entrust->getUserTag()) == 0)
	{
		///报单引用
		sprintf(req.OrderRef, "%u", m_orderRef);

		//生成本地委托单号
		//entrust->setEntrustID(generateEntrustID(m_frontID, m_sessionID, m_orderRef++).c_str());	
	}
	else
	{
		uint32_t fid, sid, orderref;
		extractEntrustID(entrust->getUserTag(), fid, sid, orderref);
		//entrust->setEntrustID(entrust->getUserTag());
		///报单引用
		sprintf(req.OrderRef, "%d", orderref);
	}

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_mapEntrustTag[entrust->getEntrustID()] = entrust->getUserTag();
	}

	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = wrapPriceType(entrust->getPriceType());
	///买卖方向: 
	req.Direction = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = wrapOffsetType(entrust->getOffsetType());
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = entrust->getPrice();
	///数量: 1
	req.VolumeTotalOriginal = (int)entrust->getVolumn();
	///有效期类型: 当日有效
	req.TimeCondition = wrapTimeCondition(entrust->getTimeCondition());
	///GTD日期
	//	TThostFtdcDateType	GTDDate;
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///止损价
	//	TThostFtdcPriceType	StopPrice;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///业务单元
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	//	TThostFtdcRequestIDType	RequestID;
	///用户强评标志: 否
	req.UserForceClose = 0;

	int iResult = m_pUserAPI->ReqOrderInsert(&req, genRequestID());
	if (iResult != 0)
	{
		m_traderSink->handleTraderLog(LL_ERROR, "[CTPTrader]插入订单失败, 错误码:%d", iResult);
	}

	return 0;
}

int TraderCTPMini::orderAction(WTSEntrustAction* action)
{
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	uint32_t frontid, sessionid, orderref;
	if (!extractEntrustID(action->getEntrustID(), frontid, sessionid, orderref))
		return -1;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_strBroker.c_str());
	///投资者代码
	strcpy(req.InvestorID, m_strUser.c_str());
	///报单引用
	sprintf(req.OrderRef, "%u", orderref);
	///请求编号
	///前置编号
	req.FrontID = frontid;
	///会话编号
	req.SessionID = sessionid;
	///操作标志
	req.ActionFlag = wrapActionFlag(action->getActionFlag());
	///合约代码
	strcpy(req.InstrumentID, action->getCode());

	req.LimitPrice = action->getPrice();

	req.VolumeChange = action->getVolumn();

	strcpy(req.OrderSysID, action->getOrderID());
	strcpy(req.ExchangeID, action->getExchg());

	int iResult = m_pUserAPI->ReqOrderAction(&req, genRequestID());
	if (iResult != 0)
	{
		m_traderSink->handleTraderLog(LL_ERROR, "[CTPTrader]撤单请求发送失败, 错误码:%d", iResult);
	}

	return 0;
}

int TraderCTPMini::queryAccount()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	BoostUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CThostFtdcQryTradingAccountField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());
		m_pUserAPI->ReqQryTradingAccount(&req, genRequestID());
	});

	triggerQuery();

	return 0;
}

int TraderCTPMini::queryPositions()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	BoostUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CThostFtdcQryInvestorPositionField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());
		m_pUserAPI->ReqQryInvestorPosition(&req, genRequestID());
	});

	triggerQuery();

	return 0;
}

int TraderCTPMini::queryOrders()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	BoostUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CThostFtdcQryOrderField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());

		m_pUserAPI->ReqQryOrder(&req, genRequestID());
	});

	triggerQuery();

	return 0;
}

int TraderCTPMini::queryTrades()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	BoostUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CThostFtdcQryTradeField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());

		m_pUserAPI->ReqQryTrade(&req, genRequestID());
	});

	triggerQuery();

	return 0;
}

void TraderCTPMini::OnFrontConnected()
{
	if (m_traderSink)
		m_traderSink->handleEvent(WTE_Connect, 0);
}

void TraderCTPMini::OnFrontDisconnected(int nReason)
{
	//m_traderSink->handleTraderLog(LL_ERROR, "[CTPTrader]CTP交易服务器已断开");
	m_wrapperState = WS_NOTLOGIN;
	if (m_traderSink)
		m_traderSink->handleEvent(WTE_Close, nReason);
}

void TraderCTPMini::OnHeartBeatWarning(int nTimeLapse)
{

}

void TraderCTPMini::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo))
	{
		doLogin();
	}
	else
	{
		m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]终端认证失败,错误信息:%s", m_strBroker.c_str(), m_strUser.c_str(), pRspInfo->ErrorMsg);
		m_wrapperState = WS_LOGINFAILED;

		if (m_traderSink)
			m_traderSink->onLoginResult(false, pRspInfo->ErrorMsg, 0);
	}

}

void TraderCTPMini::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo))
	{
		m_wrapperState = WS_LOGINED;

		// 保存会话参数
		m_frontID = pRspUserLogin->FrontID;
		m_sessionID = pRspUserLogin->SessionID;
		m_orderRef = atoi(pRspUserLogin->MaxOrderRef);
		///获取当前交易日
		m_lDate = atoi(m_pUserAPI->GetTradingDay());

		m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]账户登录成功……", m_strBroker.c_str(), m_strUser.c_str());

		m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]正在查询结算确认信息……", m_strBroker.c_str(), m_strUser.c_str());
		queryConfirm();
	}
	else
	{
		m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]账户登录失败,错误信息:%s", m_strBroker.c_str(), m_strUser.c_str(), pRspInfo->ErrorMsg);
		m_wrapperState = WS_LOGINFAILED;

		if (m_traderSink)
			m_traderSink->onLoginResult(false, pRspInfo->ErrorMsg, 0);
	}
}

void TraderCTPMini::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_wrapperState = WS_NOTLOGIN;
	if (m_traderSink)
		m_traderSink->handleEvent(WTE_Logout, 0);
}

void TraderCTPMini::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (!IsErrorRspInfo(pRspInfo))
	{
		if (pSettlementInfoConfirm != NULL)
		{
			uint32_t uConfirmDate = strtoul(pSettlementInfoConfirm->ConfirmDate, NULL, 10);
			if (uConfirmDate >= m_lDate)
			{
				m_wrapperState = WS_CONFIRMED;

				m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]账户数据初始化完成……", m_strBroker.c_str(), m_strUser.c_str());
				m_wrapperState = WS_ALLREADY;
				if (m_traderSink)
					m_traderSink->onLoginResult(true, "", m_lDate);
			}
			else
			{
				m_wrapperState = WS_CONFIRM_QRYED;

				m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]正在确认结算结果……", m_strBroker.c_str(), m_strUser.c_str());
				confirm();
			}
		}
		else
		{
			m_wrapperState = WS_CONFIRM_QRYED;
			confirm();
		}
	}

}

void TraderCTPMini::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm != NULL)
	{
		if (m_wrapperState == WS_CONFIRM_QRYED)
		{
			m_wrapperState = WS_CONFIRMED;

			m_traderSink->handleTraderLog(LL_INFO, "[%s-%s]账户数据初始化完成……", m_strBroker.c_str(), m_strUser.c_str());
			m_wrapperState = WS_ALLREADY;
			if (m_traderSink)
				m_traderSink->onLoginResult(true, "", m_lDate);
		}
	}
}

void TraderCTPMini::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	WTSEntrust* entrust = makeEntrust(pInputOrder);
	if (entrust)
	{
		WTSError *err = makeError(pRspInfo);
		//g_orderMgr.onRspEntrust(entrust, err);
		if (m_traderSink)
			m_traderSink->onRspEntrust(entrust, err);
		entrust->release();
		err->release();
	}
}

void TraderCTPMini::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (IsErrorRspInfo(pRspInfo))
	{

	}
	else
	{
		WTSError* error = WTSError::create(WEC_ORDERCANCEL, pRspInfo->ErrorMsg);
		if (m_traderSink)
			m_traderSink->onTraderError(error);
	}
}

void TraderCTPMini::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
		accountInfo->setPreBalance(pTradingAccount->PreBalance);
		accountInfo->setCloseProfit(pTradingAccount->CloseProfit);
		accountInfo->setDynProfit(pTradingAccount->PositionProfit);
		accountInfo->setMargin(pTradingAccount->CurrMargin);
		accountInfo->setAvailable(pTradingAccount->Available);
		accountInfo->setCommission(pTradingAccount->Commission);
		accountInfo->setFrozenMargin(pTradingAccount->FrozenMargin);
		accountInfo->setFrozenCommission(pTradingAccount->FrozenCommission);
		accountInfo->setDeposit(pTradingAccount->Deposit);
		accountInfo->setWithdraw(pTradingAccount->Withdraw);
		accountInfo->setBalance(accountInfo->getPreBalance() + accountInfo->getCloseProfit() - accountInfo->getCommission() + accountInfo->getDeposit() - accountInfo->getWithdraw());
		accountInfo->setCurrency("CNY");

		WTSArray * ay = WTSArray::create();
		ay->append(accountInfo, false);
		if (m_traderSink)
			m_traderSink->onRspAccount(ay);

		ay->release();
	}
}

void TraderCTPMini::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (!IsErrorRspInfo(pRspInfo) && pInvestorPosition)
	{
		if (NULL == m_ayPosition)
			m_ayPosition = WTSArray::create();

		WTSContractInfo* contract = m_bdMgr->getContract(pInvestorPosition->InstrumentID);
		WTSCommodityInfo* commInfo = m_bdMgr->getCommodity(contract);
		if (contract)
		{
			//ReqQryInvestorPositionDetail();
			WTSPositionItem *pos = WTSPositionItem::create(pInvestorPosition->InstrumentID, commInfo->getCurrency(), commInfo->getExchg());
			pos->setDirection(wrapPosDirection(pInvestorPosition->PosiDirection));
			pos->setNewPosition(pInvestorPosition->TodayPosition);
			pos->setPrePosition(pInvestorPosition->Position - pInvestorPosition->TodayPosition);

			pos->setMargin(pInvestorPosition->UseMargin + pInvestorPosition->PreMargin);
			pos->setDynProfit(pInvestorPosition->PositionProfit);
			pos->setPositionCost(pInvestorPosition->PositionCost);

			if (pos->getTotalPosition() != 0)
			{
				pos->setAvgPrice((uint32_t)(pos->getPositionCost() / pos->getTotalPosition() / commInfo->getVolScale()));
			}
			else
			{
				pos->setAvgPrice(0);
			}

			if (commInfo->getCategoty() != CC_Combination)
			{
				int availNew = pInvestorPosition->TodayPosition;
				if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
				{
					availNew -= pInvestorPosition->LongFrozen;
				}
				else
				{
					availNew -= pInvestorPosition->ShortFrozen;
				}
				if (availNew < 0)
					availNew = 0;
				pos->setAvailNewPos(availNew);

				int availPre = pos->getNewPosition() + pos->getPrePosition()
					- pInvestorPosition->LongFrozen - pInvestorPosition->ShortFrozen
					- pos->getAvailNewPos();
				pos->setAvailPrePos(availPre);
			}
			else
			{

			}

			if (pos->getTotalPosition() > 0 && pos->getMargin() == 0)
			{
				//有仓位，但是保证金为0，则说明是套利合约，单个合约的可用持仓全部置为0
				pos->setAvailNewPos(0);
				pos->setAvailPrePos(0);
			}

			m_ayPosition->append(pos, false);
		}
	}

	if (bIsLast)
	{
		if (m_traderSink)
			m_traderSink->onRspPosition(m_ayPosition);

		if (m_ayPosition)
		{
			m_ayPosition->release();
			m_ayPosition = NULL;
		}
	}
}

void TraderCTPMini::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (!IsErrorRspInfo(pRspInfo) && pTrade)
	{
		if (NULL == m_ayTrades)
			m_ayTrades = WTSArray::create();

		WTSTradeInfo* trade = makeTradeRecord(pTrade);
		if (trade)
		{
			m_ayTrades->append(trade, false);
		}
	}

	if (bIsLast)
	{
		if (m_traderSink)
			m_traderSink->onRspTrades(m_ayTrades);

		if (NULL != m_ayTrades)
			m_ayTrades->clear();
	}
}

void TraderCTPMini::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		m_bInQuery = false;
		triggerQuery();
	}

	if (!IsErrorRspInfo(pRspInfo) && pOrder)
	{
		if (NULL == m_ayOrders)
			m_ayOrders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(pOrder);
		if (orderInfo)
		{
			m_ayOrders->append(orderInfo, false);
		}
	}

	if (bIsLast)
	{
		if (m_traderSink)
			m_traderSink->onRspOrders(m_ayOrders);

		if (m_ayOrders)
			m_ayOrders->clear();
	}
}

void TraderCTPMini::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int x = 0;
}

void TraderCTPMini::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	WTSOrderInfo *orderInfo = makeOrderInfo(pOrder);
	if (orderInfo)
	{
		if (m_traderSink)
			m_traderSink->onPushOrder(orderInfo);

		orderInfo->release();
	}

	//ReqQryTradingAccount();
}

void TraderCTPMini::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	WTSTradeInfo *tRecord = makeTradeRecord(pTrade);
	if (tRecord)
	{
		if (m_traderSink)
			m_traderSink->onPushTrade(tRecord);

		tRecord->release();
	}
}

int TraderCTPMini::wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return THOST_FTDC_D_Buy;
		else
			return THOST_FTDC_D_Sell;
	else
		if (offsetType == WOT_OPEN)
			return THOST_FTDC_D_Sell;
		else
			return THOST_FTDC_D_Buy;
}

WTSDirectionType TraderCTPMini::wrapDirectionType(TThostFtdcDirectionType dirType, TThostFtdcOffsetFlagType offsetType)
{
	if (THOST_FTDC_D_Buy == dirType)
		if (offsetType == THOST_FTDC_OF_Open)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if (offsetType == THOST_FTDC_OF_Open)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

WTSDirectionType TraderCTPMini::wrapPosDirection(TThostFtdcPosiDirectionType dirType)
{
	if (THOST_FTDC_PD_Long == dirType)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

int TraderCTPMini::wrapOffsetType(WTSOffsetType offType)
{
	if (WOT_OPEN == offType)
		return THOST_FTDC_OF_Open;
	else if (WOT_CLOSE == offType)
		return THOST_FTDC_OF_Close;
	else if (WOT_CLOSETODAY == offType)
		return THOST_FTDC_OF_CloseToday;
	else if (WOT_CLOSEYESTERDAY == offType)
		return THOST_FTDC_OF_Close;
	else
		return THOST_FTDC_OF_ForceClose;
}

WTSOffsetType TraderCTPMini::wrapOffsetType(TThostFtdcOffsetFlagType offType)
{
	if (THOST_FTDC_OF_Open == offType)
		return WOT_OPEN;
	else if (THOST_FTDC_OF_Close == offType)
		return WOT_CLOSE;
	else if (THOST_FTDC_OF_CloseToday == offType)
		return WOT_CLOSETODAY;
	else
		return WOT_FORCECLOSE;
}

int TraderCTPMini::wrapPriceType(WTSPriceType priceType)
{
	if (WPT_ANYPRICE == priceType)
		return THOST_FTDC_OPT_AnyPrice;
	else if (WPT_LIMITPRICE == priceType)
		return THOST_FTDC_OPT_LimitPrice;
	else if (WPT_BESTPRICE == priceType)
		return THOST_FTDC_OPT_BestPrice;
	else
		return THOST_FTDC_OPT_LastPrice;
}

WTSPriceType TraderCTPMini::wrapPriceType(TThostFtdcPriceType priceType)
{
	if (THOST_FTDC_OPT_AnyPrice == priceType)
		return WPT_ANYPRICE;
	else if (THOST_FTDC_OPT_LimitPrice == priceType)
		return WPT_LIMITPRICE;
	else if (THOST_FTDC_OPT_BestPrice == priceType)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
}

int TraderCTPMini::wrapTimeCondition(WTSTimeCondition timeCond)
{
	if (WTC_IOC == timeCond)
		return THOST_FTDC_TC_IOC;
	else if (WTC_GFD == timeCond)
		return THOST_FTDC_TC_GFD;
	else
		return THOST_FTDC_TC_GFS;
}

WTSTimeCondition TraderCTPMini::wrapTimeCondition(TThostFtdcTimeConditionType timeCond)
{
	if (THOST_FTDC_TC_IOC == timeCond)
		return WTC_IOC;
	else if (THOST_FTDC_TC_GFD == timeCond)
		return WTC_GFD;
	else
		return WTC_GFS;
}

WTSOrderState TraderCTPMini::wrapOrderState(TThostFtdcOrderStatusType orderState)
{
	if (orderState != THOST_FTDC_OST_Unknown)
		return (WTSOrderState)orderState;
	else
		return WOS_Submitting;
}

int TraderCTPMini::wrapActionFlag(WTSActionFlag actionFlag)
{
	if (WAF_CANCEL == actionFlag)
		return THOST_FTDC_AF_Delete;
	else
		return THOST_FTDC_AF_Modify;
}


WTSOrderInfo* TraderCTPMini::makeOrderInfo(CThostFtdcOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->InstrumentID);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(orderField->LimitPrice);
	pRet->setVolumn(orderField->VolumeTotalOriginal);
	pRet->setDirection(wrapDirectionType(orderField->Direction, orderField->CombOffsetFlag[0]));
	pRet->setPriceType(wrapPriceType(orderField->OrderPriceType));
	pRet->setTimeCondition(wrapTimeCondition(orderField->TimeCondition));
	pRet->setOffsetType(wrapOffsetType(orderField->CombOffsetFlag[0]));

	pRet->setVolTraded(orderField->VolumeTraded);
	pRet->setVolLeft(orderField->VolumeTotal);

	pRet->setCode(orderField->InstrumentID);

	pRet->setOrderDate(strtoul(orderField->InsertDate, NULL, 10));
	std::string strTime = orderField->InsertTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10) * 1000));
	pRet->setOrderState(wrapOrderState(orderField->OrderStatus));

	pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef)).c_str());
	pRet->setOrderID(orderField->OrderSysID);

	pRet->setStateMsg(orderField->StatusMsg);

	StringMap::iterator it = m_mapEntrustTag.find(pRet->getEntrustID());
	if (it != m_mapEntrustTag.end())
	{
		pRet->setUserTag(it->second.c_str());

		m_mapOrderTag[pRet->getOrderID()] = it->second;
	}
	else
	{
		pRet->setUserTag(pRet->getEntrustID());
	}

	return pRet;
}

WTSEntrust* TraderCTPMini::makeEntrust(CThostFtdcInputOrderField *entrustField)
{
	WTSEntrust* pRet = WTSEntrust::create(
		entrustField->InstrumentID,
		entrustField->VolumeTotalOriginal,
		entrustField->LimitPrice);

	pRet->setDirection(wrapDirectionType(entrustField->Direction, entrustField->CombOffsetFlag[0]));
	pRet->setPriceType(wrapPriceType(entrustField->OrderPriceType));
	pRet->setOffsetType(wrapOffsetType(entrustField->CombOffsetFlag[0]));
	pRet->setTimeCondition(wrapTimeCondition(entrustField->TimeCondition));

	pRet->setEntrustID(generateEntrustID(m_frontID, m_sessionID, atoi(entrustField->OrderRef)).c_str());

	StringMap::iterator it = m_mapEntrustTag.find(pRet->getEntrustID());
	if (it != m_mapEntrustTag.end())
	{
		pRet->setUserTag(it->second.c_str());
	}

	return pRet;
}

WTSError* TraderCTPMini::makeError(CThostFtdcRspInfoField* rspInfo)
{
	WTSError* pRet = WTSError::create((WTSErroCode)rspInfo->ErrorID, rspInfo->ErrorMsg);
	return pRet;
}

WTSTradeInfo* TraderCTPMini::makeTradeRecord(CThostFtdcTradeField *tradeField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->InstrumentID);
	if (contract == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = m_bdMgr->getCommodity(contract);
	WTSSessionInfo* sInfo = m_bdMgr->getSession(commInfo->getSession());

	WTSTradeInfo *pRet = WTSTradeInfo::create(tradeField->InstrumentID);
	pRet->setVolumn(tradeField->Volume);
	pRet->setPrice(tradeField->Price);
	pRet->setTradeID(tradeField->TradeID);

	std::string strTime = tradeField->TradeTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	uint32_t uDate = strtoul(tradeField->TradeDate, NULL, 10);

	//if(uDate == m_pContractMgr->getTradingDate())
	//{
	//	//如果当前日期和交易日一致，且时间大于21点，说明是夜盘，也就是实际日期要单独计算
	//	if (uTime / 10000 >= 21)
	//	{
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//	}
	//	else if(uTime <= 3)
	//	{
	//		//如果在3点以内，就要先获取上一个交易日，再获取下一个自然日
	//		//这样做的目的是，遇到周五晚上的情况，可以处理过来
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//		uDate = TimeUtils::getNextDate(uDate);
	//	}
	//}

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType(tradeField->Direction, tradeField->OffsetFlag);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(tradeField->OffsetFlag));
	pRet->setRefOrder(tradeField->OrderSysID);
	pRet->setTradeType((WTSTradeType)tradeField->TradeType);

	double amount = commInfo->getVolScale()*tradeField->Volume*pRet->getPrice();
	pRet->setAmount(amount);

	StringMap::iterator it = m_mapOrderTag.find(pRet->getRefOrder());
	if (it != m_mapOrderTag.end())
	{
		pRet->setUserTag(it->second.c_str());
	}

	return pRet;
}

std::string TraderCTPMini::generateEntrustID(uint32_t frontid, uint32_t sessionid, uint32_t orderRef)
{
	return StrUtil::printf("%06u.%010u.%06u", frontid, sessionid, orderRef);
}

bool TraderCTPMini::extractEntrustID(const char* entrustid, uint32_t &frontid, uint32_t &sessionid, uint32_t &orderRef)
{
	//Market.FrontID.SessionID.OrderRef
	StringVector &vecString = StrUtil::split(entrustid, ".");
	if (vecString.size() != 3)
		return false;

	frontid = strtoul(vecString[0].c_str(), NULL, 10);
	sessionid = strtoul(vecString[1].c_str(), NULL, 10);
	orderRef = strtoul(vecString[2].c_str(), NULL, 10);

	return true;
}

bool TraderCTPMini::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
		return true;

	return false;
}

void TraderCTPMini::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{

}

bool TraderCTPMini::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}

int TraderCTPMini::queryConfirm()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_LOGINED)
	{
		return -1;
	}

	BoostUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		CThostFtdcQrySettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_strBroker.c_str());
		strcpy(req.InvestorID, m_strUser.c_str());

		int iResult = m_pUserAPI->ReqQrySettlementInfoConfirm(&req, genRequestID());
		if (iResult != 0)
		{
			m_traderSink->handleTraderLog(LL_ERROR, "[%s-%s]查询账户结算确认请求发送失败, 错误码:%d", m_strBroker.c_str(), m_strUser.c_str(), iResult);
		}
	});

	triggerQuery();

	return 0;
}

int TraderCTPMini::confirm()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_CONFIRM_QRYED)
	{
		return -1;
	}

	//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.InvestorID, m_strUser.c_str());

	sprintf(req.ConfirmDate, "%u", TimeUtils::getCurDate());
	strncpy(req.ConfirmTime, TimeUtils::getLocalTime().c_str(), 8);

	int iResult = m_pUserAPI->ReqSettlementInfoConfirm(&req, genRequestID());
	if (iResult != 0)
	{
		m_traderSink->handleTraderLog(LL_ERROR, "[%s-%s]确认结算信息请求发送失败, 错误码:%d", m_strBroker.c_str(), m_strUser.c_str(), iResult);
		return -1;
	}

	return 0;
}

int TraderCTPMini::authenticate()
{
	CThostFtdcReqAuthenticateField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUser.c_str());
	//strcpy(req.UserProductInfo, m_strProdInfo.c_str());
	strcpy(req.AuthCode, m_strAuthCode.c_str());
	strcpy(req.AppID, m_strAppID.c_str());
	m_pUserAPI->ReqAuthenticate(&req, genRequestID());

	return 0;
}

void TraderCTPMini::triggerQuery()
{
	m_strandIO->post([this](){
		if (m_queQuery.empty() || m_bInQuery)
			return;

		uint64_t curTime = TimeUtils::getLocalTimeNow();
		if (curTime - m_lastQryTime < 1000)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(50));
			m_strandIO->post([this](){
				triggerQuery();
			});
			return;
		}


		m_bInQuery = true;
		CommonExecuter& handler = m_queQuery.front();
		handler();

		{
			BoostUniqueLock lock(m_mtxQuery);
			m_queQuery.pop();
		}

		m_lastQryTime = TimeUtils::getLocalTimeNow();
	});
}