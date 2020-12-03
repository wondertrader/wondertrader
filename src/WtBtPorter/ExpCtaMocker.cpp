/*!
 * \file PyCtaMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ExpCtaMocker.h"
#include "WtBtRunner.h"

#include "../Share/StrUtil.hpp"

extern WtBtRunner& getRunner();

ExpCtaMocker::ExpCtaMocker(HisDataReplayer* replayer, const char* name)
	: CtaMocker(replayer, name)
{
}


ExpCtaMocker::~ExpCtaMocker()
{
}

void ExpCtaMocker::on_init()
{
	CtaMocker::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, ET_CTA);
}

void ExpCtaMocker::on_session_begin()
{
	CtaMocker::on_session_begin();
}

void ExpCtaMocker::on_session_end()
{
	CtaMocker::on_session_end();
}

void ExpCtaMocker::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	CtaMocker::on_tick_updated(stdCode, newTick);
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_CTA);
}

void ExpCtaMocker::on_bar_close(const char* code, const char* period, WTSBarStruct* newBar)
{
	CtaMocker::on_bar_close(code, period, newBar);
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, code, period, newBar, ET_CTA);
}

void ExpCtaMocker::on_mainkline_updated(uint32_t curDate, uint32_t curTime)
{
	CtaMocker::on_mainkline_updated(curDate, curTime);
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_CTA);
}
