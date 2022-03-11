#pragma once
#include <unordered_set>
#include <memory>
#include <thread>
#include <mutex>

#include "../Includes/UftStrategyDefs.h"

class WtUftStraDemo : public UftStrategy
{
public:
	WtUftStraDemo(const char* id);
	~WtUftStraDemo();

private:
	void	check_orders();

public:
	virtual const char* getName() override;

	virtual const char* getFactName() override;

	virtual bool init(WTSVariant* cfg) override;

	virtual void on_init(IUftStraCtx* ctx) override;

	virtual void on_tick(IUftStraCtx* ctx, const char* code, WTSTickData* newTick) override;

	virtual void on_bar(IUftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_trade(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double qty, double price) override;

	virtual void on_position(IUftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) override;

	virtual void on_order(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled) override;

	virtual void on_channel_ready(IUftStraCtx* ctx) override;

	virtual void on_channel_lost(IUftStraCtx* ctx) override;

	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message) override;

private:
	WTSTickData*	_last_tick;
	IUftStraCtx*	_ctx;
	std::string		_code;
	uint32_t		_secs;
	uint32_t		_freq;
	int32_t			_offset;
	double			_lots;

	typedef std::unordered_set<uint32_t> IDSet;
	IDSet			_orders;
	std::mutex		_mtx_ords;

	uint64_t		_last_entry_time;

	bool			_channel_ready;
	uint32_t		_last_calc_time;
	uint32_t		_cancel_cnt;
};

