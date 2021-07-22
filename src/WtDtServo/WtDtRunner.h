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
	void	initialize(const char* cfgFile, bool isFile = true, const char* modDir = "");

	inline WTSBaseDataMgr& getBaseDataMgr() { return m_baseDataMgr; }
	inline WTSHotMgr& getHotMgr() { return m_hotMgr; }

public:
	WTSKlineSlice*	get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	WTSArray*		get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

	WTSKlineSlice*	get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime = 0);

	WTSArray*		get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime = 0);

private:
	void	initDataMgr(WTSVariant* config);

private:

	WTSBaseDataMgr	m_baseDataMgr;
	WTSHotMgr		m_hotMgr;

	WtDataStorage*		_data_store;
	WtDataManager		_data_mgr;
};

