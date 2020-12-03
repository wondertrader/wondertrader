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
#include <unordered_map>
#include <stdint.h>
#include <string.h>

#include "../Includes/WTSMarcos.h"


NS_OTP_BEGIN
class WTSVariant;

typedef enum tagActionType
{
	AT_Unknown = 8888,
	AT_Open = 9999,	//开仓
	AT_Close,		//平仓
	AT_CloseToday,	//平今
	AT_CloseYestoday	//平昨
} ActionType;

typedef struct _ActionRule
{
	ActionType	_atype;
	uint32_t	_limit;
	uint32_t	_limit_l;
	uint32_t	_limit_s;

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
	typedef std::unordered_map<std::string, ActionRuleGroup> RulesMap;
	RulesMap	_rules;	//规则表

	std::unordered_map<std::string, std::string> _comm_rule_map;	//品种规则映射
};

NS_OTP_END
