// TestPorter.cpp : 定义控制台应用程序的入口点。
//

#include "./BTApi/WtBtPorter.h"

#include "../Includes/WTSStruct.h"

#define WIN32_LEAN_AND_MEAN
#include <wtypes.h>

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "./BTAPI/WtBtPorter_x64_mdd.lib")
#else
#pragma comment(lib, "./BTAPI/WtBtPorter_x64_md.lib")
#endif
#else ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "./BTAPI/WtBtPorter_x86_mdd.lib")
#else
#pragma comment(lib, "./BTAPI/WtBtPorter_x86_md.lib")
#endif
#endif

void main()
{
#ifdef _WIN64
#ifdef _DEBUG
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x64_mdd.dll");
#else
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x64_md.dll");
#endif
#else ifdef _WIN32
#ifdef _DEBUG
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x86_mdd.dll");
#else
	HINSTANCE hInst = LoadLibrary("WtBtPorter_x86_md.dll");
#endif
#endif
	printf("WtBtPorter.dll loaded, the version is %s\r\n", get_version());

	//register_callbacks(on_init, on_tick, on_calc, on_bar);

	init_backtest("logcfg.json", true);

	config_backtest("configbt.json", true);

	run_backtest();

	printf("press enter key to exit\r\n");
	getchar();

	release_backtest();
}

