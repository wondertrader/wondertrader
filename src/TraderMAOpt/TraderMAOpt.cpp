/*!
 * \file TraderMAOpt.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
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
#pragma comment(lib, "../API/mCliApi3.7/x64/maTradeApi.lib")
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

void ShowErrorInfo(int iRetCode)
{
	printf("错误码:%d;  错误信息:", iRetCode);
	switch (iRetCode)
	{
	case -3:
		printf("invalid parameter\n");
		break;
	case -2:
		printf("invalid handle\n");
		break;
	case 100:
		printf("no data\n");
		break;
	case 101:
		printf("timeout\n");
		break;
	case 102:
		printf("exists\n");
		break;
	case 103:
		printf("more data\n");
		break;
	case 500:
		printf("call object function failed\n");
		break;
	case 501:
		printf("create object failed\n");
		break;
	case 502:
		printf("initialize object failed \n");
		break;
	case 503:
		printf("object uninitiated\n");
		break;
	case 504:
		printf("create resource failed\n");
		break;
	case 505:
		printf("dispatch event failed\n");
		break;
	case 506:
		printf("event  undefined \n");
		break;
	case 507:
		printf("register event {@1} from {@2} failed\n");
		break;
	case 508:
		printf("startup service {@1} failed\n");
		break;
	case 509:
		printf("init service env {@1} failed\n");
		break;
	case 510:
		printf("kernel/service env {@1} invalid\n");
		break;
	case 511:
		printf("service {@1} status not expect\n");
		break;
	case 512:
		printf("open internal queue {@1} failed\n");
		break;
	case 513:
		printf("open internal queue {@1} failed\n");
		break;
	case 514:
		printf("invalid message queue\n");
		break;
	case 515:
		printf("xml file {@1} format invalid\n");
		break;
	case 516:
		printf("open runtimedb {@1} failed\n");
		break;
	case 517:
		printf("create or initialize service function {@1}:{@2} fail \n");
		break;
	case 518:
		printf("option {@2} read only\n");
		break;
	case 519:
		printf("option {@2} unsupported \n");
		break;
	case 520:
		printf("purpose access {@2},but not granted\n");
		break;
	case 521:
		printf("queue {@1} fulled, max depth\n");
		break;
	case 522:
		printf("xa {@1} undefined\n");
		break;
	case 523:
		printf("call biz function {@1} exception\n");
		break;
	case 524:
		printf("timer {@1} callback failed, return\n");
		break;
	case 525:
		printf("filter expression {@1} invalid\n");
		break;
	case 526:
		printf("oem {@1} illegal\n");
		break;
	case 1000:
		printf("API基本错误\n");
		break;
	case 1001:
		printf("DLL缺失\n");
		break;
	case 1002:
		printf("DLL初始化失败(版本不对)\n");
		break;
	case 1003:
		printf("API实例已存在\n");
		break;
	case 1101:
		printf("insufficient space expect\n");
		break;
	case 1102:
		printf("receive packet from {@1} failed\n");
		break;
	case 1103:
		printf("send packet to {@1} failed\n");
		break;
	case 1104:
		printf("connect to {@1} failed\n");
		break;
	case 1105:
		printf("reconnect failed in function\n");
		break;
	case 1106:
		printf("reconnect {@1} success\n");
		break;
	case 1107:
		printf("disconnect\n");
		break;
	case 1100:
		printf("call zmq api {@2} failed\n");
		break;
	case 1200:
		printf("MA_ERROR_DB_EXCEPTION\n");
		break;
	case 1201:
		printf("data {@1} unload\n");
		break;
	case 1202:
		printf("table {@1} cursor {@2} has already opened\n");
		break;
	case 1203:
		printf("table {@1} cursor {@2} not opened\n");
		break;
	case 1204:
		printf("database {@1} not opened\n");
		break;
	case 1205:
		printf("invalid database connect string\n");
		break;
	case 1250:
		printf("MA_ERROR_DAO_EXCEPTION\n");
		break;
	case 1500:
		printf("call fix api {@2} failed\n");
		break;
	case 1501:
		printf("fix parse from {@1} failed\n");
		break;
	case 1502:
		printf("call kcbp api {@2} failed\n");
		break;
	case 1503:
		printf("invalid packet {@2} failed\n");
		break;
	case 1504:
		printf("call json api {@2} failed\n");
		break;
	case 1600:
		printf("call kcxp api {@2} failed\n");
		break;
	case 2000:
		printf("API套接字错误\n");
		break;
	case 2001:
		printf("客户端连接失败(请检查连接参数与服务器是否开启)\n");
		break;
	case 2002:
		printf("服务器创建失败\n");
		break;
	case 3000:
		printf("API配置错误\n");
		break;
	case 3001:
		printf("GTU节点配置文件错误\n");
		break;
	default:
		printf("尚无详细信息\n");
		break;

	}
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
			return MA_BuyClose;
	else
		if (offsetType == WOT_OPEN)
			return MA_SellOpen;
		else
			return MA_SellClose;
}

inline int wrapPosDirType(WTSDirectionType dirType)
{
	if (WDT_LONG == dirType)
		return MA_BuyOpen;
	else if (WDT_SHORT == dirType)
		return MA_SellOpen;
}

inline WTSDirectionType wrapPosDirType(StkBiz dirType)
{
	if (MA_BuyOpen == dirType)
		return WDT_LONG;
	else if (MA_SellOpen == dirType)
		return WDT_SHORT;
	else
		return WDT_NET;
}

inline WTSDirectionType wrapDirectionType(StkBiz dirType)
{
	if (dirType == MA_BuyOpen || dirType == MA_BuyClose)
		return WDT_LONG;
	else if (dirType == MA_SellOpen || dirType == MA_SellClose)
		return WDT_SHORT;
}

//inline int wrapOffsetType(WTSOffsetType offType)
//{
//	if (WOT_OPEN == offType)
//		return THOST_FTDC_OF_Open;
//	else if (WOT_CLOSE == offType)
//		return THOST_FTDC_OF_Close;
//	else if (WOT_CLOSETODAY == offType)
//		return THOST_FTDC_OF_CloseToday;
//	else if (WOT_CLOSEYESTERDAY == offType)
//		return THOST_FTDC_OF_Close;
//	else
//		return THOST_FTDC_OF_ForceClose;
//}

//inline WTSOffsetType wrapOffsetType(TThostFtdcOffsetFlagType offType)
//{
//	if (THOST_FTDC_OF_Open == offType)
//		return WOT_OPEN;
//	else if (THOST_FTDC_OF_Close == offType)
//		return WOT_CLOSE;
//	else if (THOST_FTDC_OF_CloseToday == offType)
//		return WOT_CLOSETODAY;
//	else
//		return WOT_FORCECLOSE;
//}

inline int wrapPriceType(WTSPriceType priceType, bool isCFFEX /* = false */)
{
	if (WPT_ANYPRICE == priceType)
		return isCFFEX ? 121 : 134;
	else if (WPT_LIMITPRICE == priceType)
		return 130;
	else if (WPT_BESTPRICE == priceType)
		return 134;
}

inline WTSPriceType wrapPriceType(int priceType)
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

inline int wrapTimeCondition(WTSTimeCondition timeCond)
{
	if (WTC_IOC == timeCond)
		return MA_IOC;
	else if (WTC_GFD == timeCond)
		return MA_GFD;
	else
		return MA_FOK;
}

inline WTSTimeCondition wrapTimeCondition(MATimeConditionType timeCond)
{
	if (MA_IOC == timeCond)
		return WTC_IOC;
	else if (MA_GFD == timeCond)
		return WTC_GFD;
	else
		return WTC_GFS;
}

inline WTSOrderState wrapOrderState(MAOrderState orderState)
{
	if (orderState == MA_Canceled)
		return WOS_Canceled;
	else if (orderState == MA_Nottouched || orderState == MA_ToSubmit)
		return WOS_Nottouched;
	else if (orderState == MA_Submitted)
		return WOS_Submitting;
	else if (orderState == MA_Withdraw_NotFilled)
		return WOS_PartTraded_Queuing;
	else if (orderState == MA_AllFilled)
		return WOS_AllTraded;
	else if (orderState == MA_PartFilled_Withdrawn)
		return WOS_PartTraded_NotQueuing;
}

inline char wrapActionFlag(WTSActionFlag actionFlag)
{
	if (WAF_CANCEL == actionFlag)
		return MA_ORDER_CANCEL;
	else
		return MA_ORDER_NORMAL;
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
	, m_accountInfo(WTSAccountInfo::create())
{
}


TraderMAOpt::~TraderMAOpt()
{

}

bool TraderMAOpt::init(WTSVariant* params)
{	
	m_strUser = params->getCString("user");
	m_strPass = params->getCString("pass");

	std::string str;
	m_llCustCode = atoll(m_strUser.c_str());
	str = params->getCString("cust_code");
	m_llCuacctCode = atoll(str.c_str());

	m_iInitOrg = params->getInt32("int_org");

	m_strHost = params->getCString("ip");
	m_iPort = params->getInt32("port");

	m_strchChannel = params->getCString("channel");
	m_strEncryptKey = params->getCString("encrypt_key");
	m_strUseScope = params->getCString("use_scope");
	m_strAuthType = params->getCString("auth_type");
	m_strEncryptType = params->getCString("encrypt_type");
	m_strAcctType = params->getCString("acct_type");
	m_strChannel = params->getCString("channel");

	std::cout << "ip address: " << m_strHost << "  port: " << m_iPort << "  cust code: " << m_llCustCode << "  account code: " << m_llCuacctCode << "\n";

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

	if (m_accountInfo) {
		delete m_accountInfo;
		m_accountInfo = nullptr;
	}
}

void TraderMAOpt::connect()
{
	// 创建Api实例
	m_pUserAPI = new CCliOptTradeApi();

	// 注册响应实例
	m_pUserAPI->RegisterSpi(this);

	m_pUserAPI->RegisterServer(m_strHost.c_str(), m_iPort);

	int iRetCode;

	if (m_pUserAPI)
	{
		iRetCode = m_pUserAPI->Init();
	}

	if (iRetCode != 0) {
		printf("init failed\n");
		ShowErrorInfo(iRetCode);
	}
	else {
		printf("init successfully\n");
	}

	if (_thrd_worker == NULL)
	{
		boost::asio::io_service::work work(_asyncio);
		_thrd_worker.reset(new StdThread([this]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(2));
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
		//fmt::format_to(buffer, "{:06d}#{:010d}#{:06d}", m_frontID, (uint32_t)m_sessionID, orderref);
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
	CReqOptUserLoginField req = { 0 };
	memset(&req, 0, sizeof(req));
	//strcpy(std::to_string(req.iIntOrg), m_strBroker);

	char chChannel = m_strChannel.at(0);

	strncpy(req.szAcctType, m_strAcctType.c_str(), sizeof(req.szAcctType));
	strncpy(req.szAcctId, m_strUser.c_str(), sizeof(req.szAcctId));
	strncpy(req.szAuthData, m_strPass.c_str(), sizeof(req.szAuthData));
	req.chUseScope = m_strUseScope[0];
	req.chEncryptType = m_strEncryptKey[0];
	req.chAuthType = m_strAuthType[0];
	strncpy(req.szEncryptKey, m_strEncryptKey.c_str(), sizeof(req.szEncryptKey));

	int iRetCode;
	iRetCode = m_pUserAPI->RegisterAcct(chChannel);
	if (iRetCode)
		std::cout << "RegisterAcct Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

	//strcpy(req.UserProductInfo, m_strProdInfo.c_str());

	iRetCode = m_pUserAPI->ReqUserLogin(&req, genRequestID());
	//iRetCode = m_pUserAPI->ReqUserLogin(&req, 1);
	if (iRetCode) {
		std::cout << "ReqUserLogin Error::" << m_pUserAPI->GetLastErrorText() << std::endl;
		ShowErrorInfo(iRetCode);
	}

	if (iRetCode != 0)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending login request failed: {}", iRetCode);
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

	//CThostFtdcUserLogoutField req;
	//memset(&req, 0, sizeof(req));
	//strcpy(req.BrokerID, m_strBroker.c_str());
	//strcpy(req.UserID, m_strUser.c_str());
	//int iResult = m_pUserAPI->ReqUserLogout(&req, genRequestID());
	//if (iResult != 0)
	//{
	//	write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending logout request failed: {}", iResult);
	//}

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

	/*CThostFtdcInputExecOrderField req;

	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_strBroker.c_str());
	///投资者代码
	strcpy(req.InvestorID, m_strUser.c_str());
	///合约代码
	strcpy(req.InstrumentID, entrust->getCode());

	strcpy(req.ExchangeID, entrust->getExchg());

	req.ActionType = THOST_FTDC_ACTP_Exec;

	if (strlen(entrust->getUserTag()) == 0)
	{
		///报单引用
		fmt::format_to(req.ExecOrderRef, "{}", m_orderRef.fetch_add(0));

	}
	else
	{
		uint32_t fid, sid, orderref;
		extractEntrustID(entrust->getEntrustID(), fid, sid, orderref);
		///报单引用
		fmt::format_to(req.ExecOrderRef, "{}", orderref);
	}

	if (strlen(entrust->getUserTag()) > 0)
	{
		m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
		m_iniHelper.save();
	}

	req.PosiDirection = wrapPosDirType(entrust->getDirection());
	req.OffsetFlag = wrapOffsetType(entrust->getOffsetType());
	req.HedgeFlag = THOST_FTDC_HF_Speculation;
	req.Volume = (int)entrust->getVolume();
	req.CloseFlag = THOST_FTDC_EOCF_AutoClose;

	int iResult = m_pUserAPI->ReqExecOrderInsert(&req, genRequestID());
	if (iResult != 0)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Option execution order inserting failed: {}", iResult);
	}*/

	return 0;
}

int TraderMAOpt::orderActionOpt(WTSEntrustAction* action)
{
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

	//LONGLONG        llCustCode;                 // 客户代码
	//LONGLONG        llCuacctCode;               // 资产账户 
	//int             iIntOrg;                    // 内部机构 
	//char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
	//char            szTrdacct[20 + 1];          // 证券账户 
	//LONGLONG        llOrderQty;                 // 委托数量 
	//char            szLeg1Num[16 + 1];          // 成分一合约编码 
	//char            szLeg2Num[16 + 1];          // 成分二合约编码 
	//int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
	//int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]，默认送100
	//char            szStkpbu[8 + 1];            // 交易单元 
	//int             iOrderBsn;                  // 委托批号 
	//char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
	//char            chSecurityLevel;            // 安全手段 0:无安全 1:校验票据 2:校验密码
	//char            szSecurityInfo[256 + 1];    // 安全信息 安全手段为校验票据则需填写票据 安全手段为密码则需填写交易密码密文
	//char            szOrderIdEx[64 + 1];        // 外部合同序号 此序号为外部传入，期权系统不做任何处理
	//char            chEncryptType;              // 加密方式 当安全手段为2：校验密码时，为必填入参。 加密方式: ‘0’:金证KBSS系统加密方式 ‘1’:金证Win版集中交易加密方式 ‘2’:金证Unix版集中交易加密方式 ‘3’:外部重加密方式 ‘4’:外部加密方式 说明：该入参根据接入券商要求填写，默认值为0。 外部重加密方式’3’：使用非金证加密函数将密码明文加密为密文后，然后使用金证公司提供的通信加密函数kbss_comencrypt对密文再一次加密。 外部加密方式’4’： 使用非金证加密函数将密码明文加密为密文后作为入参。
	//char            szEncryptKey[32 + 1];       // 加密因子 当安全手段为2：校验密码时，为必填入参。 


	CReqOptCombExeField req;
	memset(&req, 0, sizeof(req));
	///内部机构
	req.iIntOrg = m_iInitOrg;
	///客户代码
	req.llCustCode = m_llCustCode;
	///报单引用
	//fmt::format_to(req.ExecOrderRef, "{}", orderref);
	///请求编号
	///前置编号
	//req.FrontID = frontid;
	///会话编号
	//req.SessionID = sessionid;
	///操作标志
	//req.ActionFlag = wrapActionFlag(action->getActionFlag());
	///合约1代码
	strcpy(req.szLeg1Num, action->getCode());

	//strcpy(req.ExecOrderSysID, action->getOrderID());
	strcpy(req.szStkbd, action->getExchg());

	int iResult = m_pUserAPI->ReqCombExe(&req, genRequestID());
	if (iResult != 0)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending cancel request of option execution order failed: {}", iResult);
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

	CReqOptQryCombExeLegNumField req;
	memset(&req, 0, sizeof(req));
	//strcpy(req.BrokerID, m_strBroker.c_str());
	//strcpy(req.InvestorID, m_strUser.c_str());
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;

	m_pUserAPI->ReqQryCombExeLegNum(&req, genRequestID());

	return 0;
}

int TraderMAOpt::orderInsert(WTSEntrust* entrust)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	WTSContractInfo* ct = entrust->getContractInfo();
	if (ct == NULL)
		return -1;

	/*
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy(req.BrokerID, m_strBroker.c_str());
	///投资者代码
	strcpy(req.InvestorID, m_strUser.c_str());
	///合约代码
	strcpy(req.InstrumentID, entrust->getCode());

	strcpy(req.ExchangeID, entrust->getExchg());

	if (strlen(entrust->getUserTag()) == 0)
	{
		///报单引用
		fmt::format_to(req.OrderRef, "{}", m_orderRef.fetch_add(0));

	}
	else
	{
		uint32_t fid, sid, orderref;
		extractEntrustID(entrust->getEntrustID(), fid, sid, orderref);
		///报单引用
		fmt::format_to(req.OrderRef, "{}", orderref);
	}

	if (strlen(entrust->getUserTag()) > 0)
	{
		//m_mapEntrustTag[entrust->getEntrustID()] = entrust->getUserTag();
		m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
		m_iniHelper.save();
	}

	WTSCommodityInfo* commInfo = ct->getCommInfo();

	///用户代码
	//	TThostFtdcUserIDType	UserID;
	///报单价格条件: 限价
	req.OrderPriceType = wrapPriceType(entrust->getPriceType(), strcmp(commInfo->getExchg(), "CFFEX") == 0);
	///买卖方向:
	req.Direction = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = wrapOffsetType(entrust->getOffsetType());
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///价格
	req.LimitPrice = entrust->getPrice();
	///数量: 1
	req.VolumeTotalOriginal = (int)entrust->getVolume();

	if (entrust->getOrderFlag() == WOF_NOR)
	{
		req.TimeCondition = THOST_FTDC_TC_GFD;
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	else if (entrust->getOrderFlag() == WOF_FAK)
	{
		req.TimeCondition = THOST_FTDC_TC_IOC;
		req.VolumeCondition = THOST_FTDC_VC_AV;
	}
	else if (entrust->getOrderFlag() == WOF_FOK)
	{
		req.TimeCondition = THOST_FTDC_TC_IOC;
		req.VolumeCondition = THOST_FTDC_VC_CV;
	}

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
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Order inserting failed: {}", iResult);
	}
	*/

	return 0;
}

int TraderMAOpt::orderAction(WTSEntrustAction* action)
{
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	/*
	uint32_t frontid, sessionid, orderref;
	if (!extractEntrustID(action->getEntrustID(), frontid, sessionid, orderref))
		return -1;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	wt_strcpy(req.BrokerID, m_strBroker.c_str(), m_strBroker.size());
	wt_strcpy(req.InvestorID, m_strUser.c_str(), m_strUser.size());
	///报单引用
	fmt::format_to(req.OrderRef, "{}", orderref);
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

	req.VolumeChange = (int32_t)action->getVolume();

	strcpy(req.OrderSysID, action->getOrderID());
	strcpy(req.ExchangeID, action->getExchg());

	int iResult = m_pUserAPI->ReqOrderAction(&req, genRequestID());
	if (iResult != 0)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending cancel request failed: {}", iResult);
	}
	*/

	return 0;
}

int TraderMAOpt::queryAccount()
{
	//std::cout << "m_wrapperState: " << m_wrapperState << "\n";

	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		printf("查询账户失败");
		return -1;
	}

	CReqOptAcctField req = { 0 };
	memset(&req, 0, sizeof(req));

	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	req.chQueryFlag = '1';
	req.iQryNum = 13;
	strncpy(req.szStkbd, "STKBD", sizeof(req.szStkbd));
	
	//std::cout << " llcustcode: " << req.llCustCode << "  llcuacctcode: " << req.llCuacctCode << "  queryFlag: " << req.chQueryFlag << " szStkbd: " << req.szStkbd << "\n";

	int iRet = m_pUserAPI->ReqQryAcct(&req, genRequestID());
	if (iRet) {
		ShowErrorInfo(iRet);
		std::cout << "ReqQryAcct Error: " << m_pUserAPI->GetLastErrorText() << std::endl;
	}

	return 0;
}

int TraderMAOpt::queryPositions()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	
	//m_queQuery.push([this](){
	//	CThostFtdcQryInvestorPositionField req;
	//	memset(&req, 0, sizeof(req));
	//	strcpy(req.BrokerID, m_strBroker.c_str());
	//	strcpy(req.InvestorID, m_strUser.c_str());
	//	m_pUserAPI->ReqQryInvestorPosition(&req, genRequestID());
	//});

	return 0;
}

int TraderMAOpt::queryOrders()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CReqOptCurrDayOrderField req;
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	//strcpy(req.szTrdacct, m_strUser.c_str());

	m_pUserAPI->ReqQryCurrDayOrder(&req, genRequestID());

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
	//strcpy(req.szTrdacct, m_strUser.c_str());

	m_pUserAPI->ReqQryCurrDayFill(&req, genRequestID());

	return 0;
}

int TraderMAOpt::querySettlement(uint32_t uDate)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	CReqOptSettListField req;
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	req.iTrdDate = uDate;

	m_pUserAPI->ReqQrySettList(&req, genRequestID());

	return 0;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptOrderField* orderField)
{
	//std::cout << " 委托批号:" << p_pRspField->iOrderBsn;
	//std::cout << " 合同序号:" << p_pRspField->szOrderId;
	//std::cout << " 资金账户:" << p_pRspField->llCuacctCode;
	//std::cout << " 委托价格:" << p_pRspField->szOrderPrice;
	//std::cout << " 委托数量:" << p_pRspField->llOrderQty;
	//std::cout << " 委托金额:" << p_pRspField->szOrderAmt;
	//std::cout << " 委托冻结金额:" << p_pRspField->szOrderFrzAmt;
	//std::cout << " 交易单元:" << p_pRspField->szStkpbu;
	//std::cout << " 交易板块:" << p_pRspField->szStkbd;
	//std::cout << " 证券账户:" << p_pRspField->szTrdacct;
	//std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
	//std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
	//std::cout << " 合约编码:" << p_pRspField->szOptNum;
	//std::cout << " 合约代码:" << p_pRspField->szOptCode;
	//std::cout << " 标的证券代码:" << p_pRspField->szOptUndlCode;
	//std::cout << " 标的证券名称:" << p_pRspField->szOptUndlName;
	//std::cout << " 证券业务:" << p_pRspField->iStkBiz;
	//std::cout << " 证券业务行为:" << p_pRspField->iStkBizAction;
	//std::cout << " 外部合同序号:" << p_pRspField->szOrderIdEx << std::endl;

	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szOptCode, orderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapDirectionType((StkBiz)orderField->iStkBiz));
	pRet->setPriceType(wrapPriceType(orderField->iStkBiz));
	//pRet->setOffsetType(wrapOffsetType(orderField->CombOffsetFlag[0]));
	pRet->setContractInfo(contract);

	if (orderField->iStkBiz == 130 || orderField->iStkBiz == 132)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (orderField->iStkBiz == 134)
	{
		//if (orderField->VolumeCondition == THOST_FTDC_VC_AV || orderField->VolumeCondition == THOST_FTDC_VC_MV)
		//	pRet->setOrderFlag(WOF_FAK);
		//else
		//	pRet->setOrderFlag(WOF_FOK);
	}

	pRet->setCode(orderField->szOptCode);
	pRet->setExchange(contract->getExchg());

	/*pRet->setOrderDate(strtoul(orderField->InsertDate, NULL, 10));
	std::string strTime = orderField->InsertTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10) * 1000));*/

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef)).c_str());
	//generateEntrustID(pRet->getEntrustID(), orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef));
	pRet->setOrderID(orderField->szOrderId);

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
				write_log(m_bscSink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRtnOptOrderField* orderField)
{
	//std::cout << "    ==成交回报 委托信息==" << std::endl;
	//std::cout << " 交易市场:" << p_pRtnField->chStkex;
	//std::cout << " 交易板块:" << p_pRtnField->szStkbd;
	//std::cout << " 委托日期:" << p_pRtnField->iOrderDate;
	//std::cout << " 证券代码:" << p_pRtnField->szStkCode;
	//std::cout << " 委托批号:" << p_pRtnField->iOrderBsn;
	//std::cout << " 合同序号:" << p_pRtnField->szOrderId;
	//std::cout << " 客户代码:" << p_pRtnField->llCustCode;
	//std::cout << " 资产账户:" << p_pRtnField->llCuacctCode;
	//std::cout << " 账户序号:" << p_pRtnField->iCuacctSn;
	//std::cout << " 交易账户:" << p_pRtnField->szTrdacct;
	//std::cout << " 委托价格:" << p_pRtnField->szOrderPrice;
	//std::cout << " 委托数量:" << p_pRtnField->llOrderQty;
	//std::cout << " 委托金额:" << p_pRtnField->szOrderAmt;
	//std::cout << " 委托冻结金额:" << p_pRtnField->szOrderFrzAmt;
	//std::cout << " 委托状态:" << p_pRtnField->chOrderStatus;
	//std::cout << " 证券业务:" << p_pRtnField->iStkBiz;
	//std::cout << " 证券业务行为:" << p_pRtnField->iStkBizAction;
	//std::cout << " 已撤单数量:" << p_pRtnField->llWithdrawnQty;
	//std::cout << " 累计成交数量:" << p_pRtnField->llTotalMatchedQty;
	//std::cout << " 交易单元:" << p_pRtnField->szStkpbu << std::endl;

	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szStkCode, orderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((StkBiz)orderField->iStkBiz));
	//pRet->setOffsetType(wrapOffsetType(orderField->OffsetFlag));

	pRet->setVolTraded((double)orderField->llTotalMatchedQty);
	pRet->setVolLeft((double)(orderField->llOrderQty - orderField->llTotalMatchedQty));

	pRet->setCode(orderField->szStkCode);
	pRet->setExchange(contract->getExchg());

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef)).c_str());
	//generateEntrustID(pRet->getEntrustID(), orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef));
	pRet->setOrderID(orderField->szOrderId);


	pRet->setOrderDate(orderField->iOrderDate);

	pRet->setOrderState(wrapOrderState((MAOrderState)orderField->chOrderStatus));
	if ((MAOrderState)orderField->chOrderStatus == MA_Canceled) {
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

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
				write_log(m_bscSink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptCurrDayOrderField* daiOrderField)
{
	/*std::cout << "  定位串:" << p_pRspField->szQryPos;
	std::cout << "  交易日期:" << p_pRspField->iTrdDate;
	std::cout << "  委托日期:" << p_pRspField->iOrderDate;
	std::cout << " 委托时间:" << p_pRspField->szOrderTime;
	std::cout << "  委托批号:" << p_pRspField->iOrderBsn;
	std::cout << "  合同序号:" << p_pRspField->szOrderId;
	std::cout << " 委托状态:" << p_pRspField->chOrderStatus;
	std::cout << " 委托有效标志:" << p_pRspField->chOrderValidFlag;
	std::cout << "  内部机构:" << p_pRspField->iIntOrg;
	std::cout << "  客户代码:" << p_pRspField->llCustCode;
	std::cout << "  资产账户:" << p_pRspField->llCuacctCode;
	std::cout << "  交易市场:" << p_pRspField->chStkex;
	std::cout << "  交易板块:" << p_pRspField->szStkbd;
	std::cout << "  交易单元:" << p_pRspField->szStkpbu;
	std::cout << "  证券账户:" << p_pRspField->szTrdacct;
	std::cout << "  证券账户子编码:" << p_pRspField->szSubacctCode;
	std::cout << "  期权合约账户:" << p_pRspField->szOptTrdacct;
	std::cout << "  证券业务:" << p_pRspField->iStkBiz;
	std::cout << "  证券业务行为:" << p_pRspField->iStkBizAction;
	std::cout << "  订单所有类型:" << p_pRspField->szOwnerType;
	std::cout << "  合约编码:" << p_pRspField->szOptNum;
	std::cout << "  合约代码:" << p_pRspField->szOptCode;
	std::cout << "  合约简称:" << p_pRspField->szOptName;
	std::cout << "  组合编码:" << p_pRspField->szCombNum;
	std::cout << "  组合策略代码:" << p_pRspField->szCombStraCode;
	std::cout << "  成分一:" << p_pRspField->szLeg1Num;
	std::cout << "  成分二:" << p_pRspField->szLeg2Num;
	std::cout << "  成分三:" << p_pRspField->szLeg3Num;
	std::cout << "  成分四:" << p_pRspField->szLeg4Num;
	std::cout << "  货币代码:" << p_pRspField->chCurrency;
	std::cout << "  委托价格:" << p_pRspField->szOrderPrice;
	std::cout << "  委托数量:" << p_pRspField->llOrderQty;
	std::cout << "  委托金额:" << p_pRspField->szOrderAmt;
	std::cout << "  委托冻结金额:" << p_pRspField->szOrderFrzAmt;
	std::cout << " 委托解冻金额:" << p_pRspField->szOrderUfzAmt;
	std::cout << " 申报数量:" << p_pRspField->llOfferQty;
	std::cout << " 申报时间:" << p_pRspField->iOfferStime;
	std::cout << " 已撤单数量:" << p_pRspField->llWithdrawnQty;
	std::cout << "  已成交数量:" << p_pRspField->llMatchedQty;
	std::cout << "  已成交金额:" << p_pRspField->szMatchedAmt;
	std::cout << " 撤单标志:" << p_pRspField->chIsWithdraw;
	std::cout << " 已撤单标志:" << p_pRspField->chIsWithdrawn;
	std::cout << " 标的证券类别:" << p_pRspField->chOptUndlCls;
	std::cout << " 标的证券代码:" << p_pRspField->szOptUndlCode;
	std::cout << " 标的证券名称:" << p_pRspField->szOptUndlName;
	std::cout << " 标的券委托冻结数量:" << p_pRspField->llUndlFrzQty;
	std::cout << " 标的券委托解冻数量:" << p_pRspField->llUndlUfzQty;
	std::cout << " 标的券已撤单数量:" << p_pRspField->llUndlWthQty;
	std::cout << " 申报返回信息:" << p_pRspField->szOfferRetMsg;
	std::cout << "  外部合同序号:" << p_pRspField->szOrderIdEx;
	std::cout << " 委托序号:" << p_pRspField->iOrderSn;
	std::cout << " 外部合同序号:" << p_pRspField->szRawOrderId;
	std::cout << "  预占用保证金:" << p_pRspField->szMarginPreFrz;
	std::cout << "  占用保证金:" << p_pRspField->szMarginFrz;
	std::cout << "  预解冻保证金:" << p_pRspField->szMarginPreUfz;
	std::cout << "  解冻保证金:" << p_pRspField->szMarginUfz << std::endl;
*/
	WTSContractInfo* contract = m_bdMgr->getContract(daiOrderField->szOptCode, daiOrderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(0);
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume((double)daiOrderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((StkBiz)daiOrderField->iStkBiz));
	//pRet->setOffsetType(wrapOffsetType(daiOrderField->OffsetFlag));

	pRet->setVolTraded((double)daiOrderField->llMatchedQty);
	pRet->setVolLeft((double)(daiOrderField->llOrderQty - daiOrderField->llMatchedQty));

	pRet->setCode(daiOrderField->szOptCode);
	pRet->setExchange(contract->getExchg());

	pRet->setOrderState(WOS_Nottouched);
	if (daiOrderField->chOrderStatus == MA_Canceled)
	{
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef)).c_str());
	//generateEntrustID(pRet->getEntrustID(), daiOrderField->szRawOrderId, daiOrderField->iOrderSn, atoi(daiOrderField->szOrderIdEx));
	pRet->setOrderID(daiOrderField->szOrderIdEx);

	pRet->setOrderDate(daiOrderField->iOrderDate);
	std::string strTime = daiOrderField->szOrderTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10) * 1000));

	pRet->setOrderState(wrapOrderState((MAOrderState)daiOrderField->chOrderStatus));
	if (daiOrderField->chOrderStatus == MA_Canceled)
		pRet->setError(true);

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef)).c_str());
	//generateEntrustID(pRet->getEntrustID(), orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef));
	pRet->setOrderID(daiOrderField->szOrderIdEx);

	//pRet->setStateMsg(daiOrderField->chOrderValidFlag);

	pRet->setStateMsg(daiOrderField->szOfferRetMsg);


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
				write_log(m_bscSink, LL_ERROR, message);
			});
		}
	}

	return pRet;
}

WTSTradeInfo* TraderMAOpt::makeTradeRecord(CRtnOptOrderFillField *tradeField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->szStkCode, tradeField->szStkbd);
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

	WTSDirectionType dType = wrapDirectionType((StkBiz)tradeField->iStkBiz);

	pRet->setDirection(dType);
	//pRet->setOffsetType(wrapOffsetType(tradeField->OffsetFlag));
	pRet->setRefOrder(std::to_string(tradeField->iOrderBsn).c_str());
	pRet->setTradeType((WTSTradeType)tradeField->chMatchedType);

	double amount = atof(tradeField->szMatchedAmt);
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

WTSTradeInfo* TraderMAOpt::makeTradeRecord(CRspOptCurrDayFillField *tradeField)
{
	/*std::cout << " 定位串:" << p_pRspField->szQryPos;
	std::cout << " 交易日期:" << p_pRspField->iTrdDate;
	std::cout << " 成交时间:" << p_pRspField->szMatchedTime;
	std::cout << " 委托日期:" << p_pRspField->iOrderDate;
	std::cout << " 委托序号:" << p_pRspField->iOrderSn;
	std::cout << " 委托批号:" << p_pRspField->iOrderBsn;
	std::cout << " 合同序号:" << p_pRspField->szOrderId;
	std::cout << " 内部机构:" << p_pRspField->iIntOrg;
	std::cout << " 客户代码:" << p_pRspField->llCustCode;
	std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
	std::cout << " 交易市场:" << p_pRspField->chStkex;
	std::cout << " 交易板块:" << p_pRspField->szStkbd;
	std::cout << " 交易单元:" << p_pRspField->szStkpbu;
	std::cout << " 证券账户:" << p_pRspField->szTrdacct;
	std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
	std::cout << "  期权合约账户:" << p_pRspField->szOptTrdacct;
	std::cout << " 证券业务:" << p_pRspField->iStkBiz;
	std::cout << " 证券业务行为:" << p_pRspField->iStkBizAction;
	std::cout << " 订单所有类型:" << p_pRspField->szOwnerType;
	std::cout << " 合约编码:" << p_pRspField->szOptNum;
	std::cout << " 合约代码:" << p_pRspField->szOptCode;
	std::cout << " 合约简称:" << p_pRspField->szOptName;
	std::cout << "  组合编码:" << p_pRspField->szCombNum;
	std::cout << "  组合策略代码:" << p_pRspField->szCombStraCode;
	std::cout << "  成分一:" << p_pRspField->szLeg1Num;
	std::cout << "  成分二:" << p_pRspField->szLeg2Num;
	std::cout << "  成分三:" << p_pRspField->szLeg3Num;
	std::cout << "  成分四:" << p_pRspField->szLeg4Num;
	std::cout << " 货币代码:" << p_pRspField->chCurrency;
	std::cout << " 标的证券类别:" << p_pRspField->chOptUndlCls;
	std::cout << " 标的证券代码:" << p_pRspField->szOptUndlCode;
	std::cout << " 标的证券名称:" << p_pRspField->szOptUndlName;
	std::cout << " 委托价格:" << p_pRspField->szOrderPrice;
	std::cout << " 委托数量:" << p_pRspField->llOrderQty;
	std::cout << " 委托金额:" << p_pRspField->szOrderAmt;
	std::cout << " 委托冻结金额:" << p_pRspField->szOrderFrzAmt;
	std::cout << " 撤单标志:" << p_pRspField->chIsWithdraw;
	std::cout << " 成交类型:" << p_pRspField->chMatchedType;
	std::cout << " 成交编号:" << p_pRspField->szMatchedSn;
	std::cout << " 成交价格:" << p_pRspField->szMatchedPrice;
	std::cout << " 已成交数量:" << p_pRspField->llMatchedQty;
	std::cout << " 已成交金额:" << p_pRspField->szMatchedAmt;
	std::cout << " 外部合同序号:" << p_pRspField->szOrderIdEx;
	std::cout << " 预占用保证金:" << p_pRspField->szMarginPreFrz;
	std::cout << " 占用保证金:" << p_pRspField->szMarginFrz;
	std::cout << " 预解冻保证金:" << p_pRspField->szMarginPreUfz;
	std::cout << " 解冻保证金:" << p_pRspField->szMarginUfz;
	std::cout << " 成交费用:" << p_pRspField->szMatchedFee << std::endl;*/

	WTSContractInfo* contract = m_bdMgr->getContract(tradeField->szOptCode, tradeField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSCommodityInfo* commInfo = contract->getCommInfo();

	WTSTradeInfo *pRet = WTSTradeInfo::create(tradeField->szOptCode, commInfo->getExchg());
	pRet->setVolume((double)tradeField->llMatchedQty);
	pRet->setPrice(atof(tradeField->szMatchedPrice));
	pRet->setTradeID(tradeField->szMatchedSn);
	pRet->setContractInfo(contract);

	std::string strTime = tradeField->szMatchedTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	uint32_t uDate = tradeField->iTrdDate;

	//if(uDate == m_pContractMgr->getTradingDate())
	//{
	//	//如果当前日期和交易日一致,且时间大于21点,说明是夜盘,也就是实际日期要单独计算
	//	if (uTime / 10000 >= 21)
	//	{
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//	}
	//	else if(uTime <= 3)
	//	{
	//		//如果在3点以内,就要先获取上一个交易日,再获取下一个自然日
	//		//这样做的目的是,遇到周五晚上的情况,可以处理过来
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//		uDate = TimeUtils::getNextDate(uDate);
	//	}
	//}

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType((StkBiz)tradeField->iStkBiz);

	pRet->setDirection(dType);
	//pRet->setOffsetType(wrapOffsetType(tradeField->OffsetFlag));
	pRet->setRefOrder(std::to_string(tradeField->iOrderSn).c_str());
	pRet->setTradeType((WTSTradeType)tradeField->chMatchedType);

	double amount = atof(tradeField->szMatchedAmt);
	pRet->setAmount(amount);

	const char* usertag = m_oidCache.get(StrUtil::trim(pRet->getRefOrder()).c_str());
	if (strlen(usertag))
		pRet->setUserTag(usertag);

	return pRet;
}

/*
WTSEntrust* TraderMAOpt::makeEntrust(CThostFtdcInputOrderField *entrustField)
{
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->InstrumentID, entrustField->ExchangeID);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		entrustField->InstrumentID,
		entrustField->VolumeTotalOriginal,
		entrustField->LimitPrice,
		ct->getExchg());

	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType(entrustField->Direction, entrustField->CombOffsetFlag[0]));
	pRet->setPriceType(wrapPriceType(entrustField->OrderPriceType));
	pRet->setOffsetType(wrapOffsetType(entrustField->CombOffsetFlag[0]));

	if (entrustField->TimeCondition == THOST_FTDC_TC_GFD)
	{
		pRet->setOrderFlag(WOF_NOR);
	}
	else if (entrustField->TimeCondition == THOST_FTDC_TC_IOC)
	{
		if (entrustField->VolumeCondition == THOST_FTDC_VC_AV || entrustField->VolumeCondition == THOST_FTDC_VC_MV)
			pRet->setOrderFlag(WOF_FAK);
		else
			pRet->setOrderFlag(WOF_FOK);
	}

	//pRet->setEntrustID(generateEntrustID(m_frontID, m_sessionID, atoi(entrustField->OrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), m_frontID, m_sessionID, atoi(entrustField->OrderRef));

	//StringMap::iterator it = m_mapEntrustTag.find(pRet->getEntrustID());
	//if (it != m_mapEntrustTag.end())
	//{
	//	pRet->setUserTag(it->second.c_str());
	//}
	std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID());
	if (!usertag.empty())
		pRet->setUserTag(usertag.c_str());

	return pRet;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CThostFtdcExecOrderField* orderField)
{
	WTSContractInfo* contract = m_bdMgr->getContract(orderField->InstrumentID, orderField->ExchangeID);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(0);
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume(orderField->Volume);
	pRet->setDirection(wrapPosDirType(orderField->PosiDirection));
	pRet->setOffsetType(wrapOffsetType(orderField->OffsetFlag));

	pRet->setCode(orderField->InstrumentID);
	pRet->setExchange(contract->getExchg());

	pRet->setOrderDate(strtoul(orderField->InsertDate, NULL, 10));
	std::string strTime = orderField->InsertTime;
	StrUtil::replace(strTime, ":", "");
	uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10) * 1000));

	pRet->setOrderState(WOS_Nottouched);
	if (orderField->OrderSubmitStatus >= THOST_FTDC_OSS_InsertRejected)
	{
		pRet->setError(true);
		pRet->setOrderState(WOS_Canceled);
	}

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef));
	pRet->setOrderID(orderField->ExecOrderSysID);

	pRet->setStateMsg(orderField->StatusMsg);


	std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID(), "");
	if (usertag.empty())
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

WTSEntrust* TraderMAOpt::makeEntrust(CThostFtdcInputExecOrderField *entrustField)
{
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->InstrumentID, entrustField->ExchangeID);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(entrustField->InstrumentID, entrustField->Volume, 0, ct->getExchg(), BT_EXECUTE);
	pRet->setContractInfo(ct);

	pRet->setDirection(wrapPosDirType(entrustField->PosiDirection));
	pRet->setOffsetType(wrapOffsetType(entrustField->OffsetFlag));

	//pRet->setEntrustID(generateEntrustID(m_frontID, m_sessionID, atoi(entrustField->ExecOrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), m_frontID, m_sessionID, atoi(entrustField->ExecOrderRef));

	std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, pRet->getEntrustID());
	if (!usertag.empty())
		pRet->setUserTag(usertag.c_str());

	return pRet;
}


WTSError* TraderMAOpt::makeError(CThostFtdcRspInfoField* rspInfo)
{
	WTSError* pRet = WTSError::create((WTSErroCode)rspInfo->ErrorID, rspInfo->ErrorMsg);
	return pRet;
}
*/

/*
void TraderMAOpt::generateEntrustID(char* buffer, uint32_t frontid, uint32_t sessionid, uint32_t orderRef)
{
	fmtutil::format_to(buffer, "{:06d}#{:010d}#{:06d}", frontid, sessionid, orderRef);
}

bool TraderMAOpt::extractEntrustID(const char* entrustid, uint32_t &frontid, uint32_t &sessionid, uint32_t &orderRef)
{
	//Market.FrontID.SessionID.OrderRef
	thread_local static char buffer[64];
	wt_strcpy(buffer, entrustid);
	char* s = buffer;
	auto idx = StrUtil::findFirst(s, '#');
	if (idx == std::string::npos)
		return false;
	s[idx] = '\0';
	frontid = strtoul(s, NULL, 10);
	s += idx + 1;

	idx = StrUtil::findFirst(s, '#');
	if (idx == std::string::npos)
		return false;
	s[idx] = '\0';
	sessionid = strtoul(s, NULL, 10);
	s += idx + 1;

	orderRef = strtoul(s, NULL, 10);

	return true;
}

bool TraderMAOpt::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	if (pRspInfo && pRspInfo->ErrorID != 0)
		return true;

	return false;
}

void TraderMAOpt::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	WTSEntrust* entrust = makeEntrust(pInputOrder);
	if (entrust)
	{
		WTSError *err = makeError(pRspInfo);
		//g_orderMgr.	virtual int OnConnected(void) override;
Entrust(entrust, err);
		if (m_bscSink)
			m_bscSink->onRspEntrust(entrust, err);
		entrust->release();
		err->release();
	}
}

void TraderMAOpt::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	if (m_bscSink)
		m_bscSink->onPushInstrumentStatus(pInstrumentStatus->ExchangeID, pInstrumentStatus->InstrumentID, (WTSTradeStatus)pInstrumentStatus->InstrumentStatus);
}
*/

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

	return 0;
}

int TraderMAOpt::OnDisconnected(int p_nReason, const char *p_pszErrInfo)
{
	m_wrapperState = WS_NOTLOGIN;
	if (m_bscSink)
		m_bscSink->handleEvent(WTE_Close, p_nReason);
	std::cout << "客户端与服务器通信连接断开[" << p_nReason << "][" << p_pszErrInfo << "]" << std::endl;

	return 0;
}

int TraderMAOpt::OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	write_log(m_bscSink, LL_DEBUG, "[TraderMAOpt][{}] Heartbeating...", m_strUser.c_str());

	if (p_pFirstSet == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "p_pFirstSet is NULL");
		return -1;
	}
	if (p_iFieldNum == 0)
	{
		write_log(m_bscSink, LL_DEBUG, "[TraderMAOpt][MsgCode:{} MsgLevel:{} MsgDebug:{} MsgText:{}] Heartbeating...", p_pFirstSet->iMsgCode, p_pFirstSet->chMsgLevel, p_pFirstSet->szMsgDebug, p_pFirstSet->szMsgText);

	}
	if (/*p_iFieldNum > 0 &&*/ NULL != p_pRspField)
	{
		write_log(m_bscSink, LL_INFO,
			"[TraderMAOpt][{},{}]\nSeverName:{}, NodeID:{}, NodeGID:{}, NodePW:{}, NodeType:{}, NodeUse:{}, LocalNodeID:{}, BackupIP:{}",
			p_iFieldNum, p_iFieldIndex, p_pRspField->szServerSiteName, p_pRspField->szServerSiteIP, p_pRspField->szServerNodeId, p_pRspField->szServerNodeGid,
			p_pRspField->szServerNodePwd, p_pRspField->szServerNodeUse, p_pRspField->szServerLocalNodeId, p_pRspField->szServerBackIp);
	}

	return 0;
}

int TraderMAOpt::OnRspUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSet == NULL)
	{
		//printf("[%s: %d", __FUNCDNAME__, __LINE__);

		write_log(m_bscSink, LL_ERROR, "[TraderCTPOpt][p_pFirstSet is NULL]");
		m_wrapperState = WS_LOGINFAILED;

		if (m_bscSink)
			m_bscSink->onLoginResult(false, "p_pFirstSet is NULL", 0);
		return -1;
	}

	std::cout << "p_iFieldNum: " << p_iFieldNum << "  p_iFieldIndex: " << p_iFieldIndex << "\n";

	if (p_iFieldNum == 0)
	{
		printf("[%s: %d", __FUNCDNAME__, __LINE__);

		m_wrapperState = WS_LOGINING;

		write_log(m_bscSink, LL_INFO, "[TraderMAOpt][Login info: 用户登录请求返回结果: {},{},{},{}, 返回内容: {}]", p_pFirstSet->chMsgLevel, p_pFirstSet->iMsgCode, p_pFirstSet->szMsgDebug, p_pFirstSet->szMsgText, p_iFieldNum);
	}

	if (p_iFieldNum  == p_iFieldIndex)
	{
		std::cout << " 客户代码:" << p_pRspField->llCustCode;
		std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
		std::cout << " 交易市场:" << p_pRspField->chStkex;
		std::cout << " 交易板块:" << p_pRspField->szStkbd;
		std::cout << " 证券账户:" << p_pRspField->szTrdacct;
		std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
		std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
		std::cout << " 内部机构:" << p_pRspField->iIntOrg;
		std::cout << " 账户状态:" << p_pRspField->chTrdacctStatus;
		std::cout << " 交易单元:" << p_pRspField->szStkpbu;
		std::cout << " 账户类型:" << p_pRspField->szAcctType;
		std::cout << " 账户标识:" << p_pRspField->szAcctId;
		std::cout << " 账户名称:" << p_pRspField->szTrdacctName << std::endl;
		//std::cout << " 会话凭证:" << p_pRspField->szSession << std::endl;

		m_wrapperState = WS_LOGINED;

		/*write_log(m_bscSink, LL_INFO, "[TraderCTPOpt][{}-{}] Login succeed, 客户代码: {}, 资产账户: {}, 交易市场: {}， 交易板块: {}, 证券账户: {}, 证券账户子编码: {}, 期权合约账户: {}, 内部机构: {}, 账户状态: {}, 交易单元: {}, 账户类型: {}, 账户标识: {}, 账户名称: {}",
			p_pRspField->llCustCode, p_pRspField->llCuacctCode, p_pRspField->chStkex, std::string(p_pRspField->szStkbd), std::string(p_pRspField->szTrdacct), std::string(p_pRspField->szSubacctCode), std::string(p_pRspField->szOptTrdacct), p_pRspField->iIntOrg, p_pRspField->chTrdacctStatus, std::string(p_pRspField->szStkpbu), std::string(p_pRspField->szAcctType), std::string(p_pRspField->szAcctId), std::string(p_pRspField->szTrdacctName));
		*/

		{
			//初始化委托单缓存器
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

		{
			//初始化订单标记缓存器
			std::stringstream ss;
			ss << "maOptData/";
			std::string path = StrUtil::standardisePath(ss.str());
			if (!StdFile::exists(path.c_str()))
				boost::filesystem::create_directories(path.c_str());
			ss << m_strUser << "_oid.sc";
			m_oidCache.init(ss.str().c_str(), m_lDate, [this](const char* message) {
				write_log(m_bscSink, LL_WARN, message);
			});
		}

		write_log(m_bscSink, LL_INFO, "[TraderMAOpt][{}] Login succeed, trading date: {}...", m_strUser.c_str(), m_lDate);

		_asyncio.post([this] {
			m_bscSink->onLoginResult(true, "", 0);
		});

		m_wrapperState = WS_ALLREADY;
	}

	return 0;
}

int TraderMAOpt::OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][可用资金查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " 客户代码:" << p_pRspField->llCustCode;
		std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
		std::cout << " 货币代码:" << p_pRspField->chCurrency;
		std::cout << " 内部机构:" << p_pRspField->iIntOrg;
		std::cout << " 资产总值:" << p_pRspField->szMarketValue;
		std::cout << " 资金资产:" << p_pRspField->szFundValue;
		std::cout << " 市值:" << p_pRspField->szStkValue;
		std::cout << " 资金昨日余额:" << p_pRspField->szFundPrebln;
		std::cout << " 资金余额:" << p_pRspField->szFundBln;
		std::cout << " 资金可用金额:" << p_pRspField->szFundAvl;
		std::cout << " 资金冻结金额:" << p_pRspField->szFundFrz;
		std::cout << " 资金解冻金额:" << p_pRspField->szFundUfz;
		std::cout << " 资金交易冻结金额:" << p_pRspField->szFundTrdFrz;
		std::cout << " 资金交易解冻金额:" << p_pRspField->szFundTrdUfz;
		std::cout << " 资金交易在途金额:" << p_pRspField->szFundTrdOtd;
		std::cout << " 资金交易轧差金额:" << p_pRspField->szFundTrdBln;
		std::cout << " 资金状态:" << p_pRspField->chFundStatus;
		std::cout << " 占用保证金:" << p_pRspField->szMarginUsed;
		std::cout << " 已占用保证金(含未成交):" << p_pRspField->szMarginInclRlt;
		std::cout << " 行权锁定保证金:" << p_pRspField->szFundExeMargin;
		std::cout << " 行权资金冻结金额:" << p_pRspField->szFundExeFrz;
		std::cout << " 资金费用冻结金额:" << p_pRspField->szFundFeeFrz;
		std::cout << " 垫付资金:" << p_pRspField->szPaylater;
		std::cout << " 预计垫资金额:" << p_pRspField->szPreadvaPay;
		std::cout << " 预计负债利息:" << p_pRspField->szExpPenInt;
		std::cout << " 资金可取金额:" << p_pRspField->szFundDraw;
		std::cout << " 资金动态可用:" << p_pRspField->szFundAvlRlt;
		std::cout << " 动态占用保证金(含未成交):" << p_pRspField->szMarginInclDyn;
		std::cout << " 当日入金:" << p_pRspField->szDailyInAmt;
		std::cout << " 当日出金:" << p_pRspField->szDailyOutAmt;
		std::cout << " 资金实际可用:" << p_pRspField->szFundRealAvl << std::endl;

		m_accountInfo->setPreBalance(atof(p_pRspField->szFundPrebln)); // 资金昨日余额
		m_accountInfo->setCommission(atof(p_pRspField->szExpPenInt));  // 预计负债利息
		m_accountInfo->setFrozenMargin(atof(p_pRspField->szMarginInclRlt));  // 已占用保证金(含未成交)
		m_accountInfo->setFrozenCommission(atof(p_pRspField->szFundFeeFrz));  // 资金费用冻结金额
		m_accountInfo->setDeposit(atof(p_pRspField->szDailyInAmt));  // 当日入金
		m_accountInfo->setAvailable(atof(p_pRspField->szFundAvl));  // 资金可用金额
		m_accountInfo->setWithdraw(atof(p_pRspField->szDailyOutAmt));  // 当日出金
		m_accountInfo->setBalance(atof(p_pRspField->szFundBln));  // 资金余额
	}

	return 0;
}

int TraderMAOpt::OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
			std::cout << " 定位串:" << p_pRspField->szQryPos;
			std::cout << " 客户代码:" << p_pRspField->llCustCode;
			std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
			std::cout << " 内部机构:" << p_pRspField->iIntOrg;
			std::cout << " 交易市场:" << p_pRspField->chStkex;
			std::cout << " 交易板块:" << p_pRspField->szStkbd;
			std::cout << " 交易单元:" << p_pRspField->szStkpbu;
			std::cout << " 证券账户:" << p_pRspField->szTrdacct;
			std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
			std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
			std::cout << " 货币代码:" << p_pRspField->chCurrency;
			std::cout << " 合约编码:" << p_pRspField->szOptNum;
			std::cout << " 合约代码:" << p_pRspField->szOptCode;
			std::cout << " 合约简称:" << p_pRspField->szOptName;
			std::cout << " 合约类型:" << p_pRspField->chOptType;
			std::cout << " 持仓方向:" << p_pRspField->chOptSide;
			std::cout << " 备兑标志:" << p_pRspField->chOptCvdFlag;
			std::cout << " 合约昨日余额:" << p_pRspField->llOptPrebln;
			std::cout << " 合约余额:" << p_pRspField->llOptBln;
			std::cout << " 合约可用数量:" << p_pRspField->llOptAvl;
			std::cout << " 合约冻结数量:" << p_pRspField->llOptFrz;
			std::cout << " 合约解冻数量:" << p_pRspField->llOptUfz;
			std::cout << " 合约交易冻结数量:" << p_pRspField->llOptTrdFrz;
			std::cout << " 合约交易解冻数量:" << p_pRspField->llOptTrdUfz;
			std::cout << " 合约交易在途数量:" << p_pRspField->llOptTrdOtd;
			std::cout << " 合约交易轧差数量:" << p_pRspField->llOptTrdBln;
			std::cout << " 合约清算冻结数量:" << p_pRspField->llOptClrFrz;
			std::cout << " 合约清算解冻数量:" << p_pRspField->llOptClrUfz;
			std::cout << " 合约清算在途数量:" << p_pRspField->llOptClrOtd;
			std::cout << " 合约买入成本:" << p_pRspField->szOptBcost;
			std::cout << " 合约买入成本（实时）:" << p_pRspField->szOptBcostRlt;
			std::cout << " 合约盈亏金额:" << p_pRspField->szOptPlamt;
			std::cout << " 合约盈亏金额（实时）:" << p_pRspField->szOptPlamtRlt;
			std::cout << " 合约市值:" << p_pRspField->szOptMktVal;
			std::cout << " 权利金:" << p_pRspField->szOptPremium;
			std::cout << " 保证金" << p_pRspField->szOptMargin;
			std::cout << " 备兑股份数量:" << p_pRspField->llOptCvdAsset;
			std::cout << " 当日平仓盈亏:" << p_pRspField->szOptClsProfit;
			std::cout << " 累计平仓盈亏:" << p_pRspField->szSumClsProfit;
			std::cout << " 浮动盈亏:" << p_pRspField->szOptFloatProfit;
			std::cout << " 总盈亏:" << p_pRspField->szTotalProfit;
			std::cout << " 合约实际持仓:" << p_pRspField->llOptRealPosi;
			std::cout << " 合约平仓挂单数量:" << p_pRspField->llOptClsUnmatched;
			std::cout << " 当日累计开仓数量:" << p_pRspField->llOptDailyOpenRlt;
			std::cout << " 标的证券代码:" << p_pRspField->szOptUndlCode << std::endl;

	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][可用合约资产查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		m_accountInfo->setCloseProfit(atof(p_pRspField->szOptClsProfit));   // 当日平仓盈亏
		m_accountInfo->setDynProfit(atof(p_pRspField->szOptFloatProfit));  // 浮动盈亏
		m_accountInfo->setMargin(atof(p_pRspField->szOptMargin));  // 保证金
		m_accountInfo->setCurrency("CNY");

		//if (NULL == m_mapPosition)
		//	m_mapPosition = PositionMap::create();

		//WTSContractInfo* contract = m_bdMgr->getContract(p_pRspField->szOptCode, p_pRspField->szStkbd);
		//if (contract == NULL)
		//	return;

		//WTSCommodityInfo* commInfo = contract->getCommInfo();
		//if (contract)
		//{
		//	std::string key = fmt::format("{}-{}", p_pRspField->szOptCode, p_pRspField->szStkbd);
		//	WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
		//	if (pos == NULL)
		//	{
		//		pos = WTSPositionItem::create(p_pRspField->szOptCode, commInfo->getCurrency(), commInfo->getExchg());
		//		pos->setContractInfo(contract);
		//		m_mapPosition->add(key, pos, false);
		//	}
		//	//pos->setDirection(wrapPosDirection(p_pRspField->));
		//	if (commInfo->getCoverMode() == CM_CoverToday)
		//	{
		//		//if (p_pRspField->PositionDate == THOST_FTDC_PSD_Today)
		//		//	pos->setNewPosition(p_pRspField->llOptBln);
		//		//else
		//		//	pos->setPrePosition(p_pRspField->llOptPrebln);
		//	}
		//	else
		//	{
		//		pos->setNewPosition(p_pRspField->llOptBln);
		//		pos->setPrePosition(p_pRspField->llOptBln - p_pRspField->llOptPrebln);
		//	}

		//	pos->setMargin(pos->getMargin() + p_pRspField->UseMargin);
		//	pos->setDynProfit(pos->getDynProfit() + p_pRspField->PositionProfit);
		//	pos->setPositionCost(pos->getPositionCost() + p_pRspField->PositionCost);

		//	if (pos->getTotalPosition() != 0)
		//	{
		//		pos->setAvgPrice(pos->getPositionCost() / pos->getTotalPosition() / commInfo->getVolScale());
		//	}
		//	else
		//	{
		//		pos->setAvgPrice(0);
		//	}

		//	if (commInfo->getCategoty() != CC_Combination)
		//	{
		//		if (commInfo->getCoverMode() == CM_CoverToday)
		//		{
		//			if (p_pRspField->PositionDate == THOST_FTDC_PSD_Today)
		//			{
		//				int availNew = p_pRspField->Position;
		//				if (p_pRspField->PosiDirection == THOST_FTDC_PD_Long)
		//				{
		//					availNew -= p_pRspField->ShortFrozen;
		//				}
		//				else
		//				{
		//					availNew -= p_pRspField->LongFrozen;
		//				}
		//				if (availNew < 0)
		//					availNew = 0;
		//				pos->setAvailNewPos(availNew);
		//			}
		//			else
		//			{
		//				int availPre = p_pRspField->Position;
		//				if (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Long)
		//				{
		//					availPre -= pInvestorPosition->ShortFrozen;
		//				}
		//				else
		//				{
		//					availPre -= pInvestorPosition->LongFrozen;
		//				}
		//				if (availPre < 0)
		//					availPre = 0;
		//				pos->setAvailPrePos(availPre);
		//			}
		//		}
		//		else
		//		{
		//			int availNew = p_pRspField->TodayPosition;
		//			if (p_pRspField->PosiDirection == THOST_FTDC_PD_Long)
		//			{
		//				availNew -= p_pRspField->ShortFrozen;
		//			}
		//			else
		//			{
		//				availNew -= p_pRspField->LongFrozen;
		//			}
		//			if (availNew < 0)
		//				availNew = 0;
		//			pos->setAvailNewPos(availNew);

		//			double availPre = pos->getNewPosition() + pos->getPrePosition()
		//				- p_pRspField->LongFrozen - p_pRspField->ShortFrozen
		//				- pos->getAvailNewPos();
		//			pos->setAvailPrePos(availPre);
		//		}
		//	}
		//	else
		//	{

		//	}

		//	if (decimal::lt(pos->getTotalPosition(), 0.0) && decimal::eq(pos->getMargin(), 0.0))
		//	{
		//		//有仓位,但是保证金为0,则说明是套利合约,单个合约的可用持仓全部置为0
		//		pos->setAvailNewPos(0);
		//		pos->setAvailPrePos(0);
		//	}
		//}
	}

	return 0;
}

int TraderMAOpt::OnRspQryTradingAccount(void)
{
	WTSArray * ay = WTSArray::create();
	ay->append(m_accountInfo, false);

	_asyncio.post([this, ay] {
		if (m_bscSink)
			m_bscSink->onRspAccount(ay);

		ay->release();
	});
	
	return 0;
}

int TraderMAOpt::OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][股期权合约账户查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " 定位串:" << p_pRspField->szQryPos;
		std::cout << " 客户代码:" << p_pRspField->llCustCode;
		std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
		std::cout << " 交易市场:" << p_pRspField->chStkex;
		std::cout << " 账户标识:" << p_pRspField->szStkbd;
		std::cout << " 证券账户:" << p_pRspField->szTrdacct;
		std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
		std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
		std::cout << " 个股期权账户级别:" << p_pRspField->chOptTrdacctLvl;
		std::cout << " 内部机构:" << p_pRspField->iIntOrg;
		std::cout << " 账户序号:" << p_pRspField->iTrdacctSn;
		std::cout << " 报盘账户:" << p_pRspField->szTrdacctExid;
		std::cout << " 账户状态:" << p_pRspField->chTrdacctStatus;
		std::cout << " 指定状态:" << p_pRspField->chTregStatus;
		std::cout << " 交易单元:" << p_pRspField->szStkpbu;
		std::cout << " 证件类型:" << p_pRspField->szIdType;
		std::cout << " 证件号码:" << p_pRspField->szIdCode;
		std::cout << " 客户名称:" << p_pRspField->szCustName;
		//std::cout<< " 保证金比例模板编号:"<< p_pRspField->iOptMarginSn ;
		std::cout << " 买入额度:" << p_pRspField->szQuotaVal;
		std::cout << " 占用买入额度:" << p_pRspField->szQuotaValUsed;
		std::cout << " 操作标志:" << p_pRspField->chOpFlag;
		std::cout << " 当日询价次数:" << p_pRspField->llDailyEnqryCount << std::endl;
	}

	//TODO: 没回回调

	return 0;
}

int TraderMAOpt::OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");

		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][个股期权委托撤单请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " 委托批号:" << p_pRspField->iOrderBsn;
		std::cout << " 合同序号:" << p_pRspField->szOrderId;
		std::cout << " 资金代码:" << p_pRspField->llCuacctCode;
		std::cout << " 委托价格:" << p_pRspField->szOrderPrice;
		std::cout << " 委托数量:" << p_pRspField->llOrderQty;
		std::cout << " 委托金额:" << p_pRspField->szOrderAmt;
		std::cout << " 委托冻结金额:" << p_pRspField->szOrderFrzAmt;
		std::cout << " 交易单元:" << p_pRspField->szStkpbu;
		std::cout << " 交易板块:" << p_pRspField->szStkbd;
		std::cout << " 证券账户:" << p_pRspField->szTrdacct;
		std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
		std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
		std::cout << " 标的证券代码:" << p_pRspField->szStkCode;
		std::cout << " 标的证券名称:" << p_pRspField->szStkName;
		std::cout << " 证券业务代码:" << p_pRspField->iStkBiz;
		std::cout << " 证券业务行为:" << p_pRspField->iStkBizAction;
		std::cout << " 撤销状态:" << p_pRspField->chCancelStatus;
		std::cout << " 外部合同序号:" << p_pRspField->szOrderIdEx << std::endl;
	}

	//TODO: 没回回调

	return 0;
}

int TraderMAOpt::OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}
	std::cout << "交易市场:" << p_pRtnField->chStkex;
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
	std::cout << "标的证券名称" << p_pRtnField->szOptUndlName;

	//TODO: 没回回调
	return 0;
}

int TraderMAOpt::OnRtnOrder(CRtnOptOrderField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

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

int TraderMAOpt::OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

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

int TraderMAOpt::OnRtnContract(CRtnOptContractField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}

	std::cout << "    ==  ==  ==成交回报 合约信息==  ==  ==" << std::endl;
	std::cout << " 交易市场:" << p_pRtnField->chStkex;
	std::cout << " 交易板块:" << p_pRtnField->szStkbd;
	std::cout << " 证券代码:" << p_pRtnField->szStkCode;
	std::cout << " 交易账户:" << p_pRtnField->szTrdacct;
	std::cout << " 客户代码:" << p_pRtnField->llCustCode;
	std::cout << " 资产账户:" << p_pRtnField->llCuacctCode;
	std::cout << " 证券业务:" << p_pRtnField->iStkBiz;
	std::cout << " 证券业务行为:" << p_pRtnField->iStkBizAction;
	std::cout << " 证券账户子编码:" << p_pRtnField->szSubacctCode;
	std::cout << " 期权合约账户:" << p_pRtnField->szOptTrdacct;
	std::cout << " 订单所有类型:" << p_pRtnField->szOwnerType;
	std::cout << " 合约代码:" << p_pRtnField->szOptCode;
	std::cout << " 合约简称:" << p_pRtnField->szOptName;
	std::cout << " 货币代码:" << p_pRtnField->chCurrency;
	std::cout << " 标的证券类别:" << p_pRtnField->chOptUndlCls;
	std::cout << " 标的证券代码:" << p_pRtnField->szOptUndlCode;
	std::cout << " 标的证券名称:" << p_pRtnField->szOptUndlName;
	std::cout << " 预占用保证金:" << p_pRtnField->szMarginPreFrz;
	std::cout << " 占用保证金:" << p_pRtnField->szMarginFrz;
	std::cout << " 预释放保证金:" << p_pRtnField->szMarginPreUfz;
	std::cout << " 释放保证金:" << p_pRtnField->szMarginUfz << std::endl;

	return 0;
}

// 个股期权当日委托查询响应
int TraderMAOpt::OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspOptCurrDayOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		//triggerQuery();
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][当日委托查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		if (NULL == m_ayOrders)
			m_ayOrders = WTSArray::create();

		WTSOrderInfo* orderInfo = makeOrderInfo(p_pRspField);
		if (orderInfo)
		{
			m_ayOrders->append(orderInfo, false);
		}

		_asyncio.post([this] {
			if (m_bscSink)
				m_bscSink->onRspOrders(m_ayOrders);

			if (m_ayOrders)
				m_ayOrders->clear();
		});		
	}
	return 0;
}

// 个股期权当日成交查询响应
int TraderMAOpt::OnRspQryCurrDayFill(CFirstSetField *p_pFirstSetField, CRspOptCurrDayFillField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");
		//triggerQuery();
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][个股期权当日成交查询请求返回结果: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][返回内容: {}", p_iFieldNum);
		}
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

		_asyncio.post([this] {
			if (m_bscSink)
				m_bscSink->onRspTrades(m_ayTrades);

			if (NULL != m_ayTrades)
				m_ayTrades->clear();
		});
	}

	return 0;
}


int TraderMAOpt::OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSet, CRspOptCombStraPosDetailField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSet == NULL)
	{
		std::cout << "p_pFirstSet is NULL" << std::endl;
		return -1;
	}
	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		std::cout << "  可用组合持仓明细查询返回结果:[" << p_pFirstSet->chMsgLevel << "," << p_pFirstSet->iMsgCode << "," << p_pFirstSet->szMsgDebug << "," << p_pFirstSet->szMsgText << "]\n";
		if (p_iFieldNum > 0)
			std::cout << "返回内容[" << p_iFieldNum << "]:" << std::endl;
	}
	if (0 == p_pFirstSet->iMsgCode&&NULL != p_pRspField)
	{

		std::cout << " 定位串:" << p_pRspField->szQueryPos;
		std::cout << " 客户代码:" << p_pRspField->llCustCode;
		std::cout << " 资产账户:" << p_pRspField->llCuacctCode;
		std::cout << " 内部机构:" << p_pRspField->iIntOrg;
		std::cout << " 交易市场:" << p_pRspField->chStkex;
		std::cout << " 交易板块:" << p_pRspField->szStkbd;
		std::cout << " 交易单元:" << p_pRspField->szStkpbu;
		std::cout << " 证券账户:" << p_pRspField->szTrdacct;
		std::cout << " 证券账户子编码:" << p_pRspField->szSubacctCode;
		std::cout << " 期权合约账户:" << p_pRspField->szOptTrdacct;
		std::cout << " 货币代码:" << p_pRspField->chCurrency;
		std::cout << " 组合编码:" << p_pRspField->szCombNum;
		std::cout << " 组合策略代码:" << p_pRspField->szCombStraCode;
		std::cout << " 组合策略名称:" << p_pRspField->szCombStraName;
		std::cout << " 组合持仓昨日余额:" << p_pRspField->llCombPrebln;
		std::cout << " 组合持仓余额:" << p_pRspField->llCombBln;
		std::cout << " 组合持仓可用:" << p_pRspField->llCombAvl;
		std::cout << " 组合持仓交易冻结:" << p_pRspField->llCombTrdFrz;
		std::cout << " 组合持仓交易解冻:" << p_pRspField->llCombTrdUfz;
		std::cout << " 组合持仓交易在途:" << p_pRspField->llCombTrdOtd;
		std::cout << " 组合持仓交易轧差:" << p_pRspField->llCombTrdBln;
		std::cout << " 组合持仓清算冻结:" << p_pRspField->llCombClrFrz;
		std::cout << " 组合持仓清算解冻:" << p_pRspField->llCombClrUfz;
		std::cout << " 组合持仓清算在途:" << p_pRspField->llCombClrOtd;
		std::cout << " 组合占用保证金:" << p_pRspField->llSecuCombMargin;
		std::cout << " 成分合约个数:" << p_pRspField->iLegCnt;
		std::cout << " 成分一合约编码:" << p_pRspField->szLeg1Num;
		std::cout << " 成分一合约类型:" << p_pRspField->chLeg1Type;
		std::cout << " 成分一持仓方向:" << p_pRspField->chLeg1Side;
		std::cout << " 成分一持仓数量:" << p_pRspField->llLeg1Posi;
		std::cout << " 成分二合约编码:" << p_pRspField->szLeg2Num;
		std::cout << " 成分二合约类型:" << p_pRspField->chLeg2Type;
		std::cout << " 成分二持仓方向:" << p_pRspField->chLeg2Side;
		std::cout << " 成分二持仓数量:" << p_pRspField->llLeg2Posi;
		std::cout << " 成分三合约编码:" << p_pRspField->szLeg3Num;
		std::cout << " 成分三合约类型:" << p_pRspField->chLeg3Type;
		std::cout << " 成分三持仓方向:" << p_pRspField->chLeg3Side;
		std::cout << " 成分三持仓数量:" << p_pRspField->llLeg3Posi;
		std::cout << " 成分四合约编码:" << p_pRspField->szLeg4Num;
		std::cout << " 成分四合约类型:" << p_pRspField->chLeg4Type;
		std::cout << " 成分四持仓方向:" << p_pRspField->chLeg4Side;
		std::cout << " 成分四持仓数量:" << p_pRspField->llLeg4Posi << std::endl;
	}

	return 0;
}

int TraderMAOpt::OnRspQryPosition()
{
	return 0;
}