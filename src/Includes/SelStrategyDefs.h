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

#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSVariant;
class ISelStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_WTP_END

USING_NS_WTP;

class SelStrategy
{
public:
	SelStrategy(const char* id) :_id(id){}
	virtual ~SelStrategy(){}

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
	virtual void on_init(ISelStraCtx* ctx){}

	/*
	 *	交易日开始
	 */
	virtual void on_session_begin(ISelStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	交易日结束
	 */
	virtual void on_session_end(ISelStraCtx* ctx, uint32_t uTDate) {}

	/*
	*	主体逻辑执行入口
	*/
	virtual void on_schedule(ISelStraCtx* ctx, uint32_t uDate, uint32_t uTime){}

	/*
	*	tick数据
	*/
	virtual void on_tick(ISelStraCtx* ctx, const char* stdCode, WTSTickData* newTick){}

	/*
	*	K线闭合
	*/
	virtual void on_bar(ISelStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar){}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//策略工厂接口
typedef void(*FuncEnumSelStrategyCallback)(const char* factName, const char* straName, bool isLast);

class ISelStrategyFact
{
public:
	ISelStrategyFact(){}
	virtual ~ISelStrategyFact(){}

public:
	/*
	*	获取工厂名
	*/
	virtual const char* getName() = 0;

	/*
	*	枚举策略
	*/
	virtual void enumStrategy(FuncEnumSelStrategyCallback cb) = 0;

	/*
	*	根据名称创建K线级别策略
	*/
	virtual SelStrategy* createStrategy(const char* name, const char* id) = 0;


	/*
	*	删除策略
	*/
	virtual bool deleteStrategy(SelStrategy* stra) = 0;
};

//创建工厂
typedef ISelStrategyFact* (*FuncCreateSelStraFact)();
//删除工厂
typedef void(*FuncDeleteSelStraFact)(ISelStrategyFact* &fact);
