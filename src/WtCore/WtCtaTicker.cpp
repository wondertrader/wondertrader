/*!
 * \file WtCtaTicker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtCtaTicker.h"
#include "WtCtaEngine.h"
#include "../Includes/IDataReader.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSContractInfo.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

//////////////////////////////////////////////////////////////////////////
//WtTimeTicker
void WtCtaRtTicker::init(IDataReader* store, const char* sessionID)
{
	_store = store;
	_s_info = _engine->get_session_info(sessionID);
	if(_s_info == NULL)
		WTSLogger::fatal("Session {} is invalid, CtaTicker cannot run correctly", sessionID);
	else
		WTSLogger::info("CtaTicker will drive engine with session {}", sessionID);

	TimeUtils::getDateTime(_date, _time);
}

void WtCtaRtTicker::trigger_price(WTSTickData* curTick)
{
	if (_engine )
	{
		WTSContractInfo* cInfo = curTick->getContractInfo();
		std::string stdCode = curTick->code();
		_engine->on_tick(stdCode.c_str(), curTick);

		if (!cInfo->isFlat())
		{
			WTSTickData* hotTick = WTSTickData::create(curTick->getTickStruct());
			const char* hotCode = cInfo->getHotCode();
			hotTick->setCode(hotCode);
			_engine->on_tick(hotCode, hotTick);
			hotTick->release();
		}
	}
}

void WtCtaRtTicker::on_tick(WTSTickData* curTick)
{
	if (_thrd == NULL)
	{
		trigger_price(curTick);
		return;
	}

	uint32_t uDate = curTick->actiondate();
	uint32_t uTime = curTick->actiontime();

	if (_date != 0 && (uDate < _date || (uDate == _date && uTime < _time)))
	{
		//WTSLogger::info("行情时间{}小于本地时间{}", uTime, _time);
		trigger_price(curTick);
		return;
	}

	_date = uDate;
	_time = uTime;

	uint32_t curMin = _time / 100000;
	uint32_t curSec = _time % 100000;


	static uint32_t prevMin = UINT_MAX;
	static uint32_t minutes = UINT_MAX;
	static bool isSecEnd = false;
	static uint32_t wrapMin = UINT_MAX;

	//By Wesley @ 2023.11.01
	//如果新的分钟和上一次处理的分钟数不同，才进行处理
	//否则就不用处理，减少一些开销
	if(prevMin != curMin)
	{
		minutes = _s_info->timeToMinutes(curMin);
		isSecEnd = _s_info->isLastOfSection(curMin);
		prevMin = curMin;

		if (isSecEnd)
		{
			minutes--;
		}
		minutes++;

		wrapMin = _s_info->minuteToTime(minutes);
	}

	if (_cur_pos == 0)
	{
		//如果当前时间是0, 则直接赋值即可
		_cur_pos = minutes;
	}
	else if (_cur_pos < minutes)
	{
		//如果已记录的分钟小于新的分钟, 则需要触发闭合事件
		//这个时候要先触发闭合, 再修改平台时间和价格
		if (_last_emit_pos < _cur_pos)
		{
			//触发数据回放模块
			StdUniqueLock lock(_mtx);

			//优先修改时间标记
			_last_emit_pos = _cur_pos;

			uint32_t thisMin = _s_info->minuteToTime(_cur_pos);
			
			bool bEndingTDate = false;
			uint32_t offMin = _s_info->offsetTime(thisMin, true);
			if (offMin == _s_info->getCloseTime(true))
				bEndingTDate = true;

			WTSLogger::info("Minute Bar {}.{:04d} Closed by data", _date, thisMin);
			if (_store)
				_store->onMinuteEnd(_date, thisMin, bEndingTDate ? _engine->getTradingDate() : 0);

			//任务调度
			_engine->on_schedule(_date, thisMin);

			if(bEndingTDate)
				_engine->on_session_end();
		}

		//By Wesley @ 2022.02.09
		//这里先修改时间，再调用trigger_price
		//无论分钟线是否切换，先修改时间都是对的
		if (_engine)
		{
			_engine->set_date_time(_date, wrapMin, curSec, prevMin);
			_engine->set_trading_date(curTick->tradingdate());
		}
		trigger_price(curTick);

		_cur_pos = minutes;
	}
	else
	{
		//如果分钟数还是一致的, 则直接触发行情和时间即可
		trigger_price(curTick);
		if (_engine)
			_engine->set_date_time(_date, wrapMin, curSec, prevMin);
	}

	uint32_t sec = curSec / 1000;
	uint32_t msec = curSec % 1000;
	uint32_t left_ticks = (60 - sec) * 1000 - msec;
	_next_check_time = TimeUtils::getLocalTimeNow() + left_ticks;
}

void WtCtaRtTicker::run()
{
	if (_thrd)
		return;

	/*
	 *	By Wesley @ 2022.12.06
	 *	这里一定要在初始化之前把交易日确定下来
	 *	不然如果策略在on_init的时候调用一些依赖交易日的接口就会出错
	 */
	uint32_t curTDate = _engine->get_basedata_mgr()->calcTradingDate(_s_info->id(), _engine->get_date(), _engine->get_min_time(), true);
	_engine->set_trading_date(curTDate);
	WTSLogger::info("Trading date confirmed: {}", curTDate);
	_engine->on_init();
	_engine->on_session_begin();

	//先检查当前时间, 如果大于

	_thrd.reset(new StdThread([this](){
		while(!_stopped)
		{
			uint32_t offTime = _s_info->offsetTime(_engine->get_min_time(), true);

			if (_time != UINT_MAX && _s_info->isInTradingTime(_time / 100000, true))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				uint64_t now = TimeUtils::getLocalTimeNow();

				if (now >= _next_check_time && _last_emit_pos < _cur_pos)
				{
					//触发数据回放模块
					StdUniqueLock lock(_mtx);

					//优先修改时间标记
					_last_emit_pos = _cur_pos;

					uint32_t thisMin = _s_info->minuteToTime(_cur_pos);
					_time = thisMin*100000;//这里要还原成毫秒为单位

					//如果thisMin是0, 说明换日了
					//这里是本地计时导致的换日, 说明日期其实还是老日期, 要自动+1
					//同时因为时间是235959xxx, 所以也要手动置为0
					if (thisMin == 0)
					{
						uint32_t lastDate = _date;
						_date = TimeUtils::getNextDate(_date);
						_time = 0;
						WTSLogger::info("Data automatically changed at time 00:00: {} -> {}", lastDate, _date);
					}

					bool bEndingTDate = false;
					uint32_t offMin = _s_info->offsetTime(thisMin, true);
					if (offMin == _s_info->getCloseTime(true))
						bEndingTDate = true;

					WTSLogger::info("Minute bar {}.{:04d} closed automatically", _date, thisMin);
					if (_store)
						_store->onMinuteEnd(_date, thisMin, bEndingTDate ? _engine->getTradingDate() : 0);

					//任务调度
					_engine->on_schedule(_date, thisMin);

					if (bEndingTDate)
						_engine->on_session_end();

					//145959000
					if (_engine)
						_engine->set_date_time(_date, thisMin, 0);
				}
			}
			else //if(offTime >= _s_info->getOpenTime(true) && offTime <= _s_info->getCloseTime(true))
			{
				//收盘以后，如果发现上次触发的位置不等于总的分钟数，说明少了最后一分钟的闭合逻辑
				uint32_t total_mins = _s_info->getTradingMins();
				if(_time != UINT_MAX && _last_emit_pos != 0 && _last_emit_pos < total_mins && offTime >= _s_info->getCloseTime(true))
				{
					WTSLogger::warn("Tradingday {} will be ended forcely, last_emit_pos: {}, time: {}", _engine->getTradingDate(), _last_emit_pos.fetch_add(0), _time);

					//触发数据回放模块
					StdUniqueLock lock(_mtx);

					//优先修改时间标记
					_last_emit_pos = total_mins;

					bool bEndingTDate = true;
					uint32_t thisMin = _s_info->getCloseTime(false);
					uint32_t offMin = _s_info->getCloseTime(true);

					WTSLogger::info("Minute bar {}.{:04d} closed automatically", _date, thisMin);
					if (_store)
						_store->onMinuteEnd(_date, thisMin, _engine->getTradingDate());

					//任务调度
					_engine->on_schedule(_date, thisMin);

					_engine->on_session_end();

				}
				else
				{
					std::this_thread::sleep_for(std::chrono::seconds(10));
				}
			}
		}
	}));
}

void WtCtaRtTicker::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}

bool WtCtaRtTicker::is_in_trading() const 
{
	if (_s_info == NULL)
		return false;

	return _s_info->isInTradingTime(_time/100000, true);
}

uint32_t WtCtaRtTicker::time_to_mins(uint32_t uTime) const
{
	if (_s_info == NULL)
		return uTime;

	return _s_info->timeToMinutes(uTime, true);
}