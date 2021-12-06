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
struct WTSTickStruct;
struct WTSBarStruct;
struct WTSOrdDtlStruct;
struct WTSOrdQueStruct;
struct WTSTransStruct;
NS_OTP_END

USING_NS_OTP;

//////////////////////////////////////////////////////////////////////////
//扩展Parser回调函数
static const WtUInt32	EVENT_PARSER_INIT = 1;	//Parser初始化
static const WtUInt32	EVENT_PARSER_CONNECT = 2;	//Parser连接
static const WtUInt32	EVENT_PARSER_DISCONNECT = 3;	//Parser断开连接
static const WtUInt32	EVENT_PARSER_RELEASE = 4;	//Parser释放

typedef void(PORTER_FLAG *FuncParserEvtCallback)(WtUInt32 evtId, const char* id);
typedef void(PORTER_FLAG *FuncParserSubCallback)(const char* id, const char* fullCode, bool isForSub);


//////////////////////////////////////////////////////////////////////////
//扩展Dumper回调函数
typedef bool(PORTER_FLAG *FuncDumpBars)(const char* id, const char* stdCode, const char* period, WTSBarStruct* bars, WtUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTicks)(const char* id, const char* stdCode, WtUInt32 uDate, WTSTickStruct* ticks, WtUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdQue)(const char* id, const char* stdCode, WtUInt32 uDate, WTSOrdQueStruct* items, WtUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpOrdDtl)(const char* id, const char* stdCode, WtUInt32 uDate, WTSOrdDtlStruct* items, WtUInt32 count);
typedef bool(PORTER_FLAG *FuncDumpTrans)(const char* id, const char* stdCode, WtUInt32 uDate, WTSTransStruct* items, WtUInt32 count);
