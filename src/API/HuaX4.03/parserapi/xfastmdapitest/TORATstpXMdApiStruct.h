/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpXMdApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
/////////////////////////////////////////////////////////////////////////
#ifndef TORATSTPXMDAPISTRUCT_H__
#define TORATSTPXMDAPISTRUCT_H__

#include "TORATstpXMdApiDataType.h"

namespace TORALEV1API
{
	
	/// 登录请求
	struct CTORATstpReqUserLoginField
	{
		///登录账户
		TTORATstpLogInAccountType	LogInAccount;
	
		///登录账户类型
		TTORATstpLogInAccountTypeType	LogInAccountType;
	
		///经纪公司部门代码（以资金账号方式登录时必填）
		TTORATstpDepartmentIDType	DepartmentID;
	
		///认证方式(指纹或钥匙串认证时必填)
		TTORATstpAuthModeType	AuthMode;
	
		///密码(密码认证时必填)
		TTORATstpPasswordType	Password;
	
		///用户端产品信息
		TTORATstpUserProductInfoType	UserProductInfo;
	
		///接口端产品信息
		TTORATstpInterfaceProductInfoType	InterfaceProductInfo;
	
		///终端采集信息
		TTORATstpTerminalInfoType	TerminalInfo;
	
		///内网IP地址
		TTORATstpIPAddressType	InnerIPAddress;
	
		///外网IP地址
		TTORATstpIPAddressType	OuterIPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	
		///接口语言
		TTORATstpLangType	Lang;
	
		///终端动态秘钥
		TTORATstpPasswordType	DynamicPassword;
	
		///设备标识(指纹认证时必填)
		TTORATstpDeviceIDType	DeviceID;
	
		///认证序列号(指纹或钥匙串认证时必填)
		TTORATstpCertSerialType	CertSerial;
	
		///设备类别(指纹或钥匙串认证时必填)
		TTORATstpDeviceTypeType	DeviceType;
	};
	
	/// 登录响应
	struct CTORATstpRspUserLoginField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///登录账户
		TTORATstpLogInAccountType	LogInAccount;
	
		///登录账户类型
		TTORATstpLogInAccountTypeType	LogInAccountType;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///最大报单引用
		TTORATstpOrderRefType	MaxOrderRef;
	
		///私有流长度
		TTORATstpVolumeType	PrivateFlowCount;
	
		///公有流长度
		TTORATstpVolumeType	PublicFlowCount;
	
		///登录时间
		TTORATstpTimeType	LoginTime;
	
		///交易系统名称
		TTORATstpSystemNameType	SystemName;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///用户名称
		TTORATstpUserNameType	UserName;
	
		///用户类型
		TTORATstpUserTypeType	UserType;
	
		///报单流控
		TTORATstpCommFluxType	OrderInsertCommFlux;
	
		///撤单流控
		TTORATstpCommFluxType	OrderActionCommFlux;
	
		///密码到期日期
		TTORATstpDateType	PasswordExpiryDate;
	
		///是否需要改密
		TTORATstpBoolType	NeedUpdatePassword;
	
		///认证序列号
		TTORATstpCertSerialType	CertSerial;
	
		///内网IP地址
		TTORATstpIPAddressType	InnerIPAddress;
	
		///外网IP地址
		TTORATstpIPAddressType	OuterIPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 响应信息
	struct CTORATstpRspInfoField
	{
		///错误代码
		TTORATstpErrorIDType	ErrorID;
	
		///错误信息
		TTORATstpErrorMsgType	ErrorMsg;
	};
	
	/// 用户登出
	struct CTORATstpUserLogoutField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	};
	
	/// 订阅行情
	struct CTORATstpSpecificSecurityField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 订阅市场
	struct CTORATstpSpecificMarketField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	};
	
	/// 查询行情快照
	struct CTORATstpInquiryMarketDataField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 深度行情通知
	struct CTORATstpMarketDataField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///昨收盘价
		TTORATstpPriceType	PreClosePrice;
	
		///今开盘价
		TTORATstpPriceType	OpenPrice;
	
		///成交量
		TTORATstpLongVolumeType	Volume;
	
		///成交额
		TTORATstpMoneyType	Turnover;
	
		///成交笔数
		TTORATstpLongVolumeType	TradingCount;
	
		///最新价
		TTORATstpPriceType	LastPrice;
	
		///最高价
		TTORATstpPriceType	HighestPrice;
	
		///最低价
		TTORATstpPriceType	LowestPrice;
	
		///买1价
		TTORATstpPriceType	BidPrice1;
	
		///卖1价
		TTORATstpPriceType	AskPrice1;
	
		///涨停价
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停价
		TTORATstpPriceType	LowerLimitPrice;
	
		///市盈率1
		TTORATstpRatioType	PERatio1;
	
		///市盈率2
		TTORATstpRatioType	PERatio2;
	
		///价格升跌1
		TTORATstpPriceType	PriceUpDown1;
	
		///价格升跌2
		TTORATstpPriceType	PriceUpDown2;
	
		///持仓量
		TTORATstpLargeVolumeType	OpenInterest;
	
		///买1量
		TTORATstpLongVolumeType	BidVolume1;
	
		///卖1量
		TTORATstpLongVolumeType	AskVolume1;
	
		///买2价
		TTORATstpPriceType	BidPrice2;
	
		///买2量
		TTORATstpLongVolumeType	BidVolume2;
	
		///卖2价
		TTORATstpPriceType	AskPrice2;
	
		///卖2量
		TTORATstpLongVolumeType	AskVolume2;
	
		///买3价
		TTORATstpPriceType	BidPrice3;
	
		///买3量
		TTORATstpLongVolumeType	BidVolume3;
	
		///卖3价
		TTORATstpPriceType	AskPrice3;
	
		///卖3量
		TTORATstpLongVolumeType	AskVolume3;
	
		///买4价
		TTORATstpPriceType	BidPrice4;
	
		///买4量
		TTORATstpLongVolumeType	BidVolume4;
	
		///卖4价
		TTORATstpPriceType	AskPrice4;
	
		///卖4量
		TTORATstpLongVolumeType	AskVolume4;
	
		///买5价
		TTORATstpPriceType	BidPrice5;
	
		///买5量
		TTORATstpLongVolumeType	BidVolume5;
	
		///卖5价
		TTORATstpPriceType	AskPrice5;
	
		///卖5量
		TTORATstpLongVolumeType	AskVolume5;
	
		///更新时间
		TTORATstpTimeType	UpdateTime;
	
		///更新毫秒
		TTORATstpMillisecType	UpdateMillisec;
	
		///今收盘价
		TTORATstpPriceType	ClosePrice;
	
		///今结算价
		TTORATstpPriceType	SettlementPrice;
	
		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;
	
		///警示级别
		TTORATstpHWLevelType	HWLevel;
	
		///基金T-1日收盘时刻净值
		TTORATstpPriceType	PreCloseIOPV;
	
		///基金实时参考净值
		TTORATstpPriceType	IOPV;
	};
	
	/// 查询响应域
	struct CTORATstpQryRspInfoField
	{
		///结束标识
		TTORATstpEndFlagType	EndFlag;
	
		///错误代码
		TTORATstpErrorIDType	ErrorID;
	
		///错误信息
		TTORATstpErrorMsgType	ErrorMsg;
	};
	
	/// 盘后行情通知
	struct CTORATstpPHMarketDataField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///成交量
		TTORATstpLongVolumeType	Volume;
	
		///成交金额
		TTORATstpMoneyType	Turnover;
	
		///今收盘价
		TTORATstpPriceType	ClosePrice;
	
		///涨停价
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停价
		TTORATstpPriceType	LowerLimitPrice;
	
		///买入申报数量
		TTORATstpLongVolumeType	BidVolume;
	
		///卖出申报数量
		TTORATstpLongVolumeType	AskVolume;
	
		///更新时间
		TTORATstpTimeType	UpdateTime;
	
		///更新毫秒
		TTORATstpMillisecType	UpdateMillisec;
	
		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;
	
		///警示级别
		TTORATstpHWLevelType	HWLevel;
	};
	
	/// 查询特定行情快照
	struct CTORATstpInquirySpecialMarketDataField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 特别行情通知
	struct CTORATstpSpecialMarketDataField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///移动平均价
		TTORATstpPriceType	MovingAvgPrice;
	
		///采样数量
		TTORATstpVolumeType	MovingAvgPriceSamplingNum;
	
		///最后修改时间
		TTORATstpTimeType	UpdateTime;
	
		///最后修改毫秒
		TTORATstpMillisecType	UpdateMillisec;
	};
	
	/// 简易行情
	struct CTORATstpSimplifyMarketDataField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///昨收盘价
		TTORATstpPriceType	PreClosePrice;
	
		///最新价
		TTORATstpPriceType	LastPrice;
	
		///买1价
		TTORATstpPriceType	BidPrice1;
	
		///卖1价
		TTORATstpPriceType	AskPrice1;
	
		///最高价
		TTORATstpPriceType	HighestPrice;
	
		///最低价
		TTORATstpPriceType	LowestPrice;
	
		///涨停价
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停价
		TTORATstpPriceType	LowerLimitPrice;
	
		///今收盘价
		TTORATstpPriceType	ClosePrice;
	
		///今结算价
		TTORATstpPriceType	SettlementPrice;
	
		///更新时间
		TTORATstpTimeType	UpdateTime;
	};
	
	/// 合约状态
	struct CTORATstpSecurityStatusField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///是否停牌
		TTORATstpBoolType	IsSuspend;
	
		///是否波动性中断
		TTORATstpBoolType	IsBreak;
	
		///是否长期停牌
		TTORATstpBoolType	IsLongSuspend;
	
		///是否熔断
		TTORATstpBoolType	IsCircuitBreak;
	
		///是否可融资买入
		TTORATstpBoolType	IsSupportMarginBuy;
	
		///是否可融券卖出
		TTORATstpBoolType	IsSupportShortSell;
	
		///是否可申购
		TTORATstpBoolType	IsSupportPur;
	
		///是否可赎回
		TTORATstpBoolType	IsSupportRed;
	
		///是否可拆分
		TTORATstpBoolType	IsSupportSplit;
	
		///是否可合并
		TTORATstpBoolType	IsSupportMerge;
	
		///是否可质押入库
		TTORATstpBoolType	IsSupportPleadgeIn;
	
		///是否可质押出库
		TTORATstpBoolType	IsSupportPleadgeOut;
	
		///是否可整手买
		TTORATstpBoolType	IsSupportRoundLotBuy;
	
		///是否可整手卖
		TTORATstpBoolType	IsSupportRoundLotSell;
	
		///是否可零股买
		TTORATstpBoolType	IsSupportOddLotBuy;
	
		///是否可零股卖
		TTORATstpBoolType	IsSupportOddLotSell;
	
		///是否可行权
		TTORATstpBoolType	IsSupportExercise;
	
		///是否限制买入开仓
		TTORATstpBoolType	IsLimitBuy;
	
		///是否限制卖出开仓
		TTORATstpBoolType	IsLimitSell;
	
		///是否限制备兑开仓
		TTORATstpBoolType	IsLimitCover;
	
		///是否限制做市商报价
		TTORATstpBoolType	IsLimitMarketMaker;
	};
	
	/// 市场状态
	struct CTORATstpMarketStatusField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///市场状态
		TTORATstpMarketStatusType	MarketStatus;
	};
	
	/// 深港通国际市场互联状态信息
	struct CTORATstpImcParamsField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///是否开放
		TTORATstpBoolType	OpenFlag;
	
		///初始额度
		TTORATstpMoneyType	ThresholdAmount;
	
		///日中剩余额度
		TTORATstpMoneyType	PosAmt;
	
		///额度是否可用
		TTORATstpBoolType	AmountStatus;
	};
	
	/// 极速行情快照
	struct CTORATstpRapidMarketDataField
	{
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///时间戳
		TTORATstpTimeStampType	DataTimeStamp;
	
		///昨收盘
		TTORATstpPriceType	PreClosePrice;
	
		///今开盘
		TTORATstpPriceType	OpenPrice;
	
		///成交笔数
		TTORATstpLongVolumeType	NumTrades;
	
		///成交总量
		TTORATstpLongVolumeType	TotalVolumeTrade;
	
		///成交总金额
		TTORATstpMoneyType	TotalValueTrade;
	
		///最高价
		TTORATstpPriceType	HighestPrice;
	
		///最低价
		TTORATstpPriceType	LowestPrice;
	
		///现价
		TTORATstpPriceType	LastPrice;
	
		///申买价一
		TTORATstpPriceType	BidPrice1;
	
		///申买量一
		TTORATstpLongVolumeType	BidVolume1;
	
		///申卖价一
		TTORATstpPriceType	AskPrice1;
	
		///申卖量一
		TTORATstpLongVolumeType	AskVolume1;
	
		///申卖价二
		TTORATstpPriceType	AskPrice2;
	
		///申卖量二
		TTORATstpLongVolumeType	AskVolume2;
	
		///申卖价三
		TTORATstpPriceType	AskPrice3;
	
		///申卖量三
		TTORATstpLongVolumeType	AskVolume3;
	
		///申买价二
		TTORATstpPriceType	BidPrice2;
	
		///申买量二
		TTORATstpLongVolumeType	BidVolume2;
	
		///申买价三
		TTORATstpPriceType	BidPrice3;
	
		///申买量三
		TTORATstpLongVolumeType	BidVolume3;
	
		///申卖价四
		TTORATstpPriceType	AskPrice4;
	
		///申卖量四
		TTORATstpLongVolumeType	AskVolume4;
	
		///申卖价五
		TTORATstpPriceType	AskPrice5;
	
		///申卖量五
		TTORATstpLongVolumeType	AskVolume5;
	
		///申买价四
		TTORATstpPriceType	BidPrice4;
	
		///申买量四
		TTORATstpLongVolumeType	BidVolume4;
	
		///申买价五
		TTORATstpPriceType	BidPrice5;
	
		///申买量五
		TTORATstpLongVolumeType	BidVolume5;
	
		///申卖价六
		TTORATstpPriceType	AskPrice6;
	
		///申卖量六
		TTORATstpLongVolumeType	AskVolume6;
	
		///申卖价七
		TTORATstpPriceType	AskPrice7;
	
		///申卖量七
		TTORATstpLongVolumeType	AskVolume7;
	
		///申买价六
		TTORATstpPriceType	BidPrice6;
	
		///申买量六
		TTORATstpLongVolumeType	BidVolume6;
	
		///申买价七
		TTORATstpPriceType	BidPrice7;
	
		///申买量七
		TTORATstpLongVolumeType	BidVolume7;
	
		///申卖价八
		TTORATstpPriceType	AskPrice8;
	
		///申卖量八
		TTORATstpLongVolumeType	AskVolume8;
	
		///申卖价九
		TTORATstpPriceType	AskPrice9;
	
		///申卖量九
		TTORATstpLongVolumeType	AskVolume9;
	
		///申买价八
		TTORATstpPriceType	BidPrice8;
	
		///申买量八
		TTORATstpLongVolumeType	BidVolume8;
	
		///申买价九
		TTORATstpPriceType	BidPrice9;
	
		///申买量九
		TTORATstpLongVolumeType	BidVolume9;
	
		///申买价十
		TTORATstpPriceType	BidPrice10;
	
		///申买量十
		TTORATstpLongVolumeType	BidVolume10;
	
		///申卖价十
		TTORATstpPriceType	AskPrice10;
	
		///申卖量十
		TTORATstpLongVolumeType	AskVolume10;
	
		///涨停板价(只有深圳行情有效)
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停板价(只有深圳行情有效)
		TTORATstpPriceType	LowerLimitPrice;
	
		///今收盘价(只有上海行情有效)
		TTORATstpPriceType	ClosePrice;
	
		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;
	
		///基金实时参考净值
		TTORATstpPriceType	IOPV;
	
		///内盘
		TTORATstpLongVolumeType	InnerSell;
	
		///外盘
		TTORATstpLongVolumeType	OuterBuy;
	
		///申买剩余笔数一
		TTORATstpVolumeType	BidCount1;
	
		///申卖剩余笔数一
		TTORATstpVolumeType	AskCount1;
	
		///申卖剩余笔数二
		TTORATstpVolumeType	AskCount2;
	
		///申卖剩余笔数三
		TTORATstpVolumeType	AskCount3;
	
		///申买剩余笔数二
		TTORATstpVolumeType	BidCount2;
	
		///申买剩余笔数三
		TTORATstpVolumeType	BidCount3;
	
		///申卖剩余笔数四
		TTORATstpVolumeType	AskCount4;
	
		///申卖剩余笔数五
		TTORATstpVolumeType	AskCount5;
	
		///申买剩余笔数四
		TTORATstpVolumeType	BidCount4;
	
		///申买剩余笔数五
		TTORATstpVolumeType	BidCount5;
	
		///申卖剩余笔数六
		TTORATstpVolumeType	AskCount6;
	
		///申卖剩余笔数七
		TTORATstpVolumeType	AskCount7;
	
		///申买剩余笔数六
		TTORATstpVolumeType	BidCount6;
	
		///申买剩余笔数七
		TTORATstpVolumeType	BidCount7;
	
		///申卖剩余笔数八
		TTORATstpVolumeType	AskCount8;
	
		///申卖剩余笔数九
		TTORATstpVolumeType	AskCount9;
	
		///申买剩余笔数八
		TTORATstpVolumeType	BidCount8;
	
		///申买剩余笔数九
		TTORATstpVolumeType	BidCount9;
	
		///申买剩余笔数十
		TTORATstpVolumeType	BidCount10;
	
		///申卖剩余笔数十
		TTORATstpVolumeType	AskCount10;
	};
	
	/// 资金流向数据行情
	struct CTORATstpFundsFlowMarketDataField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///最后修改时间
		TTORATstpTimeType	UpdateTime;
	
		///最后修改毫秒
		TTORATstpMillisecType	UpdateMillisec;
	
		///散户买入金额
		TTORATstpMoneyType	RetailBuyTurnover;
	
		///散户买入数量
		TTORATstpLongVolumeType	RetailBuyVolume;
	
		///散户买入笔数
		TTORATstpLongVolumeType	RetailBuyAmount;
	
		///散户卖出金额
		TTORATstpMoneyType	RetailSellTurnover;
	
		///散户卖出数量
		TTORATstpLongVolumeType	RetailSellVolume;
	
		///散户卖出笔数
		TTORATstpLongVolumeType	RetailSellAmount;
	
		///中户买入金额
		TTORATstpMoneyType	MiddleBuyTurnover;
	
		///中户买入数量
		TTORATstpLongVolumeType	MiddleBuyVolume;
	
		///中户买入笔数
		TTORATstpLongVolumeType	MiddleBuyAmount;
	
		///中户卖出金额
		TTORATstpMoneyType	MiddleSellTurnover;
	
		///中户卖出数量
		TTORATstpLongVolumeType	MiddleSellVolume;
	
		///中户卖出笔数
		TTORATstpLongVolumeType	MiddleSellAmount;
	
		///大户买入金额
		TTORATstpMoneyType	LargeBuyTurnover;
	
		///大户买入数量
		TTORATstpLongVolumeType	LargeBuyVolume;
	
		///大户买入笔数
		TTORATstpLongVolumeType	LargeBuyAmount;
	
		///大户卖出金额
		TTORATstpMoneyType	LargeSellTurnover;
	
		///大户卖出数量
		TTORATstpLongVolumeType	LargeSellVolume;
	
		///大户卖出笔数
		TTORATstpLongVolumeType	LargeSellAmount;
	
		///机构买入金额
		TTORATstpMoneyType	InstitutionBuyTurnover;
	
		///机构买入数量
		TTORATstpLongVolumeType	InstitutionBuyVolume;
	
		///机构买入笔数
		TTORATstpLongVolumeType	InstitutionBuyAmount;
	
		///机构卖出金额
		TTORATstpMoneyType	InstitutionSellTurnover;
	
		///机构卖出数量
		TTORATstpLongVolumeType	InstitutionSellVolume;
	
		///机构卖出笔数
		TTORATstpLongVolumeType	InstitutionSellAmount;
	};
	
	/// fens用户信息
	struct CTORATstpFensUserInfoField
	{
		///版本信息
		TTORATstpFensVerType	FensVer;
	
		///环境编号
		TTORATstpFensEnvIDType	FensEnvID;
	
		///节点编号
		TTORATstpFensNodeIDType	FensNodeID;
	
		///Fens用户代码
		TTORATstpFensUserIDType	FensUserID;
	
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///终端信息
		TTORATstpClientInfoType	ClientInfo;
	};
	
	/// 连接信息
	struct CTORATstpConnectionInfoField
	{
		///内网IP地址
		TTORATstpIPAddressType	InnerIPAddress;
	
		///内网端口号
		TTORATstpPortType	InnerPort;
	
		///外网IP地址
		TTORATstpIPAddressType	OuterIPAddress;
	
		///外网端口号
		TTORATstpPortType	OuterPort;
	
		///MAC地址
		TTORATstpMacAddressType	MacAddress;
	};
	
}
#endif // TORATSTPXMDAPISTRUCT_H__