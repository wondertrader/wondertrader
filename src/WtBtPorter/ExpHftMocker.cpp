#include "ExpHftMocker.h"
#include "WtBtRunner.h"

#include "../Share/StrUtil.hpp"

extern WtBtRunner& getRunner();

ExpHftMocker::ExpHftMocker(HisDataReplayer* replayer, const char* name)
	: HftMocker(replayer, name)
{

}

void ExpHftMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	std::string realPeriod;
	if (period[0] == 'd')
		realPeriod = StrUtil::printf("%s%u", period, times);
	else
		realPeriod = StrUtil::printf("m%u", times);

	HftMocker::on_bar(stdCode, period, times, newBar);

	getRunner().ctx_on_bar(_context_id, stdCode, realPeriod.c_str(), newBar, ET_HFT);
}

void ExpHftMocker::on_channel_ready()
{
	HftMocker::on_channel_ready();

	getRunner().hft_on_channel_ready(_context_id, "");
}

void ExpHftMocker::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag)
{
	HftMocker::on_entrust(localid, stdCode, bSuccess, message, userTag);

	getRunner().hft_on_entrust(_context_id, localid, stdCode, bSuccess, message, userTag);
}

void ExpHftMocker::on_init()
{
	HftMocker::on_init();

	getRunner().ctx_on_init(_context_id, ET_HFT);

	getRunner().on_initialize_event();
}

void ExpHftMocker::on_session_begin(uint32_t uDate)
{
	HftMocker::on_session_begin(uDate);

	getRunner().ctx_on_session_event(_context_id, uDate, true, ET_HFT);
	getRunner().on_session_event(uDate, true);
}

void ExpHftMocker::on_session_end(uint32_t uDate)
{
	HftMocker::on_session_end(uDate);

	getRunner().ctx_on_session_event(_context_id, uDate, false, ET_HFT);
	getRunner().on_session_event(uDate, false);
}

void ExpHftMocker::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag)
{
	HftMocker::on_order(localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled, userTag);

	getRunner().hft_on_order(_context_id, localid, stdCode, isBuy, totalQty, leftQty, price, isCanceled, userTag);
}

void ExpHftMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_HFT);
}

void ExpHftMocker::on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	getRunner().hft_on_order_queue(_context_id, stdCode, newOrdQue);
}

void ExpHftMocker::on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	getRunner().hft_on_order_detail(_context_id, stdCode, newOrdDtl);
}

void ExpHftMocker::on_trans_updated(const char* stdCode, WTSTransData* newTrans)
{
	getRunner().hft_on_transaction(_context_id, stdCode, newTrans);
}

void ExpHftMocker::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag)
{
	HftMocker::on_trade(localid, stdCode, isBuy, vol, price, userTag);

	getRunner().hft_on_trade(_context_id, localid, stdCode, isBuy, vol, price, userTag);
}

void ExpHftMocker::on_bactest_end()
{
	getRunner().on_backtest_end();
}