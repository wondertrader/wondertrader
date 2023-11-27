#pragma once
#include <functional>
#include "WtLocalExecuter.h"

NS_WTP_BEGIN
class WtFilterMgr;

typedef std::function<void(ExecCmdPtr)> EnumExecuterCb;

class WtExecuterMgr : private boost::noncopyable
{
public:
	WtExecuterMgr():_filter_mgr(NULL){}

	inline void set_filter_mgr(WtFilterMgr* mgr) { _filter_mgr = mgr; }

	inline void	add_executer(ExecCmdPtr executer)
	{
		_executers[executer->name()] = executer;
	}

	void	enum_executer(EnumExecuterCb cb);

	void	set_positions(wt_hashmap<std::string, double> target_pos);
	void	handle_pos_change(const char* stdCode, double targetPos, double diffPos, const char* execid = "ALL");
	void	handle_tick(const char* stdCode, WTSTickData* curTick);

	/*
	 *	加载路由规则
	 */
	bool	load_router_rules(WTSVariant* config);

	/*
	 *	
	 */
	inline const wt_hashset<std::string>& get_route(const char* strategyid)
	{
		static wt_hashset<std::string> ALL_EXECUTERS;
		if (ALL_EXECUTERS.empty())
			ALL_EXECUTERS.insert("ALL");

		if (_router_rules.empty())
			return ALL_EXECUTERS;

		auto it = _router_rules.find(strategyid);
		if (it == _router_rules.end())
			return ALL_EXECUTERS;

		return it->second;
	}

	/*
	 *	清除缓存的目标仓位
	 */
	inline void	clear_cached_targets()
	{
		_all_cached_targets.clear();
	}

	/*
	 *	将目标仓位加入缓存
	 *	@stdCode	合约代码
	 *	@targetPos	目标仓位
	 *	@execid		执行器ID
	 */
	void	add_target_to_cache(const char* stdCode, double targetPos, const char* execid = "ALL");

	/*
	 *	提交缓存的目标头寸
	 *	@scale	风控系数
	 */
	void	commit_cached_targets(double scale = 1.0);

private:
	typedef wt_hashmap<std::string, ExecCmdPtr> ExecuterMap;
	ExecuterMap		_executers;
	WtFilterMgr*	_filter_mgr;

	typedef wt_hashmap<std::string, double> TargetsMap;
	wt_hashmap<std::string, TargetsMap>	_all_cached_targets;

	typedef wt_hashset<std::string>	ExecuterSet;
	wt_hashmap<std::string, ExecuterSet>	_router_rules;

	wt_hashset<std::string>	_routed_executers;
};
NS_WTP_END
