#include "PyMfMocker.h"
#include "WtBtRunner.h"

extern WtBtRunner& getRunner();


PyMfMocker::PyMfMocker(HisDataReplayer* replayer, const char* name)
	: MfMocker(replayer, name)
{
}


PyMfMocker::~PyMfMocker()
{
}

void PyMfMocker::on_init()
{
	MfMocker::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, false);
}

void PyMfMocker::on_session_begin()
{
	MfMocker::on_session_begin();
}

void PyMfMocker::on_session_end()
{
	MfMocker::on_session_end();
}

void PyMfMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	MfMocker::on_tick_updated(stdCode, newTick);

	//向外部回调
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, false);
}

void PyMfMocker::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	MfMocker::on_bar_close(stdCode, period, newBar);
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, false);
}

void PyMfMocker::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	MfMocker::on_strategy_schedule(curDate, curTime);

	//向外部回调
	getRunner().ctx_on_calc(_context_id, false);
}
