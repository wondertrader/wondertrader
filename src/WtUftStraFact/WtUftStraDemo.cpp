#include "WtUftStraDemo.h"
#include "../Includes/IUftStraCtx.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Share/fmtlib.h"

extern const char* FACT_NAME;

WtUftStraDemo::WtUftStraDemo(const char* id)
	: UftStrategy(id)
	, _last_tick(NULL)
	, _last_entry_time(UINT64_MAX)
	, _channel_ready(false)
	, _last_calc_time(0)
	, _lots(1)
	, _cancel_cnt(0)
{
}


WtUftStraDemo::~WtUftStraDemo()
{
	if (_last_tick)
		_last_tick->release();
}

const char* WtUftStraDemo::getName()
{
	return "UftDemoStrategy";
}

const char* WtUftStraDemo::getFactName()
{
	return FACT_NAME;
}

bool WtUftStraDemo::init(WTSVariant* cfg)
{
	//这里演示一下外部传入参数的获取
	_code = cfg->getCString("code");
	_secs = cfg->getUInt32("second");
	_freq = cfg->getUInt32("freq");
	_offset = cfg->getUInt32("offset");

	_lots = cfg->getDouble("lots");

	return true;
}

void WtUftStraDemo::on_entrust(uint32_t localid, bool bSuccess, const char* message)
{
	if(!bSuccess)
	{
		auto it = _orders.find(localid);
		if(it != _orders.end())
			_orders.erase(it);
	}
}

void WtUftStraDemo::on_init(IUftStraCtx* ctx)
{
	//WTSTickSlice* ticks = ctx->stra_get_ticks(_code.c_str(), _count);
	//if (ticks)
	//	ticks->release();

	WTSKlineSlice* kline = ctx->stra_get_bars(_code.c_str(), "m1", 30);
	if (kline)
		kline->release();

	ctx->stra_sub_ticks(_code.c_str());

	_ctx = ctx;
}

void WtUftStraDemo::on_tick(IUftStraCtx* ctx, const char* code, WTSTickData* newTick)
{	
	if (_code.compare(code) != 0)
		return;

	if (!_orders.empty())
	{
		check_orders();
		return;
	}

	if (!_channel_ready)
		return;

	WTSTickData* curTick = ctx->stra_get_last_tick(code);
	if (curTick)
		curTick->release();

	uint32_t curMin = newTick->actiontime() / 100000;	//actiontime是带毫秒的,要取得分钟,则需要除以10w
	if (curMin > _last_calc_time)
	{//如果spread上次计算的时候小于当前分钟,则重算spread
		//WTSKlineSlice* kline = ctx->stra_get_bars(code, "m5", 30);
		//if (kline)
		//	kline->release();

		//重算晚了以后,更新计算时间
		_last_calc_time = curMin;
	}

	//30秒内不重复计算
	uint64_t now = TimeUtils::makeTime(ctx->stra_get_date(), ctx->stra_get_time() * 100000 + ctx->stra_get_secs());
	if(now - _last_entry_time <= _freq * 1000)
	{
		return;
	}

	int32_t signal = 0;
	double price = newTick->price();
	//计算部分
	double pxInThry = (newTick->bidprice(0)*newTick->askqty(0) + newTick->askprice(0)*newTick->bidqty(0)) / (newTick->bidqty(0) + newTick->askqty(0));

	//理论价格大于最新价
	if (pxInThry > price)
	{
		//正向信号
		signal = 1;
	}
	else if (pxInThry < price)
	{
		//反向信号
		signal = -1;
	}

	if (signal != 0)
	{
		double curPos = ctx->stra_get_position(code);

		WTSCommodityInfo* cInfo = ctx->stra_get_comminfo(code);

		if(signal > 0  && decimal::le(curPos, 0))
		{//正向信号,且当前仓位小于等于0
			//最新价+2跳下单
			double targetPx = price + cInfo->getPriceTick() * _offset;

			uint32_t localid;
			if(decimal::lt(curPos, 0))
				localid = ctx->stra_exit_short(code, targetPx, _lots, true, UFT_OrderFlag_FAK);
			else
				localid = ctx->stra_enter_long(code, targetPx, _lots, UFT_OrderFlag_FAK);

			if(localid != 0)
			{
				_mtx_ords.lock();
				_orders.insert(localid);
				_mtx_ords.unlock();
				_last_entry_time = now;
			}
			
		}
		else if (signal < 0 && decimal::ge(curPos, 0))
		{//反向信号,且当前仓位大于0,或者仓位为0但不是股票,或者仓位为0但是基础仓位有修正
			//最新价-2跳下单
			double targetPx = price - cInfo->getPriceTick()*_offset;

			uint32_t localid;
			if (decimal::gt(curPos, 0))
				localid = ctx->stra_exit_long(code, targetPx, _lots, true, UFT_OrderFlag_FAK);
			else
				localid = ctx->stra_enter_short(code, targetPx, _lots, UFT_OrderFlag_FAK);

			if (localid != 0)
			{
				_mtx_ords.lock();
				_orders.insert(localid);
				_mtx_ords.unlock();
				_last_entry_time = now;
			}
		}
	}
}

void WtUftStraDemo::check_orders()
{
	if (!_orders.empty() && _last_entry_time != UINT64_MAX)
	{
		uint64_t now = TimeUtils::makeTime(_ctx->stra_get_date(), _ctx->stra_get_time() * 100000 + _ctx->stra_get_secs());
		if (now - _last_entry_time >= _secs * 1000)	//如果超过一定时间没有成交完,则撤销
		{
			_mtx_ords.lock();
			for (auto localid : _orders)
			{
				_ctx->stra_cancel(localid);
				_cancel_cnt++;
				_ctx->stra_log_info(fmt::format("Order expired, cancelcnt updated to {}", _cancel_cnt).c_str());
			}
			_mtx_ords.unlock();
		}
	}
}

void WtUftStraDemo::on_bar(IUftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	
}

void WtUftStraDemo::on_trade(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double qty, double price)
{
	
}

void WtUftStraDemo::on_position(IUftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	
}

void WtUftStraDemo::on_order(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled)
{
	//如果不是我发出去的订单,我就不管了
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	//如果已撤销或者剩余数量为0,则清除掉原有的id记录
	if(isCanceled || leftQty == 0)
	{
		_mtx_ords.lock();
		_orders.erase(it);
		if (_cancel_cnt > 0)
		{
			_cancel_cnt--;
			_ctx->stra_log_info(fmt::format("cancelcnt -> {}", _cancel_cnt).c_str());
		}
		_mtx_ords.unlock();
	}
}


void WtUftStraDemo::on_channel_ready(IUftStraCtx* ctx)
{
	double undone = _ctx->stra_get_undone(_code.c_str());
	if (!decimal::eq(undone, 0) && _orders.empty())
	{
		//这说明有未完成单不在监控之中,先撤掉
		_ctx->stra_log_info(fmt::format("{}有不在管理中的未完成单 {} 手,全部撤销", _code, undone).c_str());

		OrderIDs ids = _ctx->stra_cancel_all(_code.c_str());
		for (auto localid : ids)
		{
			_orders.insert(localid);
		}
		_cancel_cnt += ids.size();

		_ctx->stra_log_info(fmt::format("cancelcnt -> {}", _cancel_cnt).c_str());
	}

	_channel_ready = true;
}

void WtUftStraDemo::on_channel_lost(IUftStraCtx* ctx)
{
	_channel_ready = false;
}