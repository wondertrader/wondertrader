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

	/*
	 *	初始化事件
	 *	生命周期中只会回调一次
	 */
	virtual void on_init(IUftStraCtx* ctx) = 0;

	/*
	 *	交易日开始事件
	 *	实盘时因为每天重启，所以会在on_init后调用一次
	 *	回测时，生命周期中会调用多次
	 *	如果有什么数据需要每天初始化，可以放到这个回调中处理，实盘就和回测保持一致了
	 */
	virtual void on_session_begin(IUftStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	交易日结束事件
	 */
	virtual void on_session_end(IUftStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	tick推送
	 */
	virtual void on_tick(IUftStraCtx* ctx, const char* stdCode, WTSTickData* newTick) {}

	/*
	 *	委托队列推送
	 */
	virtual void on_order_queue(IUftStraCtx* ctx, const char* stdCode, WTSOrdQueData* newOrdQue) {}

	/*
	 *	逐笔委托推送
	 */
	virtual void on_order_detail (IUftStraCtx* ctx, const char* stdCode, WTSOrdDtlData* newOrdDtl) {}

	/*
	 *	逐笔成交推送
	 */
	virtual void on_transaction(IUftStraCtx* ctx, const char* stdCode, WTSTransData* newTrans) {}

	/*
	 *	K线闭合事件
	 *
	 *	@stdCode	合约代码，格式如SHFE.rb2205
	 *	@period		周期
	 *	@times		重采样倍数
	 */
	virtual void on_bar(IUftStraCtx* ctx, const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) {}

	/*
	 *	成交回报
	 *
	 *	@localid	本地订单id
	 *	@stdCode	合约代码，格式如SHFE.rb2205
	 *	@isLong		是否做多
	 *	@offset		开平，0-开仓，1-平仓，2-平今
	 *	@vol		成交量
	 *	@price		成交价格
	 */
	virtual void on_trade(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price) {}

	/*
	 *	持仓同步回报
	 *	交易通道连接成功时，如果查到持仓，会推送一次
	 *	如果没有查到，则不会推送，所以这个事件接口不适合放任何状态相关的东西
	 * 
	 *	@stdCode	合约代码，格式如SHFE.rb2205
	 *	@isLong		是否做多
	 *	@prevol		昨仓
	 *	@preavail	可用昨仓
	 *	@newvol		今仓	
	 *	@newavail	可用今仓
	 */
	virtual void on_position(IUftStraCtx* ctx, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) {}

	/*
	 *	订单
	 *
	 *	@localid	本地订单id
	 *	@stdCode	合约代码，格式如SHFE.rb2205
	 *	@isLong		是否做多
	 *	@offset		开平，0-开仓，1-平仓，2-平今
	 *	@totalQty	下单总数
	 *	@leftQty	剩余数量
	 *	@price		委托价格
	 *	@isCanceled	是否已撤销
	 */
	virtual void on_order(IUftStraCtx* ctx, uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled) {}

	/*
	 *	交易通道就绪事件
	 */
	virtual void on_channel_ready(IUftStraCtx* ctx) {}

	/*
	 *	交易通道断线事件
	 */
	virtual void on_channel_lost(IUftStraCtx* ctx) {}

	/*
	 *	下单回报
	 *	有些接口只有错单才会有回报，所以不能使用该接口作为下单是否成功的回报
	 *
	 *	@localid	本地单号
	 *	@bSuccess	是否成功
	 *	@message	返回消息
	 */
	virtual void on_entrust(uint32_t localid, bool bSuccess, const char* message) {}


	/*
	 *	参数更新回调
	 */
	virtual void on_params_updated() {}

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
