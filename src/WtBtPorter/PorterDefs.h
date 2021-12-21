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
struct WTSOrdDtlStruct;
struct WTSOrdQueStruct;
struct WTSTransStruct;
NS_OTP_END

USING_NS_OTP;

typedef unsigned long		CtxHandler;

static const WtUInt32	EVENT_ENGINE_INIT		= 1;	//框架初始化
static const WtUInt32	EVENT_SESSION_BEGIN		= 2;	//交易日开始
static const WtUInt32	EVENT_SESSION_END		= 3;	//交易日结束
static const WtUInt32	EVENT_ENGINE_SCHDL		= 4;	//框架调度
static const WtUInt32	EVENT_BACKTEST_END		= 5;	//回测结束

/*
*	回调函数定义
*/
typedef void(PORTER_FLAG *FuncGetBarsCallback)(CtxHandler cHandle, const char* stdCode, const char* period, WTSBarStruct* bar, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraInitCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncSessionEvtCallback)(CtxHandler cHandle, WtUInt32 curTDate, bool isBegin);
typedef void(PORTER_FLAG *FuncStraTickCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncStraCalcCallback)(CtxHandler cHandle, WtUInt32 uDate, WtUInt32 uTime);
typedef void(PORTER_FLAG *FuncStraBarCallback)(CtxHandler cHandle, const char* stdCode, const char* period, WTSBarStruct* newBar);
typedef void(PORTER_FLAG *FuncGetPositionCallback)(CtxHandler cHandle, const char* stdCode, double position, bool isLast);

typedef void(PORTER_FLAG *FuncStraOrdQueCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdQueStruct* ordQue);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdQueStruct* ordQue, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdDtlStruct* ordDtl);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdDtlStruct* ordDtl, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraTransCallback)(CtxHandler cHandle, const char* stdCode, WTSTransStruct* trans);
typedef void(PORTER_FLAG *FuncGetTransCallback)(CtxHandler cHandle, const char* stdCode, WTSTransStruct* trans, WtUInt32 count, bool isLast);

//////////////////////////////////////////////////////////////////////////
//HFT回调函数
typedef void(PORTER_FLAG *FuncHftChannelCallback)(CtxHandler cHandle, const char* trader, WtUInt32 evtid);	//交易通道事件回调
typedef void(PORTER_FLAG *FuncHftOrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
typedef void(PORTER_FLAG *FuncHftTrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
typedef void(PORTER_FLAG *FuncHftEntrustCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);

typedef void(PORTER_FLAG *FuncEventCallback)(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime);

//////////////////////////////////////////////////////////////////////////
//外部数据加载模块
typedef bool(PORTER_FLAG *FuncLoadFnlBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadRawBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadAdjFactors)(const char* stdCode);
typedef bool(PORTER_FLAG *FuncLoadRawTicks)(const char* stdCode, uint32_t uDate);
