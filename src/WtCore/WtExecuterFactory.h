#pragma once
#include "IExecCommand.h"
#include "../Includes/ExecuteDefs.h"
#include "../Share/DLLHelper.hpp"

#include <boost/core/noncopyable.hpp>

NS_WTP_BEGIN

//////////////////////////////////////////////////////////////////////////
//执行单元封装
//因为执行单元是dll里创建的, 如果不封装的话, 直接delete可能会有问题
//所以要把工厂指针一起封装到这里, 直接调用工厂实例的deleteUnit方法释放执行单元
class ExeUnitWrapper
{
public:
	ExeUnitWrapper(ExecuteUnit* unitPtr, IExecuterFact* fact) :_unit(unitPtr), _fact(fact) {}
	~ExeUnitWrapper()
	{
		if (_unit)
		{
			_fact->deleteExeUnit(_unit);
		}
	}

	ExecuteUnit* self() { return _unit; }


private:
	ExecuteUnit*	_unit;
	IExecuterFact*	_fact;
};

typedef std::shared_ptr<ExeUnitWrapper>	ExecuteUnitPtr;
typedef wt_hashmap<std::string, ExecuteUnitPtr> ExecuteUnitMap;

//////////////////////////////////////////////////////////////////////////
//执行器工厂类
class WtExecuterFactory : private boost::noncopyable
{
public:
	~WtExecuterFactory() {}

public:
	bool loadFactories(const char* path);

	ExecuteUnitPtr createExeUnit(const char* name);
	ExecuteUnitPtr createDiffExeUnit(const char* name);
	ExecuteUnitPtr createArbiExeUnit(const char* name);

	ExecuteUnitPtr createExeUnit(const char* factname, const char* unitname);
	ExecuteUnitPtr createDiffExeUnit(const char* factname, const char* unitname);
	ExecuteUnitPtr createArbiExeUnit(const char* factname, const char* unitname);

private:
	typedef struct _ExeFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IExecuterFact*	_fact;
		FuncCreateExeFact	_creator;
		FuncDeleteExeFact	_remover;
	} ExeFactInfo;
	typedef wt_hashmap<std::string, ExeFactInfo> ExeFactMap;

	ExeFactMap	_factories;
};


NS_WTP_END
