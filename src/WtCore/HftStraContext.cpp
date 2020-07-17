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


HftStraContext::HftStraContext(WtHftEngine* engine, const char* name)
	: HftStraBaseCtx(engine, name)
	, _strategy(NULL)
{
}


HftStraContext::~HftStraContext()
{
}

void HftStraContext::on_init()
{
	if (_strategy)
		_strategy->on_init(this);

	HftStraBaseCtx::on_init();
}

void HftStraContext::on_tick(const char* code, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, code, newTick);

	HftStraBaseCtx::on_tick(code, newTick);
}

void HftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);

	HftStraBaseCtx::on_bar(code, period, times, newBar);
}

void HftStraContext::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	if (_strategy)
		_strategy->on_trade(this, get_inner_code(stdCode), isBuy, vol, price);

	HftStraBaseCtx::on_trade(stdCode, isBuy, vol, price);
}

void HftStraContext::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	if (_strategy)
		_strategy->on_order(this, localid, get_inner_code(stdCode), isBuy, totalQty, leftQty, price, isCanceled);

	HftStraBaseCtx::on_order(localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);
}

void HftStraContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	if (_strategy)
		_strategy->on_position(this, get_inner_code(stdCode), isLong, prevol, preavail, newvol, newavail);

	HftStraBaseCtx::on_position(stdCode, isLong, prevol, preavail, newvol, newavail);
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
		_strategy->on_entrust(localid, bSuccess, message);

	HftStraBaseCtx::on_entrust(localid, stdCode, bSuccess, message);
}