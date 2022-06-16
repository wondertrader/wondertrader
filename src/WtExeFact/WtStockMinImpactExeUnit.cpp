/*
 * �����ڹ�Ʊ����С����㷨
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

	_price_offset = cfg->getInt32("offset");	//�۸�ƫ��������һ��Ͷ���ͬ����
	_expire_secs = cfg->getUInt32("expire");	//������ʱ����
	_price_mode = cfg->getInt32("pricemode");	//�۸�����,0-���¼�,-1-���ż�,1-���ּ�,2-�Զ�,Ĭ��Ϊ0
	_entrust_span = cfg->getUInt32("span");		//����ʱ��������λ����
	_by_rate = cfg->getBoolean("byrate");		//�Ƿ��ն��ֵĹҵ����ı����µ��������true����rate�ֶ���Ч�������false��lots�ֶ���Ч
	_order_lots = cfg->getDouble("lots");		//���η�������
	_qty_rate = cfg->getDouble("rate");			//�µ���������

	int code = std::stoi(StrUtil::split(stdCode, ".")[2]);
	if (code >= 688000)
	{
		_is_KC = true;
	}
	if (cfg->has("min_order"))					//��С�µ���
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
	ctx->writeLog(fmt::format("MiniImpactExecUnit {} inited, order price: {} �� {} ticks, order expired: {} secs, order timespan:{} millisec, order qty: {} @ {:.2f} min_order: {:.2f}",
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

	//����г���,Ҳ�������¼���
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

	//���ԭ����tick��Ϊ��,��Ҫ�ͷŵ�
	if (_last_tick)
	{
		_last_tick->release();
	}
	else
	{
		//�������ʱ�䲻�ڽ���ʱ��,�������һ���Ǽ��Ͼ��۵��������,�µ���ʧ��,����ֱ�ӹ��˵��������
		if (_sess_info != NULL && !_sess_info->isInTradingTime(newTick->actiontime() / 100000))
			return;
	}

	//�µ�tick����,Ҫ����
	_last_tick = newTick;
	_last_tick->retain();

	/*
	 *	������Կ���һ��
	 *	���д����һ�ζ���ȥ�ĵ��Ӳ����ﵽĿ���λ
	 *	��ô���µ��������ݽ�����ʱ������ٴδ��������߼�
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
		//��������ҷ���ȥ�Ķ���,�ҾͲ�����
		if (!_orders_mon.has_order(localid))
			return;

		_orders_mon.erase_order(localid);
		// ί�в��ɹ������´���
		do_calc();
	}
}

void WtStockMinImpactExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	//���ԭ����Ŀ���λ��DBL_MAX��˵���Ѿ����������߼�
	//������ʱ��������Ϊ0����ֱ��������
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

	//���ԭ����Ŀ���λ��DBL_MAX��˵���Ѿ����������߼�
	//������ʱ��������Ϊ0����ֱ��������
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

	//�����һ��������Ҫԭ����ʵ�̹����з���
	//���޸�Ŀ���λ��ʱ�򣬻ᴥ��һ��do_calc
	//��ontickҲ�ᴥ��һ��do_calc�����ε����Ǵ������̷ֱ߳𴥷��ģ����Ի����ͬʱ���������
	//������������ͻ���������
	//���������ԭ����SimpleExecUnitû�г��֣���ΪSimpleExecUnitֻ��set_position��ʱ�򴥷�
	StdUniqueLock lock(_mtx_calc);

	// ��һ��Ŀ��ֲ�
	if (!_is_target_pos)
	{
		_target_pos = _target_amount / _last_tick->price();
		_target_pos = round_hands(_target_pos);
		_ctx->writeLog(fmtutil::format("{} target_pos calculate by {} / {} to be set as {}", _code, _target_amount, _last_tick->price(), (int)_target_pos));
		_is_target_pos = true;
	}

	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	// ���ò�λ������ֵ�
	double vailyPos = _ctx->getPosition(stdCode, true);
	// �ܲ�λ��������� + ��������
	double curPos = _ctx->getPosition(stdCode, false);

	double target_pos = max(curPos - vailyPos, _target_pos);

	if (!decimal::eq(target_pos, _target_pos))
	{
		_ctx->writeLog(fmtutil::format("���ò�λ���㣬Ŀ���λ��� {}->{}", _target_pos, target_pos));
		_target_pos = target_pos;
	}

	double diffPos = target_pos - curPos;
	if (decimal::eq(diffPos, 0))
	{
		//_ctx->writeLog(fmtutil::format("{}: target position {} set finish", _code.c_str(), _target_pos));
		return;
	}

	bool isBuy = decimal::gt(diffPos,0);
	//��δ��ɶ�������ʵ�ʲ�λ�䶯�����෴
	//����Ҫ�������ж���
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

	//��Ϊ����ʷ�������������в���Ҫ������δ��ɵ������ݲ�����
	if (!decimal::eq(undone, 0))
		return;

	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("No lastest tick data of {}, execute later", _code.c_str()).c_str());
		return;
	}

	//����µ�ʱ����
	uint64_t now = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (now - _last_place_time < _entrust_span)
		return;

	//�������ϴ�û�и��µ�tick���������Ȳ��µ�����ֹ����ǰ�����µ�����ͨ������
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

	// ������۸�Ϊ0������һ������
	if (decimal::eq(buyPx, 0.0))
		buyPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

	if (decimal::eq(sellPx, 0.0))
		sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

	buyPx += _comm_info->getPriceTick() * _cancel_times;
	sellPx -= _comm_info->getPriceTick() * _cancel_times;

	//����ǵ�ͣ��
	bool isCanCancel = true;
	if (!decimal::eq(_last_tick->upperlimit(), 0) && decimal::gt(buyPx, _last_tick->upperlimit()))
	{
		_ctx->writeLog(fmt::format("Buy price {} of {} modified to upper limit price", buyPx, _code.c_str(), _last_tick->upperlimit()).c_str());
		buyPx = _last_tick->upperlimit();
		isCanCancel = false;	//����۸�����Ϊ�ǵ�ͣ�ۣ��������ɳ���
	}

	if (!decimal::eq(_last_tick->lowerlimit(), 0) && decimal::lt(sellPx, _last_tick->lowerlimit()))
	{
		_ctx->writeLog(fmt::format("Sell price {} of {} modified to lower limit price", sellPx, _code.c_str(), _last_tick->lowerlimit()).c_str());
		sellPx = _last_tick->lowerlimit();
		isCanCancel = false;	//����۸�����Ϊ�ǵ�ͣ�ۣ��������ɳ���
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