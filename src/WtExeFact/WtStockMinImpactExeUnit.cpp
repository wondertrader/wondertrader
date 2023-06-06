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
	//, _cancel_cnt(0)
	, _target_pos(0)
	, _cancel_times(0)
	, _last_place_time(0)
	, _last_tick_time(0)
	, _is_clear{ false }
	, _min_order{ 0 }
	, _is_KC{ false }
	, _is_cancel_unmanaged_order{ true }
	, _is_finish{ true }
	, _is_first_tick{ true }
	, _is_ready{ false }
	, _is_total_money_ready{ false }
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
	if (cfg->has("total_money"))
	{
		_is_total_money_ready = true;
		_total_money = cfg->getDouble("total_money"); //执行器使用的金额，配合目标比例使用，该字段留空或值小于0，则以账户当前余额为准
	}
	if (cfg->has("is_cancel_unmanaged_order"))
		_is_cancel_unmanaged_order = cfg->getBoolean("is_cancel_unmanaged_order");
	if (cfg->has("max_cancel_time"))
		_max_cancel_time = cfg->getInt32("max_cancel_time");

	int code = std::stoi(StrUtil::split(stdCode, ".")[2]);
	if (code >= 688000)
	{
		_is_KC = true;
	}
	_min_hands = get_minOrderQty(stdCode);
	if (cfg->has("min_order"))					//最小下单数
		_min_order = cfg->getDouble("min_order");

	if (_min_order != 0)
	{
		if (_is_KC)
		{
			_min_order = max(_min_order, _min_hands);
		}
		else
		{
			//_min_order = max(_min_order, _min_hands);
			_min_order = min(_min_order, _min_hands);//2023.6.5-zhaoyk
		}
	}

	// 确定T0交易模式
	if (_comm_info->getTradingMode() == TradingMode::TM_Long)
		_is_t0 = true;

	//auto ticks = _ctx->getTicks(_code.c_str(),1);
	//ticks->release();
	ctx->writeLog(fmt::format("MiniImpactExecUnit {} inited, order price: {} ± {} ticks, order expired: {} secs, order timespan:{} millisec, order qty: {} @ {:.2f} min_order: {:.2f} is_cancel_unmanaged_order: {}",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots, _min_order, _is_cancel_unmanaged_order ? "true" : "false").c_str());
}

void WtStockMinImpactExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	{
		StdUniqueLock lock(_mtx_calc);
		_ctx->writeLog(fmtutil::format("on_order localid:{} stdCode:{} isBuy:{} leftover:{} price:{} isCanceled:{}", localid, stdCode, isBuy, leftover, price, isCanceled));
		if (!_orders_mon.has_order(localid))
		{
			// 非管理的合约订单
			_ctx->writeLog(fmtutil::format("{} {} didnt in mon", stdCode, localid));
			return;
		}

		if (isCanceled || leftover == 0)
		{
			_orders_mon.erase_order(localid);
			if (isCanceled)
				_ctx->writeLog(fmtutil::format("{} {} canceled, earse from mon", stdCode, localid));
			else
				_ctx->writeLog(fmtutil::format("{} {} done, earse from mon", stdCode, localid));
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
	_ctx->writeLog("=================================channle ready==============================");
	_is_ready = true;
	check_unmanager_order();
	do_calc();
}

void WtStockMinImpactExeUnit::on_channel_lost()
{
}

void WtStockMinImpactExeUnit::on_account(const char* currency, double prebalance, double balance, double dynbalance, double avaliable, double closeprofit, double dynprofit, double margin, double fee, double deposit, double withdraw)
{
	if (strcmp(currency, "CNY") == 0)
	{
		_ctx->writeLog(fmtutil::format("avaliable update {}->:{}", _avaliable, avaliable));
		_avaliable = avaliable;
	}
}

void WtStockMinImpactExeUnit::check_unmanager_order()
{
	double undone = _ctx->getUndoneQty(_code.c_str());
	_orders_mon.clear_orders();

	if (!decimal::eq(undone, 0) && _is_cancel_unmanaged_order)
	{
		_ctx->writeLog(fmt::format("{} Unmanaged live orders with qty {} of {} found, cancel all", _code, undone, _code.c_str()).c_str());
		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _now);
		for (auto id : ids)
			_ctx->writeLog(fmt::format("{} mon push unmanager order {} enter time:{}", _code.c_str(), id, _now).c_str());
	}
}

void WtStockMinImpactExeUnit::on_tick(WTSTickData* newTick)
{
	_now = TimeUtils::getLocalTimeNow();
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

	//如果相比上次没有更新的tick进来，则先不下单，防止开盘前集中下单导致通道被封
	uint64_t curTickTime = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (curTickTime <= _last_tick_time)
	{
		_ctx->writeLog(fmt::format("No tick of {} updated, {} <= {}, execute later",
			_code, curTickTime, _last_tick_time).c_str());
		return;
	}
	_last_tick_time = curTickTime;

	/*
	 *	这里可以考虑一下
	 *	如果写的上一次丢出去的单子不够达到目标仓位
	 *	那么在新的行情数据进来的时候可以再次触发核心逻辑
	 */

	_orders_mon.enumOrder([this](uint32_t localid, uint64_t entertime, bool cancancel) {
		_ctx->writeLog(fmtutil::format("[{}]{} entertime:{} cancancel:{} now:{} last_tick_time:{} live_time:{}", _code, localid, entertime, cancancel, _now, _last_tick_time, _now - entertime));
	});

	if (_expire_secs != 0 && _orders_mon.has_order())
	{
		_orders_mon.check_orders(_expire_secs, _now, [this](uint32_t localid) {
			if (_ctx->cancel(localid))
			{
				_ctx->writeLog(fmt::format("[{}] Expired order of {} canceled", localid, _code.c_str()).c_str());
				if (_cancel_map.find(localid) == _cancel_map.end())
				{
					_cancel_map[localid] = 0;
				}
				_cancel_map[localid] += 1;
			}
		});
	}
	if (!_cancel_map.empty())
	{
		std::vector<uint32_t> erro_cancel_orders{};
		for (auto item : _cancel_map)
		{
			if (item.second > _max_cancel_time)
			{
				erro_cancel_orders.push_back(item.first);
			}
		}
		for (uint32_t localid : erro_cancel_orders)
		{
			_cancel_map.erase(localid);
			_orders_mon.erase_order(localid);
			_ctx->writeLog(fmtutil::format("erro order:{} canceled by {} times,erase forcely", localid, _max_cancel_time));
		}
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
		StdUniqueLock lock(_mtx_calc);
		//如果不是我发出去的订单,我就不管了
		if (!_orders_mon.has_order(localid))
			return;

		_orders_mon.erase_order(localid);
		_ctx->writeLog(fmtutil::format("{} {} entrust failed erase from mon", _code.c_str(), localid));
	}
	// 委托不成功，重新处理
	do_calc();
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
	double cur_pos = _ctx->getPosition(stdCode);

	if (decimal::eq(cur_pos, newVol))
		return;

	if (decimal::lt(newVol, 0))
	{
		_ctx->writeLog(fmt::format("{} is a erro stock target position", newVol).c_str());
		return;
	}

	_target_pos = newVol;

	_target_mode = TargetMode::stocks;
	if (is_clear())
		_ctx->writeLog(fmt::format("{} is set to be in clearing processing", stdCode).c_str());
	else
		_ctx->writeLog(fmt::format("Target position of {} is set tb be {}", stdCode, _target_pos).c_str());

	_is_finish = false;
	_start_time = TimeUtils::getLocalTimeNow();
	WTSTickData* tick = _ctx->grabLastTick(_code.c_str());
	if (tick)
	{
		_start_price = tick->price();
		tick->release();
	}
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
	if (!_last_tick)
		return;
	if (_is_finish)
		return;
	if (!_is_ready)
	{
		_ctx->writeLog(fmtutil::format("{} wait channel ready", _code));
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
	// 总仓位，等于昨仓 + 今仓买入的
	double curPos = _ctx->getPosition(stdCode, false);
	// 可用仓位，即昨仓的
	double vailyPos = _ctx->getPosition(stdCode, true);
	if (_is_t0)
		vailyPos = curPos;
	double target_pos = max(curPos - vailyPos, _target_pos);

	if (!decimal::eq(target_pos, _target_pos))
	{
		_ctx->writeLog(fmtutil::format("{} can sell hold pos not enough, target adjust {}->{}", stdCode, _target_pos, target_pos));
		_target_pos = target_pos;
	}

	// 补一次
	if (decimal::ge(_start_price, 0))
	{
		_start_price = _last_tick->price();
	}

	double diffPos = target_pos - curPos;
	_ctx->writeLog(fmtutil::format("{}: target: {} hold:{} left {} wait to execute", _code.c_str(), target_pos, curPos, diffPos));
	// 在判断的时候，要两边四舍五入，防止一些碎股导致一直无法完成执行
	if (decimal::eq(round_hands(target_pos, _min_hands), round_hands(curPos, _min_hands)) && !(target_pos == 0 && curPos < _min_hands && curPos > target_pos))
	{
		_ctx->writeLog(fmtutil::format("{}: target position {} set finish", _code.c_str(), _target_pos));
		_is_finish = true;
		return;
	}

	bool isBuy = decimal::gt(diffPos, 0);
	//有未完成订单，与实际仓位变动方向相反
	//则需要撤销现有订单
	if (decimal::lt(diffPos * undone, 0))
	{
		_ctx->writeLog(fmt::format("{} undone:{} diff:{} cancel", stdCode, undone, diffPos).c_str());
		bool isBuy = decimal::gt(undone, 0);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		if (!ids.empty())
		{
			_orders_mon.push_order(ids.data(), ids.size(), _now);
			for (auto localid : ids)
			{
				_ctx->writeLog(fmt::format("{} mon push wait cancel order {} enter time:{}", _code.c_str(), localid, _now).c_str());
				_ctx->writeLog(fmt::format("[{}] live opposite order of {} canceled", localid, _code.c_str()).c_str());
			}
		}
		return;
	}

	//因为是逐笔发单，所以如果有不需要撤销的未完成单，则暂不发单
	if (!decimal::eq(undone, 0))
	{
		_ctx->writeLog(fmtutil::format("{} undone {} wait...", _code, undone));
		return;
	}

	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("No lastest tick data of {}, execute later", _code.c_str()).c_str());
		return;
	}

	//检查下单时间间隔;
	if (_now - _last_place_time < _entrust_span)
	{
		_ctx->writeLog(fmtutil::format("entrust span {} last_place_time {} _now {}", _entrust_span, _last_place_time, _now));
		return;
	}

	double this_qty = _order_lots;
	if (_by_rate)
	{
		double book_qty = isBuy ? _last_tick->askqty(0) : _last_tick->bidqty(0);
		book_qty = book_qty * _qty_rate;
		//book_qty = round_hands(book_qty, _min_hands); 
		book_qty = round_hands(book_qty, _min_order);//2023.6.5-zhaoyk
		book_qty = max(_min_order, book_qty);
		this_qty = book_qty;
	}
	diffPos = abs(diffPos);
	this_qty = min(this_qty, diffPos);
	// 买
	if (isBuy)
	{
		//如果是买的话，要考虑取整和资金余额
		//this_qty = round_hands(this_qty, _min_hands);
		this_qty = round_hands(this_qty, _min_order);//2023.6.5-zhaoyk
		if (_avaliable)
		{
			double max_can_buy = _avaliable / _last_tick->price();
			//max_can_buy = (int)(max_can_buy / _min_hands) * _min_hands;
			max_can_buy = (int)(max_can_buy / _min_order) * _min_order;//2023.6.5-zhaoyk
			this_qty = min(max_can_buy, this_qty);
		}
	}
	// 卖要对碎股做检查
	else
	{
		//double chip_stk = vailyPos - int(vailyPos / _min_hands) * _min_hands;
		//if (decimal::lt(vailyPos, _min_hands))
			if (decimal::lt(vailyPos, _min_order))//2023.6.5-zhaoyk
		{
			this_qty = vailyPos;
		}
		else
		{
			//this_qty = round_hands(this_qty, _min_hands);
			this_qty = round_hands(this_qty, _min_order);//2023.6.5-zhaoyk

		}
		this_qty = min(vailyPos, this_qty);
	}

	if (decimal::eq(this_qty, 0))
		return;

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
			buyPx = _last_tick->askprice(0) + _comm_info->getPriceTick() * _price_offset;
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
		_orders_mon.push_order(ids.data(), ids.size(), _now, isCanCancel);
		for (auto id : ids)
		{
			_ctx->writeLog(fmt::format("{} mon push buy order {} enter time:{}", _code.c_str(), id, _now).c_str());
		}
	}
	else
	{
		OrderIDs ids = _ctx->sell(stdCode, sellPx, this_qty);
		_orders_mon.push_order(ids.data(), ids.size(), _now, isCanCancel);
		for (auto id : ids)
		{
			_ctx->writeLog(fmt::format("{} mon push sell order {} enter time:{}", _code.c_str(), id, _now).c_str());
		}
	}

	_last_place_time = _now;
}