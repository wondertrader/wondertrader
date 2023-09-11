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

NS_WTP_BEGIN
struct WTSBarStruct;
struct WTSTickStruct;
NS_WTP_END

USING_NS_WTP;

typedef void(PORTER_FLAG *FuncGetBarsCallback)(WTSBarStruct* bar, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(WTSTickStruct* tick, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncCountDataCallback)(WtUInt32 dataCnt);
typedef void(PORTER_FLAG *FuncOnTickCallback)(const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncOnBarCallback)(const char* stdCode, const char* period, WTSBarStruct* bar);

