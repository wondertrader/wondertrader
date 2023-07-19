#pragma once
#include <stdint.h>

class ShareManager
{
public:
	static bool	initialize();

public:
	static bool	init_domain(const char* id);

	static bool	set_value(const char* domain, const char* section, const char* key, const char* val);

	static bool	set_value(const char* domain, const char* section, const char* key, int32_t val);

	static bool	set_value(const char* domain, const char* section, const char* key, int64_t val);

	static bool	set_value(const char* domain, const char* section, const char* key, uint32_t val);

	static bool	set_value(const char* domain, const char* section, const char* key, uint64_t val);

	static bool	set_value(const char* domain, const char* section, const char* key, double val);

	static const char*	get_value(const char* domain, const char* section, const char* key, const char* defVal = "");

	static int32_t	get_value(const char* domain, const char* section, const char* key, int32_t defVal = 0);

	static int64_t	get_value(const char* domain, const char* section, const char* key, int64_t defVal = 0);

	static uint32_t	get_value(const char* domain, const char* section, const char* key, uint32_t defVal = 0);

	static uint64_t	get_value(const char* domain, const char* section, const char* key, uint64_t defVal = 0);

	static double	get_value(const char* domain, const char* section, const char* key, double defVal = 0);

private:
	static bool _inited;
};

