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

	EXPORT_FLAG void		initialize(WtString cfgFile, WtString logCfg);

	EXPORT_FLAG	WtString	get_version();

	EXPORT_FLAG	WtUInt32	get_bars(const char* stdCode, const char* period, WtUInt32 count, WtUInt64 endTime, FuncGetBarsCallback cb, FuncDataCountCallback cbCnt);

	EXPORT_FLAG	WtUInt32	get_ticks(const char* stdCode, WtUInt32 count, WtUInt64 endTime, FuncGetTicksCallback cb, FuncDataCountCallback cbCnt);

#ifdef __cplusplus
}
#endif