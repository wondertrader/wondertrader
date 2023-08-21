/*!
 * \file WtExeFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtExeFact.h"

#include "WtTWapExeUnit.h"
#include "WtMinImpactExeUnit.h"
#include "WtDiffMinImpactExeUnit.h"
#include "WtStockMinImpactExeUnit.h"
#include "WtVWapExeUnit.h"
#include "WtStockVWapExeUnit.h"
const char* FACT_NAME = "WtExeFact";

extern "C"
{
	EXPORT_FLAG IExecuterFact* createExecFact()
	{
		IExecuterFact* fact = new WtExeFact();
		return fact;
	}

	EXPORT_FLAG void deleteExecFact(IExecuterFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


WtExeFact::WtExeFact()
{
}


WtExeFact::~WtExeFact()
{
}

const char* WtExeFact::getName()
{
	return FACT_NAME;
}

void WtExeFact::enumExeUnit(FuncEnumUnitCallback cb)
{
	cb(FACT_NAME, "WtTWapExeUnit", false);
	cb(FACT_NAME, "WtMinImpactExeUnit", true);
}

ExecuteUnit* WtExeFact::createExeUnit(const char* name)
{
	if (strcmp(name, "WtTWapExeUnit") == 0)
		return new WtTWapExeUnit();
	else if (strcmp(name, "WtMinImpactExeUnit") == 0)
		return new WtMinImpactExeUnit();
	else if (strcmp(name, "WtStockMinImpactExeUnit") == 0)
		return new WtStockMinImpactExeUnit();
	else if (strcmp(name, "WtVWapExeUnit") == 0)
		return  new WtVWapExeUnit();
	else if (strcmp(name, "WtStockVWapExeUnit") == 0)
		return new WtStockVWapExeUnit();
	return NULL;
}

ExecuteUnit* WtExeFact::createDiffExeUnit(const char* name)
{
	if (strcmp(name, "WtDiffMinImpactExeUnit") == 0)
		return new WtDiffMinImpactExeUnit();

	return NULL;
}

ExecuteUnit* WtExeFact::createArbiExeUnit(const char* name)
{
	return NULL;
}

bool WtExeFact::deleteExeUnit(ExecuteUnit* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
