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

#ifdef _MSC_VER
#include "../Common/mdump.h"
#endif

#include "../Share/cppcli.hpp"
//#include <vld.h>

int main(int argc, char* argv[])
{
#ifdef _MSC_VER
	CMiniDumper::Enable("WtUftRunner.exe", true);
#endif

	cppcli::Option opt(argc, argv);

	auto cParam = opt("-c", "--config", "configure filepath, dtcfg.yaml as default", false);
	auto lParam = opt("-l", "--logcfg", "logging configure filepath, logcfgbt.yaml as default", false);

	auto hParam = opt("-h", "--help", "gain help doc", false)->asHelpParam();

	opt.parse();

	if (hParam->exists())
		return 0;

	std::string filename;
	if (lParam->exists())
		filename = lParam->get<std::string>();
	else
		filename = "./logcfg.yaml";

	WtUftRunner runner;
	runner.init(filename);

	if (cParam->exists())
		filename = cParam->get<std::string>();
	else
		filename = "./config.yaml";
	runner.config(filename);

	runner.run(false);
	return 0;
}

