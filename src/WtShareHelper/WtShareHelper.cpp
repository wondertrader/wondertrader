#include "WtShareHelper.h"
#include "ShareBlocks.h"

using namespace shareblock;

bool init_master(const char* id, const char* path/* = ""*/)
{
	return ShareBlocks::one().init_master(id, path);
}

bool init_slave(const char* id, const char* path/* = ""*/)
{
	return ShareBlocks::one().init_slave(id, path);
}

bool update_slave(const char* id)
{
	return ShareBlocks::one().update_slave(id);
}

uint32_t get_sections(const char* domain, FuncGetSections cb)
{
	auto ay = ShareBlocks::one().get_sections(domain);
	for (const std::string& v : ay)
		cb(v.c_str());

	return (uint32_t)ay.size();
}

uint32_t get_keys(const char* domain, const char* section, FuncGetKeys cb)
{
	auto ay = ShareBlocks::one().get_keys(domain, section);
	for (KeyInfo* v : ay)
		cb(v->_key, v->_type);

	return (uint32_t)ay.size();
}

uint64_t get_section_updatetime(const char* domain, const char* section)
{
	return ShareBlocks::one().get_section_updatetime(domain, section);
}

bool commit_section(const char* domain, const char* section)
{
	return ShareBlocks::one().commit_section(domain, section);
}

const char* allocate_string(const char* domain, const char* section, const char* key)
{
	return (const char*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_STRING);
}

int32_t* allocate_int32(const char* domain, const char* section, const char* key)
{
	return (int32_t*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_INT32);
}

int64_t* allocate_int64(const char* domain, const char* section, const char* key)
{
	return (int64_t*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_INT64);
}

uint32_t* allocate_uint32(const char* domain, const char* section, const char* key)
{
	return (uint32_t*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_UINT32);
}

uint64_t* allocate_uint64(const char* domain, const char* section, const char* key)
{
	return (uint64_t*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_UINT64);
}

double* allocate_double(const char* domain, const char* section, const char* key)
{
	return (double*)ShareBlocks::one().allocate_key(domain, section, key, SMVT_DOUBLE);
}

bool set_string(const char* domain, const char* section, const char* key, const char* val)
{
	return ShareBlocks::one().set_string(domain, section, key, val);
}

bool set_int32(const char* domain, const char* section, const char* key, int32_t val)
{
	return ShareBlocks::one().set_int32(domain, section, key, val);
}

bool set_int64(const char* domain, const char* section, const char* key, int64_t val)
{
	return ShareBlocks::one().set_int64(domain, section, key, val);
}

bool set_uint32(const char* domain, const char* section, const char* key, uint32_t val)
{
	return ShareBlocks::one().set_uint32(domain, section, key, val);
}

bool set_uint64(const char* domain, const char* section, const char* key, uint64_t val)
{
	return ShareBlocks::one().set_uint64(domain, section, key, val);
}

bool set_double(const char* domain, const char* section, const char* key, double val)
{
	return ShareBlocks::one().set_double(domain, section, key, val);
}

const char* get_string(const char* domain, const char* section, const char* key, const char* defVal /* = "" */)
{
	return ShareBlocks::one().get_string(domain, section, key, defVal);
}

int32_t get_int32(const char* domain, const char* section, const char* key, int32_t defVal /* = 0 */)
{
	return ShareBlocks::one().get_int32(domain, section, key, defVal);
}

int64_t get_int64(const char* domain, const char* section, const char* key, int64_t defVal /* = 0 */)
{
	return ShareBlocks::one().get_int64(domain, section, key, defVal);
}

uint32_t get_uint32(const char* domain, const char* section, const char* key, uint32_t defVal /* = 0 */)
{
	return ShareBlocks::one().get_uint32(domain, section, key, defVal);
}

uint64_t get_uint64(const char* domain, const char* section, const char* key, uint64_t defVal /* = 0 */)
{
	return ShareBlocks::one().get_uint64(domain, section, key, defVal);
}

double get_double(const char* domain, const char* section, const char* key, double defVal /* = 0 */)
{
	return ShareBlocks::one().get_double(domain, section, key, defVal);
}