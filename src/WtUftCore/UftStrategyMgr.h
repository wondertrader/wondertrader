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
#include "../Includes/UftStrategyDefs.h"

#include "../Share/DLLHelper.hpp"

class UftStraWrapper
{
public:
	UftStraWrapper(UftStrategy* stra, IUftStrategyFact* fact) :_stra(stra), _fact(fact){}
	~UftStraWrapper()
	{
		if (_stra)
		{
			_fact->deleteStrategy(_stra);
		}
	}

	UftStrategy* self(){ return _stra; }


private:
	UftStrategy*		_stra;
	IUftStrategyFact*	_fact;
};

typedef std::shared_ptr<UftStraWrapper>	UftStrategyPtr;

class UftStrategyMgr : private boost::noncopyable
{
public:
	UftStrategyMgr();
	~UftStrategyMgr();

public:
	bool loadFactories(const char* path);

	UftStrategyPtr createStrategy(const char* name, const char* id);
	UftStrategyPtr createStrategy(const char* factname, const char* unitname, const char* id);

	UftStrategyPtr getStrategy(const char* id);

private:
	typedef struct _StraFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IUftStrategyFact*	_fact;
		FuncCreateUftStraFact	_creator;
		FuncDeleteUftStraFact	_remover;

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

	typedef wt_hashmap<std::string, UftStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

