/*!
 * \file IDataWriter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据落地接口定义
 */
#pragma once
#include <stdint.h>
#include "WTSTypes.h"
#include "FasterDefs.h"

typedef faster_hashset<std::string> CodeSet;

NS_OTP_BEGIN
class WTSTickData;
class WTSOrdQueData;
class WTSOrdDtlData;
class WTSTransData;
class WTSVariant;
class IBaseDataMgr;

class IDataWriterSink
{
public:
	
	virtual IBaseDataMgr* getBDMgr() = 0;

	virtual bool canSessionReceive(const char* sid) = 0;

	virtual void broadcastTick(WTSTickData* curTick) = 0;

	virtual void broadcastOrdQue(WTSOrdQueData* curOrdQue) = 0;

	virtual void broadcastOrdDtl(WTSOrdDtlData* curOrdDtl) = 0;

	virtual void broadcastTrans(WTSTransData* curTrans) = 0;

	virtual CodeSet* getSessionComms(const char* sid) = 0;

	virtual uint32_t getTradingDate(const char* pid) = 0;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputWriterLog(WTSLogLevel ll, const char* format, ...) = 0;
};

/*
 *	数据落地接口
 */
class IDataWriter
{
public:
	virtual bool init(WTSVariant* params, IDataWriterSink* sink) = 0;
	virtual void release() = 0;

public:
	virtual bool writeTick(WTSTickData* curTick, bool bNeedSlice = true) = 0;

	virtual bool writeOrderQueue(WTSOrdQueData* curOrdQue) = 0;

	virtual bool writeOrderDetail(WTSOrdDtlData* curOrdDetail) = 0;

	virtual bool writeTransaction(WTSTransData* curTrans) = 0;

	virtual void transHisData(const char* sid) = 0;

	virtual bool isSessionProceeded(const char* sid) = 0;

	virtual WTSTickData* getCurTick(const char* code, const char* exchg = "") = 0;
};

NS_OTP_END


//获取IDataWriter的函数指针类型
typedef otp::IDataWriter* (*FuncCreateWriter)();
typedef void(*FuncDeleteWriter)(otp::IDataWriter* &writer);