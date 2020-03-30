#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/BoostFile.hpp"

#ifdef _WIN32
#include <direct.h>
#else	//UNIX
#include <stdio.h>
#endif


std::string WtHelper::getCWD()
{
	static std::string _cwd;
	if(_cwd.empty())
	{
		char   buffer[255];
#ifdef _WIN32
		_getcwd(buffer, 255);
#else	//UNIX
		getcwd(buffer, 255);
#endif
		_cwd = buffer;
		_cwd = StrUtil::standardisePath(_cwd);
	}	
	return _cwd;
}

std::string WtHelper::getTraderModule(const char* moduleName)
{
	std::stringstream ss;
	ss << getCWD() << "traders/" << moduleName;
	return StrUtil::standardisePath(ss.str());
}

std::string WtHelper::getParserModule(const char* moduleName)
{
	std::stringstream ss;
	ss << getCWD() << "parsers/" << moduleName;
	return StrUtil::standardisePath(ss.str());
}

const char* WtHelper::getOutputDir()
{
	const char* folder = "./outputs_bt/";
	if (!BoostFile::exists(folder))
		BoostFile::create_directories(folder);
	return folder;
}