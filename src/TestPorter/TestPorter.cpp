// TestPorter.cpp : 定义控制台应用程序的入口点。
//
#include "../WtPorter/WtPorter.h"
//#include "../WtExecMon/WtExecPorter.h"

#include "../Includes/WTSStruct.h"
#include "../Share/DLLHelper.hpp"
#include "../Share/CodeHelper.hpp"

void PORTER_FLAG on_init(CtxHandler ctxid)
{
	printf("on_init\r\n");
	hft_sub_ticks(ctxid, "CFFEX.IF.HOT");
}

void PORTER_FLAG on_tick(CtxHandler ctxid, const char* stdCode, WTSTickStruct* newTick)
{
	printf("on_tick\r\n");
}

void PORTER_FLAG on_calc(CtxHandler ctxid, WtUInt32 uDate, WtUInt32 uTime)
{
	printf("on_calc\r\n");
}

void PORTER_FLAG on_bar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* newBar)
{
	printf("on_bar\r\n");
}

void PORTER_FLAG on_getbar(CtxHandler ctxid, const char* code, const char* period, WTSBarStruct* bar, bool isLast)
{
	if (bar)
		printf("on_getbar%u\r\n", bar->time);
	else
		int x = 1;
}

void PORTER_FLAG on_getticks(CtxHandler cHandle, const char* code, WTSTickStruct* tick, bool isLast)
{
	printf("on_getticks\r\n");
}

void PORTER_FLAG on_event(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime)
{
	printf("on_event\r\n");
}

void PORTER_FLAG on_channel_evt(CtxHandler cHandle, const char* trader, WtUInt32 evtid)
{
	printf("on_channel_evt\r\n");
	double undone = hft_get_undone(cHandle, "CFFEX.IF.HOT");
}

void PORTER_FLAG on_order(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag)
{

}

void PORTER_FLAG on_trade(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag)
{

}

void PORTER_FLAG on_entrust(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag)
{

}

void PORTER_FLAG on_order_queue(CtxHandler cHandle, const char* stdCode, WTSOrdQueStruct* ordQue)
{

}

void PORTER_FLAG on_order_detail(CtxHandler cHandle, const char* stdCode, WTSOrdDtlStruct* ordDtl)
{

}

void PORTER_FLAG on_transaction(CtxHandler cHandle, const char* stdCode, WTSTransStruct* trans)
{

}

void test_porter()
{
#ifdef _WIN32
	DLLHelper::load_library("WtPorter.dll");
#else
	DLLHelper::load_library("libWtPorter.so");
#endif
	init_porter("logcfg.json", true, "./generated");

	reg_hft_factories("./hft");

	config_porter("config.json", true);

	run_porter(true);

	printf("press enter key to exit\n");
	getchar();

	release_porter();
}

int main()
{
	test_porter();
	getchar();
	return 0;
}
