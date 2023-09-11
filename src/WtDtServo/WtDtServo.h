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
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG void		initialize(WtString cfgFile, bool isFile, WtString logCfg, FuncOnTickCallback cbTick, FuncOnBarCallback cbBar);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	WtUInt32	get_bars_by_range(const char* stdCode, const char* period, WtUInt64 beginTime, WtUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_ticks_by_range(const char* stdCode, WtUInt64 beginTime, WtUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_bars_by_count(const char* stdCode, const char* period, WtUInt32 count, WtUInt64 endTime, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_ticks_by_count(const char* stdCode, WtUInt32 count, WtUInt64 endTime, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_ticks_by_date(const char* stdCode, WtUInt32 uDate, FuncGetTicksCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_sbars_by_date(const char* stdCode, WtUInt32 secs, WtUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_bars_by_date(const char* stdCode, const char* period, WtUInt32 uDate, FuncGetBarsCallback cb, FuncCountDataCallback cbCnt);

	EXPORT_FLAG void		subscribe_tick(const char* stdCode, bool bReplace);

	EXPORT_FLAG void		subscribe_bar(const char* stdCode, const char* period);

	EXPORT_FLAG void		clear_cache();

#ifdef __cplusplus
}
#endif