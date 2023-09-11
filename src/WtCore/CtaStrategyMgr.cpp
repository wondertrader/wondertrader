/*!
 * \file CtaStrategyMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaStrategyMgr.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSLogger.h"

#include <boost/filesystem.hpp>


CtaStrategyMgr::CtaStrategyMgr()
{
}


CtaStrategyMgr::~CtaStrategyMgr()
{
}

bool CtaStrategyMgr::loadFactories(const char* path)
{
	if (!StdFile::exists(path))
	{
		WTSLogger::error("Directory {} of CTA strategy factory not exists", path);
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

		FuncCreateStraFact creator = (FuncCreateStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
		if (creator == NULL)
		{
			DLLHelper::free_library(hInst);
			continue;
		}

		ICtaStrategyFact* fact = creator();
		if(fact != NULL)
		{
			StraFactInfo& fInfo = _factories[fact->getName()];
			fInfo._module_inst = hInst;
			fInfo._module_path = iter->path().string();
			fInfo._creator = creator;
			fInfo._remover = (FuncDeleteStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
			fInfo._fact = fact;

			WTSLogger::info("CTA strategy factory[{}] loaded", fact->getName());

			count++;
		}
		else
		{
			DLLHelper::free_library(hInst);
			continue;
		}
		
	}

	WTSLogger::info("{} CTA strategy factories in directory[{}] loaded", count, path);

	return true;
}

CtaStrategyPtr CtaStrategyMgr::createStrategy(const char* factname, const char* unitname, const char* id)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return CtaStrategyPtr();

	StraFactInfo& fInfo = (StraFactInfo&)it->second;
	CtaStrategyPtr ret(new CtaStraWrapper(fInfo._fact->createStrategy(unitname, id), fInfo._fact));
	_strategies[id] = ret;
	return ret;
}

CtaStrategyPtr CtaStrategyMgr::createStrategy(const char* name, const char* id)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return CtaStrategyPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return CtaStrategyPtr();

	StraFactInfo& fInfo = (StraFactInfo&)it->second;
	CtaStrategyPtr ret(new CtaStraWrapper(fInfo._fact->createStrategy(unitname, id), fInfo._fact));
	_strategies[id] = ret;
	return ret;
}

CtaStrategyPtr CtaStrategyMgr::getStrategy(const char* id)
{
	auto it = _strategies.find(id);
	if (it == _strategies.end())
		return CtaStrategyPtr();

	return it->second;
}

