/*!
 * \file WtDataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDataManager.h"
#include "WtEngine.h"
#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/DLLHelper.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"


WTSDataFactory g_dataFact;

WtDataManager::WtDataManager()
	: _reader(NULL)
	, _engine(NULL)
	, _bars_cache(NULL)
	, _ticks_cache(NULL)
	, _rt_tick_map(NULL)
{
}


WtDataManager::~WtDataManager()
{
	if (_bars_cache)
		_bars_cache->release();

	if (_ticks_cache)
		_ticks_cache->release();

	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool WtDataManager::initStore(WTSVariant* cfg)
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
	if(hInst == NULL)
	{
		WTSLogger::error("数据存储模块%s加载失败", module.c_str());
		return false;
	}

	FuncCreateDataReader funcCreator = (FuncCreateDataReader)DLLHelper::get_symbol(hInst, "createDataReader");
	if(funcCreator == NULL)
	{
		WTSLogger::error("数据存储模块%s加载失败，没有找到正确的入口函数", module);
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader = funcCreator();
	if(_reader == NULL)
	{
		WTSLogger::error("数据存储模块%s实例创建失败", module);
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader->init(cfg, this);

	return true;
}

bool WtDataManager::init(WTSVariant* cfg, WtEngine* engine)
{
	_engine = engine;

	return initStore(cfg->get("store"));
}

void WtDataManager::on_all_bar_updated(uint32_t updateTime)
{
	if (_bar_notifies.empty())
		return;

	for (const NotifyItem& item : _bar_notifies)
	{
		_engine->on_bar(item._code.c_str(), item._period.c_str(), item._times, item._newBar);
	}

	_bar_notifies.clear();
}

IBaseDataMgr* WtDataManager::get_basedata_mgr()
{ 
	return _engine->get_basedata_mgr(); 
}

IHotMgr* WtDataManager::get_hot_mgr() 
{ 
	return _engine->get_hot_mgr(); 
}

uint32_t WtDataManager::get_date() 
{ 
	return _engine->get_date(); 
}

uint32_t WtDataManager::get_min_time()
{ 
	return _engine->get_min_time(); 
}

uint32_t WtDataManager::get_secs() 
{ 
	return _engine->get_secs(); 
}

void WtDataManager::reader_log(WTSLogLevel ll, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WTSLogger::log_direct(ll, fmt, args);
	va_end(args);
}

void WtDataManager::on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar)
{
	std::string key_pattern = StrUtil::printf("%s-%u", code, period);

	std::string speriod;
	uint32_t times = 1;
	switch (period)
	{
	case KP_Minute1:
		speriod = "m";
		times = 1;
		break;
	case KP_Minute5:
		speriod = "m";
		times = 5;
		break;
	default:
		speriod = "d";
		times = 1;
		break;
	}

	if(_subed_basic_bars.find(key_pattern) != _subed_basic_bars.end())
	{
		//如果是基础周期, 直接触发on_bar事件
		//_engine->on_bar(code, speriod.c_str(), times, newBar);
		//更新完K线以后, 统一通知交易引擎
		_bar_notifies.push_back(NotifyItem({ code, speriod, times, newBar }));
	}

	//然后再处理非基础周期
	if (_bars_cache == NULL || _bars_cache->size() == 0)
		return;
	
	WTSSessionInfo* sInfo = _engine->get_session_info(code, true);

	for (auto it = _bars_cache->begin(); it != _bars_cache->end(); it++)
	{
		const std::string& key = it->first;
		if(!StrUtil::startsWith(key, key_pattern, false))
			continue;

		WTSKlineData* kData = (WTSKlineData*)it->second;
		{
			g_dataFact.updateKlineData(kData, newBar, sInfo);
			if (kData->isClosed())
			{
				//如果基础周期K线的时间和自定义周期K线的时间一致, 说明K线关闭了
				//这里也要触发on_bar事件
				times *= kData->times();
				WTSBarStruct* lastBar = kData->at(-1);
				//_engine->on_bar(code, speriod.c_str(), times, lastBar);
				//更新完K线以后, 统一通知交易引擎
				_bar_notifies.push_back(NotifyItem({ code, speriod, times, lastBar }));
			}
		}
	}
}

void WtDataManager::handle_push_quote(const char* stdCode, WTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, newTick, true);

	if(_ticks_cache != NULL)
	{
		WTSHisTickData* tData = (WTSHisTickData*)_ticks_cache->get(stdCode);
		if (tData == NULL)
			return;

		if (tData->isValidOnly() && newTick->volumn() == 0)
			return;

		tData->appendTick(newTick->getTickStruct());
	}
}

WTSTickData* WtDataManager::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	WTSTickData* curTick = (WTSTickData*)_rt_tick_map->get(code);
	if (curTick == NULL)
		return NULL;

	curTick->retain();
	return curTick;
}

WTSHisTickData* WtDataManager::get_ticks(const char* code, uint32_t count)
{
	if (_ticks_cache == NULL)
		_ticks_cache = DataCacheMap::create();

	WTSHisTickData* tData = (WTSHisTickData*)_ticks_cache->get(code);
	if(tData != NULL && tData->size() >= count)
	{
		tData->retain();
		return tData;
	}
	else
	{
		tData = _reader->readTicks(code, count, 0, true);
		if (tData)
			_ticks_cache->add(code, tData, true);
	}


	return tData;
}

WTSTickSlice* WtDataManager::get_tick_slice(const char* code, uint32_t count)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readTickSlice(code, count);
}

/*
WTSKlineData* WtDataManager::get_bars(const char* code, WTSKlinePeriod period, uint32_t times, uint32_t count)
{
	WTSSessionInfo* sInfo = _engine->get_session_info(code, true);

	if (_bars_cache == NULL)
		_bars_cache = DataCacheMap::create();

	std::string key = StrUtil::printf("%s-%u-%u", code, period, times);
	WTSKlineData* kData = (WTSKlineData*)_bars_cache->get(key);
	//如果缓存里的K线条数大于请求的条数, 则直接返回
	if (kData != NULL && kData->size() >= count)
	{
		kData->retain();
		return kData;
	}

	{
		if (times == 1)
		{
			kData = _store->readBars(code, period, count);
		}
		else
		{
			uint32_t realCount = count*times + times;
			WTSKlineData* rawData = _store->readBars(code, period, realCount);
			if(rawData != NULL)
			{
				kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
				rawData->release();
			}
		}
		
		if(kData)
			_bars_cache->add(key, kData);
		return kData;
	}
}
*/

WTSKlineSlice* WtDataManager::get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count)
{
	if (_reader == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s-%u", stdCode, period);

	if (times == 1)
	{
		_subed_basic_bars.insert(key);

		return _reader->readKlineSlice(stdCode, period, count);
	}

	//只有非基础周期的会进到下面的步骤
	WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

	if (_bars_cache == NULL)
		_bars_cache = DataCacheMap::create();

	key = StrUtil::printf("%s-%u-%u", stdCode, period, times);

	WTSKlineData* kData = (WTSKlineData*)_bars_cache->get(key);
	//如果缓存里的K线条数大于请求的条数, 则直接返回
	if (kData == NULL || kData->size() < count)
	{
		uint32_t realCount = count*times + times;
		WTSKlineData* rawData = _reader->readBars(stdCode, period, realCount);
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

//WTSKlineSlice* WtDataManager::get_kline_slice(const char* code, WTSKlinePeriod period, uint32_t count)
//{
//	if (_store == NULL)
//		return NULL;
//
//	return _store->readKlineSlice(code, period, count);
//}