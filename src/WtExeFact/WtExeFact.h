#pragma once
#include "../WtCore/ExecuteDefs.h"

USING_NS_OTP;

class WtExeFact : public IExecuterFact
{
public:
	WtExeFact();
	virtual ~WtExeFact();

public:
	virtual const char* getName() override;
	virtual void enumExeUnit(FuncEnumUnitCallback cb) override;

	virtual ExecuteUnit* createExeUnit(const char* name) override;

	virtual bool deleteExeUnit(ExecuteUnit* unit) override;

};

