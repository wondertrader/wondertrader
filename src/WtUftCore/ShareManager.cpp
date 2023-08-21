#include "ShareManager.h"
#include "WtUftEngine.h"
#include "../WTSTools/WTSLogger.h"
#include "../WtShareHelper/WtShareHelper.h"

#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/TimeUtils.hpp"


bool ShareManager::initialize(const char* module)
{
	if (_inited)
		return true;

	if(!StdFile::exists(module))
	{
		WTSLogger::warn("WtShareHelper not exist");
		return false;
	}

	auto hInst = DLLHelper::load_library(module);
	_inited = (hInst != NULL);

	return _inited;
}

bool ShareManager::start_watching(uint32_t microsecs)
{
	if (!_inited)
		return false;

	if (_inited && !_stopped && _worker == nullptr)
	{
		_worker.reset(new StdThread([this, microsecs]() {
			while (!_stopped)
			{
				for(auto& v : _secnames)
				{
					if(_stopped)
						break;

					const char* section = v.first.c_str();
					uint64_t& udtTime = (uint64_t&)v.second;

					uint64_t lastUdtTime = get_section_updatetime(_exchg.c_str(), section);
					if(lastUdtTime > v.second)
					{
						//触发通知
						_engine->notify_params_update(section);
						udtTime = lastUdtTime;
					}
				}

				//如果等待时间为0，则进入无限循环的检查中
				if(microsecs > 0 && !_stopped)
					std::this_thread::sleep_for(std::chrono::microseconds(microsecs));
			}
		}));
	}

	return true;
}

bool ShareManager::init_domain(const char* id)
{
	if (!_inited)
		return false;

	bool ret = init_master(id, ".share");
	_exchg = id;
	WTSLogger::info("Share domain [{}] initialing {}", id, ret ? "succeed" : "fail");

	//初始化同步区
	ret = init_master("sync", ".sync");

	return ret;
}

bool ShareManager::commit_param_watcher(const char* section)
{
	if (!_inited)
		return false;

	_secnames[section] = TimeUtils::getLocalTimeNow();

	return commit_section(_exchg.c_str(), section);
}

bool ShareManager::set_value(const char* section, const char* key, double val)
{
	if (!_inited)
		return false;

	return set_double(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint64_t val)
{
	if (!_inited)
		return false;

	return set_uint64(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint32_t val)
{
	if (!_inited)
		return false;

	return set_uint32(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int64_t val)
{
	if (!_inited)
		return false;

	return set_int64(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int32_t val)
{
	if (!_inited)
		return false;

	return set_int32(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, const char* val)
{
	if (!_inited)
		return false;

	return set_string(_exchg.c_str(), section, key, val);
}

const char* ShareManager::get_value(const char* section, const char* key, const char* defVal /* = "" */)
{
	if (!_inited)
		return defVal;

	return get_string(_exchg.c_str(), section, key, defVal);
}

int32_t ShareManager::get_value(const char* section, const char* key, int32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_int32(_exchg.c_str(), section, key, defVal);
}

int64_t ShareManager::get_value(const char* section, const char* key, int64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_int64(_exchg.c_str(), section, key, defVal);
}

uint32_t ShareManager::get_value(const char* section, const char* key, uint32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_uint32(_exchg.c_str(), section, key, defVal);
}

uint64_t ShareManager::get_value(const char* section, const char* key, uint64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_uint64(_exchg.c_str(), section, key, defVal);
}

double ShareManager::get_value(const char* section, const char* key, double defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return get_double(_exchg.c_str(), section, key, defVal);
}

const char* ShareManager::allocate_value(const char* section, const char* key, const char* initVal/* = ""*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_string(_sync.c_str(), section, key, initVal);
}

int32_t* ShareManager::allocate_value(const char* section, const char* key, int32_t initVal/* = 0*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_int32(_sync.c_str(), section, key, initVal);
}

int64_t* ShareManager::allocate_value(const char* section, const char* key, int64_t initVal/* = 0*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_int64(_sync.c_str(), section, key, initVal);
}

uint32_t* ShareManager::allocate_value(const char* section, const char* key, uint32_t initVal/* = 0*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_uint32(_sync.c_str(), section, key, initVal);
}

uint64_t* ShareManager::allocate_value(const char* section, const char* key, uint64_t initVal/* = 0*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_uint64(_sync.c_str(), section, key, initVal);
}

double* ShareManager::allocate_value(const char* section, const char* key, double initVal/* = 0*/)
{
	if (!_inited)
		return nullptr;

	return ::allocate_double(_sync.c_str(), section, key, initVal);
}