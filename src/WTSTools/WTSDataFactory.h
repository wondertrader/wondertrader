#pragma once
#include "../Share/IDataFactory.h"

USING_NS_OTP;

class WTSDataFactory : public IDataFactory
{
public:
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo);
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo);

	virtual WTSKlineData*	extractKlineData(WTSKlineData* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);

	virtual WTSKlineData*	extractKlineData(WTSArray* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false);

	virtual bool			mergeKlineData(WTSKlineData* klineData, WTSKlineData* newKline);

protected:
	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateDayData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateSecData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);

	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar);

	WTSKlineData* extractMin1Data(WTSKlineData* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);
	WTSKlineData* extractMin5Data(WTSKlineData* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);
	WTSKlineData* extractDayData(WTSKlineData* baseKline, uint32_t times, bool bIncludeOpen = true);

protected:
	static uint32_t getPrevMinute(uint32_t curMinute, int period = 1);
};

