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
#include "../WTSUtils/SignalHook.hpp"
#include "../Share/JsonToVariant.hpp"

#include "../WTSTools/WTSLogger.h"


WtDtRunner::WtDtRunner()
	: _data_store(NULL)
	, _is_inited(false)
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
	if(_is_inited)
	{
		WTSLogger::error("WtDtServo has already been initialized");
		return;
	}

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

	if (cfgBF->get("hot"))
	{
		_hot_mgr.loadHots(cfgBF->getCString("hot"));
		WTSLogger::info("Hot rules loaded");
	}

	if (cfgBF->get("second"))
	{
		_hot_mgr.loadSeconds(cfgBF->getCString("second"));
		WTSLogger::info("Second rules loaded");
	}

	initDataMgr(config->get("data"));

	config->release();

	_is_inited = true;
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
	if(!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

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
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

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
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

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
	if (!_is_inited)
	{
		WTSLogger::error("WtDtServo not initialized");
		return NULL;
	}

	if (endTime == 0)
	{
		uint32_t curDate, curTime;
		TimeUtils::getDateTime(curDate, curTime);
		endTime = (uint64_t)curDate * 10000 + curTime;
	}
	return _data_mgr.get_tick_slices_by_count(stdCode, count, endTime);
}