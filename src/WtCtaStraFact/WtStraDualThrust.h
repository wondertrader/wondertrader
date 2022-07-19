#pragma once
#include "../Includes/CtaStrategyDefs.h"

class WtStraDualThrust : public CtaStrategy
{
public:
	WtStraDualThrust(const char* id);
	virtual ~WtStraDualThrust();

public:
	virtual const char* getFactName() override;

	virtual const char* getName() override;

	virtual bool init(WTSVariant* cfg) override;

	virtual void on_schedule(ICtaStraCtx* ctx, uint32_t curDate, uint32_t curTime) override;

	virtual void on_init(ICtaStraCtx* ctx) override;

	virtual void on_tick(ICtaStraCtx* ctx, const char* stdCode, WTSTickData* newTick) override;

	virtual void on_session_begin(ICtaStraCtx* ctx, uint32_t uTDate) override;

private:
	//ָ�����
	double		_k1;
	double		_k2;
	uint32_t	_days;

	std::string _moncode;

	//��������
	std::string _period;
	//K������
	uint32_t	_count;

	//��Լ����
	std::string _code;

	bool		_isstk;
};

