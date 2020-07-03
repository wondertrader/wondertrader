/*!
 * \file WtRtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <unordered_map>

#include "PorterDefs.h"

#include "../WtCore/CtaStrategyMgr.h"
#include "../WtCore/HftStrategyMgr.h"
#include "../WtCore/SelStrategyMgr.h"
#include "../WtCore/WtCtaEngine.h"
#include "../WtCore/WtHftEngine.h"
#include "../WtCore/WtSelEngine.h"
#include "../WtCore/WtExecuter.h"
#include "../WtCore/TraderAdapter.h"
#include "../WtCore/ParserAdapter.h"
#include "../WtCore/WtDataManager.h"
#include "../WtCore/ActionPolicyMgr.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_OTP_BEGIN
class WTSVariant;
class WtDataStorage;
NS_OTP_END

USING_NS_OTP;

class WtRtRunner : public IEngineEvtListener
{
public:
	WtRtRunner();
	~WtRtRunner();

public:
	/*
	 *	≥ı ºªØ
	 */
	bool init(const char* logProfile = "log4cxx.prop");

	bool config(const char* cfgFile);

	void run(bool bAsync = false);

	void release();

	void registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);
	void registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	void registerEvtCallback(FuncEventCallback cbEvt);

	uint32_t		createCtaContext(const char* name);
	uint32_t		createSelContext(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session="TRADING");

	CtaContextPtr	getCtaContext(uint32_t id);
	SelContextPtr	getSelContext(uint32_t id);
	WtEngine*		getEngine(){ return _engine; }

//////////////////////////////////////////////////////////////////////////
//ICtaEventListener
public:
	virtual void on_initialize_event() override
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_INIT, 0, 0);
	}

	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) override
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_SCHDL, uDate, uTime);
	}

	virtual void on_session_event(uint32_t uDate, bool isBegin = true) override
	{
		if (_cb_evt)
			_cb_evt(isBegin ? EVENT_SESSION_BEGIN : EVENT_SESSION_END, uDate, 0);
	}

public:
	void ctx_on_init(uint32_t id, bool isCta = true);
	void ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, bool isCta = true);
	void ctx_on_calc(uint32_t id, bool isCta = true);
	void ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, bool isCta = true);

	bool addExeFactories(const char* folder);
	bool addCtaFactories(const char* folder);
	bool addHftFactories(const char* folder);
	bool addSelFactories(const char* folder);

private:
	bool initTraders();
	bool initParsers();
	bool initExecuters();
	bool initDataMgr();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initSelStrategies();
	bool initActionPolicy();

	bool initEngine();

private:
	FuncStraInitCallback	_cb_cta_init;
	FuncStraTickCallback	_cb_cta_tick;
	FuncStraCalcCallback	_cb_cta_calc;
	FuncStraBarCallback		_cb_cta_bar;

	FuncStraInitCallback	_cb_sel_init;
	FuncStraTickCallback	_cb_sel_tick;
	FuncStraCalcCallback	_cb_sel_calc;
	FuncStraBarCallback		_cb_sel_bar;

	FuncEventCallback		_cb_evt;

	WTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	WtExecuterFactory	_exe_factory;

	WtCtaEngine			_cta_engine;
	WtHftEngine			_hft_engine;
	WtSelEngine			_sel_engine;
	WtEngine*			_engine;

	WtDataStorage*		_data_store;

	WtDataManager		_data_mgr;

	WTSBaseDataMgr		_bd_mgr;
	WTSHotMgr			_hot_mgr;

	CtaStrategyMgr		_cta_mgr;
	HftStrategyMgr		_hft_mgr;
	SelStrategyMgr		_sel_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
	bool				_is_sel;
};

