/*!
 * \file UftMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UftMocker.h"
#include "WtHelper.h"

#include <stdarg.h>

#include <boost/filesystem.hpp>

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSLogger.h"

extern uint32_t makeLocalOrderID();

const char* OFFSET_NAMES[] =
{
	"OPEN",
	"CLOSE",
	"CLOSET"
};

extern std::vector<uint32_t> splitVolume(uint32_t vol);
extern std::vector<double> splitVolume(double vol, double minQty = 1.0, double maxQty = 100.0, double qtyTick = 1.0);

extern uint32_t genRand(uint32_t maxVal = 10000);

inline uint32_t makeUftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 7000 };
	return _auto_context_id.fetch_add(1);
}

UftMocker::UftMocker(HisDataReplayer* replayer, const char* name)
	: IUftStraCtx(name)
	, _replayer(replayer)
	, _strategy(NULL)
	, _use_newpx(false)
	, _error_rate(0)
	, _match_this_tick(false)
{
	_context_id = makeUftCtxId();
}


UftMocker::~UftMocker()
{
	if(_strategy)
	{
		_factory._fact->deleteStrategy(_strategy);
	}
}

void UftMocker::procTask()
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

void UftMocker::postTask(Task task)
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

bool UftMocker::init_uft_factory(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	const char* module = cfg->getCString("module");
	
	_use_newpx = cfg->getBoolean("use_newpx");
	_error_rate = cfg->getUInt32("error_rate");
	_match_this_tick = cfg->getBoolean("match_this_tick");

	log_info("UFT match params: use_newpx-{}, error_rate-{}, match_this_tick-{}", _use_newpx, _error_rate, _match_this_tick);

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateUftStraFact creator = (FuncCreateUftStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteUftStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgStra = cfg->get("strategy");
	if(cfgStra)
	{
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), "uft");
		_strategy->init(cfgStra->get("params"));
	}
	return true;
}

void UftMocker::handle_tick(const char* stdCode, WTSTickData* curTick, uint32_t pxType)
{
	on_tick(stdCode, curTick);
}

void UftMocker::handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl)
{
	on_order_detail(stdCode, curOrdDtl);
}

void UftMocker::handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue)
{
	on_order_queue(stdCode, curOrdQue);
}

void UftMocker::handle_transaction(const char* stdCode, WTSTransData* curTrans)
{
	on_transaction(stdCode, curTrans);
}

void UftMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	on_bar(stdCode, period, times, newBar);
}

void UftMocker::handle_init()
{
	on_init();
	on_channel_ready();
}

void UftMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	//on_schedule(uDate, uTime);
}

void UftMocker::handle_session_begin(uint32_t curTDate)
{
	on_session_begin(curTDate);
}

void UftMocker::handle_session_end(uint32_t curTDate)
{
	on_session_end(curTDate);
}

void UftMocker::handle_replay_done()
{
	dump_outputs();

	this->on_bactest_end();
}

void UftMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, times, newBar);
}

void UftMocker::on_tick(const char* stdCode, WTSTickData* newTick)
{
	_price_map[stdCode] = newTick->price();
	{
		std::unique_lock<std::recursive_mutex> lck(_mtx_control);
	}

	update_dyn_profit(stdCode, newTick);
	
	//如果开启了同tick撮合，则先触发策略的ontick，再处理订单
	//如果没开启同tick撮合，则先处理订单，再触发策略的ontick
	if(_match_this_tick)
	{
		on_tick_updated(stdCode, newTick);

		procTask();

		if (!_orders.empty())
		{
			OrderIDs ids;
			for (auto it = _orders.begin(); it != _orders.end(); it++)
			{
				uint32_t localid = it->first;
				ids.emplace_back(localid);
			}

			OrderIDs to_erase;
			for (uint32_t localid : ids)
			{
				bool bNeedErase = procOrder(localid);
				if (bNeedErase)
					to_erase.emplace_back(localid);
			}

			for (uint32_t localid : to_erase)
			{
				auto it = _orders.find(localid);
				_orders.erase(it);
			}
		}
	}
	else
	{
		if (!_orders.empty())
		{
			OrderIDs ids;
			for (auto it = _orders.begin(); it != _orders.end(); it++)
			{
				uint32_t localid = it->first;
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

		on_tick_updated(stdCode, newTick);

		procTask();
	}
}

void UftMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void UftMocker::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	on_ordque_updated(stdCode, newOrdQue);
}

void UftMocker::on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);
}

void UftMocker::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	on_orddtl_updated(stdCode, newOrdDtl);
}

void UftMocker::on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);
}

void UftMocker::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	on_trans_updated(stdCode, newTrans);
}

void UftMocker::on_trans_updated(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);
}

uint32_t UftMocker::id()
{
	return _context_id;
}

void UftMocker::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}

void UftMocker::on_session_begin(uint32_t curTDate)
{
	//每个交易日开始，要把冻结持仓置零
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosItem& pInfo = (PosItem&)it.second;
		if (!decimal::eq(pInfo.frozen(), 0))
		{
			log_debug("{} frozen of {} released on {}", pInfo.frozen(), stdCode, curTDate);
			pInfo._prevol += pInfo._newvol;
			pInfo._preavail = pInfo._prevol;

			pInfo._newvol = 0;
			pInfo._newavail = 0;
		}
	}

	_strategy->on_session_begin(this, curTDate);
}

void UftMocker::on_session_end(uint32_t curTDate)
{
	_strategy->on_session_end(this, curTDate);

	uint32_t curDate = curTDate;// _replayer->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo.closeprofit();
		total_dynprofit += pInfo.dynprofit();

		if (!decimal::eq(pInfo._long.volume(), 0.0))
			_pos_logs << fmt::format("{},{},LONG,{},{:.2f},{:.2f}\n", curTDate, stdCode, pInfo._long.volume(), pInfo._long._closeprofit, pInfo._long._dynprofit);

		if (!decimal::eq(pInfo._short.volume(), 0.0))
			_pos_logs << fmt::format("{},{},LONG,{},{:.2f},{:.2f}\n", curTDate, stdCode, pInfo._short.volume(), pInfo._short._closeprofit, pInfo._short._dynprofit);
	}

	_fund_logs << fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);
}

double UftMocker::stra_get_undone(const char* stdCode)
{
	double ret = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		const OrderInfo& ordInfo = it->second;
		if (strcmp(ordInfo._code, stdCode) == 0)
		{
			ret += ordInfo._left * ordInfo._isLong ? 1 : -1;
		}
	}

	return ret;
}

bool UftMocker::stra_cancel(uint32_t localid)
{
	postTask([this, localid](){
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		OrderInfo& ordInfo = (OrderInfo&)it->second;
		
		if (ordInfo._offset != 0)
		{
			PosInfo& pInfo = _pos_map[ordInfo._code];
			PosItem& pItem = ordInfo._isLong ? pInfo._long : pInfo._short;
			WTSCommodityInfo* commInfo = _replayer->get_commodity_info(ordInfo._code);
			if(commInfo->getCoverMode() == CM_CoverToday)
			{
				if (ordInfo._offset == 2)
					pItem._newavail += ordInfo._left;
				else
					pItem._preavail += ordInfo._left;
			}
			else
			{
				//如果不分平昨平今，则先释放今仓
				double maxQty = std::min(ordInfo._left, pItem._newvol - pItem._newavail);
				pItem._newavail += maxQty;
				pItem._preavail += ordInfo._left - maxQty;
			}
		}

		log_debug("Order {} canceled, action: {} {} @ {}({})", ordInfo._localid, OFFSET_NAMES[ordInfo._offset], ordInfo._isLong?"long":"short", ordInfo._total, ordInfo._left);
		ordInfo._left = 0;
		on_order(localid, ordInfo._code, ordInfo._isLong, ordInfo._offset, ordInfo._total, ordInfo._left, ordInfo._price, true);
		_orders.erase(it);
	});

	return true;
}

OrderIDs UftMocker::stra_cancel_all(const char* stdCode)
{
	OrderIDs ret;
	uint32_t cnt = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		const OrderInfo& ordInfo = it->second;
		if(strcmp(ordInfo._code, stdCode) == 0)
		{
			stra_cancel(it->first);
		}
	}

	return ret;
}

OrderIDs UftMocker::stra_buy(const char* stdCode, double price, double qty, int flag /* = 0 */)
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

	OrderIDs ids;
	const PosInfo& pInfo = _pos_map[stdCode];

	double left = qty;
	//先检查空头
	const PosItem& pItem = pInfo._short;
	if(decimal::gt(pItem.valid(), 0.0))
	{
		if(commInfo->getCoverMode() != CM_CoverToday)
		{
			double maxQty = std::min(left, pItem.valid());
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_short(stdCode, price, maxQty, false, 0);
				if (localid != 0) ids.emplace_back(localid);
			}
			left -= maxQty;
		}
		else
		{
			double maxQty = std::min(left, pItem._preavail);
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_short(stdCode, price, maxQty, false, 0);
				if (localid != 0) ids.emplace_back(localid);
			}
			left -= maxQty;

			maxQty = std::min(left, pItem._newavail);
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_short(stdCode, price, maxQty, true, 0);
				if (localid != 0) ids.emplace_back(localid);
			}
			left -= maxQty;
		}
	}

	//还有剩余则开仓
	if(decimal::gt(left, 0.0))
	{
		ids.emplace_back(stra_enter_long(stdCode, price, left));
	}

	return ids;
}

OrderIDs UftMocker::stra_sell(const char* stdCode, double price, double qty, int flag /* = 0 */)
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

	OrderIDs ids;
	const PosInfo& pInfo = _pos_map[stdCode];

	double left = qty;
	//先检查空头
	const PosItem& pItem = pInfo._long;
	if (decimal::gt(pItem.valid(), 0.0))
	{
		if (commInfo->getCoverMode() != CM_CoverToday)
		{
			double maxQty = std::min(left, pItem.valid());
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_long(stdCode, price, maxQty, false, 0);
				if (localid != 0) ids.emplace_back(localid);
			}

			left -= maxQty;
		}
		else
		{
			double maxQty = std::min(left, pItem._preavail);
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_long(stdCode, price, maxQty, false, 0);
				if (localid != 0) ids.emplace_back(localid);
			}
			left -= maxQty;

			maxQty = std::min(left, pItem._newavail);
			if (decimal::gt(maxQty, 0.0))
			{
				uint32_t localid = stra_exit_long(stdCode, price, maxQty, true, 0);
				if (localid != 0) ids.emplace_back(localid);
			}
			left -= maxQty;
		}
	}

	//还有剩余则开仓
	if (decimal::gt(left, 0.0))
	{
		ids.emplace_back(stra_enter_short(stdCode, price, left));
	}

	return ids;
}

uint32_t UftMocker::stra_enter_long(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return 0;
	}

	if (decimal::le(qty, 0))
	{
		log_error("Entrust error: qty {} <= 0", qty);
		return 0;
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isLong = true;
	order._offset = 0;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		_mtx_ords.lock();
		_orders[localid] = order;
		_mtx_ords.unlock();
	}

	postTask([this, localid]() {
		const OrderInfo& ordInfo = _orders[localid];
		log_debug("order placed: open long of {} @ {} by {}", ordInfo._code, ordInfo._price, ordInfo._total);
		on_entrust(localid, ordInfo._code, true, "entrust success");
	});

	return localid;
}

uint32_t UftMocker::stra_enter_short(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return 0;
	}

	if (decimal::le(qty, 0))
	{
		log_error("Entrust error: qty {} <= 0", qty);
		return 0;
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isLong = false;
	order._offset = 0;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		_mtx_ords.lock();
		_orders[localid] = order;
		_mtx_ords.unlock();
	}

	postTask([this, localid]() {
		const OrderInfo& ordInfo = _orders[localid];
		log_debug("order placed: open short of {} @ {} by {}", ordInfo._code, ordInfo._price, ordInfo._total);
		on_entrust(localid, ordInfo._code, true, "entrust success");
	});

	return localid;
}

uint32_t UftMocker::stra_exit_long(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	PosItem& pItem = pInfo._long;
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	uint32_t offset = 1;
	if(commInfo->getCoverMode() != CM_CoverToday)
	{
		if(decimal::lt(pItem.valid(), qty))
		{
			log_error("Entrust error: no enough available position");
			return 0;
		}

		double maxQty = std::min(qty, pItem._preavail);
		pItem._preavail -= maxQty;
		pItem._newavail -= qty - maxQty;
	}
	else
	{
		if (isToday) offset = 2;

		double valid = isToday ? pItem._newavail : pItem._preavail;
		if (decimal::lt(valid, qty))
		{
			log_error("Entrust error: no enough available {} position", isToday?"new":"old");
			return 0;
		}

		if (isToday)
			pItem._newavail -= qty;
		else
			pItem._preavail -= qty;
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isLong = true;
	order._offset = offset;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		_mtx_ords.lock();
		_orders[localid] = order;
		_mtx_ords.unlock();
	}

	postTask([this, localid]() {
		const OrderInfo& ordInfo = _orders[localid];
		log_debug("order placed: {} long of {} @ {} by {}", OFFSET_NAMES[ordInfo._offset], ordInfo._code, ordInfo._price, ordInfo._total);
		on_entrust(localid, ordInfo._code, true, "entrust success");
	});

	return localid;
}

uint32_t UftMocker::stra_exit_short(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	PosItem& pItem = pInfo._short;
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	uint32_t offset = 1;
	if (commInfo->getCoverMode() != CM_CoverToday)
	{
		if (decimal::lt(pItem.valid(), qty))
		{
			log_error("Entrust error: no enough available position");
			return 0;
		}

		double maxQty = std::min(qty, pItem._preavail);
		pItem._preavail -= maxQty;
		pItem._newavail -= qty - maxQty;
	}
	else
	{
		if (isToday) offset = 2;

		double valid = isToday ? pItem._newavail : pItem._preavail;
		if (decimal::lt(valid, qty))
		{
			log_error("Entrust error: no enough available {} position", isToday ? "new" : "old");
			return 0;
		}

		if (isToday)
			pItem._newavail -= qty;
		else
			pItem._preavail -= qty;
	}

	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isLong = false;
	order._offset = offset;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		_mtx_ords.lock();
		_orders[localid] = order;
		_mtx_ords.unlock();
	}

	postTask([this, localid]() {
		const OrderInfo& ordInfo = _orders[localid];
		log_debug("order placed: {} short of {} @ {} by {}", OFFSET_NAMES[ordInfo._offset], ordInfo._code, ordInfo._price, ordInfo._total);
		on_entrust(localid, ordInfo._code, true, "entrust success");
	});

	return localid;
}

void UftMocker::on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled)
{
	if(_strategy)
		_strategy->on_order(this, localid, stdCode, isLong, offset, totalQty, leftQty, price, isCanceled);
}

void UftMocker::on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price)
{
	//PosInfo& posInfo = _pos_map[stdCode];
	//PosItem& posItem = isLong ? posInfo._long : posInfo._short;
	update_position(stdCode, isLong, offset, vol, price);
	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isLong, offset, vol, price);
}

void UftMocker::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);
}

void UftMocker::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);
}

void UftMocker::update_dyn_profit(const char* stdCode, WTSTickData* newTick)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
		PosInfo& pInfo = (PosInfo&)it->second;
		{
			bool isLong = true;
			PosItem& pItem = pInfo._long;
			if (pItem.volume() == 0)
				pItem._dynprofit = 0;
			else
			{
				double price = isLong ? newTick->bidprice(0) : newTick->askprice(0);
				double dynprofit = 0;
				for (auto pit = pItem._details.begin(); pit != pItem._details.end(); pit++)
				{

					DetailInfo& dInfo = *pit;
					dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale();
					if (dInfo._profit > 0)
						dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
					else if (dInfo._profit < 0)
						dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

					dynprofit += dInfo._profit;
				}

				pItem._dynprofit = dynprofit;
			}
		}

		{
			bool isLong = false;
			PosItem& pItem = pInfo._short;
			if (pItem.volume() == 0)
				pItem._dynprofit = 0;
			else
			{
				double price = isLong ? newTick->bidprice(0) : newTick->askprice(0);
				double dynprofit = 0;
				for (auto pit = pItem._details.begin(); pit != pItem._details.end(); pit++)
				{

					DetailInfo& dInfo = *pit;
					dInfo._profit = dInfo._volume*(dInfo._price - price)*commInfo->getVolScale();
					if (dInfo._profit > 0)
						dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
					else if (dInfo._profit < 0)
						dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

					dynprofit += dInfo._profit;
				}

				pItem._dynprofit = dynprofit;
			}
		}
	}
}

bool UftMocker::procOrder(uint32_t localid)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return false;

	OrderInfo ordInfo = (OrderInfo&)it->second;

	//第一步,如果在撤单概率中,则执行撤单
	if(_error_rate>0 && genRand(10000)<=_error_rate)
	{
		on_order(localid, ordInfo._code, ordInfo._isLong, ordInfo._offset, ordInfo._total, ordInfo._left, ordInfo._price, true);
		log_info("Random error order: {}", localid);
		return true;
	}
	else
	{
		on_order(localid, ordInfo._code, ordInfo._isLong, ordInfo._offset, ordInfo._total, ordInfo._left, ordInfo._price, false);
	}

	WTSTickData* curTick = stra_get_last_tick(ordInfo._code);
	if (curTick == NULL)
		return false;

	double curPx = curTick->price();
	double orderQty = ordInfo._isLong ? curTick->askqty(0) : curTick->bidqty(0);	//看对手盘的数量
	if (decimal::eq(orderQty, 0.0))
		return false;

	if (!_use_newpx)
	{
		curPx = ordInfo._isLong ? curTick->askprice(0) : curTick->bidprice(0);
		//if (curPx == 0.0)
		if(decimal::eq(curPx, 0.0))
		{
			curTick->release();
			return false;
		}
	}
	curTick->release();

	//如果没有成交条件,则退出逻辑
	if(!decimal::eq(ordInfo._price, 0.0))
	{
		if(ordInfo._isLong && decimal::gt(curPx, ordInfo._price))
		{
			//买单,但是当前价大于限价,不成交
			return false;
		}

		if (!ordInfo._isLong && decimal::lt(curPx, ordInfo._price))
		{
			//卖单,但是当前价小于限价,不成交
			return false;
		}
	}

	/*
	 *	下面就要模拟成交了
	 */
	double maxQty = min(orderQty, ordInfo._left);
	auto vols = splitVolume((uint32_t)maxQty);
	for(uint32_t curQty : vols)
	{
		on_trade(ordInfo._localid, ordInfo._code, ordInfo._isLong, ordInfo._offset, curQty, curPx);

		ordInfo._left -= curQty;
		on_order(localid, ordInfo._code, ordInfo._isLong, ordInfo._offset, ordInfo._total, ordInfo._left, ordInfo._price, false);
	}

	//if(ordInfo._left == 0)
	if(decimal::eq(ordInfo._left, 0.0))
	{
		return true;
	}

	return false;
}

WTSCommodityInfo* UftMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

WTSKlineSlice* UftMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	return _replayer->get_kline_slice(stdCode, basePeriod, count, times);
}

WTSTickSlice* UftMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSOrdQueSlice* UftMocker::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	return _replayer->get_order_queue_slice(stdCode, count);
}

WTSOrdDtlSlice* UftMocker::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	return _replayer->get_order_detail_slice(stdCode, count);
}

WTSTransSlice* UftMocker::stra_get_transaction(const char* stdCode, uint32_t count)
{
	return _replayer->get_transaction_slice(stdCode, count);
}

WTSTickData* UftMocker::stra_get_last_tick(const char* stdCode)
{
	return _replayer->get_last_tick(stdCode);
}

double UftMocker::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, int32_t iFlag /* = 3 */)
{
	const PosInfo& posInfo = _pos_map[stdCode];
	if (iFlag == 1)
		return bOnlyValid ? posInfo._long.valid() : posInfo._long.volume();
	else if (iFlag == 2)
		return bOnlyValid ? posInfo._short.valid() : posInfo._short.volume();
	else
		return bOnlyValid ? (posInfo._long.valid() - posInfo._short.valid()) : (posInfo._long.volume() - posInfo._short.volume());
}

double UftMocker::stra_enum_position(const char* stdCode)
{
	uint32_t tdate = _replayer->get_trading_date();
	double ret = 0;
	bool bAll = (strlen(stdCode) == 0);
	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		if (!bAll && strcmp(it->first.c_str(), stdCode) != 0)
			continue;

		const PosInfo& pInfo = it->second;
		_strategy->on_position(this, stdCode, true, pInfo._long._prevol, pInfo._long._preavail, pInfo._long._newvol, pInfo._long._newavail);
		_strategy->on_position(this, stdCode, false, pInfo._short._prevol, pInfo._short._preavail, pInfo._short._newvol, pInfo._short._newavail);
		ret += pInfo._long.volume() + pInfo._short.volume();
	}

	return ret;
}

double UftMocker::stra_get_price(const char* stdCode)
{
	return _replayer->get_cur_price(stdCode);
}

uint32_t UftMocker::stra_get_date()
{
	return _replayer->get_date();
}

uint32_t UftMocker::stra_get_time()
{
	return _replayer->get_raw_time();
}

uint32_t UftMocker::stra_get_secs()
{
	return _replayer->get_secs();
}

void UftMocker::stra_sub_ticks(const char* stdCode)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	主动订阅tick会在本地记一下
	 *	tick数据回调的时候先检查一下
	 */
	_tick_subs.insert(stdCode);

	_replayer->sub_tick(_context_id, stdCode);
}

void UftMocker::stra_sub_order_queues(const char* stdCode)
{
	_replayer->sub_order_queue(_context_id, stdCode);
}

void UftMocker::stra_sub_order_details(const char* stdCode)
{
	_replayer->sub_order_detail(_context_id, stdCode);
}

void UftMocker::stra_sub_transactions(const char* stdCode)
{
	_replayer->sub_transaction(_context_id, stdCode);
}

void UftMocker::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void UftMocker::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void UftMocker::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}


void UftMocker::dump_outputs()
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

	filename = folder + "positions.csv";
	content = "date,code,direct,volume,closeprofit,dynprofit\n";
	if (!_pos_logs.str().empty()) content += _pos_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());
}

void UftMocker::log_trade(const char* stdCode, bool isLong, uint32_t offset, uint64_t curTime, double price, double qty, double fee)
{
	_trade_logs << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << OFFSET_NAMES[offset]
		<< "," << price << "," << qty << "," << fee  << "\n";
}

void UftMocker::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double maxprofit, double maxloss,
	double totalprofit /* = 0 */)
{
	_close_logs << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
		<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," << maxprofit << "," << maxloss << ","
		<< totalprofit << "\n";
}

void UftMocker::update_position(const char* stdCode, bool isLong, uint32_t offset, double qty, double price /* = 0.0 */)
{
	PosItem& pItem = isLong ? _pos_map[stdCode]._long : _pos_map[stdCode]._short;

	//先确定成交价格
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode];

	const char* pos_dir = isLong ? "long" : "short";

	//获取时间
	uint64_t curTm = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_min_time()*100000 + _replayer->get_secs();
	uint32_t curTDate = _replayer->get_trading_date();

	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
		return;

	//成交价
	double trdPx = curPx;

	if (offset == 0)
	{
		//如果是开仓，则直接增加明细即可
		pItem._newvol += qty;
		//如果T+1，则冻结仓位要增加
		if (commInfo->isT1())
		{
			//ASSERT(diff>0);
			log_debug("{} position of {} frozen up to {}", pos_dir, stdCode, pItem.frozen());
		}
		else
		{
			pItem._newavail += qty;
		}

		DetailInfo dInfo;
		dInfo._price = trdPx;
		dInfo._volume = qty;
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		pItem._details.emplace_back(dInfo);

		double fee = _replayer->calc_fee(stdCode, trdPx, qty, 0);
		_fund_info._total_fees += fee;

		log_trade(stdCode, isLong, 0, curTm, trdPx, qty, fee);
	}
	else if(offset == 1)
	{
		//如果是平仓（平昨也是这个），则根据明细的时间先后处理平仓
		double maxQty = min(pItem._prevol, qty);
		pItem._prevol -= maxQty;
		pItem._newvol -= qty - maxQty;

		std::vector<DetailInfo>::iterator eit = pItem._details.end();
		double left = qty;
		for (auto it = pItem._details.begin(); it != pItem._details.end(); it++)
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
				eit = it;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (isLong)
				profit *= -1;
			pItem._closeprofit += profit;

			//等比缩放明细的相关浮盈
			dInfo._profit = dInfo._profit*dInfo._volume / (dInfo._volume + maxQty);
			dInfo._max_profit = dInfo._max_profit*dInfo._volume / (dInfo._volume + maxQty);
			dInfo._max_loss = dInfo._max_loss*dInfo._volume / (dInfo._volume + maxQty);
			_fund_info._total_profit += profit;
			double fee = _replayer->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, isLong, offset, curTm, trdPx, maxQty, fee);
			//这里写平仓记录
			log_close(stdCode, isLong, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, pItem._closeprofit);

			if (left == 0)
				break;
		}

		//需要清理掉已经平仓完的明细
		if (eit != pItem._details.end())
			pItem._details.erase(pItem._details.begin(), eit);

	}
	else if (offset == 2)
	{
		//如果是平今，只更新今仓，先找到今仓起始的位置，再开始处理
		pItem._newvol -= qty;
		std::vector<DetailInfo>::iterator sit = pItem._details.end();
		std::vector<DetailInfo>::iterator eit = pItem._details.end();

		uint32_t count = 0;
		double left = qty;
		for (auto it = pItem._details.begin(); it != pItem._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			//如果不是今仓，就直接跳过
			if(dInfo._opentdate != curTDate)
				continue;

			double maxQty = min(dInfo._volume, left);
			if (decimal::eq(maxQty, 0))
				continue;

			if (sit == pItem._details.end())
				sit = it;

			eit = it;

			double maxProf = dInfo._max_profit * maxQty / dInfo._volume;
			double maxLoss = dInfo._max_loss * maxQty / dInfo._volume;

			dInfo._volume -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!isLong)
				profit *= -1;
			pItem._closeprofit += profit;
			pItem._dynprofit = pItem._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//浮盈也要做等比缩放
			_fund_info._total_profit += profit;

			uint32_t offset = dInfo._opentdate == curTDate ? 2 : 1;
			double fee = _replayer->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, isLong, offset, curTm, trdPx, maxQty, fee);
			//这里写平仓记录
			log_close(stdCode, isLong, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, pItem._closeprofit);

			if (left == 0)
				break;
		}

		//需要清理掉已经平仓完的明细
		if (sit != pItem._details.end())
			pItem._details.erase(sit, eit);
	}

	log_info("[{:04d}.{:05d}] {} position of {} updated: {} {} to {}", _replayer->get_min_time(), _replayer->get_secs(), pos_dir, stdCode, OFFSET_NAMES[offset], qty, pItem.volume());

	double dynprofit = 0;
	for (const DetailInfo& dInfo : pItem._details)
	{
		dynprofit += dInfo._profit;
	}
	pItem._dynprofit = dynprofit;
}
