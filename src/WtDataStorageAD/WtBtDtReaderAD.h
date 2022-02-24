#pragma once
#include <string>

#include "../Includes/FasterDefs.h"
#include "../Includes/IBtDtReader.h"

#include "../WTSUtils/WtLMDB.hpp"

NS_WTP_BEGIN

class WtBtDtReaderAD : public IBtDtReader
{
public:
	WtBtDtReaderAD();
	virtual ~WtBtDtReaderAD();	

//////////////////////////////////////////////////////////////////////////
//IBtDtReader
public:
	virtual void init(WTSVariant* cfg, IBtDtReaderSink* sink);

	virtual bool read_raw_bars(const char* exchg, const char* code, WTSKlinePeriod period, std::string& buffer) override;
	virtual bool read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;

private:
	std::string		_base_dir;

private:
	//////////////////////////////////////////////////////////////////////////
	/*
	 *	这里放LMDB的数据库定义
	 *	K线数据，按照每个市场m1/m5/d1三个周期一共三个数据库，路径如./m1/CFFEX
	 *	Tick数据，每个合约一个数据库，路径如./ticks/CFFEX/IF2101
	 */
	typedef std::shared_ptr<WtLMDB> WtLMDBPtr;
	typedef faster_hashmap<std::string, WtLMDBPtr> WtLMDBMap;

	WtLMDBMap	_exchg_m1_dbs;
	WtLMDBMap	_exchg_m5_dbs;
	WtLMDBMap	_exchg_d1_dbs;

	//用exchg.code作为key，如BINANCE.BTCUSDT
	WtLMDBMap	_tick_dbs;

	WtLMDBPtr	get_k_db(const char* exchg, WTSKlinePeriod period);

	WtLMDBPtr	get_t_db(const char* exchg, const char* code);
};

NS_WTP_END