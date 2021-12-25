/*!
 * \file WtExeFact.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * 
 */
#pragma once
#include "../Includes/ExecuteDefs.h"

USING_NS_WTP;

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

