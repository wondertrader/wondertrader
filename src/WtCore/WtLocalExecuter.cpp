/*!
 * \file WtExecuter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtLocalExecuter.h"
#include "TraderAdapter.h"
#include "WtEngine.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/IDataManager.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IHotMgr.h"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;


WtLocalExecuter::WtLocalExecuter(WtExecuterFactory* factory, const char* name, IDataManager* dataMgr)
	: IExecCommand(name)
	, _factory(factory)
	, _data_mgr(dataMgr)
	, _channel_ready(false)
	, _scale(1.0)
	, _auto_clear(true)
{
}


WtLocalExecuter::~WtLocalExecuter()
{
	if (_pool)
		_pool->wait();
}

bool WtLocalExecuter::init(WTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getDouble("scale");
	uint32_t poolsize = params->getUInt32("poolsize");
	if(poolsize > 0)
	{
		_pool.reset(new boost::threadpool::pool(poolsize));
	}

	/*
	 *	By Wesley @ 2021.12.14
	 *	从配置文件中读取自动清理的策略
	 *	active: 是否启用
	 *	includes: 包含列表，格式如CFFEX.IF
	 *	excludes: 排除列表，格式如CFFEX.IF
	 */
	WTSVariant* cfgClear = params->get("clear");
	if(cfgClear)
	{
		_auto_clear = cfgClear->getBoolean("active");
		WTSVariant* cfgItem = cfgClear->get("includes");
		if(cfgItem)
		{
			if (cfgItem->type() == WTSVariant::VT_String)
				_clear_includes.insert(cfgItem->asCString());
			else if (cfgItem->type() == WTSVariant::VT_Array)
			{
				for(uint32_t i = 0; i < cfgItem->size(); i++)
					_clear_includes.insert(cfgItem->get(i)->asCString());
			}
		}

		cfgItem = cfgClear->get("excludes");
		if (cfgItem)
		{
			if (cfgItem->type() == WTSVariant::VT_String)
				_clear_excludes.insert(cfgItem->asCString());
			else if (cfgItem->type() == WTSVariant::VT_Array)
			{
				for (uint32_t i = 0; i < cfgItem->size(); i++)
					_clear_excludes.insert(cfgItem->get(i)->asCString());
			}
		}
	}

	writeLog(fmt::format("Local executer inited, scale: {}, auto_clear: {}, thread poolsize: {}", _scale, _auto_clear, poolsize).c_str());

	return true;
}

ExecuteUnitPtr WtLocalExecuter::getUnit(const char* stdCode, bool bAutoCreate /* = true */)
{
	std::string commID = CodeHelper::stdCodeToStdCommID(stdCode);

	WTSVariant* policy = _config->get("policy");
	std::string des = commID;
	if (!policy->has(commID.c_str()))
		des = "default";

	auto it = _unit_map.find(stdCode);
	if(it != _unit_map.end())
	{
		return it->second;
	}

	if (bAutoCreate)
	{
		WTSVariant* cfg = policy->get(des.c_str());

		const char* name = cfg->getCString("name");
		ExecuteUnitPtr unit = _factory->createExeUnit(name);
		if (unit != NULL)
		{
			_unit_map[stdCode] = unit;
			unit->self()->init(this, stdCode, cfg);
		}
		return unit;
	}
	else
	{
		return ExecuteUnitPtr();
	}
}


//////////////////////////////////////////////////////////////////////////
//ExecuteContext
#pragma region Context回调接口
WTSTickSlice* WtLocalExecuter::getTicks(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->get_tick_slice(stdCode, count);
}

WTSTickData* WtLocalExecuter::grabLastTick(const char* stdCode)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->grab_last_tick(stdCode);
}

double WtLocalExecuter::getPosition(const char* stdCode, int32_t flag /* = 3 */)
{
	return _trader->getPosition(stdCode, flag);
}

double WtLocalExecuter::getUndoneQty(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

OrderMap* WtLocalExecuter::getOrders(const char* stdCode)
{
	return _trader->getOrders(stdCode);
}

OrderIDs WtLocalExecuter::buy(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();
	return _trader->buy(stdCode, price, qty, bForceClose);
}

OrderIDs WtLocalExecuter::sell(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->sell(stdCode, price, qty, bForceClose);
}

bool WtLocalExecuter::cancel(uint32_t localid)
{
	if (!_channel_ready)
		return false;

	return _trader->cancel(localid);
}

OrderIDs WtLocalExecuter::cancel(const char* stdCode, bool isBuy, double qty)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->cancel(stdCode, isBuy, qty);
}

void WtLocalExecuter::writeLog(const char* fmt, ...)
{
	char szBuf[2048] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _name.c_str());
	strcat(szBuf, fmt);
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog_dyn("executer", _name.c_str(), LL_INFO, szBuf, args);
	va_end(args);
}

WTSCommodityInfo* WtLocalExecuter::getCommodityInfo(const char* stdCode)
{
	return _stub->get_comm_info(stdCode);
}

WTSSessionInfo* WtLocalExecuter::getSessionInfo(const char* stdCode)
{
	return _stub->get_sess_info(stdCode);
}

uint64_t WtLocalExecuter::getCurTime()
{
	return _stub->get_real_time();
	//return TimeUtils::makeTime(_stub->get_date(), _stub->get_raw_time() * 100000 + _stub->get_secs());
}

#pragma endregion Context回调接口
//ExecuteContext
//////////////////////////////////////////////////////////////////////////


#pragma region 外部接口
void WtLocalExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	ExecuteUnitPtr unit = getUnit(stdCode);
	if (unit == NULL)
		return;

	targetPos = round(targetPos*_scale);

	double oldVol = _target_pos[stdCode];
	//int32_t targetPos = oldVol + diffQty;
	_target_pos[stdCode] = targetPos;

	if(!decimal::eq(oldVol, targetPos))
	{
		writeLog(fmt::format("Target position of {} changed: {} -> {}", stdCode, oldVol, targetPos).c_str());
	}

	if (_trader && !_trader->checkOrderLimits(stdCode))
	{
		writeLog("%s is disabled", stdCode);
		return;
	}

	unit->self()->set_position(stdCode, targetPos);
}

void WtLocalExecuter::set_position(const faster_hashmap<std::string, double>& targets)
{
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();		
		double newVol = it->second;
		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;

		newVol = round(newVol*_scale);
		double oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		if(!decimal::eq(oldVol, newVol))
		{
			writeLog(fmt::format("Target position of {} changed: {} -> {}", stdCode, oldVol, newVol).c_str());
		}

		if (_trader && !_trader->checkOrderLimits(stdCode))
		{
			writeLog("%s is disabled", stdCode);
			continue;
		}

		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code, newVol](){
				unit->self()->set_position(code.c_str(), newVol);
			});
		}
		else
		{
			unit->self()->set_position(stdCode, newVol);
		}
	}

	for (auto it = _target_pos.begin(); it != _target_pos.end(); it++)
	{
		const char* code = it->first.c_str();
		double& pos = (double&)it->second;
		auto tit = targets.find(code);
		if(tit != targets.end())
			continue;

		ExecuteUnitPtr unit = getUnit(code);
		if (unit == NULL)
			continue;

		//unit->self()->set_position(code, 0);
		if (_pool)
		{
			_pool->schedule([unit, code](){
				unit->self()->set_position(code, 0);
			});
		}
		else
		{
			unit->self()->set_position(code, 0);
		}

		pos = 0;
	}
}

void WtLocalExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	//unit->self()->on_tick(newTick);
	if (_pool)
	{
		newTick->retain();
		_pool->schedule([unit, newTick](){
			unit->self()->on_tick(newTick);
			newTick->release();
		});
	}
	else
	{
		unit->self()->on_tick(newTick);
	}
}

void WtLocalExecuter::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	//unit->self()->on_trade(stdCode, isBuy, vol, price);
	if (_pool)
	{
		std::string code = stdCode;
		_pool->schedule([localid, unit, code, isBuy, vol, price](){
			unit->self()->on_trade(localid, code.c_str(), isBuy, vol, price);
		});
	}
	else
	{
		unit->self()->on_trade(localid, stdCode, isBuy, vol, price);
	}
}

void WtLocalExecuter::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	//unit->self()->on_order(localid, stdCode, isBuy, leftQty, price, isCanceled);
	if (_pool)
	{
		std::string code = stdCode;
		_pool->schedule([localid, unit, code, isBuy, leftQty, price, isCanceled](){
			unit->self()->on_order(localid, code.c_str(), isBuy, leftQty, price, isCanceled);
		});
	}
	else
	{
		unit->self()->on_order(localid, stdCode, isBuy, leftQty, price, isCanceled);
	}
}

void WtLocalExecuter::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	//unit->self()->on_entrust(localid, stdCode, bSuccess, message);
	if (_pool)
	{
		std::string code = stdCode;
		std::string msg = message;
		_pool->schedule([unit, localid, code, bSuccess, msg](){
			unit->self()->on_entrust(localid, code.c_str(), bSuccess, msg.c_str());
		});
	}
	else
	{
		unit->self()->on_entrust(localid, stdCode, bSuccess, message);
	}
}

void WtLocalExecuter::on_channel_ready()
{
	_channel_ready = true;
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			//unitPtr->self()->on_channel_ready();
			if (_pool)
			{
				_pool->schedule([unitPtr](){
					unitPtr->self()->on_channel_ready();
				});
			}
			else
			{
				unitPtr->self()->on_channel_ready();
			}
		}
	}
}

void WtLocalExecuter::on_channel_lost()
{
	_channel_ready = false;
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			//unitPtr->self()->on_channel_lost();
			if (_pool)
			{
				_pool->schedule([unitPtr](){
					unitPtr->self()->on_channel_lost();
				});
			}
			else
			{
				unitPtr->self()->on_channel_lost();
			}
		}
	}
}

void WtLocalExecuter::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	/*
	 *	By Wesley @ 2021.12.14
	 *	先检查自动清理过期主力合约的标记是否为true
	 *	如果不为true，则直接退出该逻辑
	 */
	if (!_auto_clear)
		return;

	//如果不是期货合约，直接退出
	if (!CodeHelper::isStdFutCode(stdCode))
		return;

	IHotMgr* hotMgr = _stub->get_hot_mon();
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdFutCode(stdCode);
	//获取上一期的主力合约
	std::string prevCode = hotMgr->getPrevRawCode(cInfo._exchg, cInfo._product, tradingday);

	//如果当前合约不是上一期的主力合约，则直接退出
	if (prevCode != cInfo._code)
		return;

	writeLog("Prev hot contract of %s.%s on %u is %s", cInfo._exchg, cInfo._product, tradingday, prevCode.c_str());

	std::string fullPid = StrUtil::printf("%s.%s", cInfo._exchg, cInfo._product);

	//先检查排除列表
	//如果在排除列表中，则直接退出
	auto it = _clear_excludes.find(fullPid);
	if(it != _clear_excludes.end())
	{
		writeLog("Position of %s, as prev hot contract, won't be cleared for it's in exclude list", stdCode);
		return;
	}

	//如果包含列表不为空，再检查是否在包含列表中
	//如果为空，则全部清理，不再进入该逻辑
	if(!_clear_includes.empty())
	{
		it = _clear_includes.find(fullPid);
		if (it == _clear_includes.end())
		{
			writeLog("Position of %s, as prev hot contract, won't be cleared for it's not in include list", stdCode);
			return;
		}
	}

	//最后再进行自动清理
	writeLog("Position of %s, as prev hot contract, will be cleared", stdCode);
	ExecuteUnitPtr unit = getUnit(stdCode);
	if (unit)
	{
		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code](){
				unit->self()->clear_all_position(code.c_str());
			});
		}
		else
		{
			unit->self()->clear_all_position(stdCode);
		}
	}
}

#pragma endregion 外部接口


//////////////////////////////////////////////////////////////////////////
//WtExecuterFactory
bool WtExecuterFactory::loadFactories(const char* path)
{
	if (!StdFile::exists(path))
	{
		WTSLogger::error("Directory %s of executer factory not exists", path);
		return false;
	}

	boost::filesystem::path myPath(path);
	boost::filesystem::directory_iterator endIter;
	for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
	{
		if (boost::filesystem::is_directory(iter->path()))
			continue;

#ifdef _WIN32
		if (iter->path().extension() != ".dll")
			continue;
#else //_UNIX
		if (iter->path().extension() != ".so")
			continue;
#endif

		const std::string& path = iter->path().string();

		DllHandle hInst = DLLHelper::load_library(path.c_str());
		if (hInst == NULL)
		{
			continue;
		}

		FuncCreateExeFact creator = (FuncCreateExeFact)DLLHelper::get_symbol(hInst, "createExecFact");
		if (creator == NULL)
		{
			DLLHelper::free_library(hInst);
			continue;
		}

		ExeFactInfo fInfo;
		fInfo._module_inst = hInst;
		fInfo._module_path = iter->path().string();
		fInfo._creator = creator;
		fInfo._remover = (FuncDeleteExeFact)DLLHelper::get_symbol(hInst, "deleteExecFact");
		fInfo._fact = fInfo._creator();

		_factories[fInfo._fact->getName()] = fInfo;

		WTSLogger::info("Executer factory %s loaded", fInfo._fact->getName());
	}

	return true;
}

ExecuteUnitPtr WtExecuterFactory::createExeUnit(const char* factname, const char* unitname)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if(unit == NULL)
	{
		WTSLogger::error("Createing execution unit failed: %s.%s", factname, unitname);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}

ExecuteUnitPtr WtExecuterFactory::createExeUnit(const char* name)
{
	StringVector ay = StrUtil::split(name, ".");
	if (ay.size() < 2)
		return ExecuteUnitPtr();

	const char* factname = ay[0].c_str();
	const char* unitname = ay[1].c_str();

	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = (ExeFactInfo&)it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if (unit == NULL)
	{
		WTSLogger::error("Createing execution unit failed: %s", name);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}
