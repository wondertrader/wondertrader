#include "PyMfContext.h"
#include "WtRtRunner.h"

extern WtRtRunner& getRunner();


PyMfContext::PyMfContext(WtMfEngine* env, const char* name)
	: MfStraBaseCtx(env, name)
{
}


PyMfContext::~PyMfContext()
{
}

void PyMfContext::on_init()
{
	MfStraBaseCtx::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, false);
}

void PyMfContext::on_session_begin()
{
	MfStraBaseCtx::on_session_begin();
}

void PyMfContext::on_session_end()
{
	MfStraBaseCtx::on_session_end();
}

void PyMfContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	getRunner().ctx_on_calc(_context_id, false);
}

void PyMfContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, false);
}

void PyMfContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, false);
}
