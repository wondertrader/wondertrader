/*!
 * \file WtBtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtBtRunner.h"
#include "ExpCtaMocker.h"
#include "ExpSelMocker.h"
#include "ExpHftMocker.h"

#include <iomanip>

#include "../WtBtCore/ExecMocker.h"
#include "../WtBtCore/WtHelper.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/ModuleHelper.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../Includes/WTSVariant.hpp"
#include "../WTSUtils/SignalHook.hpp"

#ifdef _MSC_VER
#include "../Common/mdump.h"
#include <boost/filesystem.hpp>
 //这个主要是给MiniDumper用的
const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{
		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
	}

	return MODULE_NAME;
}
#endif

WtBtRunner::WtBtRunner()
	: _cta_mocker(NULL)
	, _sel_mocker(NULL)

	, _cb_cta_init(NULL)
	, _cb_cta_tick(NULL)
	, _cb_cta_calc(NULL)
	, _cb_cta_calc_done(NULL)
	, _cb_cta_bar(NULL)
	, _cb_cta_sessevt(NULL)
	, _cb_cta_cond_trigger(NULL)

	, _cb_sel_init(NULL)
	, _cb_sel_tick(NULL)
	, _cb_sel_calc(NULL)
	, _cb_sel_calc_done(NULL)
	, _cb_sel_bar(NULL)
	, _cb_sel_sessevt(NULL)

	, _cb_hft_init(NULL)
	, _cb_hft_tick(NULL)
	, _cb_hft_bar(NULL)
	, _cb_hft_ord(NULL)
	, _cb_hft_trd(NULL)
	, _cb_hft_entrust(NULL)
	, _cb_hft_chnl(NULL)

	, _cb_hft_orddtl(NULL)
	, _cb_hft_ordque(NULL)
	, _cb_hft_trans(NULL)

	, _cb_hft_sessevt(NULL)

	, _ext_fnl_bar_loader(NULL)
	, _ext_raw_bar_loader(NULL)
	, _ext_adj_fct_loader(NULL)
	, _ext_tick_loader(NULL)

	, _inited(false)
	, _running(false)
	, _async(false)
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}


WtBtRunner::~WtBtRunner()
{
}

bool WtBtRunner::loadRawHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb)
{
	StdUniqueLock lock(_feed_mtx);
	if (_ext_raw_bar_loader == NULL)
		return false;

	_feed_obj = obj;
	_feeder_bars = cb;

	switch (period)
	{
	case KP_DAY:
        return _ext_raw_bar_loader(stdCode, "d1");
	case KP_Minute1:
        return _ext_raw_bar_loader(stdCode, "m1");
	case KP_Minute5:
        return _ext_raw_bar_loader(stdCode, "m5");
	default:
		{
			WTSLogger::error("Unsupported period of extended data loader");
			return false;
		}
	}
}

bool WtBtRunner::loadFinalHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb)
{
	StdUniqueLock lock(_feed_mtx);
	if (_ext_fnl_bar_loader == NULL)
		return false;

	_feed_obj = obj;
	_feeder_bars = cb;

	switch (period)
	{
	case KP_DAY:
		return _ext_fnl_bar_loader(stdCode, "d1");
	case KP_Minute1:
		return _ext_fnl_bar_loader(stdCode, "m1");
	case KP_Minute5:
		return _ext_fnl_bar_loader(stdCode, "m5");
	default:
		{
			WTSLogger::error("Unsupported period of extended data loader");
			return false;
		}
	}
}

bool WtBtRunner::loadAllAdjFactors(void* obj, FuncReadFactors cb)
{
	StdUniqueLock lock(_feed_mtx);
	if (_ext_adj_fct_loader == NULL)
		return false;

	_feed_obj = obj;
	_feeder_fcts = cb;

	return _ext_adj_fct_loader("");
}

bool WtBtRunner::loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb)
{
	StdUniqueLock lock(_feed_mtx);
	if (_ext_adj_fct_loader == NULL)
		return false;

	_feed_obj = obj;
	_feeder_fcts = cb;

	return _ext_adj_fct_loader(stdCode);
}

bool WtBtRunner::loadRawHisTicks(void* obj, const char* stdCode, uint32_t uDate, FuncReadTicks cb)
{
	StdUniqueLock lock(_feed_mtx);
	if (_ext_tick_loader == NULL)
		return false;

	_feed_obj = obj;
	_feeder_ticks = cb;

	return _ext_tick_loader(stdCode, uDate);
}

void WtBtRunner::feedRawBars(WTSBarStruct* bars, uint32_t count)
{
	if(_ext_fnl_bar_loader == NULL && _ext_raw_bar_loader == NULL)
	{
		WTSLogger::error("Cannot feed bars because of no extented bar loader registered.");
		return;
	}

	_feeder_bars(_feed_obj, bars, count);
}

void WtBtRunner::feedAdjFactors(const char* stdCode, uint32_t* dates, double* factors, uint32_t count)
{
	if(_ext_adj_fct_loader == NULL)
	{
		WTSLogger::error("Cannot feed adjusting factors because of no extented adjusting factor loader registered.");
		return;
	}

	_feeder_fcts(_feed_obj, stdCode, dates, factors, count);
}

void WtBtRunner::feedRawTicks(WTSTickStruct* ticks, uint32_t count)
{
	if (_ext_tick_loader == NULL)
	{
		WTSLogger::error("Cannot feed ticks because of no extented tick loader registered.");
		return;
	}

	_feeder_ticks(_feed_obj, ticks, count);
}

void WtBtRunner::registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, 
	FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone /* = NULL */, FuncStraCondTriggerCallback cbCondTrigger /* = NULL */)
{
	_cb_cta_init = cbInit;
	_cb_cta_tick = cbTick;
	_cb_cta_calc = cbCalc;
	_cb_cta_bar = cbBar;
	_cb_cta_sessevt = cbSessEvt;

	_cb_cta_calc_done = cbCalcDone;
	_cb_cta_cond_trigger = cbCondTrigger;

	WTSLogger::info("Callbacks of CTA engine registration done");
}

void WtBtRunner::registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, 
	FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone/* = NULL*/)
{
	_cb_sel_init = cbInit;
	_cb_sel_tick = cbTick;
	_cb_sel_calc = cbCalc;
	_cb_sel_bar = cbBar;
	_cb_sel_sessevt = cbSessEvt;

	_cb_sel_calc_done = cbCalcDone;

	WTSLogger::info("Callbacks of SEL engine registration done");
}

void WtBtRunner::registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
	FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust, 
	FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt)
{
	_cb_hft_init = cbInit;
	_cb_hft_tick = cbTick;
	_cb_hft_bar = cbBar;

	_cb_hft_chnl = cbChnl;
	_cb_hft_ord = cbOrd;
	_cb_hft_trd = cbTrd;
	_cb_hft_entrust = cbEntrust;

	_cb_hft_orddtl = cbOrdDtl;
	_cb_hft_ordque = cbOrdQue;
	_cb_hft_trans = cbTrans;

	_cb_hft_sessevt = cbSessEvt;

	WTSLogger::info("Callbacks of HFT engine registration done");
}

uint32_t WtBtRunner::initCtaMocker(const char* name, int32_t slippage /* = 0 */, bool hook /* = false */, 
	bool persistData /* = true */, bool bIncremental /* = false */, bool isRatioSlp /* = false */)
{
	if(_cta_mocker)
	{
		delete _cta_mocker;
		_cta_mocker = NULL;
	}

	_cta_mocker = new ExpCtaMocker(&_replayer, name, slippage, persistData, &_notifier, isRatioSlp);
	if (bIncremental)
	{
		_cta_mocker->load_incremental_data(name);
	}
	if(hook) _cta_mocker->install_hook();
	_replayer.register_sink(_cta_mocker, name);
	return _cta_mocker->id();
}

uint32_t WtBtRunner::initHftMocker(const char* name, bool hook/* = false*/)
{
	if (_hft_mocker)
	{
		delete _hft_mocker;
		_hft_mocker = NULL;
	}

	_hft_mocker = new ExpHftMocker(&_replayer, name);
	if (hook) _hft_mocker->install_hook();
	_replayer.register_sink(_hft_mocker, name);
	return _hft_mocker->id();
}

uint32_t WtBtRunner::initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period, 
	const char* trdtpl /* = "CHINA" */, const char* session /* = "TRADING" */, int32_t slippage /* = 0 */, bool isRatioSlp /* = false */)
{
	if (_sel_mocker)
	{
		delete _sel_mocker;
		_sel_mocker = NULL;
	}

	_sel_mocker = new ExpSelMocker(&_replayer, name, slippage, isRatioSlp);
	_replayer.register_sink(_sel_mocker, name);

	_replayer.register_task(_sel_mocker->id(), date, time, period, trdtpl, session);
	return _sel_mocker->id();
}

void WtBtRunner::ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, EngineType eType/*= ET_CTA*/)
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

void WtBtRunner::ctx_on_calc(uint32_t id, uint32_t curDate, uint32_t curTime, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_calc) _cb_cta_calc(id, curDate, curTime); break;
	case ET_SEL: if (_cb_sel_calc) _cb_sel_calc(id, curDate, curTime); break;
	default:
		break;
	}
}

void WtBtRunner::ctx_on_calc_done(uint32_t id, uint32_t curDate, uint32_t curTime, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_calc_done) _cb_cta_calc_done(id, curDate, curTime); break;
	case ET_SEL: if (_cb_sel_calc_done) _cb_sel_calc_done(id, curDate, curTime); break;
	default:
		break;
	}
}

void WtBtRunner::ctx_on_init(uint32_t id, EngineType eType/*= ET_CTA*/)
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

void WtBtRunner::ctx_on_cond_triggered(uint32_t id, const char* stdCode, double target, double price, const char* usertag, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_cond_trigger) _cb_cta_cond_trigger(id, stdCode, target, price, usertag); break;
	default:
		break;
	}
}

void WtBtRunner::ctx_on_session_event(uint32_t id, uint32_t curTDate, bool isBegin /* = true */, EngineType eType /* = ET_CTA */)
{
	switch (eType)
	{
	case ET_CTA: if (_cb_cta_sessevt) _cb_cta_sessevt(id, curTDate, isBegin); break;
	case ET_HFT: if (_cb_hft_sessevt) _cb_hft_sessevt(id, curTDate, isBegin); break;
	case ET_SEL: if (_cb_sel_sessevt) _cb_sel_sessevt(id, curTDate, isBegin); break;
	default:
		break;
	}
}

void WtBtRunner::ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, EngineType eType/*= ET_CTA*/)
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

void WtBtRunner::hft_on_order_queue(uint32_t id, const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_cb_hft_ordque)
		_cb_hft_ordque(id, stdCode, &newOrdQue->getOrdQueStruct());
}

void WtBtRunner::hft_on_order_detail(uint32_t id, const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_cb_hft_orddtl)
		_cb_hft_orddtl(id, stdCode, &newOrdDtl->getOrdDtlStruct());
}

void WtBtRunner::hft_on_transaction(uint32_t id, const char* stdCode, WTSTransData* newTrans)
{
	if (_cb_hft_trans)
		_cb_hft_trans(id, stdCode, &newTrans->getTransStruct());
}

void WtBtRunner::hft_on_channel_ready(uint32_t cHandle, const char* trader)
{
	if (_cb_hft_chnl)
		_cb_hft_chnl(cHandle, trader, 1000/*CHNL_EVENT_READY*/);
}

void WtBtRunner::hft_on_entrust(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag)
{
	if (_cb_hft_entrust)
		_cb_hft_entrust(cHandle, localid, stdCode, bSuccess, message, userTag);
}

void WtBtRunner::hft_on_order(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag)
{
	if (_cb_hft_ord)
		_cb_hft_ord(cHandle, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled, userTag);
}

void WtBtRunner::hft_on_trade(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag)
{
	if (_cb_hft_trd)
		_cb_hft_trd(cHandle, localid, stdCode, isBuy, vol, price, userTag);
}

void WtBtRunner::init(const char* logProfile /* = "" */, bool isFile /* = true */, const char* outDir/* = "./outputs_bt"*/)
{
#ifdef _MSC_VER
	CMiniDumper::Enable(getModuleName(), true, WtHelper::getCWD().c_str());
#endif

	WTSLogger::init(logProfile, isFile);

	WtHelper::setInstDir(getBinDir());
	WtHelper::setOutputDir(outDir);
}

void WtBtRunner::config(const char* cfgFile, bool isFile /* = true */)
{
	if(_inited)
	{
		WTSLogger::error("WtBtEngine has already been inited");
		return;
	}

	_cfg = isFile ? WTSCfgLoader::load_from_file(cfgFile) : WTSCfgLoader::load_from_content(cfgFile, false);
	if(_cfg == NULL)
	{
		WTSLogger::error("Loading config failed");
		return;
	}

	//初始化事件推送器
	initEvtNotifier(_cfg->get("notifier"));

	_replayer.init(_cfg->get("replayer"), &_notifier, _ext_fnl_bar_loader != NULL ? this : NULL);

	WTSVariant* cfgEnv = _cfg->get("env");
	const char* mode = cfgEnv->getCString("mocker");
	WTSVariant* cfgMode = _cfg->get(mode);
	if (strcmp(mode, "cta") == 0 && cfgMode)
	{
		const char* name = cfgMode->getCString("name");
		int32_t slippage = cfgMode->getInt32("slippage");
		_cta_mocker = new ExpCtaMocker(&_replayer, name, slippage, &_notifier);
		_cta_mocker->init_cta_factory(cfgMode);
		_replayer.register_sink(_cta_mocker, name);
	}
	else if (strcmp(mode, "hft") == 0 && cfgMode)
	{
		const char* name = cfgMode->getCString("name");
		_hft_mocker = new ExpHftMocker(&_replayer, name);
		_hft_mocker->init_hft_factory(cfgMode);
		_replayer.register_sink(_hft_mocker, name);
	}
	else if (strcmp(mode, "sel") == 0 && cfgMode)
	{
		const char* name = cfgMode->getCString("name");
		int32_t slippage = cfgMode->getInt32("slippage");
		_sel_mocker = new ExpSelMocker(&_replayer, name, slippage);
		_sel_mocker->init_sel_factory(cfgMode);
		_replayer.register_sink(_sel_mocker, name);

		WTSVariant* cfgTask = cfgMode->get("task");
		if(cfgTask)
			_replayer.register_task(_sel_mocker->id(), cfgTask->getUInt32("date"), cfgTask->getUInt32("time"),
				cfgTask->getCString("period"), cfgTask->getCString("trdtpl"), cfgTask->getCString("session"));
	}
	else if (strcmp(mode, "exec") == 0 && cfgMode)
	{
		const char* name = cfgMode->getCString("name");
		_exec_mocker = new ExecMocker(&_replayer);
		_exec_mocker->init(cfgMode);
		_replayer.register_sink(_exec_mocker, name);
	}
}

void WtBtRunner::run(bool bNeedDump /* = false */, bool bAsync /* = false */)
{
	if (_running)
		return;

	_async = bAsync;

	WTSLogger::info("Backtesting will run in {} mode", _async ? "async" : "sync");

	if (_cta_mocker)
		_cta_mocker->enable_hook(_async);
	else if (_hft_mocker)
		_hft_mocker->enable_hook(_async);

	_replayer.prepare();
	if (!bAsync)
	{
		_replayer.run(bNeedDump);
	}
	else
	{
		_worker.reset(new StdThread([this, bNeedDump]() {
			_running = true;
			try
			{
				_replayer.run(bNeedDump);
			}
			catch (...)
			{
				WTSLogger::error("Exception raised while worker running");
				//print_stack_trace([](const char* message) {
				//	WTSLogger::error(message);
				//});
			}
			WTSLogger::debug("Worker thread of backtest finished");
			_running = false;

		}));
	}
}

void WtBtRunner::stop()
{
	if (!_running)
	{
		if (_worker)
		{
			_worker->join();
			_worker.reset();
		}
		return;
	}

	_replayer.stop();

	WTSLogger::debug("Notify to finish last round");

	if (_cta_mocker)
		_cta_mocker->step_calc();

	if (_hft_mocker)
		_hft_mocker->step_tick();

	WTSLogger::debug("Last round ended");

	if (_worker)
	{
		_worker->join();
		_worker.reset();
	}

	WTSLogger::freeAllDynLoggers();

	WTSLogger::debug("Backtest stopped");
}

void WtBtRunner::release()
{
	WTSLogger::stop();
}

void WtBtRunner::set_time_range(WtUInt64 stime, WtUInt64 etime)
{
	_replayer.set_time_range(stime, etime);

	WTSLogger::info("Backtest time range is set to be [{},{}] mannually", stime, etime);
}

void WtBtRunner::enable_tick(bool bEnabled /* = true */)
{
	_replayer.enable_tick(bEnabled);

	WTSLogger::info("Tick data replaying is {}", bEnabled ? "enabled" : "disabled");
}

void WtBtRunner::clear_cache()
{
	_replayer.clear_cache();
}

const char* WtBtRunner::get_raw_stdcode(const char* stdCode)
{
	static thread_local std::string s;
	s = _replayer.get_rawcode(stdCode);
	return s.c_str();
}

const char* LOG_TAGS[] = {
	"all",
	"debug",
	"info",
	"warn",
	"error",
	"fatal",
	"none",
};

bool WtBtRunner::initEvtNotifier(WTSVariant* cfg)
{
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Object)
		return false;

	_notifier.init(cfg);

	return true;
}