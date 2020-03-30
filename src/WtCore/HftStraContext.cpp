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
#include "HftStrategyDefs.h"


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
	HftStraBaseCtx::on_init();

	if (_strategy)
		_strategy->on_init(this);
}

void HftStraContext::on_tick(const char* code, WTSTickData* newTick)
{
	HftStraBaseCtx::on_tick(code, newTick);

	if (_strategy)
		_strategy->on_tick(this, code, newTick);
}

void HftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	HftStraBaseCtx::on_bar(code, period, times, newBar);

	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);
}

void HftStraContext::on_schedule(uint32_t uDate, uint32_t uTime)
{
	HftStraBaseCtx::on_schedule(uDate, uTime);

	if (_strategy)
		_strategy->on_schedule(this, uDate, uTime);
}

void HftStraContext::on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price)
{
	HftStraBaseCtx::on_trade(stdCode, isBuy, vol, price);

	if (_strategy)
		_strategy->on_trade(this, get_inner_code(stdCode), isBuy, vol, price);
}

void HftStraContext::on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled /* = false */)
{
	HftStraBaseCtx::on_order(localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled);

	if (_strategy)
		_strategy->on_order(this, localid, get_inner_code(stdCode), isBuy, totalQty, leftQty, price, isCanceled);
}

void HftStraContext::on_position(const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail)
{
	HftStraBaseCtx::on_position(stdCode, isLong, prevol, preavail, newvol, newavail);

	if (_strategy)
		_strategy->on_position(this, get_inner_code(stdCode), isLong, prevol, preavail, newvol, newavail);
}

void HftStraContext::on_channel_ready()
{
	HftStraBaseCtx::on_channel_ready();

	if (_strategy)
		_strategy->on_channel_ready(this);
}

void HftStraContext::on_channel_lost()
{
	HftStraBaseCtx::on_channel_lost();

	if (_strategy)
		_strategy->on_channel_lost(this);
}


void HftStraContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	HftStraBaseCtx::on_entrust(localid, stdCode, bSuccess, message);

	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);
}