#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include <boost/filesystem.hpp>

#ifdef _WIN32
#include <direct.h>
#else	//UNIX
#include <unistd.h>
#endif

uint32_t WtHelper::_cur_date = 0;
uint32_t WtHelper::_cur_time = 0;
uint32_t WtHelper::_cur_secs = 0;
uint32_t WtHelper::_cur_tdate = 0;


std::string WtHelper::getCWD()
{
	static std::string _cwd;
	if(_cwd.empty())
	{
		char   buffer[256];
#ifdef _WIN32
		_getcwd(buffer, 255);
#else	//UNIX
		getcwd(buffer, 255);
#endif
		_cwd = StrUtil::standardisePath(buffer);
	}	
	return _cwd;
}

std::string WtHelper::getTraderModule(const char* moduleName)
{
	std::stringstream ss;
	ss << getCWD() << "traders/" << moduleName;
	return ss.str();
}

std::string WtHelper::getParserModule(const char* moduleName)
{
	std::stringstream ss;
	ss << getCWD() << "parsers/" << moduleName;
	return ss.str();
}

const char* WtHelper::getStraDataDir()
{
	static const char* folder = "./generated/stradata/";
	if (!StdFile::exists(folder))
		boost::filesystem::create_directories(folder);
	return folder;
}

const char* WtHelper::getStraUsrDatDir()
{
	static const char* folder = "./generated/userdata/";
	if (!StdFile::exists(folder))
		boost::filesystem::create_directories(folder);
	return folder;
}

const char* WtHelper::getPortifolioDir()
{
	static const char* folder = "./generated/portfolio/";
	if (!StdFile::exists(folder))
		boost::filesystem::create_directories(folder);
	return folder;
}

const char* WtHelper::getOutputDir()
{
	static const char* folder = "./generated/outputs/";
	if (!StdFile::exists(folder))
		boost::filesystem::create_directories(folder);
	return folder;
}

const char* WtHelper::getBaseDir()
{
	static const char* folder = "./generated/";
	if (!StdFile::exists(folder))
		boost::filesystem::create_directories(folder);
	return folder;
}