/*!
 * \file WtHftEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#define WIN32_LEAN_AND_MEAN

#include "WtHftEngine.h"
#include "WtHftTicker.h"
#include "WtDataManager.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Includes/WTSVariant.hpp"

#include <boost/asio.hpp>

extern boost::asio::io_service g_asyncIO;


USING_NS_OTP;

WtHftEngine::WtHftEngine()
	: _cfg(NULL)
	, _tm_ticker(NULL)
{
}


WtHftEngine::~WtHftEngine()
{
	if (_tm_ticker)
	{
		delete _tm_ticker;
		_tm_ticker = NULL;
	}

	if (_cfg)
		_cfg->release();
}

void WtHftEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr);

	_cfg = cfg;
	_cfg->retain();
}

void WtHftEngine::run(bool bAsync /*= false*/)
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		HftContextPtr& ctx = it->second;
		ctx->on_init();
	}

	_tm_ticker = new WtHftRtTicker(this);
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	_tm_ticker->run();

	if (!bAsync)
	{
		boost::asio::io_service::work work(g_asyncIO);
		g_asyncIO.run();
	}
}

void WtHftEngine::handle_push_quote(WTSTickData* newTick, bool isHot)
{
	//WtEngine::handle_push_quote(newTick, isHot);
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick, isHot);
}

void WtHftEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//uint32_t uDate = curTick->actiondate();
	//uint32_t uTime = curTick->actiontime();
	//
	//uint64_t now = (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime();
	//uint64_t preTick = (uint64_t)_cur_date * 1000000000 + _cur_time * 100000 + _cur_secs;
	//if(now >= preTick)
	//{
	//	set_date_time(uDate, uTime / 100000, uTime % 100000);
	//	set_trading_date(curTick->tradingdate());
	//}

	auto sit = _tick_sub_map.find(stdCode);
	if (sit != _tick_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				HftContextPtr& ctx = cit->second;
				ctx->on_tick(stdCode, curTick);
			}
		}
	}
}

void WtHftEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	const SIDSet& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = *it;
		auto cit = _ctx_map.find(sid);
		if (cit != _ctx_map.end())
		{
			HftContextPtr& ctx = cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}
}

void WtHftEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{
	//已去掉高频策略的on_schedule
	//for(auto& cit : _ctx_map)
	//{
	//	HftContextPtr& ctx = cit.second;
	//	ctx->on_schedule(curDate, curTime);
	//}
}

void WtHftEngine::addContext(HftContextPtr ctx)
{
	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

HftContextPtr WtHftEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return HftContextPtr();

	return it->second;
}
