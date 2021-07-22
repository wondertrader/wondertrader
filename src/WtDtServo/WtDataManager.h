/*!
 * \file WtDataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <vector>
#include <stdint.h>

#include "../Includes/IDataManager.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"

#include "WtDataReader.h"

class WtDtRunner;

NS_OTP_BEGIN
class WTSVariant;
class WTSTickData;
class WTSKlineSlice;
class WTSKlineData;
class WTSTickSlice;
class IBaseDataMgr;
class IHotMgr;
class WTSSessionInfo;

class WtDataReader;
struct WTSBarStruct;

class WtDataManager
{
public:
	WtDataManager();
	~WtDataManager();

private:
	bool	initStore(WTSVariant* cfg);

	WTSSessionInfo* get_session_info(const char* sid, bool isCode = false);

public:
	bool	init(WTSVariant* cfg, WtDtRunner* runner);

	void	handle_push_quote(const char* stdCode, WTSTickData* newTick);

	WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSTransSlice* get_transaction_slice(const char* stdCode, uint64_t stime, uint64_t etime = 0);

	WTSArray* get_tick_slices_by_range(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSKlineSlice* get_kline_slice_by_range(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint64_t stime, uint64_t etime = 0);

	WTSArray* get_tick_slices_by_count(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSKlineSlice* get_kline_slice_by_count(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0);

private:
	WtDataReader	_reader;
	IBaseDataMgr*	_bd_mgr;
	IHotMgr*		_hot_mgr;
	WtDtRunner*		_runner;

	//KÏß»º´æ
	typedef struct _BarCache
	{
		WTSKlineData*	_bars;
		uint64_t		_last_bartime;
		WTSKlinePeriod	_period;
		uint32_t		_times;

		_BarCache():_last_bartime(0),_period(KP_DAY),_times(1),_bars(NULL){}
	} BarCache;
	typedef faster_hashmap<std::string, BarCache>	BarCacheMap;
	BarCacheMap	_bars_cache;
};

NS_OTP_END