// WtRunner.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WtRunner.h"
#include "..\WTSTools\WTSLogger.h"
#include "..\Share\IniHelper.hpp"

//#include <vld.h>

void main()
{
	WtRunner runner;
	runner.init();

	runner.config("config.json");

	runner.run(false);
}

