#pragma once
#include "IExecCommand.h"
#include "../Includes/ExecuteDefs.h"
#include "../Share/DLLHelper.hpp"

#include <boost/core/noncopyable.hpp>

NS_WTP_BEGIN

//////////////////////////////////////////////////////////////////////////
//ִ�е�Ԫ��װ
//��Ϊִ�е�Ԫ��dll�ﴴ����, �������װ�Ļ�, ֱ��delete���ܻ�������
//����Ҫ�ѹ���ָ��һ���װ������, ֱ�ӵ��ù���ʵ����deleteUnit�����ͷ�ִ�е�Ԫ
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
typedef faster_hashmap<LongKey, ExecuteUnitPtr> ExecuteUnitMap;

//////////////////////////////////////////////////////////////////////////
//ִ����������
class WtExecuterFactory : private boost::noncopyable
{
public:
	~WtExecuterFactory() {}

public:
	bool loadFactories(const char* path);

	ExecuteUnitPtr createExeUnit(const char* name);
	ExecuteUnitPtr createDiffExeUnit(const char* name);

	ExecuteUnitPtr createExeUnit(const char* factname, const char* unitname);
	ExecuteUnitPtr createDiffExeUnit(const char* factname, const char* unitname);

private:
	typedef struct _ExeFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IExecuterFact*	_fact;
		FuncCreateExeFact	_creator;
		FuncDeleteExeFact	_remover;
	} ExeFactInfo;
	typedef faster_hashmap<LongKey, ExeFactInfo> ExeFactMap;

	ExeFactMap	_factories;
};


NS_WTP_END
