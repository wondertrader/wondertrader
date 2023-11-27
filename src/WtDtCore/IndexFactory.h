#pragma once
#include "IndexWorker.h"
#include "../Share/threadpool.hpp"
#include <vector>

class DataManager;

class IndexFactory
{
public:
	IndexFactory():_hot_mgr(NULL), _bd_mgr(NULL){}

public:
	bool	init(WTSVariant* config, IHotMgr* hotMgr, IBaseDataMgr* bdMgr, DataManager* dataMgr);
	void	handle_quote(WTSTickData* newTick);

public:
	inline IHotMgr*			get_hot_mgr() { return _hot_mgr; }
	inline IBaseDataMgr*	get_bd_mgr() { return _bd_mgr; }

	WTSTickData*	sub_ticks(const char* fullCode);

	void			push_tick(WTSTickData* newTick);

private:
	typedef std::vector<IndexWorkerPtr>	IndexWorkers;
	IndexWorkers	_workers;
	IHotMgr*		_hot_mgr;
	IBaseDataMgr*	_bd_mgr;
	DataManager*	_data_mgr;

	typedef std::shared_ptr<boost::threadpool::pool> ThreadPoolPtr;
	ThreadPoolPtr	_pool;

	wt_hashset<std::string>	_subbed;
};

