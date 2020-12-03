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

ExpCtaContext::ExpCtaContext(WtCtaEngine* env, const char* name)
	: CtaStraBaseCtx(env, name)
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
}

void ExpCtaContext::on_session_begin()
{
	CtaStraBaseCtx::on_session_begin();
}

void ExpCtaContext::on_session_end()
{
	CtaStraBaseCtx::on_session_end();
}

void ExpCtaContext::on_tick_updated(const char* stdCode, WTSTickData* newTick)
{
	getRunner().ctx_on_tick(_context_id, stdCode, newTick, ET_CTA);
}

void ExpCtaContext::on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar)
{
	//要向外部回调
	getRunner().ctx_on_bar(_context_id, stdCode, period, newBar, ET_CTA);
}

void ExpCtaContext::on_mainkline_updated(uint32_t curDate, uint32_t curTime)
{
	getRunner().ctx_on_calc(_context_id, curDate, curTime, ET_CTA);
}
