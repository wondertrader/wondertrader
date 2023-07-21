/*!
 * \file UftStraBaseCtx.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UftStraBaseCtx.h"
#include "WtUftEngine.h"
#include "TraderAdapter.h"
#include "WtHelper.h"
#include "ShareManager.h"

#include "../Includes/WTSContractInfo.hpp"

#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_WTP;

inline uint32_t makeUftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

UftStraBaseCtx::UftStraBaseCtx(WtUftEngine* engine, const char* name)
	: IUftStraCtx(name)
	, _engine(engine)
{
	_context_id = makeUftCtxId();
}


UftStraBaseCtx::~UftStraBaseCtx()
{
}

uint32_t UftStraBaseCtx::id()
{
	return _context_id;
}

void UftStraBaseCtx::setTrader(TraderAdapter* trader)
{
	_trader = trader;
}

void UftStraBaseCtx::on_init()
{

}

void UftStraBaseCtx::watch_param(const char* name, int64_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraBaseCtx::watch_param(const char* name, int32_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraBaseCtx::watch_param(const char* name, uint64_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraBaseCtx::watch_param(const char* name, uint32_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraBaseCtx::watch_param(const char* name, double val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraBaseCtx::commit_param_watcher()
{
	ShareManager::self().commit_param_watcher(_name.c_str());
}

void UftStraBaseCtx::on_tick(const char* stdCode, WTSTickData* newTick)
{

}

void UftStraBaseCtx::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{

}

void UftStraBaseCtx::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{

}

void UftStraBaseCtx::on_transaction(const char* stdCode, WTSTransData* newTrans)
{

}

void UftStraBaseCtx::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{

}

bool UftStraBaseCtx::stra_cancel(uint32_t localid)
{
	return _trader->cancel(localid);
}

OrderIDs UftStraBaseCtx::stra_cancel_all(const char* stdCode)
{
	//³·µ¥ÆµÂÊ¼ì²é
	//if (!_trader->checkCancelLimits(stdCode))
	//	return OrderIDs();

	return _trader->cancelAll(stdCode);
}

OrderIDs UftStraBaseCtx::stra_buy(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->buy(stdCode, price, qty, flag, false);
}

OrderIDs UftStraBaseCtx::stra_sell(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->sell(stdCode, price, qty, flag, false);
}

uint32_t UftStraBaseCtx::stra_enter_long(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->openLong(stdCode, price, qty, flag);
}

uint32_t UftStraBaseCtx::stra_exit_long(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	return _trader->closeLong(stdCode, price, qty, isToday, flag);
}

uint32_t UftStraBaseCtx::stra_enter_short(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->openShort(stdCode, price, qty, flag);
}

uint32_t UftStraBaseCtx::stra_exit_short(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	return _trader->closeShort(stdCode, price, qty, isToday, flag);
}

WTSCommodityInfo* UftStraBaseCtx::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

WTSKlineSlice* UftStraBaseCtx::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlineSlice* ret = _engine->get_kline_slice(_context_id, stdCode, basePeriod, count, times);

	if (ret)
		_engine->sub_tick(id(), stdCode);

	return ret;
}

WTSTickSlice* UftStraBaseCtx::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ticks = _engine->get_tick_slice(_context_id, stdCode, count);

	if (ticks)
		_engine->sub_tick(id(), stdCode);
	return ticks;
}

WTSOrdDtlSlice* UftStraBaseCtx::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	WTSOrdDtlSlice* ret = _engine->get_order_detail_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_detail(id(), stdCode);
	return ret;
}

WTSOrdQueSlice* UftStraBaseCtx::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	WTSOrdQueSlice* ret = _engine->get_order_queue_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_queue(id(), stdCode);
	return ret;
}


WTSTransSlice* UftStraBaseCtx::stra_get_transaction(const char* stdCode, uint32_t count)
{
	WTSTransSlice* ret = _engine->get_transaction_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_transaction(id(), stdCode);
	return ret;
}


WTSTickData* UftStraBaseCtx::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void UftStraBaseCtx::stra_sub_ticks(const char* stdCode)
{
	_engine->sub_tick(id(), stdCode);
	log_info("Market Data subscribed: {}", stdCode);
}

void UftStraBaseCtx::stra_sub_order_details(const char* stdCode)
{
	_engine->sub_order_detail(id(), stdCode);
	log_info("Order details subscribed: {}", stdCode);
}

void UftStraBaseCtx::stra_sub_order_queues(const char* stdCode)
{
	_engine->sub_order_queue(id(), stdCode);
	log_info("Order queues subscribed: {}", stdCode);
}

void UftStraBaseCtx::stra_sub_transactions(const char* stdCode)
{
	_engine->sub_transaction(id(), stdCode);
	log_info("Transactions subscribed: {}", stdCode);
}

void UftStraBaseCtx::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void UftStraBaseCtx::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void UftStraBaseCtx::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

void UftStraBaseCtx::on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price)
{

}

void UftStraBaseCtx::on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{

}

void UftStraBaseCtx::on_channel_ready()
{

}

void UftStraBaseCtx::on_channel_lost()
{

}

void UftStraBaseCtx::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{

}

void UftStraBaseCtx::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{

}

double UftStraBaseCtx::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, int32_t iFlag /* = 0 */)
{
	return _trader->getPosition(stdCode, bOnlyValid, iFlag);
}

double UftStraBaseCtx::stra_enum_position(const char* stdCode)
{
	return _trader->enumPosition(stdCode);
}

double UftStraBaseCtx::stra_get_undone(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

double UftStraBaseCtx::stra_get_price(const char* stdCode)
{
	return _engine->get_cur_price(stdCode);
}

uint32_t UftStraBaseCtx::stra_get_date()
{
	return _engine->get_date();
}

uint32_t UftStraBaseCtx::stra_get_time()
{
	return _engine->get_raw_time();
}

uint32_t UftStraBaseCtx::stra_get_secs()
{
	return _engine->get_secs();
}

