/*!
* \file WtHftTicker.cpp
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#include "WtMfTicker.h"
#include "WtMfEngine.h"
#include "IDataReader.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/WTSSessionInfo.hpp"
#include "../Share/IBaseDataMgr.h"
#include "../Share/WTSDataDef.hpp"
#include "../Share/IHotMgr.h"
#include "../Share/CodeHelper.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;


WtMfRtTicker::WtMfRtTicker(WtMfEngine* engine)
	: _engine(engine)
	, _stopped(false)
	, _date(0)
	, _time(UINT_MAX)
	, _next_check_time(0)
	, _last_emit_pos(0)
	, _cur_pos(0)
{
}


WtMfRtTicker::~WtMfRtTicker()
{
}

void WtMfRtTicker::init(IDataReader* store, const char* sessionID)
{
	_store = store;
	_s_info = _engine->get_session_info(sessionID);

	TimeUtils::getDateTime(_date, _time);
}

void WtMfRtTicker::trigger_price(WTSTickData* curTick, bool isHot/* = false*/)
{
	if (_engine)
	{
		std::string stdCode = curTick->code();
		_engine->on_tick(stdCode.c_str(), curTick);

		if (isHot)
		{
			WTSTickData* hotTick = WTSTickData::create(curTick->getTickStruct());
			std::string hotCode = CodeHelper::stdCodeToStdHotCode(stdCode.c_str());
			strcpy(hotTick->getTickStruct().code, hotCode.c_str());
			_engine->on_tick(hotCode.c_str(), hotTick);
			hotTick->release();
		}
	}
}

void WtMfRtTicker::on_tick(WTSTickData* curTick, bool isHot /* = false */)
{
	if (_thrd == NULL)
	{
		trigger_price(curTick, isHot);
		return;
	}

	uint32_t uDate = curTick->actiondate();
	uint32_t uTime = curTick->actiontime();

	if (_date != 0 && (uDate < _date || (uDate == _date && uTime < _time)))
	{
		//WTSLogger::info("行情时间%u小于本地时间%u", uTime, _time);
		trigger_price(curTick, isHot);
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
			BoostUniqueLock lock(_mtx);

			//优先修改时间标记
			_last_emit_pos = _cur_pos;

			uint32_t thisMin = _s_info->minuteToTime(_cur_pos);

			WTSLogger::info("分钟线 %u.%04u 触发闭合", _date, thisMin);
			if (_store)
				_store->onMinuteEnd(_date, thisMin);

			_engine->on_minute_end(_date, thisMin);

			uint32_t offMin = _s_info->offsetTime(thisMin);
			if (offMin == _s_info->getCloseTime(true))
			{
				_engine->on_session_end();
			}
		}

		trigger_price(curTick, isHot);
		if (_engine)
		{
			_engine->set_date_time(_date, curMin, curSec);
			_engine->set_trading_date(curTick->tradingdate());
		}

		_cur_pos = minutes;
	}
	else
	{
		//如果分钟数还是一致的, 则直接触发行情和时间即可
		trigger_price(curTick, isHot);
		if (_engine)
			_engine->set_date_time(_date, curMin, curSec);
	}

	uint32_t sec = curSec / 1000;
	uint32_t msec = curSec % 1000;
	uint32_t left_ticks = (60 - sec) * 1000 - msec;
	_next_check_time = TimeUtils::getLocalTimeNow() + left_ticks;
}

void WtMfRtTicker::run()
{
	if (_thrd)
		return;

	_engine->on_init();

	uint32_t curTDate = _engine->get_basedata_mgr()->calcTradingDate(_s_info->id(), _engine->get_date(), _engine->get_min_time(), true);
	_engine->set_trading_date(curTDate);

	_engine->on_session_begin();

	//先检查当前时间, 如果大于
	//uint32_t offTime = _s_info->offsetTime(_engine->get_min_time());

	_thrd.reset(new BoostThread([this](){
		while (!_stopped)
		{
			if (_time != UINT_MAX && _s_info->isInTradingTime(_time / 100000, true))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				uint64_t now = TimeUtils::getLocalTimeNow();

				if (now >= _next_check_time && _last_emit_pos < _cur_pos)
				{
					//触发数据回放模块
					BoostUniqueLock lock(_mtx);

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
						WTSLogger::info("0点日期自动切换: %u -> %u", lastDate, _date);
					}

					WTSLogger::info("分钟线 %u.%04u 自动闭合", _date, thisMin);
					if (_store)
						_store->onMinuteEnd(_date, thisMin);

					_engine->on_minute_end(_date, thisMin);

					uint32_t offMin = _s_info->offsetTime(thisMin);
					if (offMin >= _s_info->getCloseTime(true))
					{
						_engine->on_session_end();
					}

					//145959000
					if (_engine)
						_engine->set_date_time(_date, thisMin, 0);
				}
			}
			else
			{//如果不在交易时间，则每隔10毫秒检查一次，如果分钟发生变化则触发
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				uint32_t curTime = TimeUtils::getCurMin();
				if (_time != UINT_MAX && curTime != _time)
				{
					_engine->on_minute_end(_date, _time);
					if (curTime < _time)
						_date = TimeUtils::getNextDate(_date);
					_time = curTime;
				}
			}
		}
	}));
}

void WtMfRtTicker::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}
