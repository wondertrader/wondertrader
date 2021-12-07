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
#include "TraderAdapter.h"
#include "WtHelper.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

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

void WtHftEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier /* = NULL */)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr, notifier);

	_cfg = cfg;
	_cfg->retain();
}

void WtHftEngine::run(bool bAsync /*= false*/)
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		HftContextPtr& ctx = (HftContextPtr&)it->second;
		ctx->on_init();
	}

	_tm_ticker = new WtHftRtTicker(this);
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	//启动之前,先把运行中的策略落地
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		rj::Value jStraList(rj::kArrayType);
		for (auto& m : _ctx_map)
		{
			const HftContextPtr& ctx = m.second;
			jStraList.PushBack(rj::Value(ctx->name(), allocator), allocator);
		}

		root.AddMember("marks", jStraList, allocator);

		rj::Value jChnlList(rj::kArrayType);
		for (auto& m : _adapter_mgr->getAdapters())
		{
			const TraderAdapterPtr& adapter = m.second;
			jChnlList.PushBack(rj::Value(adapter->id(), allocator), allocator);
		}

		root.AddMember("channels", jChnlList, allocator);

		root.AddMember("engine", rj::Value("HFT", allocator), allocator);

		std::string filename = WtHelper::getBaseDir();
		filename += "marker.json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}

	_tm_ticker->run();

	if (!bAsync)
	{
		boost::asio::io_service::work work(g_asyncIO);
		g_asyncIO.run();
	}
}

void WtHftEngine::handle_push_quote(WTSTickData* newTick, uint32_t hotFlag)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick, hotFlag);
}

void WtHftEngine::handle_push_order_detail(WTSOrdDtlData* curOrdDtl)
{
	const char* stdCode = curOrdDtl->code();
	auto sit = _orddtl_sub_map.find(stdCode);
	if (sit != _orddtl_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				HftContextPtr& ctx = (HftContextPtr&)cit->second;
				ctx->on_order_detail(stdCode, curOrdDtl);
			}
		}
	}
}

void WtHftEngine::handle_push_order_queue(WTSOrdQueData* curOrdQue)
{
	const char* stdCode = curOrdQue->code();
	auto sit = _ordque_sub_map.find(stdCode);
	if (sit != _ordque_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				HftContextPtr& ctx = (HftContextPtr&)cit->second;
				ctx->on_order_queue(stdCode, curOrdQue);
			}
		}
	}
}

void WtHftEngine::handle_push_transaction(WTSTransData* curTrans)
{
	const char* stdCode = curTrans->code();
	auto sit = _trans_sub_map.find(stdCode);
	if (sit != _trans_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				HftContextPtr& ctx = (HftContextPtr&)cit->second;
				ctx->on_transaction(stdCode, curTrans);
			}
		}
	}
}

void WtHftEngine::sub_order_detail(uint32_t sid, const char* stdCode)
{
	std::size_t length = strlen(stdCode);
	if (stdCode[length - 1] == 'Q' || stdCode[length - 1] == 'H')
		length--;

	SIDSet& sids = _orddtl_sub_map[std::string(stdCode, length)];
	sids.insert(sid);
}

void WtHftEngine::sub_order_queue(uint32_t sid, const char* stdCode)
{
	std::size_t length = strlen(stdCode);
	if (stdCode[length - 1] == 'Q' || stdCode[length - 1] == 'H')
		length--;

	SIDSet& sids = _ordque_sub_map[std::string(stdCode, length)];
	sids.insert(sid);
}

void WtHftEngine::sub_transaction(uint32_t sid, const char* stdCode)
{
	std::size_t length = strlen(stdCode);
	if (stdCode[length - 1] == 'Q' || stdCode[length - 1] == 'H')
		length--;

	SIDSet& sids = _trans_sub_map[std::string(stdCode, length)];
	sids.insert(sid);
}

void WtHftEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

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
				HftContextPtr& ctx = (HftContextPtr&)cit->second;
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
			HftContextPtr& ctx = (HftContextPtr&)cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}
}

void WtHftEngine::on_session_begin()
{
	WTSLogger::info("Trading day %u begun", _cur_tdate);
	WtEngine::on_session_begin();

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		HftContextPtr& ctx = (HftContextPtr&)it->second;
		ctx->on_session_begin(_cur_tdate);
	}

	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, true);
}

void WtHftEngine::on_session_end()
{
	WtEngine::on_session_end();

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		HftContextPtr& ctx = (HftContextPtr&)it->second;
		ctx->on_session_end(_cur_tdate);
	}

	WTSLogger::info("Trading day %u ended", _cur_tdate);
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, false);
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

WTSOrdQueSlice* WtHftEngine::get_order_queue_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_queue_slice(code, count);
}

WTSOrdDtlSlice* WtHftEngine::get_order_detail_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_detail_slice(code, count);
}

WTSTransSlice* WtHftEngine::get_transaction_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_transaction_slice(code, count);
}