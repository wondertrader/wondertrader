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

	void	set_positions(faster_hashmap<LongKey, double> target_pos);
	void	handle_pos_change(const char* stdCode, double targetPos, double diffPos, const char* execid = "ALL");
	void	handle_tick(const char* stdCode, WTSTickData* curTick);

	/*
	 *	加载路由规则
	 */
	bool	load_router_rules(WTSVariant* config);

	/*
	 *	
	 */
	inline const char* get_route(const char* strategyid)
	{
		if (_router_rules.empty())
			return "ALL";

		auto it = _router_rules.find(strategyid);
		if (it == _router_rules.end())
			return "ALL";

		return it->second.c_str();
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
	typedef faster_hashmap<ShortKey, ExecCmdPtr> ExecuterMap;
	ExecuterMap		_executers;
	WtFilterMgr*	_filter_mgr;

	typedef faster_hashmap<LongKey, double> TargetsMap;
	faster_hashmap<ShortKey, TargetsMap>	_all_cached_targets;

	faster_hashmap<ShortKey, std::string>	_router_rules;

	faster_hashset<ShortKey>	_routed_executers;
};
NS_WTP_END
