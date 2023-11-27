/*!
 * \file WtRiskMonFact.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include "../Includes/RiskMonDefs.h"

USING_NS_WTP;

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

