#pragma once
#include <stdint.h>
#include "../Share/WTSMarcos.h"


#ifdef _WIN32
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#endif

typedef const char*			WtString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(WtString logProfile);

	EXPORT_FLAG	void		config_exec(WtString cfgfile);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, WtString message, WtString catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		release_exec();

	EXPORT_FLAG	void		set_position(WtString stdCode, double targetPos);

#ifdef __cplusplus
}
#endif