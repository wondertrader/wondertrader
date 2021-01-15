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
#include <math.h>

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/BoostFile.hpp"

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

inline uint32_t makeHftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

HftMocker::HftMocker(HisDataReplayer* replayer, const char* name)
	: IHftStraCtx(name)
	, _replayer(replayer)
	, _strategy(NULL)
	, _thrd(NULL)
	, _stopped(false)
	, _use_newpx(false)
{
	_commodities = CommodityMap::create();

	_context_id = makeHftCtxId();
}


HftMocker::~HftMocker()
{
	if(_strategy)
	{
		_factory._fact->deleteStrategy(_strategy);
	}

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

bool HftMocker::initHftFactory(WTSVariant* cfg)
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
	//on_schedule(uDate, uTime);
}

void HftMocker::handle_session_begin()
{

}

void HftMocker::handle_session_end()
{

}

void HftMocker::handle_replay_done()
{
	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());

	std::string filename = folder + "signals.csv";
	std::string content = "time, action, position, price\n";
	content += _ofs_signals.str();
	BoostFile::write_file_contents(filename.c_str(), content.c_str(), content.size());
}

void HftMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, times, newBar);
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
	return _context_id;
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

		on_order(localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, true);
		_orders.erase(it);
	});

	return true;
}

OrderIDs HftMocker::stra_cancel(const char* stdCode, bool isBuy, double qty /* = 0 */)
{
	OrderIDs ret;
	uint32_t cnt = 0;
	for (auto it = _orders.begin(); it != _orders.end(); it++)
	{
		OrderInfo& ordInfo = it->second;
		if(ordInfo._isBuy == isBuy && strcmp(ordInfo._code, stdCode) == 0)
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
		on_entrust(localid, _orders[localid]._code, true, "下单成功");
		procOrder(localid);
	});

	OrderIDs ids;
	ids.push_back(localid);
	return ids;
}

void HftMocker::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	if(_strategy)
		_strategy->on_order(this, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void HftMocker::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isBuy, vol, price);
}

void HftMocker::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);
}

void HftMocker::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);
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
		on_order(localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, true);
		_orders.erase(it);
		return;
	}
	else
	{
		on_order(localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, false);
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
		on_trade(ordInfo._localid, ordInfo._code, ordInfo._isBuy, curQty, curPx);

		ordInfo._left -= curQty;
		on_order(localid, ordInfo._code, ordInfo._isBuy, ordInfo._total, ordInfo._left, ordInfo._price, false);

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

WTSCommodityInfo* HftMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

WTSKlineSlice* HftMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	return _replayer->get_kline_slice(stdCode, period, count);
}

WTSTickSlice* HftMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSTickData* HftMocker::stra_get_last_tick(const char* stdCode)
{
	return _replayer->get_last_tick(stdCode);
}

double HftMocker::stra_get_position(const char* stdCode)
{
	return _positions[stdCode];
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
	_replayer->sub_tick(_context_id, stdCode);
}

void HftMocker::stra_log_text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	//logger.log(fmt, args);
	va_end(args);
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