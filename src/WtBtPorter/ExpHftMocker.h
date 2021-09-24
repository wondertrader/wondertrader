#pragma once
#include "../WtBtCore/HftMocker.h"

class ExpHftMocker : public HftMocker
{
public:
	ExpHftMocker(HisDataReplayer* replayer, const char* name);
	virtual ~ExpHftMocker(){}

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;


	virtual void on_channel_ready() override;


	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag) override;


	virtual void on_init() override;


	virtual void on_session_begin(uint32_t uDate) override;


	virtual void on_session_end(uint32_t uDate) override;


	virtual void on_bactest_end() override;


	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag) override;


	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) override;


	virtual void on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue) override;


	virtual void on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl) override;


	virtual void on_trans_updated(const char* stdCode, WTSTransData* newTrans) override;


	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag) override;

};

