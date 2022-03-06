#pragma once
#include "Includes/CtaStrategyDefs.h"

USING_NS_WTP;

class WtStraFact : public ICtaStrategyFact
{
public:
	WtStraFact();
	virtual ~WtStraFact();

public:
	virtual const char* getName() override;

	virtual CtaStrategy* createStrategy(const char* name, const char* id) override;

	virtual void enumStrategy(FuncEnumStrategyCallback cb) override;

	virtual bool deleteStrategy(CtaStrategy* stra) override;	
};

