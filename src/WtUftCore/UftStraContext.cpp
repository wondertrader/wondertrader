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
#include "WtUftEngine.h"
#include "TraderAdapter.h"
#include "WtHelper.h"
#include "ShareManager.h"

#include "../Includes/UftStrategyDefs.h"
#include "../Includes/WTSContractInfo.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

inline uint32_t makeUftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

UftStraContext::UftStraContext(WtUftEngine* engine, const char* name, bool bDependent /* = false */)
	: IUftStraCtx(name)
	, _engine(engine)
	, _dependent(bDependent)
	, _strategy(NULL)
{
}


UftStraContext::~UftStraContext()
{
}

void UftStraContext::setTrader(TraderAdapter* trader)
{
	_trader = trader;
}

void UftStraContext::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}


void UftStraContext::on_tick(const char* stdCode, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);

}

void UftStraContext::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);
}

void UftStraContext::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);
}

void UftStraContext::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);
}

void UftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);
}

void UftStraContext::on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price)
{
	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isLong, offset, vol, price);
}

void UftStraContext::on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	if (_strategy)
		_strategy->on_order(this, localid, stdCode, isLong, offset, totalQty, leftQty, price, isCanceled);
}

void UftStraContext::on_channel_ready()
{
	if (_strategy)
		_strategy->on_channel_ready(this);
}

void UftStraContext::on_channel_lost()
{
	if (_strategy)
		_strategy->on_channel_lost(this);
}

void UftStraContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);
}

void UftStraContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	if (_strategy)
		_strategy->on_position(this, stdCode, isLong, prevol, preavail, newvol, newavail);
}

void UftStraContext::on_session_begin(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_begin(this, uTDate);
}

void UftStraContext::on_session_end(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, uTDate);
}

void UftStraContext::on_params_updated()
{
	if (_strategy)
		_strategy->on_params_updated();
}

void UftStraContext::watch_param(const char* name, const char* val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::watch_param(const char* name, int64_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::watch_param(const char* name, int32_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::watch_param(const char* name, uint64_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::watch_param(const char* name, uint32_t val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::watch_param(const char* name, double val)
{
	ShareManager::self().set_value(_name.c_str(), name, val);
}

void UftStraContext::commit_param_watcher()
{
	ShareManager::self().commit_param_watcher(_name.c_str());
}

const char* UftStraContext::read_param(const char* name, const char* defVal /* = "" */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int32_t UftStraContext::read_param(const char* name, int32_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

uint32_t UftStraContext::read_param(const char* name, uint32_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int64_t UftStraContext::read_param(const char* name, int64_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

uint64_t UftStraContext::read_param(const char* name, uint64_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

double UftStraContext::read_param(const char* name, double defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int32_t* UftStraContext::sync_param(const char* name, int32_t initVal /* = 0 */)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal);
}

uint32_t* UftStraContext::sync_param(const char* name, uint32_t initVal /* = 0 */)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal);
}

int64_t* UftStraContext::sync_param(const char* name, int64_t initVal /* = 0 */)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal);
}

uint64_t* UftStraContext::sync_param(const char* name, uint64_t initVal /* = 0 */)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal);
}

double* UftStraContext::sync_param(const char* name, double initVal /* = 0 */)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal);
}

double UftStraContext::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, int32_t iFlag /* = 0 */)
{
	return _trader->getPosition(stdCode, bOnlyValid, iFlag);
}

double UftStraContext::stra_enum_position(const char* stdCode)
{
	return _trader->enumPosition(stdCode);
}

double UftStraContext::stra_get_undone(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

uint32_t UftStraContext::stra_get_infos(const char* stdCode)
{
	return _trader->getInfos(stdCode);
}

double UftStraContext::stra_get_price(const char* stdCode)
{
	return _engine->get_cur_price(stdCode);
}

uint32_t UftStraContext::stra_get_date()
{
	return _engine->get_date();
}

uint32_t UftStraContext::stra_get_time()
{
	return _engine->get_raw_time();
}

uint32_t UftStraContext::stra_get_secs()
{
	return _engine->get_secs();
}

bool UftStraContext::stra_cancel(uint32_t localid)
{
	return _trader->cancel(localid);
}

OrderIDs UftStraContext::stra_cancel_all(const char* stdCode)
{
	//³·µ¥ÆµÂÊ¼ì²é
	//if (!_trader->checkCancelLimits(stdCode))
	//	return OrderIDs();

	return _trader->cancelAll(stdCode);
}

OrderIDs UftStraContext::stra_buy(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->buy(stdCode, price, qty, flag, false);
}

OrderIDs UftStraContext::stra_sell(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->sell(stdCode, price, qty, flag, false);
}

uint32_t UftStraContext::stra_enter_long(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->openLong(stdCode, price, qty, flag);
}

uint32_t UftStraContext::stra_exit_long(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	return _trader->closeLong(stdCode, price, qty, isToday, flag);
}

uint32_t UftStraContext::stra_enter_short(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	return _trader->openShort(stdCode, price, qty, flag);
}

uint32_t UftStraContext::stra_exit_short(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	return _trader->closeShort(stdCode, price, qty, isToday, flag);
}

WTSCommodityInfo* UftStraContext::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

WTSKlineSlice* UftStraContext::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
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

WTSTickSlice* UftStraContext::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ticks = _engine->get_tick_slice(_context_id, stdCode, count);

	if (ticks)
		_engine->sub_tick(id(), stdCode);
	return ticks;
}

WTSOrdDtlSlice* UftStraContext::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	WTSOrdDtlSlice* ret = _engine->get_order_detail_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_detail(id(), stdCode);
	return ret;
}

WTSOrdQueSlice* UftStraContext::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	WTSOrdQueSlice* ret = _engine->get_order_queue_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_queue(id(), stdCode);
	return ret;
}


WTSTransSlice* UftStraContext::stra_get_transaction(const char* stdCode, uint32_t count)
{
	WTSTransSlice* ret = _engine->get_transaction_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_transaction(id(), stdCode);
	return ret;
}


WTSTickData* UftStraContext::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void UftStraContext::stra_sub_ticks(const char* stdCode)
{
	_engine->sub_tick(id(), stdCode);
	log_info("Market Data subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_order_details(const char* stdCode)
{
	_engine->sub_order_detail(id(), stdCode);
	log_info("Order details subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_order_queues(const char* stdCode)
{
	_engine->sub_order_queue(id(), stdCode);
	log_info("Order queues subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_transactions(const char* stdCode)
{
	_engine->sub_transaction(id(), stdCode);
	log_info("Transactions subscribed: {}", stdCode);
}

void UftStraContext::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void UftStraContext::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void UftStraContext::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}