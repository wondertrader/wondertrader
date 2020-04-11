/*!
 * \file WtExecuter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtExecuter.h"
#include "TraderAdapter.h"
#include "WtDataManager.h"
#include "WtEngine.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/WTSVariant.hpp"
#include "../Share/BoostFile.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/IHotMgr.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_OTP;


WtExecuter::WtExecuter(WtExecuterFactory* factory, const char* name, WtDataManager* dataMgr)
	: _factory(factory)
	, _name(name)
	, _data_mgr(dataMgr)
	, _channel_ready(false)
	, _engine(NULL)
{
}


WtExecuter::~WtExecuter()
{
}

bool WtExecuter::init(WTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getUInt32("scale");
	return true;
}

ExecuteUnitPtr WtExecuter::getUnit(const char* code, bool bAutoCreate /* = true */)
{
	WTSVariant* policy = _config->get("policy");
	std::string des = code;
	if (!policy->has(code))
		des = "default";

	auto it = _unit_map.find(code);
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
			_unit_map[code] = unit;
			unit->self()->init(this, code, cfg);
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
WTSHisTickData* WtExecuter::getTicks(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->get_ticks(stdCode, count);
}

WTSTickData* WtExecuter::grabLastTick(const char* stdCode)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->grab_last_tick(stdCode);
}

int32_t WtExecuter::getPosition(const char* stdCode, int32_t flag /* = 3 */)
{
	return _trader->getPosition(stdCode, flag);
}

int32_t WtExecuter::getUndoneQty(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

OrderMap* WtExecuter::getOrders(const char* stdCode)
{
	return _trader->getOrders(stdCode);
}

OrderIDs WtExecuter::buy(const char* stdCode, double price, uint32_t qty)
{
	if (!_channel_ready)
		return OrderIDs();
	return _trader->buy(stdCode, price, qty);
}

OrderIDs WtExecuter::sell(const char* stdCode, double price, uint32_t qty)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->sell(stdCode, price, qty);
}

bool WtExecuter::cancel(uint32_t localid)
{
	if (!_channel_ready)
		return false;

	return _trader->cancel(localid);
}

OrderIDs WtExecuter::cancel(const char* stdCode, bool isBuy, uint32_t qty)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->cancel(stdCode, isBuy, qty);
}

void WtExecuter::writeLog(const char* fmt, ...)
{
	char szBuf[2048] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _name.c_str());
	strcat(szBuf, fmt);
	va_list args;
	va_start(args, fmt);
	WTSLogger::log_dyn_direct("executer", _name.c_str(), LL_INFO, szBuf, args);
	va_end(args);
}

WTSCommodityInfo* WtExecuter::getCommodityInfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

uint64_t WtExecuter::getCurTime()
{
	return TimeUtils::makeTime(_engine->get_date(), _engine->get_raw_time() * 100000 + _engine->get_secs());
}

#pragma endregion Context回调接口
//ExecuteContext
//////////////////////////////////////////////////////////////////////////


#pragma region 外部接口
void WtExecuter::on_position_changed(const char* stdCode, int32_t targetPos)
{
	ExecuteUnitPtr unit = getUnit(stdCode);
	if (unit == NULL)
		return;

	targetPos *= _scale;

	int32_t oldVol = _target_pos[stdCode];
	//int32_t targetPos = oldVol + diffQty;
	_target_pos[stdCode] = targetPos;

	writeLog("%s目标仓位更新: %d -> %d", stdCode, oldVol, targetPos);	

	if (_trader && !_trader->checkOrderLimits(stdCode))
	{
		writeLog("合约 %s 已被禁止交易", stdCode);
		return;
	}

	unit->self()->set_position(stdCode, targetPos);
}

//void WtExecuter::set_position(const char* stdCode, int32_t targetPos)
//{
//	ExecuteUnitPtr unit = getUnit(stdCode);
//	if (unit == NULL)
//		return;
//
//	targetPos *= _scale;
//	int32_t oldVol = _target_pos[stdCode];
//	_target_pos[stdCode] = targetPos;
//	if (targetPos != oldVol)
//	{
//		writeLog("%s目标仓位更新: %d -> %d", stdCode, oldVol, targetPos);
//	}
//
//	if (_trader && !_trader->checkOrderLimits(stdCode))
//	{
//		writeLog("合约 %s 已被禁止交易", stdCode);
//		return;
//	}
//
//	unit->self()->set_position(stdCode, targetPos);
//}

void WtExecuter::set_position(const std::unordered_map<std::string, int32_t>& targets)
{
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();		
		int32_t newVol = it->second;
		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;

		newVol *= _scale;
		int32_t oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		if(newVol != oldVol)
		{
			writeLog("%s目标仓位更新: %d -> %d", stdCode, oldVol, newVol);
		}

		if (_trader && !_trader->checkOrderLimits(stdCode))
		{
			writeLog("合约 %s 已被禁止交易", stdCode);
			continue;
		}

		unit->self()->set_position(stdCode, newVol);
	}

	for (auto it = _target_pos.begin(); it != _target_pos.end(); it++)
	{
		const char* code = it->first.c_str();
		auto tit = targets.find(code);
		if(tit != targets.end())
			continue;

		ExecuteUnitPtr unit = getUnit(code);
		if (unit == NULL)
			continue;

		//WTSLogger::info2("executer", "[%s]目标仓位更新: %s -> %d", _name.c_str(), code, 0);
		unit->self()->set_position(code, 0);

		it->second = 0;
	}
}

void WtExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	unit->self()->on_tick(newTick);
}

void WtExecuter::on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	unit->self()->on_trade(stdCode, isBuy, vol, price);
}

void WtExecuter::on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled /* = false */)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	unit->self()->on_order(localid, stdCode, isBuy, leftQty, price, isCanceled);
}

void WtExecuter::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	unit->self()->on_entrust(localid, stdCode, bSuccess, message);
}

void WtExecuter::on_channel_ready()
{
	_channel_ready = true;
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = it->second;
		if (unitPtr)
			unitPtr->self()->on_channel_ready();
	}
}

void WtExecuter::on_channel_lost()
{
	_channel_ready = false;
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = it->second;
		if (unitPtr)
			unitPtr->self()->on_channel_lost();
	}
}

void WtExecuter::on_position(const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail)
{
	IHotMgr* hotMgr = _engine->get_hot_mgr();
	if(CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdFutCode(stdCode, cInfo);
		std::string code = hotMgr->getPrevRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
		if (code == stdCode)
		{
			//上期主力合约，需要清理仓位
			writeLog("合约 %s 为上一期主力合约，仓位即将自动清理");
			ExecuteUnitPtr unit = getUnit(stdCode);
			if (unit)
				unit->self()->set_position(stdCode, 0);
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
		WTSLogger::error("执行器工厂目录%s不存在", path);
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

		WTSLogger::info("执行器工厂%s加载成功", fInfo._fact->getName());
	}

	return true;
}

ExecuteUnitPtr WtExecuterFactory::createExeUnit(const char* factname, const char* unitname)
{
	auto it = _factories.find(factname);
	if (it == _factories.end())
		return ExecuteUnitPtr();

	ExeFactInfo& fInfo = it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if(unit == NULL)
	{
		WTSLogger::error("执行单元创建失败:%s.%s", factname, unitname);
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

	ExeFactInfo& fInfo = it->second;
	ExecuteUnit* unit = fInfo._fact->createExeUnit(unitname);
	if (unit == NULL)
	{
		WTSLogger::error("执行单元创建失败:", name);
		return ExecuteUnitPtr();
	}
	return ExecuteUnitPtr(new ExeUnitWrapper(unit, fInfo._fact));
}