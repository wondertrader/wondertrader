#pragma once
#include "../WtCore/SelStraBaseCtx.h"

USING_NS_WTP;

class ExpSelContext : public SelStraBaseCtx
{
public:
	ExpSelContext(WtSelEngine* env, const char* name, int32_t slippage);
	virtual ~ExpSelContext();

public:
	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uDate) override;

	virtual void on_session_end(uint32_t uDate) override;

	virtual void on_strategy_schedule(uint32_t curDate, uint32_t curTime) override;

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) override;

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;

};

