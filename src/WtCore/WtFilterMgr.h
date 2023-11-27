#pragma once
#include <string>
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN

class EventNotifier;

class WtFilterMgr
{
public:
	WtFilterMgr():_filter_timestamp(0), _notifier(NULL){}

	void		set_notifier(EventNotifier* notifier) { _notifier = notifier; }

	/*
	 *	加载信号过滤器
	 */
	void		load_filters(const char* fileName = "");

	/*
	 *	是否因为策略被过滤掉了
	 *	如果过滤器是忽略的话, 就会返回true, 如果是重定向仓位, 就会返回false, 而目标仓位是一个被过滤器改过的值
	 *
	 *	@sname		策略名称
	 *	@targetPos	目标仓位, 以该数据为准
	 *	@isDiff		是否是增量
	 *
	 *	return		是否过滤掉了, 如果过滤掉了, 该持仓就不加入最终组合目标仓位
	 */
	bool		is_filtered_by_strategy(const char* straName, double& targetPos, bool isDiff = false);

	/*
	 *	是否因为代码被过滤掉了
	 *	如果过滤器是忽略的话, 就会返回true, 如果是重定向仓位, 就会返回false, 而目标仓位是一个被过滤器改过的值
	 *
	 *	@stdCode	标准合约代码
	 *	@targetPos	目标仓位, 以该数据为准
	 *
	 *	return		是否过滤掉了, 如果过滤掉了, 该持仓就不加入最终组合目标仓位
	 */
	bool		is_filtered_by_code(const char* stdCode, double& targetPos);

	/*
	 *	是否被执行器过滤器过滤掉了
	 *
	 *	@channelid	交易通道ID
	 * 
	 *	return		是否被过滤掉了，如果过滤掉了，该执行器就不执行任何信号了
	 */
	bool		is_filtered_by_executer(const char* execid);

private:
	//////////////////////////////////////////////////////////////////////////
	//信号过滤器
	typedef enum tagFilterAction
	{
		FA_Ignore,		//忽略, 即维持原有仓位
		FA_Redirect,	//重定向持仓, 即同步到指定目标仓位
		FA_None = 99
	} FilterAction;

	typedef struct _FilterItem
	{
		std::string		_key;		//关键字
		FilterAction	_action;	//过滤操作
		double			_target;	//目标仓位, 只有当_action为FA_Redirect才生效
	} FilterItem;

	typedef wt_hashmap<std::string, FilterItem>	FilterMap;
	FilterMap		_stra_filters;	//策略过滤器

	FilterMap		_code_filters;	//代码过滤器, 包括合约代码和品种代码, 同一时间只有一个生效, 合约代码优先级高于品种代码

	//交易通道过滤器
	typedef wt_hashmap<std::string, bool>	ExecuterFilters;
	ExecuterFilters	_exec_filters;

	std::string		_filter_file;	//过滤器配置文件
	uint64_t		_filter_timestamp;	//过滤器文件时间戳

	EventNotifier*	_notifier;
};

NS_WTP_END

