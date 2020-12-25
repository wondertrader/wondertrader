#include "../WtDtCore/ParserAdapter.h"
#include "../WtDtCore/DataManager.h"
#include "../WtDtCore/StateMonitor.h"
#include "../WtDtCore/UDPCaster.h"
#include "../WtDtCore/WtHelper.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/JsonToVariant.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../Share/StrUtil.hpp"

#include <boost/asio.hpp>

WTSBaseDataMgr	g_baseDataMgr;
WTSHotMgr		g_hotMgr;
boost::asio::io_service g_asyncIO;
StateMonitor	g_stateMon;
UDPCaster		g_udpCaster;
DataManager		g_dataMgr;

#ifdef _WIN32
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


void initDataMgr(WTSVariant* config)
{
	g_dataMgr.init(config, &g_baseDataMgr, &g_stateMon, &g_udpCaster);
}

void initParsers(WTSVariant* cfg)
{
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		std::string module = cfgItem->getCString("module");
		if (!StdFile::exists(module.c_str()))
		{
			module = WtHelper::get_module_dir();
			module += "parsers/";
			module += cfgItem->getCString("module");
		}
		DllHandle libParser = DLLHelper::load_library(module.c_str());
		if (libParser)
		{
			FuncCreateParser pFuncCreateParser = (FuncCreateParser)DLLHelper::get_symbol(libParser, "createParser");
			if (pFuncCreateParser == NULL)
			{
				WTSLogger::info("行情模块初始化失败,找不到入口函数createParser...");
			}

			FuncDeleteParser pFuncDeleteParser = (FuncDeleteParser)DLLHelper::get_symbol(libParser, "deleteParser");
			if (pFuncDeleteParser == NULL)
			{
				WTSLogger::info("行情模块初始化失败,找不到入口函数deleteParser...");
			}

			if (pFuncCreateParser && pFuncDeleteParser)
			{
				WTSParams* params = cfgItem->toParams();

				ParserAdapterPtr adapter(new ParserAdapter(&g_baseDataMgr, &g_dataMgr));
				adapter->initAdapter(params, pFuncCreateParser, pFuncDeleteParser);
				ParserAdapterMgr::addAdapter(adapter);
				params->release();
			}

		}
		else
		{
			WTSLogger::info("行情模块初始化失败,加载模块%s失败...", module.c_str());
		}
	}

	WTSLogger::info("一共加载%u个Parser", ParserAdapterMgr::size());
}

void initialize()
{
	WtHelper::set_module_dir(getBinDir());

	std::string json;
	StdFile::read_file_content("QFConfig.json", json);
	rj::Document document;
	document.Parse(json.c_str());

	WTSVariant* config = WTSVariant::createObject();
	jsonToVariant(document, config);

	const char* id = config->getCString("id");
	if (strlen(id) > 0)
	{
#ifdef _WIN32
		HANDLE hHandle = ::CreateEvent(NULL, TRUE, TRUE, id);
		DWORD dwErr = GetLastError();
		if (hHandle != NULL && dwErr == ERROR_ALREADY_EXISTS)
		{
			ExitProcess(0);
			return;
		}
#else
		WTSLogger::error("Single instance mode of QuoteFactory is not OK on linux yet.");
#endif
	}


	//加载市场信息
	WTSVariant* cfgBF = config->get("basefiles");
	if (cfgBF->get("session"))
	{
		g_baseDataMgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("交易时间模板加载完成");
	}

	if (cfgBF->get("commodity"))
	{
		g_baseDataMgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("品种列表加载完成");
	}

	if (cfgBF->get("contract"))
	{
		g_baseDataMgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("合约列表加载完成");
	}

	if (cfgBF->get("holiday"))
	{
		g_baseDataMgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("节假日模板加载完成");
	}

	if (cfgBF->get("hot"))
	{
		g_hotMgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("主力切换表加载完成");
	}

	g_udpCaster.init(config->get("broadcaster"), &g_baseDataMgr, &g_dataMgr);

	initDataMgr(config->get("writer"));

	g_stateMon.initialize("statemonitor.json", &g_baseDataMgr, &g_dataMgr);

	initParsers(config->get("parsers"));

	config->release();

	g_asyncIO.post([](){
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		g_stateMon.run();
	});
}

int main()
{
	WTSLogger::init();

#ifdef _WIN32
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

	_set_error_mode(_OUT_TO_STDERR);
	_set_abort_behavior(0, _WRITE_ABORT_MSG);

	g_dwMainThreadId = GetCurrentThreadId();
	SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);

	CMiniDumper::Enable("QuoteFactory.exe", true);
#endif

	initialize();

	boost::asio::io_service::work work(g_asyncIO);
	g_asyncIO.run();
}

