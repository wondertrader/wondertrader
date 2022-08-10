/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpUserApiDataType.h
///@brief 定义了客户端接口使用的业务数据类型
///@history 
/////////////////////////////////////////////////////////////////////////
#ifndef TORATSTPUSERAPIDATATYPE_H__
#define TORATSTPUSERAPIDATATYPE_H__


#include <limits.h>
#include <float.h>

namespace TORASTOCKAPI
{
	///定义空值
	const int INT_NULL_VAL = INT_MAX;
	const double FLOAT_NULL_VAL = DBL_MAX;
	const char CHAR_NULL_VAL = 0;
	const short WORD_NULL_VAL = SHRT_MAX;
#ifdef WINDOWS
	const __int64 LONG_NULL_VAL = _I64_MAX;
#else
	const long long LONG_NULL_VAL = LLONG_MAX;
#endif
	
	///整型数据置空和判空
	inline void set_null(int &v)
	{
		v = INT_NULL_VAL;
	}
	inline bool is_null(const int &v)
	{
		return v == INT_NULL_VAL;
	}
	
	///浮点型数据置空和判空
	inline void set_null(double &v)
	{
		v = FLOAT_NULL_VAL;
	}
	inline bool is_null(const double &v)
	{
		return v == FLOAT_NULL_VAL;
	}
	
	///字符型数据置空和判空
	inline void set_null(char &v)
	{
		v = CHAR_NULL_VAL;
	}
	inline bool is_null(const char &v)
	{
		return v == CHAR_NULL_VAL;
	}
	
	///短整型数据置空和判空
	inline void set_null(short &v)
	{
		v = WORD_NULL_VAL;
	}
	inline bool is_null(const short &v)
	{
		return v == WORD_NULL_VAL;
	}
	
	///长整型数据置空和判空
	#ifdef WINDOWS
	inline void set_null(__int64 &v)
	#else
	inline void set_null(long long &v)
	#endif
	{
		v = LONG_NULL_VAL;
	}
	#ifdef WINDOWS
	inline bool is_null(const __int64 &v)
	#else
	inline bool is_null(const long long &v)
	#endif
	{
		return v == LONG_NULL_VAL;
	}
	
	///字符串型数据置空和判空
	inline void set_null(char *v)
	{
		v[0] = '\0';
	}
	inline bool is_null(const char *v)
	{
		const char *p=v;
		while (*p)
		{
			if (*p!=' ')
			{
				return false;
			}
			p++;
		}
		return true;
	}
	
	enum TORA_TE_RESUME_TYPE
	{
		TORA_TERT_RESTART = 0,
		TORA_TERT_RESUME,
		TORA_TERT_QUICK
	};
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLogInAccountType是一个登录账户类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpLogInAccountType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLogInAccountTypeType是一个登录账户类型类型
	/////////////////////////////////////////////////////////////////////////
	///用户代码
	const char TORA_TSTP_LACT_UserID = '0';
	///资金账号
	const char TORA_TSTP_LACT_AccountID = '1';
	///上海A股
	const char TORA_TSTP_LACT_SHAStock = '2';
	///深圳A股
	const char TORA_TSTP_LACT_SZAStock = '3';
	///上海B股
	const char TORA_TSTP_LACT_SHBStock = '4';
	///深圳B股
	const char TORA_TSTP_LACT_SZBStock = '5';
	///三板A
	const char TORA_TSTP_LACT_ThreeNewBoardA = '6';
	///三板B
	const char TORA_TSTP_LACT_ThreeNewBoardB = '7';
	///港股
	const char TORA_TSTP_LACT_HKStock = '8';
	///统一用户代码
	const char TORA_TSTP_LACT_UnifiedUserID = '9';
	///北京A股
	const char TORA_TSTP_LACT_BJAStock = 'a';
	typedef char TTORATstpLogInAccountTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDepartmentIDType是一个部门代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDepartmentIDType[11];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpAuthModeType是一个认证方式类型
	/////////////////////////////////////////////////////////////////////////
	///密码
	const char TORA_TSTP_AM_Password = '0';
	///指纹
	const char TORA_TSTP_AM_FingerPrint = '1';
	///钥匙串
	const char TORA_TSTP_AM_CertInfo = '2';
	typedef char TTORATstpAuthModeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPasswordType是一个密码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpPasswordType[41];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserProductInfoType是一个用户端产品信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpUserProductInfoType[11];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpInterfaceProductInfoType是一个接口端产品信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpInterfaceProductInfoType[33];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTerminalInfoType是一个终端信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTerminalInfoType[256];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIPAddressType是一个IP地址类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpIPAddressType[16];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMacAddressType是一个Mac地址类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpMacAddressType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLangType是一个语言类型
	/////////////////////////////////////////////////////////////////////////
	///简体中文
	const char TORA_TSTP_LGT_ZHCN = '0';
	///中文香港
	const char TORA_TSTP_LGT_ZHHK = '1';
	///英文美国
	const char TORA_TSTP_LGT_ENUS = '2';
	typedef char TTORATstpLangType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDeviceIDType是一个设备标识类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDeviceIDType[129];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCertSerialType是一个认证序列类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpCertSerialType[129];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDeviceTypeType是一个设备类别类型
	/////////////////////////////////////////////////////////////////////////
	///PC端
	const char TORA_TSTP_DVT_PC = '0';
	///移动端
	const char TORA_TSTP_DVT_Mobile = '1';
	typedef char TTORATstpDeviceTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFrontIDType是一个前置编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpFrontIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSessionIDType是一个会话编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpSessionIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderRefType是一个报单引用类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpOrderRefType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpVolumeType是一个数量类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpVolumeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTimeType是一个时间类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTimeType[9];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSystemNameType是一个系统名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpSystemNameType[41];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDateType是一个日期类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpDateType[9];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserIDType是一个交易用户代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpUserIDType[16];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserNameType是一个用户名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpUserNameType[81];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpUserTypeType是一个用户类型类型
	/////////////////////////////////////////////////////////////////////////
	///经纪公司用户
	const char TORA_TSTP_UTYPE_BrokerUser = '0';
	///超级用户
	const char TORA_TSTP_UTYPE_SuperUser = '1';
	///投资者用户
	const char TORA_TSTP_UTYPE_Investor = '2';
	typedef char TTORATstpUserTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCommFluxType是一个通讯流量类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpCommFluxType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBoolType是一个布尔型类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpBoolType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpErrorIDType是一个错误代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpErrorIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpErrorMsgType是一个错误信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpErrorMsgType[81];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpExchangeIDType是一个交易所代码类型
	/////////////////////////////////////////////////////////////////////////
	///通用(内部使用)
	const char TORA_TSTP_EXD_COMM = '0';
	///上海交易所
	const char TORA_TSTP_EXD_SSE = '1';
	///深圳交易所
	const char TORA_TSTP_EXD_SZSE = '2';
	///香港交易所
	const char TORA_TSTP_EXD_HK = '3';
	///北京证券交易所
	const char TORA_TSTP_EXD_BSE = '4';
	typedef char TTORATstpExchangeIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpInvestorIDType是一个投资者代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpInvestorIDType[16];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBusinessUnitIDType是一个投资单元代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpBusinessUnitIDType[16];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpShareholderIDType是一个股东账户代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpShareholderIDType[11];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSecurityIDType是一个证券代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpSecurityIDType[31];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDirectionType是一个报单方向类型
	/////////////////////////////////////////////////////////////////////////
	///买入
	const char TORA_TSTP_D_Buy = '0';
	///卖出
	const char TORA_TSTP_D_Sell = '1';
	///ETF申购
	const char TORA_TSTP_D_ETFPur = '2';
	///ETF赎回
	const char TORA_TSTP_D_ETFRed = '3';
	///新股申购
	const char TORA_TSTP_D_IPO = '4';
	///正回购
	const char TORA_TSTP_D_Repurchase = '5';
	///逆回购
	const char TORA_TSTP_D_ReverseRepur = '6';
	///开放式基金申购
	const char TORA_TSTP_D_OeFundPur = '8';
	///开放式基金赎回
	const char TORA_TSTP_D_OeFundRed = '9';
	///担保品划入
	const char TORA_TSTP_D_CollateralIn = 'a';
	///担保品划出
	const char TORA_TSTP_D_CollateralOut = 'b';
	///质押入库
	const char TORA_TSTP_D_PledgeIn = 'd';
	///质押出库
	const char TORA_TSTP_D_PledgeOut = 'e';
	///配股配债
	const char TORA_TSTP_D_Rationed = 'f';
	///基金拆分
	const char TORA_TSTP_D_Split = 'g';
	///基金合并
	const char TORA_TSTP_D_Merge = 'h';
	///融资买入
	const char TORA_TSTP_D_CreditBuy = 'i';
	///融券卖出
	const char TORA_TSTP_D_CreditSell = 'j';
	///卖券还款
	const char TORA_TSTP_D_SellRepay = 'k';
	///买券还券
	const char TORA_TSTP_D_BuyRepay = 'l';
	///还券划转
	const char TORA_TSTP_D_RepayTransfer = 'm';
	///余券划转
	const char TORA_TSTP_D_SurplusTransfer = 'n';
	///源券划转
	const char TORA_TSTP_D_SourceTransfer = 'o';
	///债券转股
	const char TORA_TSTP_D_BondConvertStock = 't';
	///债券回售
	const char TORA_TSTP_D_BondPutback = 'u';
	///ETF实物申购
	const char TORA_TSTP_D_ETFOtPur = 'v';
	///ETF实物赎回
	const char TORA_TSTP_D_ETFOtRed = 'w';
	///回售撤销
	const char TORA_TSTP_D_PutbackRelieve = 'x';
	///意向买入
	const char TORA_TSTP_D_IOIBuy = 'A';
	///意向卖出
	const char TORA_TSTP_D_IOISell = 'B';
	///成交确认买入
	const char TORA_TSTP_D_TCRBuy = 'C';
	///成交确认卖出
	const char TORA_TSTP_D_TCRSell = 'D';
	typedef char TTORATstpDirectionType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceType是一个价格类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpPriceType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderPriceTypeType是一个报单价格条件类型
	/////////////////////////////////////////////////////////////////////////
	///任意价
	const char TORA_TSTP_OPT_AnyPrice = '1';
	///限价
	const char TORA_TSTP_OPT_LimitPrice = '2';
	///最优价
	const char TORA_TSTP_OPT_BestPrice = '3';
	///盘后定价
	const char TORA_TSTP_OPT_FixPrice = '4';
	///五档价
	const char TORA_TSTP_OPT_FiveLevelPrice = '5';
	///本方最优
	const char TORA_TSTP_OPT_HomeBestPrice = '6';
	typedef char TTORATstpOrderPriceTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTimeConditionType是一个有效期类型类型
	/////////////////////////////////////////////////////////////////////////
	///立即完成，否则撤销
	const char TORA_TSTP_TC_IOC = '1';
	///本节有效
	const char TORA_TSTP_TC_GFS = '2';
	///当日有效
	const char TORA_TSTP_TC_GFD = '3';
	///指定日期前有效
	const char TORA_TSTP_TC_GTD = '4';
	///撤销前有效
	const char TORA_TSTP_TC_GTC = '5';
	///集合竞价有效
	const char TORA_TSTP_TC_GFA = '6';
	typedef char TTORATstpTimeConditionType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpVolumeConditionType是一个成交量类型类型
	/////////////////////////////////////////////////////////////////////////
	///任何数量
	const char TORA_TSTP_VC_AV = '1';
	///最小数量
	const char TORA_TSTP_VC_MV = '2';
	///全部数量
	const char TORA_TSTP_VC_CV = '3';
	typedef char TTORATstpVolumeConditionType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOperwayType是一个委托方式类型
	/////////////////////////////////////////////////////////////////////////
	///空
	const char TORA_TSTP_OPERW_Non = ' ';
	///电话委托
	const char TORA_TSTP_OPERW_Telephone = '0';
	///柜台委托
	const char TORA_TSTP_OPERW_OTC = '1';
	///移动客户端委托
	const char TORA_TSTP_OPERW_MobileClient = '2';
	///PC客户端委托
	const char TORA_TSTP_OPERW_PCClient = '3';
	///TY委托
	const char TORA_TSTP_OPERW_TY = '4';
	///通道委托
	const char TORA_TSTP_OPERW_Channel = '5';
	typedef char TTORATstpOperwayType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLotTypeType是一个港股订单数量类型类型
	/////////////////////////////////////////////////////////////////////////
	///零股订单
	const char TORA_TSTP_LT_OddLot = '0';
	///整手订单
	const char TORA_TSTP_LT_RoundLot = '1';
	typedef char TTORATstpLotTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderSysIDType是一个系统报单编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpOrderSysIDType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCondCheckType是一个委托条件检查类型
	/////////////////////////////////////////////////////////////////////////
	///不做任何检查
	const char TORA_TSTP_CCT_None = '0';
	///自成交检查
	const char TORA_TSTP_CCT_SelfDeal = '1';
	typedef char TTORATstpCondCheckType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpForceCloseReasonType是一个强平原因类型
	/////////////////////////////////////////////////////////////////////////
	///非强平
	const char TORA_TSTP_FCC_NotForceClose = '0';
	///资金不足
	const char TORA_TSTP_FCC_MoneyNotEnough = '1';
	///仓位超限
	const char TORA_TSTP_FCC_PositionOverFull = '2';
	///其它
	const char TORA_TSTP_FCC_Other = '3';
	typedef char TTORATstpForceCloseReasonType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCreditDebtIDType是一个信用负债编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpCreditDebtIDType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCreditQuotaTypeType是一个信用头寸类型类型
	/////////////////////////////////////////////////////////////////////////
	///普通
	const char TORA_TSTP_CQT_Normal = '0';
	///专项
	const char TORA_TSTP_CQT_Special = '1';
	typedef char TTORATstpCreditQuotaTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIntSerialType是一个整数流水号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpIntSerialType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpStrInfoType是一个字符串附加信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpStrInfoType[33];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIntInfoType是一个整形附加信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpIntInfoType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpRequestIDType是一个请求编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpRequestIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderLocalIDType是一个本地报单编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpOrderLocalIDType[13];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderStatusType是一个报单状态类型
	/////////////////////////////////////////////////////////////////////////
	///预埋
	const char TORA_TSTP_OST_Cached = '0';
	///未知
	const char TORA_TSTP_OST_Unknown = '1';
	///交易所已接收
	const char TORA_TSTP_OST_Accepted = '2';
	///部分成交
	const char TORA_TSTP_OST_PartTraded = '3';
	///全部成交
	const char TORA_TSTP_OST_AllTraded = '4';
	///部成部撤
	const char TORA_TSTP_OST_PartTradeCanceled = '5';
	///全部撤单
	const char TORA_TSTP_OST_AllCanceled = '6';
	///交易所已拒绝
	const char TORA_TSTP_OST_Rejected = '7';
	///发往交易核心
	const char TORA_TSTP_OST_SendTradeEngine = '#';
	typedef char TTORATstpOrderStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderSubmitStatusType是一个报单提交状态类型
	/////////////////////////////////////////////////////////////////////////
	///未提交
	const char TORA_TSTP_OSS_InsertUnSubmit = '0';
	///已提交
	const char TORA_TSTP_OSS_InsertSubmitted = '1';
	///撤单未提交
	const char TORA_TSTP_OSS_CancelUnSubmit = '2';
	///撤单已提交
	const char TORA_TSTP_OSS_CancelSubmitted = '3';
	///撤单已被拒绝
	const char TORA_TSTP_OSS_CancelRejected = '4';
	///撤单已删除
	const char TORA_TSTP_OSS_CancelDeleted = '5';
	typedef char TTORATstpOrderSubmitStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpAccountIDType是一个资金帐号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpAccountIDType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCurrencyIDType是一个币种类型
	/////////////////////////////////////////////////////////////////////////
	///人民币
	const char TORA_TSTP_CID_CNY = '1';
	///港币
	const char TORA_TSTP_CID_HKD = '2';
	///美元
	const char TORA_TSTP_CID_USD = '3';
	typedef char TTORATstpCurrencyIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPbuIDType是一个交易单元代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpPbuIDType[11];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMoneyType是一个资金类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpMoneyType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderTypeType是一个报单类型类型
	/////////////////////////////////////////////////////////////////////////
	///正常
	const char TORA_TSTP_ORDT_Normal = '0';
	///报价衍生
	const char TORA_TSTP_ORDT_DeriveFromQuote = '1';
	///组合衍生
	const char TORA_TSTP_ORDT_DeriveFromCombination = '2';
	///组合报单
	const char TORA_TSTP_ORDT_Combination = '3';
	///条件单
	const char TORA_TSTP_ORDT_ConditionalOrder = '4';
	///互换单
	const char TORA_TSTP_ORDT_Swap = '5';
	///预埋单
	const char TORA_TSTP_ORDT_Cache = '6';
	///夜市委托
	const char TORA_TSTP_ORDT_Night = '7';
	///通道委托
	const char TORA_TSTP_ORDT_Board = '8';
	typedef char TTORATstpOrderTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpQuotaIDType是一个额度编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpQuotaIDType[17];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFloatInfoType是一个浮点型附加信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpFloatInfoType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTradeIDType是一个成交编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTradeIDType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpActionFlagType是一个操作标志类型
	/////////////////////////////////////////////////////////////////////////
	///删除
	const char TORA_TSTP_AF_Delete = '0';
	///修改
	const char TORA_TSTP_AF_Modify = '3';
	///强制删除
	const char TORA_TSTP_AF_ForceDelete = '4';
	typedef char TTORATstpActionFlagType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCondOrderIDType是一个条件报单编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpCondOrderIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTriggerOrderVolumeTypeType是一个触发基准数量类型类型
	/////////////////////////////////////////////////////////////////////////
	///自定义数量
	const char TORA_TSTP_TOVT_CustomVol = '1';
	///相关数量
	const char TORA_TSTP_TOVT_RelativeVol = '2';
	typedef char TTORATstpTriggerOrderVolumeTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTriggerOrderPriceTypeType是一个触发基准价格类型类型
	/////////////////////////////////////////////////////////////////////////
	///自定义价格
	const char TORA_TSTP_TOPT_CustomPrice = '1';
	///最新价
	const char TORA_TSTP_TOPT_LastPrice = '2';
	///卖一价
	const char TORA_TSTP_TOPT_AskPrice1 = '3';
	///买一价
	const char TORA_TSTP_TOPT_BidPrice1 = '4';
	///相关价
	const char TORA_TSTP_TOPT_Relative = '5';
	typedef char TTORATstpTriggerOrderPriceTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpContingentConditionType是一个触发条件类型
	/////////////////////////////////////////////////////////////////////////
	///成交触发
	const char TORA_TSTP_CC_TradeTouch = '0';
	///撤单触发
	const char TORA_TSTP_CC_CancelTouch = '1';
	///时间触发
	const char TORA_TSTP_CC_TimeTouch = '2';
	///交易时段触发
	const char TORA_TSTP_CC_SegmentTouch = '3';
	///最新价大于等于条件价
	const char TORA_TSTP_CC_LastPriceGreaterThanStopPrice = '4';
	///最新价小于等于条件价
	const char TORA_TSTP_CC_LastPriceLesserThanStopPrice = '5';
	///卖一价大于等于条件价
	const char TORA_TSTP_CC_AskPriceGreaterEqualStopPrice = '6';
	///卖一价小于等于条件价
	const char TORA_TSTP_CC_AskPriceLesserEqualStopPrice = '7';
	///买一价大于等于条件价
	const char TORA_TSTP_CC_BidPriceGreaterEqualStopPrice = '8';
	///买一价小于等于条件价
	const char TORA_TSTP_CC_BidPriceLesserEqualStopPrice = '9';
	typedef char TTORATstpContingentConditionType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpVolumeMultipleType是一个合约数量乘数类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpVolumeMultipleType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpRelativeCondParamType是一个相关条件参数类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpRelativeCondParamType[31];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCondOrderStatusType是一个条件单状态类型
	/////////////////////////////////////////////////////////////////////////
	///初始
	const char TORA_TSTP_COST_Initial = '#';
	///未触发
	const char TORA_TSTP_COST_NotTouched = '0';
	///已触发
	const char TORA_TSTP_COST_Touched = '1';
	///已结束
	const char TORA_TSTP_COST_Finished = '2';
	///已撤销
	const char TORA_TSTP_COST_Cancel = '3';
	///触发失败
	const char TORA_TSTP_COST_Failed = '4';
	typedef char TTORATstpCondOrderStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNegoContractorType是一个联系人类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpNegoContractorType[17];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNegoContractorInfoType是一个联系方式类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpNegoContractorInfoType[65];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNegoConfirmIDType是一个约定号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpNegoConfirmIDType[17];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMarketIDType是一个市场代码类型
	/////////////////////////////////////////////////////////////////////////
	///通用(内部使用)
	const char TORA_TSTP_MKD_COMMON = '0';
	///上海A股
	const char TORA_TSTP_MKD_SHA = '1';
	///深圳A股
	const char TORA_TSTP_MKD_SZA = '2';
	///上海B股
	const char TORA_TSTP_MKD_SHB = '3';
	///深圳B股
	const char TORA_TSTP_MKD_SZB = '4';
	///深圳三版A股
	const char TORA_TSTP_MKD_SZThreeA = '5';
	///深圳三版B股
	const char TORA_TSTP_MKD_SZThreeB = '6';
	///境外市场
	const char TORA_TSTP_MKD_Foreign = '7';
	///深圳港股通市场
	const char TORA_TSTP_MKD_SZHK = '8';
	///上海港股通市场
	const char TORA_TSTP_MKD_SHHK = '9';
	///北京主板
	const char TORA_TSTP_MKD_BJMain = 'a';
	typedef char TTORATstpMarketIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMarketStatusType是一个市场状态类型
	/////////////////////////////////////////////////////////////////////////
	///未知
	const char TORA_TSTP_MST_UnKnown = '#';
	///开盘前
	const char TORA_TSTP_MST_BeforeTrading = '0';
	///连续交易
	const char TORA_TSTP_MST_Continous = '1';
	///收盘
	const char TORA_TSTP_MST_Closed = '2';
	///开盘集合竞价
	const char TORA_TSTP_MST_OpenCallAuction = '3';
	///(港股通)未开市
	const char TORA_TSTP_MST_SZSEHKUnopened = 'a';
	///(港股通)开盘集合竞价输入买卖盘
	const char TORA_TSTP_MST_SZSEHKOpenCallAuctionInput = 'b';
	///(港股通)开盘集合竞价对盘前
	const char TORA_TSTP_MST_SZSEHKOpenCallAuctionBeforeMatch = 'c';
	///(港股通)开盘集合竞价对盘
	const char TORA_TSTP_MST_SZSEHKOpenCallAuctionMatch = 'd';
	///(港股通)暂停
	const char TORA_TSTP_MST_SZSEHKHalt = 'e';
	///(港股通)持续交易
	const char TORA_TSTP_MST_SZSEHKContinous = 'f';
	///(港股通)Exchange Intervention
	const char TORA_TSTP_MST_SZSEHKExchangeIntervention = 'g';
	///(港股通)收盘集合竞价参考价定价
	const char TORA_TSTP_MST_SZSEHKCloseCallAuctionReferencePrice = 'h';
	///(港股通)收盘集合竞价输入买卖盘
	const char TORA_TSTP_MST_SZSEHKCloseCallAuctionInput = 'i';
	///(港股通)收盘集合竞价不可取消
	const char TORA_TSTP_MST_SZSEHKCloseCallAuctionCannotCancel = 'j';
	///(港股通)收盘集合竞价对盘
	const char TORA_TSTP_MST_SZSEHKCloseCallAuctionMatch = 'k';
	///(港股通)收盘集合竞价随机收市
	const char TORA_TSTP_MST_SZSEHKCloseCallAuctionRandomClosed = 'l';
	///(港股通)取消买卖盘
	const char TORA_TSTP_MST_SZSEHKCancel = 'm';
	///(港股通)收市
	const char TORA_TSTP_MST_SZSEHKClosed = 'n';
	///(港股通)全日收市
	const char TORA_TSTP_MST_SZSEHKWholeClosed = 'o';
	typedef char TTORATstpMarketStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTransferDirectionType是一个转移方向类型
	/////////////////////////////////////////////////////////////////////////
	///蓝补
	const char TORA_TSTP_TRNSD_In = '0';
	///红冲
	const char TORA_TSTP_TRNSD_Out = '1';
	///集中交易调入
	const char TORA_TSTP_TRNSD_MoveIn = '2';
	///集中交易调出
	const char TORA_TSTP_TRNSD_MoveOut = '3';
	///冻结
	const char TORA_TSTP_TRNSD_Freeze = '4';
	///解冻
	const char TORA_TSTP_TRNSD_UnFreeze = '5';
	///证券转银行
	const char TORA_TSTP_TRNSD_StockToBank = '6';
	///银行转证券
	const char TORA_TSTP_TRNSD_BankToStock = '7';
	///外部节点转入
	const char TORA_TSTP_TRNSD_NodeMoveIn = 'c';
	///外部节点转出
	const char TORA_TSTP_TRNSD_NodeMoveOut = 'd';
	///直接还款
	const char TORA_TSTP_TRNSD_CashRepay = 'f';
	///直接还息(内部使用)
	const char TORA_TSTP_TRNSD_CashRepayInterestFee = 'g';
	typedef char TTORATstpTransferDirectionType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBankIDType是一个银行代码类型
	/////////////////////////////////////////////////////////////////////////
	///中国建设银行
	const char TORA_TSTP_BKID_CCB = '1';
	///中国农业银行
	const char TORA_TSTP_BKID_ABC = '2';
	///中国工商银行
	const char TORA_TSTP_BKID_ICBC = '3';
	///中国银行
	const char TORA_TSTP_BKID_BOC = '4';
	///中国招商银行
	const char TORA_TSTP_BKID_CMB = '5';
	///中国交通银行
	const char TORA_TSTP_BKID_BC = '6';
	///浦东发展银行
	const char TORA_TSTP_BKID_SPDB = '7';
	///兴业银行
	const char TORA_TSTP_BKID_CIB = '8';
	///中国光大银行
	const char TORA_TSTP_BKID_CEB = '9';
	///广东发展银行
	const char TORA_TSTP_BKID_GDB = 'a';
	///南京银行
	const char TORA_TSTP_BKID_NJCB = 'b';
	///上海银行
	const char TORA_TSTP_BKID_SHCB = 'c';
	///中信银行
	const char TORA_TSTP_BKID_CITICB = 'd';
	///华夏银行
	const char TORA_TSTP_BKID_HXB = 'e';
	///民生银行
	const char TORA_TSTP_BKID_CMBC = 'f';
	///平安银行
	const char TORA_TSTP_BKID_PACB = 'g';
	///宁波银行
	const char TORA_TSTP_BKID_NBCB = 'h';
	///北京银行
	const char TORA_TSTP_BKID_BOB = 'i';
	///邮储银行
	const char TORA_TSTP_BKID_PSBC = 'j';
	typedef char TTORATstpBankIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNodeIDType是一个节点编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpNodeIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTransferStatusType是一个转移状态类型
	/////////////////////////////////////////////////////////////////////////
	///转移正在处理
	const char TORA_TSTP_TRANST_TranferHandling = '0';
	///转移成功
	const char TORA_TSTP_TRANST_TransferSuccess = '1';
	///转移失败
	const char TORA_TSTP_TRANST_TransferFail = '2';
	///冲正正在处理
	const char TORA_TSTP_TRANST_RepealHandling = '3';
	///冲正成功
	const char TORA_TSTP_TRANST_RepealSuccess = '4';
	///冲正失败
	const char TORA_TSTP_TRANST_RepealFail = '5';
	///外部系统已接收
	const char TORA_TSTP_TRANST_ExternalAccepted = '6';
	///发往交易核心
	const char TORA_TSTP_TRANST_SendTradeEngine = '#';
	typedef char TTORATstpTransferStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBankAccountIDType是一个签约银行账号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpBankAccountIDType[31];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTransferPositionTypeType是一个转移持仓类型类型
	/////////////////////////////////////////////////////////////////////////
	///任意仓
	const char TORA_TSTP_TPT_ALL = '0';
	///昨仓
	const char TORA_TSTP_TPT_History = '1';
	///今买卖仓
	const char TORA_TSTP_TPT_TodayBS = '2';
	///今申赎仓
	const char TORA_TSTP_TPT_TodayPR = '3';
	///今拆分合并仓
	const char TORA_TSTP_TPT_TodaySM = '4';
	typedef char TTORATstpTransferPositionTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTransferReasonType是一个外围系统仓位调拨原因类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTransferReasonType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpContentType是一个消息正文类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpContentType[501];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpRatioType是一个比率类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpRatioType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNameType是一个名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpNameType[61];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpDataSyncStatusType是一个数据同步状态类型
	/////////////////////////////////////////////////////////////////////////
	///未同步
	const char TORA_TSTP_DS_Asynchronous = '1';
	///同步中
	const char TORA_TSTP_DS_Synchronizing = '2';
	///已同步
	const char TORA_TSTP_DS_Synchronized = '3';
	///全部同步完成
	const char TORA_TSTP_DS_AllSynchronized = '4';
	///预同步完成
	const char TORA_TSTP_DS_PreSync = '5';
	typedef char TTORATstpDataSyncStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpProductIDType是一个证券品种代码类型
	/////////////////////////////////////////////////////////////////////////
	///通用(内部使用)
	const char TORA_TSTP_PID_COMMON = '0';
	///上海股票
	const char TORA_TSTP_PID_SHStock = '1';
	///上海基金
	const char TORA_TSTP_PID_SHFund = '3';
	///上海债券
	const char TORA_TSTP_PID_SHBond = '4';
	///上海标准券
	const char TORA_TSTP_PID_SHStandard = '5';
	///上海质押式回购
	const char TORA_TSTP_PID_SHRepurchase = '6';
	///深圳股票
	const char TORA_TSTP_PID_SZStock = '7';
	///深圳基金
	const char TORA_TSTP_PID_SZFund = '9';
	///深圳债券
	const char TORA_TSTP_PID_SZBond = 'a';
	///深圳标准券
	const char TORA_TSTP_PID_SZStandard = 'b';
	///深圳质押式回购
	const char TORA_TSTP_PID_SZRepurchase = 'c';
	///深港通港股主板
	const char TORA_TSTP_PID_SZSEHKMain = 'd';
	///深港通港股创业板
	const char TORA_TSTP_PID_SZSEHKGEM = 'e';
	///深港通港股扩充交易证券
	const char TORA_TSTP_PID_SZSEHKETS = 'f';
	///深港通港股NasdaqAMX市场
	const char TORA_TSTP_PID_SZSEHKNasdaqAMX = 'g';
	///上海科创板
	const char TORA_TSTP_PID_SHKC = 'i';
	///北京股票
	const char TORA_TSTP_PID_BJStock = 'j';
	typedef char TTORATstpProductIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSecurityNameType是一个证券名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpSecurityNameType[81];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSecurityTypeType是一个证券类别类型
	/////////////////////////////////////////////////////////////////////////
	///通用(内部使用)
	const char TORA_TSTP_STP_COMMON = '0';
	///上海A股
	const char TORA_TSTP_STP_SHAShares = 'a';
	///上海单市场股票ETF
	const char TORA_TSTP_STP_SHSingleMarketStockETF = 'b';
	///上海单市场实物债券ETF
	const char TORA_TSTP_STP_SHSingleMarketBondETF = 'c';
	///上海黄金ETF
	const char TORA_TSTP_STP_SHGoldETF = 'd';
	///上海货币ETF
	const char TORA_TSTP_STP_SHTradableMonetaryFund = 'e';
	///上海国债地方债
	const char TORA_TSTP_STP_SHBondNation = 'f';
	///上海企业债
	const char TORA_TSTP_STP_SHBondCorporation = 'g';
	///上海公司债
	const char TORA_TSTP_STP_SHBondCompany = 'h';
	///上海可转债
	const char TORA_TSTP_STP_SHBondConversion = 'i';
	///上海分离债
	const char TORA_TSTP_STP_SHBondSeparation = 'j';
	///上海标准券
	const char TORA_TSTP_STP_SHStandard = 'o';
	///上海质押式回购
	const char TORA_TSTP_STP_SHRepo = 'p';
	///上海封闭式基金
	const char TORA_TSTP_STP_SHCEFund = 'q';
	///上海开放式基金
	const char TORA_TSTP_STP_SHOEFund = 'r';
	///上海跨市场ETF
	const char TORA_TSTP_STP_SHCrossMarketStockETF = 's';
	///上海跨境ETF
	const char TORA_TSTP_STP_SHCrossBorderETF = 't';
	///上海分级母基金
	const char TORA_TSTP_STP_SHMontherStructFund = 'u';
	///上海分级子基金
	const char TORA_TSTP_STP_SHSubStructFund = 'v';
	///上海实时申赎货币基金
	const char TORA_TSTP_STP_SHRealTimeMonetaryFund = 'w';
	///上海可交换债
	const char TORA_TSTP_STP_SHExchangeableBond = 'x';
	///上海标准LOF基金
	const char TORA_TSTP_STP_SHLOF = 'A';
	///深圳主板A股
	const char TORA_TSTP_STP_SZMainAShares = 'B';
	///深圳中小企业板
	const char TORA_TSTP_STP_SZSME = 'C';
	///深圳国债、地方债
	const char TORA_TSTP_STP_SZBondNation = 'D';
	///深圳企业债
	const char TORA_TSTP_STP_SZBondCorporation = 'E';
	///深圳公司债
	const char TORA_TSTP_STP_SZBondCompany = 'F';
	///深圳可转债
	const char TORA_TSTP_STP_SZBondConversion = 'G';
	///深圳分离债
	const char TORA_TSTP_STP_SZBondSeparation = 'H';
	///深圳创业板(注册制)
	const char TORA_TSTP_STP_SZGEMReg = 'I';
	///深圳创业板可转债(注册制)
	const char TORA_TSTP_STP_SZGEMBondConversionReg = 'J';
	///深圳跨境ETF
	const char TORA_TSTP_STP_SZCrossBorderETF = 'K';
	///深圳黄金ETF
	const char TORA_TSTP_STP_SZGoldETF = 'L';
	///深圳现金债券ETF
	const char TORA_TSTP_STP_SZCashBondETF = 'M';
	///深圳单市场股票ETF
	const char TORA_TSTP_STP_SZSingleMarketStockETF = 'N';
	///深圳单市场实物债券ETF
	const char TORA_TSTP_STP_SZSingleMarketBondETF = 'O';
	///深圳货币ETF
	const char TORA_TSTP_STP_SZMonetaryFundETF = 'P';
	///深圳创业板
	const char TORA_TSTP_STP_SZGEM = 'Q';
	///深圳创业板可交换债
	const char TORA_TSTP_STP_SZGEMExchangeableBond = 'R';
	///深圳创业板可交换债(注册制)
	const char TORA_TSTP_STP_SZGEMExchangeableBondReg = 'S';
	///深圳标准券
	const char TORA_TSTP_STP_SZStandard = 'T';
	///深圳质押式回购
	const char TORA_TSTP_STP_SZRepo = 'U';
	///深圳封闭式基金
	const char TORA_TSTP_STP_SZCEFund = 'V';
	///深圳开放式基金
	const char TORA_TSTP_STP_SZOEFund = 'W';
	///深圳跨境开放式基金
	const char TORA_TSTP_STP_SZCrossBorderOEFund = 'X';
	///深圳跨市场股票ETF
	const char TORA_TSTP_STP_SZCrossMarketStockETF = 'Y';
	///深圳标准LOF基金
	const char TORA_TSTP_STP_SZLOF = 'Z';
	///深圳跨境LOF基金
	const char TORA_TSTP_STP_SZCrossBorderLOF = '1';
	///深圳传统分级母基金
	const char TORA_TSTP_STP_SZMontherStructFund = '2';
	///深圳传统分级子基金
	const char TORA_TSTP_STP_SZSubStructFund = '3';
	///深圳跨境分级母基金
	const char TORA_TSTP_STP_SZMontherCrossBorderStructFund = '4';
	///深圳跨境分级子基金
	const char TORA_TSTP_STP_SZSubCrossBorderStructFund = '5';
	///深圳可交换债
	const char TORA_TSTP_STP_SZExchangeableBond = '6';
	///深圳创业板可转债
	const char TORA_TSTP_STP_SZGEMBondConversion = '7';
	///深港通港股债券
	const char TORA_TSTP_STP_SZSEHKBond = '8';
	///深港通港股一篮子权证
	const char TORA_TSTP_STP_SZSEHKBasketWarrant = '9';
	///深港通港股股本
	const char TORA_TSTP_STP_SZSEHKEquity = 'y';
	///深港通港股信托
	const char TORA_TSTP_STP_SZSEHKTrust = 'z';
	///深港通港股权证
	const char TORA_TSTP_STP_SZSEHKWarrant = '#';
	///上海存托凭证
	const char TORA_TSTP_STP_SHCDR = '+';
	///上海科创板股票
	const char TORA_TSTP_STP_SHKC = '*';
	///科创板产品（上市后前5个交易日）
	const char TORA_TSTP_STP_SHKC1 = '^';
	///上海科创板存托凭证
	const char TORA_TSTP_STP_SHKCCDR = '-';
	///深圳主板、中小板创新企业股票或存托凭证
	const char TORA_TSTP_STP_SZCDR = 'k';
	///深圳创业板创新企业股票或存托凭证
	const char TORA_TSTP_STP_SZGEMCDR = 'l';
	///深圳创业板创新企业股票或存托凭证(注册制)
	const char TORA_TSTP_STP_SZGEMCDRReg = 'm';
	///深圳商品期货ETF
	const char TORA_TSTP_STP_SZCommFuturesETF = 'n';
	///深圳基础设施基金
	const char TORA_TSTP_STP_SZInfrastructureFund = '=';
	///上海科创板ETF
	const char TORA_TSTP_STP_SHKCETF = '@';
	///上海科创板LOF
	const char TORA_TSTP_STP_SHKCLOF = '%';
	///上海科创板可转债
	const char TORA_TSTP_STP_SHKCBondConversion = '$';
	///上海定向可转债
	const char TORA_TSTP_STP_SHOrientedConversionBond = '<';
	///深圳定向可转债
	const char TORA_TSTP_STP_SZOrientedConversionBond = '>';
	///上海基础设施基金
	const char TORA_TSTP_STP_SHInfrastructureFund = '~';
	///北京股票
	const char TORA_TSTP_STP_BJStock = '[';
	typedef char TTORATstpSecurityTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOrderUnitType是一个申报单位类型
	/////////////////////////////////////////////////////////////////////////
	///手
	const char TORA_TSTP_OUT_Shou = '0';
	///股
	const char TORA_TSTP_OUT_Gu = '1';
	///份
	const char TORA_TSTP_OUT_Fen = '2';
	///张
	const char TORA_TSTP_OUT_Zhang = '3';
	typedef char TTORATstpOrderUnitType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTradingUnitType是一个交易单位类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpTradingUnitType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceTickType是一个最小变动价位类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpPriceTickType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpParValueType是一个面值类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpParValueType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSecurityStatusType是一个证券状态类型
	/////////////////////////////////////////////////////////////////////////
	#ifdef WINDOWS
	typedef __int64 TTORATstpSecurityStatusType;
	#else
	typedef long long int TTORATstpSecurityStatusType;
	#endif
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpInterestType是一个利息类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpInterestType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLargeVolumeType是一个大额数量类型
	/////////////////////////////////////////////////////////////////////////
	typedef double TTORATstpLargeVolumeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIssueModeType是一个发行方式类型
	/////////////////////////////////////////////////////////////////////////
	///市值配售方式
	const char TORA_TSTP_IMO_ValueLimit = '0';
	///增发定价方式
	const char TORA_TSTP_IMO_AddIssue = '1';
	///信用申购方式
	const char TORA_TSTP_IMO_Credit = '2';
	///定价发行(北交所)
	const char TORA_TSTP_IMO_Fixed = '3';
	///竞价发行(北交所)
	const char TORA_TSTP_IMO_Auction = '4';
	typedef char TTORATstpIssueModeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLoginLimitType是一个登录限制类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpLoginLimitType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLoginStatusType是一个登录状态类型
	/////////////////////////////////////////////////////////////////////////
	///启用
	const char TORA_TSTP_USTS_Enabled = '1';
	///禁用
	const char TORA_TSTP_USTS_Disabled = '2';
	///锁定
	const char TORA_TSTP_USTS_Locked = '4';
	typedef char TTORATstpLoginStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpInvestorTypeType是一个投资者类型类型
	/////////////////////////////////////////////////////////////////////////
	///个人
	const char TORA_TSTP_CT_Person = '0';
	///机构
	const char TORA_TSTP_CT_Company = '1';
	///自营
	const char TORA_TSTP_CT_SelfOperate = '5';
	typedef char TTORATstpInvestorTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpInvestorNameType是一个投资者名称类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpInvestorNameType[81];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIdCardTypeType是一个证件类型类型
	/////////////////////////////////////////////////////////////////////////
	///组织机构代码
	const char TORA_TSTP_ICT_EID = '0';
	///中国公民身份证
	const char TORA_TSTP_ICT_IDCard = '1';
	///军官证
	const char TORA_TSTP_ICT_OfficerIDCard = '2';
	///警官证
	const char TORA_TSTP_ICT_PoliceIDCard = '3';
	///士兵证
	const char TORA_TSTP_ICT_SoldierIDCard = '4';
	///户口簿
	const char TORA_TSTP_ICT_HouseholdRegister = '5';
	///护照
	const char TORA_TSTP_ICT_Passport = '6';
	///台胞证
	const char TORA_TSTP_ICT_TaiwanCompatriotIDCard = '7';
	///回乡证
	const char TORA_TSTP_ICT_HomeComingCard = '8';
	///营业执照号
	const char TORA_TSTP_ICT_LicenseNo = '9';
	///税务登记号/当地纳税ID
	const char TORA_TSTP_ICT_TaxNo = 'A';
	///港澳居民来往内地通行证
	const char TORA_TSTP_ICT_HMMainlandTravelPermit = 'B';
	///台湾居民来往大陆通行证
	const char TORA_TSTP_ICT_TwMainlandTravelPermit = 'C';
	///驾照
	const char TORA_TSTP_ICT_DrivingLicense = 'D';
	///当地社保ID
	const char TORA_TSTP_ICT_SocialID = 'F';
	///当地身份证
	const char TORA_TSTP_ICT_LocalID = 'G';
	///商业登记证
	const char TORA_TSTP_ICT_BusinessRegistration = 'H';
	///港澳永久性居民身份证
	const char TORA_TSTP_ICT_HKMCIDCard = 'I';
	///人行开户许可证
	const char TORA_TSTP_ICT_AccountsPermits = 'J';
	///其他证件
	const char TORA_TSTP_ICT_OtherCard = 'x';
	typedef char TTORATstpIdCardTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIdCardNoType是一个证件编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpIdCardNoType[51];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTradingStatusType是一个交易状态类型
	/////////////////////////////////////////////////////////////////////////
	///正常
	const char TORA_TSTP_TS_Normal = '1';
	///正在强平
	const char TORA_TSTP_TS_ForceClosing = '2';
	///异常
	const char TORA_TSTP_TS_Exception = '3';
	typedef char TTORATstpTradingStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOperwaysType是一个委托方式合集类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpOperwaysType[41];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMobileType是一个手机类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpMobileType[41];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpTelephoneType是一个联系电话类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpTelephoneType[41];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpEmailType是一个Email类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpEmailType[61];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFaxType是一个传真类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpFaxType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpAddressType是一个通讯地址类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpAddressType[101];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpZipCodeType是一个邮政编码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpZipCodeType[21];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpProfInvestorTypeType是一个专业投资者类别类型
	/////////////////////////////////////////////////////////////////////////
	///非专业投资者
	const char TORA_TSTP_PIT_Normal = '0';
	///专业投资者
	const char TORA_TSTP_PIT_Professional = '1';
	typedef char TTORATstpProfInvestorTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPlanTypeType是一个套餐类型类型
	/////////////////////////////////////////////////////////////////////////
	///标准套餐
	const char TORA_TSTP_PLTP_Standard = '0';
	///智能套餐
	const char TORA_TSTP_PLTP_Smart = '1';
	///预留1
	const char TORA_TSTP_PLTP_Reserve1 = '2';
	///预留2
	const char TORA_TSTP_PLTP_Reserve2 = '3';
	///未定义类型（内部使用）
	const char TORA_TSTP_PLTP_Undefined = '4';
	typedef char TTORATstpPlanTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpRemarkType是一个备注类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpRemarkType[513];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpShareholderIDTypeType是一个股东账户类型类型
	/////////////////////////////////////////////////////////////////////////
	///投机
	const char TORA_TSTP_SIDT_Speculation = '1';
	///套利
	const char TORA_TSTP_SIDT_Arbitrage = '2';
	///套保
	const char TORA_TSTP_SIDT_Hedge = '3';
	///普通
	const char TORA_TSTP_SIDT_Normal = 'a';
	///信用
	const char TORA_TSTP_SIDT_Credit = 'b';
	///衍生品
	const char TORA_TSTP_SIDT_Derivatives = 'c';
	typedef char TTORATstpShareholderIDTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCancelOrderStatusType是一个撤单状态类型
	/////////////////////////////////////////////////////////////////////////
	///预埋
	const char TORA_TSTP_CORDS_Cached = '0';
	///已提交
	const char TORA_TSTP_CORDS_Submitted = '1';
	///成功
	const char TORA_TSTP_CORDS_Success = '2';
	///拒绝
	const char TORA_TSTP_CORDS_Rejected = '3';
	typedef char TTORATstpCancelOrderStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCancelOrderTypeType是一个撤单类型类型
	/////////////////////////////////////////////////////////////////////////
	///普通撤单
	const char TORA_TSTP_CORDT_Normal = '0';
	///强制撤单
	const char TORA_TSTP_CORDT_Force = '1';
	///预埋撤单
	const char TORA_TSTP_CORDT_Cache = '2';
	///通道撤单
	const char TORA_TSTP_CORDT_Board = '3';
	typedef char TTORATstpCancelOrderTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpAccountTypeType是一个资金账户类型类型
	/////////////////////////////////////////////////////////////////////////
	///普通
	const char TORA_TSTP_FAT_Normal = '1';
	///信用
	const char TORA_TSTP_FAT_Credit = '2';
	///衍生品
	const char TORA_TSTP_FAT_Derivatives = '3';
	typedef char TTORATstpAccountTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBizClassType是一个业务类别类型
	/////////////////////////////////////////////////////////////////////////
	///买入
	const char TORA_TSTP_BC_Buy = '0';
	///卖出
	const char TORA_TSTP_BC_Sell = '1';
	///ETF申购
	const char TORA_TSTP_BC_ETFPur = '2';
	///ETF赎回
	const char TORA_TSTP_BC_ETFRed = '3';
	///新股申购
	const char TORA_TSTP_BC_SubscribingShares = '4';
	///正回购
	const char TORA_TSTP_BC_Repurchase = '5';
	///逆回购
	const char TORA_TSTP_BC_ReverseRepur = '6';
	///开放式基金申购
	const char TORA_TSTP_BC_OeFundPur = '8';
	///开放式基金赎回
	const char TORA_TSTP_BC_OeFundRed = '9';
	///担保品划入
	const char TORA_TSTP_BC_CollateralIn = 'a';
	///担保品划出
	const char TORA_TSTP_BC_CollateralOut = 'b';
	///质押入库
	const char TORA_TSTP_BC_PledgeIn = 'd';
	///质押出库
	const char TORA_TSTP_BC_PledgeOut = 'e';
	///配股配债
	const char TORA_TSTP_BC_Rationed = 'f';
	///开放式基金拆分
	const char TORA_TSTP_BC_Split = 'g';
	///开放式基金合并
	const char TORA_TSTP_BC_Merge = 'h';
	///融资买入
	const char TORA_TSTP_BC_CreditBuy = 'i';
	///融券卖出
	const char TORA_TSTP_BC_CreditSell = 'j';
	///卖券还款
	const char TORA_TSTP_BC_SellRepay = 'k';
	///买券还券
	const char TORA_TSTP_BC_BuyRepay = 'l';
	///还券划转
	const char TORA_TSTP_BC_RepayTransfer = 'm';
	///余券划转
	const char TORA_TSTP_BC_SurplusTransfer = 'n';
	///源券划转
	const char TORA_TSTP_BC_SourceTransfer = 'o';
	///卖券强平还款(内部使用)
	const char TORA_TSTP_BC_ForceSellRepay = 'p';
	///买券强平还券(内部使用)
	const char TORA_TSTP_BC_ForceBuyRepay = 'q';
	///负债展期(内部使用)
	const char TORA_TSTP_BC_DebtExtend = 'r';
	///转托管
	const char TORA_TSTP_BC_CustodyTransfer = 's';
	///债券转股
	const char TORA_TSTP_BC_BondConvertStock = 't';
	///债券回售
	const char TORA_TSTP_BC_BondPutback = 'u';
	///ETF实物申购
	const char TORA_TSTP_BC_ETFOtPur = 'v';
	///ETF实物赎回
	const char TORA_TSTP_BC_ETFOtRed = 'w';
	///回售撤销
	const char TORA_TSTP_BC_PutbackRelieve = 'x';
	typedef char TTORATstpBizClassType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpBrokerageTypeType是一个佣金类型类型
	/////////////////////////////////////////////////////////////////////////
	///毛佣金
	const char TORA_TSTP_BT_Gross = '0';
	///净佣金
	const char TORA_TSTP_BT_Net = '1';
	typedef char TTORATstpBrokerageTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpLongVolumeType是一个LongVolume类型
	/////////////////////////////////////////////////////////////////////////
	#ifdef WINDOWS
	typedef __int64 TTORATstpLongVolumeType;
	#else
	typedef long long int TTORATstpLongVolumeType;
	#endif
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpOperateSourceType是一个操作来源类型
	/////////////////////////////////////////////////////////////////////////
	///实时上场
	const char TORA_TSTP_OPRTSRC_DBCommand = '0';
	///API调用
	const char TORA_TSTP_OPRTSRC_SyncAPI = '1';
	///自动触发
	const char TORA_TSTP_OPRTSRC_AutoTrigger = '2';
	///API紧急上场
	const char TORA_TSTP_OPRTSRC_EmergencyAPI = '3';
	typedef char TTORATstpOperateSourceType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpExternalSerialType是一个外部流水号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpExternalSerialType[65];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpIPONumberIDType是一个新股申购配股号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpIPONumberIDType[31];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpSpecPrivilegeTypeType是一个特殊权限类别类型
	/////////////////////////////////////////////////////////////////////////
	///创业板
	const char TORA_TSTP_SPLT_GEM = '0';
	///风险警示板
	const char TORA_TSTP_SPLT_RiskWarning = '1';
	///退市整理板
	const char TORA_TSTP_SPLT_Delisting = '2';
	///港股通
	const char TORA_TSTP_SPLT_SZSEHK = '3';
	///科创板
	const char TORA_TSTP_SPLT_SHKC = '4';
	///创业板注册制
	const char TORA_TSTP_SPLT_GEMRegistration = '5';
	///分级基金
	const char TORA_TSTP_SPLT_StructFund = '6';
	///可转债
	const char TORA_TSTP_SPLT_ConvertBond = '7';
	///基础设施基金
	const char TORA_TSTP_SPLT_InfrastructureFund = '8';
	///定向可转债
	const char TORA_TSTP_SPLT_OrientedConvertBond = '9';
	///北交所股票
	const char TORA_TSTP_SPLT_BJStock = 'a';
	typedef char TTORATstpSpecPrivilegeTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpCreRedTypeType是一个申赎类型类型
	/////////////////////////////////////////////////////////////////////////
	///普通申赎
	const char TORA_TSTP_CRT_IS = '0';
	///实物申赎
	const char TORA_TSTP_CRT_OS = '1';
	typedef char TTORATstpCreRedTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpETFCurrenceReplaceStatusType是一个ETF现金替代标志类型
	/////////////////////////////////////////////////////////////////////////
	///禁止现金替代
	const char TORA_TSTP_ETFCTSTAT_Forbidden = '0';
	///可以现金替代
	const char TORA_TSTP_ETFCTSTAT_Allow = '1';
	///必须现金替代
	const char TORA_TSTP_ETFCTSTAT_Force = '2';
	///跨市退补现金替代
	const char TORA_TSTP_ETFCTSTAT_CBAllow = '3';
	///跨市必须现金替代
	const char TORA_TSTP_ETFCTSTAT_CBForce = '4';
	typedef char TTORATstpETFCurrenceReplaceStatusType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceTickIDType是一个价差品种类型
	/////////////////////////////////////////////////////////////////////////
	///股本证券
	const char TORA_TSTP_PTID_Stock = '0';
	///债务证券
	const char TORA_TSTP_PTID_Bond = '1';
	///股票期权
	const char TORA_TSTP_PTID_Option = '2';
	typedef char TTORATstpPriceTickIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceTickGroupIDType是一个价差组号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpPriceTickGroupIDType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPriceTickTypeType是一个价差类别类型
	/////////////////////////////////////////////////////////////////////////
	///价格上下限
	const char TORA_TSTP_PPT_LimitPrice = '0';
	///价差组
	const char TORA_TSTP_PPT_PriceTick = '1';
	typedef char TTORATstpPriceTickTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFundTypeType是一个基金类型类型
	/////////////////////////////////////////////////////////////////////////
	///标准LOF
	const char TORA_TSTP_FUT_Normal = '0';
	///分级母基金
	const char TORA_TSTP_FUT_Mother = '1';
	///分级子基金
	const char TORA_TSTP_FUT_Sub = '2';
	typedef char TTORATstpFundTypeType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpNodeInfoType是一个节点信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpNodeInfoType[33];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFensVerType是一个版本号类型
	/////////////////////////////////////////////////////////////////////////
	typedef short TTORATstpFensVerType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFensEnvIDType是一个环境编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpFensEnvIDType[13];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFensNodeIDType是一个节点编号类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpFensNodeIDType[11];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpFensUserIDType是一个用户代码类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpFensUserIDType[16];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpClientInfoType是一个终端信息类型
	/////////////////////////////////////////////////////////////////////////
	typedef char TTORATstpClientInfoType[65];
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpPortType是一个端口号类型
	/////////////////////////////////////////////////////////////////////////
	typedef int TTORATstpPortType;
	
	/////////////////////////////////////////////////////////////////////////
	/// TTORATstpMDSubModeType是一个行情订阅模式类型
	/////////////////////////////////////////////////////////////////////////
	///TCP连接模式
	const char TORA_TSTP_MST_TCP = '0';
	///UDP单播模式
	const char TORA_TSTP_MST_UDP = '1';
	///UDP组播模式
	const char TORA_TSTP_MST_MCAST = '2';
	typedef char TTORATstpMDSubModeType;
	
}
#endif // TORATSTPUSERAPIDATATYPE_H__