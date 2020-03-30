#pragma once
#include "../WtCore/RiskMonDefs.h"

USING_NS_OTP;

class WtRiskMonFact : public IRiskMonitorFact
{
public:
	WtRiskMonFact();
	virtual ~WtRiskMonFact();

public:
	virtual const char* getName() override;
	virtual void enumRiskMonitors(FuncEnumRiskMonCallback cb) override;

	virtual WtRiskMonitor* createRiskMonotor(const char* name) override;

	virtual bool deleteRiskMonotor(WtRiskMonitor* unit) override;

};

