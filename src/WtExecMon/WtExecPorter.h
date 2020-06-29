#pragma once
#include <stdint.h>
#include "../Share/WTSTypes.h"

NS_OTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
NS_OTP_END

USING_NS_OTP;

#ifdef _WIN32
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#endif

typedef unsigned long		CtxHandler;
typedef unsigned long		WtUInt32;
typedef unsigned long long	WtUInt64;
typedef const char*			WtString;

#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG	void		init_exec(const char* logProfile);

	EXPORT_FLAG	void		config_exec(const char* cfgfile);

	EXPORT_FLAG	void		run_exec();

	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	void		release_exec();

#ifdef __cplusplus
}
#endif