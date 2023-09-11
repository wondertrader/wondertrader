#include "WtHftStraFact.h"
#include "WtHftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "WtHftStraFact";

extern "C"
{
	EXPORT_FLAG IHftStrategyFact* createStrategyFact()
	{
		IHftStrategyFact* fact = new WtHftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IHftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


WtHftStraFact::WtHftStraFact()
{
}


WtHftStraFact::~WtHftStraFact()
{
}

const char* WtHftStraFact::getName()
{
	return FACT_NAME;
}

void WtHftStraFact::enumStrategy(FuncEnumHftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleHft", true);
}

HftStrategy* WtHftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleHft") == 0)
	{
		return new WtHftStraDemo(id);
	}


	return NULL;
}

bool WtHftStraFact::deleteStrategy(HftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
