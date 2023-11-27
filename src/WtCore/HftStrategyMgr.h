/*!
 * \file HftStrategyMgr.h
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
#include "../Includes/HftStrategyDefs.h"

#include "../Share/DLLHelper.hpp"

class HftStraWrapper
{
public:
	HftStraWrapper(HftStrategy* stra, IHftStrategyFact* fact) :_stra(stra), _fact(fact){}
	~HftStraWrapper()
	{
		if (_stra)
		{
			_fact->deleteStrategy(_stra);
		}
	}

	HftStrategy* self(){ return _stra; }


private:
	HftStrategy*		_stra;
	IHftStrategyFact*	_fact;
};

typedef std::shared_ptr<HftStraWrapper>	HftStrategyPtr;

class HftStrategyMgr : private boost::noncopyable
{
public:
	HftStrategyMgr();
	~HftStrategyMgr();

public:
	bool loadFactories(const char* path);

	HftStrategyPtr createStrategy(const char* name, const char* id);
	HftStrategyPtr createStrategy(const char* factname, const char* unitname, const char* id);

	HftStrategyPtr getStrategy(const char* id);

private:
	typedef struct _StraFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IHftStrategyFact*	_fact;
		FuncCreateHftStraFact	_creator;
		FuncDeleteHftStraFact	_remover;

		_StraFactInfo()
		{
			_module_inst = NULL;
			_fact = NULL;
		}

		~_StraFactInfo()
		{
			if (_fact)
				_remover(_fact);
		}
	} StraFactInfo;
	typedef wt_hashmap<std::string, StraFactInfo> StraFactMap;

	StraFactMap	_factories;

	typedef wt_hashmap<std::string, HftStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

