/*!
 * \file WtRtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtRtRunner.h"
#include "ExpCtaContext.h"
#include "ExpSelContext.h"
#include "ExpHftContext.h"

#include "../WtCore/WtHelper.h"
#include "../WtCore/CtaStraContext.h"
#include "../WtCore/HftStraContext.h"
#include "../WtCore/SelStraContext.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/JsonToVariant.hpp"

#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif

extern const char* getBinDir();

WtRtRunner::WtRtRunner()
	: _data_store(NULL)
	, _cb_cta_init(NULL)
	, _cb_cta_tick(NULL)
	, _cb_cta_calc(NULL)
	, _cb_cta_bar(NULL)

	, _cb_sel_init(NULL)
	, _cb_sel_tick(NULL)
	, _cb_sel_calc(NULL)
	, _cb_sel_bar(NULL)

	, _cb_hft_init(NULL)
	, _cb_hft_tick(NULL)
	, _cb_hft_bar(NULL)
	, _cb_hft_ord(NULL)
	, _cb_hft_trd(NULL)
	, _cb_hft_pos(NULL)
	, _cb_hft_entrust(NULL)
	, _cb_hft_chnl(NULL)

	, _cb_evt(NULL)
	, _is_hft(false)
	, _is_sel(false)
{

}


WtRtRunner::~WtRtRunner()
{
}

bool WtRtRunner::init(const char* logProfile /* = "log4cxx.prop" */)
{
	std::string path = WtHelper::getCWD() + logProfile;
	WTSLogger::init(path.c_str());

	WtHelper::setInstDir(getBinDir());
	return true;
}

void WtRtRunner::registerEvtCallback(FuncEventCallback cbEvt)
{
	_cb_evt = cbEvt;

	_cta_engine.regEventListener(this);
	_hft_engine.regEventListener(this);
	_sel_engine.regEventListener(this);
}

void WtRtRunner::registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_cta_init = cbInit;
	_cb_cta_tick = cbTick;
	_cb_cta_calc = cbCalc;
	_cb_cta_bar = cbBar;
}

void WtRtRunner::registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	_cb_sel_init = cbInit;
	_cb_sel_tick = cbTick;
	_cb_sel_calc = cbCalc;
	_cb_sel_bar = cbBar;
}

void WtRtRunner::registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar, 
	FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftPosCallback cbPos, FuncHftEntrustCallback cbEntrust)
{
	_cb_hft_init = cbInit;
	_cb_hft_tick = cbTick;
	_cb_hft_bar = cbBar;

	_cb_hft_chnl = cbChnl;
	_cb_hft_ord = cbOrd;
	_cb_hft_trd = cbTrd;
	_cb_hft_pos = cbPos;
	_cb_hft_entrust = cbEntrust;
}

uint32_t WtRtRunner::createCtaContext(const char* name)
{
	ExpCtaContext* ctx = new ExpCtaContext(&_cta_engine, name);
	_cta_engine.addContext(CtaContextPtr(ctx));
	return ctx->id();
}

uint32_t WtRtRunner::createHftContext(const char* name, const char* trader)
{
	ExpHftContext* ctx = new ExpHftContext(&_hft_engine, name);
	_hft_engine.addContext(HftContextPtr(ctx));
	TraderAdapterPtr trdPtr = _traders.getAdapter(trader);
	ctx->setTrader(trdPtr.get());
	return ctx->id();
}

uint32_t WtRtRunner::createSelContext(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl /* = "CHINA" */, const char* session/* ="TRADING" */)
{
	TaskPeriodType ptype;
	if (my_stricmp(period, "d") == 0)
		ptype = TPT_Daily;
	else if (my_stricmp(period, "w") == 0)
		ptype = TPT_Weekly;
	else if (my_stricmp(period, "m") == 0)
		ptype = TPT_Monthly;
	else if (my_stricmp(period, "y") == 0)
		ptype = TPT_Yearly;
	else if (my_stricmp(period, "min") == 0)
		ptype = TPT_Minute;
	else
		ptype = TPT_None;

	ExpSelContext* ctx = new ExpSelContext(&_sel_engine, name);

	_sel_engine.addContext(SelContextPtr(ctx), date, time, ptype, true, trdtpl, session);

	return ctx->id();
}

CtaContextPtr WtRtRunner::getCtaContext(uint32_t id)
{
	return _cta_engine.getContext(id);
}

HftContextPtr WtRtRunner::getHftContext(uint32_t id)
{
	return _hft_engine.getContext(id);
}

SelContextPtr WtRtRunner::getSelContext(uint32_t id)
{
	return _sel_engine.getContext(id);
}

void WtRtRunner::ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_bar) _cb_cta_bar(id, stdCode, period, newBar); break;
	case ET_HFT: if (_cb_hft_bar) _cb_hft_bar(id, stdCode, period, newBar); break;
	case ET_SEL: if (_cb_sel_bar) _cb_sel_bar(id, stdCode, period, newBar); break;
	default:
		break;
	}
}

void WtRtRunner::ctx_on_calc(uint32_t id, uint32_t curDate, uint32_t curTime, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_calc) _cb_cta_calc(id, curDate, curTime); break;
	case ET_SEL: if (_cb_sel_calc) _cb_sel_calc(id, curDate, curTime); break;
	default:
		break;
	}
}

void WtRtRunner::ctx_on_init(uint32_t id, EngineType eType/* = ET_CTA*/)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_init) _cb_cta_init(id); break;
	case ET_HFT: if (_cb_hft_init) _cb_hft_init(id); break;
	case ET_SEL: if (_cb_sel_init) _cb_sel_init(id); break;
	default:
		break;
	}
}

void WtRtRunner::ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_tick) _cb_cta_tick(id, stdCode, &newTick->getTickStruct()); break;
	case ET_HFT: if (_cb_hft_tick) _cb_hft_tick(id, stdCode, &newTick->getTickStruct()); break;
	case ET_SEL: if (_cb_sel_tick) _cb_sel_tick(id, stdCode, &newTick->getTickStruct()); break;
	default:
		break;
	}
}

void WtRtRunner::hft_on_channel_lost(uint32_t cHandle, const char* trader)
{
	if (_cb_hft_chnl)
		_cb_hft_chnl(cHandle, trader, CHNL_EVENT_LOST);
}

void WtRtRunner::hft_on_channel_ready(uint32_t cHandle, const char* trader)
{
	if (_cb_hft_chnl)
		_cb_hft_chnl(cHandle, trader, CHNL_EVENT_READY);
}

void WtRtRunner::hft_on_entrust(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_cb_hft_entrust)
		_cb_hft_entrust(cHandle, localid, stdCode, bSuccess, message);
}

void WtRtRunner::hft_on_order(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled)
{
	if (_cb_hft_ord)
		_cb_hft_ord(cHandle, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void WtRtRunner::hft_on_position(uint32_t cHandle, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	if (_cb_hft_pos)
		_cb_hft_pos(cHandle, stdCode, isLong, prevol, preavail, newvol, newavail);
}

void WtRtRunner::hft_on_trade(uint32_t cHandle, const char* stdCode, bool isBuy, double vol, double price)
{
	if (_cb_hft_trd)
		_cb_hft_trd(cHandle, stdCode, isBuy, vol, price);
}

bool WtRtRunner::config(const char* cfgFile)
{
	std::string json;
	StdFile::read_file_content(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("交易时间模板加载完成");
	}

	if (cfgBF->get("commodity"))
	{
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("品种列表加载完成");
	}

	if (cfgBF->get("contract"))
	{
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("合约列表加载完成");
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("节假日模板加载完成");
	}

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("主力切换表加载完成");
	}

	//初始化运行环境
	initEngine();

	//初始化数据管理
	initDataMgr();

	//初始化开平策略
	if (!initActionPolicy())
		return false;

	//初始化行情通道
	initParsers();

	//初始化交易通道
	initTraders();

	//如果不是高频引擎，则需要配置执行模块
	if (!_is_hft)
		initExecuters();

	//这里本来应该有，但是对于py部分，就不要这个了
	//initStrategies();

	if (!_is_hft)
		initCtaStrategies();
	else
		initHftStrategies();
	
	return true;
}

bool WtRtRunner::initCtaStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("cta");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		CtaStrategyPtr stra = _cta_mgr.createStrategy(name, id);
		stra->self()->init(cfgItem->get("params"));
		CtaStraContext* ctx = new CtaStraContext(&_cta_engine, id);
		ctx->set_strategy(stra->self());
		_cta_engine.addContext(CtaContextPtr(ctx));
	}

	return true;
}

bool WtRtRunner::initSelStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("cta");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");

		uint32_t date = cfgItem->getUInt32("date");
		uint32_t time = cfgItem->getUInt32("time");
		const char* period = cfgItem->getCString("period");

		TaskPeriodType ptype;
		if (my_stricmp(period, "d") == 0)
			ptype = TPT_Daily;
		else if (my_stricmp(period, "w") == 0)
			ptype = TPT_Weekly;
		else if (my_stricmp(period, "m") == 0)
			ptype = TPT_Monthly;
		else if (my_stricmp(period, "y") == 0)
			ptype = TPT_Yearly;
		else
			ptype = TPT_None;

		SelStrategyPtr stra = _sel_mgr.createStrategy(name, id);
		stra->self()->init(cfgItem->get("params"));
		SelStraContext* ctx = new SelStraContext(&_sel_engine, id);
		ctx->set_strategy(stra->self());
		_sel_engine.addContext(SelContextPtr(ctx), date, time, ptype);
	}

	return true;
}

bool WtRtRunner::initHftStrategies()
{
	WTSVariant* cfg = _config->get("strategies");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	cfg = cfg->get("hft");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		const char* id = cfgItem->getCString("id");
		const char* name = cfgItem->getCString("name");
		HftStrategyPtr stra = _hft_mgr.createStrategy(name, id);
		if (stra == NULL)
			continue;

		stra->self()->init(cfgItem->get("params"));
		HftStraContext* ctx = new HftStraContext(&_hft_engine, id);
		ctx->set_strategy(stra->self());

		TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
		ctx->setTrader(trader.get());
		trader->addSink(ctx);

		_hft_engine.addContext(HftContextPtr(ctx));
	}

	return true;
}

bool WtRtRunner::initEngine()
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
		WTSLogger::info("交易环境初始化完成，交易引擎：HFT");
		_hft_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_hft_engine;
	}
	else if (_is_sel)
	{
		WTSLogger::info("交易环境初始化完成，交易引擎：SelStk");
		_sel_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_sel_engine;
	}
	else
	{
		WTSLogger::info("交易环境初始化完成，交易引擎：CTA");
		_cta_engine.init(cfg, &_bd_mgr, &_data_mgr, &_hot_mgr);
		_engine = &_cta_engine;
	}
	
	return true;
}

bool WtRtRunner::initDataMgr()
{
	WTSVariant* cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, _engine);

	WTSLogger::info("数据管理模块初始化完成");
	return true;
}

bool WtRtRunner::initParsers()
{
	WTSVariant* cfg = _config->get("parsers");
	if (cfg == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(id, cfgItem, _engine, _engine->get_basedata_mgr(), _engine->get_hot_mgr());

		_parsers.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("共加载%u个行情通道", count);

	return true;
}

bool WtRtRunner::initExecuters()
{
	WTSVariant* cfg = _config->get("executers");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	//先加载自带的执行器工厂
	std::string path = WtHelper::getInstDir() + "executer//";
	_exe_factory.loadFactories(path.c_str());

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		WtExecuter* executer = new WtExecuter(&_exe_factory, id, &_data_mgr);
		if (!executer->init(cfgItem))
			return false;

		TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
		executer->setTrader(trader.get());
		trader->addSink(executer);

		//WtExecuterPtr exec_ptr(executer);
		_cta_engine.addExecuter(ExecCmdPtr(executer));
		count++;
	}

	WTSLogger::info("共加载%u个执行器", count);

	return true;
}

bool WtRtRunner::initTraders()
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

void WtRtRunner::run(bool bAsync /* = false */)
{
	_parsers.run();
	_traders.run();

	_engine->run(bAsync);
}

void WtRtRunner::release()
{
	WTSLogger::stop();
}

bool WtRtRunner::initActionPolicy()
{
	const char* action_file = _config->getCString("bspolicy");
	if (strlen(action_file) <= 0)
		return false;

	bool ret = _act_policy.init(action_file);
	WTSLogger::info("开平策略模板初始化完成");
	return ret;
}

bool WtRtRunner::addCtaFactories(const char* folder)
{
	return _cta_mgr.loadFactories(folder);
}

bool WtRtRunner::addSelFactories(const char* folder)
{
	return _sel_mgr.loadFactories(folder);
}

bool WtRtRunner::addExeFactories(const char* folder)
{
	return _exe_factory.loadFactories(folder);
}

bool WtRtRunner::addHftFactories(const char* folder)
{
	return _hft_mgr.loadFactories(folder);
}