// TestPorter.cpp : 定义控制台应用程序的入口点。
//
#include "../WtBtPorter/WtBtPorter.h"

#include "../Includes/WTSStruct.h"
#include "../Share/DLLHelper.hpp"

void on_getbar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* bar, bool isLast)
{
	if (bar)
		printf("on_getbar@%u\r\n", bar->time);
	else
		int x = 1;
}

void on_init(CtxHandler ctxid)
{
	//mf_get_bars(ctxid, "SSE.600009Q", "m3", 10, on_getbar);
}

void on_tick(CtxHandler ctxid, const char* stdCode, WTSTickStruct* newTick)
{
	//printf("on_tick\r\n");
}

void on_calc(CtxHandler ctxid, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_calc\r\n");
	sel_get_bars(ctxid, "SSE.600009Q", "d1", 10, on_getbar);
}

void on_bar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* newBar)
{
	//printf("on_bar\r\n");
}



int main()
{
#ifdef _WIN32
	DLLHelper::load_library("WtBtPorter.dll");
#else
	DLLHelper::load_library("libWtBtPorter.so");
#endif
	register_sel_callbacks(on_init, on_tick, on_calc, on_bar);

	init_backtest("logcfg.json", true);

	config_backtest("configbt.json");

	int ctxid = init_sel_mocker("sel", 0, 5, "min");	//every money 09:26

	sel_get_bars(ctxid, "SSE.600000Q", "m5", 50000, on_getbar);

	run_backtest();
	getchar();
	release_backtest();
	return 0;
}

