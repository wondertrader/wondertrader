#pragma once
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG void		initialize(WtString cfgFile, WtString logCfg);
	EXPORT_FLAG void		start();

	EXPORT_FLAG	WtString	get_version();
	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);

#ifdef __cplusplus
}
#endif