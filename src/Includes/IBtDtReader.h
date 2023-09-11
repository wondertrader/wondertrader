/*!
 * \file IBtDtReader.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>

#include "../Includes/WTSTypes.h"

NS_WTP_BEGIN
class WTSVariant;

/*
 *	@brief 数据读取模块回调接口
 *	@details 主要用于数据读取模块向调用模块回调
 */
class IBtDtReaderSink
{
public:
	/*
	 *	@brief	输出数据读取模块的日志
	 */
	virtual void		reader_log(WTSLogLevel ll, const char* message) = 0;
};

/*
 *	@brief	随机数据读取接口
 *
 *	向核心模块提供行情数据(tick、K线)读取接口
 */
class IBtDtReader
{
public:
	IBtDtReader() :_sink(NULL) {}
	virtual ~IBtDtReader(){}

public:
	virtual void init(WTSVariant* cfg, IBtDtReaderSink* sink) { _sink = sink; }

	virtual bool read_raw_bars(const char* exchg, const char* code, WTSKlinePeriod period, std::string& buffer) = 0;
	virtual bool read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) = 0;

	virtual bool read_raw_order_details(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) { return false; }
	virtual bool read_raw_order_queues(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) { return false; }
	virtual bool read_raw_transactions(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) { return false; }

protected:
	IBtDtReaderSink*	_sink;
};

//创建数据存储对象
typedef IBtDtReader* (*FuncCreateBtDtReader)();
//删除数据存储对象
typedef void(*FuncDeleteBtDtReader)(IBtDtReader* store);

NS_WTP_END