/*!
 * \file WtHelper.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
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

