/*!
 * \file IndexWorkerMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "IndexWorkerMgr.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"

#include <filesystem>
namespace fs = std::filesystem;


IndexWorkerMgr::IndexWorkerMgr()
{
}


IndexWorkerMgr::~IndexWorkerMgr()
{
}

bool IndexWorkerMgr::loadFactories(const char* path)
{
	if (!StdFile::exists(path))
	{
		WTSLogger::error("Directory {} of index worker factory not exists", path);
		return false;
	}

	uint32_t count = 0;
	fs::path myPath(path);
	fs::directory_iterator endIter;
	for (fs::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (fs::is_directory(iter->path()))
			continue;

#ifdef _WIN32
		if (iter->path().extension() != ".dll")
			continue;
#else //_UNIX
		if (iter->path().extension() != ".so")
			continue;
#endif

		DllHandle hInst = DLLHelper::load_library(iter->path().string().c_str());
		if (hInst == NULL)
			continue;

		FuncCreateIndexFact creator = (FuncCreateIndexFact)DLLHelper::get_symbol(hInst, "create_index_factory");
		if (creator == NULL)
		{
			DLLHelper::free_library(hInst);
			continue;
		}

		IIndexWorkerFact* fact = creator();
		if(fact != NULL)
		{
			IdxFactInfo& fInfo = _factories[fact->get_name()];
			fInfo._module_inst = hInst;
			fInfo._module_path = iter->path().string();
			fInfo._creator = creator;
			fInfo._remover = (FuncDeleteIndexFact)DLLHelper::get_symbol(hInst, "delete_index_factory");
			fInfo._fact = fact;

			WTSLogger::info("Index worker factory [{}] loaded", fact->get_name());

			count++;
		}
		else
		{
			DLLHelper::free_library(hInst);
			continue;
		}
		
	}

	WTSLogger::info("{} index worker factories in directory [{}] loaded", count, path);

	return true;
}

IndexWorkerPtr IndexWorkerMgr::createWorker(const char* factname, const char* unitname, const char* id)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return IndexWorkerPtr();

	IdxFactInfo& fInfo = (IdxFactInfo&)it->second;
	IndexWorkerPtr ret(new IdxWorkerWrapper(fInfo._fact->create_worker(unitname, id), fInfo._fact));
	_workers[id] = ret;
	return ret;
}

IndexWorkerPtr IndexWorkerMgr::createWorker(const char* name, const char* id)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return IndexWorkerPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return IndexWorkerPtr();

	IdxFactInfo& fInfo = (IdxFactInfo&)it->second;
	IndexWorkerPtr ret(new IdxWorkerWrapper(fInfo._fact->create_worker(unitname, id), fInfo._fact));
	_workers[id] = ret;
	return ret;
}

IndexWorkerPtr IndexWorkerMgr::getWorker(const char* id)
{
	auto it = _workers.find(id);
	if (it == _workers.end())
		return IndexWorkerPtr();

	return it->second;
}

