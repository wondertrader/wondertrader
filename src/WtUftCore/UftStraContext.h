/*!
 * \file HftStraContext.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "ITrdNotifySink.h"
#include "../Includes/IUftStraCtx.h"
#include "../Includes/FasterDefs.h"
#include "../Share/fmtlib.h"

class UftStrategy;

NS_WTP_BEGIN
class WtUftEngine;
class TraderAdapter;

class UftStraContext : public IUftStraCtx, public ITrdNotifySink
{
public:
	UftStraContext(WtUftEngine* engine, const char* name, bool bDependent = false);
	virtual ~UftStraContext();

	void set_strategy(UftStrategy* stra){ _strategy = stra; }
	UftStrategy* get_stragety() { return _strategy; }

	void setTrader(TraderAdapter* trader);

public:
	virtual uint32_t id() { return _context_id; }

	virtual void on_init() override;

	virtual void on_tick(const char* code, WTSTickData* newTick) override;

	virtual void on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue) override;

	virtual void on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl) override;

	virtual void on_transaction(const char* stdCode, WTSTransData* newTrans) override;

	virtual void on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled = false) override;

	virtual void on_channel_ready() override;

	virtual void on_channel_lost() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	virtual void on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday) override;

	virtual void on_session_begin(uint32_t uTDate) override;
	virtual void on_session_end(uint32_t uTDate) override;

	virtual void on_params_updated() override;


public:
	virtual void watch_param(const char* name, const char* val) override;
	virtual void watch_param(const char* name, double val) override;
	virtual void watch_param(const char* name, uint32_t val) override;
	virtual void watch_param(const char* name, uint64_t val) override;
	virtual void watch_param(const char* name, int32_t val) override;
	virtual void watch_param(const char* name, int64_t val) override;

	virtual void commit_param_watcher() override;

	virtual const char*		read_param(const char* name, const char* defVal = "") override;
	virtual double		read_param(const char* name, double defVal = 0) override;
	virtual uint32_t	read_param(const char* name, uint32_t defVal = 0) override;
	virtual uint64_t	read_param(const char* name, uint64_t defVal = 0) override;
	virtual int32_t		read_param(const char* name, int32_t defVal = 0) override;
	virtual int64_t		read_param(const char* name, int64_t defVal = 0) override;

	virtual double*		sync_param(const char* name, double initVal = 0) override;
	virtual uint32_t*	sync_param(const char* name, uint32_t initVal = 0) override;
	virtual uint64_t*	sync_param(const char* name, uint64_t initVal = 0) override;
	virtual int32_t*	sync_param(const char* name, int32_t initVal = 0) override;
	virtual int64_t*	sync_param(const char* name, int64_t initVal = 0) override;

public:
	//////////////////////////////////////////////////////////////////////////
	//IHftStraCtx 接口
	virtual uint32_t stra_get_date() override;
	virtual uint32_t stra_get_time() override;
	virtual uint32_t stra_get_secs() override;

	virtual bool stra_cancel(uint32_t localid) override;

	virtual OrderIDs stra_cancel_all(const char* stdCode) override;

	/*
	 *	下单接口: 买入
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual OrderIDs	stra_buy(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	下单接口: 卖出
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual OrderIDs	stra_sell(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	下单接口: 开多
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok
	 */
	virtual uint32_t	stra_enter_long(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	下单接口: 开空
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok
	 */
	virtual uint32_t	stra_enter_short(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	下单接口: 平多
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@isToday	是否今仓，默认false
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual uint32_t	stra_exit_long(const char* stdCode, double price, double qty, bool isToday = false, int flag = 0) override;

	/*
	 *	下单接口: 平空
	 *
	 *	@stdCode	合约代码
	 *	@price		下单价格，0则是市价单
	 *	@qty		下单数量
	 *	@isToday	是否今仓，默认false
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual uint32_t	stra_exit_short(const char* stdCode, double price, double qty, bool isToday = false, int flag = 0) override;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) override;

	virtual WTSKlineSlice* stra_get_bars(const char* stdCode, const char* period, uint32_t count) override;

	virtual WTSTickSlice* stra_get_ticks(const char* stdCode, uint32_t count) override;

	virtual WTSOrdDtlSlice*	stra_get_order_detail(const char* stdCode, uint32_t count) override;

	virtual WTSOrdQueSlice*	stra_get_order_queue(const char* stdCode, uint32_t count) override;

	virtual WTSTransSlice*	stra_get_transaction(const char* stdCode, uint32_t count) override;

	virtual WTSTickData* stra_get_last_tick(const char* stdCode) override;

	virtual void stra_log_info(const char* message) override;
	virtual void stra_log_debug(const char* message) override;
	virtual void stra_log_error(const char* message) override;

	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false, int32_t iFlag = 3) override;
	virtual double stra_enum_position(const char* stdCode) override;
	virtual double stra_get_price(const char* stdCode) override;
	virtual double stra_get_undone(const char* stdCode) override;
	virtual uint32_t stra_get_infos(const char* stdCode) override;

	virtual void stra_sub_ticks(const char* stdCode) override;
	virtual void stra_sub_order_details(const char* stdCode) override;
	virtual void stra_sub_order_queues(const char* stdCode) override;
	virtual void stra_sub_transactions(const char* stdCode) override;

private:
	template<typename... Args>
	void log_debug(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_debug(buffer);
	}

	template<typename... Args>
	void log_info(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_info(buffer);
	}

	template<typename... Args>
	void log_error(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_error(buffer);
	}

	inline const char* getOrderTag(uint32_t localid)
	{
		auto it = _orders.find(localid);
		if (it == _orders.end())
			return "";

		return it->second.c_str();
	}


private:
	uint32_t		_context_id;
	WtUftEngine*	_engine;
	TraderAdapter*	_trader;
	bool			_dependent;

	typedef wt_hashmap<uint32_t, std::string> OrderMap;
	OrderMap		_orders;
	UftStrategy*		_strategy;
};

NS_WTP_END