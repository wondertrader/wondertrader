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

/*
 *	@brief 数据读取模块回调接口
 *	@details 主要用于数据读取模块向调用模块回调
 */
class IDataReaderSink
{
public:
	/*
	 *	@brief	K线闭合事件回调
	 *	
	 *	@param stdCode	标准品种代码，如SSE.600000，SHFE.au.2005
	 *	@param period	K线周期
	 *	@param newBar	闭合的K线结构指针
	 */
	virtual void on_bar(const char* stdCode, WTSKlinePeriod period, WTSBarStruct* newBar) = 0;

	/*
	 *	@brief	所有缓存的K线全部更新的事件回调
	 *
	 *	@param updateTime	K线更新时间，精确到分钟，如202004101500
	 */
	virtual void on_all_bar_updated(uint32_t updateTime) = 0;

	/*
	 *	@brief	获取基础数据管理接口指针
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() = 0;

	 /*
	  *	@brief	获取主力切换规则管理接口指针
	  */
	virtual IHotMgr*		get_hot_mgr() = 0;

	/*
	 *	@brief	获取当前日期，格式图20100410
	 */
	virtual uint32_t	get_date() = 0;

	/*
	 *	@brief	获取当前1分钟线的时间
	 *	@details 这里的分钟线时间是处理过的1分钟线时间，如现在是9:00:32秒，真实事件为0900，但是对应的1分钟线时间为0901
	 */
	virtual uint32_t	get_min_time() = 0;

	/*
	 *	@brief	获取当前的秒数，精确到毫秒，如37,500
	 */
	virtual uint32_t	get_secs() = 0;

	/*
	 *	@brief	输出数据读取模块的日志
	 */
	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) = 0;
};

/*
 *	@brief	数据读取接口
 *
 *	向核心模块提供行情数据(tick、K线)读取接口
 */
class IDataReader
{
public:
	IDataReader(){}
	virtual ~IDataReader(){}

public:
	/*
	 *	@brief 初始化数据读取模块
	 *
	 *	@param cfg	模块配置项
	 *	@param sink	模块回调接口
	 */
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink){ _sink = sink; }

	/*
	 *	@brief	分钟线闭合事件处理接口
	 *	
	 *	@param uDate	闭合的分钟线日期，如20200410，这里不是交易日
	 *	@param uTime	闭合的分钟线的分钟时间，如1115
	 *	@param endTDate	如果闭合的分钟线是交易日最后一条分钟线，则endTDate为当前交易日，如20200410，其他情况为0
	 */
	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) = 0;

	/*
	 *	@brief	读取tick数据，并返回一个存储容器类
	 *	@details	返回值WTSHisTickData包含一个WTSTickStruct的序列，读取的数据要复制到该容器中
	 *	
	 *	@param stdCode	标准品种代码，如SSE.600000，SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间，精确到毫秒，格式如yyyyMMddhhmmssmmm，如果要读取到最后一条，etime为0，默认为0
	 *	@param bOnlyValid	是否只读有效tick(成交量不为0)，默认为false
	 */
	virtual WTSHisTickData* readTicks(const char* stdCode, uint32_t count, uint64_t etime = 0, bool bOnlyValid = false) = 0;

	/*
	 *	@brief 读取K线序列，并返回一个存储容器类
	 *	@details	返回的WTSKlineData包含了WTSBarStruct的序列，读取的数据要复制到该序列中
	 *	
	 *	@param	stdCode	标准品种代码，如SSE.600000，SHFE.au.2005
	 *	@param	period	K线周期
	 *	@param	count	要读取的K线条数
	 *	@param	etime	结束时间，如果是日线数据，则格式为yyyyMMdd，如果是分钟线，则为yyyyMMddhhmm
	 */
	virtual WTSKlineData*	readBars(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief	读取tick数据切片
	 *	@details 切片不会复制数据，只把缓存中的数据指针传递出来，所以叫做切片
	 *
	 *	@param stdCode	标准品种代码，如SSE.600000，SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间，精确到毫秒，格式如yyyyMMddhhmmssmmm，如果要读取到最后一条，etime为0，默认为0
	 */
	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief 读取K线序列，并返回一个存储容器类
	 *	@details	切片不会复制数据，只把缓存中的数据指针传递出来，所以叫做切片
	 *
	 *	@param	stdCode	标准品种代码，如SSE.600000，SHFE.au.2005
	 *	@param	period	K线周期
	 *	@param	count	要读取的K线条数
	 *	@param	etime	结束时间，如果是日线数据，则格式为yyyyMMdd，如果是分钟线，则为yyyyMMddhhmm
	 */
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

protected:
	IDataReaderSink* _sink;
};

//创建数据存储对象
typedef IDataReader* (*FuncCreateDataReader)();
//删除数据存储对象
typedef void(*FuncDeleteDataReader)(IDataReader* store);

NS_OTP_END