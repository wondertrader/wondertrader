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

#include "version.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/DecimalHelper.h"
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
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void *)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif

WtRtRunner& getRunner()
{
	static WtRtRunner runner;
	return runner;
}

const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{
#ifdef _WIN32
		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
#else
		boost::filesystem::path p(g_moduleName);
		strcpy(MODULE_NAME, p.filename().string().c_str());
#endif
	}

	return MODULE_NAME;
}

void register_callbacks(FuncStraInitCallback cbInit, FuncStraTickCallback cbTick, FuncStraCalcCallback cbCalc, FuncStraBarCallback cbBar, FuncEventCallback cbEvt)
{
	getRunner().registerCallbacks(cbInit, cbTick, cbCalc, cbBar, cbEvt);
}

void init_porter(const char* logProfile)
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

void config_porter(const char* cfgfile)
{
	if (strlen(cfgfile) == 0)
		getRunner().config("config.json");
	else
		getRunner().config(cfgfile);
}

void run_porter(bool bAsync)
{
	getRunner().run(bAsync);
}

void release_porter(bool bAsync)
{
	getRunner().run(bAsync);
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

bool reg_cta_factories(const char* factFolder)
{
	return getRunner().addCtaFactories(factFolder);
}

bool reg_hft_factories(const char* factFolder)
{
	return getRunner().addHftFactories(factFolder);
}

bool reg_exe_factories(const char* factFolder)
{
	return getRunner().addExeFactories(factFolder);
}

CtxHandler create_context(const char* name)
{
	return getRunner().createContext(name);
}

void ctx_str_enter_long(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_enter_long(code, qty, userTag, limitprice, stopprice);
}

void ctx_str_exit_long(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_exit_long(code, qty, userTag, limitprice, stopprice);
}

void ctx_str_enter_short(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_enter_short(code, qty, userTag, limitprice, stopprice);
}

void ctx_str_exit_short(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_exit_short(code, qty, userTag, limitprice, stopprice);
}

WtUInt32 ctx_str_get_bars(CtxHandler cHandle, const char* code, const char* period, unsigned int barCnt, bool isMain, FuncGetBarsCallback cb)
{
	//printf("%s(%s,%s,%u)\r\n", __FUNCTION__, code, period, barCnt);
	CtaContextPtr ctx = getRunner().getContext(cHandle);
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

WtUInt32	ctx_str_get_ticks(CtxHandler cHandle, const char* code, unsigned int tickCnt, bool isMain, FuncGetTicksCallback cb)
{
	//printf("%s(%s,%s,%u)\r\n", __FUNCTION__, code, period, barCnt);
	CtaContextPtr ctx = getRunner().getContext(cHandle);
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

double ctx_str_get_position_profit(CtxHandler cHandle, const char* code)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_profit(code);
}

WtUInt64 ctx_str_get_detail_entertime(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_entertime(code, openTag);
}

double ctx_str_get_detail_cost(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_cost(code, openTag);
}

double ctx_str_get_detail_profit(CtxHandler cHandle, const char* code, const char* openTag, int flag)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_detail_profit(code, openTag, flag);
}

double ctx_str_get_position_avgpx(CtxHandler cHandle, const char* code)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position_avgpx(code);
}

int ctx_str_get_position(CtxHandler cHandle, const char* code, const char* openTag)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_position(code, openTag);
}

void ctx_str_set_position(CtxHandler cHandle, const char* code, int qty, const char* userTag, double limitprice, double stopprice)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_set_position(code, qty, userTag, limitprice, stopprice);
}

WtUInt64 ctx_str_get_first_entertime(CtxHandler cHandle, const char* code)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_first_entertime(code);
}

WtUInt64 ctx_str_get_last_entertime(CtxHandler cHandle, const char* code)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_entertime(code);
}

double ctx_str_get_last_enterprice(CtxHandler cHandle, const char* code)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return 0;

	return ctx->stra_get_last_enterprice(code);
}

double ctx_str_get_price(const char* code)
{
	return getRunner().getEnv().get_cur_price(code);
}

WtUInt32 ctx_str_get_date()
{
	return getRunner().getEnv().get_date();
}

WtUInt32 ctx_str_get_time()
{
	return getRunner().getEnv().get_min_time();
}

void ctx_str_log_text(CtxHandler cHandle, const char* message)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_log_text(message);
}

void ctx_str_save_userdata(CtxHandler cHandle, const char* key, const char* val)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return;

	ctx->stra_save_user_data(key, val);
}

WtString ctx_str_load_userdata(CtxHandler cHandle, const char* key, const char* defVal)
{
	CtaContextPtr ctx = getRunner().getContext(cHandle);
	if (ctx == NULL)
		return defVal;

	return ctx->stra_load_user_data(key, defVal);
}