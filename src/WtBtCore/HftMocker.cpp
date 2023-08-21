/*!
 * \file HftMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "HftMocker.h"
#include "WtHelper.h"

#include <stdarg.h>

#include <boost/filesystem.hpp>

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

uint32_t makeLocalOrderID()
{
	static std::atomic<uint32_t> _auto_order_id{ 0 };
	if (_auto_order_id == 0)
	{
		uint32_t curYear = TimeUtils::getCurDate() / 10000 * 10000 + 101;
		_auto_order_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(curYear, 0)) / 1000 * 50);
	}

	return _auto_order_id.fetch_add(1);
}

std::vector<uint32_t> splitVolume(uint32_t vol)
{
	if (vol == 0) return std::move(std::vector<uint32_t>());

	uint32_t minQty = 1;
	uint32_t maxQty = 100;
	uint32_t length = maxQty - minQty + 1;
	std::vector<uint32_t> ret;
	if (vol <= minQty)
	{
		ret.emplace_back(vol);
	}
	else
	{
		uint32_t left = vol;
		srand((uint32_t)time(NULL));
		while (left > 0)
		{
			uint32_t curVol = minQty + (uint32_t)rand() % length;

			if (curVol >= left)
				curVol = left;

			if (curVol == 0)
				continue;

			ret.emplace_back(curVol);
			left -= curVol;
		}
	}

	return std::move(ret);
}

std::vector<double> splitVolume(double vol, double minQty = 1.0, double maxQty = 100.0, double qtyTick = 1.0)
{
	auto length = (std::size_t)round((maxQty - minQty)/qtyTick) + 1;
	std::vector<double> ret;
	if (vol <= minQty)
	{
		ret.emplace_back(vol);
	}
	else
	{
		double left = vol;
		srand((uint32_t)time(NULL));
		while (left > 0)
		{
			double curVol = minQty + (rand() % length)*qtyTick;

			if (curVol >= left)
				curVol = left;

			if (curVol == 0)
				continue;

			ret.emplace_back(curVol);
			left -= curVol;
		}
	}

	return std::move(ret);
}

uint32_t genRand(uint32_t maxVal = 10000)
{
	srand(TimeUtils::getCurMin());
	return rand() % maxVal;
}

inline uint32_t makeHftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

HftMocker::HftMocker(HisDataReplayer* replayer, const char* name)
	: IHftStraCtx(name)
	, _replayer(replayer)
	, _strategy(NULL)
	, _use_newpx(false)
	, _error_rate(0)
	, _match_this_tick(false)
	, _has_hook(false)
	, _hook_valid(true)
	, _resumed(false)
{
	_commodities = CommodityMap::create();

	_context_id = makeHftCtxId();

	_ticks = TickCache::create();
}


HftMocker::~HftMocker()
{
	if(_strategy)
	{
		_factory._fact->deleteStrategy(_strategy);
	}

	_commodities->release();

	_ticks->release();
	_ticks = NULL;
}

void HftMocker::procTask()
{
	if (_tasks.empty())
	{
		return;
	}

	_mtx_control.lock();

	while (!_tasks.empty())
	{
		Task& task = _tasks.front();

		task();

		{
			std::unique_lock<std::mutex> lck(_mtx);
			_tasks.pop();
		}
	}

	_mtx_control.unlock();
}

void HftMocker::postTask(Task task)
{
	{
		std::unique_lock<std::mutex> lck(_mtx);
		_tasks.push(task);
		return;
	}

	//if(_thrd == NULL)
	//{
	//	_thrd.reset(new std::thread([this](){
	//		while (!_stopped)
	//		{
	//			if(_tasks.empty())
	//			{
	//				std::this_thread::sleep_for(std::chrono::milliseconds(1));
	//				continue;
	//			}

	//			_mtx_control.lock();

	//			while(!_tasks.empty())
	//			{
	//				Task& task = _tasks.front();

	//				task();

	//				{
	//					std::unique_lock<std::mutex> lck(_mtx);
	//					_tasks.pop();
	//				}
	//			}

	//			_mtx_control.unlock();
	//		}
	//	}));
	//}
}

bool HftMocker::init_hft_factory(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	const char* module = cfg->getCString("module");
	
	_use_newpx = cfg->getBoolean("use_newpx");
	_error_rate = cfg->getUInt32("error_rate");
	_match_this_tick = cfg->getBoolean("match_this_tick");

	log_info("HFT match params: use_newpx-{}, error_rate-{}, match_this_tick-{}", _use_newpx, _error_rate, _match_this_tick);

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateHftStraFact creator = (FuncCreateHftStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteHftStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgStra = cfg->get("strategy");
	if(cfgStra)
	{
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), cfgStra->getCString("id"));
		_strategy->init(cfgStra->get("params"));
		_name = _strategy->id();
	}
	return true;
}

void HftMocker::handle_tick(const char* stdCode, WTSTickData* curTick, uint32_t pxType)
{
	on_tick(stdCode, curTick);
}

void HftMocker::handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl)
{
	on_order_detail(stdCode, curOrdDtl);
}

void HftMocker::handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue)
{
	on_order_queue(stdCode, curOrdQue);
}

void HftMocker::handle_transaction(const char* stdCode, WTSTransData* curTrans)
{
	on_transaction(stdCode, curTrans);
}

void HftMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	on_bar(stdCode, period, times, newBar);
}

void HftMocker::handle_init()
{
	on_init();
	on_channel_ready();
}

void HftMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	//on_schedule(uDate, uTime);
}

void HftMocker::handle_session_begin(uint32_t curTDate)
{
	on_session_begin(curTDate);
}

void HftMocker::handle_session_end(uint32_t curTDate)
{
	on_session_end(curTDate);
}

void HftMocker::handle_replay_done()
{
	dump_outputs();

	this->on_bactest_end();
}

void HftMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, times, newBar);
}

void HftMocker::enable_hook(bool bEnabled /* = true */)
{
	_hook_valid = bEnabled;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calculating hook {}", bEnabled ? "enabled" : "disabled");
}

void HftMocker::install_hook()
{
	_has_hook = true;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "HFT hook installed");
}

void HftMocker::step_tick()
{
	if (!_has_hook)
		return;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Notify calc thread, wait for calc done");
	while (!_resumed)
		_cond_calc.notify_all();

	{
		StdUniqueLock lock(_mtx_calc);
		_cond_calc.wait(_mtx_calc);
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done notified");
		_resumed = false;
	}
}

void HftMocker::on_tick(const char* stdCode, WTSTickData* newTick)
{
	_price_map[stdCode] = newTick->price();
	{
		std::unique_lock<std::recursive_mutex> lck(_mtx_control);
	}

	update_dyn_profit(stdCode, newTick);

	OrderIDs all_ids;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
		all_ids.push_back(it->first);
	//如果开启了同tick撮合，则先触发策略的ontick，再处理订单
	//如果没开启同tick撮合，则先处理订单，再触发策略的ontick
	if (_match_this_tick)
	{
		if (_has_hook && _hook_valid)
		{
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
			StdUniqueLock lock(_mtx_calc);
			_cond_calc.wait(_mtx_calc);
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
			_resumed = true;
		}

		on_tick_updated(stdCode, newTick);

		procTask();

		if (!_orders.empty())
		{
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			OrderIDs ids;
			for (uint32_t localid : all_ids)
			{
				bool bNeedErase = procOrder(localid);
				if (bNeedErase)
					ids.emplace_back(localid);
			}

			for (uint32_t localid : ids)
			{
				_orders.erase(localid);
			}
		}
	}
	else
	{
		if (!_orders.empty())
		{
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			OrderIDs ids;
			for (uint32_t localid : all_ids)
			{
				bool bNeedErase = procOrder(localid);
				if (bNeedErase)
					ids.emplace_back(localid);
			}

			for (uint32_t localid : ids)
			{
				auto it = _orders.find(localid);
				_orders.erase(it);
			}
		}

		if (_has_hook && _hook_valid)
		{
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
			StdUniqueLock lock(_mtx_calc);
			_cond_calc.wait(_mtx_calc);
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
			_resumed = true;
		}

		on_tick_updated(stdCode, newTick);

		procTask();
	}

	if (_has_hook && _hook_valid)
	{
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done, notify control thread");
		while (_resumed)
			_cond_calc.notify_all();
	}
}

void HftMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void HftMocker::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	on_ordque_updated(stdCode, newOrdQue);
}

void HftMocker::on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);
}

void HftMocker::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	on_orddtl_updated(stdCode, newOrdDtl);
}

void HftMocker::on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);
}

void HftMocker::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	on_trans_updated(stdCode, newTrans);
}

void HftMocker::on_trans_updated(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);
}

uint32_t HftMocker::id()
{
	return _context_id;
}

void HftMocker::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}

void HftMocker::on_session_begin(uint32_t curTDate)
{
	//每个交易日开始，要把冻结持仓置零
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosInfo& pInfo = (PosInfo&)it.second;
		if (!decimal::eq(pInfo._frozen, 0))
		{
			log_debug("{} of {} frozen released on {}", pInfo._frozen, stdCode, curTDate);
			pInfo._frozen = 0;
		}
	}

	if (_strategy)
		_strategy->on_session_begin(this, curTDate);
}

void HftMocker::on_session_end(uint32_t curTDate)
{
	uint32_t curDate = curTDate;// _replayer->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;

		if (decimal::eq(pInfo._volume, 0.0))
			continue;

		_pos_logs << fmt::format("{},{},{},{:.2f},{:.2f}\n", curTDate, stdCode,
			pInfo._volume, pInfo._closeprofit, pInfo._dynprofit);
	}

	_fund_logs << fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curTDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);

	if (_strategy)
		_strategy->on_session_end(this, curTDate);
}

double HftMocker::stra_get_undone(const char* stdCode)
{
	double ret = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		OrderInfoPtr ordInfo = it->second;
		if (strcmp(ordInfo->_code, stdCode) == 0)
		{
			ret += ordInfo->_left * ordInfo->_isBuy ? 1 : -1;
		}
	}

	return ret;
}

bool HftMocker::stra_cancel(uint32_t localid)
{
	postTask([this, localid](){
		OrderInfoPtr ordInfo = NULL;
		{
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			auto it = _orders.find(localid);
			if (it == _orders.end())
				return;

			ordInfo = it->second;
		}
		
		ordInfo->_left = 0;

		on_order(localid, ordInfo->_code, ordInfo->_isBuy, ordInfo->_total, ordInfo->_left, ordInfo->_price, true, ordInfo->_usertag);

		{
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			_orders.erase(localid);
		}
	});

	return true;
}

OrderIDs HftMocker::stra_cancel(const char* stdCode, bool isBuy, double qty /* = 0 */)
{
	OrderIDs ret;
	uint32_t cnt = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		OrderInfoPtr ordInfo = it->second;
		if(ordInfo->_isBuy == isBuy && strcmp(ordInfo->_code, stdCode) == 0)
		{
			double left = ordInfo->_left;
			stra_cancel(it->first);
			ret.emplace_back(it->first);
			cnt++;
			if (left < qty)
				qty -= left;
			else
				break;
		}
	}

	return ret;
}

OrderIDs HftMocker::stra_buy(const char* stdCode, double price, double qty, const char* userTag, int flag /* = 0 */, bool bForceClose /* = false */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return OrderIDs();
	}

	if (decimal::le(qty, 0))
	{
		log_error("Entrust error: qty {} <= 0", qty);
		return OrderIDs();
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfoPtr order(new OrderInfo);
	order->_localid = localid;
	strcpy(order->_code, stdCode);
	strcpy(order->_usertag, userTag);
	order->_isBuy = true;
	order->_price = price;
	order->_total = qty;
	order->_left = qty;

	{
		StdLocker<StdRecurMutex> lock(_mtx_ords);
		_orders[localid] = order;		
	}

	postTask([this, localid](){
		const OrderInfoPtr& ordInfo = _orders[localid];
		on_entrust(localid, ordInfo->_code, true, "下单成功", ordInfo->_usertag);
	});

	OrderIDs ids;
	ids.emplace_back(localid);
	return ids;
}

void HftMocker::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */, const char* userTag /* = "" */)
{
	if(_strategy)
		_strategy->on_order(this, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled, userTag);
}

void HftMocker::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag/* = ""*/)
{
	const PosInfo& posInfo = _pos_map[stdCode];
	double curPos = posInfo._volume + vol * (isBuy ? 1 : -1);
	do_set_position(stdCode, curPos, price, userTag);
	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isBuy, vol, price, userTag);
}

void HftMocker::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag/* = ""*/)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message, userTag);
}

void HftMocker::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);
}

void HftMocker::update_dyn_profit(const char* stdCode, WTSTickData* newTick)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		PosInfo& pInfo = (PosInfo&)it->second;
		if (pInfo._volume == 0)
		{
			pInfo._dynprofit = 0;
		}
		else
		{
			bool isLong = decimal::gt(pInfo._volume, 0);
			double price = isLong ? newTick->bidprice(0) : newTick->askprice(0);

			WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{
				
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}
}

bool HftMocker::procOrder(uint32_t localid)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return false;

	OrderInfoPtr ordInfo = it->second;

	//第一步,如果在撤单概率中,则执行撤单
	if(_error_rate>0 && genRand(10000)<=_error_rate)
	{
		on_order(localid, ordInfo->_code, ordInfo->_isBuy, ordInfo->_total, ordInfo->_left, ordInfo->_price, true, ordInfo->_usertag);
		log_info("Random error order: {}", localid);
		return true;
	}
	else if(!ordInfo->_proced_after_placed)
	{
		//如果下单以后，还没处理过，则触发on_order
		on_order(localid, ordInfo->_code, ordInfo->_isBuy, ordInfo->_total, ordInfo->_left, ordInfo->_price, false, ordInfo->_usertag);
		ordInfo->_proced_after_placed = true;
	}

	WTSTickData* curTick = stra_get_last_tick(ordInfo->_code);
	if (curTick == NULL)
		return false;

	double curPx = curTick->price();
	double orderQty = ordInfo->_isBuy ? curTick->askqty(0) : curTick->bidqty(0);	//看对手盘的数量
	if (decimal::eq(orderQty, 0.0))
		return false;

	if (!_use_newpx)
	{
		curPx = ordInfo->_isBuy ? curTick->askprice(0) : curTick->bidprice(0);
		//if (curPx == 0.0)
		if(decimal::eq(curPx, 0.0))
		{
			curTick->release();
			return false;
		}
	}
	curTick->release();

	//如果没有成交条件,则退出逻辑
	if(!decimal::eq(ordInfo->_price, 0.0))
	{
		if(ordInfo->_isBuy && decimal::gt(curPx, ordInfo->_price))
		{
			//买单,但是当前价大于限价,不成交
			return false;
		}

		if (!ordInfo->_isBuy && decimal::lt(curPx, ordInfo->_price))
		{
			//卖单,但是当前价小于限价,不成交
			return false;
		}
	}

	/*
	 *	下面就要模拟成交了
	 */
	double maxQty = min(orderQty, ordInfo->_left);
	auto vols = splitVolume((uint32_t)maxQty);
	for(uint32_t curQty : vols)
	{
		on_trade(ordInfo->_localid, ordInfo->_code, ordInfo->_isBuy, curQty, curPx, ordInfo->_usertag);

		ordInfo->_left -= curQty;
		on_order(localid, ordInfo->_code, ordInfo->_isBuy, ordInfo->_total, ordInfo->_left, ordInfo->_price, false, ordInfo->_usertag);

		double curPos = stra_get_position(ordInfo->_code);

		_sig_logs << _replayer->get_date() << "." << _replayer->get_raw_time() << "." << _replayer->get_secs() << ","
			<< (ordInfo->_isBuy ? "+" : "-") << curQty << "," << curPos << "," << curPx << std::endl;
	}

	//if(ordInfo->_left == 0)
	if(decimal::eq(ordInfo->_left, 0.0))
	{
		return true;
	}

	return false;
}

OrderIDs HftMocker::stra_sell(const char* stdCode, double price, double qty, const char* userTag, int flag /* = 0 */, bool bForceClose /* = false */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of %s", stdCode);
		return OrderIDs();
	}

	if (decimal::le(qty, 0))
	{
		log_error("Entrust error: qty {} <= 0", qty);
		return OrderIDs();
	}

	//如果不能做空，则要看可用持仓
	if(!commInfo->canShort())
	{
		double curPos = stra_get_position(stdCode, true);//只读可用持仓
		if(decimal::gt(qty, curPos))
		{
			log_error("No enough position of {} to sell", stdCode);
			return OrderIDs();
		}
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfoPtr order(new OrderInfo);
	order->_localid = localid;
	strcpy(order->_code, stdCode);
	strcpy(order->_usertag, userTag);
	order->_isBuy = false;
	order->_price = price;
	order->_total = qty;
	order->_left = qty;

	{
		StdLocker<StdRecurMutex> lock(_mtx_ords);
		_orders[localid] = order;
	}

	postTask([this, localid]() {
		const OrderInfoPtr& ordInfo = _orders[localid];
		on_entrust(localid, ordInfo->_code, true, "下单成功", ordInfo->_usertag);
	});

	OrderIDs ids;
	ids.emplace_back(localid);
	return ids;
}

WTSCommodityInfo* HftMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

std::string HftMocker::stra_get_rawcode(const char* stdCode)
{
	return _replayer->get_rawcode(stdCode);
}

WTSKlineSlice* HftMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	return _replayer->get_kline_slice(stdCode, basePeriod, count, times);
}

WTSTickSlice* HftMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSOrdQueSlice* HftMocker::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	return _replayer->get_order_queue_slice(stdCode, count);
}

WTSOrdDtlSlice* HftMocker::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	return _replayer->get_order_detail_slice(stdCode, count);
}

WTSTransSlice* HftMocker::stra_get_transaction(const char* stdCode, uint32_t count)
{
	return _replayer->get_transaction_slice(stdCode, count);
}

WTSTickData* HftMocker::stra_get_last_tick(const char* stdCode)
{
	if (_ticks != NULL)
	{
		auto it = _ticks->find(stdCode);
		if (it != _ticks->end())
		{
			WTSTickData* lastTick = (WTSTickData*)it->second;
			if (lastTick)
				lastTick->retain();
			return lastTick;
		}
	}

	return _replayer->get_last_tick(stdCode);
}

double HftMocker::stra_get_position(const char* stdCode, bool bOnlyValid/* = false*/, int flag/* = 3*/)
{
	const PosInfo& pInfo = _pos_map[stdCode];
	if (bOnlyValid)
	{
		//这里理论上，只有多头才会进到这里
		//其他地方要保证，空头持仓的话，_frozen要为0
		return pInfo._volume - pInfo._frozen;
	}
	else
		return pInfo._volume;
}

double HftMocker::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

double HftMocker::stra_get_position_avgpx(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	if (decimal::eq(pInfo._volume, 0.0))
		return 0;

	double amount = 0.0;
	for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
	{
		const DetailInfo& dInfo = *dit;
		amount += dInfo._price*dInfo._volume;
	}

	return amount / pInfo._volume;
}

double HftMocker::stra_get_price(const char* stdCode)
{
	return _replayer->get_cur_price(stdCode);
}

uint32_t HftMocker::stra_get_date()
{
	return _replayer->get_date();
}

uint32_t HftMocker::stra_get_time()
{
	return _replayer->get_raw_time();
}

uint32_t HftMocker::stra_get_secs()
{
	return _replayer->get_secs();
}

void HftMocker::stra_sub_ticks(const char* stdCode)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	主动订阅tick会在本地记一下
	 *	tick数据回调的时候先检查一下
	 */
	_tick_subs.insert(stdCode);

	_replayer->sub_tick(_context_id, stdCode);
}

void HftMocker::stra_sub_order_queues(const char* stdCode)
{
	_replayer->sub_order_queue(_context_id, stdCode);
}

void HftMocker::stra_sub_order_details(const char* stdCode)
{
	_replayer->sub_order_detail(_context_id, stdCode);
}

void HftMocker::stra_sub_transactions(const char* stdCode)
{
	_replayer->sub_transaction(_context_id, stdCode);
}

void HftMocker::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void HftMocker::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void HftMocker::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void HftMocker::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

const char* HftMocker::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void HftMocker::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

void HftMocker::dump_outputs()
{
	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());

	std::string filename = folder + "trades.csv";
	std::string content = "code,time,direct,action,price,qty,fee,usertag\n";
	content += _trade_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "closes.csv";
	content = "code,direct,opentime,openprice,closetime,closeprice,qty,profit,maxprofit,maxloss,totalprofit,entertag,exittag\n";
	content += _close_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());


	filename = folder + "funds.csv";
	content = "date,closeprofit,positionprofit,dynbalance,fee\n";
	content += _fund_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());


	filename = folder + "signals.csv";
	content = "time, action, position, price\n";
	content += _sig_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "positions.csv";
	content = "date,code,volume,closeprofit,dynprofit\n";
	if (!_pos_logs.str().empty()) content += _pos_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();
		for (auto it = _user_datas.begin(); it != _user_datas.end(); it++)
		{
			root.AddMember(rj::Value(it->first.c_str(), allocator), rj::Value(it->second.c_str(), allocator), allocator);
		}

		filename = folder;
		filename += "ud_";
		filename += _name;
		filename += ".json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}
}

void HftMocker::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee, const char* userTag/* = ""*/)
{
	_trade_logs << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE")
		<< "," << price << "," << qty << "," << fee << "," << userTag << "\n";
}

void HftMocker::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double maxprofit, double maxloss,
	double totalprofit /* = 0 */, const char* enterTag/* = ""*/, const char* exitTag/* = ""*/)
{
	_close_logs << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
		<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," << maxprofit << "," << maxloss << ","
		<< totalprofit << "," << enterTag << "," << exitTag << "\n";
}

void HftMocker::do_set_position(const char* stdCode, double qty, double price /* = 0.0 */, const char* userTag /*= ""*/)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_min_time()*100000 + _replayer->get_secs();
	uint32_t curTDate = _replayer->get_trading_date();

	//手数相等则不用操作了
	if (decimal::eq(pInfo._volume, qty))
		return;

	log_debug("[{:04d}.{:05d}] {} position updated: {} -> {}", _replayer->get_min_time(), _replayer->get_secs(), stdCode, pInfo._volume, qty);

	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
		return;

	//成交价
	double trdPx = curPx;

	double diff = qty - pInfo._volume;
	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))//当前持仓和仓位变化方向一致, 增加一条明细, 增加数量即可
	{
		pInfo._volume = qty;
		//如果T+1，则冻结仓位要增加
		if (commInfo->isT1())
		{
			//ASSERT(diff>0);
			pInfo._frozen += diff;
			log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
		}

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = trdPx;
		dInfo._volume = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		strcpy(dInfo._usertag, userTag);
		pInfo._details.emplace_back(dInfo);

		double fee = _replayer->calc_fee(stdCode, trdPx, abs(diff), 0);
		_fund_info._total_fees += fee;

		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(diff), fee, userTag);
	}
	else
	{//持仓方向和仓位变化方向不一致,需要平仓
		double left = abs(diff);

		pInfo._volume = qty;
		if (decimal::eq(pInfo._volume, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			double maxQty = min(dInfo._volume, left);
			if (decimal::eq(maxQty, 0))
				continue;

			double maxProf = dInfo._max_profit * maxQty / dInfo._volume;
			double maxLoss = dInfo._max_loss * maxQty / dInfo._volume;

			dInfo._volume -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//浮盈也要做等比缩放
			_fund_info._total_profit += profit;

			double fee = _replayer->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, fee, userTag);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, pInfo._closeprofit, dInfo._usertag, userTag);

			if (left == 0)
				break;
		}

		//需要清理掉已经平仓完的明细
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//最后,如果还有剩余的,则需要反手了
		if (left > 0)
		{
			left = left * qty / abs(qty);

			//如果T+1，则冻结仓位要增加
			if (commInfo->isT1())
			{
				pInfo._frozen += left;
				log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
			}

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = trdPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			strcpy(dInfo._usertag, userTag);
			pInfo._details.emplace_back(dInfo);

			//这里还需要写一笔成交记录
			double fee = _replayer->calc_fee(stdCode, trdPx, abs(left), 0);
			_fund_info._total_fees += fee;
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), fee, userTag);
		}
	}
}
