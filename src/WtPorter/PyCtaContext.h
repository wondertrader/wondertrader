#pragma once
#include "../WtCore/CtaStraBaseCtx.h"

USING_NS_OTP;

class PyCtaContext : public CtaStraBaseCtx
{
public:
	PyCtaContext(WtCtaEngine* env, const char* name);
	virtual ~PyCtaContext();

public:
	virtual void on_init() override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) override;

	virtual void on_mainkline_updated(uint32_t curDate, uint32_t curTime) override;
};

