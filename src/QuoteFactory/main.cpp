#include "../WtDtCore/ParserAdapter.h"
#include "../WtDtCore/DataManager.h"
#include "../WtDtCore/StateMonitor.h"
#include "../WtDtCore/UDPCaster.h"
#include "../WtDtCore/WtHelper.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../Share/StrUtil.hpp"

#include "../WTSUtils/SignalHook.hpp"

WTSBaseDataMgr	g_baseDataMgr;
WTSHotMgr		g_hotMgr;
boost::asio::io_service g_asyncIO;
StateMonitor	g_stateMon;
UDPCaster		g_udpCaster;
DataManager		g_dataMgr;
ParserAdapterMgr g_parsers;

#ifdef _MSC_VER
#include "../Common/mdump.h"
DWORD g_dwMainThreadId = 0;
BOOL WINAPI ConsoleCtrlhandler(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
	{
		g_dataMgr.release();

		PostThreadMessage(g_dwMainThreadId, WM_QUIT, 0, 0);
	}
	break;
	}

	return TRUE;
}
#endif

const char* getBinDir()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}


void initDataMgr(WTSVariant* config, bool bAlldayMode = false)
{
	//如果是全天模式，则不传递状态机给DataManager
	g_dataMgr.init(config, &g_baseDataMgr, bAlldayMode ? NULL : &g_stateMon, &g_udpCaster);
}

void initParsers(WTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");
		// By Wesley @ 2021.12.14
		// 如果id为空，则生成自动id
		std::string realid = id;
		if (realid.empty())
		{
			static uint32_t auto_parserid = 1000;
			realid = StrUtil::printf("auto_parser_%u", auto_parserid++);
		}

		ParserAdapterPtr adapter(new ParserAdapter(&g_baseDataMgr, &g_dataMgr));
		adapter->init(realid.c_str(), cfgItem);
		g_parsers.addAdapter(realid.c_str(), adapter);
	}

	WTSLogger::info("%u market data parsers loaded in total", g_parsers.size());
}

void initialize()
{
	WtHelper::set_module_dir(getBinDir());

	std::string filename("QFConfig.json");
	if (!StdFile::exists(filename.c_str()))
		filename = "QFConfig.yaml";
	if (!StdFile::exists(filename.c_str()))
		filename = "dtcfg.json";
	if (!StdFile::exists(filename.c_str()))
		filename = "dtcfg.yaml";

	WTSVariant* config = WTSCfgLoader::load_from_file(filename.c_str(), true);
	if(config == NULL)
	{
		WTSLogger::error_f("Loading config file {} failed", filename);
		return;
	}

	//加载市场信息
	WTSVariant* cfgBF = config->get("basefiles");
	bool isUTF8 = cfgBF->getBoolean("utf-8");
	if (cfgBF->get("session"))
	{
		g_baseDataMgr.loadSessions(cfgBF->getCString("session"), isUTF8);
		WTSLogger::info("Trading sessions loaded");
	}

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			g_baseDataMgr.loadCommodities(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				g_baseDataMgr.loadCommodities(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			g_baseDataMgr.loadContracts(cfgItem->asCString(), isUTF8);
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				g_baseDataMgr.loadContracts(cfgItem->get(i)->asCString(), isUTF8);
			}
		}
	}

	if (cfgBF->get("holiday"))
	{
		g_baseDataMgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}

	//By Wesley @ 2021.12.27
	//datakit不需要主力映射规则
	//if (cfgBF->get("hot"))
	//{
	//	g_hotMgr.loadHots(cfgBF->getCString("hot"));
	//	WTSLogger::info("Hot rules loaded");
	//}

	//if (cfgBF->get("second"))
	//{
	//	g_hotMgr.loadSeconds(cfgBF->getCString("second"));
	//	WTSLogger::info("Second rules loaded");
	//}

	g_udpCaster.init(config->get("broadcaster"), &g_baseDataMgr, &g_dataMgr);

	//By Wesley @ 2021.12.27
	//全天候模式，不需要再使用状态机
	bool bAlldayMode = config->getBoolean("allday");
	if (!bAlldayMode)
	{
		g_stateMon.initialize(config->getCString("statemonitor"), &g_baseDataMgr, &g_dataMgr);
	}
	else
	{
		WTSLogger::info("QuoteFactory will run in allday mode");
	}
	initDataMgr(config->get("writer"), bAlldayMode);

	WTSVariant* cfgParser = config->get("parsers");
	if (cfgParser)
	{
		if (cfgParser->type() == WTSVariant::VT_String)
		{
			const char* filename = cfgParser->asCString();
			if (StdFile::exists(filename))
			{
				WTSLogger::info_f("Reading parser config from {}...", filename);
				WTSVariant* var = WTSCfgLoader::load_from_file(filename, isUTF8);
				if (var)
				{
					initParsers(var->get("parsers"));
					var->release();
				}
				else
				{
					WTSLogger::error_f("Loading parser config {} failed", filename);
				}
			}
			else
			{
				WTSLogger::error_f("Parser configuration {} not exists", filename);
			}
		}
		else if (cfgParser->type() == WTSVariant::VT_Array)
		{
			initParsers(cfgParser);
		}
	}

	config->release();

	g_asyncIO.post([bAlldayMode](){
		g_parsers.run();

		//全天候模式，不启动状态机
		if(!bAlldayMode)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			g_stateMon.run();
		}
	});
}

int main()
{
	std::string filename = "logcfgdt.json";
	if (!StdFile::exists(filename.c_str()))
		filename = "logcfgdt.yaml";

	WTSLogger::init(filename.c_str());

#ifdef _MSC_VER
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	_set_error_mode(_OUT_TO_STDERR);
	_set_abort_behavior(0, _WRITE_ABORT_MSG);

	g_dwMainThreadId = GetCurrentThreadId();
	SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);

	CMiniDumper::Enable("QuoteFactory.exe", true);
#endif

#if _WIN32
#pragma message("Signal hooks disabled in WIN32")
#else
#pragma message("Signal hooks enabled in UNIX")
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
#endif

	initialize();

	boost::asio::io_service::work work(g_asyncIO);
	g_asyncIO.run();
}

