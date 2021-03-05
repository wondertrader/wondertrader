/*!
 * \file WtCtaEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#define WIN32_LEAN_AND_MEAN

#include "WtCtaEngine.h"
#include "WtDataManager.h"
#include "WtCtaTicker.h"
#include "WtHelper.h"
#include "TraderAdapter.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSRiskDef.hpp"
#include "../Share/decimal.h"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include <boost/asio.hpp>

boost::asio::io_service g_asyncIO;


WtCtaEngine::WtCtaEngine()
	: _tm_ticker(NULL)
{
	
}


WtCtaEngine::~WtCtaEngine()
{
	if (_tm_ticker)
	{
		delete _tm_ticker;
		_tm_ticker = NULL;
	}

	if (_cfg)
		_cfg->release();
}

void WtCtaEngine::run(bool bAsync /* = false */)
{
	_tm_ticker = new WtCtaRtTicker(this);
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	//启动之前，先把运行中的策略落地
	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		rj::Value jStraList(rj::kArrayType);
		for (auto& m : _ctx_map)
		{
			const CtaContextPtr& ctx = m.second;
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

		root.AddMember("engine", rj::Value("CTA", allocator), allocator);

		std::string filename = WtHelper::getBaseDir();
		filename += "marker.json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}

	_tm_ticker->run();

	if (_risk_mon)
		_risk_mon->self()->run();

	if (!bAsync)
	{
		boost::asio::io_service::work work(g_asyncIO);
		g_asyncIO.run();
	}
}

void WtCtaEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr);

	_cfg = cfg;
	_cfg->retain();

	_exec_mgr.set_filter_mgr(&_filter_mgr);
}

void WtCtaEngine::addContext(CtaContextPtr ctx)
{
	uint32_t sid = ctx->id();
	_ctx_map[sid] = ctx;
}

CtaContextPtr WtCtaEngine::getContext(uint32_t id)
{
	auto it = _ctx_map.find(id);
	if (it == _ctx_map.end())
		return CtaContextPtr();

	return it->second;
}

void WtCtaEngine::on_init()
{
	std::unordered_map<std::string, double> target_pos;
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = it->second;
		ctx->on_init();

		ctx->enum_position([this, &target_pos](const char* stdCode, double qty){
			std::string realCode = stdCode;
			if (StrUtil::endsWith(realCode, ".HOT", false))
			{
				CodeHelper::CodeInfo cInfo;
				CodeHelper::extractStdCode(stdCode, cInfo);
				std::string code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
				realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);
			}

			double& vol = target_pos[realCode];
			vol += qty;

			_ticksubed_raw_codes.insert(realCode);
		});
	}

	_exec_mgr.set_positions(target_pos);

	if (_evt_listener)
		_evt_listener->on_initialize_event();
}

void WtCtaEngine::on_session_begin()
{
	WTSLogger::info("交易日%u已开始", _cur_tdate);
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = it->second;
		ctx->on_session_begin(_cur_tdate);
	}

	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, true);
}

void WtCtaEngine::on_session_end()
{
	WtEngine::on_session_end();

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = it->second;
		ctx->on_session_end(_cur_tdate);
	}

	WTSLogger::info("交易日%u已结束", _cur_tdate);
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, false);
}

void WtCtaEngine::on_schedule(uint32_t curDate, uint32_t curTime)
{
	//去检查一下过滤器
	_filter_mgr.load_filters();

	std::unordered_map<std::string, double> target_pos;

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = it->second;
		ctx->on_schedule(curDate, curTime);
		ctx->enum_position([this, &target_pos, ctx](const char* stdCode, double qty){

			double oldQty = qty;
			bool bFilterd = _filter_mgr.is_filtered_by_strategy(ctx->name(), qty);
			if(!bFilterd)
			{
				if(!decimal::eq(qty, oldQty))
				{
					//输出日志
					WTSLogger::info(fmt::format("[过滤器] 策略{}的{}的目标仓位被策略过滤器调整: {} -> {}", ctx->name(), stdCode, oldQty, qty).c_str());
				}

				std::string realCode = stdCode;
				if (StrUtil::endsWith(realCode, ".HOT", false))
				{
					CodeHelper::CodeInfo cInfo;
					CodeHelper::extractStdCode(stdCode, cInfo);
					std::string code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
					realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);
				}

				double& vol = target_pos[realCode];
				vol += qty;
			}
			else
			{
				//输出日志
				WTSLogger::info("[过滤器] 策略%s的%s的目标仓位被策略过滤器忽略", ctx->name(), stdCode);
			}
		});
	}

	bool bRiskEnabled = false;
	if(!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::info2("risk", "组合盘仓位风控系数为%.2f", _risk_volscale);
		bRiskEnabled = true;
	}

	//处理组合理论部位
	for (auto it = target_pos.begin(); it != target_pos.end(); it++)
	{
		std::string stdCode = it->first;
		double& pos = it->second;

		if (bRiskEnabled && !decimal::eq(pos, 0))
		{
			double symbol = pos / abs(pos);
			pos = decimal::rnd(abs(pos)*_risk_volscale)*symbol;
		}

		push_task([this, stdCode, pos]() {
			append_signal(stdCode.c_str(), pos);
		});
	}

	for(auto& m : _pos_map)
	{
		std::string stdCode = m.first;
		if (target_pos.find(stdCode) == target_pos.end())
		{
			if(!decimal::eq(m.second._volume, 0))
			{
				push_task([this, stdCode](){
					append_signal(stdCode.c_str(), 0);
				});

				WTSLogger::error("品种%s不在目标仓位内，自动设置为0", stdCode.c_str());
			}

			target_pos[stdCode] = 0;
		}
	}

	push_task([this](){
		update_fund_dynprofit();
	});

	_exec_mgr.set_positions(target_pos);

	save_datas();

	if (_evt_listener)
		_evt_listener->on_schedule_event(curDate, curTime);
}


void WtCtaEngine::handle_push_quote(WTSTickData* newTick, bool isHot)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick, isHot);
}

void WtCtaEngine::handle_pos_change(const char* straName, const char* stdCode, double diffQty)
{
	//这里是持仓增量，所以不用处理未过滤的情况，因为增量情况下，不会改变目标diffQty
	if(_filter_mgr.is_filtered_by_strategy(straName, diffQty, true))
	{
		//输出日志
		WTSLogger::info("[过滤器] 策略%s的%s的目标仓位被策略过滤器忽略", straName, stdCode);
		return;
	}

	std::string realCode = stdCode;
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdCode(stdCode, cInfo);
		std::string code = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
		realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);
	}

	PosInfo& pItem = _pos_map[realCode];
	double targetPos = pItem._volume + diffQty;

	bool bRiskEnabled = false;
	if (!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::info2("risk", "组合盘仓位风控系数为%.2f", _risk_volscale);
		bRiskEnabled = true;
	}
	if (bRiskEnabled && !decimal::eq(targetPos, 0))
	{
		double symbol = targetPos / abs(targetPos);
		targetPos = decimal::rnd(abs(targetPos)*_risk_volscale)*symbol;
	}

	push_task([this, realCode, targetPos]() {
		append_signal(realCode.c_str(), targetPos);
	});

	_exec_mgr.handle_pos_change(realCode.c_str(), targetPos);
}

void WtCtaEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//如果是真实代码, 则要传递给执行器
	auto it = _ticksubed_raw_codes.find(stdCode);
	if (it != _ticksubed_raw_codes.end())
	{
		//是否主力合约代码的标记, 主要用于给执行器发数据的
		_exec_mgr.handle_tick(stdCode, curTick);
	}

	auto sit = _tick_sub_map.find(stdCode);
	if (sit != _tick_sub_map.end())
	{
		const SIDSet& sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = *it;
			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end() && curTick->volume())
			{
				CtaContextPtr& ctx = cit->second;
				ctx->on_tick(stdCode, curTick);
			}
		}
	}

	//暂时先不考虑成本, 先计算出来
	/*
	double dynprofit = 0.0;
	for(auto v : _ctx_map)
	{
		const CtaContextPtr& ctx = v.second;
		dynprofit += ctx->get_dyn_profit();
	}

	WTSFundStruct& fundInfo = _port_fund->fundInfo();
	fundInfo._dynprofit = dynprofit;
	double dynbal = fundInfo._balance + dynprofit;
	if (fundInfo._max_dyn_bal != DBL_MAX)
		fundInfo._max_dyn_bal = max(fundInfo._max_dyn_bal, dynbal);
	else
		fundInfo._max_dyn_bal = dynbal;

	if (fundInfo._min_dyn_bal != DBL_MAX)
		fundInfo._min_dyn_bal = min(fundInfo._min_dyn_bal, dynbal);
	else
		fundInfo._min_dyn_bal = dynbal;
	*/
}

void WtCtaEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	const SIDSet& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = *it;
		auto cit = _ctx_map.find(sid);
		if(cit != _ctx_map.end())
		{
			CtaContextPtr& ctx = cit->second;
			ctx->on_bar(stdCode, period, times, newBar);
		}
	}

	WTSLogger::info("K线 [%s#%s%d] @ %u已闭合", stdCode, period, times, period[0] == 'd' ? newBar->date : newBar->time);
}

bool WtCtaEngine::isInTrading()
{
	return _tm_ticker->is_in_trading();
}

uint32_t WtCtaEngine::transTimeToMin(uint32_t uTime)
{
	return _tm_ticker->time_to_mins(uTime);
}

WTSCommodityInfo* WtCtaEngine::get_comm_info(const char* stdCode)
{
	return _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSSessionInfo* WtCtaEngine::get_sess_info(const char* stdCode)
{
	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
	if (cInfo == NULL)
		return NULL;

	return _base_data_mgr->getSession(cInfo->getSession());
}

uint64_t WtCtaEngine::get_real_time()
{
	return TimeUtils::makeTime(_cur_date, _cur_raw_time * 100000 + _cur_secs);
}