/*!
 * \file WtDiffMinImpactExeUnit.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtDiffMinImpactExeUnit.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

extern const char* PriceModeNames[4];

WtDiffMinImpactExeUnit::WtDiffMinImpactExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _price_mode(0)
	, _price_offset(0)
	, _expire_secs(0)
	, _cancel_cnt(0)
	, _left_diff(0)
	, _cancel_times(0)
	, _last_place_time(0)
	, _last_tick_time(0)
	, _in_calc(false)
{
}


WtDiffMinImpactExeUnit::~WtDiffMinImpactExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtDiffMinImpactExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtDiffMinImpactExeUnit::getName()
{
	return "WtDiffMinImpactExeUnit";
}

void WtDiffMinImpactExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_sess_info = ctx->getSessionInfo(stdCode);
	if (_sess_info)
		_sess_info->retain();

	_price_offset = cfg->getInt32("offset");	//价格偏移跳数，一般和订单同方向
	_expire_secs = cfg->getUInt32("expire");	//订单超时秒数
	_price_mode = cfg->getInt32("pricemode");	//价格类型,0-最新价,-1-最优价,1-对手价,2-自动,默认为0
	_entrust_span = cfg->getUInt32("span");		//发单时间间隔，单位毫秒
	_by_rate = cfg->getBoolean("byrate");		//是否按照对手的挂单数的比例下单，如果是true，则rate字段生效，如果是false则lots字段生效
	_order_lots = cfg->getDouble("lots");		//单次发单手数
	_qty_rate = cfg->getDouble("rate");			//下单手数比例

	ctx->writeLog(fmtutil::format("DiffMiniImpactExecUnit {} inited, order price: {} ± {} ticks, order expired: {} secs, order timespan:{} millisec, order qty: {} @ {:.2f}",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots));
}

void WtDiffMinImpactExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	{
		if (!_orders_mon.has_order(localid))
			return;

		if (isCanceled || leftover == 0)
		{	
			//By Wesley @ 2022.05.24
			//这句要注释掉，因为需要早on_trade里处理一些数据
			//这里如果从OMS中删除了订单号，ontrade就会判断失败
			//_orders_mon.erase_order(localid);
			if (_cancel_cnt > 0)
			{
				_cancel_cnt--;
				_ctx->writeLog(fmtutil::format("[{}@{}] Order of {} cancelling done, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));
			}
		}

		if (leftover == 0 && !isCanceled)
			_cancel_times = 0;
	}

	//如果有撤单,也触发重新计算
	if (isCanceled)
	{
		_ctx->writeLog(fmtutil::format("Order {} of {} canceled, recalc will be done", localid, stdCode));
		_cancel_times++;
		do_calc();
	}
}

void WtDiffMinImpactExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());

	if(!decimal::eq(undone, 0) && !_orders_mon.has_order())
	{
		/*
		 *	如果未完成单不为0，而OMS没有订单
		 *	这说明有未完成单不在监控之中,全部撤销掉
		 *	因为这些订单没有本地订单号，无法直接进行管理
		 *	这种情况，就是刚启动的时候，上次的未完成单或者外部的挂单
		 */
		_ctx->writeLog(fmtutil::format("Unmanaged live orders with qty {} of {} found, cancel all", undone, _code.c_str()));

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog(fmtutil::format("[{}@{}]cancelcnt -> {}", __FILE__, __LINE__, _cancel_cnt));
	}
	else if (decimal::eq(undone, 0) && _orders_mon.has_order())
	{
		/*
		 *	By Wesey @ 2021.12.13
		 *	如果未完成单为0，但是OMS中是有订单的
		 *	说明OMS中是错单，需要清理掉，不然超时撤单就会出错
		 *	这种情况，一般是断线重连以后，之前下出去的订单，并没有真正发送到柜台
		 *	所以这里需要清理掉本地订单
		 */
		_ctx->writeLog(fmtutil::format("Local orders of {} not confirmed in trading channel, clear all", _code.c_str()));
		_orders_mon.clear_orders();
	}
	else
	{
		_ctx->writeLog(fmtutil::format("Unrecognized condition while channle ready, {:.2f} live orders of {} exists, local orders {}exist",
			undone, _code.c_str(), _orders_mon.has_order() ? "" : "not "));
	}


	do_calc();
}

void WtDiffMinImpactExeUnit::on_channel_lost()
{
	
}

void WtDiffMinImpactExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	//如果原来的tick不为空,则要释放掉
	if (_last_tick)
	{
		_last_tick->release();
	}
	else
	{
		//如果行情时间不在交易时间,这种情况一般是集合竞价的行情进来,下单会失败,所以直接过滤掉这笔行情
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}

	//新的tick数据,要保留
	_last_tick = newTick;
	_last_tick->retain();

	/*
	 *	这里可以考虑一下
	 *	如果写的上一次丢出去的单子不够达到目标仓位
	 *	那么在新的行情数据进来的时候可以再次触发核心逻辑
	 */

	if(_expire_secs != 0 && _orders_mon.has_order() && _cancel_cnt==0)
	{
		uint64_t now = _ctx->getCurTime();

		_orders_mon.check_orders(_expire_secs, now, [this](uint32_t localid) {
			if (_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog(fmtutil::format("[{}@{}] Expired order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt));
			}
		});
	}
	
	do_calc();
}

void WtDiffMinImpactExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	//如果是本地订单，则更新差量
	if (!_orders_mon.has_order(localid))
		return;

	_left_diff -= vol * (isBuy ? 1 : -1);

	_ctx->writeLog(fmtutil::format("Left diff of {} updated to {}", _code.c_str(), _left_diff));
}

void WtDiffMinImpactExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//如果不是我发出去的订单,我就不管了
		if (!_orders_mon.has_order(localid))
			return;

		_orders_mon.erase_order(localid);

		do_calc();
	}
}

void WtDiffMinImpactExeUnit::do_calc()
{
	CalcFlag flag(&_in_calc);
	if (flag)
	{
		_ctx->writeLog(fmtutil::format("Duplicated calculating, DiffMinImpactExeUnit of {}", _code));
		return;
	}

	if (_cancel_cnt != 0)
	{
		_ctx->writeLog(fmtutil::format("In Cancelling, DiffMinImpactExeUnit of {}", _code));
		return;
	}

	//这里加一个锁，主要原因是实盘过程中发现
	//在修改目标仓位的时候，会触发一次do_calc
	//而ontick也会触发一次do_calc，两次调用是从两个线程分别触发的，所以会出现同时触发的情况
	//如果不加锁，就会引起问题
	//这种情况在原来的SimpleExecUnit没有出现，因为SimpleExecUnit只在set_position的时候触发
	StdUniqueLock lock(_mtx_calc);

	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double diffPos = _left_diff;

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
	{
		_ctx->writeLog(fmtutil::format("Live orders exist, DiffMinImpactExeUnit of {}", _code));
		return;
	}

	if (decimal::eq(diffPos, 0))
		return;

	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmtutil::format("No lastest tick data of {}, execute later", _code.c_str()));
		return;
	}

	//检查下单时间间隔
	uint64_t now = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (now - _last_place_time < _entrust_span)
		return;

	bool isBuy = decimal::gt(diffPos, 0);

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmtutil::format("No tick of {} updated, {} <= {}, execute later",
			_code, curTickTime, _last_tick_time));
		return;
	}

	_last_tick_time = curTickTime;

	double this_qty = _order_lots;
	if (_by_rate)
	{
		this_qty = isBuy ? _last_tick->askqty(0) : _last_tick->bidqty(0);
		this_qty = round(this_qty*_qty_rate);
		if (decimal::lt(this_qty, 1))
			this_qty = 1;
	}

	//By Wesley @ 2022.09.13
	//这里要对下单数量做一个修正
	this_qty = min(this_qty, abs(diffPos));

	//如果买入且有空头持仓，或者卖出且有多头持仓
	//对单次下单做一个修正，保证平仓和开仓不会同时下单
	double curPos = _ctx->getPosition(stdCode);
	if((isBuy && decimal::lt(curPos, 0)) || (!isBuy && decimal::gt(curPos, 0)))
	{
		this_qty = min(this_qty, abs(curPos));
	}

	double buyPx, sellPx;
	if (_price_mode == 2)
	{
		double mp = (_last_tick->bidqty(0) - _last_tick->askqty(0))*1.0 / (_last_tick->bidqty(0) + _last_tick->askqty(0));
		bool isUp = (mp > 0);
		if (isUp)
		{
			buyPx = _last_tick->askprice(0);
			sellPx = _last_tick->askprice(0);
		}
		else
		{
			buyPx = _last_tick->bidprice(0);
			sellPx = _last_tick->bidprice(0);
		}

		/*
		 *	By Wesley @ 2022.03.07
		 *	如果最后价格为0，再做一个修正
		 */
		if (decimal::eq(buyPx, 0.0))
			buyPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		if (decimal::eq(sellPx, 0.0))
			sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		buyPx += _comm_info->getPriceTick() * _cancel_times;
		sellPx -= _comm_info->getPriceTick() * _cancel_times;
	}
	else
	{
		if (_price_mode == -1)
		{
			buyPx = _last_tick->bidprice(0);
			sellPx = _last_tick->askprice(0);
		}
		else if (_price_mode == 0)
		{
			buyPx = _last_tick->price();
			sellPx = _last_tick->price();
		}
		else if (_price_mode == 1)
		{
			buyPx = _last_tick->askprice(0);
			sellPx = _last_tick->bidprice(0) - _comm_info->getPriceTick() * _price_offset;
		}

		/*
		 *	By Wesley @ 2022.03.07
		 *	如果最后价格为0，再做一个修正
		 */
		if (decimal::eq(buyPx, 0.0))
			buyPx = decimal::eq(_last_tick->price(), 0.0)? _last_tick->preclose(): _last_tick->price();

		if (decimal::eq(sellPx, 0.0))
			sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		buyPx += _comm_info->getPriceTick() * _price_offset;
		sellPx -= _comm_info->getPriceTick() * _price_offset;
	}
	

	//检查涨跌停价
	bool isCanCancel = true;
	if (!decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(buyPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog(fmtutil::format("Buy price {} of {} modified to upper limit price", buyPx, _code.c_str(), _last_tick->upperlimit()));
		buyPx = _last_tick->upperlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}
	
	if (!decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(sellPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmtutil::format("Sell price {} of {} modified to lower limit price", sellPx, _code.c_str(), _last_tick->lowerlimit()));
		sellPx = _last_tick->lowerlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}

	if (isBuy)
	{
		OrderIDs ids = _ctx->buy(stdCode, buyPx, this_qty, false);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, this_qty, false);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}

	_last_place_time = now;
}

void WtDiffMinImpactExeUnit::set_position(const char* stdCode, double newDiff)
{
	if (_code.compare(stdCode) != 0)
		return;

	if (newDiff == DBL_MAX)
	{
		_ctx->writeLog("Diff execute unit do not support clear command");
		return;
	}

	//这里就是最新的差量

	if(_left_diff != newDiff)
	{
		_left_diff = newDiff;

		_ctx->writeLog(fmtutil::format("Diff of {} updated to {}", stdCode, _left_diff));

		do_calc();
	}
}

void WtDiffMinImpactExeUnit::clear_all_position(const char* stdCode)
{
	_ctx->writeLog("Diff execute unit do not support clear command");
	return;
}
