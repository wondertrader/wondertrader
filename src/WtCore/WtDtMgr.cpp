/*!
 * \file WtDataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtMgr.h"
#include "WtEngine.h"
#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/CodeHelper.hpp"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"


WTSDataFactory g_dataFact;

WtDtMgr::WtDtMgr()
	: _reader(NULL)
	, _engine(NULL)
	, _loader(NULL)
	, _bars_cache(NULL)
	, _ticks_adjusted(NULL)
	, _rt_tick_map(NULL)
{
}


WtDtMgr::~WtDtMgr()
{
	if (_bars_cache)
		_bars_cache->release();

	if (_ticks_adjusted)
		_ticks_adjusted->release();

	if (_rt_tick_map)
		_rt_tick_map->release();
}

bool WtDtMgr::initStore(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	std::string module = cfg->getCString("module");
	if (module.empty())
		module = WtHelper::getInstDir() + DLLHelper::wrap_module("WtDataStorage");
	else
		module = WtHelper::getInstDir() + DLLHelper::wrap_module(module.c_str());

	DllHandle hInst = DLLHelper::load_library(module.c_str());
	if(hInst == NULL)
	{
		WTSLogger::error("Loading data reader module %s failed", module.c_str());
		return false;
	}

	FuncCreateDataReader funcCreator = (FuncCreateDataReader)DLLHelper::get_symbol(hInst, "createDataReader");
	if(funcCreator == NULL)
	{
		WTSLogger::error("Loading data reader module %s failed, entrance function createDataReader not found", module.c_str());
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader = funcCreator();
	if(_reader == NULL)
	{
		WTSLogger::error("Creating instance of data reader module %s failed", module.c_str());
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader->init(cfg, this, _loader);

	return true;
}

bool WtDtMgr::init(WTSVariant* cfg, WtEngine* engine)
{
	_engine = engine;

	return initStore(cfg->get("store"));
}

void WtDtMgr::on_all_bar_updated(uint32_t updateTime)
{
	if (_bar_notifies.empty())
		return;

	WTSLogger::debug("All bars updated, on_bar will be triggered");

	for (const NotifyItem& item : _bar_notifies)
	{
		_engine->on_bar(item._code.c_str(), item._period.c_str(), item._times, item._newBar);
	}

	_bar_notifies.clear();
}

IBaseDataMgr* WtDtMgr::get_basedata_mgr()
{ 
	return _engine->get_basedata_mgr(); 
}

IHotMgr* WtDtMgr::get_hot_mgr() 
{ 
	return _engine->get_hot_mgr(); 
}

uint32_t WtDtMgr::get_date() 
{ 
	return _engine->get_date(); 
}

uint32_t WtDtMgr::get_min_time()
{ 
	return _engine->get_min_time(); 
}

uint32_t WtDtMgr::get_secs() 
{ 
	return _engine->get_secs(); 
}

void WtDtMgr::reader_log(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_raw(ll, message);
}

void WtDtMgr::on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar)
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
		_bar_notifies.emplace_back(NotifyItem({ code, speriod, times, newBar }));
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
				WTSBarStruct* lastBar = kData->at(-1);
				//_engine->on_bar(code, speriod.c_str(), times, lastBar);
				//更新完K线以后, 统一通知交易引擎
				_bar_notifies.emplace_back(NotifyItem({ code, speriod, times*kData->times(), lastBar }));
			}
		}
	}
}

void WtDtMgr::handle_push_quote(const char* stdCode, WTSTickData* newTick)
{
	if (newTick == NULL)
		return;

	if (_rt_tick_map == NULL)
		_rt_tick_map = DataCacheMap::create();

	_rt_tick_map->add(stdCode, newTick, true);

	if(_ticks_adjusted != NULL)
	{
		WTSHisTickData* tData = (WTSHisTickData*)_ticks_adjusted->get(stdCode);
		if (tData == NULL)
			return;

		if (tData->isValidOnly() && newTick->volume() == 0)
			return;

		tData->appendTick(newTick->getTickStruct());
	}
}

WTSTickData* WtDtMgr::grab_last_tick(const char* code)
{
	if (_rt_tick_map == NULL)
		return NULL;

	WTSTickData* curTick = (WTSTickData*)_rt_tick_map->get(code);
	if (curTick == NULL)
		return NULL;

	curTick->retain();
	return curTick;
}

double WtDtMgr::get_adjusting_factor(const char* stdCode, uint32_t uDate)
{
	if (_reader)
		return _reader->getAdjFactorByDate(stdCode, uDate);

	return 1.0;
}

WTSTickSlice* WtDtMgr::get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_reader == NULL)
		return NULL;

	/*
	 *	By Wesley @ 2022.02.11
	 *	这里要重新处理一下
	 *	如果是不复权或者前复权，则直接读取底层的实时缓存即可
	 */
	auto len = strlen(stdCode);
	bool isHFQ = (stdCode[len - 1] == SUFFIX_HFQ);

	//不是后复权，缓存直接用底层缓存
	if(!isHFQ)
		return _reader->readTickSlice(stdCode, count, etime);

	//先转成不带+的标准代码
	std::string pureStdCode(stdCode, len - 1);

	if (_ticks_adjusted == NULL)
		_ticks_adjusted = DataCacheMap::create();

	//如果缓存没有，先重新生成一下缓存
	auto it = _ticks_adjusted->find(pureStdCode);
	if (it == _ticks_adjusted->end())
	{
		//先读取全部tick数据
		double factor = get_adjusting_factor(pureStdCode.c_str(), get_date());
		WTSTickSlice* slice = _reader->readTickSlice(pureStdCode.c_str(), 999999, etime);
		std::vector<WTSTickStruct> ayTicks;
		ayTicks.resize(slice->size());
		std::size_t offset = 0;
		for (std::size_t bIdx = 0; bIdx < slice->get_block_counts(); bIdx++)
		{
			memcpy(&ayTicks[0] + offset, slice->get_block_addr(bIdx), slice->get_block_size(bIdx) * sizeof(WTSTickStruct));
			offset += slice->get_block_size(bIdx);
		}

		//缓存的数据做一个复权处理
		for (WTSTickStruct& tick : ayTicks)
		{
			tick.price *= factor;
			tick.open *= factor;
			tick.high *= factor;
			tick.low *= factor;
		}

		//添加到缓存中
		WTSHisTickData* hisTick = WTSHisTickData::create(stdCode, false, factor);
		hisTick->getDataRef().swap(ayTicks);
		_ticks_adjusted->add(pureStdCode, hisTick, false);
	}

	WTSHisTickData* hisTick = (WTSHisTickData*)_ticks_adjusted->get(pureStdCode);
	uint32_t curDate, curTime, curSecs;
	if (etime == 0)
	{
		curDate = get_date();
		curTime = get_min_time();
		curSecs = get_secs();

		etime = (uint64_t)curDate * 1000000000 + curTime * 100000 + curSecs;
	}
	else
	{
		//20190807124533900
		curDate = (uint32_t)(etime / 1000000000);
		curTime = (uint32_t)(etime % 1000000000) / 100000;
		curSecs = (uint32_t)(etime % 100000);
	}

	//比较时间的对象
	WTSTickStruct eTick;
	eTick.action_date = curDate;
	eTick.action_time = curTime * 100000 + curSecs;

	auto& ticks = hisTick->getDataRef();

	WTSTickStruct* pTick = std::lower_bound(&ticks.front(), &ticks.back(), eTick, [](const WTSTickStruct& a, const WTSTickStruct& b) {
		if (a.action_date != b.action_date)
			return a.action_date < b.action_date;
		else
			return a.action_time < b.action_time;
	});

	uint32_t eIdx = pTick - &ticks.front();

	//如果光标定位的tick时间比目标时间打, 则全部回退一个
	if (pTick->action_date > eTick.action_date || pTick->action_time > eTick.action_time)
	{
		pTick--;
		eIdx--;
	}

	uint32_t cnt = min(eIdx + 1, count);
	uint32_t sIdx = eIdx + 1 - cnt;
	WTSTickSlice* slice = WTSTickSlice::create(stdCode, &ticks.front() + sIdx, cnt);
	return slice;
}

WTSOrdQueSlice* WtDtMgr::get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readOrdQueSlice(stdCode, count, etime);
}

WTSOrdDtlSlice* WtDtMgr::get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readOrdDtlSlice(stdCode, count, etime);
}

WTSTransSlice* WtDtMgr::get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_reader == NULL)
		return NULL;

	return _reader->readTransSlice(stdCode, count, etime);
}

WTSKlineSlice* WtDtMgr::get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_reader == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s-%u", stdCode, period);

	if (times == 1)
	{
		_subed_basic_bars.insert(key);

		return _reader->readKlineSlice(stdCode, period, count, etime);
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
		WTSKlineSlice* rawData = _reader->readKlineSlice(stdCode, period, realCount, etime);
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
		{
			_bars_cache->add(key, kData, false);
			WTSLogger::debug_f("{} bars of {} resampled every {} bars: {} -> {}", 
				PERIOD_NAME[period], stdCode, times, realCount, kData->size());
		}
	}

	int32_t sIdx = 0;
	uint32_t rtCnt = min(kData->size(), count);
	sIdx = kData->size() - rtCnt;
	WTSBarStruct* rtHead = kData->at(sIdx);
	WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}
