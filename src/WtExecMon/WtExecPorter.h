#pragma once
#include <stdint.h>
#include "../Includes/WTSMarcos.h"

typedef const char*			WtString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(WtString logCfg, bool isFile = true);

	EXPORT_FLAG	void		config_exec(WtString cfgfile, bool isFile = true);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, WtString message, WtString catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		release_exec();

	EXPORT_FLAG	void		set_position(WtString stdCode, double targetPos);

	EXPORT_FLAG	void		commit_positions();

#ifdef __cplusplus
}
#endif