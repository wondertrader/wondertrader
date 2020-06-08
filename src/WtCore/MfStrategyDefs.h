/*!
* \file MfStrategyDefs.h
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

#include "../Share/WTSMarcos.h"

NS_OTP_BEGIN
class WTSVariant;
class IMfStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

class MfStrategy
{
public:
	MfStrategy(const char* id) :_id(id){}
	virtual ~MfStrategy(){}

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

	/*
	*	初始化回调
	*/
	virtual void on_init(IMfStraCtx* ctx){}

	/*
	*	主体逻辑执行入口
	*/
	virtual void on_schedule(IMfStraCtx* ctx, uint32_t uDate, uint32_t uTime){}

	/*
	*	tick数据
	*/
	virtual void on_tick(IMfStraCtx* ctx, const char* stdCode, WTSTickData* newTick){}

	/*
	*	K线闭合
	*/
	virtual void on_bar(IMfStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar){}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//策略工厂接口
typedef void(*FuncEnumMfStrategyCallback)(const char* factName, const char* straName, bool isLast);

class IMfStrategyFact
{
public:
	IMfStrategyFact(){}
	virtual ~IMfStrategyFact(){}

public:
	/*
	*	获取工厂名
	*/
	virtual const char* getName() = 0;

	/*
	*	枚举策略
	*/
	virtual void enumStrategy(FuncEnumMfStrategyCallback cb) = 0;

	/*
	*	根据名称创建K线级别策略
	*/
	virtual MfStrategy* createStrategy(const char* name, const char* id) = 0;


	/*
	*	删除策略
	*/
	virtual bool deleteStrategy(MfStrategy* stra) = 0;
};

//创建执行工厂
typedef IMfStrategyFact* (*FuncCreateMfStraFact)();
//删除执行工厂
typedef void(*FuncDeleteMfStraFact)(IMfStrategyFact* &fact);
