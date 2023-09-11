#include "ExpHftContext.h"
#include "WtRtRunner.h"
#include "../Share/StrUtil.hpp"

extern WtRtRunner& getRunner();

void ExpHftContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	thread_local static char realPeriod[8] = { 0 };
	fmtutil::format_to(realPeriod, "{}{}", period, times);

	getRunner().ctx_on_bar(_context_id, code, realPeriod, newBar, ET_HFT);

	HftStraBaseCtx::on_bar(code, period, times, newBar);
}

void ExpHftContext::on_channel_lost()
{
	getRunner().hft_on_channel_lost(_context_id, _trader->id());

	HftStraBaseCtx::on_channel_lost();
}

void ExpHftContext::on_channel_ready()
{
	getRunner().hft_on_channel_ready(_context_id, _trader->id());

	HftStraBaseCtx::on_channel_ready();
}

void ExpHftContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	getRunner().hft_on_entrust(_context_id, localid, stdCode, bSuccess, message, getOrderTag(localid));

	HftStraBaseCtx::on_entrust(localid, stdCode, bSuccess, message);
}

void ExpHftContext::on_init()
{
	HftStraBaseCtx::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, ET_HFT);
}

void ExpHftContext::on_session_begin(uint32_t uTDate)
{
	HftStraBaseCtx::on_session_begin(uTDate);

	//向外部回调
	getRunner().ctx_on_session_event(_context_id, uTDate, true, ET_HFT);
}

void ExpHftContext::on_session_end(uint32_t uTDate)
{
	//向外部回调
	getRunner().ctx_on_session_event(_context_id, uTDate, false, ET_HFT);

	HftStraBaseCtx::on_session_end(uTDate);
}

void ExpHftContext::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled)
{
	getRunner().hft_on_order(_context_id, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled, getOrderTag(localid));

	HftStraBaseCtx::on_order(localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void ExpHftContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	getRunner().hft_on_position(_context_id, stdCode, isLong, prevol, preavail, newvol, newavail);
}

void ExpHftContext::on_tick(const char* code, WTSTickData* newTick)
{
	update_dyn_profit(code, newTick);

	auto it = _tick_subs.find(code);
	if (it != _tick_subs.end())
	{
		getRunner().ctx_on_tick(_context_id, code, newTick, ET_HFT);
	}

	HftStraBaseCtx::on_tick(code, newTick);
}

void ExpHftContext::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	getRunner().hft_on_order_queue(_context_id, stdCode, newOrdQue);
}

void ExpHftContext::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	getRunner().hft_on_order_detail(_context_id, stdCode, newOrdDtl);
}

void ExpHftContext::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	getRunner().hft_on_transaction(_context_id, stdCode, newTrans);
}

void ExpHftContext::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	getRunner().hft_on_trade(_context_id, localid, stdCode, isBuy, vol, price, getOrderTag(localid));

	HftStraBaseCtx::on_trade(localid, stdCode, isBuy, vol, price);
}