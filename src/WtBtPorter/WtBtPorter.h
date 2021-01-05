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

	EXPORT_FLAG	void		register_hft_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
		FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust);

	EXPORT_FLAG	void		init_backtest(const char* logProfile, bool isFile);

	EXPORT_FLAG	void		config_backtest(const char* cfgfile, bool isFile);

	EXPORT_FLAG	CtxHandler	init_cta_mocker(const char* name);

	EXPORT_FLAG	CtxHandler	init_hft_mocker(const char* name);

	EXPORT_FLAG	CtxHandler	init_sel_mocker(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl = "CHINA", const char* session = "TRADING");

	EXPORT_FLAG	void		run_backtest();

	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		dump_bars(const char* stdCode, const char* period, const char* filename);

	EXPORT_FLAG	void		release_backtest();


	//////////////////////////////////////////////////////////////////////////
	//CTA策略接口
#pragma region "CTA接口"
	EXPORT_FLAG	void		cta_enter_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_enter_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	void		cta_exit_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice);

	EXPORT_FLAG	double		cta_get_position_profit(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	WtUInt64	cta_get_detail_entertime(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_cost(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	double		cta_get_detail_profit(CtxHandler cHandle, const char* stdCode, const char* openTag, int flag);

	EXPORT_FLAG	double		cta_get_position_avgpx(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		cta_get_position(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	void		cta_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* uesrTag, double limitprice, double stopprice);

	EXPORT_FLAG	double 		cta_get_price(const char* stdCode);

	EXPORT_FLAG	WtUInt32 	cta_get_tdate();

	EXPORT_FLAG	WtUInt32 	cta_get_date();

	EXPORT_FLAG	WtUInt32 	cta_get_time();

	EXPORT_FLAG	WtUInt32	cta_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, unsigned int barCnt, bool isMain, FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	cta_get_ticks(CtxHandler cHandle, const char* stdCode, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG void		cta_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb);

	EXPORT_FLAG	WtUInt64	cta_get_first_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	WtUInt64	cta_get_last_entertime(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		cta_get_last_enterprice(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	void		cta_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		cta_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	cta_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);

	EXPORT_FLAG	void		cta_sub_ticks(CtxHandler cHandle, const char* stdCode);
#pragma endregion "CTA接口"

	//////////////////////////////////////////////////////////////////////////
	//选股策略接口
#pragma  region "选股接口"
	EXPORT_FLAG	double		sel_get_position(CtxHandler cHandle, const char* stdCode, const char* openTag);

	EXPORT_FLAG	void		sel_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* uesrTag);

	EXPORT_FLAG	double 		sel_get_price(const char* stdCode);

	EXPORT_FLAG	WtUInt32 	sel_get_date();

	EXPORT_FLAG	WtUInt32 	sel_get_time();

	EXPORT_FLAG	WtUInt32	sel_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, unsigned int barCnt,FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	sel_get_ticks(CtxHandler cHandle, const char* stdCode, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG void		sel_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb);

	EXPORT_FLAG	void		sel_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		sel_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	sel_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);

	EXPORT_FLAG	void		sel_sub_ticks(CtxHandler cHandle, const char* stdCode);
#pragma endregion "选股接口"

	//////////////////////////////////////////////////////////////////////////
//HFT策略接口
#pragma  region "HFT接口"

	EXPORT_FLAG	double		hft_get_position(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double		hft_get_undone(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	double 		hft_get_price(const char* stdCode);

	EXPORT_FLAG	WtUInt32 	hft_get_date();

	EXPORT_FLAG	WtUInt32 	hft_get_time();

	EXPORT_FLAG	WtUInt32 	hft_get_secs();

	EXPORT_FLAG	WtUInt32	hft_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, unsigned int barCnt, FuncGetBarsCallback cb);

	EXPORT_FLAG	WtUInt32	hft_get_ticks(CtxHandler cHandle, const char* stdCode, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb);

	EXPORT_FLAG	void		hft_log_text(CtxHandler cHandle, const char* message);

	EXPORT_FLAG	void		hft_sub_ticks(CtxHandler cHandle, const char* stdCode);

	EXPORT_FLAG	bool		hft_cancel(CtxHandler cHandle, WtUInt32 localid);

	EXPORT_FLAG	WtString	hft_cancel_all(CtxHandler cHandle, const char* stdCode, bool isBuy);

	EXPORT_FLAG	WtString	hft_buy(CtxHandler cHandle, const char* stdCode, double price, double qty);

	EXPORT_FLAG	WtString	hft_sell(CtxHandler cHandle, const char* stdCode, double price, double qty);

	EXPORT_FLAG	void		hft_save_userdata(CtxHandler cHandle, const char* key, const char* val);

	EXPORT_FLAG	WtString	hft_load_userdata(CtxHandler cHandle, const char* key, const char* defVal);
#pragma endregion "HFT接口"
#ifdef __cplusplus
}
#endif