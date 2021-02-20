/*!
 * \file WtBtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"
#include "../WtBtCore/HisDataReplayer.h"
#include "../Includes/WTSMarcos.h"


NS_OTP_BEGIN
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

typedef enum tagEngineType
{
	ET_CTA = 999,	//CTA引擎	
	ET_HFT,			//高频引擎
	ET_SEL			//选股引擎
} EngineType;

class SelMocker;
class CtaMocker;
class HftMocker;
class ExecMocker;

class WtBtRunner
{
public:
	WtBtRunner();
	~WtBtRunner();

public:
	void	registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt);
	void	registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt);
	void registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
		FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
		FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt);

	void registerEvtCallback(FuncEventCallback cbEvt)
	{
		_cb_evt = cbEvt;
	}

	uint32_t	initCtaMocker(const char* name);
	uint32_t	initHftMocker(const char* name);
	uint32_t	initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	void	ctx_on_init(uint32_t id, EngineType eType);
	void	ctx_on_session_event(uint32_t id, uint32_t curTDate, bool isBegin = true, EngineType eType = ET_CTA);
	void	ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, EngineType eType);
	void	ctx_on_calc(uint32_t id, uint32_t uDate, uint32_t uTime, EngineType eType);
	void	ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, EngineType eType);

	void	hft_on_order_queue(uint32_t id, const char* stdCode, WTSOrdQueData* newOrdQue);
	void	hft_on_order_detail(uint32_t id, const char* stdCode, WTSOrdDtlData* newOrdDtl);
	void	hft_on_transaction(uint32_t id, const char* stdCode, WTSTransData* newTranns);

	void	hft_on_channel_ready(uint32_t cHandle, const char* trader);
	void	hft_on_order(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
	void	hft_on_trade(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
	void	hft_on_entrust(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);

	void	init(const char* logProfile = "", bool isFile = true);
	void	config(const char* cfgFile, bool isFile = true);
	void	run();
	void	release();

	void	dump_bars(const char* stdCode, const char* period, const char* filename);

	CtaMocker*			cta_mocker() { return _cta_mocker; }
	SelMocker*			sel_mocker() { return _sel_mocker; }
	HftMocker*			hft_mocker() { return _hft_mocker; }
	HisDataReplayer&	replayer() { return _replayer; }

public:
	inline void on_initialize_event()
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_INIT, 0, 0);
	}

	inline void on_schedule_event(uint32_t uDate, uint32_t uTime)
	{
		if (_cb_evt)
			_cb_evt(EVENT_ENGINE_SCHDL, uDate, uTime);
	}

	inline void on_session_event(uint32_t uDate, bool isBegin = true)
	{
		if (_cb_evt)
		{
			_cb_evt(isBegin ? EVENT_SESSION_BEGIN : EVENT_SESSION_END, uDate, 0);
		}
	}

private:
	FuncStraInitCallback	_cb_cta_init;
	FuncSessionEvtCallback	_cb_cta_sessevt;
	FuncStraTickCallback	_cb_cta_tick;
	FuncStraCalcCallback	_cb_cta_calc;
	FuncStraBarCallback		_cb_cta_bar;

	FuncStraInitCallback	_cb_sel_init;
	FuncSessionEvtCallback	_cb_sel_sessevt;
	FuncStraTickCallback	_cb_sel_tick;
	FuncStraCalcCallback	_cb_sel_calc;
	FuncStraBarCallback		_cb_sel_bar;

	FuncStraInitCallback	_cb_hft_init;
	FuncSessionEvtCallback	_cb_hft_sessevt;
	FuncStraTickCallback	_cb_hft_tick;
	FuncStraCalcCallback	_cb_hft_calc;
	FuncStraBarCallback		_cb_hft_bar;
	FuncHftChannelCallback	_cb_hft_chnl;
	FuncHftOrdCallback		_cb_hft_ord;
	FuncHftTrdCallback		_cb_hft_trd;
	FuncHftEntrustCallback	_cb_hft_entrust;

	FuncStraOrdQueCallback	_cb_hft_ordque;
	FuncStraOrdDtlCallback	_cb_hft_orddtl;
	FuncStraTransCallback	_cb_hft_trans;

	FuncEventCallback		_cb_evt;

	CtaMocker*		_cta_mocker;
	SelMocker*		_sel_mocker;
	ExecMocker*		_exec_mocker;
	HftMocker*		_hft_mocker;
	HisDataReplayer	_replayer;
};

