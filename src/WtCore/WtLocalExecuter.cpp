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

USING_NS_WTP;


WtLocalExecuter::WtLocalExecuter(WtExecuterFactory* factory, const char* name, IDataManager* dataMgr)
	: IExecCommand(name)
	, _factory(factory)
	, _data_mgr(dataMgr)
	, _channel_ready(false)
	, _scale(1.0)
	, _auto_clear(true)
	, _trader(NULL)
{
}


WtLocalExecuter::~WtLocalExecuter()
{
	if (_pool)
		_pool->wait();
}

void WtLocalExecuter::setTrader(TraderAdapter* adapter)
{
	_trader = adapter;
	//设置的时候读取一下trader的状态
	if(_trader)
		_channel_ready = _trader->isReady();
}

bool WtLocalExecuter::init(WTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getDouble("scale");
	_strict_sync  = params->getBoolean("strict_sync");

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

	WTSVariant* cfgGroups = params->get("groups");
	if (cfgGroups)
	{
		auto names = cfgGroups->memberNames();
		for(const std::string& gpname : names)
		{
			CodeGroupPtr& gpInfo = _groups[gpname];
			if (gpInfo == NULL)
			{
				gpInfo.reset(new CodeGroup);
				wt_strcpy(gpInfo->_name, gpname.c_str(), gpname.size());
			}

			WTSVariant* cfgGrp = cfgGroups->get(gpname.c_str());
			auto codes = cfgGrp->memberNames();
			for(const std::string& code : codes)
			{
				gpInfo->_items[code] = cfgGrp->getDouble(code.c_str());
				_code_to_groups[code] = gpInfo;
			}
		}
	}

	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Local executer inited, scale: {}, auto_clear: {}, strict_sync: {}, thread poolsize: {}, code_groups: {}",
		_scale, _auto_clear, _strict_sync, poolsize, _groups.size());

	return true;
}

ExecuteUnitPtr WtLocalExecuter::getUnit(const char* stdCode, bool bAutoCreate /* = true */)
{
	CodeHelper::CodeInfo codeInfo = CodeHelper::extractStdCode(stdCode, NULL);
	std::string commID = codeInfo.stdCommID();

	WTSVariant* policy = _config->get("policy");
	std::string des = commID;
	if (!policy->has(commID.c_str()))
		des = "default";

	SpinLock lock(_mtx_units);

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

			//如果通道已经就绪，则直接通知执行单元
			if (_channel_ready)
				unit->self()->on_channel_ready();
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

double WtLocalExecuter::getPosition(const char* stdCode, bool validOnly /* = true */, int32_t flag /* = 3 */)
{
	if (NULL == _trader)
		return 0.0;

	return _trader->getPosition(stdCode, validOnly, flag);
}

double WtLocalExecuter::getUndoneQty(const char* stdCode)
{
	if (NULL == _trader)
		return 0.0;

	return _trader->getUndoneQty(stdCode);
}

OrderMap* WtLocalExecuter::getOrders(const char* stdCode)
{
	if (NULL == _trader)
		return NULL;

	return _trader->getOrders(stdCode);
}

OrderIDs WtLocalExecuter::buy(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->buy(stdCode, price, qty, 0, bForceClose);
}

OrderIDs WtLocalExecuter::sell(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->sell(stdCode, price, qty, 0, bForceClose);
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

void WtLocalExecuter::writeLog(const char* message)
{
	static thread_local char szBuf[2048] = { 0 };
	fmtutil::format_to(szBuf, "[{}]", _name.c_str());
	strcat(szBuf, message);
	WTSLogger::log_dyn_raw("executer", _name.c_str(), LL_INFO, szBuf);
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
void WtLocalExecuter::on_position_changed(const char* stdCode, double diffPos)
{
	ExecuteUnitPtr unit = getUnit(stdCode);
	if (unit == NULL)
		return;

	double oldVol = _target_pos[stdCode];
	double newVol = oldVol + diffPos;
	_target_pos[stdCode] = newVol;

	double traderTarget = round(newVol * _scale);

	if(!decimal::eq(diffPos, 0))
	{
		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target position of {} changed: {} -> {} : {} with scale:{}", stdCode, oldVol, newVol, traderTarget, _scale);
	}

	if (_trader && !_trader->checkOrderLimits(stdCode))
	{
		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "{} is disabled", stdCode);
		return;
	}

	unit->self()->set_position(stdCode, traderTarget);
}

void WtLocalExecuter::set_position(const wt_hashmap<std::string, double>& targets)
{
	/*
	 *	先要把目标头寸进行组合匹配
	 */
	auto real_targets = targets;
	for(auto& v : _groups)
	{
		const CodeGroupPtr& gpInfo = v.second;
		bool bHit = false;
		double gpQty = DBL_MAX;
		for(auto& vi : gpInfo->_items)
		{
			double unit = vi.second;
			auto it = real_targets.find(vi.first);
			if (it == real_targets.end())
			{
				bHit = false;
				break;
			}
			else
			{
				bHit = true;
				//计算最小的组合单位数量
				gpQty = std::min(gpQty, decimal::mod(it->second, unit));
			}
		}

		if(bHit && decimal::gt(gpQty, 0))
		{
			real_targets[gpInfo->_name] = gpQty;
			for (auto& vi : gpInfo->_items)
			{
				double unit = vi.second;
				real_targets[vi.first] -= gpQty * unit;
			}
		}
	}


	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		double newVol = it->second;
		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;

		double oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		// 账户的理论持仓要经过修正
		double traderTarget = round(newVol * _scale);

		if(!decimal::eq(oldVol, newVol))
		{
			WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target position of {} changed: {} -> {} : {} with scale{}", stdCode, oldVol, newVol, traderTarget, _scale);
		}

		if (_trader && !_trader->checkOrderLimits(stdCode))
		{
			WTSLogger::log_dyn("executer", _name.c_str(), LL_WARN, "{} is disabled due to entrust limit control ", stdCode);
			continue;
		}

		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code, traderTarget](){
				unit->self()->set_position(code.c_str(), traderTarget);
			});
		}
		else
		{
			unit->self()->set_position(stdCode, traderTarget);
		}
	}

	//在原来的目标头寸中，但是不在新的目标头寸中，则需要自动设置为0
	for (auto it = _target_pos.begin(); it != _target_pos.end(); it++)
	{
		const char* code = it->first.c_str();
		double& pos = (double&)it->second;
		auto tit = targets.find(code);
		if(tit != targets.end())
			continue;

		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "{} is not in target, set to 0 automatically", code);

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

	//如果开启了严格同步，则需要检查通道持仓
	//如果通道持仓不在管理中，则直接平掉
	if(_strict_sync)
	{
		for(const std::string& stdCode : _channel_holds)
		{
			auto it = _target_pos.find(stdCode.c_str());
			if(it != _target_pos.end())
				continue;

			WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "{} is not in management, set to 0 due to strict sync mode", stdCode.c_str());

			ExecuteUnitPtr unit = getUnit(stdCode.c_str());
			if (unit == NULL)
				continue;

			if (_pool)
			{
				std::string code = stdCode.c_str();
				_pool->schedule([unit, code]() {
					unit->self()->set_position(code.c_str(), 0);
				});
			}
			else
			{
				unit->self()->set_position(stdCode.c_str(), 0);
			}
		}
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
	SpinLock lock(_mtx_units);
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
	SpinLock lock(_mtx_units);
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
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

void WtLocalExecuter::on_account(const char* currency, double prebalance, double balance, double dynbalance, 
	double avaliable, double closeprofit, double dynprofit, double margin, double fee, double deposit, double withdraw)
{
	SpinLock lock(_mtx_units);
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			if (_pool)
			{
				std::string strCur = currency;
				_pool->schedule([unitPtr, strCur, prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw]() {
					unitPtr->self()->on_account(strCur.c_str(), prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw);
				});
			}
			else
			{
				unitPtr->self()->on_account(currency, prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw);
			}
		}
	}
}

void WtLocalExecuter::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	_channel_holds.insert(stdCode);

	/*
	 *	By Wesley @ 2021.12.14
	 *	先检查自动清理过期主力合约的标记是否为true
	 *	如果不为true，则直接退出该逻辑
	 */
	if (!_auto_clear)
		return;

	//如果不是分月期货合约，直接退出
	if (!CodeHelper::isStdMonthlyFutCode(stdCode))
		return;

	IHotMgr* hotMgr = _stub->get_hot_mon();
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, NULL);
	//获取上一期的主力合约
	std::string prevCode = hotMgr->getPrevRawCode(cInfo._exchg, cInfo._product, tradingday);

	//如果当前合约不是上一期的主力合约，则直接退出
	if (prevCode != cInfo._code)
		return;

	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Prev hot contract of {}.{} on {} is {}", cInfo._exchg, cInfo._product, tradingday, prevCode);

	thread_local static char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", cInfo._exchg, cInfo._product);

	//先检查排除列表
	//如果在排除列表中，则直接退出
	auto it = _clear_excludes.find(fullPid);
	if(it != _clear_excludes.end())
	{
		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Position of {}, as prev hot contract, won't be cleared for it's in exclude list", stdCode);
		return;
	}

	//如果包含列表不为空，再检查是否在包含列表中
	//如果为空，则全部清理，不再进入该逻辑
	if(!_clear_includes.empty())
	{
		it = _clear_includes.find(fullPid);
		if (it == _clear_includes.end())
		{
			WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Position of {}, as prev hot contract, won't be cleared for it's not in include list", stdCode);
			return;
		}
	}

	//最后再进行自动清理
	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Position of {}, as prev hot contract, will be cleared", stdCode);
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
