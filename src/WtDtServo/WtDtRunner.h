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

NS_WTP_BEGIN
class WTSVariant;
class WtDataStorage;
class WTSKlineSlice;
class WTSTickSlice;
NS_WTP_END

class WtDtRunner
{
public:
	WtDtRunner();
	~WtDtRunner();

public:
	void	initialize(const char* cfgFile, bool isFile = true, const char* modDir = "");

	inline WTSBaseDataMgr& getBaseDataMgr() { return _bd_mgr; }
	inline WTSHotMgr& getHotMgr() { return _hot_mgr; }

public:
	WTSKlineSlice*	get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	WTSTickSlice*	get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

	WTSKlineSlice*	get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime = 0);

	WTSTickSlice*	get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime = 0);

private:
	void	initDataMgr(WTSVariant* config);

private:

	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;

	WtDataStorage*		_data_store;
	WtDataManager		_data_mgr;

	bool			_is_inited;
};

