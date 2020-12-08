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

#define MAX_INSTRUMENT_LENGTH	32
#define MAX_EXCHANGE_LENGTH		10

#define CHECK_NULL(x)			if(NULL==x)return;
#define STATIC_CONVERT(x,T)		static_cast<T>(x)

#ifndef DBL_MAX
#define DBL_MAX 1.7976931348623158e+308
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F        /* max value */
#endif

#ifdef _WIN32
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


#define NS_OTP_BEGIN	namespace otp{
#define NS_OTP_END	}//namespace wts
#define	USING_NS_OTP	using namespace otp

#ifdef _WIN32
#	define EXPORT_FLAG __declspec(dllexport)
#else
#	define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif

static const char WT_VERSION[] = "v0.5.2";