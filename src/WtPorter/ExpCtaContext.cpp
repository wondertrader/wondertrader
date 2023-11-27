/*!
 * \file PyCtaContext.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ExpCtaContext.h"
#include "WtRtRunner.h"

#include "../Share/StrUtil.hpp"

extern WtRtRunner& getRunner();

ExpCtaContext::ExpCtaContext(WtCtaEngine* env, const char* name, int32_t slippage)
	: CtaStraBaseCtx(env, name, slippage)
{
}


ExpCtaContext::~ExpCtaContext()
{
}

void ExpCtaContext::on_init()
{
	CtaStraBaseCtx::on_init();

	//向外部回调
	getRunner().ctx_on_init(_context_id, ET_CTA);

	dump_chart_info();
}

void ExpCtaContext::on_session_begin(uint32_t uDate)
{
	CtaStraBaseCtx::on_session_begin(uDate);

	//向外部回调
	getRunner().ctx_on_session_event(_context_id, uDate, true, ET_CTA);
}

void ExpCtaContext::on_session_end(uint32_t uDate)
{
	//向外部回调
	getRunner().ctx_on_session_event(_context_id, uDate, false, ET_CTA);

	CtaStraBaseCtx::on_session_end(uDate);
}

void ExpCtaContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	auto it = _tick_subs.find(stdCode);
	if (it == _tick_subs.end())
		return;

	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_CTA);
}

void ExpCtaContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, ET_CTA);
}

void ExpCtaContext::on_calculate(uint32_t curDate, uint32_t curTime)
{
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_CTA);
}

void ExpCtaContext::on_condition_triggered(const char* stdCode, double target, double price, const char* usertag)
{
	getRunner().ctx_on_cond_triggered(_context_id, stdCode, target, price, usertag, ET_CTA);
}
