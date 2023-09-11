/*!
 * \file WtHelper.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtHelper.h"

#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"

#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#include <direct.h>
#else	//UNIX
#include <unistd.h>
#endif

uint32_t WtHelper::_cur_date = 0;
uint32_t WtHelper::_cur_time = 0;
uint32_t WtHelper::_cur_secs = 0;
uint32_t WtHelper::_cur_tdate = 0;
std::string WtHelper::_inst_dir;
std::string WtHelper::_gen_dir = "./generated/";


std::string WtHelper::getCWD()
{
	static std::string _cwd;
	if(_cwd.empty())
	{
		char   buffer[256];
#ifdef _MSC_VER
		_getcwd(buffer, 255);
#else	//UNIX
		getcwd(buffer, 255);
#endif
		_cwd = StrUtil::standardisePath(buffer);
	}	
	return _cwd;
}

std::string WtHelper::getModulePath(const char* moduleName, const char* subDir, bool isCWD /* = true */)
{
	std::stringstream ss;
	ss << (isCWD?getCWD():getInstDir()) << subDir << "/" << moduleName;
	return ss.str();
}

const char* WtHelper::getStraDataDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "stradata/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* WtHelper::getExecDataDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "execdata/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* WtHelper::getStraUsrDatDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "userdata/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* WtHelper::getPortifolioDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "portfolio/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* WtHelper::getOutputDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir) + "outputs/";
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}

const char* WtHelper::getBaseDir()
{
	static std::string folder = StrUtil::standardisePath(_gen_dir);
	if (!StdFile::exists(folder.c_str()))
		boost::filesystem::create_directories(folder);
	return folder.c_str();
}