#include "WtSelStraFact.h"
#include "WtStraDtSel.h"

#include <string.h>
#include <boost/config.hpp>

const char* FACT_NAME = "WtSelStraFact";


extern "C"
{
	EXPORT_FLAG ISelStrategyFact* createSelStrategyFact()
	{
		ISelStrategyFact* fact = new WtSelStraFact();
		return fact;
	}

	EXPORT_FLAG void deleteSelStrategyFact(ISelStrategyFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


WtSelStraFact::WtSelStraFact()
{
}


WtSelStraFact::~WtSelStraFact()
{
}

SelStrategy* WtSelStraFact::createStrategy(const char* name, const char* id)
{
	if (strcmp(name, "DualThrustSelection") == 0)
		return new WtStraDtSel(id);

	return NULL;
}

bool WtSelStraFact::deleteStrategy(SelStrategy* stra)
{
	if (stra == NULL)
		return true;

	if (strcmp(stra->getFactName(), FACT_NAME) != 0)
		return false;

	delete stra;
	return true;
}

void WtSelStraFact::enumStrategy(FuncEnumSelStrategyCallback cb)
{
	cb(FACT_NAME, "DualThrustSelection", true);
}

const char* WtSelStraFact::getName()
{
	return FACT_NAME;
}