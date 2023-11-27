#pragma once
#include "../Includes/UftStrategyDefs.h"

USING_NS_WTP;

class WtUftStraFact : public IUftStrategyFact
{
public:
	WtUftStraFact();
	virtual ~WtUftStraFact();

public:
	virtual const char* getName() override;

	virtual void enumStrategy(FuncEnumUftStrategyCallback cb) override;

	virtual UftStrategy* createStrategy(const char* name, const char* id) override;

	virtual bool deleteStrategy(UftStrategy* stra) override;
};

