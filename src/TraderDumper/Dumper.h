#pragma once
#include "PorterDefs.h"
#include "../Share/StdUtils.hpp"

class Dumper
{

public:
	Dumper(): 
		_cb_account(nullptr), _cb_order(nullptr), _cb_trade(nullptr), 
		_cb_position(nullptr), _refresh_span(10), _stopped(false){}

	void register_callbacks(FuncOnAccount cbAccount, FuncOnOrder cbOrder, FuncOnTrade cbTrade, FuncOnPosition cbPosition)
	{
		_cb_account = cbAccount;
		_cb_order = cbOrder;
		_cb_position = cbPosition;
		_cb_trade = cbTrade;
	}

	void init(const char* logProfile);

	bool config(const char* cfgfile, bool isFile, const char* modDir);

	void run(bool bOnce = true);

	void release();

	void on_account(const char* channelid, uint32_t curTDate, const char* currency, double prebalance, double balance,
		double dynbalance, double closeprofit, double dynprofit, double fee, double margin, double deposit, double withdraw, bool isLast);

	void on_order(const char* channelid, const char* exchg, const char* code, uint32_t curTDate,
		const char* orderid, uint32_t direct, uint32_t offset, double volume, double leftover, double traded, double price, uint32_t ordertype,
		uint32_t pricetype, WtUInt64 ordertime, uint32_t state, const char* statemsg, bool isLast);

	void on_trade(const char* channelid, const char* exchg, const char* code, uint32_t curTDate,
		const char* tradeid, const char* orderid, uint32_t direct, uint32_t offset, double volume, double price,
		double amount, uint32_t ordertype, uint32_t tradetype, WtUInt64 tradetime, bool isLast);

	void on_position(const char* channelid, const char* exchg, const char* code, uint32_t curTDate, uint32_t direct,
		double volume, double cost, double margin, double avgpx, double dynprofit, uint32_t volscale, bool isLast);

private:
	FuncOnAccount	_cb_account;
	FuncOnOrder		_cb_order;
	FuncOnTrade		_cb_trade;
	FuncOnPosition	_cb_position;

	StdThreadPtr	_worker;

	uint32_t		_refresh_span;
	bool			_stopped;
};

