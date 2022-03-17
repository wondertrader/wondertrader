/*!
 * /file WtUftRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "WtUftRunner.h"

#include "../WtUftCore/WtHelper.h"
#include "../WtUftCore/UftStraContext.h"

#include "../Includes/WTSVariant.hpp"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../WTSUtils/SignalHook.hpp"
#include "../Share/StrUtil.hpp"


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



WtUftRunner::WtUftRunner()
{
//#if _WIN32
//#pragma message("Signal hooks disabled in WIN32")
//#else
//#pragma message("Signal hooks enabled in UNIX")
//	install_signal_hooks([](const char* message) {
//		WTSLogger::error(message);
//	});
//#endif
}


WtUftRunner::~WtUftRunner()
{
}

bool WtUftRunner::init()
{
	std::string path = "logcfg.json";
	if(!StdFile::exists(path.c_str()))
		path = "logcfg.yaml";
	WTSLogger::init(path.c_str());

	WtHelper::setInstDir(getBinDir());

	return true;
}

bool WtUftRunner::config()
{
	std::string cfgFile = "config.json";
	if (!StdFile::exists(cfgFile.c_str()))
		cfgFile = "config.yaml";

	_config = WTSCfgLoader::load_from_file(cfgFile, true);
	if(_config == NULL)
	{
		WTSLogger::error_f("Loading config file {} failed", cfgFile);
		return false;
	}

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	bool isUTF8 = cfgBF->getBoolean("utf-8");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"), isUTF8);

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));


	//初始化运行环境
	initEngine();

	//初始化数据管理
	initDataMgr();

	//初始化行情通道
	WTSVariant* cfgParser = _config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info_f("Reading parser config from {}...", filename);
				WTSVariant* var = WTSCfgLoader::load_from_file(filename, isUTF8);
				if(var)
				{
					if (!initParsers(var->get("parsers")))
						WTSLogger::error("Loading parsers failed");
					var->release();
				}
				else
				{
					WTSLogger::error_f("Loading parser config {} failed", filename);
				}
			}
			else
			{
				WTSLogger::error_f("Parser configuration {} not exists", filename);
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
				WTSLogger::info_f("Reading trader config from {}...", filename);
				WTSVariant* var = WTSCfgLoader::load_from_file(filename, isUTF8);
				if (var)
				{
					if (!initTraders(var->get("traders")))
						WTSLogger::error("Loading traders failed");
					var->release();
				}
				else
				{
					WTSLogger::error_f("Loading trader config {} failed", filename);
				}
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

	initUftStrategies();
	
	return true;
}

bool WtUftRunner::initUftStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("uft");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "uft/";
	_uft_stra_mgr.loadFactories(path.c_str());

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		UftStrategyPtr stra = _uft_stra_mgr.createStrategy(name, id);
		if (stra == NULL)
			continue;

		stra->self()->init(cfgItem->get("params"));
		UftStraContext* ctx = new UftStraContext(&_uft_engine, id);
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

		_uft_engine.addContext(UftContextPtr(ctx));
	}

	return true;
}

bool WtUftRunner::initEngine()
{
	WTSVariant* cfg = _config->get("env");
	if (cfg == NULL)
		return false;

	WTSLogger::info("Trading enviroment initialzied with engine: UFT");
	_uft_engine.init(cfg, &_bd_mgr, &_data_mgr);

	_uft_engine.set_adapter_mgr(&_traders);

	return true;
}


bool WtUftRunner::initDataMgr()
{
	WTSVariant*cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, &_uft_engine);
	WTSLogger::info("Data manager initialized");

	return true;
}

bool WtUftRunner::initParsers(WTSVariant* cfgParser)
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
		adapter->init(realid.c_str(), cfgItem, &_uft_engine, &_bd_mgr);
		_parsers.addAdapter(realid.c_str(), adapter);

		count++;
	}

	WTSLogger::info("%u parsers loaded", count);
	return true;
}

bool WtUftRunner::initTraders(WTSVariant* cfgTrader)
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

		TraderAdapterPtr adapter(new TraderAdapter());
		adapter->init(id, cfgItem, &_bd_mgr);

		_traders.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("%u traders loaded", count);

	return true;
}

void WtUftRunner::run(bool bAsync /* = false */)
{
	try
	{
		_parsers.run();
		_traders.run();

		_uft_engine.run(bAsync);
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

void WtUftRunner::handleLogAppend(WTSLogLevel ll, const char* msg)
{
}