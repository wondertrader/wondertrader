#pragma once
#include "WtLocalExecuter.h"

NS_OTP_BEGIN
class WtFilterMgr;
class WtExecuterMgr : private boost::noncopyable
{
public:
	WtExecuterMgr():_filter_mgr(NULL){}

	inline void set_filter_mgr(WtFilterMgr* mgr) { _filter_mgr = mgr; }

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

	WtFilterMgr*	_filter_mgr;
};
NS_OTP_END
