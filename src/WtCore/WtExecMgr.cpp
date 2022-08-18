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

void WtExecuterMgr::set_positions(faster_hashmap<LongKey, double> target_pos)
{
	if(_filter_mgr != NULL)
	{
		faster_hashmap<LongKey, double> des_port;
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
					//�����־
					WTSLogger::info("[Filters] {} target position reset by code filter: {} -> {}", stdCode.c_str(), oldVol, desVol);
				}

				des_port[stdCode] = desVol;
			}
			else
			{
				//�����־
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

void WtExecuterMgr::handle_pos_change(const char* stdCode, double targetPos, const char* execid /* = "ALL" */)
{
	if(_filter_mgr != NULL)
	{
		double oldVol = targetPos;
		bool isFltd = _filter_mgr->is_filtered_by_code(stdCode, targetPos);
		if (!isFltd)
		{
			if (!decimal::eq(targetPos, oldVol))
			{
				//�����־
				WTSLogger::info("[Filters] {} target position reset by filter: {} -> {}", stdCode, oldVol, targetPos);
			}
		}
		else
		{
			//�����־
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
			executer->on_position_changed(stdCode, targetPos);
		else if(strcmp(executer->name(), execid) == 0)
			executer->on_position_changed(stdCode, targetPos);
	}
}

void WtExecuterMgr::handle_amount_change(const char* stdCode, double targetAmount, const char* execid /* = "ALL" */)
{
	if (_filter_mgr != NULL)
	{
		double oldVol = targetAmount;
		bool isFltd = _filter_mgr->is_filtered_by_code(stdCode, targetAmount);
		if (!isFltd)
		{
			if (!decimal::eq(targetAmount, oldVol))
			{
				//�����־
				WTSLogger::info("[Filters] {} target amount reset by filter: {} -> {}", stdCode, oldVol, targetAmount);
			}
		}
		else
		{
			//�����־
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
			executer->on_amount_changed(stdCode, targetAmount);
		else if (strcmp(executer->name(), execid) == 0)
			executer->on_amount_changed(stdCode, targetAmount);
	}
}

void WtExecuterMgr::handle_ratio_change(const char* stdCode, double targetRatio, const char* execid /* = "ALL" */)
{
	if (_filter_mgr != NULL)
	{
		double oldVol = targetRatio;
		bool isFltd = _filter_mgr->is_filtered_by_code(stdCode, targetRatio);
		if (!isFltd)
		{
			if (!decimal::eq(targetRatio, oldVol))
			{
				//�����־
				WTSLogger::info("[Filters] {} target amount reset by filter: {} -> {}", stdCode, oldVol, targetRatio);
			}
		}
		else
		{
			//�����־
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
			executer->on_ratio_changed(stdCode, targetRatio);
		else if (strcmp(executer->name(), execid) == 0)
			executer->on_ratio_changed(stdCode, targetRatio);
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
		//�ȶ���Ͻ�������
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

		//Ȼ����ݹ���������Ŀ���λ
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
						//�����־
						WTSLogger::info("[Filters] {} target position reset by code filter: {} -> {}", stdCode.c_str(), oldVol, desVol);
					}

					des_port[stdCode] = desVol;
				}
				else
				{
					//�����־
					WTSLogger::info("[Filters] {} target position ignored by filter", stdCode.c_str());
				}
			}

			target_pos.swap(des_port);
		}
	}

	//����ִ����
	for (auto& e : _executers)
	{
		ExecCmdPtr& executer = (ExecCmdPtr&)e.second;
		if (_filter_mgr && _filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] Executer {} is filtered, all signals will be ignored", executer->name());
			continue;
		}

		//�����Լ���Ӧ�����
		auto it = _all_cached_targets.find(executer->name());

		//����Ҳ���������ȫ�����
		if (it == _all_cached_targets.end())
			it = _all_cached_targets.find("ALL");

		if (it == _all_cached_targets.end())
			return;

		executer->set_position(it->second);
	}

	//�ύ�����Ժ������ȫ�������Ŀ���λ
	_all_cached_targets.clear();
}

bool WtExecuterMgr::load_router_rules(WTSVariant* config)
{
	if (config == NULL || !config->isArray())
		return false;

	for(uint32_t i = 0; i < config->size(); i++)
	{
		WTSVariant* item = config->get(i);
		_router_rules[item->getCString("strategy")] = item->getCString("executer");

		WTSLogger::info("Signal of strategy {} will be routed to executer {}", item->getCString("strategy"), item->getCString("executer"));
		_routed_executers.insert(item->getCString("executer"));
	}

	WTSLogger::info("{} router rules loaded", _router_rules.size());

	return true;
}

#pragma endregion "WtExecuterMgr"
