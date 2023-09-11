/*!
 * \file ParserCTPMini.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserCTPMini.h"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSVersion.h"

#include <boost/filesystem.hpp>

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
		ParserCTPMini* parser = new ParserCTPMini();
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


uint32_t strToTime(const char* strTime)
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

ParserCTPMini::ParserCTPMini()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
{
}


ParserCTPMini::~ParserCTPMini()
{
	m_pUserAPI = NULL;
}

bool ParserCTPMini::init(WTSVariant* config)
{
	m_strFrontAddr = config->getCString("front");
	m_strBroker = config->getCString("broker");
	m_strUserID = config->getCString("user");
	m_strPassword = config->getCString("pass");
	m_strFlowDir = config->getCString("flowdir");

	if (m_strFlowDir.empty())
		m_strFlowDir = "CTPMiniMDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	std::string module = config->getCString("ctpmodule");
	if (module.empty())
		module = "thostmduserapi";
	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "lib");;
	m_hInstCTP = DLLHelper::load_library(dllpath.c_str());
	std::string path = StrUtil::printf("%s/%s/%s/", m_strFlowDir.c_str(), m_strBroker.c_str(), m_strUserID.c_str());
	if (!StdFile::exists(path.c_str()))
	{
		boost::filesystem::create_directories(boost::filesystem::path(path));
	}
#ifdef _WIN32
#	ifdef _WIN64
	const char* creatorName = "?CreateFtdcMdApi@CThostFtdcMdApi@@SAPEAV1@PEBD_N1@Z";
#	else
	const char* creatorName = "?CreateFtdcMdApi@CThostFtdcMdApi@@SAPAV1@PBD_N1@Z";
#	endif
#else
	const char* creatorName = "_ZN15CThostFtdcMdApi15CreateFtdcMdApiEPKcbb";
#endif
	m_funcCreator = (CTPCreator)DLLHelper::get_symbol(m_hInstCTP, creatorName);
	m_pUserAPI = m_funcCreator(path.c_str(), false, false);
	m_pUserAPI->RegisterSpi(this);
	m_pUserAPI->RegisterFront((char*)m_strFrontAddr.c_str());

	return true;
}

void ParserCTPMini::release()
{
	disconnect();
}

bool ParserCTPMini::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool ParserCTPMini::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserCTPMini::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	IsErrorRspInfo(pRspInfo);
}

void ParserCTPMini::OnFrontConnected()
{
	if(m_sink)
	{
		write_log(m_sink, LL_INFO, "[ParserCTPMini] Market data server connected");
		m_sink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void ParserCTPMini::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
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

void ParserCTPMini::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_sink)
	{
		m_sink->handleEvent(WPE_Logout, 0);
	}
}

void ParserCTPMini::OnFrontDisconnected( int nReason )
{
	if(m_sink)
	{
		write_log(m_sink, LL_ERROR, "[ParserCTPMini] Market data server disconnected: {}", nReason);
		m_sink->handleEvent(WPE_Close, 0);
	}
}

void ParserCTPMini::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void ParserCTPMini::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

	uint32_t actDate = strtoul(pDepthMarketData->ActionDay, NULL, 10);
	uint32_t actTime = strToTime(pDepthMarketData->UpdateTime) * 1000 + pDepthMarketData->UpdateMillisec;
	uint32_t actHour = actTime / 10000000;
	
	if (actDate == 0)
		actDate = m_uTradingDate;

	if (actDate == m_uTradingDate && actHour >= 20)
	{
		//这样的时间是有问题,因为夜盘时发生日期不可能等于交易日
		//这就需要手动设置一下
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		uint32_t curHour = curTime / 10000000;

		//早上启动以后,会收到昨晚12点以前收盘的行情,这个时候可能会有发生日期=交易日的情况出现
		//这笔数据直接丢掉
		if (curHour >= 3 && curHour < 9)
			return;

		actDate = curDate;

		if (actHour == 23 && curHour == 0)
		{
			//行情时间慢于系统时间
			actDate = TimeUtils::getNextDate(curDate, -1);
		}
		else if (actHour == 0 && curHour == 23)
		{
			//系统时间慢于行情时间
			actDate = TimeUtils::getNextDate(curDate, 1);
		}
	}

	WTSContractInfo* contract = m_pBaseDataMgr->getContract(pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID);
	if (contract == NULL)
		return;

	WTSCommodityInfo* pCommInfo = contract->getCommInfo();

	//if (strcmp(contract->getExchg(), "CZCE") == 0)
	//{
	//	actTime += (uint32_t)(TimeUtils::getLocalTimeNow() % 1000);
	//}

	WTSTickData* tick = WTSTickData::create(pDepthMarketData->InstrumentID);
	tick->setContractInfo(contract);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());
	
	quote.action_date = actDate;
	quote.action_time = actTime;
	
	quote.price = checkValid(pDepthMarketData->LastPrice);
	quote.open = checkValid(pDepthMarketData->OpenPrice);
	quote.high = checkValid(pDepthMarketData->HighestPrice);
	quote.low = checkValid(pDepthMarketData->LowestPrice);
	quote.total_volume = pDepthMarketData->Volume;
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

void ParserCTPMini::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(!IsErrorRspInfo(pRspInfo))
	{

	}
	else
	{

	}
}

void ParserCTPMini::OnHeartBeatWarning( int nTimeLapse )
{
	if(m_sink)
		write_log(m_sink, LL_INFO, "[ParserCTPMini] Heartbeating, elapse: {}", nTimeLapse);
}

void ParserCTPMini::ReqUserLogin()
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
			write_log(m_sink, LL_ERROR, "[ParserCTPMini] Sending login request failed: {}", iResult);
	}
}

void ParserCTPMini::SubscribeMarketData()
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
		if(iResult != 0)
		{
			if(m_sink)
				write_log(m_sink, LL_ERROR, "[ParserCTPMini] Sending md subscribe request failed: {}", iResult);
		}
		else
		{
			if(m_sink)
				write_log(m_sink, LL_INFO, "[ParserCTPMini] Market data of {} contracts subscribed in total", nCount);
		}
	}
	codeFilter.clear();
	delete[] subscribe;
}

bool ParserCTPMini::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	return false;
}

void ParserCTPMini::subscribe(const CodeSet &vecSymbols)
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
			if(iResult != 0)
			{
				if (m_sink)
					write_log(m_sink, LL_ERROR, "[ParserCTPMini] Sending md subscribe request failed: {}", iResult);
			}
			else
			{
				if (m_sink)
					write_log(m_sink, LL_INFO, "[ParserCTPMini] Market data of {} contracts subscribed in total", nCount);
			}
		}
	}
}

void ParserCTPMini::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserCTPMini::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserCTPMini::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}