#pragma once

#include <stdint.h>
#include "../Includes/WTSMarcos.h"

typedef void(PORTER_FLAG *FuncGetSections)(const char*);

typedef void(PORTER_FLAG *FuncGetKeys)(const char*, uint64_t);

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	bool	init_master(const char* id, const char* path = "");

	EXPORT_FLAG	bool	init_slave(const char* id, const char* path = "");

	EXPORT_FLAG	uint32_t	get_sections(const char* domain, FuncGetSections cb);

	EXPORT_FLAG	uint32_t	get_keys(const char* domain, const char* section, FuncGetKeys cb);

	EXPORT_FLAG uint64_t	get_section_updatetime(const char* domain, const char* section);

	EXPORT_FLAG bool	commit_section(const char* domain, const char* section);

	EXPORT_FLAG bool	set_string(const char* domain, const char* section, const char* key, const char* val);

	EXPORT_FLAG bool	set_int32(const char* domain, const char* section, const char* key, int32_t val);

	EXPORT_FLAG bool	set_int64(const char* domain, const char* section, const char* key, int64_t val);

	EXPORT_FLAG bool	set_uint32(const char* domain, const char* section, const char* key, uint32_t val);

	EXPORT_FLAG bool	set_uint64(const char* domain, const char* section, const char* key, uint64_t val);

	EXPORT_FLAG bool	set_double(const char* domain, const char* section, const char* key, double val);

	EXPORT_FLAG const char*	get_string(const char* domain, const char* section, const char* key, const char* defVal = "");

	EXPORT_FLAG int32_t		get_int32(const char* domain, const char* section, const char* key, int32_t defVal = 0);

	EXPORT_FLAG int64_t		get_int64(const char* domain, const char* section, const char* key, int64_t defVal = 0);

	EXPORT_FLAG uint32_t	get_uint32(const char* domain, const char* section, const char* key, uint32_t defVal = 0);

	EXPORT_FLAG uint64_t	get_uint64(const char* domain, const char* section, const char* key, uint64_t defVal = 0);

	EXPORT_FLAG double		get_double(const char* domain, const char* section, const char* key, double defVal = 0);
	
#ifdef __cplusplus
}
#endif