/*!
 * \file ExecuteDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSCollection.hpp"

NS_OTP_BEGIN
class WTSTickData;
class WTSHisTickData;
class WTSVariant;
class WTSCommodityInfo;
class WTSSessionInfo;

typedef std::vector<uint32_t> OrderIDs;
typedef WTSMap<uint32_t> OrderMap;

//////////////////////////////////////////////////////////////////////////
//执行环境基础类
class ExecuteContext
{
public:
	ExecuteContext(){}
	virtual ~ExecuteContext(){}

public:
	/*
	 *	获取tick数据
	 *	code	合约代码
	 *	count	数据条数
	 *	etime	截止时间, 0则为当前时间
	 *	
	 *	返回值	历史数据封装类指针
	 */
	virtual WTSTickSlice*	getTicks(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;

	/*
	 *	读取最近一笔Tick数据
	 *	code	合约代码
	 *	
	 *	返回值	WTSTickData指针
	 */
	virtual WTSTickData*	grabLastTick(const char* stdCode) = 0;

	/*
	 *	获取仓位
	 *	code	合约代码
	 *	flag	操作标记 1-多仓, 2-空仓, 3-多空轧平
	 *	
	 *	返回值	轧平后的仓位: 多仓>0, 空仓<0
	 */
	virtual double getPosition(const char* stdCode, int32_t flag = 3) = 0;

	/*
	 *	获取未完成订单
	 *	code	合约代码
	 *
	 *	返回值	localid-WTSOrderInfo的映射
	 */
	virtual OrderMap* getOrders(const char* stdCode) = 0;

	/*
	 *	获取未完成数量
	 *	code	合约代码
	 *	
	 *	返回值	买卖轧平以后的未完成数量
	 */
	virtual double getUndoneQty(const char* stdCode) = 0;

	/*
	 *	买入接口
	 *	code	合约代码
	 *	price	委托价格, 0为市价单
	 *	qty		委托数量
	 *
	 *	返回值	本地订单号数组: 一个买入操作可能会拆成最多3个订单发出
	 */
	virtual OrderIDs buy(const char* stdCode, double price, double qty, bool bForceClose = false) = 0;

	/*
	*	卖出接口
	*	code	合约代码
	*	price	委托价格, 0为市价单
	*	qty		委托数量
	*
	*	返回值	本地订单号数组: 一个买入操作可能会拆成最多3个订单发出
	*/
	virtual OrderIDs sell(const char* stdCode, double price, double qty, bool bForceClose = false) = 0;

	/*
	 *	根据本地订单号撤单
	 *	localid 本地订单号
	 *
	 *	返回值	返回撤单指令是否发送成功
	 */
	virtual bool	cancel(uint32_t localid) = 0;

	/*
	 *	根据指定的方向和数量撤单
	 *	code 合约代码
	 *	qty	最少撤单数量, 如果有多个委托, 那么按照时间顺序一个一个撤单, 直到撤销的数量大于等于qty
	 *	这个一般用不到, 所以传0的时候, 就是把全部买单或者卖单撤销
	 *	isBuy 买单or卖单
	 *	
	 *	返回值 返回实际发送了撤单指令的数量
	 */
	virtual OrderIDs cancel(const char* stdCode, bool isBuy, double qty = 0) = 0;

	/*
	 *	写日志
	 */
	virtual void writeLog(const char* fmt, ...) = 0;

	/*
	 *	获取品种参数
	 */
	virtual WTSCommodityInfo* getCommodityInfo(const char* stdCode) = 0;

	/*
	 *	获取交易时间模板信息
	 */
	virtual WTSSessionInfo* getSessionInfo(const char* stdCode) = 0;

	/*
	 *	获取当前时间,精确到毫秒,格式如20191127174139500
	 */
	virtual uint64_t	getCurTime() = 0;

	/*
	 *	注册定时器
	 *	@stdCode	合约代码
	 *	@elapse		时间间隔,单位毫秒
	 *
	 *	返回值		是否注册成功
	 */
	virtual bool		registerTimer(const char* stdCode, uint32_t elapse){ return false; }
};

//////////////////////////////////////////////////////////////////////////
//执行单元基础类
class ExecuteUnit
{
public:
	ExecuteUnit() :_ctx(NULL), _code(""){}
	virtual ~ExecuteUnit(){}

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
	 *	初始化执行单元
	 *	ctx		执行单元运行环境
	 *	code	管理的合约代码
	 */
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg){ _ctx = ctx; _code = stdCode; }

public:
	/*
	 *	设置新的目标仓位
	 *	stdCode	合约代码
	 *	newVol	新的目标仓位
	 */
	virtual void set_position(const char* stdCode, double newVol) = 0;

	/*
	 *	清理全部持仓，锁仓的情况下也要清理
	 *	stdCode	合约代码	
	 */
	virtual void clear_all_position(const char* stdCode){}

	/*
	 *	tick数据回调
	 *	newTick	最新的tick数据
	 */
	virtual void on_tick(WTSTickData* newTick) = 0;

	/*
	 *	成交回报
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	vol		成交数量, 这里没有正负, 通过isBuy确定买入还是卖出
	 *	price	成交价格
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) = 0;

	/*
	 *	订单回报
	 *	localid	本地单号
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	leftover	剩余数量
	 *	price	委托价格
	 *	isCanceled	是否已撤销
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled) = 0;

	/*
	 *	
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) = 0;

	/*
	 *	交易通道就绪回调
	 */
	virtual void on_channel_ready() = 0;

	/*
	 *	交易通道丢失回调
	 */
	virtual void on_channel_lost() = 0;

protected:
	ExecuteContext*	_ctx;
	std::string		_code;
};

//////////////////////////////////////////////////////////////////////////
//执行单元工厂接口
typedef void(*FuncEnumUnitCallback)(const char* factName, const char* unitName, bool isLast);

class IExecuterFact
{
public:
	IExecuterFact(){}
	virtual ~IExecuterFact(){}

public:
	/*
	 *	获取工厂名
	 */
	virtual const char* getName() = 0;

	/*
	 *	枚举执行单元
	 */
	virtual void enumExeUnit(FuncEnumUnitCallback cb) = 0;

	/*
	 *	根据名称创建执行单元
	 */
	virtual ExecuteUnit* createExeUnit(const char* name) = 0;

	/*
	 *	删除执行单元
	 */
	virtual bool deleteExeUnit(ExecuteUnit* unit) = 0;
};

//创建执行工厂
typedef IExecuterFact* (*FuncCreateExeFact)();
//删除执行工厂
typedef void(*FuncDeleteExeFact)(IExecuterFact* &fact);

NS_OTP_END