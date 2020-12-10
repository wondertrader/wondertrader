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
	void	registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);
	void	registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);
	void registerHftCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
		FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust);

	uint32_t	initCtaMocker(const char* name);
	uint32_t	initHftMocker(const char* name);
	uint32_t	initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	void	ctx_on_init(uint32_t id, EngineType eType);
	void	ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, EngineType eType);
	void	ctx_on_calc(uint32_t id, uint32_t uDate, uint32_t uTime, EngineType eType);
	void	ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, EngineType eType);

	void	hft_on_channel_ready(uint32_t cHandle, const char* trader);
	void	hft_on_order(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled);
	void	hft_on_trade(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price);
	void	hft_on_entrust(uint32_t cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message);

	void	init(const char* logProfile = "", bool isFile = true);
	void	config(const char* cfgFile, bool isFile = true);
	void	run();
	void	release();

	void	dump_bars(const char* stdCode, const char* period, const char* filename);

	CtaMocker*			cta_mocker() { return _cta_mocker; }
	SelMocker*			sel_mocker() { return _sel_mocker; }
	HftMocker*			hft_mocker() { return _hft_mocker; }
	HisDataReplayer&	replayer() { return _replayer; }


	static void trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period);

private:
	FuncStraInitCallback	_cb_cta_init;
	FuncStraTickCallback	_cb_cta_tick;
	FuncStraCalcCallback	_cb_cta_calc;
	FuncStraBarCallback		_cb_cta_bar;

	FuncStraInitCallback	_cb_sel_init;
	FuncStraTickCallback	_cb_sel_tick;
	FuncStraCalcCallback	_cb_sel_calc;
	FuncStraBarCallback		_cb_sel_bar;

	FuncStraInitCallback	_cb_hft_init;
	FuncStraTickCallback	_cb_hft_tick;
	FuncStraCalcCallback	_cb_hft_calc;
	FuncStraBarCallback		_cb_hft_bar;
	FuncHftChannelCallback	_cb_hft_chnl;
	FuncHftOrdCallback		_cb_hft_ord;
	FuncHftTrdCallback		_cb_hft_trd;
	FuncHftEntrustCallback	_cb_hft_entrust;

	CtaMocker*		_cta_mocker;
	SelMocker*		_sel_mocker;
	ExecMocker*		_exec_mocker;
	HftMocker*		_hft_mocker;
	HisDataReplayer	_replayer;
};

