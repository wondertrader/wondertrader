#pragma once
#include "SelStraBaseCtx.h"

USING_NS_WTP;

class SelStrategy;

class SelStraContext : public SelStraBaseCtx
{
public:
	SelStraContext(WtSelEngine* engine, const char* name);
	virtual ~SelStraContext();

	void set_strategy(SelStrategy* stra){ _strategy = stra; }
	SelStrategy* get_stragety() { return _strategy; }

public:
	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uTDate) override;

	virtual void on_session_end(uint32_t uTDate) override;

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) override;

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_strategy_schedule(uint32_t curDate, uint32_t curTime) override;

private:
	SelStrategy* _strategy;
};

