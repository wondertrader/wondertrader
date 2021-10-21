#pragma once
#include <string>
#include <stdint.h>

#include "../Includes/FasterDefs.h"
#include "../Includes/IDataReader.h"

#include "../WtDataWriter/DataDefine.h"
#include "../WtDataWriter/MysqlDB.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_OTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;
typedef std::shared_ptr<MysqlDb>	MysqlDbPtr;

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

	} RTKlineBlockPair;
	typedef faster_hashmap<std::string, RTKlineBlockPair>	RTKBlockFilesMap;

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
	} TickBlockPair;
	typedef faster_hashmap<std::string, TickBlockPair>	TBlockFilesMap;

	typedef struct _TransBlockPair
	{
		RTTransBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;

		std::shared_ptr< std::ofstream>	_fstream;

		_TransBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
		}
	} TransBlockPair;
	typedef faster_hashmap<std::string, TransBlockPair>	TransBlockFilesMap;

	typedef struct _OdrDtlBlockPair
	{
		RTOrdDtlBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;

		std::shared_ptr< std::ofstream>	_fstream;

		_OdrDtlBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
		}
	} OrdDtlBlockPair;
	typedef faster_hashmap<std::string, OrdDtlBlockPair>	OrdDtlBlockFilesMap;

	typedef struct _OdrQueBlockPair
	{
		RTOrdQueBlock*	_block;
		BoostMFPtr		_file;
		uint64_t		_last_cap;

		std::shared_ptr< std::ofstream>	_fstream;

		_OdrQueBlockPair()
		{
			_block = NULL;
			_file = NULL;
			_last_cap = 0;
		}
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
	bool		cacheHisBarsFromDB(const std::string& key, const char* stdCode, WTSKlinePeriod period);

	uint32_t	readBarsFromCache(const std::string& key, uint64_t etime, uint32_t count, std::vector<WTSBarStruct>& ayBars, bool isDay = false);
	WTSBarStruct*	indexBarFromCache(const std::string& key, uint64_t etime, uint32_t& count, bool isDay = false);

	bool	loadStkAdjFactorsFromFile(const char* adjfile);
	bool	loadStkAdjFactorsFromDB();

	void	init_db();

public:
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink) override;

	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) override;

	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSOrdDtlSlice*	readOrdDtlSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSOrdQueSlice*	readOrdQueSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSTransSlice*	readTransSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

	virtual double getAdjFactor(const char* stdCode, uint32_t date = 0) override;

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
		double			_factor;

		_BarsList() :_his_cursor(UINT_MAX), _rt_cursor(UINT_MAX), _factor(DBL_MAX){}
	} BarsList;

	typedef faster_hashmap<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	uint64_t	_last_time;

	//除权因子
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef faster_hashmap<std::string, AdjFactorList>	AdjFactorMap;
	AdjFactorMap	_adj_factors;

	inline const AdjFactorList& getAdjFactors(const char* code, const char* exchg)
	{
		char key[20] = { 0 };
		sprintf(key, "%s.%s", exchg, code);
		return _adj_factors[key];
	}

	typedef struct _DBConfig
	{
		bool	_active;
		char	_host[64];
		int32_t	_port;
		char	_dbname[32];
		char	_user[32];
		char	_pass[32];

		_DBConfig() { memset(this, 0, sizeof(_DBConfig)); }
	} DBConfig;

	DBConfig	_db_conf;
	MysqlDbPtr	_db_conn;
};

NS_OTP_END