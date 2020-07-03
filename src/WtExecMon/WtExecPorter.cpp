#include "WtExecPorter.h"
#include "WtExecRunner.h"

#include "../Common/version.h"

#include "../WTSTools/WTSLogger.h"
#include "../Share/StrUtil.hpp"

#ifdef _WIN32
#include "../Common/mdump.h"
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "WIN32";
#endif

HMODULE	g_dllModule = NULL;

BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dllModule = (HMODULE)hModule;
		break;
	}
	return TRUE;
}

#else
#include <dlfcn.h>

char PLATFORM_NAME[] = "UNIX";

std::string	g_moduleName;

__attribute__((constructor))
void on_load(void) {
	Dl_info dl_info;
	dladdr((void*)on_load, &dl_info);
	g_moduleName = dl_info.dli_fname;
}
#endif

WtExecRunner& getRunner()
{
	static WtExecRunner runner;
	return runner;
}

const char* getModuleName()
{
	static char MODULE_NAME[250] = { 0 };
	if (strlen(MODULE_NAME) == 0)
	{
#ifdef _WIN32
		GetModuleFileName(g_dllModule, MODULE_NAME, 250);
		boost::filesystem::path p(MODULE_NAME);
		strcpy(MODULE_NAME, p.filename().string().c_str());
#else
		boost::filesystem::path p(g_moduleName);
		strcpy(MODULE_NAME, p.filename().string().c_str());
#endif
	}

	return MODULE_NAME;
}

const char* getBinDir()
{
	static std::string _bin_dir;
	if (_bin_dir.empty())
	{


#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		_bin_dir = g_moduleName;
#endif

		uint32_t nPos = _bin_dir.find_last_of('/');
		_bin_dir = _bin_dir.substr(0, nPos + 1);
	}

	return _bin_dir.c_str();
}

void init_exec(WtString logProfile)
{
	static bool inited = false;

	if (inited)
		return;
#ifdef _WIN32
	CMiniDumper::Enable(getModuleName(), true);
#endif

	getRunner().init(logProfile);

	inited = true;
}

void config_exec(WtString cfgfile)
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

void write_log(unsigned int level, WtString message, const char* catName)
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