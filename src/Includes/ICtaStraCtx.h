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

NS_OTP_BEGIN
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

	virtual void on_bar_close(const char* stdCode, const char* period, WTSBarStruct* newBar) = 0;
	virtual void on_mainkline_updated(uint32_t curDate, uint32_t curTime) = 0;
	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick){}

	virtual void enum_position(FuncEnumCtaPosCallBack cb) = 0;

	//策略接口
	virtual void stra_enter_long(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_enter_short(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_exit_long(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual void stra_exit_short(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;

	virtual double stra_get_position(const char* stdCode, const char* userTag = "") = 0;
	virtual void stra_set_position(const char* stdCode, double qty, const char* userTag = "", double limitprice = 0.0, double stopprice = 0.0) = 0;
	virtual double stra_get_price(const char* stdCode) = 0;

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
	virtual double stra_get_detail_profit(const char* stdCode, const char* userTag, int flag = 0) = 0;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) = 0;
	virtual WTSKlineSlice*	stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain = false) = 0;
	virtual WTSTickSlice*	stra_get_ticks(const char* stdCode, uint32_t count) = 0;
	virtual WTSTickData*	stra_get_last_tick(const char* stdCode) = 0;

	virtual void stra_sub_ticks(const char* stdCode) = 0;

	virtual void stra_log_text(const char* fmt, ...) = 0;

	virtual void stra_save_user_data(const char* key, const char* val){}

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") { return defVal; }

protected:
	std::string _name;
};

NS_OTP_END