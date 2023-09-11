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
#include "../Share/StdUtils.hpp"

#include "PorterDefs.h"
#include "ParserAdapter.h"
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
	void	initialize(const char* cfgFile, bool isFile = true, const char* modDir = "", const char* logCfg = "logcfg.yaml", 
				FuncOnTickCallback cbTick = NULL, FuncOnBarCallback cbBar = NULL);
	void	start();

	inline WTSBaseDataMgr& getBaseDataMgr() { return _bd_mgr; }
	inline WTSHotMgr& getHotMgr() { return _hot_mgr; }

public:
	void	proc_tick(WTSTickData* curTick);
	void	trigger_tick(const char* stdCode, WTSTickData* curTick);
	void	trigger_bar(const char* stdCode, const char* period, WTSBarStruct* lastBar);
	void	sub_tick(const char* stdCode, bool bReplace, bool bInner = false);
	void	sub_bar(const char* stdCode, const char* period);

	void	clear_cache();

public:
	WTSKlineSlice*	get_bars_by_range(const char* stdCode, const char* period, uint64_t beginTime, uint64_t endTime = 0);

	WTSKlineSlice*	get_bars_by_date(const char* stdCode, const char* period, uint32_t uDate = 0);

	WTSTickSlice*	get_ticks_by_range(const char* stdCode, uint64_t beginTime, uint64_t endTime = 0);

	WTSKlineSlice*	get_bars_by_count(const char* stdCode, const char* period, uint32_t count, uint64_t endTime = 0);

	WTSTickSlice*	get_ticks_by_count(const char* stdCode, uint32_t count, uint64_t endTime = 0);

	WTSTickSlice*	get_ticks_by_date(const char* stdCode, uint32_t uDate = 0);

	WTSKlineSlice*	get_sbars_by_date(const char* stdCode, uint32_t secs, uint32_t uDate = 0);

private:
	void	initDataMgr(WTSVariant* config);
	void	initParsers(WTSVariant* cfg);

private:
	FuncOnTickCallback	_cb_tick;
	FuncOnBarCallback	_cb_bar;
	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;

	WtDataStorage*	_data_store;
	WtDataManager	_data_mgr;
	ParserAdapterMgr	_parsers;

	bool			_is_inited;

	typedef std::set<uint32_t> SubFlags;
	typedef wt_hashmap<std::string, SubFlags>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_subs;

	StraSubMap		_tick_innersub_map;	//tick数据订阅表
	StdUniqueMutex	_mtx_innersubs;
};

