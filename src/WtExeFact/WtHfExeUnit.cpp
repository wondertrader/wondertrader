#include "WtHfExeUnit.h"
#include <math.h>

#include "../Share/TimeUtils.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"

extern const char* FACT_NAME;


WtHfExeUnit::WtHfExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _sticky(0)
	, _cancel_cnt(0)
	, _channel_ready(false)
	, _last_fire_time(0)
	, _exec_begin_time(UINT64_MAX)
	, _target_pos(0)
{
}


WtHfExeUnit::~WtHfExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtHfExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtHfExeUnit::getName()
{
	return "WtHfExeUnit";
}

void WtHfExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_sticky = cfg->getUInt32("sticky");
	_exec_secs = cfg->getUInt32("execsecs");
	_exec_tail = cfg->getUInt32("tailsecs");

	ctx->writeLog("执行单元 WtHfExeUnit[%s] 初始化完成，订单超时 %u 秒，执行时限 %u 秒，收尾时间 %u 秒", stdCode, _sticky, _exec_secs, _exec_tail);
}

void WtHfExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t leftover, double price, bool isCanceled)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	if (isCanceled || leftover == 0)
	{
		StdLocker<StdRecurMutex> lock(_mtx_ords);
		_orders.erase(it);
		if (_cancel_cnt > 0)
			_cancel_cnt--;

		_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
	}

	//如果有撤单，也触发重新计算
	if (isCanceled)
	{
		_ctx->writeLog("%s的订单%u已撤销，重新触发执行逻辑", stdCode, localid);
		doCalculate();
	}
}

void WtHfExeUnit::on_channel_ready()
{
	_channel_ready = true;
	int32_t undone = _ctx->getUndoneQty(_code.c_str());
	if (undone != 0 && _orders.empty())
	{
		//这说明有未完成单不在监控之中，先撤掉
		_ctx->writeLog("%s有不在管理中的未完成单 %d 手，全部撤销", _code.c_str(), undone);

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		for (auto localid : ids)
		{
			_orders[localid] = _ctx->getCurTime();
		}
		_cancel_cnt += ids.size();

		_ctx->writeLog("%s cancelcnt -> %u", __FUNCTION__, _cancel_cnt);
	}


	doCalculate();
}

void WtHfExeUnit::on_channel_lost()
{

}

void WtHfExeUnit::on_tick(WTSTickData* newTick)
{
	if (newTick == NULL || _code.compare(newTick->code()) != 0)
		return;

	bool isFirstTick = false;
	//如果原来的tick不为空，则要释放掉
	if (_last_tick)
		_last_tick->release();
	else
		isFirstTick = true;

	//新的tick数据，要保留
	_last_tick = newTick;
	_last_tick->retain();

	/*
	*	这里可以考虑一下
	*	如果写的上一次丢出去的单子不够达到目标仓位
	*	那么在新的行情数据进来的时候可以再次触发核心逻辑
	*/

	if (isFirstTick)	//如果是第一笔tick，则检查目标仓位，不符合则下单
	{
		int32_t newVol = _target_pos;
		const char* stdCode = _code.c_str();

		int32_t undone = _ctx->getUndoneQty(stdCode);
		int32_t realPos = _ctx->getPosition(stdCode);

		if (newVol != undone + realPos)
		{
			doCalculate();
		}
	}
	else
	{
		uint64_t now = _ctx->getCurTime();
		if (_sticky != 0 && !_orders.empty())
		{
			
			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto v : _orders)
			{
				uint32_t localid = v.first;
				uint64_t firetime = v.second;
				if (now - firetime > _sticky * 1000)
				{
					if (_ctx->cancel(localid))
					{
						_cancel_cnt++;
						_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
					}
				}
			}
		}
		
		if(now - _last_fire_time >= _fire_span)
		{
			doCalculate();
		}
	}
}

void WtHfExeUnit::on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price)
{
	//不用触发，这里在ontick里触发吧
	//_ctx->writeLog("%s合约%s%u手，重新触发执行逻辑", stdCode, isBuy?"买入":"卖出", vol);
	//doCalculate();
}

void WtHfExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!bSuccess)
	{
		//如果不是我发出去的订单，我就不管了
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		_orders.erase(it);

		doCalculate();
	}
}

void WtHfExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;

	int32_t newVol = _target_pos;
	const char* stdCode = _code.c_str();

	int32_t undone = _ctx->getUndoneQty(stdCode);
	int32_t realPos = _ctx->getPosition(stdCode);

	//如果有反向未完成单，则直接撤销
	//如果目标仓位为0，且当前持仓为0，则撤销全部挂单
	if (newVol * undone < 0)
	{
		bool isBuy = (undone > 0);
		//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		_cancel_cnt += ids.size();
		_ctx->writeLog("%s@%d cancelcnt -> %u", __FUNCTION__, __LINE__, _cancel_cnt);
		return;
	}
	else if (newVol == 0 && undone != 0)
	{
		//如果目标仓位为0，且未完成不为0
		//那么当目前仓位为0，或者 目前仓位和未完成手数方向相同
		//这样也要全部撤销
		if (realPos == 0 || (realPos * undone > 0))
		{
			bool isBuy = (undone > 0);
			//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
			OrderIDs ids = _ctx->cancel(stdCode, isBuy);
			_cancel_cnt += ids.size();
			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			return;
		}
	}

	bool bNeedShowHand = false;
	uint64_t now = _ctx->getCurTime();
	//如果已经进入尾部时间，则需要一次性发出去
	if (_exec_begin_time != UINT64_MAX && ((now - _exec_begin_time) >= (_exec_secs - _exec_tail) * 1000))
		bNeedShowHand = true;

	if (bNeedShowHand && !_show_hand && undone != 0)
	{
		_ctx->writeLog("已到执行时限末尾阶段，全部撤销所有订单");
		OrderIDs ids = _ctx->cancel(stdCode, undone > 0);
		_cancel_cnt += ids.size();
		_ctx->writeLog("%s@%d cancelcnt -> %u", __FUNCTION__, __LINE__, _cancel_cnt);
		_show_hand = true;
		return;
	}

	//如果都是同向的，则纳入计算
	int32_t curPos = realPos + undone;
	if (curPos == newVol)
		return;

	if (_last_tick == NULL)
	{
		//grabLastTick会自动增加引用计数，不需要再retain
		_last_tick = _ctx->grabLastTick(stdCode);
	}

	if (_last_tick == NULL)
	{
		_ctx->writeLog("%s没有最新tick数据，退出执行逻辑", _code.c_str());
		return;
	}

	if (!_channel_ready)
		return;

	_last_tick->retain();
	WTSTickData* curTick = _last_tick;

	if(bNeedShowHand)
	{
		int32_t qty = newVol - realPos;
		double targetPx = 0;
		if(qty > 0)
			targetPx = curTick->askprice(0) + _comm_info->getPriceTick() * 3;
		else
			targetPx = curTick->bidprice(0) - _comm_info->getPriceTick() * 3;
		OrderIDs ids;
		if(qty > 0)
			ids = _ctx->buy(_code.c_str(), targetPx, abs(qty));
		else
			ids = _ctx->sell(_code.c_str(), targetPx, abs(qty));

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
		return;
	}

	const char* code = _code.c_str();
	int32_t signal = 0;
	double price = curTick->price();
	//计算部分
	double alpha_bias = on_tickalpha_sum(curTick);

	//理论价格大于最新价
	if ((alpha_bias) >= 2)
	{
		//正向信号
		signal = 2;

	}
	else if ((alpha_bias) <= -2)
	{
		//反向信号
		signal = -2;
	}

	if ((alpha_bias) < 2 && (alpha_bias) >= 1)
	{
		//正向信号

		signal = 1;
	}
	else if ((alpha_bias) > -2 && (alpha_bias) <= -1)
	{
		//反向信号
		signal = -1;
	}
	if ((alpha_bias) > -1 && (alpha_bias) < 1)
	{
		signal = 0;
	}

	int32_t diffQty = newVol - curPos;

	if (diffQty > 0)
	{//正向信号，且当前仓位小于等于0
		double targetPx = curTick->bidprice(0) + _comm_info->getPriceTick() * abs(signal);
		auto ids = _ctx->buy(code, targetPx, 1);

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
	}
	else if (diffQty < 0)
	{//反向信号，且当前仓位大于等于0
		double targetPx = curTick->askprice(0) - _comm_info->getPriceTick()* abs(signal);
		auto ids = _ctx->sell(code, targetPx, 1);

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;
	}

	/*
	if (abs(signal) == 2) //  +2tick 追价
	{
		if (signal > 0 && diffQty > 0)	
		{//正向信号，且当前仓位小于等于0
			//最新价+2跳下单
			double targetPx = curTick->askprice(0) + _comm_info->getPriceTick() * abs(signal);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (signal < 0 && diffQty < 0)
		{//反向信号，且当前仓位大于等于0
			//最新价-2跳下单
			double targetPx = curTick->bidprice(0) - _comm_info->getPriceTick()* abs(signal);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	else if (abs(signal) == 1)  //打对手价
	{
		if (signal > 0 && diffQty > 0)
		{//正向信号，且当前仓位小于等于0
			//最新价+2跳下单
			double targetPx = curTick->askprice(0);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (signal < 0 && diffQty < 0)
		{//反向信号，且当前仓位大于等于0
			//最新价-2跳下单
			double targetPx = curTick->bidprice(0);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	else if (abs(signal) == 0)  //挂最优价等待
	{
		if (diffQty > 0)
		{//正向信号，且当前仓位小于等于0
			//最新价+2跳下单
			double targetPx = curTick->bidprice(0);
			auto ids = _ctx->buy(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
		else if (diffQty < 0)
		{//反向信号，且当前仓位大于等于0
			//最新价-2跳下单
			double targetPx = curTick->askprice(0);
			auto ids = _ctx->sell(code, targetPx, 1);

			StdLocker<StdRecurMutex> lock(_mtx_ords);
			for (auto localid : ids)
			{
				_orders[localid] = now;
			}
			_last_fire_time = now;
		}
	}
	*/

	curTick->release();
}

void WtHfExeUnit::set_position(const char* stdCode, int32_t newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	int32_t diff = newVol - _target_pos;
	if (diff == 0)
		return;

	_show_hand = false;

	_target_pos = newVol;

	_exec_begin_time = _ctx->getCurTime();

	_fire_span = (uint32_t)round((_exec_secs - _exec_tail)*1000.0 / abs(diff));

	doCalculate();
}

//**********************************************************************************************************************************
double WtHfExeUnit::on_tick_bias(WTSTickData* curTick)
{
	//每隔30s 更新一次tickbias值
	uint64_t now = (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime();
	if (now - _last_tickbias_cal_time <= 30 * 1000)
	{
		if (curTick->price() == curTick->bidprice(0)) // 内盘
		{
			_total_s = _total_s + -1 * curTick->volumn();
		}
		if (curTick->price() == curTick->askprice(0)) // 外盘
		{
			_total_s = _total_s + 1 * curTick->volumn();
		}
		if (_total_s > 0)
		{

			_alpha_tickbias = sqrt(_total_s);
		}
		if (_total_s < 0)
		{

			_alpha_tickbias = -sqrt(-_total_s);
		}

		return _old_alpha_tickbias;
	}

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_tickbias = 0.1 * _alpha_tickbias / curTick->volumn();
	}
	else
	{
		_alpha_tickbias = 0;
	}
	_total_s = 0;
	_last_tickbias_cal_time = now;
	_old_alpha_tickbias = _alpha_tickbias;

	return _alpha_tickbias;
}

double WtHfExeUnit::on_trade_bias(WTSTickData* curTick)
{
	double bidsz_tick = 0.0;
	double asksz_tick = 0.0;
	double _alpha_tradebias = 0.0;

	bidsz_tick = curTick->volumn() * (curTick->askprice(0) - curTick->price()) / (curTick->askprice(0) - curTick->bidprice(0));
	asksz_tick = curTick->volumn() * (curTick->price() - curTick->bidprice(0)) / (curTick->askprice(0) - curTick->bidprice(0));
	_alpha_tradebias = asksz_tick - bidsz_tick;


	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_tradebias = 0.1 * _alpha_tradebias / curTick->volumn();
	}
	else
	{
		_alpha_tradebias = 0;
	}

	return _alpha_tradebias;
}


double WtHfExeUnit::on_size_bias(WTSTickData* curTick)
{
	double _alpha_sizebias = 0.0;

	if (curTick->askqty(0) != 0)
	{
		_alpha_sizebias = log(curTick->bidqty(0) / curTick->askqty(0));
	}
	else
	{
		_alpha_sizebias = 0.0;
	}

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_sizebias = 0.2 * _alpha_sizebias / curTick->volumn();
	}
	else
	{
		_alpha_sizebias = 0;
	}

	return _alpha_sizebias;
}


double WtHfExeUnit::on_book_bias(WTSTickData* curTick)
{
	double _alpha_bookbias = 0.0;
	double  ask_level = 0.0;
	double  bid_level = 0.0;


	ask_level = (curTick->askprice(0) - curTick->price()) / 2;
	bid_level = (curTick->bidprice(0) - curTick->price()) / 2;

	_alpha_bookbias = ask_level + bid_level;

	if (curTick->volumn() != 0) // weights adjust
	{
		_alpha_bookbias = 0.5 * _alpha_bookbias;
	}
	else
	{
		_alpha_bookbias = 0;
	}

	return _alpha_bookbias;
}


double WtHfExeUnit::on_tickalpha_sum(WTSTickData* curTick)
{
	double _alpha_Sum = 0.0;
	double alpha_1 = 0.0;
	double alpha_2 = 0.0;
	double alpha_3 = 0.0;
	double alpha_4 = 0.0;

	alpha_1 = on_book_bias(curTick);
	alpha_2 = on_size_bias(curTick);
	alpha_3 = on_trade_bias(curTick);
	alpha_4 = on_tick_bias(curTick);

	_alpha_Sum = alpha_1 + alpha_2 + alpha_3 + alpha_4;

	if (abs(_alpha_Sum) >= 10)
	{
		if (_alpha_Sum > 0)
		{
			return 2; // 追价买，alpha极强时
		}
		if (_alpha_Sum < 0)
		{
			return -2; // 追价卖，alpha极强时
		}
	}
	else if (abs(_alpha_Sum) >= 5 && abs(_alpha_Sum) < 10)
	{
		if (_alpha_Sum > 0)
		{
			return 1; // 对手价挂单买，alpha较强时
		}
		if (_alpha_Sum < 0)
		{
			return -1; // 对手价挂单卖，alpha较强时
		}
	}
	else if (abs(_alpha_Sum) < 5)
	{
		if (_alpha_Sum > 0)
		{
			return 0; // 最优价挂单买，alpha较弱方向不明时
		}
		if (_alpha_Sum < 0)
		{
			return 0; // 最优价挂单卖，alpha较弱方向不明时
		}
	}

	return 0;
}
//**************************************************************************************
