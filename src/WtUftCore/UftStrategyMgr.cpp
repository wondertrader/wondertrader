/*!
 * \file UftStrategyMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UftStrategyMgr.h"

#include <boost/filesystem.hpp>

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"


UftStrategyMgr::UftStrategyMgr()
{
}


UftStrategyMgr::~UftStrategyMgr()
{
}

bool UftStrategyMgr::loadFactories(const char* path)
{
	if (!StdFile::exists(path))
	{
		WTSLogger::error("Directory %s of UFT strategy factory not exists", path);
		return false;
	}

	uint32_t count = 0;
	boost::filesystem::path myPath(path);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
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

		FuncCreateUftStraFact creator = (FuncCreateUftStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
		if (creator == NULL)
		{
			DLLHelper::free_library(hInst);
			continue;
		}
		
		IUftStrategyFact* pFact = creator();
		if (pFact != NULL)
		{
			StraFactInfo& fInfo = _factories[pFact->getName()];

			fInfo._module_inst = hInst;
			fInfo._module_path = iter->path().string();
			fInfo._creator = creator;
			fInfo._remover = (FuncDeleteUftStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
			fInfo._fact = pFact;
			WTSLogger::info("UFT strategy factory[%s] loaded", pFact->getName());

			count++;
		}
		else
		{
			DLLHelper::free_library(hInst);
			continue;
		}
	}

	WTSLogger::info("%u UFT strategy factories in directory[%s] loaded", count, path);

	return true;
}

UftStrategyPtr UftStrategyMgr::createStrategy(const char* factname, const char* unitname, const char* id)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return UftStrategyPtr();

	StraFactInfo& fInfo = (StraFactInfo&)it->second;
	UftStrategyPtr ret(new UftStraWrapper(fInfo._fact->createStrategy(unitname, id), fInfo._fact));
	_strategies[id] = ret;
	return ret;
}

UftStrategyPtr UftStrategyMgr::createStrategy(const char* name, const char* id)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return UftStrategyPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return UftStrategyPtr();

	StraFactInfo& fInfo = (StraFactInfo&)it->second;
	UftStrategyPtr ret(new UftStraWrapper(fInfo._fact->createStrategy(unitname, id), fInfo._fact));
	_strategies[id] = ret;
	return ret;
}

UftStrategyPtr UftStrategyMgr::getStrategy(const char* id)
{
	auto it = _strategies.find(id);
	if (it == _strategies.end())
		return UftStrategyPtr();

	return it->second;
}