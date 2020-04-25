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

class ICtaEventListener
{
public:
	virtual void on_initialize_event() {}
	virtual void on_schedule_event(uint32_t uDate, uint32_t uTime) {}
	virtual void on_session_event(uint32_t uDate, bool isBegin = true) {}
};

class WtCtaEngine : public WtEngine
{
public:
	WtCtaEngine();
	virtual ~WtCtaEngine();

public:
	//////////////////////////////////////////////////////////////////////////
	//WtEngine½Ó¿Ú
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

public:
	void on_schedule(uint32_t curDate, uint32_t curTime);	

	void handle_pos_change(const char* stdCode, double diffQty);

	void addContext(CtaContextPtr ctx);
	
	CtaContextPtr	getContext(uint32_t id);

	inline void addExecuter(WtExecuterPtr& executer)
	{
		_executers.push_back(executer);
		executer->setEngine(this);
	}

	inline void regEventListener(ICtaEventListener* listener)
	{
		_evt_listener = listener;
	}

private:
	typedef std::unordered_map<uint32_t, CtaContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtCtaRtTicker*	_tm_ticker;
	ICtaEventListener*	_evt_listener;

	typedef std::vector<WtExecuterPtr> ExecuterList;
	ExecuterList	_executers;

	WTSVariant*		_cfg;
};

NS_OTP_END

