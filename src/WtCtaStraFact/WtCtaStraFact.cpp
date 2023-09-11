#include "WtCtaStraFact.h"
#include "WtStraDualThrust.h"

#include <string.h>
#include <boost/config.hpp>

const char* FACT_NAME = "WtCtaStraFact";


extern "C"
{
	EXPORT_FLAG ICtaStrategyFact* createStrategyFact()
	{
		ICtaStrategyFact* fact = new WtStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(ICtaStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


WtStraFact::WtStraFact()
{
}


WtStraFact::~WtStraFact()
{
}

CtaStrategy* WtStraFact::createStrategy(const char* name, const char* id)
{
	if (strcmp(name, "DualThrust") == 0)
		return new WtStraDualThrust(id);

	return NULL;
}

bool WtStraFact::deleteStrategy(CtaStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}

void WtStraFact::enumStrategy(FuncEnumStrategyCallback cb)
{
	cb(FACT_NAME, "DualThrust", true);
}

const char* WtStraFact::getName()
{
	return FACT_NAME;
}