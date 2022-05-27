/*!
 * \file WTSHotMgr.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WTSHotMgr.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include "../Includes/WTSSwitchItem.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"


WTSHotMgr::WTSHotMgr()
	: m_pExchgHotMap(NULL)
	, m_pExchgScndMap(NULL)
	, m_mapCustRules(NULL)
	, m_bInitialized(false)
{
}


WTSHotMgr::~WTSHotMgr()
{
}

const char* WTSHotMgr::getRuleTag(const char* stdCode)
{
	if (m_mapCustRules == NULL)
		return "";

	auto idx = StrUtil::findLast(stdCode, '.');
	if (idx == std::string::npos)
		return "";

	const char* tail = stdCode + idx + 1;
	auto it = m_mapCustRules->find(tail);
	if (it == m_mapCustRules->end())
		return "";

	return it->first.c_str();
}

#pragma region "次主力接口"
bool WTSHotMgr::loadHots(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		return false;
	}

	WTSVariant* root = WTSCfgLoader::load_from_file(filename, true);
	if (root == NULL)
		return false;

	if (m_pExchgHotMap != NULL)
	{
		m_pExchgHotMap->release();
		m_pExchgHotMap = NULL;
	}

	m_pExchgHotMap = WTSExchgHotMap::create();

	for(const std::string& exchg : root->memberNames())
	{
		WTSVariant* jExchg = root->get(exchg);

		WTSProductHotMap* productMap = WTSProductHotMap::create();
		m_pExchgHotMap->add(exchg, productMap, false);

		for (const std::string& pid : jExchg->memberNames())
		{
			WTSVariant* jProduct = jExchg->get(pid);

			WTSDateHotMap* dateMap = WTSDateHotMap::create();
			productMap->add(pid.c_str(), dateMap, false);

			std::string lastCode;
			for (uint32_t i = 0; i < jProduct->size(); i++)
			{
				WTSVariant* jHotItem = jProduct->get(i);
				WTSSwitchItem* pItem = WTSSwitchItem::create(exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), jHotItem->getUInt32("date"));
				dateMap->add(pItem->switchdate(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = fmt::format("{}.{}", exchg.c_str(), lastCode.c_str());
			m_curHotCodes.insert(fullCode);
		}
	}

	root->release();
	m_bInitialized = true;
	return true;
}

const char* WTSHotMgr::getPrevRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	if (dt == 0)
		dt = TimeUtils::getCurDate();

	std::string product = pid;

	if (strlen(exchg) == 0)
		return "";

	if (m_pExchgHotMap == NULL)
		return "";

	if (strcmp(exchg, HOTS_MARKET) != 0)
	{
		WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgHotMap->get(exchg), WTSProductHotMap*);
		if (proMap == NULL)
			return "";

		WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
		if (dtMap == NULL)
			return "";

		WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
		if (cit != dtMap->end())
		{
			if (dt < cit->first)
				cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
		else
		{
			cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
	}
	else
	{
		for (auto pit = m_pExchgHotMap->begin(); pit != m_pExchgHotMap->end(); pit++)
		{
			WTSProductHotMap* proMap = STATIC_CONVERT(pit->second, WTSProductHotMap*);
			if (proMap == NULL)
				continue;

			WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
			if (dtMap == NULL)
				continue;

			WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
			if (cit != dtMap->end())
			{
				if (dt < cit->first)
					cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
				return pItem->to();
			}
			else
			{
				cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
				return pItem->to();
			}
		}
	}

	return "";
}

const char* WTSHotMgr::getRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	if (dt == 0)
		dt = TimeUtils::getCurDate();

	std::string product = pid;

	if (strlen(exchg) == 0)
		return "";

	if(m_pExchgHotMap == NULL)
		return "";

	if (strcmp(exchg, HOTS_MARKET) != 0)
	{
		WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgHotMap->get(exchg), WTSProductHotMap*);
		if (proMap == NULL)
			return "";

		WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
		if (dtMap == NULL)
			return "";

		WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
		if (cit != dtMap->end())
		{
			if (dt < cit->first)
				cit--;

			if (cit == dtMap->end())
				return "";

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
		else
		{
			WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
			return pItem->to();
		}
	}
	else
	{
		for (auto pit = m_pExchgHotMap->begin(); pit != m_pExchgHotMap->end(); pit++)
		{
			WTSProductHotMap* proMap = STATIC_CONVERT(pit->second, WTSProductHotMap*);
			if (proMap == NULL)
				continue;

			WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
			if (dtMap == NULL)
				continue;

			WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
			if (cit != dtMap->end())
			{
				if (dt < cit->first)
					cit--;

				if (cit == dtMap->end())
					return "";

				WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
				return pItem->to();
			}
			else
			{
				WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
				return pItem->to();
			}
		}
	}

	return "";
}

bool WTSHotMgr::isHot(const char* exchg, const char* rawCode, uint32_t dt)
{
	if (strlen(exchg) == 0)
		return "";

	if (dt == 0)
	{
		thread_local static char buf[64] = { 0 };
		auto len = strlen(exchg);
		wt_strcpy(buf, exchg, len);
		buf[len] = '.';
		wt_strcpy(buf + len + 1, rawCode);
		len += strlen(rawCode)  + 1;
		buf[len] = '\0';

		auto it = m_curHotCodes.find(buf);
		if (it == m_curHotCodes.end())
			return false;
		else
			return true;
	}

	std::string product = CodeHelper::rawMonthCodeToRawCommID(rawCode);

	if(m_pExchgHotMap == NULL)
		false;

	WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgHotMap->get(exchg), WTSProductHotMap*);
	if(proMap == NULL)
		false;

	WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
	if(dtMap == NULL)
		false;

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if(cit != dtMap->end())
	{
		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		if(strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if(dtMap->size() > 0)
	{
		WTSSwitchItem* pItem = (WTSSwitchItem*)dtMap->last();
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}

	return false;
}

bool WTSHotMgr::splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	std::string product = pid;

	if (strlen(exchg) == 0)
		return false;

	if (m_pExchgHotMap == NULL)
		return false;

	WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgHotMap->get(exchg), WTSProductHotMap*);
	if (proMap == NULL)
		return false;

	WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
	if (dtMap == NULL)
		return false;

	uint32_t leftDate = sDt;
	uint32_t lastDate = 0;
	const char* curHot = "";
	auto cit = dtMap->begin();
	for (; cit != dtMap->end(); cit++)
	{
		uint32_t curDate = cit->first;
		WTSSwitchItem* hotItem = (WTSSwitchItem*)cit->second;
		
		if(curDate > eDt)
		{
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt));
		}
		else if(leftDate < curDate)
		{
			//如果开始日期小于当前切换的日期,则添加一段
			if(strlen(hotItem->from()) > 0)//这里from为空,主要是第一条规则,如果真的遇到这种情况,也没有太好的办法,只能不要这一段数据了,一般情况下是够的
			{
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1)));
			}
			
			leftDate = curDate;
		}

		lastDate = curDate;
		curHot = hotItem->to();
	}

	if(leftDate >= lastDate && lastDate != 0)
	{
		sections.emplace_back(HotSection(curHot, leftDate, eDt));
	}

	return true;
}
#pragma endregion "主力接口"

#pragma region "次主力接口"
bool WTSHotMgr::loadSeconds(const char* filename)
{
	if (!StdFile::exists(filename))
	{
		return false;
	}

	WTSVariant* root = WTSCfgLoader::load_from_file(filename, true);
	if (root == NULL)
		return false;

	if (m_pExchgScndMap != NULL)
	{
		m_pExchgScndMap->release();
		m_pExchgScndMap = NULL;
	}

	m_pExchgScndMap = WTSExchgHotMap::create();

	for (const std::string& exchg : root->memberNames())
	{
		WTSVariant* jExchg = root->get(exchg);

		WTSProductHotMap* productMap = WTSProductHotMap::create();
		m_pExchgScndMap->add(exchg, productMap, false);

		for (const std::string& pid : jExchg->memberNames())
		{
			WTSVariant* jProduct = jExchg->get(pid);

			WTSDateHotMap* dateMap = WTSDateHotMap::create();
			productMap->add(pid.c_str(), dateMap, false);

			std::string lastCode;
			for (uint32_t i = 0; i < jProduct->size(); i++)
			{
				WTSVariant* jHotItem = jProduct->get(i);
				WTSSwitchItem* pItem = WTSSwitchItem::create(exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), jHotItem->getUInt32("date"));
				dateMap->add(pItem->switchdate(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = fmt::format("{}.{}", exchg.c_str(), lastCode.c_str());
			m_curSecCodes.insert(fullCode);
		}
	}

	return true;
}

const char* WTSHotMgr::getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	if (dt == 0)
		dt = TimeUtils::getCurDate();

	std::string product = pid;

	if (strlen(exchg) == 0)
		return "";

	if (m_pExchgScndMap == NULL)
		return "";

	if (strcmp(exchg, SECONDS_MARKET) != 0)
	{
		WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgScndMap->get(exchg), WTSProductHotMap*);
		if (proMap == NULL)
			return "";

		WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
		if (dtMap == NULL)
			return "";

		WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
		if (cit != dtMap->end())
		{
			if (dt < cit->first)
				cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
		else
		{
			cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
	}
	else
	{
		for (auto pit = m_pExchgScndMap->begin(); pit != m_pExchgScndMap->end(); pit++)
		{
			WTSProductHotMap* proMap = STATIC_CONVERT(pit->second, WTSProductHotMap*);
			if (proMap == NULL)
				continue;

			WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
			if (dtMap == NULL)
				continue;

			WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
			if (cit != dtMap->end())
			{
				if (dt < cit->first)
					cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
				return pItem->to();
			}
			else
			{
				cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
				return pItem->to();
			}
		}
	}


	return "";
}

const char* WTSHotMgr::getSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	if (dt == 0)
		dt = TimeUtils::getCurDate();

	std::string product = pid;

	if (strlen(exchg) == 0)
		return "";

	if (m_pExchgScndMap == NULL)
		return "";

	if (strcmp(exchg, SECONDS_MARKET) != 0)
	{
		WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgScndMap->get(exchg), WTSProductHotMap*);
		if (proMap == NULL)
			return "";

		WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
		if (dtMap == NULL)
			return "";

		WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
		if (cit != dtMap->end())
		{
			if (dt < cit->first)
				cit--;

			if (cit == dtMap->end())
				return "";

			WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
			return pItem->to();
		}
		else
		{
			WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
			return pItem->to();
		}
	}
	else
	{
		for (auto pit = m_pExchgScndMap->begin(); pit != m_pExchgScndMap->end(); pit++)
		{
			WTSProductHotMap* proMap = STATIC_CONVERT(pit->second, WTSProductHotMap*);
			if (proMap == NULL)
				continue;

			WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
			if (dtMap == NULL)
				continue;

			WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
			if (cit != dtMap->end())
			{
				if (dt < cit->first)
					cit--;

				if (cit == dtMap->end())
					return "";

				WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
				return pItem->to();
			}
			else
			{
				WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
				return pItem->to();
			}
		}
	}


	return "";
}

bool WTSHotMgr::isSecond(const char* exchg, const char* rawCode, uint32_t dt)
{
	//if (dt == 0)
	//	dt = TimeUtils::getCurDate();

	if (strlen(exchg) == 0)
		false;

	if (dt == 0)
	{
		thread_local static char buf[64] = { 0 };
		auto len = strlen(exchg);
		wt_strcpy(buf, exchg, len);
		buf[len] = '.';
		wt_strcpy(buf + len + 1, rawCode);
		auto it = m_curSecCodes.find(buf);
		if (it == m_curSecCodes.end())
			return false;
		else
			return true;
	}

	std::string product = CodeHelper::rawMonthCodeToRawCommID(rawCode);

	if (m_pExchgScndMap == NULL)
		false;

	WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgScndMap->get(exchg), WTSProductHotMap*);
	if (proMap == NULL)
		false;

	WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
	if (dtMap == NULL)
		false;

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if (dtMap->size() > 0)
	{
		WTSSwitchItem* pItem = (WTSSwitchItem*)dtMap->last();
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}

	return false;
}

bool WTSHotMgr::splitSecondSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	std::string product = pid;

	if (strlen(exchg) == 0)
		return false;

	if (m_pExchgScndMap == NULL)
		return false;

	WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgScndMap->get(exchg), WTSProductHotMap*);
	if (proMap == NULL)
		return false;

	WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
	if (dtMap == NULL)
		return false;

	uint32_t leftDate = sDt;
	uint32_t lastDate = 0;
	const char* curHot = "";
	auto cit = dtMap->begin();
	for (; cit != dtMap->end(); cit++)
	{
		uint32_t curDate = cit->first;
		WTSSwitchItem* hotItem = (WTSSwitchItem*)cit->second;

		if (curDate > eDt)
		{
			//sections[hotItem->from()] = HotSection(leftDate, eDt);
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt));
		}
		else if (leftDate < curDate)
		{
			//如果开始日期小于当前切换的日期,则添加一段
			if (strlen(hotItem->from()) > 0)//这里from为空,主要是第一条规则,如果真的遇到这种情况,也没有太好的办法,只能不要这一段数据了,一般情况下是够的
			{
				//sections[hotItem->from()] = HotSection(leftDate, TimeUtils::getNextDate(curDate, -1));
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1)));
			}

			leftDate = curDate;
		}

		lastDate = curDate;
		curHot = hotItem->to();
	}

	if (leftDate >= lastDate && lastDate != 0)
	{
		//sections[curHot] = HotSection(leftDate, eDt);
		sections.emplace_back(HotSection(curHot, leftDate, eDt));
	}

	return true;
}

#pragma endregion "次主力接口"

#pragma region "自定义主力接口"
bool WTSHotMgr::loadCustomRules(const char* tag, const char* filename)
{
	if (!StdFile::exists(filename))
	{
		return false;
	}

	WTSVariant* root = WTSCfgLoader::load_from_file(filename, true);
	if (root == NULL)
		return false;

	if (m_mapCustRules == NULL)
		m_mapCustRules = WTSCustomSwitchMap::create();

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(tag);
	if(prodMap == NULL)
	{
		prodMap = WTSProductHotMap::create();
		m_mapCustRules->add(tag, prodMap, false);
	}

	for (const std::string& exchg : root->memberNames())
	{
		WTSVariant* jExchg = root->get(exchg);

		for (const std::string& pid : jExchg->memberNames())
		{
			WTSVariant* jProduct = jExchg->get(pid);
			std::string fullPid = fmt::format("{}.{}", exchg, pid);

			WTSDateHotMap* dateMap = WTSDateHotMap::create();
			prodMap->add(fullPid.c_str(), dateMap, false);

			std::string lastCode;
			for (uint32_t i = 0; i < jProduct->size(); i++)
			{
				WTSVariant* jHotItem = jProduct->get(i);
				WTSSwitchItem* pItem = WTSSwitchItem::create(
					exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), 
					jHotItem->getUInt32("date"),
					jHotItem->getDouble("oldclose"), jHotItem->getDouble("newclose"));
				dateMap->add(pItem->switchdate(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = fmt::format("{}.{}", exchg.c_str(), lastCode.c_str());
			m_mapCustCodes[tag].insert(fullCode);
		}
	}

	root->release();
	return true;
}

const char* WTSHotMgr::getPrevCustomRawCode(const char* tag, const char* fullPid, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return "";

	if (dt == 0)
		dt = TimeUtils::getCurDate();

	if (m_mapCustRules == NULL)
		return "";

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		if (dt < cit->first)
			cit--;

		if (cit == dtMap->end() || cit == dtMap->begin())
			return "";

		cit--;

		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		return pItem->to();
	}
	else
	{
		cit--;

		if (cit == dtMap->end() || cit == dtMap->begin())
			return "";

		cit--;

		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		return pItem->to();
	}

	return "";
}

const char* WTSHotMgr::getCustomRawCode(const char* tag, const char* fullPid, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return "";

	if (dt == 0)
		dt = TimeUtils::getCurDate();

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		if (dt < cit->first)
			cit--;

		if (cit == dtMap->end())
			return "";

		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		return pItem->to();
	}
	else
	{
		WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->last(), WTSSwitchItem*);
		return pItem->to();
	}

	return "";
}

bool WTSHotMgr::isCustomHot(const char* tag, const char* fullCode, uint32_t dt /* = 0 */)
{
	if (m_mapCustRules == NULL)
		return false;

	const auto& curHotCodes = m_mapCustCodes[tag];
	if (curHotCodes.empty())
		return false;

	if (dt == 0)
	{
		auto it = curHotCodes.find(fullCode);
		if (it == curHotCodes.end())
			return false;
		else
			return true;
	}

	auto idx = StrUtil::findFirst(fullCode, '.');
	const char* rawCode = fullCode + idx + 1;
	std::string fullPid(fullCode, idx);
	fullPid += ".";
	fullPid += CodeHelper::rawMonthCodeToRawCommID(rawCode);

	if (m_pExchgHotMap == NULL)
		false;

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if (cit != dtMap->end())
	{
		WTSSwitchItem* pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		//因为登记的换月日期是开始生效的交易日，如果是下午盘后确定主力的话
		//那么dt就会是第二天，所以，dt必须大于等于切换日期
		if (pItem->switchdate() > dt)
			cit--;

		pItem = STATIC_CONVERT(cit->second, WTSSwitchItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if (dtMap->size() > 0)
	{
		WTSSwitchItem* pItem = (WTSSwitchItem*)dtMap->last();
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}

	return false;
}

bool WTSHotMgr::splitCustomSections(const char* tag, const char* fullPid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	if (m_mapCustRules == NULL)
		return false;

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(tag);
	if (prodMap == NULL)
		return "";

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return "";

	uint32_t leftDate = sDt;
	uint32_t lastDate = 0;
	const char* curHot = "";
	auto cit = dtMap->begin();
	for (; cit != dtMap->end(); cit++)
	{
		uint32_t curDate = cit->first;
		WTSSwitchItem* hotItem = (WTSSwitchItem*)cit->second;

		if (curDate > eDt)
		{
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt));
		}
		else if (leftDate < curDate)
		{
			//如果开始日期小于当前切换的日期,则添加一段
			if (strlen(hotItem->from()) > 0)//这里from为空,主要是第一条规则,如果真的遇到这种情况,也没有太好的办法,只能不要这一段数据了,一般情况下是够的
			{
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1)));
			}

			leftDate = curDate;
		}

		lastDate = curDate;
		curHot = hotItem->to();
	}

	if (leftDate >= lastDate && lastDate != 0)
	{
		sections.emplace_back(HotSection(curHot, leftDate, eDt));
	}

	return true;
}
#pragma endregion "自定义主力接口"

void WTSHotMgr::release()
{
	if (m_pExchgHotMap)
	{
		m_pExchgHotMap->release();
		m_pExchgHotMap = NULL;
	}

	if (m_pExchgScndMap)
	{
		m_pExchgScndMap->release();
		m_pExchgScndMap = NULL;
	}
}