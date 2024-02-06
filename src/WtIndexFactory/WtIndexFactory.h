#pragma once
#include "../Includes/IIndexDefs.h"

USING_NS_WTP;

class WtIdxFactory : public IIndexWorkerFact
{
public:
	WtIdxFactory();
	virtual ~WtIdxFactory();

public:
	virtual const char* get_name() override;

	virtual IIndexWorker* create_worker(const char* name, const char* id) override;

	virtual bool delete_worker(IIndexWorker* worker) override;
};

