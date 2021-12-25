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

USING_NS_WTP;

class WTSDataFactory : public IDataFactory
{
public:
	/*
	 *	利用tick数据更新K线
	 *	@klineData	K线数据
	 *	@tick		tick数据
	 *	@sInfo		交易时间模板
	 */
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo);

	/*
	 *	利用基础周期K线数据更新K线
	 *	@klineData		K线数据
	 *	@newBasicBar	基础周期K线数据
	 *	@sInfo			交易时间模板
	 */
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo);

	/*
	 *	从基础周期K线数据提取非基础周期的K线数据
	 *	@baseKline	基础周期K线
	 *	@period		基础周期，m1/m5/day
	 *	@times		周期倍数
	 *	@sInfo		交易时间模板
	 *	@bIncludeOpen	是否包含未闭合的K线
	 */
	virtual WTSKlineData*	extractKlineData(WTSKlineSlice* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true);

	/*
	 *	从tick数据提取秒周期的K线数据
	 *	@ayTicks	tick数据
	 *	@seconds	目标周期
	 *	@sInfo		交易时间模板
	 *	@bUnixTime	tick时间戳是否是unixtime
	 */
	virtual WTSKlineData*	extractKlineData(WTSTickSlice* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false);

	/*
	 *	合并K线
	 *	@klineData	目标K线
	 *	@newKline	待合并的K线
	 */
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

