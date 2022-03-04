/*!
 * \file WtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtPorter.h"
#include "WtRtRunner.h"

#include "../WtCore/WtHelper.h"
#include "../WTSTools/WTSLogger.h"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSVersion.h"

#ifdef _WIN32
#   ifdef _WIN64
    char PLATFORM_NAME[] = "X64";
#   else
    char PLATFORM_NAME[] = "X86";
#endif
#else
    char PLATFORM_NAME[] = "UNIX";
#endif

WtRtRunner& getRunner()
{
	static WtRtRunner runner;
	return runner;
}


void register_evt_callback(FuncEventCallback cbEvt)
{
	getRunner().registerEvtCallback(cbEvt);
}

void register_cta_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt)
{
	getRunner().registerCtaCallbacks(cbInit, cbTick, cbCalc, cbBar, cbSessEvt);
}

void register_sel_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncSessionEvtCallback cbSessEvt)
{
	getRunner().registerSelCallbacks(cbInit, cbTick, cbCalc, cbBar, cbSessEvt);
}

void register_hft_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraBarCallback cbBar, 
	FuncHftChannelCallback cbChnl, FuncHftOrdCallback cbOrd, FuncHftTrdCallback cbTrd, FuncHftEntrustCallback cbEntrust,
	FuncStraOrdDtlCallback cbOrdDtl, FuncStraOrdQueCallback cbOrdQue, FuncStraTransCallback cbTrans, FuncSessionEvtCallback cbSessEvt, FuncHftPosCallback cbPosition)
{
	getRunner().registerHftCallbacks(cbInit, cbTick, cbBar, cbChnl, cbOrd, cbTrd, cbEntrust, cbOrdDtl, cbOrdQue, cbTrans, cbSessEvt, cbPosition);
}

void register_parser_callbacks(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub)
{
	getRunner().registerParserPorter(cbEvt, cbSub);
}

void register_exec_callbacks(FuncExecInitCallback cbInit, FuncExecCmdCallback cbExec)
{
	getRunner().registerExecuterPorter(cbInit, cbExec);
}

bool create_ext_parser(const char* id)
{
	return getRunner().createExtParser(id);
}

bool create_ext_executer(const char* id)
{
	return getRunner().createExtExecuter(id);
}

void register_ext_data_loader(FuncLoadFnlBars fnlBarLoader, FuncLoadRawBars rawBarLoader, FuncLoadAdjFactors fctLoader, FuncLoadRawTicks tickLoader)
{
	getRunner().registerExtDataLoader(fnlBarLoader, rawBarLoader, fctLoader, tickLoader);
}

void feed_raw_bars(WTSBarStruct* bars, WtUInt32 count)
{
	getRunner().feedRawBars(bars, count);
}

void feed_adj_factors(WtString stdCode, WtUInt32* dates, double* factors, WtUInt32 count)
{
	getRunner().feedAdjFactors(stdCode, (uint32_t*)dates, factors, count);
}

void feed_raw_ticks(WTSTickStruct* ticks, WtUInt32 count)
{
	WTSLogger::error("API not implemented");
}

void init_porter(const char* logProfile, bool isFile, const char* genDir)
{
	static bool inited = false;

	if (inited)
		return;

	getRunner().init(logProfile, isFile, genDir);

	inited = true;
}

void config_porter(const char* cfgfile, bool isFile)
{
	if (strlen(cfgfile) == 0)
		getRunner().config("config.json", true);
	else
		getRunner().config(cfgfile, isFile);
}

void run_porter(bool bAsync)
{
	getRunner().run(bAsync);
}

void release_porter()
{
	getRunner().release();
}

const char* get_version()
{
	static std::string _ver;
	if (_ver.empty())
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

bool reg_cta_factories(const char* factFolder)
{
	return getRunner().addCtaFactories(factFolder);
}

bool reg_sel_factories(const char* factFolder)
{
	return getRunner().addSelFactories(factFolder);
}

bool reg_hft_factories(const char* factFolder)
{
	return getRunner().addHftFactories(factFolder);
}

bool reg_exe_factories(const char* factFolder)
{
	return getRunner().addExeFactories(factFolder);
}


#pragma region "CTA策略接口"

CtxHandler create_cta_context(const char* name)
{
	return getRunner().createCtaContext(name);
}

void cta_enter_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_enter_long(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_exit_long(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_exit_long(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_enter_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_enter_short(stdCode, qty, userTag, limitprice, stopprice);
}

void cta_exit_short(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_exit_short(stdCode, qty, userTag, limitprice, stopprice);
}

WtUInt32 cta_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, bool isMain, FuncGetBarsCallback cb)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;
	try
	{
		WTSKlineSlice* kData = ctx->stra_get_bars(stdCode, period, barCnt, isMain);
		if (kData)
		{
			WtUInt32 reaCnt = (WtUInt32)kData->size();

			uint32_t blkCnt = kData->get_block_counts();
			for (uint32_t i = 0; i < blkCnt; i++)
			{
				if(kData->get_block_addr(i) != NULL)
					cb(cHandle, stdCode, period, kData->get_block_addr(i), kData->get_block_size(i), i == blkCnt - 1);
			}

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

WtUInt32	cta_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
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
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_profit(stdCode);
}

WtUInt64 cta_get_detail_entertime(CtxHandler cHandle, const char* stdCode, const char* openTag)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_entertime(stdCode, openTag);
}

double cta_get_detail_cost(CtxHandler cHandle, const char* stdCode, const char* openTag)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_cost(stdCode, openTag);
}

double cta_get_detail_profit(CtxHandler cHandle, const char* stdCode, const char* openTag, int flag)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_profit(stdCode, openTag, flag);
}

double cta_get_position_avgpx(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_avgpx(stdCode);
}

void cta_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
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
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(stdCode, bOnlyValid, openTag);
}

double cta_get_fund_data(CtxHandler cHandle, int flag)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_fund_data(flag);
}


void cta_set_position(CtxHandler cHandle, const char* stdCode, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_set_position(stdCode, qty, userTag, limitprice, stopprice);
}


WtUInt64 cta_get_first_entertime(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_first_entertime(stdCode);
}

WtUInt64 cta_get_last_entertime(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_entertime(stdCode);
}

WtUInt64 cta_get_last_exittime(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_exittime(stdCode);
}

double cta_get_last_enterprice(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_enterprice(stdCode);
}

double cta_get_price(const char* stdCode)
{
	return getRunner().getEngine()->get_cur_price(stdCode);
}

WtUInt32 cta_get_tdate()
{
	return getRunner().getEngine()->get_trading_date();
}

WtUInt32 cta_get_date()
{
	return getRunner().getEngine()->get_date();
}

WtUInt32 cta_get_time()
{
	return getRunner().getEngine()->get_min_time();
}

void cta_log_text(CtxHandler cHandle, const char* message)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_log_info(message);
}

void cta_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString cta_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}

void cta_sub_ticks(CtxHandler cHandle, const char* stdCode)
{
	CtaContextPtr ctx = getRunner().getCtaContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_ticks(stdCode);
}

#pragma endregion

#pragma region "多因子策略接口"
CtxHandler create_sel_context(const char* name, uint32_t date, uint32_t time, const char* period, const char* trdtpl/* = "CHINA"*/, const char* session/* = "TRADING"*/)
{
	return getRunner().createSelContext(name, date, time, period, trdtpl, session);
}


void sel_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString sel_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}

void sel_log_text(CtxHandler cHandle, const char* message)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_log_info(message);
}

double sel_get_price(const char* stdCode)
{
	return getRunner().getEngine()->get_cur_price(stdCode);
}

WtUInt32 sel_get_date()
{
	return getRunner().getEngine()->get_date();
}

WtUInt32 sel_get_time()
{
	return getRunner().getEngine()->get_min_time();
}

void sel_get_all_position(CtxHandler cHandle, FuncGetPositionCallback cb)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
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
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(stdCode, bOnlyValid, openTag);
}

WtUInt32 sel_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, FuncGetBarsCallback cb)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
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
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return;

	//多因子引擎,限价和止价都无效
	ctx->stra_set_position(stdCode, qty, userTag);
}

WtUInt32	sel_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
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
	SelContextPtr ctx = getRunner().getSelContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_ticks(stdCode);
}
#pragma endregion

#pragma region "HFT策略接口"
CtxHandler create_hft_context(const char* name, const char* trader, bool agent)
{
	return getRunner().createHftContext(name, trader, agent);
}

double hft_get_position(CtxHandler cHandle, const char* stdCode, bool bOnlyValid)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(stdCode, bOnlyValid);
}

double hft_get_position_profit(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_profit(stdCode);
}


double hft_get_undone(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_undone(stdCode);
}

double hft_get_price(const char* stdCode)
{
	return getRunner().getEngine()->get_cur_price(stdCode);
}

WtUInt32 hft_get_date()
{
	return getRunner().getEngine()->get_date();
}

WtUInt32 hft_get_time()
{
	return getRunner().getEngine()->get_raw_time();
}

WtUInt32 hft_get_secs()
{
	return getRunner().getEngine()->get_secs();
}

WtUInt32 hft_get_bars(CtxHandler cHandle, const char* stdCode, const char* period, WtUInt32 barCnt, FuncGetBarsCallback cb)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
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

WtUInt32 hft_get_ticks(CtxHandler cHandle, const char* stdCode, WtUInt32 tickCnt, FuncGetTicksCallback cb)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
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

WtUInt32 hft_get_ordque(CtxHandler cHandle, const char* stdCode, WtUInt32 itemCnt, FuncGetOrdQueCallback cb)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;
	try
	{
		WTSOrdQueSlice* dataSlice = ctx->stra_get_order_queue(stdCode, itemCnt);
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
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;
	try
	{
		WTSOrdDtlSlice* dataSlice = ctx->stra_get_order_detail(stdCode, itemCnt);
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
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return 0;
	try
	{
		WTSTransSlice* dataSlice = ctx->stra_get_transaction(stdCode, itemCnt);
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
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_log_info(message);
}

void hft_sub_ticks(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_ticks(stdCode);
}

void hft_sub_order_detail(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_order_details(stdCode);
}

void hft_sub_order_queue(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_order_queues(stdCode);
}

void hft_sub_transaction(CtxHandler cHandle, const char* stdCode)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_sub_transactions(stdCode);
}

bool hft_cancel(CtxHandler cHandle, WtUInt32 localid)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return false;

	return ctx->stra_cancel(localid);
}

WtString hft_cancel_all(CtxHandler cHandle, const char* stdCode, bool isBuy)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = ctx->stra_cancel(stdCode, isBuy, DBL_MAX);
	for(uint32_t localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	if (ret.size() > 0)
		ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

WtString hft_buy(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = ctx->stra_buy(stdCode, price, qty, userTag, flag);
	for (uint32_t localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	if(ret.size() > 0)
		ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

WtString hft_sell(CtxHandler cHandle, const char* stdCode, double price, double qty, const char* userTag, int flag)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return "";

	static std::string ret;

	std::stringstream ss;
	OrderIDs ids = ctx->stra_sell(stdCode, price, qty, userTag, flag);
	for (uint32_t localid : ids)
	{
		ss << localid << ",";
	}

	ret = ss.str();
	if (ret.size() > 0)
		ret = ret.substr(0, ret.size() - 1);
	return ret.c_str();
}

void hft_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString hft_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	HftContextPtr ctx = getRunner().getHftContext(cHandle);
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}
#pragma endregion "HFT策略接口"

#pragma region "扩展Parser接口"
void parser_push_quote(const char* id, WTSTickStruct* curTick, WtUInt32 uProcFlag)
{
	getRunner().on_ext_parser_quote(id, curTick, uProcFlag);
}
#pragma endregion "扩展Parser接口"