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
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	利用基础周期K线数据更新K线
	 *	@klineData		K线数据
	 *	@newBasicBar	基础周期K线数据
	 *	@sInfo			交易时间模板
	 */
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo, bool bAlignSec = false);

	/*
	 *	从基础周期K线数据提取非基础周期的K线数据
	 *	@baseKline	基础周期K线
	 *	@period		基础周期，m1/m5/day
	 *	@times		周期倍数
	 *	@sInfo		交易时间模板
	 *	@bIncludeOpen	是否包含未闭合的K线
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual WTSKlineData*	extractKlineData(WTSKlineSlice* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);

	/*
	 *	从tick数据提取秒周期的K线数据
	 *	@ayTicks	tick数据
	 *	@seconds	目标周期
	 *	@sInfo		交易时间模板
	 *	@bUnixTime	tick时间戳是否是unixtime
	 *	@bAlignSec	是否按小节对齐
	 */
	virtual WTSKlineData*	extractKlineData(WTSTickSlice* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false, bool bAlignSec = false);

	/*
	 *	合并K线
	 *	@klineData	目标K线
	 *	@newKline	待合并的K线
	 */
	virtual bool			mergeKlineData(WTSKlineData* klineData, WTSKlineData* newKline);

protected:
	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick, bool bAlignSec = false);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick, bool bAlignSec = false);
	WTSBarStruct* updateDayData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);
	WTSBarStruct* updateSecData(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSTickData* tick);

	WTSBarStruct* updateMin1Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar, bool bAlignSec = false);
	WTSBarStruct* updateMin5Data(WTSSessionInfo* sInfo, WTSKlineData* klineData, WTSBarStruct* newBasicBar, bool bAlignSec = false);

	WTSKlineData* extractMin1Data(WTSKlineSlice* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	WTSKlineData* extractMin5Data(WTSKlineSlice* baseKline, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true, bool bAlignSec = false);
	WTSKlineData* extractDayData(WTSKlineSlice* baseKline, uint32_t times, bool bIncludeOpen = true);

protected:
	static uint32_t getPrevMinute(uint32_t curMinute, int period = 1);
};

