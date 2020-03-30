/*!
 * \file IDataFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据拼接工厂接口定义
 */
#pragma once
#include <stdint.h>
#include "../Share/WTSTypes.h"

//USING_NS_OTP;

NS_OTP_BEGIN
class WTSKlineData;
class WTSHisTrendData;
class WTSTickData;
class WTSSessionInfo;
class WTSArray;
class WTSContractInfo;
struct WTSBarStruct;

/*
 *	数据工厂
 *	主要用于各种数据的拼接
 *	这里只定义一个接口
 */
class IDataFactory
{
public:
	/*
	 *	更新K线数据
	 *	@klineData 要更新的K线数据
	 *	@tick 最新的tick数据
	 *	返回值 是否有新的K线生成
	 */
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSTickData* tick, WTSSessionInfo* sInfo)						= 0;
	virtual WTSBarStruct*	updateKlineData(WTSKlineData* klineData, WTSBarStruct* newBasicBar, WTSSessionInfo* sInfo)				= 0;

	virtual WTSKlineData*	extractKlineData(WTSKlineData* baseKline, WTSKlinePeriod period, uint32_t times, WTSSessionInfo* sInfo, bool bIncludeOpen = true)	= 0;

	virtual WTSKlineData*	extractKlineData(WTSArray* ayTicks, uint32_t seconds, WTSSessionInfo* sInfo, bool bUnixTime = false)	= 0;

	virtual bool			mergeKlineData(WTSKlineData* klineData, WTSKlineData* newKline)											= 0;

};

NS_OTP_END
