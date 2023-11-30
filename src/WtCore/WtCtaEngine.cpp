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
#include "WtDtMgr.h"
#include "WtCtaTicker.h"
#include "WtHelper.h"
#include "TraderAdapter.h"
#include "EventNotifier.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSRiskDef.hpp"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

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

void WtCtaEngine::run()
{
	_tm_ticker = new WtCtaRtTicker(this);
	WTSVariant* cfgProd = _cfg->get("product");
	_tm_ticker->init(_data_mgr->reader(), cfgProd->getCString("session"));

	//启动之前,先把运行中的策略落地
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

		rj::Value jExecList(rj::kArrayType);
		_exec_mgr.enum_executer([&jExecList, &allocator](ExecCmdPtr executer) {
			if(executer)
				jExecList.PushBack(rj::Value(executer->name(), allocator), allocator);
		});

		root.AddMember("executers", jExecList, allocator);

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

}

void WtCtaEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier /* = NULL */)
{
	WtEngine::init(cfg, bdMgr, dataMgr, hotMgr, notifier);

	_cfg = cfg;
	_cfg->retain();

	_exec_mgr.set_filter_mgr(&_filter_mgr);

	uint32_t poolsize = cfg->getUInt32("poolsize");
	if (poolsize > 0)
	{
		_pool.reset(new boost::threadpool::pool(poolsize));
	}
	WTSLogger::info("Engine task poolsize is {}", poolsize);
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
	//wt_hashmap<std::string, double> target_pos;
	_exec_mgr.clear_cached_targets();
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = (CtaContextPtr&)it->second;
		ctx->on_init();

		const auto& exec_ids = _exec_mgr.get_route(ctx->name());

		ctx->enum_position([this, ctx, exec_ids](const char* stdCode, double qty){

			double oldQty = qty;
			bool bFilterd = _filter_mgr.is_filtered_by_strategy(ctx->name(), qty);
			if (!bFilterd)
			{
				if (!decimal::eq(qty, oldQty))
				{
					//输出日志
					WTSLogger::info("[Filters] Target position of {} of strategy {} reset by strategy filter: {} -> {}", 
						stdCode, ctx->name(), oldQty, qty);
				}

				std::string realCode = stdCode;
				CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
				if(strlen(cInfo._ruletag) > 0)
				{
					std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _cur_tdate);
					realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
				}

				for(auto& execid : exec_ids)
					_exec_mgr.add_target_to_cache(realCode.c_str(), qty, execid.c_str());
			}
			else
			{
				//输出日志
				WTSLogger::info("[Filters] Target position of {} of strategy {} ignored by strategy filter", stdCode, ctx->name());
			}
		}, true);
	}

	bool bRiskEnabled = false;
	if (!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::log_by_cat("risk", LL_INFO, "Risk scale of portfolio is {:.2f}", _risk_volscale);
		bRiskEnabled = true;
	}

	////初始化仓位打印出来
	//for (auto it = target_pos.begin(); it != target_pos.end(); it++)
	//{
	//	const auto& stdCode = it->first;
	//	double& pos = (double&)it->second;

	//	if (bRiskEnabled && !decimal::eq(pos, 0))
	//	{
	//		double symbol = pos / abs(pos);
	//		pos = decimal::rnd(abs(pos)*_risk_volscale)*symbol;
	//	}

	//	WTSLogger::info("Portfolio initial position of {} is {}", stdCode.c_str(), pos);
	//}

	_exec_mgr.commit_cached_targets(bRiskEnabled?_risk_volscale:1.0);

	if (_evt_listener)
		_evt_listener->on_initialize_event();
}

void WtCtaEngine::on_session_begin()
{
	WTSLogger::info("Trading day {} begun", _cur_tdate);
	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = (CtaContextPtr&)it->second;
		ctx->on_session_begin(_cur_tdate);
	}

	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, true);

	_ready = true;
}

void WtCtaEngine::on_session_end()
{
	WtEngine::on_session_end();

	for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
	{
		CtaContextPtr& ctx = (CtaContextPtr&)it->second;
		ctx->on_session_end(_cur_tdate);
	}

	WTSLogger::info("Trading day {} ended", _cur_tdate);
	if (_evt_listener)
		_evt_listener->on_session_event(_cur_tdate, false);
}

void WtCtaEngine::on_schedule(uint32_t curDate, uint32_t curTime)
{
	//去检查一下过滤器
	_filter_mgr.load_filters();
	_exec_mgr.clear_cached_targets();
	wt_hashmap<std::string, double> target_pos;
	if(_pool)
	{
		/*
		 *	By Wesley @ 2023.06.27
		 *	如果通过线程池并发
		 *	先并发所有的on_schedule
		 *	然后再wait所有任务结束
		 *	最后再统一读取全部持仓
		 */
		for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
		{
			CtaContextPtr& ctx = (CtaContextPtr&)it->second;
			_pool->schedule([ctx, curDate, curTime] (){
				ctx->on_schedule(curDate, curTime);
			});
		}

		/*
		 *	By Wesley @ 2023.06.27
		 *	等待全部on_schedule执行完成
		 */
		_pool->wait();
		
		for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
		{
			CtaContextPtr& ctx = (CtaContextPtr&)it->second;
			const auto& exec_ids = _exec_mgr.get_route(ctx->name());
			ctx->enum_position([this, ctx, exec_ids, &target_pos](const char* stdCode, double qty) {

				double oldQty = qty;
				bool bFilterd = _filter_mgr.is_filtered_by_strategy(ctx->name(), qty);
				if (!bFilterd)
				{
					if (!decimal::eq(qty, oldQty))
					{
						//输出日志
						WTSLogger::info("[Filters] Target position of {} of strategy {} reset by strategy filter: {} -> {}",
							stdCode, ctx->name(), oldQty, qty);
					}

					std::string realCode = stdCode;
					CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
					if (strlen(cInfo._ruletag) > 0)
					{
						std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _cur_tdate);
						realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
					}

					double& vol = target_pos[realCode];
					vol += qty;
					for (auto& execid : exec_ids)
						_exec_mgr.add_target_to_cache(realCode.c_str(), qty, execid.c_str());
				}
				else
				{
					//输出日志
					WTSLogger::info("[Filters] Target position of {} of strategy {} ignored by strategy filter", stdCode, ctx->name());
				}
			}, true);
		}
	}
	else
	{
		for (auto it = _ctx_map.begin(); it != _ctx_map.end(); it++)
		{
			CtaContextPtr& ctx = (CtaContextPtr&)it->second;
			ctx->on_schedule(curDate, curTime);
			const auto& exec_ids = _exec_mgr.get_route(ctx->name());
			ctx->enum_position([this, ctx, exec_ids, &target_pos](const char* stdCode, double qty) {

				double oldQty = qty;
				bool bFilterd = _filter_mgr.is_filtered_by_strategy(ctx->name(), qty);
				if (!bFilterd)
				{
					if (!decimal::eq(qty, oldQty))
					{
						//输出日志
						WTSLogger::info("[Filters] Target position of {} of strategy {} reset by strategy filter: {} -> {}",
							stdCode, ctx->name(), oldQty, qty);
					}

					std::string realCode = stdCode;
					CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
					if (strlen(cInfo._ruletag) > 0)
					{
						std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _cur_tdate);
						realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
					}

					double& vol = target_pos[realCode];
					vol += qty;
					for (auto& execid : exec_ids)
						_exec_mgr.add_target_to_cache(realCode.c_str(), qty, execid.c_str());
				}
				else
				{
					//输出日志
					WTSLogger::info("[Filters] Target position of {} of strategy {} ignored by strategy filter", stdCode, ctx->name());
				}
			}, true);
		}
	}
	

	bool bRiskEnabled = false;
	if(!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::log_by_cat("risk", LL_INFO, "Risk scale of strategy group is {:.2f}", _risk_volscale);
		bRiskEnabled = true;
	}

	//处理组合理论部位
	for (auto it = target_pos.begin(); it != target_pos.end(); it++)
	{
		const auto& stdCode = it->first;
		double& pos = (double&)it->second;

		if (bRiskEnabled && !decimal::eq(pos, 0))
		{
			double symbol = pos / abs(pos);
			pos = decimal::rnd(abs(pos)*_risk_volscale)*symbol;
		}

		append_signal(stdCode.c_str(), pos, true);
	}

	for(auto& m : _pos_map)
	{
		const auto& stdCode = m.first;
		if (target_pos.find(stdCode) == target_pos.end())
		{
			if(!decimal::eq(m.second->_volume, 0))
			{
				//这里是通知WtEngine去更新组合持仓数据
				append_signal(stdCode.c_str(), 0, true);

				WTSLogger::error("Instrument {} not in target positions, setup to 0 automatically", stdCode.c_str());
			}

			//因为组合持仓里会有过期的合约代码存在，所以这里在丢给执行以前要做一个检查
			auto cInfo = get_contract_info(stdCode.c_str());
			if (cInfo != NULL)
			{
				//target_pos[stdCode] = 0;
				_exec_mgr.add_target_to_cache(stdCode.c_str(), 0);
			}
		}
	}

	push_task([this](){
		update_fund_dynprofit();
		/*
		 *	By Wesley @ 2023.01.30
		 *	增加一个定时刷新交易账号资金的入口
		 */
		_adapter_mgr->refresh_funds();
	});

	//_exec_mgr.set_positions(target_pos);
	_exec_mgr.commit_cached_targets(bRiskEnabled ? _risk_volscale : 1);

	save_datas();

	if (_evt_listener)
		_evt_listener->on_schedule_event(curDate, curTime);
}


void WtCtaEngine::handle_push_quote(WTSTickData* newTick)
{
	if (_tm_ticker)
		_tm_ticker->on_tick(newTick);
}

void WtCtaEngine::handle_pos_change(const char* straName, const char* stdCode, double diffPos)
{
	//这里是持仓增量,所以不用处理未过滤的情况,因为增量情况下,不会改变目标diffQty
	if(_filter_mgr.is_filtered_by_strategy(straName, diffPos, true))
	{
		//输出日志
		WTSLogger::info("[Filters] Target position of {} of strategy {} ignored by strategy filter", stdCode, straName);
		return;
	}

	std::string realCode = stdCode;
	//const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _cur_tdate);
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
	}

	/*
	 *	这里必须要算一个总的目标仓位
	 */
	PosInfoPtr& pInfo = _pos_map[realCode];	
	if (pInfo == NULL)
		pInfo.reset(new PosInfo);

	bool bRiskEnabled = false;
	if (!decimal::eq(_risk_volscale, 1.0) && _risk_date == _cur_tdate)
	{
		WTSLogger::log_by_cat("risk", LL_INFO, "Risk scale of portfolio is {:.2f}", _risk_volscale);
		bRiskEnabled = true;
	}

	if (bRiskEnabled && !decimal::eq(diffPos, 0))
	{
		double symbol = diffPos / abs(diffPos);
		diffPos = decimal::rnd(abs(diffPos)*_risk_volscale)*symbol;
	}

	double targetPos = pInfo->_volume + diffPos;

	append_signal(realCode.c_str(), targetPos, false);
	save_datas();

	/*
	 *	如果策略绑定了执行通道
	 *	那么就只提交增量
	 *	如果策略没有绑定执行通道，就提交全量
	 */
	const auto& exec_ids = _exec_mgr.get_route(straName);
	for(auto& execid : exec_ids)
		_exec_mgr.handle_pos_change(realCode.c_str(), targetPos, diffPos, execid.c_str());
}

void WtCtaEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	WtEngine::on_tick(stdCode, curTick);

	_data_mgr->handle_push_quote(stdCode, curTick);

	//如果是真实代码, 则要传递给执行器
	/*
	 *	这里不再做判断，直接全部传递给执行器管理器，因为执行器可能会处理未订阅的合约
	 *	主要场景为主力合约换月期间
	 *	By Wesley @ 2021.08.19
	 */
	{
		//是否主力合约代码的标记, 主要用于给执行器发数据的
		_exec_mgr.handle_tick(stdCode, curTick);
	}

	/*
	 *	By Wesley @ 2022.02.07
	 *	这里做了一个彻底的调整
	 *	第一，检查订阅标记，如果标记为0，即无复权模式，则直接按照原始代码触发ontick
	 *	第二，如果标记为1，即前复权模式，则将代码转成xxxx-，再触发ontick
	 *	第三，如果标记为2，即后复权模式，则将代码转成xxxx+，再把tick数据做一个修正，再触发ontick
	 */
	if(_ready)
	{
		auto sit = _tick_sub_map.find(stdCode);
		if (sit == _tick_sub_map.end())
			return;

		uint32_t flag = get_adjusting_flag();
		WTSTickData* adjTick = nullptr;

		//By Wesley
		//这里做一个拷贝，虽然有点开销，但是可以规避掉一些问题，比如ontick的时候订阅tick
		SubList sids = sit->second;
		for (auto it = sids.begin(); it != sids.end(); it++)
		{
			uint32_t sid = it->first;
				

			auto cit = _ctx_map.find(sid);
			if (cit != _ctx_map.end())
			{
				CtaContextPtr& ctx = (CtaContextPtr&)cit->second;
				uint32_t opt = it->second.second;
					
				if (opt == 0)
				{
					/*
					 *	By Wesley @ 2023.06.27
					 *	如果使用线程池，则到线程池里去调度
					 */
					if(_pool)
					{
						_pool->schedule([ctx, stdCode, curTick]() {
							ctx->on_tick(stdCode, curTick);
						});
					}
					else
						ctx->on_tick(stdCode, curTick);
				}
				else
				{
					std::string wCode = stdCode;
					wCode = fmt::format("{}{}", stdCode, opt == 1 ? SUFFIX_QFQ : SUFFIX_HFQ);
					if (opt == 1)
					{
						if (_pool)
						{
							_pool->schedule([ctx, wCode, curTick]() {
								ctx->on_tick(wCode.c_str(), curTick);
							});
						}
						else
							ctx->on_tick(wCode.c_str(), curTick);
					}
					else //(opt == 2)
					{
						if (adjTick == nullptr)
						{
							WTSTickData* adjTick = WTSTickData::create(curTick->getTickStruct());
							WTSTickStruct& adjTS = adjTick->getTickStruct();
							adjTick->setContractInfo(curTick->getContractInfo());

							//这里做一个复权因子的处理
							double factor = get_exright_factor(stdCode);
							adjTS.open *= factor;
							adjTS.high *= factor;
							adjTS.low *= factor;
							adjTS.price *= factor;

							adjTS.settle_price *= factor;

							adjTS.pre_close *= factor;
							adjTS.pre_settle *= factor;

							/*
							 *	By Wesley @ 2022.08.15
							 *	这里对tick的复权做一个完善
							 */
							if (flag & 1)
							{
								adjTS.total_volume /= factor;
								adjTS.volume /= factor;
							}

							if (flag & 2)
							{
								adjTS.total_turnover *= factor;
								adjTS.turn_over *= factor;
							}

							if (flag & 4)
							{
								adjTS.open_interest /= factor;
								adjTS.diff_interest /= factor;
								adjTS.pre_interest /= factor;
							}

							_price_map[wCode] = adjTS.price;
						}

						if (_pool)
						{
							_pool->schedule([ctx, wCode, adjTick]() {
								ctx->on_tick(wCode.c_str(), adjTick);
							});
						}
						else
							ctx->on_tick(wCode.c_str(), adjTick);

					}
				}
			}				
		}

		if(nullptr != adjTick)
			adjTick->release();
		/*
		 *	By Wesley @ 223.06.27
		 *	这里一定要等待线程池全部调度完成
		 */
		if (_pool)
			_pool->wait();
	}
	
}

void WtCtaEngine::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}-{}-{}", stdCode, period, times);

	const SubList& sids = _bar_sub_map[key];
	for (auto it = sids.begin(); it != sids.end(); it++)
	{
		uint32_t sid = it->first;
		auto cit = _ctx_map.find(sid);
		if(cit != _ctx_map.end())
		{
			CtaContextPtr& ctx = (CtaContextPtr&)cit->second;
			if (_pool)
			{
				_pool->schedule([ctx, stdCode, period, times, newBar]() {
					ctx->on_bar(stdCode, period, times, newBar);
				});
			}
			else
				ctx->on_bar(stdCode, period, times, newBar);
		}
	}

	/*
	 *	By Wesley @ 223.06.27
	 *	这里一定要等待线程池全部调度完成
	 */
	if (_pool)
		_pool->wait();

	WTSLogger::info("KBar [{}] @ {} closed", key, period[0] == 'd' ? newBar->date : newBar->time);
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
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	return _base_data_mgr->getCommodity(codeInfo._exchg, codeInfo._product);
}

WTSSessionInfo* WtCtaEngine::get_sess_info(const char* stdCode)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(codeInfo._exchg, codeInfo._product);
	if (cInfo == NULL)
		return NULL;

	return cInfo->getSessionInfo();
}

uint64_t WtCtaEngine::get_real_time()
{
	return TimeUtils::makeTime(_cur_date, _cur_raw_time * 100000 + _cur_secs);
}

void WtCtaEngine::notify_chart_marker(uint64_t time, const char* straId, double price, const char* icon, const char* tag)
{
	if (_notifier)
		_notifier->notify_chart_marker(time, straId, price, icon, tag);
}

void WtCtaEngine::notify_chart_index(uint64_t time, const char* straId, const char* idxName, const char* lineName, double val)
{
	if (_notifier)
		_notifier->notify_chart_index(time, straId, idxName, lineName, val);
}

void WtCtaEngine::notify_trade(const char* straId, const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, const char* userTag)
{
	if (_notifier)
		_notifier->notify_trade(straId, stdCode, isLong, isOpen, curTime, price, userTag);
}