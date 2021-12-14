/*!
 * /file WtRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "WtRunner.h"

#include "../WtCore/WtHelper.h"
#include "../WtCore/CtaStraContext.h"
#include "../WtCore/HftStraContext.h"

#include "../Includes/WTSVariant.hpp"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/SignalHook.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/JsonToVariant.hpp"


const char* getBinDir()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}



WtRunner::WtRunner()
	: _data_store(NULL)
	, _is_hft(false)
	, _is_sel(false)
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}


WtRunner::~WtRunner()
{
}

bool WtRunner::init()
{
	std::string path = WtHelper::getCWD() + "logcfg.json";
	WTSLogger::init(path.c_str());

	WtHelper::setInstDir(getBinDir());

	return true;
}

bool WtRunner::config(const char* cfgFile)
{
	std::string json;
	BoostFile::read_file_contents(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	if(document.HasParseError())
	{
		auto ec = document.GetParseError();
		WTSLogger::error("Configuration file parsing failed");
		return false;
	}

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

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
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	if (cfgBF->get("second"))
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));

	//初始化运行环境
	initEngine();

	//初始化数据管理
	initDataMgr();

	if (!initActionPolicy())
		return false;

	//初始化行情通道
	WTSVariant* cfgParser = _config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info("Reading parser configuration from %s……", filename);
				std::string json;
				StdFile::read_file_content(filename, json);

				rj::Document document;
				document.Parse(json.c_str());
				WTSVariant* var = WTSVariant::createObject();
				jsonToVariant(document, var);

				if (!initParsers(var->get("parsers")))
					WTSLogger::error("Loading parsers failed");
				var->release();
			}
			else
			{
				WTSLogger::error("Parser configuration %s not exists", filename);
			}
		}
		else if (cfgParser->type() == WTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}

	//初始化交易通道
	WTSVariant* cfgTraders = _config->get("traders");
	if (cfgTraders)
	{
		if (cfgTraders->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgTraders->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info("Reading trader configuration from %s……", filename);
				std::string json;
				StdFile::read_file_content(filename, json);

				rj::Document document;
				document.Parse(json.c_str());
				WTSVariant* var = WTSVariant::createObject();
				jsonToVariant(document, var);

				if (!initTraders(var->get("traders")))
					WTSLogger::error("Loading traders failed");
				var->release();
			}
			else
			{
				WTSLogger::error("Trader configuration %s not exists", filename);
			}
		}
		else if (cfgTraders->type() == WTSVariant::VT_Array)
		{
			initTraders(cfgTraders);
		}
	}

	initEvtNotifier();

	//如果不是高频引擎,则需要配置执行模块
	if (!_is_hft)
	{
		WTSVariant* cfgExec = _config->get("executers");
		if (cfgExec != NULL)
		{
			if (cfgExec->type() == WTSVariant::VT_String)
			{
				const char* filename = cfgExec->asCString();
				if (StdFile::exists(filename))
				{
					WTSLogger::info("Reading executer configuration from %s……", filename);
					std::string json;
					StdFile::read_file_content(filename, json);

					rj::Document document;
					document.Parse(json.c_str());
					WTSVariant* var = WTSVariant::createObject();
					jsonToVariant(document, var);

					if (!initExecuters(var->get("executers")))
						WTSLogger::error("Loading executer failed");
					var->release();
				}
				else
				{
					WTSLogger::error("Trader configuration %s not exists", filename);
				}
			}
			else if (cfgExec->type() == WTSVariant::VT_Array)
			{
				initExecuters(cfgExec);
			}
		}

	}

	if (!_is_hft)
		initCtaStrategies();
	else
		initHftStrategies();
	
	return true;
}

bool WtRunner::initCtaStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("cta");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "cta/";
	_cta_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		CtaStrategyPtr stra = _cta_stra_mgr.createStrategy(name, id);
		stra->self()->init(cfgItem->get("params"));
		CtaStraContext* ctx = new CtaStraContext(&_cta_engine, id);
		ctx->set_strategy(stra->self());
		_cta_engine.addContext(CtaContextPtr(ctx));
	}

	return true;
}

bool WtRunner::initHftStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("hft");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "hft/";
	_hft_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		bool agent = cfgItem->getBoolean("agent");
		HftStrategyPtr stra = _hft_stra_mgr.createStrategy(name, id);
		if (stra == NULL)
			continue;

		stra->self()->init(cfgItem->get("params"));
		HftStraContext* ctx = new HftStraContext(&_hft_engine, id, agent);
		ctx->set_strategy(stra->self());

		const char* traderid = cfgItem->getCString("trader");
		TraderAdapterPtr trader = _traders.getAdapter(traderid);
		if(trader)
		{
			ctx->setTrader(trader.get());
			trader->addSink(ctx);
		}
		else
		{
			WTSLogger::error("Trader %s not exists, binding trader to HFT strategy failed", traderid);
		}

		_hft_engine.addContext(HftContextPtr(ctx));
	}

	return true;
}


bool WtRunner::initEngine()
{
	WTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	const char* name = cfg->getCString("name");
	
	if (strlen(name) == 0 || wt_stricmp(name, "cta") == 0)
	{
		_is_hft = false;
		_is_sel = false;
	}
	else if (wt_stricmp(name, "sel") == 0)
	{
		_is_sel = true;
	}
	else //if (wt_stricmp(name, "hft") == 0)
	{
		_is_hft = true;
	}

	if (_is_hft)
	{
		WTSLogger::info("Trading enviroment initialzied with engine: HFT");
		_hft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr, &_notifier);
		_engine = &_hft_engine;
	}
	else if (_is_sel)
	{
		WTSLogger::info("Trading enviroment initialzied with engine: SEL");
		_sel_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr, &_notifier);
		_engine = &_sel_engine;
	}
	else
	{
		WTSLogger::info("Trading enviroment initialzied with engine: CTA");
		_cta_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr, &_notifier);
		_engine = &_cta_engine;
	}

	_engine->set_adapter_mgr(&_traders);

	return true;
}

bool WtRunner::initActionPolicy()
{
	return _act_policy.init(_config->getCString("bspolicy"));
}

bool WtRunner::initDataMgr()
{
	WTSVariant*cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, _engine);
	WTSLogger::info("Data manager initialized");

	return true;
}

bool WtRunner::initParsers(WTSVariant* cfgParser)
{
	if (cfgParser == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgParser->size(); idx++)
	{
		WTSVariant* cfgItem = cfgParser->get(idx);
		if(!cfgItem->getBoolean("active"))
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

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(realid.c_str(), cfgItem, _engine, &_bd_mgr, &_hot_mgr);
		_parsers.addAdapter(realid.c_str(), adapter);

		count++;
	}

	WTSLogger::info("%u parsers loaded", count);
	return true;
}

bool WtRunner::initExecuters(WTSVariant* cfgExecuter)
{
	if (cfgExecuter == NULL || cfgExecuter->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "executer/";
	_exe_factory.loadFactories(path.c_str());

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgExecuter->size(); idx++)
	{
		WTSVariant* cfgItem = cfgExecuter->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");
		bool bLocal = cfgItem->getBoolean("local");

		if (bLocal)
		{
			WtLocalExecuter* executer = new WtLocalExecuter(&_exe_factory, id, &_data_mgr);
			if (!executer->init(cfgItem))
				return false;

			TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
			executer->setTrader(trader.get());
			trader->addSink(executer);

			_cta_engine.addExecuter(ExecCmdPtr(executer));
		}
		else
		{
			WtDistExecuter* executer = new WtDistExecuter(id);
			if (!executer->init(cfgItem))
				return false;

			_cta_engine.addExecuter(ExecCmdPtr(executer));
		}
		count++;
	}

	WTSLogger::info("%u executers loaded", count);

	return true;
}

bool WtRunner::initTraders(WTSVariant* cfgTrader)
{
	if (cfgTrader == NULL || cfgTrader->type() != WTSVariant::VT_Array)
		return false;
	
	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfgTrader->size(); idx++)
	{
		WTSVariant* cfgItem = cfgTrader->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter(&_notifier));
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("%u traders loaded", count);

	return true;
}

void WtRunner::run(bool bAsync /* = false */)
{
	try
	{
		_parsers.run();
		_traders.run();

		_engine->run(bAsync);
	}
	catch (...)
	{
		//print_stack_trace([](const char* message) {
		//	WTSLogger::error(message);
		//});
	}
}

const char* LOG_TAGS[] = {
	"all",
	"debug",
	"info",
	"warn",
	"error",
	"fatal",
	"none"
};

void WtRunner::handleLogAppend(WTSLogLevel ll, const char* msg)
{
	_notifier.notifyLog(LOG_TAGS[ll - 100], msg);
}

bool WtRunner::initEvtNotifier()
{
	WTSVariant* cfg = _config->get("notifier");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	_notifier.init(cfg);

	return true;
}