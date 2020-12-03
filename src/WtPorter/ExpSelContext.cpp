#include "ExpSelContext.h"
#include "WtRtRunner.h"

extern WtRtRunner& getRunner();


ExpSelContext::ExpSelContext(WtSelEngine* env, const char* name)
	: SelStraBaseCtx(env, name)
{
}


ExpSelContext::~ExpSelContext()
{
}

void ExpSelContext::on_init()
{
	SelStraBaseCtx::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, ET_SEL);
}

void ExpSelContext::on_session_begin()
{
	SelStraBaseCtx::on_session_begin();
}

void ExpSelContext::on_session_end()
{
	SelStraBaseCtx::on_session_end();
}

void ExpSelContext::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_SEL);
}

void ExpSelContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, ET_SEL);
}

void ExpSelContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_SEL);
}
