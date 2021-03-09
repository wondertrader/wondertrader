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
#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/JsonToVariant.hpp"

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif


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
		WTSLogger::error("配置文件解析失败");
		return false;
	}

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

	if (cfgBF->get("commodity"))
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));

	if (cfgBF->get("contract"))
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	//初始化运行环境
	initEngine();

	//初始化数据管理
	initDataMgr();

	if (!initActionPolicy())
		return false;

	//初始化行情通道
	initParsers();

	//初始化交易通道
	initTraders();

	//如果不是高频引擎,则需要配置执行模块
	if (!_is_hft)
		initExecuters();

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
			WTSLogger::error("交易通道%s不存在,HFT策略绑定交易通道失败", traderid);
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
	
	if (strlen(name) == 0 || my_stricmp(name, "cta") == 0)
	{
		_is_hft = false;
		_is_sel = false;
	}
	else if (my_stricmp(name, "sel") == 0)
	{
		_is_sel = true;
	}
	else //if (my_stricmp(name, "hft") == 0)
	{
		_is_hft = true;
	}

	if (_is_hft)
	{
		WTSLogger::info("交易环境初始化完成,交易引擎: HFT");
		_hft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_hft_engine;
	}
	else if (_is_sel)
	{
		WTSLogger::info("交易环境初始化完成,交易引擎: SelStk");
		_sel_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_sel_engine;
	}
	else
	{
		WTSLogger::info("交易环境初始化完成,交易引擎: CTA");
		_cta_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
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
	WTSLogger::info("数据管理模块初始化完成");

	return true;
}

bool WtRunner::initParsers()
{
	WTSVariant* cfg = _config->get("parsers");
	if (cfg == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if(!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(id, cfgItem, _engine, &_bd_mgr, &_hot_mgr);

		_parsers.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("共加载%u个行情通道", count);
	return true;
}

bool WtRunner::initExecuters()
{
	WTSVariant* cfg = _config->get("executers");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	std::string path = WtHelper::getCWD() + "executer/";
	_exe_factory.loadFactories(path.c_str());

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
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

	WTSLogger::info("共加载%u个执行器", count);

	return true;
}

bool WtRunner::initTraders()
{
	WTSVariant* cfg = _config->get("traders");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;
	
	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter);
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("共加载%u个交易通道", count);

	return true;
}

void WtRunner::run(bool bAsync /* = false */)
{
	_parsers.run();
	_traders.run();

	_engine->run(bAsync);
}