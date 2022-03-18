/*!
 * \file WTSMarcos.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief WonderTrader基础宏定义文件
 */
#pragma once
#include <limits.h>
#include <string.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define MAX_INSTRUMENT_LENGTH	32
#define MAX_EXCHANGE_LENGTH		16

#define STATIC_CONVERT(x,T)		static_cast<T>(x)

#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623158e+308
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F        /* max value */
#endif

#ifdef _MSC_VER
#define INVALID_DOUBLE		DBL_MAX
#define INVALID_INT32		INT_MAX
#define INVALID_UINT32		UINT_MAX
#define INVALID_INT64		_I64_MAX
#define INVALID_UINT64		_UI64_MAX
#else
#define INVALID_DOUBLE		1.7976931348623158e+308 /* max value */
#define INVALID_INT32		2147483647
#define INVALID_UINT32		0xffffffffUL
#define INVALID_INT64		9223372036854775807LL
#define INVALID_UINT64		0xffffffffffffffffULL
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#define NS_WTP_BEGIN	namespace wtp{
#define NS_WTP_END	}//namespace wpt
#define	USING_NS_WTP	using namespace wtp

#ifndef EXPORT_FLAG
#ifdef _MSC_VER
#	define EXPORT_FLAG __declspec(dllexport)
#else
#	define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif
#endif

#ifndef PORTER_FLAG
#ifdef _MSC_VER
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG 
#endif
#endif

typedef unsigned long		WtUInt32;
typedef unsigned long long	WtUInt64;
typedef const char*			WtString;

#ifdef _MSC_VER
#define wt_stricmp _stricmp
#else
#define wt_stricmp strcasecmp
#endif

/*
 *	By Wesley @ 2022.03.17
 *	重写一个strcpy
 *	核心的要点就是不用strcpy
 */
inline size_t wt_strcpy(char* des, const char* src, size_t len = 0)
{
	len = (len == 0) ? strlen(src) : len;
	memcpy(des, src, len);
	des[len] = '\0';
	return len;
}