/*!
 * \file TraderDD.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/07/15
 * 
 * \brief 
 */
//#ifndef _WIN64
//#define _WIN64
//#endif // !_WIN64

#include "TraderDD.h"
#include "../API/FixApi/include/fiddef.h"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"

#include <iostream>

#include "../Share/BoostFile.hpp"
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

/*
 *	顶点A5接口的要点：
 *	1 Fix_GetItem需要传入一个buffer长度的参数，这个参数每次调用以后会被改写，必须重置为buffer长度，下次调用才能成功
 *	2 回报推送混乱，订单回报有时候不会推送，成交以后、撤单以后都不会推送原订单的最新状态，所以需要手动做以下处理：
 *		a 下单成功以后直接模拟一个订单回报，状态“未成交”
 *		b 本地缓存未完成订单，当撤单回报来的时候，读取原订单的数据，然后修改原订单的状态，再模拟一个原订单的回报
 *		c 收到成交回报以后，也需要读取原订单的数据，修改原订单的状态和成交数量、剩余数量，再模拟一个原订单的回报
 */
void inst_hlp() {}

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/FixApi/x64/fixapitool.lib")
#pragma comment(lib, "../API/FixApi/x64/FixApi50.lib")	//64位的库
#else
#pragma message("x86 version")
#pragma comment(lib, "../API/FixApi/x86/fixapi50_x86.lib")	//32位的库
#endif
#include <wtypes.h>
HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}
#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

const std::string& getInstPath()
{
	static std::string moduleName;
	if (moduleName.empty())
	{
		Dl_info dl_info;
		dladdr((void *)inst_hlp, &dl_info);
		moduleName = dl_info.dli_fname;
	}

	return moduleName;
}
#endif

std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
#ifdef _WIN64
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#endif
#else
		_bin_dir = getInstPath();
#endif
		boost::filesystem::path p(_bin_dir);
		_bin_dir = p.branch_path().string() + "/";
	}

	return _bin_dir;
}

const char* ENTRUST_SECTION = "entrusts";
const char* ORDER_SECTION = "orders";

inline const char* exchgI2O(const char* exchg)
{
	if (strcmp(exchg, "SSE") == 0)
		return "SH";
	else if (strcmp(exchg, "SZSE") == 0)
		return "SZ";

	return "";
}

inline const char* exchgO2I(const char* exchg)
{
	if (strcmp(exchg, "SH") == 0)
		return "SSE";
	else if (strcmp(exchg, "SZ") == 0)
		return "SZSE";

	return "";
}

WTSOrderState wrapOrderState(int state)
{
	switch (state)
	{
	case 0: return WOS_Nottouched; break;
	case 1: return WOS_Submitting; break;
	case 2: return WOS_NotTraded_NotQueuing; break;
	case 5: return WOS_PartTraded_Queuing; break;
	case 6: return WOS_AllTraded; break;
	default:
		return WOS_Canceled;
		break;
	}
}


uint32_t strToTime(const char* strTime)
{
	std::string str;
	const char *pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':' && pos[0] != '.')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	return strtoul(str.c_str(), NULL, 10);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderDD *instance = new TraderDD();
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

static bool cb_global(HANDLE_CONN conn, HANDLE_SESSION sess, int nResv)
{//异步run函数回调原型
	int code = -1;
	std::string errmsg;
	char buf[256];
	int len = 256;
	code = Fix_GetCode(sess);
	len = 256;
	Fix_GetErrMsg(sess, buf, len);
	errmsg = buf;
	printf("[%lld][%lld]call back : code: %d, errmsg: %s\n", conn, sess, code, errmsg.c_str());
	return true;
}

//订单回调
bool cb_rtn_order(HANDLE_CONN hconn, HANDLE_SESSION hsess, int64_t subid, void *pData) {
	/**
	调用Fix_GetPublishType先判断事件。
	*/
	FIX_PUBLISH_TYPE type = Fix_GetPublishType(hsess);
	if (type == FIX_PUB_TYPE_COMM)
	{//收到发布,一般用户在此应快速处理，如进队列，不应积压
		//printf("push order report begin ... \n");

		TraderDD* trader = (TraderDD*)pData;
		trader->OnRtnOrder(hconn, hsess);
	}
	else if (type == FIX_PUB_TYPE_RESUBS_BEFORE)
	{//底层重订阅开始
	}
	else if (type == FIX_PUB_TYPE_RESUBS_AFTER_SUCC)
	{//底层重订阅成功
	}
	else if (type == FIX_PUB_TYPE_RESUBS_AFTER_FAIL)
	{//底层重订阅失败
	}
	else if (type == FIX_PUB_TYPE_DOWN)
	{//网关中断或异常
	}
	else
	{//未知
	}
	return true;
}

//成交回调
bool cb_rtn_trade(HANDLE_CONN hconn, HANDLE_SESSION hsess, int64_t subid, void *pData)
{
	/**
	调用Fix_GetPublishType先判断事件。
	*/
	FIX_PUBLISH_TYPE type = Fix_GetPublishType(hsess);
	if (type == FIX_PUB_TYPE_COMM)
	{//收到发布,一般用户在此应快速处理，如进队列，不应积压
		//printf("trade report begin ... \n");

		TraderDD* trader = (TraderDD*)pData;
		trader->OnRtnTrade(hconn, hsess);
	}
	else if (type == FIX_PUB_TYPE_RESUBS_BEFORE)
	{//底层重订阅开始
	}
	else if (type == FIX_PUB_TYPE_RESUBS_AFTER_SUCC)
	{//底层重订阅成功
	}
	else if (type == FIX_PUB_TYPE_RESUBS_AFTER_FAIL)
	{//底层重订阅失败
	}
	else if (type == FIX_PUB_TYPE_DOWN)
	{//网关中断或异常
	}
	else
	{//未知
	}

	//printf("gointo sub resp\n");
	//int code = -1;
	//std::string errmsg;
	//char buf[256];
	//int len = 256;

	//code = Fix_GetCode(hsess);
	//len = 256;
	//Fix_GetErrMsg(hsess, buf, len);
	//printf("type:%d,code:%d errmsg:%s\n", type, code, buf);
	return true;
}

/*获取字段的值*/
string GetItem(HANDLE_SESSION sess, int fid)
{
	int out_len = 2048;
	char out_buf[2048];
	memset(out_buf, 0, out_len);

	if (Fix_GetItem(sess, fid, out_buf, out_len) != nullptr)
	{
		string item = out_buf;
		return item;
	}

	return "";
}

void InitializeFix(WTSVariant* params)
{
	static bool bInited = false;
	if(!bInited)
	{
		bInited = true;

		Fix_Initialize();
		Fix_SetAppInfo("WonderTrader", "1.2");
		//Fix_SetDefaultInfo("9991", "4", "100001", "100002");
		Fix_WriteLog(true);
		Fix_SetLogPath("./FixApi5Data/Logs/");
		Fix_SetLogLevel(1);
		//Fix_SetParamEx("apexsoft-losap-atongle", "000");

		Fix_RegReplyCallFunc(0, (void*)cb_global);//设置全局回调函数
	}
}

TraderDD::TraderDD()
	: m_hConn(NULL)
	, m_wrapperState(WS_NOTLOGIN)
	, m_uLastQryTime(0)
	, m_bInQuery(false)
	, m_strandIO(NULL)
	, m_lastQryTime(0)
	, m_orderRef(1)
	, m_lDate(0)
	, m_mapLives(NULL)
{
	m_mapLives = TradeDataMap::create();
}


TraderDD::~TraderDD()
{

}

bool TraderDD::init(WTSVariant* params)
{
	m_strFront = params->getCString("front");

	m_strCommUser = params->getCString("commuser");
	m_strCommPass = params->getCString("commpass");

	m_strUser = params->getCString("user");
	m_strPass = params->getCString("pass");

	m_strNode = params->getCString("node");
	m_strTrust = params->getCString("trustmethod");

	WTSVariant* param = params->get("ddmodule");
	if (param != NULL)
	{
		m_strModule = getBinDir() + param->asCString();
	}
	else
	{
#ifdef _WIN32
#ifdef _WIN64
		m_strModule = getBinDir() + "fixapitool.dll";
		m_strModule = getBinDir() + "FixApi50.dll";
#else
		m_strModule = getBinDir() + "fixapitool.dll";
		m_strModule = getBinDir() + "fixapi50_x86.dll";
#endif
#else
		m_strModule = getBinDir() + "libfixapitool.dll";
		m_strModule =  getBinDir() + "libfixapi.so";
#endif
	}

	m_hInstDD = DLLHelper::load_library(m_strModule.c_str());

	InitializeFix(params);

	return true;
}

void TraderDD::release()
{
	if(m_hConn != NULL)
	{
		Fix_Close(m_hConn);
		m_hConn = NULL;
	}

	if (m_mapLives)
		m_mapLives->release();
}

void TraderDD::reconnect()
{
	if(m_hConn != NULL)
	{
		Fix_Close(m_hConn);
		m_hConn = NULL;
	}

	m_hConn = Fix_Connect(m_strFront.c_str(), m_strCommUser.c_str(), m_strCommPass.c_str(), 20000);
	if (m_hConn == NULL)
	{
		if (m_traderSink)
		{
			m_traderSink->handleEvent(WTE_Connect, -1);
			m_traderSink->handleTraderLog(LL_ERROR, "[TraderDD]通讯连接失败");
		}

		StdThreadPtr thrd(new StdThread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			if (m_traderSink) write_log(m_traderSink, LL_WARN, "[TraderDD]账号{}正在重连……", m_strUser.c_str());
			reconnect();
		}));
		return;
	}

	//int64_t subid = 0;
	//int code = -1;
	//char buf[256];
	////订阅成交回报
	//{
	//	HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
	//	Fix_SetNode(sess, m_strNode.c_str());
	//	Fix_SetWTFS(sess, "8");
	//	Fix_CreateHead(sess, "399001");//399001 订阅委托成交消息
	//	m_strToken = GetItem(sess, FID_TOKEN);
	//	if (!Fix_SubscribeByToken(sess, subid, m_strUser.c_str(), m_strToken.c_str(), (void *)cb_rtn_trade, this))   //  Fix_SubscribeByCustomer
	//	{
	//		int len = 128;
	//		code = Fix_GetCode(sess);
	//		len = 256;
	//		Fix_GetErrMsg(sess, buf, len);
	//		if (m_traderSink)
	//			write_log(m_traderSink, LL_ERROR, "[TraderDD]订阅成交回报失败：{}({})", buf, code);
	//		Fix_ReleaseSession(sess);
	//	}
	//	Fix_ReleaseSession(sess);
	//}

	////订阅订单回报
	//{
	//	HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
	//	Fix_SetNode(sess, m_strNode.c_str());
	//	Fix_SetWTFS(sess, "8");
	//	Fix_CreateHead(sess, "399000");//399000 订阅委托确认消息
	//	int64_t subid = 0;
	//	m_strToken = GetItem(sess, FID_TOKEN);
	//	if (!Fix_SubscribeByToken(sess, subid, m_strUser.c_str(), m_strToken.c_str(), (void *)cb_rtn_order, this))  // Fix_SubscribeByCustomer
	//	{//session 订阅id，客户号，交易密码，回调函数，pData(回调中传回)
	//		int len = 128;
	//		code = Fix_GetCode(sess);
	//		len = 256;
	//		Fix_GetErrMsg(sess, buf, len);
	//		if (m_traderSink) 
	//			write_log(m_traderSink, LL_ERROR, "[TraderDD]订阅委托回报失败：{}({})", buf, code);
	//		Fix_ReleaseSession(sess);
	//		return;
	//	}
	//	Fix_ReleaseSession(sess);
	//}

	if (m_traderSink) m_traderSink->handleEvent(WTE_Connect, 0);
}

void TraderDD::connect()
{
	if (m_thrdWorker == NULL)
	{
		m_strandIO = new boost::asio::io_service::strand(m_asyncIO);
		boost::asio::io_service::work work(m_asyncIO);
		m_thrdWorker.reset(new StdThread([this]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				m_asyncIO.run_one();
			}
		}));
	}

	reconnect();
}

void TraderDD::disconnect()
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

bool TraderDD::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		uint32_t orderref = m_orderRef.fetch_add(1) + 1;
		//sprintf(buffer, "%s#%u",StrUtil::fmtUInt64(m_uSessID).c_str(), orderref);
		fmt::format_to(buffer, "{}#{}", m_uSessID, orderref);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

void TraderDD::registerSpi(ITraderSpi *listener)
{
	m_traderSink = listener;
	if (m_traderSink)
	{
		m_bdMgr = listener->getBaseDataMgr();
	}
}

int TraderDD::login(const char* user, const char* pass, const char* productInfo)
{
	m_strUser = user;
	m_strPass = pass;

	if (m_hConn == NULL)
	{
		return -1;
	}

	m_wrapperState = WS_LOGINING;

	doLogin();

	return 0;
}

void TraderDD::qryGDNo()
{
	m_strandIO->post([this]() {

		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "310001");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]股东号查询失败: {}({})", buf, code);
		}
		else
		{
			int row = Fix_GetCount(sess);
			for (int i = 0; i < row; i++)
			{
				int col = Fix_GetColumnCount(sess, i);
				char buf[256] = { 0 };
				int nSize = 256;
				std::string thisNO;
				std::string thisExchg;
				Fix_GetItem(sess, FID_GDH, buf, nSize, i);
				thisNO = buf;
				Fix_GetItem(sess, FID_JYS, buf, nSize, i);
				thisExchg = buf;

				if (thisExchg == "SH")
					m_strSHNO = thisNO;
				else if (thisExchg == "SZ")
					m_strSZNO = thisNO;
			}

			qryZJZH();

			//m_traderSink->handleTraderLog(LL_ERROR, "[TraderDD]账户数据初始化完成...");
			//m_wrapperState = WS_ALLREADY;
			//m_traderSink->onLoginResult(true, "", m_lDate);
		}
		Fix_ReleaseSession(sess);
	});
}

void TraderDD::qryZJZH()
{
	m_strandIO->post([this]() {

		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "200001");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]资金账户查询失败: {}({})", buf, code);

		}
		else
		{
			
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetItem(sess, FID_ZJZH, buf, len);

			m_strFDNO = buf;

			write_log(m_traderSink, LL_INFO, "[TraderDD]账户数据初始化完成...");
			m_wrapperState = WS_ALLREADY;
			m_traderSink->onLoginResult(true, "", m_lDate);
		}
		Fix_ReleaseSession(sess);
	});
}

void TraderDD::doLogin()
{
	m_strandIO->post([this]() {

		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_CreateHead(sess, "100001");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_JYMM, m_strPass.c_str());
		Fix_SetItem(sess, FID_JMLX, "2");
		Fix_SetItem(sess, FID_PRODUCT, m_strProdInfo.c_str());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);
		m_uSessID = sess;

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			m_wrapperState = WS_LOGINFAILED;
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]交易通道登录失败: {}({})", buf, code);
			m_traderSink->onLoginResult(false, buf, 0);
		}
		else
		{
			std::string msg = GetItem(sess, FID_MESSAGE);
			write_log(m_traderSink, LL_INFO, "[TraderDD] Request Login success, code: {}, message: {}", code,  msg);
			
			Fix_SetDefaultSystemId(4);

			int row = Fix_GetCount(sess);
			for (int i = 0; i < row; i++)
			{
				int col = Fix_GetColumnCount(sess, i);
				char buf[256] = { 0 };
				int nSize = 256;
				for (int j = 0; j < col; j++)
				{
					int nFid = 0;
					nSize = 256;
					if (Fix_GetValWithIdByIndex(sess, i, j, nFid, buf, nSize))
					{
						switch (nFid)
						{
						case FID_KHXM: m_strUserName = buf; break;
						case FID_YWLX: m_strSystemID = buf; break;
						case FID_NODEID: 
							{
								auto ayNodes = StrUtil::split(buf, ",");
								m_strNodeID = ayNodes[0];
							}
							break;
						case FID_TOKEN: m_strToken = buf; break;
						default:
							break;
						}
					}
				}
			}

			Fix_SetDefaultNodeId(atoi(m_strNodeID.c_str()));

			std::stringstream ss;
			ss << "./FixApi5Data/local/";
			std::string path = StrUtil::standardisePath(ss.str());
			if (!StdFile::exists(path.c_str()))
				boost::filesystem::create_directories(path.c_str());
			ss << m_strUser << ".dat";

			m_iniHelper.load(ss.str().c_str());
			uint32_t lastDate = m_iniHelper.readUInt("marker", "date", 0);
			if (lastDate != m_lDate)
			{
				//交易日不同，清理掉原来的数据
				m_iniHelper.removeSection(ENTRUST_SECTION);
				m_iniHelper.removeSection(ORDER_SECTION);
				m_iniHelper.writeUInt("marker", "date", m_lDate);
				m_iniHelper.save();

				write_log(m_traderSink, LL_INFO, "[TraderDD][%s]交易日已切换[{} -> {}]，清空本地数据缓存……", m_strUser.c_str(), lastDate, m_lDate);
			}

			m_wrapperState = WS_LOGINED;
			m_lDate = TimeUtils::getCurDate();
			
			qryGDNo();

			//m_traderSink->handleTraderLog(LL_ERROR, "[TraderDD]账户数据初始化完成...");
			//m_wrapperState = WS_ALLREADY;
			//m_traderSink->onLoginResult(true, "", m_lDate);
		}
		Fix_ReleaseSession(sess);

		int64_t subid = 0;
		char buf[256];

		//订阅订单回报
		{
			HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
			Fix_SetNode(sess, m_strNode.c_str());
			Fix_SetWTFS(sess, "8");
			Fix_CreateHead(sess, "399000");//399000 订阅委托确认消息
			int64_t subid = 0;

			if (!Fix_SubscribeByToken(sess, subid, m_strUser.c_str(), m_strToken.c_str(), (void *)cb_rtn_order, this))  // Fix_SubscribeByCustomer
			{//session 订阅id，客户号，交易密码，回调函数，pData(回调中传回)
				int len = 128;
				code = Fix_GetCode(sess);
				len = 256;
				Fix_GetErrMsg(sess, buf, len);
				if (m_traderSink)
					write_log(m_traderSink, LL_ERROR, "[TraderDD]订阅委托回报失败：{}({})", buf, code);
				Fix_ReleaseSession(sess);
				return;
			}
			else
				write_log(m_traderSink, LL_INFO, "[TraderDD]订阅委托回报成功");
			Fix_ReleaseSession(sess);
		}

		//订阅成交回报
		{
			HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
			Fix_SetNode(sess, m_strNode.c_str());
			Fix_SetWTFS(sess, "8");
			Fix_CreateHead(sess, "399001");//399001 订阅委托成交消息
			//m_strToken = GetItem(sess, FID_TOKEN);
			if (!Fix_SubscribeByToken(sess, subid, m_strUser.c_str(), m_strToken.c_str(), (void *)cb_rtn_trade, this))   //  Fix_SubscribeByCustomer  
			{
				int len = 128;
				code = Fix_GetCode(sess);
				len = 256;
				Fix_GetErrMsg(sess, buf, len);
				if (m_traderSink)
					write_log(m_traderSink, LL_ERROR, "[TraderDD]订阅成交回报失败：{}({})", buf, code);
				Fix_ReleaseSession(sess);
				return;
			}
			else
				write_log(m_traderSink, LL_INFO, "[TraderDD]订阅成交回报成功");
			Fix_ReleaseSession(sess);
		}
	});
}

int TraderDD::logout()
{
	if (m_hConn == NULL)
	{
		return -1;
	}

	return 0;
}

int TraderDD::orderInsert(WTSEntrust* entrust)
{
	if (m_hConn == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	entrust->retain();
	m_strandIO->post([this, entrust]() {

		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		bool isBuy = entrust->getOffsetType() == WOT_OPEN;
		bool isSH = strcmp(entrust->getExchg(), "SSE") == 0;
		Fix_CreateHead(sess, isBuy ? "310005" : "310006");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, 1013, "0");
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetItem(sess, FID_JYS, exchgI2O(entrust->getExchg()));
		Fix_SetItem(sess, FID_ZQDM, entrust->getCode());
		Fix_SetItem(sess, FID_GDH, isSH ? m_strSHNO.c_str() : m_strSZNO.c_str());
		Fix_SetItem(sess, FID_WBSQBH, entrust->getEntrustID());

		Fix_SetInt(sess, FID_WTSL, entrust->getVolume());

		Fix_SetDouble(sess, FID_WTJG, entrust->getPrice());

		if (strlen(entrust->getUserTag()) > 0)
		{
			//m_mapEntrustTag[entrust->getEntrustID()] = entrust->getUserTag();
			m_iniHelper.writeString(ENTRUST_SECTION, entrust->getEntrustID(), entrust->getUserTag());
			m_iniHelper.save();
		}

		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]委托指令发送失败: {}({})", buf, code);
			
			WTSError* err = WTSError::create(WEC_ORDERINSERT, buf);
			m_traderSink->onRspEntrust(entrust, err);
		}
		else
		{
			m_traderSink->onRspEntrust(entrust, NULL);

			//这里手动发一笔回报，不然会有问题的
			WTSOrderInfo* ordInfo = WTSOrderInfo::create(entrust);
			ordInfo->setOrderState(WOS_NotTraded_NotQueuing);
			ordInfo->setVolTraded(0);
			ordInfo->setVolLeft(ordInfo->getVolume());
			ordInfo->setOrderDate(m_lDate);
			ordInfo->setOrderTime(TimeUtils::getLocalTimeNow());

			ordInfo->setError(false);

			int len = 256;
			char buf[256] = { 0 };
			Fix_GetItem(sess, FID_WTH, buf, len);
			ordInfo->setOrderID(buf);

			m_iniHelper.writeString(ORDER_SECTION, StrUtil::trim(ordInfo->getOrderID()).c_str(), ordInfo->getUserTag());
			m_iniHelper.save();

			if (m_mapLives == NULL)
				m_mapLives = TradeDataMap::create();

			m_mapLives->add(ordInfo->getOrderID(), ordInfo, false);

			m_traderSink->onPushOrder(ordInfo);
		}

		entrust->release();
		Fix_ReleaseSession(sess);
	});

	return 0;
}

int TraderDD::orderAction(WTSEntrustAction* action)
{
	if (m_wrapperState != WS_ALLREADY)
		return -1;

	action->retain();
	m_strandIO->post([this, action]() {
		write_log(m_traderSink, LL_INFO, "[TraderDD] 调用撤单接口 ..."）;

		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);  // 4 - 股票 9 - 期权
		Fix_CreateHead(sess, "310007");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetItem(sess, FID_WTH, action->getOrderID());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]撤单指令发送失败，错误信息：{}({})", buf, code);

			WTSError* err = WTSError::create(WEC_ORDERCANCEL, buf);
			m_traderSink->onTraderError(err);
		}
		else
			write_log(m_traderSink, LL_INFO, "[TraderDD] 撤单指令发送成功, 返回代码: {}", code);

		action->release();
		Fix_ReleaseSession(sess);
	});

	return 0;
}

int TraderDD::queryAccount()
{
	if (m_hConn == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this]() {
		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "200014");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_ZJZH, m_strFDNO.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]可用资金查询失败: {}({})", buf, code);
		}
		else
		{
			WTSArray* ayFunds = WTSArray::create();
			int row = Fix_GetCount(sess);
			char buf[256] = { 0 };
			int nSize = 256;
			for (int i = 0; i < row; i++)
			{
				WTSAccountInfo* fundInfo = WTSAccountInfo::create();
				fundInfo->setAvailable(Fix_GetDouble(sess, FID_KYZJ, i));
				fundInfo->setBalance(Fix_GetDouble(sess, FID_ZQSZ, i)+fundInfo->getAvailable());

				ayFunds->append(fundInfo, false);
			}

			if (m_traderSink) m_traderSink->onRspAccount(ayFunds);
			ayFunds->release();
		}
		Fix_ReleaseSession(sess);
		m_bInQuery = false;
	});

	triggerQuery();

	return 0;
}

int TraderDD::queryPositions()
{
	if (m_hConn == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this](){
		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "310003");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetItem(sess, FID_ROWCOUNT, "5000");
		Fix_SetSessData(sess, this);
		bool ret = Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (!ret || code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]用户持仓查询失败: {}({})", buf, code);
		}
		else
		{
			WTSArray* ayPositions = WTSArray::create();
			int row = Fix_GetCount(sess);
			char buf[256] = { 0 };
			int nSize = 256;

			for (int i = 0; i < row; i++)
			{
				std::unordered_map<int32_t, std::string> fields;
				int col = Fix_GetColumnCount(sess, i);
				for (int j = 0; j < col; j++)
				{
					int nFid = 0;
					nSize = 256;
					if (Fix_GetValWithIdByIndex(sess, i, j, nFid, buf, nSize))
						fields[nFid] = buf;
				}

				nSize = 256;
				Fix_GetItem(sess, FID_JYS, buf, nSize, i);
				std::string exchg = exchgO2I(buf);

				nSize = 256;
				Fix_GetItem(sess, FID_ZQDM, buf, nSize, i);
				std::string code = buf;

				Fix_GetItem(sess, FID_LTLX, buf, nSize, i);
				if (strncmp(buf, "0", 1) == 0)  // 流通类型,1 流通 0非流通，如果是非流通则过掉
					continue;

				WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg.c_str());
				if(contract)
				{
					WTSCommodityInfo* commInfo = contract->getCommInfo();
					WTSPositionItem* pInfo = WTSPositionItem::create(code.c_str(), commInfo->getCurrency(), exchg.c_str());
					pInfo->setDirection(WDT_LONG);

					double prevol = Fix_GetInt64(sess, FID_ZQSL, i);	//昨天的持仓，今天是不会变的
					double newvol = Fix_GetInt64(sess, FID_JCCL, i);	//今天的持仓，这个是真实持仓
					double openvol = Fix_GetInt64(sess, FID_DRMRCJSL, i);	//今日买入数量
					double closevol = Fix_GetInt64(sess, FID_DRMCCJSL, i);	//今日卖出数量

					pInfo->setPrePosition(prevol - closevol);
					pInfo->setNewPosition(openvol);
					pInfo->setAvailPrePos(pInfo->getPrePosition());

					double cost = Fix_GetDouble(sess, FID_TBCBJ, i);  // atof(fields[FID_TBCBJ].c_str());
					pInfo->setMargin(cost);
					pInfo->setPositionCost(cost);
					if(pInfo->getTotalPosition() > 0)
						pInfo->setAvgPrice(cost/pInfo->getTotalPosition());

					ayPositions->append(pInfo, false);
				}
			}

			if (row >= 200)
			{
				
			}

			if (m_traderSink) m_traderSink->onRspPosition(ayPositions);
			ayPositions->release();
		}
		Fix_ReleaseSession(sess);
		m_bInQuery = false;
	});

	triggerQuery();

	return 0;
}

void TraderDD::OnRtnOrder(HANDLE_CONN hconn, HANDLE_SESSION hsess)
{
	//std::cout << "order report callback " << std::endl;

	char buf[256] = { 0 };
	int len = 256;
	std::map<int32_t, std::string> fields;
	int col = Fix_GetColumnCount(hsess, 0);
	for (int j = 0; j < col; j++)
	{
		int nFid = 0;
		len = 256;
		if (Fix_GetValWithIdByIndex(hsess, 0, j, nFid, buf, len))
			fields[nFid] = buf;
	}

	len = 256;
	Fix_GetItem(hsess, FID_JYS, buf, len);
	std::string exchg = exchgO2I(buf);
	
	len = 256;
	Fix_GetItem(hsess, FID_ZQDM, buf, len);
	std::string code = buf;

	len = 256;
	Fix_GetItem(hsess, FID_CXBZ, buf, len);
	bool isCancel = (buf[0] == 'W');

	WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return;

	WTSOrderInfo* ordInfo = NULL;
	if(!isCancel)
	{
		len = 256;
		Fix_GetItem(hsess, FID_WTH, buf, len);
		ordInfo = (WTSOrderInfo*)m_mapLives->grab(buf);
		if (ordInfo == NULL)
		{
			ordInfo = WTSOrderInfo::create();
			ordInfo->setPrice(Fix_GetDouble(hsess, FID_WTJG));
			ordInfo->setVolume(Fix_GetDouble(hsess, FID_WTSL));
			ordInfo->setDirection(WDT_LONG);
			ordInfo->setPriceType(WPT_LIMITPRICE);
			ordInfo->setOffsetType(Fix_GetDouble(hsess, FID_JYLB) == 1 ? WOT_OPEN : WOT_CLOSE);

			//ordInfo->setOrderDate(m_lDate);
			//len = 256;
			//Fix_GetItem(hsess, FID_WTSJ, buf, len);
			//uint32_t uTime = strToTime(buf);
			//ordInfo->setOrderTime(TimeUtils::makeTime(ordInfo->getOrderDate(), uTime));
			ordInfo->setOrderDate(TimeUtils::getCurDate());
			ordInfo->setOrderTime(TimeUtils::getLocalTimeNow());

			ordInfo->setCode(code.c_str());
			ordInfo->setExchange(contract->getExchg());

			len = 256;
			Fix_GetItem(hsess, FID_WTH, buf, len);
			ordInfo->setOrderID(buf);

			len = 256;
			Fix_GetItem(hsess, FID_WBSQBH, buf, len);
			ordInfo->setEntrustID(buf);

			std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, ordInfo->getEntrustID(), "");
			if (usertag.empty())
			{
				ordInfo->setUserTag(ordInfo->getEntrustID());
			}
			else
			{
				ordInfo->setUserTag(usertag.c_str());

				if (strlen(ordInfo->getOrderID()) > 0)
				{
					m_iniHelper.writeString(ORDER_SECTION, StrUtil::trim(ordInfo->getOrderID()).c_str(), usertag.c_str());
					m_iniHelper.save();
				}
			}

			if (m_mapLives == NULL)
				m_mapLives = TradeDataMap::create();

			if(ordInfo->isAlive())
				m_mapLives->add(ordInfo->getOrderID(), ordInfo, true);
		}

		int state = Fix_GetInt64(hsess, FID_SBJG);
		ordInfo->setOrderState(wrapOrderState(state));
		if (state == 3)
			ordInfo->setError(true);

		double total = Fix_GetInt64(hsess, FID_WTSL);
		double traded = Fix_GetInt64(hsess, FID_CJSL);
		double canceled = Fix_GetInt64(hsess, FID_CDSL);
		ordInfo->setVolume(total);

		if (ordInfo->isAlive())
			ordInfo->setVolLeft(total - canceled - traded);
		else
			ordInfo->setVolLeft(0);		

		len = 256;
		Fix_GetItem(hsess, FID_JGSM, buf, len);
		ordInfo->setStateMsg(buf);

		if (!ordInfo->isAlive())
			m_mapLives->remove(ordInfo->getOrderID());

		//std::cout << "push order report: " << " code: " << ordInfo->getCode() << " exchg: " << ordInfo->getExchg() << " vol: " << ordInfo->getVolume() << " price: " << ordInfo->getPrice() << " msg: " << buf << std::endl;
	}
	else
	{
		len = 256;
		Fix_GetItem(hsess, FID_CXWTH, buf, len);
		printf("cancel order id: %s", buf);
		ordInfo = (WTSOrderInfo*)m_mapLives->grab(buf);
		if (ordInfo == NULL)
		{
			write_log(m_traderSink, LL_ERROR, "[TraderDD] 委托撤单的订单为空！");
			return;
		}

		m_mapLives->remove(buf);

		ordInfo->setVolLeft(0);
		ordInfo->setOrderState(WOS_Canceled);

		len = 256;
		Fix_GetItem(hsess, FID_JGSM, buf, len);
		ordInfo->setStateMsg(buf);

		write_log(m_traderSink, LL_INFO, "[TraderDD] 成功撤销委托订单, 返回信息: {}", buf);
	}

	if (m_traderSink)
		m_traderSink->onPushOrder(ordInfo);

	ordInfo->release();
}

void TraderDD::OnRtnTrade(HANDLE_CONN hconn, HANDLE_SESSION hsess)
{
	char buf[256] = { 0 };
	int len = 256;
	std::map<int32_t, std::string> fields;
	int col = Fix_GetColumnCount(hsess, 0);
	for (int j = 0; j < col; j++)
	{
		int nFid = 0;
		len = 256;
		if (Fix_GetValWithIdByIndex(hsess, 0, j, nFid, buf, len))
			fields[nFid] = buf;
	}
	
	len = 256;
	Fix_GetItem(hsess, FID_JYS, buf, len);
	std::string exchg = exchgO2I(buf);

	len = 256;
	Fix_GetItem(hsess, FID_ZQDM, buf, len);
	std::string code = buf;

	std::string tradeid;
	len = 256;
	Fix_GetItem(hsess, FID_CJBH, buf, len);
	tradeid = buf;

	auto it = m_tradeids.find(tradeid);
	if (it != m_tradeids.end())
		return;

	m_tradeids.insert(tradeid);

	//过滤掉撤单回报
	if (!decimal::eq(Fix_GetDouble(hsess, FID_CDSL), 0.0))
		return;

	WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
		return;

	std::string orderid;
	len = 256;
	Fix_GetItem(hsess, FID_WTH, buf, len);
	orderid = buf;


	WTSCommodityInfo* commInfo = contract->getCommInfo();
	WTSTradeInfo *trdInfo = WTSTradeInfo::create(code.c_str(), exchg.c_str());
	trdInfo->setPrice(Fix_GetDouble(hsess, FID_CJJG));
	trdInfo->setVolume(Fix_GetDouble(hsess, FID_CJSL));

	WTSOrderInfo* ordInfo = (WTSOrderInfo*)m_mapLives->get(orderid);
	if (ordInfo)
	{
		ordInfo->setVolTraded(ordInfo->getVolTraded() + trdInfo->getVolume());
		ordInfo->setVolLeft(ordInfo->getVolLeft() - trdInfo->getVolume());

		if(ordInfo->getVolLeft() == 0)
		{
			ordInfo->setOrderState(WOS_AllTraded);
			if (m_traderSink)
				m_traderSink->onPushOrder(ordInfo);
		}
	}

	trdInfo->setTradeID(tradeid.c_str());

	trdInfo->setTradeDate(m_lDate);

	len = 256;
	Fix_GetItem(hsess, FID_CJSJ, buf, len);
	uint32_t uTime = strToTime(buf);
	trdInfo->setTradeTime(TimeUtils::makeTime(m_lDate, uTime));

	trdInfo->setDirection(WDT_LONG);
	trdInfo->setOffsetType(Fix_GetDouble(hsess, FID_JYLB) == 1 ? WOT_OPEN : WOT_CLOSE);	
	trdInfo->setRefOrder(orderid.c_str());
	trdInfo->setTradeType(WTT_Common);

	trdInfo->setAmount(Fix_GetDouble(hsess, FID_CJJE));

	std::cout << "code: " << trdInfo->getCode() << " exchg: " << trdInfo->getExchg() << " vol: " << trdInfo->getVolume() << " price: " << trdInfo->getPrice() << std::endl;

	std::string usertag = m_iniHelper.readString(ORDER_SECTION, StrUtil::trim(trdInfo->getRefOrder()).c_str());
	if (!usertag.empty())
		trdInfo->setUserTag(usertag.c_str());

	if (m_traderSink)
		m_traderSink->onPushTrade(trdInfo);
}

int TraderDD::queryOrders()
{
	if (m_hConn == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this]() {
		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "310033");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]用户订单查询失败: {}({})", buf, code);
		}
		else
		{
			WTSArray* ayOrds = WTSArray::create();
			int row = Fix_GetCount(sess);
			char buf[256] = { 0 };
			int nSize = 256;
			for (int i = 0; i < row; i++)
			{
				std::unordered_map<int32_t, std::string> fields;
				int col = Fix_GetColumnCount(sess, i);
				for (int j = 0; j < col; j++)
				{
					int nFid = 0;
					nSize = 256;
					if(Fix_GetValWithIdByIndex(sess, i, j, nFid, buf, nSize))
						fields[nFid] = buf;
				}

				nSize = 256;
				Fix_GetItem(sess, FID_JYS, buf, nSize, i);
				std::string exchg = exchgO2I(buf);

				nSize = 256;
				Fix_GetItem(sess, FID_ZQDM, buf, nSize, i);
				std::string code = buf;

				//撤销委托忽略掉
				Fix_GetItem(sess, FID_CXBZ, buf, nSize, i);
				if (strncmp(buf, "W", 1) == 0)
					continue;

				WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg.c_str());
				if (contract)
				{
					WTSCommodityInfo* commInfo = contract->getCommInfo();
					WTSOrderInfo* ordInfo = WTSOrderInfo::create();
					ordInfo->setCode(code.c_str());
					ordInfo->setExchange(exchg.c_str());

					ordInfo->setPrice(Fix_GetDouble(sess, FID_WTJG, i));
					ordInfo->setDirection(WDT_LONG);
					ordInfo->setPriceType(WPT_LIMITPRICE);
					ordInfo->setOffsetType(Fix_GetInt64(sess, FID_JYLB, i) == 1 ? WOT_OPEN : WOT_CLOSE);

					double total = Fix_GetInt64(sess, FID_WTSL, i);
					double traded = Fix_GetInt64(sess, FID_CJSL, i);
					double canceled = Fix_GetInt64(sess, FID_CDSL, i);
					ordInfo->setVolume(total);
					ordInfo->setVolTraded(traded);
					ordInfo->setVolLeft(total - canceled - traded);

					ordInfo->setOrderDate((uint32_t)Fix_GetInt64(sess, FID_WTRQ, i));

					nSize = 256;
					Fix_GetItem(sess, FID_WTSJ, buf, nSize, i);
					uint32_t uTime = strToTime(buf);
					ordInfo->setOrderTime(TimeUtils::makeTime(ordInfo->getOrderDate(), uTime));

					int state = Fix_GetInt64(sess, FID_SBJG, i);
					ordInfo->setOrderState(wrapOrderState(state));
					if (state == 3)
						ordInfo->setError(true);

					nSize = 256;
					Fix_GetItem(sess, FID_WBSQBH, buf, nSize, i);
					ordInfo->setEntrustID(buf);

					nSize = 256;
					Fix_GetItem(sess, FID_WTH, buf, nSize, i);
					ordInfo->setOrderID(buf);

					ordInfo->setStateMsg("");

					//std::cout << "code: " << ordInfo->getCode() << "  exchg: " << ordInfo->getExchg() << "  price: " << ordInfo->getPrice() << "  volume: " << ordInfo->getVolume() << std::endl;

					std::string usertag = m_iniHelper.readString(ENTRUST_SECTION, ordInfo->getEntrustID(), "");
					if (usertag.empty())
					{
						ordInfo->setUserTag(ordInfo->getEntrustID());
					}
					else
					{
						ordInfo->setUserTag(usertag.c_str());

						if (strlen(ordInfo->getOrderID()) > 0)
						{
							m_iniHelper.writeString(ORDER_SECTION, StrUtil::trim(ordInfo->getOrderID()).c_str(), usertag.c_str());
							m_iniHelper.save();
						}
					}

					if(ordInfo->isAlive())
					{
						if (m_mapLives == NULL)
							m_mapLives = TradeDataMap::create();

						m_mapLives->add(ordInfo->getOrderID(), ordInfo, true);
					}
					
					ayOrds->append(ordInfo, false);
				}
			}

			if (m_traderSink) m_traderSink->onRspOrders(ayOrds);
			ayOrds->release();
		}
		Fix_ReleaseSession(sess);
		m_bInQuery = false;
	});

	triggerQuery();

	return 0;
}

int TraderDD::queryTrades()
{
	if (m_hConn == NULL || m_wrapperState != WS_ALLREADY)
	{
		return -1;
	}

	StdUniqueLock lock(m_mtxQuery);
	m_queQuery.push([this]() {
		HANDLE_SESSION sess = Fix_AllocateSession(m_hConn);
		Fix_SetNode(sess, m_strNode.c_str());
		Fix_SetWTFS(sess, "8");
		Fix_SetSystemId(sess, 4);
		Fix_CreateHead(sess, "310034");
		Fix_SetItem(sess, FID_KHH, m_strUser.c_str());
		Fix_SetItem(sess, FID_NODEID, m_strNodeID.c_str());
		Fix_SetItem(sess, FID_TOKEN, m_strToken.c_str());
		Fix_SetItem(sess, FID_ROWCOUNT, "10000");
		Fix_SetSessData(sess, this);
		Fix_Run(sess);

		int code = Fix_GetCode(sess);
		if (code < 0)
		{
			int len = 256;
			char buf[256] = { 0 };
			Fix_GetErrMsg(sess, buf, len);
			write_log(m_traderSink, LL_ERROR, "[TraderDD]用户成交查询失败: {}({})", buf, code);
		}
		else
		{
			WTSArray* ayTrds = WTSArray::create();
			int row = Fix_GetCount(sess);
			char buf[256] = { 0 };
			int nSize = 256;
			for (int i = 0; i < row; i++)
			{
				std::unordered_map<int32_t, std::string> fields;
				int col = Fix_GetColumnCount(sess, i);
				for (int j = 0; j < col; j++)
				{
					int nFid = 0;
					nSize = 256;
					Fix_GetValWithIdByIndex(sess, i, j, nFid, buf, nSize);
					fields[nFid] = buf;
				}

				//撤销委托忽略掉
				//if (fields[FID_CXBZ].compare("O") != 0)
				//	continue;
				Fix_GetItem(sess, FID_CXBZ, buf, nSize, i);
				if (strncmp(buf, "W", 1) == 0)
					continue;

				nSize = 256;
				Fix_GetItem(sess, FID_JYS, buf, nSize, i);
				std::string exchg = exchgO2I(buf);

				nSize = 256;
				Fix_GetItem(sess, FID_ZQDM, buf, nSize, i);
				std::string code = buf;

				WTSContractInfo* contract = m_bdMgr->getContract(code.c_str(), exchg.c_str());
				if (contract)
				{
					WTSCommodityInfo* commInfo = contract->getCommInfo();
					WTSTradeInfo *trdInfo = WTSTradeInfo::create(code.c_str(), exchg.c_str());
					trdInfo->setPrice(Fix_GetDouble(sess, FID_CJJG, i));
					trdInfo->setVolume(Fix_GetInt64(sess, FID_CJSL, i));
					
					nSize = 256;
					Fix_GetItem(sess, FID_CJBH, buf, nSize, i);  // 成交编号
					trdInfo->setTradeID(buf);

					trdInfo->setTradeDate(m_lDate);
					nSize = 256;
					Fix_GetItem(sess, FID_CJSJ, buf, nSize, i);
					trdInfo->setTradeTime(strToTime(buf));

					trdInfo->setDirection(WDT_LONG);
					nSize = 256;
					Fix_GetItem(sess, FID_JYLB, buf, nSize, i);
					trdInfo->setOffsetType(strncpy(buf, "1", 1) == 0 ? WOT_OPEN : WOT_CLOSE);
					nSize = 256;
					Fix_GetItem(sess, FID_WTH, buf, nSize, i);
					trdInfo->setRefOrder(buf);
					trdInfo->setTradeType(WTT_Common);

					nSize = 256;
					Fix_GetItem(sess, FID_CJJE, buf, nSize, i);
					trdInfo->setAmount(atof(buf));

					//std::cout << "code: " << trdInfo->getCode() << "  exchg: " << trdInfo->getExchg() << "  price: " << trdInfo->getPrice() << "  volume: " << trdInfo->getVolume() << std::endl;

					std::string usertag = m_iniHelper.readString(ORDER_SECTION, StrUtil::trim(trdInfo->getRefOrder()).c_str());
					if (!usertag.empty())
						trdInfo->setUserTag(usertag.c_str());

					ayTrds->append(trdInfo, false);
				}
			}

			if (m_traderSink) m_traderSink->onRspTrades(ayTrds);
			ayTrds->release();
		}
		Fix_ReleaseSession(sess);
		m_bInQuery = false;
	});

	triggerQuery();

	return 0;
}

bool TraderDD::isConnected()
{
	return (m_wrapperState == WS_ALLREADY);
}
void TraderDD::triggerQuery()
{
	m_strandIO->post([this](){
		if (m_queQuery.empty() || m_bInQuery)
			return;

		//顶点接口好像没有流量控制，先去掉
		//uint64_t curTime = TimeUtils::getLocalTimeNow();
		//if (curTime - m_lastQryTime < 1000)
		//{
		//	boost::this_thread::sleep(boost::posix_time::milliseconds(50));
		//	m_strandIO->post([this](){
		//		triggerQuery();
		//	});
		//	return;
		//}

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