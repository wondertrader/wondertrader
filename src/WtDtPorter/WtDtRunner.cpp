/*!
 * \file WtDtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtRunner.h"
#include "../WtDtCore/WtHelper.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../WTSTools/WTSLogger.h"


WtDtRunner::WtDtRunner()
{
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::start()
{
	m_asyncIO.post([this](){
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		m_stateMon.run();
	});

	boost::asio::io_service::work work(m_asyncIO);
	m_asyncIO.run();
}

void WtDtRunner::initialize(const char* cfgFile, const char* logCfg, const char* modDir /* = "" */)
{
	WTSLogger::init(logCfg);
	WtHelper::set_module_dir(modDir);

	std::string json;
	StdFile::read_file_content(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	WTSVariant* config = WTSVariant::createObject();
	jsonToVariant(document, config);

	//基础数据文件
	WTSVariant* cfgBF = config->get("basefiles");
	if (cfgBF->get("session"))
	{
		m_baseDataMgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("Trading sessions loaded");
	}

	if (cfgBF->get("commodity"))
	{
		m_baseDataMgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("Commodities loaded");
	}

	if (cfgBF->get("contract"))
	{
		m_baseDataMgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("Contracts loades");
	}

	if (cfgBF->get("holiday"))
	{
		m_baseDataMgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}

	if (cfgBF->get("hot"))
	{
		m_hotMgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("Hot rules loaded");
	}

	m_udpCaster.init(config->get("broadcaster"), &m_baseDataMgr, &m_dataMgr);

	initDataMgr(config->get("writer"));

	m_stateMon.initialize("statemonitor.json", &m_baseDataMgr, &m_dataMgr);

	initParsers(config->get("parsers"));

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	m_dataMgr.init(config, &m_baseDataMgr, &m_stateMon, &m_udpCaster);
}

void WtDtRunner::initParsers(WTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		std::string module = cfgItem->getCString("module");
		if (!StdFile::exists(module.c_str()))
		{
			module = WtHelper::get_module_dir();
			module += "parsers/";
			module += cfgItem->getCString("module");
		}
		DllHandle libParser = DLLHelper::load_library(module.c_str());
		if (libParser)
		{
			FuncCreateParser pFuncCreateParser = (FuncCreateParser)DLLHelper::get_symbol(libParser, "createParser");
			if (pFuncCreateParser == NULL)
			{
				WTSLogger::error("Initializing of market data parser failed: function createParser not found...");
			}

			FuncDeleteParser pFuncDeleteParser = (FuncDeleteParser)DLLHelper::get_symbol(libParser, "deleteParser");
			if (pFuncDeleteParser == NULL)
			{
				WTSLogger::error("Initializing of market data parser failed: function deleteParser not found...");
			}

			if (pFuncCreateParser && pFuncDeleteParser)
			{
				WTSParams* params = cfgItem->toParams();

				ParserAdapterPtr adapter(new ParserAdapter(&m_baseDataMgr, &m_dataMgr));
				adapter->initAdapter(params, pFuncCreateParser, pFuncDeleteParser);
				ParserAdapterMgr::addAdapter(adapter);
				params->release();
			}

		}
		else
		{
			WTSLogger::error("Initializing of market data parser failed: loading module %s failed...", module.c_str());
		}
	}

	WTSLogger::info("%u market data parsers loaded in total", ParserAdapterMgr::size());
}
