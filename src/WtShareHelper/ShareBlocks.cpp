#include "ShareBlocks.h"
#include "../Share/BoostFile.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"

using namespace shareblock;

bool ShareBlocks::init_master(const char* name, const char* path/* = ""*/)
{
	ShmPair& shm = (ShmPair&)_shm_blocks[name];
	if (shm._block != NULL)
		return true;

	std::string filename = path;
	if (filename.empty())
		filename = name;

	if(!StdFile::exists(filename.c_str()))
	{
		BoostFile bf;
		bf.create_new_file(filename.c_str());
		bf.truncate_file(sizeof(ShmBlock));
		bf.close_file();
	}

	shm._domain.reset(new BoostMappingFile);
	shm._domain->map(filename.c_str());
	shm._master = true;
	shm._block = (ShmBlock*)shm._domain->addr();

	/*
	 *	By Wesley @ 2023.09.20
	 *	这里要做一个清理，如果state为0，则不再保留
	 */
	std::vector<SecInfo> aySecs;
	for (uint32_t i = 0; i < shm._block->_count; i++)
	{
		SecInfo& secInfo = shm._block->_sections[i];
		if (secInfo._count == 0 || secInfo._state != 1)
			continue;

		aySecs.emplace_back(secInfo);
	}

	if(aySecs.size() != shm._block->_count)
	{
		shm._block->_count = (uint32_t)aySecs.size();
		memset(shm._block->_sections, 0, sizeof(SecInfo)*MAX_SEC_CNT);
		if (shm._block->_count > 0)
			memcpy(shm._block->_sections, aySecs.data(), sizeof(SecInfo)*shm._block->_count);

		shm._blocktime = shm._block->_updatetime;
	}

	{
		//这里要做初始化，要把已经有的key加载进去
		for (uint32_t i = 0; i < shm._block->_count; i++)
		{
			SecInfo& secInfo = shm._block->_sections[i];
			if (secInfo._count == 0)
				continue;

			//置零的目的看后面会不会用到
			//如果不会用到，那么就不会变成1
			//下次启动就会删掉这个section
			secInfo._state = 0;

			ShmPair::KVPair& kvPair = shm._sections[secInfo._name];
			kvPair._index = i;
			for (uint32_t j = 0; j < secInfo._count; j++)
			{
				KeyInfo& key = secInfo._keys[j];
				kvPair._keys[key._key] = &key;
			}
		}
	}

	return true;
}

bool ShareBlocks::init_slave(const char* name, const char* path/* = ""*/)
{
	ShmPair& shm = (ShmPair&)_shm_blocks[name];
	if (shm._block != NULL)
		return true;

	std::string filename = path;
	if (filename.empty())
		filename = name;

	if (!BoostFile::exists(filename.c_str()))
		return false;

	shm._domain.reset(new BoostMappingFile);
	shm._domain->map(filename.c_str());
	shm._master = false;
	shm._block = (ShmBlock*)shm._domain->addr();
	shm._blocktime = shm._block->_updatetime;

	//slave模式下，应该需要加载一下
	//if (strcmp(shm._block->_flag, BLK_FLAG) == 0)
	{
		//这里要做初始化，要把已经有的key加载进去
		for (uint32_t i = 0; i < shm._block->_count; i++)
		{
			SecInfo& secInfo = shm._block->_sections[i];
			if (secInfo._count == 0 || secInfo._state != 1)
				continue;

			ShmPair::KVPair& kvPair = shm._sections[secInfo._name];
			kvPair._index = i;
			for (uint32_t j = 0; j < secInfo._count; j++)
			{
				KeyInfo& key = secInfo._keys[j];
				kvPair._keys[key._key] = &key;
			}
		}
	}

	return true;
}

bool ShareBlocks::update_slave(const char* name, bool bForce)
{
	ShmPair& shm = (ShmPair&)_shm_blocks[name];
	if (shm._block == NULL)
		return false;

	if (shm._blocktime == shm._block->_updatetime && !bForce)
		return false;

	{
		shm._sections.clear();

		//这里要做初始化，要把已经有的key加载进去
		for (uint32_t i = 0; i < shm._block->_count; i++)
		{
			SecInfo& secInfo = shm._block->_sections[i];
			if (secInfo._count == 0)
				continue;

			ShmPair::KVPair& kvPair = shm._sections[secInfo._name];
			kvPair._index = i;
			for (uint32_t j = 0; j < secInfo._count; j++)
			{
				KeyInfo& key = secInfo._keys[j];
				kvPair._keys[key._key] = &key;
			}
		}
	}

	shm._blocktime = shm._block->_updatetime;

	return true;
}

bool ShareBlocks::release_slave(const char* name)
{
	auto it = _shm_blocks.find(name);
	if (it == _shm_blocks.end())
		return true;

	ShmPair& shm = it->second;

	//只有slave需要释放
	if (shm._master)
		return false;

	shm._block = NULL;
	shm._sections.clear();
	shm._domain.reset();
	shm._blocktime = 0;

	_shm_blocks.erase(it);
	return true;
}

uint64_t ShareBlocks::get_section_updatetime(const char* domain, const char* section)
{
	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return 0;

	const ShmPair& shm = (ShmPair&)it->second;
	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
		return 0;

	const ShmPair::KVPair& kvPair = sit->second;
	const SecInfo& secInfo = shm._block->_sections[kvPair._index];
	return secInfo._updatetime;
}

bool ShareBlocks::commit_section(const char* domain, const char* section)
{
	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return false;

	ShmPair& shm = (ShmPair&)it->second;
	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
		return false;

	ShmPair::KVPair& kvPair = (ShmPair::KVPair&)sit->second;
	SecInfo& secInfo = shm._block->_sections[kvPair._index];
	secInfo._updatetime = TimeUtils::getLocalTimeNow();
	return true;
}

bool ShareBlocks::delete_section(const char* domain, const char*section)
{
	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return false;

	ShmPair& shm = (ShmPair&)it->second;
	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
		return true;

	uint32_t idx = sit->second._index;
	shm._sections.erase(sit);
	shm._block->_sections[idx]._state = 2;
	shm._block->_updatetime = TimeUtils::getLocalTimeNow();
	return true;
}

void* ShareBlocks::make_valid(const char* domain, const char* section, const char* key, ValueType vType, SecInfo* &secInfo)
{
	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return nullptr;

	std::size_t len = SMVT_SIZES[vType];

	ShmPair& shm = (ShmPair&)it->second;
	KeyInfo* keyInfo = nullptr;
	ShmPair::KVPair* kvPair = nullptr;
	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
	{
		//如果不是master，就不能创建
		if (!shm._master)
			return nullptr;

		if (shm._block->_count == MAX_SEC_CNT)
		{
			//已经没有额外的空间可以分配了
			return nullptr;
		}

		secInfo = &shm._block->_sections[shm._block->_count];
		wt_strcpy(secInfo->_name, section);
		secInfo->_updatetime = TimeUtils::getLocalTimeNow();
		kvPair = &shm._sections[section];
		kvPair->_index = shm._block->_count;
		shm._block->_count++;
	}
	else
	{
		kvPair = (ShmPair::KVPair*)&sit->second;
	}

	secInfo = &shm._block->_sections[kvPair->_index];
	secInfo->_state = 1;

	auto kit = kvPair->_keys.find(key);
	if (kit == kvPair->_keys.end())
	{
		//如果不是master，就不能创建
		if (!shm._master)
			return nullptr;

		if (secInfo->_count == MAX_KEY_CNT)
			return nullptr;

		if (secInfo->_offset + len > 1024)
			return nullptr;

		keyInfo = &secInfo->_keys[secInfo->_count];
		wt_strcpy(keyInfo->_key, key);
		keyInfo->_updatetime = TimeUtils::getLocalTimeNow();
		keyInfo->_offset = secInfo->_offset;
		kvPair->_keys[key] = keyInfo;

		//字符串固定最大长度为64
		secInfo->_count++;
		secInfo->_offset += (uint32_t)len;
	}
	else
	{
		keyInfo = kit->second;
	}

	return keyInfo;
}

void* ShareBlocks::check_valid(const char* domain, const char* section, const char* key, ValueType vType, SecInfo* &secInfo)
{
	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return nullptr;

	ShmPair& shm = (ShmPair&)it->second;
	KeyInfo* keyInfo = nullptr;
	ShmPair::KVPair* kvPair = nullptr;
	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
	{
		return nullptr;
	}
	else
	{
		kvPair = (ShmPair::KVPair*)&sit->second;
	}

	secInfo = &shm._block->_sections[kvPair->_index];

	auto kit = kvPair->_keys.find(key);
	if (kit == kvPair->_keys.end())
	{
		return nullptr;
	}
	else
	{
		keyInfo = kit->second;
		if (keyInfo->_type != vType)
			return nullptr;

		return keyInfo;
	}
}

std::vector<std::string> ShareBlocks::get_sections(const char* domain)
{
	static std::vector<std::string> emptyRet;

	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return emptyRet;

	std::vector<std::string> ret;
	const ShmPair& shm = it->second;
	for (uint32_t i = 0; i < shm._block->_count; i++)
	{
		if (shm._block->_sections[i]._state != 1)
			continue;

		ret.emplace_back(shm._block->_sections[i]._name);
	}

	return std::move(ret);
}

std::vector<KeyInfo*> ShareBlocks::get_keys(const char* domain, const char* section)
{
	static std::vector<KeyInfo*> emptyRet;

	auto it = _shm_blocks.find(domain);
	if (it == _shm_blocks.end())
		return emptyRet;

	const ShmPair& shm = it->second;
	if(shm._sections.size() != shm._block->_count)
	{
		update_slave(domain, true);
	}

	auto sit = shm._sections.find(section);
	if (sit == shm._sections.end())
		return emptyRet;

	std::vector<KeyInfo*> ret;
	const ShmPair::KVPair& kvPair = sit->second;
	for (auto& v : kvPair._keys)
	{
		ret.emplace_back(v.second);
	}

	return std::move(ret);
}

const char* ShareBlocks::allocate_string(const char* domain, const char* section, const char* key, const char* initVal /* = "" */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_STRING, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if (keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_STRING;
		wt_strcpy(secInfo->_data + keyInfo->_offset, initVal, SMVT_SIZES[SMVT_STRING]);
	}

	return (secInfo->_data + keyInfo->_offset);
}

int32_t* ShareBlocks::allocate_int32(const char* domain, const char* section, const char* key, int32_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_INT32, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if (keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_INT32;
		*secInfo->get<int32_t>(keyInfo->_offset) = initVal;
	}

	return secInfo->get<int32_t>(keyInfo->_offset);
}

int64_t* ShareBlocks::allocate_int64(const char* domain, const char* section, const char* key, int64_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_INT64, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if (keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_INT64;
		*secInfo->get<int64_t>(keyInfo->_offset) = initVal;
	}

	return secInfo->get<int64_t>(keyInfo->_offset);
}

uint32_t* ShareBlocks::allocate_uint32(const char* domain, const char* section, const char* key, uint32_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_UINT32, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if (keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_UINT32;
		*secInfo->get<uint32_t>(keyInfo->_offset) = initVal;
	}

	return secInfo->get<uint32_t>(keyInfo->_offset);
}

uint64_t* ShareBlocks::allocate_uint64(const char* domain, const char* section, const char* key, uint64_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_UINT64, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if (keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_UINT64;
		*secInfo->get<uint64_t>(keyInfo->_offset) = initVal;
	}

	return secInfo->get<uint64_t>(keyInfo->_offset);
}

double* ShareBlocks::allocate_double(const char* domain, const char* section, const char* key, double initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_DOUBLE, secInfo);
	if (keyInfo == nullptr)
		return NULL;

	if(keyInfo->_type == 0 || bForceWrite)
	{
		//如果type为0，说明是新分配的，则用初始值填充
		keyInfo->_type = SMVT_DOUBLE;
		*secInfo->get<double>(keyInfo->_offset) = initVal;
	}

	return secInfo->get<double>(keyInfo->_offset);
}

bool ShareBlocks::set_string(const char* domain, const char* section, const char* key, const char* val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_STRING, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_STRING;
	wt_strcpy(secInfo->_data + keyInfo->_offset, val, SMVT_SIZES[SMVT_STRING]);

	return true;
}

bool ShareBlocks::set_int32(const char* domain, const char* section, const char* key, int32_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_INT32, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_INT32;
	*secInfo->get<int32_t>(keyInfo->_offset) = val;

	return true;
}

bool ShareBlocks::set_int64(const char* domain, const char* section, const char* key, int64_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_INT64, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_INT64;
	*secInfo->get<int64_t>(keyInfo->_offset) = val;

	return true;
}

bool ShareBlocks::set_uint32(const char* domain, const char* section, const char* key, uint32_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_UINT32, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_UINT32;
	*secInfo->get<uint32_t>(keyInfo->_offset) = val;

	return true;
}

bool ShareBlocks::set_uint64(const char* domain, const char* section, const char* key, uint64_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_UINT64, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_UINT64;
	*secInfo->get<uint64_t>(keyInfo->_offset) = val;

	return true;
}

bool ShareBlocks::set_double(const char* domain, const char* section, const char* key, double val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, SMVT_DOUBLE, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_DOUBLE;
	*secInfo->get<double>(keyInfo->_offset) = val;

	return true;
}

const char* ShareBlocks::get_string(const char* domain, const char* section, const char* key, const char* defVal /* = "" */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_STRING, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return (const char*)(secInfo->_data + keyInfo->_offset);
}

int32_t ShareBlocks::get_int32(const char* domain, const char* section, const char* key, int32_t defVal /* = 0 */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_INT32, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return *(int32_t*)(secInfo->_data + keyInfo->_offset);
}

uint32_t ShareBlocks::get_uint32(const char* domain, const char* section, const char* key, uint32_t defVal /* = 0 */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_UINT32, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return *(uint32_t*)(secInfo->_data + keyInfo->_offset);
}

int64_t ShareBlocks::get_int64(const char* domain, const char* section, const char* key, int64_t defVal /* = 0 */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_INT64, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return *(int64_t*)(secInfo->_data + keyInfo->_offset);
}

uint64_t ShareBlocks::get_uint64(const char* domain, const char* section, const char* key, uint64_t defVal /* = 0 */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_UINT64, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return *(uint64_t*)(secInfo->_data + keyInfo->_offset);
}

double ShareBlocks::get_double(const char* domain, const char* section, const char* key, double defVal /* = 0 */)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)check_valid(domain, section, key, SMVT_DOUBLE, secInfo);
	if (keyInfo == nullptr)
		return defVal;

	return *(double*)(secInfo->_data + keyInfo->_offset);
}

bool ShareBlocks::init_cmder(const char* name, bool isCmder /* = false */, const char* path /* = "" */)
{
	CmdPair& cmdPair = _cmd_blocks[name];
	if (cmdPair._block != NULL)
		return true;

	std::string filename = path;
	if (filename.empty())
		filename = ".cmd";

	if (!StdFile::exists(filename.c_str()))
	{
		BoostFile bf;
		bf.create_new_file(filename.c_str());
		bf.truncate_file(sizeof(CmdBlock));
		bf.close_file();
	}

	cmdPair._domain.reset(new BoostMappingFile);
	cmdPair._domain->map(filename.c_str());
	cmdPair._cmder = isCmder;
	cmdPair._block = (CmdBlock*)cmdPair._domain->addr();
	if(cmdPair._block->_capacity == 0)
		new(cmdPair._domain->addr()) CmdBlock();

	if(cmdPair._cmder)
#ifdef _MSC_VER
		cmdPair._block->_cmdpid = _getpid();
#else
		cmdPair._block->_cmdpid = getpid();
#endif
  
	
	//启动的时候都做一下偏移
	cmdPair._block->_writable %= cmdPair._block->_capacity;
	if(cmdPair._block->_readable != UINT32_MAX)
	{
		cmdPair._block->_readable %= cmdPair._block->_capacity;
		if (cmdPair._block->_readable > cmdPair._block->_writable)
			cmdPair._block->_writable += cmdPair._block->_capacity;
	}

	return true;
}

bool ShareBlocks::add_cmd(const char* name, const char* cmd)
{
	auto it = _cmd_blocks.find(name);
	if (it == _cmd_blocks.end())
		return false;

	CmdPair& cmdPair = it->second;

	if (cmdPair._block == NULL)
		return false;

#ifdef _MSC_VER
    if (cmdPair._block->_cmdpid != _getpid())
#else
	if (cmdPair._block->_cmdpid != getpid())
#endif
	
		return false;

	/*
	 *	先移动写的下标，然后写入数据
	 *	写完了以后，再移动读的下标
	 */
	uint32_t wIdx = cmdPair._block->_writable++;
	uint32_t realIdx = wIdx % cmdPair._block->_capacity;
	cmdPair._block->_commands[realIdx]._state = 0;
	strcpy(cmdPair._block->_commands[realIdx]._command, cmd);
	cmdPair._block->_readable = wIdx;
	return true;
}

const char* ShareBlocks::get_cmd(const char* name, uint32_t& lastIdx)
{
	auto it = _cmd_blocks.find(name);
	if (it == _cmd_blocks.end())
		return "";

	CmdPair& cmdPair = it->second;

	if (cmdPair._block == NULL)
		return "";

	//指令下达者就不需要获取指令了
	if (cmdPair._cmder)
		return "";

	//说明刚启动，之前的命令全部作废
	if (cmdPair._block->_readable == UINT32_MAX)
	{
		lastIdx = 999999;
		return "";
	}
	else if (lastIdx == UINT32_MAX && cmdPair._block->_readable != UINT32_MAX)
	{
		lastIdx = cmdPair._block->_readable;
		return "";
	}
	else if(lastIdx == 999999 && cmdPair._block->_readable != UINT32_MAX)
	{
		lastIdx = 0;
		return cmdPair._block->_commands[lastIdx]._command;
	}
	else if(lastIdx >= cmdPair._block->_readable)
	{
		return "";
	}
	else
	{
		lastIdx++;
		return cmdPair._block->_commands[lastIdx % cmdPair._block->_capacity]._command;
	}
}