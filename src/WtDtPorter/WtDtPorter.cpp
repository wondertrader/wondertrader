/*!
 * \file WtDtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtPorter.h"
#include "WtDtRunner.h"

#include "version.h"

#include "../WTSTools/WTSLogger.h"

#ifdef _WIN32
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "WIN32";
#endif
#endif

WtDtRunner& getRunner()
{
	static WtDtRunner runner;
	return runner;
}

void initialize(WtString cfgFile, WtString logCfg)
{
	getRunner().initialize(cfgFile, logCfg);
}

void start()
{
	getRunner().start();
}

const char* get_version()
{
	static std::string _ver;
	if (_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += GetVersionStr();
	}
	return _ver.c_str();
}

void write_log(unsigned int level, const char* message, const char* catName)
{
	if (strlen(catName) > 0)
	{
		WTSLogger::log2(catName, (WTSLogLevel)level, message);
	}
	else
	{
		WTSLogger::log((WTSLogLevel)level, message);
	}
}