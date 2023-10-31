/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpLev2ApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
/////////////////////////////////////////////////////////////////////////

#if !defined(_TORA_TSTPLEV2APISTRUCT_H)
#define _TORA_TSTPLEV2APISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TORATstpLev2ApiDataType.h"

namespace TORALEV2API
{

	/// fens用户信息
	struct CTORATstpFensUserInfoField
	{
			
		///登录账户
		TTORATstpLogInAccountType	LogInAccount;

		///登录账户类型
		TTORATstpLogInAccountTypeType	LogInAccountType;

	};

	/// 登录请求
	struct CTORATstpReqUserLoginField
	{
			
		///登录账户
		TTORATstpLogInAccountType	LogInAccount;

		///登录账户类型
		TTORATstpLogInAccountTypeType	LogInAccountType;

		///密码(密码认证时必填)
		TTORATstpPasswordType	Password;

		///用户端产品信息
		TTORATstpProductInfoType	UserProductInfo;

		///接口端产品信息
		TTORATstpProductInfoType	InterfaceProductInfo;

		///协议信息
		TTORATstpProtocolInfoType	ProtocolInfo;

		///Mac地址
		TTORATstpMacAddressType	MacAddress;

		///移动设备手机号
		TTORATstpMobileType	Mobile;

		///内网IP地址
		TTORATstpIPAddressType	InnerIPAddress;

		///接口语言
		TTORATstpLangType	Lang;

		///终端信息
		TTORATstpTerminalInfoType	TerminalInfo;

		///网关Mac地址
		TTORATstpMacAddressType	GWMacAddress;

		///网关内网IP地址
		TTORATstpIPAddressType	GWInnerIPAddress;

		///网关外网IP地址
		TTORATstpIPAddressType	GWOuterIPAddress;

		///一级机构代码（以资金账号方式登录时必填）
		TTORATstpDepartmentIDType	DepartmentID;

		///硬盘序列号
		TTORATstpHDSerialType	HDSerial;

		///认证方式(指纹或钥匙串认证时必填)
		TTORATstpAuthModeType	AuthMode;

		///设备标识(指纹认证时必填)
		TTORATstpDeviceIDType	DeviceID;

		///认证序列号(指纹或钥匙串认证时必填)
		TTORATstpCertSerialType	CertSerial;

		///外网IP地址
		TTORATstpIPAddressType	OuterIPAddress;

		///动态密码
		TTORATstpPasswordType	DynamicPassword;

		///外网端口号
		TTORATstpPortType	OuterPort;

	};

	/// 登录应答
	struct CTORATstpRspUserLoginField
	{
			
		///登录时间
		TTORATstpTimeType	LoginTime;

		///登录账户
		TTORATstpLogInAccountType	LogInAccount;

		///登录账户类型
		TTORATstpLogInAccountTypeType	LogInAccountType;

		///交易系统名称
		TTORATstpSystemNameType	SystemName;

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

		///交易日
		TTORATstpDateType	TradingDay;

		///用户代码
		TTORATstpUserIDType	UserID;

		///用户名称
		TTORATstpUserNameType	UserName;

		///用户类型
		TTORATstpUserTypeType	UserType;

		///一级机构代码
		TTORATstpDepartmentIDType	DepartmentID;

		///终端IP地址
		TTORATstpIPAddressType	InnerIPAddress;

		///Mac地址
		TTORATstpMacAddressType	MacAddress;

		///硬盘序列号
		TTORATstpHDSerialType	HDSerial;

		///报单流控
		TTORATstpCommFluxType	OrderInsertCommFlux;

		///密码修改周期(天),置为0表永久有效
		TTORATstpCountType	PasswordUpdatePeriod;

		///密码有效剩余天数
		TTORATstpCountType	PasswordRemainDays;

		///是否需要改密
		TTORATstpBoolType	NeedUpdatePassword;

		///撤单流控
		TTORATstpCommFluxType	OrderActionCommFlux;

		///移动设备手机号
		TTORATstpMobileType	Mobile;

		///外网IP地址
		TTORATstpIPAddressType	OuterIPAddress;

		///认证序列号
		TTORATstpCertSerialType	CertSerial;

		///外网端口号
		TTORATstpPortType	OuterPort;

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

	/// Lev2行情
	struct CTORATstpLev2MarketDataField
	{
			
		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///行情交易所代码
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

		///委托买入总量
		TTORATstpLongVolumeType	TotalBidVolume;

		///加权平均委托买价格
		TTORATstpPriceType	AvgBidPrice;

		///委托卖出总量
		TTORATstpLongVolumeType	TotalAskVolume;

		///加权平均委托卖价格
		TTORATstpPriceType	AvgAskPrice;

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

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///涨停板价(只有深圳行情有效)
		TTORATstpPriceType	UpperLimitPrice;

		///跌停板价(只有深圳行情有效)
		TTORATstpPriceType	LowerLimitPrice;

		///今收盘价(只有上海行情有效)
		TTORATstpPriceType	ClosePrice;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///买入总笔数(只有上海行情有效)
		TTORATstpVolumeType	TotalBidNumber;

		///卖出总笔数(只有上海行情有效)
		TTORATstpVolumeType	TotalOfferNumber;

		///买入委托成交最大等待时间(只有上海行情有效)
		TTORATstpVolumeType	BidTradeMaxDuration;

		///卖出委托成交最大等待时间(只有上海行情有效)
		TTORATstpVolumeType	OfferTradeMaxDuration;

		///基金实时参考净值
		TTORATstpPriceType	IOPV;

		///卖一价上总委托笔数
		TTORATstpVolumeType	Ask1NumOrders;

		///买一价上总委托笔数
		TTORATstpVolumeType	Bid1NumOrders;

		///卖二价上总委托笔数
		TTORATstpVolumeType	Ask2NumOrders;

		///买二价上总委托笔数
		TTORATstpVolumeType	Bid2NumOrders;

		///卖三价上总委托笔数
		TTORATstpVolumeType	Ask3NumOrders;

		///买三价上总委托笔数
		TTORATstpVolumeType	Bid3NumOrders;

		///卖四价上总委托笔数
		TTORATstpVolumeType	Ask4NumOrders;

		///买四价上总委托笔数
		TTORATstpVolumeType	Bid4NumOrders;

		///卖五价上总委托笔数
		TTORATstpVolumeType	Ask5NumOrders;

		///买五价上总委托笔数
		TTORATstpVolumeType	Bid5NumOrders;

		///卖六价上总委托笔数
		TTORATstpVolumeType	Ask6NumOrders;

		///买六价上总委托笔数
		TTORATstpVolumeType	Bid6NumOrders;

		///卖七价上总委托笔数
		TTORATstpVolumeType	Ask7NumOrders;

		///买七价上总委托笔数
		TTORATstpVolumeType	Bid7NumOrders;

		///卖八价上总委托笔数
		TTORATstpVolumeType	Ask8NumOrders;

		///买八价上总委托笔数
		TTORATstpVolumeType	Bid8NumOrders;

		///卖九价上总委托笔数
		TTORATstpVolumeType	Ask9NumOrders;

		///买九价上总委托笔数
		TTORATstpVolumeType	Bid9NumOrders;

		///卖十价上总委托笔数
		TTORATstpVolumeType	Ask10NumOrders;

		///买十价上总委托笔数
		TTORATstpVolumeType	Bid10NumOrders;

		///买入撤单笔数(只有上海行情有效)
		TTORATstpVolumeType	WithdrawBuyNumber;

		///买入撤单数量(只有上海行情有效)
		TTORATstpLongVolumeType	WithdrawBuyAmount;

		///买入撤单金额(只有上海行情有效)
		TTORATstpMoneyType	WithdrawBuyMoney;

		///卖出撤单笔数(只有上海行情有效)
		TTORATstpVolumeType	WithdrawSellNumber;

		///卖出撤单数量(只有上海行情有效)
		TTORATstpLongVolumeType	WithdrawSellAmount;

		///卖出撤单金额(只有上海行情有效)
		TTORATstpMoneyType	WithdrawSellMoney;

	};

	/// Lev2指数行情
	struct CTORATstpLev2IndexField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	DataTimeStamp;

		///前收盘指数
		TTORATstpPriceType	PreCloseIndex;

		///今开盘指数
		TTORATstpPriceType	OpenIndex;

		///最高指数
		TTORATstpPriceType	HighIndex;

		///最低指数
		TTORATstpPriceType	LowIndex;

		///最新指数
		TTORATstpPriceType	LastIndex;

		///参与计算相应指数的成交金额（元
		TTORATstpMoneyType	Turnover;

		///参与计算相应指数的交易数量(手)
		TTORATstpLongVolumeType	TotalVolumeTraded;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///收盘指数
		TTORATstpPriceType	CloseIndex;

	};

	/// Lev2逐笔成交
	struct CTORATstpLev2TransactionField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	TradeTime;

		///成交价格
		TTORATstpPriceType	TradePrice;

		///成交数量
		TTORATstpLongVolumeType	TradeVolume;

		///成交类别（只有深圳行情有效）
		TTORATstpExecTypeType	ExecType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///内外盘标志（只有上海行情有效）
		TTORATstpTradeBSFlagType	TradeBSFlag;

		///业务序号（只有上海行情有效）
		TTORATstpLongSequenceType	BizIndex;

	};

	/// Lev2逐笔委托
	struct CTORATstpLev2OrderDetailField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	OrderTime;

		///委托价格
		TTORATstpPriceType	Price;

		///委托数量
		TTORATstpLongVolumeType	Volume;

		///委托方向
		TTORATstpLSideType	Side;

		///订单类别（只有深圳行情有效）
		TTORATstpLOrderTypeType	OrderType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpSequenceNoType	SubSeq;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///委托序号
		TTORATstpLongSequenceType	OrderNO;

		///订单状态
		TTORATstpLOrderStatusType	OrderStatus;

		///业务序号（只有上海行情有效）
		TTORATstpLongSequenceType	BizIndex;

	};

	/// Lev2盘后定价行情
	struct CTORATstpLev2PHMarketDataField
	{
			
		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///行情交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///时间戳
		TTORATstpTimeStampType	DataTimeStamp;

		///今收盘价(只有上海行情有效)
		TTORATstpPriceType	ClosePrice;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///成交笔数
		TTORATstpLongVolumeType	NumTrades;

		///成交总量
		TTORATstpLongVolumeType	TotalVolumeTrade;

		///成交总金额
		TTORATstpMoneyType	TotalValueTrade;

		///委托买入总量
		TTORATstpLongVolumeType	TotalBidVolume;

		///委托卖出总量
		TTORATstpLongVolumeType	TotalAskVolume;

		///买入撤单笔数
		TTORATstpLongVolumeType	WithdrawBuyNumber;

		///买入撤单数量
		TTORATstpLongVolumeType	WithdrawBuyAmount;

		///卖出撤单笔数
		TTORATstpLongVolumeType	WithdrawSellNumber;

		///卖出撤单数量
		TTORATstpLongVolumeType	WithdrawSellAmount;

		///申买量1
		TTORATstpLongVolumeType	BidOrderQty;

		///实际的买委托笔数1
		TTORATstpLongVolumeType	BidNumOrders;

		///申卖量1
		TTORATstpLongVolumeType	AskOrderQty;

		///实际的卖委托笔数1
		TTORATstpLongVolumeType	AskNumOrders;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2盘后定价逐笔成交
	struct CTORATstpLev2PHTransactionField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	TradeTime;

		///成交价格
		TTORATstpPriceType	TradePrice;

		///成交数量
		TTORATstpLongVolumeType	TradeVolume;

		///成交金额(元)
		TTORATstpMoneyType	TradeMoney;

		///成交类别
		TTORATstpExecTypeType	ExecType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///内外盘标志
		TTORATstpTradeBSFlagType	TradeBSFlag;

	};

	/// Lev2逐笔成交重传
	struct CTORATstpLev2ResendTransactionField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	TradeTime;

		///成交价格
		TTORATstpPriceType	TradePrice;

		///成交数量
		TTORATstpLongVolumeType	TradeVolume;

		///成交类别（只有深圳行情有效）
		TTORATstpExecTypeType	ExecType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///内外盘标志（只有上海行情有效）
		TTORATstpTradeBSFlagType	TradeBSFlag;

		///业务序号（只有上海行情有效）
		TTORATstpLongSequenceType	BizIndex;

	};

	/// Lev2逐笔委托重传
	struct CTORATstpLev2ResendOrderDetailField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	OrderTime;

		///委托价格
		TTORATstpPriceType	Price;

		///委托数量
		TTORATstpLongVolumeType	Volume;

		///委托方向
		TTORATstpLSideType	Side;

		///订单类别（只有深圳行情有效）
		TTORATstpLOrderTypeType	OrderType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpSequenceNoType	SubSeq;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

		///委托序号
		TTORATstpLongSequenceType	OrderNO;

		///订单状态
		TTORATstpLOrderStatusType	OrderStatus;

		///业务序号（只有上海行情有效）
		TTORATstpLongSequenceType	BizIndex;

	};

	/// Lev2债券快照行情(上海)
	struct CTORATstpLev2XTSMarketDataField
	{
			
		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///行情交易所代码
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

		///委托买入总量
		TTORATstpLongVolumeType	TotalBidVolume;

		///加权平均委托买价格
		TTORATstpPriceType	AvgBidPrice;

		///委托卖出总量
		TTORATstpLongVolumeType	TotalAskVolume;

		///加权平均委托卖价格
		TTORATstpPriceType	AvgAskPrice;

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

		///买一价上总委托笔数
		TTORATstpVolumeType	Bid1NumOrders;

		///申卖价一
		TTORATstpPriceType	AskPrice1;

		///申卖量一
		TTORATstpLongVolumeType	AskVolume1;

		///卖一价上总委托笔数
		TTORATstpVolumeType	Ask1NumOrders;

		///申卖价二
		TTORATstpPriceType	AskPrice2;

		///申卖量二
		TTORATstpLongVolumeType	AskVolume2;

		///卖二价上总委托笔数
		TTORATstpVolumeType	Ask2NumOrders;

		///申卖价三
		TTORATstpPriceType	AskPrice3;

		///申卖量三
		TTORATstpLongVolumeType	AskVolume3;

		///卖三价上总委托笔数
		TTORATstpVolumeType	Ask3NumOrders;

		///申买价二
		TTORATstpPriceType	BidPrice2;

		///申买量二
		TTORATstpLongVolumeType	BidVolume2;

		///买二价上总委托笔数
		TTORATstpVolumeType	Bid2NumOrders;

		///申买价三
		TTORATstpPriceType	BidPrice3;

		///申买量三
		TTORATstpLongVolumeType	BidVolume3;

		///买三价上总委托笔数
		TTORATstpVolumeType	Bid3NumOrders;

		///申卖价四
		TTORATstpPriceType	AskPrice4;

		///申卖量四
		TTORATstpLongVolumeType	AskVolume4;

		///卖四价上总委托笔数
		TTORATstpVolumeType	Ask4NumOrders;

		///申卖价五
		TTORATstpPriceType	AskPrice5;

		///申卖量五
		TTORATstpLongVolumeType	AskVolume5;

		///卖五价上总委托笔数
		TTORATstpVolumeType	Ask5NumOrders;

		///申买价四
		TTORATstpPriceType	BidPrice4;

		///申买量四
		TTORATstpLongVolumeType	BidVolume4;

		///买四价上总委托笔数
		TTORATstpVolumeType	Bid4NumOrders;

		///申买价五
		TTORATstpPriceType	BidPrice5;

		///申买量五
		TTORATstpLongVolumeType	BidVolume5;

		///买五价上总委托笔数
		TTORATstpVolumeType	Bid5NumOrders;

		///申卖价六
		TTORATstpPriceType	AskPrice6;

		///申卖量六
		TTORATstpLongVolumeType	AskVolume6;

		///卖六价上总委托笔数
		TTORATstpVolumeType	Ask6NumOrders;

		///申卖价七
		TTORATstpPriceType	AskPrice7;

		///申卖量七
		TTORATstpLongVolumeType	AskVolume7;

		///卖七价上总委托笔数
		TTORATstpVolumeType	Ask7NumOrders;

		///申买价六
		TTORATstpPriceType	BidPrice6;

		///申买量六
		TTORATstpLongVolumeType	BidVolume6;

		///买六价上总委托笔数
		TTORATstpVolumeType	Bid6NumOrders;

		///申买价七
		TTORATstpPriceType	BidPrice7;

		///申买量七
		TTORATstpLongVolumeType	BidVolume7;

		///买七价上总委托笔数
		TTORATstpVolumeType	Bid7NumOrders;

		///申卖价八
		TTORATstpPriceType	AskPrice8;

		///申卖量八
		TTORATstpLongVolumeType	AskVolume8;

		///卖八价上总委托笔数
		TTORATstpVolumeType	Ask8NumOrders;

		///申卖价九
		TTORATstpPriceType	AskPrice9;

		///申卖量九
		TTORATstpLongVolumeType	AskVolume9;

		///卖九价上总委托笔数
		TTORATstpVolumeType	Ask9NumOrders;

		///申买价八
		TTORATstpPriceType	BidPrice8;

		///申买量八
		TTORATstpLongVolumeType	BidVolume8;

		///买八价上总委托笔数
		TTORATstpVolumeType	Bid8NumOrders;

		///申买价九
		TTORATstpPriceType	BidPrice9;

		///申买量九
		TTORATstpLongVolumeType	BidVolume9;

		///买九价上总委托笔数
		TTORATstpVolumeType	Bid9NumOrders;

		///申买价十
		TTORATstpPriceType	BidPrice10;

		///申买量十
		TTORATstpLongVolumeType	BidVolume10;

		///买十价上总委托笔数
		TTORATstpVolumeType	Bid10NumOrders;

		///申卖价十
		TTORATstpPriceType	AskPrice10;

		///申卖量十
		TTORATstpLongVolumeType	AskVolume10;

		///卖十价上总委托笔数
		TTORATstpVolumeType	Ask10NumOrders;

		///今收盘价
		TTORATstpPriceType	ClosePrice;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///买入总笔数
		TTORATstpVolumeType	TotalBidNumber;

		///卖出总笔数
		TTORATstpVolumeType	TotalOfferNumber;

		///买入委托成交量最大等待时间
		TTORATstpVolumeType	BidTradeMaxDuration;

		///卖出委托成交量最大等待时间
		TTORATstpVolumeType	OfferTradeMaxDuration;

		///买入撤单笔数
		TTORATstpVolumeType	WithdrawBuyNumber;

		///买入撤单数量
		TTORATstpLongVolumeType	WithdrawBuyAmount;

		///买入撤单金额
		TTORATstpMoneyType	WithdrawBuyMoney;

		///卖出撤单笔数
		TTORATstpVolumeType	WithdrawSellNumber;

		///卖出撤单数量
		TTORATstpLongVolumeType	WithdrawSellAmount;

		///卖出撤单金额
		TTORATstpMoneyType	WithdrawSellMoney;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2债券逐笔行情(上海)
	struct CTORATstpLev2XTSTickField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///时间戳
		TTORATstpTimeStampType	TickTime;

		///逐笔类型
		TTORATstpLTickTypeType	TickType;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///价格
		TTORATstpPriceType	Price;

		///数量
		TTORATstpLongVolumeType	Volume;

		///成交金额
		TTORATstpMoneyType	TradeMoney;

		///委托方向
		TTORATstpLSideType	Side;

		///内外盘标志
		TTORATstpTradeBSFlagType	TradeBSFlag;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2逐笔行情(上海)
	struct CTORATstpLev2NGTSTickField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///时间戳
		TTORATstpTimeStampType	TickTime;

		///逐笔类型
		TTORATstpLTickTypeType	TickType;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///价格
		TTORATstpPriceType	Price;

		///数量
		TTORATstpLongVolumeType	Volume;

		///成交金额或新增委托订单已成交委托量
		TTORATstpMoneyType	TradeMoney;

		///委托方向
		TTORATstpLSideType	Side;

		///内外盘标志
		TTORATstpTradeBSFlagType	TradeBSFlag;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2深圳债券快照行情
	struct CTORATstpLev2BondMarketDataField
	{
			
		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///行情交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///时间戳
		TTORATstpTimeStampType	DataTimeStamp;

		///昨收盘
		TTORATstpPriceType	PreClosePrice;

		///今开盘
		TTORATstpPriceType	OpenPrice;

		///昨收盘加权平均价
		TTORATstpPriceType	AvgPreClosePrice;

		///成交笔数
		TTORATstpLongVolumeType	NumTrades;

		///成交总量
		TTORATstpLongVolumeType	TotalVolumeTrade;

		///成交总金额
		TTORATstpMoneyType	TotalValueTrade;

		///匹配成交成交量
		TTORATstpLongVolumeType	AuctionVolumeTrade;

		///匹配成交成交金额
		TTORATstpMoneyType	AuctionValueTrade;

		///委托买入总量
		TTORATstpLongVolumeType	TotalBidVolume;

		///加权平均委托买价格
		TTORATstpPriceType	AvgBidPrice;

		///委托卖出总量
		TTORATstpLongVolumeType	TotalAskVolume;

		///加权平均委托卖价格
		TTORATstpPriceType	AvgAskPrice;

		///最高价
		TTORATstpPriceType	HighestPrice;

		///最低价
		TTORATstpPriceType	LowestPrice;

		///最近价
		TTORATstpPriceType	LastPrice;

		///匹配成交最近价
		TTORATstpPriceType	AuctionLastPrice;

		///加权平均价格
		TTORATstpPriceType	AvgPrice;

		///升跌1
		TTORATstpPriceType	PriceUpDown1;

		///升跌2
		TTORATstpPriceType	PriceUpDown2;

		///今收盘价
		TTORATstpPriceType	ClosePrice;

		///行情产品实时状态
		TTORATstpMDSecurityStatType	MDSecurityStat;

		///申买价一
		TTORATstpPriceType	BidPrice1;

		///申买量一
		TTORATstpLongVolumeType	BidVolume1;

		///买一价上总委托笔数
		TTORATstpVolumeType	Bid1NumOrders;

		///申卖价一
		TTORATstpPriceType	AskPrice1;

		///申卖量一
		TTORATstpLongVolumeType	AskVolume1;

		///卖一价上总委托笔数
		TTORATstpVolumeType	Ask1NumOrders;

		///申卖价二
		TTORATstpPriceType	AskPrice2;

		///申卖量二
		TTORATstpLongVolumeType	AskVolume2;

		///卖二价上总委托笔数
		TTORATstpVolumeType	Ask2NumOrders;

		///申卖价三
		TTORATstpPriceType	AskPrice3;

		///申卖量三
		TTORATstpLongVolumeType	AskVolume3;

		///卖三价上总委托笔数
		TTORATstpVolumeType	Ask3NumOrders;

		///申买价二
		TTORATstpPriceType	BidPrice2;

		///申买量二
		TTORATstpLongVolumeType	BidVolume2;

		///买二价上总委托笔数
		TTORATstpVolumeType	Bid2NumOrders;

		///申买价三
		TTORATstpPriceType	BidPrice3;

		///申买量三
		TTORATstpLongVolumeType	BidVolume3;

		///买三价上总委托笔数
		TTORATstpVolumeType	Bid3NumOrders;

		///申卖价四
		TTORATstpPriceType	AskPrice4;

		///申卖量四
		TTORATstpLongVolumeType	AskVolume4;

		///卖四价上总委托笔数
		TTORATstpVolumeType	Ask4NumOrders;

		///申卖价五
		TTORATstpPriceType	AskPrice5;

		///申卖量五
		TTORATstpLongVolumeType	AskVolume5;

		///卖五价上总委托笔数
		TTORATstpVolumeType	Ask5NumOrders;

		///申买价四
		TTORATstpPriceType	BidPrice4;

		///申买量四
		TTORATstpLongVolumeType	BidVolume4;

		///买四价上总委托笔数
		TTORATstpVolumeType	Bid4NumOrders;

		///申买价五
		TTORATstpPriceType	BidPrice5;

		///申买量五
		TTORATstpLongVolumeType	BidVolume5;

		///买五价上总委托笔数
		TTORATstpVolumeType	Bid5NumOrders;

		///申卖价六
		TTORATstpPriceType	AskPrice6;

		///申卖量六
		TTORATstpLongVolumeType	AskVolume6;

		///卖六价上总委托笔数
		TTORATstpVolumeType	Ask6NumOrders;

		///申卖价七
		TTORATstpPriceType	AskPrice7;

		///申卖量七
		TTORATstpLongVolumeType	AskVolume7;

		///卖七价上总委托笔数
		TTORATstpVolumeType	Ask7NumOrders;

		///申买价六
		TTORATstpPriceType	BidPrice6;

		///申买量六
		TTORATstpLongVolumeType	BidVolume6;

		///买六价上总委托笔数
		TTORATstpVolumeType	Bid6NumOrders;

		///申买价七
		TTORATstpPriceType	BidPrice7;

		///申买量七
		TTORATstpLongVolumeType	BidVolume7;

		///买七价上总委托笔数
		TTORATstpVolumeType	Bid7NumOrders;

		///申卖价八
		TTORATstpPriceType	AskPrice8;

		///申卖量八
		TTORATstpLongVolumeType	AskVolume8;

		///卖八价上总委托笔数
		TTORATstpVolumeType	Ask8NumOrders;

		///申卖价九
		TTORATstpPriceType	AskPrice9;

		///申卖量九
		TTORATstpLongVolumeType	AskVolume9;

		///卖九价上总委托笔数
		TTORATstpVolumeType	Ask9NumOrders;

		///申买价八
		TTORATstpPriceType	BidPrice8;

		///申买量八
		TTORATstpLongVolumeType	BidVolume8;

		///买八价上总委托笔数
		TTORATstpVolumeType	Bid8NumOrders;

		///申买价九
		TTORATstpPriceType	BidPrice9;

		///申买量九
		TTORATstpLongVolumeType	BidVolume9;

		///买九价上总委托笔数
		TTORATstpVolumeType	Bid9NumOrders;

		///申买价十
		TTORATstpPriceType	BidPrice10;

		///申买量十
		TTORATstpLongVolumeType	BidVolume10;

		///买十价上总委托笔数
		TTORATstpVolumeType	Bid10NumOrders;

		///申卖价十
		TTORATstpPriceType	AskPrice10;

		///申卖量十
		TTORATstpLongVolumeType	AskVolume10;

		///卖十价上总委托笔数
		TTORATstpVolumeType	Ask10NumOrders;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2深圳债券逐笔行情
	struct CTORATstpLev2BondOrderDetailField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	OrderTime;

		///委托价格
		TTORATstpPriceType	Price;

		///委托数量
		TTORATstpLongVolumeType	Volume;

		///委托方向
		TTORATstpLSideType	Side;

		///订单类别
		TTORATstpLOrderTypeType	OrderType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

	/// Lev2深圳债券逐笔行情
	struct CTORATstpLev2BondTransactionField
	{
			
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;

		///证券代码
		TTORATstpSecurityIDType	SecurityID;

		///时间戳
		TTORATstpTimeStampType	TradeTime;

		///成交价格
		TTORATstpPriceType	TradePrice;

		///成交数量
		TTORATstpLongVolumeType	TradeVolume;

		///成交类别
		TTORATstpExecTypeType	ExecType;

		///主序号
		TTORATstpSequenceNoType	MainSeq;

		///子序号
		TTORATstpLongSequenceType	SubSeq;

		///买方委托序号
		TTORATstpLongSequenceType	BuyNo;

		///卖方委托序号
		TTORATstpLongSequenceType	SellNo;

		///附加信息1
		TTORATstpIntInfoType	Info1;

		///附加信息2
		TTORATstpIntInfoType	Info2;

		///附加信息3
		TTORATstpIntInfoType	Info3;

	};

}
#endif