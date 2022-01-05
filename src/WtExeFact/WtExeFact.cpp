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
