/*!
 * \file ParserMA.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserMA.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/ModuleHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"

#include <boost/filesystem.hpp>


#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "../API/mCliApi3.7/x64/maMktData.lib")
#else
#pragma comment(lib, "../API/mCliApi3.7/x86/maMktData.lib")
#endif
#endif

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(IParserSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	fmtutil::format_to(buffer, format, args...);

	sink->handleParserLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserMA* parser = new ParserMA();
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
	static char str[10] = { 0 };
	const char *pos = strTime;
	int idx = 0;
	auto len = strlen(strTime);
	for(std::size_t i = 0; i < len; i++)
	{
		if(strTime[i] != ':')
		{
			str[idx] = strTime[i];
			idx++;
		}
	}
	str[idx] = '\0';

	return strtoul(str, NULL, 10);
}

inline double checkValid(double val)
{
	if (val == DBL_MAX || val == FLT_MAX)
		return 0;

	return val;
}

ParserMA::ParserMA()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserMA::~ParserMA()
{
	m_pUserAPI = NULL;
}

bool ParserMA::init(WTSVariant* config)
{
	m_strHost = config->getCString("host");
	m_strUser = config->getCString("user");
	m_strPass = config->getCString("pass");

	m_iPort = config->getInt32("port");
	m_iMode = config->getInt32("mode");

	std::string module = config->getCString("mamodule");
	if (module.empty())
		module = "thostmduserapi_se";

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");
	m_hInstMA = DLLHelper::load_library(dllpath.c_str());
	m_pUserAPI = new CCliMktDataApi();
	m_pUserAPI->RegisterSpi(this);
	m_pUserAPI->RegisterServer(m_strHost.c_str(), m_iPort);

	return true;
}

void ParserMA::release()
{
	disconnect();
}

bool ParserMA::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool ParserMA::disconnect()
{
	if(m_pUserAPI)
	{
		delete m_pUserAPI;
		m_pUserAPI = NULL;
	}

	return true;
}

int ParserMA::OnConnected()
{
	if(m_sink)
	{
		write_log(m_sink, LL_INFO, "[ParserMA] Market data server connected");
		m_sink->handleEvent(WPE_Connect, 0);
	}

	DoLogin();

	return 0;
}

int ParserMA::OnRspOptUserLogin(CFirstSetField *p_pFirstSet, CRspOptUserLoginField *p_pRspUserLoginField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	bool bIsLast = (p_iFieldNum == p_iFieldIndex);
	if(bIsLast && p_pFirstSet && p_pFirstSet->iMsgCode == 0)
	{
		m_uTradingDate = TimeUtils::getCurDate();
		
		if(m_sink)
		{
			m_sink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		DoSubscribeMD();
	}

	return 0;
}

int ParserMA::OnRspStkUserLogin(CFirstSetField *p_pFirstSet, CRspStkUserLoginField *p_pRspUserLoginField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	bool bIsLast = (p_iFieldNum == p_iFieldIndex);
	if (bIsLast && p_pFirstSet && p_pFirstSet->iMsgCode == 0)
	{
		m_uTradingDate = TimeUtils::getCurDate();

		if (m_sink)
		{
			m_sink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		DoSubscribeMD();
	}

	return 0;
}

int ParserMA::OnDisconnected(int p_nReason, const char *p_pszErrInfo)
{
	if(m_sink)
	{
		write_log(m_sink, LL_ERROR, "[ParserMA] Market data server disconnected");
		m_sink->handleEvent(WPE_Close, 0);
	}

	return 0;
}

int ParserMA::OnRtnSubMktData(CRspSubTopicField* p_pRspSubTopicField)
{
	return 0;
}

int ParserMA::OnRtnUnsubMktData(CRspUnsubTopicField* p_pRspUnsubTopicField)
{
	return 0;
}

inline const char* wrapExchg(char chExchg)
{
	switch (chExchg)
	{
	case '0': return "SSE";
	case '1': return "SZSE";
	case 'A': return "SHFE";
	case 'B': return "DCE";
	case 'C': return "CZCE";
	case 'G': return "CFFEX";
	default:
		return "";
	}
}

int ParserMA::OnRtnMktData(ST_MKT_DATA* pDepthMarketData)
{	
	if(m_pBaseDataMgr == NULL)
	{
		return -1;
	}

    WTSContractInfo* contract = m_pBaseDataMgr->getContract(pDepthMarketData->szTrdCode, wrapExchg(pDepthMarketData->chExchange));
    if (contract == NULL)
        return -1;

    uint32_t actDate, actTime, actHour;

	actDate = pDepthMarketData->iTrdDate;
	actTime = pDepthMarketData->iDataTime;
	actHour = actTime / 10000000;

	WTSCommodityInfo* pCommInfo = contract->getCommInfo();

	WTSTickData* tick = WTSTickData::create(pDepthMarketData->szTrdCode);
	tick->setContractInfo(contract);

	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());
	
	quote.action_date = actDate;
	quote.action_time = actTime;
	
	quote.price = pDepthMarketData->uiCurPrice / 10000.0;
	quote.open = pDepthMarketData->uiOpenPrice / 10000.0;
	quote.high = pDepthMarketData->uiHighPrice / 10000.0;
	quote.low = pDepthMarketData->uiLowPrice / 10000.0;
	quote.total_volume = (double)pDepthMarketData->llMatchedVol;
	quote.trading_date = m_uTradingDate;
	quote.total_turnover = pDepthMarketData->llMatchedAmtTot / 10000.0;

	quote.open_interest = (double)pDepthMarketData->llOpenVol;
	quote.settle_price = pDepthMarketData->uiSettPrice / 10000.0;

	quote.upper_limit = pDepthMarketData->uiHighLmtPrice / 10000.0;
	quote.lower_limit = pDepthMarketData->uiLowLmtPrice / 10000.0;

	quote.pre_close = pDepthMarketData->uiPreClosePrice / 10000.0;
	quote.pre_settle = pDepthMarketData->uiPreSettPrice / 10000.0;
	quote.pre_interest = (double)pDepthMarketData->llPreOpenVol;

	//委卖价格
	for(int i = 0; i < 10; i++)
	{
		quote.ask_prices[i] = pDepthMarketData->uiAnsPrice[i] / 10000.0;
		quote.bid_prices[i] = pDepthMarketData->uiBidPrice[i] / 10000.0;

		quote.ask_qty[i] = (double)pDepthMarketData->llAnsVol[i];
		quote.bid_qty[i] = (double)pDepthMarketData->llBidVol[i];
	}

	if(m_sink)
		m_sink->handleQuote(tick, 1);

	tick->release();

	return 0;
}

int ParserMA::OnRspHeartBeat(CFirstSetField *p_pFirstSet, CRspHeartBeatField *p_pRspHeartBeatField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex)
{
	if(m_sink)
		write_log(m_sink, LL_INFO, "[ParserMA] Heartbeating...");

	return 0;
}

void ParserMA::DoLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	int iResult = -1;
	if(m_iMode == 0)
	{//股票
		CReqStkUserLoginField req = { 0 };
		strncpy(req.szAcctType, m_strAcctType.c_str(), sizeof(req.szAcctType));
		strncpy(req.szAcctId, m_strUser.c_str(), sizeof(req.szAcctId));
		strncpy(req.szAuthData, m_strPass.c_str(), sizeof(req.szAuthData));
		strncpy(req.szEncryptKey, m_strEncryptKey.c_str(), sizeof(req.szEncryptKey));
		iResult = m_pUserAPI->ReqStkUserLogin(&req, 1);
	}
	else if (m_iMode == 1)
	{//期权
		CReqOptUserLoginField req = { 0 };
		strncpy(req.szAcctType, m_strAcctType.c_str(), sizeof(req.szAcctType));
		strncpy(req.szAcctId, m_strUser.c_str(), sizeof(req.szAcctId));
		strncpy(req.szAuthData, m_strPass.c_str(), sizeof(req.szAuthData));
		req.chUseScope = m_strUseScope[0];
		req.chEncryptType = m_strEncryptKey[0];
		req.chAuthType = m_strAuthType[0];
		strncpy(req.szEncryptKey, m_strEncryptKey.c_str(), sizeof(req.szEncryptKey));
		iResult = m_pUserAPI->ReqOptUserLogin(&req, 1);
	}
	
	if(iResult != 0)
	{
		if(m_sink)
			write_log(m_sink, LL_ERROR, "[ParserMA] Sending login request failed: {}", iResult);
	}
}

void ParserMA::DoSubscribeMD()
{
	CodeSet codeFilter = m_filterSubs;
	if (codeFilter.empty())
	{//如果订阅礼包只空的,则取出全部合约列表
		return;
	}

	CReqSubTopicField req = { 0 };
	strcpy(req.szTopic, "MARKET1");
	int length = 0;
	for (const LongKey& fullCode : codeFilter)
	{
		const char* code = fullCode.c_str();
		if (StrUtil::startsWith(code, "SSE.", false))
		{
			strcpy(req.szFilter + length, "SH");
			length += 2;
			strcpy(req.szFilter + length, code + 4);
			length += strlen(code + 4);
			strcpy(req.szFilter + length, "|");
			length += 1;
		}
		else
		{
			strcpy(req.szFilter + length, "SZ");
			length += 2;
			strcpy(req.szFilter + length, code + 5);
			length += strlen(code + 5);
			strcpy(req.szFilter + length, "|");
			length += 1;
		}

		if (length >= 500)
		{
			req.szFilter[length - 1] = '\0';
			m_pUserAPI->SubMktData(&req);
			memset(req.szFilter, 0, length);
			length = 0;
		}
	}

	if (length >= 0)
	{
		req.szFilter[length - 1] = '\0';
		m_pUserAPI->SubMktData(&req);
	}

	if (m_sink)
		write_log(m_sink, LL_INFO, "[ParserMA] Market data of {} contracts subscribed totally", codeFilter.size());
	codeFilter.clear();
}

void ParserMA::subscribe(const CodeSet &vecSymbols)
{
	if(m_uTradingDate == 0)
	{
		m_filterSubs = vecSymbols;
	}
	else
	{
		m_filterSubs = vecSymbols;
		DoSubscribeMD();
	}
}

void ParserMA::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserMA::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserMA::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}