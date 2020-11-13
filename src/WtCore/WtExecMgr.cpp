#include "WtExecMgr.h"

USING_NS_OTP;

//////////////////////////////////////////////////////////////////////////
#pragma region "WtExecuterMgr"
void WtExecuterMgr::set_positions(std::unordered_map<std::string, double> target_pos)
{
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
		executer->set_position(target_pos);
	}
}

void WtExecuterMgr::handle_pos_change(const char* stdCode, double targetPos)
{
	for (auto it = _executers.begin(); it != _executers.end(); it++)
	{
		ExecCmdPtr& executer = (*it);
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
