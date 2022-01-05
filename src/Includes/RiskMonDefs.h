/*!
 * \file RiskMonDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/WTSMarcos.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 *	这是风控模块基本定义文件
 *	目前风控模块暂时不考虑根据行情风控
 *	只实现基本的资金风控的要求
 *	行情风控通过更高层的择时策略来实现
 *	不在风控模块里处理
 *	风控模块只处理高效的风控策略
 */

NS_WTP_BEGIN
class WTSVariant;
class WTSPortFundInfo;

/*
 *	组合上下文
 */
class WtPortContext
{
public:
	/*
	 *	获取组合资金数据
	 */
	virtual WTSPortFundInfo*	getFundInfo() = 0;

	/*
	 *	设置数量倍数
	 *	@scale	数量倍率, 一般小于等于1, 用于控制整体仓位比例
	 */
	virtual void	setVolScale(double scale) = 0;

	/*
	 *	检查是否处于交易状态
	 *
	 *	return	处于交易状态为true
	 */
	virtual bool	isInTrading() = 0;

	/*
	 *	写日志
	 */
	virtual void	writeRiskLog(const char* message) = 0;

	/*
	 *	获取当前日期
	 */
	virtual uint32_t	getCurDate() = 0;

	/*
	 *	获取当前时间
	 */
	virtual uint32_t	getCurTime() = 0;

	/*
	 *	获取当前交易日
	 */
	virtual uint32_t	getTradingDate() = 0;

	/*
	 *	将时间转换为分钟数(日内有效)
	 */
	virtual uint32_t	transTimeToMin(uint32_t uTime) = 0;
};

/*
 *	组合风控模块
 */
class WtRiskMonitor
{
public:
	WtRiskMonitor():_ctx(NULL){}
	virtual ~WtRiskMonitor(){}

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
	virtual void init(WtPortContext* ctx, WTSVariant* cfg){ _ctx = ctx; }

	/*
	 *	启动风控模块
	 */
	virtual void run(){}

	/*
	 *	停止风控模块
	 */
	virtual void stop(){}

protected:
	WtPortContext*	_ctx;
};


//////////////////////////////////////////////////////////////////////////
//风控模块工厂接口
typedef void(*FuncEnumRiskMonCallback)(const char* factName, const char* unitName, bool isLast);

class IRiskMonitorFact
{
public:
	IRiskMonitorFact(){}
	virtual ~IRiskMonitorFact(){}

public:
	/*
	*	获取工厂名
	*/
	virtual const char* getName() = 0;

	/*
	*	枚举交易通道风控模块
	*/
	virtual void enumRiskMonitors(FuncEnumRiskMonCallback cb) = 0;

	/*
	*	根据名称创建交易通道风控模块
	*/
	virtual WtRiskMonitor* createRiskMonotor(const char* name) = 0;

	/*
	*	删除交易通道风控模块
	*/
	virtual bool deleteRiskMonotor(WtRiskMonitor* unit) = 0;
};

//创建执行工厂
typedef IRiskMonitorFact* (*FuncCreateRiskMonFact)();
//删除执行工厂
typedef void(*FuncDeleteRiskMonFact)(IRiskMonitorFact* &fact);

NS_WTP_END
