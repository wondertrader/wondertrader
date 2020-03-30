#pragma once
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		register_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar);

	EXPORT_FLAG	void		init_backtest(const char* logProfile);

	EXPORT_FLAG	void		config_backtest(const char* cfgfile);

	EXPORT_FLAG	CtxHandler	init_cta_mocker(const char* name);

	EXPORT_FLAG	void		run_backtest();

	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		dump_bars(const char* code, const char* period, const char* filename);

	EXPORT_FLAG	void		trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period);

	EXPORT_FLAG	void		release_backtest();


	/*
	 *	Context×¨Êô½Ó¿Ú
	 */
	EXPORT_FLAG	void		ctx_str_enter_long(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		ctx_str_exit_long(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		ctx_str_enter_short(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		ctx_str_exit_short(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	double		ctx_str_get_position_profit(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	WtUInt64	ctx_str_get_detail_entertime(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	double		ctx_str_get_detail_cost(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	double		ctx_str_get_detail_profit(CtxHandler cHandle, const char* code, const char* openTag, int flag);

	EXPORT_FLAG	double		ctx_str_get_position_avgpx(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	int			ctx_str_get_position(CtxHandler cHandle, const char* code, const char* openTag);

	EXPORT_FLAG	void		ctx_str_set_position(CtxHandler cHandle, const char* code, int qty, const char* uesrTag, double limitprice, double stopprice);

	EXPORT_FLAG	double 		ctx_str_get_price(const char* code);

	EXPORT_FLAG	WtUInt32 	ctx_str_get_date();

	EXPORT_FLAG	WtUInt32 	ctx_str_get_time();

	EXPORT_FLAG	WtUInt32	ctx_str_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt, bool isMain, FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	ctx_str_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG	WtUInt64	ctx_str_get_first_entertime(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	WtUInt64	ctx_str_get_last_entertime(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	double		ctx_str_get_last_enterprice(CtxHandler cHandle, const char* code);

	EXPORT_FLAG	void		ctx_str_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		ctx_str_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	ctx_str_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);


#ifdef __cplusplus
}
#endif