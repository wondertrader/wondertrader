#pragma once
#include <string>
#include <stdint.h>
#include <unordered_map>

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

	//除权因子
	typedef struct _AdjFactor
	{
		uint32_t	_date;
		double		_factor;
	} AdjFactor;
	typedef std::vector<AdjFactor> AdjFactorList;
	typedef std::unordered_map<std::string, AdjFactorList>	AdjFactorMap;
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