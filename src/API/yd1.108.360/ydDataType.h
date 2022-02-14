#ifndef YD_DATA_TYPE_H
#define YD_DATA_TYPE_H

#include <stdio.h>

typedef char YDExchangeID[12];
typedef char YDProductID[32];
typedef char YDInstrumentID[32];
typedef char YDAccountID[16];
typedef char YDPassword[16];
typedef char YDString[32];

typedef int YDSysOrderID;
typedef int YDTradeID;
typedef int YDLocalOrderID;
typedef int YDRFQID;

// Product Class
const int YD_PC_Futures=1;
const int YD_PC_Options=2;
const int YD_PC_Combination=3;
const int YD_PC_Index=4;
const int YD_PC_Cash=5;

// Options Type
const int YD_OT_NotOption=0;
const int YD_OT_CallOption=1;
const int YD_OT_PutOption=2;

// Direction
const int YD_D_Buy=0;
const int YD_D_Sell=1;

// Position Direction
const int YD_PD_Long=2;
const int YD_PD_Short=3;

// Position Date
const int YD_PSD_Today=1;
const int YD_PSD_History=2;

// Hedge Flag
const int YD_HF_Speculation=1;
const int YD_HF_Arbitrage=2;
const int YD_HF_Hedge=3;
const int YD_HF_Internal=4;

const int YD_MaxHedgeFlag=4;

// Trade Right
const int YD_TR_Allow=0;
const int YD_TR_CloseOnly=1;
const int YD_TR_Forbidden=2;

// Offset Flag
const int YD_OF_Open=0;
const int YD_OF_Close=1;
const int YD_OF_ForceClose=2;
const int YD_OF_CloseToday=3;
const int YD_OF_CloseYesterday=4;
/// For SHFE and INE, only YD_OF_CloseYesterday and YD_OF_CloseToday are valid for close. For all other exchanges, only YD_OF_Close is valid for close

// Order Type
const int YD_ODT_Limit=0;
const int YD_ODT_FAK=1;
const int YD_ODT_Market=2;
const int YD_ODT_FOK=3;

// Order Status
const int YD_OS_Accepted=0;
const int YD_OS_Queuing=1;
const int YD_OS_Canceled=2;
const int YD_OS_AllTraded=3;
const int YD_OS_Rejected=4;

// YD OrderFlag
const int YD_YOF_Normal=0;
const int YD_YOF_QuoteDerived=1;
const int YD_YOF_OptionExecute=2;
const int YD_YOF_OptionAbandonExecute=3;
const int YD_YOF_RequestForQuote=4;
const int YD_YOF_CombPosition=5;
const int YD_YOF_OptionExecuteTogether=6;
const int YD_YOF_PositionOffset=7;

// YD QuoteFlag, can be bitwise or of following flags
const int YD_YQF_ResponseOfRFQ=1;

// Direction when YDOrderFlag==YD_YOF_CombPosition
const int YD_D_Make=0;
const int YD_D_Split=1;

// Combine Hedge Flag
const int YD_CHF_SpecSpec=1;
const int YD_CHF_SpecHedge=2;
const int YD_CHF_HedgeHedge=3;
const int YD_CHF_HedgeSpec=4;

const int YD_MaxCombHedgeFlag=4;

// DCE Combine Position Type
const int YD_CPT_DCE_FuturesOffset=0;
const int YD_CPT_DCE_OptionsOffset=1;
const int YD_CPT_DCE_FuturesCalendarSpread=2;
const int YD_CPT_DCE_FuturesProductSpread=3;
const int YD_CPT_DCE_BuyOptionsVerticalSpread=4;
const int YD_CPT_DCE_SellOptionsVerticalSpread=5;
const int YD_CPT_DCE_OptionsStraddle=7;
const int YD_CPT_DCE_OptionsStrangle=8;
const int YD_CPT_DCE_BuyOptionsCovered=9;
const int YD_CPT_DCE_SellOptionsCovered=10;

// SSE/SZSE Combine Position Type
const int YD_CPT_StockOption_CNSJC=100;
const int YD_CPT_StockOption_CXSJC=101;
const int YD_CPT_StockOption_PNSJC=102;
const int YD_CPT_StockOption_PXSJC=103;
const int YD_CPT_StockOption_KS=104;
const int YD_CPT_StockOption_KKS=105;

// Connection Selection Type
const int YD_CS_Any=0;
const int YD_CS_Fixed=1;
const int YD_CS_Prefered=2;

// Alter Money Type
const int YD_AM_ModifyUsage=0;
const int YD_AM_Deposit=1;
const int YD_AM_FrozenWithdraw=2;
const int YD_AM_CancelFrozenWithdraw=3;
const int YD_AM_Withdraw=4;
const int YD_AM_DepositTo=5;
const int YD_AM_WithdrawTo=6;
const int YD_AM_ForceModifyUsage=7;

// Request Type
const int YD_RT_ChangePassword=0;
const int YD_RT_SetTradingRight=1;
const int YD_RT_AlterMoney=2;
const int YD_RT_SelectConnection=3;
const int YD_RT_AdminTrading=4;
const int YD_RT_UpdateMarginRate=5;

// API Event
const int YD_AE_TCPTradeConnected=0;
const int YD_AE_TCPTradeDisconnected=1;
const int YD_AE_TCPMarketDataConnected=2;
const int YD_AE_TCPMarketDataDisconnected=3;
const int YD_AE_ServerRestarted=4;
const int YD_AE_ServerSwitched=5;

// Account Flag
const int YD_AF_SelectConnection=1;
const int YD_AF_AutoMakeCombPosition=2;
const int YD_AF_BareProtocol=4;
const int YD_AF_DisableSelfTradeCheck=8;

// Margin and Premium Calculation Base Price Type
const int YD_CBT_PreSettlementPrice=0;
const int YD_CBT_OpenPrice=1;
const int YD_CBT_LastPrice=2;
const int YD_CBT_MarketAveragePrice=3;
const int YD_CBT_MaxLastPreSettlementPrice=4;
const int YD_CBT_OrderPrice=5;
const int YD_CBT_None=6;
const int YD_CBT_SamePrice=7;

// IDType in IDFromExchange
const int YD_IDT_NormalOrderSysID=0;
const int YD_IDT_QuoteDerivedOrderSysID=1;
const int YD_IDT_OptionExecuteOrderSysID=2;
const int YD_IDT_OptionAbandonExecuteOrderSysID=3;
const int YD_IDT_RequestForQuoteOrderSysID=4;
const int YD_IDT_CombPositionOrderSysID=5;
const int YD_IDT_TradeID=128;
const int YD_IDT_CombPositionDetailID=129;

// General Risk Param Types
const int YD_GRPT_OptionLongPositionCost=1;
const int YD_GRPT_TradePositionRatio=2;
const int YD_GRPT_OrderCancelRatio=3;

// get the opposite position direction
inline int oppositePositionDirection(int positiondir)
{
	return YD_PD_Long+YD_PD_Short-positiondir;
}

#endif
