#include "WtExecRunner.h"

#include "../WtCore/WtHelper.h"
#include "../WtCore/WtDiffExecuter.h"
#include "../WtCore/WtDistExecuter.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/ModuleHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../WTSUtils/SignalHook.hpp"

#ifdef _MSC_VER
#include "../Common/mdump.h"
#include <boost/filesystem.hpp>
const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{

		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
	}

	return MODULE_NAME;
}
#endif

WtExecRunner::WtExecRunner()
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}

bool WtExecRunner::init(const char* logCfg /* = "logcfgexec.json" */, bool isFile /* = true */)
{
#ifdef _MSC_VER
	CMiniDumper::Enable(getModuleName(), true, WtHelper::getCWD().c_str());
#endif

	if(isFile)
	{
		std::string path = WtHelper::getCWD() + logCfg;
		WTSLogger::init(path.c_str(), true);
	}
	else
	{
		WTSLogger::init(logCfg, false);
	}
	

	WtHelper::setInstDir(getBinDir());
	return true;
}

bool WtExecRunner::config(const char* cfgFile, bool isFile /* = true */)
{
	_config = isFile ? WTSCfgLoader::load_from_file(cfgFile) : WTSCfgLoader::load_from_content(cfgFile, false);
	if(_config == NULL)
	{
		WTSLogger::log_raw(LL_ERROR, "Loading config file failed");
		return false;
	}

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("Trading sessions loaded");
	}

	WTSVariant* cfgItem = cfgBF->get("commodity");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadCommodities(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadCommodities(cfgItem->get(i)->asCString());
			}
		}
	}

	cfgItem = cfgBF->get("contract");
	if (cfgItem)
	{
		if (cfgItem->type() == WTSVariant::VT_String)
		{
			_bd_mgr.loadContracts(cfgItem->asCString());
		}
		else if (cfgItem->type() == WTSVariant::VT_Array)
		{
			for (uint32_t i = 0; i < cfgItem->size(); i++)
			{
				_bd_mgr.loadContracts(cfgItem->get(i)->asCString());
			}
		}
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}


	//初始化数据管理
	initDataMgr();

	//初始化开平策略
	if (!initActionPolicy())
		return false;

	//初始化行情通道
	const char* cfgParser = _config->getCString("parsers");
	if (StdFile::exists(cfgParser))
	{
		WTSLogger::info("Reading parser config from {}...", cfgParser);
		WTSVariant* var = WTSCfgLoader::load_from_file(cfgParser);
		if (var)
		{
			if (!initParsers(var))
				WTSLogger::error("Loading parsers failed");
			var->release();
		}
		else
		{
			WTSLogger::error("Loading parser config {} failed", cfgParser);
		}
	}

	//初始化交易通道
	const char* cfgTraders = _config->getCString("traders");
	if (StdFile::exists(cfgTraders))
	{
		WTSLogger::info("Reading trader config from {}...", cfgTraders);
		WTSVariant* var = WTSCfgLoader::load_from_file(cfgTraders);
		if (var)
		{
			if (!initTraders(var))
				WTSLogger::error("Loading traders failed");
			var->release();
		}
		else
		{
			WTSLogger::error("Loading trader config {} failed", cfgTraders);
		}
	}

	const char* cfgExecuters = _config->getCString("executers");
	if (StdFile::exists(cfgExecuters))
	{
		WTSLogger::info("Reading executer config from {}...", cfgExecuters);
		WTSVariant* var = WTSCfgLoader::load_from_file(cfgExecuters);
		if (var)
		{
			if (!initExecuters(var))
				WTSLogger::error("Loading executers failed");
			var->release();
		}
		else
		{
			WTSLogger::error("Loading executer config {} failed", cfgExecuters);
		}
	}

	return true;
}


void WtExecRunner::run()
{
	try
	{
		_parsers.run();
		_traders.run();
	}
	catch (...)
	{
		print_stack_trace([](const char* message) {
			WTSLogger::error(message);
		});
	}
}

bool WtExecRunner::initParsers(WTSVariant* cfgParser)
{
	WTSVariant* cfg = cfgParser->get("parsers");
	if (cfg == NULL)
		return false;

	uint32_t count = 0;
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

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(realid.c_str(), cfgItem, this, &_bd_mgr, &_hot_mgr);
		_parsers.addAdapter(realid.c_str(), adapter);

		count++;
	}

	WTSLogger::info("{} parsers loaded", count);

	return true;
}

bool WtExecRunner::initExecuters(WTSVariant* cfgExecuter)
{
	WTSVariant* cfg = cfgExecuter->get("executers");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	//先加载自带的执行器工厂
	std::string path = WtHelper::getInstDir() + "executer//";
	_exe_factory.loadFactories(path.c_str());

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");
		std::string name = cfgItem->getCString("name");	//local,diff,dist
		if (name.empty())
			name = "local";

		if (name == "local")
		{
			WtLocalExecuter* executer = new WtLocalExecuter(&_exe_factory, id, &_data_mgr);
			if (!executer->init(cfgItem))
				return false;

			const char* tid = cfgItem->getCString("trader");
			if (strlen(tid) == 0)
			{
				WTSLogger::error("No Trader configured for Executer {}", id);
			}
			else
			{
				TraderAdapterPtr trader = _traders.getAdapter(tid);
				if (trader)
				{
					executer->setTrader(trader.get());
					trader->addSink(executer);
				}
				else
				{
					WTSLogger::error("Trader {} not exists, cannot configured for executer %s", tid, id);
				}
			}

			executer->setStub(this);
			_exe_mgr.add_executer(ExecCmdPtr(executer));
		}
		else if (name == "diff")
		{
			WtDiffExecuter* executer = new WtDiffExecuter(&_exe_factory, id, &_data_mgr, &_bd_mgr);
			if (!executer->init(cfgItem))
				return false;

			const char* tid = cfgItem->getCString("trader");
			if (strlen(tid) == 0)
			{
				WTSLogger::error("No Trader configured for Executer {}", id);
			}
			else
			{
				TraderAdapterPtr trader = _traders.getAdapter(tid);
				if (trader)
				{
					executer->setTrader(trader.get());
					trader->addSink(executer);
				}
				else
				{
					WTSLogger::error("Trader {} not exists, cannot configured for executer %s", tid, id);
				}
			}

			executer->setStub(this);
			_exe_mgr.add_executer(ExecCmdPtr(executer));
		}
		else
		{
			WtDistExecuter* executer = new WtDistExecuter(id);
			if (!executer->init(cfgItem))
				return false;

			executer->setStub(this);
			_exe_mgr.add_executer(ExecCmdPtr(executer));
		}
		count++;
	}

	WTSLogger::info("{} executers loaded", count);

	return true;
}

bool WtExecRunner::initTraders(WTSVariant* cfgTrader)
{
	WTSVariant* cfg = cfgTrader->get("traders");
	if (cfg == NULL || cfg->type() != WTSVariant::VT_Array)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		TraderAdapterPtr adapter(new TraderAdapter);
		adapter->init(id, cfgItem, &_bd_mgr, &_act_policy);

		_traders.addAdapter(id, adapter);
		count++;
	}

	WTSLogger::info("{} traders loaded", count);

	return true;
}

bool WtExecRunner::initDataMgr()
{
	WTSVariant* cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, this);

	WTSLogger::info("Data Manager initialized");
	return true;
}

bool WtExecRunner::addExeFactories(const char* folder)
{
	return _exe_factory.loadFactories(folder);
}

WTSSessionInfo* WtExecRunner::get_session_info(const char* sid, bool isCode /* = true */)
{
	if (!isCode)
		return _bd_mgr.getSession(sid);

	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(sid, NULL);
	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(codeInfo._exchg, codeInfo._product);
	if (cInfo == NULL)
		return NULL;

	return cInfo->getSessionInfo();
}

void WtExecRunner::handle_push_quote(WTSTickData* quote)
{
	if (quote == NULL)
		return;

	uint32_t uDate = quote->actiondate();
	uint32_t uTime = quote->actiontime();
	uint32_t curMin = uTime / 100000;
	uint32_t curSec = uTime % 100000;
	WtHelper::setTime(uDate, curMin, curSec);
	WtHelper::setTDate(quote->tradingdate());

	_data_mgr.handle_push_quote(quote->code(), quote);

	_exe_mgr.handle_tick(quote->code(), quote);
}

void WtExecRunner::release()
{
	WTSLogger::stop();
}


void WtExecRunner::setPosition(const char* stdCode, double targetPos)
{
	_positions[stdCode] = targetPos;
}

void WtExecRunner::commitPositions()
{
	_exe_mgr.set_positions(_positions);
	_positions.clear();
}

bool WtExecRunner::initActionPolicy()
{
	const char* action_file = _config->getCString("bspolicy");
	if (strlen(action_file) <= 0)
		return false;

	bool ret = _act_policy.init(action_file);
	WTSLogger::info("Action policies initialized");
	return ret;
}

uint64_t WtExecRunner::get_real_time()
{
	return TimeUtils::makeTime(_data_mgr.get_date(), _data_mgr.get_raw_time() * 100000 + _data_mgr.get_secs());
}

WTSCommodityInfo* WtExecRunner::get_comm_info(const char* stdCode)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, NULL);
	return _bd_mgr.getCommodity(codeInfo._exchg, codeInfo._product);
}

WTSSessionInfo* WtExecRunner::get_sess_info(const char* stdCode)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, NULL);
	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(codeInfo._exchg, codeInfo._product);
	if (cInfo == NULL)
		return NULL;

	return cInfo->getSessionInfo();
}

uint32_t WtExecRunner::get_trading_day()
{
	return _data_mgr.get_trading_day();
}