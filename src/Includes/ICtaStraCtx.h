/*!
 * \file ICtaStraCtx.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include<string>
#include <stdint.h>
#include <functional>
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSCommodityInfo;
class WTSTickData;
struct WTSBarStruct;
class WTSKlineSlice;
class WTSTickSlice;

//typedef void(*FuncEnumPositionCallBack)(const char* stdCode, int32_t qty);
typedef std::function<void(const char*, double)> FuncEnumCtaPosCallBack;

class ICtaStraCtx
{
public:
	ICtaStraCtx(const char* name) :_name(name){}
	virtual ~ICtaStraCtx(){}

	inline const char* name() const{ return _name.c_str(); }

public:
	virtual uint32_t id() = 0;

	//回调函数
	virtual void on_init() = 0;
	virtual void on_session_begin(uint32_t uTDate) = 0;
	virtual void on_session_end(uint32_t uTDate) = 0;
	virtual void on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy = true) = 0;
	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual bool on_schedule(uint32_t curDate, uint32_t curTime) = 0;
	/*
	 *	回测结束事件
	 *	只在回测下才会触发
	 */
	virtual void on_bactest_end() {};

	/*
	 *	重算结束
	 *	设计目的是要把on_calculate分成两步
	 *	方便一些外挂的逻辑接入进来，可以在on_calculate_done执行信号
	 */
	virtual void on_calculate_done(uint32_t curDate, uint32_t curTime) { };

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) = 0;
	virtual void on_calculate(uint32_t curDate, uint32_t curTime) = 0;
	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick){}
	virtual void on_condition_triggered(const char* stdCode, double target, double price, const char* usertag){}

	virtual void enum_position(FuncEnumCtaPosCallBack cb, bool bForExecute = false) = 0;

	//策略接口
	virtual void stra_enter_long(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_enter_short(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_exit_long(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_exit_short(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;

	/*
	 *	获取当前持仓
	 *	@stdCode	合约代码
	 *	@userTag	下单标记，如果下单标记为空，则读取持仓汇总，如果下单标记不为空，则读取对应的持仓明细
	 *	@bOnlyValid	是否只读可用持仓，默认为false，只有当userTag为空时生效，主要针对T+1的品种
	 */
	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false, const char* userTag = "") = 0;
	virtual void stra_set_position(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;

	/*
	 *	获取当前价格
	 */
	virtual double stra_get_price(const char* stdCode) = 0;

	/*
	 *	读取当日价格
	 *	@stdCode	合约代码
	 *	@flag		价格标记：0-开盘价，1-最高价，2-最低价，3-收盘价/最新价
	 */
	virtual double stra_get_day_price(const char* stdCode, int flag = 0) = 0;

	virtual uint32_t stra_get_tdate() = 0;
	virtual uint32_t stra_get_date() = 0;
	virtual uint32_t stra_get_time() = 0;

	virtual double stra_get_fund_data(int flag = 0) = 0;

	virtual uint64_t stra_get_first_entertime(const char* stdCode) = 0;
	virtual uint64_t stra_get_last_entertime(const char* stdCode) = 0;
	virtual uint64_t stra_get_last_exittime(const char* stdCode) = 0;
	virtual double stra_get_last_enterprice(const char* stdCode) = 0;
	virtual double stra_get_position_avgpx(const char* stdCode) = 0;
	virtual double stra_get_position_profit(const char* stdCode) = 0;

	virtual uint64_t stra_get_detail_entertime(const char* stdCode, const char* userTag) = 0;
	virtual double stra_get_detail_cost(const char* stdCode, const char* userTag) = 0;

	/*
	 *	读取持仓明细的浮盈
	 *	@stdCode	合约代码
	 *	@userTag	下单标记
	 *	@flag		浮盈标志：0-浮动盈亏，1-最大浮盈，2-最高浮动价格，-1-最大浮亏，-2-最小浮动价格
	 */
	virtual double stra_get_detail_profit(const char* stdCode, const char* userTag, int flag = 0) = 0;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) = 0;
	virtual WTSKlineSlice*	stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain = false) = 0;
	virtual WTSTickSlice*	stra_get_ticks(const char* stdCode, uint32_t count) = 0;
	virtual WTSTickData*	stra_get_last_tick(const char* stdCode) = 0;

	/*
	 *	获取分月合约代码
	 */
	virtual std::string		stra_get_rawcode(const char* stdCode) = 0;

	virtual void stra_sub_ticks(const char* stdCode) = 0;
	virtual void stra_sub_bar_events(const char* stdCode, const char* period) = 0;

	virtual void stra_log_info(const char* message) = 0;
	virtual void stra_log_debug(const char* message) = 0;
	virtual void stra_log_error(const char* message) = 0;
	virtual void stra_log_warn(const char* message) {}

	virtual void stra_save_user_data(const char* key, const char* val){}

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") { return defVal; }

	/*
	 *	设置图表K线
	 */
	virtual void set_chart_kline(const char* stdCode, const char* period) {}

	/*
	 *	添加信号
	 */
	virtual void add_chart_mark(double price, const char* icon, const char* tag) {}

	/*
	 *	添加指标
	 *	@idxName	指标名称
	 *	@indexType	指标类型：0-主图指标，1-副图指标
	 */
	virtual void register_index(const char* idxName, uint32_t indexType) {}


	/*
	 *	添加指标线
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@lineType	线性，0-曲线
	 */
	virtual bool register_index_line(const char* idxName, const char* lineName, uint32_t lineType) { return false; }

	/*
	 *	添加基准线
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@val		数值
	 */
	virtual bool add_index_baseline(const char* idxName, const char* lineName, double val) { return false; }

	/*
	 *	设置指标值
	 *	@idxName	指标名称
	 *	@lineName	线条名称
	 *	@val		指标值
	 */
	virtual bool set_index_value(const char* idxName, const char* lineName, double val) { return false; }

	/*
	 *	获取最后的进场标记
	 *	@stdCode	合约代码
	 */
	virtual const char* stra_get_last_entertag(const char* stdCode) = 0;

protected:
	std::string _name;
};

NS_WTP_END