/*!
 * \file WtRunner.h
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

#include "..\WtCore\CtaStrategyMgr.h"
#include "..\WtCore\HftStrategyMgr.h"

#include "..\WtCore\WtCtaEngine.h"
#include "..\WtCore\WtHftEngine.h"
#include "..\WtCore\WtLocalExecuter.h"
#include "..\WtCore\TraderAdapter.h"
#include "..\WtCore\ParserAdapter.h"
#include "..\WtCore\WtDataManager.h"
#include "..\WtCore\ActionPolicyMgr.h"

#include "..\WTSTools\WTSHotMgr.h"
#include "..\WTSTools\WTSBaseDataMgr.h"

NS_OTP_BEGIN
class WTSVariant;
class WtDataStorage;
NS_OTP_END

USING_NS_OTP;

class WtRunner
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
	bool initTraders();
	bool initParsers();
	bool initExecuters();
	bool initDataMgr();
	bool initCtaStrategies();
	bool initHftStrategies();
	bool initActionPolicy();

	bool initEnv();

private:
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

	CtaStrategyMgr		_cta_stra_mgr;
	HftStrategyMgr		_hft_stra_mgr;
	ActionPolicyMgr		_act_policy;

	bool				_is_hft;
};

