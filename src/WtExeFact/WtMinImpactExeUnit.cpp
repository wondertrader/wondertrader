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
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

const char* PriceModeNames[] =
{
	"BESTPX",		//���ż�
	"LASTPX",		//���¼�
	"MARKET",		//���ּ�
	"AUTOPX"		//�Զ�
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

	_price_offset = cfg->getInt32("offset");	//�۸�ƫ��������һ��Ͷ���ͬ����
	_expire_secs = cfg->getUInt32("expire");	//������ʱ����
	_price_mode = cfg->getInt32("pricemode");	//�۸�����,0-���¼�,-1-���ż�,1-���ּ�,2-�Զ�,Ĭ��Ϊ0
	_entrust_span = cfg->getUInt32("span");		//����ʱ��������λ����
	_by_rate = cfg->getBoolean("byrate");		//�Ƿ��ն��ֵĹҵ����ı����µ��������true����rate�ֶ���Ч�������false��lots�ֶ���Ч
	_order_lots = cfg->getDouble("lots");		//���η�������
	_qty_rate = cfg->getDouble("rate");			//�µ���������

	ctx->writeLog(fmt::format("MiniImpactExecUnit {} inited, order price: {} �� {} ticks, order expired: {} secs, order timespan:{} millisec, order qty: {} @ {:.2f}",
		stdCode, PriceModeNames[_price_mode + 1], _price_offset, _expire_secs, _entrust_span, _by_rate ? "byrate" : "byvol", _by_rate ? _qty_rate : _order_lots).c_str());
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
				_ctx->writeLog(fmt::format("[{}@{}] Order of {} cancelling done, cancelcnt -> {}", __FILE__, __LINE__, _code.c_str(), _cancel_cnt).c_str());
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

void WtMinImpactExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());

	if(!decimal::eq(undone, 0) && !_orders_mon.has_order())
	{
		/*
		 *	���δ��ɵ���Ϊ0����OMSû�ж���
		 *	��˵����δ��ɵ����ڼ��֮��,ȫ��������
		 *	��Ϊ��Щ����û�б��ض����ţ��޷�ֱ�ӽ��й���
		 *	������������Ǹ�������ʱ���ϴε�δ��ɵ������ⲿ�Ĺҵ�
		 */
		_ctx->writeLog(fmt::format("Unmanaged live orders with qty {} of {} found, cancel all", undone, _code.c_str()).c_str());

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		_orders_mon.push_order(ids.data(), ids.size(), _ctx->getCurTime());
		_cancel_cnt += ids.size();

		_ctx->writeLog(fmt::format("[{}@{}]cancelcnt -> {}", __FILE__, __LINE__, _cancel_cnt).c_str());
	}
	else if (decimal::eq(undone, 0) && _orders_mon.has_order())
	{
		/*
		 *	By Wesey @ 2021.12.13
		 *	���δ��ɵ�Ϊ0������OMS�����ж�����
		 *	˵��OMS���Ǵ�����Ҫ���������Ȼ��ʱ�����ͻ����
		 *	���������һ���Ƕ��������Ժ�֮ǰ�³�ȥ�Ķ�������û���������͵���̨
		 *	����������Ҫ��������ض���
		 */
		_ctx->writeLog(fmt::format("Local orders of {} not confirmed in trading channel, clear all", _code.c_str()).c_str());
		_orders_mon.clear_orders();
	}
	else
	{
		_ctx->writeLog(fmt::format("Unrecognized condition while channle ready, {:.2f} live orders of {} exists, local orders {}exist",
			undone, _code.c_str(), _orders_mon.has_order() ? "" : "not ").c_str());
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

	if(_expire_secs != 0 && _orders_mon.has_order() && _cancel_cnt==0)
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

void WtMinImpactExeUnit::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	//���ô���,������ontick�ﴥ����
}

void WtMinImpactExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//��������ҷ���ȥ�Ķ���,�ҾͲ�����
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

	//�����һ��������Ҫԭ����ʵ�̹����з���
	//���޸�Ŀ���λ��ʱ�򣬻ᴥ��һ��do_calc
	//��ontickҲ�ᴥ��һ��do_calc�����ε����Ǵ������̷ֱ߳𴥷��ģ����Ի����ͬʱ���������
	//������������ͻ���������
	//���������ԭ����SimpleExecUnitû�г��֣���ΪSimpleExecUnitֻ��set_position��ʱ�򴥷�
	StdUniqueLock lock(_mtx_calc);

	double newVol = get_real_target(_target_pos);
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);
	double diffPos = newVol - realPos;

	//��δ��ɶ�������ʵ�ʲ�λ�䶯�����෴
	//����Ҫ�������ж���
	if (decimal::lt(diffPos * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		if(!ids.empty())
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

	double curPos = realPos;


	if (_last_tick == NULL)
	{
		_ctx->writeLog(fmt::format("No lastest tick data of {}, execute later", _code.c_str()).c_str());
		return;
	}

	//����µ�ʱ����
	uint64_t now = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());
	if (now - _last_place_time < _entrust_span)
		return;

	if (decimal::eq(curPos, newVol))
	{
		//��ǰ��λ�����²�λƥ��ʱ���������ȫ����ֵ�������ֱ���˳�������
		if (!is_clear(_target_pos))
			return;

		//�������ֵ����󣬻�Ҫ�ٽ��жԱ�
		//�����ͷΪ0��˵���Ѿ�ȫ��������ˣ���ֱ���˳�
		double lPos = _ctx->getPosition(stdCode, 1);
		if (decimal::eq(lPos, 0))
			return;

		//������ж�ͷ��λ����Ŀ���λ����Ϊ��0��ǿ�ƴ���
		newVol = -min(lPos, _order_lots);
		_ctx->writeLog(fmt::format("Clearing process triggered, target position of {} has been set to {}", _code.c_str(), newVol).c_str());
	}

	bool bForceClose = is_clear(_target_pos);

	bool isBuy = decimal::gt(newVol, curPos);

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
		this_qty = round(this_qty*_qty_rate);
		if (decimal::lt(this_qty, 1))
			this_qty = 1;

		this_qty = min(this_qty, abs(newVol - curPos));
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
		 *	������۸�Ϊ0������һ������
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
		 *	������۸�Ϊ0������һ������
		 */
		if (decimal::eq(buyPx, 0.0))
			buyPx = decimal::eq(_last_tick->price(), 0.0)? _last_tick->preclose(): _last_tick->price();

		if (decimal::eq(sellPx, 0.0))
			sellPx = decimal::eq(_last_tick->price(), 0.0) ? _last_tick->preclose() : _last_tick->price();

		buyPx += _comm_info->getPriceTick() * _price_offset;
		sellPx -= _comm_info->getPriceTick() * _price_offset;
	}
	

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

	//���ԭ����Ŀ���λ��DBL_MAX��˵���Ѿ����������߼�
	//������ʱ��������Ϊ0����ֱ��������
	if (is_clear(_target_pos) && decimal::eq(newVol, 0))
	{
		_ctx->writeLog(fmt::format("{} is in clearing processing, position can not be set to 0", stdCode).c_str());
		return;
	}

	if (decimal::eq(_target_pos, newVol))
		return;

	_target_pos = newVol;

	if (is_clear(_target_pos))
		_ctx->writeLog(fmt::format("{} is set to be in clearing processing", stdCode).c_str());
	else
		_ctx->writeLog(fmt::format("Target position of {} is set tb be {}", stdCode, _target_pos).c_str());

	do_calc();
}

void WtMinImpactExeUnit::clear_all_position(const char* stdCode)
{
	if (_code.compare(stdCode) != 0)
		return;

	_target_pos = DBL_MAX;

	do_calc();
}
