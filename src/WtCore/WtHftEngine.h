/*!
 * \file WtHftEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "WtEngine.h"
#include "WtLocalExecuter.h"

#include "../Includes/IHftStraCtx.h"

NS_WTP_BEGIN

class WTSVariant;
class WtHftRtTicker;

typedef std::shared_ptr<IHftStraCtx> HftContextPtr;

class WtHftEngine :	public WtEngine
{
public:
	WtHftEngine();
	virtual ~WtHftEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine 接口
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier) override;

	virtual void run() override;

	virtual void handle_push_quote(WTSTickData* newTick) override;
	virtual void handle_push_order_detail(WTSOrdDtlData* curOrdDtl) override;
	virtual void handle_push_order_queue(WTSOrdQueData* curOrdQue) override;
	virtual void handle_push_transaction(WTSTransData* curTrans) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

public:
	WTSOrdQueSlice* get_order_queue_slice(uint32_t sid, const char* stdCode, uint32_t count);
	WTSOrdDtlSlice* get_order_detail_slice(uint32_t sid, const char* stdCode, uint32_t count);
	WTSTransSlice* get_transaction_slice(uint32_t sid, const char* stdCode, uint32_t count);

public:
	void on_minute_end(uint32_t curDate, uint32_t curTime);

	void addContext(HftContextPtr ctx);

	HftContextPtr	getContext(uint32_t id);

	void sub_order_queue(uint32_t sid, const char* stdCode);
	void sub_order_detail(uint32_t sid, const char* stdCode);
	void sub_transaction(uint32_t sid, const char* stdCode);

private:
	typedef wt_hashmap<uint32_t, HftContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtHftRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;


	StraSubMap		_ordque_sub_map;	//委托队列订阅表
	StraSubMap		_orddtl_sub_map;	//委托明细订阅表
	StraSubMap		_trans_sub_map;		//成交明细订阅表
};

NS_WTP_END