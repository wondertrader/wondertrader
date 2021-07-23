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
	cta_get_bars(ctxid, "CFFEX.IF.HOT", "m5", 30, true, on_getbar);
}

void on_tick(CtxHandler ctxid, const char* stdCode, WTSTickStruct* newTick)
{
	//printf("on_tick\r\n");
}

void on_calc(CtxHandler ctxid, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_calc @ %u.%u\r\n", curDate, curTime);
	//sel_get_bars(ctxid, "SSE.600009Q", "d1", 10, on_getbar);
}

void on_bar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* newBar)
{
	//printf("on_bar\r\n");
}

void on_session_event(CtxHandler cHandle, WtUInt32 curTDate, bool isBegin)
{

}


void run_bt()
{
#ifdef _WIN32
	DLLHelper::load_library("WtBtPorter.dll");
#else
	DLLHelper::load_library("libWtBtPorter.so");
#endif
	register_cta_callbacks(on_init, on_tick, on_calc, on_bar, on_session_event);

	init_cta_mocker("test");

	init_backtest("logcfgbt.json", true);

	config_backtest("configbt.json", true);

	run_backtest();

	init_cta_mocker("test2");

	set_time_range(202107010930, 202107161500);
	enable_tick(false);

	run_backtest();

	printf("press enter key to exit\n");
	getchar();
	release_backtest();
}


int main()
{
	run_bt();
	return 0;
}

