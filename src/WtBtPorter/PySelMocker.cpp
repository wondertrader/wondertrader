#include "PySelMocker.h"
#include "WtBtRunner.h"

extern WtBtRunner& getRunner();


PySelMocker::PySelMocker(HisDataReplayer* replayer, const char* name)
	: SelMocker(replayer, name)
{
}


PySelMocker::~PySelMocker()
{
}

void PySelMocker::on_init()
{
	SelMocker::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, false);
}

void PySelMocker::on_session_begin()
{
	SelMocker::on_session_begin();
}

void PySelMocker::on_session_end()
{
	SelMocker::on_session_end();
}

void PySelMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	SelMocker::on_tick_updated(stdCode, newTick);

	//向外部回调
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, false);
}

void PySelMocker::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	SelMocker::on_bar_close(stdCode, period, newBar);
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, false);
}

void PySelMocker::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	SelMocker::on_strategy_schedule(curDate, curTime);

	//向外部回调
	getRunner().ctx_on_calc(_context_id, false);
}
