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
#include "../Share/StrUtil.hpp"

extern const char* FACT_NAME;

const char* PriceModeNames[] =
{
	"BESTPX",		//最优价
	"LASTPX",		//最新价
	"MARKET",		//对手价
	"AUTOPX"		//自动
};

inline double get_real_target(double target)
{
	if (target == DBL_MAX)
		return 0;

	return target;
}

inline bool is_clear(double target)
{
	return (target == DBL_MAX);
}


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
			{
				_cancel_cnt--;
				_ctx->writeLog("[%s@%d] Order of %s cancelling done, cancelcnt -> %u", __FILE__, __LINE__, _code.c_str(), _cancel_cnt);
			}
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
		/*
		 *	如果未完成单不为0，而OMS没有订单
		 *	这说明有未完成单不在监控之中,全部撤销掉
		 *	因为这些订单没有本地订单号，无法直接进行管理
		 *	这种情况，就是刚启动的时候，上次的未完成单或者外部的挂单
		 */
		_ctx->writeLog("Unmanaged live orders with qty %f of %s found, cancel all", undone, _code.c_str());

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog("[%s@%d]cancelcnt -> %u", __FILE__, __LINE__, _cancel_cnt);
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
		_ctx->writeLog("Local orders of %s not confirmed in trading channel, clear all", _code.c_str());
		_orders_mon.clear_orders();
	}
	else
	{
		_ctx->writeLog("Unrecognized condition while channle ready, %.2f live orders of %s exists, local orders %sexist", undone, _code.c_str(), _orders_mon.has_order() ? "" : "not ");
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
				_ctx->writeLog("[%s@%d] Expired order of %s canceled, cancelcnt -> %u", __FILE__, __LINE__, _code.c_str(), _cancel_cnt);
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

	//这里加一个锁，主要原因是实盘过程中发现
	//在修改目标仓位的时候，会触发一次do_calc
	//而ontick也会触发一次do_calc，两次调用是从两个线程分别触发的，所以会出现同时触发的情况
	//如果不加锁，就会引起问题
	//这种情况在原来的SimpleExecUnit没有出现，因为SimpleExecUnit只在set_position的时候触发
	StdUniqueLock lock(_mtx_calc);

	double newVol = get_real_target(_target_pos);
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);
	double diffPos = newVol - realPos;

	//有未完成订单，与实际仓位变动方向相反
	//则需要撤销现有订单
	if (decimal::lt(diffPos * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		if(!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog("[%s@%d] live opposite order of %s canceled, cancelcnt -> %u", __FILE__, __LINE__, _code.c_str(), _cancel_cnt);
		}
		return;
	}

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
		return;

	double curPos = realPos;

	//检查下单时间间隔
	uint64_t now = TimeUtils::getLocalTimeNow();
	if (now - _last_place_time < _entrust_span)
		return;

	if (_last_tick == NULL)
		_last_tick = _ctx->grabLastTick(stdCode);

	if (_last_tick == NULL)
	{
		_ctx->writeLog("No lastest tick data of %s, execute later", _code.c_str());
		return;
	}

	if (decimal::eq(curPos, newVol))
	{
		//当前仓位和最新仓位匹配时，如果不是全部清仓的需求，则直接退出计算了
		if (!is_clear(_target_pos))
			return;

		//如果是清仓的需求，还要再进行对比
		//如果多头为0，说明已经全部清理掉了，则直接退出
		double lPos = _ctx->getPosition(stdCode, 1);
		if (decimal::eq(lPos, 0))
			return;

		//如果还有都头仓位，则将目标仓位设置为非0，强制触发
		newVol = -min(lPos, _order_lots);
		_ctx->writeLog("Clearing process triggered, target position of %s has been set to %f", _code.c_str(), newVol);
	}

	bool bForceClose = is_clear(_target_pos);

	bool isBuy = decimal::gt(newVol, curPos);

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog("No tick of %s updated, %s <= %s, execute later", _code.c_str(), StrUtil::fmtUInt64(curTickTime).c_str(), StrUtil::fmtUInt64(_last_tick_time).c_str());
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
		OrderIDs ids = _ctx->buy(stdCode, buyPx, this_qty, bForceClose);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, this_qty, bForceClose);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}

	_last_place_time = now;
}

void WtMinImpactExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	//如果原来的目标仓位是DBL_MAX，说明已经进入清理逻辑
	//如果这个时候又设置为0，则直接跳过了
	if (is_clear(_target_pos) && decimal::eq(newVol, 0))
	{
		_ctx->writeLog("%s is in clearing processing, position can not be set to 0", stdCode);
		return;
	}

	if (decimal::eq(_target_pos, newVol))
		return;

	_target_pos = newVol;

	if (is_clear(_target_pos))
		_ctx->writeLog("%s is set to be in clearing processing", stdCode);
	else
		_ctx->writeLog("Target position of %s is set tb be %f", stdCode, _target_pos);

	do_calc();
}

void WtMinImpactExeUnit::clear_all_position(const char* stdCode)
{
	if (_code.compare(stdCode) != 0)
		return;

	_target_pos = DBL_MAX;

	do_calc();
}
