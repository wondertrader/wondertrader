#pragma once
#include "../Includes/SelStrategyDefs.h"

#include <unordered_set>

USING_NS_WTP;

class WtStraDtSel : public SelStrategy
{
public:
	WtStraDtSel(const char* id);
	~WtStraDtSel();

public:
	virtual const char* getName() override;

	virtual const char* getFactName() override;

	virtual bool init(WTSVariant* cfg) override;

	virtual void on_init(ISelStraCtx* ctx) override;

	virtual void on_schedule(ISelStraCtx* ctx, uint32_t uDate, uint32_t uTime) override;

	virtual void on_tick(ISelStraCtx* ctx, const char* stdCode, WTSTickData* newTick) override;

	virtual void on_bar(ISelStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar) override;

private:
	//指标参数
	double		_k1;
	double		_k2;
	uint32_t	_days;

	//数据周期
	std::string _period;
	//K线条数
	uint32_t	_count;

	bool		_isstk;

	//合约代码
	std::unordered_set<std::string> _codes;

};

