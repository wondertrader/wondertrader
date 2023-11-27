// TestLoader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>

#include "../Share/DLLHelper.hpp"
#include "../Share/cppcli.hpp"
#include "../Share/fmtlib.h"
#include "../Share/StdUtils.hpp"

typedef int (*LoaderRunner)(const char*, bool, bool);

int main(int argc, char *argv[])
{
	cppcli::Option opt(argc, argv);

	auto mParam = opt("-m", "--module", "loader module filepath, CTPLoader.dll for win and libCTPLoader.so for linux as default", false);
	auto cParam = opt("-c", "--config", "configure filepath, config.ini as default", false);

	auto hParam = opt("-h", "--help", "gain help doc", false)->asHelpParam();

	opt.parse();

	if (hParam->exists())
		return 0;

	std::string module;
	DllHandle handle = NULL;
	if(!mParam->exists())
	{
		module = "./";
		module += DLLHelper::wrap_module("CTPLoader").c_str();
	}
	else
	{
		module = mParam->get<std::string>();
	}
	
	handle = DLLHelper::load_library(module.c_str());
	if (handle == NULL)
	{
		fmt::print("module {} not found\n", module);
	}
	else
	{
		LoaderRunner runner = (LoaderRunner)DLLHelper::get_symbol(handle, "run");
		if(runner == NULL)
		{
			fmt::print("module {} is invalid\n", module);
		}
		else
		{
			std::string cfgfile = cParam->exists() ? cParam->get<std::string>() : "config.ini";
			if(!StdFile::exists(cfgfile.c_str()))
			{
				fmt::print("configure {} not found\n", cfgfile);
				return 0;
			}
			return runner(cfgfile.c_str(), false, true);
		}
	}

	return 0;
}