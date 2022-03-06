#pragma once
#include <unordered_set>
#include <memory>
#include <thread>
#include <mutex>

#include "Includes/HftStrategyDefs.h"

class WtHftStraDemo : public HftStrategy
{
public:
	WtHftStraDemo(const char* id);
	~WtHftStraDemo();

private:
	void	check_orders();

public:
	virtual const char* getName() override;

	virtual const char* getFactName() override;

	virtual bool init(WTSVariant* cfg) override;

	virtual void on_init(IHftStraCtx* ctx) override;

	virtual void on_tick(IHftStraCtx* ctx, const char* code, WTSTickData* newTick) override;

	virtual void on_bar(IHftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_trade(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, double qty, double price, const char* userTag) override;

	virtual void on_position(IHftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) override;

	virtual void on_order(IHftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag) override;

	virtual void on_channel_ready(IHftStraCtx* ctx) override;

	virtual void on_channel_lost(IHftStraCtx* ctx) override;

	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message, const char* userTag) override;

private:
	WTSTickData*	_last_tick;
	IHftStraCtx*	_ctx;
	std::string		_code;
	uint32_t		_secs;
	uint32_t		_freq;
	int32_t			_offset;
	uint32_t		_unit;
	double			_reserved;
	bool			_stock;

	typedef std::unordered_set<uint32_t> IDSet;
	IDSet			_orders;
	std::mutex		_mtx_ords;

	uint64_t		_last_entry_time;

	bool			_channel_ready;
	uint32_t		_last_calc_time;
	uint32_t		_cancel_cnt;
};

