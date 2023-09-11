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
#include "WtDtRunner.h"
#include "WtHelper.h"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/DLLHelper.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"


WTSDataFactory g_dataFact;

WtDataManager::WtDataManager()
	: _bd_mgr(NULL)
	, _hot_mgr(NULL)
	, _runner(NULL)
	, _reader(NULL)
	, _rt_bars(NULL)
{
}


WtDataManager::~WtDataManager()
{
	for(auto& m : _bars_cache)
	{
		if (m.second._bars != NULL)
			m.second._bars->release();
	}
	_bars_cache.clear();
}

bool WtDataManager::initStore(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	std::string module = cfg->getCString("module");
	if (module.empty())
		module = "WtDataStorage";

	module = WtHelper::get_module_dir() + DLLHelper::wrap_module(module.c_str());
	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateRdmDtReader pFuncCreateReader = (FuncCreateRdmDtReader)DLLHelper::get_symbol(libParser, "createRdmDtReader");
		if (pFuncCreateReader == NULL)
		{
			WTSLogger::error("Initializing of random data reader failed: function createRdmDtReader not found...");
		}

		FuncDeleteRdmDtReader pFuncDeleteReader = (FuncDeleteRdmDtReader)DLLHelper::get_symbol(libParser, "deleteRdmDtReader");
		if (pFuncDeleteReader == NULL)
		{
			WTSLogger::error("Initializing of random data reader failed: function deleteRdmDtReader not found...");
		}

		if (pFuncCreateReader && pFuncDeleteReader)
		{
			_reader = pFuncCreateReader();
			_remover = pFuncDeleteReader;
		}

	}
	else
	{
		WTSLogger::error("Initializing of random data reader failed: loading module {} failed...", module);

	}

	_reader->init(cfg, this);
	return true;
}

bool WtDataManager::init(WTSVariant* cfg, WtDtRunner* runner)
{
	_runner = runner;
	if (_runner)
	{
		_bd_mgr = &_runner->getBaseDataMgr();
		_hot_mgr = &_runner->getHotMgr();
	}

	_align_by_section = cfg->getBoolean("align_by_section");

	WTSLogger::info("Resampled bars will be aligned by section: {}", _align_by_section ? "yes" : " no");

	return initStore(cfg->get("store"));
}

void WtDataManager::reader_log(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_raw(ll, message);
}

WTSTickSlice* WtDataManager::get_tick_slices_by_range(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readTickSliceByRange(stdCode, stime, etime);
}

WTSTickSlice* WtDataManager::get_tick_slice_by_date(const char* stdCode, uint32_t uDate /* = 0 */)
{
	return _reader->readTickSliceByDate(stdCode, uDate);
}

WTSOrdQueSlice* WtDataManager::get_order_queue_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readOrdQueSliceByRange(stdCode, stime, etime);
}

WTSOrdDtlSlice* WtDataManager::get_order_detail_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readOrdDtlSliceByRange(stdCode, stime, etime);
}

WTSTransSlice* WtDataManager::get_transaction_slice(const char* stdCode,uint64_t stime, uint64_t etime /* = 0 */)
{
	stime = stime * 100000;
	etime = etime * 100000;
	return _reader->readTransSliceByRange(stdCode, stime, etime);
}

WTSSessionInfo* WtDataManager::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _bd_mgr->getSession(sid);

	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(sid, _hot_mgr);
	WTSCommodityInfo* cInfo = _bd_mgr->getCommodity(codeInfo._exchg, codeInfo._product);
	if (cInfo == NULL)
		return NULL;

	return cInfo->getSessionInfo();
}

WTSKlineSlice* WtDataManager::get_skline_slice_by_date(const char* stdCode, uint32_t secs, uint32_t uDate /* = 0 */)
{
	std::string key = StrUtil::printf("%s-%u-s%u", stdCode, uDate, secs);

	//只有非基础周期的会进到下面的步骤
	WTSSessionInfo* sInfo = get_session_info(stdCode, true);
	BarCache& barCache = _bars_cache[key];
	barCache._period = KP_Tick;
	barCache._times = secs;
	if (barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		WTSTickSlice* ticks = _reader->readTickSliceByDate(stdCode, uDate);
		if (ticks != NULL)
		{
			WTSKlineData* kData = g_dataFact.extractKlineData(ticks, secs, sInfo, true);
			barCache._bars = kData;		
			ticks->release();
		}
		else
		{
			return NULL;
		}
	}
	
	if (barCache._bars == NULL)
		return NULL;

	WTSBarStruct* rtHead = barCache._bars->at(0);
	WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, KP_Tick, secs, rtHead, barCache._bars->size());
	return slice;
}

WTSKlineSlice* WtDataManager::get_kline_slice_by_date(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t uDate /* = 0 */)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	uint64_t stime = _bd_mgr->getBoundaryTime(codeInfo.stdCommID(), uDate, false, true);
	uint64_t etime = _bd_mgr->getBoundaryTime(codeInfo.stdCommID(), uDate, false, false);
	return get_kline_slice_by_range(stdCode, period, times, stime, etime);
}

WTSKlineSlice* WtDataManager::get_kline_slice_by_range(const char* stdCode, WTSKlinePeriod period, uint32_t times,uint64_t stime, uint64_t etime /* = 0 */)
{
	if (times == 1)
	{
		return _reader->readKlineSliceByRange(stdCode, period, stime, etime);
	}

	//只有非基础周期的会进到下面的步骤
	WTSSessionInfo* sInfo = get_session_info(stdCode, true);
	std::string key = StrUtil::printf("%s-%u-%u", stdCode, period, times);
	BarCache& barCache = _bars_cache[key];
	barCache._period = period;
	barCache._times = times;
	if(barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		WTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, UINT_MAX, 0);
		if (rawData != NULL)
		{
			WTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, false);
			barCache._bars = kData;

			//不管如何，都删除最后一条K线
			//不能通过闭合标记判断，因为读取的基础周期可能本身没有闭合
			if (barCache._bars->size() > 0)
			{
				auto& bars = barCache._bars->getDataRef();
				bars.erase(bars.begin() + bars.size() - 1, bars.end());
			}

			if (period == KP_DAY)
				barCache._last_bartime = kData->date(-1);
			else
			{
				uint64_t lasttime = kData->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}

			rawData->release();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//后面则增量更新
		WTSKlineSlice* rawData = _reader->readKlineSliceByRange(stdCode, period, barCache._last_bartime, 0);
		if (rawData != NULL)
		{
			for(int32_t idx = 0; idx < rawData->size(); idx ++)
			{
				uint64_t barTime = 0;
				if (period == KP_DAY)
					barTime = rawData->at(0)->date;
				else
					barTime = 199000000000 + rawData->at(0)->time;
				
				//只有时间上次记录的最后一条时间，才可以用于更新K线
				if(barTime <= barCache._last_bartime)
					continue;

				g_dataFact.updateKlineData(barCache._bars, rawData->at(idx), sInfo, _align_by_section);
			}

			//不管如何，都删除最后一条K线
			//不能通过闭合标记判断，因为读取的基础周期可能本身没有闭合
			if(barCache._bars->size() > 0)
			{
				auto& bars = barCache._bars->getDataRef();
				bars.erase(bars.begin() + bars.size() - 1, bars.end());
			}

			if (period == KP_DAY)
				barCache._last_bartime = barCache._bars->date(-1);
			else
			{
				uint64_t lasttime = barCache._bars->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}
			

			rawData->release();
		}
	}

	//最后到缓存中定位
	bool isDay = period == KP_DAY;
	uint32_t rDate, rTime, lDate, lTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);
	lDate = (uint32_t)(stime / 10000);
	lTime = (uint32_t)(stime % 10000);

	WTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	WTSBarStruct sBar;
	sBar.date = lDate;
	sBar.time = (lDate - 19900000) * 10000 + lTime;

	uint32_t eIdx, sIdx;
	auto& bars = barCache._bars->getDataRef();
	auto eit = std::lower_bound(bars.begin(), bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	if (eit == bars.end())
		eIdx = bars.size() - 1;
	else
	{
		if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
		{
			eit--;
		}

		eIdx = eit - bars.begin();
	}

	auto sit = std::lower_bound(bars.begin(), eit, sBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});
	sIdx = sit - bars.begin();
	uint32_t rtCnt = eIdx - sIdx + 1;
	WTSBarStruct* rtHead = barCache._bars->at(sIdx);
	WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}

WTSKlineSlice* WtDataManager::get_kline_slice_by_count(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime /* = 0 */)
{
	if (times == 1)
	{
		return _reader->readKlineSliceByCount(stdCode, period, count, etime);
	}

	//只有非基础周期的会进到下面的步骤
	WTSSessionInfo* sInfo = get_session_info(stdCode, true);
	std::string key = StrUtil::printf("%s-%u-%u", stdCode, period, times);
	BarCache& barCache = _bars_cache[key];
	barCache._period = period;
	barCache._times = times;

	const char* tag = PERIOD_NAME[period-KP_Tick];

	if (barCache._bars == NULL)
	{
		//第一次将全部数据缓存到内存中
		WTSLogger::info("Caching all {} bars of {}...", tag, stdCode);
		WTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, UINT_MAX, 0);
		if (rawData != NULL)
		{
			WTSLogger::info("Resampling {} {} bars by {}-TO-1 of {}...", rawData->size(), tag, times, stdCode);
			WTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			barCache._bars = kData;

			//如果不是日线，要考虑最后一条K线是否闭合的情况
			//这里采用保守的方案，如果本地时间大于最后一条K线的时间，则认为真正闭合了
			if (period != KP_DAY)
			{
				uint64_t last_bartime = 0;
				last_bartime = 199000000000 + kData->time(-1);

				uint64_t now = TimeUtils::getYYYYMMDDhhmmss() / 100;
				if (now <= last_bartime && barCache._bars->size() > 0)
				{
					auto& bars = barCache._bars->getDataRef();
					bars.erase(bars.begin() + bars.size() - 1, bars.end());
				}
			}


			if (period == KP_DAY)
				barCache._last_bartime = kData->date(-1);
			else
			{
				uint64_t lasttime = kData->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}

			rawData->release();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		//后面则增量更新
		WTSKlineSlice* rawData = _reader->readKlineSliceByRange(stdCode, period, barCache._last_bartime, 0);
		if (rawData != NULL)
		{
			WTSLogger::info("{} {} bars of {} updated, adding to cache...", rawData->size(), tag, stdCode);
			for (int32_t idx = 0; idx < rawData->size(); idx++)
			{
				uint64_t barTime = 0;
				if (period == KP_DAY)
					barTime = rawData->at(0)->date;
				else
					barTime = 199000000000 + rawData->at(0)->time;

				//只有时间上次记录的最后一条时间，才可以用于更新K线
				if (barTime <= barCache._last_bartime)
					continue;

				g_dataFact.updateKlineData(barCache._bars, rawData->at(idx), sInfo, _align_by_section);
			}

			//如果不是日线，要考虑最后一条K线是否闭合的情况
			//这里采用保守的方案，如果本地时间大于最后一条K线的时间，则认为真正闭合了
			if (period != KP_DAY)
			{
				uint64_t last_bartime = 0;
				last_bartime = 199000000000 + barCache._bars->time(-1);

				uint64_t now = TimeUtils::getYYYYMMDDhhmmss() / 100;
				if (now <= last_bartime && barCache._bars->size() > 0)
				{
					auto& bars = barCache._bars->getDataRef();
					bars.erase(bars.begin() + bars.size() - 1, bars.end());
				}
			}

			if (period == KP_DAY)
				barCache._last_bartime = barCache._bars->date(-1);
			else
			{
				uint64_t lasttime = barCache._bars->time(-1);
				barCache._last_bartime = 199000000000 + lasttime;
			}


			rawData->release();
		}
	}

	//最后到缓存中定位
	bool isDay = period == KP_DAY;
	uint32_t rDate, rTime;
	rDate = (uint32_t)(etime / 10000);
	rTime = (uint32_t)(etime % 10000);

	WTSBarStruct eBar;
	eBar.date = rDate;
	eBar.time = (rDate - 19900000) * 10000 + rTime;

	uint32_t eIdx, sIdx;
	auto& bars = barCache._bars->getDataRef();
	auto eit = std::lower_bound(bars.begin(), bars.end(), eBar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b) {
		if (isDay)
			return a.date < b.date;
		else
			return a.time < b.time;
	});


	if (eit == bars.end())
		eIdx = bars.size() - 1;
	else
	{
		if ((isDay && eit->date > eBar.date) || (!isDay && eit->time > eBar.time))
		{
			eit--;
		}

		eIdx = eit - bars.begin();
	}

	sIdx = (eIdx + 1 >= count) ? (eIdx + 1 - count) : 0;
	uint32_t rtCnt = eIdx - sIdx + 1;
	WTSBarStruct* rtHead = barCache._bars->at(sIdx);
	WTSKlineSlice* slice = WTSKlineSlice::create(stdCode, period, times, rtHead, rtCnt);
	return slice;
}

WTSTickSlice* WtDataManager::get_tick_slice_by_count(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	etime = etime * 100000;
	return _reader->readTickSliceByCount(stdCode, count, etime);
}

double WtDataManager::get_exright_factor(const char* stdCode, WTSCommodityInfo* commInfo /* = NULL */)
{
	if (commInfo == NULL)
		return 1.0;

	if (commInfo->isStock())
		return _reader->getAdjFactorByDate(stdCode, 0);
	else
	{
		const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
		if (strlen(ruleTag) > 0)
			return _hot_mgr->getRuleFactor(ruleTag, commInfo->getFullPid(), 0);
	}

	return 1.0;
}

void WtDataManager::subscribe_bar(const char* stdCode, WTSKlinePeriod period, uint32_t times)
{
	std::string key = fmtutil::format("{}-{}-{}", stdCode, (uint32_t)period, times);

	uint32_t curDate = TimeUtils::getCurDate();
	uint64_t etime = (uint64_t)curDate * 10000 + 2359;

	if (times == 1)
	{
		WTSKlineSlice* slice = _reader->readKlineSliceByCount(stdCode, period, 10, etime);
		if (slice == NULL)
			return;

		WTSKlineData* kline = WTSKlineData::create(stdCode, slice->size());
		kline->setPeriod(period);
		uint32_t offset = 0;
		for(uint32_t blkIdx = 0; blkIdx < slice->get_block_counts(); blkIdx++)
		{
			memcpy(kline->getDataRef().data() + offset, slice->get_block_addr(blkIdx), sizeof(WTSBarStruct)*slice->get_block_size(blkIdx));
			offset += slice->get_block_size(blkIdx);
		}
		
		{
			StdUniqueLock lock(_mtx_rtbars);
			if (_rt_bars == NULL)
				_rt_bars = RtBarMap::create();

			_rt_bars->add(key, kline, false);
		}

		slice->release();
	}
	else
	{
		//只有非基础周期的会进到下面的步骤
		WTSSessionInfo* sInfo = get_session_info(stdCode, true);
		WTSKlineSlice* rawData = _reader->readKlineSliceByCount(stdCode, period, 10*times, 0);
		if (rawData != NULL)
		{
			WTSKlineData* kData = g_dataFact.extractKlineData(rawData, period, times, sInfo, true);
			{
				StdUniqueLock lock(_mtx_rtbars);
				if (_rt_bars == NULL)
					_rt_bars = RtBarMap::create();
				_rt_bars->add(key, kData, false);
			}
			rawData->release();
		}
	}

	WTSLogger::info("Realtime bar {} has subscribed", key);
}

void WtDataManager::clear_subbed_bars()
{
	StdUniqueLock lock(_mtx_rtbars);
	if (_rt_bars)
		_rt_bars->clear();
}

void WtDataManager::update_bars(const char* stdCode, WTSTickData* newTick)
{
	if (_rt_bars == NULL)
		return;

	StdUniqueLock lock(_mtx_rtbars);
	auto it = _rt_bars->begin();
	for(; it != _rt_bars->end(); it++)
	{
		WTSKlineData* kData = (WTSKlineData*)it->second;
		if (strcmp(kData->code(), stdCode) != 0)
			continue;

		WTSSessionInfo* sInfo = NULL;
		if (newTick->getContractInfo())
			sInfo = newTick->getContractInfo()->getCommInfo()->getSessionInfo();
		else
			sInfo = get_session_info(kData->code(), true);
		g_dataFact.updateKlineData(kData, newTick, sInfo, _align_by_section);
		WTSBarStruct* lastBar = kData->at(-1);

		std::string speriod;
		uint32_t times = kData->times();
		switch (kData->period())
		{
		case KP_Minute1:
			speriod = fmtutil::format("m{}", times);
			break;
		case KP_Minute5:
			speriod = fmtutil::format("m{}", times*5);
			break;
		default:
			speriod = fmtutil::format("d{}", times);
			break;
		}

		_runner->trigger_bar(stdCode, speriod.c_str(), lastBar);
	}
}

void WtDataManager::clear_cache()
{
	if (_reader == NULL)
	{
		WTSLogger::warn("DataReader not initialized, clearing canceled");
		return;
	}

	_reader->clearCache();
	WTSLogger::warn("All cache cleared");
}