// TestPorter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "./BTApi/WtBtPorter.h"

#include "../Includes/WTSStruct.h"

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>

#ifdef _DEBUG
#pragma comment(lib, "./BTAPI/WtBtPorter_x86_mdd.lib")
#else
#pragma comment(lib, "./BTAPI/WtBtPorter_x86_md.lib")
#endif

void main()
{
#ifdef _DEBUG
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x86_mdd.dll");
#else
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x86_md.dll");
#endif
	printf("WtBtPorter.dll loaded, the version is %s\r\n", get_version());

	//register_callbacks(on_init, on_tick, on_calc, on_bar);

	init_backtest("logcfg.json");

	config_backtest("configbt.json");

	run_backtest();

	printf("press enter key to exit\r\n");
	getchar();

	release_backtest();
}

