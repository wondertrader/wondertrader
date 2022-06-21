/*
 * 适用于股票的最小冲击算法
 *
 * \author Huerjie
 * \date 2022/06/01
 *
 *
 */

#include "WtStockMinImpactExeUnit.h"

extern const char* FACT_NAME;

WtStockMinImpactExeUnit::WtStockMinImpactExeUnit()
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
	, _is_clear{ false }
	, _is_target_pos{ false }
	, _min_order{100}
	, _is_KC{false}
{
}

WtStockMinImpactExeUnit::~WtStockMinImpactExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtStockMinImpactExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtStockMinImpactExeUnit::getName()
{
	return "WtStockMinImpactExeUnit";
}

void WtStockMinImpactExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
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

	int code = std::stoi(StrUtil::split(stdCode, ".")[2]);
	if (code >= 688000)
	{
		_is_KC = true;
	}
	if (cfg->has("min_order"))					//最小下单数
		_min_order = cfg->getDouble("min_order");

	if (_min_order != 0)
	{
		if (_is_KC)
		{
			_min_order = max(_min_order, 200.0);
		}
		else
		{
			_min_order = max(_min_order, 100.0);
		}
	}

	//auto ticks = _ctx->getTicks(_code.c_str(),1);
	//ticks->release();
	ctx->writeLog(fmt::format("MiniImpactExecUnit {} inited, order price: {} ± {} ticks, order expired: {} secs, order timespan:{} millisec, order qty: {} @ {:.2f} min_order: {:.2f}",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots, _min_order).c_str());
}

void WtStockMinImpactExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
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
				_ctx->writeLog(fmt::format("Order of {} cancelling done, cancelcnt -> {}",_code.c_str(), _cancel_cnt).c_str());
			}
		}

		if (leftover == 0 && !isCanceled)
			_cancel_times = 0;
	}

	//如果有撤单,也触发重新计算
	if (isCanceled)
	{
		_ctx->writeLog(fmt::format("Order {} of {} canceled, recalc will be done", localid, stdCode).c_str());
		_cancel_times++;
		do_calc();
	}
}

void WtStockMinImpactExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());
	_orders_mon.clear_orders();

	if (!decimal::eq(undone, 0))
	{
		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();
		_ctx->writeLog(fmt::format("cancelcnt -> {}", _cancel_cnt).c_str());
	}
	_ctx->writeLog("channle ready");
	do_calc();
}

void WtStockMinImpactExeUnit::on_channel_lost()
{
}

void WtStockMinImpactExeUnit::on_tick(WTSTickData* newTick)
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

	if (_expire_secs != 0 && _orders_mon.has_order() && _cancel_cnt == 0)
	{
		uint64_t now = _ctx->getCurTime();

		_orders_mon.check_orders(_expire_secs, now, [this](uint32_t localid) {
			if (_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog(fmt::format("[{}@{}] Expired order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt).c_str());
			}
		});
	}

	do_calc();
}

void WtStockMinImpactExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
}

void WtStockMinImpactExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//如果不是我发出去的订单,我就不管了
		if (!_orders_mon.has_order(localid))
			return;

		_orders_mon.erase_order(localid);
		// 委托不成功，重新处理
		do_calc();
	}
}

void WtStockMinImpactExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	//如果原来的目标仓位是DBL_MAX，说明已经进入清理逻辑
	//如果这个时候又设置为0，则直接跳过了
	if (is_clear() && decimal::eq(newVol, 0))
	{
		_ctx->writeLog(fmt::format("{} is in clearing processing, position can not be set to 0", stdCode).c_str());
		return;
	}

	if (decimal::eq(_target_pos, newVol))
		return;

	if (decimal::lt(_target_pos, 0))
	{
		_ctx->writeLog(fmt::format("{} is a erro stock target position", newVol).c_str());
		return;
	}


	if (newVol < 0)
	{
		_ctx->writeLog(fmt::format("{} is not a valid stock pos value", newVol).c_str());
	}
	_target_pos = newVol;
	if (!decimal::eq(newVol, 0))
		_is_clear = false;
	else
		_is_clear = true;
	_is_target_pos = true;
	if (is_clear())
		_ctx->writeLog(fmt::format("{} is set to be in clearing processing", stdCode).c_str());
	else
		_ctx->writeLog(fmt::format("Target position of {} is set tb be {}", stdCode, _target_pos).c_str());

	do_calc();
}

void WtStockMinImpactExeUnit::set_amount(const char* stdCode, double newAmount)
{
	if (_code.compare(stdCode) != 0)
		return;

	//如果原来的目标仓位是DBL_MAX，说明已经进入清理逻辑
	//如果这个时候又设置为0，则直接跳过了
	if (is_clear() && decimal::eq(newAmount, 0))
	{
		_ctx->writeLog(fmt::format("{} is in clearing processing, position can not be set to 0", stdCode).c_str());
		return;
	}

	if (decimal::eq(_target_amount, newAmount))
		return;

	if (newAmount < 0)
	{
		_ctx->writeLog(fmt::format("{} is not a valid stock amount value", newAmount).c_str());
	}
	_target_amount = newAmount;
	if (!decimal::eq(newAmount, 0))
		_is_clear = false;
	_is_target_pos = false;
	if (is_clear())
		_ctx->writeLog(fmt::format("{} is set to be in clearing processing", stdCode).c_str());
	else
		_ctx->writeLog(fmt::format("Target amount of {} is set tb be {}", stdCode, _target_amount).c_str());

	do_calc();
}

void WtStockMinImpactExeUnit::clear_all_position(const char* stdCode)
{
	if (_code.compare(stdCode) != 0)
		return;

	_is_clear = true;
	_target_pos = 0;
	_target_amount = 0;
	do_calc();
}

inline bool WtStockMinImpactExeUnit::is_clear()
{
	return _is_clear;
}

void WtStockMinImpactExeUnit::do_calc()
{
	if (_cancel_cnt != 0)
		return;
	if (!_last_tick)
		return;

	//这里加一个锁，主要原因是实盘过程中发现
	//在修改目标仓位的时候，会触发一次do_calc
	//而ontick也会触发一次do_calc，两次调用是从两个线程分别触发的，所以会出现同时触发的情况
	//如果不加锁，就会引起问题
	//这种情况在原来的SimpleExecUnit没有出现，因为SimpleExecUnit只在set_position的时候触发
	StdUniqueLock lock(_mtx_calc);

	// 算一次目标持仓
	if (!_is_target_pos)
	{
		_target_pos = _target_amount / _last_tick->price();
		_target_pos = round_hands(_target_pos);
		_ctx->writeLog(fmtutil::format("{} target_pos calculate by {} / {} to be set as {}", _code, _target_amount, _last_tick->price(), (int)_target_pos));
		_is_target_pos = true;
	}

	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	// 可用仓位，即昨仓的
	double vailyPos = _ctx->getPosition(stdCode, true);
	// 总仓位，等于昨仓 + 今仓买入的
	double curPos = _ctx->getPosition(stdCode, false);

	double target_pos = max(curPos - vailyPos, _target_pos);

	if (!decimal::eq(target_pos, _target_pos))
	{
		_ctx->writeLog(fmtutil::format("可用仓位不足，目标仓位变更 {}->{}", _target_pos, target_pos));
		_target_pos = target_pos;
	}

	double diffPos = target_pos - curPos;
	if (decimal::eq(diffPos, 0))
	{
		//_ctx->writeLog(fmtutil::format("{}: target position {} set finish", _code.c_str(), _target_pos));
		return;
	}

	bool isBuy = decimal::gt(diffPos,0);
	//有未完成订单，与实际仓位变动方向相反
	//则需要撤销现有订单
	if (decimal::lt(diffPos * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		if (!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
			_cancel_cnt += ids.size();
			_ctx->writeLog(fmt::format("[{}@{}] live opposite order of {} canceled, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt).c_str());
		}
		return;
	}

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
		return;

	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("No lastest tick data of {}, execute later", _code.c_str()).c_str());
		return;
	}

	//检查下单时间间隔
	uint64_t now = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (now - _last_place_time < _entrust_span)
		return;

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmt::format("No tick of {} updated, {} <= {}, execute later",
			_code, curTickTime, _last_tick_time).c_str());
		return;
	}

	_last_tick_time = curTickTime;

	double this_qty = _order_lots;
	if (_by_rate)
	{
		this_qty = isBuy ? _last_tick->askqty(0) : _last_tick->bidqty(0);
		this_qty = round_hands(this_qty * _qty_rate);
	}

	this_qty = min(this_qty, diffPos);
	if (!decimal::eq(_min_order,0))
	{
		this_qty = max(_min_order, this_qty);
	}
	else
	{
		if (decimal::lt(this_qty, _min_order))
			return;
	}

	double buyPx, sellPx;
	if (_price_mode == AUTOPX)
	{
		double mp = (_last_tick->bidqty(0) - _last_tick->askqty(0)) * 1.0 / (_last_tick->bidqty(0) + _last_tick->askqty(0));
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
	}
	else
	{
		if (_price_mode == BESTPX)
		{
			buyPx = _last_tick->bidprice(0);
			sellPx = _last_tick->askprice(0);
		}
		else if (_price_mode == LASTPX)
		{
			buyPx = _last_tick->price();
			sellPx = _last_tick->price();
		}
		else if (_price_mode == MARKET)
		{
			buyPx = _last_tick->askprice(0);
			sellPx = _last_tick->bidprice(0) - _comm_info->getPriceTick() * _price_offset;
		}
	}

	// 如果最后价格为0，再做一个修正
	if (decimal::eq(buyPx, 0.0))
		buyPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

	if (decimal::eq(sellPx, 0.0))
		sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

	buyPx += _comm_info->getPriceTick() * _cancel_times;
	sellPx -= _comm_info->getPriceTick() * _cancel_times;

	//检查涨跌停价
	bool isCanCancel = true;
	if (!decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(buyPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog(fmt::format("Buy price {} of {} modified to upper limit price", buyPx, _code.c_str(), _last_tick->upperlimit()).c_str());
		buyPx = _last_tick->upperlimit();
		isCanCancel = false;	//如果价格被修正为涨跌停价，订单不可撤销
	}

	if (!decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(sellPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmt::format("Sell price {} of {} modified to lower limit price", sellPx, _code.c_str(), _last_tick->lowerlimit()).c_str());
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
		OrderIDs ids = _ctx->sell(stdCode, sellPx, this_qty);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime(), isCanCancel);
	}

	_last_place_time = now;
}