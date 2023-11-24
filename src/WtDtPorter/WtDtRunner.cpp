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
#include "../Includes/WTSContractInfo.hpp"

#include "../Share/StrUtil.hpp"

#include "../WTSUtils/WTSCfgLoader.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/SignalHook.hpp"


WtDtRunner::WtDtRunner()
	: _dumper_for_bars(NULL)
	, _dumper_for_ticks(NULL)
	, _dumper_for_ordque(NULL)
	, _dumper_for_orddtl(NULL)
	, _dumper_for_trans(NULL)
	, _to_exit(false)
{
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::start(bool bAsync /* = false */, bool bAlldayMode /* = false */)
{
	_parsers.run();

    if(!bAsync)
    {
		install_signal_hooks([this](const char* message) {
			if(!_to_exit)
				WTSLogger::error(message);
		}, [this](bool toExit) {
			if (_to_exit)
				return;
			_to_exit = toExit;
			WTSLogger::info("Exit flag is {}", _to_exit);
		});

		_async_io.post([this, bAlldayMode]() {
			if(!bAlldayMode)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				_state_mon.run();
			}
		});

		StdThread trd([this] {
			while (!_to_exit)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				_async_io.run_one();
			}
		});

		trd.join();
    }
	else
	{
		if (!bAlldayMode)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			_state_mon.run();
		}
	}
}

void WtDtRunner::initialize(const char* cfgFile, const char* logCfg, const char* modDir /* = "" */, bool bCfgFile /* = true */, bool bLogCfgFile /* = true */)
{
	WTSLogger::init(logCfg, bLogCfgFile);
	WtHelper::set_module_dir(modDir);

	WTSVariant* config = NULL;
	if (bCfgFile)
		config = WTSCfgLoader::load_from_file(cfgFile);
	else
		config = WTSCfgLoader::load_from_content(cfgFile, false);

	if(config == NULL)
	{
		WTSLogger::error("Loading config file {} failed", cfgFile);
		return;
	}

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
		WTSLogger::log_raw(LL_INFO, "Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::log_raw(LL_INFO, "Second rules loaded");
	}

	if (cfgBF->has("rules"))
	{
		auto cfgRules = cfgBF->get("rules");
		auto tags = cfgRules->memberNames();
		for (const std::string& ruleTag : tags)
		{
			_hot_mgr.loadCustomRules(ruleTag.c_str(), cfgRules->getCString(ruleTag.c_str()));
			WTSLogger::info("{} rules loaded from {}", ruleTag, cfgRules->getCString(ruleTag.c_str()));
		}
	}

	if (config->has("shmcaster"))
	{
		_shm_caster.init(config->get("shmcaster"));
		_data_mgr.add_caster(&_shm_caster);
	}

	if(config->has("broadcaster"))
	{
		_udp_caster.init(config->get("broadcaster"), &_bd_mgr, &_data_mgr);
		_data_mgr.add_caster(&_udp_caster);
	}

	//By Wesley @ 2021.12.27
	//全天候模式，不需要再使用状态机
	bool bAlldayMode = config->getBoolean("allday");
	if (!bAlldayMode)
	{
		_state_mon.initialize(config->getCString("statemonitor"), &_bd_mgr, &_data_mgr);
	}
	else
	{
		WTSLogger::info("datakit will run in allday mode");
	}

	initDataMgr(config->get("writer"), bAlldayMode);

	if (config->has("index"))
	{
		//如果存在指数模块要，配置指数
		const char* filename = config->getCString("index");
		WTSLogger::info("Reading index config from {}...", filename);
		WTSVariant* var = WTSCfgLoader::load_from_file(filename);
		if (var)
		{
			_idx_factory.init(var, &_hot_mgr, &_bd_mgr, &_data_mgr);
			var->release();
		}
		else
		{
			WTSLogger::error("Loading index config {} failed", filename);
		}
	}

	WTSVariant* cfgParser = config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info("Reading parser config from {}...", filename);
				WTSVariant* var = WTSCfgLoader::load_from_file(filename);
				if (var)
				{
					initParsers(var->get("parsers"));
					var->release();
				}
				else
				{
					WTSLogger::error("Loading parser config {} failed", filename);
				}
			}
			else
			{
				WTSLogger::error("Parser configuration {} not exists", filename);
			}
		}
		else if (cfgParser->type() == WTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}
	else
		WTSLogger::log_raw(LL_WARN, "No parsers config, skipped loading parsers");

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config, bool bAlldayMode /* = false */)
{
	_data_mgr.init(config, &_bd_mgr, bAlldayMode ? NULL : &_state_mon);
}

void WtDtRunner::initParsers(WTSVariant* cfg)
{
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
		
		ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr, &_idx_factory));
		adapter->init(realid.c_str(), cfgItem);
		_parsers.addAdapter(realid.c_str(), adapter);
	}

	WTSLogger::info("{} market data parsers loaded in total", _parsers.size());
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

void WtDtRunner::on_ext_parser_quote(const char* id, WTSTickStruct* curTick, uint32_t uProcFlag)
{
	ParserAdapterPtr adapter = _parsers.getAdapter(id);
	if (adapter)
	{
		WTSTickData* newTick = WTSTickData::create(*curTick);
		adapter->handleQuote(newTick, uProcFlag);
		newTick->release();
	}
	else
	{
		WTSLogger::warn("Parser {} not exists", id);
	}
}

bool WtDtRunner::createExtParser(const char* id)
{
	ParserAdapterPtr adapter(new ParserAdapter(&_bd_mgr, &_data_mgr, &_idx_factory));
	ExpParser* parser = new ExpParser(id);
	adapter->initExt(id, parser);
	_parsers.addAdapter(id, adapter);
	WTSLogger::info("Extended parser {} created", id);
	return true;
}

#pragma endregion 

bool WtDtRunner::createExtDumper(const char* id)
{
	ExpDumperPtr dumper(new ExpDumper(id));
	_dumpers[id] = dumper;

	_data_mgr.add_ext_dumper(id, dumper.get());

	WTSLogger::info("Extended dumper {} created", id);
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