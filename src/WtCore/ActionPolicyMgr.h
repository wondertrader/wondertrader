/*!
 * \file ActionPolicyMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <vector>
#include <stdint.h>
#include <string.h>

#include "../Includes/FasterDefs.h"


NS_WTP_BEGIN
class WTSVariant;

typedef enum tagActionType
{
	AT_Unknown = 8888,
	AT_Open = 9999,		//开仓
	AT_Close,			//平仓
	AT_CloseToday,		//平今
	AT_CloseYestoday	//平昨
} ActionType;

typedef struct _ActionRule
{
	ActionType	_atype;		//动作类型
	uint32_t	_limit;		//手数限制
	uint32_t	_limit_l;	//多头手数限制
	uint32_t	_limit_s;	//空头手数限制
	bool		_pure;		//主要针对AT_CloseToday和AT_CloseYestoday，用于判断是否是净今仓或者净昨仓

	_ActionRule()
	{
		memset(this, 0, sizeof(_ActionRule));
	}
} ActionRule;

typedef std::vector<ActionRule>	ActionRuleGroup;

class ActionPolicyMgr
{
public:
	ActionPolicyMgr();
	~ActionPolicyMgr();

public:
	bool init(const char* filename);

	const ActionRuleGroup& getActionRules(const char* pid);

private:
	typedef wt_hashmap<std::string, ActionRuleGroup> RulesMap;
	RulesMap	_rules;	//规则表

	wt_hashmap<std::string, std::string> _comm_rule_map;	//品种规则映射
};

NS_WTP_END
