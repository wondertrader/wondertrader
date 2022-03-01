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
	SelStraBaseCtx::on_init();

	if (_strategy)
		_strategy->on_init(this);
}

void SelStraContext::on_session_begin(uint32_t uTDate)
{
	SelStraBaseCtx::on_session_begin(uTDate);

	if (_strategy)
		_strategy->on_session_begin(this, uTDate);
}

void SelStraContext::on_session_end(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, uTDate);

	SelStraBaseCtx::on_session_end(uTDate);
}

void SelStraContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, stdCode, period, newBar);
}

void SelStraContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void SelStraContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}
