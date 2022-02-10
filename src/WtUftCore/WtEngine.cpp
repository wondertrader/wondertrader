/*!
 * \file WtEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtEngine.h"
#include "WtUftDtMgr.h"
#include "WtHelper.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSRiskDef.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;


USING_NS_WTP;

WtEngine::WtEngine()
	: _evt_listener(NULL)
	, _adapter_mgr(NULL)
	, _all_tick_mode(false)
{
	TimeUtils::getDateTime(_cur_date, _cur_time);
	_cur_secs = _cur_time % 100000;
	_cur_time /= 100000;
	_cur_raw_time = _cur_time;
	_cur_tdate = _cur_date;

	WtHelper::setTime(_cur_date, _cur_time, _cur_secs);
}

void WtEngine::set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs /* = 0 */, uint32_t rawTime /* = 0 */)
{
	_cur_date = curDate;
	_cur_time = curTime;
	_cur_secs = curSecs;

	if (rawTime == 0)
		rawTime = curTime;

	_cur_raw_time = rawTime;

	WtHelper::setTime(_cur_date, _cur_raw_time, _cur_secs);
}

void WtEngine::set_trading_date(uint32_t curTDate)
{
	_cur_tdate = curTDate; 

	WtHelper::setTDate(curTDate);
}

WTSCommodityInfo* WtEngine::get_commodity_info(const char* stdCode)
{
	return _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSContractInfo* WtEngine::get_contract_info(const char* stdCode)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	return _base_data_mgr->getContract(cInfo._code, cInfo._exchg);
}

WTSSessionInfo* WtEngine::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _base_data_mgr->getSession(sid);

	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(sid).c_str());
	if (cInfo == NULL)
		return NULL;

	return _base_data_mgr->getSession(cInfo->getSession());
}

void WtEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	_price_map[stdCode] = curTick->price();
}

void WtEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtUftDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier)
{
	_base_data_mgr = bdMgr;
	_data_mgr = dataMgr;

	WTSLogger::info("Platform running mode: Production");

	//全tick模式，即如果tick数据成交量为0，也触发ontick，默认为false，即只触发有成交量的tick
	_all_tick_mode = cfg->getBoolean("alltick");
}

WTSTickSlice* WtEngine::get_tick_slice(uint32_t sid, const char* code, uint32_t count)
{
	return NULL;
	return _data_mgr->get_tick_slice(code, count);
}

WTSTickData* WtEngine::get_last_tick(uint32_t sid, const char* stdCode)
{
	return _data_mgr->grab_last_tick(stdCode);
}

WTSKlineSlice* WtEngine::get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, uint64_t etime /* = 0 */)
{
	return NULL;
	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(stdCode);
	if (cInfo == NULL)
		return NULL;

	WTSSessionInfo* sInfo = _base_data_mgr->getSession(cInfo->getSession());
	if (sInfo == NULL)
		return NULL;

	std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	SubList& sids = _bar_sub_map[key];
	sids.insert(sid);

	WTSKlinePeriod kp;
	if (strcmp(period, "m") == 0)
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			times /= 5;
		}
		else
			kp = KP_Minute1;
	}
	else
	{
		kp = KP_DAY;
	}

	return _data_mgr->get_kline_slice(stdCode, kp, times, count, etime);
}


void WtEngine::handle_push_quote(WTSTickData* curTick, uint32_t hotFlag)
{
	std::string stdCode = curTick->code();
	_data_mgr->handle_push_quote(stdCode.c_str(), curTick);
	on_tick(stdCode.c_str(), curTick);

	double price = curTick->price();

	if(hotFlag == 1)
	{
		std::string hotCode = CodeHelper::stdCodeToStdHotCode(stdCode.c_str());
		WTSTickData* hotTick = WTSTickData::create(curTick->getTickStruct());
		hotTick->setCode(hotCode.c_str());
		
		_data_mgr->handle_push_quote(hotCode.c_str(), hotTick);
		on_tick(hotCode.c_str(), hotTick);

		hotTick->release();
	}
	else if (hotFlag == 2)
	{
		std::string scndCode = CodeHelper::stdCodeToStd2ndCode(stdCode.c_str());
		WTSTickData* scndTick = WTSTickData::create(curTick->getTickStruct());
		scndTick->setCode(scndCode.c_str());

		_data_mgr->handle_push_quote(scndCode.c_str(), scndTick);
		on_tick(scndCode.c_str(), scndTick);

		scndTick->release();
	}
}

double WtEngine::get_cur_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if(it == _price_map.end())
	{
		WTSTickData* lastTick = _data_mgr->grab_last_tick(stdCode);
		if (lastTick == NULL)
			return 0.0;

		double ret = lastTick->price();
		lastTick->release();
		_price_map[stdCode] = ret;
		return ret;
	}
	else
	{
		return it->second;
	}
}

void WtEngine::sub_tick(uint32_t sid, const char* stdCode)
{
	SubList& sids = _tick_sub_map[stdCode];
	sids.insert(sid);
}
