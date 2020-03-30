/*!
 * \file IDataReader.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>

#include "../Share/WTSMarcos.h"
#include "../Share/WTSTypes.h"

NS_OTP_BEGIN
class WTSHisTickData;
class WTSKlineData;
class WTSKlineSlice;
class WTSTickSlice;
struct WTSBarStruct;
class WTSVariant;
class IBaseDataMgr;
class IHotMgr;

class IDataReaderSink
{
public:
	virtual void on_bar(const char* code, WTSKlinePeriod period, WTSBarStruct* newBar) = 0;
	virtual void on_all_bar_updated(uint32_t updateTime) = 0;

	virtual IBaseDataMgr*	get_basedata_mgr() = 0;
	virtual IHotMgr*		get_hot_mgr() = 0;
	virtual uint32_t	get_date() = 0;
	virtual uint32_t	get_min_time() = 0;
	virtual uint32_t	get_secs() = 0;

	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) = 0;
};


class IDataReader
{
public:
	IDataReader(){}
	virtual ~IDataReader(){}

public:
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink){ _sink = sink; }

	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) = 0;

	virtual WTSHisTickData* readTicks(const char* stdCode, uint32_t count, uint64_t etime = 0, bool bOnlyValid = false) = 0;
	virtual WTSKlineData*	readBars(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

protected:
	IDataReaderSink* _sink;
};

//创建数据存储对象
typedef IDataReader* (*FuncCreateDataReader)();
//删除数据存储对象
typedef void(*FuncDeleteDataReader)(IDataReader* store);

NS_OTP_END