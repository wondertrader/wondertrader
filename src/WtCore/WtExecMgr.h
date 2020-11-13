#pragma once
#include "WtLocalExecuter.h"

NS_OTP_BEGIN
class WtExecuterMgr : private boost::noncopyable
{
public:
	inline void	add_executer(ExecCmdPtr executer)
	{
		_executers.push_back(executer);
	}

	void	set_positions(std::unordered_map<std::string, double> target_pos);
	void	handle_pos_change(const char* stdCode, double targetPos);
	void	handle_tick(const char* stdCode, WTSTickData* curTick);

public:
	typedef std::vector<ExecCmdPtr> ExecuterList;
	ExecuterList	_executers;
};
NS_OTP_END
