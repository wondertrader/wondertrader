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
#include "../Share/WTSMarcos.h"


NS_OTP_BEGIN
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

class SelMocker;
class CtaMocker;
class ExecMocker;

class WtBtRunner
{
public:
	WtBtRunner();
	~WtBtRunner();

public:
	void	registerCtaCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);
	void	registerSelCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	uint32_t	initCtaMocker(const char* name);
	uint32_t	initSelMocker(const char* name, uint32_t date, uint32_t time, const char* period);

	void	ctx_on_init(uint32_t id, bool isCta = true);
	void	ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick, bool isCta = true);
	void	ctx_on_calc(uint32_t id, bool isCta = true);
	void	ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar, bool isCta = true);

	void	init(const char* logProfile = "");
	void	config(const char* cfgFile);
	void	run();
	void	release();

	void	dump_bars(const char* code, const char* period, const char* filename);

	CtaMocker*			cta_mocker() { return _cta_mocker; }
	SelMocker*			sel_mocker() { return _sel_mocker; }
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

	CtaMocker*		_cta_mocker;
	SelMocker*		_sel_mocker;
	ExecMocker*		_exec_mocker;
	HisDataReplayer	_replayer;
};

