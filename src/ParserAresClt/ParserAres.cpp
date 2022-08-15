/*!
 * \file ParserAresClt.cpp
 * \project	WonderTrader
 *
 * \author Suuuunnseeker
 * \date 2022/08/10
 *
 * \brief
 */
#include "ParserAres.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include <boost/filesystem.hpp>
#include <iostream>

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
		ParserAresClt* parser = new ParserAresClt();
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
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
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

ParserAresClt::ParserAresClt()
	: m_pUserAPI(NULL)
	, m_iRequestID(0)
	, m_pfnCreateObj(NULL)
	, m_pfnDestroyObj(NULL)
	, m_oMarketTime(NULL)
{

}


ParserAresClt::~ParserAresClt()
{
	m_pUserAPI = NULL;
	m_pfnCreateObj = NULL;
	m_pfnDestroyObj = NULL;
	m_oMarketTime = NULL;
}

bool ParserAresClt::init(WTSVariant* config)
{
	m_strUserID = config->getCString("user_id");
	m_strFlowDir = config->getCString("flowdir");

	if (m_strFlowDir.empty())
		m_strFlowDir = "AresCltFlow";

	m_strFlowDir = StrUtil::standardisePath(m_strFlowDir);

	WTSVariant* param = config->get("aresmodule");
	if (param != NULL)
		m_strModule = getBinDir() + DLLHelper::wrap_module(param->asCString(), "");
	else
		m_strModule = getBinDir() + DLLHelper::wrap_module("AresCltCpp", "");
	m_hInstAres = DLLHelper::load_library(m_strModule.c_str());

	//std::string module;
	//if (module.empty())
	//{
	//	char szDllFile[255] = { 0 };
	//	GetModuleFileNameA(NULL, szDllFile, 255);

	//	for (int i = strlen(szDllFile) - 1; i >= 0; i--)
	//	{
	//		if (szDllFile[i] == '\\' || szDllFile[i] == '/')
	//		{
	//			szDllFile[i + 1] = '\0';
	//			break;
	//		}
	//	}

	//	strcat(szDllFile, "AresCltCpp");
	//	module = szDllFile;
	//}

	////std::string dllpath = getBinDir() + DLLHelper::wrap_module(module.c_str(), "");
	//std::string dllpath = DLLHelper::wrap_module(module.c_str(), "");

	//std::cout << "module: " << module << std::endl;
	//std::cout << "dllpath: " << dllpath << std::endl;

	////m_hInstAres = DLLHelper::load_library(dllpath.c_str());
	//m_hInstAres = ::LoadLibrary(dllpath.c_str());

	std::string path = StrUtil::printf("%s/%s/", m_strFlowDir.c_str(), m_strUserID.c_str());

	if (!StdFile::exists(path.c_str()))
	{
		boost::filesystem::create_directories(boost::filesystem::path(path));
	}

	if (m_hInstAres)
	{
		m_pfnCreateObj = (pfnCreateObj)::GetProcAddress(m_hInstAres, "CreateAresObj");
		m_pfnDestroyObj = (pfnDestroyObj)::GetProcAddress(m_hInstAres, "DestroyAresObj");

		if (m_pfnCreateObj)
		{
			void* p = m_pfnCreateObj("AresCltApi", 0);
			m_pUserAPI = (IAresCltApi*)p;

			return true;
		}
	}

	return false;
}

void ParserAresClt::release()
{
	disconnect();
}

bool ParserAresClt::connect()
{
	bool flag = false;
	if (m_pUserAPI)
	{
		m_pUserAPI->RegisterSpi(this);
		int iRet = m_pUserAPI->StartWork();  
		if (iRet < 0)
		{
			write_log(m_sink, LL_ERROR, "API StartWork Failed [{}] ", iRet);
		}
		else
		{
			write_log(m_sink, LL_INFO, "API StartWork Running normally [{}]", iRet);
			flag = true;
		}
	}

	return flag;
}

bool ParserAresClt::disconnect()
{
	if (m_pUserAPI)
	{
		printf("EndWork begin \n");
		m_pUserAPI->EndWork();
		FreeApi(m_pUserAPI);
		m_pUserAPI = NULL;
	}

	if (m_hInstAres)
	{
		::FreeLibrary(m_hInstAres);
		m_pfnCreateObj = 0;
		m_pfnDestroyObj = 0;
		m_hInstAres = 0;
	}

	if (m_oMarketTime)
	{
		delete m_oMarketTime;
		m_oMarketTime = NULL;
	}

	return true;
}

void ParserAresClt::OnMarketTime(AClt_Market cMarket, tagAClt_MarketField* pField)
{
	if (pField)
	{
		m_oMarketTime = pField;
	}
}

void ParserAresClt::OnMarketData(AClt_Market cMarket, tagAClt_QuoteField* pQuoteField)
{
	if (pQuoteField == NULL)
		return;

	if (m_pBaseDataMgr == NULL)
		return;

	std::string code, exchg;
	if (cMarket == ACLT_MARKET_SSE)
	{
		exchg = "SSE";
	}
	else if (cMarket == ACLT_MARKET_SZSE)
	{
		exchg = "SZSE";
	}
	else if (cMarket == ACLT_MARKET_CFFEX)
	{
		exchg = "CFFEX";
	}
	else if (cMarket == ACLT_MARKET_CNF)
	{
		exchg = "CNF";
	}
	else
	{
		exchg = "UNKNOWN";
	}
	code = pQuoteField->Code;

	//std::cout << "Code: " << code << "  Exchange: " << exchg << std::endl;
	//std::cout << "Date: " << m_oMarketTime->Date << " Time: " << m_oMarketTime->Time <<  " Open: " << pQuoteField->Open << " High: " << pQuoteField->High << " Low: " << pQuoteField->Low << " Close: " << pQuoteField->Now << std::endl;

	WTSContractInfo* cInfo = m_pBaseDataMgr->getContract(code.c_str(), exchg.c_str());
	if (cInfo == NULL)
		return;

	WTSCommodityInfo* pCommInfo = cInfo->getCommInfo();

	WTSTickData* tick = WTSTickData::create(code.c_str());
	tick->setContractInfo(cInfo);
	WTSTickStruct& quote = tick->getTickStruct();
	strcpy(quote.exchg, pCommInfo->getExchg());

	quote.action_date = m_oMarketTime->Date;
	quote.action_time = m_oMarketTime->Time;

	quote.price = checkValid(pQuoteField->Now);
	quote.open = checkValid(pQuoteField->Open);
	quote.high = checkValid(pQuoteField->High);
	quote.low = checkValid(pQuoteField->Low);
	quote.total_volume = pQuoteField->Volume;

	if (pQuoteField->SettlePrice != DBL_MAX)
		quote.settle_price = checkValid(pQuoteField->SettlePrice);
	if (strcmp(quote.exchg, "CFFEX") == 0 || strcmp(quote.exchg, "CNF") == 0)
	{
		quote.total_turnover = pQuoteField->Amount * pCommInfo->getVolScale();
	}
	else
	{
		if (pQuoteField->Amount != DBL_MAX)
			quote.total_turnover = pQuoteField->Amount;
	}

	quote.open_interest = (uint32_t)pQuoteField->Position;

	// 委买委卖价量
	for (int i = 0; i < 5; i++)
	{
		quote.ask_prices[i] = pQuoteField->Sell[i].Price;
		quote.ask_qty[i] = (double)pQuoteField->Sell[i].Volume;

		quote.bid_prices[i] = pQuoteField->Buy[i].Price;
		quote.bid_qty[i] = (double)pQuoteField->Buy[i].Volume;
	}

	if (m_sink)
		m_sink->handleQuote(tick, 1);

	tick->release();
}


void ParserAresClt::subscribe(const CodeSet &vecSymbols)
{
	//m_pUserAPI->StartWork();
}

void ParserAresClt::unsubscribe(const CodeSet &vecSymbols)
{
	//m_pUserAPI->EndWork();
}

bool ParserAresClt::isConnected()
{
	return m_pUserAPI != NULL;
}

void ParserAresClt::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if (m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}

void ParserAresClt::FreeApi(IAresCltApi* p)
{
	if (m_pfnDestroyObj)
		m_pfnDestroyObj(p);
}