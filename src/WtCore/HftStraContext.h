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
#include "HftStraBaseCtx.h"


USING_NS_OTP;

class HftStrategy;

class HftStraContext : public HftStraBaseCtx
{
public:
	HftStraContext(WtHftEngine* engine, const char* name);
	virtual ~HftStraContext();

	void set_strategy(HftStrategy* stra){ _strategy = stra; }
	HftStrategy* get_stragety() { return _strategy; }

public:
	virtual void on_init() override;

	virtual void on_tick(const char* code, WTSTickData* newTick) override;

	virtual void on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar) override;
	
	virtual void on_schedule(uint32_t uDate, uint32_t uTime) override;

	virtual void on_trade(const char* stdCode, bool isBuy, uint32_t vol, double price) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, uint32_t totalQty, uint32_t leftQty, double price, bool isCanceled = false) override;

	virtual void on_position(const char* stdCode, bool isLong, uint32_t prevol, uint32_t preavail, uint32_t newvol, uint32_t newavail) override;

	virtual void on_channel_ready() override;

	virtual void on_channel_lost() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;


private:
	HftStrategy*		_strategy;
};

