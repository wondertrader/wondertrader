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

#include "WtUftEngine.h"
#include "WtUftTicker.h"
#include "WtUftDtMgr.h"
#include "TraderAdapter.h"
#include "WtHelper.h"

#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include <boost/asio.hpp>

extern boost::asio::io_service g_asyncIO;


USING_NS_WTP;

WtUftEngine::WtUftEngine()
	: _cfg(NULL)
	, _tm_ticker(NULL)
{
}


WtUftEngine::~WtUftEngine()
{
	if (_tm_ticker)
	{
		delete _tm_ticker;
		_tm_ticker = NULL;
	}

	if (_cfg)
		_cfg->release();
}

void WtUftEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtUftDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier /* = NULL */)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr, notifier);

	_cfg = cfg;
	_cfg->retain();
}

void WtUftEngine::run(bool bAsync /*= false*/)
{
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		UftContextPtr& ctx = (UftContextPtr&)it->second;
		ctx->on_init();
	}

	_tm_ticker = new WtUftRtTicker(this);
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker->init(cfgProd->getCString("session"));

	//启动之前,先把运行中的策略落地
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		rj::Value jStraList(rj::kArrayType);
		for (auto& m : _ctx_map)
		{
			const UftContextPtr& ctx = m.second;
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

void WtUftEngine::handle_push_quote(WTSTickData* newTick, uint32_t hotFlag)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick, hotFlag);
}

void WtUftEngine::handle_push_order_detail(WTSOrdDtlData* curOrdDtl)
{
	const char* stdCode = curOrdDtl->code();
	auto sit = _orddtl_sub_map.find(stdCode);
	if (sit != _orddtl_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_order_detail(stdCode, curOrdDtl);
			}
		}
	}
}

void WtUftEngine::handle_push_order_queue(WTSOrdQueData* curOrdQue)
{
	const char* stdCode = curOrdQue->code();
	auto sit = _ordque_sub_map.find(stdCode);
	if (sit != _ordque_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_order_queue(stdCode, curOrdQue);
			}
		}
	}
}

void WtUftEngine::handle_push_transaction(WTSTransData* curTrans)
{
	const char* stdCode = curTrans->code();
	auto sit = _trans_sub_map.find(stdCode);
	if (sit != _trans_sub_map.end())
	{
		const SubList& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			//By Wesley @ 2022.02.07
			//Level2数据一般用于HFT场景，所以不做复权处理
			//所以不读取订阅标记
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				UftContextPtr& ctx = (UftContextPtr&)cit->second;
				ctx->on_transaction(stdCode, curTrans);
			}
		}
	}
}

void WtUftEngine::sub_order_detail(uint32_t sid, const char* stdCode)
{
	SubList& sids = _orddtl_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::sub_order_queue(uint32_t sid, const char* stdCode)
{
	SubList& sids = _ordque_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::sub_transaction(uint32_t sid, const char* stdCode)
{
	SubList& sids = _trans_sub_map[stdCode];
	sids.insert(sid);
}

void WtUftEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//auto sit = _tick_sub_map.find(stdCode);
	//if (sit != _tick_sub_map.end())
	//{
	//	const SubList& sids = sit->second;
	//	for (auto it = sids.begin(); it != sids.end(); it++)
	//	{
	//		uint32_t sid = it->first;
	//		auto cit = _ctx_map.find(sid);
	//		if (cit != _ctx_map.end())
	//		{
	//			HftContextPtr& ctx = (HftContextPtr&)cit->second;
	//			ctx->on_tick(stdCode, curTick);
	//		}
	//	}
	//}

	/*
	 *	By Wesley @ 2022.02.07
	 *	这里做了一个彻底的调整
	 *	第一，检查订阅标记，如果标记为0，即无复权模式，则直接按照原始代码触发ontick
	 *	第二，如果标记为1，即前复权模式，则将代码转成xxxx-，再触发ontick
	 *	第三，如果标记为2，即后复权模式，则将代码转成xxxx+，再把tick数据做一个修正，再触发ontick
	 */
	{
		auto sit = _tick_sub_map.find(stdCode);
		if (sit != _tick_sub_map.end())
		{
			const SubList& sids = sit->second;
			for (auto it = sids.begin(); it != sids.end(); it++)
			{
				uint32_t sid = *it;

				auto cit = _ctx_map.find(sid);
				if (cit != _ctx_map.end())
				{
					UftContextPtr& ctx = (UftContextPtr&)cit->second;
					ctx->on_tick(stdCode, curTick);
				}
			}
		}
	}
}

void WtUftEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	const SubList& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = *it;
		auto cit = _ctx_map.find(sid);
		if (cit != _ctx_map.end())
		{
			UftContextPtr& ctx = (UftContextPtr&)cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}
}

void WtUftEngine::on_minute_end(uint32_t curDate, uint32_t curTime)
{

}

void WtUftEngine::addContext(UftContextPtr ctx)
{
	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

UftContextPtr WtUftEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return UftContextPtr();

	return it->second;
}

WTSOrdQueSlice* WtUftEngine::get_order_queue_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_queue_slice(code, count);
}

WTSOrdDtlSlice* WtUftEngine::get_order_detail_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_order_detail_slice(code, count);
}

WTSTransSlice* WtUftEngine::get_transaction_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_transaction_slice(code, count);
}