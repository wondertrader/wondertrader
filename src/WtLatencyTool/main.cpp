/*!
 * /file main.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */

#include "WtLatencyTool.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/IniHelper.hpp"

//#include <vld.h>

int main()
{
	WtLatencyTool runner;
	runner.init();

	runner.run();
	return 0;
}

