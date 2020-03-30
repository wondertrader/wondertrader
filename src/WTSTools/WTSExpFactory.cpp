#include "WTSExpFactory.h"
#include "../Share/StrUtil.hpp"
#include "../Share/WTSExpressData.hpp"

USING_NS_OTP;

#ifdef _WIN32
#define my_stricmp _stricmp
#else
#define my_stricmp strcasecmp
#endif



double WTSExpFactory::EMA(double preData, double curValue, int nPeriod)
{
	double k=2.0/(nPeriod+1);

	return k*curValue + (1-k)*preData;
}

double WTSExpFactory::SMA(double preData, double curValue, int nParam)
{
	double result = curValue/nParam + preData*(nParam-1)/nParam;

	return result;
}

double WTSExpFactory::RSA(double dClose, double dMax, double dMin)
{
	double result = (dClose - dMin)*100.0/(dMax - dMin);

	return result;
}

double WTSExpFactory::MAX(WTSValueArray* ayData, int b, int e)
{
	if(b < 0)
		b = 0;

	if(b > e)
		return 0.0;

	double nMaxPrice = ayData->at(b);
	for(int i = b+1; i < e; i++)
	{
		nMaxPrice = max(nMaxPrice, ayData->at(i));
	}

	return nMaxPrice;
}

double WTSExpFactory::MIN(WTSValueArray* ayData, int b, int e)
{
	if(b < 0)
		b = 0;

	if(b > e)
		return 0.0;

	double nMinPrice = ayData->at(b);
	for(int i = b+1; i < e; i++)
	{
		nMinPrice = min(nMinPrice, ayData->at(i));
	}

	return nMinPrice;
}

WTSExpressData* WTSExpFactory::calcKlineExpress(const char* expName, WTSKlineData* klineData, WTSExpressParams* params)
{
	if(klineData == NULL)
		return NULL;

	WTSExpressData* pRet = NULL;
	if (my_stricmp(expName, "MA") == 0)
	{
		WTSValueArray* ayData = klineData->extractData(KFT_CLOSE);
		if(ayData)
		{
			pRet = calcExpressMA(ayData, params);
			pRet->setDevide(1.0);
			ayData->release();
		}
	}
	else if (my_stricmp(expName, "VOL") == 0)
	{
		WTSValueArray* ayData = klineData->extractData(KFT_SVOLUMN);
		if (ayData)
		{
			pRet = calcExpressVOL(ayData, params);
			pRet->setDevide(1.0);
			ayData->release();
		}
	}
	else if (my_stricmp(expName, "KDJ") == 0)
	{
		WTSValueArray* ayClose = klineData->extractData(KFT_CLOSE);
		WTSValueArray* ayHigh = klineData->extractData(KFT_HIGH);
		WTSValueArray* ayLow = klineData->extractData(KFT_LOW);
		if(ayClose)
		{
			pRet = calcExpressKDJ(ayClose, ayHigh, ayLow, params);
			pRet->setDevide(1.0);
			ayClose->release();
			ayHigh->release();
			ayLow->release();
		}
	}
	else if (my_stricmp(expName, "MACD") == 0)
	{
		WTSValueArray* ayData = klineData->extractData(KFT_CLOSE);
		if(ayData)
		{
			pRet = calcExpressMACD(ayData, params);
			pRet->setDevide(1.0);
			ayData->release();
		}
	}
	else if (my_stricmp(expName, "BIAS") == 0)
	{
		WTSValueArray* ayData = klineData->extractData(KFT_CLOSE);
		if(ayData)
		{
			pRet = calcExpressBIAS(ayData, params);
			pRet->setDevide(1.0);
			ayData->release();
		}
	}

	return pRet;
}

WTSExpressData* WTSExpFactory::calcExpressMA(WTSValueArray* ayData, WTSExpressParams* params)
{
	std::string strTitle = StrUtil::printf("MA(%d,%d,%d)", (*params)[0], (*params)[1], (*params)[2]);
	WTSExpressData* pRet = WTSExpressData::create(strTitle.c_str(), WET_SubExp);

	uint32_t sPeriod = (*params)[0];
	uint32_t mPeriod = (*params)[1];
	uint32_t lPeriod = (*params)[2];

	uint32_t nCount = ayData?ayData->size():0;
	
	WTSExpressLine* sline = WTSExpressLine::create(nCount, WELT_Polyline, 2); 
	WTSExpressLine* mline = WTSExpressLine::create(nCount, WELT_Polyline, 2);
	WTSExpressLine* lline = WTSExpressLine::create(nCount, WELT_Polyline, 2);

	sline->setLineTag(StrUtil::printf("MA%u", sPeriod).c_str());
	mline->setLineTag(StrUtil::printf("MA%u", mPeriod).c_str());
	lline->setLineTag(StrUtil::printf("MA%u", lPeriod).c_str());

	double sSum = 0;
	double mSum = 0;
	double lSum = 0;

	for(uint32_t i = 0; i < nCount; i++)
	{
		sSum += ayData->at(i);
		mSum += ayData->at(i);
		lSum += ayData->at(i);

		//短周期
		if(i >= sPeriod)
		{
			sSum -= ayData->at(i-sPeriod);
		}
		if(i >= sPeriod - 1)
		{
			(*sline)[i] = sSum/sPeriod;
		}

		//中周期
		if(i >= mPeriod)
		{
			mSum -= ayData->at(i-mPeriod);
		}
		if(i >= mPeriod - 1)
		{
			//mline->appendValue(mSum/mPeriod);
			(*mline)[i] = mSum/mPeriod;
		}

		//长周期
		if(i >= lPeriod)
		{
			lSum -= ayData->at(i-lPeriod);
		}
		if(i >= lPeriod - 1)
		{
			//lline->appendValue(lSum/lPeriod);
			(*lline)[i] = lSum/lPeriod;
		}
	}

	pRet->addExpLine(sline);
	pRet->addExpLine(mline);
	pRet->addExpLine(lline);

	return pRet;
}

WTSExpressData* WTSExpFactory::calcExpressVOL(WTSValueArray* ayData, WTSExpressParams* params, bool isTrend /* = false */)
{
	std::string strTitle = "VOL";
	if (params)
		strTitle = StrUtil::printf("VOL(%d,%d,%d)", (*params)[0], (*params)[1], (*params)[2]);
	WTSExpressData* pRet = WTSExpressData::create(strTitle.c_str());
	pRet->setPrecision(0);

	uint32_t sPeriod = 0, mPeriod = 0, lPeriod = 0;
	if (!isTrend)
	{
		sPeriod = (*params)[0];
		mPeriod = (*params)[1];
		lPeriod = (*params)[2];
	}

	uint32_t nCount = ayData ? ayData->size() : 0;

	WTSExpressLine* vline = WTSExpressLine::create(nCount, isTrend ? WELT_AStickLine : WELT_VolStick);
	vline->setLineTag("VOL");

	WTSExpressLine *sline = NULL, *mline = NULL, *lline = NULL;

	if (!isTrend)
	{
		sline = WTSExpressLine::create(nCount);
		mline = WTSExpressLine::create(nCount);
		lline = WTSExpressLine::create(nCount);

		sline->setLineTag(StrUtil::printf("MA%u", sPeriod).c_str());
		mline->setLineTag(StrUtil::printf("MA%u", mPeriod).c_str());
		lline->setLineTag(StrUtil::printf("MA%u", lPeriod).c_str());
	}	

	double sSum = 0;
	double mSum = 0;
	double lSum = 0;

	for(uint32_t i = 0; i < nCount; i++)
	{
		double curVal = ayData->at(i);
		double absVal = abs(curVal);

		(*vline)[i] = curVal;

		if (!isTrend)
		{
			sSum += absVal;
			mSum += absVal;
			lSum += absVal;

			//短周期
			if (i >= sPeriod)
			{
				sSum -= abs(ayData->at(i - sPeriod));
			}
			if (i >= sPeriod - 1)
			{
				(*sline)[i] = sSum / sPeriod;
			}

			//中周期
			if (i >= mPeriod)
			{
				mSum -= abs(ayData->at(i - mPeriod));
			}
			if (i >= mPeriod - 1)
			{
				//mline->appendValue(mSum/mPeriod);
				(*mline)[i] = mSum / mPeriod;
			}

			//长周期
			if (i >= lPeriod)
			{
				lSum -= abs(ayData->at(i - lPeriod));
			}
			if (i >= lPeriod - 1)
			{
				//lline->appendValue(lSum/lPeriod);
				(*lline)[i] = lSum / lPeriod;
			}
		}		
	}
	pRet->addExpLine(vline);

	if(!isTrend)
	{
		pRet->addExpLine(sline);
		pRet->addExpLine(mline);
		pRet->addExpLine(lline);
	}

	return pRet;
}

WTSExpressData* WTSExpFactory::calcExpressKDJ(WTSValueArray* ayClose, WTSValueArray* ayHigh, WTSValueArray* ayLow, WTSExpressParams* params)
{
	std::string strTitle = StrUtil::printf("KDJ(%d,%d,%d)", (*params)[0], (*params)[1], (*params)[2]);
	WTSExpressData* pRet = WTSExpressData::create(strTitle.c_str());
	pRet->setPrecision(2);

	uint32_t n = (*params)[0];
	uint32_t m1 = (*params)[1];
	uint32_t m2 = (*params)[2];

	uint32_t nCount = ayClose ? ayClose->size() : 0;

	WTSExpressLine* lineK = WTSExpressLine::create(nCount, WELT_Polyline, 2); 
	WTSExpressLine* lineD = WTSExpressLine::create(nCount, WELT_Polyline, 2);
	WTSExpressLine* lineJ = WTSExpressLine::create(nCount, WELT_Polyline, 2);

	lineK->setLineTag("K");
	lineD->setLineTag("D");
	lineJ->setLineTag("J");

	for(uint32_t i = 0; i < nCount; i++)
	{
		double k = 50.0;
		double d = 50.0;
		double j = 50.0;

		if(i != 0){

			//(N日收盘价 - N日内最低价) ÷ (N日内最高价 - N日内最低价) × 100 = N日RSV
			double rsa = RSA(
				ayClose->at(i), 
				MAX(ayHigh, i-n, i), 
				MIN(ayLow, i-n, i));

			//当日K值 = 1/3当日RSV + 2/3前1日K值 ;
			k = SMA(rsa, (*lineK)[i-1], m1);	
			//当日D值 = 2/3前1日D值 + 1/3当日K值;
			d = SMA(k, (*lineD)[i-1], m2);
			//当日J值 = 3当日K值 - 2当日D值
			j = 3*k - 2*d;
		}

		(*lineK)[i] = k;
		(*lineD)[i] = d;
		(*lineJ)[i] = j;
	}

	pRet->addExpLine(lineK);
	pRet->addExpLine(lineD);
	pRet->addExpLine(lineJ);

	return pRet;
}

WTSExpressData* WTSExpFactory::calcExpressMACD(WTSValueArray* ayData, WTSExpressParams* params)
{
	std::string strTitle = StrUtil::printf("MACD(%d,%d,%d)", (*params)[0], (*params)[1], (*params)[2]);
	WTSExpressData* pRet = WTSExpressData::create(strTitle.c_str());
	pRet->setPrecision(3);
	pRet->setBaseLine(true, 0.0);

	uint32_t sPeriod = (*params)[0];
	uint32_t mPeriod = (*params)[1];
	uint32_t lPeriod = (*params)[2];

	uint32_t nCount = ayData ? ayData->size() : 0;

	//WTSExpressLine* bline = WTSExpressLine::create(0, WELT_Polyline, 0, ELS_LINE_VISIBLE); 
	//bline->resize(nCount, 0.0);
	WTSExpressLine* diff = WTSExpressLine::create(nCount, WELT_StickLine, 3); 
	WTSExpressLine* dea = WTSExpressLine::create(nCount, WELT_Polyline, 3);
	WTSExpressLine* macd = WTSExpressLine::create(nCount, WELT_Polyline, 3);

	diff->setLineTag("DIFF");
	dea->setLineTag("DEA");
	macd->setLineTag("MACD");

	double preEMA1 = ayData->at(0);
	double preEMA2 = ayData->at(0);
	double preDEA = 0.0;

	for(uint32_t i = 0; i < nCount; i++)
	{
		double curValue = ayData->at(i);

		//短期EMA
		double curEMA1 = EMA(preEMA1, curValue, sPeriod);
		preEMA1 = curEMA1;

		//长期EMA
		double curEMA2 = EMA(preEMA2, curValue, lPeriod);
		preEMA2 = curEMA2;

		double curDiff = curEMA1 - curEMA2;
		(*diff)[i] = curDiff;

		double curDEA = EMA(preDEA, curDiff, mPeriod);
		(*dea)[i] = curDEA;
		preDEA = curDEA;

		double curMACD = 2*(curDiff - curDEA);
		(*macd)[i] = curMACD;
	}

	//pRet->addExpLine(bline);
	pRet->addExpLine(diff);
	pRet->addExpLine(dea);
	pRet->addExpLine(macd);

	return pRet;
}

WTSExpressData* WTSExpFactory::calcExpressBIAS(WTSValueArray* ayData, WTSExpressParams* params)
{
	std::string strTitle = StrUtil::printf("BIAS(%d,%d,%d)", (*params)[0], (*params)[1], (*params)[2]);
	WTSExpressData* pRet = WTSExpressData::create(strTitle.c_str());
	pRet->setPrecision(2);

	uint32_t sPeriod = (*params)[0];
	uint32_t mPeriod = (*params)[1];
	uint32_t lPeriod = (*params)[2];

	uint32_t nCount = ayData ? ayData->size() : 0;

	WTSExpressLine* lineB1 = WTSExpressLine::create(nCount, WELT_Polyline, 2); 
	WTSExpressLine* lineB2 = WTSExpressLine::create(nCount, WELT_Polyline, 2);
	WTSExpressLine* lineB3 = WTSExpressLine::create(nCount, WELT_Polyline, 2);

	lineB1->setLineTag(StrUtil::printf("B%u", sPeriod).c_str());
	lineB2->setLineTag(StrUtil::printf("B%u", mPeriod).c_str());
	lineB3->setLineTag(StrUtil::printf("B%u", lPeriod).c_str());

	double sSum = 0;
	double mSum = 0;
	double lSum = 0;

	for(uint32_t i = 0; i < nCount; i++)
	{
		double curClose = ayData->at(i);
		sSum += curClose;
		mSum += curClose;
		lSum += curClose;

		//短周期
		if(i >= sPeriod)
		{
			sSum -= ayData->at(i-sPeriod);
		}

		if(i >= sPeriod - 1)
		{
			(*lineB1)[i] = (curClose - sSum/sPeriod)*100/(sSum/sPeriod);
		}

		//中周期
		if(i >= mPeriod)
		{
			mSum -= ayData->at(i-mPeriod);
		}
		if(i >= mPeriod - 1)
		{
			(*lineB2)[i] = (curClose - mSum/mPeriod)*100/(mSum/mPeriod);
		}

		//长周期
		if(i >= lPeriod)
		{
			lSum -= ayData->at(i-lPeriod);
		}

		if(i >= lPeriod - 1)
		{
			(*lineB3)[i] = (curClose - lSum/lPeriod)*100/(lSum/lPeriod);
		}
	}

	pRet->addExpLine(lineB1);
	pRet->addExpLine(lineB2);
	pRet->addExpLine(lineB3);

	return pRet;
}

WTSExpressData* WTSExpFactory::makeEmptyExpress(const char* expName, WTSExpressParams* params)
{
	WTSExpressData* pRet = NULL;
	if (my_stricmp(expName, "MA") == 0)
	{
		pRet = calcExpressMA(NULL, params);
		pRet->setDevide(1.0);
	}
	else if (my_stricmp(expName, "VOL") == 0)
	{
		pRet = calcExpressVOL(NULL, params);
		pRet->setDevide(1.0);
	}
	else if (my_stricmp(expName, "KDJ") == 0)
	{
		pRet = calcExpressKDJ(NULL, NULL, NULL, params);
		pRet->setDevide(1.0);
	}
	else if (my_stricmp(expName, "MACD") == 0)
	{
		pRet = calcExpressMACD(NULL, params);
		pRet->setDevide(1.0);
	}
	else if (my_stricmp(expName, "BIAS") == 0)
	{
		pRet = calcExpressBIAS(NULL, params);
		pRet->setDevide(1.0);
	}

	return pRet;
}