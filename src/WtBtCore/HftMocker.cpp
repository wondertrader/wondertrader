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
//#include "Logger.h"

#include <math.h>

#include "../Share/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/decimal.h"

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

std::vector<uint32_t> splitVolumn(uint32_t vol)
{
	uint32_t minQty = 1;
	uint32_t maxQty = 100;
	uint32_t length = maxQty - minQty + 1;
	std::vector<uint32_t> ret;
	if (vol <= minQty)
	{
		ret.push_back(vol);
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

			ret.push_back(curVol);
			left -= curVol;
		}
	}

	return ret;
}

uint32_t genRand(uint32_t maxVal = 10000)
{
	srand(TimeUtils::getCurMin());
	return rand() % maxVal;
}

HftMocker::HftMocker(HisDataReplayer* replayer)
	: IHftStraCtx("mocker")
	, _replayer(replayer)
	, _strategy(NULL)
	, _thrd(NULL)
	, _stopped(false)
	, _use_newpx(false)
{
	_commodities = CommodityMap::create();
}


HftMocker::~HftMocker()
{
	if(_strategy)
	{
		_factory._fact->deleteStrategy(_strategy);
	}

	_ofs_signals.close();

	_commodities->release();
}

void HftMocker::postTask(Task task)
{
	{
		std::unique_lock<std::mutex> lck(_mtx);
		_tasks.push(task);
	}

	if(_thrd == NULL)
	{
		_thrd.reset(new std::thread([this](){
			while (!_stopped)
			{
				if(_tasks.empty())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(2));
					continue;
				}

				_mtx_control.lock();

				while(!_tasks.empty())
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
		}));
	}
}

bool HftMocker::init(WTSVariant* cfg)
{
	const char* module = cfg->getCString("module");
	
	_use_newpx = cfg->getBoolean("use_newpx");
	_error_rate = cfg->getUInt32("error_rate");

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
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), "hft");
		_strategy->init(cfgStra->get("params"));
		_strategy->on_init(this);
		_strategy->on_channel_ready(this);
	}

	_ofs_signals.open("signals.log");
	_ofs_signals << "time, action, position, price" << std::endl;

	return true;
}

void HftMocker::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	on_tick(stdCode, curTick);
}

void HftMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	on_bar(stdCode, period, times, newBar);
}

void HftMocker::handle_init()
{
	on_init();
}

void HftMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	on_schedule(uDate, uTime);
}

void HftMocker::handle_session_begin()
{

}

void HftMocker::handle_session_end()
{

}

void HftMocker::on_tick(const char* stdCode, WTSTickData* newTick)
{
	{
		std::unique_lock<std::recursive_mutex> lck(_mtx_control);
	}
	
	if (_orders.empty())
	{
		for (auto it = _orders.begin(); it != _orders.end(); it++)
		{
			uint32_t localid = it->first;
			postTask([this, localid](){
				procOrder(localid);
			});
		}
	}

	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);

}

uint32_t HftMocker::id()
{
	return 0;
}

void HftMocker::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}

double HftMocker::stra_get_undone(const char* stdCode)
{
	int32_t ret = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		OrderInfo& ordInfo = it->second;
		if (strcmp(ordInfo._code, stdCode) == 0)
		{
			ret += ordInfo._left * ordInfo._isBuy ? 1 : -1;
		}
	}

	return ret;
}

bool HftMocker::stra_cancel(uint32_t localid)
{
	postTask([this, localid](){
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		OrderInfo& ordInfo = it->second;
		ordInfo._left = 0;

		_strategy->on_order(this, localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, true);
		_orders.erase(it);
	});

	return true;
}

OrderIDs HftMocker::stra_cancel(const char* code, bool isBuy, double qty /* = 0 */)
{
	OrderIDs ret;
	uint32_t cnt = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		OrderInfo& ordInfo = it->second;
		if(ordInfo._isBuy == isBuy && strcmp(ordInfo._code, code) == 0)
		{
			double left = ordInfo._left;
			stra_cancel(it->first);
			ret.push_back(it->first);
			cnt++;
			if (left < qty)
				qty -= left;
			else
				break;
		}
	}

	return ret;
}

otp::OrderIDs HftMocker::stra_buy(const char* stdCode, double price, double qty)
{
	//logger.log("买入%s合约%u手，价格：%f", stdCode, qty, price);
	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isBuy = true;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		_mtx_ords.lock();
		_orders[localid] = order;
		_mtx_ords.unlock();
	}

	postTask([this, localid](){
		procOrder(localid);
	});

	OrderIDs ids;
	ids.push_back(localid);
	return ids;
}

void HftMocker::procOrder(uint32_t localid)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	StdLocker<StdRecurMutex> lock(_mtx_ords);
	OrderInfo& ordInfo = it->second;

	//第一步，如果在撤单概率中，则执行撤单
	if(_error_rate>0 && genRand(10000)<=_error_rate)
	{
		_strategy->on_order(this, localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, true);
		_orders.erase(it);
		return;
	}
	else
	{
		_strategy->on_order(this, localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, false);
	}

	WTSTickData* curTick = stra_get_last_tick(ordInfo._code);
	if (curTick == NULL)
		return;

	double curPx = curTick->price();
	double orderQty = ordInfo._isBuy ? curTick->askqty(0) : curTick->bidqty(0);	//看对手盘的数量
	if (!_use_newpx)
	{
		curPx = ordInfo._isBuy ? curTick->askprice(0) : curTick->bidprice(0);
		//if (curPx == 0.0)
		if(decimal::eq(curPx, 0.0))
			return;
	}
	curTick->release();

	//如果没有成交条件，则退出逻辑
	if(!decimal::eq(ordInfo._price, 0.0))
	{
		if(ordInfo._isBuy && decimal::gt(curPx, ordInfo._price))
		{
			//买单，但是当前价大于限价，不成交
			return;
		}

		if (!ordInfo._isBuy && decimal::lt(curPx, ordInfo._price))
		{
			//卖单，但是当前价小于限价，不成交
			return;
		}
	}

	/*
	 *	下面就要模拟成交了
	 */

	double maxQty = min(orderQty, ordInfo._total);
	auto vols = splitVolumn((uint32_t)maxQty);
	for(uint32_t curQty : vols)
	{
		_strategy->on_trade(this, ordInfo._code, ordInfo._isBuy, curQty, curPx);

		ordInfo._left -= curQty;
		_strategy->on_order(this, localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, false);

		double& curPos = _positions[ordInfo._code];
		curPos += curQty * (ordInfo._isBuy ? 1 : -1);

		_ofs_signals << _replayer->get_date() << "." << _replayer->get_raw_time() << "." << _replayer->get_secs() << ","
			<< (ordInfo._isBuy ? "+" : "-") << curQty << "," << curPos << "," << curPx << std::endl;
	}

	//if(ordInfo._left == 0)
	if(decimal::eq(ordInfo._left, 0.0))
	{
		_orders.erase(it);
	}
}

otp::OrderIDs HftMocker::stra_sell(const char* stdCode, double price, double qty)
{
	//logger.log("卖出%s合约%u手，价格：%f", stdCode, qty, price);
	uint32_t localid = makeLocalOrderID();

	OrderInfo order;
	order._localid = localid;
	strcpy(order._code, stdCode);
	order._isBuy = false;
	order._price = price;
	order._total = qty;
	order._left = qty;

	{
		StdLocker<StdRecurMutex> lock(_mtx_ords);
		_orders[localid] = order;
	}

	postTask([this, localid](){
		procOrder(localid);
	});

	OrderIDs ids;
	ids.push_back(localid);
	return ids;
}

WTSCommodityInfo* HftMocker::stra_get_comminfo(const char* code)
{
	WTSCommodityInfo* commInfo = (WTSCommodityInfo*)_commodities->get(code);
	if (commInfo == NULL)
	{
		commInfo = WTSCommodityInfo::create(code, code, "", "", "");
		commInfo->setPriceTick(1.0);
		_commodities->add(code, commInfo, false);
	}

	return commInfo;
}

WTSKlineSlice* HftMocker::stra_get_bars(const char* code, const char* period, uint32_t count)
{
	return _replayer->get_kline_slice(code, period, count);
}

WTSTickSlice* HftMocker::stra_get_ticks(const char* code, uint32_t count)
{
	return _replayer->get_tick_slice(code, count);
}

WTSTickData* HftMocker::stra_get_last_tick(const char* code)
{
	return _replayer->get_last_tick(code);
}

double HftMocker::stra_get_position(const char* code)
{
	return _positions[code];
}

double HftMocker::stra_get_price(const char* code)
{
	return _replayer->get_cur_price(code);
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

void HftMocker::sub_ticks(const char* code)
{
}

void HftMocker::stra_log_text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	//logger.log(fmt, args);
	va_end(args);
}
