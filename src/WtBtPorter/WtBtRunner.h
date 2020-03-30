#pragma once
#include "PorterDefs.h"
#include "../WtBtCore/HisDataReplayer.h"
#include "../Share/WTSMarcos.h"


NS_OTP_BEGIN
class WTSTickData;
struct WTSBarStruct;
NS_OTP_END

USING_NS_OTP;

class CtaMocker;
class ExecMocker;

class WtBtRunner
{
public:
	WtBtRunner();
	~WtBtRunner();

public:
	void	registerCallbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	uint32_t	initCtaMocker(const char* name);

	void	ctx_on_init(uint32_t id);
	void	ctx_on_tick(uint32_t id, const char* stdCode, WTSTickData* newTick);
	void	ctx_on_calc(uint32_t id);
	void	ctx_on_bar(uint32_t id, const char* stdCode, const char* period, WTSBarStruct* newBar);

	void	init(const char* logProfile = "");
	void	config(const char* cfgFile);
	void	run();
	void	release();

	void	dump_bars(const char* code, const char* period, const char* filename);

	CtaMocker*			cta_mocker() { return _cta_mocker; }
	HisDataReplayer&	replayer() { return _replayer; }


	static void trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period);

private:
	FuncStraInitCallback	_cb_init;
	FuncStraTickCallback	_cb_tick;
	FuncStraCalcCallback	_cb_calc;
	FuncStraBarCallback		_cb_bar;

	CtaMocker*		_cta_mocker;
	ExecMocker*		_exec_mocker;
	HisDataReplayer	_replayer;
};

