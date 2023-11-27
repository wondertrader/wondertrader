#pragma once
#include "../Share/StrUtil.hpp"

#ifdef _MSC_VER
#include <wtypes.h>
static HMODULE	g_dllModule = NULL;

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
void inst_hlp() {}
static const std::string& getInstPath()
{
	static std::string moduleName;
	if (moduleName.empty())
	{
		Dl_info dl_info;
		dladdr((void *)inst_hlp, &dl_info);
		moduleName = dl_info.dli_fname;
	}

	return moduleName;
}
#endif

static const char* getBinDir()
{
	static std::string g_bin_dir;

	if (g_bin_dir.empty())
	{
#ifdef _MSC_VER
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		g_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		g_bin_dir = getInstPath();
#endif
		std::size_t nPos = g_bin_dir.find_last_of('/');
		g_bin_dir = g_bin_dir.substr(0, nPos + 1);
	}

	return g_bin_dir.c_str();
}