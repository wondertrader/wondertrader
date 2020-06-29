/*!
 * \file WtSimpDataMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "WtSimpDataMgr.h"
#include "WtExecRunner.h"
#include "../WtCore/WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/DLLHelper.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"

USING_NS_OTP;


WTSDataFactory g_dataFact;

WtSimpDataMgr::WtSimpDataMgr()
	: _reader(NULL)
	, _runner(NULL)
	, _bars_cache(NULL)
	, _rt_tick_map(NULL)
{
}


WtSimpDataMgr::~WtSimpDataMgr()
{
	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool WtSimpDataMgr::initStore(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	std::string module = cfg->getCString("module");
	if (module.empty())
	{
		module = WtHelper::getInstDir();
#ifdef _WIN32
		module += "WtDataReader.dll";
#else
		module += "libWtDataReader.so";
#endif
	}
	DllHandle hInst = DLLHelper::load_library(module.c_str());
	if (hInst == NULL)
	{
		WTSLogger::error("数据存储模块%s加载失败", module.c_str());
		return false;
	}

	FuncCreateDataReader funcCreator = (FuncCreateDataReader)DLLHelper::get_symbol(hInst, "createDataReader");
	if (funcCreator == NULL)
	{
		WTSLogger::error("数据存储模块%s加载失败，没有找到正确的入口函数", module);
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader = funcCreator();
	if (_reader == NULL)
	{
		WTSLogger::error("数据存储模块%s实例创建失败", module);
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader->init(cfg, this);

	return true;
}

bool WtSimpDataMgr::init(WTSVariant* cfg, WtExecRunner* runner)
{
	_runner = runner;
	return initStore(cfg->get("store"));
}

void WtSimpDataMgr::on_all_bar_updated(uint32_t updateTime)
{

}

IBaseDataMgr* WtSimpDataMgr::get_basedata_mgr()
{
	return _runner->get_bd_mgr();
}

IHotMgr* WtSimpDataMgr::get_hot_mgr()
{
	return _runner->get_hot_mgr();
}

uint32_t WtSimpDataMgr::get_date()
{
	return _cur_date;
}

uint32_t WtSimpDataMgr::get_min_time()
{
	return _cur_min_time;
}

uint32_t WtSimpDataMgr::get_secs()
{
	return _cur_secs;
}

void WtSimpDataMgr::reader_log(WTSLogLevel ll, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WTSLogger::log_direct(ll, fmt, args);
	va_end(args);
}

void WtSimpDataMgr::on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar)
{

}

void WtSimpDataMgr::handle_push_quote(const char* stdCode, WTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, newTick, true);
}

WTSTickData* WtSimpDataMgr::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	WTSTickData* curTick = (WTSTickData*)_rt_tick_map->get(code);
	if (curTick == NULL)
		return NULL;

	curTick->retain();
	return curTick;
}


WTSTickSlice* WtSimpDataMgr::get_tick_slice(const char* code, uint32_t count)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readTickSlice(code, count);
}


WTSKlineSlice* WtSimpDataMgr::get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count)
{
	if (_reader == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s-%u", stdCode, period);

	if (times == 1)
	{
		return _reader->readKlineSlice(stdCode, period, count);
	}

	//只有非基础周期的会进到下面的步骤
	WTSSessionInfo* sInfo = _runner->get_session_info(stdCode, true);

	if (_bars_cache == NULL)
		_bars_cache = DataCacheMap::create();

	key = StrUtil::printf("%s-%u-%u", stdCode, period, times);

	WTSKlineData* kData = (WTSKlineData*)_bars_cache->get(key);
	//如果缓存里的K线条数大于请求的条数, 则直接返回
	if (kData == NULL || kData->size() < count)
	{
		uint32_t realCount = count * times + times;
		WTSKlineSlice* rawData = _reader->readKlineSlice(stdCode, period, realCount);
		if (rawData != NULL)
		{
			kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			rawData->release();
		}
		else
		{
			return NULL;
		}

		if (kData)
			_bars_cache->add(key, kData, false);
	}

	int32_t sIdx = 0;
	uint32_t rtCnt = min(kData->size(), count);
	sIdx = kData->size() - rtCnt;
	WTSBarStruct* rtHead = kData->at(sIdx);
	WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, times, NULL, 0, rtHead, rtCnt);
	return slice;
}
