#include "ShareManager.h"
#include "WtUftEngine.h"
#include "../WTSTools/WTSLogger.h"

#include "../Share/StdUtils.hpp"

#include "../Share/TimeUtils.hpp"


bool ShareManager::initialize(const char* module)
{
	if (_inited)
		return true;

	if(!StdFile::exists(module))
	{
		WTSLogger::warn("WtShareHelper {} not exist", module);
		return false;
	}

	_module = module;
	_inst = DLLHelper::load_library(_module.c_str());
	_inited = (_inst != NULL);

	_init_master = (func_init_master)DLLHelper::get_symbol(_inst, "init_master");
	_get_section_updatetime = (func_get_section_updatetime)DLLHelper::get_symbol(_inst, "get_section_updatetime");
	_commit_section = (func_commit_section)DLLHelper::get_symbol(_inst, "commit_section");

	_set_double = (func_set_double)DLLHelper::get_symbol(_inst, "set_double");
	_set_int32 = (func_set_int32)DLLHelper::get_symbol(_inst, "set_int32");
	_set_int64 = (func_set_int64)DLLHelper::get_symbol(_inst, "set_int64");
	_set_uint32 = (func_set_uint32)DLLHelper::get_symbol(_inst, "set_uint32");
	_set_uint64 = (func_set_uint64)DLLHelper::get_symbol(_inst, "set_uint64");
	_set_string = (func_set_string)DLLHelper::get_symbol(_inst, "set_string");

	_get_double = (func_get_double)DLLHelper::get_symbol(_inst, "get_double");
	_get_int32 = (func_get_int32)DLLHelper::get_symbol(_inst, "get_int32");
	_get_int64 = (func_get_int64)DLLHelper::get_symbol(_inst, "get_int64");
	_get_uint32 = (func_get_uint32)DLLHelper::get_symbol(_inst, "get_uint32");
	_get_uint64 = (func_get_uint64)DLLHelper::get_symbol(_inst, "get_uint64");
	_get_string = (func_get_string)DLLHelper::get_symbol(_inst, "get_string");

	_allocate_double = (func_allocate_double)DLLHelper::get_symbol(_inst, "allocate_double");
	_allocate_int32 = (func_allocate_int32)DLLHelper::get_symbol(_inst, "allocate_int32");
	_allocate_int64 = (func_allocate_int64)DLLHelper::get_symbol(_inst, "allocate_int64");
	_allocate_uint32 = (func_allocate_uint32)DLLHelper::get_symbol(_inst, "allocate_uint32");
	_allocate_uint64 = (func_allocate_uint64)DLLHelper::get_symbol(_inst, "allocate_uint64");
	_allocate_string = (func_allocate_string)DLLHelper::get_symbol(_inst, "allocate_string");

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

					uint64_t lastUdtTime = _get_section_updatetime(_exchg.c_str(), section);
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

		WTSLogger::info("Share domain is on watch");
	}

	return true;
}

bool ShareManager::init_domain(const char* id)
{
	if (!_inited)
		return false;

	bool ret = _init_master(id, ".share");
	_exchg = id;
	WTSLogger::info("Share domain [{}] initialing {}", id, ret ? "succeed" : "failed");

	//初始化同步区
	ret = _init_master("sync", ".sync");
	WTSLogger::info("Sync domain [sync] initialing {}", ret ? "succeed" : "failed");

	return ret;
}

bool ShareManager::commit_param_watcher(const char* section)
{
	if (!_inited)
		return false;

	bool ret = _commit_section(_exchg.c_str(), section);
	_secnames[section] = TimeUtils::getLocalTimeNow();
	return ret;
}

bool ShareManager::set_value(const char* section, const char* key, double val)
{
	if (!_inited)
		return false;

	return _set_double(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint64_t val)
{
	if (!_inited)
		return false;

	return _set_uint64(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, uint32_t val)
{
	if (!_inited)
		return false;

	return _set_uint32(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int64_t val)
{
	if (!_inited)
		return false;

	return _set_int64(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, int32_t val)
{
	if (!_inited)
		return false;

	return _set_int32(_exchg.c_str(), section, key, val);
}

bool ShareManager::set_value(const char* section, const char* key, const char* val)
{
	if (!_inited)
		return false;

	return _set_string(_exchg.c_str(), section, key, val);
}

const char* ShareManager::get_value(const char* section, const char* key, const char* defVal /* = "" */)
{
	if (!_inited)
		return defVal;

	return _get_string(_exchg.c_str(), section, key, defVal);
}

int32_t ShareManager::get_value(const char* section, const char* key, int32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return _get_int32(_exchg.c_str(), section, key, defVal);
}

int64_t ShareManager::get_value(const char* section, const char* key, int64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return _get_int64(_exchg.c_str(), section, key, defVal);
}

uint32_t ShareManager::get_value(const char* section, const char* key, uint32_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return _get_uint32(_exchg.c_str(), section, key, defVal);
}

uint64_t ShareManager::get_value(const char* section, const char* key, uint64_t defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return _get_uint64(_exchg.c_str(), section, key, defVal);
}

double ShareManager::get_value(const char* section, const char* key, double defVal /* = 0 */)
{
	if (!_inited)
		return defVal;

	return _get_double(_exchg.c_str(), section, key, defVal);
}

const char* ShareManager::allocate_value(const char* section, const char* key, const char* initVal/* = ""*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_string(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}

int32_t* ShareManager::allocate_value(const char* section, const char* key, int32_t initVal/* = 0*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_int32(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}

int64_t* ShareManager::allocate_value(const char* section, const char* key, int64_t initVal/* = 0*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_int64(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}

uint32_t* ShareManager::allocate_value(const char* section, const char* key, uint32_t initVal/* = 0*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_uint32(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}

uint64_t* ShareManager::allocate_value(const char* section, const char* key, uint64_t initVal/* = 0*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_uint64(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}

double* ShareManager::allocate_value(const char* section, const char* key, double initVal/* = 0*/, bool bForceWrite/* = false*/, bool isExchg/* = false*/)
{
	if (!_inited)
		return nullptr;

	return _allocate_double(isExchg ? _exchg.c_str() : _sync.c_str(), section, key, initVal, bForceWrite);
}