#pragma once
#include "MfStraBaseCtx.h"

USING_NS_OTP;

class MfStrategy;

class MfStraContext : public MfStraBaseCtx
{
public:
	MfStraContext(WtMfEngine* engine, const char* name);
	virtual ~MfStraContext();

	void set_strategy(MfStrategy* stra){ _strategy = stra; }
	MfStrategy* get_stragety() { return _strategy; }

public:
	virtual void on_init() override;

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) override;

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_strategy_schedule(uint32_t curDate, uint32_t curTime) override;

private:
	MfStrategy* _strategy;
};

