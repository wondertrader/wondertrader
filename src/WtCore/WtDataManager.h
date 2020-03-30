#pragma once
#include <vector>
#include <unordered_set>
#include "IDataReader.h"

#include "../Share/WTSCollection.hpp"

NS_OTP_BEGIN
class WTSVariant;
class WTSHisTickData;
class WTSKlineData;
class WTSTickData;
class WTSKlineSlice;
class WTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class WtEngine;

class WtDataManager : public IDataReaderSink
{
public:
	WtDataManager();
	~WtDataManager();

private:
	bool	initStore(WTSVariant* cfg);

public:
	bool	init(WTSVariant* cfg, WtEngine* engine);

	void	handle_push_quote(const char* stdCode, WTSTickData* newTick);

	WTSHisTickData* get_ticks(const char* code, uint32_t count);
	//WTSKlineData*	get_bars(const char* code, WTSKlinePeriod period, uint32_t times, uint32_t count);

	WTSTickSlice*	get_tick_slice(const char* code, uint32_t count);
	WTSKlineSlice*	get_kline_slice(const char* code, WTSKlinePeriod period, uint32_t times, uint32_t count);

	WTSTickData*	grab_last_tick(const char* code);

	//////////////////////////////////////////////////////////////////////////
	//IDataStoreListener
	virtual void	on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar) override;
	virtual void	on_all_bar_updated(uint32_t updateTime) override;

	virtual IBaseDataMgr*	get_basedata_mgr() override;
	virtual IHotMgr*		get_hot_mgr() override;
	virtual uint32_t	get_date() override;
	virtual uint32_t	get_min_time()override;
	virtual uint32_t	get_secs() override;

	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) override;

	IDataReader*	reader() { return _reader; }

private:
	IDataReader*	_reader;
	WtEngine*		_engine;

	std::unordered_set<std::string> _subed_basic_bars;
	typedef WTSHashMap<std::string> DataCacheMap;
	DataCacheMap*	_bars_cache;	//K线缓存
	DataCacheMap*	_ticks_cache;	//历史Tick缓存
	DataCacheMap*	_rt_tick_map;	//实时tick缓存

	typedef struct _NotifyItem
	{
		std::string _code;
		std::string _period;
		uint32_t	_times;
		WTSBarStruct* _newBar;
	} NotifyItem;

	std::vector<NotifyItem> _bar_notifies;
};

NS_OTP_END