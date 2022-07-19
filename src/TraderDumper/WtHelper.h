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
	static const char* get_cwd();

	static const char* get_module_dir(){ return _bin_dir.c_str(); }

	static void set_module_dir(const char* mod_dir){ _bin_dir = mod_dir; }

private:
	static std::string	_bin_dir;
};

