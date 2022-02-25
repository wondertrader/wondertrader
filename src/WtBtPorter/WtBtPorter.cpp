/*!
 * \file WtBtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtBtPorter.h"
#include "WtBtRunner.h"

#include "../WtBtCore/WtHelper.h"
#include "../WtBtCore/CtaMocker.h"
#include "../WtBtCore/SelMocker.h"
#include "../WtBtCore/HftMocker.h"

#include "../WTSTools/WTSLogger.h"

#include "../Includes/WTSVersion.h"


#ifdef _WIN32
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "X86";
#endif
#else
char PLATFORM_NAME[] = "UNIX";
#endif


WtBtRunner& getRunner()
{
	static WtBtRunner runner;
	return runner;
}

void register_evt_callback(FuncEventCallback cbEvt)
{
	getRunner().registerEvtCallback(cbEvt);
}

void register_cta_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, 
	FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone/* = NULL*/)
{
	getRunner().registerCtaCallbacks(cbInit, cbTick, cbCalc, cbBar, cbSessEvt, cbCalcDone);
}

void register_sel_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, 
	FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt, FuncStraCalcCallback cbCalcDone/* = NULL*/)
{
	getRunner().registerSelCallbacks(cbInit, cbTick, cbCalc, cbBar, cbSessEvt, cbCalcDone);
}

void register_hft_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar,
	FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
	FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt)
{
	getRunner().registerHftCallbacks(cbInit, cbTick, cbBar, cbChnl, cbOrd, cbTrd, cbEntrust, cbOrdDtl, cbOrdQue, cbTrans, cbSessEvt);
}

void register_ext_data_loader(FuncLoadFnlBars fnlBarLoader, FuncLoadRawBars rawBarLoader, FuncLoadAdjFactors fctLoader, FuncLoadRawTicks tickLoader, bool bAutoTrans)
{
	getRunner().registerExtDataLoader(fnlBarLoader, rawBarLoader, fctLoader, tickLoader, bAutoTrans);
}

void feed_raw_bars(WTSBarStruct* bars, WtUInt32 count)
{
	getRunner().feedRawBars(bars, count);
}

void feed_raw_ticks(WTSTickStruct* ticks, WtUInt32 count)
{
	getRunner().feedRawTicks(ticks, count);
}

void init_backtest(const char* logProfile, bool isFile, const char* outDir)
{
	static bool inited = false;

	if (inited)
		return;

	getRunner().init(logProfile, isFile, outDir);

	inited = true;
}

void config_backtest(const char* cfgfile, bool isFile)
{
	static bool inited = false;

	if (inited)
		return;

	if (strlen(cfgfile) == 0)
		getRunner().config("configbt.json", true);
	else
		getRunner().config(cfgfile, isFile);
}

void set_time_range(WtUInt64 stime, WtUInt64 etime)
{
	getRunner().set_time_range(stime, etime);
}

void enable_tick(bool bEnabled /* = true */)
{
	getRunner().enable_tick(bEnabled);
}

void run_backtest(bool bNeedDump, bool bAsync)
{
	getRunner().run(bNeedDump, bAsync);
}

void stop_backtest()
{
	getRunner().stop();
}

void release_backtest()
{
	getRunner().release();
}

const char* get_version()
{
	static std::string _ver;
	if(_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += WT_VERSION;
		_ver += " Build@";
		_ver += __DATE__;
		_ver += " ";
		_ver += __TIME__;
	}
	return _ver.c_str();
}

void clear_cache()
{
	getRunner().clear_cache();
}

void write_log(WtUInt32 level, const char* message, const char* catName)
{
	if (strlen(catName) > 0)
	{
		WTSLogger::log_raw_by_cat(catName, (WTSLogLevel)level, message);
	}
	else
	{
		WTSLogger::log_raw((WTSLogLevel)level, message);
	}
}

CtxHandler init_cta_mocker(const char* name, int slippage/* = 0*/, bool hook/* = false*/, bool persistData/* = true*/)
{
	return getRunner().initCtaMocker(name, slippage, hook, persistData);
}

CtxHandler init_hft_mocker(const char* name, bool hook/* = false*/)
{
	return getRunner().initHftMocker(name, hook);
}

CtxHandler init_sel_mocker(const char* name, WtUInt32 date, WtUInt32 time, const char* period, const char* trdtpl/* = "CHINA"*/, const char* session/* = "TRADING"*/, int slippage/* = 0*/)
{
	return getRunner().initSelMocker(name, date, time, period, trdtpl, session, slippage);
}

#pragma region "CTA策略接口"
void cta_enter_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_enter_long(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_exit_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_exit_long(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_enter_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_enter_short(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_exit_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_exit_short(stdCode, qty, userTag, limitprice, stopprice);
}

WtUInt32 cta_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, bool isMain, FuncGetBarsCallback cb)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSKlineSlice* kData = ctx->stra_get_bars(stdCode, period, barCnt, isMain);
		if (kData)
		{
			WtUInt32 reaCnt = (WtUInt32)kData->size();

			for (uint32_t i = 0; i < kData->get_block_counts(); i++)
				cb(cHandle, stdCode, period, kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts()-1);

			kData->release();
			return reaCnt;
		}
		else
		{
			return 0;
		}
	}
	catch(...)
	{
		return 0;
	}
}

WtUInt32	cta_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSTickSlice* tData = ctx->stra_get_ticks(stdCode, tickCnt);
		if (tData)
		{
			uint32_t thisCnt = min(tickCnt, (WtUInt32)tData->size());
			cb(cHandle, stdCode, (WTSTickStruct*)tData->at(0), thisCnt, true);
			tData->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

double cta_get_position_profit(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_profit(stdCode);
}

WtUInt64 cta_get_detail_entertime(CtxHandler cHandle, const char* stdCode, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_entertime(stdCode, openTag);
}

double cta_get_detail_cost(CtxHandler cHandle, const char* stdCode, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_cost(stdCode, openTag);
}

double cta_get_detail_profit(CtxHandler cHandle, const char* stdCode, const char* openTag, int flag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_profit(stdCode, openTag, flag);
}

double cta_get_position_avgpx(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_avgpx(stdCode);
}

void cta_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
	{
		cb(cHandle, "", 0, true);
		return;
	}

	ctx->enum_position([cb, cHandle](const char* stdCode, double qty) {
		cb(cHandle, stdCode, qty, false);
	});

	cb(cHandle, "", 0, true);
}

double cta_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(stdCode, bOnlyValid, openTag);
}

double cta_get_fund_data(CtxHandler cHandle, int flag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_fund_data(flag);
}

void cta_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_set_position(stdCode, qty, userTag, limitprice, stopprice);
}

WtUInt64 cta_get_first_entertime(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_first_entertime(stdCode);
}

WtUInt64 cta_get_last_entertime(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_entertime(stdCode);
}

WtUInt64 cta_get_last_exittime(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_exittime(stdCode);
}

double cta_get_last_enterprice(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_enterprice(stdCode);
}

double cta_get_price(const char* stdCode)
{
	return getRunner().replayer().get_cur_price(stdCode);
}

WtUInt32 cta_get_tdate()
{
	return getRunner().replayer().get_trading_date();
}

WtUInt32 cta_get_date()
{
	return getRunner().replayer().get_date();
}

WtUInt32 cta_get_time()
{
	return getRunner().replayer().get_min_time();
}

void cta_log_text(CtxHandler cHandle, const char* message)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_log_info(message);
}

void cta_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString cta_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}

void cta_sub_ticks(CtxHandler cHandle, const char* stdCode)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return ;

	ctx->stra_sub_ticks(stdCode);
}

bool cta_step(CtxHandler cHandle)
{
	//只有异步模式才有意义
	if (!getRunner().isAsync())
		return false;

	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return false;

	return ctx->step_calc();
}

#pragma endregion "CTA策略接口"

#pragma region "SEL策略接口"
void sel_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString sel_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}

void sel_log_text(CtxHandler cHandle, const char* message)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_log_info(message);
}

double sel_get_price(const char* stdCode)
{
	return getRunner().replayer().get_cur_price(stdCode);
}

WtUInt32 sel_get_date()
{
	return getRunner().replayer().get_date();
}

WtUInt32 sel_get_time()
{
	return getRunner().replayer().get_min_time();
}

void sel_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
	{
		cb(cHandle, "", 0, true);
		return;
	}

	ctx->enum_position([cb, cHandle](const char* stdCode, double qty) {
		cb(cHandle, stdCode, qty, false);
	});

	cb(cHandle, "", 0, true);
}

double sel_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid, const char* openTag)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(stdCode, bOnlyValid, openTag);
}

WtUInt32 sel_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, FuncGetBarsCallback cb)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSKlineSlice* kData = ctx->stra_get_bars(stdCode, period, barCnt);
		if (kData)
		{
			WtUInt32 reaCnt = (WtUInt32)kData->size();

			for (uint32_t i = 0; i < kData->get_block_counts(); i++)
				cb(cHandle, stdCode, period, kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts() - 1);

			kData->release();
			return reaCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

void sel_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return;

	//多因子引擎,限价和止价都无效
	ctx->stra_set_position(stdCode, qty, userTag);
}

WtUInt32	sel_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSTickSlice* tData = ctx->stra_get_ticks(stdCode, tickCnt);
		if (tData)
		{
			uint32_t thisCnt = min(tickCnt, (WtUInt32)tData->size());
			cb(cHandle, stdCode, (WTSTickStruct*)tData->at(0), thisCnt, true);
			tData->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

void sel_sub_ticks(CtxHandler cHandle, const char* stdCode)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_sub_ticks(stdCode);
}

#pragma endregion "SEL策略接口"

#pragma region "HFT策略接口"
double hft_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;

	return mocker->stra_get_position(stdCode, bOnlyValid);
}

double hft_get_position_profit(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;

	return mocker->stra_get_position_profit(stdCode);
}

double hft_get_undone(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;

	return mocker->stra_get_undone(stdCode);
}

double hft_get_price(const char* stdCode)
{
	return getRunner().replayer().get_cur_price(stdCode);
}

WtUInt32 hft_get_date()
{
	return getRunner().replayer().get_date();
}

WtUInt32 hft_get_time()
{
	return getRunner().replayer().get_raw_time();
}

WtUInt32 hft_get_secs()
{
	return getRunner().replayer().get_secs();
}

WtUInt32 hft_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, FuncGetBarsCallback cb)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;

	try
	{
		WTSKlineSlice* kData = mocker->stra_get_bars(stdCode, period, barCnt);
		if (kData)
		{
			WtUInt32 reaCnt = (WtUInt32)kData->size();

			for (uint32_t i = 0; i < kData->get_block_counts(); i++)
				cb(cHandle, stdCode, period, kData->get_block_addr(i), kData->get_block_size(i), i == kData->get_block_counts() - 1);

			kData->release();
			return reaCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

WtUInt32 hft_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;
	try
	{
		WTSTickSlice* tData = mocker->stra_get_ticks(stdCode, tickCnt);
		if (tData)
		{
			uint32_t thisCnt = min(tickCnt, (WtUInt32)tData->size());
			cb(cHandle, stdCode, (WTSTickStruct*)tData->at(0), thisCnt, true);
			tData->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

WtUInt32 hft_get_ordque(CtxHandler cHandle, const char* stdCode, WtUInt32 itemCnt, FuncGetOrdQueCallback cb)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;
	try
	{
		WTSOrdQueSlice* dataSlice = mocker->stra_get_order_queue(stdCode, itemCnt);
		if (dataSlice)
		{
			uint32_t thisCnt = min(itemCnt, (WtUInt32)dataSlice->size());
			cb(cHandle, stdCode, (WTSOrdQueStruct*)dataSlice->at(0), thisCnt, true);
			dataSlice->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

WtUInt32 hft_get_orddtl(CtxHandler cHandle, const char* stdCode, WtUInt32 itemCnt, FuncGetOrdDtlCallback cb)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;
	try
	{
		WTSOrdDtlSlice* dataSlice = mocker->stra_get_order_detail(stdCode, itemCnt);
		if (dataSlice)
		{
			uint32_t thisCnt = min(itemCnt, (WtUInt32)dataSlice->size());
			cb(cHandle, stdCode, (WTSOrdDtlStruct*)dataSlice->at(0), thisCnt, true);
			dataSlice->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

WtUInt32 hft_get_trans(CtxHandler cHandle, const char* stdCode, WtUInt32 itemCnt, FuncGetTransCallback cb)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return 0;
	try
	{
		WTSTransSlice* dataSlice = mocker->stra_get_transaction(stdCode, itemCnt);
		if (dataSlice)
		{
			uint32_t thisCnt = min(itemCnt, (WtUInt32)dataSlice->size());
			cb(cHandle, stdCode, (WTSTransStruct*)dataSlice->at(0), thisCnt, true);
			dataSlice->release();
			return thisCnt;
		}
		else
		{
			return 0;
		}
	}
	catch (...)
	{
		return 0;
	}
}

void hft_log_text(CtxHandler cHandle, const char* message)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_log_info(message);
}

void hft_sub_ticks(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_sub_ticks(stdCode);
}

void hft_sub_order_detail(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_sub_order_details(stdCode);
}

void hft_sub_order_queue(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_sub_order_queues(stdCode);
}

void hft_sub_transaction(CtxHandler cHandle, const char* stdCode)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_sub_transactions(stdCode);
}

bool hft_cancel(CtxHandler cHandle, WtUInt32 localid)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return false;

	return mocker->stra_cancel(localid);
}

WtString hft_cancel_all(CtxHandler cHandle, const char* stdCode, bool isBuy)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = mocker->stra_cancel(stdCode, isBuy, DBL_MAX);
	for (WtUInt32 localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

WtString hft_buy(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = mocker->stra_buy(stdCode, price, qty, userTag, flag);
	for (WtUInt32 localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

WtString hft_sell(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = mocker->stra_sell(stdCode, price, qty, userTag, flag);
	for (WtUInt32 localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

void hft_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->stra_save_user_data(key, val);
}

WtString hft_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return defVal;

	return mocker->stra_load_user_data(key, defVal);
}

void hft_step(CtxHandler cHandle)
{
	//只有异步模式才有意义
	if (!getRunner().isAsync())
		return;

	HftMocker* mocker = getRunner().hft_mocker();
	if (mocker == NULL)
		return;

	mocker->step_tick();
}
#pragma endregion "HFT策略接口"