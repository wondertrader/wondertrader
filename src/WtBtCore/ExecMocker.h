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

#include "../Includes/ExecuteDefs.h"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "MatchEngine.h"

USING_NS_OTP;

class ExecMocker : public ExecuteContext, public IDataSink, public IMatchSink
{
public:
	ExecMocker(HisDataReplayer* replayer);
	virtual ~ExecMocker();

public:
	//////////////////////////////////////////////////////////////////////////
	//IMatchSink
	virtual void handle_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double fireprice, double price, uint64_t ordTime) override;
	virtual void handle_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled, uint64_t ordTime) override;
	virtual void handle_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, uint64_t ordTime) override;

	//////////////////////////////////////////////////////////////////////////
	//IDataSink
	virtual void handle_tick(const char* stdCode, WTSTickData* curTick) override;
	virtual void handle_schedule(uint32_t uDate, uint32_t uTime) override;
	virtual void handle_init() override;

	virtual void handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void handle_session_begin(uint32_t curTDate) override;

	virtual void handle_session_end(uint32_t curTDate) override;

	//////////////////////////////////////////////////////////////////////////
	//ExecuteContext
	virtual WTSTickSlice* getTicks(const char* stdCode, uint32_t count, uint64_t etime = 0) override;

	virtual WTSTickData* grabLastTick(const char* stdCode) override;

	virtual double getPosition(const char* stdCode, int32_t flag = 3) override;

	virtual OrderMap* getOrders(const char* stdCode) override;

	virtual double getUndoneQty(const char* stdCode) override;

	virtual OrderIDs buy(const char* stdCode, double price, double qty, bool bForceClose = false) override;

	virtual OrderIDs sell(const char* stdCode, double price, double qty, bool bForceClose = false) override;

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

	double			_position;
	double			_undone;
	WTSTickData*	_last_tick;
	double			_sig_px;
	uint64_t		_sig_time;

	std::ofstream	_trade_logs;
	uint32_t	_ord_cnt;
	double		_ord_qty;
	uint32_t	_cacl_cnt;
	double		_cacl_qty;
	uint32_t	_sig_cnt;

	std::string	_id;

	MatchEngine	_matcher;
};

