/*!
 * \file WTSHotMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 主力合约管理器实现
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

//换月主力映射
typedef WTSMap<uint32_t>			WTSDateHotMap;
//品种主力映射
typedef WTSHashMap<std::string>		WTSProductHotMap;
//分市场主力映射
typedef WTSHashMap<std::string>		WTSExchgHotMap;

//自定义切换规则映射
typedef WTSHashMap<std::string>		WTSCustomSwitchMap;

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
	//主力接口
	virtual const char* getRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool	isHot(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool	splitHotSecions(const char* exchg, const char* pid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//次主力接口
	virtual const char* getSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual const char* getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt = 0) override;

	virtual bool		isSecond(const char* exchg, const char* rawCode, uint32_t dt = 0) override;

	virtual bool		splitSecondSecions(const char* exchg, const char* hotCode, uint32_t sDt, uint32_t eDt, HotSections& sections) override;

	//////////////////////////////////////////////////////////////////////////
	//通用接口
	virtual const char* getCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual const char* getPrevCustomRawCode(const char* tag, const char* fullPid, uint32_t dt) override;

	virtual bool		isCustomHot(const char* tag, const char* fullCode, uint32_t dt) override;

	virtual bool		splitCustomSections(const char* tag, const char* fullPid, uint32_t sDt, uint32_t eDt, HotSections& sections) override;


private:
	//WTSExchgHotMap*	m_pExchgHotMap;
	//WTSExchgHotMap*	m_pExchgScndMap;
	//wt_hashset<std::string>	m_curHotCodes;
	//wt_hashset<std::string>	m_curSecCodes;
	bool			m_bInitialized;

	WTSCustomSwitchMap*	m_mapCustRules;
	typedef wt_hashmap<std::string, wt_hashset<std::string>>	CustomSwitchCodes;
	CustomSwitchCodes	m_mapCustCodes;
};

