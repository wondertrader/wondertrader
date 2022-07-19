/*!
 * \file ActionPolicyMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ActionPolicyMgr.h"

#include "../Share/StdUtils.hpp"
#include "../WTSTools/WTSLogger.h"

#include "../Includes/WTSVariant.hpp"
#include "../WTSUtils/WTSCfgLoader.h"

USING_NS_WTP;

ActionPolicyMgr::ActionPolicyMgr()
{
}


ActionPolicyMgr::~ActionPolicyMgr()
{
}

bool ActionPolicyMgr::init(const char* filename)
{
	WTSVariant* cfg = WTSCfgLoader::load_from_file(filename, true);
	if (cfg == NULL)
		return false;

	auto keys = cfg->memberNames();
	for (auto it = keys.begin(); it != keys.end(); it++)
	{
		const char* gpName = (*it).c_str();
		WTSVariant*	vGpItem = cfg->get(gpName);
		ActionRuleGroup& gp = _rules[gpName];

		WTSVariant* vOrds = vGpItem->get("order");
		if(vOrds != NULL && vOrds->isArray())
		{
			for (uint32_t i = 0; i < vOrds->size(); i++)
			{
				WTSVariant* vObj = vOrds->get(i);
				ActionRule aRule;
				const char* action = vObj->getCString("action");
				uint32_t uLimit = vObj->getUInt32("limit");
				uint32_t uLimitS = vObj->getUInt32("limit_s");
				uint32_t uLimitL = vObj->getUInt32("limit_l");
				if (wt_stricmp(action, "open") == 0)
					aRule._atype = AT_Open;
				else if (wt_stricmp(action, "close") == 0)
					aRule._atype = AT_Close;
				else if (wt_stricmp(action, "closetoday") == 0)
					aRule._atype = AT_CloseToday;
				else if (wt_stricmp(action, "closeyestoday") == 0)
					aRule._atype = AT_CloseYestoday;
				else 
				{
					WTSLogger::error("Loading action policy failed: unrecognized type {}", action);
					continue;
				}

				aRule._limit = uLimit;
				aRule._limit_s = uLimitS;
				aRule._limit_l = uLimitL;
				aRule._pure = vObj->getBoolean("pure");
				gp.emplace_back(aRule);
			}
		}

		WTSVariant* filters = vGpItem->get("filters");
		if(filters!=NULL && filters->isArray() && filters->size()>0)
		{
			for (uint32_t i = 0; i < filters->size(); i++)
			{
				const char* commid = filters->get(i)->asCString();
				_comm_rule_map[commid] = gpName;
			}
		}
	}

	cfg->release();
	return true;
}

const ActionRuleGroup& ActionPolicyMgr::getActionRules(const char* pid)
{
	std::string gpName = "default";

	{//���ҵ�Ʒ�ֶ�Ӧ�Ĺ���������
		auto it = _comm_rule_map.find(pid);
		if (it != _comm_rule_map.end())
			gpName = it->second;
	}

	{
		auto it = _rules.find(gpName);
		if (it == _rules.end())
		{
			it = _rules.find("default");
			WTSLogger::error("Action policy group {} not exists, changed to default group", gpName.c_str());
		}

		assert(it != _rules.end());
		return it->second;
	}
}
