/*!
 * \file WTSDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据拼接工厂类定义
 */
#pragma once
#include "../Includes/IDataFactory.h"

USING_NS_OTP;

class WTSDataFactory : public IDataFactory
{
public:
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo);
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo);

	virtual WTSKlineData*	extractKlineData(WTSKlineSlice* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);

	virtual WTSKlineData*	extractKlineData(WTSTickSlice* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false);

	virtual bool			mergeKlineData(WTSKlineData* klineData, WTSKlineData* newKline);

protected:
	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateDayData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateSecData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);

	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar);

	WTSKlineData* extractMin1Data(WTSKlineSlice* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);
	WTSKlineData* extractMin5Data(WTSKlineSlice* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);
	WTSKlineData* extractDayData(WTSKlineSlice* baseKline, uint32_t times, bool bIncludeOpen = true);

protected:
	static uint32_t getPrevMinute(uint32_t curMinute, int period = 1);
};

