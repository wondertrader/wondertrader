/*!
 * \file DLLHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 动态库辅助类,主要是把跨平台的差异封装起来,方便调用
 */
#pragma once
#include <string>

#ifdef _MSC_VER
#include <wtypes.h>
typedef HMODULE		DllHandle;
typedef void*		ProcHandle;
#else
#include <dlfcn.h>
typedef void*		DllHandle;
typedef void*		ProcHandle;
#endif

class DLLHelper
{
public:
	static DllHandle load_library(const char *filename)
	{
		try
		{
#ifdef _MSC_VER
			return ::LoadLibrary(filename);
#else
			DllHandle ret = dlopen(filename, RTLD_NOW);
			if (ret == NULL)
				printf("%s\n", dlerror());
			return ret;
#endif
		}
		catch(...)
		{
			return NULL;
		}
	}

	static void free_library(DllHandle handle)
	{
		if (NULL == handle)
			return;

#ifdef _MSC_VER
		::FreeLibrary(handle);
#else
		dlclose(handle);
#endif
	}

	static ProcHandle get_symbol(DllHandle handle, const char* name)
	{
		if (NULL == handle)
			return NULL;

#ifdef _MSC_VER
		return ::GetProcAddress(handle, name);
#else
		return dlsym(handle, name);
#endif
	}

	static std::string wrap_module(const char* name, const char* unixPrefix = "lib")
	{

#ifdef _WIN32
		std::string ret = name;
		ret += ".dll";
		return std::move(ret);
#else
		std::string ret(unixPrefix);
		ret += name;
		ret += ".so";
		return std::move(ret);
#endif
	}
};