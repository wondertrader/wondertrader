#pragma once
#include "../WtCore/SelStrategyDefs.h"

USING_NS_OTP;

class WtSelStraFact : public ISelStrategyFact
{
public:
	WtSelStraFact();
	virtual ~WtSelStraFact();

public:
	virtual const char* getName() override;

	virtual SelStrategy* createStrategy(const char* name, const char* id) override;

	virtual void enumStrategy(FuncEnumSelStrategyCallback cb) override;

	virtual bool deleteStrategy(SelStrategy* stra) override;
};

