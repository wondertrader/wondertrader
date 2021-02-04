/*!
 * \file main.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */

#include "WtRunner.h"

#include "../Common/mdump.h"

#include "..\WTSTools\WTSLogger.h"
#include "..\Share\IniHelper.hpp"

//#include <vld.h>

void main()
{
	CMiniDumper::Enable("WtRunner.exe", true);
	WtRunner runner;
	runner.init();

	runner.config("config.json");

	runner.run(false);
}

