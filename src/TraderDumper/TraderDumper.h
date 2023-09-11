#pragma once

#include "PorterDefs.h"

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		register_callbacks(FuncOnAccount cbAccount, FuncOnOrder  cbOrder, FuncOnTrade cbTrade, FuncOnPosition cbPosition);

	EXPORT_FLAG	void		init(const char* logProfile);

	EXPORT_FLAG	bool		config(const char* cfgfile, bool isFile);

	EXPORT_FLAG	void		run(bool bOnce);

	EXPORT_FLAG	void		release();

#ifdef __cplusplus
}
#endif