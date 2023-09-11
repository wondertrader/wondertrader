/*!
* \file MfStrategyMgr.h
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
#include "../Includes/SelStrategyDefs.h"

#include "../Share/DLLHelper.hpp"

class SelStraWrapper
{
public:
	SelStraWrapper(SelStrategy* stra, ISelStrategyFact* fact) :_stra(stra), _fact(fact){}
	~SelStraWrapper()
	{
		if (_stra)
		{
			_fact->deleteStrategy(_stra);
		}
	}

	SelStrategy* self(){ return _stra; }


private:
	SelStrategy*		_stra;
	ISelStrategyFact*	_fact;
};
typedef std::shared_ptr<SelStraWrapper>	SelStrategyPtr;


class SelStrategyMgr : private boost::noncopyable
{
public:
	SelStrategyMgr();
	~SelStrategyMgr();

public:
	bool loadFactories(const char* path);

	SelStrategyPtr createStrategy(const char* name, const char* id);
	SelStrategyPtr createStrategy(const char* factname, const char* unitname, const char* id);

	SelStrategyPtr getStrategy(const char* id);
private:
	typedef struct _StraFactInfo
	{
		std::string			_module_path;
		DllHandle			_module_inst;
		ISelStrategyFact*	_fact;
		FuncCreateSelStraFact	_creator;
		FuncDeleteSelStraFact	_remover;
	} StraFactInfo;
	typedef wt_hashmap<std::string, StraFactInfo> StraFactMap;
	StraFactMap	_factories;

	typedef wt_hashmap<std::string, SelStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

