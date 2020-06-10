#include "MfStraContext.h"
#include "MfStrategyDefs.h"


MfStraContext::MfStraContext(WtMfEngine* engine, const char* name)
	: MfStraBaseCtx(engine, name)
	, _strategy(NULL)
{
}


MfStraContext::~MfStraContext()
{
}

void MfStraContext::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}

void MfStraContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, newBar);
}

void MfStraContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void MfStraContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}
