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
#include "../WtCore/WtCtaEngine.h"
#include "../WtCore/WtHftEngine.h"
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

class WtRtRunner : public ICtaEventListener
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

	void registerCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncEventCallback cbEvt);

	uint32_t		createContext(const char* name);

	CtaContextPtr	getContext(uint32_t id);
	WtCtaEngine&	getEnv(){ return _cta_engine; }

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
	void ctx_on_init(uint32_t id);
	void ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick);
	void ctx_on_calc(uint32_t id);
	void ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar);

	bool addExeFactories(const char* folder);
	bool addCtaFactories(const char* folder);
	bool addHftFactories(const char* folder);

private:
	bool initTraders();
	bool initParsers();
	bool initExecuters();
	bool initDataMgr();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initActionPolicy();

	bool initEnv();

private:
	FuncStraInitCallback	_cb_init;
	FuncStraTickCallback	_cb_tick;
	FuncStraCalcCallback	_cb_calc;
	FuncStraBarCallback		_cb_bar;
	FuncEventCallback		_cb_evt;

	WTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	WtExecuterFactory	_exe_factory;

	WtCtaEngine			_cta_engine;
	WtHftEngine			_hft_engine;
	WtEngine*			_engine;

	WtDataStorage*		_data_store;

	WtDataManager		_data_mgr;

	WTSBaseDataMgr		_bd_mgr;
	WTSHotMgr			_hot_mgr;

	CtaStrategyMgr		_cta_mgr;
	HftStrategyMgr		_hft_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
};

