#pragma once
#include <string>
#include <stdint.h>

class WtHelper
{
public:
	static std::string getCWD();

	static std::string getTraderModule(const char* moduleName);
	static std::string getParserModule(const char* moduleName);

	static const char* getOutputDir();
};

