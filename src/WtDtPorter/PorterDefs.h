/*!
 * \file PorterDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include <stdint.h>
#include "../Includes/WTSTypes.h"

NS_OTP_BEGIN
NS_OTP_END

USING_NS_OTP;

#ifdef _WIN32
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#endif

typedef unsigned long		WtUInt32;
typedef unsigned long long	WtUInt64;
typedef const char*			WtString;

