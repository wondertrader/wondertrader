/*!
* \file WtHftTicker.cpp
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#include "WtSelTicker.h"
#include "WtSelEngine.h"
#include "../Includes/IDataReader.h"

#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Share/CodeHelper.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;


WtSelRtTicker::WtSelRtTicker(WtSelEngine* engine)
	: _engine(engine)
	, _stopped(false)
	, _date(0)
	, _time(UINT_MAX)
	, _next_check_time(0)
	, _last_emit_pos(0)
	, _cur_pos(0)
{
}


WtSelRtTicker::~WtSelRtTicker()
{
}

void WtSelRtTicker::init(IDataReader* store, const char* sessionID)
{
	_store = store;
	_s_info = _engine->get_session_info(sessionID);

	TimeUtils::getDateTime(_date, _time);
}

void WtSelRtTicker::trigger_price(WTSTickData* curTick, uint32_t hotFlag /* = 0 */)
{
	if (_engine)
	{
		std::string stdCode = curTick->code();
		_engine->on_tick(stdCode.c_str(), curTick);

		if (hotFlag != 0)
		{
			WTSTickData* hotTick = WTSTickData::create(curTick->getTickStruct());
			std::string hotCode = (hotFlag == 1) ? CodeHelper::stdCodeToStdHotCode(stdCode.c_str()) : CodeHelper::stdCodeToStd2ndCode(stdCode.c_str());
			hotTick->setCode(hotCode.c_str(), hotCode.size());
			_engine->on_tick(hotCode.c_str(), hotTick);
			hotTick->release();
		}
	}
}

void WtSelRtTicker::on_tick(WTSTickData* curTick, uint32_t hotFlag /* = 0 */)
{
	if (_thrd == NULL)
	{
		trigger_price(curTick, hotFlag);
		return;
	}

	uint32_t uDate = curTick->actiondate();
	uint32_t uTime = curTick->actiontime();

	if (_date != 0 && (uDate < _date || (uDate == _date && uTime < _time)))
	{
		//WTSLogger::info("����ʱ��{}С�ڱ���ʱ��{}", uTime, _time);
		trigger_price(curTick, hotFlag);
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
		//�����ǰʱ����0, ��ֱ�Ӹ�ֵ����
		_cur_pos = minutes;
	}
	else if (_cur_pos < minutes)
	{
		//����Ѽ�¼�ķ���С���µķ���, ����Ҫ�����պ��¼�
		//���ʱ��Ҫ�ȴ����պ�, ���޸�ƽ̨ʱ��ͼ۸�
		if (_last_emit_pos < _cur_pos)
		{
			//�������ݻط�ģ��
			StdUniqueLock lock(_mtx);

			//�����޸�ʱ����
			_last_emit_pos = _cur_pos;

			uint32_t thisMin = _s_info->minuteToTime(_cur_pos);

			WTSLogger::info("Minute Bar {}.{:04d} Closed by data", _date, thisMin);
			if (_store)
				_store->onMinuteEnd(_date, thisMin);

			_engine->on_minute_end(_date, thisMin);

			uint32_t offMin = _s_info->offsetTime(thisMin, true);
			if (offMin == _s_info->getCloseTime(true))
			{
				_engine->on_session_end();
			}
		}

		trigger_price(curTick, hotFlag);
		if (_engine)
		{
			_engine->set_date_time(_date, curMin, curSec, rawMin);
			_engine->set_trading_date(curTick->tradingdate());
		}

		_cur_pos = minutes;
	}
	else
	{
		//�������������һ�µ�, ��ֱ�Ӵ��������ʱ�伴��
		trigger_price(curTick, hotFlag);
		if (_engine)
			_engine->set_date_time(_date, curMin, curSec, rawMin);
	}

	uint32_t sec = curSec / 1000;
	uint32_t msec = curSec % 1000;
	uint32_t left_ticks = (60 - sec) * 1000 - msec;
	_next_check_time = TimeUtils::getLocalTimeNow() + left_ticks;
}

void WtSelRtTicker::run()
{
	if (_thrd)
		return;

	_engine->on_init();

	uint32_t curTDate = _engine->get_basedata_mgr()->calcTradingDate(_s_info->id(), _engine->get_date(), _engine->get_min_time(), true);
	_engine->set_trading_date(curTDate);

	_engine->on_session_begin();

	//�ȼ�鵱ǰʱ��, �������
	//uint32_t offTime = _s_info->offsetTime(_engine->get_min_time());

	_thrd.reset(new StdThread([this](){
		while (!_stopped)
		{
			if (_time != UINT_MAX && _s_info->isInTradingTime(_time / 100000, true))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				uint64_t now = TimeUtils::getLocalTimeNow();

				if (now >= _next_check_time && _last_emit_pos < _cur_pos)
				{
					//�������ݻط�ģ��
					StdUniqueLock lock(_mtx);

					//�����޸�ʱ����
					_last_emit_pos = _cur_pos;

					uint32_t thisMin = _s_info->minuteToTime(_cur_pos);
					_time = thisMin;

					//���thisMin��0, ˵��������
					//�����Ǳ��ؼ�ʱ���µĻ���, ˵��������ʵ����������, Ҫ�Զ�+1
					//ͬʱ��Ϊʱ����235959xxx, ����ҲҪ�ֶ���Ϊ0
					if (thisMin == 0)
					{
						uint32_t lastDate = _date;
						_date = TimeUtils::getNextDate(_date);
						_time = 0;
						WTSLogger::info("Data automatically changed at time 00:00: {} -> {}", lastDate, _date);
					}

					WTSLogger::info("Minute bar {}.{:04d} closed automatically", _date, thisMin);
					if (_store)
						_store->onMinuteEnd(_date, thisMin);

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
			else
			{//������ڽ���ʱ��,��ÿ��10������һ��,������ӷ����仯�򴥷�
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

void WtSelRtTicker::stop()
{
	_stopped = true;
	if (_thrd)
		_thrd->join();
}
