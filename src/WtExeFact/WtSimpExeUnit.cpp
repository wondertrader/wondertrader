/*!
 * \file WtSimpExeUnit.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtSimpExeUnit.h"

#include "../Share/WTSVariant.hpp"
#include "../Share/WTSContractInfo.hpp"
#include "../Share/decimal.h"

extern const char* FACT_NAME;


WtSimpExeUnit::WtSimpExeUnit()
	: _last_tick(NULL)
	, _comm_info(NULL)
	, _price_mode(0)
	, _price_offset(0)
	, _expire_secs(0)
	, _cancel_cnt(0)
	, _target_pos(0)
{
}


WtSimpExeUnit::~WtSimpExeUnit()
{
	if (_last_tick)
		_last_tick->release();

	if (_comm_info)
		_comm_info->release();
}

const char* WtSimpExeUnit::getFactName()
{
	return FACT_NAME;
}

const char* WtSimpExeUnit::getName()
{
	return "WtSimpExeUnit";
}

const char* PriceModeNames[] = 
{
	"最优价",
	"最新价",
	"对手价"
};
void WtSimpExeUnit::init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg)
{
	ExecuteUnit::init(ctx, stdCode, cfg);

	_comm_info = ctx->getCommodityInfo(stdCode);
	if (_comm_info)
		_comm_info->retain();

	_price_offset = cfg->getInt32("offset");
	_expire_secs = cfg->getUInt32("expire");
	_price_mode = cfg->getBoolean("pricemode");	//价格类型，0-最新价，-1-最优价，1-对手价，默认为0

	ctx->writeLog("执行单元 %s 初始化完成，委托价 %s ± %d 跳，订单超时 %u 秒", stdCode, PriceModeNames[_price_mode+1], _price_offset, _expire_secs);
}

void WtSimpExeUnit::on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled)
{
	{
		std::unique_lock<std::mutex> lck(_mtx_ords);
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return;

		if (isCanceled || leftover == 0)
		{
			_orders.erase(it);
			if (_cancel_cnt > 0)
				_cancel_cnt--;

			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		}
	}

	//如果有撤单，也触发重新计算
	if (isCanceled)
	{
		_ctx->writeLog("%s的订单%u已撤销，重新触发执行逻辑", stdCode, localid);
		doCalculate();
	}
}

void WtSimpExeUnit::on_channel_ready()
{
	double undone = _ctx->getUndoneQty(_code.c_str());
	if(!decimal::eq(undone, 0) && _orders.empty())
	{
		//这说明有未完成单不在监控之中，先撤掉
		_ctx->writeLog("%s有不在管理中的未完成单 %d 手，全部撤销", _code.c_str(), undone);

		bool isBuy = (undone > 0);
		OrderIDs ids = _ctx->cancel(_code.c_str(), isBuy);
		for(auto localid : ids)
		{
			_orders.insert(localid);
		}
		_cancel_cnt += ids.size();

		_ctx->writeLog("%s cancelcnt -> %u", __FUNCTION__, _cancel_cnt);
	}


	doCalculate();
}

void WtSimpExeUnit::on_channel_lost()
{
	
}

void WtSimpExeUnit::on_tick(WTSTickData* newTick)
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

	if(isFirstTick)	//如果是第一笔tick，则检查目标仓位，不符合则下单
	{
		double newVol = _target_pos;
		const char* stdCode = _code.c_str();

		double undone = _ctx->getUndoneQty(stdCode);
		double realPos = _ctx->getPosition(stdCode);

		//if (newVol != undone + realPos)
		if (!decimal::eq(newVol, undone + realPos))
		{
			doCalculate();
		}
	}
	else if(_expire_secs != 0 && !_orders.empty())
	{
		uint64_t now = _ctx->getCurTime();
		if (now - _last_entry_time < _expire_secs * 1000 || _cancel_cnt!=0)
			return;

		_mtx_ords.lock();
		for (auto localid : _orders)
		{
			if(_ctx->cancel(localid))
			{
				_cancel_cnt++;
				_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			}
		}
		_mtx_ords.unlock();
	}
}

void WtSimpExeUnit::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	//不用触发，这里在ontick里触发吧
	//_ctx->writeLog("%s合约%s%u手，重新触发执行逻辑", stdCode, isBuy?"买入":"卖出", vol);
	//doCalculate();
}

void WtSimpExeUnit::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
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

void WtSimpExeUnit::doCalculate()
{
	if (_cancel_cnt != 0)
		return;

	double newVol = _target_pos;
	const char* stdCode = _code.c_str();

	double undone = _ctx->getUndoneQty(stdCode);
	double realPos = _ctx->getPosition(stdCode);

	//如果有反向未完成单，则直接撤销
	//如果目标仓位为0，且当前持仓为0，则撤销全部挂单
	if (decimal::lt(newVol * undone, 0))
	{
		bool isBuy = decimal::gt(undone, 0);
		//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
		OrderIDs ids = _ctx->cancel(stdCode, isBuy);
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_cancel_cnt += ids.size();
		_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
		return;
	}
	//else if(newVol == 0 && undone != 0)
	else if (decimal::eq(newVol,0) && !decimal::eq(undone, 0))
	{
		//如果目标仓位为0，且未完成不为0
		//那么当目前仓位为0，或者 目前仓位和未完成手数方向相同
		//这样也要全部撤销
		//if (realPos == 0 || (realPos * undone > 0))
		if (decimal::eq(realPos, 0) || decimal::gt(realPos * undone, 0))
		{
			bool isBuy = decimal::gt(undone, 0);
			//_cancel_cnt += _ctx->cancel(stdCode, isBuy);
			OrderIDs ids = _ctx->cancel(stdCode, isBuy);
			for (auto localid : ids)
			{
				_orders.insert(localid);
			}
			_cancel_cnt += ids.size();
			_ctx->writeLog("@ %d cancelcnt -> %u", __LINE__, _cancel_cnt);
			return;
		}
	}

	//如果都是同向的，则纳入计算
	double curPos = realPos + undone;
	//if (curPos == newVol)
	if (decimal::eq(curPos, newVol))
		return;

	if(_last_tick == NULL)
	{
		//grabLastTick会自动增加引用计数，不需要再retain
		_last_tick = _ctx->grabLastTick(stdCode);
	}

	if (_last_tick == NULL)
	{
		_ctx->writeLog("%s没有最新tick数据，退出执行逻辑", _code.c_str());
		return;
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
	else //if (_price_mode == 1)
	{
		buyPx = _last_tick->askprice(0) + _comm_info->getPriceTick() * _price_offset;
		sellPx = _last_tick->bidprice(0) - _comm_info->getPriceTick() * _price_offset;
	}

	//if (newVol > curPos)
	if (decimal::gt(newVol, curPos))
	{
		OrderIDs ids = _ctx->buy(stdCode, buyPx, newVol - curPos);

		_mtx_ords.lock();
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_mtx_ords.unlock();
		_last_entry_time = _ctx->getCurTime();
	}
	else
	{
		OrderIDs ids  = _ctx->sell(stdCode, sellPx, curPos - newVol);

		_mtx_ords.lock();
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_mtx_ords.unlock();
		_last_entry_time = _ctx->getCurTime();
	}
}

void WtSimpExeUnit::set_position(const char* stdCode, double newVol)
{
	if (_code.compare(stdCode) != 0)
		return;

	if (_target_pos != newVol)
	{
		_target_pos = newVol;
	}

	doCalculate();
}
