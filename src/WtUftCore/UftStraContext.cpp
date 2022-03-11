/*!
 * \file HftStraContext.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UftStraContext.h"
#include "../Includes/UftStrategyDefs.h"


UftStraContext::UftStraContext(WtUftEngine* engine, const char* name)
	: UftStraBaseCtx(engine, name)
	, _strategy(NULL)
{
}


UftStraContext::~UftStraContext()
{
}

void UftStraContext::on_init()
{
	UftStraBaseCtx::on_init();

	if (_strategy)
		_strategy->on_init(this);
}


void UftStraContext::on_tick(const char* stdCode, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);

	UftStraBaseCtx::on_tick(stdCode, newTick);
}

void UftStraContext::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);

	UftStraBaseCtx::on_order_queue(stdCode, newOrdQue);
}

void UftStraContext::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);

	UftStraBaseCtx::on_order_detail(stdCode, newOrdDtl);
}

void UftStraContext::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);

	UftStraBaseCtx::on_transaction(stdCode, newTrans);
}

void UftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);

	UftStraBaseCtx::on_bar(code, period, times, newBar);
}

void UftStraContext::on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price)
{
	//const char* innerCode = get_inner_code(stdCode);
	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isLong, offset, vol, price);

	UftStraBaseCtx::on_trade(localid, stdCode, isLong, offset, vol, price);
}

void UftStraContext::on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	//const char* innerCode = get_inner_code(stdCode);
	if (_strategy)
		_strategy->on_order(this, localid, stdCode, isLong, offset, totalQty, leftQty, price, isCanceled);

	UftStraBaseCtx::on_order(localid, stdCode, isLong, offset, totalQty, leftQty, price, isCanceled);
}

void UftStraContext::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);

	UftStraBaseCtx::on_channel_ready();
}

void UftStraContext::on_channel_lost()
{
	if (_strategy)
		_strategy->on_channel_lost(this);

	UftStraBaseCtx::on_channel_lost();
}

void UftStraContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);

	UftStraBaseCtx::on_entrust(localid, stdCode, bSuccess, message);
}