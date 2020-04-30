#include "WtTWapExeUnit.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"

#include <math.h>


extern const char* FACT_NAME;


WtTWapExeUnit::WtTWapExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _sticky(0)
	, _cancel_cnt(0)
	, _channel_ready(false)
	, _last_fire_time(0)
	, _exec_begin_time(UINT64_MAX)
	, _target_pos(0)
	, _exeSecondTime(60)
	, _twap_times(10)
{
}


WtTWapExeUnit::~WtTWapExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtTWapExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtTWapExeUnit::getName()
{
	return "WtTWapExeUnit";
}

void WtTWapExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_sticky = cfg->getUInt32("sticky");
	_exec_secs = cfg->getUInt32("execsecs");
	_exec_tail = cfg->getUInt32("tailsecs");

	ctx->writeLog("执行单元 WtTWapExeUnit[%s] 初始化完成，订单超时 %u 秒，执行时限 %u 秒，收尾时间 %u 秒", stdCode, _sticky, _exec_secs, _exec_tail);
}

void WtTWapExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
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

void WtTWapExeUnit::on_channel_ready()
{
	_channel_ready = true;
	double undone = _ctx->getUndoneQty(_code.c_str());
	if (undone != 0 && _orders.empty())
	{
		//这说明有未完成单不在监控之中，先撤掉
		_ctx->writeLog("%s有不在管理中的未完成单 %.0f 手，全部撤销", _code.c_str(), undone);

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

void WtTWapExeUnit::on_channel_lost()
{

}

void WtTWapExeUnit::on_tick(WTSTickData* newTick)
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
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();
		uint64_t    _fire_span = _exeSecondTime * 1000 / _twap_times;

		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);
		uint64_t total_ExeSecond = _exeSecondTime;
		uint64_t now = _ctx->getCurTime();
		if (newVol != undone + realPos) // 仓位变化要交易
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
				if (now - firetime > _sticky * 1000) //要撤单重挂
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

void WtTWapExeUnit::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	//不用触发，这里在ontick里触发吧
	//_ctx->writeLog("%s合约%s%u手，重新触发执行逻辑", stdCode, isBuy?"买入":"卖出", vol);
	//doCalculate();
}

void WtTWapExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtTWapExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;
	double newVol = 0;
	if (abs(_target_pos) >= _twap_times) {
		double newVol = _target_pos / _twap_times;
	}
	if (abs(_target_pos) <_twap_times  && _target_pos != 0)
	{
		newVol = 1;
	}
	if (_target_pos ==0 )
	{
		newVol = 0;
	} //figure out min order newVol to place 
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);

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
	if (_exec_begin_time != UINT64_MAX && ((now - _exec_begin_time) >= _exeSecondTime * 1000)) //超过60s 执行时间
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
	double curPos = realPos + undone;
	if (curPos == _target_pos) //下单到目标仓位
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

	if(bNeedShowHand) //  last showhand time
	{
		double qty = _target_pos - realPos; // showhand qty,  place  in one time
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
	// ------------- TWAP place order--------------------------------------
	double diffQty = _target_pos - curPos;
	const char* code = _code.c_str();
	if (diffQty > 0) {
		//正向信号，且当前仓位小于等于0
		 //最新价+2跳下单
		double targetPx = curTick->bidprice(0);
		auto ids = _ctx->buy(code, targetPx, abs(newVol)); //  twap place order

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;

	}
	if (diffQty < 0) {
		//反向信号，净仓位为负，做空
		//最新价+2跳下单
		double targetPx = curTick->askprice(0);
		auto ids = _ctx->sell(code, targetPx, abs(newVol)); //  twap place order

		StdLocker<StdRecurMutex> lock(_mtx_ords);
		for (auto localid : ids)
		{
			_orders[localid] = now;
		}
		_last_fire_time = now;

	}

	curTick->release();
}

void WtTWapExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	double diff = newVol - _target_pos;
	if (diff == 0)
		return;

	_show_hand = false;

	_target_pos = newVol;

	_exec_begin_time = _ctx->getCurTime();

	_fire_span = (uint32_t)round((_exec_secs - _exec_tail)*1000.0 / abs(diff));

	doCalculate();
}
