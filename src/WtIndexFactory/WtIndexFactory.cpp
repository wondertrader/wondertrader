#include "WtIndexFactory.h"
#include "WtBlkIdxWorker.h"

#include <string.h>

const char* FACT_NAME = "WtIndexFactory";

extern "C"
{
	EXPORT_FLAG IIndexWorkerFact* create_index_factory()
	{
		IIndexWorkerFact* fact = new WtIdxFactory();
		return fact;
	}

	EXPORT_FLAG void delete_index_factory(IIndexWorkerFact* fact)
	{
		if (fact != NULL)
			delete fact;
	}
};


WtIdxFactory::WtIdxFactory()
{
}


WtIdxFactory::~WtIdxFactory()
{
}

IIndexWorker* WtIdxFactory::create_worker(const char* name, const char* id)
{
	if (strcmp(name, "WtBuiltinIndex") == 0)
		return new WtBlkIdxWorker(id);

	return NULL;
}

bool WtIdxFactory::delete_worker(IIndexWorker* worker)
{
	if (worker == NULL)
		return true;

	if (strcmp(worker->get_fact_name(), FACT_NAME) != 0)
		return false;

	delete worker;
	return true;
}
const char* WtIdxFactory::get_name()
{
	return FACT_NAME;
}