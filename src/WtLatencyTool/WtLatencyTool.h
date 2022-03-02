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
#include "../WtUftCore/WtUftEngine.h"
#include "../WtUftCore/UftStrategyMgr.h"
#include "../WtUftCore/TraderAdapter.h"
#include "../WtUftCore/ParserAdapter.h"

#include "../WTSTools/WTSBaseDataMgr.h"

NS_WTP_BEGIN
class WTSVariant;
NS_WTP_END


class WtLatencyTool
{
public:
	WtLatencyTool();
	~WtLatencyTool();

public:
	/*
	 *	≥ı ºªØ
	 */
	bool init();

	void run();

private:
	bool initParsers();
	bool initUftStrategies();

	bool initEngine(WTSVariant* cfg);

private:
	TraderAdapterMgr	_traders;
	ParserAdapterMgr	_parsers;
	UftStrategyMgr		_uft_stra_mgr;

	WtUftEngine			_uft_engine;

	WTSBaseDataMgr		_bd_mgr;
};

