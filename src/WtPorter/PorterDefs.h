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

static const WtUInt32	EVENT_ENGINE_INIT	= 1;	//框架初始化
static const WtUInt32	EVENT_SESSION_BEGIN = 2;	//交易日开始
static const WtUInt32	EVENT_SESSION_END	= 3;	//交易日结束
static const WtUInt32	EVENT_ENGINE_SCHDL	= 4;	//框架调度

static const WtUInt32	CHNL_EVENT_READY	= 1000;	//通道就绪事件
static const WtUInt32	CHNL_EVENT_LOST		= 1001;	//通道断开事件

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncGetBarsCallback)(CtxHandler cHandle, const char* code, const char* period, WTSBarStruct* bar, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(CtxHandler cHandle, const char* code, WTSTickStruct* tick, bool isLast);
typedef void(PORTER_FLAG *FuncStraInitCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncStraTickCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncStraCalcCallback)(CtxHandler cHandle, WtUInt32 curDate, WtUInt32 curTime);
typedef void(PORTER_FLAG *FuncStraBarCallback)(CtxHandler cHandle, const char* code, const char* period, WTSBarStruct* newBar);

//////////////////////////////////////////////////////////////////////////
//HFT回调函数
typedef void(PORTER_FLAG *FuncHftChannelCallback)(CtxHandler cHandle, const char* trader, WtUInt32 evtid);	//交易通道事件回调
typedef void(PORTER_FLAG *FuncHftOrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled);
typedef void(PORTER_FLAG *FuncHftTrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price);
typedef void(PORTER_FLAG *FuncHftEntrustCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message);


typedef void(PORTER_FLAG *FuncEventCallback)(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime);
