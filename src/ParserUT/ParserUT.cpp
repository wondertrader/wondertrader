/*!
 * \file ParserUT.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserUT.h"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/WTSParams.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/IBaseDataMgr.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/charconv.hpp"

#include <boost/filesystem.hpp>

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "./UTApi/xyzq_ut_api64.lib")
#else
#pragma comment(lib, "./UTApi/xyzq_ut_api32.lib")
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

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void *)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif


std::string getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = g_moduleName;
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir;
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserUT* parser = new ParserUT();
		return parser;
	}

	EXPORT_FLAG void deleteParser(IParserApi* &parser)
	{
		if (NULL != parser)
		{
			delete parser;
			parser = NULL;
		}
	}
};


sub_fields& getFields()
{
	static sub_fields fields;
	if (fields.empty())
	{
		for (int j = 1; j <= 50; j++)
			fields.push_back(j);

		fields.push_back(60);	//交易日
		fields.push_back(61);	//毫秒数
		fields.push_back(63);	//涨停价
		fields.push_back(64);	//跌停价
	}
	return fields;
}

ParserUT::ParserUT()
	: m_pUserAPI(NULL)
	, m_bLogined(false)
{
}


ParserUT::~ParserUT()
{
	m_pUserAPI = NULL;
}

bool ParserUT::init(WTSParams* config)
{
	m_strFrontAddr	= config->getCString("front");
	m_strUserID = config->getCString("user");
	m_strUserNo = config->getCString("userno");
	m_strPassword = config->getCString("pass");

	m_bHasOrder = config->getBoolean("order");
	m_bHasOrdQue = config->getBoolean("order_queue");
	m_bHasTrans = config->getBoolean("transaction");

	std::string module = config->getCString("utmodule");
	if (module.empty())
	{

#ifdef _WIN32
		module = "XYZQ_UT_API.dll";
#else
		module = "libxyzq_ut_api.so";
#endif
	}
	std::string dllpath = getBinDir() + module;
	DLLHelper::load_library(dllpath.c_str());

	m_pUserAPI = xyzq_ut_api::create_ut_api();
	m_pUserAPI->init();
	m_pUserAPI->register_spi(this);
	m_pUserAPI->register_address((char*)m_strFrontAddr.c_str());

	return true;
}

void ParserUT::release()
{
	disconnect();
}

bool ParserUT::connect()
{
	DoLogin();

	return true;
}

bool ParserUT::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->logout();
		m_pUserAPI->register_spi(NULL);
		m_pUserAPI->release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserUT::rsp_error(error_msg *arg)
{
	bool isError = IsErrorRspInfo(arg);
	if(isError)
	{
		std::string s = UTF8toChar(arg->msg);
		if (m_parserSink)
			m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]%s(%d)", s.c_str(), arg->err_code);
	}
	else
	{
		std::string s = UTF8toChar(arg->msg);
		if (m_parserSink)
			m_parserSink->handleParserLog(LL_INFO, "[ParserUT]%s", s.c_str());
	}
}

void ParserUT::rsp_status_changed(int status)
{
	switch (status)
	{
	case XYUT_INFO_LOGIN_SUCCESS:
	break;
	case XYUT_INFO_CONNECTION_LOST:
	{
		if (m_parserSink)
		{
			m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]行情服务连接已断开...");
			m_parserSink->handleEvent(WPE_Close, 0);
		}
	}
	break;
	default:
		break;
	}
}

void ParserUT::rsp_market_data_ex(ut_data_list *arg)
{
	if (arg == NULL)
		return;

	int code_count = arg->data_list.size();
	for (int j = 0; j < code_count; j++)
	{
		WTSTickData* tick = makeTickData(&arg->data_list[j]);
		if(tick)
		{
			if (m_parserSink)
				m_parserSink->handleQuote(tick, true);
			tick->release();
		}
	}
}

void ParserUT::rsp_transactions(ut_data *pTransData)
{
	StringVector ay = StrUtil::split(pTransData->xyzq_code, ".");
	std::string code;
	if (ay[1].compare("SSE") == 0)
		code += "SH";
	else
		code += "SZ";
	code += ay[0];

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(code.c_str(), ay[1].c_str());
	if (contract == NULL)
	{
		//if (m_ignoreCodes.find(code) == m_ignoreCodes.end())
		//{
		//	m_ignoreCodes.insert(code);
		//	if (m_parserSink)
		//		m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]品种%s不存在，忽略该品种数据", code.c_str());
		//}
		return;
	}

	WTSCommodityInfo* pCommInfo = m_pBaseDataMgr->getCommodity(contract);

	WTSTransData* trans = WTSTransData::create(code.c_str());
	WTSTransStruct& ts = trans->getTransStruct();
	strcpy(ts.exchg, pCommInfo->getExchg());

	uint32_t actTime = 0;
	for (ut_data_item& item : pTransData->quotations)
	{
		switch (item.tag)
		{
		case 1:
			actTime += item.uint_value * 1000; break;
		case 2:
			ts.price = item.double_value; break;
		case 9:
			ts.volumn = item.int_value; break;
		case 61:
			actTime += item.int_value; break;;
		case 110:	//买卖方向
			ts.side = (WTSBSDirectType)item.int_value; break;
		case 113:	//成交编号
			ts.index = item.int_value; break;
		case 117:	//卖方委托号
			ts.bidorder = item.int_value; break;
		case 118:	//买方委托号
			ts.askorder = item.int_value; break;
		case 119:	//成交类型
			ts.ttype = item.int_value == 0 ? TT_Unknown : (item.int_value == 1 ? TT_Match : TT_Cancel); break;
		default:
			break;
		}
	}

	ts.action_date = TimeUtils::getCurDate();
	ts.action_time = actTime;
	ts.trading_date = ts.action_date;

	if(ts.side == BDT_Unknown)
	{
		if (ts.askorder > ts.bidorder)
			ts.side = BDT_Sell;
		else
			ts.side = BDT_Buy;
	}

	if (m_parserSink)
		m_parserSink->handleTransaction(trans);

	trans->release();
}

void ParserUT::rsp_orders(ut_data *pOrderData)
{
	StringVector ay = StrUtil::split(pOrderData->xyzq_code, ".");
	std::string code;
	if (ay[1].compare("SSE") == 0)
		code += "SH";
	else
		code += "SZ";
	code += ay[0];

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(code.c_str(), ay[1].c_str());
	if (contract == NULL)
	{
		//if (m_ignoreCodes.find(code) == m_ignoreCodes.end())
		//{
		//	m_ignoreCodes.insert(code);
		//	if (m_parserSink)
		//		m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]品种%s不存在，忽略该品种数据", code.c_str());
		//}
		return;
	}

	WTSOrdDtlData* order = WTSOrdDtlData::create(code.c_str());
	WTSOrdDtlStruct& ods = order->getOrdDtlStruct();
	strcpy(ods.exchg, contract->getExchg());

	uint32_t actTime = 0;
	for (ut_data_item& item : pOrderData->quotations)
	{
		switch (item.tag)
		{
		case 1:
			actTime += item.uint_value * 1000; break;
		case 111:
			ods.price = item.double_value; break;
		case 112:
			ods.volumn = item.int_value; break;
		case 61:
			actTime += item.int_value; break;;
		case 110:	//买卖方向
			ods.side = (WTSBSDirectType)item.int_value; break;
		case 114:	//委托编号
			ods.index = item.int_value; break;
		case 115:	//委托类型
			ods.otype = (WTSOrdDetailType)item.int_value; break;
		default:
			break;
		}
	}

	ods.action_date = TimeUtils::getCurDate();
	ods.action_time = actTime;
	ods.trading_date = ods.action_date;

	if (m_parserSink)
		m_parserSink->handleOrderDetail(order);

	order->release();
}

void ParserUT::rsp_order_list(ut_order_queue_list *arg)
{
	StringVector ay = StrUtil::split(arg->xyzq_code, ".");
	std::string code;
	if (ay[1].compare("SSE") == 0)
		code += "SH";
	else
		code += "SZ";
	code += ay[0];

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(code.c_str(), ay[1].c_str());
	if (contract == NULL)
	{
		//if (m_ignoreCodes.find(code) == m_ignoreCodes.end())
		//{
		//	m_ignoreCodes.insert(code);
		//	if (m_parserSink)
		//		m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]品种%s不存在，忽略该品种数据", code.c_str());
		//}
		return;
	}
	WTSCommodityInfo* pCommInfo = m_pBaseDataMgr->getCommodity(contract);

	uint32_t actTime = arg->time * 1000 + arg->millisecond;
	uint32_t actDate = TimeUtils::getCurDate();

	for(auto& item : arg->order_queues)
	{
		WTSOrdQueData* pData = WTSOrdQueData::create(code.c_str());
		WTSOrdQueStruct& oqs = pData->getOrdQueStruct();
		strcpy(oqs.exchg, contract->getExchg());
		oqs.trading_date = actDate;
		oqs.action_date = actDate;
		oqs.action_time = actTime;
		oqs.side = (item.direction%2 == 0) ? BDT_Buy : BDT_Sell;
		oqs.order_items = item.orders_count;
		oqs.price = item.price;
		oqs.qsize = item.orders.size();
		memcpy(oqs.volumns, item.orders.data(), sizeof(int)*oqs.qsize);

		if (m_parserSink)
			m_parserSink->handleOrderQueue(pData);

		pData->release();
	}
}

void ParserUT::rsp_base_info(rsp_base_info_s *arg)
{

}

void ParserUT::rsp_market_data(ut_data *arg)
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	WTSTickData* tick = makeTickData(arg);
	if (tick)
	{
		if (m_parserSink)
			m_parserSink->handleQuote(tick, true);

		tick->release();
	}
}


WTSTickData* ParserUT::makeTickData(ut_data* pMktData)
{
	StringVector ay = StrUtil::split(pMktData->xyzq_code, ".");
	std::string code = ay[0];
	std::string exchg;
	if (ay[1].compare("SSE") == 0)
		exchg = "SSE";
	else
		exchg = "SZSE";

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(code.c_str(), exchg.c_str());
	if (contract == NULL)
	{
		//if (m_ignoreCodes.find(code) == m_ignoreCodes.end())
		//{
		//	m_ignoreCodes.insert(code);
		//	if (m_parserSink)
		//		m_parserSink->handleParserLog(LL_ERROR, "[ParserUT]品种%s不存在，忽略该品种数据", code.c_str());
		//}
		return NULL;
	}

	WTSCommodityInfo* pCommInfo = m_pBaseDataMgr->getCommodity(contract);

	WTSTickData* tick = WTSTickData::create(code.c_str());
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());

	uint32_t actTime = 0;
	uint32_t actDate = 0;
	for(ut_data_item& item : pMktData->quotations)
	{
		switch(item.tag)
		{
		case 1:
			actTime += item.uint_value*1000; break;
		case 2:
			quote.price = item.double_value; break;
		case 3:
			quote.pre_close = item.double_value; break;
		case 4:
			quote.open = item.double_value; break;
		case 5:
			quote.high = item.double_value; break;
		case 6:
			quote.low = item.double_value; break;
		case 7:
			quote.total_volumn = (uint32_t)(item.int64_value/100); break;
		case 8:
			quote.total_turnover = item.double_value; break;
		case 9:
			quote.volumn = item.int_value; break;
		case 10:
			quote.turn_over = item.double_value; break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
			quote.bid_prices[item.tag-11] = item.double_value; break;
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
			quote.ask_prices[item.tag - 21] = item.double_value; break;
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
			quote.bid_qty[item.tag - 31] = item.int_value; break;
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
			quote.ask_qty[item.tag - 41] = item.int_value; break;
		case 60:
			actDate = item.int_value; break;
		case 61:
			actTime += item.int_value; break;
		case 63:
			quote.upper_limit = item.double_value; break;
		case 64:
			quote.lower_limit = item.double_value; break;
		}
	}

	quote.action_date = actDate;
	quote.action_time = actTime;
	quote.trading_date = actDate;

	return tick;
}

void ParserUT::DoLogin()
{
	if (m_pUserAPI == NULL)
	{
		return;
	}

	authorize_struct as = { m_strUserNo.c_str(), m_strUserID.c_str(), m_strPassword.c_str() };

	int iResult = m_pUserAPI->authorize(&as);
	if (iResult != 0)
	{
		if (m_parserSink)
		{
			if (iResult == -1)
			{
				m_parserSink->handleEvent(WPE_Connect, iResult);
			}
			else
			{
				m_parserSink->handleEvent(WPE_Connect, 0);

				m_parserSink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP]登录请求发送失败, 错误码:%d", iResult).c_str());
			}

		}
	}
	else
	{
		m_bLogined = true;
		ut_set_config(XYUT_CONFIG_AUTO_RELOGIN, XYUT_CONFIG_VALUE_AUTO_RELOGIN);

		// 设置参数，让浮点数数据按整数收取（需要自行除以放大倍数，以得到正确的数据）
		// 此参数默认为按浮点数收取，在本演示程序中不作设置
		//ut_set_config(XYUT_CONFIG_RETRIEVE_FLOAT_AS_INT, 1);

		// 设置参数，使用多个合约的数据一次性推送的回调rsp_market_data_ex
		ut_set_config(XYUT_CONFIG_MARKET_DATA_MODE, XYUT_CONFIG_VALUE_MARKET_DATA_MULTIPLE_MODE);
		if (m_parserSink)
		{
			m_parserSink->handleEvent(WPE_Connect, 0);
			m_parserSink->handleEvent(WPE_Login, 0);
		}

		SubscribeMDs(false);
	}
}

void ParserUT::SubscribeMDs(bool isAll /* = false */)
{
	code_list codes;

	if(!isAll)
	{
		CodeSet codeFilter = m_filterSubs;
		if (m_filterSubs.empty())
		{//如果订阅礼包只空的,则取出全部合约列表
			return;
		}


		for (const std::string& code : m_filterSubs)
		{
			std::string s;
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				s = code.substr(4) + ".SSE";
			}
			else //if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				s += code.substr(5) + ".SZE";
			}
			codes.push_back(s);
		}
	}
	else
	{
		codes.push_back("ALLCODES.ALL");
	}
	

	m_pUserAPI->subscribe(&codes, &getFields());

	if (m_bHasOrder)
		m_pUserAPI->subscribe_orders(&codes);

	if (m_bHasOrdQue)
		m_pUserAPI->subscribe_order_queues(&codes);

	if (m_bHasTrans)
		m_pUserAPI->subscribe_transactions(&codes);
}

bool ParserUT::IsErrorRspInfo(error_msg *pRspInfo)
{
	if (pRspInfo == NULL || pRspInfo->err_code == 0)
		return false;

	return true;
}

void ParserUT::subscribe(const CodeSet &vecSymbols)
{
	if (!m_bLogined)
	{
		m_filterSubs = vecSymbols;
	}
	else
	{
		m_filterSubs = vecSymbols;

		code_list codes;
		for (const std::string& code : vecSymbols)
		{
			std::string s;
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				s = code.substr(4) + ".SSE";
			}
			else //if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				s += code.substr(5) + ".SZE";
			}
			codes.push_back(s);
		}

		m_pUserAPI->subscribe(&codes, &getFields());

		if (m_bHasOrder)
			m_pUserAPI->subscribe_orders(&codes);

		if (m_bHasOrdQue)
			m_pUserAPI->subscribe_order_queues(&codes);

		if (m_bHasTrans)
			m_pUserAPI->subscribe_transactions(&codes);
	}
}

void ParserUT::unsubscribe(const CodeSet &vecSymbols)
{
	unsub stru;
	for (const std::string& code : vecSymbols)
	{
		std::string s;
		if (strncmp(code.c_str(), "SSE.", 4) == 0)
		{
			s = code.substr(4) + ".SSE";
		}
		else //if (strncmp(code.c_str(), "SZSE.", 5) == 0)
		{
			s += code.substr(5) + ".SZE";
		}
		stru.codes.push_back(s);
	}
	stru.sub_type = ut_all_sub;
	m_pUserAPI->unsubscribe(&stru);
}

bool ParserUT::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserUT::registerListener(IParserApiListener* listener)
{
	m_parserSink = listener;

	if(m_parserSink)
		m_pBaseDataMgr = m_parserSink->getBaseDataMgr();
}