/*!
 * \file HftStraContext.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "HftStraContext.h"
#include "../Includes/HftStrategyDefs.h"


HftStraContext::HftStraContext(WtHftEngine* engine, const char* name, bool bAgent /* = true */)
	: HftStraBaseCtx(engine, name, bAgent)
	, _strategy(NULL)
{
}


HftStraContext::~HftStraContext()
{
}

void HftStraContext::on_init()
{
	HftStraBaseCtx::on_init();

	if (_strategy)
		_strategy->on_init(this);
}

void HftStraContext::on_session_begin(uint32_t uTDate)
{
	HftStraBaseCtx::on_session_begin(uTDate);

	if (_strategy)
		_strategy->on_session_begin(this, uTDate);
}

void HftStraContext::on_session_end(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, uTDate);

	HftStraBaseCtx::on_session_end(uTDate);
}

void HftStraContext::on_tick(const char* stdCode, WTSTickData* newTick)
{
	update_dyn_profit(stdCode, newTick);

	auto it = _tick_subs.find(stdCode);
	if (it != _tick_subs.end())
	{
		if (_strategy)
			_strategy->on_tick(this, stdCode, newTick);
	}

	HftStraBaseCtx::on_tick(stdCode, newTick);
}

void HftStraContext::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);

	HftStraBaseCtx::on_order_queue(stdCode, newOrdQue);
}

void HftStraContext::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);

	HftStraBaseCtx::on_order_detail(stdCode, newOrdDtl);
}

void HftStraContext::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);

	HftStraBaseCtx::on_transaction(stdCode, newTrans);
}

void HftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);

	HftStraBaseCtx::on_bar(code, period, times, newBar);
}

void HftStraContext::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	const char* innerCode = get_inner_code(stdCode);
	if (_strategy)
		_strategy->on_trade(this, localid, innerCode, isBuy, vol, price, getOrderTag(localid));

	HftStraBaseCtx::on_trade(localid, innerCode, isBuy, vol, price);
}

void HftStraContext::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	const char* innerCode = get_inner_code(stdCode);
	if (_strategy)
		_strategy->on_order(this, localid, innerCode, isBuy, totalQty, leftQty, price, isCanceled, getOrderTag(localid));

	HftStraBaseCtx::on_order(localid, innerCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void HftStraContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	if (_strategy)
		_strategy->on_position(this, stdCode, isLong, prevol, preavail, newvol, newavail);
}

void HftStraContext::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);

	HftStraBaseCtx::on_channel_ready();
}

void HftStraContext::on_channel_lost()
{
	if (_strategy)
		_strategy->on_channel_lost(this);

	HftStraBaseCtx::on_channel_lost();
}

void HftStraContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message, getOrderTag(localid));

	HftStraBaseCtx::on_entrust(localid, get_inner_code(stdCode), bSuccess, message);
}