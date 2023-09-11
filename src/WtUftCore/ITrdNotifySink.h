/*!
 * \file ITrdNotifySink.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN

class ITrdNotifySink
{
public:
	/*
	 *	成交回报
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price) = 0;

	/*
	 *	订单回报
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled = false) = 0;

	/*
	 *	持仓更新回调
	 */
	virtual void on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday) {}

	/*
	 *	交易通道就绪
	 */
	virtual void on_channel_ready(uint32_t tradingday) = 0;

	/*
	 *	交易通道丢失
	 */
	virtual void on_channel_lost() = 0;

	/*
	 *	下单回报
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message){}
};

NS_WTP_END