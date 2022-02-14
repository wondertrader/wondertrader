#include "WtUftStraFact.h"
#include "WtUftStraDemo.h"

#include <string.h>

const char* FACT_NAME = "WtUftStraFact";

extern "C"
{
	EXPORT_FLAG IUftStrategyFact* createStrategyFact()
	{
		IUftStrategyFact* fact = new WtUftStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteStrategyFact(IUftStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
}


WtUftStraFact::WtUftStraFact()
{
}


WtUftStraFact::~WtUftStraFact()
{
}

const char* WtUftStraFact::getName()
{
	return FACT_NAME;
}

void WtUftStraFact::enumStrategy(FuncEnumUftStrategyCallback cb)
{
	cb(FACT_NAME, "SimpleUft", true);
}

UftStrategy* WtUftStraFact::createStrategy(const char* name, const char* id)
{
	if(strcmp(name, "SimpleUft") == 0)
	{
		return new WtUftStraDemo(id);
	}


	return NULL;
}

bool WtUftStraFact::deleteStrategy(UftStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}
