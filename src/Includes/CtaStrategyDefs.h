/*!
 * \file CtaStrategyDefs.h
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
class ICtaStraCtx;
class ICtaTickStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_WTP_END

USING_NS_WTP;

class CtaStrategy
{
public:
	CtaStrategy(const char* id) :_id(id){}
	virtual ~CtaStrategy(){}

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
	virtual void on_init(ICtaStraCtx* ctx){}

	/*
	 *	交易日开始
	 */
	virtual void on_session_begin(ICtaStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	交易日结束
	 */
	virtual void on_session_end(ICtaStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	主体逻辑执行入口
	 */
	virtual void on_schedule(ICtaStraCtx* ctx, uint32_t uDate, uint32_t uTime){}

	/*
	 *	主体逻辑执行完成
	 */
	virtual void on_schedule_done(ICtaStraCtx* ctx, uint32_t uDate, uint32_t uTime) {}

	/*
	 *	tick数据
	 */
	virtual void on_tick(ICtaStraCtx* ctx, const char* stdCode, WTSTickData* newTick){}

	/*
	 *	K线闭合
	 */
	virtual void on_bar(ICtaStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar){}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//策略工厂接口
typedef void(*FuncEnumStrategyCallback)(const char* factName, const char* straName, bool isLast);

class ICtaStrategyFact
{
public:
	ICtaStrategyFact(){}
	virtual ~ICtaStrategyFact(){}

public:
	/*
	 *	获取工厂名
	 */
	virtual const char* getName() = 0;

	/*
	 *	枚举策略
	 */
	virtual void enumStrategy(FuncEnumStrategyCallback cb) = 0;

	/*
	 *	根据名称创建K线级别策略
	 */
	virtual CtaStrategy* createStrategy(const char* name, const char* id) = 0;


	/*
	 *	删除策略
	 */
	virtual bool deleteStrategy(CtaStrategy* stra) = 0;
};

//创建执行工厂
typedef ICtaStrategyFact* (*FuncCreateStraFact)();
//删除执行工厂
typedef void(*FuncDeleteStraFact)(ICtaStrategyFact* &fact);
