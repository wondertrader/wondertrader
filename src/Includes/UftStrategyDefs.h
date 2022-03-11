/*!
 * \file UftStrategyDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <stdint.h>

#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSVariant;
class IUftStraCtx;
class WTSTickData;
class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;
struct WTSBarStruct;
NS_WTP_END

USING_NS_WTP;

class UftStrategy
{
public:
	UftStrategy(const char* id) :_id(id){}
	virtual ~UftStrategy(){}

public:
	/*
	*	执行单元名称
	*/
	virtual const char* getName() = 0;

	/*
	*	所属执行器工厂名称
	*/
	virtual const char* getFactName() = 0;

	/*
	*	初始化
	*/
	virtual bool init(WTSVariant* cfg){ return true; }

	virtual const char* id() const { return _id.c_str(); }

	//回调函数
	virtual void on_init(IUftStraCtx* ctx) = 0;

	virtual void on_tick(IUftStraCtx* ctx, const char* code, WTSTickData* newTick) {}
	virtual void on_order_queue(IUftStraCtx* ctx, const char* code, WTSOrdQueData* newOrdQue) {}
	virtual void on_order_detail (IUftStraCtx* ctx, const char* code, WTSOrdDtlData* newOrdDtl) {}
	virtual void on_transaction(IUftStraCtx* ctx, const char* code, WTSTransData* newTrans) {}
	virtual void on_bar(IUftStraCtx* ctx, const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) {}

	virtual void on_trade(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price) {}
	virtual void on_position(IUftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) {}
	virtual void on_order(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled) {}
	virtual void on_channel_ready(IUftStraCtx* ctx) {}
	virtual void on_channel_lost(IUftStraCtx* ctx) {}
	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message) {}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//策略工厂接口
typedef void(*FuncEnumUftStrategyCallback)(const char* factName, const char* straName, bool isLast);

class IUftStrategyFact
{
public:
	IUftStrategyFact(){}
	virtual ~IUftStrategyFact(){}

public:
	/*
	*	获取工厂名
	*/
	virtual const char* getName() = 0;

	/*
	*	枚举策略
	*/
	virtual void enumStrategy(FuncEnumUftStrategyCallback cb) = 0;

	/*
	*	根据名称创建执行单元
	*/
	virtual UftStrategy* createStrategy(const char* name, const char* id) = 0;

	/*
	*	删除执行单元
	*/
	virtual bool deleteStrategy(UftStrategy* stra) = 0;
};

//创建执行工厂
typedef IUftStrategyFact* (*FuncCreateUftStraFact)();
//删除执行工厂
typedef void(*FuncDeleteUftStraFact)(IUftStrategyFact* &fact);
