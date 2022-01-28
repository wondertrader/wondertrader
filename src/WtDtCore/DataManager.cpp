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

#include "../Includes/WTSVariant.hpp"
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

bool DataManager::init(WTSVariant* params, WTSBaseDataMgr* bdMgr, StateMonitor* stMonitor, UDPCaster* caster /* = NULL */)
{
	_bd_mgr = bdMgr;
	_state_mon = stMonitor;
	_udp_caster = caster;

	std::string module = params->getCString("module");
	if (module.empty())
		module = WtHelper::get_module_dir() + DLLHelper::wrap_module("WtDataStorage");
	else
		module = WtHelper::get_module_dir() + DLLHelper::wrap_module(module.c_str());
	
	DllHandle libParser = DLLHelper::load_library(module.c_str());
	if (libParser)
	{
		FuncCreateWriter pFuncCreateWriter = (FuncCreateWriter)DLLHelper::get_symbol(libParser, "createWriter");
		if (pFuncCreateWriter == NULL)
		{
			WTSLogger::error("Initializing of data writer failed: function createWriter not found...");
		}

		FuncDeleteWriter pFuncDeleteWriter = (FuncDeleteWriter)DLLHelper::get_symbol(libParser, "deleteWriter");
		if (pFuncDeleteWriter == NULL)
		{
			WTSLogger::error("Initializing of data writer failed: function deleteWriter not found...");
		}

		if (pFuncCreateWriter && pFuncDeleteWriter)
		{
			_writer = pFuncCreateWriter();
			_remover = pFuncDeleteWriter;
		}
		WTSLogger::info_f("Data storage module {} loaded", module);
	}
	else
	{
		WTSLogger::error("Initializing of data writer failed: loading module %s failed...", module.c_str());

	}

	return _writer->init(params, this);
}

void DataManager::add_ext_dumper(const char* id, IHisDataDumper* dumper)
{
	if (_writer == NULL)
		return;

	_writer->add_ext_dumper(id, dumper);
}

void DataManager::release()
{
	if (_writer)
	{
		_writer->release();
		_remover(_writer);
	}
}

bool DataManager::writeTick(WTSTickData* curTick, uint32_t procFlag)
{
	if (_writer == NULL)
		return false;

	return _writer->writeTick(curTick, procFlag);
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
	//By Wesley @ 2021.12.27
	//如果状态机为NULL，说明是全天候模式，直接返回true即可
	if (_state_mon == NULL)
		return true;

	return _state_mon->isInState(sid, SS_RECEIVING);
}

void DataManager::broadcastTick(WTSTickData* curTick)
{
	if (_udp_caster)
		_udp_caster->broadcast(curTick);
}

void DataManager::broadcastOrdDtl(WTSOrdDtlData* curOrdDtl)
{
	if (_udp_caster)
		_udp_caster->broadcast(curOrdDtl);
}

void DataManager::broadcastOrdQue(WTSOrdQueData* curOrdQue)
{
	if (_udp_caster)
		_udp_caster->broadcast(curOrdQue);
}

void DataManager::broadcastTrans(WTSTransData* curTrans)
{
	if (_udp_caster)
		_udp_caster->broadcast(curTrans);
}

CodeSet* DataManager::getSessionComms(const char* sid)
{
	return  _bd_mgr->getSessionComms(sid);
}

uint32_t DataManager::getTradingDate(const char* pid)
{
	return  _bd_mgr->getTradingDate(pid);
}

void DataManager::outputLog(WTSLogLevel ll, const char* message)
{
	WTSLogger::log_raw(ll, message);
}

#pragma endregion "IDataWriterSink"
