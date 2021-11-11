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

	static const char* getOutputDir();

	static const std::string& getInstDir() { return _inst_dir; }
	static void setInstDir(const char* inst_dir) { _inst_dir = inst_dir; }
	static void setOutputDir(const char* out_dir);

private:
	static std::string	_inst_dir;	//实例所在目录
	static std::string	_out_dir;
};

