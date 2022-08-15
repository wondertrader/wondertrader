#pragma once
#include "IAresCltStruct.h"

class IAresCltSpi
{
public:
	//通知：市场日期和时间
	virtual		void			OnMarketTime(AClt_Market cMarket, tagAClt_MarketField*) = 0;
	//通知：行情快照
	virtual		void			OnMarketData(AClt_Market cMarket, tagAClt_QuoteField*) = 0;

};

class IAresExchange
{
public:
	// 取交易所商品码表个数
	virtual		int				GetCommodityCount() = 0;
	// 取交易所商品码表
	virtual		int				GetCommodityData(tagAClt_Instrument* pArr, int nCount) = 0;
	// 取某一商品基础数据
	virtual		int				GetOneStaticData(tagAClt_Instrument* pInstrument, tagAClt_CommBaseData* pData) = 0;
	// 取交易所商品基础数据
	virtual		int				GetStaticData(tagAClt_CommBaseData* pArr, int nCount) = 0;

	// 取某一商品的补充数据(1.14+)
	virtual		int				GetOneSupplementData(tagAClt_Instrument* pInstrument, tagAClt_SupplementData* pData) = 0;
	// 取交易所商品补充数据(1.14+)
	virtual		int				GetSupplementData(tagAClt_SupplementData* pArr, int nCount) = 0;

};

class IAresCltApi
{
public:
	//注册行情通知Spi
	virtual		void			RegisterSpi(IAresCltSpi*) = 0;
	//启动工作
	virtual		int				StartWork() = 0;
	//停止工作
	virtual		void			EndWork() = 0;

public:
	//获取市场列表
	virtual	 IAresExchange*		GetExchPtr(AClt_Market cMarket) = 0;

};


























