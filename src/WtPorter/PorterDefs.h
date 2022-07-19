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
struct WTSOrdDtlStruct;
struct WTSOrdQueStruct;
struct WTSTransStruct;
NS_WTP_END

USING_NS_WTP;

typedef unsigned long		CtxHandler;

static const WtUInt32	EVENT_ENGINE_INIT	= 1;	//��ܳ�ʼ��
static const WtUInt32	EVENT_SESSION_BEGIN = 2;	//�����տ�ʼ
static const WtUInt32	EVENT_SESSION_END	= 3;	//�����ս���
static const WtUInt32	EVENT_ENGINE_SCHDL	= 4;	//��ܵ���

static const WtUInt32	CHNL_EVENT_READY	= 1000;	//ͨ�������¼�
static const WtUInt32	CHNL_EVENT_LOST		= 1001;	//ͨ���Ͽ��¼�

//��־����
static const WtUInt32	LOG_LEVEL_DEBUG		= 0;
static const WtUInt32	LOG_LEVEL_INFO		= 1;
static const WtUInt32	LOG_LEVEL_WARN		= 2;
static const WtUInt32	LOG_LEVEL_ERROR		= 3;


/*
*	�ص���������
*/
typedef void(PORTER_FLAG *FuncGetBarsCallback)(CtxHandler cHandle, const char* stdCode, const char* period, WTSBarStruct* bar, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncGetTicksCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraInitCallback)(CtxHandler cHandle);
typedef void(PORTER_FLAG *FuncSessionEvtCallback)(CtxHandler cHandle, WtUInt32 curTDate, bool isBegin);
typedef void(PORTER_FLAG *FuncStraTickCallback)(CtxHandler cHandle, const char* stdCode, WTSTickStruct* tick);
typedef void(PORTER_FLAG *FuncStraCalcCallback)(CtxHandler cHandle, WtUInt32 curDate, WtUInt32 curTime);
typedef void(PORTER_FLAG *FuncStraBarCallback)(CtxHandler cHandle, const char* stdCode, const char* period, WTSBarStruct* newBar);
typedef void(PORTER_FLAG *FuncGetPositionCallback)(CtxHandler cHandle, const char* stdCode, double position, bool isLast);
typedef void(PORTER_FLAG *FuncStraCondTriggerCallback)(CtxHandler cHandle, const char* stdCode, double target, double price, const char* usertag);

typedef void(PORTER_FLAG *FuncStraOrdQueCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdQueStruct* ordQue);
typedef void(PORTER_FLAG *FuncGetOrdQueCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdQueStruct* ordQue, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdDtlStruct* ordDtl);
typedef void(PORTER_FLAG *FuncGetOrdDtlCallback)(CtxHandler cHandle, const char* stdCode, WTSOrdDtlStruct* ordDtl, WtUInt32 count, bool isLast);
typedef void(PORTER_FLAG *FuncStraTransCallback)(CtxHandler cHandle, const char* stdCode, WTSTransStruct* trans);
typedef void(PORTER_FLAG *FuncGetTransCallback)(CtxHandler cHandle, const char* stdCode, WTSTransStruct* trans, WtUInt32 count, bool isLast);

//////////////////////////////////////////////////////////////////////////
//HFT�ص�����
typedef void(PORTER_FLAG *FuncHftChannelCallback)(CtxHandler cHandle, const char* trader, WtUInt32 evtid);	//����ͨ���¼��ص�
typedef void(PORTER_FLAG *FuncHftOrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled, const char* userTag);
typedef void(PORTER_FLAG *FuncHftTrdCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool isBuy, double vol, double price, const char* userTag);
typedef void(PORTER_FLAG *FuncHftEntrustCallback)(CtxHandler cHandle, WtUInt32 localid, const char* stdCode, bool bSuccess, const char* message, const char* userTag);
typedef void(PORTER_FLAG *FuncHftPosCallback)(CtxHandler cHandle, const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail);


typedef void(PORTER_FLAG *FuncEventCallback)(WtUInt32 evtId, WtUInt32 curDate, WtUInt32 curTime);

//////////////////////////////////////////////////////////////////////////
//��չParser�ص�����
static const WtUInt32	EVENT_PARSER_INIT		= 1;	//Parser��ʼ��
static const WtUInt32	EVENT_PARSER_CONNECT	= 2;	//Parser����
static const WtUInt32	EVENT_PARSER_DISCONNECT = 3;	//Parser�Ͽ�����
static const WtUInt32	EVENT_PARSER_RELEASE	= 4;	//Parser�ͷ�

typedef void(PORTER_FLAG *FuncParserEvtCallback)(WtUInt32 evtId, const char* id);
typedef void(PORTER_FLAG *FuncParserSubCallback)(const char* id, const char* fullCode, bool isForSub);

//////////////////////////////////////////////////////////////////////////
//��չExecuter�ص�����
typedef void(PORTER_FLAG *FuncExecInitCallback)(const char* id);
typedef void(PORTER_FLAG *FuncExecCmdCallback)(const char* id, const char* StdCode, double targetPos);

//////////////////////////////////////////////////////////////////////////
//�ⲿ���ݼ���ģ��
typedef bool(PORTER_FLAG *FuncLoadFnlBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadRawBars)(const char* stdCode, const char* period);
typedef bool(PORTER_FLAG *FuncLoadAdjFactors)(const char* stdCode);
typedef bool(PORTER_FLAG *FuncLoadRawTicks)(const char* stdCode, uint32_t uDate);
