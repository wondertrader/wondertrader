/*!
 * /file main.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */

#include "WtUftRunner.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/IniHelper.hpp"

#ifdef _MSC_VER
#include "../Common/mdump.h"
#endif

//#include <vld.h>

int main()
{
#ifdef _MSC_VER
	CMiniDumper::Enable("WtUftRunner.exe", true);
#endif

	WtUftRunner runner;
	runner.init();

	runner.config();

	runner.run(false);
	return 0;
}

