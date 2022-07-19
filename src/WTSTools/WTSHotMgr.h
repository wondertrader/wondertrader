/*!
 * \file WTSHotMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ������Լ������ʵ��
 */
#pragma once
#include "../Includes/IHotMgr.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"
#include <string>

NS_WTP_BEGIN
	class WTSSwitchItem;
NS_WTP_END

USING_NS_WTP;

//��������ӳ��
typedef WTSMap<uint32_t>			WTSDateHotMap;
//Ʒ������ӳ��
typedef WTSHashMap<ShortKey>		WTSProductHotMap;
//���г�����ӳ��
typedef WTSHashMap<ShortKey>		WTSExchgHotMap;

//�Զ����л�����ӳ��
typedef WTSHashMap<ShortKey>		WTSCustomSwitchMap;

class WTSHotMgr : public IHotMgr
{
public:
	WTSHotMgr();
	~WTSHotMgr();

public:
	bool loadHots(const char* filename);
	bool loadSeconds(const char* filename);
	void release();

	bool loadCustomRules(const char* tag, const char* filename);

	inline bool isInitialized() const {return m_bInitialized;}

public:
	virtual const char* getRuleTag(const char* stdCode) override;

	virtual double		getRuleFactor(const char* ruleTag, const char* fullPid, uint32_t uDate  = 0 ) override;

	//////////////////////////////////////////////////////////////////////////
	//�����ӿ�
	virtual const char* getRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool	isHot(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool	splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//�������ӿ�
	virtual const char* getSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool		isSecond(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool		splitSecondSecions(const char* exchg, const char* hotCode, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//ͨ�ýӿ�
	virtual const char* getCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual const char* getPrevCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual bool		isCustomHot(const char* tag, const char* fullCode, uint32_t dt) override;

	virtual bool		splitCustomSections(const char* tag, const char* fullPid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;


private:
	//WTSExchgHotMap*	m_pExchgHotMap;
	//WTSExchgHotMap*	m_pExchgScndMap;
	//faster_hashset<ShortKey>	m_curHotCodes;
	//faster_hashset<ShortKey>	m_curSecCodes;
	bool			m_bInitialized;

	WTSCustomSwitchMap*	m_mapCustRules;
	typedef faster_hashmap<ShortKey, faster_hashset<ShortKey>>	CustomSwitchCodes;
	CustomSwitchCodes	m_mapCustCodes;
};

