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
#include "../Includes/IDataReader.h"
#include "../Includes/IDataManager.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/WTSCollection.hpp"

NS_OTP_BEGIN
class WTSVariant;
class WTSTickData;
class WTSKlineSlice;
class WTSTickSlice;
class IBaseDataMgr;
class IBaseDataMgr;
class WtEngine;

class WtDataManager : public IDataReaderSink, public IDataManager
{
public:
	WtDataManager();
	~WtDataManager();

private:
	bool	initStore(WTSVariant* cfg);

public:
	bool	init(WTSVariant* cfg, WtEngine* engine);

	void	regsiter_loader(IHisDataLoader* loader) { _loader = loader; }

	void	handle_push_quote(const char* stdCode, WTSTickData* newTick);

	//////////////////////////////////////////////////////////////////////////
	//IDataManager 接口
	virtual WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSOrdQueSlice* get_order_queue_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSOrdDtlSlice* get_order_detail_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSTransSlice* get_transaction_slice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSKlineSlice* get_kline_slice(const char* stdCode, WTSKlinePeriod period, uint32_t times, uint32_t count, uint64_t etime = 0) override;
	virtual WTSTickData* grab_last_tick(const char* stdCode) override;

	//////////////////////////////////////////////////////////////////////////
	//IDataReaderSink
	virtual void	on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar) override;
	virtual void	on_all_bar_updated(uint32_t updateTime) override;

	virtual IBaseDataMgr*	get_basedata_mgr() override;
	virtual IHotMgr*		get_hot_mgr() override;
	virtual uint32_t	get_date() override;
	virtual uint32_t	get_min_time()override;
	virtual uint32_t	get_secs() override;

	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) override;

	inline IDataReader*	reader() { return _reader; }
	inline IHisDataLoader*	loader() { return _loader; }

private:
	IDataReader*	_reader;
	IHisDataLoader*	_loader;
	WtEngine*		_engine;

	faster_hashset<std::string> _subed_basic_bars;
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