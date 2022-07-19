#pragma once
#include <string>
#include <stdint.h>
#include <boost/circular_buffer.hpp>

#include "DataDefineAD.h"

#include "../WTSUtils/WtLMDB.hpp"
#include "../Includes/FasterDefs.h"
#include "../Includes/IRdmDtReader.h"

#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_WTP_BEGIN

typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtRdmDtReaderAD : public IRdmDtReader
{
public:
	WtRdmDtReaderAD();
	virtual ~WtRdmDtReaderAD();


//////////////////////////////////////////////////////////////////////////
//IRdmDtReader
public:
	virtual void init(WTSVariant* cfg, IRdmDtReaderSink* sink);

	virtual WTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual WTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }
	virtual WTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override { return NULL; }

	virtual WTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) override;

	virtual WTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) override;
	virtual WTSKlineSlice*	readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) override;

	virtual WTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate /* = 0 */) override;

private:
	std::string		_base_dir;
	IBaseDataMgr*	_base_data_mgr;
	IHotMgr*		_hot_mgr;

	typedef struct _BarsList
	{
		std::string		_exchg;
		std::string		_code;
		WTSKlinePeriod	_period;
		uint64_t		_last_bar_time;

		std::vector<WTSBarStruct>	_bars;

		_BarsList() :_last_bar_time(0){}
	} BarsList;

	typedef struct _TicksList
	{
		std::string		_exchg;
		std::string		_code;
		uint64_t		_first_tick_time;
		uint64_t		_last_tick_time;

		std::vector<WTSTickStruct>	_ticks;

		_TicksList() :_last_tick_time(0), _first_tick_time(UINT64_MAX){}
	} TicksList;

	typedef faster_hashmap<std::string, BarsList> BarsCache;
	BarsCache	_bars_cache;

	typedef faster_hashmap<std::string, TicksList> TicksCache;
	TicksCache	_ticks_cache;

private:
	//////////////////////////////////////////////////////////////////////////
	/*
	 *	�����LMDB�����ݿⶨ��
	 *	K�����ݣ�����ÿ���г�m1/m5/d1��������һ���������ݿ⣬·����./m1/CFFEX
	 *	Tick���ݣ�ÿ����Լһ�����ݿ⣬·����./ticks/CFFEX/IF2101
	 */
	typedef std::shared_ptr<WtLMDB> WtLMDBPtr;
	typedef faster_hashmap<std::string, WtLMDBPtr> WtLMDBMap;

	WtLMDBMap	_exchg_m1_dbs;
	WtLMDBMap	_exchg_m5_dbs;
	WtLMDBMap	_exchg_d1_dbs;

	//��exchg.code��Ϊkey����BINANCE.BTCUSDT
	WtLMDBMap	_tick_dbs;

	WtLMDBPtr	get_k_db(const char* exchg, WTSKlinePeriod period);

	WtLMDBPtr	get_t_db(const char* exchg, const char* code);
};

NS_WTP_END
