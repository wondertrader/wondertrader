/*!
 * \file WtDtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

#include "WtDataManager.h"

NS_OTP_BEGIN
class WTSVariant;
class WtDataStorage;
class WTSKlineSlice;
class WTSTickSlice;
NS_OTP_END

class WtDtRunner
{
public:
	WtDtRunner();
	~WtDtRunner();

public:
	void	initialize(const char* cfgFile, const char* logCfg, const char* modDir = "");

	inline WTSBaseDataMgr& getBaseDataMgr() { return m_baseDataMgr; }
	inline WTSHotMgr& getHotMgr() { return m_hotMgr; }

public:
	WTSKlineSlice*	get_bars(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	WTSTickSlice*	get_ticks(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

private:
	void	initDataMgr(WTSVariant* config);

private:

	WTSBaseDataMgr	m_baseDataMgr;
	WTSHotMgr		m_hotMgr;

	WtDataStorage*		_data_store;
	WtDataManager		_data_mgr;
};

