/*!
 * \file IndexWorkerMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <memory>

#include "../Includes/FasterDefs.h"
#include "../Includes/IIndexDefs.h"
#include "../Share/DLLHelper.hpp"

#include "../Share/threadpool.hpp"

USING_NS_WTP;

class IdxWorkerWrapper
{
public:
	IdxWorkerWrapper(IIndexWorker* stra, IIndexWorkerFact* fact) :_worker(stra), _fact(fact){}
	~IdxWorkerWrapper()
	{
		if (_worker)
		{
			_fact->delete_worker(_worker);
		}
	}

	IIndexWorker* self(){ return _worker; }


private:
	IIndexWorker*		_worker;
	IIndexWorkerFact*	_fact;
};
typedef std::shared_ptr<IdxWorkerWrapper>	IndexWorkerPtr;


class IndexWorkerMgr : public IIndexContext
{
public:
	IndexWorkerMgr();
	~IndexWorkerMgr();

public:
	bool loadFactories(const char* path);

	IndexWorkerPtr createWorker(const char* name, const char* id);
	IndexWorkerPtr createWorker(const char* factname, const char* unitname, const char* id);

	IndexWorkerPtr getWorker(const char* id);

private:
	typedef struct _IdxFactInfo
	{
		std::string			_module_path;
		DllHandle			_module_inst;
		IIndexWorkerFact*	_fact;
		FuncCreateIndexFact	_creator;
		FuncDeleteIndexFact	_remover;
	} IdxFactInfo;
	typedef wt_hashmap<std::string, IdxFactInfo> StraFactMap;

	StraFactMap	_factories;

	typedef wt_hashmap<std::string, IndexWorkerPtr> IndexWorkerMap;
	IndexWorkerMap	_workers;

	IHotMgr*		_hot_mgr;
	IBaseDataMgr*	_bd_mgr;
	DataManager*	_data_mgr;

	typedef std::shared_ptr<boost::threadpool::pool> ThreadPoolPtr;
	ThreadPoolPtr	_pool;

	wt_hashset<std::string>	_subbed;
};

