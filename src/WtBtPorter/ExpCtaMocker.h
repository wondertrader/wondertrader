/*!
 * \file PyCtaMocker.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../WtBtCore/CtaMocker.h"

USING_NS_OTP;

class ExpCtaMocker : public CtaMocker
{
public:
	ExpCtaMocker(HisDataReplayer* replayer, const char* name, int32_t slippage = 0, bool persistData = true, EventNotifier* notifier = NULL);
	virtual ~ExpCtaMocker();

public:
	virtual void on_init() override;

	virtual void on_session_begin(uint32_t uCurDate) override;

	virtual void on_session_end(uint32_t uCurDate) override;

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) override;

	virtual void on_calculate(uint32_t curDate, uint32_t curTime) override;

	virtual void on_calculate_done(uint32_t curDate, uint32_t curTime) override;

	virtual void on_bactest_end() override;
};

