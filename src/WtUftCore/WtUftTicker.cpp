/*!
 * \file WtHftTicker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtUftTicker.h"
#include "WtUftEngine.h"
#include "../Includes/IDataReader.h"

#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;


WtUftRtTicker::WtUftRtTicker(WtUftEngine* engine)
	: _engine(engine)
	, _stopped(false)
	, _date(0)
	, _time(UINT_MAX)
	, _next_check_time(0)
	, _last_emit_pos(0)
	, _cur_pos(0)
{
}


WtUftRtTicker::~WtUftRtTicker()
{
}

void WtUftRtTicker::init(const char* sessionID)
{
	_s_info = _engine->get_session_info(sessionID);

	TimeUtils::getDateTime(_date, _time);
}

void WtUftRtTicker::on_tick(WTSTickData* curTick)
{
	if (_thrd == NULL)
	{
		if (_engine)
			_engine->on_tick(curTick->code(), curTick);
		return;
	}

	uint32_t uDate = curTick->actiondate();
	uint32_t uTime = curTick->actiontime();

	if (_date != 0 && (uDate < _date || (uDate == _date && uTime < _time)))
	{
		//WTSLogger::info("行情时间{}小于本地时间{}", uTime, _time);
		if (_engine)
			_engine->on_tick(curTick->code(), curTick);
		return;
	}

	_date = uDate;
	_time = uTime;

	uint32_t curMin = _time / 100000;
	uint32_t curSec = _time % 100000;
	uint32_t minutes = _s_info->timeToMinutes(curMin);
	bool isSecEnd = _s_info->isLastOfSection(curMin);
	if (isSecEnd)
	{
		minutes--;
	}
	minutes++;
	uint32_t rawMin = curMin;
	curMin = _s_info->minuteToTime(minutes);

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

			WTSLogger::info("Minute Bar {}.{:04d} Closed by data", _date, thisMin);
			_engine->on_minute_end(_date, thisMin);
		}
			
		if (_engine)
		{
			_engine->on_tick(curTick->code(), curTick);
			_engine->set_date_time(_date, curMin, curSec, rawMin);
			_engine->set_trading_date(curTick->tradingdate());
		}

		_cur_pos = minutes;
	}
	else
	{
		//如果分钟数还是一致的, 则直接触发行情和时间即可
		if (_engine)
		{
			_engine->on_tick(curTick->code(), curTick);
			_engine->set_date_time(_date, curMin, curSec, rawMin);
		}
	}

	uint32_t sec = curSec / 1000;
	uint32_t msec = curSec % 1000;
	uint32_t left_ticks = (60 - sec) * 1000 - msec;
	_next_check_time = TimeUtils::getLocalTimeNow() + left_ticks;
}

void WtUftRtTicker::run()
{
	if (_thrd)
		return;

	_engine->on_init();

	uint32_t curTDate = _engine->get_basedata_mgr()->calcTradingDate(_s_info->id(), _engine->get_date(), _engine->get_min_time(), true);
	_engine->set_trading_date(curTDate);

	_engine->on_session_begin();

	//先检查当前时间, 如果大于
	uint32_t offTime = _s_info->offsetTime(_engine->get_min_time(), true);

	_thrd.reset(new StdThread([this, offTime](){
		while (!_stopped)
		{
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
					_time = thisMin;

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

					WTSLogger::info("Minute bar {}.{:04d} closed automatically", _date, thisMin);
					//if (_store)
					//	_store->onMinuteEnd(_date, thisMin);

					_engine->on_minute_end(_date, thisMin);

					uint32_t offMin = _s_info->offsetTime(thisMin, true);
					if (offMin >= _s_info->getCloseTime(true))
					{
						_engine->on_session_end();
					}

					//145959000
					if (_engine)
						_engine->set_date_time(_date, thisMin, 0);
				}
			}
			else //if (offTime >= _s_info->getOpenTime(true) && offTime <= _s_info->getCloseTime(true))
			{
				//不在交易时间，则休息10s再进行检查
				//因为这个逻辑是处理分钟线的，所以休盘时间休息10s，不会引起数据踏空的问题
				std::this_thread::sleep_for(std::chrono::seconds(10));
			}
			
		}
	}));
}

void WtUftRtTicker::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}
