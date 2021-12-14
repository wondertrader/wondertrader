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
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/SignalHook.hpp"


WtDtRunner::WtDtRunner()
	: _dumper_for_bars(NULL)
	, _dumper_for_ticks(NULL)
	, _dumper_for_ordque(NULL)
	, _dumper_for_orddtl(NULL)
	, _dumper_for_trans(NULL)
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::start(bool bAsync/* = false*/)
{
	_parsers.run();

    if(!bAsync)
    {
		_async_io.post([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			_state_mon.run();
		});

        boost::asio::io_service::work work(_async_io);
        _async_io.run();
    }
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		_state_mon.run();
	}
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
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("Trading sessions loaded");
	}

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString());
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString());
			}
		}
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::info("Second rules loaded");
	}

	_udp_caster.init(config->get("broadcaster"), &_bd_mgr, &_data_mgr);

	initDataMgr(config->get("writer"));

	_state_mon.initialize(config->getCString("statemonitor"), &_bd_mgr, &_data_mgr);

	initParsers(config->getCString("parsers"));

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	bool bDumperEnabled = (_dumper_for_bars != NULL || _dumper_for_ticks != NULL);
	_data_mgr.init(config, &_bd_mgr, &_state_mon, &_udp_caster);
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

		// By Wesley @ 2021.12.14
		// 如果id为空，则生成自动id
		std::string realid = id;
		if (realid.empty())
		{
			static uint32_t auto_parserid = 1000;
			realid = StrUtil::printf("auto_parser_%u", auto_parserid++);
		}
		
		ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr));
		adapter->init(realid.c_str(), cfgItem);
		_parsers.addAdapter(realid.c_str(), adapter);
	}

	WTSLogger::info("%u market data parsers loaded in total", _parsers.size());
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
	ParserAdapterPtr adapter = _parsers.getAdapter(id);
	if (adapter)
	{
		WTSTickData* newTick = WTSTickData::create(*curTick);
		adapter->handleQuote(newTick, bNeedSlice);
		newTick->release();
	}
}

bool WtDtRunner::createExtParser(const char* id)
{
	ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr));
	ExpParser* parser = new ExpParser(id);
	adapter->initExt(id, parser);
	_parsers.addAdapter(id, adapter);
	WTSLogger::info("Extended parser %s created", id);
	return true;
}

#pragma endregion 

bool WtDtRunner::createExtDumper(const char* id)
{
	ExpDumperPtr dumper(new ExpDumper(id));
	_dumpers[id] = dumper;

	_data_mgr.add_ext_dumper(id, dumper.get());

	WTSLogger::info("Extended dumper %s created", id);
	return true;
}

void WtDtRunner::registerExtDumper(FuncDumpBars barDumper, FuncDumpTicks tickDumper)
{
	_dumper_for_bars = barDumper;
	_dumper_for_ticks = tickDumper;
}

void WtDtRunner::registerExtHftDataDumper(FuncDumpOrdQue ordQueDumper, FuncDumpOrdDtl ordDtlDumper, FuncDumpTrans transDumper)
{
	_dumper_for_ordque = ordQueDumper;
	_dumper_for_orddtl = ordDtlDumper;
	_dumper_for_trans = transDumper;
}

bool WtDtRunner::dumpHisTicks(const char* id, const char* stdCode, uint32_t uDate, WTSTickStruct* ticks, uint32_t count)
{
	if (NULL == _dumper_for_ticks)
	{
		WTSLogger::error("Extended tick dumper not enabled");
		return false;
	}

	return _dumper_for_ticks(id, stdCode, uDate, ticks, count);
}

bool WtDtRunner::dumpHisBars(const char* id, const char* stdCode, const char* period, WTSBarStruct* bars, uint32_t count)
{
	if (NULL == _dumper_for_bars)
	{
		WTSLogger::error("Extended bar dumper not enabled");
		return false;
	}

	return _dumper_for_bars(id, stdCode, period, bars, count);
}

bool WtDtRunner::dumpHisOrdDtl(const char* id, const char* stdCode, uint32_t uDate, WTSOrdDtlStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_orddtl)
	{
		WTSLogger::error("Extended order detail dumper not enabled");
		return false;
	}

	return _dumper_for_orddtl(id, stdCode, uDate, items, count);
}

bool WtDtRunner::dumpHisOrdQue(const char* id, const char* stdCode, uint32_t uDate, WTSOrdQueStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_ordque)
	{
		WTSLogger::error("Extended order queue dumper not enabled");
		return false;
	}

	return _dumper_for_ordque(id, stdCode, uDate, items, count);
}

bool WtDtRunner::dumpHisTrans(const char* id, const char* stdCode, uint32_t uDate, WTSTransStruct* items, uint32_t count)
{
	if (NULL == _dumper_for_trans)
	{
		WTSLogger::error("Extended transaction dumper not enabled");
		return false;
	}

	return _dumper_for_trans(id, stdCode, uDate, items, count);
}