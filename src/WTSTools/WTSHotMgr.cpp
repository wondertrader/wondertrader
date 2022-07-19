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
#include "../Share/decimal.h"


WTSHotMgr::WTSHotMgr()
	: m_mapCustRules(NULL)
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

	auto len = strlen(stdCode);
	if (stdCode[len - 1] == '+' || stdCode[len - 1] == '-')
		len--;

	auto idx = StrUtil::findLast(stdCode, '.');
	if (idx == std::string::npos)
	{
		auto it = m_mapCustRules->find(ShortKey(stdCode, len));
		if (it == m_mapCustRules->end())
			return "";

		return it->first.c_str();
	}

	const char* tail = stdCode + idx + 1;
	auto it = m_mapCustRules->find(ShortKey(tail, len - idx - 1));
	if (it == m_mapCustRules->end())
		return "";

	return it->first.c_str();
}

double WTSHotMgr::getRuleFactor(const char* ruleTag, const char* fullPid, uint32_t uDate /* = 0 */ )
{
	if (m_mapCustRules == NULL)
		return 1.0;

	WTSProductHotMap* prodMap = (WTSProductHotMap*)m_mapCustRules->get(ruleTag);
	if (prodMap == NULL)
		return 1.0;

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return 1.0;

	if(uDate == 0)
	{
		WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->rbegin()->second, WTSSwitchItem*);
		return pItem->get_factor();
	}

	auto it = dtMap->lower_bound(uDate);
	if(it == dtMap->end())
	{
		//�Ҳ�����˵����¼�����ڶ��ȴ��������С�����Է������һ���ĸ�Ȩ����
		WTSSwitchItem* pItem = STATIC_CONVERT(dtMap->rbegin()->second, WTSSwitchItem*);
		return pItem->get_factor();
	}
	else
	{
		//�ҵ��ˣ���Ҫ���л������Ƿ���ڴ�������
		//�����ȣ�˵���պ��л�����ô��ֱ�ӷ��ظ�Ȩ����
		WTSSwitchItem* pItem = STATIC_CONVERT(it->second, WTSSwitchItem*);
		if (pItem->switch_date() == uDate)
		{
			return pItem->get_factor();
		}
		else
		{
			//����л����ڴ��ڴ������ڣ���Ҫ��ǰһ���׶�
			//����Ѿ��ǵ�һ���ˣ���ֱ�ӷ���1.0
			if (it == dtMap->begin())
			{
				return 1.0;
			}
			else
			{
				//������ǵ�һ���������һ�����ٷ��ؼ���
				it--;
				WTSSwitchItem* pItem = STATIC_CONVERT(it->second, WTSSwitchItem*);
				return pItem->get_factor();
			}
		}
	}

}

#pragma region "�������ӿ�"
bool WTSHotMgr::loadHots(const char* filename)
{
	loadCustomRules("HOT", filename);
	m_bInitialized = true;
	return true;
}

const char* WTSHotMgr::getPrevRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getPrevCustomRawCode("HOT", fullPid, dt);
}

const char* WTSHotMgr::getRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getCustomRawCode("HOT", fullPid, dt);
}

bool WTSHotMgr::isHot(const char* exchg, const char* rawCode, uint32_t dt)
{
	static thread_local char fullCode[64] = { 0 };
	fmtutil::format_to(fullCode, "{}.{}", exchg, rawCode);

	return isCustomHot("HOT", fullCode, dt);
}

bool WTSHotMgr::splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return splitCustomSections("HOT", fullPid, sDt, eDt, sections);
}
#pragma endregion "�����ӿ�"

#pragma region "�������ӿ�"
bool WTSHotMgr::loadSeconds(const char* filename)
{
	return loadCustomRules("2ND", filename);
}

const char* WTSHotMgr::getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getPrevCustomRawCode("2ND", fullPid, dt);
}

const char* WTSHotMgr::getSecondRawCode(const char* exchg, const char* pid, uint32_t dt)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return getCustomRawCode("2ND", fullPid, dt);
}

bool WTSHotMgr::isSecond(const char* exchg, const char* rawCode, uint32_t dt)
{
	static thread_local char fullCode[64] = { 0 };
	fmtutil::format_to(fullCode, "{}.{}", exchg, rawCode);

	return isCustomHot("2NDT", fullCode, dt);
}

bool WTSHotMgr::splitSecondSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections)
{
	static thread_local char fullPid[64] = { 0 };
	fmtutil::format_to(fullPid, "{}.{}", exchg, pid);

	return splitCustomSections("2ND", fullPid, sDt, eDt, sections);
}

#pragma endregion "�������ӿ�"

#pragma region "�Զ��������ӿ�"
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
			double factor = 1.0;
			for (uint32_t i = 0; i < jProduct->size(); i++)
			{
				WTSVariant* jHotItem = jProduct->get(i);
				WTSSwitchItem* pItem = WTSSwitchItem::create(
					exchg.c_str(), pid.c_str(),
					jHotItem->getCString("from"), jHotItem->getCString("to"), 
					jHotItem->getUInt32("date"));

				//���㸴Ȩ����
				double oldclose = jHotItem->getDouble("oldclose");
				double newclose = jHotItem->getDouble("newclose");
				factor *= (decimal::eq(oldclose, 0.0) ? 1.0 : (oldclose/ newclose));
				pItem->set_factor(factor);
				dateMap->add(pItem->switch_date(), pItem, false);
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
		//��Ϊ�ǼǵĻ��������ǿ�ʼ��Ч�Ľ����գ�����������̺�ȷ�������Ļ�
		//��ôdt�ͻ��ǵڶ��죬���ԣ�dt������ڵ����л�����
		if (pItem->switch_date() > dt)
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
		return false;

	WTSDateHotMap* dtMap = STATIC_CONVERT(prodMap->get(fullPid), WTSDateHotMap*);
	if (dtMap == NULL)
		return false;

	uint32_t leftDate = sDt;
	uint32_t lastDate = 0;
	const char* curHot = "";
	auto cit = dtMap->begin();
	double prevFactor = 1.0;
	for (; cit != dtMap->end(); cit++)
	{
		uint32_t curDate = cit->first;
		WTSSwitchItem* hotItem = (WTSSwitchItem*)cit->second;

		if (curDate > eDt)
		{
			sections.emplace_back(HotSection(hotItem->from(), leftDate, eDt, prevFactor));
		}
		else if (leftDate < curDate)
		{
			//�����ʼ����С�ڵ�ǰ�л�������,�����һ��
			if (strlen(hotItem->from()) > 0)//����fromΪ��,��Ҫ�ǵ�һ������,�����������������,Ҳû��̫�õİ취,ֻ�ܲ�Ҫ��һ��������,һ��������ǹ���
			{
				sections.emplace_back(HotSection(hotItem->from(), leftDate, TimeUtils::getNextDate(curDate, -1), prevFactor));
			}

			leftDate = curDate;
		}

		lastDate = curDate;
		prevFactor = hotItem->get_factor();
		curHot = hotItem->to();
	}

	if (leftDate >= lastDate && lastDate != 0)
	{
		sections.emplace_back(HotSection(curHot, leftDate, eDt, prevFactor));
	}

	return true;
}
#pragma endregion "�Զ��������ӿ�"

void WTSHotMgr::release()
{
	//if (m_pExchgHotMap)
	//{
	//	m_pExchgHotMap->release();
	//	m_pExchgHotMap = NULL;
	//}

	//if (m_pExchgScndMap)
	//{
	//	m_pExchgScndMap->release();
	//	m_pExchgScndMap = NULL;
	//}

	if(m_mapCustRules)
	{
		m_mapCustRules->release();
		m_mapCustRules = NULL;
	}
}