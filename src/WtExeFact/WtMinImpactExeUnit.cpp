/*!
 * \file WtMinImpactExeUnit.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtMinImpactExeUnit.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/decimal.h"

extern const char* FACT_NAME;


WtMinImpactExeUnit::WtMinImpactExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _price_mode(0)
	, _price_offset(0)
	, _expire_secs(0)
	, _cancel_cnt(0)
	, _target_pos(0)
	, _cancel_times(0)
	, _last_place_time(0)
	, _last_tick_time(0)
{
}


WtMinImpactExeUnit::~WtMinImpactExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtMinImpactExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtMinImpactExeUnit::getName()
{
	return "WtMinImpactExeUnit";
}

extern const char* PriceModeNames[4];
void WtMinImpactExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
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

	ctx->writeLog("MiniImpactExecUnit %s inited, order price: %s ± %d ticks, order expired: %u secs, order timespan:%u millisec, order qty: %s @ %.2f",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots);
}

void WtMinImpactExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	{
		if (!_orders_mon.has_order(localid))
			return;

		if (isCanceled || leftover == 0)
		{
			_orders_mon.erase_order(localid);
			if (_cancel_cnt > 0)
				_cancel_cnt--;

			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		}

		if (leftover == 0 && !isCanceled)
			_cancel_times = 0;
	}

	//如果有撤单,也触发重新计算
	if (isCanceled)
	{
		//_ctx->writeLog("%s的订单%u已撤销,重新触发执行逻辑", stdCode, localid);
		_ctx->writeLog("Order %u of %s canceled, recalc will be done", localid, stdCode);
		_cancel_times++;
		do_calc();
	}
}

void WtMinImpactExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());

	if(!decimal::eq(undone, 0) && !_orders_mon.has_order())
	{
		//这说明有未完成单不在监控之中,先撤掉
		//_ctx->writeLog("%s有不在管理中的未完成单 %f ,全部撤销", _code.c_str(), undone);
		_ctx->writeLog("Live orders with qty %f of %s found, cancel all", undone, _code.c_str());

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog("%s cancelcnt -> %u", __FUNCTION__, _cancel_cnt);
	}


	do_calc();
}

void WtMinImpactExeUnit::on_channel_lost()
{
	
}

void WtMinImpactExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	bool isFirstTick = false;
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

		isFirstTick = true;
	}

	//新的tick数据,要保留
	_last_tick = newTick;
	_last_tick->retain();

	/*
	 *	这里可以考虑一下
	 *	如果写的上一次丢出去的单子不够达到目标仓位
	 *	那么在新的行情数据进来的时候可以再次触发核心逻辑
	 */

	if(isFirstTick)	//如果是第一笔tick,则检查目标仓位,不符合则下单
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();

		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);

		if (!decimal::eq(newVol, undone + realPos))
		{
			do_calc();
			return;
		}
	}
	else if(_expire_secs != 0 && _orders_mon.has_order() && _cancel_cnt==0)
	{
		uint64_t now = _ctx->getCurTime();

		_orders_mon.check_orders(_expire_secs, now, [this](uint32_t localid) {
			if (_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			}
		});
	}
	
	do_calc();
}

void WtMinImpactExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	//不用触发,这里在ontick里触发吧
}

void WtMinImpactExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtMinImpactExeUnit::do_calc()
{
	if (_cancel_cnt != 0)
		return;

	double newVol = _target_pos;
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);

	//如果有反向未完成单,则直接撤销
	//如果目标仓位为0,且当前持仓为0,则撤销全部挂单
	if (decimal::lt(newVol * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();
		_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		return;
	}
	else if (decimal::eq(newVol,0) && !decimal::eq(undone, 0))
	{
		//如果目标仓位为0,且未完成不为0
		//那么当目前仓位为0,或者 目前仓位和未完成数量方向相同
		//这样也要全部撤销
		//if (realPos == 0 || (realPos * undone > 0))
		if (decimal::eq(realPos, 0) || decimal::gt(realPos * undone, 0))
		{
			bool isBuy = decimal::gt(undone, 0);
			OrderIDs ids = _ctx->cancel(stdCode, isBuy);
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			return;
		}
	}

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
		return;

	double curPos = realPos;
	//if (curPos == newVol)
	if (decimal::eq(curPos, newVol))
		return;

	//检查下单时间间隔
	uint64_t now = TimeUtils::getLocalTimeNow();
	if (now - _last_place_time < _entrust_span)
		return;

	bool isBuy = decimal::gt(newVol, curPos);

	if(_last_tick == NULL)
		_last_tick = _ctx->grabLastTick(stdCode);

	if (_last_tick == NULL)
	{
		_ctx->writeLog("No lastest tick data of %s, execute later", _code.c_str());
		return;
	}

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog("No tick of %s updated, execute later", _code.c_str());
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

		this_qty = min(this_qty, abs(newVol - curPos));
	}

	double buyPx, sellPx;
	if(_price_mode == -1)
	{
		buyPx = _last_tick->bidprice(0) + _comm_info->getPriceTick() * _price_offset;
		sellPx = _last_tick->askprice(0) - _comm_info->getPriceTick() * _price_offset;
	}
	else if(_price_mode == 0)
	{
		buyPx = _last_tick->price() + _comm_info->getPriceTick() * _price_offset;
		sellPx = _last_tick->price() - _comm_info->getPriceTick() * _price_offset;
	}
	else if(_price_mode == 1)
	{
		buyPx = _last_tick->askprice(0) + _comm_info->getPriceTick() * _price_offset;
		sellPx = _last_tick->bidprice(0) - _comm_info->getPriceTick() * _price_offset;
	}
	else if(_price_mode == 2)
	{
		double mp = (_last_tick->bidqty(0) - _last_tick->askqty(0))*1.0 / (_last_tick->bidqty(0) + _last_tick->askqty(0));
		bool isUp = (mp > 0);
		if(isUp)
		{
			buyPx = _last_tick->askprice(0) + _comm_info->getPriceTick() * _cancel_times;
			sellPx = _last_tick->askprice(0) - _comm_info->getPriceTick() * _cancel_times;
		}
		else
		{
			buyPx = _last_tick->bidprice(0) + _comm_info->getPriceTick() * _cancel_times;
			sellPx = _last_tick->bidprice(0) - _comm_info->getPriceTick() * _cancel_times;
		}
	}

	//检查涨跌停价
	bool isCanCancel = true;
	if (!decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(buyPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog("Buy price %f of %s modified to upper limit price", buyPx, _code.c_str(), _last_tick->upperlimit());
		buyPx = _last_tick->upperlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}
	
	if (!decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(sellPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog("Sell price %f of %s modified to lower limit price", buyPx, _code.c_str(), _last_tick->upperlimit());
		sellPx = _last_tick->lowerlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}

	if (isBuy)
	{
		OrderIDs ids = _ctx->buy(stdCode, buyPx, this_qty);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, this_qty);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}

	_last_place_time = now;
}

void WtMinImpactExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	if (_target_pos != newVol)
	{
		_target_pos = newVol;
	}

	do_calc();
}
