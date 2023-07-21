#include "ShareBlocks.h"
#include "../Share/BoostFile.hpp"
#include "../Share/TimeUtils.hpp"

using namespace shareblock;

bool ShareBlocks::init_master(const char* name, const char* path/* = ""*/)
{
	ShmPair& shm = (ShmPair&)_shm_blocks[name];
	if (shm._block != NULL)
		return true;

	std::string filename = path;
	if (filename.empty())
		filename = name;

	{
		BoostFile bf;
		bf.create_new_file(filename.c_str());
		bf.truncate_file(sizeof(ShmBlock));
		bf.close_file();
	}

	shm._domain.reset(new BoostMappingFile);
	shm._domain->map(name);
	shm._master = true;
	shm._block = (ShmBlock*)shm._domain->addr();
	wt_strcpy(shm._block->_flag, BLK_FLAG, 8);
	shm._block->_updatetime = TimeUtils::getLocalTimeNow();
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
	shm._domain->map(name);
	shm._master = false;
	shm._block = (ShmBlock*)shm._domain->addr();

	//slave模式下，应该需要加载一下
	//if (strcmp(shm._block->_flag, BLK_FLAG) == 0)
	{
		//这里要做初始化，要把已经有的key加载进去
		for (uint32_t i = 0; i < shm._block->_count; i++)
		{
			SecInfo& secInfo = shm._block->_sections[i];
			if(secInfo._count == 0)
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

void* ShareBlocks::make_valid(const char* domain, const char* section, const char* key, std::size_t len, SecInfo* &secInfo)
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

	auto kit = kvPair->_keys.find(key);
	if (kit == kvPair->_keys.end())
	{
		//如果不是master，就不能创建
		if (!shm._master)
			return nullptr;

		if (secInfo->_count == 32)
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
	for(uint32_t i = 0; i < shm._block->_count; i++)
	{
		ret.emplace_back(shm._block->_sections->_name);
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
	auto sit = shm._sections.find(section);
	if(sit == shm._sections.end())
		return emptyRet;

	std::vector<KeyInfo*> ret;
	const ShmPair::KVPair& kvPair = sit->second;
	for(auto& v : kvPair._keys)
	{
		ret.emplace_back(v.second);
	}

	return std::move(ret);
}

bool ShareBlocks::set_string(const char* domain, const char* section, const char* key, const char* val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_STRING, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_STRING;
	wt_strcpy(secInfo->_data + keyInfo->_offset, val, VTL_STRING);

	return true;
}

bool ShareBlocks::set_int32(const char* domain, const char* section, const char* key, int32_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_INT32, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_INT32;
	*((int32_t*)(secInfo->_data + keyInfo->_offset)) = val;

	return true;
}

bool ShareBlocks::set_int64(const char* domain, const char* section, const char* key, int64_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_INT64, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_INT64;
	*((int64_t*)(secInfo->_data + keyInfo->_offset)) = val;

	return true;
}

bool ShareBlocks::set_uint32(const char* domain, const char* section, const char* key, uint32_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_UINT32, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_UINT32;
	*((uint32_t*)(secInfo->_data + keyInfo->_offset)) = val;

	return true;
}

bool ShareBlocks::set_uint64(const char* domain, const char* section, const char* key, uint64_t val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_UINT64, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_UINT64;
	*((uint64_t*)(secInfo->_data + keyInfo->_offset)) = val;

	return true;
}

bool ShareBlocks::set_double(const char* domain, const char* section, const char* key, double val)
{
	SecInfo* secInfo = nullptr;
	KeyInfo* keyInfo = (KeyInfo*)make_valid(domain, section, key, VTL_DOUBLE, secInfo);
	if (keyInfo == nullptr)
		return false;

	keyInfo->_type = SMVT_DOUBLE;
	*((double*)(secInfo->_data + keyInfo->_offset)) = val;

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