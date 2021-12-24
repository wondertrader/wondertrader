// TestPorter.cpp : 定义控制台应用程序的入口点。
//
#include "../WtBtPorter/WtBtPorter.h"

#include "../Includes/WTSStruct.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/StdUtils.hpp"

void on_getbar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* bar, WtUInt32 count, bool isLast)
{
	if (bar)
		printf("on_getbar@%u\r\n", bar->time);
	else
		int x = 1;
}

void on_gettick(CtxHandler ctxid, const char* code, WTSTickStruct* tick, bool isLast)
{
	if (tick)
		printf("on_gettick@%u\r\n", tick->action_time);
}

void on_init(CtxHandler ctxid)
{
	cta_get_bars(ctxid, "CFFEX.IF.HOT", "m5", 30, true, on_getbar);
	//cta_get_ticks(ctxid, "CFFEX.IF.HOT", 100, on_gettick);
	//cta_log_text(ctxid, "this is a test message");
}

void on_tick(CtxHandler ctxid, const char* stdCode, WTSTickStruct* newTick)
{
	//printf("on_tick\r\n");
}

void on_calc(CtxHandler ctxid, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_calc @ %u.%u\r\n", curDate, curTime);
	//cta_get_ticks(ctxid, "CFFEX.IF.HOT", 100, on_gettick);
}

void on_calc_done(CtxHandler ctxid, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_calc_done @ %u.%u\r\n", curDate, curTime);
	//cta_get_ticks(ctxid, "CFFEX.IF.HOT", 100, on_gettick);
}


void on_bar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* newBar)
{
	//printf("on_bar\r\n");
}

void on_session_event(CtxHandler cHandle, WtUInt32 curTDate, bool isBegin)
{

}

bool on_load_his_bars(const char* stdCode, const char* period)
{
	WTSBarStruct bars[10];
	memset(&bars[0], 0, sizeof(WTSBarStruct) * 10);

	for(int i = 0; i < 10; i++)
	{
		bars[i].open = i + 1003;
	}

	feed_raw_bars(&bars[0], 10);

	return true;
}


void run_bt()
{
#ifdef _WIN32
	DLLHelper::load_library("WtBtPorter.dll");
#else
	DLLHelper::load_library("libWtBtPorter.so");
#endif
	register_cta_callbacks(on_init, on_tick, on_calc, on_bar, on_session_event, on_calc_done);

	//register_ext_data_loader(on_load_his_bars, NULL, false);

	auto id = init_cta_mocker("test", 0, true);

	init_backtest("logcfgbt.json", true, "./outputs_bt");

	config_backtest("configbt.json", true);

	run_backtest(true, true);

	for(int i = 0; i < 20; i++)
	{
		printf("%d\r\n", i);
		cta_step(id);

		if (i == 5)
			stop_backtest();
	}

	printf("press enter key to exit\n");
	getchar();
	release_backtest();
}


int main()
{
	run_bt();
	return 0;
}

