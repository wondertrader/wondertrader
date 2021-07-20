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

#include "../Includes/IDataManager.h"
#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"

#include "WtDataReader.h"

class WtDtRunner;

NS_OTP_BEGIN
class WTSVariant;
class WTSTickData;
class WTSKlineSlice;
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

	WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSKlineSlice* get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0);

private:
	WtDataReader	_reader;
	IBaseDataMgr*	_bd_mgr;
	IHotMgr*		_hot_mgr;
	WtDtRunner*		_runner;

	typedef WTSHashMap<std::string> DataCacheMap;
	DataCacheMap*	_bars_cache;	//KÏß»º´æ
};

NS_OTP_END