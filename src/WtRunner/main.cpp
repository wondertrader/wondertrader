/*!
 * /file main.cpp
 * /project	WonderTrader
 *
 * /author Wesley
 * /date 2020/03/30
 * 
 * /brief 
 */

#include "WtRunner.h"

#include "../WTSTools/WTSLogger.h"

#ifdef _MSC_VER
#include "../Common/mdump.h"
#endif

//#include <vld.h>

int main()
{
#ifdef _MSC_VER
	CMiniDumper::Enable("WtRunner.exe", true);
#endif

	WtRunner runner;
	runner.init();

	runner.config();

	runner.run(false);
	return 0;
}

