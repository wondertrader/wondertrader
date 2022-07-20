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
	printf("������:%d;  ������Ϣ:", iRetCode);
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
		printf("API��������\n");
		break;
	case 1001:
		printf("DLLȱʧ\n");
		break;
	case 1002:
		printf("DLL��ʼ��ʧ��(�汾����)\n");
		break;
	case 1003:
		printf("APIʵ���Ѵ���\n");
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
		printf("API�׽��ִ���\n");
		break;
	case 2001:
		printf("�ͻ�������ʧ��(�������Ӳ�����������Ƿ���)\n");
		break;
	case 2002:
		printf("����������ʧ��\n");
		break;
	case 3000:
		printf("API���ô���\n");
		break;
	case 3001:
		printf("GTU�ڵ������ļ�����\n");
		break;
	default:
		printf("������ϸ��Ϣ\n");
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
	if (dirType == MA_BuyOpen || dirType == MA_BuyClose)
		return WDT_LONG;
	else if (dirType == MA_SellOpen || dirType == MA_SellClose)
		return WDT_SHORT;
}

//inline WTSDirectionType wrapDirectionType(TThostFtdcDirectionType dirType, TThostFtdcOffsetFlagType offsetType)
//{
//	if (THOST_FTDC_D_Buy == dirType)
//		if (offsetType == THOST_FTDC_OF_Open)
//			return WDT_LONG;
//		else
//			return WDT_SHORT;
//	else
//		if (offsetType == THOST_FTDC_OF_Open)
//			return WDT_SHORT;
//		else
//			return WDT_LONG;
//}

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
		return isCFFEX ? MA_BESTFIVELEVELPRICE : MA_OPT_ANYPRICE_IOC;
	else if (WPT_LIMITPRICE == priceType)
		return MA_OPT_LIMITPRICE_GFD;
	else if (WPT_BESTPRICE == priceType)
		return MA_BESTPRICE_THIS_SIDE;
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

inline WTSPriceType wrapPriceType(MA_STK_BIZ_ACTION priceType)
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
	, b_inQryAcct(false)
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

	m_strChannel = params->getCString("channel");
	m_strEncryptKey = params->getCString("encrypt_key");
	m_strUseScope = params->getCString("use_scope");
	m_strAuthType = params->getCString("auth_type");
	m_strEncryptType = params->getCString("encrypt_type");
	m_strAcctType = params->getCString("acct_type");

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
	// ����Apiʵ��
	m_pUserAPI = new CCliOptTradeApi();

	// ע����Ӧʵ��
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
	strncpy(req.szStkpbu, m_strStkPBU.c_str(), sizeof(req.szStkpbu));

	//CThostFtdcInputExecOrderField req;
	/////��Լ����
	//strcpy(req.InstrumentID, entrust->getCode());

	//strcpy(req.ExchangeID, entrust->getExchg());

	//req.ActionType = THOST_FTDC_ACTP_Exec;  ///ִ��

	//if (strlen(entrust->getUserTag()) == 0)
	//{
	//	///��������
	//	fmt::format_to(req.ExecOrderRef, "{}", m_orderRef.fetch_add(0));

	//}
	//else
	//{
	//	uint32_t fid, sid, orderref;
	//	extractEntrustID(entrust->getEntrustID(), fid, sid, orderref);
	//	///��������
	//	fmt::format_to(req.ExecOrderRef, "{}", orderref);
	//}

	//if (strlen(entrust->getUserTag()) > 0)
	//{
	//	m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
	//	m_iniHelper.save();
	//}

	req.iStkBiz = wrapPosDirType(entrust->getDirection());
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
{   // ��������

	//std::cout << " ί������:" << p_pRspField->iOrderBsn;
	//std::cout << " ��ͬ���:" << p_pRspField->szOrderId;
	//std::cout << " �ʽ����:" << p_pRspField->llCuacctCode;
	//std::cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
	//std::cout << " ί������:" << p_pRspField->llOrderQty;
	//std::cout << " ί�н��:" << p_pRspField->szOrderAmt;
	//std::cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
	//std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
	//std::cout << " ���װ��:" << p_pRspField->szStkbd;
	//std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
	//std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
	//std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
	//std::cout << " ���֤ȯ����:" << p_pRspField->szStkCode;
	//std::cout << " ���֤ȯ����:" << p_pRspField->szStkName;
	//std::cout << " ֤ȯҵ�����:" << p_pRspField->iStkBiz;
	//std::cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
	//std::cout << " ����״̬:" << p_pRspField->chCancelStatus;
	//std::cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx << std::endl;

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
	///�ڲ�����
	req.iIntOrg = m_iInitOrg;
	///�ͻ�����
	req.llCuacctCode = m_llCuacctCode;
	/// ���װ��
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
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
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));
	req.chQueryFlag = '1';
	req.iQryNum = 10;
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
	if (ct == NULL) {
		cout << "��Լ��ϢΪ��" << endl;
		//return -1;
	}

	CReqOptOrderField req;
	memset(&req, 0, sizeof(req));
	/// �ͻ�����
	req.llCustCode = m_llCustCode;
	/// �ʽ��˻�����
	req.llCuacctCode = m_llCuacctCode;
	/// ���װ��
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	/// �����˻�
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));
	/// ���׵�Ԫ
	strncpy(req.szStkpbu, m_strStkPBU.c_str(), sizeof(req.szStkpbu));
	/// �ڲ�����
	req.iIntOrg = m_iInitOrg;

	//if (strlen(entrust->getUserTag()) == 0)
	//{
	//	///��������
	//	fmt::format_to(req.OrderRef, "{}", m_orderRef.fetch_add(0));

	//}
	//else
	//{
	//	uint32_t fid, sid, orderref;
	//	extractEntrustID(entrust->getEntrustID(), fid, sid, orderref);
	//	///��������
	//	fmt::format_to(req.OrderRef, "{}", orderref);
	//}

	//if (strlen(entrust->getUserTag()) > 0)
	//{
	//	//m_mapEntrustTag[entrust->getEntrustID()] = entrust->getUserTag();
	//	m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
	//	m_iniHelper.save();
	//}

	//WTSCommodityInfo* commInfo = ct->getCommInfo();

	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	//req.iStkBizAction = wrapPriceType(entrust->getPriceType(), strcmp(commInfo->getExchg(), "CFFEX") == 0);
	req.iStkBizAction = 130;
	///��������:
	//req.iStkBiz = wrapDirectionType(entrust->getDirection(), entrust->getOffsetType());
	req.iStkBiz = 401;
	///�۸�
	//strncpy(req.szOrderPrice, std::to_string(entrust->getPrice()).c_str(), sizeof(req.szOrderPrice));
	strncpy(req.szOrderPrice, "0.4264", sizeof(req.szOrderPrice));
	strncpy(req.szOptNum, "10003975", sizeof(req.szOptNum));
	///����: 1
	//req.llOrderQty = (long long)entrust->getVolume();
	req.llOrderQty = 1;
	/// ���ܷ�ʽ
	req.chEncryptType = '0';
	/// ��������
	strncpy(req.szEncryptKey, m_strEncryptKey.c_str(), sizeof(req.szEncryptKey));

	//if (entrust->getOrderFlag() == WOF_NOR)
	//{
	//	//req.TimeCondition = THOST_FTDC_TC_GFD;
	//	//req.VolumeCondition = THOST_FTDC_VC_AV;
	//}
	//else if (entrust->getOrderFlag() == WOF_FAK)
	//{
	//	//req.TimeCondition = THOST_FTDC_TC_IOC;
	//	//req.VolumeCondition = THOST_FTDC_VC_AV;
	//}
	//else if (entrust->getOrderFlag() == WOF_FOK)
	//{
	//	//req.TimeCondition = THOST_FTDC_TC_IOC;
	//	//req.VolumeCondition = THOST_FTDC_VC_CV;
	//}

	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	//req.UserForceClose = 0;

	cout << "�ύ����" << endl;
	int iResult = m_pUserAPI->ReqOrder(&req, genRequestID());
	if (iResult)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Order inserting failed: {}, error: {}", iResult, m_pUserAPI->GetLastErrorText());
	}

	return 0;
}

int TraderMAOpt::orderAction(WTSEntrustAction* action)
{
	// ��������
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	LONGLONG ll_cust_code, ll_cuacct_code;
	int order_bsn;
	//if (!extractEntrustID(action->getEntrustID(), ll_cust_code, ll_cuacct_code, order_bsn)) {
	//	cout << "extract entrust id failed..." << endl;
	//	return -1;
	//}

	CReqOptCancelOrderField req = { 0 };
	memset(&req, 0, sizeof(req));
	///�ڲ�����
	req.iIntOrg = m_iInitOrg;
	///�ͻ�����
	req.llCuacctCode = m_llCuacctCode;
	/// ���װ��
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	///������־
	//req.chForceWth = wrapActionFlag(action->getActionFlag());
	///��Լ����
	//strcpy(req.InstrumentID, action->getCode());
	//req.iOrderBsn = action->getOrderID();
	//strcpy(req.szOrderId, action->getOrderID());
	//strcpy(req.szStkbd, action->getExchg());
	strcpy(req.szOrderId, "J010000602");

	cout << "ί�г�����Ϣ: " << " stk board: " << req.szStkbd << "  order id: " << req.szOrderId << endl;

	int iResult = m_pUserAPI->ReqCancelOrder(&req, genRequestID());
	if (iResult)
	{
		write_log(m_bscSink, LL_ERROR, "[TraderMAOpt] Sending cancel request failed: {}, error: {}", iResult, m_pUserAPI->GetLastErrorText());
	}

	return 0;
}

int TraderMAOpt::queryAccount()
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		printf("��ѯ�˻�ʧ��");
		return -1;
	}

	CReqOptAcctField req = { 0 };
	memset(&req, 0, sizeof(req));

	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	req.chQueryFlag = '1';
	req.iQryNum = 13;
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));

	//std::cout << " llcustcode: " << req.llCustCode << "  llcuacctcode: " << req.llCuacctCode << "  queryFlag: " << req.chQueryFlag << " szStkbd: " << req.szStkbd << "\n";

	int iRet = m_pUserAPI->ReqQryAcct(&req, genRequestID());
	if (iRet) {
		ShowErrorInfo(iRet);
		std::cout << "ReqQryAcct Error: " << m_pUserAPI->GetLastErrorText() << std::endl;
	}

	this->RspQryTradingAccount();

	//m_queQuery.push([this]() {
	//	CThostFtdcQryTradingAccountField req;
	//	memset(&req, 0, sizeof(req));
	//	strcpy(req.BrokerID, m_strBroker.c_str());
	//	strcpy(req.InvestorID, m_strUser.c_str());
	//	m_pUserAPI->ReqQryTradingAccount(&req, genRequestID());
	//});

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

	strncpy(stField.szStkbd, m_strStkBD.c_str(), sizeof(stField.szStkbd));
	strncpy(stField.szStkpbu, m_strStkPBU.c_str(), sizeof(stField.szStkpbu));
	strncpy(stField.szTrdacct, m_strTrdAcct.c_str(), sizeof(stField.szTrdacct));

	int iRet = m_pUserAPI->ReqQryExpendableCu(&stField, genRequestID());
	if (iRet)
		cout << "ReqQryExpendableCu Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

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

	CReqOptCurrDayOrderField req = { 0 };
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));
	req.chQueryFlag = '1';
	req.iQryNum = 10;
	//strcpy(req.szTrdacct, m_strUser.c_str());

	int iRet = m_pUserAPI->ReqQryCurrDayOrder(&req, genRequestID());
	if (iRet)
		cout << "ReqQryCurrDayOrder Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

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
	strncpy(req.szStkbd, m_strStkBD.c_str(), sizeof(req.szStkbd));
	strncpy(req.szTrdacct, m_strTrdAcct.c_str(), sizeof(req.szTrdacct));

	req.chQueryFlag = '1';  // ��ѯ���� 0:���ȡ���� 1:��ǰȡ���� ����ȫ������
	req.iQryNum = 10;  // ��ѯ����

	int iRet = m_pUserAPI->ReqQryCurrDayFill(&req, genRequestID());
	if (iRet)
		cout << "ReqQryCurrDayFill Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

	return 0;
}

int TraderMAOpt::querySettlement(uint32_t uDate)
{
	if (m_pUserAPI == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	cout << "��ѯ���㵥����: " << uDate << endl;

	CReqOptSettListField req;
	memset(&req, 0, sizeof(req));
	req.llCustCode = m_llCustCode;
	req.llCuacctCode = m_llCuacctCode;
	req.iTrdDate = uDate;
	req.chCurrency = '0';

	int iRet = m_pUserAPI->ReqQrySettList(&req, genRequestID());
	if (iRet) {
		cout << __FUNCDNAME__ << "::" << __LINE__ << "::RRROR" << m_pUserAPI->GetLastErrorText();
	}

	return 0;
}

WTSOrderInfo* TraderMAOpt::makeOrderInfo(CRspOptOrderField* orderField)
{
	//std::cout << "    ==�ɽ��ر� �ɽ�֪ͨ==" << std::endl;
	//std::cout << " ί������:" << p_pRspField->iOrderBsn;
	//std::cout << " ��ͬ���:" << p_pRspField->szOrderId;
	//std::cout << " �ʽ��˻�:" << p_pRspField->llCuacctCode;
	//std::cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
	//std::cout << " ί������:" << p_pRspField->llOrderQty;
	//std::cout << " ί�н��:" << p_pRspField->szOrderAmt;
	//std::cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
	//std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
	//std::cout << " ���װ��:" << p_pRspField->szStkbd;
	//std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
	//std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
	//std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
	//std::cout << " ��Լ����:" << p_pRspField->szOptNum;
	//std::cout << " ��Լ����:" << p_pRspField->szOptCode;
	//std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
	//std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
	//std::cout << " ֤ȯҵ��:" << p_pRspField->iStkBiz;
	//std::cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
	//std::cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx << std::endl;

	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szOptCode, orderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)orderField->iStkBiz));
	pRet->setPriceType(wrapPriceType(orderField->iStkBiz));
	//pRet->setOffsetType(wrapOffsetType(orderField->CombOffsetFlag[0]));
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
	}

	pRet->setCode(orderField->szOptCode);
	pRet->setExchange(contract->getExchg());

	//pRet->setOrderDate(strtoul(orderField->InsertDate, NULL, 10));
	//std::string strTime = orderField->InsertTime;
	//StrUtil::replace(strTime, ":", "");
	//uint32_t uTime = strtoul(strTime.c_str(), NULL, 10);
	//pRet->setOrderTime(TimeUtils::makeTime(pRet->getOrderDate(), strtoul(strTime.c_str(), NULL, 10) * 1000));

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->OrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), orderField->llCuacctCode, orderField->llCuacctCode, orderField->iOrderBsn);
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
	//std::cout << "    ==�ɽ��ر� ί����Ϣ==" << std::endl;
	//std::cout << " �����г�:" << p_pRtnField->chStkex;
	//std::cout << " ���װ��:" << p_pRtnField->szStkbd;
	//std::cout << " ί������:" << p_pRtnField->iOrderDate;
	//std::cout << " ֤ȯ����:" << p_pRtnField->szStkCode;
	//std::cout << " ί������:" << p_pRtnField->iOrderBsn;
	//std::cout << " ��ͬ���:" << p_pRtnField->szOrderId;
	//std::cout << " �ͻ�����:" << p_pRtnField->llCustCode;
	//std::cout << " �ʲ��˻�:" << p_pRtnField->llCuacctCode;
	//std::cout << " �˻����:" << p_pRtnField->iCuacctSn;
	//std::cout << " �����˻�:" << p_pRtnField->szTrdacct;
	//std::cout << " ί�м۸�:" << p_pRtnField->szOrderPrice;
	//std::cout << " ί������:" << p_pRtnField->llOrderQty;
	//std::cout << " ί�н��:" << p_pRtnField->szOrderAmt;
	//std::cout << " ί�ж�����:" << p_pRtnField->szOrderFrzAmt;
	//std::cout << " ί��״̬:" << p_pRtnField->chOrderStatus;
	//std::cout << " ֤ȯҵ��:" << p_pRtnField->iStkBiz;
	//std::cout << " ֤ȯҵ����Ϊ:" << p_pRtnField->iStkBizAction;
	//std::cout << " �ѳ�������:" << p_pRtnField->llWithdrawnQty;
	//std::cout << " �ۼƳɽ�����:" << p_pRtnField->llTotalMatchedQty;
	//std::cout << " ���׵�Ԫ:" << p_pRtnField->szStkpbu << std::endl;

	WTSContractInfo* contract = m_bdMgr->getContract(orderField->szStkCode, orderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(atof(orderField->szOrderPrice));
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume((double)orderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((MA_STK_BIZ)orderField->iStkBiz));
	//pRet->setOffsetType(wrapOffsetType(orderField->OffsetFlag));

	pRet->setVolTraded((double)orderField->llTotalMatchedQty);
	pRet->setVolLeft((double)(orderField->llOrderQty - orderField->llTotalMatchedQty));

	pRet->setCode(orderField->szStkCode);
	pRet->setExchange(contract->getExchg());

	//pRet->setEntrustID(generateEntrustID(orderField->FrontID, orderField->SessionID, atoi(orderField->ExecOrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), orderField->llCustCode, orderField->llCuacctCode, orderField->iOrderBsn);
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
	/*std::cout << "  ��λ��:" << p_pRspField->szQryPos;
	std::cout << "  ��������:" << p_pRspField->iTrdDate;
	std::cout << "  ί������:" << p_pRspField->iOrderDate;
	std::cout << " ί��ʱ��:" << p_pRspField->szOrderTime;
	std::cout << "  ί������:" << p_pRspField->iOrderBsn;
	std::cout << "  ��ͬ���:" << p_pRspField->szOrderId;
	std::cout << " ί��״̬:" << p_pRspField->chOrderStatus;
	std::cout << " ί����Ч��־:" << p_pRspField->chOrderValidFlag;
	std::cout << "  �ڲ�����:" << p_pRspField->iIntOrg;
	std::cout << "  �ͻ�����:" << p_pRspField->llCustCode;
	std::cout << "  �ʲ��˻�:" << p_pRspField->llCuacctCode;
	std::cout << "  �����г�:" << p_pRspField->chStkex;
	std::cout << "  ���װ��:" << p_pRspField->szStkbd;
	std::cout << "  ���׵�Ԫ:" << p_pRspField->szStkpbu;
	std::cout << "  ֤ȯ�˻�:" << p_pRspField->szTrdacct;
	std::cout << "  ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
	std::cout << "  ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
	std::cout << "  ֤ȯҵ��:" << p_pRspField->iStkBiz;
	std::cout << "  ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
	std::cout << "  ������������:" << p_pRspField->szOwnerType;
	std::cout << "  ��Լ����:" << p_pRspField->szOptNum;
	std::cout << "  ��Լ����:" << p_pRspField->szOptCode;
	std::cout << "  ��Լ���:" << p_pRspField->szOptName;
	std::cout << "  ��ϱ���:" << p_pRspField->szCombNum;
	std::cout << "  ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
	std::cout << "  �ɷ�һ:" << p_pRspField->szLeg1Num;
	std::cout << "  �ɷֶ�:" << p_pRspField->szLeg2Num;
	std::cout << "  �ɷ���:" << p_pRspField->szLeg3Num;
	std::cout << "  �ɷ���:" << p_pRspField->szLeg4Num;
	std::cout << "  ���Ҵ���:" << p_pRspField->chCurrency;
	std::cout << "  ί�м۸�:" << p_pRspField->szOrderPrice;
	std::cout << "  ί������:" << p_pRspField->llOrderQty;
	std::cout << "  ί�н��:" << p_pRspField->szOrderAmt;
	std::cout << "  ί�ж�����:" << p_pRspField->szOrderFrzAmt;
	std::cout << " ί�нⶳ���:" << p_pRspField->szOrderUfzAmt;
	std::cout << " �걨����:" << p_pRspField->llOfferQty;
	std::cout << " �걨ʱ��:" << p_pRspField->iOfferStime;
	std::cout << " �ѳ�������:" << p_pRspField->llWithdrawnQty;
	std::cout << "  �ѳɽ�����:" << p_pRspField->llMatchedQty;
	std::cout << "  �ѳɽ����:" << p_pRspField->szMatchedAmt;
	std::cout << " ������־:" << p_pRspField->chIsWithdraw;
	std::cout << " �ѳ�����־:" << p_pRspField->chIsWithdrawn;
	std::cout << " ���֤ȯ���:" << p_pRspField->chOptUndlCls;
	std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
	std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
	std::cout << " ���ȯί�ж�������:" << p_pRspField->llUndlFrzQty;
	std::cout << " ���ȯί�нⶳ����:" << p_pRspField->llUndlUfzQty;
	std::cout << " ���ȯ�ѳ�������:" << p_pRspField->llUndlWthQty;
	std::cout << " �걨������Ϣ:" << p_pRspField->szOfferRetMsg;
	std::cout << "  �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx;
	std::cout << " ί�����:" << p_pRspField->iOrderSn;
	std::cout << " �ⲿ��ͬ���:" << p_pRspField->szRawOrderId;
	std::cout << "  Ԥռ�ñ�֤��:" << p_pRspField->szMarginPreFrz;
	std::cout << "  ռ�ñ�֤��:" << p_pRspField->szMarginFrz;
	std::cout << "  Ԥ�ⶳ��֤��:" << p_pRspField->szMarginPreUfz;
	std::cout << "  �ⶳ��֤��:" << p_pRspField->szMarginUfz << std::endl;
*/
	WTSContractInfo* contract = m_bdMgr->getContract(daiOrderField->szOptCode, daiOrderField->szStkbd);
	if (contract == NULL)
		return NULL;

	WTSOrderInfo* pRet = WTSOrderInfo::create();
	pRet->setContractInfo(contract);
	pRet->setPrice(0);
	pRet->setBusinessType(BT_EXECUTE);
	pRet->setVolume((double)daiOrderField->llOrderQty);
	pRet->setDirection(wrapPosDirType((MA_STK_BIZ)daiOrderField->iStkBiz));
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

	WTSDirectionType dType = wrapDirectionType((MA_STK_BIZ)tradeField->iStkBiz);

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
	/*std::cout << " ��λ��:" << p_pRspField->szQryPos;
	std::cout << " ��������:" << p_pRspField->iTrdDate;
	std::cout << " �ɽ�ʱ��:" << p_pRspField->szMatchedTime;
	std::cout << " ί������:" << p_pRspField->iOrderDate;
	std::cout << " ί�����:" << p_pRspField->iOrderSn;
	std::cout << " ί������:" << p_pRspField->iOrderBsn;
	std::cout << " ��ͬ���:" << p_pRspField->szOrderId;
	std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
	std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
	std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
	std::cout << " �����г�:" << p_pRspField->chStkex;
	std::cout << " ���װ��:" << p_pRspField->szStkbd;
	std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
	std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
	std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
	std::cout << "  ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
	std::cout << " ֤ȯҵ��:" << p_pRspField->iStkBiz;
	std::cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
	std::cout << " ������������:" << p_pRspField->szOwnerType;
	std::cout << " ��Լ����:" << p_pRspField->szOptNum;
	std::cout << " ��Լ����:" << p_pRspField->szOptCode;
	std::cout << " ��Լ���:" << p_pRspField->szOptName;
	std::cout << "  ��ϱ���:" << p_pRspField->szCombNum;
	std::cout << "  ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
	std::cout << "  �ɷ�һ:" << p_pRspField->szLeg1Num;
	std::cout << "  �ɷֶ�:" << p_pRspField->szLeg2Num;
	std::cout << "  �ɷ���:" << p_pRspField->szLeg3Num;
	std::cout << "  �ɷ���:" << p_pRspField->szLeg4Num;
	std::cout << " ���Ҵ���:" << p_pRspField->chCurrency;
	std::cout << " ���֤ȯ���:" << p_pRspField->chOptUndlCls;
	std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
	std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
	std::cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
	std::cout << " ί������:" << p_pRspField->llOrderQty;
	std::cout << " ί�н��:" << p_pRspField->szOrderAmt;
	std::cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
	std::cout << " ������־:" << p_pRspField->chIsWithdraw;
	std::cout << " �ɽ�����:" << p_pRspField->chMatchedType;
	std::cout << " �ɽ����:" << p_pRspField->szMatchedSn;
	std::cout << " �ɽ��۸�:" << p_pRspField->szMatchedPrice;
	std::cout << " �ѳɽ�����:" << p_pRspField->llMatchedQty;
	std::cout << " �ѳɽ����:" << p_pRspField->szMatchedAmt;
	std::cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx;
	std::cout << " Ԥռ�ñ�֤��:" << p_pRspField->szMarginPreFrz;
	std::cout << " ռ�ñ�֤��:" << p_pRspField->szMarginFrz;
	std::cout << " Ԥ�ⶳ��֤��:" << p_pRspField->szMarginPreUfz;
	std::cout << " �ⶳ��֤��:" << p_pRspField->szMarginUfz;
	std::cout << " �ɽ�����:" << p_pRspField->szMatchedFee << std::endl;*/

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
	//	//�����ǰ���ںͽ�����һ��,��ʱ�����21��,˵����ҹ��,Ҳ����ʵ������Ҫ��������
	//	if (uTime / 10000 >= 21)
	//	{
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//	}
	//	else if(uTime <= 3)
	//	{
	//		//�����3������,��Ҫ�Ȼ�ȡ��һ��������,�ٻ�ȡ��һ����Ȼ��
	//		//��������Ŀ����,�����������ϵ����,���Դ������
	//		uDate = m_pMarketMgr->getPrevTDate(commInfo->getExchg(), uDate, 1);
	//		uDate = TimeUtils::getNextDate(uDate);
	//	}
	//}

	pRet->setTradeDate(uDate);
	pRet->setTradeTime(TimeUtils::makeTime(uDate, uTime * 1000));

	WTSDirectionType dType = wrapDirectionType((MA_STK_BIZ)tradeField->iStkBiz);

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

WTSEntrust* TraderMAOpt::makeEntrust(CRspOptOrderField *entrustField)
{
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->szOptNum, entrustField->szStkbd);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(
		entrustField->szOptNum,
		entrustField->llOrderQty,
		atof(entrustField->szOrderPrice),
		ct->getExchg());

	pRet->setContractInfo(ct);
	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)entrustField->iStkBiz));
	pRet->setPriceType(wrapPriceType((MA_STK_BIZ_ACTION)entrustField->iStkBizAction));
	//pRet->setOffsetType(wrapOffsetType(entrustField->CombOffsetFlag[0]));

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
	generateEntrustID(pRet->getEntrustID(), m_llCustCode, m_llCuacctCode, entrustField->iOrderBsn);

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
	WTSContractInfo* ct = m_bdMgr->getContract(entrustField->szStkCode, entrustField->szStkbd);
	if (ct == NULL)
		return NULL;

	WTSEntrust* pRet = WTSEntrust::create(entrustField->szStkCode, entrustField->llMatchedQty, 0, ct->getExchg(), BT_EXECUTE);
	pRet->setContractInfo(ct);

	pRet->setDirection(wrapDirectionType((MA_STK_BIZ)entrustField->iStkBiz));
	pRet->setPriceType(wrapPriceType((MA_STK_BIZ_ACTION)entrustField->iStkBizAction));

	//pRet->setEntrustID(generateEntrustID(m_frontID, m_sessionID, atoi(entrustField->ExecOrderRef)).c_str());
	generateEntrustID(pRet->getEntrustID(), m_llCustCode, m_llCuacctCode, entrustField->iOrderBsn);

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


void TraderMAOpt::generateEntrustID(char* buffer, long long ll_cust_code, long long ll_cuacct_code, long order_bsn)
{
	fmtutil::format_to(buffer, "{:010d}#{:010d}#{:06d}", ll_cust_code, ll_cuacct_code, order_bsn);
}

bool TraderMAOpt::extractEntrustID(const char* entrustid, long long &ll_cust_code, long long &ll_cuacct_code, long &order_bsn)
{
	//Market.FrontID.SessionID.OrderRef
	thread_local static char buffer[64];
	wt_strcpy(buffer, entrustid);
	char* s = buffer;
	auto idx = StrUtil::findFirst(s, '#');
	if (idx == std::string::npos)
		return false;
	s[idx] = '\0';
	ll_cust_code = strtoul(s, NULL, 10);
	s += idx + 1;

	idx = StrUtil::findFirst(s, '#');
	if (idx == std::string::npos)
		return false;
	s[idx] = '\0';
	ll_cuacct_code = strtoul(s, NULL, 10);
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

/*
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
	std::cout << "�ͻ����������ͨ�����ӶϿ�[" << p_nReason << "][" << p_pszErrInfo << "]" << std::endl;

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

	// std::cout << "p_iFieldNum: " << p_iFieldNum << "  p_iFieldIndex: " << p_iFieldIndex << "\n";

	//if (p_iFieldNum == 0)
	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		printf("[%s: %d", __FUNCDNAME__, __LINE__);

		m_wrapperState = WS_LOGINING;

		write_log(m_bscSink, LL_INFO, "[TraderMAOpt][Login info: �û���¼���󷵻ؽ��: {},{},{},{}, ��������: {}]", p_pFirstSet->chMsgLevel, p_pFirstSet->iMsgCode, p_pFirstSet->szMsgDebug, p_pFirstSet->szMsgText, p_iFieldNum);
	}

	//if (p_iFieldNum == p_iFieldIndex)
	if (p_iFieldNum > 0)
	{
		std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
		std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		std::cout << " �����г�:" << p_pRspField->chStkex;
		std::cout << " ���װ��:" << p_pRspField->szStkbd;
		std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		std::cout << " �˻�״̬:" << p_pRspField->chTrdacctStatus;
		std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		std::cout << " �˻�����:" << p_pRspField->szAcctType;
		std::cout << " �˻���ʶ:" << p_pRspField->szAcctId;
		std::cout << " �˻�����:" << p_pRspField->szTrdacctName << std::endl;
		//std::cout << " �Ựƾ֤:" << p_pRspField->szSession << std::endl;

		m_strTrdAcct = p_pRspField->szTrdacct;
		m_strStkBD = p_pRspField->szStkbd;
		m_strStkPBU = p_pRspField->szStkpbu;

		m_wrapperState = WS_LOGINED;

		//write_log(m_bscSink, LL_INFO, "[TraderCTPOpt][{}-{}] Login succeed, �ͻ�����: {}, �ʲ��˻�: {}, �����г�: {}�� ���װ��: {}, ֤ȯ�˻�: {}, ֤ȯ�˻��ӱ���: {}, ��Ȩ��Լ�˻�: {}, �ڲ�����: {}, �˻�״̬: {}, ���׵�Ԫ: {}, �˻�����: {}, �˻���ʶ: {}, �˻�����: {}",
		//	p_pRspField->llCustCode, p_pRspField->llCuacctCode, p_pRspField->chStkex, std::string(p_pRspField->szStkbd), std::string(p_pRspField->szTrdacct), std::string(p_pRspField->szSubacctCode), std::string(p_pRspField->szOptTrdacct), p_pRspField->iIntOrg, p_pRspField->chTrdacctStatus, std::string(p_pRspField->szStkpbu), std::string(p_pRspField->szAcctType), std::string(p_pRspField->szAcctId), std::string(p_pRspField->szTrdacctName));

		{
			//��ʼ��ί�е�������
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
			//��ʼ��������ǻ�����
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
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][�����ʽ��ѯ���󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
		std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		std::cout << " ���Ҵ���:" << p_pRspField->chCurrency;
		std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		std::cout << " �ʲ���ֵ:" << p_pRspField->szMarketValue;
		std::cout << " �ʽ��ʲ�:" << p_pRspField->szFundValue;
		std::cout << " ��ֵ:" << p_pRspField->szStkValue;
		std::cout << " �ʽ��������:" << p_pRspField->szFundPrebln;
		std::cout << " �ʽ����:" << p_pRspField->szFundBln;
		std::cout << " �ʽ���ý��:" << p_pRspField->szFundAvl;
		std::cout << " �ʽ𶳽���:" << p_pRspField->szFundFrz;
		std::cout << " �ʽ�ⶳ���:" << p_pRspField->szFundUfz;
		std::cout << " �ʽ��׶�����:" << p_pRspField->szFundTrdFrz;
		std::cout << " �ʽ��׽ⶳ���:" << p_pRspField->szFundTrdUfz;
		std::cout << " �ʽ�����;���:" << p_pRspField->szFundTrdOtd;
		std::cout << " �ʽ���������:" << p_pRspField->szFundTrdBln;
		std::cout << " �ʽ�״̬:" << p_pRspField->chFundStatus;
		std::cout << " ռ�ñ�֤��:" << p_pRspField->szMarginUsed;
		std::cout << " ��ռ�ñ�֤��(��δ�ɽ�):" << p_pRspField->szMarginInclRlt;
		std::cout << " ��Ȩ������֤��:" << p_pRspField->szFundExeMargin;
		std::cout << " ��Ȩ�ʽ𶳽���:" << p_pRspField->szFundExeFrz;
		std::cout << " �ʽ���ö�����:" << p_pRspField->szFundFeeFrz;
		std::cout << " �渶�ʽ�:" << p_pRspField->szPaylater;
		std::cout << " Ԥ�Ƶ��ʽ��:" << p_pRspField->szPreadvaPay;
		std::cout << " Ԥ�Ƹ�ծ��Ϣ:" << p_pRspField->szExpPenInt;
		std::cout << " �ʽ��ȡ���:" << p_pRspField->szFundDraw;
		std::cout << " �ʽ�̬����:" << p_pRspField->szFundAvlRlt;
		std::cout << " ��̬ռ�ñ�֤��(��δ�ɽ�):" << p_pRspField->szMarginInclDyn;
		std::cout << " �������:" << p_pRspField->szDailyInAmt;
		std::cout << " ���ճ���:" << p_pRspField->szDailyOutAmt;
		std::cout << " �ʽ�ʵ�ʿ���:" << p_pRspField->szFundRealAvl << std::endl;

		if (b_inQryAcct) {
			m_accountInfo->setPreBalance(atof(p_pRspField->szFundPrebln)); // �ʽ��������
			//m_accountInfo->setCommission(atof(p_pRspField->szExpPenInt));  // Ԥ�Ƹ�ծ��Ϣ
			m_accountInfo->setFrozenMargin(atof(p_pRspField->szMarginInclRlt));  // ��ռ�ñ�֤��(��δ�ɽ�)
			m_accountInfo->setFrozenCommission(atof(p_pRspField->szFundFeeFrz));  // �ʽ���ö�����
			m_accountInfo->setDeposit(atof(p_pRspField->szDailyInAmt));  // �������
			m_accountInfo->setAvailable(atof(p_pRspField->szFundAvl));  // �ʽ���ý��
			m_accountInfo->setWithdraw(atof(p_pRspField->szDailyOutAmt));  // ���ճ���
			m_accountInfo->setBalance(atof(p_pRspField->szFundBln));  // �ʽ����

			return 0;
		}

		//cout << "�ʽ����: " << p_pRspField->szFundBln << "  �ʽ�������: " << p_pRspField->szFundAvl << endl;
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

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][���ú�Լ�ʲ���ѯ���󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
		std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		std::cout << " �����г�:" << p_pRspField->chStkex;
		std::cout << " ���װ��:" << p_pRspField->szStkbd;
		std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		std::cout << " ���Ҵ���:" << p_pRspField->chCurrency;
		std::cout << " ��Լ����:" << p_pRspField->szOptNum;
		std::cout << " ��Լ����:" << p_pRspField->szOptCode;
		std::cout << " ��Լ���:" << p_pRspField->szOptName;
		std::cout << " ��Լ����:" << p_pRspField->chOptType;
		std::cout << " �ֲַ���:" << p_pRspField->chOptSide;
		std::cout << " ���ұ�־:" << p_pRspField->chOptCvdFlag;
		std::cout << " ��Լ�������:" << p_pRspField->llOptPrebln;
		std::cout << " ��Լ���:" << p_pRspField->llOptBln;
		std::cout << " ��Լ��������:" << p_pRspField->llOptAvl;
		std::cout << " ��Լ��������:" << p_pRspField->llOptFrz;
		std::cout << " ��Լ�ⶳ����:" << p_pRspField->llOptUfz;
		std::cout << " ��Լ���׶�������:" << p_pRspField->llOptTrdFrz;
		std::cout << " ��Լ���׽ⶳ����:" << p_pRspField->llOptTrdUfz;
		std::cout << " ��Լ������;����:" << p_pRspField->llOptTrdOtd;
		std::cout << " ��Լ������������:" << p_pRspField->llOptTrdBln;
		std::cout << " ��Լ���㶳������:" << p_pRspField->llOptClrFrz;
		std::cout << " ��Լ����ⶳ����:" << p_pRspField->llOptClrUfz;
		std::cout << " ��Լ������;����:" << p_pRspField->llOptClrOtd;
		std::cout << " ��Լ����ɱ�:" << p_pRspField->szOptBcost;
		std::cout << " ��Լ����ɱ���ʵʱ��:" << p_pRspField->szOptBcostRlt;
		std::cout << " ��Լӯ�����:" << p_pRspField->szOptPlamt;
		std::cout << " ��Լӯ����ʵʱ��:" << p_pRspField->szOptPlamtRlt;
		std::cout << " ��Լ��ֵ:" << p_pRspField->szOptMktVal;
		std::cout << " Ȩ����:" << p_pRspField->szOptPremium;
		std::cout << " ��֤��" << p_pRspField->szOptMargin;
		std::cout << " ���ҹɷ�����:" << p_pRspField->llOptCvdAsset;
		std::cout << " ����ƽ��ӯ��:" << p_pRspField->szOptClsProfit;
		std::cout << " �ۼ�ƽ��ӯ��:" << p_pRspField->szSumClsProfit;
		std::cout << " ����ӯ��:" << p_pRspField->szOptFloatProfit;
		std::cout << " ��ӯ��:" << p_pRspField->szTotalProfit;
		std::cout << " ��Լʵ�ʳֲ�:" << p_pRspField->llOptRealPosi;
		std::cout << " ��Լƽ�ֹҵ�����:" << p_pRspField->llOptClsUnmatched;
		std::cout << " �����ۼƿ�������:" << p_pRspField->llOptDailyOpenRlt;
		std::cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode << std::endl;

		if (b_inQryAcct) {
			m_accountInfo->setCloseProfit(atof(p_pRspField->szOptClsProfit));   // ����ƽ��ӯ��
			m_accountInfo->setDynProfit(atof(p_pRspField->szOptFloatProfit));  // ����ӯ��
			m_accountInfo->setMargin(atof(p_pRspField->szOptMargin));  // ��֤��

			return 0;
		}

		if (NULL == m_mapPosition)
			m_mapPosition = PositionMap::create();

		WTSContractInfo* contract = m_bdMgr->getContract(p_pRspField->szOptNum, p_pRspField->szStkbd);
		if (contract == NULL)
			return 0;

		WTSCommodityInfo* commInfo = contract->getCommInfo();
		if (contract)
		{
			std::string key = fmt::format("{}-{}", p_pRspField->szOptCode, p_pRspField->szStkbd);
			WTSPositionItem* pos = (WTSPositionItem*)m_mapPosition->get(key);
			if (pos == NULL)
			{
				pos = WTSPositionItem::create(p_pRspField->szOptCode, commInfo->getCurrency(), commInfo->getExchg());
				pos->setContractInfo(contract);
				m_mapPosition->add(key, pos, false);
			}
			//pos->setDirection(wrapPosDirection(p_pRspField->));
			if (commInfo->getCoverMode() == CM_CoverToday)
			{
				//if (p_pRspField->PositionDate == THOST_FTDC_PSD_Today)
				//	pos->setNewPosition(p_pRspField->llOptBln);
				//else
				//	pos->setPrePosition(p_pRspField->llOptPrebln);
			}
			else
			{
				//pos->setNewPosition(pInvestorPosition->TodayPosition);
				//pos->setPrePosition(pInvestorPosition->Position - pInvestorPosition->TodayPosition);
			}

			pos->setNewPosition(p_pRspField->llOptBln);
			pos->setPrePosition(p_pRspField->llOptPrebln);

			pos->setMargin(pos->getMargin() + atof(p_pRspField->szOptMargin));
			pos->setDynProfit(pos->getDynProfit() + atof(p_pRspField->szOptClsProfit));
			pos->setPositionCost(pos->getPositionCost() + atof(p_pRspField->szOptBcost));

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
				if (commInfo->getCoverMode() == CM_CoverToday)
				{
					pos->setAvailNewPos(p_pRspField->llOptAvl);

					//if (p_pRspField->PositionDate == THOST_FTDC_PSD_Today)  // ///���ճֲ�
					//{
					//	int availNew = p_pRspField->Position;  // ///�ֲ�
					//	if (p_pRspField->PosiDirection == THOST_FTDC_PD_Long)  // ///��ͷ
					//	{
					//		availNew -= p_pRspField->ShortFrozen;  // ///��ͷ����
					//	}
					//	else
					//	{
					//		availNew -= p_pRspField->LongFrozen;
					//	}
					//	if (availNew < 0)
					//		availNew = 0;
					//	pos->setAvailNewPos(availNew);
					//}
					//else
					//{
					//	int availPre = p_pRspField->Position;
					//	if (p_pRspField->PosiDirection == THOST_FTDC_PD_Long)
					//	{
					//		availPre -= p_pRspField->ShortFrozen;
					//	}
					//	else
					//	{
					//		availPre -= p_pRspField->LongFrozen;
					//	}
					//	if (availPre < 0)
					//		availPre = 0;
					//	pos->setAvailPrePos(availPre);
					//}
				}
				else
				{
					pos->setAvailNewPos(p_pRspField->llOptAvl);

					//int availNew = p_pRspField->TodayPosition;
					//if (p_pRspField->PosiDirection == THOST_FTDC_PD_Long)
					//{
					//	availNew -= p_pRspField->ShortFrozen;
					//}
					//else
					//{
					//	availNew -= p_pRspField->LongFrozen;
					//}
					//if (availNew < 0)
					//	availNew = 0;
					//pos->setAvailNewPos(availNew);

					//double availPre = pos->getNewPosition() + pos->getPrePosition()
					//	- p_pRspField->LongFrozen - p_pRspField->ShortFrozen
					//	- pos->getAvailNewPos();
					//pos->setAvailPrePos(availPre);
				}
			}
			else
			{

			}

			if (decimal::lt(pos->getTotalPosition(), 0.0) && decimal::eq(pos->getMargin(), 0.0))
			{
				//�в�λ,���Ǳ�֤��Ϊ0,��˵����������Լ,������Լ�Ŀ��óֲ�ȫ����Ϊ0
				pos->setAvailNewPos(0);
				pos->setAvailPrePos(0);
			}
		}
	}

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
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][����Ȩ��Լ�˻���ѯ���󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " ��λ��:" << p_pRspField->szQryPos;
		std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
		std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		std::cout << " �����г�:" << p_pRspField->chStkex;
		std::cout << " �˻���ʶ:" << p_pRspField->szStkbd;
		std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		std::cout << " ������Ȩ�˻�����:" << p_pRspField->chOptTrdacctLvl;
		std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		std::cout << " �˻����:" << p_pRspField->iTrdacctSn;
		std::cout << " �����˻�:" << p_pRspField->szTrdacctExid;
		std::cout << " �˻�״̬:" << p_pRspField->chTrdacctStatus;
		std::cout << " ָ��״̬:" << p_pRspField->chTregStatus;
		std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		std::cout << " ֤������:" << p_pRspField->szIdType;
		std::cout << " ֤������:" << p_pRspField->szIdCode;
		std::cout << " �ͻ�����:" << p_pRspField->szCustName;
		//std::cout<< " ��֤�����ģ����:"<< p_pRspField->iOptMarginSn ;
		std::cout << " ������:" << p_pRspField->szQuotaVal;
		std::cout << " ռ��������:" << p_pRspField->szQuotaValUsed;
		std::cout << " ������־:" << p_pRspField->chOpFlag;
		std::cout << " ����ѯ�۴���:" << p_pRspField->llDailyEnqryCount << std::endl;
	}

	return 0;
}

int TraderMAOpt::OnRspOrder(CFirstSetField *p_pFirstSetField, CRspOptOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL) {
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");

		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][������Ȩί���걨���ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		cout << " ί������:" << p_pRspField->iOrderBsn;
		cout << " ��ͬ���:" << p_pRspField->szOrderId;
		cout << " �ʽ��˻�:" << p_pRspField->llCuacctCode;
		cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
		cout << " ί������:" << p_pRspField->llOrderQty;
		cout << " ί�н��:" << p_pRspField->szOrderAmt;
		cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
		cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		cout << " ���װ��:" << p_pRspField->szStkbd;
		cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		cout << " ��Լ����:" << p_pRspField->szOptNum;
		cout << " ��Լ����:" << p_pRspField->szOptCode;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
		cout << " ֤ȯҵ��:" << p_pRspField->iStkBiz;
		cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
		cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx << std::endl;

		WTSEntrust* entrust = makeEntrust(p_pRspField);
		if (entrust)
		{
			WTSError *err = makeError(p_pFirstSetField);
			//g_orderMgr.onRspEntrust(entrust, err);
			if (m_bscSink)
				m_bscSink->onRspEntrust(entrust, err);
			entrust->release();
			err->release();
		}

		m_strOptNum = p_pRspField->szOptNum;
	}
}

int TraderMAOpt::OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSetField == NULL)
	{
		write_log(m_bscSink, LL_ERROR, "[TradeMAOpt][p_pFirstSetField is NULL!]");

		return -1;
	}

	if (!IsErrorRspInfo(p_pFirstSetField)) {
		WTSError* error = WTSError::create(WEC_ORDERCANCEL, p_pFirstSetField->szMsgText);
		if (m_bscSink)
			m_bscSink->onTraderError(error);

		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][������Ȩί�г������󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		std::cout << " ί������:" << p_pRspField->iOrderBsn;
		std::cout << " ��ͬ���:" << p_pRspField->szOrderId;
		std::cout << " �ʽ����:" << p_pRspField->llCuacctCode;
		std::cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
		std::cout << " ί������:" << p_pRspField->llOrderQty;
		std::cout << " ί�н��:" << p_pRspField->szOrderAmt;
		std::cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
		std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		std::cout << " ���װ��:" << p_pRspField->szStkbd;
		std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		std::cout << " ���֤ȯ����:" << p_pRspField->szStkCode;
		std::cout << " ���֤ȯ����:" << p_pRspField->szStkName;
		std::cout << " ֤ȯҵ�����:" << p_pRspField->iStkBiz;
		std::cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
		std::cout << " ����״̬:" << p_pRspField->chCancelStatus;
		std::cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx << std::endl;

	}

	return 0;
}

int TraderMAOpt::OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRtnField)
{
	if (p_pRtnField == NULL)
	{
		return -1;
	}
	std::cout << "�����г�:" << p_pRtnField->chStkex;
	std::cout << "֤ȯ����:" << p_pRtnField->szStkCode;
	std::cout << "��ͬ���:" << p_pRtnField->szOrderId;
	std::cout << "�����˻�:" << p_pRtnField->szTrdacct;
	std::cout << "������־:" << p_pRtnField->chIsWithdraw;
	std::cout << "�ͻ�����:" << p_pRtnField->llCustCode;
	std::cout << "�ʲ��˻�:" << p_pRtnField->llCuacctCode;
	std::cout << "ί������:" << p_pRtnField->iOrderBsn;
	std::cout << "�˻����:" << p_pRtnField->iCuacctSn;
	std::cout << "���װ��:" << p_pRtnField->szStkbd;
	std::cout << "ί��״̬:" << p_pRtnField->chOrderStatus;
	std::cout << "֤ȯҵ��:" << p_pRtnField->iStkBiz;
	std::cout << "ҵ����Ϊ:" << p_pRtnField->iStkBizAction;
	std::cout << "ί������" << p_pRtnField->iOrderDate;
	std::cout << "ί�����" << p_pRtnField->iOrderSn;
	std::cout << "�ڲ�����" << p_pRtnField->iIntOrg;
	std::cout << "���׵�Ԫ" << p_pRtnField->szStkpbu;
	std::cout << "ί�м۸�" << p_pRtnField->szOrderPrice;
	std::cout << "ί������" << p_pRtnField->llOrderQty;
	std::cout << "֤ȯ�˻��ӱ���" << p_pRtnField->szSubacctCode;
	std::cout << "��Ȩ��Լ�˻�" << p_pRtnField->szOptTrdacct;
	std::cout << "��Լ����" << p_pRtnField->szOptCode;
	std::cout << "��Լ���" << p_pRtnField->szOptName;
	std::cout << "���Ҵ���" << p_pRtnField->chCurrency;
	std::cout << "���֤ȯ���" << p_pRtnField->chOptUndlCls;
	std::cout << "���֤ȯ����" << p_pRtnField->szOptUndlCode;
	std::cout << "���֤ȯ����" << p_pRtnField->szOptUndlName;

	//TODO: û�ػص�
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

	std::cout << "    ==  ==  ==�ɽ��ر� ��Լ��Ϣ==  ==  ==" << std::endl;
	std::cout << " �����г�:" << p_pRtnField->chStkex;
	std::cout << " ���װ��:" << p_pRtnField->szStkbd;
	std::cout << " ֤ȯ����:" << p_pRtnField->szStkCode;
	std::cout << " �����˻�:" << p_pRtnField->szTrdacct;
	std::cout << " �ͻ�����:" << p_pRtnField->llCustCode;
	std::cout << " �ʲ��˻�:" << p_pRtnField->llCuacctCode;
	std::cout << " ֤ȯҵ��:" << p_pRtnField->iStkBiz;
	std::cout << " ֤ȯҵ����Ϊ:" << p_pRtnField->iStkBizAction;
	std::cout << " ֤ȯ�˻��ӱ���:" << p_pRtnField->szSubacctCode;
	std::cout << " ��Ȩ��Լ�˻�:" << p_pRtnField->szOptTrdacct;
	std::cout << " ������������:" << p_pRtnField->szOwnerType;
	std::cout << " ��Լ����:" << p_pRtnField->szOptCode;
	std::cout << " ��Լ���:" << p_pRtnField->szOptName;
	std::cout << " ���Ҵ���:" << p_pRtnField->chCurrency;
	std::cout << " ���֤ȯ���:" << p_pRtnField->chOptUndlCls;
	std::cout << " ���֤ȯ����:" << p_pRtnField->szOptUndlCode;
	std::cout << " ���֤ȯ����:" << p_pRtnField->szOptUndlName;
	std::cout << " Ԥռ�ñ�֤��:" << p_pRtnField->szMarginPreFrz;
	std::cout << " ռ�ñ�֤��:" << p_pRtnField->szMarginFrz;
	std::cout << " Ԥ�ͷű�֤��:" << p_pRtnField->szMarginPreUfz;
	std::cout << " �ͷű�֤��:" << p_pRtnField->szMarginUfz << std::endl;

	return 0;
}

// ������Ȩ����ί�в�ѯ��Ӧ
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
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][����ί�в�ѯ���󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		cout << "  ��λ��:" << p_pRspField->szQryPos;
		cout << "  ��������:" << p_pRspField->iTrdDate;
		cout << "  ί������:" << p_pRspField->iOrderDate;
		cout << " ί��ʱ��:" << p_pRspField->szOrderTime;
		cout << "  ί������:" << p_pRspField->iOrderBsn;
		cout << "  ��ͬ���:" << p_pRspField->szOrderId;
		cout << " ί��״̬:" << p_pRspField->chOrderStatus;
		cout << " ί����Ч��־:" << p_pRspField->chOrderValidFlag;
		cout << "  �ڲ�����:" << p_pRspField->iIntOrg;
		cout << "  �ͻ�����:" << p_pRspField->llCustCode;
		cout << "  �ʲ��˻�:" << p_pRspField->llCuacctCode;
		cout << "  �����г�:" << p_pRspField->chStkex;
		cout << "  ���װ��:" << p_pRspField->szStkbd;
		cout << "  ���׵�Ԫ:" << p_pRspField->szStkpbu;
		cout << "  ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		cout << "  ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		cout << "  ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		cout << "  ֤ȯҵ��:" << p_pRspField->iStkBiz;
		cout << "  ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
		cout << "  ������������:" << p_pRspField->szOwnerType;
		cout << "  ��Լ����:" << p_pRspField->szOptNum;
		cout << "  ��Լ����:" << p_pRspField->szOptCode;
		cout << "  ��Լ���:" << p_pRspField->szOptName;
		cout << "  ��ϱ���:" << p_pRspField->szCombNum;
		cout << "  ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
		cout << "  �ɷ�һ:" << p_pRspField->szLeg1Num;
		cout << "  ���Ҵ���:" << p_pRspField->chCurrency;
		cout << "  ί�м۸�:" << p_pRspField->szOrderPrice;
		cout << "  ί������:" << p_pRspField->llOrderQty;
		cout << "  ί�н��:" << p_pRspField->szOrderAmt;
		cout << "  ί�ж�����:" << p_pRspField->szOrderFrzAmt;
		cout << " ί�нⶳ���:" << p_pRspField->szOrderUfzAmt;
		cout << " �걨����:" << p_pRspField->llOfferQty;
		cout << " �걨ʱ��:" << p_pRspField->iOfferStime;
		cout << " �ѳ�������:" << p_pRspField->llWithdrawnQty;
		cout << "  �ѳɽ�����:" << p_pRspField->llMatchedQty;
		cout << "  �ѳɽ����:" << p_pRspField->szMatchedAmt;
		cout << " ������־:" << p_pRspField->chIsWithdraw;
		cout << " �ѳ�����־:" << p_pRspField->chIsWithdrawn;
		cout << " ���֤ȯ���:" << p_pRspField->chOptUndlCls;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode << endl;

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

// ������Ȩ���ճɽ���ѯ��Ӧ
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
		write_log(m_bscSink, LL_INFO, "[TradeMAOpt][������Ȩ���ճɽ���ѯ���󷵻ؽ��: {},{},{},{}, ]", p_pFirstSetField->chMsgLevel, p_pFirstSetField->iMsgCode, p_pFirstSetField->szMsgDebug, p_pFirstSetField->szMsgText);

		if (p_iFieldNum > 0)
		{
			write_log(m_bscSink, LL_INFO, "[TradeMAOpt][��������: {}", p_iFieldNum);
		}
	}

	if (p_iFieldNum > 0)
	{
		cout << " ��λ��:" << p_pRspField->szQryPos;
		cout << " ��������:" << p_pRspField->iTrdDate;
		cout << " �ɽ�ʱ��:" << p_pRspField->szMatchedTime;
		cout << " ί������:" << p_pRspField->iOrderDate;
		cout << " ί�����:" << p_pRspField->iOrderSn;
		cout << " ί������:" << p_pRspField->iOrderBsn;
		cout << " ��ͬ���:" << p_pRspField->szOrderId;
		cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		cout << " �ͻ�����:" << p_pRspField->llCustCode;
		cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		cout << " �����г�:" << p_pRspField->chStkex;
		cout << " ���װ��:" << p_pRspField->szStkbd;
		cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		cout << "  ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		cout << " ֤ȯҵ��:" << p_pRspField->iStkBiz;
		cout << " ֤ȯҵ����Ϊ:" << p_pRspField->iStkBizAction;
		cout << " ������������:" << p_pRspField->szOwnerType;
		cout << " ��Լ����:" << p_pRspField->szOptNum;
		cout << " ��Լ����:" << p_pRspField->szOptCode;
		cout << " ��Լ���:" << p_pRspField->szOptName;
		cout << "  ��ϱ���:" << p_pRspField->szCombNum;
		cout << "  ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
		cout << "  �ɷ�һ:" << p_pRspField->szLeg1Num;
		cout << "  �ɷֶ�:" << p_pRspField->szLeg2Num;
		cout << " ���Ҵ���:" << p_pRspField->chCurrency;
		cout << " ���֤ȯ���:" << p_pRspField->chOptUndlCls;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
		cout << " ί�м۸�:" << p_pRspField->szOrderPrice;
		cout << " ί������:" << p_pRspField->llOrderQty;
		cout << " ί�н��:" << p_pRspField->szOrderAmt;
		cout << " ί�ж�����:" << p_pRspField->szOrderFrzAmt;
		cout << " ������־:" << p_pRspField->chIsWithdraw;
		cout << " �ɽ�����:" << p_pRspField->chMatchedType;
		cout << " �ɽ����:" << p_pRspField->szMatchedSn;
		cout << " �ѳɽ�����:" << p_pRspField->llMatchedQty;
		cout << " �ѳɽ����:" << p_pRspField->szMatchedAmt;
		cout << " �ⲿ��ͬ���:" << p_pRspField->szOrderIdEx;
		cout << " Ԥռ�ñ�֤��:" << p_pRspField->szMarginPreFrz;
		cout << " ռ�ñ�֤��:" << p_pRspField->szMarginFrz;
		cout << " Ԥ�ⶳ��֤��:" << p_pRspField->szMarginPreUfz;
		cout << " �ⶳ��֤��:" << p_pRspField->szMarginUfz;
		cout << " �ɽ�����:" << p_pRspField->szMatchedFee << std::endl;

		if (b_inQryAcct) {
			double fee = m_accountInfo->getCommission();
			m_accountInfo->setCommission(fee + atof(p_pRspField->szMatchedFee));

			return 0;
		}

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
		std::cout << "  ������ϳֲ���ϸ��ѯ���ؽ��:[" << p_pFirstSet->chMsgLevel << "," << p_pFirstSet->iMsgCode << "," << p_pFirstSet->szMsgDebug << "," << p_pFirstSet->szMsgText << "]\n";
		if (p_iFieldNum > 0)
			std::cout << "��������[" << p_iFieldNum << "]:" << std::endl;
	}
	if (0 == p_pFirstSet->iMsgCode&&NULL != p_pRspField)
	{

		std::cout << " ��λ��:" << p_pRspField->szQueryPos;
		std::cout << " �ͻ�����:" << p_pRspField->llCustCode;
		std::cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		std::cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		std::cout << " �����г�:" << p_pRspField->chStkex;
		std::cout << " ���װ��:" << p_pRspField->szStkbd;
		std::cout << " ���׵�Ԫ:" << p_pRspField->szStkpbu;
		std::cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
		std::cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubacctCode;
		std::cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
		std::cout << " ���Ҵ���:" << p_pRspField->chCurrency;
		std::cout << " ��ϱ���:" << p_pRspField->szCombNum;
		std::cout << " ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
		std::cout << " ��ϲ�������:" << p_pRspField->szCombStraName;
		std::cout << " ��ϳֲ��������:" << p_pRspField->llCombPrebln;
		std::cout << " ��ϳֲ����:" << p_pRspField->llCombBln;
		std::cout << " ��ϳֲֿ���:" << p_pRspField->llCombAvl;
		std::cout << " ��ϳֲֽ��׶���:" << p_pRspField->llCombTrdFrz;
		std::cout << " ��ϳֲֽ��׽ⶳ:" << p_pRspField->llCombTrdUfz;
		std::cout << " ��ϳֲֽ�����;:" << p_pRspField->llCombTrdOtd;
		std::cout << " ��ϳֲֽ�������:" << p_pRspField->llCombTrdBln;
		std::cout << " ��ϳֲ����㶳��:" << p_pRspField->llCombClrFrz;
		std::cout << " ��ϳֲ�����ⶳ:" << p_pRspField->llCombClrUfz;
		std::cout << " ��ϳֲ�������;:" << p_pRspField->llCombClrOtd;
		std::cout << " ���ռ�ñ�֤��:" << p_pRspField->llSecuCombMargin;
		std::cout << " �ɷֺ�Լ����:" << p_pRspField->iLegCnt;
		std::cout << " �ɷ�һ��Լ����:" << p_pRspField->szLeg1Num;
		std::cout << " �ɷ�һ��Լ����:" << p_pRspField->chLeg1Type;
		std::cout << " �ɷ�һ�ֲַ���:" << p_pRspField->chLeg1Side;
		std::cout << " �ɷ�һ�ֲ�����:" << p_pRspField->llLeg1Posi;
		std::cout << " �ɷֶ���Լ����:" << p_pRspField->szLeg2Num;
		std::cout << " �ɷֶ���Լ����:" << p_pRspField->chLeg2Type;
		std::cout << " �ɷֶ��ֲַ���:" << p_pRspField->chLeg2Side;
		std::cout << " �ɷֶ��ֲ�����:" << p_pRspField->llLeg2Posi;
		std::cout << " �ɷ�����Լ����:" << p_pRspField->szLeg3Num;
		std::cout << " �ɷ�����Լ����:" << p_pRspField->chLeg3Type;
		std::cout << " �ɷ����ֲַ���:" << p_pRspField->chLeg3Side;
		std::cout << " �ɷ����ֲ�����:" << p_pRspField->llLeg3Posi;
		std::cout << " �ɷ��ĺ�Լ����:" << p_pRspField->szLeg4Num;
		std::cout << " �ɷ��ĺ�Լ����:" << p_pRspField->chLeg4Type;
		std::cout << " �ɷ��ĳֲַ���:" << p_pRspField->chLeg4Side;
		std::cout << " �ɷ��ĳֲ�����:" << p_pRspField->llLeg4Posi << std::endl;
	}

	return 0;
}

int TraderMAOpt::RspQryTradingAccount(void)
{
	cout << "��ѯ�ʽ��˻���Ϣ" << endl;

	// ���ò�ѯΪtrue
	b_inQryAcct = true;

	// ���ú�Լ
	CReqOptExpendableCuField stField = { 0 };
	stField.llCustCode = m_llCustCode;
	stField.llCuacctCode = m_llCuacctCode;

	strncpy(stField.szStkbd, m_strStkBD.c_str(), sizeof(stField.szStkbd));
	strncpy(stField.szStkpbu, m_strStkPBU.c_str(), sizeof(stField.szStkpbu));
	strncpy(stField.szTrdacct, m_strTrdAcct.c_str(), sizeof(stField.szTrdacct));

	int iRet = m_pUserAPI->ReqQryExpendableCu(&stField, genRequestID());
	if (iRet)
		cout << "ReqQryExpendableCu Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

	// �����ʽ�
	CReqOptExpendableFundField stFieldFund = { 0 };
	stFieldFund.llCustCode = m_llCustCode;
	stFieldFund.llCuacctCode = m_llCuacctCode;
	stFieldFund.iValueFlag = 15;
	iRet = m_pUserAPI->ReqQryExpendableFund(&stFieldFund, genRequestID());
	if (iRet)
		cout << "ReqQryExpendableFund Error::" << m_pUserAPI->GetLastErrorText() << std::endl;

	// ���ճɽ�
	this->queryTrades();

	Sleep(5000);

	/* ��ӡ�˻���Ϣ */
	cout << "�������: " << m_accountInfo->getPreBalance();
	cout << "  �������: " << m_accountInfo->getBalance();
	cout << "  ��ȡ�ʽ�: " << m_accountInfo->getAvailable();
	cout << "  ƽ������: " << m_accountInfo->getCloseProfit();
	cout << "  ����ӯ��: " << m_accountInfo->getDynProfit();
	cout << "  ռ�ñ�֤��: " << m_accountInfo->getMargin();
	cout << "  ����������: " << m_accountInfo->getCommission() << endl;

	WTSArray * ay = WTSArray::create();
	ay->append(m_accountInfo, false);

	_asyncio.post([this, ay] {
		if (m_bscSink)
			m_bscSink->onRspAccount(ay);

		ay->release();
	});

	b_inQryAcct = false;

	return 0;
}

int TraderMAOpt::OnRspQryBaseInfo(CFirstSetField *p_pFirstSet, CRspOptBaseInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (p_pFirstSet == NULL)
	{
		std::cout << "p_pFirstSet is NULL" << std::endl;
		return -1;
	}

	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		std::cout << "  ��Ȩ������Ϣ��ѯ���ؽ��:[" << p_pFirstSet->chMsgLevel << "," << p_pFirstSet->iMsgCode << "," << p_pFirstSet->szMsgDebug << "," << p_pFirstSet->szMsgText << "]\n";
		if (p_iFieldNum > 0)
			std::cout << "��������[" << p_iFieldNum << "]:" << std::endl;
	}

	if (p_iFieldNum > 0)
	{
		cout << " �����г�:" << p_pRspField->chStkex;
		cout << " ���װ��:" << p_pRspField->szStkbd;
		cout << " ��Լ����:" << p_pRspField->szOptNum;
		cout << " ��Լ����:" << p_pRspField->szOptCode;
		cout << " ��Լ���:" << p_pRspField->szOptName;
		cout << " ��Լ����:" << p_pRspField->chOptType;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlCode;
		cout << " ���֤ȯ����:" << p_pRspField->szOptUndlName;
		cout << " ���֤ȯ���:" << p_pRspField->chOptUndlCls;
		cout << " ���з�ʽ:" << p_pRspField->chOptExeType;
		cout << " ��Լ��λ:" << p_pRspField->llOptUnit;
		cout << " ��Ȩ��:" << p_pRspField->szExepcisePrice;
		cout << " �׸�������:" << p_pRspField->iStartDate;
		cout << " �������:" << p_pRspField->iEndDate;
		cout << " ��Ȩ��Ȩ��:" << p_pRspField->iExeRciseDate;
		cout << " ��Ȩ������:" << p_pRspField->iExpireDate;
		cout << " ��Լ�汾��:" << p_pRspField->chUpdVersion;
		cout << " δƽ�ֺ�Լ��:" << p_pRspField->llLeavesQty;
		cout << " �������̼�:" << p_pRspField->szPreClosePx;
		cout << " ���ս����:" << p_pRspField->szPreSettPrice;
		cout << " ���ȯǰ����:" << p_pRspField->szUndlClsPrice;
		cout << " �ǵ�����������:" << p_pRspField->chPriceLmtType;
		cout << " ��ͣ�۸�:" << p_pRspField->szOptUplmtPrice;
		cout << " ��ͣ�۸�:" << p_pRspField->szOptLwlmtPrice;
		cout << " ��λ��֤��:" << p_pRspField->szMarginUnit;
		cout << " ��֤������������һ:" << p_pRspField->szMarginRatio1;
		cout << " ��֤��������������:" << p_pRspField->szMarginRatio2;
		cout << " ������:" << p_pRspField->llOptLotSize;
		cout << " �޼��򵥱��걨����:" << p_pRspField->llOptLbuplmtQty;
		cout << " �޼��������걨����:" << p_pRspField->llOptLsuplmtQty;
		cout << " �޼۵����걨����:" << p_pRspField->llOptLlwlmtQty;
		cout << " �м��򵥱��걨����:" << p_pRspField->llOptMbuplmtQty;
		cout << " �м��������걨����:" << p_pRspField->llOptMsuplmtQty;
		cout << " �м۵����걨����:" << p_pRspField->llOptMlwlmtQty;
		cout << " ���ֱ�־:" << p_pRspField->chOpenFlag;
		cout << " ͣ�Ʊ�־:" << p_pRspField->chSuspendedFlag;
		cout << " �����ձ�־:" << p_pRspField->chExpireFlag;
		cout << " ������־:" << p_pRspField->chAdjustFlag;
		cout << " ��Լ״̬:" << p_pRspField->chOptStatus;
		cout << " ��Լ��ϱ�־λ:" << p_pRspField->chCombFlag;
		cout << " ��ϲ��Ե��ڽ����:" << p_pRspField->iAutoSplitDate;
		cout << " ��������:" << p_pRspField->iUpdDate;
		cout << " ��Ȩ������:" << p_pRspField->iDeliveryDate << std::endl;

		m_strOptNum = p_pRspField->szOptName;
		cout << "opt num: " << m_strOptNum << endl;
	}

	return 0;
}

int TraderMAOpt::OnRspQrySettList(CFirstSetField* p_pFirstSet, CRspOptSettListField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	//g_vc6_vs10_IO<<"  Debug info:"<<m_pApi->GetLastErrorText()<<std::endl;
	if (p_pFirstSet == NULL)
	{
		cout << "p_pFirstSet is NULL" << std::endl;
		return -1;
	}
	if (p_iFieldNum == 0 || p_iFieldIndex == 1)
	{
		cout << "  �ͻ����㵥��ѯ���ؽ��:[" << p_pFirstSet->chMsgLevel << "," << p_pFirstSet->iMsgCode << "," << p_pFirstSet->szMsgDebug << "," << p_pFirstSet->szMsgText << "]\n";

		if (p_iFieldNum > 0)
			cout << "��������[" << p_iFieldNum << "]:" << std::endl;
	}
	if (0 == p_pFirstSet->iMsgCode && NULL != p_pRspField)
	{
		cout << " ��������:" << p_pRspField->iTrdDate;
		cout << " �ڲ�����:" << p_pRspField->iIntOrg;
		cout << " �ͻ�����:" << p_pRspField->llCustCode;
		cout << " �ͻ�����:" << p_pRspField->szCustName;
		cout << " �ʲ��˻�:" << p_pRspField->llCuacctCode;
		cout << " ���Ҵ���:" << p_pRspField->chCurrency;
		cout << " �ڳ����:" << p_pRspField->szFundPrebln;
		cout << " ��ĩ���:" << p_pRspField->szFundBln;
		cout << " �����ʽ�:" << p_pRspField->szFundAvl;
		cout << " ��Ȩ�ʽ𶳽���:" << p_pRspField->szFundExeFrz;
		cout << " ��Ȩ����ά�ֱ�֤��:" << p_pRspField->szFundExeMargin;
		cout << " ռ�ñ�֤��:" << p_pRspField->szFundUsedMargin;
		cout << " �渶�ʽ�:" << p_pRspField->szPaylater;
		cout << " Ԥ�Ƶ��ʷ�Ϣ:" << p_pRspField->szExpectPnint;
		cout << " �黹����:" << p_pRspField->szRepayPaylater;
		cout << " �黹��Ϣ:" << p_pRspField->szRepayPnint;
		cout << " ���ⷣϢ:" << p_pRspField->szRemitPnint;
		cout << " ��Ϣ�鱾:" << p_pRspField->szInterstIncome;
		cout << " ��Ϣ˰:" << p_pRspField->szInterstIncomeTax;
		cout << " �������:" << p_pRspField->szInAmt;
		cout << " ���ܳ���:" << p_pRspField->szOutAmt;
		cout << " Ȩ�����ո�:" << p_pRspField->szPremiumPay;
		cout << " ��Ȩ�ո�:" << p_pRspField->szExecPay;
		cout << " ������:" << p_pRspField->szSXFFee;
		cout << " �����:" << p_pRspField->szQsfFee;
		cout << " ���ַ�:" << p_pRspField->szJsfFee;
		cout << " ���������ַ�:" << p_pRspField->szJysJsfFee;
		cout << " ��Ȩ������:" << p_pRspField->szXqghFee;
		cout << " ��Ȩ�����:" << p_pRspField->szXqjsfFee;
		cout << " ��Ȩ������:" << p_pRspField->szXqsxfFee;
		cout << " ����ӯ��:" << p_pRspField->llOptFloatProfit;
		cout << " ռ��������:" << p_pRspField->llQuotaValUsed;
		cout << " ������:" << p_pRspField->llQuotaVal;
		cout << " Ȩ������ֵ:" << p_pRspField->szRightMktVal;
		cout << " �������ֵ:" << p_pRspField->szDuteMktVal;
		cout << " ��֤�������:" << p_pRspField->szMarginRiskRate;
		cout << " Ӧ׷�ӱ�֤��:" << p_pRspField->szAddMarginAmt;
		cout << " ׷��֪ͨ����:" << p_pRspField->szAddMarginNotice;
		cout << " �ͻ�ȷ�ϱ�־:" << p_pRspField->chConfirmFlag;
		cout << " �ͻ�ȷ�ϱ�־:" << p_pRspField->chConfirmFlag;
		cout << " �ͻ�ȷ��ʱ��:" << p_pRspField->szConfirmTime << std::endl;
	}
	else
	{
		cout << " �ͻ����㵥��ѯʧ��" << std::endl;
	}
	return 0;
}

int TraderMAOpt::OnRspMaxTradeQty(CFirstSetField *p_pFirstSet, CRspOptMaxTradeQtyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if (IsErrorRspInfo(p_pFirstSet)) {
		if (p_iFieldNum == 0 || p_iFieldIndex == 1)
		{
			cout << "  ���������Ȩ���ɽ����������󷵻ؽ��:[" << p_pFirstSet->chMsgLevel << "," << p_pFirstSet->iMsgCode << "," << p_pFirstSet->szMsgDebug << "," << p_pFirstSet->szMsgText << "]\n";
			if (p_iFieldNum > 0)
			{
				cout << "��������[" << p_iFieldNum << "]:" << std::endl;
			}
		}

		if (p_iFieldNum > 0)
		{
			cout << " �����г�:" << p_pRspField->chStkex;
			cout << " ���װ��:" << p_pRspField->szStkbd;
			cout << " ֤ȯ�˻�:" << p_pRspField->szTrdacct;
			cout << " ֤ȯ�˻��ӱ���:" << p_pRspField->szSubAcctcode;
			cout << " ��Ȩ��Լ�˻�:" << p_pRspField->szOptTrdacct;
			cout << " ��Լ����:" << p_pRspField->szOptNum;
			cout << " ֤ȯ����:" << p_pRspField->szStkCode;

			cout << " ��ϱ���:" << p_pRspField->szCombNum;
			cout << " ��ϲ��Դ���:" << p_pRspField->szCombStraCode;
			cout << " �ɷ�һ:" << p_pRspField->szLeg1Num;
			cout << " �ɷֶ�:" << p_pRspField->szLeg2Num;
			cout << " �ɷ���:" << p_pRspField->szLeg3Num;
			cout << " �ɷ���:" << p_pRspField->szLeg4Num;

			cout << " ί������:" << p_pRspField->llOrderQty << std::endl;
		}

		return 0;
	}
	
	return -1;
}
