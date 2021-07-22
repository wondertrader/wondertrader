/*!
 * \file WtDtRunner.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtRunner.h"
#include "../WtDtCore/WtHelper.h"

#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/DLLHelper.hpp"
#include "../Share/JsonToVariant.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/SignalHook.hpp"


WtDtRunner::WtDtRunner():_data_store(NULL)
{
	install_signal_hooks([](const char* message) {
		WTSLogger::error(message);
	});
}


WtDtRunner::~WtDtRunner()
{
}

void WtDtRunner::initialize(const char* cfgFile, bool isFile /* = true */, const char* modDir /* = "" */)
{
	WTSLogger::init();
	WtHelper::set_module_dir(modDir);

	std::string json;
	if (isFile)
		StdFile::read_file_content(cfgFile, json);
	else
		json = cfgFile;

	if(json.empty())
	{
		throw std::runtime_error("configuration file not exists");
		return;
	}

	rj::Document document;
	document.Parse(json.c_str());

	WTSVariant* config = WTSVariant::createObject();
	jsonToVariant(document, config);

	//基础数据文件
	WTSVariant* cfgBF = config->get("basefiles");
	if (cfgBF->get("session"))
	{
		m_baseDataMgr.loadSessions(cfgBF->getCString("session"));
		WTSLogger::info("Trading sessions loaded");
	}

	if (cfgBF->get("commodity"))
	{
		m_baseDataMgr.loadCommodities(cfgBF->getCString("commodity"));
		WTSLogger::info("Commodities loaded");
	}

	if (cfgBF->get("contract"))
	{
		m_baseDataMgr.loadContracts(cfgBF->getCString("contract"));
		WTSLogger::info("Contracts loades");
	}

	if (cfgBF->get("holiday"))
	{
		m_baseDataMgr.loadHolidays(cfgBF->getCString("holiday"));
		WTSLogger::info("Holidays loaded");
	}

	if (cfgBF->get("hot"))
	{
		m_hotMgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		m_hotMgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::info("Second rules loaded");
	}

	initDataMgr(config->get("data"));

	config->release();
}

void WtDtRunner::initDataMgr(WTSVariant* config)
{
	if (config == NULL)
		return;

	_data_mgr.init(config, this);

	WTSLogger::info("Data manager initialized");
}

WTSKlineSlice* WtDtRunner::get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	std::string basePeriod = "";
	uint32_t times = 1;
	if (strlen(period) > 1)
	{
		basePeriod.append(period, 1);
		times = strtoul(period + 1, NULL, 10);
	}
	else
	{
		basePeriod = period;
	}

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (strcmp(basePeriod.c_str(), "m") == 0)
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime/100000;
	}

	return _data_mgr.get_kline_slice_by_range(stdCode, kp, realTimes, beginTime, endTime);
}

WTSArray* WtDtRunner::get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime /* = 0 */)
{
	if(endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime;
	}
	return _data_mgr.get_tick_slices_by_range(stdCode, beginTime, endTime);
}

WTSKlineSlice* WtDtRunner::get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime /* = 0 */)
{
	std::string basePeriod = "";
	uint32_t times = 1;
	if (strlen(period) > 1)
	{
		basePeriod.append(period, 1);
		times = strtoul(period + 1, NULL, 10);
	}
	else
	{
		basePeriod = period;
	}

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (strcmp(basePeriod.c_str(), "m") == 0)
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			realTimes /= 5;
		}
		else
		{
			kp = KP_Minute1;
		}
	}
	else
		kp = KP_DAY;

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime / 100000;
	}

	return _data_mgr.get_kline_slice_by_count(stdCode, kp, realTimes, count, endTime);
}

WTSArray* WtDtRunner::get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime /* = 0 */)
{
	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime;
	}
	return _data_mgr.get_tick_slices_by_count(stdCode, count, endTime);
}