/*!
 * \file HisDataReplayer.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "HisDataReplayer.h"

#include <fstream>

#include "../Share/WTSVariant.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/WTSSessionInfo.hpp"

#include "../Share/BoostFile.hpp"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSDataFactory.h"
#include "../WTSTools/WTSCmpHelper.hpp"

#include "../Share/JsonToVariant.hpp"
#include "../Share/CodeHelper.hpp"

uint64_t readFileContent(const char* filename, std::string& content)
{
	FILE* f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	uint32_t length = ftell(f);
	content.resize(length);   // allocate memory for a buffer of appropriate dimension
	fseek(f, 0, 0);
	fread((void*)content.data(), sizeof(char), length, f);
	fclose(f);
	return length;
}


HisDataReplayer::HisDataReplayer()
	: _listener(NULL)
	, _cur_date(0)
	, _cur_time(0)
	, _cur_secs(0)
	, _cur_tdate(0)
	, _tick_enabled(true)
	, _opened_tdate(0)
	, _closed_tdate(0)
{
}


HisDataReplayer::~HisDataReplayer()
{
}


bool HisDataReplayer::init(WTSVariant* cfg)
{
	_mode = cfg->getCString("mode");
	_base_dir = StrUtil::standardisePath(cfg->getCString("path"));

	_begin_time = cfg->getUInt64("stime");
	_end_time = cfg->getUInt64("etime");

	_tick_enabled = cfg->getBoolean("tick");

	//基础数据文件
	WTSVariant* cfgBF = cfg->get("basefiles");
	if (cfgBF->get("session"))
		_bd_mgr.loadSessions(cfgBF->getCString("session"));

	if (cfgBF->get("commodity"))
		_bd_mgr.loadCommodities(cfgBF->getCString("commodity"));

	if (cfgBF->get("contract"))
		_bd_mgr.loadContracts(cfgBF->getCString("contract"));

	if (cfgBF->get("holiday"))
		_bd_mgr.loadHolidays(cfgBF->getCString("holiday"));

	if (cfgBF->get("hot"))
		_hot_mgr.loadHots(cfgBF->getCString("hot"));

	loadFees(cfg->getCString("fees"));

	return true;
}

void HisDataReplayer::run()
{
	_cur_date = (uint32_t)(_begin_time / 10000);
	_cur_time = (uint32_t)(_begin_time % 10000);
	_cur_secs = 0;
	_cur_tdate = _bd_mgr.calcTradingDate(DEFAULT_SESSIONID, _cur_date, _cur_time, true);

	_listener->handle_init();	

	BarsList& barList = _bars_cache[_main_key];
	WTSSessionInfo* sInfo = get_session_info(barList._code.c_str(), true);
	std::string commId = CodeHelper::stdCodeToStdCommID(barList._code.c_str());

	WTSLogger::info("开始数据回放……");

	for (;;)
	{
		bool isDay = barList._period == KP_DAY;
		if(barList._cursor != UINT_MAX)
		{
			uint64_t nextBarTime = 0;
			if(isDay)
				nextBarTime = (uint64_t)barList._bars[barList._cursor].date * 10000 + sInfo->getCloseTime();
			else
			{
				nextBarTime = (uint64_t)barList._bars[barList._cursor].time;
				nextBarTime +=199000000000;
			}

			if (nextBarTime > _end_time)
			{
				WTSLogger::info("%s超过结束时间%s，回放结束", StrUtil::fmtUInt64(nextBarTime).c_str(), StrUtil::fmtUInt64(_end_time).c_str());
				break;
			}

			uint32_t nextDate = (uint32_t)(nextBarTime / 10000);
			uint32_t nextTime = (uint32_t)(nextBarTime % 10000);

			uint32_t nextTDate = _bd_mgr.calcTradingDate(commId.c_str(), nextDate, nextTime, false);
			if (_opened_tdate != nextTDate)
			{
				_listener->handle_session_begin();
				_opened_tdate = nextTDate;
				WTSLogger::info("交易日%u开始", nextTDate);
				_cur_tdate = nextTDate;
			}
			;
			if(_tick_enabled)
			{
				uint64_t curBarTime = (uint64_t)_cur_date * 10000 + _cur_time;
				replayTicks(curBarTime, nextBarTime);
			}

			_cur_date = nextDate;
			_cur_time = nextTime;
			_cur_secs = 0;

			uint32_t offTime = sInfo->offsetTime(_cur_time);
			bool isEndTDate = (offTime >= sInfo->getCloseTime(true));

			onMinuteEnd(nextDate, nextTime, (isDay || isEndTDate) ? nextTDate : 0);			
			
			if (isEndTDate && _closed_tdate != _cur_tdate)
			{
				_listener->handle_session_end();
				_closed_tdate = _cur_tdate;
				_day_cache.clear();
				WTSLogger::info("交易日%u结束", _cur_tdate);
			}

			if (barList._cursor >= barList._bars.size())
			{
				WTSLogger::info("全部数据都已回放，回放结束");
				break;
			}
		}
		else
		{
			WTSLogger::info("数据尚未初始化，回放直接退出");
			break;
		}
	}

	if (_closed_tdate != _cur_tdate)
	{
		_listener->handle_session_end();
	}
}

void HisDataReplayer::replayTicks(uint64_t stime, uint64_t etime)
{
	for (;;)
	{
		uint64_t nextTime = UINT64_MAX;
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			if(!checkTicks(stdCode.c_str(), _cur_tdate))
				continue;

			TickList& tickList = _ticks_cache[stdCode];
			if (tickList._cursor == UINT_MAX)
			{
				uint32_t uDate = (uint32_t)(stime / 10000);
				uint32_t uTime = (uint32_t)(stime % 10000);

				WTSTickStruct curTick;
				curTick.action_date = uDate;
				curTick.action_time = uTime * 100000;

				auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
					if (a.action_date != b.action_date)
						return a.action_date < b.action_date;
					else
						return a.action_time < b.action_time;
				});

				uint32_t idx = tit - tickList._ticks.begin();
				tickList._cursor = idx + 1;
			}

			if(tickList._cursor >= tickList._count)
				continue;

			const WTSTickStruct& nextTick = tickList._ticks[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;

			nextTime = min(lastTime, nextTime);
		}

		if (nextTime/100000 >= etime)
			break;

		_cur_date = (uint32_t)(nextTime / 1000000000);
		_cur_time = nextTime % 1000000000 / 100000;
		_cur_secs = nextTime % 100000;
		for (auto v : _tick_sub_map)
		{
			std::string stdCode = v.first;
			TickList& tickList = _ticks_cache[stdCode];
			WTSTickStruct& nextTick = tickList._ticks[tickList._cursor - 1];
			uint64_t lastTime = (uint64_t)nextTick.action_date * 1000000000 + nextTick.action_time;
			if(lastTime <= nextTime)
			{
				WTSTickData* newTick = WTSTickData::create(nextTick);
				newTick->setCode(stdCode.c_str());
				_listener->handle_tick(stdCode.c_str(), newTick);
				newTick->release();

				tickList._cursor++;
			}
		}
	}
}

void HisDataReplayer::onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate /* = 0 */)
{
	//这里应该触发检查
	uint64_t nowTime = (uint64_t)uDate * 10000 + uTime;

	for (auto it = _bars_cache.begin(); it != _bars_cache.end(); it++)
	{
		BarsList& barsList = it->second;
		if (barsList._period != KP_DAY)
		{
			//如果历史数据指标不在尾部, 说明是回测模式, 要继续回放历史数据
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					uint64_t barTime = 199000000000 + nextBar.time;
					if (barTime <= nowTime)
					{
						if (!_tick_enabled)
						{
							const std::string& ticker = _ticker_keys[barsList._code];
							if (ticker == it->first)
							{
								//开高低收
								WTSTickStruct& curTS = _day_cache[barsList._code];
								strcpy(curTS.code, barsList._code.c_str());
								curTS.action_date = _cur_date;
								curTS.action_time = _cur_time * 100000;

								curTS.price = nextBar.open;
								curTS.volumn = nextBar.vol;

								//更新开高低三个字段
								if (decimal::eq(curTS.open, 0))
									curTS.open = curTS.price;
								curTS.high = max(curTS.price, curTS.high);
								if (decimal::eq(curTS.low, 0))
									curTS.low = curTS.price;
								else
									curTS.low = min(curTS.price, curTS.low);
								
								
								WTSTickData* curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.high;
								curTS.volumn = nextBar.vol;
								curTS.high = max(curTS.price, curTS.high);
								curTS.low = min(curTS.price, curTS.low);
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.low;
								curTS.high = max(curTS.price, curTS.high);
								curTS.low = min(curTS.price, curTS.low);
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.close;
								curTS.high = max(curTS.price, curTS.high);
								curTS.low = min(curTS.price, curTS.low);
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
							}
						}

						uint32_t times = barsList._times;
						if (barsList._period == KP_Minute5)
							times *= 5;
						_listener->handle_bar_close(barsList._code.c_str(), "m", times, &nextBar);
					}
					else
					{
						break;
					}

					barsList._cursor++;

					if (barsList._cursor == barsList._bars.size())
						break;
				}
			}
		}
		else
		{
			if (barsList._bars.size() > barsList._cursor)
			{
				for (;;)
				{
					WTSBarStruct& nextBar = barsList._bars[barsList._cursor];

					if (nextBar.date <= endTDate)
					{
						if (!_tick_enabled)
						{
							const std::string& ticker = _ticker_keys[barsList._code];
							if (ticker == it->first)
							{
								WTSSessionInfo* sInfo = get_session_info(barsList._code.c_str(), true);
								uint32_t curTime = sInfo->getOpenTime();
								//开高低收
								WTSTickStruct curTS;
								strcpy(curTS.code, barsList._code.c_str());
								curTS.action_date = _cur_date;
								curTS.action_time = curTime * 100000;

								curTS.price = nextBar.open;
								curTS.volumn = nextBar.vol;
								WTSTickData* curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.high;
								curTS.volumn = nextBar.vol;
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.low;
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
								curTick->release();

								curTS.price = nextBar.close;
								curTick = WTSTickData::create(curTS);
								_listener->handle_tick(barsList._code.c_str(), curTick);
							}
						}

						_listener->handle_bar_close(barsList._code.c_str(), "d", barsList._times, &nextBar);
					}
					else
					{
						break;
					}

					barsList._cursor++;

					if (barsList._cursor >= barsList._bars.size())
						break;
				}
			}
		}
	}

	if (_listener)
		_listener->handle_schedule(uDate, uTime);
}

WTSKlineSlice* HisDataReplayer::get_kline_slice(const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, bool isMain /* = false */)
{
	std::string key = StrUtil::printf("%s#%s#%u", stdCode, period, times);
	bool isStk = CodeHelper::isStdStkCode(stdCode);
	if (isMain)
		_main_key = key;

	//if(!_tick_enabled)
	//不做判断，主要为了防止没有tick数据，而采用第二方案
	{
		if(_ticker_keys.find(stdCode) == _ticker_keys.end())
			_ticker_keys[stdCode] = key;
		else
		{
			std::string oldKey = _ticker_keys[stdCode];
			oldKey = oldKey.substr(strlen(stdCode) + 1);
			if (strcmp(period, "m") == 0 && oldKey.at(0) == 'd')
			{
				_ticker_keys[stdCode] = key;
			}
			else if (oldKey.at(0) == period[0] && times < strtoul(oldKey.substr(2, 1).c_str(), NULL, 10))
			{
				_ticker_keys[stdCode] = key;
			}
		}
	}

	WTSKlinePeriod kp;
	uint32_t realTimes = times;
	if (strcmp(period, "m") == 0)
	{
		if(times % 5 == 0)
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

	bool isDay = kp == KP_DAY;

	auto it = _bars_cache.find(key);
	bool bHasHisData = false;
	if (it == _bars_cache.end())
	{
		if (realTimes != 1)
		{
			std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, 1);
			if (_bars_cache.find(rawKey) == _bars_cache.end())
			{
				if (_mode == "csv")
				{
					bHasHisData = cacheRawBarsFromCSV(rawKey, stdCode, kp);
				}
				else
				{
					bHasHisData = cacheRawBars(rawKey, stdCode, kp);
				}
			}
		}
		else
		{
			if (_mode == "csv")
			{
				bHasHisData = cacheRawBarsFromCSV(key, stdCode, kp);
			}
			else
			{
				bHasHisData = cacheRawBars(key, stdCode, kp);
			}
		}
	}
	else
	{
		bHasHisData = true;
	}

	if (!bHasHisData)
		return NULL;

	if (realTimes != 1 && it == _bars_cache.end())
	{
		WTSSessionInfo* sInfo = get_session_info(stdCode, true);

		std::string rawKey = StrUtil::printf("%s#%s#%u", stdCode, period, 1);
		const BarsList& rawBars = _bars_cache[rawKey];

		WTSKlineData* rawKline = WTSKlineData::create(stdCode, rawBars._bars.size());
		rawKline->setCode(stdCode);
		rawKline->setPeriod(kp);
		rawKline->setClosed(true);
		memcpy(rawKline->getDataRef().data(), rawBars._bars.data(), sizeof(WTSBarStruct)*rawBars._bars.size());

		static WTSDataFactory dataFact;
		WTSKlineData* kData = dataFact.extractKlineData(rawKline, kp, realTimes, sInfo, true);
		rawKline->release();

		if(kData)
		{
			BarsList& barsList = _bars_cache[key];
			barsList._code = stdCode;
			barsList._period = kp;
			barsList._times = realTimes;
			barsList._count = kData->size();
			barsList._bars.swap(kData->getDataRef());
			kData->release();
		}
	}

	BarsList& kBlkPair = _bars_cache[key];
	if (kBlkPair._cursor == UINT_MAX)
	{
		//还没有经过初始定位
		WTSBarStruct bar;
		bar.date = _cur_tdate;
		if(kp != KP_DAY)
			bar.time = (_cur_date - 19900000) * 10000 + _cur_time;
		auto it = std::lower_bound(kBlkPair._bars.begin(), kBlkPair._bars.end(), bar, [isDay](const WTSBarStruct& a, const WTSBarStruct& b){
			if (isDay)
				return a.date < b.date;
			else
				return a.time < b.time;
		});

		uint32_t eIdx = it - kBlkPair._bars.begin();

		if (it != kBlkPair._bars.end())
		{
			WTSBarStruct& curBar = *it;
			if (isDay)
			{
				if (curBar.date >= _cur_tdate)
				{
					if (eIdx > 0)
					{
						it--;
						eIdx--;
					}

				}
			}
			else
			{
				if (curBar.time > bar.time)
				{
					if (eIdx > 0)
					{
						it--;
						eIdx--;
					}
					else
					{
						return NULL;
					}
				}
			}
		}
		else
		{

		}

		kBlkPair._cursor = eIdx + 1;
	}
	else
	{
		uint32_t curMin = (_cur_date - 19900000) * 10000 + _cur_time;
		if (isDay)
		{
			if (kBlkPair._cursor <= kBlkPair._count)
			{
				while (kBlkPair._bars[kBlkPair._cursor - 1].date < _cur_tdate  && kBlkPair._cursor < kBlkPair._count && kBlkPair._bars[kBlkPair._cursor].date < _cur_tdate)
				{
					kBlkPair._cursor++;
				}
			}
		}
		else
		{
			if (kBlkPair._cursor <= kBlkPair._count)
			{
				while (kBlkPair._bars[kBlkPair._cursor-1].time < curMin && kBlkPair._cursor < kBlkPair._count)
				{
					kBlkPair._cursor++;
				}
			}
		}
	}


	if (kBlkPair._cursor == 0)
		return NULL;

	uint32_t sIdx = 0;
	if (kBlkPair._cursor > count)
		sIdx = kBlkPair._cursor - count;

	uint32_t realCnt = kBlkPair._cursor - sIdx;
	WTSKlineSlice* kline = WTSKlineSlice::create(stdCode, kp, 1, kBlkPair._bars.data() + sIdx, realCnt);
	return kline;
}

WTSTickSlice* HisDataReplayer::get_tick_slice(const char* stdCode, uint32_t count)
{
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	TickList& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._ticks.begin();
		tickList._cursor = idx + 1;
	}

	uint32_t eIdx = tickList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSTickSlice* ticks = WTSTickSlice::create(stdCode, tickList._ticks.data() + sIdx, realCnt);
	return ticks;
}

WTSHisTickData* HisDataReplayer::get_ticks(const char* stdCode, uint32_t count, uint64_t etime/* = 0*/)
{
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	TickList& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;
		if(etime != 0)
		{
			uDate = (uint32_t)(etime / 10000);
			uTime = (uint32_t)(etime % 10000 * 100000);
		}

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._ticks.begin();
		tickList._cursor = idx + 1;
	}

	uint32_t eIdx = tickList._cursor - 1;
	uint32_t sIdx = 0;
	if (eIdx >= count - 1)
		sIdx = eIdx + 1 - count;

	uint32_t realCnt = eIdx - sIdx + 1;
	if (realCnt == 0)
		return NULL;

	WTSHisTickData* ticks = WTSHisTickData::create(stdCode, realCnt);
	memcpy(ticks->getDataRef().data(), tickList._ticks.data() + sIdx, sizeof(WTSTickStruct)*realCnt);
	return ticks;
}

bool HisDataReplayer::checkTicks(const char* stdCode, uint32_t uDate)
{
	bool bNeedCache = false;
	auto it = _ticks_cache.find(stdCode);
	if (it == _ticks_cache.end())
		bNeedCache = true;
	else
	{
		TickList& tickList = it->second;
		if (tickList._date != uDate)
			bNeedCache = true;
	}

	
	if (bNeedCache)
	{
		bool hasTicks = false;
		if (_mode == "csv")
		{
			hasTicks = cacheRawTicksFromCSV(stdCode, stdCode, uDate);
		}
		else
		{
			hasTicks = cacheRawTicks(stdCode, stdCode, uDate);
		}

		if (!hasTicks)
			return false;
	}

	return true;
}

WTSTickData* HisDataReplayer::get_last_tick(const char* stdCode)
{
	if (!checkTicks(stdCode, _cur_tdate))
		return NULL;

	TickList& tickList = _ticks_cache[stdCode];
	if (tickList._cursor == 0)
		return NULL;

	if (tickList._cursor == UINT_MAX)
	{
		uint32_t uDate = _cur_date;
		uint32_t uTime = _cur_time * 100000 + _cur_secs;

		WTSTickStruct curTick;
		curTick.action_date = uDate;
		curTick.action_time = uTime;

		auto tit = std::lower_bound(tickList._ticks.begin(), tickList._ticks.end(), curTick, [](const WTSTickStruct& a, const WTSTickStruct& b){
			if (a.action_date != b.action_date)
				return a.action_date < b.action_date;
			else
				return a.action_time < b.action_time;
		});

		uint32_t idx = tit - tickList._ticks.begin();
		tickList._cursor = idx + 1;
	}

	return WTSTickData::create(tickList._ticks[tickList._cursor - 1]);
}

WTSCommodityInfo* HisDataReplayer::get_commodity_info(const char* stdCode)
{
	return _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSSessionInfo* HisDataReplayer::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _bd_mgr.getSession(sid);

	WTSCommodityInfo* cInfo = _bd_mgr.getCommodity(CodeHelper::stdCodeToStdCommID(sid).c_str());
	if (cInfo == NULL)
		return NULL;

	return _bd_mgr.getSession(cInfo->getSession());
}

void HisDataReplayer::loadFees(const char* filename)
{
	if (strlen(filename) == 0)
		return;

	if (!StdFile::exists(filename))
	{
		WTSLogger::error("手续费模板文件%s不存在", filename);
		return;
	}


	std::string content;
	StdFile::read_file_content(filename, content);
	if (content.empty())
	{
		WTSLogger::error("手续费模板文件%s为空", filename);
		return;
	}

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
	{
		WTSLogger::error("手续费模板文件%s解析失败", filename);
		return;
	}

	WTSVariant* cfg = WTSVariant::createObject();
	if (!jsonToVariant(root, cfg))
	{
		WTSLogger::error("手续费模板文件%s转换失败", filename);
		return;
	}

	auto keys = cfg->memberNames();
	for (const std::string& key : keys)
	{
		WTSVariant* cfgItem = cfg->get(key.c_str());
		FeeItem& fItem = _fee_map[key];
		fItem._by_volumn = cfgItem->getBoolean("byvolumn");
		fItem._open = cfgItem->getDouble("open");
		fItem._close = cfgItem->getDouble("close");
		fItem._close_today = cfgItem->getDouble("closetoday");
	}

	cfg->release();

	WTSLogger::info("共加载%u条手续费模板", _fee_map.size());
}


double HisDataReplayer::calc_fee(const char* stdCode, double price, double qty, uint32_t offset)
{
	std::string stdPID = CodeHelper::stdCodeToStdCommID(stdCode);
	auto it = _fee_map.find(stdPID);
	if (it == _fee_map.end())
		return 0.0;

	double ret = 0.0;
	WTSCommodityInfo* commInfo = _bd_mgr.getCommodity(stdPID.c_str());
	const FeeItem& fItem = it->second;
	if (fItem._by_volumn)
	{
		switch (offset)
		{
		case 0: ret = fItem._open*qty; break;
		case 1: ret = fItem._close*qty; break;
		case 2: ret = fItem._close_today*qty; break;
		default: ret = 0.0; break;
		}
	}
	else
	{
		double amount = price*qty*commInfo->getVolScale();
		switch (offset)
		{
		case 0: ret = fItem._open*amount; break;
		case 1: ret = fItem._close*amount; break;
		case 2: ret = fItem._close_today*amount; break;
		default: ret = 0.0; break;
		}
	}

	return (int32_t)(ret * 100 + 0.5) / 100.0;
}

double HisDataReplayer::get_cur_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if (it == _price_map.end())
	{
		return 0.0;
	}
	else
	{
		return it->second;
	}
}

void HisDataReplayer::sub_tick(uint32_t sid, const char* stdCode)
{
	SIDSet& sids = _tick_sub_map[stdCode];
	sids.insert(sid);

	//如果是主力合约代码, 如SHFE.ag.HOT, 那么要转换成原合约代码, SHFE.ag.1912
	//因为执行器只识别原合约代码
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdFutCode(stdCode, cInfo);
		std::string rawCode = _hot_mgr.getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
		std::string stdRawCode = CodeHelper::bscFutCodeToStdCode(rawCode.c_str(), cInfo._exchg, false);
		_subed_raw_codes.insert(stdRawCode);
	}
	else
	{
		_subed_raw_codes.insert(stdCode);
	}
}

uint32_t strToTime(const char* strTime, bool bHasSec = false)
{
	std::string str;
	const char *pos = strTime;
	while (strlen(pos) > 0)
	{
		if (pos[0] != ':')
		{
			str.append(pos, 1);
		}
		pos++;
	}

	uint32_t ret = strtoul(str.c_str(), NULL, 10);
	if (ret > 10000 && !bHasSec)
		ret /= 100;

	return ret;
}

uint32_t strToDate(const char* strDate)
{
	StringVector ay = StrUtil::split(strDate, "/");
	if(ay.size() == 1)
		ay = StrUtil::split(strDate, "-");
	std::stringstream ss;
	if (ay.size() > 1)
	{
		auto pos = ay[2].find(" ");
		if (pos != std::string::npos)
			ay[2] = ay[2].substr(0, pos);
		ss << ay[0] << (ay[1].size() == 1 ? "0" : "") << ay[1] << (ay[2].size() == 1 ? "0" : "") << ay[2];
	}
	else
		ss << ay[0];

	return strtoul(ss.str().c_str(), NULL, 10);
}

bool HisDataReplayer::cacheRawTicks(const std::string& key, const char* stdCode, uint32_t uDate)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdFutCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);
	
	std::string rawCode = cInfo._code;
	if (cInfo._hot)
	{
		rawCode = _hot_mgr.getRawCode(cInfo._exchg, cInfo._product, uDate);
	}

	std::stringstream ss;
	ss << _base_dir << "his/ticks/" << cInfo._exchg << "/" << uDate << "/" << rawCode << ".dsb";

	std::string filename = ss.str();
	if (!StdFile::exists(filename.c_str()))
		return false;

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.size() < sizeof(HisTickBlock))
	{
		WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
		return false;
	}

	HisTickBlock* tBlock = (HisTickBlock*)content.c_str();
	HisTickBlockV2* tBlockV2 = NULL;
	if(tBlock->_version == BLOCK_VERSION_CMP)
	{
		//压缩版本，要重新检查文件大小
		tBlockV2 = (HisTickBlockV2*)content.c_str();

		if (content.size() != (sizeof(HisTickBlockV2) + tBlockV2->_size))
		{
			WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
			return false;
		}
	}

	TickList& ticksList = _ticks_cache[key];
	uint32_t tickcnt = 0;
	if (tBlockV2 == NULL)
	{
		tickcnt = (content.size() - sizeof(HisTickBlock)) / sizeof(WTSTickStruct);
		ticksList._ticks.resize(tickcnt);
		memcpy(ticksList._ticks.data(), tBlock->_ticks, sizeof(WTSTickStruct)*tickcnt);
	}
	else
	{
		//需要解压
		std::string buf = WTSCmpHelper::uncompress_data(tBlockV2->_data, (uint32_t)tBlockV2->_size);
		tickcnt = buf.size() / sizeof(WTSTickStruct);
		ticksList._ticks.resize(tickcnt);
		memcpy(ticksList._ticks.data(), buf.data(), buf.size());
	}	
	
	ticksList._cursor = UINT_MAX;
	ticksList._code = stdCode;
	ticksList._date = uDate;
	ticksList._count = tickcnt;

	return true;
}

bool HisDataReplayer::cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate)
{
	std::stringstream ss;
	ss << _base_dir << "bin/ticks/";
	std::string path = ss.str();
	if (!StdFile::exists(path.c_str()))
		boost::filesystem::create_directories(path.c_str());
	ss << stdCode << "_tick_" << uDate << ".dsb";
	std::string filename = ss.str();
	if (StdFile::exists(filename.c_str()))
	{
		//如果有格式化的历史数据文件, 则直接读取
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisTickBlockV2))
		{
			WTSLogger::error("历史Tick数据文件%s大小校验失败", filename.c_str());
			return false;
		}

		HisTickBlockV2* tBlock = (HisTickBlockV2*)content.data();
		std::string rawData = WTSCmpHelper::uncompress_data(tBlock->_data, (uint32_t)tBlock->_size);
		uint32_t tickcnt = rawData.size() / sizeof(WTSTickStruct);

		TickList& ticksList = _ticks_cache[key];
		ticksList._ticks.resize(tickcnt);
		memcpy(ticksList._ticks.data(), rawData.data(), rawData.size());
		ticksList._cursor = UINT_MAX;
		ticksList._code = stdCode;
		ticksList._date = uDate;
		ticksList._count = tickcnt;
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/ticks/" << stdCode << "_tick_" << uDate << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			WTSLogger::error("历史Tick数据文件不存在", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		WTSLogger::info("正在读取数据文件%s……", csvfile.c_str());

		char buffer[512];
		bool headerskipped = false;
		TickList& tickList = _ticks_cache[key];
		tickList._code = stdCode;
		tickList._date = uDate;
		while (!ifs.eof())
		{
			ifs.getline(buffer, 512);
			if (strlen(buffer) == 0)
				continue;

			//跳过头部
			if (!headerskipped)
			{
				headerskipped = true;
				continue;
			}

			//逐行读取
			StringVector ay = StrUtil::split(buffer, ",");
			WTSTickStruct ticks;
			ticks.action_date = strToDate(ay[0].c_str());
			ticks.action_time = strToTime(ay[1].c_str(), true) * 1000;
			ticks.price = strtod(ay[2].c_str(), NULL);
			ticks.volumn = strtoul(ay[3].c_str(), NULL, 10);
			tickList._ticks.push_back(ticks);

			if (tickList._ticks.size() % 1000 == 0)
			{
				WTSLogger::info("已读取数据%u条", tickList._ticks.size());
			}
		}
		tickList._count = tickList._ticks.size();
		ifs.close();
		WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), tickList._ticks.size());

		HisTickBlockV2 tBlock;
		strcpy(tBlock._blk_flag, BLK_FLAG);
		tBlock._type = BT_HIS_Ticks;
		tBlock._version = BLOCK_VERSION_CMP;
		
		std::string cmpData = WTSCmpHelper::compress_data(tickList._ticks.data(), sizeof(WTSTickStruct)*tickList._count);
		tBlock._size = cmpData.size();

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&tBlock, sizeof(HisTickBlockV2));
		}
		bf.write_file(cmpData);
		bf.close_file();
		WTSLogger::info("数据已转储至%s", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "m1"; break;
	case KP_Minute5: pname = "m5"; break;
	case KP_DAY: pname = "d"; break;
	default: pname = ""; break;
	}

	std::stringstream ss;
	ss << _base_dir << "bin/";
	if (!StdFile::exists(ss.str().c_str()))
		BoostFile::create_directories(ss.str().c_str());
	ss << stdCode << "_" << pname << ".dsb";
	std::string filename = ss.str();
	if (StdFile::exists(filename.c_str()))
	{
		//如果有格式化的历史数据文件, 则直接读取
		std::string content;
		StdFile::read_file_content(filename.c_str(), content);
		if (content.size() < sizeof(HisKlineBlockV2))
		{
			WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
			return false;
		}

		HisKlineBlockV2* kBlock = (HisKlineBlockV2*)content.c_str();
		std::string rawData = WTSCmpHelper::uncompress_data(kBlock->_data, (uint32_t)kBlock->_size);
		uint32_t barcnt = rawData.size() / sizeof(WTSBarStruct);

		BarsList& barList = _bars_cache[key];
		barList._bars.resize(barcnt);
		memcpy(barList._bars.data(), rawData.data(), rawData.size());
		barList._cursor = UINT_MAX;
		barList._code = stdCode;
		barList._period = period;
		barList._count = barcnt;
	}
	else
	{
		//如果没有格式化的历史数据文件, 则从csv加载
		std::stringstream ss;
		ss << _base_dir << "csv/" << stdCode << "_" << pname << ".csv";
		std::string csvfile = ss.str();

		if (!StdFile::exists(csvfile.c_str()))
		{
			WTSLogger::error("历史K线数据文件%s不存在", csvfile.c_str());
			return false;
		}

		std::ifstream ifs;
		ifs.open(csvfile.c_str());

		WTSLogger::info("正在读取数据文件%s……", csvfile.c_str());

		char buffer[512];
		bool headerskipped = false;
		BarsList& barList = _bars_cache[key];
		barList._code = stdCode;
		barList._period = period;
		while (!ifs.eof())
		{
			ifs.getline(buffer, 512);
			if (strlen(buffer) == 0)
				continue;

			//跳过头部
			if (!headerskipped)
			{
				headerskipped = true;
				continue;
			}

			//逐行读取
			StringVector ay = StrUtil::split(buffer, ",");
			WTSBarStruct bs;
			bs.date = strToDate(ay[0].c_str());
			if (period != KP_DAY)
				bs.time = TimeUtils::timeToMinBar(bs.date, strToTime(ay[1].c_str()));
			bs.open = strtod(ay[2].c_str(), NULL);
			bs.high = strtod(ay[3].c_str(), NULL);
			bs.low = strtod(ay[4].c_str(), NULL);
			bs.close = strtod(ay[5].c_str(), NULL);
			bs.vol = strtoul(ay[6].c_str(), NULL, 10);
			barList._bars.push_back(bs);

			if (barList._bars.size() % 1000 == 0)
			{
				WTSLogger::info("已读取数据%u条", barList._bars.size());
			}
		}
		barList._count = barList._bars.size();
		ifs.close();
		WTSLogger::info("数据文件%s全部读取完成, 共%u条", csvfile.c_str(), barList._bars.size());

		BlockType btype;
		switch (period)
		{
		case KP_Minute1: btype = BT_HIS_Minute1; break;
		case KP_Minute5: btype = BT_HIS_Minute5; break;
		default: btype = BT_HIS_Day; break;
		}

		HisKlineBlockV2 kBlock;
		strcpy(kBlock._blk_flag, BLK_FLAG);
		kBlock._type = btype;
		kBlock._version = BLOCK_VERSION_CMP;

		std::string cmpData = WTSCmpHelper::compress_data(barList._bars.data(), sizeof(WTSBarStruct)*barList._count);
		kBlock._size = cmpData.size();

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			bf.write_file(&kBlock, sizeof(HisKlineBlockV2));
		}
		bf.write_file(cmpData);
		bf.close_file();
		WTSLogger::info("数据已转储至%s", filename.c_str());
	}

	return true;
}

bool HisDataReplayer::cacheRawBars(const std::string& key, const char* stdCode, WTSKlinePeriod period)
{
	CodeHelper::CodeInfo cInfo;
	CodeHelper::extractStdFutCode(stdCode, cInfo);
	std::string stdPID = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	uint32_t curDate = TimeUtils::getCurDate();
	uint32_t curTime = TimeUtils::getCurMin() / 100;

	uint32_t endTDate = _bd_mgr.calcTradingDate(stdPID.c_str(), curDate, curTime, false);

	std::string pname;
	switch (period)
	{
	case KP_Minute1: pname = "min1"; break;
	case KP_Minute5: pname = "min5"; break;
	default: pname = "day"; break;
	}

	BarsList& barList = _bars_cache[key];
	barList._code = stdCode;
	barList._period = period;

	std::vector<std::vector<WTSBarStruct>*> barsSections;

	uint32_t realCnt = 0;
	if (cInfo._hot)
	{
		HotSections secs;
		if (!_hot_mgr.splitHotSecions(cInfo._exchg, cInfo._product, 19900102, endTDate, secs))
			return false;

		if (secs.empty())
			return false;

		//先按照HOT代码进行读取, 如rb.HOT
		std::vector<WTSBarStruct>* hotAy = NULL;
		uint32_t lastHotTime = 0;
		for (;;)
		{
			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._exchg << "." << cInfo._product << "_HOT.dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				break;

			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				break;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			uint32_t barcnt = 0;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					break;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					break;

				std::string rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), rawData.data(), rawData.size());
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					break;

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				hotAy = new std::vector<WTSBarStruct>();
				hotAy->resize(barcnt);
				memcpy(hotAy->data(), kBlock->_bars, sizeof(WTSBarStruct)*barcnt);
			}

			if (period != KP_DAY)
				lastHotTime = hotAy->at(barcnt - 1).time;
			else
				lastHotTime = hotAy->at(barcnt - 1).date;

			WTSLogger::info("主力合约%s历史%s数据直接缓存%u条", stdCode, pname.c_str(), barcnt);

			break;
		}

		bool bAllCovered = false;
		for (auto it = secs.rbegin(); it != secs.rend() && left > 0; it++)
		{
			//const char* curCode = it->first.c_str();
			//uint32_t rightDt = it->second.second;
			//uint32_t leftDt = it->second.first;
			const HotSection& hotSec = *it;
			const char* curCode = hotSec._code.c_str();
			uint32_t rightDt = hotSec._e_date;
			uint32_t leftDt = hotSec._s_date;

			//要先将日期转换为边界时间
			WTSBarStruct sBar, eBar;
			if (period != KP_DAY)
			{
				uint64_t sTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), leftDt, false, true);
				uint64_t eTime = _bd_mgr.getBoundaryTime(stdPID.c_str(), rightDt, false, false);

				sBar.date = leftDt;
				sBar.time = ((uint32_t)(sTime / 10000) - 19900000) * 10000 + (uint32_t)(sTime % 10000);

				if (sBar.time < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					sBar.time = lastHotTime + 1;
				}

				eBar.date = rightDt;
				eBar.time = ((uint32_t)(eTime / 10000) - 19900000) * 10000 + (uint32_t)(eTime % 10000);

				if (eBar.time <= lastHotTime)	//右边界时间小于最后一条Hot时间, 说明全部交叉了, 没有再找的必要了
					break;
			}
			else
			{
				sBar.date = leftDt;
				if (sBar.date < lastHotTime)	//如果边界时间小于主力的最后一根Bar的时间, 说明已经有交叉了, 则不需要再处理了
				{
					bAllCovered = true;
					sBar.date = lastHotTime + 1;
				}

				eBar.date = rightDt;

				if (eBar.date <= lastHotTime)
					break;
			}

			std::stringstream ss;
			ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << curCode << ".dsb";
			std::string filename = ss.str();
			if (!StdFile::exists(filename.c_str()))
				continue;

			{
				std::string content;
				StdFile::read_file_content(filename.c_str(), content);
				if (content.size() < sizeof(HisKlineBlock))
				{
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
				WTSBarStruct* firstBar = NULL;
				uint32_t barcnt = 0;
				std::string rawData;
				if (kBlock->_version == BLOCK_VERSION_CMP)
				{
					if (content.size() < sizeof(HisKlineBlockV2))
					{
						WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
						break;
					}

					HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
					if (kBlockV2->_size == 0)
						break;

					rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
					barcnt = rawData.size() / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						break;

					firstBar = (WTSBarStruct*)rawData.data();
				}
				else
				{
					barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
					if (barcnt <= 0)
						continue;

					firstBar = kBlock->_bars;
				}

				WTSBarStruct* pBar = std::lower_bound(firstBar, firstBar + (barcnt - 1), sBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
					if (period == KP_DAY)
					{
						return a.date < b.date;
					}
					else
					{
						return a.time < b.time;
					}
				});

				uint32_t sIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date < sBar.date) || (period != KP_DAY && pBar->time < sBar.time))	//早于边界时间
				{
					//早于边界时间, 说明没有数据了, 因为lower_bound会返回大于等于目标位置的数据
					continue;
				}

				pBar = std::lower_bound(firstBar + sIdx, firstBar + (barcnt - 1), eBar, [period](const WTSBarStruct& a, const WTSBarStruct& b){
					if (period == KP_DAY)
					{
						return a.date < b.date;
					}
					else
					{
						return a.time < b.time;
					}
				});
				uint32_t eIdx = pBar - firstBar;
				if ((period == KP_DAY && pBar->date > eBar.date) || (period != KP_DAY && pBar->time > eBar.time))
				{
					pBar--;
					eIdx--;
				}

				if (eIdx < sIdx)
					continue;

				uint32_t curCnt = eIdx - sIdx + 1;
				std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(curCnt);
				memcpy(tempAy->data(), &kBlock->_bars[sIdx], sizeof(WTSBarStruct)*curCnt);
				realCnt += curCnt;

				barsSections.push_back(tempAy);

				if (bAllCovered)
					break;
			}
		}

		if (hotAy)
		{
			barsSections.push_back(hotAy);
			realCnt += hotAy->size();
		}
	}
	else
	{
		//读取历史的
		std::stringstream ss;
		ss << _base_dir << "his/" << pname << "/" << cInfo._exchg << "/" << cInfo._code << ".dsb";
		std::string filename = ss.str();
		if (StdFile::exists(filename.c_str()))
		{
			//如果有格式化的历史数据文件, 则直接读取
			std::string content;
			StdFile::read_file_content(filename.c_str(), content);
			if (content.size() < sizeof(HisKlineBlock))
			{
				WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
				return false;
			}

			HisKlineBlock* kBlock = (HisKlineBlock*)content.c_str();
			WTSBarStruct* firstBar = NULL;
			uint32_t barcnt = 0;
			std::string rawData;
			if (kBlock->_version == BLOCK_VERSION_CMP)
			{
				if (content.size() < sizeof(HisKlineBlockV2))
				{
					WTSLogger::error("历史K线数据文件%s大小校验失败", filename.c_str());
					return false;
				}

				HisKlineBlockV2* kBlockV2 = (HisKlineBlockV2*)content.c_str();
				if (kBlockV2->_size == 0)
					return false;

				rawData = WTSCmpHelper::uncompress_data(kBlockV2->_data, (uint32_t)kBlockV2->_size);
				barcnt = rawData.size() / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = (WTSBarStruct*)rawData.data();
			}
			else
			{
				barcnt = (content.size() - sizeof(HisKlineBlock)) / sizeof(WTSBarStruct);
				if (barcnt <= 0)
					return false;

				firstBar = kBlock->_bars;
			}

			if (barcnt > 0)
			{

				uint32_t sIdx = 0;
				uint32_t idx = barcnt - 1;
				uint32_t curCnt = (idx - sIdx + 1);

				std::vector<WTSBarStruct>* tempAy = new std::vector<WTSBarStruct>();
				tempAy->resize(curCnt);
				memcpy(tempAy->data(), &firstBar[sIdx], sizeof(WTSBarStruct)*curCnt);
				realCnt += curCnt;

				barsSections.push_back(tempAy);
			}
		}
	}

	if (realCnt > 0)
	{
		barList._bars.resize(realCnt);

		uint32_t curIdx = 0;
		for (auto it = barsSections.rbegin(); it != barsSections.rend(); it++)
		{
			std::vector<WTSBarStruct>* tempAy = *it;
			memcpy(barList._bars.data() + curIdx, tempAy->data(), tempAy->size()*sizeof(WTSBarStruct));
			curIdx += tempAy->size();
			delete tempAy;
		}
		barList._count = barList._bars.size();
		barsSections.clear();
	}

	WTSLogger::info("合约%s的历史%s数据已缓存%u条", stdCode, pname.c_str(), realCnt);
	return true;
}