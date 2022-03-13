/*!
 * /file WtUftRunner.h
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

#include "../WtUftCore/UftStrategyMgr.h"

#include "../WtUftCore/WtUftEngine.h"
#include "../WtUftCore/TraderAdapter.h"
#include "../WtUftCore/ParserAdapter.h"
#include "../WtUftCore/WtUftDtMgr.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_WTP_BEGIN
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

class WtUftRunner : public ILogHandler
{
public:
	WtUftRunner();
	~WtUftRunner();

public:
	/*
	 *	≥ı ºªØ
	 */
	bool init();

	bool config();

	void run(bool bAsync = false);

private:
	bool initTraders(WTSVariant* cfgTrader);
	bool initParsers(WTSVariant* cfgParser);
	bool initDataMgr();
	bool initUftStrategies();

	bool initEngine();

//////////////////////////////////////////////////////////////////////////
//ILogHandler
public:
	virtual void handleLogAppend(WTSLogLevel ll, const char* msg) override;

private:
	WTSVariant*			_config;
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;

	WtUftEngine			_uft_engine;

	WtUftDtMgr			_data_mgr;

	WTSBaseDataMgr		_bd_mgr;
	WTSHotMgr			_hot_mgr;

	UftStrategyMgr		_uft_stra_mgr;
};

