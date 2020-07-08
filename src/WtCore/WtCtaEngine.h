/*!
 * \file WtCtaEngine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "ICtaStraCtx.h"
#include "WtExecuter.h"
#include "WtEngine.h"

NS_OTP_BEGIN
class WTSVariant;
typedef boost::shared_ptr<ICtaStraCtx> CtaContextPtr;

class WtCtaRtTicker;

class WtCtaEngine : public WtEngine, public IExecuterStub
{
public:
	WtCtaEngine();
	virtual ~WtCtaEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine接口
	virtual void handle_push_quote(WTSTickData* newTick, bool isHot) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_init() override;
	virtual void on_session_begin() override;
	virtual void on_session_end() override;

	virtual void run(bool bAsync = false) override;

	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr) override;

	virtual bool isInTrading() override;
	virtual uint32_t transTimeToMin(uint32_t uTime) override;

	///////////////////////////////////////////////////////////////////////////
	//IExecuterStub 接口
	virtual uint64_t get_real_time() override;
	virtual WTSCommodityInfo* get_comm_info(const char* stdCode) override;
	virtual WTSSessionInfo* get_sess_info(const char* stdCode) override;
	virtual IHotMgr* get_hot_mon() { return _hot_mgr; }
	virtual uint32_t get_trading_day() { return _cur_tdate; }


public:
	void on_schedule(uint32_t curDate, uint32_t curTime);	

	void handle_pos_change(const char* stdCode, double diffQty);

	void addContext(CtaContextPtr ctx);
	
	CtaContextPtr	getContext(uint32_t id);

	inline void addExecuter(ExecCmdPtr executer)
	{
		_exec_mgr.add_executer(executer);
		executer->setStub(this);
	}

private:
	typedef std::unordered_map<uint32_t, CtaContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtCtaRtTicker*	_tm_ticker;

	WtExecuterMgr	_exec_mgr;

	WTSVariant*		_cfg;
};

NS_OTP_END

