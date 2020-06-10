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

#include "../Common/version.h"

#include "../WtBtCore/CtaMocker.h"
#include "../WtBtCore/SelMocker.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/decimal.h"
#include "../Share/WTSTradeDef.hpp"


#ifdef _WIN32
#include "../Common/mdump.h"
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "WIN32";
#endif

HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}

#else
char PLATFORM_NAME[] = "UNIX";

//__attribute__((constructor))
//void on_load(void) {
//	printf("module loaded\r\n");
//}
#endif



WtBtRunner& getRunner()
{
	static WtBtRunner runner;
	return runner;
}

#ifdef _WIN32
const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{
		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
	}

	return MODULE_NAME;
}
#endif

void register_cta_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	getRunner().registerCtaCallbacks(cbInit, cbTick, cbCalc, cbBar);
}

void register_sel_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar)
{
	getRunner().registerSelCallbacks(cbInit, cbTick, cbCalc, cbBar);
}

void init_backtest(const char* logProfile)
{
	static bool inited = false;

	if (inited)
		return;

#ifdef _WIN32
	CMiniDumper::Enable(getModuleName(), true);
#endif

	getRunner().init(logProfile);

	inited = true;
}

void config_backtest(const char* cfgfile)
{
	if (strlen(cfgfile) == 0)
		getRunner().config("config.json");
	else
		getRunner().config(cfgfile);
}

void run_backtest()
{
	getRunner().run();
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
		_ver += GetVersionStr();
	}
	return _ver.c_str();
}

void dump_bars(const char* code, const char* period, const char* filename)
{
	getRunner().dump_bars(code, period, filename);
}

void trans_mc_bars(const char* csvFolder, const char* binFolder, const char* period)
{
	WtBtRunner::trans_mc_bars(csvFolder, binFolder, period);
}

void write_log(unsigned int level, const char* message, const char* catName)
{
	if (strlen(catName) > 0)
	{
		WTSLogger::log2(catName, (WTSLogLevel)level, message);
	}
	else
	{
		WTSLogger::log((WTSLogLevel)level, message);
	}
}

CtxHandler init_cta_mocker(const char* name)
{
	return getRunner().initCtaMocker(name);
}

CtxHandler init_sel_mocker(const char* name, uint32_t date, uint32_t time, const char* period)
{
	return getRunner().initSelMocker(name, date, time, period);
}

#pragma region "CTA策略接口"
void cta_enter_long(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_enter_long(code, qty, userTag, limitprice, stopprice);
}

void cta_exit_long(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_exit_long(code, qty, userTag, limitprice, stopprice);
}

void cta_enter_short(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_enter_short(code, qty, userTag, limitprice, stopprice);
}

void cta_exit_short(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_exit_short(code, qty, userTag, limitprice, stopprice);
}

WtUInt32 cta_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt, bool isMain, FuncGetBarsCallback cb)
{
	//printf("%s(%s,%s,%u)\r\n", __FUNCTION__, code, period, barCnt);
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSKlineSlice* kData = ctx->stra_get_bars(code, period, barCnt, isMain);
		if (kData)
		{
			//printf("K线条数%u\r\n", kData->size());
			uint32_t left = barCnt + 1;
			uint32_t reaCnt = 0;
			for (int32_t idx = 0; idx < kData->size() && left > 0; idx++, left--)
			{
				WTSBarStruct* curBar = kData->at(idx);
				cb(cHandle, code, period, curBar, false);
				reaCnt += 1;
			}

			//printf("数据已读完\r\n");
			cb(cHandle, code, period, NULL, true);

			kData->release();
			return reaCnt;
		}
		else
		{
			//printf("K线条数0\r\n");
			cb(cHandle, code, period, NULL, true);
			return 0;
		}
	}
	catch(...)
	{
		printf("K线读取异常\r\n");
		cb(cHandle, code, period, NULL, true);
		return 0;
	}
}

WtUInt32	cta_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb)
{
	//printf("%s(%s,%s,%u)\r\n", __FUNCTION__, code, period, barCnt);
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSTickSlice* tData = ctx->stra_get_ticks(code, tickCnt);
		if (tData)
		{
			//printf("K线条数%u\r\n", kData->size());
			uint32_t left = tickCnt + 1;
			uint32_t reaCnt = 0;
			for (uint32_t idx = 0; idx < tData->size() && left > 0; idx++, left--)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)tData->at(idx);
				cb(cHandle, code, curTick, false);
				reaCnt += 1;
			}

			//printf("数据已读完\r\n");
			cb(cHandle, code, NULL, true);

			tData->release();
			return reaCnt;
		}
		else
		{
			//printf("K线条数0\r\n");
			cb(cHandle, code, NULL, true);
			return 0;
		}
	}
	catch (...)
	{
		printf("tick读取异常\r\n");
		cb(cHandle, code, NULL, true);
		return 0;
	}
}

double cta_get_position_profit(CtxHandler cHandle, const char* code)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_profit(code);
}

WtUInt64 cta_get_detail_entertime(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_entertime(code, openTag);
}

double cta_get_detail_cost(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_cost(code, openTag);
}

double cta_get_detail_profit(CtxHandler cHandle, const char* code, const char* openTag, int flag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_profit(code, openTag, flag);
}

double cta_get_position_avgpx(CtxHandler cHandle, const char* code)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_avgpx(code);
}

double cta_get_position(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(code, openTag);
}

void cta_set_position(CtxHandler cHandle, const char* code, double qty, const char* userTag, double limitprice, double stopprice)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return;

	ctx->stra_set_position(code, qty, userTag, limitprice, stopprice);
}

WtUInt64 cta_get_first_entertime(CtxHandler cHandle, const char* code)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_first_entertime(code);
}

WtUInt64 cta_get_last_entertime(CtxHandler cHandle, const char* code)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_entertime(code);
}

double cta_get_last_enterprice(CtxHandler cHandle, const char* code)
{
	CtaMocker* ctx = getRunner().cta_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_enterprice(code);
}

double cta_get_price(const char* code)
{
	return getRunner().replayer().get_cur_price(code);
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

	ctx->stra_log_text(message);
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
#pragma endregion

#pragma region "多因子策略接口"
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

	ctx->stra_log_text(message);
}

double sel_get_price(const char* code)
{
	return getRunner().replayer().get_cur_price(code);
}

WtUInt32 sel_get_date()
{
	return getRunner().replayer().get_date();
}

WtUInt32 sel_get_time()
{
	return getRunner().replayer().get_min_time();
}

double sel_get_position(CtxHandler cHandle, const char* code, const char* openTag)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(code, openTag);
}

WtUInt32 sel_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt, FuncGetBarsCallback cb)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSKlineSlice* kData = ctx->stra_get_bars(code, period, barCnt);
		if (kData)
		{
			//printf("K线条数%u\r\n", kData->size());
			uint32_t left = barCnt;
			uint32_t reaCnt = 0;
			for (int32_t idx = 0; idx < kData->size() && left > 0; idx++, left--)
			{
				WTSBarStruct* curBar = kData->at(idx);
				cb(cHandle, code, period, curBar, false);
				reaCnt += 1;
			}

			//printf("数据已读完\r\n");
			cb(cHandle, code, period, NULL, true);

			kData->release();
			return reaCnt;
		}
		else
		{
			//printf("K线条数0\r\n");
			cb(cHandle, code, period, NULL, true);
			return 0;
		}
	}
	catch (...)
	{
		printf("K线读取异常\r\n");
		cb(cHandle, code, period, NULL, true);
		return 0;
	}
}

void sel_set_position(CtxHandler cHandle, const char* code, double qty, const char* userTag)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return;

	//多因子引擎，限价和止价都无效
	ctx->stra_set_position(code, qty, userTag);
}

WtUInt32	sel_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb)
{
	SelMocker* ctx = getRunner().sel_mocker();
	if (ctx == NULL)
		return 0;
	try
	{
		WTSTickSlice* tData = ctx->stra_get_ticks(code, tickCnt);
		if (tData)
		{
			//printf("K线条数%u\r\n", kData->size());
			uint32_t left = tickCnt + 1;
			uint32_t reaCnt = 0;
			for (uint32_t idx = 0; idx < tData->size() && left > 0; idx++, left--)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)tData->at(idx);
				cb(cHandle, code, curTick, false);
				reaCnt += 1;
			}

			//printf("数据已读完\r\n");
			cb(cHandle, code, NULL, true);

			tData->release();
			return reaCnt;
		}
		else
		{
			//printf("K线条数0\r\n");
			cb(cHandle, code, NULL, true);
			return 0;
		}
	}
	catch (...)
	{
		printf("tick读取异常\r\n");
		cb(cHandle, code, NULL, true);
		return 0;
	}
}
#pragma endregion
