/*!
 * /file WtRunner.h
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#pragma once
#include <string>
#include <unordered_map>

#include "../Includes/ILogHandler.h"

#include "../WtCore/EventNotifier.h"
#include "../WtCore/CtaStrategyMgr.h"
#include "../WtCore/HftStrategyMgr.h"
#include "../WtCore/SelStrategyMgr.h"

#include "../WtCore/WtCtaEngine.h"
#include "../WtCore/WtHftEngine.h"
#include "../WtCore/WtSelEngine.h"
#include "../WtCore/WtLocalExecuter.h"
#include "../WtCore/WtDistExecuter.h"
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

class WtRunner : public ILogHandler
{
public:
	WtRunner();
	~WtRunner();

public:
	/*
	 *	≥ı ºªØ
	 */
	bool init();

	bool config(const char* cfgFile);

	void run(bool bAsync = false);

private:
	bool initTraders(WTSVariant* cfgTrader);
	bool initParsers(WTSVariant* cfgParser);
	bool initExecuters(WTSVariant* cfgExecuter);
	bool initDataMgr();
	bool initEvtNotifier();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initActionPolicy();

	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(WTSLogLevel ll, const char* msg) override;

private:
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
	EventNotifier		_notifier;

	CtaStrategyMgr		_cta_stra_mgr;
	HftStrategyMgr		_hft_stra_mgr;
	SelStrategyMgr		_sel_stra_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
	bool				_is_sel;
};

