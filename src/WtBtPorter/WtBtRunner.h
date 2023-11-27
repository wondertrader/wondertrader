/*!
 * \file WtBtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"
#include "../WtBtCore/EventNotifier.h"
#include "../WtBtCore/HisDataReplayer.h"
#include "../Includes/WTSMarcos.h"


NS_WTP_BEGIN
class WTSTickData;
struct WTSBarStruct;
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

typedef enum tagEngineType
{
	ET_CTA = 999,	//CTA引擎	
	ET_HFT,			//高频引擎
	ET_SEL			//选股引擎
} EngineType;

class SelMocker;
class CtaMocker;
class HftMocker;
class ExecMocker;

class WtBtRunner : public IBtDataLoader
{
public:
	WtBtRunner();
	~WtBtRunner();


	//////////////////////////////////////////////////////////////////////////
	//IBtDataLoader
	virtual bool loadFinalHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) override;

	virtual bool loadRawHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) override;

	virtual bool loadAllAdjFactors(void* obj, FuncReadFactors cb) override;

	virtual bool loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb) override;

	virtual bool loadRawHisTicks(void* obj, const char* stdCode, uint32_t uDate, FuncReadTicks cb) override;

	virtual bool isAutoTrans() override
	{
		return _loader_auto_trans;
	}

	void feedRawBars(WTSBarStruct* bars, uint32_t count);
	void feedRawTicks(WTSTickStruct* ticks, uint32_t count);
	void feedAdjFactors(const char* stdCode, uint32_t* dates, double* factors, uint32_t count);

public:
	void	registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, 
		FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone = NULL, FuncStraCondTriggerCallback cbCondTrigger = NULL);
	void	registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, 
		FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone = NULL);
	void registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
		FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
		FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt);

	void registerEvtCallback(FuncEventCallback cbEvt)
	{
		_cb_evt = cbEvt;
	}

	void		registerExtDataLoader(FuncLoadFnlBars fnlBarLoader, FuncLoadRawBars rawBarLoader, FuncLoadAdjFactors fctLoader, FuncLoadRawTicks tickLoader, bool bAutoTrans = true)
	{
		_ext_fnl_bar_loader = fnlBarLoader;
		_ext_raw_bar_loader = rawBarLoader;
		_ext_adj_fct_loader = fctLoader;
		_ext_tick_loader = tickLoader;
		_loader_auto_trans = bAutoTrans;
	}

	uint32_t	initCtaMocker(const char* name, int32_t slippage = 0, bool hook = false, bool persistData = true, bool bIncremental = false, bool isRatioSlp = false);
	uint32_t	initHftMocker(const char* name, bool hook = false);
	uint32_t	initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period, 
		const char* trdtpl = "CHINA", const char* session = "TRADING", int32_t slippage = 0, bool isRatioSlp = false);

	bool	initEvtNotifier(WTSVariant* cfg);

	void	ctx_on_init(uint32_t id, EngineType eType);
	void	ctx_on_session_event(uint32_t id, uint32_t curTDate, bool isBegin = true, EngineType eType = ET_CTA);
	void	ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, EngineType eType);
	void	ctx_on_calc(uint32_t id, uint32_t uDate, uint32_t uTime, EngineType eType);
	void	ctx_on_calc_done(uint32_t id, uint32_t uDate, uint32_t uTime, EngineType eType);
	void	ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, EngineType eType);
	void	ctx_on_cond_triggered(uint32_t id, const char* stdCode, double target, double price, const char* usertag, EngineType eType = ET_CTA);

	void	hft_on_order_queue(uint32_t id, const char* stdCode, WTSOrdQueData* newOrdQue);
	void	hft_on_order_detail(uint32_t id, const char* stdCode, WTSOrdDtlData* newOrdDtl);
	void	hft_on_transaction(uint32_t id, const char* stdCode, WTSTransData* newTranns);

	void	hft_on_channel_ready(uint32_t cHandle, const char* trader);
	void	hft_on_order(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
	void	hft_on_trade(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
	void	hft_on_entrust(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);

	void	init(const char* logProfile = "", bool isFile = true, const char* outDir = "./outputs_bt");
	void	config(const char* cfgFile, bool isFile = true);
	void	run(bool bNeedDump = false, bool bAsync = false);
	void	release();
	void	stop();

	void	set_time_range(WtUInt64 stime, WtUInt64 etime);

	void	enable_tick(bool bEnabled = true);

	void	clear_cache();

	const char*	get_raw_stdcode(const char* stdCode);

	inline CtaMocker*		cta_mocker() { return _cta_mocker; }
	inline SelMocker*		sel_mocker() { return _sel_mocker; }
	inline HftMocker*		hft_mocker() { return _hft_mocker; }
	inline HisDataReplayer&	replayer() { return _replayer; }

	inline bool	isAsync() const { return _async; }

public:
	inline void on_initialize_event()
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_INIT, 0, 0);
	}

	inline void on_schedule_event(uint32_t uDate, uint32_t uTime)
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_SCHDL, uDate, uTime);
	}

	inline void on_session_event(uint32_t uDate, bool isBegin = true)
	{
		if (_cb_evt)
		{
			_cb_evt(isBegin ? EVENT_SESSION_BEGIN : EVENT_SESSION_END, uDate, 0);
		}
	}

	inline void on_backtest_end()
	{
		if (_cb_evt)
			_cb_evt(EVENT_BACKTEST_END, 0, 0);
	}

private:
	FuncStraInitCallback	_cb_cta_init;
	FuncSessionEvtCallback	_cb_cta_sessevt;
	FuncStraTickCallback	_cb_cta_tick;
	FuncStraCalcCallback	_cb_cta_calc;
	FuncStraCalcCallback	_cb_cta_calc_done;
	FuncStraBarCallback		_cb_cta_bar;
	FuncStraCondTriggerCallback _cb_cta_cond_trigger;

	FuncStraInitCallback	_cb_sel_init;
	FuncSessionEvtCallback	_cb_sel_sessevt;
	FuncStraTickCallback	_cb_sel_tick;
	FuncStraCalcCallback	_cb_sel_calc;
	FuncStraCalcCallback	_cb_sel_calc_done;
	FuncStraBarCallback		_cb_sel_bar;

	FuncStraInitCallback	_cb_hft_init;
	FuncSessionEvtCallback	_cb_hft_sessevt;
	FuncStraTickCallback	_cb_hft_tick;
	FuncStraBarCallback		_cb_hft_bar;
	FuncHftChannelCallback	_cb_hft_chnl;
	FuncHftOrdCallback		_cb_hft_ord;
	FuncHftTrdCallback		_cb_hft_trd;
	FuncHftEntrustCallback	_cb_hft_entrust;

	FuncStraOrdQueCallback	_cb_hft_ordque;
	FuncStraOrdDtlCallback	_cb_hft_orddtl;
	FuncStraTransCallback	_cb_hft_trans;

	FuncEventCallback		_cb_evt;

	FuncLoadFnlBars			_ext_fnl_bar_loader;//最终K线加载器
	FuncLoadRawBars			_ext_raw_bar_loader;//原始K线加载器
	FuncLoadAdjFactors		_ext_adj_fct_loader;//复权因子加载器
	FuncLoadRawTicks		_ext_tick_loader;	//tick加载器
	bool					_loader_auto_trans;	//是否自动转储

	CtaMocker*		_cta_mocker;
	SelMocker*		_sel_mocker;
	ExecMocker*		_exec_mocker;
	HftMocker*		_hft_mocker;
	HisDataReplayer	_replayer;
	EventNotifier	_notifier;

	bool			_inited;
	bool			_running;

	StdThreadPtr	_worker;
	bool			_async;

	void*			_feed_obj;
	FuncReadBars	_feeder_bars;
	FuncReadTicks	_feeder_ticks;
	FuncReadFactors	_feeder_fcts;
	StdUniqueMutex	_feed_mtx;
	WTSVariant* _cfg;
};

