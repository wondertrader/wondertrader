#pragma once
#include <unordered_map>

#include "IHftStraCtx.h"
#include "ITrdNotifySink.h"

NS_OTP_BEGIN

class WtHftEngine;
class TraderAdapter;

class HftStraBaseCtx : public IHftStraCtx, public ITrdNotifySink
{
public:
	HftStraBaseCtx(WtHftEngine* engine, const char* name);
	virtual ~HftStraBaseCtx();

	void setTrader(TraderAdapter* trader);

public:
	//////////////////////////////////////////////////////////////////////////
	//IHftStraCtx ½Ó¿Ú
	virtual uint32_t id() override;

	virtual void on_init() override;

	virtual void on_tick(const char* code, WTSTickData* newTick) override;

	virtual void on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_schedule(uint32_t uDate, uint32_t uTime) override;

	virtual bool stra_cancel(uint32_t localid) override;

	virtual OrderIDs stra_cancel(const char* code, bool isBuy, uint32_t qty) override;

	virtual OrderIDs stra_buy(const char* stdCode, double price, uint32_t qty) override;

	virtual OrderIDs stra_sell(const char* stdCode, double price, uint32_t qty) override;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* code) override;

	virtual WTSKlineSlice* stra_get_bars(const char* code, const char* period, uint32_t count) override;

	virtual WTSTickSlice* stra_get_ticks(const char* code, uint32_t count) override;

	virtual WTSTickData* stra_get_last_tick(const char* code) override;

	virtual void stra_log_text(const char* fmt, ...) override;

	virtual int32_t stra_get_position(const char* code) override;
	virtual double stra_get_price(const char* code) override;
	virtual int32_t stra_get_undone(const char* stdCode) override;

	virtual uint32_t stra_get_date() override;
	virtual uint32_t stra_get_time() override;
	virtual uint32_t stra_get_secs() override;

	virtual void sub_ticks(const char* code) override;

	//////////////////////////////////////////////////////////////////////////
	virtual void on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled = false) override;

	virtual void on_position(const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail) override;

	virtual void on_channel_ready() override;

	virtual void on_channel_lost() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

protected:
	const char* get_inner_code(const char* stdCode);

private:
	uint32_t		_context_id;
	WtHftEngine*	_engine;
	TraderAdapter*	_trader;

	std::unordered_map<std::string, std::string> _code_map;
};

NS_OTP_END
