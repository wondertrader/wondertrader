/*!
* \file IMfStraCtx.h
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#pragma once
#include <stdint.h>
#include <functional>
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSCommodityInfo;
class WTSSessionInfo;
class WTSTickData;
struct WTSBarStruct;
class WTSKlineSlice;
class WTSTickSlice;

//typedef void(*FuncEnumPositionCallBack)(const char* stdCode, int32_t qty);
typedef std::function<void(const char*, double)> FuncEnumSelPositionCallBack;

class ISelStraCtx
{
public:
	ISelStraCtx(const char* name) :_name(name){}
	virtual ~ISelStraCtx(){}

	inline const char* name() const{ return _name.c_str(); }

public:
	virtual uint32_t id() = 0;

	//回调函数
	virtual void on_init() = 0;
	virtual void on_session_begin(uint32_t uTDate) = 0;
	virtual void on_session_end(uint32_t uTDate) = 0;
	virtual void on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy = true) = 0;
	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) = 0;
	virtual bool on_schedule(uint32_t curDate, uint32_t curTime, uint32_t fireTime) = 0;
	/*
	 *	回测结束事件
	 *	只在回测下才会触发
	 */
	virtual void on_bactest_end() {};

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) = 0;
	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick){}
	virtual void on_strategy_schedule(uint32_t curDate, uint32_t curTime) {}

	virtual void enum_position(FuncEnumSelPositionCallBack cb) = 0;

	//策略接口
	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false, const char* userTag = "") = 0;
	virtual void stra_set_position(const char* stdCode, double qty, const char* userTag = "") = 0;

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
	virtual const char* stra_get_last_entertag(const char* stdCode)  = 0;
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
	virtual WTSSessionInfo* stra_get_sessinfo(const char* stdCode) = 0;
	virtual WTSKlineSlice*	stra_get_bars(const char* stdCode, const char* period, uint32_t count) = 0;
	virtual WTSTickSlice*	stra_get_ticks(const char* stdCode, uint32_t count) = 0;
	virtual WTSTickData*	stra_get_last_tick(const char* stdCode) = 0;

	/*
	 *	获取分月合约代码
	 */
	virtual std::string		stra_get_rawcode(const char* stdCode) = 0;

	virtual void stra_sub_ticks(const char* stdCode) = 0;

	virtual void stra_log_info(const char* message) = 0;
	virtual void stra_log_debug(const char* message) = 0;
	virtual void stra_log_error(const char* message) = 0;
	virtual void stra_log_warn(const char* message) {}

	virtual void stra_save_user_data(const char* key, const char* val){}

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") { return defVal; }

protected:
	std::string _name;
};

NS_WTP_END