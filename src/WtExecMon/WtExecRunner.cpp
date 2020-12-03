#include "WtExecRunner.h"

#include "../WtCore/WtHelper.h"
#include "../WtCore/WtLocalExecuter.h"
#include "../WTSTools/WTSLogger.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/JsonToVariant.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/TimeUtils.hpp"

extern const char* getBinDir();

bool WtExecRunner::init(const char* logProfile /* = "log4cxx.prop" */)
{
	std::string path = WtHelper::getCWD() + logProfile;
	WTSLogger::init(path.c_str());

	WtHelper::setInstDir(getBinDir());
	return true;
}

bool WtExecRunner::config(const char* cfgFile)
{
	std::string json;
	StdFile::read_file_content(cfgFile, json);
	rj::Document document;
	document.Parse(json.c_str());

	_config = WTSVariant::createObject();
	jsonToVariant(document, _config);

	//基础数据文件
	WTSVariant* cfgBF = _config->get("basefiles");
	if (cfgBF->get("session"))
	{
		_bd_mgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("交易时间模板加载完成");
	}

	if (cfgBF->get("commodity"))
	{
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("品种列表加载完成");
	}

	if (cfgBF->get("contract"))
	{
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("合约列表加载完成");
	}

	if (cfgBF->get("holiday"))
	{
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("节假日模板加载完成");
	}


	//初始化数据管理
	initDataMgr();

	//初始化开平策略
	if (!initActionPolicy())
		return false;

	//初始化行情通道
	initParsers();

	//初始化交易通道
	initTraders();

	initExecuters();

	return true;
}


void WtExecRunner::run(bool bAsync /* = false */)
{
	_parsers.run();
	_traders.run();
}

bool WtExecRunner::initParsers()
{
	WTSVariant* cfg = _config->get("parsers");
	if (cfg == NULL)
		return false;

	uint32_t count = 0;
	for (uint32_t idx = 0; idx < cfg->size(); idx++)
	{
		WTSVariant* cfgItem = cfg->get(idx);
		if (!cfgItem->getBoolean("active"))
			continue;

		const char* id = cfgItem->getCString("id");

		ParserAdapterPtr adapter(new ParserAdapter);
		adapter->init(id, cfgItem, this, &_bd_mgr, &_hot_mgr);

		_parsers.addAdapter(id, adapter);

		count++;
	}

	WTSLogger::info("共加载%u个行情通道", count);

	return true;
}

bool WtExecRunner::initExecuters()
{
	WTSVariant* cfg = _config->get("executers");
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

		WtExecuterPtr executer(new WtLocalExecuter(&_exe_factory, id, &_data_mgr));
		executer->setStub(this);
		if (!executer->init(cfgItem))
			return false;

		TraderAdapterPtr trader = _traders.getAdapter(cfgItem->getCString("trader"));
		executer->setTrader(trader.get());
		trader->addSink(executer.get());

		_exe_mgr.add_executer(executer);

		count++;
	}

	WTSLogger::info("共加载%u个执行器", count);

	return true;
}

bool WtExecRunner::initTraders()
{
	WTSVariant* cfg = _config->get("traders");
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

	WTSLogger::info("共加载%u个交易通道", count);

	return true;
}

bool WtExecRunner::initDataMgr()
{
	WTSVariant* cfg = _config->get("data");
	if (cfg == NULL)
		return false;

	_data_mgr.init(cfg, this);

	WTSLogger::info("数据管理模块初始化完成");
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

	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(sid).c_str());
	if (cInfo == NULL)
		return NULL;

	return _bd_mgr.getSession(cInfo->getSession());
}

void WtExecRunner::handle_push_quote(WTSTickData* curTick, bool isHot /*= false*/)
{
	std::string stdCode = curTick->code();
	_data_mgr.handle_push_quote(stdCode.c_str(), curTick);
}

void WtExecRunner::release()
{
	WTSLogger::stop();
}


void WtExecRunner::setPosition(const char* stdCode, double targetPos)
{
	_exe_mgr.handle_pos_change(stdCode, targetPos);
}

bool WtExecRunner::initActionPolicy()
{
	const char* action_file = _config->getCString("bspolicy");
	if (strlen(action_file) <= 0)
		return false;

	bool ret = _act_policy.init(action_file);
	WTSLogger::info("开平策略模板初始化完成");
	return ret;
}

uint64_t WtExecRunner::get_real_time()
{
	return TimeUtils::makeTime(_data_mgr.get_date(), _data_mgr.get_raw_time() * 100000 + _data_mgr.get_secs());
}

WTSCommodityInfo* WtExecRunner::get_comm_info(const char* stdCode)
{
	return _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSSessionInfo* WtExecRunner::get_sess_info(const char* stdCode)
{
	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
	if (cInfo == NULL)
		return NULL;

	return _bd_mgr.getSession(cInfo->getSession());
}

uint32_t WtExecRunner::get_trading_day()
{
	return _data_mgr.get_trading_day();
}