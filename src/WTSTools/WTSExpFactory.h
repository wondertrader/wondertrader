#pragma once
#include "../Share/IExpFactory.h"

NS_OTP_BEGIN
class WTSValueArray;
class IExpFactory;
NS_OTP_END

USING_NS_OTP;

class WTSExpFactory : public IExpFactory
{
public:
	static double EMA(double preData, double curValue, int nPeriod);
	static double SMA(double preData, double curValue, int nParam);
	static double RSA(double dClose, double dMax, double dMin);

	static double MAX(WTSValueArray* ayData, int iStart, int iEnd);
	static double MIN(WTSValueArray* ayData, int iStart, int iEnd);

private:
	WTSExpressData *calcExpressMA(WTSValueArray* ayData, WTSExpressParams* params);
	WTSExpressData *calcExpressVOL(WTSValueArray* ayData, WTSExpressParams* params, bool isTrend = false);
	WTSExpressData *calcExpressMACD(WTSValueArray* ayData, WTSExpressParams* params);
	WTSExpressData *calcExpressBIAS(WTSValueArray* ayData, WTSExpressParams* params);
	WTSExpressData *calcExpressKDJ(WTSValueArray* ayClose, WTSValueArray* ayHigh, WTSValueArray* ayLow, WTSExpressParams* params);

public:
	WTSExpressData*	makeEmptyExpress(const char* expName,  WTSExpressParams* params);

public:
	virtual WTSExpressData *calcKlineExpress(const char* expName, WTSKlineData* klineData, WTSExpressParams* params) override;
}; 