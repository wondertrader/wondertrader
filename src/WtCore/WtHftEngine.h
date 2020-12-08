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
#include "../Share/BoostDefine.h"

NS_OTP_BEGIN

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
	//WtEngine ½Ó¿Ú
	virtual void init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDataManager* dataMgr, IHotMgr* hotMgr) override;

	virtual void run(bool bAsync = false) override;

	virtual void handle_push_quote(WTSTickData* newTick, bool isHot) override;

	virtual void on_tick(const char* stdCode, WTSTickData* curTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

public:
	void on_minute_end(uint32_t curDate, uint32_t curTime);

	void addContext(HftContextPtr ctx);

	HftContextPtr	getContext(uint32_t id);

private:
	typedef std::unordered_map<uint32_t, HftContextPtr> ContextMap;
	ContextMap		_ctx_map;

	WtHftRtTicker*	_tm_ticker;
	WTSVariant*		_cfg;
};

NS_OTP_END