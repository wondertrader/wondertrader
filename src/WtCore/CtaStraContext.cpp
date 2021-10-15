/*!
 * \file CtaStraContext.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaStraContext.h"
#include "WtCtaEngine.h"
#include "../Includes/CtaStrategyDefs.h"

#include <exception>

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSContractInfo.hpp"


CtaStraContext::CtaStraContext(WtCtaEngine* engine, const char* name)
	: CtaStraBaseCtx(engine, name)
{
}


CtaStraContext::~CtaStraContext()
{
}

//////////////////////////////////////////////////////////////////////////
//»Øµ÷º¯Êý
void CtaStraContext::on_bar_close(const char* code, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, newBar);
}

void CtaStraContext::on_init()
{
	CtaStraBaseCtx::on_init();

	if (_strategy)
		_strategy->on_init(this);
}

void CtaStraContext::on_session_begin(uint32_t uTDate)
{
	CtaStraBaseCtx::on_session_begin(uTDate);

	if (_strategy)
		_strategy->on_session_begin(this, uTDate);
}

void CtaStraContext::on_session_end(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, uTDate);

	CtaStraBaseCtx::on_session_end(uTDate);
}

void CtaStraContext::on_tick_updated(const char* code, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, code, newTick);
}

void CtaStraContext::on_calculate(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}



