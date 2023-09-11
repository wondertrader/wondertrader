/*!
 * \file IExpFactory.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 指标工接口定义
 */
#pragma once
#include "WTSMarcos.h"

NS_WTP_BEGIN
class WTSKlineData;
class WTSExpressParams;
class WTSExpressData;
class WTSHisTrendData;

class IExpFactory
{
public:
	virtual WTSExpressData *calcKlineExpress(const char* expName, WTSKlineData* klineData, WTSExpressParams* params) = 0;
	virtual WTSExpressData *calcTrendExpress(const char* expName, WTSHisTrendData* trendData, WTSExpressParams* params) = 0;
};
NS_WTP_END