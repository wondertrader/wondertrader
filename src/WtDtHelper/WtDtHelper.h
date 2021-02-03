/*!
 * \file WtDtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once

#include "../Includes/WTSTypes.h"

NS_OTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
struct WTSOrdDtlStruct;
struct WTSOrdQueStruct;
struct WTSTransStruct;
NS_OTP_END

USING_NS_OTP;

#ifdef _WIN32
#	define EXPORT_FLAG __declspec(dllexport)
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#	define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

typedef unsigned long		WtUInt32;
typedef const char*			WtString;

typedef void(PORTER_FLAG *FuncLogCallback)(WtString message);
typedef void(PORTER_FLAG *FuncGetBarsCallback)(WTSBarStruct* bar, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(WTSTickStruct* tick, bool isLast);

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG	void		dump_bars(WtString binFolder, WtString csvFolder, WtString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		dump_ticks(WtString binFolder, WtString csvFolder, WtString strFilter = "", FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	void		trans_csv_bars(WtString csvFolder, WtString binFolder, WtString period, FuncLogCallback cbLogger = NULL);

	EXPORT_FLAG	WtUInt32	read_dsb_ticks(WtString tickFile, FuncGetTicksCallback cb, FuncLogCallback cbLogger = NULL);
	EXPORT_FLAG	WtUInt32	read_dsb_bars(WtString barFile, FuncGetBarsCallback cb, FuncLogCallback cbLogger = NULL);

#ifdef __cplusplus
}
#endif