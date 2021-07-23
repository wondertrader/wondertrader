/*!
 * \file WtDtPorter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDtServo.h"
#include "WtDtRunner.h"

#include "../WtDtCore/WtHelper.h"
#include "../WTSTools/WTSLogger.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSVersion.h"
#include "../Includes/WTSDataDef.hpp"

#include <boost/filesystem.hpp>

#ifdef _WIN32
#ifdef _WIN64
char PLATFORM_NAME[] = "X64";
#else
char PLATFORM_NAME[] = "WIN32";
#endif
#else
char PLATFORM_NAME[] = "UNIX";
#endif

std::string g_bin_dir;

void inst_hlp() {}

#ifdef _WIN32
#include <wtypes.h>
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

const std::string& getInstPath()
{
	static std::string moduleName;
	if (moduleName.empty())
	{
		Dl_info dl_info;
		dladdr((void *)inst_hlp, &dl_info);
		moduleName = dl_info.dli_fname;
		//printf("1:%s\n", moduleName.c_str());
	}

	return moduleName;
}
#endif

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
		boost::filesystem::path p(getInstPath());
		strcpy(MODULE_NAME, p.filename().string().c_str());
#endif
	}

	return MODULE_NAME;
}

const char* getBinDir()
{
	if (g_bin_dir.empty())
	{
#ifdef _WIN32
		char strPath[MAX_PATH];
		GetModuleFileName(g_dllModule, strPath, MAX_PATH);

		g_bin_dir = StrUtil::standardisePath(strPath, false);
#else
		g_bin_dir = getInstPath();
#endif
		boost::filesystem::path p(g_bin_dir);
		g_bin_dir = p.branch_path().string() + "/";
	}

	return g_bin_dir.c_str();
}

WtDtRunner& getRunner()
{
	static WtDtRunner runner;
	return runner;
}

void initialize(WtString cfgFile, bool isFile)
{
	getRunner().initialize(cfgFile, isFile, getBinDir());
}

const char* get_version()
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

WtUInt32 get_bars_by_range(const char* stdCode, const char* period, WtUInt64 beginTime, WtUInt64 endTime, FuncGetBarsCallback cb)
{
	WTSKlineSlice* kData = getRunner().get_bars_by_range(stdCode, period, beginTime, endTime);
	if (kData)
	{
		uint32_t reaCnt = 0;
		uint32_t kcnt = kData->size();
		for (uint32_t idx = 0; idx < kcnt; idx++)
		{
			WTSBarStruct* curBar = kData->at(idx);

			bool isLast = (idx == kcnt - 1);
			cb(curBar, isLast);
			reaCnt += 1;
		}

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

WtUInt32	get_ticks_by_range(const char* stdCode, WtUInt64 beginTime, WtUInt64 endTime, FuncGetTicksCallback cb)
{
	WTSArray* aySlices = getRunner().get_ticks_by_range(stdCode, beginTime, endTime);
	if (aySlices)
	{
		uint32_t reaCnt = 0;
		uint32_t sliceCnt = aySlices->size();
		for(uint32_t sIdx = 0; sIdx < sliceCnt; sIdx++)
		{
			WTSTickSlice* slice = (WTSTickSlice*)aySlices->at(sIdx);
			uint32_t tcnt = slice->size();
			for (uint32_t idx = 0; idx < tcnt; idx++)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)slice->at(idx);
				bool isLast = (idx == tcnt - 1) && (sIdx == sliceCnt - 1);
				cb(curTick, isLast);
				reaCnt += 1;
			}
		}
		

		aySlices->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

WtUInt32 get_bars_by_count(const char* stdCode, const char* period, WtUInt32 count, WtUInt64 endTime, FuncGetBarsCallback cb)
{
	WTSKlineSlice* kData = getRunner().get_bars_by_count(stdCode, period, count, endTime);
	if (kData)
	{
		uint32_t reaCnt = 0;
		uint32_t kcnt = kData->size();
		for (uint32_t idx = 0; idx < kcnt; idx++)
		{
			WTSBarStruct* curBar = kData->at(idx);

			bool isLast = (idx == kcnt - 1);
			cb(curBar, isLast);
			reaCnt += 1;
		}

		kData->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}

WtUInt32	get_ticks_by_count(const char* stdCode, WtUInt32 count, WtUInt64 endTime, FuncGetTicksCallback cb)
{
	WTSArray* aySlices = getRunner().get_ticks_by_count(stdCode, count, endTime);
	if (aySlices)
	{
		uint32_t reaCnt = 0;
		uint32_t sliceCnt = aySlices->size();
		for (uint32_t sIdx = 0; sIdx < sliceCnt; sIdx++)
		{
			WTSTickSlice* slice = (WTSTickSlice*)aySlices->at(sIdx);
			uint32_t tcnt = slice->size();
			for (uint32_t idx = 0; idx < tcnt; idx++)
			{
				WTSTickStruct* curTick = (WTSTickStruct*)slice->at(idx);
				bool isLast = (idx == tcnt - 1) && (sIdx == sliceCnt - 1);
				cb(curTick, isLast);
				reaCnt += 1;
			}
		}


		aySlices->release();
		return reaCnt;
	}
	else
	{
		return 0;
	}
}