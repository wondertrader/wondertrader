#include "ShareManager.h"
#include "../WTSTools/WTSLogger.h"
#include "../WtShareHelper/WtShareHelper.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"

bool ShareManager::_inited = false;
std::string ShareManager::_domain;

bool ShareManager::initialize()
{
	if (_inited)
		return true;

	std::string path = DLLHelper::wrap_module("./WtShareHelper");
	if(!StdFile::exists(path.c_str()))
	{
		WTSLogger::warn("WtShareHelper not exist");
		return false;
	}

	auto hInst = DLLHelper::load_library(path.c_str());
	_inited = (hInst != NULL);
	return _inited;
}

bool ShareManager::init_domain(const char* id)
{
	if (!_inited)
		return false;

	bool ret = init_master(id);
	_domain = id;
	WTSLogger::info("Share domain {} initialing {}", id, ret ? "succeed" : "fail");

	return ret;
}

bool ShareManager::set_value(const char* section, const char* key, double val)
{
	if (!_inited)
		return false;

	return set_double(_domain.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint64_t val)
{
	if (!_inited)
		return false;

	return set_uint64(_domain.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint32_t val)
{
	if (!_inited)
		return false;

	return set_uint32(_domain.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int64_t val)
{
	if (!_inited)
		return false;

	return set_int64(_domain.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int32_t val)
{
	if (!_inited)
		return false;

	return set_int32(_domain.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, const char* val)
{
	if (!_inited)
		return false;

	return set_string(_domain.c_str(), section, key, val);
}

const char* ShareManager::get_value(const char* section, const char* key, const char* defVal /* = "" */)
{
	if (!_inited)
		return defVal;

	return get_string(_domain.c_str(), section, key, defVal);
}

int32_t ShareManager::get_value(const char* section, const char* key, int32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_int32(_domain.c_str(), section, key, defVal);
}

int64_t ShareManager::get_value(const char* section, const char* key, int64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_int64(_domain.c_str(), section, key, defVal);
}

uint32_t ShareManager::get_value(const char* section, const char* key, uint32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_uint32(_domain.c_str(), section, key, defVal);
}

uint64_t ShareManager::get_value(const char* section, const char* key, uint64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_uint64(_domain.c_str(), section, key, defVal);
}

double ShareManager::get_value(const char* section, const char* key, double defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_double(_domain.c_str(), section, key, defVal);
}