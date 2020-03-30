/*!
 * \file WtExeFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtExeFact.h"

#include "WtSimpExeUnit.h"
#include "WtHfExeUnit.h"

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
	cb(FACT_NAME, "WtSimpExeUnit", false);
	cb(FACT_NAME, "WtHfExeUnit", true);
}

ExecuteUnit* WtExeFact::createExeUnit(const char* name)
{
	if (strcmp(name, "WtSimpExeUnit") == 0)
		return new WtSimpExeUnit();
	else if (strcmp(name, "WtHfExeUnit") == 0)
		return new WtHfExeUnit();

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
