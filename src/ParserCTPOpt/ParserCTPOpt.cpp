/*!
 * \file ParserCTPOpt.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserCTPOpt.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSVersion.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/ModuleHelper.hpp"
#include "../Share/Converter.hpp"

#include <filesystem>
namespace fs = std::filesystem;

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
		ParserCTPOpt* parser = new ParserCTPOpt();
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


static char tmp_timestr[10] = { 0 };
constexpr inline uint32_t strToTime(const char* strTime) noexcept
{
	int idx = 0;
	char* c = (char*)strTime;
	while (*c)
	{
		if ('0' <= *c && *c <= '9')
		{
			tmp_timestr[idx] = *c;
			idx++;
		}

		c = c + 1;
	}
	tmp_timestr[idx] = '\0';

	return convert::to_uint32(tmp_timestr);
}

constexpr inline double checkValid(double val) noexcept
{
	return (val == DBL_MAX || val == FLT_MAX) ? 0 : val;
}

ParserCTPOpt::ParserCTPOpt()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserCTPOpt::~ParserCTPOpt()
{
	m_pUserAPI = NULL;
}

bool ParserCTPOpt::init(WTSVariant* config)
{
	m_strFrontAddr = config->getCString("front");
	m_strBroker = config->getCString("broker");
	m_strUserID = config->getCString("user");
	m_strPassword = config->getCString("pass");
	m_strFlowDir = config->getCString("flowdir");

	if (m_strFlowDir.empty())
		m_strFlowDir = "CTPOptMDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	std::string module = config->getCString("ctpmodule");
	if (module.empty())
		module = "soptthostmduserapi_se";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");
	m_hInstCTP = DLLHelper::load_library(dllpath.c_str());
	std::string path = fmtutil::format("{}/{}/{}/", m_strFlowDir.c_str(), m_strBroker.c_str(), m_strUserID.c_str());
	if (!StdFile::exists(path.c_str()))
	{
		fs::create_directories(fs::path(path));
	}	
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateFtdcMdApi@CThostFtdcMdApi@ctp_sopt@@SAPEAV12@PEBD_N1@Z";
#	else
	const char* creatorName = "?CreateFtdcMdApi@CThostFtdcMdApi@ctp_sopt@@SAPAV12@PBD_N1@Z";
#	endif
#else
	const char* creatorName = "_ZN8ctp_sopt15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb";
#endif
	m_funcCreator = (CTPCreator)DLLHelper::get_symbol(m_hInstCTP, creatorName);
	m_pUserAPI = m_funcCreator(path.c_str(), false, false);
	m_pUserAPI->RegisterSpi(this);
	m_pUserAPI->RegisterFront((char*)m_strFrontAddr.c_str());

	return true;
}

void ParserCTPOpt::release()
{
	disconnect();
}

bool ParserCTPOpt::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool ParserCTPOpt::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserCTPOpt::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	IsErrorRspInfo(pRspInfo);
}

void ParserCTPOpt::OnFrontConnected()
{
	if(m_sink)
	{
		write_log(m_sink, LL_INFO, "[ParserCTPOpt] Market data server connected");
		m_sink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void ParserCTPOpt::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_uTradingDate = convert::to_uint32(m_pUserAPI->GetTradingDay());
		
		if(m_sink)
		{
			m_sink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		SubscribeMarketData();
	}
}

void ParserCTPOpt::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_sink)
	{
		m_sink->handleEvent(WPE_Logout, 0);
	}
}

void ParserCTPOpt::OnFrontDisconnected( int nReason )
{
	if(m_sink)
	{
		write_log(m_sink, LL_ERROR, "[ParserCTPOpt] Market data server disconnected: {}", nReason);
		m_sink->handleEvent(WPE_Close, 0);
	}
}

void ParserCTPOpt::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void ParserCTPOpt::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = convert::to_uint32(pDepthMarketData->ActionDay);
	uint32_t actTime = strToTime(pDepthMarketData->UpdateTime) * 1000 + pDepthMarketData->UpdateMillisec;
	/*
	 *	By Wesley @ 2024.08.29
	 *	ETF期权行情比较特别，时间戳是最后一笔成交时间
	 *	因此不活跃的合约的行情时间戳可能是较早时候的时间，甚至是0（如果一直没成交）
	 *	所以这里对行情时间戳小于已经收到的最大的时间戳的数据做一个处理
	 *	1、如果第一笔行情时间戳就是0，则直接读取本地时间
	 *	2、其他情况，则根据记录的最后一笔行情时间，以及当时的本地时间，计算一个最新的时间
	 */
	if (actTime == 0 || actTime < m_uLatestTime)
	{
		if (m_uLatestTime == 0)
			TimeUtils::getDateTime(actDate, actTime);
		else
		{
			uint32_t msecs = (uint32_t)(TimeUtils::getLocalTimeNow() - m_iLastTimestamp);
			actTime = m_uLatestTime + msecs;
		}
	}
	else
	{
		m_iLastTimestamp = TimeUtils::getLocalTimeNow();
		m_uLatestTime = actTime;
	}

	WTSContractInfo* cInfo = m_pBaseDataMgr->getContract(pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID);
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* pCommInfo = cInfo->getCommInfo();

	WTSTickData* tick = WTSTickData::create(cInfo->getCode());
	tick->setContractInfo(cInfo);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());
	
	quote.action_date = actDate != 0 ? actDate : m_uTradingDate;
	quote.action_time = actTime;
	
	quote.price = checkValid(pDepthMarketData->LastPrice);
	quote.open = checkValid(pDepthMarketData->OpenPrice);
	quote.high = checkValid(pDepthMarketData->HighestPrice);
	quote.low = checkValid(pDepthMarketData->LowestPrice);
	quote.total_volume = (uint32_t)pDepthMarketData->Volume;
	quote.trading_date = m_uTradingDate;
	if(pDepthMarketData->SettlementPrice != DBL_MAX)
		quote.settle_price = checkValid(pDepthMarketData->SettlementPrice);
	if(strcmp(quote.exchg, "CZCE") == 0)
	{
		quote.total_turnover = pDepthMarketData->Turnover*pCommInfo->getVolScale();
	}
	else
	{
		if(pDepthMarketData->Turnover != DBL_MAX)
			quote.total_turnover = pDepthMarketData->Turnover;
	}

	quote.open_interest = (uint32_t)pDepthMarketData->OpenInterest;

	quote.upper_limit = checkValid(pDepthMarketData->UpperLimitPrice);
	quote.lower_limit = checkValid(pDepthMarketData->LowerLimitPrice);

	quote.pre_close = checkValid(pDepthMarketData->PreClosePrice);
	quote.pre_settle = checkValid(pDepthMarketData->PreSettlementPrice);
	quote.pre_interest = (uint32_t)pDepthMarketData->PreOpenInterest;

	//委卖价格
	quote.ask_prices[0] = checkValid(pDepthMarketData->AskPrice1);
	quote.ask_prices[1] = checkValid(pDepthMarketData->AskPrice2);
	quote.ask_prices[2] = checkValid(pDepthMarketData->AskPrice3);
	quote.ask_prices[3] = checkValid(pDepthMarketData->AskPrice4);
	quote.ask_prices[4] = checkValid(pDepthMarketData->AskPrice5);

	//委买价格
	quote.bid_prices[0] = checkValid(pDepthMarketData->BidPrice1);
	quote.bid_prices[1] = checkValid(pDepthMarketData->BidPrice2);
	quote.bid_prices[2] = checkValid(pDepthMarketData->BidPrice3);
	quote.bid_prices[3] = checkValid(pDepthMarketData->BidPrice4);
	quote.bid_prices[4] = checkValid(pDepthMarketData->BidPrice5);

	//委卖量
	quote.ask_qty[0] = pDepthMarketData->AskVolume1;
	quote.ask_qty[1] = pDepthMarketData->AskVolume2;
	quote.ask_qty[2] = pDepthMarketData->AskVolume3;
	quote.ask_qty[3] = pDepthMarketData->AskVolume4;
	quote.ask_qty[4] = pDepthMarketData->AskVolume5;

	//委买量
	quote.bid_qty[0] = pDepthMarketData->BidVolume1;
	quote.bid_qty[1] = pDepthMarketData->BidVolume2;
	quote.bid_qty[2] = pDepthMarketData->BidVolume3;
	quote.bid_qty[3] = pDepthMarketData->BidVolume4;
	quote.bid_qty[4] = pDepthMarketData->BidVolume5;

	if(m_sink)
		m_sink->handleQuote(tick, 1);

	tick->release();
}

void ParserCTPOpt::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(!IsErrorRspInfo(pRspInfo))
	{

	}
	else
	{

	}
}

void ParserCTPOpt::OnHeartBeatWarning( int nTimeLapse )
{
	if (m_sink)
		write_log(m_sink, LL_INFO, "[ParserCTPOpt] Heartbeating, elapse: {}", nTimeLapse);
}

void ParserCTPOpt::ReqUserLogin()
{
	if(m_pUserAPI == NULL)
	{
		return;
	}

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_strBroker.c_str());
	strcpy(req.UserID, m_strUserID.c_str());
	strcpy(req.Password, m_strPassword.c_str());
	strcpy(req.UserProductInfo, WT_PRODUCT);
	int iResult = m_pUserAPI->ReqUserLogin(&req, ++m_iRequestID);
	if(iResult != 0)
	{
		if(m_sink)
			write_log(m_sink, LL_ERROR, "[ParserCTPOpt] Sending login request failed: {}", iResult);
	}
}

void ParserCTPOpt::SubscribeMarketData()
{
	CodeSet codeFilter = m_filterSubs;
	if(codeFilter.empty())
	{//如果订阅礼包只空的,则取出全部合约列表
		return;
	}

	char ** subscribe = new char*[codeFilter.size()];
	int nCount = 0;
	for(auto& code : codeFilter)
	{
		std::size_t pos = code.find('.');
		if (pos != std::string::npos)
			subscribe[nCount++] = (char*)code.c_str() + pos + 1;
		else
			subscribe[nCount++] = (char*)code.c_str();
	}

	if(m_pUserAPI && nCount > 0)
	{
		int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount);
		if (iResult != 0)
		{
			if (m_sink)
				write_log(m_sink, LL_ERROR, "[ParserCTPOpt] Sending md subscribe request failed: {}", iResult);
		}
		else
		{
			if (m_sink)
				write_log(m_sink, LL_INFO, "[ParserCTPOpt] Market data of {} contracts subscribed in total", nCount);
		}
	}
	codeFilter.clear();
	delete[] subscribe;
}

bool ParserCTPOpt::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	return false;
}

void ParserCTPOpt::subscribe(const CodeSet &vecSymbols)
{
	if(m_uTradingDate == 0)
	{
		m_filterSubs = vecSymbols;
	}
	else
	{
		m_filterSubs = vecSymbols;
		char * subscribe[500] = {NULL};
		int nCount = 0;
		for (auto& code  : vecSymbols)
		{
			std::size_t pos = code.find('.');
			if (pos != std::string::npos)
				subscribe[nCount++] = (char*)code.c_str() + pos + 1;
			else
				subscribe[nCount++] = (char*)code.c_str();
		}

		if(m_pUserAPI && nCount > 0)
		{
			int iResult = m_pUserAPI->SubscribeMarketData(subscribe, nCount);
			if (iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserCTPOpt] Sending md subscribe request failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserCTPOpt] Market data of {} contracts subscribed in total", nCount);
			}
		}
	}
}

void ParserCTPOpt::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserCTPOpt::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserCTPOpt::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}