/*!
 * \file UftMocker.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <queue>
#include <sstream>

#include "HisDataReplayer.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IUftStraCtx.h"
#include "../Includes/UftStrategyDefs.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/fmtlib.h"

class HisDataReplayer;

class UftMocker : public IDataSink, public IUftStraCtx
{
public:
	UftMocker(HisDataReplayer* replayer, const char* name);
	virtual ~UftMocker();

private:
	template<typename... Args>
	void log_debug(const char* format, const Args& ...args)
	{
		std::string s = fmt::sprintf(format, args...);
		stra_log_debug(s.c_str());
	}

	template<typename... Args>
	void log_info(const char* format, const Args& ...args)
	{
		std::string s = fmt::sprintf(format, args...);
		stra_log_info(s.c_str());
	}

	template<typename... Args>
	void log_error(const char* format, const Args& ...args)
	{
		std::string s = fmt::sprintf(format, args...);
		stra_log_error(s.c_str());
	}

public:
	//////////////////////////////////////////////////////////////////////////
	//IDataSink
	virtual void	handle_tick(const char* stdCode, WTSTickData* curTick) override;
	virtual void	handle_order_queue(const char* stdCode, WTSOrdQueData* curOrdQue) override;
	virtual void	handle_order_detail(const char* stdCode, WTSOrdDtlData* curOrdDtl) override;
	virtual void	handle_transaction(const char* stdCode, WTSTransData* curTrans) override;

	virtual void	handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;
	virtual void	handle_schedule(uint32_t uDate, uint32_t uTime) override;

	virtual void	handle_init() override;
	virtual void	handle_session_begin(uint32_t curTDate) override;
	virtual void	handle_session_end(uint32_t curTDate) override;

	virtual void	handle_replay_done() override;

	virtual void	on_tick_updated(const char* stdCode, WTSTickData* newTick) override;
	virtual void	on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue) override;
	virtual void	on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl) override;
	virtual void	on_trans_updated(const char* stdCode, WTSTransData* newTrans) override;

	//////////////////////////////////////////////////////////////////////////
	//IUftStraCtx
	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue) override;

	virtual void on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl) override;

	virtual void on_transaction(const char* stdCode, WTSTransData* newTrans) override;

	virtual uint32_t id() override;

	virtual void on_init() override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_session_begin(uint32_t curTDate) override;

	virtual void on_session_end(uint32_t curTDate) override;

	virtual bool stra_cancel(uint32_t localid) override;

	virtual OrderIDs stra_cancel_all(const char* stdCode) override;

	virtual OrderIDs stra_buy(const char* stdCode, double price, double qty, int flag = 0) override;

	virtual OrderIDs stra_sell(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	开多
	 *	@stdCode	代码，格式如SSE.600000
	 *	@price		委托价格
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual uint32_t	stra_enter_long(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	开空
	 *	@stdCode	代码，格式如SSE.600000
	 *	@price		委托价格
	 *	@qty		下单数量
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual uint32_t	stra_enter_short(const char* stdCode, double price, double qty, int flag = 0) override;

	/*
	 *	平多
	 *	@stdCode	代码，格式如SSE.600000
	 *	@price		委托价格
	 *	@qty		下单数量
	 *	@isToday	是否今仓，SHFE、INE专用
	 *	@flag		下单标志: 0-normal，1-fak，2-fok，默认0
	 */
	virtual uint32_t	stra_exit_long(const char* stdCode, double price, double qty, bool isToday = false, int flag = 0) override;

	/*
	 *	平空
	 *	@stdCode	代码，格式如SSE.600000
	 *	@price		委托价格
	 *	@qty		下单数量
	 *	@isToday	是否今仓，SHFE、INE专用
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

	/*
	 *	获取持仓
	 *	@stdCode	代码，格式如SSE.600000
	 *	@bOnlyValid	获取可用持仓
	 *	@iFlag		读取标记，1-多头，2-空头，3-净头寸
	 */
	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false, int32_t iFlag = 3) override;

	virtual double stra_get_undone(const char* stdCode) override;

	virtual double stra_get_price(const char* stdCode) override;

	virtual uint32_t stra_get_date() override;

	virtual uint32_t stra_get_time() override;

	virtual uint32_t stra_get_secs() override;

	virtual void stra_sub_ticks(const char* stdCode) override;

	virtual void stra_sub_order_queues(const char* stdCode) override;

	virtual void stra_sub_order_details(const char* stdCode) override;

	virtual void stra_sub_transactions(const char* stdCode) override;

	virtual void stra_log_info(const char* message) override;
	virtual void stra_log_debug(const char* message) override;
	virtual void stra_log_error(const char* message) override;


	//////////////////////////////////////////////////////////////////////////
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price);

	virtual void on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled);

	virtual void on_channel_ready();

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message);

public:
	bool	init_uft_factory(WTSVariant* cfg);

private:
	typedef std::function<void()> Task;
	void	postTask(Task task);
	void	procTask();

	bool	procOrder(uint32_t localid);

	void	update_position(const char* stdCode, bool isLong, uint32_t offset, double qty, double price = 0.0);
	void	update_dyn_profit(const char* stdCode, WTSTickData* newTick);

	void	dump_outputs();
	void	log_trade(const char* stdCode, bool isLong, uint32_t offset, uint64_t curTime, double price, double qty, double fee);
	void	log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
		double profit, double maxprofit, double maxloss, double totalprofit);

private:
	HisDataReplayer*	_replayer;

	bool			_use_newpx;
	uint32_t		_error_rate;

	typedef faster_hashmap<std::string, double> PriceMap;
	PriceMap		_price_map;


	typedef struct _StraFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IUftStrategyFact*	_fact;
		FuncCreateUftStraFact	_creator;
		FuncDeleteUftStraFact	_remover;

		_StraFactInfo()
		{
			_module_inst = NULL;
			_fact = NULL;
		}

		~_StraFactInfo()
		{
			if (_fact)
				_remover(_fact);
		}
	} StraFactInfo;
	StraFactInfo	_factory;

	UftStrategy*	_strategy;

	StdThreadPtr		_thrd;
	StdUniqueMutex		_mtx;
	std::queue<Task>	_tasks;
	bool				_stopped;

	StdRecurMutex		_mtx_control;

	typedef struct _OrderInfo
	{
		bool	_isLong;
		char	_code[32];
		double	_price;
		double	_total;
		double	_left;
		
		uint32_t	_offset;
		uint32_t	_localid;

		_OrderInfo()
		{
			memset(this, 0, sizeof(_OrderInfo));
		}

	} OrderInfo;
	typedef faster_hashmap<uint32_t, OrderInfo> Orders;
	StdRecurMutex	_mtx_ords;
	Orders			_orders;

	//用户数据
	typedef faster_hashmap<std::string, std::string> StringHashMap;
	StringHashMap	_user_datas;
	bool			_ud_modified;

	typedef struct _DetailInfo
	{
		double		_price;
		double		_volume;
		uint64_t	_opentime;
		uint32_t	_opentdate;
		double		_max_profit;
		double		_max_loss;
		double		_profit;

		_DetailInfo()
		{
			memset(this, 0, sizeof(_DetailInfo));
		}
	} DetailInfo;

	typedef struct _PosItem
	{
		bool		_long;
		double		_closeprofit;
		double		_dynprofit;

		double		_prevol;
		double		_newvol;
		double		_preavail;
		double		_newavail;

		std::vector<DetailInfo> _details;

		_PosItem()
		{
			_prevol = 0;
			_newvol = 0;
			_preavail = 0;
			_newavail = 0;

			_closeprofit = 0;
			_dynprofit = 0;
		}

		inline double valid() const { return _preavail + _newavail; }
		inline double volume() const { return _prevol + _newvol; }
		inline double frozen() const { return volume() - valid(); }
	} PosItem;

	typedef struct _PosInfo
	{
		PosItem	_long;
		PosItem	_short;

		inline double closeprofit() const{ return _long._closeprofit + _short._closeprofit; }
		inline double dynprofit() const { return _long._dynprofit + _short._dynprofit; }
	} PosInfo;
	typedef faster_hashmap<std::string, PosInfo> PositionMap;
	PositionMap		_pos_map;

	std::stringstream	_trade_logs;
	std::stringstream	_close_logs;
	std::stringstream	_fund_logs;
	std::stringstream	_sig_logs;

	typedef struct _StraFundInfo
	{
		double	_total_profit;
		double	_total_dynprofit;
		double	_total_fees;

		_StraFundInfo()
		{
			memset(this, 0, sizeof(_StraFundInfo));
		}
	} StraFundInfo;

	StraFundInfo		_fund_info;

protected:
	uint32_t		_context_id;

	//tick订阅列表
	faster_hashset<std::string> _tick_subs;
};

