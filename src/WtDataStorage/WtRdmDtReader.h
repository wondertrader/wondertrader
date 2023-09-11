#pragma once
#include <string>
#include <stdint.h>
#include <unordered_map>

#include "DataDefine.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IRdmDtReader.h"

#include "../Share/BoostMappingFile.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/fmtlib.h"

NS_WTP_BEGIN
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

class WtRdmDtReader : public IRdmDtReader
{
public:
	WtRdmDtReader();
	virtual ~WtRdmDtReader();

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
	typedef std::unordered_map<std::string, RTKlineBlockPair>	RTKBlockFilesMap;

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
	typedef std::unordered_map<std::string, TickBlockPair>	TBlockFilesMap;

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
	typedef std::unordered_map<std::string, TransBlockPair>	TransBlockFilesMap;

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
	typedef std::unordered_map<std::string, OrdDtlBlockPair>	OrdDtlBlockFilesMap;

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
	typedef std::unordered_map<std::string, OrdQueBlockPair>	OrdQueBlockFilesMap;

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

	typedef std::unordered_map<std::string, HisTBlockPair>	HisTickBlockMap;

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

	typedef std::unordered_map<std::string, HisTransBlockPair>	HisTransBlockMap;

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

	typedef std::unordered_map<std::string, HisOrdDtlBlockPair>	HisOrdDtlBlockMap;

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

	typedef std::unordered_map<std::string, HisOrdQueBlockPair>	HisOrdQueBlockMap;

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
	bool		cacheHisBarsFromFile(void* codeInfo, const std::string& key, const char* stdCode, WTSKlinePeriod period);

	uint32_t		readBarsFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, std::vector<WTSBarStruct>& ayBars, bool isDay = false);
	WTSBarStruct*	indexBarFromCacheByRange(const std::string& key, uint64_t stime, uint64_t etime, uint32_t& count, bool isDay = false);

	WTSBarStruct*	indexBarFromCacheByCount(const std::string& key, uint64_t etime, uint32_t& count, bool isDay = false);

	bool	loadStkAdjFactorsFromFile(const char* adjfile);
	

//////////////////////////////////////////////////////////////////////////
//IRdmDtReader
public:
	virtual void init(WTSVariant* cfg, IRdmDtReaderSink* sink);

	virtual WTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual WTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual WTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;

	virtual WTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) override;

	virtual WTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

	virtual WTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate = 0 ) override;

	virtual double		getAdjFactorByDate(const char* stdCode, uint32_t date = 0) override;

	virtual void		clearCache() override;

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
		double			_factor;

		_BarsList():_factor(1.0){}

		std::vector<WTSBarStruct>	_bars;
		std::vector<WTSBarStruct>	_rt_bars;	//如果是后复权，就需要把实时数据拷贝到这里来
	} BarsList;

	typedef std::unordered_map<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	//除权因子
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef std::unordered_map<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	inline const AdjFactorList& getAdjFactors(const char* code, const char* exchg, const char* pid)
	{
		thread_local static char key[20] = { 0 };
		fmtutil::format_to(key, "{}.{}.{}", exchg, pid, code);
		return _adj_factors[key];
	}
};

NS_WTP_END