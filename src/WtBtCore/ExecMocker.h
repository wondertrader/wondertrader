/*!
 * \file ExecMocker.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <fstream>
#include "HisDataReplayer.h"

#include "../WtCore/ExecuteDefs.h"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

USING_NS_OTP;

class ExecMocker : public ExecuteContext, public IDataSink
{
public:
	ExecMocker(HisDataReplayer* replayer);
	virtual ~ExecMocker();

private:
	void	fire_orders(const char* stdCode, OrderIDs& to_erase);
	void	match_orders(WTSTickData* curTick, OrderIDs& to_erase);
	void	update_lob(WTSTickData* curTick);

public:
	//////////////////////////////////////////////////////////////////////////
	//IDataSink
	virtual void handle_tick(const char* stdCode, WTSTickData* curTick) override;
	virtual void handle_schedule(uint32_t uDate, uint32_t uTime) override;
	virtual void handle_init() override;

	virtual void handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_session_begin() override;

	virtual void handle_session_end() override;

	//////////////////////////////////////////////////////////////////////////
	//ExecuteContext
	virtual WTSTickSlice* getTicks(const char* stdCode, uint32_t count, uint64_t etime = 0) override;

	virtual WTSTickData* grabLastTick(const char* stdCode) override;

	virtual double getPosition(const char* stdCode, int32_t flag = 3) override;

	virtual OrderMap* getOrders(const char* stdCode) override;

	virtual double getUndoneQty(const char* stdCode) override;

	virtual OrderIDs buy(const char* stdCode, double price, double qty) override;

	virtual OrderIDs sell(const char* stdCode, double price, double qty) override;

	virtual bool cancel(uint32_t localid) override;

	virtual OrderIDs cancel(const char* stdCode, bool isBuy, double qty = 0) override;

	virtual void writeLog(const char* fmt, ...) override;

	virtual WTSCommodityInfo* getCommodityInfo(const char* stdCode) override;
	virtual WTSSessionInfo* getSessionInfo(const char* stdCode) override;

	virtual uint64_t getCurTime() override;

public:
	bool	init(WTSVariant* cfg);

private:
	HisDataReplayer*	_replayer;

	typedef struct _ExecFactInfo
	{
		std::string		_module_path;
		DllHandle		_module_inst;
		IExecuterFact*	_fact;
		FuncCreateExeFact	_creator;
		FuncDeleteExeFact	_remover;

		_ExecFactInfo()
		{
			_module_inst = NULL;
			_fact = NULL;
		}

		~_ExecFactInfo()
		{
			if (_fact)
				_remover(_fact);
		}
	} ExecFactInfo;
	ExecFactInfo	_factory;

	ExecuteUnit*	_exec_unit;
	std::string		_code;
	std::string		_period;
	int32_t			_volunit;
	double			_cancelrate;

	double			_position;
	double			_undone;
	WTSTickData*	_last_tick;
	double			_sig_px;
	uint64_t		_sig_time;

	typedef struct _OrderInfo
	{
		char		_code[32];
		bool		_buy;
		double		_qty;
		double		_left;
		double		_traded;
		double		_limit;
		double		_price;
		uint32_t	_state;
		uint64_t	_time;
		uint32_t	_queue;
		bool		_positive;

		_OrderInfo()
		{
			memset(this, 0, sizeof(_OrderInfo));
		}
	} OrderInfo;

	typedef std::unordered_map<uint32_t, OrderInfo> Orders;
	Orders	_orders;

	std::ofstream	_trade_logs;
	uint32_t	_ord_cnt;
	double		_ord_qty;
	uint32_t	_cacl_cnt;
	double		_cacl_qty;
	uint32_t	_sig_cnt;

	std::string	_id;

	typedef std::map<uint32_t, uint32_t>	LOBItems;

	typedef struct _LmtOrdBook
	{
		LOBItems	_items;
		uint32_t	_cur_px;
		uint32_t	_ask_px;
		uint32_t	_bid_px;

		void clear()
		{
			_items.clear();
			_cur_px = 0;
			_ask_px = 0;
			_bid_px = 0;
		}

		_LmtOrdBook()
		{
			_cur_px = 0;
			_ask_px = 0;
			_bid_px = 0;
		}
	} LmtOrdBook;
	typedef std::unordered_map<std::string, LmtOrdBook> LmtOrdBooks;
	LmtOrdBooks	_lmt_ord_books;
};

