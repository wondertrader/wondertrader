#include "ExpSelMocker.h"
#include "WtBtRunner.h"

extern WtBtRunner& getRunner();


ExpSelMocker::ExpSelMocker(HisDataReplayer* replayer, const char* name, int32_t slippage /* = 0 */, bool isRatioSlp/* = false*/)
	: SelMocker(replayer, name, slippage, isRatioSlp)
{
}


ExpSelMocker::~ExpSelMocker()
{
}

void ExpSelMocker::on_init()
{
	SelMocker::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, ET_SEL);

	getRunner().on_initialize_event();
}

void ExpSelMocker::on_session_begin(uint32_t uDate)
{
	SelMocker::on_session_begin(uDate);

	getRunner().ctx_on_session_event(_context_id, uDate, true, ET_SEL);
	getRunner().on_session_event(uDate, true);
}

void ExpSelMocker::on_session_end(uint32_t uDate)
{
	SelMocker::on_session_end(uDate);

	getRunner().ctx_on_session_event(_context_id, uDate, false, ET_SEL);
	getRunner().on_session_event(uDate, false);
}

void ExpSelMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	//向外部回调
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_SEL);
}

void ExpSelMocker::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	SelMocker::on_bar_close(stdCode, period, newBar);
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, ET_SEL);
}

void ExpSelMocker::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	SelMocker::on_strategy_schedule(curDate, curTime);

	//向外部回调
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_SEL);

	getRunner().on_schedule_event(curDate, curTime);
}

void ExpSelMocker::on_bactest_end()
{
	getRunner().on_backtest_end();
}