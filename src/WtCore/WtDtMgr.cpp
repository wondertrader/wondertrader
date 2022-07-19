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
		WTSLogger::error("Loading data reader module {} failed", module.c_str());
		return false;
	}

	FuncCreateDataReader funcCreator = (FuncCreateDataReader)DLLHelper::get_symbol(hInst, "createDataReader");
	if(funcCreator == NULL)
	{
		WTSLogger::error("Loading data reader module {} failed, entrance function createDataReader not found", module.c_str());
		DLLHelper::free_library(hInst);
		return false;
	}

	_reader = funcCreator();
	if(_reader == NULL)
	{
		WTSLogger::error("Creating instance of data reader module {} failed", module.c_str());
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
		_engine->on_bar(item._code, item._period, item._times, item._newBar);
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
	std::string key_pattern = fmt::format("{}-{}", code, period);

	char speriod;
	uint32_t times = 1;
	switch (period)
	{
	case KP_Minute1:
		speriod = 'm';
		times = 1;
		break;
	case KP_Minute5:
		speriod = 'm';
		times = 5;
		break;
	default:
		speriod = 'd';
		times = 1;
		break;
	}

	if(_subed_basic_bars.find(key_pattern) != _subed_basic_bars.end())
	{
		//����ǻ�������, ֱ�Ӵ���on_bar�¼�
		//_engine->on_bar(code, speriod.c_str(), times, newBar);
		//������K���Ժ�, ͳһ֪ͨ��������
		_bar_notifies.emplace_back(NotifyItem(code, speriod, times, newBar));
	}

	//Ȼ���ٴ���ǻ�������
	if (_bars_cache == NULL || _bars_cache->size() == 0)
		return;
	
	WTSSessionInfo* sInfo = _engine->get_session_info(code, true);

	for (auto it = _bars_cache->begin(); it != _bars_cache->end(); it++)
	{
		const char* key = it->first.c_str();
		if(memcmp(key, key_pattern.c_str(), key_pattern.size()) != 0)
			continue;

		WTSKlineData* kData = (WTSKlineData*)it->second;
		{
			g_dataFact.updateKlineData(kData, newBar, sInfo);
			if (kData->isClosed())
			{
				//�����������K�ߵ�ʱ����Զ�������K�ߵ�ʱ��һ��, ˵��K�߹ر���
				//����ҲҪ����on_bar�¼�
				WTSBarStruct* lastBar = kData->at(-1);
				//_engine->on_bar(code, speriod.c_str(), times, lastBar);
				//������K���Ժ�, ͳһ֪ͨ��������
				_bar_notifies.emplace_back(NotifyItem(code, speriod, times*kData->times(), lastBar));
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
	 *	����Ҫ���´���һ��
	 *	����ǲ���Ȩ����ǰ��Ȩ����ֱ�Ӷ�ȡ�ײ��ʵʱ���漴��
	 */
	auto len = strlen(stdCode);
	bool isHFQ = (stdCode[len - 1] == SUFFIX_HFQ);

	//���Ǻ�Ȩ������ֱ���õײ㻺��
	if(!isHFQ)
		return _reader->readTickSlice(stdCode, count, etime);

	//��ת�ɲ���+�ı�׼����
	std::string pureStdCode(stdCode, len - 1);

	if (_ticks_adjusted == NULL)
		_ticks_adjusted = DataCacheMap::create();

	//�������û�У�����������һ�»���
	auto it = _ticks_adjusted->find(pureStdCode);
	if (it == _ticks_adjusted->end())
	{
		//�ȶ�ȡȫ��tick����
		double factor = _engine->get_exright_factor(stdCode, NULL);
		WTSTickSlice* slice = _reader->readTickSlice(pureStdCode.c_str(), 999999, etime);
		std::vector<WTSTickStruct> ayTicks;
		ayTicks.resize(slice->size());
		std::size_t offset = 0;
		for (std::size_t bIdx = 0; bIdx < slice->get_block_counts(); bIdx++)
		{
			memcpy(&ayTicks[0] + offset, slice->get_block_addr(bIdx), slice->get_block_size(bIdx) * sizeof(WTSTickStruct));
			offset += slice->get_block_size(bIdx);
		}

		//�����������һ����Ȩ����
		for (WTSTickStruct& tick : ayTicks)
		{
			tick.price *= factor;
			tick.open *= factor;
			tick.high *= factor;
			tick.low *= factor;
		}

		//��ӵ�������
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

	//�Ƚ�ʱ��Ķ���
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

	//�����궨λ��tickʱ���Ŀ��ʱ���, ��ȫ������һ��
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

	//std::string key = StrUtil::printf("%s-%u", stdCode, period);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}-{}", stdCode, (uint32_t)period);

	if (times == 1)
	{
		_subed_basic_bars.insert(key);

		return _reader->readKlineSlice(stdCode, period, count, etime);
	}

	//ֻ�зǻ������ڵĻ��������Ĳ���
	WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

	if (_bars_cache == NULL)
		_bars_cache = DataCacheMap::create();

	//key = StrUtil::printf("%s-%u-%u", stdCode, period, times);
	fmtutil::format_to(key, "{}-{}-{}", stdCode, (uint32_t)period, times);

	WTSKlineData* kData = (WTSKlineData*)_bars_cache->get(key);
	//����������K�������������������, ��ֱ�ӷ���
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
			WTSLogger::debug("{} bars of {} resampled every {} bars: {} -> {}", 
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
