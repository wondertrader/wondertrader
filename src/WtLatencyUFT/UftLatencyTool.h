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

namespace uft
{
	class UftLatencyTool
	{
	public:
		UftLatencyTool();
		~UftLatencyTool();

	public:
		/*
		 *	≥ı ºªØ
		 */
		bool init();

		void run();

	private:
		bool initModules();
		bool initStrategies();

		bool initEngine(WTSVariant* cfg);

	private:
		TraderAdapterMgr	_traders;
		ParserAdapterMgr	_parsers;
		UftStrategyMgr		_stra_mgr;

		WtUftEngine			_engine;

		WTSBaseDataMgr		_bd_mgr;

		uint32_t			_times;
		uint32_t			_core;
	};
}

