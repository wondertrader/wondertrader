/*!
 * \file HisDataReplayer.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <unordered_map>

#include "DataDefine.h"

#include "../Share/WTSMarcos.h"
#include "../Share/WTSTypes.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

NS_OTP_BEGIN
class WTSTickData;
class WTSVariant;
class WTSKlineSlice;
class WTSTickSlice;
class WTSHisTickData;
class WTSSessionInfo;
class WTSCommodityInfo;
NS_OTP_END

USING_NS_OTP;

class IDataSink
{
public:
	virtual void	handle_tick(const char* stdCode, WTSTickData* curTick) = 0;
	virtual void	handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual void	handle_schedule(uint32_t uDate, uint32_t uTime) = 0;

	virtual void	handle_init() = 0;
	virtual void	handle_session_begin() = 0;
	virtual void	handle_session_end() = 0;
};

class HisDataReplayer
{

private:
	typedef struct _TickList
	{
		std::string		_code;
		uint32_t		_date;
		uint32_t		_cursor;
		uint32_t		_count;

		std::vector<WTSTickStruct> _ticks;

		_TickList() :_cursor(UINT_MAX), _count(0), _date(0){}
	} TickList;

	typedef std::unordered_map<std::string, TickList>	TickCache;


	typedef struct _BarsList
	{
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_cursor;
		uint32_t		_count;
		uint32_t		_times;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_cursor(UINT_MAX), _count(0), _times(1){}
	} BarsList;

	typedef std::unordered_map<std::string, BarsList>	BarsCache;


public:
	HisDataReplayer();
	~HisDataReplayer();

private:
	/*
	*	将历史数据放入缓存
	*/
	bool		cacheRawBars(const std::string& key, const char* stdCode, WTSKlinePeriod period);

	bool		cacheRawBarsFromCSV(const std::string& key, const char* stdCode, WTSKlinePeriod period);

	bool		cacheRawTicks(const std::string& key, const char* stdCode, uint32_t uDate);

	bool		cacheRawTicksFromCSV(const std::string& key, const char* stdCode, uint32_t uDate);

	void		onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0);

	void		loadFees(const char* filename);

	void		replayTicks(uint64_t stime, uint64_t etime);

	bool		checkTicks(const char* stdCode, uint32_t uDate);

public:
	bool init(WTSVariant* cfg);

	void run();

	void register_sink(IDataSink* listener){ _listener = listener; }

	WTSKlineSlice* get_kline_slice(const char* stdCode, const char* period, uint32_t count, uint32_t times = 1, bool isMain = false);

	WTSHisTickData* get_ticks(const char* stdCode, uint32_t count, uint64_t etime);

	WTSTickSlice* get_tick_slice(const char* stdCode, uint32_t count);

	WTSTickData* get_last_tick(const char* stdCode);

	uint32_t get_date() const{ return _cur_date; }
	uint32_t get_min_time() const{ return _cur_time; }
	uint32_t get_raw_time() const{ return _cur_time; }
	uint32_t get_secs() const{ return _cur_secs; }
	uint32_t get_trading_date() const{ return _cur_tdate; }

	double calc_fee(const char* stdCode, double price, double qty, uint32_t offset);
	WTSSessionInfo*		get_session_info(const char* sid, bool isCode = false);
	WTSCommodityInfo*	get_commodity_info(const char* stdCode);
	double get_cur_price(const char* stdCode);
	void sub_tick(uint32_t sid, const char* stdCode);

	bool	is_tick_enabled() const{ return _tick_enabled; }

private:
	IDataSink*		_listener;

	TickCache		_ticks_cache;
	BarsCache		_bars_cache;

	std::string		_main_key;
	bool			_tick_enabled;
	std::map<std::string, WTSTickStruct>	_day_cache;	//每日Tick缓存,当tick回放未开放时，会用到该缓存
	std::map<std::string, std::string>		_ticker_keys;

	uint32_t		_cur_date;
	uint32_t		_cur_time;
	uint32_t		_cur_secs;
	uint32_t		_cur_tdate;
	uint32_t		_closed_tdate;
	uint32_t		_opened_tdate;

	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;

	std::string		_base_dir;
	std::string		_mode;
	uint64_t		_begin_time;
	uint64_t		_end_time;

	//////////////////////////////////////////////////////////////////////////
	//手续费模板
	typedef struct _FeeItem
	{
		double	_open;
		double	_close;
		double	_close_today;
		bool	_by_volumn;

		_FeeItem()
		{
			memset(this, 0, sizeof(_FeeItem));
		}
	} FeeItem;
	typedef std::unordered_map<std::string, FeeItem>	FeeMap;
	FeeMap		_fee_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_map<std::string, double> PriceMap;
	PriceMap		_price_map;

	//////////////////////////////////////////////////////////////////////////
	//
	typedef std::unordered_set<uint32_t> SIDSet;
	typedef std::unordered_map<std::string, SIDSet>	StraSubMap;
	StraSubMap		_tick_sub_map;	//tick数据订阅表

	std::unordered_set<std::string>		_subed_raw_codes;	//tick订阅表（真实代码模式）
};

