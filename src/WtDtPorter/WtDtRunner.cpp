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
#include "ExpParser.h"

#include "../WtDtCore/WtHelper.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/JsonToVariant.hpp"
#include "../Share/DLLHelper.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/SignalHook.hpp"


WtDtRunner::WtDtRunner()
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::start()
{
	m_parsers.run();

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

	if (cfgBF->get("second"))
	{
		m_hotMgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::info("Second rules loaded");
	}

	m_udpCaster.init(config->get("broadcaster"), &m_baseDataMgr, &m_dataMgr);

	initDataMgr(config->get("writer"));

	m_stateMon.initialize(config->getCString("statemonitor"), &m_baseDataMgr, &m_dataMgr);

	initParsers(config->getCString("parsers"));

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	m_dataMgr.init(config, &m_baseDataMgr, &m_stateMon, &m_udpCaster);
}

void WtDtRunner::initParsers(const char* filename)
{
	std::string json;
	StdFile::read_file_content(filename, json);
	rj::Document document;
	document.Parse(json.c_str());

	WTSVariant* config = WTSVariant::createObject();
	jsonToVariant(document, config);
	WTSVariant* cfg = config->get("parsers");

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter(&m_baseDataMgr, &m_dataMgr));
		adapter->init(id, cfgItem);
		m_parsers.addAdapter(id, adapter);
	}

	WTSLogger::info("%u market data parsers loaded in total", m_parsers.size());
	config->release();
}

#pragma region "Extended Parser"
void WtDtRunner::registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub)
{
	_cb_parser_evt = cbEvt;
	_cb_parser_sub = cbSub;

	WTSLogger::info("Callbacks of Extented Parser registration done");
}

void WtDtRunner::parser_init(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_INIT, id);
}

void WtDtRunner::parser_connect(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_CONNECT, id);
}

void WtDtRunner::parser_disconnect(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_DISCONNECT, id);
}

void WtDtRunner::parser_release(const char* id)
{
	if (_cb_parser_evt)
		_cb_parser_evt(EVENT_PARSER_RELEASE, id);
}

void WtDtRunner::parser_subscribe(const char* id, const char* code)
{
	if (_cb_parser_sub)
		_cb_parser_sub(id, code, true);
}

void WtDtRunner::parser_unsubscribe(const char* id, const char* code)
{
	if (_cb_parser_sub)
		_cb_parser_sub(id, code, false);
}

void WtDtRunner::on_parser_quote(const char* id, WTSTickStruct* curTick, bool bNeedSlice /* = true */)
{
	ParserAdapterPtr adapter = m_parsers.getAdapter(id);
	if (adapter)
	{
		WTSTickData* newTick = WTSTickData::create(*curTick);
		adapter->handleQuote(newTick, bNeedSlice);
		newTick->release();
	}
}

bool WtDtRunner::createExtParser(const char* id)
{
	ParserAdapterPtr adapter(new ParserAdapter(&m_baseDataMgr, &m_dataMgr));
	ExpParser* parser = new ExpParser(id);
	adapter->initExt(id, parser);
	m_parsers.addAdapter(id, adapter);
	WTSLogger::info("Extended parser %s created", id);
	return true;
}

#pragma endregion 