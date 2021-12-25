#include "WtExecMgr.h"
#include "WtFilterMgr.h"

#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

USING_NS_WTP;

//////////////////////////////////////////////////////////////////////////
#pragma region "WtExecuterMgr"
void WtExecuterMgr::enum_executer(EnumExecuterCb cb)
{
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		cb(executer);
	}
}

void WtExecuterMgr::set_positions(faster_hashmap<std::string, double> target_pos)
{
	if(_filter_mgr != NULL)
	{
		faster_hashmap<std::string, double> des_port;
		for(auto& m : target_pos)
		{
			const std::string& stdCode = m.first;
			double& desVol = (double&)m.second;
			double oldVol = desVol;

			bool isFltd = _filter_mgr->is_filtered_by_code(stdCode.c_str(), desVol);
			if (!isFltd)
			{
				if (!decimal::eq(desVol, oldVol))
				{
					//输出日志
					WTSLogger::info(fmt::format("[Filters] {} target position reset by code filter: {} -> {}", stdCode.c_str(), oldVol, desVol).c_str());
				}

				des_port[stdCode] = desVol;
			}
			else
			{
				//输出日志
				WTSLogger::info("[Filters] %s target position ignored by filter", stdCode.c_str());
			}
		}

		des_port.swap(target_pos);
	}

	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		if (_filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] Executer %s is filtered, all signals will be ignored", executer->name());
			continue;
		}
		executer->set_position(target_pos);
	}
}

void WtExecuterMgr::handle_pos_change(const char* stdCode, double targetPos)
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
				WTSLogger::info(fmt::format("[Filters] {} target position reset by filter: {} -> {}", stdCode, oldVol, targetPos).c_str());
			}
		}
		else
		{
			//输出日志
			WTSLogger::info("[Filters] %s target position ignored by filter", stdCode);
			return;
		}
	}
	
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		if(_filter_mgr->is_filtered_by_executer(executer->name()))
		{
			WTSLogger::info("[Filters] All signals to executer %s are ignored by executer filter", executer->name());
			continue;
		}
		executer->on_position_changed(stdCode, targetPos);
	}
}

void WtExecuterMgr::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		executer->on_tick(stdCode, curTick);
	}
}
#pragma endregion "WtExecuterMgr"
