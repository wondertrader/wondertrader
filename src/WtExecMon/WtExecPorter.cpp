#include "WtExecPorter.h"
#include "WtExecRunner.h"

#include "../WtCore/WtHelper.h"
#include "../WTSTools/WTSLogger.h"
#include "../Includes/WTSVersion.h"

#ifdef _WIN32
#   ifdef _WIN64
    char PLATFORM_NAME[] = "X64";
#   else
    char PLATFORM_NAME[] = "X86";
#   endif
#else
    char PLATFORM_NAME[] = "linux";
#endif

WtExecRunner& getRunner()
{
	static WtExecRunner runner;
	return runner;
}

#ifdef _MSC_VER
#include "../Common/mdump.h"
const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{

		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
	}

	return MODULE_NAME;
}
#endif

void init_exec(WtString logCfg, bool isFile /*= true*/)
{
	static bool inited = false;

	if (inited)
		return;
#ifdef _MSC_VER
	CMiniDumper::Enable(getModuleName(), true, WtHelper::getCWD().c_str());
#endif

	getRunner().init(logCfg);

	inited = true;
}

void config_exec(WtString cfgfile, bool isFile /*= true*/)
{
	if (strlen(cfgfile) == 0)
		getRunner().config("cfgexec.json");
	else
		getRunner().config(cfgfile);
}

void run_exec()
{
	getRunner().run();
}

void release_exec()
{
	getRunner().release();
}

WtString get_version()
{
	static std::string _ver;
	if (_ver.empty())
	{
		_ver = PLATFORM_NAME;
		_ver += " ";
		_ver += WT_VERSION;
		_ver += " Build@";
		_ver += __DATE__;
		_ver += " ";
		_ver += __TIME__;
	}
	return _ver.c_str();
}

void write_log(unsigned int level, WtString message, WtString catName)
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

void set_position(WtString stdCode, double targetPos)
{
	getRunner().setPosition(stdCode, targetPos);
}