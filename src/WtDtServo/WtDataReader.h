#pragma once
#include <string>
#include <stdint.h>

#include "../Includes/FasterDefs.h"

#include "../WtDataStorage/DataDefine.h"
#include "../Share/BoostMappingFile.hpp"
#include "../Share/StdUtils.hpp"

NS_OTP_BEGIN
class WTSVariant;
class WTSTickSlice;
class WTSKlineSlice;
class WTSOrdDtlSlice;
class WTSOrdQueSlice;
class WTSTransSlice;
class WTSArray;

class IBaseDataMgr;
class IHotMgr;
typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtDataReader
{
public:
	WtDataReader();
	virtual ~WtDataReader();

private:
	typedef struct _RTKBlockPair
	{
		StdUniqueMutex*	_mtx;
		RTKlineBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;
		uint64_t		_last_time;

		_RTKBlockPair()
		{
			_mtx = new StdUniqueMutex();
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
			_last_time = 0;
		}
		~_RTKBlockPair() { delete _mtx; }

	} RTKlineBlockPair;
	typedef faster_hashmap<std::string, RTKlineBlockPair>	RTKBlockFilesMap;

	typedef struct _TBlockPair
	{
		StdUniqueMutex*	_mtx;
		RTTickBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;
		uint64_t		_last_time;

		_TBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
			_last_time = 0;
			_mtx = new StdUniqueMutex();
		}
		~_TBlockPair() { delete _mtx; }
	} TickBlockPair;
	typedef faster_hashmap<std::string, TickBlockPair>	TBlockFilesMap;

	typedef struct _TransBlockPair
	{
		StdUniqueMutex*	_mtx;
		RTTransBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;
		uint64_t		_last_time;

		_TransBlockPair()
		{
			_mtx = new StdUniqueMutex();
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
			_last_time = 0;
		}
		~_TransBlockPair() { delete _mtx; }
	} TransBlockPair;
	typedef faster_hashmap<std::string, TransBlockPair>	TransBlockFilesMap;

	typedef struct _OdeDtlBlockPair
	{
		StdUniqueMutex*	_mtx;
		RTOrdDtlBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;
		uint64_t		_last_time;

		_OdeDtlBlockPair()
		{
			_mtx = new StdUniqueMutex();
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
			_last_time = 0;
		}
		~_OdeDtlBlockPair() { delete _mtx; }
	} OrdDtlBlockPair;
	typedef faster_hashmap<std::string, OrdDtlBlockPair>	OrdDtlBlockFilesMap;

	typedef struct _OdeQueBlockPair
	{
		StdUniqueMutex*	_mtx;
		RTOrdQueBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;
		uint64_t		_last_time;

		_OdeQueBlockPair()
		{
			_mtx = new StdUniqueMutex();
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
			_last_time = 0;
		}
		~_OdeQueBlockPair() { delete _mtx; }
	} OrdQueBlockPair;
	typedef faster_hashmap<std::string, OrdQueBlockPair>	OrdQueBlockFilesMap;

	RTKBlockFilesMap	_rt_min1_map;
	RTKBlockFilesMap	_rt_min5_map;

	TBlockFilesMap		_rt_tick_map;
	TransBlockFilesMap	_rt_trans_map;
	OrdDtlBlockFilesMap	_rt_orddtl_map;
	OrdQueBlockFilesMap	_rt_ordque_map;

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

	typedef faster_hashmap<std::string, HisTBlockPair>	HisTickBlockMap;

	typedef struct _HisTransBlockPair
	{
		HisTransBlock*	_block;
		uint64_t		_date;
		std::string		_buffer;

		_HisTransBlockPair()
		{
			_block = NULL;
			_date = 0;
			_buffer.clear();
		}
	} HisTransBlockPair;

	typedef faster_hashmap<std::string, HisTransBlockPair>	HisTransBlockMap;

	typedef struct _HisOrdDtlBlockPair
	{
		HisOrdDtlBlock*	_block;
		uint64_t		_date;
		std::string		_buffer;

		_HisOrdDtlBlockPair()
		{
			_block = NULL;
			_date = 0;
			_buffer.clear();
		}
	} HisOrdDtlBlockPair;

	typedef faster_hashmap<std::string, HisOrdDtlBlockPair>	HisOrdDtlBlockMap;

	typedef struct _HisOrdQueBlockPair
	{
		HisOrdQueBlock*	_block;
		uint64_t		_date;
		std::string		_buffer;

		_HisOrdQueBlockPair()
		{
			_block = NULL;
			_date = 0;
			_buffer.clear();
		}
	} HisOrdQueBlockPair;

	typedef faster_hashmap<std::string, HisOrdQueBlockPair>	HisOrdQueBlockMap;

	HisTickBlockMap		_his_tick_map;
	HisOrdDtlBlockMap	_his_orddtl_map;
	HisOrdQueBlockMap	_his_ordque_map;
	HisTransBlockMap	_his_trans_map;

private:
	RTKlineBlockPair* getRTKilneBlock(const char* exchg, const char* code, WTSKlinePeriod period);
	TickBlockPair* getRTTickBlock(const char* exchg, const char* code);
	OrdQueBlockPair* getRTOrdQueBlock(const char* exchg, const char* code);
	OrdDtlBlockPair* getRTOrdDtlBlock(const char* exchg, const char* code);
	TransBlockPair* getRTTransBlock(const char* exchg, const char* code);

	/*
	 *	将历史数据放入缓存
	 */
	bool		cacheHisBarsFromFile(const std::string& key, const char* stdCode, WTSKlinePeriod period);

	uint32_t		readBarsFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, std::vector<WTSBarStruct>& ayBars, bool isDay = false);
	WTSBarStruct*	indexBarFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, uint32_t& count, bool isDay = false);

	WTSBarStruct*	indexBarFromCacheByCount(const std::string& key, uint64_t etime, uint32_t& count, bool isDay = false);

	bool	loadStkAdjFactorsFromFile(const char* adjfile);

public:
	void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, IHotMgr* hotMgr);

	WTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0);

	WTSArray*		readTickSlicesByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0);
	WTSKlineSlice*	readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime = 0);

	WTSArray*		readTickSlicesByCount(const char* stdCode, uint32_t count, uint64_t etime = 0);
	WTSKlineSlice*	readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0);

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;
	StdThreadPtr	_thrd_check;
	bool			_stopped;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		WTSKlinePeriod	_period;
		std::string		_raw_code;

		std::vector<WTSBarStruct>	_bars;
	} BarsList;

	typedef faster_hashmap<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	//除权因子
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef faster_hashmap<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	inline const AdjFactorList& getAdjFactors(const char* code, const char* exchg, const char* pid)
	{
		char key[20] = { 0 };
		sprintf(key, "%s.%s.%s", exchg, pid, code);
		return _adj_factors[key];
	}
};

NS_OTP_END