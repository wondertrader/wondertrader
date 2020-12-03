// TestPorter.cpp : 定义控制台应用程序的入口点。
//
#include "../WtPorter/WtPorter.h"
#include "../WtExecMon/WtExecPorter.h"

#include "../Includes/WTSStruct.h"
#include "../Share/DLLHelper.hpp"

void _cdecl on_init(CtxHandler ctxid)
{
	printf("on_init\r\n");
	hft_sub_ticks(ctxid, "CFFEX.IF.HOT");
}

void _cdecl on_tick(CtxHandler ctxid, const char* stdCode, WTSTickStruct* newTick)
{
	printf("on_tick\r\n");
}

void _cdecl on_calc(CtxHandler ctxid, WtUInt32 uDate, WtUInt32 uTime)
{
	printf("on_calc\r\n");
}

void _cdecl on_bar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* newBar)
{
	printf("on_bar\r\n");
}

void _cdecl on_getbar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* bar, bool isLast)
{
	if (bar)
		printf("on_getbar%u\r\n", bar->time);
	else
		int x = 1;
}

void _cdecl on_getticks(CtxHandler cHandle, const char* code, WTSTickStruct* tick, bool isLast)
{
	printf("on_getticks\r\n");
}

void _cdecl on_event(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_event\r\n");
}

void _cdecl on_channel_evt(CtxHandler cHandle, const char* trader, WtUInt32 evtid)
{
	printf("on_channel_evt\r\n");
	double undone = hft_get_undone(cHandle, "CFFEX.IF.HOT");
}

void _cdecl on_order(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled)
{

}

void _cdecl on_trade(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price)
{

}

void _cdecl on_entrust(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message)
{

}

void test_porter()
{
#ifdef _WIN32
	DLLHelper::load_library("WtPorter.dll");
#else
	DLLHelper::load_library("libWtPorter.so");
#endif
	register_evt_callback(on_event);
	//register_cta_callbacks(on_init, on_tick, on_calc, on_bar);
	//register_sel_callbacks(on_init, on_tick, on_calc, on_bar);
	register_hft_callbacks(on_init, on_tick, on_bar, on_channel_evt, on_order, on_trade, on_entrust);

	init_porter("logcfg.json");

	//reg_cta_factories("./cta/");

	//reg_sel_factories("./sel/");

	config_porter("config.json");

	CtxHandler ctx = create_hft_context("test", "simnow");
	//ctx_str_get_bars(ctx, "DCE.pp.HOT", "m5", 220, true, on_getbar);
	//dump_bars("DCE.a.HOT", "m5", "CZCE.a.HOT_m5.csv");

	run_porter(true);

	printf("press enter key to exit\n");
	getchar();

	release_porter();
}

void test_exec()
{
#ifdef _WIN32
	DLLHelper::load_library("WtExecMon.dll");
#else
	DLLHelper::load_library("libWtExecMon.so");
#endif

	init_exec("logcfgexec.json");

	config_exec("config_exec.json");

	run_exec();

	set_position("CFFEX.IF.2007", 1);

	printf("press enter key to exit\n");
	getchar();

	release_exec();
}

void main()
{
	test_porter();
}
