/*!
 * \file WtRiskMonFact.cpp
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#include "WtRiskMonFact.h"
#include "WtSimpRiskMon.h"

const char* FACT_NAME = "WtRiskMonFact";

extern "C"
{
	EXPORT_FLAG IRiskMonitorFact* createRiskMonFact()
	{
		IRiskMonitorFact* fact = new WtRiskMonFact();
		return fact;
	}

	EXPORT_FLAG void deleteRiskMonFact(IRiskMonitorFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


WtRiskMonFact::WtRiskMonFact()
{
}


WtRiskMonFact::~WtRiskMonFact()
{
}

const char* WtRiskMonFact::getName()
{
	return FACT_NAME;
}

void WtRiskMonFact::enumRiskMonitors(FuncEnumRiskMonCallback cb)
{
	//cb(FACT_NAME, "WtSimpExeUnit", false);
	cb(FACT_NAME, "SimpleRiskMon", true);
}

WtRiskMonitor* WtRiskMonFact::createRiskMonotor(const char* name)
{
	if (strcmp(name, "SimpleRiskMon") == 0)
		return new WtSimpleRiskMon();
	return NULL;
}

bool WtRiskMonFact::deleteRiskMonotor(WtRiskMonitor* unit)
{
	if (unit == NULL)
		return true;

	if (strcmp(unit->getFactName(), FACT_NAME) != 0)
		return false;

	delete unit;
	return true;
}
