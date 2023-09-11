/*!
 * \file PorterDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include "../Includes/WTSTypes.h"

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncOnAccount)(const char* channelid, WtUInt32 curTDate, const char* currency, double prebalance, double balance, 
	double dynbalance, double closeprofit, double dynprofit, double fee, double margin, double deposit, double withdraw, bool isLast);

typedef void(PORTER_FLAG *FuncOnOrder)(const char* channelid, const char* exchg, const char* code, WtUInt32 curTDate, 
	const char* orderid, WtUInt32 direct, WtUInt32 offset, double volume, double leftover, double traded, double price, WtUInt32 ordertype, 
	WtUInt32 pricetype, WtUInt64 ordertime, WtUInt32 state, const char* statemsg, bool isLast);

typedef void(PORTER_FLAG *FuncOnTrade)(const char* channelid, const char* exchg, const char* code, WtUInt32 curTDate,  
	const char* tradeid, const char* orderid, WtUInt32 direct, WtUInt32 offset, double volume, double price, 
	double amount, WtUInt32 ordertype, WtUInt32 tradetype, WtUInt64 tradetime, bool isLast);

typedef void(PORTER_FLAG *FuncOnPosition)(const char* channelid, const char* exchg, const char* code, WtUInt32 curTDate, WtUInt32 direct,
	double volume, double cost, double margin, double avgpx, double dynprofit, WtUInt32 volscale, bool isLast);

