/*!
 * /file WtUftRunner.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */
#include "UftLatencyTool.h"
#include "../WtUftCore/UftStraContext.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/IParserApi.h"
#include "../Includes/ITraderApi.h"
#include "../Includes/WTSContractInfo.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CpuHelper.hpp"


USING_NS_WTP;

void test_uft()
{
	uft::UftLatencyTool runner;
	runner.init();

	runner.run();
}

namespace uft
{
	inline double checkValid(double x)
	{
		return ((x == DBL_MAX || x == FLT_MAX) ? 0.0 : x);
	}


	inline uint32_t strToTime(const char* strTime)
	{
		static char str[10] = { 0 };
		const char *pos = strTime;
		int idx = 0;
		auto len = strlen(strTime);
		for (std::size_t i = 0; i < len; i++)
		{
			if (strTime[i] != ':')
			{
				str[idx] = strTime[i];
				idx++;
			}
		}
		str[idx] = '\0';

		return strtoul(str, NULL, 10);
	}

	class TestParser : public IParserApi
	{
	public:
		void	run(uint32_t times)
		{
			srand(time(NULL));
			TimeUtils::Ticker ticker;
			for (uint32_t i = 0; i < times; i++)
			{
				uint32_t actDate = 20220303;// strtoul("20220303", NULL, 10);
				uint32_t actTime = 100523 * 1000 + 500; //strToTime("10:05:23") * 1000 + 500;

				WTSContractInfo* contract = _bd_mgr->getContract("rb2205", "SHFE");
				if (contract == NULL)
					return;

				double x = rand();

				WTSCommodityInfo* pCommInfo = contract->getCommInfo();

				WTSTickData* tick = WTSTickData::create("rb2205");
				tick->setContractInfo(contract);

				WTSTickStruct& quote = tick->getTickStruct();
				wt_strcpy(quote.exchg, pCommInfo->getExchg());

				quote.action_date = actDate;
				quote.action_time = actTime;

				quote.price = x;
				quote.open = x;
				quote.high = x;
				quote.low = x;
				quote.total_volume = 0;
				quote.trading_date = 20220303;
				quote.settle_price = x;

				quote.open_interest = 0;

				quote.upper_limit = x;
				quote.lower_limit = x;

				quote.pre_close = x;
				quote.pre_settle = x;
				quote.pre_interest = 0;

				//委卖价格
				quote.ask_prices[0] = x;
				quote.ask_prices[1] = x;
				quote.ask_prices[2] = x;
				quote.ask_prices[3] = x;
				quote.ask_prices[4] = x;

				//委买价格
				quote.bid_prices[0] = x;
				quote.bid_prices[1] = x;
				quote.bid_prices[2] = x;
				quote.bid_prices[3] = x;
				quote.bid_prices[4] = x;

				//委卖量
				quote.ask_qty[0] = 0;
				quote.ask_qty[1] = 0;
				quote.ask_qty[2] = 0;
				quote.ask_qty[3] = 0;
				quote.ask_qty[4] = 0;

				//委买量
				quote.bid_qty[0] = 0;
				quote.bid_qty[1] = 0;
				quote.bid_qty[2] = 0;
				quote.bid_qty[3] = 0;
				quote.bid_qty[4] = 0;

				_parser_spi->handleQuote(tick, 0);
				tick->release();
			}
			auto total = ticker.nano_seconds();
			double t2t = total * 1.0 / times;
			WTSLogger::warn("%u ticks simulated in %.0f ns, UftEngine Innner Latency: %.3f ns", times, total*1.0, t2t);
		}

	public:
		virtual void registerSpi(IParserSpi* listener) override
		{
			_parser_spi = listener;
			_bd_mgr = listener->getBaseDataMgr();
		}

	private:
		IParserSpi*		_parser_spi;
		IBaseDataMgr*	_bd_mgr;
	};

	TestParser* theParser = NULL;

	class TestTrader : public ITraderApi
	{
	public:

		virtual void registerSpi(ITraderSpi* listener) override
		{
			_trader_spi = listener;
		}

		virtual bool makeEntrustID(char* buffer, int length) override
		{
			wt_strcpy(buffer, "123456");
			return true;
		}

		virtual int orderInsert(WTSEntrust* eutrust) override
		{
			//WTSLogger::debug_f("{}", __FUNCTION__);
			return 0;
		}

	private:
		ITraderSpi*	_trader_spi;
	};

	class TestStrategy : public UftStrategy
	{
	public:
		TestStrategy(const char* id) : UftStrategy(id) {}

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
			//WTSLogger::debug_f("{}", __FUNCTION__);
			//ctx->stra_enter_long("SHFE.rb2205", 2300, 1, 0);
			ctx->stra_enter_short("SHFE.rb2205", 2300, 1, 0);
		}
	};


	UftLatencyTool::UftLatencyTool()
	{
	}


	UftLatencyTool::~UftLatencyTool()
	{
	}

	bool UftLatencyTool::init()
	{
		WTSLogger::init("logcfg.yaml");

		std::string cfgFile = "config.yaml";

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

		_times = _config->getUInt32("times");
		WTSLogger::warn_f("{} ticks will be simulated", _times);

		_core = _config->getUInt32("core");
		WTSLogger::warn_f("Testing thread will be bind to core {}", _core);

		initEngine(_config->get("env"));
		initModules();
		initStrategies();

		_config->release();
		return true;
	}

	bool UftLatencyTool::initStrategies()
	{
		UftStraContext* ctx = new UftStraContext(&_engine, "stra");
		ctx->set_strategy(new TestStrategy("stra"));

		TraderAdapterPtr trader = _traders.getAdapter("trader");
		ctx->setTrader(trader.get());
		trader->addSink(ctx);

		_engine.addContext(UftContextPtr(ctx));

		return true;
	}

	bool UftLatencyTool::initEngine(WTSVariant* cfg)
	{
		WTSLogger::warn_f("Trading enviroment initialzied with engine: UFT");
		_engine.init(cfg, &_bd_mgr, NULL);
		_engine.set_adapter_mgr(&_traders);

		return true;
	}


	bool UftLatencyTool::initModules()
	{
		{
			theParser = new TestParser();
			ParserAdapterPtr adapter(new ParserAdapter);
			adapter->initExt("parser", theParser, &_engine, &_bd_mgr);
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

	void UftLatencyTool::run()
	{
		if (_core != 0)
		{
			if(!CpuHelper::bind_core(_core-1))
			{
				WTSLogger::error_f("Binding to core {} failed", _core);
			}
		}

		try
		{
			_parsers.run();
			_traders.run();

			_engine.run(true);

			theParser->run(_times);
		}
		catch (...)
		{
		}
	}
}