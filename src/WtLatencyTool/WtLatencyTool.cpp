/*!
 * /file WtUftRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "WtLatencyTool.h"
#include "../WtUftCore/UftStraContext.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/IParserApi.h"
#include "../Includes/ITraderApi.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"


USING_NS_WTP;

class TestParser: public IParserApi
{
public:
	void	run(uint32_t times)
	{
		WTSTickData* newTick = WTSTickData::create("rb2205");
		TimeUtils::Ticker ticker;
		for(uint32_t i = 0; i < times; i++)
		{
			WTSTickStruct& ts = newTick->getTickStruct();
			strcpy(ts.exchg, "SHFE");
			ts.trading_date = TimeUtils::getCurDate();
			TimeUtils::getDateTime(ts.action_date, ts.action_time);

			_parser_spi->handleQuote(newTick, 0);
		}
		auto total = ticker.nano_seconds();
		double t2t = total*1.0 / times;
		printf("%u ticks simulated in %.0f ns, Inner Tick-2-Trade: %.3f ns\r\n", times, total*1.0, t2t);
		newTick->release();
	}

public:
	virtual void registerSpi(IParserSpi* listener) override
	{
		_parser_spi = listener;
	}

private:
	IParserSpi* _parser_spi;
};

TestParser* theParser = NULL;

class TestTrader : public ITraderApi
{
public:

	virtual void registerSpi(ITraderSpi* listener) override
	{
		_trader_spi = listener;
	}

	virtual int orderInsert(WTSEntrust* eutrust) override
	{
		return 0;
	}

private:
	ITraderSpi*	_trader_spi;
};

class TestStrategy : public UftStrategy
{
public:
	TestStrategy(const char* id): UftStrategy(id){}

	/*
	*	执行单元名称
	*/
	virtual const char* getName() { return "TestStrategy"; }

	/*
	*	所属执行器工厂名称
	*/
	virtual const char* getFactName() { return "TestStrategyFact"; }


	virtual void on_init(IUftStraCtx* ctx) override
	{
		ctx->stra_sub_ticks("SHFE.rb2205");
	}

	virtual void on_tick(IUftStraCtx* ctx, const char* code, WTSTickData* newTick)
	{
		ctx->stra_enter_long("SHFE.rb2205", 2300, 1, 0);
	}
};


WtLatencyTool::WtLatencyTool()
{
}


WtLatencyTool::~WtLatencyTool()
{
}

bool WtLatencyTool::init()
{
	WTSLogger::init();

	std::string cfgFile = "config.json";
	if (!StdFile::exists(cfgFile.c_str()))
		cfgFile = "config.yaml";

	WTSVariant* _config = WTSCfgLoader::load_from_file(cfgFile, true);
	if (_config == NULL)
	{
		WTSLogger::error_f("Loading config file {} failed", cfgFile);
		return false;
	}

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	bool isUTF8 = cfgBF->getBoolean("utf-8");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"), isUTF8);

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	initEngine(_config->get("env"));
	initParsers();
	initUftStrategies();

	_config->release();
	return true;
}

bool WtLatencyTool::initUftStrategies()
{
	UftStraContext* ctx = new UftStraContext(&_uft_engine, "stra");
	ctx->set_strategy(new TestStrategy("stra"));

	TraderAdapterPtr trader = _traders.getAdapter("trader");
	ctx->setTrader(trader.get());
	trader->addSink(ctx);

	_uft_engine.addContext(UftContextPtr(ctx));

	return true;
}

bool WtLatencyTool::initEngine(WTSVariant* cfg)
{
	WTSLogger::info("Trading enviroment initialzied with engine: UFT");
	_uft_engine.init(cfg, &_bd_mgr, NULL);
	_uft_engine.set_adapter_mgr(&_traders);

	return true;
}


bool WtLatencyTool::initParsers()
{
	{
		theParser = new TestParser();
		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->initExt("parser", theParser, &_uft_engine, &_bd_mgr);
		_parsers.addAdapter("parser", adapter);
	}

	{
		TestTrader * tester = new TestTrader();
		TraderAdapterPtr adapter(new TraderAdapter());
		adapter->initExt("trader", tester, &_bd_mgr);
		_traders.addAdapter("trader", adapter);
	}

	return true;
}

void WtLatencyTool::run()
{
	try
	{
		_parsers.run();
		_traders.run();

		_uft_engine.run(true);

		theParser->run(10000000);
	}
	catch (...)
	{
	}
}
