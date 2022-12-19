/*!
 * \file TraderMAOpt.cpp
 * \project	WonderTrader
 *
 * \author Suuuunseeker
 * \date 2022/07/13
 *
 * \brief
 */
#include "TraderMAOpt.h"

#include "../Includes/WTSError.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/ModuleHelper.hpp"
#include "../Share/decimal.h"

#include <boost/filesystem.hpp>
#include <iostream>

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/maCliApi_Patch3.8(R)/x64/maTradeApi.lib")
#else
#pragma comment(lib, "../API/mCliApi3.7/x86/maTradeApi.lib")
#endif
#endif


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
		TraderMAOpt *instance = new TraderMAOpt();
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

inline int wrapDirectionType(WTSDirectionType dirType, WTSOffsetType offsetType)
{
	if (WDT_LONG == dirType)
		if (offsetType == WOT_OPEN)
			return MA_BuyOpen;
		else
			return MA_SellClose;
	else
		if (offsetType == WOT_OPEN)
			return MA_SellOpen;
		else
			return MA_BuyClose;
}

inline int wrapPosDirType(WTSDirectionType dirType)
{
	if (WDT_LONG == dirType)
		return MA_LONG;
	else if (WDT_SHORT == dirType)
		return MA_SHORT;
	else
		return MA_COVERED_CALL;
}

inline WTSDirectionType wrapPosDirType(MA_OPT_SIDE dirType)
{
	if (MA_LONG == dirType)
		return WDT_LONG;
	else if (MA_SHORT == dirType)
		return WDT_SHORT;
	else
		return WDT_NET;
}

inline WTSDirectionType wrapPosDirType(MA_STK_BIZ dirType)
{
	if (dirType == MA_BuyOpen || dirType == MA_BuyClose)
		return WDT_LONG;
	else if (dirType == MA_SellClose || dirType == MA_SellOpen)
		return WDT_SHORT;
	else
		return WDT_NET;
}

inline WTSDirectionType wrapDirectionType(MA_STK_BIZ dirType)
{
	if (dirType == MA_BuyOpen || dirType == MA_SellClose || dirType == MA_OPT_CALL)
		return WDT_LONG;
	else
		return WDT_SHORT;
}

inline WTSOffsetType wrapOffsetType(MA_STK_BIZ dirType)
{
	if (dirType == MA_BuyOpen || dirType == MA_SellOpen)
		return WOT_OPEN;
	else 
		return WOT_CLOSE;
}

inline const char* exchgO2I(const char* exchg)
{
	if (strncmp(exchg, "0", 1) == 0)
		return "SZSE";
	else
		return "SSE";
}

inline const char* exchgI2O(const char* exchg)
{
	if (strcmp(exchg, "SSE") == 0)
		return "15";
	else
		return "05";
}


inline int wrapStkBzAction(WTSPriceType priceType, WTSOrderFlag flag, bool isSH)
{
	
	if (WPT_LIMITPRICE == priceType && flag == WOF_NOR)
		return 130;

	if (WPT_LIMITPRICE == priceType && flag == WOF_FOK)
		return 131;

	if (isSH)
	{
		if (WPT_ANYPRICE == priceType && flag == WOF_NOR)
			return 134;

		if (WPT_ANYPRICE == priceType && flag == WOF_FOK)
			return 132;

		return 134;
	}
	else
	{
		if (WPT_ANYPRICE == priceType && flag == WOF_FOK)
			return 122;

		if (WPT_ANYPRICE == priceType && flag == WOF_FAK)
			return 121;

		return 124;
	}

}

inline WTSPriceType wrapStkBzAction(int priceType)
{
	if (132 == priceType || 134 == priceType || 133 == priceType || 121 == priceType || 122 == priceType || 125 == priceType)
		return WPT_ANYPRICE;
	else if (130 == priceType || 131 == priceType)
		return WPT_LIMITPRICE;
	else if (123 == priceType || 124 == priceType)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
}

inline WTSPriceType wrapStkBzAction(MA_STK_BIZ_ACTION priceType)
{
	if (MA_OPT_ANYPRICE_LEFT_LIMIT == priceType || MA_OPT_ANYPRICE_IOC == priceType || MA_OPT_ANYPRICE_FOK == priceType || MA_BESTFIVELEVELPRICE == priceType || MA_MARKEPRICE_ALLORDRAW == priceType || MA_MARKETPRICE_LEFT_CANCEL == priceType)
		return WPT_ANYPRICE;
	else if (MA_OPT_LIMITPRICE_GFD == priceType || MA_OPT_LIMITPRICE_FOK == priceType)
		return WPT_LIMITPRICE;
	else if (MA_BESTPRICE_THIS_SIDE == priceType || MA_BESTPRICE_OTHER_SIDE == priceType)
		return WPT_BESTPRICE;
	else
		return WPT_LASTPRICE;
}

inline int wrapStkBz(WTSDirectionType dType, WTSOffsetType oType)
{
	if (dType == WDT_LONG)
		return oType == WOT_OPEN ? 400 : 401;
	else
		return oType == WOT_OPEN ? 402 : 403;
}

inline WTSOrderState wrapOrderState(MAOrderState orderState)
{
	if (orderState == MA_Canceled || orderState == MA_Withdawn || orderState == MA_PartFilled_Withdrawn)
		return WOS_Canceled;
	else if (orderState == MA_Nottouched || orderState == MA_ToSubmit)
		return WOS_Nottouched;
	else if (orderState == MA_Submitted)
		return WOS_Submitting;
	else if (orderState == MA_Withdraw_NotFilled || orderState == MA_PartFilled_ToWithdraw)
		return WOS_Cancelling;
	else if (orderState == MA_AllFilled)
		return WOS_AllTraded;

	return WOS_Nottouched;
}

inline char wrapActionFlag(WTSActionFlag actionFlag)
{
	if (WAF_CANCEL == actionFlag)
		return MA_ORDER_CANCEL;
	else
		return MA_ORDER_NORMAL;
}

inline uint32_t makeRefID()
{
	static std::atomic<uint32_t> auto_refid(0);
	if (auto_refid == 0)
		auto_refid = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20220101, 0)) / 1000 * 100);
	return auto_refid.fetch_add(1);
}

TraderMAOpt::TraderMAOpt()
	: m_pUserAPI(NULL)
	, m_mapPosition(NULL)
	, m_ayOrders(NULL)
	, m_ayTrades(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_iRequestID(0)
	, m_optSink(NULL)
	, m_bscSink(NULL)
	, m_orderRef(makeRefID())
{

}


TraderMAOpt::~TraderMAOpt()
{

}

bool TraderMAOpt::init(WTSVariant* params)
{
	m_strUser = params->getCString("user");
	m_strPass = params->getCString("pass");

	m_strHost = params->getCString("ip");
	m_iPort = params->getInt32("port");

	m_strChannel = params->getCString("channel");
	m_strEncryptKey = params->getCString("encrypt_key");
	m_strUseScope = params->getCString("use_scope");
	m_strAuthType = params->getCString("auth_type");
	m_strEncryptType = params->getCString("encrypt_type");
	m_strAcctType = params->getCString("acct_type");

	m_strOptSite = params->getCString("op_site");

	WTSVariant* param = params->get("mamodule");
	if (param != NULL)
		m_strModule = getBinDir() + DLLHelper::wrap_module(param->asCString(), "");
	else
		m_strModule = getBinDir() + DLLHelper::wrap_module("maTradeApi", "lib");

	m_hInstMA = DLLHelper::load_library(m_strModule.c_str());

	return true;
}

void TraderMAOpt::release()
{
	if (m_pUserAPI)
	{
		//m_pUserAPI->RegisterSpi(NULL);
		delete m_pUserAPI;
		m_pUserAPI = NULL;
	}

	if (m_ayOrders)
		m_ayOrders->clear();

	if (m_mapPosition)
		m_mapPosition->clear();

	if (m_ayTrades)
		m_ayTrades->clear();
}

void TraderMAOpt::connect()
{
	// 创建Api实例
	m_pUserAPI = new CCliOptTradeApi();

	// 注册响应实例
	m_pUserAPI->RegisterSpi(this);

	// 注册账号信息，应该在ReqUserLogin调用之前
	CReqSysField stSysField = { 0 };
	strncpy(stSysField.szSvrAddress, m_strHost.c_str(), sizeof(stSysField.szSvrAddress) - 1);
	stSysField.nSvrPort = m_iPort;
	stSysField.uiTimeout = 30;
	stSysField.chDataSet = '1';  /// 推送自动订阅 '0' = 按连接，‘1’= 按账户，0 = 不订阅
	m_pUserAPI->RegisterServer(&stSysField);

	write_log(m_bscSink, LL_INFO, "[TraderMAOpt] Register server connecting address");

	int iRetCode;

	if (m_pUserAPI)
	{
		iRetCode = m_pUserAPI->Init();
	}

	if (_thrd_worker == NULL)
	{
		boost::asio::io_service::work work(_asyncio);
		_thrd_worker.reset(new StdThread([this]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_asyncio.run_one();
				//m_asyncIO.run();
			}
		}));
	}
}

void TraderMAOpt::disconnect()
{
	release();
}

bool TraderMAOpt::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t orderref = m_orderRef.fetch_add(1) + 1;
		fmtutil::format_to(buffer, "{}#{}#{}", m_strUser, m_lDate, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderMAOpt::registerSpi(ITraderSpi *listener)
{
	m_bscSink = listener;
	if (m_bscSink)
	{
		m_bdMgr = listener->getBaseDataMgr();

		m_optSink = listener->getOptSpi();
	}
}

uint32_t TraderMAOpt::genRequestID()
{
	return m_iRequestID.fetch_add(1) + 1;
}

int TraderMAOpt::login(const char* user, const char* pass, const char* productInfo)
{
	m_strUser = user;
	m_strPass = pass;

	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	doLogin();

	return 0;
}

int TraderMAOpt::doLogin()
{
	m_wrapperState = WS_LOGINING;

	CReqFixField stFixField = { 0 };
	strncpy(stFixField.szOpSite, m_strOptSite.c_str(), sizeof(stFixField.szOpSite) - 1);
	char chChannel = m_strChannel.at(0);
	stFixField.chChannel = m_strChannel.at(0);

	CReqOptUserLoginField req = { 0 };
	memset(&req, 0, sizeof(req));
	//strcpy(std::to_string(req.iIntOrg), m_strBroker);

	strncpy(req.szAcctType, m_strAcctType.c_str(), sizeof(req.szAcctType));
	strncpy(req.szAcctId, m_strUser.c_str(), sizeof(req.szAcctId));
	strncpy(req.szAuthData, m_strPass.c_str(), sizeof(req.szAuthData));
	req.chUseScope = m_strUseScope[0];
	req.chEncryptType = m_strEncryptType[0];
	req.chAuthType = m_strAuthType[0];
	strncpy(req.szEncryptKey, m_strEncryptKey.c_str(), sizeof(req.szEncryptKey));

	m_pUserAPI->RegisterAcct(&stFixField);
	write_log(m_bscSink, LL_INFO, "[TraderMAOpt] Register account info");

	int iRetCode;
	iRetCode = m_pUserAPI->Init();  /// 在ReqUserLogin调用之前
	if (iRetCode != 0)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt]Init failed, account error {}", m_pUserAPI->GetLastErrorText());

	iRetCode = m_pUserAPI->ReqUserLogin(&req, genRequestID());

	if (iRetCode != 0)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending login request failed: {}, error msg: {}", iRetCode, m_pUserAPI->GetLastErrorText());
	}
	else
	{
		write_log(m_bscSink, LL_INFO, "[TraderMAOpt] Sending login request successed: {}", iRetCode);
	}

	return 0;
}

int TraderMAOpt::logout()
{
	if (m_pUserAPI == NULL)
	{
		return -1;
	}

	return 0;
}

int TraderMAOpt::orderInsertOpt(WTSEntrust* entrust)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	if (entrust == NULL)
		return -1;

	if (entrust->getBusinessType() != BT_EXECUTE)
	{
		if (m_bscSink) write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Bz type not supported except option execution");
		return -1;
	}

	CReqOptOrderField req;
	memset(&req, 0, sizeof(req));

	req.llCuacctCode = m_llCuacctCode;
	req.llCustCode = m_llCustCode;
	strncpy(req.szStkbd, entrust->getExchg(), sizeof(req.szStkbd));
	//strncpy(req.szStkpbu, m_strStkPBU.c_str(), sizeof(req.szStkpbu));

	//if (strlen(entrust->getUserTag()) == 0)
	//{
	//	///报单引用
	//	fmt::format_to(req.ExecOrderRef, "{}", m_orderRef.fetch_add(0));

	//}
	//else
	//{
	//	uint32_t fid, sid, orderref;
	//	extractEntrustID(entrust->getEntrustID(), fid, sid, orderref);
	//	///报单引用
	//	fmt::format_to(req.ExecOrderRef, "{}", orderref);
	//}

	//if (strlen(entrust->getUserTag()) > 0)
	//{
	//	m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
	//	m_iniHelper.save();
	//}

	req.iStkBiz = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.llOrderQty = (long long)entrust->getVolume();
	strncpy(req.szOptNum, entrust->getCode(), sizeof(req.szOptNum));

	int iResult = m_pUserAPI->ReqOrder(&req, genRequestID());
	if (iResult)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Option execution order inserting failed: {}, order error: {}", iResult, m_pUserAPI->GetLastErrorText());
	}

	return 0;
}

int TraderMAOpt::orderActionOpt(WTSEntrustAction* action)
{   // 撤单请求

	//std::cout << " 委托批号:" << p_pRspField->iOrderBsn;
	//std::cout << " 合同序号:" << p_pRspField->szOrderId;
	//std::cout << " 资金代码:" << p_pRspField->llCuacctCode;
	//std::cout << " 委托价格:" << p_pRspField->szOrderPrice;
	//std::cout << " 委托数量:" << p_pRspField->llOrderQty;
	//std::cout << " 委托金额:" << p_pRspField->szOrderAmt;
	//std::cout << " 委托冻结金额:" << p_pRspField->szOrderFrzAmt;
	//std::cout << " 交易单元:" << p_pRspField->szStkpbu;
	//std::cout << " 交易板块:" << p_pRspField->szStkbd;
	//std::cout << " 证券账户:" << p_pRspField->szTrdacct;
	//std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
	//std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
	//std::cout << " 标的证券代码:" << p_pRspField->szStkCode;
	//std::cout << " 标的证券名称:" << p_pRspField->szStkName;
	//std::cout << " 证券业务代码:" << p_pRspField->iStkBiz;
	//std::cout << " 证券业务行为:" << p_pRspField->iStkBizAction;
	//std::cout << " 撤销状态:" << p_pRspField->chCancelStatus;
	//std::cout << " 外部合同序号:" << p_pRspField->szOrderIdEx << std::endl;

	if (m_wrapperState != WS_ALLREADY)
		return -1;

	if (action->getBusinessType() != BT_EXECUTE)
	{
		if (m_bscSink) write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Bz type not supported except option execution");
		return -1;
	}

	//uint32_t frontid, sessionid, orderref;
	//if (!extractEntrustID(action->getEntrustID(), frontid, sessionid, orderref))
	//	return -1;

	CReqOptCancelOrderField req = { 0 };

	memset(&req, 0, sizeof(req));
	///内部机构
	req.iIntOrg = m_iInitOrg;
	///客户代码
	req.llCuacctCode = m_llCuacctCode;
	/// 交易板块
	//strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	//req.iOrderBsn = 0
	strcpy(req.szOrderId, action->getOrderID());

	int iResult = m_pUserAPI->ReqCancelOrder(&req, genRequestID());
	if (iResult)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending cancel request of option execution order failed: {}, cancel error: {}", iResult, m_pUserAPI->GetLastErrorText());
	}

	return 0;
}

int TraderMAOpt::queryOrdersOpt(WTSBusinessType bType)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	if (bType != BT_EXECUTE)
	{
		if (m_bscSink) write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Bz type not supported except option execution");
		return -1;
	}

	CReqOptCurrDayOrderField req = { 0 };
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	//strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));
	req.chQueryFlag = '9';
	req.iQryNum = 10000;
	// req.szCombStraCode, "", sizeof(req.szCombStraCode));

	int iRet = m_pUserAPI->ReqQryCurrDayOrder(&req, genRequestID());
	if (iRet)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] ReqQryCurrDayOrder Error:: {}", m_pUserAPI->GetLastErrorText());

	return 0;
}

int TraderMAOpt::orderInsert(WTSEntrust* entrust)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	WTSContractInfo* ct = entrust->getContractInfo();

	if (ct != NULL)
	{
		int n = 10;
		for (int i = 0; i < 10; i++)
		{
		CReqOptOrderField req;
		memset(&req, 0, sizeof(req));
		/// 客户代码
		req.llCustCode = m_llCustCode;
		/// 资金账户代码
		req.llCuacctCode = m_llCuacctCode;
		/// 交易板块
		wt_strcpy(req.szStkbd, exchgI2O(ct->getExchg()));
		/// 交易账户
		wt_strcpy(req.szTrdacct, m_strTrdAcct.c_str());
		/// 交易单元
		if (strcmp(ct->getExchg(), "SSE") == 0)
			wt_strcpy(req.szStkpbu, m_strShPBU.c_str());
		else
			wt_strcpy(req.szStkpbu, m_strSzPBU.c_str());
		/// 内部机构
		req.iIntOrg = m_iInitOrg;

		wt_strcpy(req.szOrderIdEx, entrust->getUserTag());

		///报单价格条件: 限价
		req.iStkBizAction = wrapStkBzAction(entrust->getPriceType(), entrust->getOrderFlag(), strcmp(entrust->getExchg(), "SSE") == 0);
		///买卖方向:
		req.iStkBiz = wrapStkBz(entrust->getDirection(), entrust->getOffsetType());
		///价格
		fmt::format_to(req.szOrderPrice, "{:.5f}", entrust->getPrice());
		wt_strcpy(req.szOptNum, entrust->getCode());
		///数量: 1
		req.llOrderQty = (long long)entrust->getVolume();
		/// 加密方式
		req.chEncryptType = '0';
		/// 加密数据
		wt_strcpy(req.szEncryptKey, m_strEncryptKey.c_str());

		extractEntrustID(entrust->getEntrustID(), req.iOrderBsn);

		//write_log(m_bscSink, LL_INFO, "Order info: code: {}, qty: {}, price: {}, biz action: {}, side: {}", req.szOptNum, req.llOrderQty, req.szOrderPrice, req.iStkBizAction, req.iStkBiz);

		int iResult = m_pUserAPI->ReqOrder(&req, genRequestID());
		if (iResult)
		{
			write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Order inserting failed: {}, error: {}", iResult, m_pUserAPI->GetLastErrorText());
			return -1;
		}

		if (strlen(entrust->getUserTag()) > 0)
			m_eidCache.put(entrust->getEntrustID(), entrust->getUserTag());
		}
	}
	else
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Order insert failed. Invalid order!");
	}

	return 0;
}

int TraderMAOpt::orderAction(WTSEntrustAction* action)
{
	// 撤单请求
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	CReqOptCancelOrderField req = { 0 };
	memset(&req, 0, sizeof(req));
	///内部机构
	req.iIntOrg = m_iInitOrg;
	///客户代码
	req.llCuacctCode = m_llCuacctCode;
	/// 交易板块
	wt_strcpy(req.szStkbd, exchgI2O(action->getExchg()));
	wt_strcpy(req.szOrderId, action->getOrderID());
	int ordSN;
	extractEntrustID(action->getEntrustID(), ordSN);
	req.iOrderBsn = ordSN;

	int iResult = m_pUserAPI->ReqCancelOrder(&req, genRequestID());
	if (iResult)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending cancel request failed, code: {}, error: {}", req.szOrderId, iResult, m_pUserAPI->GetLastErrorText());
	else
		write_log(m_bscSink, LL_INFO, "[TraderMAOpt]  Sending cancel request successed, code: {}", req.szOrderId, iResult);

	return 0;
}

int TraderMAOpt::queryAccount()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		write_log(m_bscSink, LL_FATAL, "[TraderMAOpt] Query account info failed, check the trading api...");
		return -1;
	}

	// 可用资金
	CReqOptExpendableFundField stFieldFund = { 0 };
	stFieldFund.llCustCode = m_llCustCode;
	stFieldFund.llCuacctCode = m_llCuacctCode;
	stFieldFund.iValueFlag = 15;
	auto iRet = m_pUserAPI->ReqQryExpendableFund(&stFieldFund, genRequestID());
	if (iRet)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Query account fail, code: {}, err msg: {}", iRet, m_pUserAPI->GetLastErrorText());

	return 0;
}

int TraderMAOpt::queryPositions()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CReqOptExpendableCuField stField = { 0 };
	stField.llCustCode = m_llCustCode;
	stField.llCuacctCode = m_llCuacctCode;

	//strncpy(stField.szStkbd, m_strStkBD.c_str(), sizeof(stField.szStkbd));
	//strncpy(stField.szStkpbu, m_strStkPBU.c_str(), sizeof(stField.szStkpbu));
	strncpy(stField.szTrdacct, m_strTrdAcct.c_str(), sizeof(stField.szTrdacct));

	int iRet = m_pUserAPI->ReqQryExpendableCu(&stField, genRequestID());
	if (iRet)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Query positions fail, code: {}, err msg: {}", iRet, m_pUserAPI->GetLastErrorText());

	return 0;
}

int TraderMAOpt::queryOrders()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CReqOptCurrDayOrderField req = { 0 };
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	//strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));
	req.chQueryFlag = '1';
	req.iQryNum = 1000;
	//strcpy(req.szTrdacct, m_strUser.c_str());

	int iRet = m_pUserAPI->ReqQryCurrDayOrder(&req, genRequestID());
	if (iRet)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Query orders fail, code: {}, err msg: {}", iRet, m_pUserAPI->GetLastErrorText());

	return 0;
}

int TraderMAOpt::queryTrades()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CReqOptCurrDayFillField req;
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	//strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));

	req.chQueryFlag = '1';  // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
	req.iQryNum = 1000;  // 查询行数

	int iRet = m_pUserAPI->ReqQryCurrDayFill(&req, genRequestID());
	if (iRet)
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Query trades fail, code: {}, err msg: {}", iRet, m_pUserAPI->GetLastErrorText());

	return 0;
}

int TraderMAOpt::querySettlement(uint32_t uDate)
{
	_asyncio.post([this, uDate]() {
		if (m_bscSink)
			m_bscSink->onRspSettlementInfo(uDate, "No settlement data");
	});

	//if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	//{
	//	return -1;
	//}

	//CReqOptSettListField req;
	//memset(&req, 0, sizeof(req));
	//req.llCustCode = m_llCustCode;
	//req.llCuacctCode = m_llCuacctCode;
	//req.iTrdDate = uDate;
	//req.chCurrency = '0';

	//int iRet = m_pUserAPI->ReqQrySettList(&req, genRequestID());
	//if (iRet) {
	//	cout << __FUNCDNAME__ << "::" << __LINE__ << "::RRROR" << m_pUserAPI->GetLastErrorText();
	//}

	return 0;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szOptNum, exchgO2I(orderField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setPriceType(wrapStkBzAction(orderField->iStkBiz));
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setContractInfo(contract);

	if (orderField->iStkBiz == (int)MA_OPT_LIMITPRICE_GFD || orderField->iStkBiz == (int)MA_OPT_ANYPRICE_LEFT_LIMIT)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (orderField->iStkBiz == (int)MA_OPT_ANYPRICE_IOC)
	{
		//if (orderField->VolumeCondition == THOST_FTDC_VC_AV || orderField->VolumeCondition == THOST_FTDC_VC_MV)
		//	pRet->setOrderFlag(WOF_FAK);
		//else
		//	pRet->setOrderFlag(WOF_FOK);

		pRet->setOrderFlag(WOF_NOR);
	}

	pRet->setCode(orderField->szOptNum);
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate(m_lDate);
	pRet->setOrderTime(TimeUtils::getLocalTimeNow());

	pRet->setOrderID(orderField->szOrderId);
	pRet->setUserTag(orderField->szOrderIdEx);

	return pRet;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptCancelOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szStkCode, exchgO2I(orderField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setPriceType(wrapStkBzAction(orderField->iStkBiz));
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setContractInfo(contract);

	pRet->setOrderFlag(WOF_NOR);

	pRet->setCode(orderField->szStkCode);
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate(m_lDate);
	pRet->setOrderTime(TimeUtils::getLocalTimeNow());

	pRet->setOrderID(orderField->szOrderId);
	pRet->setUserTag(orderField->szOrderIdEx);
	pRet->setOrderState(WOS_Canceled);
	pRet->setStateMsg("Canceled");

	return pRet;
}


WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRtnOptOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szStkCode, exchgO2I(orderField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)orderField->iStkBiz));

	pRet->setVolTraded((double)orderField->llTotalMatchedQty);
	pRet->setVolLeft((double)(orderField->llOrderQty - orderField->llTotalMatchedQty));

	pRet->setCode(orderField->szStkCode);
	pRet->setExchange(contract->getExchg());

	generateEntrustID(pRet->getEntrustID(), orderField->iOrderBsn);
	pRet->setOrderID(orderField->szOrderId);

	const char* usertag = m_eidCache.get(pRet->getEntrustID());
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	pRet->setOrderDate(m_lDate);
	pRet->setOrderTime(TimeUtils::getLocalTimeNow());

	pRet->setOrderState(wrapOrderState((MAOrderState)orderField->chOrderStatus));
	if ((MAOrderState)orderField->chOrderStatus == MA_Canceled) {
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

	return pRet;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField)
{
	if (daiOrderField->chOrderValidFlag != '1' || daiOrderField->chIsWithdraw == 'T')
		return NULL;

	WTSContractInfo* contract = m_bdMgr->getContract(daiOrderField->szOptNum, exchgO2I(daiOrderField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(atof(daiOrderField->szOrderPrice));
	pRet->setBusinessType(BT_CASH);
	pRet->setVolume((double)daiOrderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((MA_STK_BIZ)daiOrderField->iStkBiz));
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)daiOrderField->iStkBiz));

	pRet->setVolTraded((double)daiOrderField->llMatchedQty);
	pRet->setVolLeft((double)(daiOrderField->llOrderQty - daiOrderField->llMatchedQty));

	pRet->setCode(daiOrderField->szOptNum);
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate(m_lDate);
	std::string strTime(daiOrderField->szOrderTime+11);
	StrUtil::replace(strTime, ":", "");
	StrUtil::replace(strTime, ".", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), uTime));

	pRet->setOrderState(wrapOrderState((MAOrderState)daiOrderField->chOrderStatus));
	if (daiOrderField->chOrderStatus == MA_Canceled)
	{
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

	generateEntrustID(pRet->getEntrustID(), daiOrderField->iOrderBsn);
	pRet->setOrderID(daiOrderField->szOrderId);
	pRet->setUserTag(daiOrderField->szOrderIdEx);
	pRet->setStateMsg(daiOrderField->szOfferRetMsg);

	return pRet;
}

WTSTradeInfo* TraderMAOpt::makeTradeRecord(CRtnOptOrderFillField *tradeField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->szStkCode, exchgO2I(tradeField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = contract->getCommInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(tradeField->szStkCode, commInfo->getExchg());
	pRet->setVolume((double)tradeField->llMatchedQty);
	pRet->setPrice(atof(tradeField->szMatchedPrice));
	pRet->setTradeID(tradeField->szMatchedSn);
	pRet->setContractInfo(contract);

	std::string strTime = tradeField->szMatchedTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	uint32_t uDate = tradeField->iMatchedDate;

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType((MA_STK_BIZ)tradeField->iStkBiz);

	pRet->setDirection(dType);
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)tradeField->iStkBiz));
	pRet->setRefOrder(tradeField->szOrderId);
	pRet->setTradeType((WTSTradeType)tradeField->chMatchedType);

	double amount = atof(tradeField->szMatchedAmt);
	pRet->setAmount(amount);

	thread_local static char entrustid[64] = { 0 };
	generateEntrustID(entrustid, tradeField->iOrderBsn);
	const char* usertag = m_eidCache.get(entrustid);
	if (strlen(usertag) > 0)
		pRet->setUserTag(usertag);

	return pRet;
}

WTSTradeInfo* TraderMAOpt::makeTradeRecord(CRspOptCurrDayFillField *tradeField)
{
	if (tradeField == NULL || tradeField->chIsWithdraw == 'T')
		return NULL;

	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->szOptNum, exchgO2I(tradeField->szStkbd));
	if (contract == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = contract->getCommInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(tradeField->szOptNum, commInfo->getExchg());
	pRet->setVolume((double)tradeField->llMatchedQty);
	pRet->setPrice(atof(tradeField->szMatchedPrice));
	pRet->setTradeID(tradeField->szMatchedSn);
	pRet->setContractInfo(contract);

	uint32_t uTime = strtoul(tradeField->szMatchedTime, NULL, 10);
	uint32_t uDate = tradeField->iTrdDate;

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType((MA_STK_BIZ)tradeField->iStkBiz);
	WTSOffsetType oType = wrapOffsetType((MA_STK_BIZ)tradeField->iStkBiz);

	pRet->setDirection(dType);
	pRet->setOffsetType(oType);
	pRet->setRefOrder(tradeField->szOrderId);
	pRet->setUserTag(tradeField->szOrderIdEx);
	pRet->setTradeType((WTSTradeType)tradeField->chMatchedType);

	double amount = atof(tradeField->szMatchedAmt);
	pRet->setAmount(amount);

	return pRet;
}

WTSEntrust* TraderMAOpt::makeEntrust(CRspOptOrderField *entrustField)
{
	if (entrustField == NULL)
		return NULL;

	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->szOptNum, exchgO2I(entrustField->szStkbd));
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		entrustField->szOptNum,
		(double)entrustField->llOrderQty,
		atof(entrustField->szOrderPrice),
		ct->getExchg());

	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)entrustField->iStkBiz));
	pRet->setPriceType(wrapStkBzAction((MA_STK_BIZ_ACTION)entrustField->iStkBizAction));
	pRet->setOffsetType(wrapOffsetType((MA_STK_BIZ)entrustField->iStkBiz));

	MA_STK_BIZ_ACTION timeCondition = (MA_STK_BIZ_ACTION)entrustField->iStkBizAction;
	if (timeCondition == MA_OPT_LIMITPRICE_GFD || timeCondition == MA_OPT_ANYPRICE_LEFT_LIMIT)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (timeCondition == MA_OPT_ANYPRICE_IOC)
	{
		/*if (entrustField->VolumeCondition == THOST_FTDC_VC_AV || entrustField->VolumeCondition == THOST_FTDC_VC_MV)
			pRet->setOrderFlag(WOF_FAK);
		else
			pRet->setOrderFlag(WOF_FOK);*/
		pRet->setOrderFlag(WOF_FOK);
	}

	//pRet->setEntrustID(generateEntrustID(m_frontID, m_sessionID, atoi(entrustField->OrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), entrustField->iOrderBsn);

	//StringMap::iterator it = m_mapEntrustTag.find(pRet->getEntrustID());
	//if (it != m_mapEntrustTag.end())
	//{
	//	pRet->setUserTag(it->second.c_str());
	//}

	//std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID());
	//if (!usertag.empty())
	//	pRet->setUserTag(usertag.c_str());

	return pRet;
}

WTSEntrust* TraderMAOpt::makeEntrust(CRtnOptOrderFillField *entrustField)
{
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->szStkCode, exchgO2I(entrustField->szStkbd));
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(entrustField->szStkCode, (double)entrustField->llMatchedQty, 0, ct->getExchg(), BT_CASH);
	pRet->setContractInfo(ct);

	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)entrustField->iStkBiz));
	pRet->setPriceType(wrapStkBzAction((MA_STK_BIZ_ACTION)entrustField->iStkBizAction));

	generateEntrustID(pRet->getEntrustID(), entrustField->iOrderBsn);

	//std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID());
	//if (!usertag.empty())
	//	pRet->setUserTag(usertag.c_str());

	return pRet;
}


WTSError* TraderMAOpt::makeError(CFirstSetField* rspInfo)
{
	WTSError* pRet = WTSError::create((WTSErroCode)rspInfo->iMsgCode, rspInfo->szMsgText);
	return pRet;
}


void TraderMAOpt::generateEntrustID(char* buffer, int order_bsn)
{
	fmtutil::format_to(buffer, "{}#{}#{}", m_strUser, m_lDate, order_bsn);
}

bool TraderMAOpt::extractEntrustID(const char* entrustid, int &order_bsn)
{
	//Market.FrontID.SessionID.OrderRef
	thread_local static char buffer[64];
	wt_strcpy(buffer, entrustid);
	char* s = buffer;
	auto idx = StrUtil::findLast(s, '#');
	if (idx == std::string::npos)
		return false;
	s += idx + 1;

	order_bsn = strtol(s, NULL, 10);

	return true;
}

bool TraderMAOpt::IsErrorRspInfo(CFirstSetField *pRspInfo)
{
	if (pRspInfo && pRspInfo->iMsgCode != 0)
		return true;

	return false;
}

bool TraderMAOpt::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}


int TraderMAOpt::OnConnected(void)
{
	_asyncio.post([this] {
		if (m_bscSink)
			m_bscSink->handleEvent(WTE_Connect, 0);
	});

	write_log(m_bscSink, LL_INFO, "[TraderMAOpt] Connected to server");

	return 0;
}

int TraderMAOpt::OnDisconnected(int p_nReason, const char *p_pszErrInfo)
{
	m_wrapperState = WS_NOTLOGIN;
	if (m_bscSink)
		m_bscSink->handleEvent(WTE_Close, p_nReason);

	return 0;
}

int TraderMAOpt::OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	write_log(m_bscSink, LL_DEBUG, "[TraderMAOpt][{}] Heartbeating...", m_strUser.c_str());

	return 0;
}

int TraderMAOpt::OnRspUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSet == NULL)
	{
		m_wrapperState = WS_LOGINFAILED;

		if (m_bscSink)
			m_bscSink->onLoginResult(false, "p_pFirstSet is NULL", 0);
		return -1;
	}


	if (p_iFieldNum > 0)
	{
		m_wrapperState = WS_LOGINED;
		m_strTrdAcct = p_pRspField->szTrdacct;

		if(p_pRspField->szStkbd[0] == '1')
			m_strShPBU = p_pRspField->szStkpbu;
		else
			m_strSzPBU = p_pRspField->szStkpbu;

		m_llCustCode = p_pRspField->llCustCode;
		m_llCuacctCode = p_pRspField->llCuacctCode;
		m_iInitOrg = p_pRspField->iIntOrg;

		m_lDate = TimeUtils::getCurDate();

		if (p_iFieldNum == p_iFieldIndex)
		{
			{
				// 初始化委托单缓存器
				std::stringstream ss;
				ss << "maOptData/";
				std::string path = StrUtil::standardisePath(ss.str());
				if (!StdFile::exists(path.c_str()))
					boost::filesystem::create_directories(path.c_str());
				ss << m_strUser << "_eid.sc";
				m_eidCache.init(ss.str().c_str(), m_lDate, [this](const char* message) {
					write_log(m_bscSink, LL_WARN, message);
				});
			}

			//{
			//	初始化订单标记缓存器
			//	std::stringstream ss;
			//	ss << "maOptData/";
			//	std::string path = StrUtil::standardisePath(ss.str());
			//	if (!StdFile::exists(path.c_str()))
			//		boost::filesystem::create_directories(path.c_str());
			//	ss << m_strUser << "_oid.sc";
			//	m_oidCache.init(ss.str().c_str(), m_lDate, [this](const char* message) {
			//		write_log(m_bscSink, LL_WARN, message);
			//	});
			//}

			write_log(m_bscSink, LL_INFO, "[TraderMAOpt][{}] Login succeed, trading date: {}...", m_strUser.c_str(), m_lDate);
			m_wrapperState = WS_ALLREADY;
			m_bscSink->onLoginResult(true, "", m_lDate);
		}
	}

	return 0;
}

int TraderMAOpt::OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		return -1;
	}

	//if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	//{
	//	write_log(m_bscSink, LL_INFO, "[TradeMAOpt][可用资金查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

	//	if (p_iFieldNum > 0)
	//	{
	//		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
	//	}
	//}

	if (p_iFieldNum > 0)
	{
		write_log(m_bscSink, LL_INFO, "[{}] Fund data updated, available: {}", m_strUser, p_pRspField->szFundAvl);
		write_log(m_bscSink, LL_INFO, "[{}] Fund data updated, frozen margin: {}", m_strUser, p_pRspField->szMarginInclRlt);

		WTSAccountInfo* accInfo = WTSAccountInfo::create();
		accInfo->setPreBalance(atof(p_pRspField->szFundPrebln));
		accInfo->setFrozenMargin(atof(p_pRspField->szMarginInclRlt));  // 已占用保证金(含未成交)
		accInfo->setFrozenCommission(atof(p_pRspField->szFundFeeFrz));  // 资金费用冻结金额
		accInfo->setDeposit(atof(p_pRspField->szDailyInAmt));  // 当日入金
		accInfo->setAvailable(atof(p_pRspField->szFundAvl));  // 资金可用金额
		accInfo->setWithdraw(atof(p_pRspField->szDailyOutAmt));  // 当日出金
		accInfo->setBalance(atof(p_pRspField->szFundBln));  // 资金余额

		if (m_bscSink)
		{
			WTSArray* ayFunds = WTSArray::create();
			ayFunds->append(accInfo, false);
			m_bscSink->onRspAccount(ayFunds);
			ayFunds->release();
		}
	}

	return 0;
}

int TraderMAOpt::OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		return -1;
	}

	//char            szOptCode[40 + 1];          // 合约代码 
	//char            szOptName[120 + 1];         // 合约简称 
	//char            chOptType;                  // 合约类型 字典[OPT_TYPE]
	//char            chOptSide;                  // 持仓方向 
	//char            chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
	//LONGLONG        llOptPrebln;                // 合约昨日余额 
	//LONGLONG        llOptBln;                   // 合约余额 
	//LONGLONG        llOptAvl;                   // 合约可用数量 
	//LONGLONG        llOptFrz;                   // 合约冻结数量 
	//LONGLONG        llOptUfz;                   // 合约解冻数量 
	//LONGLONG        llOptTrdFrz;                // 合约交易冻结数量 
	//LONGLONG        llOptTrdUfz;                // 合约交易解冻数量 
	//LONGLONG        llOptTrdOtd;                // 合约交易在途数量 
	//LONGLONG        llOptTrdBln;                // 合约交易轧差数量 
	//LONGLONG        llOptClrFrz;                // 合约清算冻结数量 
	//LONGLONG        llOptClrUfz;                // 合约清算解冻数量 
	//LONGLONG        llOptClrOtd;                // 合约清算在途数量 
	//char            szOptBcostRlt[21 + 1];      // 合约买入成本（实时） 
	//char            szOptPlamtRlt[21 + 1];      // 合约盈亏金额（实时） 
	//char            szOptMktVal[21 + 1];        // 合约市值 
	//char            szOptPremium[21 + 1];       // 权利金 
	//char            szOptMargin[21 + 1];        // 保证金 
	//LONGLONG        llOptCvdAsset;              // 备兑股份数量 
	//char            szOptClsProfit[21 + 1];     // 当日平仓盈亏 
	//char            szSumClsProfit[21 + 1];     // 累计平仓盈亏 
	//char            szOptFloatProfit[21 + 1];   // 浮动盈亏 浮动盈亏=证券市值-买入成本
	//char            szTotalProfit[21 + 1];      // 总盈亏 
	//LONGLONG        llOptRealPosi;              // 合约实际持仓 
	//LONGLONG        llOptClsUnmatched;          // 合约平仓挂单数量 即平仓委托未成交数量

	//if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	//{
	//	write_log(m_bscSink, LL_INFO, "[TradeMAOpt][可用合约查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

	//	if (p_iFieldNum > 0)
	//	{
	//		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
	//	}
	//}

	if (p_iFieldNum > 0 && p_pRspField != NULL)
	{
	/*	write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRspQryExpendableCu] code: {}, avail qty: {}, frozen qty: {}, side: {}, market value: {}, dyn profit: {}, premium: {}, margin: {}, close profit: {}, cum close profit: {}",
			p_pRspField->szOptCode, p_pRspField->llOptAvl, p_pRspField->llOptFrz, p_pRspField->chOptSide, p_pRspField->szOptMktVal,
			p_pRspField->szOptFloatProfit, p_pRspField->szOptPremium, p_pRspField->szOptMargin, p_pRspField->szOptClsProfit, p_pRspField->szSumClsProfit);*/

		if (NULL == m_mapPosition)
			m_mapPosition = PositionMap::create();

		WTSContractInfo* contract = m_bdMgr->getContract(p_pRspField->szOptNum, exchgO2I(p_pRspField->szStkbd));

		if (contract != NULL)
		{
			WTSCommodityInfo* commInfo = contract->getCommInfo();
			if (contract)
			{
				WTSDirectionType dType = wrapPosDirType((MA_OPT_SIDE)p_pRspField->chOptSide);
				std::string key = fmt::format("{}-{}", p_pRspField->szOptNum, dType);
				WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
				if (pos == NULL)
				{
					pos = WTSPositionItem::create(contract->getCode(), commInfo->getCurrency(), commInfo->getExchg());
					pos->setContractInfo(contract);
					m_mapPosition->add(key, pos, false);
				}

				pos->setDirection(dType);
				pos->setNewPosition((double)(p_pRspField->llOptBln + p_pRspField->llOptTrdBln));
				pos->setAvailNewPos((double)p_pRspField->llOptAvl);

				pos->setMargin(pos->getMargin() + atof(p_pRspField->szOptMargin));
				//pos->setDynProfit(pos->getDynProfit() + atof(p_pRspField->szOptClsProfit));
				pos->setDynProfit(pos->getDynProfit() + atof(p_pRspField->szOptFloatProfit));
				pos->setPositionCost(pos->getPositionCost() + atof(p_pRspField->szOptBcost));

				if (pos->getTotalPosition() != 0)
				{
					pos->setAvgPrice(pos->getPositionCost() / pos->getTotalPosition() / commInfo->getVolScale());
				}
				else
				{
					pos->setAvgPrice(0);
				}

				if (decimal::lt(pos->getTotalPosition(), 0.0) && decimal::eq(pos->getMargin(), 0.0))
				{
					//有仓位,但是保证金为0,则说明是套利合约,单个合约的可用持仓全部置为0
					pos->setAvailNewPos(0);
					pos->setAvailPrePos(0);
				}
			}
		}
	}

	if(p_iFieldNum == p_iFieldIndex)
	{
		WTSArray* ayPos = WTSArray::create();

		if (m_mapPosition && m_mapPosition->size() > 0)
		{
			for (auto it = m_mapPosition->begin(); it != m_mapPosition->end(); it++)
			{
				ayPos->append(it->second, true);
			}
		}

		if (m_bscSink)
			m_bscSink->onRspPosition(ayPos);

		if (m_mapPosition)
		{
			m_mapPosition->release();
			m_mapPosition = NULL;
		}

		ayPos->release();
	}

	return 0;
}

int TraderMAOpt::OnRspOrder(CFirstSetField *p_pFirstSetField, CRspOptOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL) {
		return -1;
	}

	//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRspOrder] Inserting order: {}, code: {}, qty: {}, side: {}, price: {}, frozen: {}", p_pRspField->szOrderId, 
	//	p_pRspField->szOptCode, p_pRspField->llOrderQty, p_pRspField->iStkBiz, p_pRspField->szOrderPrice, p_pRspField->szOrderFrzAmt);

	WTSEntrust* entrust = makeEntrust(p_pRspField);
	if (p_pFirstSetField->iMsgCode == 0)
	{
		if (m_bscSink)
			m_bscSink->onRspEntrust(entrust, NULL);

		WTSOrderInfo* ordInfo = makeOrderInfo(p_pRspField);
		if (m_bscSink)
			m_bscSink->onPushOrder(ordInfo);

		if (ordInfo)
			ordInfo->release();
	}
	else
	{
		WTSError *err = WTSError::create(WEC_ORDERINSERT, p_pFirstSetField->szMsgText);
		if (m_bscSink)
			m_bscSink->onRspEntrust(entrust, err);
		err->release();
	}

	if(entrust)
		entrust->release();
	
	return 0;
}

int TraderMAOpt::OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		return -1;
	}

	//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRspCancelOrder] Cancel order: {}, code: {}, qty: {}, side: {}, price: {}, frozen: {}", p_pRspField->szOrderId,
	//	p_pRspField->szStkCode, p_pRspField->llOrderQty, p_pRspField->iStkBiz, p_pRspField->szOrderPrice, p_pRspField->szOrderFrzAmt);

	if (p_pFirstSetField->iMsgCode != 0)
	{
		write_log(m_bscSink, LL_INFO, "[TraderMAOpt]Cancel [{}] failed, code: {}, msg: {}", p_pRspField->szOrderId, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgText);

		WTSError* error = WTSError::create(WEC_ORDERCANCEL, p_pFirstSetField->szMsgText);
		if (m_bscSink)
			m_bscSink->onTraderError(error);
	}
	else
	{
		write_log(m_bscSink, LL_INFO, "[TraderMAOpt]Cancel [{}] successed, code: {}, msg: {}", p_pRspField->szOrderId, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgText);

		WTSOrderInfo* ordInfo = makeOrderInfo(p_pRspField);
		if (m_bscSink)
			m_bscSink->onPushOrder(ordInfo);

		if (ordInfo)
			ordInfo->release();
	}

	return 0;
}

int TraderMAOpt::OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnOrderConfirm] code: {}, order id: {}, type: {}, qty: {}, price: {}", 
	//	p_pRtnField->szOptCode, p_pRtnField->szOrderId, p_pRtnField->chIsWithdraw == 'T' ? "Cancel" : "Normal", p_pRtnField->llOrderQty, p_pRtnField->szOrderPrice);

	/*std::cout << "交易市场:" << p_pRtnField->chStkex;
	std::cout << "证券代码:" << p_pRtnField->szStkCode;
	std::cout << "合同序号:" << p_pRtnField->szOrderId;
	std::cout << "交易账户:" << p_pRtnField->szTrdacct;
	std::cout << "撤单标志:" << p_pRtnField->chIsWithdraw;
	std::cout << "客户代码:" << p_pRtnField->llCustCode;
	std::cout << "资产账户:" << p_pRtnField->llCuacctCode;
	std::cout << "委托批号:" << p_pRtnField->iOrderBsn;
	std::cout << "账户序号:" << p_pRtnField->iCuacctSn;
	std::cout << "交易板块:" << p_pRtnField->szStkbd;
	std::cout << "委托状态:" << p_pRtnField->chOrderStatus;
	std::cout << "证券业务:" << p_pRtnField->iStkBiz;
	std::cout << "业务行为:" << p_pRtnField->iStkBizAction;
	std::cout << "委托日期" << p_pRtnField->iOrderDate;
	std::cout << "委托序号" << p_pRtnField->iOrderSn;
	std::cout << "内部机构" << p_pRtnField->iIntOrg;
	std::cout << "交易单元" << p_pRtnField->szStkpbu;
	std::cout << "委托价格" << p_pRtnField->szOrderPrice;
	std::cout << "委托数量" << p_pRtnField->llOrderQty;
	std::cout << "证券账户子编码" << p_pRtnField->szSubacctCode;
	std::cout << "期权合约账户" << p_pRtnField->szOptTrdacct;
	std::cout << "合约代码" << p_pRtnField->szOptCode;
	std::cout << "合约简称" << p_pRtnField->szOptName;
	std::cout << "货币代码" << p_pRtnField->chCurrency;
	std::cout << "标的证券类别" << p_pRtnField->chOptUndlCls;
	std::cout << "标的证券代码" << p_pRtnField->szOptUndlCode;
	std::cout << "标的证券名称" << p_pRtnField->szOptUndlName << std::endl;*/

	//TODO: 没回回调
	return 0;
}

//确认回报推送（极速）响应
int TraderMAOpt::OnRtnOrderConfirmFlash(CRtnOptOrderConfirmFlashField *p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	std::cout << "合同序号:" << p_pRtnField->szOrderId;
	std::cout << "撤单标志:" << p_pRtnField->chIsWithdraw;
	std::cout << "资产账户:" << p_pRtnField->llCuacctCode;
	std::cout << "交易板块:" << p_pRtnField->szStkbd;
	std::cout << "委托状态:" << p_pRtnField->chOrderStatus;
	std::cout << "委托日期" << p_pRtnField->iOrderDate;
	std::cout << "合约代码" << p_pRtnField->szOptCode << std::endl;

	//TODO: 没回回调
	return 0;
}

int TraderMAOpt::OnRtnOrder(CRtnOptOrderField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRtnOrder] Insert Order: order: {}, code: {}, qty: {}, side: {}, price: {}, mached qty: {}, order status: {}", p_pRtnField->szOrderId,
	//	p_pRtnField->szStkCode, p_pRtnField->llOrderQty, p_pRtnField->iStkBiz, p_pRtnField->szOrderPrice, p_pRtnField->llTotalMatchedQty, p_pRtnField->chOrderStatus);

	WTSOrderInfo *orderInfo = makeOrderInfo(p_pRtnField);
	if (orderInfo)
	{
		_asyncio.post([this, orderInfo] {
			if (m_bscSink)
				m_bscSink->onPushOrder(orderInfo);

			orderInfo->release();
		});
	}

	return 0;
}

int TraderMAOpt::OnRtnSubTopic(CRspSubTopicField* p_pRspField)
{
	if (p_pRspField == NULL)
	{
		return -1;
	}

	return 0;
}

int TraderMAOpt::OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		//std::cout << "Invalid filled order" << std::endl;
		return -1;
	}

	//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRtnOrderFill] Filled order: {}, code: {}, qty: {}, side: {}, price: {}, avail asset: {}, avail qty: {}, sett amt: {}", p_pRtnField->szOrderId,
	//	p_pRtnField->szStkCode, p_pRtnField->llMatchedQty, p_pRtnField->iStkBiz, p_pRtnField->szMatchedPrice, p_pRtnField->szFundAvl, p_pRtnField->llStkAvl, p_pRtnField->szRltSettAmt);

	WTSTradeInfo *tRecord = makeTradeRecord(p_pRtnField);
	if (tRecord)
	{
		_asyncio.post([this, tRecord] {
			if (m_bscSink)
				m_bscSink->onPushTrade(tRecord);

			tRecord->release();
		});
	}

	return 0;
}

int TraderMAOpt::OnRtnOrderFillFlash(CRtnOptOrderFillFlashField *p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRtnOrderFillFlash] Filled order: {}, code: {}, qty: {}, price: {}, avail asset: {}", p_pRtnField->szOrderId,
		p_pRtnField->szOptCode, p_pRtnField->llMatchedQty,  p_pRtnField->szMatchedPrice, p_pRtnField->szFundAvl);

	return 0;
}

int TraderMAOpt::OnRtnContract(CRtnOptContractField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRtnContract] code: {}, stk biz: {}, stk biz action: {}, margin pre frozen: {}, margin fronzen: {}, release pre margin: {}, release margin: {}",
		p_pRtnField->szOptCode, p_pRtnField->iStkBiz, p_pRtnField->iStkBizAction, p_pRtnField->szMarginPreFrz, p_pRtnField->szMarginFrz, p_pRtnField->szMarginPreUfz, p_pRtnField->szMarginUfz);

	return 0;
}

// 个股期权当日委托查询响应
int TraderMAOpt::OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspOptCurrDayOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		//triggerQuery();
		return -1;
	}

	if (p_iFieldNum > 0)
	{
		//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRspQryCurrDayOrder] order id: {}, code: {}, qty: {}, matched qty: {}, stk biz: {}, stk biz action: {}, price: {}, frozen amt: {}, unfrozen amt: {}",
		//	p_pRspField->szOrderId, p_pRspField->szOptCode, p_pRspField->llOrderQty, p_pRspField->llMatchedQty, p_pRspField->iStkBiz, p_pRspField->iStkBizAction, p_pRspField->szOrderPrice, p_pRspField->szOrderFrzAmt, p_pRspField->szOrderUfzAmt);

		if (NULL == m_ayOrders)
			m_ayOrders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(p_pRspField);
		if (orderInfo)
		{
			m_ayOrders->append(orderInfo, false);
		}
	}

	if(p_iFieldNum == p_iFieldIndex)
	{
		if (m_bscSink)
			m_bscSink->onRspOrders(m_ayOrders);

		if (m_ayOrders)
			m_ayOrders->clear();
	}
	return 0;
}

// 个股期权当日成交查询响应
int TraderMAOpt::OnRspQryCurrDayFill(CFirstSetField *p_pFirstSetField, CRspOptCurrDayFillField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		//triggerQuery();
		return -1;
	}


	if (p_iFieldNum > 0)
	{
		if (NULL == m_ayTrades)
			m_ayTrades = WTSArray::create();

		WTSTradeInfo* trade = makeTradeRecord(p_pRspField);
		if (trade)
		{
			m_ayTrades->append(trade, false);
		}

		//write_log(m_bscSink, LL_INFO, "[TraderMAOpt][OnRspQryCurrDayFill] order id: {}, code: {}, qty: {}, matched qty: {}, stk biz: {}, stk biz action: {}, price: {}, matched price: {}, frozen amt: {}, frozen margin: {}, unfrozen margin; {}, pre unfrozen margin: {}, fee: {}",
		//	p_pRspField->szOrderId, p_pRspField->szOptCode, p_pRspField->llOrderQty, p_pRspField->llMatchedQty, p_pRspField->iStkBiz, p_pRspField->iStkBizAction, p_pRspField->szOrderPrice, p_pRspField->szMatchedAmt, 
		//	p_pRspField->szOrderFrzAmt, p_pRspField->szMarginFrz, p_pRspField->szMarginUfz, p_pRspField->szMarginPreUfz, p_pRspField->szMatchedFee);
	}

	if(p_iFieldNum == p_iFieldIndex)
	{
		if (m_bscSink)
			m_bscSink->onRspTrades(m_ayTrades);

		if (NULL != m_ayTrades)
			m_ayTrades->clear();
	}
	return 0;
}

int TraderMAOpt::OnRspQrySettList(CFirstSetField* p_pFirstSet, CRspOptSettListField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSet == NULL)
	{
		return -1;
	}


	if (0 == p_pFirstSet->iMsgCode && NULL != p_pRspField)
	{
		cout << " 交易日期:" << p_pRspField->iTrdDate;
		cout << " 内部机构:" << p_pRspField->iIntOrg;
		cout << " 客户代码:" << p_pRspField->llCustCode;
		cout << " 客户名称:" << p_pRspField->szCustName;
		cout << " 资产账户:" << p_pRspField->llCuacctCode;
		cout << " 货币代码:" << p_pRspField->chCurrency;
		cout << " 期初结存:" << p_pRspField->szFundPrebln;
		cout << " 期末结存:" << p_pRspField->szFundBln;
		cout << " 可用资金:" << p_pRspField->szFundAvl;
		cout << " 行权资金冻结金额:" << p_pRspField->szFundExeFrz;
		cout << " 行权冻结维持保证金:" << p_pRspField->szFundExeMargin;
		cout << " 占用保证金:" << p_pRspField->szFundUsedMargin;
		cout << " 垫付资金:" << p_pRspField->szPaylater;
		cout << " 预计垫资罚息:" << p_pRspField->szExpectPnint;
		cout << " 归还垫资:" << p_pRspField->szRepayPaylater;
		cout << " 归还罚息:" << p_pRspField->szRepayPnint;
		cout << " 减免罚息:" << p_pRspField->szRemitPnint;
		cout << " 利息归本:" << p_pRspField->szInterstIncome;
		cout << " 利息税:" << p_pRspField->szInterstIncomeTax;
		cout << " 银衍入金:" << p_pRspField->szInAmt;
		cout << " 银衍出金:" << p_pRspField->szOutAmt;
		cout << " 权利金收付:" << p_pRspField->szPremiumPay;
		cout << " 行权收付:" << p_pRspField->szExecPay;
		cout << " 手续费:" << p_pRspField->szSXFFee;
		cout << " 结算费:" << p_pRspField->szQsfFee;
		cout << " 经手费:" << p_pRspField->szJsfFee;
		cout << " 交易所经手费:" << p_pRspField->szJysJsfFee;
		cout << " 行权过户费:" << p_pRspField->szXqghFee;
		cout << " 行权结算费:" << p_pRspField->szXqjsfFee;
		cout << " 行权手续费:" << p_pRspField->szXqsxfFee;
		cout << " 浮动盈亏:" << p_pRspField->llOptFloatProfit;
		cout << " 占用买入额度:" << p_pRspField->llQuotaValUsed;
		cout << " 买入额度:" << p_pRspField->llQuotaVal;
		cout << " 权利仓市值:" << p_pRspField->szRightMktVal;
		cout << " 义务仓市值:" << p_pRspField->szDuteMktVal;
		cout << " 保证金风险率:" << p_pRspField->szMarginRiskRate;
		cout << " 应追加保证金:" << p_pRspField->szAddMarginAmt;
		cout << " 追保通知内容:" << p_pRspField->szAddMarginNotice;
		cout << " 客户确认标志:" << p_pRspField->chConfirmFlag;
		cout << " 客户确认标志:" << p_pRspField->chConfirmFlag;
		cout << " 客户确认时间:" << p_pRspField->szConfirmTime << std::endl;
	}
	else
	{
		cout << " 客户结算单查询失败" << std::endl;
	}
	return 0;
}

