#include "SelStraContext.h"
#include "../Includes/SelStrategyDefs.h"


SelStraContext::SelStraContext(WtSelEngine* engine, const char* name)
	: SelStraBaseCtx(engine, name)
	, _strategy(NULL)
{
}


SelStraContext::~SelStraContext()
{
}

void SelStraContext::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}

void SelStraContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, newBar);
}

void SelStraContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void SelStraContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}
