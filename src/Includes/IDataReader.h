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

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSTypes.h"

NS_OTP_BEGIN
class WTSKlineData;
class WTSKlineSlice;
class WTSTickSlice;
class WTSOrdQueSlice;
class WTSOrdDtlSlice;
class WTSTransSlice;
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
	 *	@param stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param period	K线周期
	 *	@param newBar	闭合的K线结构指针
	 */
	virtual void on_bar(const char* stdCode, WTSKlinePeriod period, WTSBarStruct* newBar) = 0;

	/*
	 *	@brief	所有缓存的K线全部更新的事件回调
	 *
	 *	@param updateTime	K线更新时间,精确到分钟,如202004101500
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
	 *	@brief	获取当前日期,格式图20100410
	 */
	virtual uint32_t	get_date() = 0;

	/*
	 *	@brief	获取当前1分钟线的时间
	 *	@details 这里的分钟线时间是处理过的1分钟线时间,如现在是9:00:32秒,真实事件为0900,但是对应的1分钟线时间为0901
	 */
	virtual uint32_t	get_min_time() = 0;

	/*
	 *	@brief	获取当前的秒数,精确到毫秒,如37,500
	 */
	virtual uint32_t	get_secs() = 0;

	/*
	 *	@brief	输出数据读取模块的日志
	 */
	virtual void		reader_log(WTSLogLevel ll, const char* fmt, ...) = 0;
};


/*
 *	历史数据加载器的回调函数
 *	@obj	回传用的，原样返回即可
 *	@key	数据缓存的key
 *	@bars	K线数据
 *	@count	K线条数
 *	@factor	复权因子，最新的复权因子，如果是后复权，则factor不为1.0，如果是前复权，则factor必须为1.0
 */
typedef void(*FuncReadBars)(void* obj, WTSBarStruct* bars, uint32_t count);

/*
 *	加载复权因子回调
 *	@obj	回传用的，原样返回即可
 *	@stdCode	合约代码
 *	@dates	
 */
typedef void(*FuncReadFactors)(void* obj, const char* stdCode, uint32_t* dates, double* factors, uint32_t count);

/*
 *	历史数据加载器
 */
class IHisDataLoader
{
public:
	/*
	 *	加载最终历史K线数据
	 *	和loadRawHisBars的区别在于，loadFinalHisBars，系统认为是最终所需数据，不在进行加工，例如复权数据、主力合约数据
	 *	loadRawHisBars是加载未加工的原始数据的接口
	 *
	 *	@obj	回传用的，原样返回即可
	 *	@key	数据缓存的key
	 *	@stdCode	合约代码
	 *	@period	K线周期
	 *	@cb		回调函数
	 */
	virtual bool loadFinalHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	加载原始历史K线数据
	 *
	 *	@obj	回传用的，原样返回即可
	 *	@key	数据缓存的key
	 *	@stdCode	合约代码
	 *	@period	K线周期
	 *	@cb		回调函数
	 */
	virtual bool loadRawHisBars(void* obj, const char* stdCode, WTSKlinePeriod period, FuncReadBars cb) = 0;

	/*
	 *	加载全部除权因子
	 */
	virtual bool loadAllAdjFactors(void* obj, FuncReadFactors cb) = 0;

	/*
	 *	加指定合约除权因子
	 */
	virtual bool loadAdjFactors(void* obj, const char* stdCode, FuncReadFactors cb) = 0;
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
	virtual void init(WTSVariant* cfg, IDataReaderSink* sink, IHisDataLoader* loader = NULL) { _sink = sink; _loader = loader; }

	/*
	 *	@brief	分钟线闭合事件处理接口
	 *	
	 *	@param uDate	闭合的分钟线日期,如20200410,这里不是交易日
	 *	@param uTime	闭合的分钟线的分钟时间,如1115
	 *	@param endTDate	如果闭合的分钟线是交易日最后一条分钟线,则endTDate为当前交易日,如20200410,其他情况为0
	 */
	virtual void onMinuteEnd(uint32_t uDate, uint32_t uTime, uint32_t endTDate = 0) = 0;

	/*
	 *	@brief	读取tick数据切片
	 *	@details 切片不会复制数据,只把缓存中的数据指针传递出来,所以叫做切片
	 *
	 *	@param stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间,精确到毫秒,格式如yyyyMMddhhmmssmmm,如果要读取到最后一条,etime为0,默认为0
	 */
	virtual WTSTickSlice*	readTickSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief	读取逐笔委托数据切片
	 *	@details 切片不会复制数据,只把缓存中的数据指针传递出来,所以叫做切片
	 *
	 *	@param stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间,精确到毫秒,格式如yyyyMMddhhmmssmmm,如果要读取到最后一条,etime为0,默认为0
	 */
	virtual WTSOrdDtlSlice*	readOrdDtlSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief	读取委托队列数据切片
	 *	@details 切片不会复制数据,只把缓存中的数据指针传递出来,所以叫做切片
	 *
	 *	@param stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间,精确到毫秒,格式如yyyyMMddhhmmssmmm,如果要读取到最后一条,etime为0,默认为0
	 */
	virtual WTSOrdQueSlice*	readOrdQueSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief	读取逐笔成交数据切片
	 *	@details 切片不会复制数据,只把缓存中的数据指针传递出来,所以叫做切片
	 *
	 *	@param stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param count	要读取的tick条数
	 *	@param etime	结束时间,精确到毫秒,格式如yyyyMMddhhmmssmmm,如果要读取到最后一条,etime为0,默认为0
	 */
	virtual WTSTransSlice*	readTransSlice(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief 读取K线序列,并返回一个存储容器类
	 *	@details	切片不会复制数据,只把缓存中的数据指针传递出来,所以叫做切片
	 *
	 *	@param	stdCode	标准品种代码,如SSE.600000,SHFE.au.2005
	 *	@param	period	K线周期
	 *	@param	count	要读取的K线条数
	 *	@param	etime	结束时间,格式yyyyMMddhhmm
	 */
	virtual WTSKlineSlice*	readKlineSlice(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	@brief 获取个股指定日期的复权因子
	 *
	 *	@param	stdCode	标准品种代码,如SSE.600000
	 *	@param	date	指定日期,格式yyyyMMdd，默认为0，为0则按当前日期处理
	 */
	virtual double		getAdjFactorByDate(const char* stdCode, uint32_t date = 0) { return 1.0; }

protected:
	IDataReaderSink*	_sink;
	IHisDataLoader*		_loader;
};

//创建数据存储对象
typedef IDataReader* (*FuncCreateDataReader)();
//删除数据存储对象
typedef void(*FuncDeleteDataReader)(IDataReader* store);

NS_OTP_END