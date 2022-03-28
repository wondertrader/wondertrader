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

#include "../Includes/WTSHotItem.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/StrUtil.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/StdUtils.hpp"


WTSHotMgr::WTSHotMgr()
	: m_pExchgHotMap(NULL)
	, m_pExchgScndMap(NULL)
	, m_bInitialized(false)
{
}


WTSHotMgr::~WTSHotMgr()
{
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
				WTSHotItem* pItem = WTSHotItem::create(exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), jHotItem->getUInt32("date"));
				dateMap->add(pItem->switchdate(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = StrUtil::printf("%s.%s", exchg.c_str(), lastCode.c_str());
			m_curHotCodes[fullCode] = pid + "0001";
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

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
			return pItem->to();
		}
		else
		{
			cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
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

				WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
				return pItem->to();
			}
			else
			{
				cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
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

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
			return pItem->to();
		}
		else
		{
			WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
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

				WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
				return pItem->to();
			}
			else
			{
				WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
				return pItem->to();
			}
		}
	}
	

	return "";
}

const char* WTSHotMgr::getHotCode(const char* exchg, const char* rawCode, uint32_t dt)
{
	if(strlen(exchg) == 0)
		return "";

	if(dt == 0)
	{
		thread_local static char buf[64] = { 0 };
		auto len = strlen(exchg);
		wt_strcpy(buf, exchg, len);
		buf[len] = '.';
		wt_strcpy(buf + len + 1, rawCode);

		auto it = m_curHotCodes.find(buf);
		if (it == m_curHotCodes.end())
			return "";
		else
			return it->second.c_str();
	}

	std::string product = CodeHelper::rawMonthCodeToRawCommID(rawCode);

	if(m_pExchgHotMap == NULL)
		return "";

	WTSProductHotMap* proMap = STATIC_CONVERT(m_pExchgHotMap->get(exchg), WTSProductHotMap*);
	if(proMap == NULL)
		return "";

	WTSDateHotMap* dtMap = STATIC_CONVERT(proMap->get(product), WTSDateHotMap*);
	if(dtMap == NULL)
		return "";

	WTSDateHotMap::ConstIterator cit = dtMap->lower_bound(dt);
	if(cit != dtMap->end())
	{
		if(dt < cit->first)
			cit--;

		if(cit == dtMap->end())
			return "";

		WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
		if(strcmp(pItem->to(), rawCode) == 0)
			return pItem->hot();
	}
	else
	{
		WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
		if(strcmp(pItem->to(), rawCode) == 0)
			return pItem->hot();
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
		WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
		if(strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if(dtMap->size() > 0)
	{
		WTSHotItem* pItem = (WTSHotItem*)dtMap->last();
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
		WTSHotItem* hotItem = (WTSHotItem*)cit->second;
		
		if(curDate > eDt)
		{
			//sections[hotItem->from()] = HotSection(leftDate, eDt);
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt));
		}
		else if(leftDate < curDate)
		{
			//如果开始日期小于当前切换的日期,则添加一段
			if(strlen(hotItem->from()) > 0)//这里from为空,主要是第一条规则,如果真的遇到这种情况,也没有太好的办法,只能不要这一段数据了,一般情况下是够的
			{
				//sections[hotItem->from()] = HotSection(leftDate, TimeUtils::getNextDate(curDate, -1));
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1)));
			}
			
			leftDate = curDate;
		}

		lastDate = curDate;
		curHot = hotItem->to();
	}

	if(leftDate >= lastDate && lastDate != 0)
	{
		//sections[curHot] = HotSection(leftDate, eDt);
		sections.emplace_back(HotSection(curHot, leftDate, eDt));
	}

	return true;
}

void WTSHotMgr::getHotCodes(const char* exchg, std::map<std::string, std::string> &mapHots)
{
	if(m_pExchgHotMap == NULL)
		return;

	WTSExchgHotMap::ConstIterator cit = m_pExchgHotMap->begin();
	for(; cit != m_pExchgHotMap->end(); cit++)
	{
		if (strlen(exchg) == 0 || cit->first.compare(exchg)==0)
		{
			WTSProductHotMap* pMap = (WTSProductHotMap*)cit->second;
			WTSProductHotMap::ConstIterator pit = pMap->begin();
			for(; pit != pMap->end(); pit++)
			{
				WTSDateHotMap* dtMap = (WTSDateHotMap*)pit->second;
				WTSHotItem* item = (WTSHotItem*)dtMap->last();
				mapHots[item->hot()] = item->to();
			}
		}
	}
}

bool WTSHotMgr::hasHotCodes()
{
	if (m_pExchgHotMap == NULL)
		return false;

	uint32_t cnt = 0;
	WTSExchgHotMap::ConstIterator cit = m_pExchgHotMap->begin();
	for (; cit != m_pExchgHotMap->end(); cit++)
	{
		WTSProductHotMap* pMap = (WTSProductHotMap*)cit->second;
		cnt += pMap->size();
	}

	return cnt > 0;
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
				WTSHotItem* pItem = WTSHotItem::create(exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), jHotItem->getUInt32("date"));
				dateMap->add(pItem->switchdate(), pItem, false);
				lastCode = jHotItem->getCString("to");
			}

			std::string fullCode = StrUtil::printf("%s.%s", exchg.c_str(), lastCode.c_str());
			m_curSecCodes[fullCode] = pid + "0002";
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

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
			return pItem->to();
		}
		else
		{
			cit--;

			if (cit == dtMap->end() || cit == dtMap->begin())
				return "";

			cit--;

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
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

				WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
				return pItem->to();
			}
			else
			{
				cit--;

				if (cit == dtMap->end() || cit == dtMap->begin())
					return "";

				cit--;

				WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
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

			WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
			return pItem->to();
		}
		else
		{
			WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
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

				WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
				return pItem->to();
			}
			else
			{
				WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
				return pItem->to();
			}
		}
	}


	return "";
}

const char* WTSHotMgr::getSecondCode(const char* exchg, const char* rawCode, uint32_t dt)
{
	//if (dt == 0)
	//	dt = TimeUtils::getCurDate();

	if (strlen(exchg) == 0)
		return "";

	if (dt == 0)
	{
		thread_local static char buf[64] = { 0 };
		auto len = strlen(exchg);
		wt_strcpy(buf, exchg, len);
		buf[len] = '.';
		wt_strcpy(buf + len + 1, rawCode);
		auto it = m_curSecCodes.find(buf);
		if (it == m_curSecCodes.end())
			return "";
		else
			return it->second.c_str();
	}

	std::string product = CodeHelper::rawMonthCodeToRawCommID(rawCode);

	if (m_pExchgScndMap == NULL)
		return "";

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

		WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return pItem->hot();
	}
	else
	{
		WTSHotItem* pItem = STATIC_CONVERT(dtMap->last(), WTSHotItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return pItem->hot();
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
		WTSHotItem* pItem = STATIC_CONVERT(cit->second, WTSHotItem*);
		if (strcmp(pItem->to(), rawCode) == 0)
			return true;
	}
	else if (dtMap->size() > 0)
	{
		WTSHotItem* pItem = (WTSHotItem*)dtMap->last();
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
		WTSHotItem* hotItem = (WTSHotItem*)cit->second;

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