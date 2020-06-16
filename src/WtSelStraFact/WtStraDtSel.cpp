#include "WtStraDtSel.h"

#include "../WtCore/ISelStraCtx.h"

#include "../Share/WTSContractInfo.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/WTSDataDef.hpp"
#include "../Share/decimal.h"

extern const char* FACT_NAME;

WtStraDtSel::WtStraDtSel(const char* id)
	:SelStrategy(id)
{
}


WtStraDtSel::~WtStraDtSel()
{
}

const char* WtStraDtSel::getName()
{
	return "DualThrustSelection";
}

const char* WtStraDtSel::getFactName()
{
	return FACT_NAME;
}

bool WtStraDtSel::init(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	_days = cfg->getUInt32("days");
	_k1 = cfg->getDouble("k1");
	_k2 = cfg->getDouble("k2");

	_period = cfg->getCString("period");
	_count = cfg->getUInt32("count");
	_code = cfg->getCString("code");

	return true;
}

void WtStraDtSel::on_init(ISelStraCtx* ctx)
{
	
}

void WtStraDtSel::on_schedule(ISelStraCtx* ctx, uint32_t uDate, uint32_t uTime)
{
	
}

void WtStraDtSel::on_tick(ISelStraCtx* ctx, const char* stdCode, WTSTickData* newTick)
{
}

void WtStraDtSel::on_bar(ISelStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar)
{

}
