#pragma once
#include <string>
#include <stdint.h>
#include <unordered_map>

#include "../WtCore/IDataReader.h"

#include "../WtDataWriter/DataDefine.h"
#include "../Share/BoostMappingFile.hpp"

NS_OTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtDataReader : public IDataReader
{
public:
	WtDataReader();
	virtual ~WtDataReader();

private:
	typedef struct _RTKBlockPair
	{
		RTKlineBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;

		_RTKBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
		}

	} RTKBlockPair;
	typedef std::unordered_map<std::string, RTKBlockPair>	RTKBlockFilesMap;

	typedef struct _TBlockPair
	{
		RTTickBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;

		_TBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
		}
	} TBlockPair;
	typedef std::unordered_map<std::string, TBlockPair>	TBlockFilesMap;

	RTKBlockFilesMap	_rt_min1_map;
	RTKBlockFilesMap	_rt_min5_map;

	TBlockFilesMap		_rt_tick_map;

	typedef struct _HisTBlockPair
	{
		HisTickBlock*	_block;
		uint64_t		_date;
		std::string		_buffer;

		_HisTBlockPair()
		{
			_block = NULL;
			_date = 0;
			_buffer.clear();
		}
	} HisTBlockPair;

	typedef std::unordered_map<std::string, HisTBlockPair>	HisTBlockMap;
	HisTBlockMap		_his_tick_map;

private:
	RTKBlockPair* getRTKBlock(const char* exchg, const char* code, WTSKlinePeriod period);
	TBlockPair* getRTTBlock(const char* exchg, const char* code);

	/*
	 *	将历史数据放入缓存
	 */
	bool		cacheHisBars(const std::string& key, const char* stdCode, WTSKlinePeriod period);
	uint32_t	readBarsFromCache(const std::string& key, uint64_t etime, uint32_t count, std::vector<WTSBarStruct>& ayBars, bool isDay = false);
	WTSBarStruct*	indexBarFromCache(const std::string& key, uint64_t etime, uint32_t& count, bool isDay = false);

public:
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink) override;

	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) override;

	virtual WTSHisTickData* readTicks(const char* stdCode, uint32_t count, uint64_t etime = 0, bool bOnlyValid = false) override;

	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		WTSKlinePeriod	_period;
		uint32_t		_his_cursor;
		uint32_t		_rt_cursor;
		std::string		_raw_code;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_his_cursor(UINT_MAX), _rt_cursor(UINT_MAX){}
	} BarsList;

	typedef std::unordered_map<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	uint64_t	_last_time;
};

NS_OTP_END