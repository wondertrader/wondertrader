/*!
 * \file DataManager.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "DataManager.h"
#include "StateMonitor.h"
#include "UDPCaster.h"
#include "WtHelper.h"

#include "../Share/WTSVariant.hpp"
#include "../Share/DLLHelper.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"


DataManager::DataManager()
	: _writer(NULL)
	, _bd_mgr(NULL)
	, _state_mon(NULL)
	, _udp_caster(NULL)
{
}


DataManager::~DataManager()
{
}

bool DataManager::isSessionProceeded(const char* sid)
{
	if (_writer == NULL)
		return false;

	return _writer->isSessionProceeded(sid);
}

bool DataManager::init(WTSVariant* params, WTSBaseDataMgr* bdMgr, StateMonitor* stMonitor, UDPCaster* caster)
{
	_bd_mgr = bdMgr;
	_state_mon = stMonitor;
	_udp_caster = caster;

	std::string module = params->getCString("module");
	if (module.empty())
	{
		module = WtHelper::get_module_dir();
#ifdef _WIN32
		module += "WtDataWriter.dll";
#else
		module += "libWtDataWriter.so";
#endif
	}
	
	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateWriter pFuncCreateWriter = (FuncCreateWriter)DLLHelper::get_symbol(libParser, "createWriter");
		if (pFuncCreateWriter == NULL)
		{
			WTSLogger::info("数据落地模块初始化失败,找不到对应的入口函数...");
		}

		FuncDeleteWriter pFuncDeleteWriter = (FuncDeleteWriter)DLLHelper::get_symbol(libParser, "deleteWriter");
		if (pFuncDeleteWriter == NULL)
		{
			WTSLogger::info("数据落地模块初始化失败,找不到对应的入口函数...");
		}

		if (pFuncCreateWriter && pFuncDeleteWriter)
		{
			_writer = pFuncCreateWriter();
			_remover = pFuncDeleteWriter;
		}

	}
	else
	{
		WTSLogger::info("数据落地模块初始化失败,加载模块%s失败...",  module.c_str());
	}

	return _writer->init(params, this);
}

void DataManager::release()
{
	if (_writer)
	{
		_writer->release();
		_remover(_writer);
	}
}

bool DataManager::writeTick(WTSTickData* curTick, bool bNeedSlice /* = true */)
{
	if (_writer == NULL)
		return false;

	return _writer->writeTick(curTick, bNeedSlice);
}

bool DataManager::writeOrderQueue(WTSOrdQueData* curOrdQue)
{
	if (_writer == NULL)
		return false;

	return _writer->writeOrderQueue(curOrdQue);
}

bool DataManager::writeOrderDetail(WTSOrdDtlData* curOrdDtl)
{
	if (_writer == NULL)
		return false;

	return _writer->writeOrderDetail(curOrdDtl);
}

bool DataManager::writeTransaction(WTSTransData* curTrans)
{
	if (_writer == NULL)
		return false;

	return _writer->writeTransaction(curTrans);
}

WTSTickData* DataManager::getCurTick(const char* code, const char* exchg/* = ""*/)
{
	if (_writer == NULL)
		return NULL;

	return _writer->getCurTick(code, exchg);
}

void DataManager::transHisData(const char* sid)
{
	if (_writer)
		_writer->transHisData(sid);
}

//////////////////////////////////////////////////////////////////////////
#pragma region "IDataWriterSink"
IBaseDataMgr* DataManager::getBDMgr()
{
	return _bd_mgr;
}

bool DataManager::canSessionReceive(const char* sid)
{
	return _state_mon->isInState(sid, SS_RECEIVING);
}

void DataManager::broadcastTick(WTSTickData* curTick)
{
	if (_udp_caster)
		_udp_caster->broadcast(curTick);
}

CodeSet* DataManager::getSessionComms(const char* sid)
{
	return  _bd_mgr->getSessionComms(sid);
}

uint32_t DataManager::getTradingDate(const char* pid)
{
	return  _bd_mgr->getTradingDate(pid);
}

void DataManager::outputWriterLog(WTSLogLevel ll, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	WTSLogger::log_direct(ll, format, args);
	va_end(args);
}

#pragma endregion "IDataWriterSink"
