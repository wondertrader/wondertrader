/*!
 * \file ParserXTP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserXTP.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSParams.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserXTP* parser = new ParserXTP();
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

inline double checkValid(double val)
{
	if (val == DBL_MAX || val == FLT_MAX)
		return 0;

	return val;
}

ParserXTP::ParserXTP()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserXTP::~ParserXTP()
{
	m_pUserAPI = NULL;
}

bool ParserXTP::init(WTSParams* config)
{
	m_strHost	= config->getCString("host");
	m_iPort		= config->getInt32("port");
	m_strUser = config->getCString("user");
	m_strPass = config->getCString("pass");
	m_iProtocol = (XTP_PROTOCOL_TYPE)config->getUInt32("protocol");
	m_uClientID = config->getUInt32("clientid");
	m_uHBInterval = config->getUInt32("hbinterval");
	m_uBuffSize = config->getUInt32("buffsize");
	m_strFlowDir = config->getCString("flowdir");

	if (m_strFlowDir.empty())
		m_strFlowDir = "XTPMDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	std::string module = config->getCString("xtpmodule");
	if (module.empty())
		module = "xtpquoteapi";

	std::string path = StrUtil::printf("%s/%s/", m_strFlowDir.c_str(), m_strUser.c_str());
	boost::filesystem::create_directories(path.c_str());

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;
	m_hInst = DLLHelper::load_library(dllpath.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateQuoteApi@QuoteApi@API@XTP@@SAPEAV123@EPEBDW4XTP_LOG_LEVEL@@@Z";
#	else
	const char* creatorName = "?CreateQuoteApi@QuoteApi@API@XTP@@SAPAV123@EPBDW4XTP_LOG_LEVEL@@@Z";
#	endif
#else
	const char* creatorName = "_ZN3XTP3API8QuoteApi14CreateQuoteApiEhPKc13XTP_LOG_LEVEL";
#endif
	m_funcCreator = (XTPCreater)DLLHelper::get_symbol(m_hInst, creatorName);
	m_pUserAPI = m_funcCreator(m_uClientID, path.c_str(), XTP_LOG_LEVEL_DEBUG);
	m_pUserAPI->RegisterSpi(this);

	return true;
}

void ParserXTP::release()
{
	disconnect();
}

bool ParserXTP::connect()
{
	DoLogin();

	return true;
}

bool ParserXTP::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserXTP::OnError(XTPRI *error_info)
{
	IsErrorRspInfo(error_info);
}

/*
void ParserXTP::OnFrontConnected()
{
	if (m_sink)
	{
		m_sink->handleParserLog(LL_INFO, "[ParserXTP]CTP行情服务已连接");
		m_sink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void ParserXTP::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_uTradingDate = strtoul(m_pUserAPI->GetTradingDay(), NULL, 10);
		
		if(m_sink)
		{
			m_sink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		SubscribeMarketData();
	}
}
*/

void ParserXTP::OnDisconnected(int nReason)
{
	if(m_sink)
	{
		m_sink->handleParserLog(LL_ERROR, "[ParserXTP] Market data server disconnected: %d...", nReason);
		m_sink->handleEvent(WPE_Close, 0);
	}
}

void ParserXTP::OnUnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{

}

void ParserXTP::OnDepthMarketData(XTPMD *market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count, int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count)
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = (uint32_t)(market_data->data_time / 1000000000);
	uint32_t actTime = market_data->data_time % 1000000000;
	uint32_t actHour = actTime / 10000000;

	std::string code, exchg;
	if (market_data->exchange_id == XTP_EXCHANGE_SH)
	{
		exchg = "SSE";
	}
	else
	{
		exchg = "SZSE";
	}
	code = market_data->ticker;

	WTSContractInfo* ct = m_pBaseDataMgr->getContract(code.c_str(), exchg.c_str());
	if(ct == NULL)
	{
		if (m_sink)
			m_sink->handleParserLog(LL_ERROR, "[ParserXTP] Instrument %s.%s not exists...", exchg.c_str(), market_data->ticker);
		return;
	}
	WTSCommodityInfo* commInfo = m_pBaseDataMgr->getCommodity(ct);

	WTSTickData* tick = WTSTickData::create(code.c_str());
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, commInfo->getExchg());
	
	quote.action_date = actDate;
	quote.action_time = actTime;
	
	quote.price = checkValid(market_data->last_price);
	quote.open = checkValid(market_data->open_price);
	quote.high = checkValid(market_data->high_price);
	quote.low = checkValid(market_data->low_price);
	quote.total_volume = (uint32_t)market_data->qty;
	quote.trading_date = m_uTradingDate;
	quote.total_turnover = market_data->turnover;

	if (commInfo->getCategoty() == CC_Future)
	{
		quote.settle_price = market_data->settl_price;
		quote.open_interest = (uint32_t)market_data->total_long_positon;

		quote.pre_settle = checkValid(market_data->pre_settl_price);
		quote.pre_interest = (uint32_t)market_data->pre_total_long_positon;
	}

	quote.upper_limit = checkValid(market_data->upper_limit_price);
	quote.lower_limit = checkValid(market_data->lower_limit_price);

	quote.pre_close = checkValid(market_data->pre_close_price);	

	//委卖价格
	for (int i = 0; i < 10; i++)
	{
		quote.ask_prices[i] = checkValid(market_data->ask[i]);
		quote.ask_qty[i] = (uint32_t)market_data->ask_qty[i];

		quote.bid_prices[i] = checkValid(market_data->bid[i]);
		quote.bid_qty[i] = (uint32_t)market_data->bid_qty[i];
	}

	if(m_sink)
		m_sink->handleQuote(tick, true);

	tick->release();
}

void ParserXTP::OnSubMarketData(XTPST *ticker, XTPRI *error_info, bool is_last)
{
	if (!IsErrorRspInfo(error_info))
	{

	}
	else
	{
		if(m_sink)
			m_sink->handleParserLog(LL_ERROR, "[ParserXTP] Market data subscribe faile, code: %s%s", ticker->exchange_id == XTP_EXCHANGE_SH ? "SSE." : "SZSE.", ticker->ticker);
	}
}

void ParserXTP::DoLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	int iResult = m_pUserAPI->Login(m_strHost.c_str(), m_iPort, m_strUser.c_str(), m_strPass.c_str(), m_iProtocol);
	if(iResult != 0)
	{
		if (m_sink)
		{
			if(iResult == -1)
			{
				m_sink->handleEvent(WPE_Connect, iResult);
			}
			else
			{
				m_sink->handleEvent(WPE_Connect, 0);

				m_sink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP] Sending login request failed: %d", iResult).c_str());
			}
			
		}
	}
	else
	{
		m_uTradingDate = strToTime(m_pUserAPI->GetTradingDay());
		if (m_sink)
		{
			m_sink->handleEvent(WPE_Connect, 0);
			m_sink->handleEvent(WPE_Login, 0);
		}

		DoSubscribeMD();
	}
}

void ParserXTP::DoSubscribeMD()
{
	CodeSet codeFilter = m_fitSHSubs;
	if(!codeFilter.empty())
	{
		char ** subscribe = new char*[codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SH);
			if (iResult != 0)
			{
				if (m_sink)
					m_sink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP] Sending md subscribe request of SSE failed: %d", iResult).c_str());
			}
			else
			{
				if (m_sink)
					m_sink->handleParserLog(LL_INFO, StrUtil::printf("[ParserXTP] Market data of %u instruments of SSE subscribed", nCount).c_str());
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = m_pUserAPI->SubscribeAllMarketData(XTP_EXCHANGE_SH);
	}

	codeFilter = m_fitSZSubs;
	if (!codeFilter.empty())
	{
		char ** subscribe = new char*[codeFilter.size()];
		int nCount = 0;
		CodeSet::iterator it = codeFilter.begin();
		for (; it != codeFilter.end(); it++)
		{
			subscribe[nCount++] = (char*)(*it).c_str();
		}

		if (m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SZ);
			if (iResult != 0)
			{
				if (m_sink)
					m_sink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP] Sending md subscribe request of SZSE failed: %d", iResult).c_str());
			}
			else
			{
				if (m_sink)
					m_sink->handleParserLog(LL_INFO, StrUtil::printf("[ParserXTP] Market data of %u instruments of SZSE subscribed", nCount).c_str());
			}
		}
		codeFilter.clear();
		delete[] subscribe;
		//int iResult = m_pUserAPI->SubscribeAllMarketData(XTP_EXCHANGE_SZ);
	}
}

bool ParserXTP::IsErrorRspInfo(XTPRI *error_info)
{
	if (error_info == NULL || error_info->error_id ==0)
		return false;

	return true;
}

void ParserXTP::subscribe(const CodeSet &vecSymbols)
{
	if(m_uTradingDate == 0)
	{
		for(auto& code : vecSymbols)
		{
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				m_fitSHSubs.insert(code.substr(4));
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				m_fitSZSubs.insert(code.substr(5));
			}
		}
	}
	else
	{
		CodeSet setSH, setSZ;
		for (auto& code : vecSymbols)
		{
			if (strncmp(code.c_str(), "SSE.", 4) == 0)
			{
				m_fitSHSubs.insert(code.substr(4));
				setSH.insert(code.substr(4));
			}
			else if (strncmp(code.c_str(), "SZSE.", 5) == 0)
			{
				m_fitSZSubs.insert(code.substr(5));
				setSZ.insert(code.substr(5));
			}
		}

		if (!setSH.empty())
		{
			char ** subscribe = new char*[setSH.size()];
			int nCount = 0;
			CodeSet::iterator it = setSH.begin();
			for (; it != setSH.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (m_pUserAPI && nCount > 0)
			{
				int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SH);
				if (iResult != 0)
				{
					if (m_sink)
						m_sink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP] Sending md subscribe request of SSE failed: %d", iResult).c_str());
				}
				else
				{
					if (m_sink)
						m_sink->handleParserLog(LL_INFO, StrUtil::printf("[ParserXTP] Market data of %u instruments of SSE subscribed", nCount).c_str());
				}
			}
			delete[] subscribe;
		}

		if (!setSZ.empty())
		{
			char ** subscribe = new char*[setSZ.size()];
			int nCount = 0;
			CodeSet::iterator it = setSZ.begin();
			for (; it != setSZ.end(); it++)
			{
				subscribe[nCount++] = (char*)(*it).c_str();
			}

			if (m_pUserAPI && nCount > 0)
			{
				int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount, XTP_EXCHANGE_SZ);
				if (iResult != 0)
				{
					if (m_sink)
						m_sink->handleParserLog(LL_ERROR, StrUtil::printf("[ParserXTP] Sending md subscribe request of SZSE failed: %d", iResult).c_str());
				}
				else
				{
					if (m_sink)
						m_sink->handleParserLog(LL_INFO, StrUtil::printf("[ParserXTP] Market data of %u instruments of SZSE subscribed", nCount).c_str());
				}
			}
			delete[] subscribe;
		}
	}
}

void ParserXTP::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserXTP::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserXTP::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}