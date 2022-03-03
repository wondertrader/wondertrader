/*!
 * \file TraderYD.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "TraderYD.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/decimal.h"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

const char* ENTRUST_SECTION = "entrusts";
const char* ORDER_SECTION = "orders";

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	fmt::format_to(buffer, format, args...);

	sink->handleTraderLog(ll, buffer);
}

inline WTSDirectionType wrapPosDirection(int dirType)
{
	if (YD_PD_Long == dirType)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

inline WTSDirectionType wrapDirectionType(int dirType, int offsetType)
{
	if (YD_D_Buy == dirType)
		if (offsetType == YD_OF_Open)
			return WDT_LONG;
		else
			return WDT_SHORT;
	else
		if (offsetType == YD_OF_Open)
			return WDT_SHORT;
		else
			return WDT_LONG;
}

inline int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return YD_D_Buy;
		else
			return YD_D_Sell;
	else
		if (offsetType == WOT_OPEN)
			return YD_D_Sell;
		else
			return YD_D_Buy;
}

inline int wrapPriceType(WTSPriceType pType, WTSOrderFlag oFlag)
{
	if (WOF_FAK == oFlag)
		return YD_ODT_FAK;

	if (WOF_FOK == oFlag)
		return YD_ODT_FOK;

	if (WPT_ANYPRICE == pType)
		return YD_ODT_Market;

	if (WPT_LIMITPRICE == pType)
		return YD_ODT_Limit;
	
	return YD_ODT_Market;
}

inline WTSOffsetType wrapOffsetType(int offType)
{
	if (YD_OF_Open == offType)
		return WOT_OPEN;
	else if (YD_OF_Close == offType)
		return WOT_CLOSE;
	else if (YD_OF_CloseToday == offType)
		return WOT_CLOSETODAY;
	else if (YD_OF_CloseYesterday == offType)
		return WOT_CLOSEYESTERDAY;
	else
		return WOT_FORCECLOSE;
}

inline int wrapOffsetType(WTSOffsetType offType)
{
	if (WOT_OPEN == offType)
		return YD_OF_Open;
	else if (WOT_CLOSE == offType)
		return YD_OF_Close;
	else if (WOT_CLOSETODAY == offType)
		return YD_OF_CloseToday;
	else if (WOT_CLOSEYESTERDAY == offType)
		return YD_OF_CloseYesterday;
	else
		return YD_OF_ForceClose;
}

inline WTSOrderState wrapOrderState(int orderState)
{
	switch (orderState)
	{
	case YD_OS_Accepted: return WOS_NotTraded_NotQueuing;
	case YD_OS_Queuing: return WOS_NotTraded_Queuing;
	case YD_OS_Canceled:
	case YD_OS_Rejected:
		return WOS_Canceled;
	case YD_OS_AllTraded: return WOS_AllTraded;
	default:
		return WOS_Submitting;
	}
}


extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderYD *instance = new TraderYD();
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

TraderYD::TraderYD()
	: m_pUserAPI(NULL)
	, m_mapPosition(NULL)
	, m_mapOrders(NULL)
	, m_mapTrades(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_ayFunds(NULL)
	, m_uLastQryTime(0)
	, m_iRequestID(0)
	, m_bCatchup(false)
	, m_bStopped(false)
	, m_bApiInited(false)
{
}


TraderYD::~TraderYD()
{
}


void TraderYD::notifyReadyForLogin(bool hasLoginFailed)
{
	if (m_sink)
		m_sink->handleEvent(WTE_Connect, 0);
}


void TraderYD::notifyLogin(int errorNo, int maxOrderRef, bool isMonitor)
{
	if (errorNo == 0)
	{
		m_wrapperState = WS_LOGINED;

		// 保存会话参数
		m_orderRef = maxOrderRef;
		///获取当前交易日
		m_lDate = m_pUserAPI->getTradingDay();

		write_log(m_sink, LL_INFO, "[TraderYD] {} Login succeed, Trading Day: {}",
			m_strUser.c_str(), m_lDate);
	}
	else
	{
		write_log(m_sink, LL_ERROR, "[TraderYD] {} Login failed: {}", m_strUser.c_str(), errorNo);
		m_wrapperState = WS_LOGINFAILED;

		if (m_sink)
			m_sink->onLoginResult(false, "Login failed", 0);
	}
}

void TraderYD::notifyFailedOrder(const YDInputOrder *pFailedOrder, const YDInstrument *pInstrument, const YDAccount *pAccount)
{
	WTSEntrust* entrust = makeEntrust(pFailedOrder, pInstrument);
	if (entrust)
	{
		WTSError *err = makeError(pFailedOrder->ErrorNo, WEC_ORDERINSERT);
		//g_orderMgr.onRspEntrust(entrust, err);
		if (m_sink)
			m_sink->onRspEntrust(entrust, err);
		entrust->release();
		err->release();
	}
}

void TraderYD::notifyFailedCancelOrder(const YDFailedCancelOrder *pFailedCancelOrder, const YDExchange *pExchange, const YDAccount *pAccount)
{
	WTSError* error = makeError(pFailedCancelOrder->ErrorNo, WEC_ORDERCANCEL);
	if (m_sink)
		m_sink->onTraderError(error);
}


void TraderYD::notifyFinishInit()
{
	/*
	 *	这里的处理逻辑比较复杂
	 *	在第一次登陆成功以后，底层会去查询各种数据
	 *	底层查完了以后，会触发这个回调
	 *	这里需要进行拉取上日结算以后的数据
	 *	然后再利用回报进行数据拼接
	 *	最后在notifyCatchup的时候，再通知listener
	 */

	//先查资金
	{
		const YDAccount* accInfo = m_pUserAPI->getMyAccount();

		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setDescription(m_strUser.c_str());
		//accountInfo->setUsername(m_strUserName.c_str());
		accountInfo->setPreBalance(accInfo->PreBalance);
		accountInfo->setDeposit(accInfo->Deposit);
		accountInfo->setWithdraw(accInfo->Withdraw);
		accountInfo->setBalance(accountInfo->getPreBalance() + accountInfo->getDeposit() - accountInfo->getWithdraw());
		accountInfo->setCurrency("CNY");

		if (m_ayFunds == NULL)
			m_ayFunds = WTSArray::create();

		m_ayFunds->append(accountInfo, false);
	}

	//再查持仓
	{
		if (NULL == m_mapPosition)
			m_mapPosition = DataMap::create();

		int cnt = m_pUserAPI->getPrePositionCount();
		for (int i = 0; i < cnt; i++)
		{
			const YDPrePosition* pInfo = m_pUserAPI->getPrePosition(i);
			const YDInstrument* instInfo = pInfo->m_pInstrument;

			WTSContractInfo* contract = m_bdMgr->getContract(instInfo->InstrumentID, instInfo->m_pExchange->ExchangeID);
			if (contract)
			{
				WTSCommodityInfo* commInfo = contract->getCommInfo();
				std::string key = StrUtil::printf("{}-{}", contract->getCode(), wrapPosDirection(pInfo->PositionDirection));
				WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
				if (pos == NULL)
				{
					pos = WTSPositionItem::create(contract->getCode(), commInfo->getCurrency(), commInfo->getExchg());
					pos->setContractInfo(contract);
					m_mapPosition->add(key, pos, false);
				}

				pos->setDirection(wrapPosDirection(pInfo->PositionDirection));
				pos->setPrePosition(pInfo->PrePosition);
				pos->setNewPosition(0);

				pos->setMargin(0);
				pos->setDynProfit(0);
				pos->setPositionCost(pInfo->PrePosition*commInfo->getVolScale()*pInfo->PreSettlementPrice);

				if (pos->getTotalPosition() != 0)
				{
					pos->setAvgPrice(pInfo->PreSettlementPrice);
				}
				else
				{
					pos->setAvgPrice(0);
				}

				pos->setAvailPrePos(pos->getPrePosition());
				pos->setAvailNewPos(0);
			}
		}
	}
}


void TraderYD::notifyOrder(const YDOrder *pOrder, const YDInstrument *pInstrument, const YDAccount *pAccount)
{
	WTSOrderInfo *orderInfo = makeOrderInfo(pOrder, pInstrument);
	if (orderInfo)
	{
		//先往缓存里丢
		if (NULL == m_mapOrders)
			m_mapOrders = DataMap::create();

		const char* oid = orderInfo->getOrderID();
		auto it = m_mapOrders->find(oid);
		if(it == m_mapOrders->end())
		{
			//如果该订单是第一次被推送
			//则检查是否是平仓委托
			//如果是平仓委托，需要调整冻结手数
			if (orderInfo->getOffsetType() != WOT_OPEN)
			{
				std::string key = StrUtil::printf("{}-{}", orderInfo->getCode(), orderInfo->getDirection());
				WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
				double preQty = pos->getPrePosition();
				double newQty = pos->getNewPosition();
				double availPre = pos->getAvailPrePos();
				double availNew = pos->getAvailNewPos();

				WTSCommodityInfo* commInfo = orderInfo->getContractInfo()->getCommInfo();
				if(commInfo->getCoverMode() == CM_CoverToday)
				{
					if (orderInfo->getOffsetType() == WOT_CLOSETODAY)
						availNew -= orderInfo->getVolume();
					else
						availPre -= orderInfo->getVolume();
				}
				else
				{
					//不区分平昨平今，则先冻结昨仓，再冻结今仓
					double maxQty = min(availPre, orderInfo->getVolume());
					availPre -= maxQty;
					if(decimal::lt(orderInfo->getVolume(), maxQty))
					{
						availNew -= orderInfo->getVolume() - maxQty;
					}
				}

				pos->setAvailPrePos(availPre);
				pos->setAvailNewPos(availNew);
			}
		}
		else
		{
			WTSOrderInfo* preOrd = (WTSOrderInfo*)it->second;
			//如果订单不是第一次被推送，则看是否是撤单
			//如果是撤单，并且之间订单状态还是有效的，则对平仓委托要释放冻结的手数
			if(preOrd->isAlive() && orderInfo->getOrderState() == WOS_Canceled && orderInfo->getOffsetType() != WOT_OPEN)
			{
				std::string key = StrUtil::printf("{}-{}", orderInfo->getCode(), orderInfo->getDirection());
				WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
				double preQty = pos->getPrePosition();
				double newQty = pos->getNewPosition();
				double availPre = pos->getAvailPrePos();
				double availNew = pos->getAvailNewPos();

				double untrade = orderInfo->getVolume() - orderInfo->getVolTraded();

				WTSCommodityInfo* commInfo = orderInfo->getContractInfo()->getCommInfo();
				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					if (orderInfo->getOffsetType() == WOT_CLOSETODAY)
						availNew += untrade;
					else
						availPre += untrade;
				}
				else
				{
					//不区分平昨平今，则先释放今仓，再释放昨仓
					double maxQty = min(newQty-availNew , untrade);
					availNew += maxQty;
					if (decimal::lt(untrade, maxQty))
					{
						availPre += orderInfo->getVolume() - maxQty;
					}
				}

				pos->setAvailPrePos(availPre);
				pos->setAvailNewPos(availNew);
			}
		}
		m_mapOrders->add(oid, orderInfo, false);

		//如果已经追上了，则直接主推出去
		if (m_sink && m_bCatchup)
		{
			m_sink->onPushOrder(orderInfo);
		}
	}
}


void TraderYD::notifyTrade(const YDTrade *pTrade, const YDInstrument *pInstrument, const YDAccount *pAccount)
{
	WTSTradeInfo *trdInfo = makeTradeRecord(pTrade, pInstrument);
	if (trdInfo)
	{
		//先往缓存里丢
		if (NULL == m_mapTrades)
			m_mapTrades = DataMap::create();

		const char* tid = trdInfo->getTradeID();
		WTSContractInfo* contract = trdInfo->getContractInfo();
		WTSCommodityInfo* commInfo = contract->getCommInfo();
		auto it = m_mapTrades->find(tid);
		if(it == m_mapTrades->end())
		{
			m_mapTrades->add(tid, trdInfo, false);

			//成交回报，主要更新持仓
			std::string key = StrUtil::printf("{}-{}", trdInfo->getCode(), trdInfo->getDirection());
			WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
			if(pos == NULL)
			{
				pos = WTSPositionItem::create(contract->getCode(), commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				m_mapPosition->add(key, pos, false);
			}

			double preQty = pos->getPrePosition();
			double newQty = pos->getNewPosition();
			double availPre = pos->getAvailPrePos();
			double availNew = pos->getAvailNewPos();

			double qty = trdInfo->getVolume();

			if(trdInfo->getOffsetType() == WOT_OPEN)
			{
				newQty += qty;
				availNew += qty;

				//开仓一定是今仓
				pos->setNewPosition(newQty);
				pos->setAvailNewPos(availNew);
			}
			else
			{
				//平仓要区分
				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					//平仓不用更新可用持仓
					//因为可用持仓在订单回报的地方已经更新过了
					if (trdInfo->getOffsetType() == WOT_CLOSETODAY)
						newQty -= qty;
					else
						preQty -= qty;
				}
				else
				{
					//不区分平昨平今，则先减掉昨仓，在调整今仓
					double maxQty = min(preQty, qty);
					preQty -= maxQty;
					if (decimal::lt(qty, maxQty))
					{
						newQty -= (qty - maxQty);
					}
				}

				pos->setNewPosition(newQty);
				pos->setPrePosition(preQty);
			}
		}

		if (m_sink && m_bCatchup)
			m_sink->onPushTrade(trdInfo);
	}
}

void TraderYD::notifyCaughtUp()
{
	m_bCatchup = true;

	if(!m_bApiInited)
	{
		//全部初始化好了以后，再通知登录成功
		m_wrapperState = WS_ALLREADY;

		if (m_sink)
			m_sink->onLoginResult(true, "", m_lDate);

		m_bApiInited = true;
	}
}

void TraderYD::notifyAccount(const YDAccount *pAccount)
{
	
}

bool TraderYD::init(WTSVariant* config)
{
	m_strCfgFile = config->getCString("config");
	m_strUser = config->getCString("user");
	m_strPass = config->getCString("pass");

	std::string module = config->getCString("ydmodule");
	if (module.empty())
		module = "yd";

	m_strModule = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");

	m_hInstYD = DLLHelper::load_library(m_strModule.c_str());
	m_funcCreator = (YDCreator)DLLHelper::get_symbol(m_hInstYD, "makeYDApi");

	return true;
}

void TraderYD::release()
{
	m_bStopped = true;

	if (m_pUserAPI)
	{
		m_pUserAPI->startDestroy();
		m_pUserAPI = NULL;
	}

	if (m_mapOrders)
		m_mapOrders->clear();

	if (m_mapPosition)
		m_mapPosition->clear();

	if (m_mapTrades)
		m_mapTrades->clear();
}

void TraderYD::connect()
{
	m_pUserAPI = m_funcCreator(m_strCfgFile.c_str());

	if (m_pUserAPI)
	{
		m_pUserAPI->start(this);
	}

	if (m_thrdWorker == NULL)
	{
		m_thrdWorker.reset(new StdThread([this]() {
			while (!m_bStopped)
			{
				if (m_queQuery.empty())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				CommonExecuter& handler = m_queQuery.front();
				handler();

				{
					StdUniqueLock lock(m_mtxQuery);
					m_queQuery.pop();
				}
			}
		}));
	}
}

void TraderYD::disconnect()
{
	if (m_pUserAPI == NULL)
		return;

	m_pUserAPI->disconnect();
}

bool TraderYD::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t orderref = m_orderRef.fetch_add(1) + 1;
		sprintf(buffer, "%s#%010u", m_strUser.c_str(), orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderYD::registerSpi(ITraderSpi *listener)
{
	m_sink = listener;
	if (m_sink)
	{
		m_bdMgr = listener->getBaseDataMgr();
	}
}

uint32_t TraderYD::genRequestID()
{
	return m_iRequestID.fetch_add(1) + 1;
}

int TraderYD::login(const char* user, const char* pass, const char* productInfo)
{
	m_strUser = user;
	m_strPass = pass;

	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	m_wrapperState = WS_LOGINING;
	doLogin();

	return 0;
}

int TraderYD::doLogin()
{
	if (m_pUserAPI == NULL)
	{
		return 0;
	}

	if (!m_pUserAPI->login(m_strUser.c_str(), m_strPass.c_str(), NULL, NULL))
	{
		if (m_sink)
			write_log(m_sink, LL_ERROR, "[TraderYD] Sending login request failed");
	}

	return 0;
}

int TraderYD::logout()
{
	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	return 0;
}

int TraderYD::orderInsert(WTSEntrust* entrust)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	const YDInstrument* pInst = m_pUserAPI->getInstrumentByID(entrust->getCode());

	YDInputOrder req;
	// inputOrder中的所有不用的字段，应当统一清0
	memset(&req, 0, sizeof(req));

	if (strlen(entrust->getUserTag()) == 0)
	{
		///报单引用
		req.OrderRef = m_orderRef.fetch_add(0);
	}
	else
	{
		uint32_t orderref;
		extractEntrustID(entrust->getEntrustID(), orderref);
		///报单引用
		req.OrderRef = orderref;
	}

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
		m_iniHelper.save();
	}

	req.Price = entrust->getPrice();
	req.Direction = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.OffsetFlag = wrapOffsetType(entrust->getOffsetType());
	req.HedgeFlag = YD_HF_Speculation;
	req.OrderVolume = (int)entrust->getVolume();
	// 使用下一个下一个报单引用。YD服务器不检查OrderRef，只是将其用于在报单和成交回报中返回
	// 用户可以自行选择OrderRef的编码方式
	// 对于非本系统本次运行产生的报单，系统返回的OrderRef一律是-1
	// YDClient产生的报单，OrderRef一律是0
	// 这个例子使用限价单
	req.OrderType = wrapPriceType(entrust->getPriceType(), entrust->getOrderFlag());
	// 说明是普通报单
	req.YDOrderFlag = YD_YOF_Normal;
	// 说明如何选择连接
	req.ConnectionSelectionType = YD_CS_Any;
	// 如果ConnectionSelectionType不是YD_CS_Any，需要指定ConnectionID，范围是0到对应的YDExchange中的ConnectionCount-1
	req.ConnectionID = 0;
	// inputOrder中的RealConnectionID和ErrorNo是在返回时由服务器填写的
	if(!m_pUserAPI->insertOrder(&req, pInst))
	{
		write_log(m_sink, LL_ERROR, "[TraderCTP] Order inserting failed");
	}
	return 0;
}

int TraderYD::orderAction(WTSEntrustAction* action)
{
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	uint32_t orderref;
	if (!extractEntrustID(action->getEntrustID(), orderref))
		return -1;

	const YDInstrument* pInst = m_pUserAPI->getInstrumentByID(action->getCode());
	const YDExchange* pExchg = pInst->m_pExchange;
	const YDAccount* pAccount = m_pUserAPI->getMyAccount();

	YDCancelOrder req;
	memset(&req, 0, sizeof(req));

	req.OrderSysID = atoi(action->getOrderID());
	req.YDOrderFlag = YD_YOF_Normal;
	req.ConnectionSelectionType = YD_CS_Any;
	req.ConnectionID = 0;

	if(!m_pUserAPI->cancelOrder(&req, pExchg, pAccount))
	{
		write_log(m_sink, LL_ERROR, "[TraderYD] Sending cancel request failed");
	}

	return 0;
}

int TraderYD::queryAccount()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	{
		StdUniqueLock lock(m_mtxQuery);
		m_queQuery.push([this]() {
			if (m_sink) m_sink->onRspAccount(m_ayFunds);
		});
	}

	return 0;
}

int TraderYD::queryPositions()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	{
		StdUniqueLock lock(m_mtxQuery);
		m_queQuery.push([this]() {
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

			ayPos->release();
		});
	}

	return 0;
}

int TraderYD::queryOrders()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	{
		StdUniqueLock lock(m_mtxQuery);
		m_queQuery.push([this]() {
			WTSArray* ayOrders = WTSArray::create();
			if(m_mapOrders)
			{
				for(auto it = m_mapOrders->begin(); it != m_mapOrders->end(); it++)
				{
					ayOrders->append(it->second, true);
				}
			}
			if (m_sink) m_sink->onRspOrders(ayOrders);
		});
	}

	return 0;
}

int TraderYD::queryTrades()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	{
		StdUniqueLock lock(m_mtxQuery);
		m_queQuery.push([this]() {
			WTSArray* ayTrades = WTSArray::create();
			if (m_mapTrades)
			{
				for (auto it = m_mapTrades->begin(); it != m_mapTrades->end(); it++)
				{
					ayTrades->append(it->second, true);
				}
			}
			if (m_sink) m_sink->onRspTrades(ayTrades);
		});
	}

	return 0;
}

WTSOrderInfo* TraderYD::makeOrderInfo(const YDOrder* orderField, const YDInstrument* instInfo)
{
	const YDExchange* exchgInfo = instInfo->m_pExchange;

	WTSContractInfo* contract = m_bdMgr->getContract(instInfo->InstrumentID, exchgInfo->ExchangeID);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(orderField->Price);
	pRet->setVolume(orderField->OrderVolume);
	pRet->setDirection(wrapDirectionType(orderField->Direction, orderField->OffsetFlag));
	pRet->setOffsetType(wrapOffsetType(orderField->OffsetFlag));

	pRet->setPriceType(decimal::eq(orderField->Price, 0.0) ? WPT_ANYPRICE : WPT_LIMITPRICE);
	if (orderField->OrderType == YD_ODT_FAK)
		pRet->setOrderFlag(WOF_FAK);
	else if (orderField->OrderType == YD_ODT_FOK)
		pRet->setOrderFlag(WOF_FOK);
	else
		pRet->setOrderFlag(WOF_NOR);

	pRet->setVolTraded(orderField->TradeVolume);
	pRet->setVolLeft(orderField->OrderVolume - orderField->TradeVolume);

	pRet->setCode(contract->getCode());
	pRet->setExchange(contract->getExchg());

	uint32_t uDate = m_lDate;
	uint32_t uTime = orderField->InsertTime;
	pRet->setOrderDate(uDate);
	pRet->setOrderTime(TimeUtils::makeTime(uDate, uTime * 1000));

	pRet->setOrderState(wrapOrderState(orderField->OrderStatus));
	if (orderField->OrderStatus == YD_OS_Rejected)
		pRet->setError(true);

	pRet->setEntrustID(generateEntrustID(orderField->OrderRef).c_str());
	pRet->setOrderID(fmt::format("{}",orderField->OrderSysID).c_str());

	pRet->setStateMsg("");

	std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID(), "");
	if(usertag.empty())
	{
		pRet->setUserTag(pRet->getEntrustID());
	}
	else
	{
		pRet->setUserTag(usertag.c_str());

		if (strlen(pRet->getOrderID()) > 0)
		{
			m_iniHelper.writeString(ORDER_SECTION, StrUtil::trim(pRet->getOrderID()).c_str(), usertag.c_str());
			m_iniHelper.save();
		}
	}

	return pRet;
}

WTSEntrust* TraderYD::makeEntrust(const YDInputOrder *entrustField, const YDInstrument* instInfo)
{
	WTSContractInfo* ct = m_bdMgr->getContract(instInfo->InstrumentID, instInfo->m_pExchange->ExchangeID);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		ct->getCode(),
		entrustField->OrderVolume,
		entrustField->Price,
		ct->getExchg());
	pRet->setContractInfo(ct);

	pRet->setDirection(wrapDirectionType(entrustField->Direction, entrustField->OffsetFlag));
	pRet->setOffsetType(wrapOffsetType(entrustField->OffsetFlag));
	
	pRet->setPriceType(decimal::eq(entrustField->Price, 0.0) ? WPT_ANYPRICE : WPT_LIMITPRICE);
	if (entrustField->OrderType == YD_ODT_FAK)
		pRet->setOrderFlag(WOF_FAK);
	else if (entrustField->OrderType == YD_ODT_FOK)
		pRet->setOrderFlag(WOF_FOK);
	else
		pRet->setOrderFlag(WOF_NOR);

	pRet->setEntrustID(generateEntrustID(entrustField->OrderRef).c_str());

	std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID());
	if (!usertag.empty())
		pRet->setUserTag(usertag.c_str());

	return pRet;
}

WTSError* TraderYD::makeError(int errorno, WTSErroCode ec)
{
	WTSError* pRet = WTSError::create(ec, StrUtil::printf("ErrorNo: %d", errorno).c_str());
	return pRet;
}

WTSTradeInfo* TraderYD::makeTradeRecord(const YDTrade *tradeField, const YDInstrument* instInfo)
{
	WTSContractInfo* contract = m_bdMgr->getContract(instInfo->InstrumentID, instInfo->m_pExchange->ExchangeID);
	if (contract == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = contract->getCommInfo();
	WTSSessionInfo* sInfo = commInfo->getSessionInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(contract->getCode(), commInfo->getExchg());
	pRet->setContractInfo(contract);
	pRet->setVolume(tradeField->Volume);
	pRet->setPrice(tradeField->Price);
	pRet->setTradeID(fmt::format("{}",tradeField->TradeID).c_str());

	uint32_t uDate = m_lDate;
	uint32_t uTime = tradeField->TradeTime;

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType(tradeField->Direction, tradeField->OffsetFlag);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType(tradeField->OffsetFlag));
	pRet->setRefOrder(fmt::format("{}", tradeField->OrderSysID).c_str());
	pRet->setTradeType(WTT_Common);

	double amount = commInfo->getVolScale()*tradeField->Volume*pRet->getPrice();
	pRet->setAmount(amount);

	std::string usertag = m_iniHelper.readString(ORDER_SECTION, StrUtil::trim(pRet->getRefOrder()).c_str());
	if (!usertag.empty())
		pRet->setUserTag(usertag.c_str());

	return pRet;
}

std::string TraderYD::generateEntrustID(uint32_t orderRef)
{
	return StrUtil::printf("%s#%010u", m_strUser, orderRef);
}

bool TraderYD::extractEntrustID(const char* entrustid, uint32_t &orderRef)
{
	//Market.FrontID.SessionID.OrderRef
	const StringVector &vecString = StrUtil::split(entrustid, "#");
	if (vecString.size() != 2)
		return false;

	orderRef = strtoul(vecString[1].c_str(), NULL, 10);

	return true;
}

bool TraderYD::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}
