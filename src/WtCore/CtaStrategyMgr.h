/*!
 * \file CtaStrategyMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <memory>
#include <boost/core/noncopyable.hpp>

#include "../Includes/FasterDefs.h"
#include "../Includes/CtaStrategyDefs.h"

#include "../Share/DLLHelper.hpp"

class CtaStraWrapper
{
public:
	CtaStraWrapper(CtaStrategy* stra, ICtaStrategyFact* fact) :_stra(stra), _fact(fact){}
	~CtaStraWrapper()
	{
		if (_stra)
		{
			_fact->deleteStrategy(_stra);
		}
	}

	CtaStrategy* self(){ return _stra; }


private:
	CtaStrategy*		_stra;
	ICtaStrategyFact*	_fact;
};
typedef std::shared_ptr<CtaStraWrapper>	CtaStrategyPtr;


class CtaStrategyMgr : private boost::noncopyable
{
public:
	CtaStrategyMgr();
	~CtaStrategyMgr();

public:
	bool loadFactories(const char* path);

	CtaStrategyPtr createStrategy(const char* name, const char* id);
	CtaStrategyPtr createStrategy(const char* factname, const char* unitname, const char* id);

	CtaStrategyPtr getStrategy(const char* id);
private:
	typedef struct _StraFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		ICtaStrategyFact*	_fact;
		FuncCreateStraFact	_creator;
		FuncDeleteStraFact	_remover;
	} StraFactInfo;
	typedef wt_hashmap<std::string, StraFactInfo> StraFactMap;

	StraFactMap	_factories;

	typedef wt_hashmap<std::string, CtaStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

