/*!
 * \file WtBtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		register_cta_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	EXPORT_FLAG	void		register_sel_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	EXPORT_FLAG	void		init_backtest(const char* logProfile);

	EXPORT_FLAG	void		config_backtest(const char* cfgfile);

	EXPORT_FLAG	CtxHandler	init_cta_mocker(const char* name);

	EXPORT_FLAG	CtxHandler	init_sel_mocker(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	EXPORT_FLAG	void		run_backtest();

	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		dump_bars(const char* code, const char* period, const char* filename);

	EXPORT_FLAG	void		trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period);

	EXPORT_FLAG	void		release_backtest();


	/*
	 *	Context专属接口
	 */
	EXPORT_FLAG	void		cta_enter_long(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_long(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_enter_short(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_short(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	double		cta_get_position_profit(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	WtUInt64	cta_get_detail_entertime(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_cost(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_profit(CtxHandler cHandle, const char* code, const char* openTag, int flag);

	EXPORT_FLAG	double		cta_get_position_avgpx(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	double		cta_get_position(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	void		cta_set_position(CtxHandler cHandle, const char* code, double qty, const char* uesrTag, double limitprice, double stopprice);

	EXPORT_FLAG	double 		cta_get_price(const char* code);

	EXPORT_FLAG	WtUInt32 	cta_get_date();

	EXPORT_FLAG	WtUInt32 	cta_get_time();

	EXPORT_FLAG	WtUInt32	cta_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt, bool isMain, FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	cta_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG	WtUInt64	cta_get_first_entertime(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	WtUInt64	cta_get_last_entertime(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	double		cta_get_last_enterprice(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	void		cta_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		cta_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	cta_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);

	//////////////////////////////////////////////////////////////////////////
	//多因子策略接口
	EXPORT_FLAG	double		sel_get_position(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	void		sel_set_position(CtxHandler cHandle, const char* code, double qty, const char* uesrTag);

	EXPORT_FLAG	double 		sel_get_price(const char* code);

	EXPORT_FLAG	WtUInt32 	sel_get_date();

	EXPORT_FLAG	WtUInt32 	sel_get_time();

	EXPORT_FLAG	WtUInt32	sel_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt,FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	sel_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG	void		sel_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		sel_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	sel_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);


#ifdef __cplusplus
}
#endif