// TestLoader.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <string>

#include "../Share/DLLHelper.hpp"

typedef int (*LoaderRunner)(const char*, bool);

int main()
{
	std::string module = "./";
	module += DLLHelper::wrap_module("CTPLoader").c_str();
	DllHandle handle = DLLHelper::load_library(module.c_str());
	if (handle == NULL)
	{
		printf("module not found\r\n");
	}
	else
	{
		LoaderRunner runner = (LoaderRunner)DLLHelper::get_symbol(handle, "run");
		if(runner == NULL)
		{
			printf("entrance not found\r\n");
		}
		else
		{
			return runner("config.ini", false);
		}
	}

	return 0;
}