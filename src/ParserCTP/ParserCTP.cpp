/*!
 * \file ParserCTP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserCTP.h"

#include "../Includes/WTSVersion.h"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/ModuleHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"

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
		ParserCTP* parser = new ParserCTP();
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

ParserCTP::ParserCTP()
	:m_pUserAPI(NULL)
	,m_iRequestID(0)
	,m_uTradingDate(0)
    ,m_bLocaltime(false)
{
}


ParserCTP::~ParserCTP()
{
	m_pUserAPI = NULL;
}

bool ParserCTP::init(WTSVariant* config)
{
	m_strFrontAddr = config->getCString("front");
	m_strBroker = config->getCString("broker");
	m_strUserID = config->getCString("user");
	m_strPassword = config->getCString("pass");
	m_strFlowDir = config->getCString("flowdir");
    /*
     * By Wesley @ 2022.03.09
     * 这个参数主要是给非标准CTP环境用的
     * 如simnow全天候行情，openctp等环境
     * 如果为true，就用本地时间戳，默认为false
     */
    m_bLocaltime = config->getBoolean("localtime");

	if (m_strFlowDir.empty())
		m_strFlowDir = "CTPMDFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	std::string module = config->getCString("ctpmodule");
	if (module.empty())
		module = "thostmduserapi_se";

	std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");
	m_hInstCTP = DLLHelper::load_library(dllpath.c_str());
	std::string path = StrUtil::printf("%s%s/%s/", m_strFlowDir.c_str(), m_strBroker.c_str(), m_strUserID.c_str());
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

void ParserCTP::release()
{
	disconnect();
}

bool ParserCTP::connect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->Init();
	}

	return true;
}

bool ParserCTP::disconnect()
{
	if(m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(NULL);
		m_pUserAPI->Release();
		m_pUserAPI = NULL;
	}

	return true;
}

void ParserCTP::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	IsErrorRspInfo(pRspInfo);
}

void ParserCTP::OnFrontConnected()
{
	if(m_sink)
	{
		write_log(m_sink, LL_INFO, "[ParserCTP] Market data server connected");
		m_sink->handleEvent(WPE_Connect, 0);
	}

	ReqUserLogin();
}

void ParserCTP::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		m_uTradingDate = strtoul(m_pUserAPI->GetTradingDay(), NULL, 10);
        //By Wesley @ 2022.03.09
        //这里加一个判断，但是这样的交易日不准确，在夜盘会出错
        if(m_uTradingDate == 0)
            m_uTradingDate = TimeUtils::getCurDate();
		
		write_log(m_sink, LL_INFO, "[ParserCTP] Market data server logined, {}", m_uTradingDate);

		if(m_sink)
		{
			m_sink->handleEvent(WPE_Login, 0);
		}

		//订阅行情数据
		DoSubscribeMD();
	}
}

void ParserCTP::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_sink)
	{
		m_sink->handleEvent(WPE_Logout, 0);
	}
}

void ParserCTP::OnFrontDisconnected( int nReason )
{
	if(m_sink)
	{
		write_log(m_sink, LL_ERROR, "[ParserCTP] Market data server disconnected: {}", nReason);
		m_sink->handleEvent(WPE_Close, 0);
	}
}

void ParserCTP::OnRspUnSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void ParserCTP::OnRtnDepthMarketData( CThostFtdcDepthMarketDataField *pDepthMarketData )
{	
	if(m_pBaseDataMgr == NULL)
	{
		return;
	}

    WTSContractInfo* contract = m_pBaseDataMgr->getContract(pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID);
    if (contract == NULL)
        return;

    uint32_t actDate, actTime, actHour;

    if(m_bLocaltime)
    {
        TimeUtils::getDateTime(actDate, actTime);
        actHour = actTime / 10000000;
    }
    else
    {
        actDate = strtoul(pDepthMarketData->ActionDay, NULL, 10);
        actTime = strToTime(pDepthMarketData->UpdateTime) * 1000 + pDepthMarketData->UpdateMillisec;
        actHour = actTime / 10000000;

        if (actDate == m_uTradingDate && actHour >= 20) {
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

            if (actHour == 23 && curHour == 0) {
                //行情时间慢于系统时间
                actDate = TimeUtils::getNextDate(curDate, -1);
            } else if (actHour == 0 && curHour == 23) {
                //系统时间慢于行情时间
                actDate = TimeUtils::getNextDate(curDate, 1);
            }
        }
    }

	WTSCommodityInfo* pCommInfo = contract->getCommInfo();

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

	quote.open_interest = pDepthMarketData->OpenInterest;

	quote.upper_limit = checkValid(pDepthMarketData->UpperLimitPrice);
	quote.lower_limit = checkValid(pDepthMarketData->LowerLimitPrice);

	quote.pre_close = checkValid(pDepthMarketData->PreClosePrice);
	quote.pre_settle = checkValid(pDepthMarketData->PreSettlementPrice);
	quote.pre_interest = pDepthMarketData->PreOpenInterest;

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

void ParserCTP::OnRspSubMarketData( CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	if(!IsErrorRspInfo(pRspInfo))
	{

	}
	else
	{

	}
}

void ParserCTP::OnHeartBeatWarning( int nTimeLapse )
{
	if(m_sink)
		write_log(m_sink, LL_INFO, "[ParserCTP] Heartbeating, elapse: {}", nTimeLapse);
}

void ParserCTP::ReqUserLogin()
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
			write_log(m_sink, LL_ERROR, "[ParserCTP] Sending login request failed: {}", iResult);
	}
}

void ParserCTP::DoSubscribeMD()
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
				write_log(m_sink, LL_ERROR, "[ParserCTP] Sending md subscribe request failed: {}", iResult);
		}
		else
		{
			if(m_sink)
				write_log(m_sink, LL_INFO, "[ParserCTP] Market data of {} contracts subscribed totally", nCount);
		}
	}
	codeFilter.clear();
	delete[] subscribe;
}

bool ParserCTP::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	return false;
}

void ParserCTP::subscribe(const CodeSet &vecSymbols)
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

void ParserCTP::unsubscribe(const CodeSet &vecSymbols)
{
}

bool ParserCTP::isConnected()
{
	return m_pUserAPI!=NULL;
}

void ParserCTP::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if(m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}