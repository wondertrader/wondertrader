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

		// wt_hashmap (robin_map) relocates entries on insert/rehash: the
		// moved-from entry must not call _remover, or the factory registered
		// first would be unloaded when a second factory is registered,
		// leaving a dangling pointer that crashes on the next createStrategy.
		_StraFactInfo(_StraFactInfo&& other) noexcept
			: _module_path(std::move(other._module_path))
			, _module_inst(other._module_inst)
			, _fact(other._fact)
			, _creator(other._creator)
			, _remover(other._remover)
		{
			other._module_inst = NULL;
			other._fact = NULL;
			other._creator = NULL;
			other._remover = NULL;
		}

		_StraFactInfo& operator=(_StraFactInfo&& other) noexcept
		{
			if (this != &other)
			{
				_module_path = std::move(other._module_path);
				_module_inst = other._module_inst;
				_fact = other._fact;
				_creator = other._creator;
				_remover = other._remover;
				other._module_inst = NULL;
				other._fact = NULL;
				other._creator = NULL;
				other._remover = NULL;
			}
			return *this;
		}

		_StraFactInfo(const _StraFactInfo&) = delete;
		_StraFactInfo& operator=(const _StraFactInfo&) = delete;
	} StraFactInfo;
	typedef wt_hashmap<std::string, StraFactInfo> StraFactMap;

	StraFactMap	_factories;

	typedef wt_hashmap<std::string, HftStrategyPtr> StrategyMap;
	StrategyMap	_strategies;
};

