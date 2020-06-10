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
#include <unordered_map>
#include <boost/core/noncopyable.hpp>

#include "MfStrategyDefs.h"

#include "../Share/BoostDefine.h"
#include "../Share/DLLHelper.hpp"

class MfStraWrapper
{
public:
	MfStraWrapper(MfStrategy* stra, IMfStrategyFact* fact) :_stra(stra), _fact(fact){}
	~MfStraWrapper()
	{
		if (_stra)
		{
			_fact->deleteStrategy(_stra);
		}
	}

	MfStrategy* self(){ return _stra; }


private:
	MfStrategy*		_stra;
	IMfStrategyFact*	_fact;
};
typedef std::shared_ptr<MfStraWrapper>	MfStrategyPtr;


class MfStrategyMgr : private boost::noncopyable
{
public:
	MfStrategyMgr();
	~MfStrategyMgr();

public:
	bool loadFactories(const char* path);

	MfStrategyPtr createStrategy(const char* name, const char* id);
	MfStrategyPtr createStrategy(const char* factname, const char* unitname, const char* id);

	MfStrategyPtr getStrategy(const char* id);
private:
	typedef struct _StraFactInfo
	{
		std::string			_module_path;
		DllHandle			_module_inst;
		IMfStrategyFact*	_fact;
		FuncCreateMfStraFact	_creator;
		FuncDeleteMfStraFact	_remover;
	} StraFactInfo;
	typedef std::unordered_map<std::string, StraFactInfo> StraFactMap;

	StraFactMap	_factories;

	typedef std::unordered_map<std::string, MfStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

