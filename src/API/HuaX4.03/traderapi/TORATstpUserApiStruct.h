/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpUserApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
/////////////////////////////////////////////////////////////////////////
#ifndef TORATSTPUSERAPISTRUCT_H__
#define TORATSTPUSERAPISTRUCT_H__

#include "TORATstpUserApiDataType.h"

namespace TORASTOCKAPI
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
	
	/// 修改用户密码
	struct CTORATstpUserPasswordUpdateField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///旧密码
		TTORATstpPasswordType	OldPassword;
	
		///新密码
		TTORATstpPasswordType	NewPassword;
	};
	
	/// 请求录入设备序列
	struct CTORATstpReqInputDeviceSerialField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///设备标识
		TTORATstpDeviceIDType	DeviceID;
	
		///设备序列号
		TTORATstpCertSerialType	CertSerial;
	
		///设备类别
		TTORATstpDeviceTypeType	DeviceType;
	};
	
	/// 录入设备序列响应
	struct CTORATstpRspInputDeviceSerialField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	};
	
	/// 输入报单
	struct CTORATstpInputOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///报单价格条件
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///报单引用
		TTORATstpOrderRefType	OrderRef;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///条件检查
		TTORATstpCondCheckType	CondCheck;
	
		///有效日期
		TTORATstpDateType	GTDate;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///指定偿还的信用负债编号（该字段置空表示不指定偿还）(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///头寸类型(两融专用)
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	
		///费息折扣券编号（0表示不使用折扣券）(两融专用)
		TTORATstpIntSerialType	DiscountCouponID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 报单
	struct CTORATstpOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///报单价格条件
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///有效日期
		TTORATstpDateType	GTDate;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///条件检查
		TTORATstpCondCheckType	CondCheck;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///报单引用
		TTORATstpOrderRefType	OrderRef;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///报单状态
		TTORATstpOrderStatusType	OrderStatus;
	
		///报单提交状态
		TTORATstpOrderSubmitStatusType	OrderSubmitStatus;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///已成交数量
		TTORATstpVolumeType	VolumeTraded;
	
		///已撤销数量
		TTORATstpVolumeType	VolumeCanceled;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///申报用户
		TTORATstpUserIDType	InsertUser;
	
		///申报日期
		TTORATstpDateType	InsertDate;
	
		///申报时间
		TTORATstpTimeType	InsertTime;
	
		///交易所接收时间
		TTORATstpTimeType	AcceptTime;
	
		///撤销用户
		TTORATstpUserIDType	CancelUser;
	
		///撤销时间
		TTORATstpTimeType	CancelTime;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///成交金额
		TTORATstpMoneyType	Turnover;
	
		///报单类型
		TTORATstpOrderTypeType	OrderType;
	
		///用户端产品信息
		TTORATstpUserProductInfoType	UserProductInfo;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///信用头寸编号(两融专用)
		TTORATstpQuotaIDType	CreditQuotaID;
	
		///头寸类型(两融专用)
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	
		///信用负债编号(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	
		///回报附加浮点型数据信息
		TTORATstpFloatInfoType	RtnFloatInfo;
	
		///回报附加整型数据
		TTORATstpIntInfoType	RtnIntInfo;
	
		///回报附加浮点型数据1
		TTORATstpFloatInfoType	RtnFloatInfo1;
	
		///回报附加浮点型数据2
		TTORATstpFloatInfoType	RtnFloatInfo2;
	};
	
	/// 用户标识
	struct CTORATstpUserRefField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	};
	
	/// 成交
	struct CTORATstpTradeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///成交价格
		TTORATstpPriceType	Price;
	
		///成交数量
		TTORATstpVolumeType	Volume;
	
		///成交日期
		TTORATstpDateType	TradeDate;
	
		///成交时间
		TTORATstpTimeType	TradeTime;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///报单引用
		TTORATstpOrderRefType	OrderRef;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	};
	
	/// 输入撤单操作
	struct CTORATstpInputOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///被撤报单前置编号
		TTORATstpFrontIDType	FrontID;
	
		///被撤报单会话编号
		TTORATstpSessionIDType	SessionID;
	
		///被撤报单引用
		TTORATstpOrderRefType	OrderRef;
	
		///被撤报单系统编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///报单操作引用
		TTORATstpOrderRefType	OrderActionRef;
	
		///系统撤单编号
		TTORATstpOrderSysIDType	CancelOrderSysID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 条件单录入域
	struct CTORATstpInputCondOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///价格类型
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///报单价格
		TTORATstpPriceType	LimitPrice;
	
		///报单数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///条件检查
		TTORATstpCondCheckType	CondCheck;
	
		///有效日期
		TTORATstpDateType	GTDate;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///指定偿还的信用负债编号（该字段置空表示不指定偿还）(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///头寸类型(两融专用)
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	
		///费息折扣券编号（0表示不使用折扣券）(两融专用)
		TTORATstpIntSerialType	DiscountCouponID;
	
		///条件报单引用
		TTORATstpOrderRefType	CondOrderRef;
	
		///条件报单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///触发基准数量类型
		TTORATstpTriggerOrderVolumeTypeType	TriggerOrderVolumeType;
	
		///触发基准价类型
		TTORATstpTriggerOrderPriceTypeType	TriggerOrderPriceType;
	
		///条件单触发条件
		TTORATstpContingentConditionType	ContingentCondition;
	
		///条件价
		TTORATstpPriceType	ConditionPrice;
	
		///价格浮动tick数
		TTORATstpVolumeType	PriceTicks;
	
		///数量浮动倍数
		TTORATstpVolumeMultipleType	VolumeMultiple;
	
		///相关前置编号
		TTORATstpFrontIDType	RelativeFrontID;
	
		///相关会话编号
		TTORATstpSessionIDType	RelativeSessionID;
	
		///相关条件参数
		TTORATstpRelativeCondParamType	RelativeParam;
	
		///附加条件单触发条件
		TTORATstpContingentConditionType	AppendContingentCondition;
	
		///附加条件价
		TTORATstpPriceType	AppendConditionPrice;
	
		///附加相关前置编号
		TTORATstpFrontIDType	AppendRelativeFrontID;
	
		///附加相关会话编号
		TTORATstpSessionIDType	AppendRelativeSessionID;
	
		///附加相关条件参数
		TTORATstpRelativeCondParamType	AppendRelativeParam;
	};
	
	/// 条件单域
	struct CTORATstpConditionOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///价格类型
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///报单价格
		TTORATstpPriceType	LimitPrice;
	
		///报单数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///条件检查
		TTORATstpCondCheckType	CondCheck;
	
		///有效日期
		TTORATstpDateType	GTDate;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///指定偿还的信用负债编号（该字段置空表示不指定偿还）(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///头寸类型(两融专用)
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	
		///费息折扣券编号（0表示不使用折扣券）(两融专用)
		TTORATstpIntSerialType	DiscountCouponID;
	
		///条件报单引用
		TTORATstpOrderRefType	CondOrderRef;
	
		///报单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///触发基准数量类型
		TTORATstpTriggerOrderVolumeTypeType	TriggerOrderVolumeType;
	
		///触发基准价类型
		TTORATstpTriggerOrderPriceTypeType	TriggerOrderPriceType;
	
		///条件单触发条件
		TTORATstpContingentConditionType	ContingentCondition;
	
		///条件价
		TTORATstpPriceType	ConditionPrice;
	
		///价格浮动tick数
		TTORATstpVolumeType	PriceTicks;
	
		///数量浮动倍数
		TTORATstpVolumeMultipleType	VolumeMultiple;
	
		///相关前置编号
		TTORATstpFrontIDType	RelativeFrontID;
	
		///相关会话编号
		TTORATstpSessionIDType	RelativeSessionID;
	
		///相关条件参数
		TTORATstpRelativeCondParamType	RelativeParam;
	
		///附加条件单触发条件
		TTORATstpContingentConditionType	AppendContingentCondition;
	
		///附加条件价
		TTORATstpPriceType	AppendConditionPrice;
	
		///附加相关前置编号
		TTORATstpFrontIDType	AppendRelativeFrontID;
	
		///附加相关会话编号
		TTORATstpSessionIDType	AppendRelativeSessionID;
	
		///附加相关条件参数
		TTORATstpRelativeCondParamType	AppendRelativeParam;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///条件单状态
		TTORATstpCondOrderStatusType	CondOrderStatus;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///申报用户
		TTORATstpUserIDType	InsertUser;
	
		///申报日期
		TTORATstpDateType	InsertDate;
	
		///申报时间
		TTORATstpTimeType	InsertTime;
	
		///触发日期
		TTORATstpDateType	ActiveDate;
	
		///触发时间
		TTORATstpTimeType	ActiveTime;
	
		///撤销用户
		TTORATstpUserIDType	CancelUser;
	
		///撤销时间
		TTORATstpTimeType	CancelTime;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///用户端产品信息
		TTORATstpUserProductInfoType	UserProductInfo;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 条件单操作录入
	struct CTORATstpInputCondOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///被撤条件单前置编号
		TTORATstpFrontIDType	FrontID;
	
		///被撤条件单会话编号
		TTORATstpSessionIDType	SessionID;
	
		///被撤条件单引用
		TTORATstpOrderRefType	CondOrderRef;
	
		///被撤条件单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///条件单操作引用
		TTORATstpOrderRefType	CondOrderActionRef;
	
		///条件单撤单编号
		TTORATstpCondOrderIDType	CancelCondOrderID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 输入报单
	struct CTORATstpInputNegoOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///联系人(意向申报时必填)
		TTORATstpNegoContractorType	Contractor;
	
		///联系方式(意向申报时必填)
		TTORATstpNegoContractorInfoType	ContractorInfo;
	
		///约定号(成交申报时必填)
		TTORATstpNegoConfirmIDType	ConfirmID;
	
		///对手方席位号(成交申报时必填)
		TTORATstpPbuIDType	CounterpartyPbuID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 协议交易报单
	struct CTORATstpNegoOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///数量
		TTORATstpVolumeType	VolumeTotalOriginal;
	
		///联系人
		TTORATstpNegoContractorType	Contractor;
	
		///联系方式
		TTORATstpNegoContractorInfoType	ContractorInfo;
	
		///约定号
		TTORATstpNegoConfirmIDType	ConfirmID;
	
		///对手方席位号
		TTORATstpPbuIDType	CounterpartyPbuID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///报单状态
		TTORATstpOrderStatusType	OrderStatus;
	
		///报单提交状态
		TTORATstpOrderSubmitStatusType	OrderSubmitStatus;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///已成交数量
		TTORATstpVolumeType	VolumeTraded;
	
		///已撤销数量
		TTORATstpVolumeType	VolumeCanceled;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///申报用户
		TTORATstpUserIDType	InsertUser;
	
		///申报日期
		TTORATstpDateType	InsertDate;
	
		///申报时间
		TTORATstpTimeType	InsertTime;
	
		///交易所接收时间
		TTORATstpTimeType	AcceptTime;
	
		///撤销用户
		TTORATstpUserIDType	CancelUser;
	
		///撤销时间
		TTORATstpTimeType	CancelTime;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///用户端产品信息
		TTORATstpUserProductInfoType	UserProductInfo;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 协议交易成交
	struct CTORATstpNegoTradeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///成交价格
		TTORATstpPriceType	Price;
	
		///成交数量
		TTORATstpVolumeType	Volume;
	
		///成交日期
		TTORATstpDateType	TradeDate;
	
		///成交时间
		TTORATstpTimeType	TradeTime;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///对手方交易单元代码
		TTORATstpPbuIDType	CounterpartyPbuID;
	
		///对手方股东账号
		TTORATstpShareholderIDType	CounterpartyShareholderID;
	};
	
	/// 输入协议交易撤单操作
	struct CTORATstpInputNegoOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///被撤报单系统编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///系统撤单编号
		TTORATstpOrderSysIDType	CancelOrderSysID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 市场状态
	struct CTORATstpMarketStatusField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///市场状态
		TTORATstpMarketStatusType	MarketStatus;
	};
	
	/// 资金转移
	struct CTORATstpInputTransferFundField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///转移金额
		TTORATstpMoneyType	Amount;
	
		///银行代码(银证转账时必填)
		TTORATstpBankIDType	BankID;
	
		///资金密码(证券转银行时必填)
		TTORATstpPasswordType	AccountPassword;
	
		///银行密码(银行转证券时必填)
		TTORATstpPasswordType	BankPassword;
	
		///外部节点号(节点间资金调拨时必填)
		TTORATstpNodeIDType	ExternalNodeID;
	
		///指定偿还的信用负债编号(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///实际偿还金额(返回)(两融专用)
		TTORATstpMoneyType	RealAmount;
	
		///强制标识(证券转银行时有效)
		TTORATstpBoolType	ForceFlag;
	};
	
	/// 资金转移回报
	struct CTORATstpTransferFundField
	{
		///转移流水号
		TTORATstpIntSerialType	FundSerial;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///转移金额
		TTORATstpMoneyType	Amount;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作人员
		TTORATstpUserIDType	OperatorID;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///签约银行账户
		TTORATstpBankAccountIDType	BankAccountID;
	
		///银行代码
		TTORATstpBankIDType	BankID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///外部节点号
		TTORATstpNodeIDType	ExternalNodeID;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 仓位转移
	struct CTORATstpInputTransferPositionField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///数量
		TTORATstpVolumeType	Volume;
	
		///转移持仓类型
		TTORATstpTransferPositionTypeType	TransferPositionType;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///外部节点编号(外部系统转仓时必填)
		TTORATstpNodeIDType	ExternalNodeID;
	};
	
	/// 仓位转移回报
	struct CTORATstpTransferPositionField
	{
		///仓位转移流水号
		TTORATstpIntSerialType	PositionSerial;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///转移持仓类型
		TTORATstpTransferPositionTypeType	TransferPositionType;
	
		///昨日持仓数量
		TTORATstpVolumeType	HistoryVolume;
	
		///今日买卖持仓数量
		TTORATstpVolumeType	TodayBSVolume;
	
		///今日申赎持仓数量
		TTORATstpVolumeType	TodayPRVolume;
	
		///今日拆分合并持仓数量
		TTORATstpVolumeType	TodaySMVolume;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作人员
		TTORATstpUserIDType	OperatorID;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	
		///外部节点编号
		TTORATstpNodeIDType	ExternalNodeID;
	};
	
	/// 外围系统仓位转移回报
	struct CTORATstpPeripheryTransferPositionField
	{
		///仓位调拨流水号
		TTORATstpIntSerialType	PositionSerial;
	
		///仓位调拨请求流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///仓位调拨方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///今日买卖仓位转入转出数量
		TTORATstpVolumeType	TodayBSPos;
	
		///今日申赎仓位转入数量
		TTORATstpVolumeType	TodayPRPos;
	
		///昨日仓位转入数量
		TTORATstpVolumeType	HistoryPos;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///仓位调拨原因
		TTORATstpTransferReasonType	TransferReason;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///冲正日期
		TTORATstpDateType	RepealDate;
	
		///冲正时间
		TTORATstpTimeType	RepealTime;
	
		///冲正原因
		TTORATstpTransferReasonType	RepealReason;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///今日拆分合并仓位转入数量
		TTORATstpVolumeType	TodaySMPos;
	};
	
	/// 外围系统资金转移回报
	struct CTORATstpPeripheryTransferFundField
	{
		///资金调拨流水号
		TTORATstpIntSerialType	FundSerial;
	
		///资金调拨请求流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///资金调拨方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///转移金额
		TTORATstpMoneyType	Amount;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///资金调拨原因
		TTORATstpTransferReasonType	TransferReason;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///冲正日期
		TTORATstpDateType	RepealDate;
	
		///冲正时间
		TTORATstpTimeType	RepealTime;
	
		///冲正原因
		TTORATstpTransferReasonType	RepealReason;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	};
	
	/// 查询集中交易系统资金请求
	struct CTORATstpReqInquiryJZFundField
	{
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	};
	
	/// 查询集中交易系统资金响应
	struct CTORATstpRspInquiryJZFundField
	{
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///可用金额
		TTORATstpMoneyType	UsefulMoney;
	
		///可取额度
		TTORATstpMoneyType	FetchLimit;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	};
	
	/// 查询银行账户余额请求
	struct CTORATstpReqInquiryBankAccountFundField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///银行代码
		TTORATstpBankIDType	BankID;
	
		///银行密码
		TTORATstpPasswordType	BankPassword;
	};
	
	/// 查询银行账户余额响应
	struct CTORATstpRspInquiryBankAccountFundField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///银行代码
		TTORATstpBankIDType	BankID;
	
		///签约银行账户
		TTORATstpBankAccountIDType	BankAccountID;
	
		///账户余额
		TTORATstpMoneyType	Balance;
	};
	
	/// 交易通知
	struct CTORATstpTradingNoticeField
	{
		///通知流水号
		TTORATstpIntSerialType	NoticeSerial;
	
		///通知日期
		TTORATstpDateType	InsertDate;
	
		///通知时间
		TTORATstpTimeType	InsertTime;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///通知消息内容
		TTORATstpContentType	Content;
	
		///操作员
		TTORATstpUserIDType	OperatorID;
	};
	
	/// 查询最大报单量请求
	struct CTORATstpReqInquiryMaxOrderVolumeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///报单价格条件
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///最大委托手数
		TTORATstpVolumeType	MaxVolume;
	
		///指定偿还的信用负债编号（该字段置空表示不指定偿还）（两融专用）
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///头寸类型（两融专用）
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	};
	
	/// 查询最大报单量响应
	struct CTORATstpRspInquiryMaxOrderVolumeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///报单价格条件
		TTORATstpOrderPriceTypeType	OrderPriceType;
	
		///有效期类型
		TTORATstpTimeConditionType	TimeCondition;
	
		///成交量类型
		TTORATstpVolumeConditionType	VolumeCondition;
	
		///价格
		TTORATstpPriceType	LimitPrice;
	
		///港股通订单数量类型
		TTORATstpLotTypeType	LotType;
	
		///最大委托手数
		TTORATstpVolumeType	MaxVolume;
	
		///指定偿还的信用负债编号（该字段置空表示不指定偿还）（两融专用）
		TTORATstpCreditDebtIDType	CreditDebtID;
	
		///头寸类型（两融专用）
		TTORATstpCreditQuotaTypeType	CreditQuotaType;
	};
	
	/// 交易成交集中度
	struct CTORATstpInquiryTradeConcentrationField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///投资者成交量/成交总量
		TTORATstpRatioType	ConcentrationRatio1;
	
		///投资者成交金额/成交总金额
		TTORATstpRatioType	ConcentrationRatio2;
	};
	
	/// 请求修改开仓成本
	struct CTORATstpReqModifyOpenPosCostField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///开仓成本
		TTORATstpMoneyType	OpenPosCost;
	};
	
	/// 录入节点资金分配信息
	struct CTORATstpInputNodeFundAssignmentField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账号
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///节点编号1
		TTORATstpNodeIDType	NodeID1;
	
		///资金比例1
		TTORATstpRatioType	AmtRatio1;
	
		///节点编号2
		TTORATstpNodeIDType	NodeID2;
	
		///资金比例2
		TTORATstpRatioType	AmtRatio2;
	
		///节点编号3
		TTORATstpNodeIDType	NodeID3;
	
		///资金比例3
		TTORATstpRatioType	AmtRatio3;
	
		///节点编号4
		TTORATstpNodeIDType	NodeID4;
	
		///资金比例4
		TTORATstpRatioType	AmtRatio4;
	
		///节点编号5
		TTORATstpNodeIDType	NodeID5;
	
		///资金比例5
		TTORATstpRatioType	AmtRatio5;
	};
	
	/// 查询节点资金分配比例请求
	struct CTORATstpReqInquiryNodeFundAssignmentField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账号
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///节点编号
		TTORATstpNodeIDType	NodeID;
	};
	
	/// 查询节点资金分配比例响应
	struct CTORATstpRspInquiryNodeFundAssignmentField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账号
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///节点编号1
		TTORATstpNodeIDType	NodeID1;
	
		///资金比例1
		TTORATstpRatioType	AmtRatio1;
	
		///节点编号2
		TTORATstpNodeIDType	NodeID2;
	
		///资金比例2
		TTORATstpRatioType	AmtRatio2;
	
		///节点编号3
		TTORATstpNodeIDType	NodeID3;
	
		///资金比例3
		TTORATstpRatioType	AmtRatio3;
	
		///节点编号4
		TTORATstpNodeIDType	NodeID4;
	
		///资金比例4
		TTORATstpRatioType	AmtRatio4;
	
		///节点编号5
		TTORATstpNodeIDType	NodeID5;
	
		///资金比例5
		TTORATstpRatioType	AmtRatio5;
	};
	
	/// 查询交易所
	struct CTORATstpQryExchangeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	};
	
	/// 交易所
	struct CTORATstpExchangeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///交易所名称
		TTORATstpNameType	ExchangeName;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///数据同步状态
		TTORATstpDataSyncStatusType	DataSyncStatus;
	};
	
	/// 查询证券信息
	struct CTORATstpQrySecurityField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///产品代码
		TTORATstpProductIDType	ProductID;
	};
	
	/// 证券信息
	struct CTORATstpSecurityField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///证券名称(短)
		TTORATstpSecurityNameType	ShortSecurityName;
	
		///基础证券代码
		TTORATstpSecurityIDType	UnderlyingSecurityID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///产品代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别
		TTORATstpSecurityTypeType	SecurityType;
	
		///申报单位
		TTORATstpOrderUnitType	OrderUnit;
	
		///限价买入交易单位
		TTORATstpTradingUnitType	LimitBuyTradingUnit;
	
		///限价单买最大下单量
		TTORATstpVolumeType	MaxLimitOrderBuyVolume;
	
		///限价单买最小下单量
		TTORATstpVolumeType	MinLimitOrderBuyVolume;
	
		///限价卖出交易单位
		TTORATstpTradingUnitType	LimitSellTradingUnit;
	
		///限价单卖最大下单量
		TTORATstpVolumeType	MaxLimitOrderSellVolume;
	
		///限价单卖最小下单量
		TTORATstpVolumeType	MinLimitOrderSellVolume;
	
		///市价买入交易单位
		TTORATstpTradingUnitType	MarketBuyTradingUnit;
	
		///市价单买最大下单量
		TTORATstpVolumeType	MaxMarketOrderBuyVolume;
	
		///市价单买最小下单量
		TTORATstpVolumeType	MinMarketOrderBuyVolume;
	
		///市价卖出交易单位
		TTORATstpTradingUnitType	MarketSellTradingUnit;
	
		///市价单卖最大下单量
		TTORATstpVolumeType	MaxMarketOrderSellVolume;
	
		///市价单卖最小下单量
		TTORATstpVolumeType	MinMarketOrderSellVolume;
	
		///盘后定价买入交易单位
		TTORATstpTradingUnitType	FixPriceBuyTradingUnit;
	
		///盘后定价买最大下单量
		TTORATstpVolumeType	MaxFixPriceOrderBuyVolume;
	
		///盘后定价买最小下单量
		TTORATstpVolumeType	MinFixPriceOrderBuyVolume;
	
		///盘后定价卖出交易单位
		TTORATstpTradingUnitType	FixPriceSellTradingUnit;
	
		///盘后定价卖最大下单量
		TTORATstpVolumeType	MaxFixPriceOrderSellVolume;
	
		///盘后定价卖最小下单量
		TTORATstpVolumeType	MinFixPriceOrderSellVolume;
	
		///数量乘数
		TTORATstpVolumeMultipleType	VolumeMultiple;
	
		///最小变动价位
		TTORATstpPriceTickType	PriceTick;
	
		///上市日
		TTORATstpDateType	OpenDate;
	
		///面值
		TTORATstpParValueType	ParValue;
	
		///证券状态
		TTORATstpSecurityStatusType	SecurityStatus;
	
		///债券应计利息
		TTORATstpInterestType	BondInterest;
	
		///折算率
		TTORATstpRatioType	ConversionRate;
	
		///总股本
		TTORATstpLargeVolumeType	TotalEquity;
	
		///流通股本
		TTORATstpLargeVolumeType	CirculationEquity;
	
		///是否限制涨跌停板价
		TTORATstpBoolType	bPriceLimit;
	
		///昨收盘价
		TTORATstpPriceType	PreClosePrice;
	
		///涨停板价
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停板价
		TTORATstpPriceType	LowerLimitPrice;
	};
	
	/// 查询新股信息
	struct CTORATstpQryIPOInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///申购代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 新股信息
	struct CTORATstpIPOInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///申购代码
		TTORATstpSecurityIDType	SecurityID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///产品代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///最小申购价格
		TTORATstpPriceType	MinPrice;
	
		///币种代码
		TTORATstpCurrencyIDType	CurrencyID;
	
		///申购证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///新股证券代码
		TTORATstpSecurityIDType	UnderlyingSecurityID;
	
		///新股证券名称
		TTORATstpSecurityNameType	UnderlyingSecurityName;
	
		///网上申购最小数量
		TTORATstpVolumeType	MinVolume;
	
		///网上申购最大数量
		TTORATstpVolumeType	MaxVolume;
	
		///网上申购单位数量
		TTORATstpVolumeType	VolumeUnit;
	
		///发行方式
		TTORATstpIssueModeType	IssueMode;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///最大申购价格
		TTORATstpPriceType	MaxPrice;
	};
	
	/// 查询用户
	struct CTORATstpQryUserField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///用户类型
		TTORATstpUserTypeType	UserType;
	};
	
	/// 用户
	struct CTORATstpUserField
	{
		///用户代码
		TTORATstpUserIDType	UserID;
	
		///用户名称
		TTORATstpUserNameType	UserName;
	
		///用户类型
		TTORATstpUserTypeType	UserType;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///登录限制数
		TTORATstpLoginLimitType	LoginLimit;
	
		///登录状态
		TTORATstpLoginStatusType	LoginStatus;
	
		///开户日期
		TTORATstpDateType	OpenDate;
	
		///销户日期
		TTORATstpDateType	CloseDate;
	
		///报单流控
		TTORATstpCommFluxType	OrderInsertCommFlux;
	
		///撤单流控
		TTORATstpCommFluxType	OrderActionCommFlux;
	};
	
	/// 查询投资者
	struct CTORATstpQryInvestorField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	};
	
	/// 投资者
	struct CTORATstpInvestorField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资者类型
		TTORATstpInvestorTypeType	InvestorType;
	
		///投资者名称
		TTORATstpInvestorNameType	InvestorName;
	
		///证件类型
		TTORATstpIdCardTypeType	IdCardType;
	
		///证件号码
		TTORATstpIdCardNoType	IdCardNo;
	
		///开户日期
		TTORATstpDateType	OpenDate;
	
		///销户日期
		TTORATstpDateType	CloseDate;
	
		///交易状态
		TTORATstpTradingStatusType	TradingStatus;
	
		///委托方式
		TTORATstpOperwaysType	Operways;
	
		///手机
		TTORATstpMobileType	Mobile;
	
		///联系电话
		TTORATstpTelephoneType	Telephone;
	
		///电子邮件
		TTORATstpEmailType	Email;
	
		///传真
		TTORATstpFaxType	Fax;
	
		///通讯地址
		TTORATstpAddressType	Address;
	
		///邮政编码
		TTORATstpZipCodeType	ZipCode;
	
		///专业投资者类别
		TTORATstpProfInvestorTypeType	ProfInvestorType;
	
		///套餐类型(两融专用)
		TTORATstpPlanTypeType	PlanType;
	
		///是否允许投资者自切套餐(两融专用)
		TTORATstpBoolType	AllowSelfSwitchPlan;
	
		///备注
		TTORATstpRemarkType	Remark;
	};
	
	/// 查询交易股东账户
	struct CTORATstpQryShareholderAccountField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///股东账户类型
		TTORATstpShareholderIDTypeType	ShareholderIDType;
	};
	
	/// 交易股东账户
	struct CTORATstpShareholderAccountField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///客户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///股东账户类型
		TTORATstpShareholderIDTypeType	ShareholderIDType;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///普通买卖白名单控制标志(两融专用)
		TTORATstpBoolType	BSWhiteListCtl;
	
		///主账户标识
		TTORATstpBoolType	MainFlag;
	};
	
	/// 查询配股配债信息
	struct CTORATstpQryRationalInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///配股配债代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 配股配债信息
	struct CTORATstpRationalInfoField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///申购代码
		TTORATstpSecurityIDType	SecurityID;
	
		///配股配债价格
		TTORATstpPriceType	Price;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券品种代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///配股名称
		TTORATstpSecurityNameType	SecurityName;
	
		///基础证券代码
		TTORATstpSecurityIDType	UnderlyingSecurityID;
	
		///基础证券名称
		TTORATstpSecurityNameType	UnderlyingSecurityName;
	
		///配股配债最小数量
		TTORATstpVolumeType	MinVolume;
	
		///配股配债最大数量
		TTORATstpVolumeType	MaxVolume;
	
		///配股配债单位数量
		TTORATstpVolumeType	VolumeUnit;
	};
	
	/// 查询报单
	struct CTORATstpQryOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeStart;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeEnd;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///是否可撤
		TTORATstpBoolType	IsCancel;
	};
	
	/// 查询撤单
	struct CTORATstpQryOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///本地撤单编号
		TTORATstpOrderLocalIDType	CancelOrderLocalID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 撤单
	struct CTORATstpOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///被撤报单前置编号
		TTORATstpFrontIDType	FrontID;
	
		///被撤报单会话编号
		TTORATstpSessionIDType	SessionID;
	
		///被撤报单引用
		TTORATstpOrderRefType	OrderRef;
	
		///被撤报单系统编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///本地撤单编号
		TTORATstpOrderLocalIDType	CancelOrderLocalID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///被撤报单本地编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///操作用户
		TTORATstpUserIDType	ActionUser;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///操作日期
		TTORATstpDateType	ActionDate;
	
		///操作时间
		TTORATstpTimeType	ActionTime;
	
		///撤单状态
		TTORATstpCancelOrderStatusType	CancelOrderStatus;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///撤单前置编号
		TTORATstpFrontIDType	ActionFrontID;
	
		///撤单会话编号
		TTORATstpSessionIDType	ActionSessionID;
	
		///撤单引用
		TTORATstpOrderRefType	OrderActionRef;
	
		///本地撤单系统编号
		TTORATstpOrderSysIDType	CancelOrderSysID;
	
		///撤单类型
		TTORATstpCancelOrderTypeType	CancelOrderType;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 查询成交
	struct CTORATstpQryTradeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	
		///Insert Time
		TTORATstpTimeType	TradeTimeStart;
	
		///Insert Time
		TTORATstpTimeType	TradeTimeEnd;
	};
	
	/// 查询资金账户
	struct CTORATstpQryTradingAccountField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///资金账户类型
		TTORATstpAccountTypeType	AccountType;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	};
	
	/// 资金账户
	struct CTORATstpTradingAccountField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种代码
		TTORATstpCurrencyIDType	CurrencyID;
	
		///上日结存
		TTORATstpMoneyType	PreDeposit;
	
		///可用资金
		TTORATstpMoneyType	UsefulMoney;
	
		///可取资金
		TTORATstpMoneyType	FetchLimit;
	
		///上日未交收金额(港股通专用字段)
		TTORATstpMoneyType	PreUnDeliveredMoney;
	
		///可用未交收金额(港股通专用字段)
		TTORATstpMoneyType	UnDeliveredMoney;
	
		///当日入金金额
		TTORATstpMoneyType	Deposit;
	
		///当日出金金额
		TTORATstpMoneyType	Withdraw;
	
		///冻结的资金(港股通该字段不包括未交收部分冻结资金)
		TTORATstpMoneyType	FrozenCash;
	
		///冻结未交收金额(港股通专用)
		TTORATstpMoneyType	UnDeliveredFrozenCash;
	
		///冻结的手续费(港股通该字段不包括未交收部分冻结手续费)
		TTORATstpMoneyType	FrozenCommission;
	
		///冻结未交收手续费(港股通专用)
		TTORATstpMoneyType	UnDeliveredFrozenCommission;
	
		///手续费(港股通该字段不包括未交收部分手续费)
		TTORATstpMoneyType	Commission;
	
		///占用未交收手续费(港股通专用)
		TTORATstpMoneyType	UnDeliveredCommission;
	
		///资金账户类型
		TTORATstpAccountTypeType	AccountType;
	
		///资金账户所属投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///银行代码
		TTORATstpBankIDType	BankID;
	
		///银行账户
		TTORATstpBankAccountIDType	BankAccountID;
	
		///权利金收入(两融专用)
		TTORATstpMoneyType	RoyaltyIn;
	
		///权利金支出(两融专用)
		TTORATstpMoneyType	RoyaltyOut;
	
		///融券卖出金额(两融专用)
		TTORATstpMoneyType	CreditSellAmount;
	
		///融券卖出使用金额(用于偿还融资负债或买特殊品种的金额)(两融专用)
		TTORATstpMoneyType	CreditSellUseAmount;
	
		///虚拟资产(两融专用)
		TTORATstpMoneyType	VirtualAssets;
	
		///融券卖出金额冻结(用于偿还融资负债或买特殊品种的未成交冻结金额)(两融专用)
		TTORATstpMoneyType	CreditSellFrozenAmount;
	};
	
	/// 查询投资者持仓
	struct CTORATstpQryPositionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	};
	
	/// 投资者持仓
	struct CTORATstpPositionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///昨仓
		TTORATstpVolumeType	HistoryPos;
	
		///昨仓冻结
		TTORATstpVolumeType	HistoryPosFrozen;
	
		///今买卖仓
		TTORATstpVolumeType	TodayBSPos;
	
		///今买卖仓冻结
		TTORATstpVolumeType	TodayBSPosFrozen;
	
		///今日申赎持仓
		TTORATstpVolumeType	TodayPRPos;
	
		///今日申赎持仓冻结
		TTORATstpVolumeType	TodayPRPosFrozen;
	
		///今拆分合并持仓
		TTORATstpVolumeType	TodaySMPos;
	
		///今拆分合并持仓冻结
		TTORATstpVolumeType	TodaySMPosFrozen;
	
		///昨仓成本价
		TTORATstpPriceType	HistoryPosPrice;
	
		///持仓成本
		TTORATstpMoneyType	TotalPosCost;
	
		///上次余额(盘中不变)
		TTORATstpVolumeType	PrePosition;
	
		///股份可用
		TTORATstpVolumeType	AvailablePosition;
	
		///股份余额
		TTORATstpVolumeType	CurrentPosition;
	
		///开仓成本
		TTORATstpMoneyType	OpenPosCost;
	
		///融资仓位(两融专用)
		TTORATstpVolumeType	CreditBuyPos;
	
		///融券仓位(两融专用)
		TTORATstpVolumeType	CreditSellPos;
	
		///今日融券仓位(两融专用)
		TTORATstpVolumeType	TodayCreditSellPos;
	
		///划出仓位(两融专用)
		TTORATstpVolumeType	CollateralOutPos;
	
		///还券未成交数量(两融专用)
		TTORATstpVolumeType	RepayUntradeVolume;
	
		///直接还券未成交数量(两融专用)
		TTORATstpVolumeType	RepayTransferUntradeVolume;
	
		///担保品买入未成交金额(两融专用)
		TTORATstpMoneyType	CollateralBuyUntradeAmount;
	
		///担保品买入未成交数量(两融专用)
		TTORATstpVolumeType	CollateralBuyUntradeVolume;
	
		///融资买入金额(包含交易费用)(两融专用)
		TTORATstpMoneyType	CreditBuyAmount;
	
		///融资买入未成交金额(包含交易费用)(两融专用)
		TTORATstpMoneyType	CreditBuyUntradeAmount;
	
		///融资冻结保证金(两融专用)
		TTORATstpMoneyType	CreditBuyFrozenMargin;
	
		///融资买入利息(两融专用)
		TTORATstpMoneyType	CreditBuyInterestFee;
	
		///融资买入未成交数量(两融专用)
		TTORATstpVolumeType	CreditBuyUntradeVolume;
	
		///融券卖出金额(以成交价计算)(两融专用)
		TTORATstpMoneyType	CreditSellAmount;
	
		///融券卖出未成交金额(两融专用)
		TTORATstpMoneyType	CreditSellUntradeAmount;
	
		///融券冻结保证金(两融专用)
		TTORATstpMoneyType	CreditSellFrozenMargin;
	
		///融券卖出息费(两融专用)
		TTORATstpMoneyType	CreditSellInterestFee;
	
		///融券卖出未成交数量(两融专用)
		TTORATstpVolumeType	CreditSellUntradeVolume;
	
		///划入待收仓(两融专用)
		TTORATstpVolumeType	CollateralInPos;
	
		///融资流动冻结保证金(两融专用)
		TTORATstpMoneyType	CreditBuyFrozenCirculateMargin;
	
		///融券流动冻结保证金(两融专用)
		TTORATstpMoneyType	CreditSellFrozenCirculateMargin;
	
		///累计平仓盈亏(两融专用)
		TTORATstpMoneyType	CloseProfit;
	
		///当日累计开仓数量(两融专用)
		TTORATstpVolumeType	TodayTotalOpenVolume;
	
		///今手续费
		TTORATstpMoneyType	TodayCommission;
	};
	
	/// 查询基础交易费率
	struct CTORATstpQryTradingFeeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	};
	
	/// 基础交易费率
	struct CTORATstpTradingFeeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///产品代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///业务类别
		TTORATstpBizClassType	BizClass;
	
		///印花税(港股印花税)按金额收取比例
		TTORATstpRatioType	StampTaxRatioByAmt;
	
		///印花税(港股印花税)按面值收取比例
		TTORATstpRatioType	StampTaxRatioByPar;
	
		///印花税(港股印花税,单位港币)按笔收取金额
		TTORATstpMoneyType	StampTaxFeePerOrder;
	
		///印花税(港股印花税,单位港币)按数量收取金额
		TTORATstpMoneyType	StampTaxFeeByVolume;
	
		///印花税(港股印花税,单位港币)最低收取金额
		TTORATstpMoneyType	StampTaxFeeMin;
	
		///印花税(港股印花税,单位港币)最高收取金额
		TTORATstpMoneyType	StampTaxFeeMax;
	
		///过户费(港股证券组合费)按金额收取比例
		TTORATstpRatioType	TransferRatioByAmt;
	
		///过户费(港股证券组合费)按面值收取比例
		TTORATstpRatioType	TransferRatioByPar;
	
		///过户费(港股证券组合费,单位港币)按笔收取金额
		TTORATstpMoneyType	TransferFeePerOrder;
	
		///过户费(港股证券组合费,单位港币)按数量收取金额
		TTORATstpMoneyType	TransferFeeByVolume;
	
		///过户费(港股证券组合费,单位港币)最低收取金额
		TTORATstpMoneyType	TransferFeeMin;
	
		///过户费(港股证券组合费,单位港币)最高收取金额
		TTORATstpMoneyType	TransferFeeMax;
	
		///经手费(港股交易费)按金额收取比例
		TTORATstpRatioType	HandlingRatioByAmt;
	
		///经手费(港股交易费)按面值收取比例
		TTORATstpRatioType	HandlingRatioByPar;
	
		///经手费(港股交易费,单位港币)按笔收取金额
		TTORATstpMoneyType	HandlingFeePerOrder;
	
		///经手费(港股交易费,单位港币)按数量收取金额
		TTORATstpMoneyType	HandlingFeeByVolume;
	
		///经手费(港股交易费,单位港币)最低收取金额
		TTORATstpMoneyType	HandlingFeeMin;
	
		///经手费(港股交易费,单位港币)最高收取金额
		TTORATstpMoneyType	HandlingFeeMax;
	
		///证管费(港股交易征费)按金额收取比例
		TTORATstpRatioType	RegulateRatioByAmt;
	
		///证管费(港股交易征费)按面值收取比例
		TTORATstpRatioType	RegulateRatioByPar;
	
		///证管费(港股交易征费,单位港币)按笔收取金额
		TTORATstpMoneyType	RegulateFeePerOrder;
	
		///证管费(港股交易征费,单位港币)按数量收取金额
		TTORATstpMoneyType	RegulateFeeByVolume;
	
		///证管费(港股交易征费,单位港币)最低收取金额
		TTORATstpMoneyType	RegulateFeeMin;
	
		///证管费(港股交易征费,单位港币)最高收取金额
		TTORATstpMoneyType	RegulateFeeMax;
	
		///结算费(港股股份交收费)按金额收取比例
		TTORATstpRatioType	SettlementRatioByAmt;
	
		///结算费(港股股份交收费)按面值收取比例
		TTORATstpRatioType	SettlementRatioByPar;
	
		///结算费(港股股份交收费,单位港币)按笔收取金额
		TTORATstpMoneyType	SettlementFeePerOrder;
	
		///结算费(港股股份交收费,单位港币)按数量收取金额
		TTORATstpMoneyType	SettlementFeeByVolume;
	
		///结算费(港股股份交收费,单位港币)最低收取金额
		TTORATstpMoneyType	SettlementFeeMin;
	
		///结算费(港股股份交收费,单位港币)最高收取金额
		TTORATstpMoneyType	SettlementFeeMax;
	};
	
	/// 查询佣金费率
	struct CTORATstpQryInvestorTradingFeeField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	};
	
	/// 佣金费率
	struct CTORATstpInvestorTradingFeeField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///产品代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///合约代码
		TTORATstpSecurityIDType	SecurityID;
	
		///业务类别
		TTORATstpBizClassType	BizClass;
	
		///佣金类型
		TTORATstpBrokerageTypeType	BrokerageType;
	
		///佣金按金额收取比例
		TTORATstpRatioType	RatioByAmt;
	
		///佣金按面值收取比例
		TTORATstpRatioType	RatioByPar;
	
		///佣金按笔收取金额
		TTORATstpMoneyType	FeePerOrder;
	
		///佣金最低收取金额
		TTORATstpMoneyType	FeeMin;
	
		///佣金最高收取金额
		TTORATstpMoneyType	FeeMax;
	
		///佣金按数量收取金额
		TTORATstpMoneyType	FeeByVolume;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///报单类型
		TTORATstpOrderTypeType	OrderType;
	};
	
	/// 查询新股申购额度
	struct CTORATstpQryIPOQuotaField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	};
	
	/// 新股申购额度
	struct CTORATstpIPOQuotaField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///客户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///可申购额度
		TTORATstpLongVolumeType	MaxVolume;
	
		///科创板可申购额度
		TTORATstpLongVolumeType	KCMaxVolume;
	};
	
	/// 查询报单明细资金
	struct CTORATstpQryOrderFundDetailField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	};
	
	/// 报单明细资金
	struct CTORATstpOrderFundDetailField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///报单金额
		TTORATstpMoneyType	OrderAmount;
	
		///成交金额
		TTORATstpMoneyType	Turnover;
	
		///印花税(港股为印花税)
		TTORATstpMoneyType	StampTaxFee;
	
		///经手费(港股为交易费)
		TTORATstpMoneyType	HandlingFee;
	
		///过户费(港股为证券组合费)
		TTORATstpMoneyType	TransferFee;
	
		///证管费(港股为交易征费)
		TTORATstpMoneyType	RegulateFee;
	
		///结算费(港股为股份交收费)
		TTORATstpMoneyType	SettlementFee;
	
		///佣金
		TTORATstpMoneyType	BrokerageFee;
	
		///总费用
		TTORATstpMoneyType	TotalFee;
	
		///报单初始冻结金额
		TTORATstpMoneyType	OrderCashFrozen;
	
		///预估冻结现金
		TTORATstpMoneyType	EstimateCashFrozen;
	
		///初始冻结总费用
		TTORATstpMoneyType	TotalFeeFrozen;
	
		///初始冻结金额合计
		TTORATstpMoneyType	TotalFrozen;
	
		///保证金(两融专用)
		TTORATstpMoneyType	Margin;
	
		///偿还金额(两融专用)
		TTORATstpMoneyType	RepayAmount;
	
		///偿还数量(两融专用)
		TTORATstpVolumeType	RepayVolume;
	
		///初始冻结保证金(两融专用)
		TTORATstpMoneyType	MarginFrozen;
	
		///初始冻结流动保证金(两融专用)
		TTORATstpMoneyType	CirculateMarginFrozen;
	};
	
	/// 查询资金转移流水
	struct CTORATstpQryFundTransferDetailField
	{
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	};
	
	/// 资金转移流水
	struct CTORATstpFundTransferDetailField
	{
		///转账流水号
		TTORATstpIntSerialType	FundSerial;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///出入金金额
		TTORATstpMoneyType	Amount;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作来源
		TTORATstpOperateSourceType	OperateSource;
	
		///操作人员
		TTORATstpUserIDType	OperatorID;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///银行代码
		TTORATstpBankIDType	BankID;
	
		///签约银行账户
		TTORATstpBankAccountIDType	BankAccountID;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///外部流水号
		TTORATstpExternalSerialType	ExternalSerial;
	
		///外部系统节点号
		TTORATstpNodeIDType	ExternalNodeID;
	
		///强平原因(两融专用)
		TTORATstpForceCloseReasonType	ForceCloseReason;
	
		///(直接还款用)指定偿还时的负债编号(两融专用)
		TTORATstpCreditDebtIDType	CreditDebtID;
	};
	
	/// 查询持仓转移流水
	struct CTORATstpQryPositionTransferDetailField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	};
	
	/// 持仓转移流水
	struct CTORATstpPositionTransferDetailField
	{
		///流水号
		TTORATstpIntSerialType	PositionSerial;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///交易账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易日期
		TTORATstpDateType	TradingDay;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///转移持仓类型
		TTORATstpTransferPositionTypeType	TransferPositionType;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///昨日仓位数量
		TTORATstpVolumeType	HistoryVolume;
	
		///今日买卖仓位数量
		TTORATstpVolumeType	TodayBSVolume;
	
		///今日申赎仓位数量
		TTORATstpVolumeType	TodayPRVolume;
	
		///今日拆分合并仓位数量
		TTORATstpVolumeType	TodaySMVolume;
	
		///操作人员
		TTORATstpUserIDType	OperatorID;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	
		///外部节点编号
		TTORATstpNodeIDType	ExternalNodeID;
	};
	
	/// 查询外围系统仓位调拨流水
	struct CTORATstpQryPeripheryPositionTransferDetailField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///仓位调拨方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	};
	
	/// 外围系统仓位调拨流水
	struct CTORATstpPeripheryPositionTransferDetailField
	{
		///仓位调拨系统流水号
		TTORATstpIntSerialType	PositionSerial;
	
		///仓位调拨请求流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///仓位调拨方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///现货系统投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///现货系统投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///现货系统交易账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///今日买卖仓位转入转出数量
		TTORATstpVolumeType	TodayBSPos;
	
		///今日申赎仓位转入数量
		TTORATstpVolumeType	TodayPRPos;
	
		///今日拆分合并仓位转入数量
		TTORATstpVolumeType	TodaySMPos;
	
		///昨日仓位转入数量
		TTORATstpVolumeType	HistoryPos;
	
		///交易日期
		TTORATstpDateType	TradingDay;
	
		///仓位调拨原因
		TTORATstpTransferReasonType	TransferReason;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///冲正日期
		TTORATstpDateType	RepealDate;
	
		///冲正时间
		TTORATstpTimeType	RepealTime;
	
		///冲正原因
		TTORATstpTransferReasonType	RepealReason;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	};
	
	/// 查询外围系统资金转移流水
	struct CTORATstpQryPeripheryFundTransferDetailField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///资金调拨方向
		TTORATstpTransferDirectionType	TransferDirection;
	};
	
	/// 外围系统资金转移流水
	struct CTORATstpPeripheryFundTransferDetailField
	{
		///转账流水号
		TTORATstpIntSerialType	FundSerial;
	
		///申请流水号
		TTORATstpIntSerialType	ApplySerial;
	
		///前置编号
		TTORATstpFrontIDType	FrontID;
	
		///会话编号
		TTORATstpSessionIDType	SessionID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///资金账户代码
		TTORATstpAccountIDType	AccountID;
	
		///币种
		TTORATstpCurrencyIDType	CurrencyID;
	
		///转移方向
		TTORATstpTransferDirectionType	TransferDirection;
	
		///出入金金额
		TTORATstpMoneyType	Amount;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///转移状态
		TTORATstpTransferStatusType	TransferStatus;
	
		///资金调拨原因
		TTORATstpTransferReasonType	TransferReason;
	
		///操作日期
		TTORATstpDateType	OperateDate;
	
		///操作时间
		TTORATstpTimeType	OperateTime;
	
		///冲正日期
		TTORATstpDateType	RepealDate;
	
		///冲正时间
		TTORATstpTimeType	RepealTime;
	
		///冲正原因
		TTORATstpTransferReasonType	RepealReason;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	};
	
	/// 查询债券转股信息
	struct CTORATstpQryBondConversionInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 债券转股信息
	struct CTORATstpBondConversionInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///转股申报代码
		TTORATstpSecurityIDType	ConvertOrderID;
	
		///转股价格
		TTORATstpPriceType	ConvertPrice;
	
		///每次可以转股最小交易单位
		TTORATstpTradingUnitType	ConvertVolUnit;
	
		///证券可以转股的最大数量
		TTORATstpVolumeType	ConvertVolMax;
	
		///证券可以转股的最小数量
		TTORATstpVolumeType	ConvertVolMin;
	
		///转股开始日期
		TTORATstpDateType	BeginDate;
	
		///转股截至日期
		TTORATstpDateType	EndDate;
	
		///转股名称
		TTORATstpSecurityNameType	ConvertName;
	};
	
	/// 查询债券回售信息
	struct CTORATstpQryBondPutbackInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 债券回售信息
	struct CTORATstpBondPutbackInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///回售申报代码
		TTORATstpSecurityIDType	PutbackOrderID;
	
		///回售名称
		TTORATstpSecurityNameType	PutbackName;
	
		///回售价格
		TTORATstpPriceType	PutbackPrice;
	
		///每次可以回售最小交易单位
		TTORATstpTradingUnitType	PutbackVolUnit;
	
		///债券可以回售的最大数量
		TTORATstpVolumeType	PutbackVolMax;
	
		///债券可以回售的最小数量
		TTORATstpVolumeType	PutbackVolMin;
	
		///回售开始日期
		TTORATstpDateType	PutbackBeginDate;
	
		///回售结束日期
		TTORATstpDateType	PutbackEndDate;
	
		///回售撤销开始日期
		TTORATstpDateType	RelieveBeginDate;
	
		///回售撤销结束日期
		TTORATstpDateType	RelieveEndDate;
	};
	
	/// 查询投资者条件单限制参数
	struct CTORATstpQryInvestorCondOrderLimitParamField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	};
	
	/// 投资者条件单限制参数
	struct CTORATstpInvestorCondOrderLimitParamField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///最大条件单数
		TTORATstpVolumeType	MaxCondOrderLimitCnt;
	
		///当前条件单数
		TTORATstpVolumeType	CurrCondOrderCnt;
	};
	
	/// 查询条件单
	struct CTORATstpQryConditionOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///报单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeStart;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeEnd;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 查询条件单撤单
	struct CTORATstpQryCondOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///被撤条件单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///条件单撤单编号
		TTORATstpCondOrderIDType	CancelCondOrderID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 条件单撤单
	struct CTORATstpCondOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///被撤条件单前置编号
		TTORATstpFrontIDType	FrontID;
	
		///被撤条件单会话编号
		TTORATstpSessionIDType	SessionID;
	
		///被撤条件单引用
		TTORATstpOrderRefType	CondOrderRef;
	
		///被撤条件单编号
		TTORATstpCondOrderIDType	CondOrderID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///条件单操作引用
		TTORATstpOrderRefType	CondOrderActionRef;
	
		///条件单撤单编号
		TTORATstpCondOrderIDType	CancelCondOrderID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///撤单前置编号
		TTORATstpFrontIDType	ActionFrontID;
	
		///撤单会话编号
		TTORATstpSessionIDType	ActionSessionID;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///操作用户
		TTORATstpUserIDType	ActionUser;
	
		///操作日期
		TTORATstpDateType	ActionDate;
	
		///操作时间
		TTORATstpTimeType	ActionTime;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 查询交易通知
	struct CTORATstpQryTradingNoticeField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///Insert Date
		TTORATstpDateType	InsertDateStart;
	
		///Insert Date
		TTORATstpDateType	InsertDateEnd;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeStart;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeEnd;
	};
	
	/// 查询新股申购配号结果
	struct CTORATstpQryIPONumberResultField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///申购代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///股东代码
		TTORATstpShareholderIDType	ShareholderID;
	};
	
	/// 新股申购配号结果
	struct CTORATstpIPONumberResultField
	{
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///日期
		TTORATstpDateType	Day;
	
		///申购证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///起始配号
		TTORATstpIPONumberIDType	BeginNumberID;
	
		///配号数量
		TTORATstpVolumeType	Volume;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	};
	
	/// 查询新股申购中签结果
	struct CTORATstpQryIPOMatchNumberResultField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///申购代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///股东代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///中签配号
		TTORATstpIPONumberIDType	MatchNumberID;
	};
	
	/// 新股申购中签结果
	struct CTORATstpIPOMatchNumberResultField
	{
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///中签配号
		TTORATstpIPONumberIDType	MatchNumberID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///日期
		TTORATstpDateType	Day;
	
		///申购证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///此中签号拥有的证券数量
		TTORATstpVolumeType	Volume;
	
		///申购价格
		TTORATstpPriceType	Price;
	
		///申购金额
		TTORATstpMoneyType	Amout;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	};
	
	/// 查询交易协议
	struct CTORATstpQryShareholderSpecPrivilegeField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///特殊权限类别
		TTORATstpSpecPrivilegeTypeType	SpecPrivilegeType;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	};
	
	/// 交易协议
	struct CTORATstpShareholderSpecPrivilegeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///交易账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///特殊权限类别
		TTORATstpSpecPrivilegeTypeType	SpecPrivilegeType;
	
		///报单类别
		TTORATstpDirectionType	Direction;
	
		///是否禁止
		TTORATstpBoolType	bForbidden;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	};
	
	/// 查询市场
	struct CTORATstpQryMarketField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	};
	
	/// 市场
	struct CTORATstpMarketField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///市场名称
		TTORATstpNameType	MarketName;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场状态
		TTORATstpMarketStatusType	MarketStatus;
	};
	
	/// 查询ETF清单信息
	struct CTORATstpQryETFFileField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///ETF二级市场交易代码
		TTORATstpSecurityIDType	ETFSecurityID;
	
		///ETF一级市场申赎代码
		TTORATstpSecurityIDType	ETFCreRedSecurityID;
	};
	
	/// ETF清单信息
	struct CTORATstpETFFileField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///ETF交易代码
		TTORATstpSecurityIDType	ETFSecurityID;
	
		///ETF申赎代码
		TTORATstpSecurityIDType	ETFCreRedSecurityID;
	
		///最小申购赎回单位份数
		TTORATstpVolumeType	CreationRedemptionUnit;
	
		///最大现金替代比例
		TTORATstpRatioType	Maxcashratio;
	
		///预估现金差额
		TTORATstpMoneyType	EstimateCashComponent;
	
		///前一交易日现金差额
		TTORATstpMoneyType	CashComponent;
	
		///前一交易日基金单位净值
		TTORATstpMoneyType	NAV;
	
		///前一交易日申赎基准单位净值
		TTORATstpMoneyType	NAVperCU;
	
		///当日申购赎回基准单位的红利金额
		TTORATstpMoneyType	DividendPerCU;
	
		///ETF申赎类型
		TTORATstpCreRedTypeType	ETFCreRedType;
	
		///ETF证券名称
		TTORATstpSecurityNameType	ETFSecurityName;
	};
	
	/// 查询ETF成份证券信息
	struct CTORATstpQryETFBasketField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///ETF二级市场交易代码
		TTORATstpSecurityIDType	ETFSecurityID;
	
		///ETF成份证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// ETF成份证券信息
	struct CTORATstpETFBasketField
	{
		///交易日
		TTORATstpDateType	TradingDay;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///ETF交易代码
		TTORATstpSecurityIDType	ETFSecurityID;
	
		///ETF成份证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///成分证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///成分证券数量
		TTORATstpVolumeType	Volume;
	
		///现金替代标志
		TTORATstpETFCurrenceReplaceStatusType	ETFCurrenceReplaceStatus;
	
		///溢价比例
		TTORATstpRatioType	Premium;
	
		///申购替代金额
		TTORATstpMoneyType	CreationReplaceAmount;
	
		///赎回替代金额
		TTORATstpMoneyType	RedemptionReplaceAmount;
	
		///挂牌市场
		TTORATstpMarketIDType	MarketID;
	
		///ETF申赎类型
		TTORATstpCreRedTypeType	ETFCreRedType;
	};
	
	/// 查询投资者限仓信息
	struct CTORATstpQryInvestorPositionLimitField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 投资者限仓信息
	struct CTORATstpInvestorPositionLimitField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///当日买入额度
		TTORATstpVolumeType	BuyLimit;
	
		///已使用当日买入额度
		TTORATstpVolumeType	BuyFrozen;
	
		///当日卖出额度
		TTORATstpVolumeType	SellLimit;
	
		///已使用当日卖出额度
		TTORATstpVolumeType	SellFrozen;
	
		///当日申购额度
		TTORATstpVolumeType	PurchaseLimit;
	
		///已使用当日申购额度
		TTORATstpVolumeType	PurchaseFrozen;
	
		///当日赎回额度
		TTORATstpVolumeType	RedeemLimit;
	
		///已使用当日赎回额度
		TTORATstpVolumeType	RedeemFrozen;
	
		///当日质押入库额度
		TTORATstpVolumeType	PledgeInLimit;
	
		///已使用当日质押入库额度
		TTORATstpVolumeType	PledgeInFrozen;
	
		///当日质押出库额度
		TTORATstpVolumeType	PledgeOutLimit;
	
		///已使用当日质押出库额度
		TTORATstpVolumeType	PledgeOutFrozen;
	
		///当日债转股额度
		TTORATstpVolumeType	ConvertLimit;
	
		///已使用当日债转股额度
		TTORATstpVolumeType	ConvertFrozen;
	
		///当日债券回售额度
		TTORATstpVolumeType	PutbackLimit;
	
		///已使用当日债券回售额度
		TTORATstpVolumeType	PutbackFrozen;
	
		///当日配股配债额度
		TTORATstpVolumeType	RationalLimit;
	
		///已使用当日配股配债额度
		TTORATstpVolumeType	RationalFrozen;
	
		///总持仓额度
		TTORATstpVolumeType	TotalPositionLimit;
	
		///已使用总持仓额度
		TTORATstpVolumeType	TotalPositionFrozen;
	
		///当日拆分额度
		TTORATstpVolumeType	SplitLimit;
	
		///已使用当日拆分额度
		TTORATstpVolumeType	SplitFrozen;
	
		///当日合并额度
		TTORATstpVolumeType	MergeLimit;
	
		///已使用当日合并额度
		TTORATstpVolumeType	MergeFrozen;
	};
	
	/// 查询深港通国际市场互联状态信息
	struct CTORATstpQrySZSEImcParamsField
	{
		///市场代码
		TTORATstpMarketIDType	MarketID;
	};
	
	/// 深港通国际市场互联状态信息
	struct CTORATstpSZSEImcParamsField
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
	
	/// 查询深港通国际市场互联汇率信息
	struct CTORATstpQrySZSEImcExchangeRateField
	{
		///源货币币种
		TTORATstpCurrencyIDType	FromCurrency;
	
		///目标货币币种
		TTORATstpCurrencyIDType	ToCurrency;
	};
	
	/// 深港通国际市场互联汇率信息
	struct CTORATstpSZSEImcExchangeRateField
	{
		///源货币币种
		TTORATstpCurrencyIDType	FromCurrency;
	
		///目标货币币种
		TTORATstpCurrencyIDType	ToCurrency;
	
		///参考汇率买入价
		TTORATstpPriceType	BidRate;
	
		///参考汇率卖出价
		TTORATstpPriceType	OfferRate;
	
		///参考汇率中间价
		TTORATstpPriceType	MidPointRate;
	};
	
	/// 查询深港通最小价差信息
	struct CTORATstpQrySZSEHKPriceTickInfoField
	{
		///价差品种
		TTORATstpPriceTickIDType	PriceTickID;
	};
	
	/// 深港通最小价差信息
	struct CTORATstpSZSEHKPriceTickInfoField
	{
		///价差品种
		TTORATstpPriceTickIDType	PriceTickID;
	
		///价差组号
		TTORATstpPriceTickGroupIDType	PriceTickGroupID;
	
		///价差类别
		TTORATstpPriceTickTypeType	PriceTickType;
	
		///价差组起始价格
		TTORATstpPriceType	BeginPrice;
	
		///价差组结束价格
		TTORATstpPriceType	EndPrice;
	
		///价差值
		TTORATstpPriceType	PriceTick;
	};
	
	/// 查询LOF基金信息
	struct CTORATstpQryLofFundInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///基金代码
		TTORATstpSecurityIDType	FundID;
	
		///主基金代码
		TTORATstpSecurityIDType	MainFundID;
	};
	
	/// LOF基金信息
	struct CTORATstpLofFundInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///基金代码
		TTORATstpSecurityIDType	FundID;
	
		///主基金代码
		TTORATstpSecurityIDType	MainFundID;
	
		///基金类型
		TTORATstpFundTypeType	FundType;
	
		///拆分数量单位
		TTORATstpVolumeType	SplitUnit;
	
		///最小拆分数量
		TTORATstpVolumeType	SplitMinVol;
	
		///合并数量单位
		TTORATstpVolumeType	MergeUnit;
	
		///最小合并数量
		TTORATstpVolumeType	MergeMinVol;
	
		///基金转换系数
		TTORATstpRatioType	FundRatio;
	};
	
	/// 查询投资者质押持仓
	struct CTORATstpQryPledgePositionField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	};
	
	/// 投资者质押持仓
	struct CTORATstpPledgePositionField
	{
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///客户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///昨日质押持仓
		TTORATstpVolumeType	HisPledgePos;
	
		///昨日质押持仓冻结
		TTORATstpVolumeType	HisPledgePosFrozen;
	
		///今日入库的质押持仓
		TTORATstpVolumeType	TodayPledgePos;
	
		///今日入库的质押持仓冻结
		TTORATstpVolumeType	TodayPledgePosFrozen;
	
		///昨日质押入库的现券总量
		TTORATstpVolumeType	PreTotalPledgePos;
	
		///昨日质押入库的现券可用数量
		TTORATstpVolumeType	PreAvailablePledgePos;
	};
	
	/// 查询证券质押信息
	struct CTORATstpQryPledgeInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 证券质押信息
	struct CTORATstpPledgeInfoField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///质押申报代码
		TTORATstpSecurityIDType	PledgeOrderID;
	
		///质押名称
		TTORATstpSecurityNameType	PledgeName;
	
		///标准券代码
		TTORATstpSecurityIDType	StandardBondID;
	
		///标准券折算率/折算值
		TTORATstpRatioType	ConversionRate;
	
		///每次可以入库的最小交易单位
		TTORATstpTradingUnitType	PledgeInTradingUnit;
	
		///每次可以出库的最小交易单位
		TTORATstpTradingUnitType	PledgeOutTradingUnit;
	
		///证券可以入库的最大数量
		TTORATstpVolumeType	PledgeInVolMax;
	
		///证券可以入库的最小数量
		TTORATstpVolumeType	PledgeInVolMin;
	
		///证券可以出库的最大数量
		TTORATstpVolumeType	PledgeOutVolMax;
	
		///证券可以出库的最小数量
		TTORATstpVolumeType	PledgeOutVolMin;
	};
	
	/// 查询系统节点信息
	struct CTORATstpQrySystemNodeInfoField
	{
		///节点编号
		TTORATstpNodeIDType	NodeID;
	};
	
	/// 系统节点信息
	struct CTORATstpSystemNodeInfoField
	{
		///节点编号
		TTORATstpNodeIDType	NodeID;
	
		///节点信息
		TTORATstpNodeInfoType	NodeInfo;
	
		///是否当前节点
		TTORATstpBoolType	bCurrent;
	};
	
	/// 查询投资者标准券额度
	struct CTORATstpQryStandardBondPositionField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	};
	
	/// 投资者标准券额度
	struct CTORATstpStandardBondPositionField
	{
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///客户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///标准券可用额度
		TTORATstpVolumeType	AvailablePosition;
	
		///标准券可用额度冻结
		TTORATstpVolumeType	AvailablePosFrozen;
	
		///标准券额度总量
		TTORATstpVolumeType	TotalPosition;
	};
	
	/// 查询未到期债券质押回购委托
	struct CTORATstpQryPrematurityRepoOrderField
	{
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///证券品种代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///买卖方向
		TTORATstpDirectionType	Direction;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	};
	
	/// 未到期债券质押回购委托
	struct CTORATstpPrematurityRepoOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///市场代码
		TTORATstpMarketIDType	MarketID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///交易账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///成交日期
		TTORATstpDateType	TradeDay;
	
		///到期日期
		TTORATstpDateType	ExpireDay;
	
		///报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///证券名称
		TTORATstpSecurityNameType	SecurityName;
	
		///证券品种代码
		TTORATstpProductIDType	ProductID;
	
		///证券类别代码
		TTORATstpSecurityTypeType	SecurityType;
	
		///报单类别
		TTORATstpDirectionType	Direction;
	
		///成交数量
		TTORATstpVolumeType	VolumeTraded;
	
		///成交价格
		TTORATstpPriceType	Price;
	
		///成交金额
		TTORATstpMoneyType	Turnover;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	
		///购回应收金额
		TTORATstpMoneyType	RepoTotalMoney;
	
		///利息金额
		TTORATstpMoneyType	InterestAmount;
	};
	
	/// 查询协议交易报单
	struct CTORATstpQryNegoOrderField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeStart;
	
		///Insert Time
		TTORATstpTimeType	InsertTimeEnd;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 查询协议交易撤单
	struct CTORATstpQryNegoOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///本地撤单编号
		TTORATstpOrderLocalIDType	CancelOrderLocalID;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	};
	
	/// 协议交易撤单
	struct CTORATstpNegoOrderActionField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///操作标志
		TTORATstpActionFlagType	ActionFlag;
	
		///本地撤单编号
		TTORATstpOrderLocalIDType	CancelOrderLocalID;
	
		///系统撤单编号
		TTORATstpOrderSysIDType	CancelOrderSysID;
	
		///被撤报单系统报单编号
		TTORATstpOrderSysIDType	OrderSysID;
	
		///被撤报单本地报单编号
		TTORATstpOrderLocalIDType	OrderLocalID;
	
		///委托方式
		TTORATstpOperwayType	Operway;
	
		///字符串附加信息
		TTORATstpStrInfoType	SInfo;
	
		///整形附加信息
		TTORATstpIntInfoType	IInfo;
	
		///经纪公司部门代码
		TTORATstpDepartmentIDType	DepartmentID;
	
		///操作用户
		TTORATstpUserIDType	ActionUser;
	
		///交易日
		TTORATstpDateType	TradingDay;
	
		///操作日期
		TTORATstpDateType	ActionDate;
	
		///操作时间
		TTORATstpTimeType	ActionTime;
	
		///撤单状态
		TTORATstpCancelOrderStatusType	CancelOrderStatus;
	
		///状态信息
		TTORATstpErrorMsgType	StatusMsg;
	
		///请求编号
		TTORATstpRequestIDType	RequestID;
	
		///前置编号
		TTORATstpFrontIDType	ActionFrontID;
	
		///会话编号
		TTORATstpSessionIDType	ActionSessionID;
	
		///交易单元代码
		TTORATstpPbuIDType	PbuID;
	
		///IP地址
		TTORATstpIPAddressType	IPAddress;
	
		///Mac地址
		TTORATstpMacAddressType	MacAddress;
	};
	
	/// 查询协议交易成交
	struct CTORATstpQryNegoTradeField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///投资者代码
		TTORATstpInvestorIDType	InvestorID;
	
		///投资单元代码
		TTORATstpBusinessUnitIDType	BusinessUnitID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	
		///股东账户代码
		TTORATstpShareholderIDType	ShareholderID;
	
		///成交编号
		TTORATstpTradeIDType	TradeID;
	
		///Insert Time
		TTORATstpTimeType	TradeTimeStart;
	
		///Insert Time
		TTORATstpTimeType	TradeTimeEnd;
	};
	
	/// 查询协议交易参数
	struct CTORATstpQryNegotiationParamField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///证券代码
		TTORATstpSecurityIDType	SecurityID;
	};
	
	/// 协议交易参数
	struct CTORATstpNegotiationParamField
	{
		///交易所代码
		TTORATstpExchangeIDType	ExchangeID;
	
		///合约代码
		TTORATstpSecurityIDType	SecurityID;
	
		///买入交易单位
		TTORATstpTradingUnitType	BuyTradingUnit;
	
		///卖出交易单位
		TTORATstpTradingUnitType	SellTradingUnit;
	
		///数量门槛
		TTORATstpVolumeType	MinOrderVolume;
	
		///金额门槛
		TTORATstpMoneyType	MinOrderAmt;
	
		///最小变动价位
		TTORATstpPriceTickType	PriceTick;
	
		///涨停板价
		TTORATstpPriceType	UpperLimitPrice;
	
		///跌停板价
		TTORATstpPriceType	LowerLimitPrice;
	
		///是否有做市商
		TTORATstpBoolType	bMarketMaker;
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
#endif // TORATSTPUSERAPISTRUCT_H__