#include "WtExecMgr.h"
#include "WtFilterMgr.h"

#include "../Share/decimal.h"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

//////////////////////////////////////////////////////////////////////////
#pragma region "WtExecuterMgr"
void WtExecuterMgr::enum_executer(EnumExecuterCb cb)
{
	for (auto& v : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)v.second;
		cb(executer);
	}
}

void WtExecuterMgr::set_positions(wt_hashmap<std::string, double> target_pos)
{
	if(_filter_mgr != NULL)
	{
		wt_hashmap<std::string, double> des_port;
		for(auto& m : target_pos)
		{
			const auto& stdCode = m.first;
			double& desVol = (double&)m.second;
			double oldVol = desVol;

			bool isFltd = _filter_mgr->is_filtered_by_code(stdCode.c_str(), desVol);
			if (!isFltd)
			{
				if (!decimal::eq(desVol, oldVol))
				{
					//输出日志
					WTSLogger::info("[Filters] {} target position reset by code filter: {} -> {}", stdCode.c_str(), oldVol, desVol);
				}

				des_port[stdCode] = desVol;
			}
			else
			{
				//输出日志
				WTSLogger::info("[Filters] {} target position ignored by filter", stdCode.c_str());
			}
		}

		des_port.swap(target_pos);
	}

	for (auto& v : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)v.second;

		if (_filter_mgr && _filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] Executer {} is filtered, all signals will be ignored", executer->name());
			continue;
		}
		executer->set_position(target_pos);
	}
}

void WtExecuterMgr::handle_pos_change(const char* stdCode, double targetPos, double diffPos, const char* execid /* = "ALL" */)
{
	if(_filter_mgr != NULL)
	{
		double oldVol = targetPos;
		bool isFltd = _filter_mgr->is_filtered_by_code(stdCode, targetPos);
		if (!isFltd)
		{
			if (!decimal::eq(targetPos, oldVol))
			{
				//输出日志
				WTSLogger::info("[Filters] {} target position reset by filter: {} -> {}", stdCode, oldVol, targetPos);
				//差量也要重算
				diffPos += (targetPos - oldVol);
			}
		}
		else
		{
			//输出日志
			WTSLogger::info("[Filters] {} target position ignored by filter", stdCode);
			return;
		}
	}

	for (auto& v : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)v.second;

		if (_filter_mgr && _filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] All signals to executer {} are ignored by executer filter", executer->name());
			continue;
		}

		auto it = _routed_executers.find(executer->name());
		if (it == _routed_executers.end() && strcmp(execid, "ALL") == 0)
			executer->on_position_changed(stdCode, diffPos);
		else if(strcmp(executer->name(), execid) == 0)
			executer->on_position_changed(stdCode, diffPos);
	}
}

void WtExecuterMgr::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	//for (auto it = _executers.begin(); it != _executers.end(); it++)
	//{
	//	ExecCmdPtr& executer = (*it);
	//	executer->on_tick(stdCode, curTick);
	//}

	for (auto& v : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)v.second;
		executer->on_tick(stdCode, curTick);
	}
}

void WtExecuterMgr::add_target_to_cache(const char* stdCode, double targetPos, const char* execid /* = "ALL" */)
{
	TargetsMap& targets = _all_cached_targets[execid];
	double& vol = targets[stdCode];
	vol += targetPos;
}

void WtExecuterMgr::commit_cached_targets(double scale /* = 1.0 */)
{
	for(auto& v : _all_cached_targets)
	{	
		//先对组合进行缩放
		const char* execid = v.first.c_str();
		TargetsMap& target_pos = (TargetsMap&)v.second;
		for(auto& item : target_pos)
		{
			const auto& stdCode = item.first;
			double& pos = (double&)item.second;

			if(decimal::eq(pos, 0))
				continue;

			double symbol = pos / abs(pos);
			pos = decimal::rnd(abs(pos)*scale)*symbol;
		}

		//然后根据过滤器调整目标仓位
		if (_filter_mgr != NULL)
		{
			TargetsMap des_port;
			for (auto& m : target_pos)
			{
				const auto& stdCode = m.first;
				double& desVol = (double&)m.second;
				double oldVol = desVol;

				bool isFltd = _filter_mgr->is_filtered_by_code(stdCode.c_str(), desVol);
				if (!isFltd)
				{
					if (!decimal::eq(desVol, oldVol))
					{
						//输出日志
						WTSLogger::info("[Filters] {} target position reset by code filter: {} -> {}", stdCode.c_str(), oldVol, desVol);
					}

					des_port[stdCode] = desVol;
				}
				else
				{
					//输出日志
					WTSLogger::info("[Filters] {} target position ignored by filter", stdCode.c_str());
				}
			}

			target_pos.swap(des_port);
		}
	}

	//遍历执行器
	for (auto& e : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)e.second;
		if (_filter_mgr && _filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] Executer {} is filtered, all signals will be ignored", executer->name());
			continue;
		}

		//先找自己对应的组合
		auto it = _all_cached_targets.find(executer->name());

		//如果找不到，就找全部组合
		if (it == _all_cached_targets.end())
			it = _all_cached_targets.find("ALL");

		if (it == _all_cached_targets.end())
			continue;

		executer->set_position(it->second);
	}

	//提交完了以后，清理掉全部缓存的目标仓位
	_all_cached_targets.clear();
}

bool WtExecuterMgr::load_router_rules(WTSVariant* config)
{
	if (config == NULL || !config->isArray())
		return false;

	for(uint32_t i = 0; i < config->size(); i++)
	{
		WTSVariant* item = config->get(i);
		const char* straName = item->getCString("strategy");
		WTSVariant* itemExec = item->get("executer");
		if(itemExec->isArray())
		{
			uint32_t cnt = itemExec->size();
			for(uint32_t k = 0; k < cnt; k++)
			{
				const char* execId = itemExec->get(k)->asCString();
				_router_rules[straName].insert(execId);
				WTSLogger::info("Signal of strategy {} will be routed to executer {}", straName, execId);
				_routed_executers.insert(execId);
			}
		}
		else
		{
			const char* execId = itemExec->asCString();
			_router_rules[straName].insert(execId);
			WTSLogger::info("Signal of strategy {} will be routed to executer {}", straName, execId);
			_routed_executers.insert(execId);
		}
	}

	WTSLogger::info("{} router rules loaded", _router_rules.size());

	return true;
}

#pragma endregion "WtExecuterMgr"
