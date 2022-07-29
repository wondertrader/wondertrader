// code_generator_tag
// constants:912382e0db96306ce6a0563abfa85fcb types:b27cbce8a0509cca6b8d355c8f46e949 template:e3fd0e61c5b8727361552db76c1636b8 code:9eb42ad7f790a18f3d28bf6c626e4b2a
#ifndef ATP_TRADE_CONSTANTS_H
#define ATP_TRADE_CONSTANTS_H



#include "atp_trade_types.h"
#include <cstring>
#include <vector>
#include <map>
#include <set>

#include"atp_trade_export.h"

/**
 * @brief ATP协议版本信息
 */
struct TRADE_API ATPVersionTypeConst
{
    static  const  ATPVersionType  kV210 = 1;  ///< V2.10 老版本现货
    static  const  ATPVersionType  kV212 = 3;  ///< V2.12 期权做市
    static  const  ATPVersionType  kV217 = 2;  ///< V2.17 现货,ETF,非交易；217-220
    static  const  ATPVersionType  kV220 = 4;  ///< V2.20
	static  const  ATPVersionType  kV222 = 5;  ///< V2.22
	static  const  ATPVersionType  kV300 = 6;  ///< V3.0
	static  const  ATPVersionType  kV310 = 7;  ///< V3.1.0
	static  const  ATPVersionType  kV320 = 8;  ///< V3.1.2


	/**
	*   @brief 		判断是否有效
	*   @param[in] 	ver 版本信息
	*	@return		是否有效
	*/
	static bool IsValid(const ATPVersionType& ver);
};


/**
* @brief 日志级别枚举
*/
enum ATPLogLevel
{
	kTrace = 0,	///<TRACE级别
	kDebug = 1,	///<DEBUG级别
	kInfo = 2,	///<INFO级别
	kWarn = 3,	///<WARN级别
	kError = 4,	///<ERROR级别
	kFatal = 5,	///<FATAL级别
};

/**
* @brief API错误码
*/
enum ErrorCode
{
	kSuccess = 10000,	///<成功
	kNoSetClientHandler = 10001,	///<未设置处理句柄
	kLocationsError = 10002,	///<未设置ip和端口
	kUserPwdError = 10003,	///<用户名或密码错误
	kNoConnection = 10004,	///<没有建立连接
	kNoSyncFinished = 10005,	///<没有完成回报同步请求
	kAlreadyConnected = 10006,	///<已连接
	kFailure = 10007,	///<失败
	kCheckError = 10008,	///<资金账户与股东账户校验失败等
	kNotAuth = 10009,	///<该用户未认证
	kInitLogError = 10010,	///<初始化日志错误
	kLoadCfgError = 10011,	///<加载配置文件失败
	kNotInit = 10012,	///<没有Init初始化
	kSpeedLimit = 10013,	///<流速超限
	kLinkNumLimit = 10014,	///<连接数超限
	kLicenseError = 10015,	///<license 错误
	kBlackWhiteListAndFuncCodeError = 10016,	///<黑白名单和功能号检测错误
	kCustIdInBlackList = 10017,	///<客户号在黑名单
	kCustIdNotInWhiteList = 10018,	///<客户号不在白名单
	kFuncCodeNotFound = 10019,	///<功能号列表中找不到客户号对应的功能号
	kCannotFindProcessor = 10201,	///<不支持该功能
	kApiMsgIsNull = 10202,	///<消息结构为空
	kTimeInForceError = 10203,	///<订单有效时间类型取值错误
	kSideError = 10204,	///<买卖方向取值错误
	kVotingPerferenceError = 10205,	///<投票意向取值错误
	kPositionEffectError = 10206,	///<开平仓取值错误
	kCoveredOrUncoveredError = 10207,	///<备兑标签取值错误
	kOrderWayError = 10208,	///<委托方式取值错误
	kMarketIDError = 10209,	///<所属市场取值错误
	kEnforceFlagError = 10210,	///<风险强制标识取值错误
	kTransferWayError = 10211,	///<划拨方式取值错误
	kOrderTypeError = 10212,	///<订单类型取值错误
	kOrderQueryConditionError = 10213,	///<订单查询条件取值错误
	kDesignationTransTypeError = 10214,	///<转托管注册类型取值错误
	kReturnSeqError = 10215,	///<返回顺序取值错误
	kDesignatedTransactionSecurityIDError = 10216,	///<指定交易业务委托消息证券代码错误
	kPricePropertyError = 10217,	///<价格类型错误
	kTrdSubTypeError = 10218,	///<是否受限错误
	kPricingError = 10219,	///<大宗交易定价申报类型错误
	kLotTypeError = 10220,	///<订单数量类型错误
	kCurrencyError = 10221,	///<币种类型取值错误
	kRepayWayError = 10222,	///<融资融券直接还款方式取值错误
	kCloseOutFlagError = 10223,	///<融资融券了结标识取值错误
	kQueryContentsError = 10224,	///<融资融券查询内容取值错误
	kAccountIDNULLError = 10225,	///<客户号为空错误
	kCRDUnderlyingTypeError = 10226,	///<融资融券标的类型错误
	kAccountIDArrayNULLError = 10227,	///<证券账户组为空错误
	kNoMsgVersion = 11000,	///<没有这个版本的消息
	kNotNegotiateVersion = 11001,	///<没有协商版本
	kLoginModeError = 11002,	///<客户登录模式错误
	kBusinessAbstractTypeError = 11003,	///<抽象业务类型错误
	kBusinessATypeError = 11004,	///<业务类型错误
	kTradeReportTypeError = 11005,	///<成交申报类型错误
	kInvestorTypeError = 11006,	///<对手方交易主体类型错误
	kATPTransAffairTypeError = 11007,	///<成交申报事物类型错误
	kAlgoOperationError = 11008,	///<算法操作类型错误
	kEncryptSchemaError = 11009,	///<加密值ENCRYPT_SCHEMA错误
	kRsaFileError = 11010,	///<Rsa公钥错误
	kMultiChannelFlagrror = 11011,	///<消息订阅参数错误
	kEtfPurchaseRedemptionFlagError = 11012,	///<是否返回ETF申赎成交标识错误
	kAnonyLoginReportSynUnfinished = 11013,	///<匿名登录客户回报同步处理未完成
};

/**
* @brief 业务类型枚举
*/
struct TRADE_API ATPBusinessTypeConst
{
	static const ATPBusinessTypeType kDefault = 0xFF;	///<默认值
	static const ATPBusinessTypeType kAll = 0;	///<所有类型
	static const ATPBusinessTypeType kCashAuction = 1;	///<现货集中交易
	static const ATPBusinessTypeType kRepoAuction = 2;	///<质押式回购交易
	static const ATPBusinessTypeType kBondDistribution = 3;	///<债券分销
	static const ATPBusinessTypeType kOptionAuction = 4;	///<期权集中交易
	static const ATPBusinessTypeType kBlockTrade = 5;	///<协议交易(大宗交易)
	static const ATPBusinessTypeType kAfterHoursPricing = 6;	///<盘后定价交易
	static const ATPBusinessTypeType kRefinancing = 7;	///<转融通
	static const ATPBusinessTypeType kAssetTransfer = 8;	///<资产管理计划份额转让
	static const ATPBusinessTypeType kCashPledgeRepo = 9;	///<股票质押式回购
	static const ATPBusinessTypeType kAgreedRepo = 10;	///<约定购回
	static const ATPBusinessTypeType kPledgeQuoteRepo = 11;	///<质押式报价回购
	static const ATPBusinessTypeType kETFRealTimePurchaseRedemption = 12;	///<ETF实时申购赎回
	static const ATPBusinessTypeType kIssue = 13;	///<网上发行认购
	static const ATPBusinessTypeType kRightsIssue = 14;	///<配售
	static const ATPBusinessTypeType kSwapPutback = 15;	///<债券转股回售
	static const ATPBusinessTypeType kOptionExercise = 16;	///<期权行权
	static const ATPBusinessTypeType kLOF = 17;	///<开放式基金申赎
	static const ATPBusinessTypeType kTenderOffer = 18;	///<要约收购
	static const ATPBusinessTypeType kPledgeInOrOut = 19;	///<质押出入库业务
	static const ATPBusinessTypeType kPledgeRepoPledgeOrSign = 19;	///<质押式回购质押解押
	static const ATPBusinessTypeType kDesignation = 20;	///<转托管
	static const ATPBusinessTypeType kVoting = 21;	///<网络投票
	static const ATPBusinessTypeType kDesignatedTransaction = 22;	///<指定交易
	static const ATPBusinessTypeType kWarrantsExercise = 23;	///<权证行权
	static const ATPBusinessTypeType kPasswordService = 24;	///<密码服务
	static const ATPBusinessTypeType kOpenMarginQuery = 25;	///<保证金查询
	static const ATPBusinessTypeType kOpenFundSetting = 26;	///<开放式基金转托管、分红方式设置
	static const ATPBusinessTypeType kTurnToDisposal = 27;	///<转处置
	static const ATPBusinessTypeType kCouponSwap = 28;	///<垫券还券
	static const ATPBusinessTypeType kPayOff = 29;	///<待清偿扣划
	static const ATPBusinessTypeType kBondPledgeAgreementRepo = 30;	///<债券质押式协议回购
	static const ATPBusinessTypeType kStructuredFund = 31;	///<分级基金实时分拆合并
	static const ATPBusinessTypeType kCoverLockOrUnlock = 32;	///<备兑锁定解锁
	static const ATPBusinessTypeType kOptionCombinationExercise = 33;	///<合并行权
	static const ATPBusinessTypeType kOptionCombination = 34;	///<组合保证金
	static const ATPBusinessTypeType kOptionCoveredTransUncovered = 35;	///<期权普通与备兑互转
	static const ATPBusinessTypeType kReduceHoldingTransfer = 36;	///<高管额度划转
	static const ATPBusinessTypeType kBondConsultTrade = 37;	///<债券协商成交
	static const ATPBusinessTypeType kHKStocksThrough = 63;	///<港股通
	static const ATPBusinessTypeType kHKVoting = 64;	///<港股通投票
	static const ATPBusinessTypeType kHKDesignationTransfer = 61;	///<港股通转托管
	static const ATPBusinessTypeType kHKCorporateAction = 62;	///<港股通公司行为
	static const ATPBusinessTypeType kTibAfterHour = 97;	///<盘后定价(科创板、创业板)交易业务
	static const ATPBusinessTypeType kMarginTrade = 99;	///<融资融券交易业务
	static const ATPBusinessTypeType kMarginNoTrade = 100;	///<融资融券非交易业务
	static const ATPBusinessTypeType kMarginRepay = 101;	///<融资融券交易偿还
	static const ATPBusinessTypeType kMarginDirectRepay = 102;	///<融资融券直接还款
	static const ATPBusinessTypeType kAlgoBusiness = 103;	///<算法业务

};
/**
* @brief 账户登录模式枚举
*/
struct TRADE_API ATPCustLoginModeType
{
	static const ATPLoginModeType kCustIDMode = 1;	///<客户号登录模式
	static const ATPLoginModeType kFundAccountIDMode = 2;	///<资金账户登录模式
	static const ATPLoginModeType kAccountIDMode = 3;	///<证券账户登录模式
	static bool IsValid(const ATPLoginModeType& value);

};
/**
* @brief 所属账户角色枚举
*/
struct TRADE_API ATPAccountRoleConst
{
	static const ATPAccountRole kNormal = 0;	///<普通A股账户
	static const ATPAccountRole kFund = 1;	///<基金账户
	static const ATPAccountRole kMargin = 2;	///<信用账户
	static const ATPAccountRole kLoan = 3;	///<融券专用账户
	static const ATPAccountRole kQFII = 4;	///<QFII账户
	static const ATPAccountRole kShareB = 5;	///<普通B股账户
	static bool IsValid(const ATPAccountRole& value);

};
/**
* @brief 证券所属市场枚举
*/
struct TRADE_API ATPMarketIDConst
{
	static const ATPMarketIDType kNULL = 0;	///<空
	static const ATPMarketIDType kShangHai = 101;	///<上海
	static const ATPMarketIDType kShenZhen = 102;	///<深圳
	static bool IsValid(const ATPMarketIDType& value);

};
/**
* @brief 订单有效时间类型枚举
*/
struct TRADE_API ATPTimeInForceConst
{
	static const ATPTimeInForceType kDefault = ' ';	///<默认值
	static const ATPTimeInForceType kGFD = '0';	///<当日有效
	static const ATPTimeInForceType kIOC = '3';	///<即时成交剩余撤销
	static const ATPTimeInForceType kFOK = '4';	///<即时全部成交或撤销,适用于上交所
	static const ATPTimeInForceType kAtCrossing = '9';	///<竞价限价盘,适用于深交所
static bool IsValid(const ATPBusinessTypeType& busi, const ATPTimeInForceType& time);
};
/**
* @brief 信用标识枚举
*/
struct TRADE_API ATPCashMarginConst
{
	static const ATPCashMarginType kDefault = ' ';	///<默认值
	static const ATPCashMarginType kCash = '1';	///<普通交易
	static const ATPCashMarginType kOpen = '2';	///<融资融券开仓
	static const ATPCashMarginType kClose = '3';	///<融资融券平仓
	static bool IsValid(const ATPCashMarginType& value);

};
/**
* @brief 批量委托类型枚举
*/
struct TRADE_API ATPBatchTypeConst
{
	static const ATPBatchType kDefault = 0;	///<默认值
	static const ATPBatchType kEqual = 1;	///<单代码等量拆单
	static const ATPBatchType kDecline = 2;	///<单代码递减拆单
	static const ATPBatchType kBatch = 3;	///<多代码批量报单
	static bool IsValid(const ATPBatchType& value);

};
/**
* @brief 买卖方向枚举
*/
struct TRADE_API ATPSideConst
{
	static const ATPSideType kDefault = ' ';	///<默认值
	static const ATPSideType kAll = '0';	///<所有
	static const ATPSideType kBuy = '1';	///<买
	static const ATPSideType kSell = '2';	///<卖
	static const ATPSideType kRepo = '3';	///<正回购
	static const ATPSideType kAntiRepo = '4';	///<逆回购
	static const ATPSideType kSwap = '5';	///<转股
	static const ATPSideType kPutBack = '6';	///<回售
	static const ATPSideType kTenderOffer = '7';	///<预受要约
	static const ATPSideType kDisTenderOffer = '8';	///<解除预受要约
	static const ATPSideType kPutBackRevoke = '9';	///<回售撤销
	static const ATPSideType kActivation = 'A';	///<密码激活
	static const ATPSideType kCancellation = 'C';	///<密码注销
	static const ATPSideType kPurchase = 'D';	///<申购
	static const ATPSideType kRedeem = 'E';	///<赎回
	static const ATPSideType kLoan = 'F';	///<出借
	static const ATPSideType kBorrow = 'G';	///<借入
	static const ATPSideType kPhysicalPurchase = 'P';	///<实物申购
	static const ATPSideType kPhysicalRedemption = 'R';	///<实物赎回
	static const ATPSideType kOpen = 'O';	///<分拆
	static const ATPSideType kMerge = 'M';	///<合并
	static const ATPSideType kQuery = 'Q';	///<查询
	static const ATPSideType kDesignationTransfer = 'J';	///<转托管
	static const ATPSideType kReverDsignationTransfer = 'K';	///<反向托管
	static const ATPSideType kLock = 'Z';	///<锁定
	static const ATPSideType kUnLock = 'Y';	///<解锁
	static const ATPSideType kDisposal = '@';	///<转处置
	static const ATPSideType kDisposalReturn = '%';	///<转处置返还
	static const ATPSideType kPledgeIn = 'H';	///<质押入库
	static const ATPSideType kPledgeOut = 'I';	///<质押出库
	static const ATPSideType kSellRepay = 'N';	///<卖券还款
	static const ATPSideType kBuyRedeliver = 'U';	///<买券还券
	static const ATPSideType kFinancingBuy = 'B';	///<融资买入
	static const ATPSideType kLoanSell = 'S';	///<融券卖出
	static const ATPSideType kReDeliver = 'T';	///<直接还券
	static const ATPSideType kGageIn = 'V';	///<担保品提交
	static const ATPSideType kGageOut = 'L';	///<担保品返还
	static const ATPSideType kMarginOffset = 'W';	///<融资平仓
	static const ATPSideType kShortSaleOffset = 'X';	///<融券平仓
	static const ATPSideType kSourceSecurityIn = '$';	///<券源提交
	static const ATPSideType kSourceSecurityOut = '*';	///<券源返还
	static const ATPSideType kExtraSecurityOut = '#';	///<余券划转
static bool IsValid(const ATPSideType& value);
static bool IsValid(const ATPBusinessTypeType& busi, const ATPSideType& side);
};
/**
* @brief 了结标志枚举
*/
struct TRADE_API ATPCloseOutFlagConst
{
	static const ATPCloseOutFlagType kDefault = ' ';	///<默认值
	static const ATPCloseOutFlagType kAll = '0';	///<所有
	static const ATPCloseOutFlagType kNotCloseOut = '1';	///<未了结
	static const ATPCloseOutFlagType kCloseOut = '2';	///<了结
	static bool IsValid(const ATPCloseOutFlagType& value);

};
/**
* @brief 查询内容枚举
*/
struct TRADE_API ATPQueryContentsConst
{
	static const ATPQueryContentsType kDefault = ' ';	///<默认值
	static const ATPQueryContentsType kBalance = '0';	///<只查询余额
	static const ATPQueryContentsType kBalanceDetail = '1';	///<查询余额及明细
	static bool IsValid(const ATPQueryContentsType& value);

};
/**
* @brief 融资融券标的类型枚举
*/
struct TRADE_API ATPCRDUnderlyingTypeConst
{
	static const ATPCRDUnderlyingTypeType kDefault = 0xffff;	///<默认值
	static const ATPCRDUnderlyingTypeType kAll = 0;	///<全部-用于查询
	static const ATPCRDUnderlyingTypeType kCollateral = 1;	///<担保品
	static const ATPCRDUnderlyingTypeType kSecuritiesLending = 2;	///<融券
	static const ATPCRDUnderlyingTypeType kMarginTrading = 3;	///<融资
	static bool IsValid(const ATPCRDUnderlyingTypeType& value);

};
/**
* @brief 执行类型枚举
*/
struct TRADE_API ATPExecTypeConst
{
	static const ATPExecTypeType kDefault = ' ';	///<默认值
	static const ATPExecTypeType kNew = '0';	///<新订单
	static const ATPExecTypeType kInternal = '1';	///<内部执行
	static const ATPExecTypeType kCancelled = '4';	///<已撤销
	static const ATPExecTypeType kReject = '8';	///<已拒绝
	static const ATPExecTypeType kTrade = 'F';	///<已成交
	static const ATPExecTypeType kGold = 'J';	///<黄金交易所成交
	static bool IsValid(const ATPExecTypeType& value);

};
/**
* @brief 订单类型枚举
*/
struct TRADE_API ATPOrdTypeConst
{
	static const ATPOrdTypeType kDefault = ' ';	///<默认值
	static const ATPOrdTypeType kTransferFixed = 'K';	///<市价剩余转限价,适用于上交所（已弃用）
	static const ATPOrdTypeType kFixedNew = 'a';	///<限价委托、增强限价
	static const ATPOrdTypeType kLocalOptimalNew = 'b';	///<本方最优
	static const ATPOrdTypeType kCountPartyOptimalTransferFixed = 'c';	///<对手方最优剩余转限价
	static const ATPOrdTypeType kImmediateDealTransferCancel = 'd';	///<立即成交剩余撤销
	static const ATPOrdTypeType kFullDealOrCancel = 'e';	///<全额成交或撤销
	static const ATPOrdTypeType kOptimalFiveLevelFullDealTransferCancel = 'f';	///<最优五档全额成交剩余撤销
	static const ATPOrdTypeType kOptimalFiveLevelImmediateDealTransferFixed = 'g';	///<最优五档即时成交剩余转限价
	static const ATPOrdTypeType kMarketTransferFixed = 'h';	///<市价剩余转限价
	static const ATPOrdTypeType kFixedFullDealOrCancel = 'i';	///<限价全额成交或撤销
	static const ATPOrdTypeType kSzBiddingFixed = 'j';	///<竞价限价（深圳）（用于港股通业务）
	static const ATPOrdTypeType kShBiddingFixed = 'k';	///<竞价限价（上海)（用于港股通业务）
	static const ATPOrdTypeType kMarket = '1';	///<市价（已弃用）
	static const ATPOrdTypeType kFixed = '2';	///<限价（已弃用）
	static const ATPOrdTypeType kLocalOptimal = 'U';	///<本方最优（已弃用）
	static bool IsValid(const ATPOrdTypeType& value);

};
/**
* @brief 投票意向枚举
*/
struct TRADE_API ATPVotingPreferenceConst
{
	static const ATPVotingPreferenceType kDefault = ' ';	///<默认值
	static const ATPVotingPreferenceType kConsent = '1';	///<同意
	static const ATPVotingPreferenceType kAgainst = '2';	///<反对
	static const ATPVotingPreferenceType kWaiver = '3';	///<弃权
	static bool IsValid(const ATPVotingPreferenceType& value);

};
/**
* @brief 开平仓标识枚举
*/
struct TRADE_API ATPPositionEffectConst
{
	static const ATPPositionEffectType kDefault = ' ';	///<默认值
	static const ATPPositionEffectType kOpen = 'O';	///<开仓
	static const ATPPositionEffectType kClose = 'C';	///<平仓
	static bool IsValid(const ATPPositionEffectType& value);

};
/**
* @brief 备兑标签枚举
*/
struct TRADE_API ATPCoveredOrUncoveredConst
{
	static const ATPCoveredOrUncoveredType kCovered = 0;	///<备兑仓
	static const ATPCoveredOrUncoveredType kUnCovered = 1;	///<普通仓
	static const ATPCoveredOrUncoveredType kDefaultCovered = 0xff;	///<默认值
	static bool IsValid(const ATPCoveredOrUncoveredType& value);

};
/**
* @brief 转托管注册类型枚举
*/
struct TRADE_API ATPDesignationTransTypeConst
{
	static const ATPDesignationTransTypeType kDdefault = 0xFF;	///<默认值
	static const ATPDesignationTransTypeType kNew = 1;	///<新注册请求
	static const ATPDesignationTransTypeType kCancel = 3;	///<注册撤销
	static bool IsValid(const ATPDesignationTransTypeType& value);

};
/**
* @brief 订单查询条件枚举
*/
struct TRADE_API ATPOrderQueryConditionConst
{
	static const ATPOrderQueryConditionType kDefault = 0xFF;	///<默认值
	static const ATPOrderQueryConditionType kAll = 0;	///<查询全部（不含撤单委托）
	static const ATPOrderQueryConditionType kCancelAvailable = 1;	///<查询可撤单订单
	static const ATPOrderQueryConditionType kTradeExecOrCanceled = 2;	///<查询有成交或已经撤单订单
	static const ATPOrderQueryConditionType kUnavailableCancel = 3;	///<查询不可撤单订单
	static const ATPOrderQueryConditionType AllWithCancleOrd = 4;	///<查询全部委托（含撤单委托）
	static bool IsValid(const ATPOrderQueryConditionType& value);

};
/**
* @brief 返回顺序枚举
*/
struct TRADE_API ATPReturnSeqConst
{
	static const ATPReturnSeqType kDefault = 0xFF;	///<默认值
	static const ATPReturnSeqType kTimeOrder = 1;	///<时间正序
	static const ATPReturnSeqType kTimeOrderRe = 2;	///<时间倒序
	static bool IsValid(const ATPReturnSeqType& value);

};
/**
* @brief 划拨方式枚举
*/
struct TRADE_API ATPTransferWayConst
{
	static const ATPTransferWayType kDefault = 0xFF;	///<默认值
	static const ATPTransferWayType kIn = 1;	///<转入
	static const ATPTransferWayType kOut = 2;	///<转出
	static const ATPTransferWayType kQuery = 4;	///<可划入资金查询
	static bool IsValid(const ATPTransferWayType& value);

};
/**
* @brief 风险强制标识枚举
*/
struct TRADE_API ATPEnforceFlagConst
{
	static const ATPEnforceFlagType kDefault = '0';	///<默认
	static const ATPEnforceFlagType kForce = '1';	///<强制通过
	static bool IsValid(const ATPEnforceFlagType& value);

};
/**
* @brief 指定交易业务委托消息证券代码枚举
*/
struct TRADE_API ATPDesignatedTransactionSecurityIDConst
{
	static const ATPDesignatedTransactionSecurityIDType kCancel;	///<撤销指定交易
	static const ATPDesignatedTransactionSecurityIDType kLogin;	///<登记指定交易
	static bool IsValid(const ATPDesignatedTransactionSecurityIDType& value);

};
/**
* @brief 权利仓类型枚举
*/
struct TRADE_API ATPLegSideConst
{
	static const ATPLegSideType kDefaultPosition = ' ';	///<默认值
	static const ATPLegSideType kLongPosition = '1';	///<权利仓
	static const ATPLegSideType kShortPosition = '2';	///<义务仓
	static bool IsValid(const ATPLegSideType& value);

};
/**
* @brief 价格类型枚举
*/
struct TRADE_API ATPPricePropertyConst
{
	static const ATPPricePropertyType kDefault = 0xFF;	///<默认值
	static const ATPPricePropertyType kFixed = 1;	///<盘后固定价格
	static const ATPPricePropertyType kWeighted = 2;	///<盘后加权平均价(暂深圳支持)
	static bool IsValid(const ATPPricePropertyType& value);

};
/**
* @brief 是否受限枚举
*/
struct TRADE_API ATPTrdSubTypeConst
{
	static const ATPTrdSubTypeType kDefault = 0xFF;	///<默认值
	static const ATPTrdSubTypeType kLimitedShare = 1;	///<受限股份
	static const ATPTrdSubTypeType kUnLimitedShare = 0;	///<非受限股份
	static bool IsValid(const ATPTrdSubTypeType& value);

};
/**
* @brief 大宗交易定价申报类型枚举
*/
struct TRADE_API ATPPricingConst
{
	static const ATPPricingType kDefault = 0xFF;	///<默认值
	static const ATPPricingType kPricing = 1;	///<定价申报
	static const ATPPricingType kTransaction = 2;	///<定价成交申报
	static bool IsValid(const ATPPricingType& value);

};
/**
* @brief 订单状态枚举
*/
struct TRADE_API ATPOrdStatusConst
{
	static const ATPOrdStatusType kDefault = 0xFF;	///<默认值
	static const ATPOrdStatusType kNew = 0;	///<已申报
	static const ATPOrdStatusType kPartiallyFilled = 1;	///<部分成交
	static const ATPOrdStatusType kFilled = 2;	///<全部成交
	static const ATPOrdStatusType kPartiallyFilledPartiallyCancelled = 3;	///<部分成交部分撤销
	static const ATPOrdStatusType kCancelled = 4;	///<已撤销
	static const ATPOrdStatusType kPartiallyCancelled = 5;	///<部分撤销
	static const ATPOrdStatusType kReject = 8;	///<已拒绝
	static const ATPOrdStatusType kUnSend = 9;	///<未申报
	static const ATPOrdStatusType kSended = 10;	///<正报
	static const ATPOrdStatusType kWaitCancelled = 11;	///<待撤销
	static const ATPOrdStatusType kPartiallyFilledWaitCancelled = 12;	///<部成待撤
	static const ATPOrdStatusType kProcessed = 13;	///<已处理
	static bool IsValid(const ATPOrdStatusType& value);

};
/**
* @brief 货币种类枚举
*/
struct TRADE_API ATPCurrencyConst
{
	static const ATPCurrencyType kCNY;	///<人民币
	static const ATPCurrencyType kUSD;	///<美元
	static const ATPCurrencyType kHKD;	///<港币
	static bool IsValid(const ATPCurrencyType& value);

};
/**
* @brief 还款方式枚举
*/
struct TRADE_API ATPRepayWayConst
{
	static const ATPRepayWayType kDefault = 0xFF;	///<默认值
	static const ATPRepayWayType kDirect = 0;	///<直接还款
	static const ATPRepayWayType kIntent = 1;	///<意向还款
	static bool IsValid(const ATPRepayWayType& value);

};
/**
* @brief 订单数量类型枚举
*/
struct TRADE_API ATPLotTypeConst
{
	static const ATPLotTypeType kDefalut = ' ';	///<默认值
	static const ATPLotTypeType kOddLot = '1';	///<零股订单
	static const ATPLotTypeType kRoundLot = '2';	///<整手订单
	static bool IsValid(const ATPLotTypeType& value);

};
/**
* @brief 查询结果错误码
*/
struct TRADE_API ATPQueryResultCodeConst
{
	static const ATPQueryResultCodeType kDefault = 0xFFFF;	///<默认值
	static const ATPQueryResultCodeType kSuccess = 0;	///<成功
	static const ATPQueryResultCodeType kUnKnownBusinessType = 1;	///<未知业务类型
	static const ATPQueryResultCodeType kInvalidSecurityID = 2;	///<无效的证券代码
	static const ATPQueryResultCodeType kInvalidSide = 3;	///<交易方向错误
	static const ATPQueryResultCodeType kPriceLessThanZero = 4;	///<价格小于0
	static const ATPQueryResultCodeType kPriceLessThanUnit = 5;	///<价格小于价格档位
	static const ATPQueryResultCodeType kUnitPriceLessEqualZero = 6;	///<单位数量价格小于等于0
	static const ATPQueryResultCodeType kInvalidFundId = 7;	///<无效的资金帐号
	static const ATPQueryResultCodeType kNotFoundOrder = 8;	///<未找到指定的订单
	static const ATPQueryResultCodeType kNotFoundTER = 9;	///<未找到指定的成交回报
	static const ATPQueryResultCodeType kNotFoundShare = 10;	///<未找到股份
	static const ATPQueryResultCodeType kSharePositionNotExist = 11;	///<账户无此持仓
	static const ATPQueryResultCodeType kSharePositionInsufficient = 12;	///<账户证券持仓不足一手
	static const ATPQueryResultCodeType kFundInsufficient = 13;	///<账户可用资金不足一手
	static const ATPQueryResultCodeType kShareTradeRuleNotExist = 14;	///<没找到股份交易规则
	static const ATPQueryResultCodeType kShareDataError = 15;	///<证券基础数据错误
	static const ATPQueryResultCodeType kFundTradeRuleNotExist = 16;	///<资金交易规则不存在
	static const ATPQueryResultCodeType kInvalidAccountID = 17;	///<证券账户不存在
	static const ATPQueryResultCodeType kCustIDNonExist = 18;	///<客户号在客户信息表中不存在
	static const ATPQueryResultCodeType kCustPWDError = 19;	///<客户密码错误
	static const ATPQueryResultCodeType kCustIDNotEnable = 20;	///<客户号未启用
	static const ATPQueryResultCodeType kETFCashComponentOverRange = 21;	///<ETF申购超最大现金替代比例
	static const ATPQueryResultCodeType kNotFoundETFComponentSecurity = 22;	///<未找到etf的成分股基础证券信息
	static const ATPQueryResultCodeType kNotFoundBizParam = 23;	///<获取业务参数失败
	static const ATPQueryResultCodeType kNotFoundStandardShare = 24;	///<获取质押券对应的标准券总量失败
	static const ATPQueryResultCodeType kNotFoundBondAssets = 25;	///<未找到客户债券托管量
	static const ATPQueryResultCodeType kNotFoundNetAssets = 26;	///<未找到客户净资产
	static const ATPQueryResultCodeType kFundAccountIDNotExist = 27;	///<资金帐号不存在
	static const ATPQueryResultCodeType kOrderPriceRangeOut = 28;	///<价格错误
	static const ATPQueryResultCodeType kHKDTIDBothInsufficient = 29;	///<港股转托管委托不能同时选择证券编码和权益编码
	static const ATPQueryResultCodeType kHKDTHaveAll = 30;	///<港股转托管已经做过全部转托管不能再做转托管
	static const ATPQueryResultCodeType kFeeInsufficient = 31;	///<费用不足
	static const ATPQueryResultCodeType kNotFoundContractSpecifications = 32;	///<融资融券合约明细不存在
	static const ATPQueryResultCodeType kNotFoundSpecialAccount = 33;	///<资金专户不存在
	static const ATPQueryResultCodeType kNotFoundCreditInfo = 34;	///<信用资料不存在
	static const ATPQueryResultCodeType kNotFoundCreditLimit = 35;	///<信用额度不存在
	static const ATPQueryResultCodeType kErrorOccursOnCalcMarginBalance = 36;	///<计算保证金余额时发生错误
	static const ATPQueryResultCodeType kNoSecuritySpecialAccount = 37;	///<融券专户不存在
	static const ATPQueryResultCodeType kNotFoundShareLiability = 38;	///<未找到融资融券股份负债
	static const ATPQueryResultCodeType kBizParamInvalid = 39;	///<回购业务参数表中的入库集中度上限字段为0或质押出入库业务规则表中的上海市场信用债总发行量为0
	static const ATPQueryResultCodeType kPledgeCentrationNoPassed = 40;	///<集中度检查不通过
	static const ATPQueryResultCodeType kFundTradeTemplateNotExist = 42;	///<找不到资金交易规则模板
	static const ATPQueryResultCodeType kFinanceGuaranteeRatioNotExist = 43;	///<融资保证金比例不存在
	static const ATPQueryResultCodeType kSecurityGuaranteeRatioNotExist = 44;	///<融券保证金比例不存在
	static const ATPQueryResultCodeType kCrdUnderlyingNonExist = 45;	///<该证券在标的清单中不存在
	static const ATPQueryResultCodeType kNotCollateral = 46;	///<该证券非担保品标的
	static const ATPQueryResultCodeType kNotFoundFundLiability = 47;	///<未找到融资融券资金负债
	static const ATPQueryResultCodeType kMntGuaranteeRatioNotExist = 48;	///<维持担保比例不存在
	static const ATPQueryResultCodeType kNotAvailExtraSecurity = 49;	///<没有可用余券
	static const ATPQueryResultCodeType kNotCrdBuyUnderlying = 50;	///<非融资标的
	static const ATPQueryResultCodeType kNotCrdSellUnderlying = 51;	///<非融券标的
	static const ATPQueryResultCodeType kSellQtyUnitLessEqualZero = 52;	///<卖数量单位小于等于0
	static const ATPQueryResultCodeType kSpecialAccountNotAllowTheBusiness = 53;	///<两融专户禁止该交易业务
	static const ATPQueryResultCodeType kBuyPrivateAmountNotFound = 54;	///<融资保底额度信息不存在
	static const ATPQueryResultCodeType kSellPrivateAmountNotFound = 55;	///<融券保底额度信息不存在
	static const ATPQueryResultCodeType kFinanSpecialAccountNotAllowTheBusiness = 56;	///<融资专户禁止该交易业务
	static const ATPQueryResultCodeType kGetCustFeeConfigFail = 57;	///<未获取到汇率信息
	static const ATPQueryResultCodeType kFundNotSupportCurrency = 58;	///<资金账号不支持该币种
	static const ATPQueryResultCodeType kGetSecurityVarietyFail = 59;	///<证券类型对应的证券品种不存在
	static const ATPQueryResultCodeType kFundFeeInfoFail = 60;	///<客户费用信息不存在
	static const ATPQueryResultCodeType kNoQuoteInfo = 61;	///<无行情数据
	static const ATPQueryResultCodeType kDeclaredNumNotInt = 62;	///<申报数量不合法，必须为正整数
	static const ATPQueryResultCodeType kDeclaredNumGreaterThanMax = 63;	///<申报数量大于最大可委托数
	static const ATPQueryResultCodeType kGetPriceFailed = 64;	///<获取价格失败
	static const ATPQueryResultCodeType kUnknownThirdSystemType = 65;	///<融资融券未知三方系统类型
	static const ATPQueryResultCodeType kPasswordDecryptFail = 66;	///<交易密码解密失败
	static const ATPQueryResultCodeType kSendThirdPartyFail = 67;	///<发送外部系统失败
	static const ATPQueryResultCodeType kThirdPartyReject = 68;	///<外部系统拒绝
	static const ATPQueryResultCodeType kIPOIsZero = 69;	///<新股额度为零
	static const ATPQueryResultCodeType kDesignateContractInvalid = 70;	///<无效指定偿还合约
	static const ATPQueryResultCodeType kDesignateContractRedeliverTypeDeny = 71;	///<当前还券类型不支持偿还指定合约
	static const ATPQueryResultCodeType kComninationPutExPriceMustUpCallExPrice = 72;	///<期权合并行权认沽行权价格必须高于认购行权价格
	static const ATPQueryResultCodeType kCombinationLegCallOrPutConsistent = 73;	///<期权合并行权腿认沽认购类型不能相同
	static const ATPQueryResultCodeType kCombinationContractUnitDiff = 74;	///<成分合约单位不相等
	static const ATPQueryResultCodeType kOptionExerciseOverDateRange = 75;	///<期权行权操作不在规定的行权日期范围内
	static const ATPQueryResultCodeType kDesignateContractRepayControlNotSet = 76;	///<指定合约偿还未配置指定合约偿还参数

};
/**
* @brief 抽象业务编码枚举
*/
struct TRADE_API BusinessAbstractTypeConst
{
	static const BusinessAbstractType kDefault = 0xFF;	///<默认值
	static const BusinessAbstractType kCash = 1;	///<现货业务
	static const BusinessAbstractType kBrokerageOption = 2;	///<经纪期权
	static const BusinessAbstractType kMargin = 3;	///<两融
	static bool IsValid(const BusinessAbstractType& value);

};
/**
* @brief 客户登录错误码
*/
struct TRADE_API ATPPermissonErrorCodeTypeConst
{
	static const ATPPermissonErrorCodeType kSuccess = 0;	///<登录成功
	static const ATPPermissonErrorCodeType kCannotFind = 1;	///<找不到用户
	static const ATPPermissonErrorCodeType kForbidLogin = 2;	///<禁止登录
	static const ATPPermissonErrorCodeType kRepeatLogin = 3;	///<重复登录
	static const ATPPermissonErrorCodeType kPwdError = 4;	///<密码错误
	static const ATPPermissonErrorCodeType kEnterModeError = 5;	///<委托方式错误
	static const ATPPermissonErrorCodeType kNoPartion = 6;	///<未找到分区号
	static const ATPPermissonErrorCodeType kNoPbuID = 7;	///<未找到报盘交易单元
	static const ATPPermissonErrorCodeType kPassWDReEncodeError = 8;	///<密码二次加密失败

};
/**
* @brief 外部指令拒绝原因错误码
*/
struct TRADE_API ExternalInsErrorCodeConst
{
	static const ExternalInsErrorCodeType kSuccess = 0;	///<成功
	static const ExternalInsErrorCodeType kFundOutTEFundIDNonExist = 1;	///<转出资金TE找不到资金账户
	static const ExternalInsErrorCodeType kFundOutTEAccountIDNonExist = 2;	///<找不到资金转出证券账户
	static const ExternalInsErrorCodeType kFundOutTEOtherAccountIDNonExist = 3;	///<找不到资金转入证券账户
	static const ExternalInsErrorCodeType kFundOutTEInsufficient = 4;	///<资金不足扣减
	static const ExternalInsErrorCodeType kFundInTEFundIDNonExist = 5;	///<转入资金TE找不到资金账户
	static const ExternalInsErrorCodeType kFundInTEAccountIDNonExist = 6;	///<转入资金TE找不到证券账户
	static const ExternalInsErrorCodeType kFundInTEAmountOver = 7;	///<资金转入TE资金增加后超过资金数值上限
	static const ExternalInsErrorCodeType kPWDError = 8;	///<密码错误
	static const ExternalInsErrorCodeType kCustIDNonExist = 9;	///<客户号不存在
	static const ExternalInsErrorCodeType kCustIDNotAvailable = 10;	///<客户号不可用
	static const ExternalInsErrorCodeType kSameTransferAccount = 11;	///<转入转出账号相同
	static const ExternalInsErrorCodeType kFundInTEOtherAccountIDNonExist = 12;	///<找不到资金转入证券账户
	static const ExternalInsErrorCodeType kFundOutTEAmountOver = 13;	///<资金转出TE回滚后，资金增加后超过资金数值上限
	static const ExternalInsErrorCodeType kExternalInsRepeat = 14;	///<外部指令重复
	static const ExternalInsErrorCodeType kFundInValueTooSmall = 15;	///<转出资金小于等于0
	static const ExternalInsErrorCodeType kSameTransferAccountPartionNO = 16;	///<转入转出证券账号的TE分区相同
	static const ExternalInsErrorCodeType kNoTransFundRight = 17;	///<客户没有划拨资金的权限
	static const ExternalInsErrorCodeType kFundIDNotMatchOutAccountID = 18;	///<资金账户与转出证券账号不匹配
	static const ExternalInsErrorCodeType kFundIDNotMatchInAccountID = 19;	///<资金账户与转入证券账号不匹配

};
/**
* @brief 拒绝原因错误码
*/
struct TRADE_API ATPRejectReasonCodeConst
{
	static const ATPRejectReasonCodeType kNormal = 0;	///<正常
	static const ATPRejectReasonCodeType kSecurityIDNonExist = 1;	///<证券编码与市场编码在交易信息表中不存在
	static const ATPRejectReasonCodeType kSecurityBusinessSwitchClose = 2;	///<业务开关被关闭
	static const ATPRejectReasonCodeType kPbuBusinessPermissionNonExist = 3;	///<交易单元权限不存在
	static const ATPRejectReasonCodeType kEffectiveContractAccountNonExist = 4;	///<账户信息表中不存在账户
	static const ATPRejectReasonCodeType kIssueDuplicate = 5;	///<在发行业务不允许重复认购的情况下，做重复认购
	static const ATPRejectReasonCodeType kInvestorLimitTradeLimit = 6;	///<证券账户对应业务产品交易被限制
	static const ATPRejectReasonCodeType kOrderPriceRangeOut = 7;	///<委托价格在价格涨跌幅范围之外
	static const ATPRejectReasonCodeType kOrderPriceStallError = 8;	///<委托价格不为价格档位的整数倍
	static const ATPRejectReasonCodeType kOrderQtyRangeOut = 9;	///<委托数量超过数量上下限
	static const ATPRejectReasonCodeType kOrderQtyStallError = 10;	///<委托数量不为交易数量单位的整数倍
	static const ATPRejectReasonCodeType kSellOpenFundInsufficient = 11;	///<卖开仓保证金不足
	static const ATPRejectReasonCodeType kBuyOpenFundInsufficient = 12;	///<买开仓权利金不足
	static const ATPRejectReasonCodeType kTransactionCostInsufficient = 13;	///<交易费用资金不足
	static const ATPRejectReasonCodeType kInvestorContractPositionLimitOut = 14;	///<客户标的合约持仓超过限额
	static const ATPRejectReasonCodeType kMemberContractPositionLimitOut = 15;	///<券商标的合约持仓超过限额
	static const ATPRejectReasonCodeType kInvestorContractAmountLimitOut = 16;	///<客户标的合约交易量超过限额
	static const ATPRejectReasonCodeType kMemberContractAmountLimitOut = 17;	///<券商标的合约交易量超过限额
	static const ATPRejectReasonCodeType kClosePositionInsufficient = 18;	///<平仓可平量不足
	static const ATPRejectReasonCodeType kCoverOpenPositionShareInsufficient = 19;	///<备兑开仓标的券股份不足
	static const ATPRejectReasonCodeType kMarketNotOpen = 20;	///<当前市场状态不允许申报委托
	static const ATPRejectReasonCodeType kBranchBusinessPermissionNonExist = 21;	///<营业部无对应业务权限
	static const ATPRejectReasonCodeType kUnableToCancelled = 22;	///<订单无法被撤销
	static const ATPRejectReasonCodeType kInvestorGradedNotEnough = 23;	///<证券账户无对应业务产品权限
	static const ATPRejectReasonCodeType kBuyOpenFundOver = 24;	///<期权买入开仓超过客户买入额度限制
	static const ATPRejectReasonCodeType kAccountBusinessPermissionNonPrivileged = 25;	///<证券账户业务和产品无权限
	static const ATPRejectReasonCodeType kAccountIDNonExist = 26;	///<证券账户在证券账户信息表中不存在
	static const ATPRejectReasonCodeType kFundInsufficient = 27;	///<账户可用资金不足
	static const ATPRejectReasonCodeType kShareInsufficient = 28;	///<账户持有股份不足
	static const ATPRejectReasonCodeType kSecurityMarketInconsistency = 29;	///<发行申购证券与账户所属市场不一致
	static const ATPRejectReasonCodeType kBusinessForbid = 30;	///<投资者的业务被黑名单限制
	static const ATPRejectReasonCodeType kBusinessWhiteListNonExist = 31;	///<投资者的业务不在白名单中
	static const ATPRejectReasonCodeType kAccountQualificationClassNotFound = 32;	///<合格投资者身份信息不存在
	static const ATPRejectReasonCodeType kQualificationClassOut = 33;	///<合格投资者身份不匹配
	static const ATPRejectReasonCodeType kInvestorSubscriptionQuotaTradeLimit = 34;	///<超过投资者认购额度限制
	static const ATPRejectReasonCodeType kInvestorSubscriptionQuotaNonExist = 35;	///<投资者认购额度表中不存在
	static const ATPRejectReasonCodeType kPbuBusinessNonPrivileged = 36;	///<交易单元业务权限不足
	static const ATPRejectReasonCodeType kTransfereePBUIDSameAsPBUID = 37;	///<转托管交易单元的转入和转出不能一致
	static const ATPRejectReasonCodeType kSecurityBusinessSwitchNonExist = 38;	///<证券业务开关表中不存在
	static const ATPRejectReasonCodeType kAccountBusinessPermissionNonExist = 39;	///<证券账户无该类证券交易权限
	static const ATPRejectReasonCodeType kDesignatedTransactionDenied = 40;	///<账户当前的状态不允许做指定交易
	static const ATPRejectReasonCodeType kCancelDesignatedTransactionDenied = 41;	///<账户当前的状态不允许做撤销指定交易
	static const ATPRejectReasonCodeType kDesignatedTransactionSecurityIDError = 42;	///<指定交易证券ID错误
	static const ATPRejectReasonCodeType kUnassignedPBU = 43;	///<账户未指定交易单元
	static const ATPRejectReasonCodeType kPbuSecurityNonPrivileged = 44;	///<交易单元证券权限不足
	static const ATPRejectReasonCodeType kBusinessUnsupportedInstruction = 45;	///<业务不支持当前的交易指令
	static const ATPRejectReasonCodeType kOriginOrderNotExist = 46;	///<该指令消息找不到对应的原始订单
	static const ATPRejectReasonCodeType kAccountIDNotMatchedFundAccountID = 47;	///<证券账户与资金账户不匹配
	static const ATPRejectReasonCodeType kMarketPriceNotAllowed = 48;	///<不允许市价指令
	static const ATPRejectReasonCodeType kBusinessMarketTradingPeriodNotExist = 49;	///<市场不允许该业务
	static const ATPRejectReasonCodeType kOptionExerciseOverDateRange = 50;	///<期权行权操作不在规定的行权日期范围内
	static const ATPRejectReasonCodeType kDesignatedTransactionPBUInvalid = 51;	///<指定的交易单元不在交易单元信息表中
	static const ATPRejectReasonCodeType kAccountStatusInvalid = 52;	///<当前证券账户状态不允许交易
	static const ATPRejectReasonCodeType kOptionExerciseBuyOpenQtyInsufficient = 53;	///<权利仓的净持仓量不足以行权
	static const ATPRejectReasonCodeType kSecurityStatusSuspend = 54;	///<停牌证券不允许交易
	static const ATPRejectReasonCodeType kAccountSecurityStatusNonPrivileged = 55;	///<客户未开通买入风险警示板证券的权限
	static const ATPRejectReasonCodeType kExchageHasBeenClose = 56;	///<交易所已闭市
	static const ATPRejectReasonCodeType kBuyRiskWarningOverRange = 57;	///<当日买入的风险警示板证券数量超过上限
	static const ATPRejectReasonCodeType kUnableToAllocClOrdID = 58;	///<无法分配申报合同号
	static const ATPRejectReasonCodeType kNotInTEAcceptOrderPeriod = 59;	///<不在引擎接受订单的时间范围内
	static const ATPRejectReasonCodeType kInRejectCancelOrderPeriod = 60;	///<当前时间禁止撤单
	static const ATPRejectReasonCodeType kDesignationSecurityInsufficient = 61;	///<可转托管的证券余量不足
	static const ATPRejectReasonCodeType kFundTradeTemplateNotExist = 62;	///<找不到订单对应的资金交易规则模板
	static const ATPRejectReasonCodeType kPurchaseNotAllowed = 63;	///<当前证券代码不允许申购指令
	static const ATPRejectReasonCodeType kRedemptionNotAllowed = 64;	///<当前证券代码不允许赎回指令
	static const ATPRejectReasonCodeType kPurchaseRedemptionLimitOut = 65;	///<单个账户ETF委托数量超过账户累计申赎总额限制
	static const ATPRejectReasonCodeType kNetPurchaseRedemptionLimitOut = 66;	///<单个账户ETF委托数量超过账户净申赎总额限制
	static const ATPRejectReasonCodeType kSecurityIsNotRightsIssueWarrant = 67;	///<配股配债时订单输入证券代码需为配售代码
	static const ATPRejectReasonCodeType kBusinessDateNotAllowed = 68;	///<当前日期非配售日期
	static const ATPRejectReasonCodeType kETFPurRedCancelNotAllowed = 69;	///<ETF申赎不允许撤单
	static const ATPRejectReasonCodeType kETFCashComponentOverRange = 70;	///<ETF申购超过最大现金替代比例
	static const ATPRejectReasonCodeType kCannotFindOfferWay = 71;	///<找不到对应的报盘通道
	static const ATPRejectReasonCodeType kBusinessDateNotAllowedIssue = 72;	///<该证券不在认购期内
	static const ATPRejectReasonCodeType kVotingProposalNotExist = 73;	///<投票议案不存在
	static const ATPRejectReasonCodeType kCustQualificationInfoNotFound = 74;	///<投资者适当性信息不存在
	static const ATPRejectReasonCodeType kProfessionalKindNotMatchBiz = 75;	///<专业投资者身份不符合业务要求
	static const ATPRejectReasonCodeType kRiskLevelNotMatchBiz_1 = 76;	///<客户风险等级不满足业务要求
	static const ATPRejectReasonCodeType kRiskLevelNotMatchBiz_2 = 77;	///<客户风险等级不满足业务最低要求
	static const ATPRejectReasonCodeType kRiskLevelNotMatchBiz_3 = 78;	///<客户风险等级与业务的不匹配
	static const ATPRejectReasonCodeType kInvestVarietyNotMatchBiz = 79;	///<客户投资品种集合与业务的不匹配
	static const ATPRejectReasonCodeType kDeficitLevelNotMatchBiz = 80;	///<客户可承受亏损等级与业务的不匹配
	static const ATPRejectReasonCodeType kProfessionalKindNotMatchSec = 81;	///<专业投资者身份不符合标的要求
	static const ATPRejectReasonCodeType kInvestPeridNotMatchSec = 82;	///<客户投资期限不满足标的要求
	static const ATPRejectReasonCodeType kRiskLevelNotMatchSec_2 = 83;	///<客户风险等级不满足标的最低要求
	static const ATPRejectReasonCodeType kRiskLevelNotMatchSec_3 = 84;	///<客户风险等级与标的的不匹配
	static const ATPRejectReasonCodeType kInvestVarietyNotMatchSec = 85;	///<客户投资品种集合与标的的不匹配
	static const ATPRejectReasonCodeType kDeficitLevelNotMatchSec = 86;	///<客户可承受亏损等级与标的的不匹配
	static const ATPRejectReasonCodeType kJudicialFreezeRecordNotExist = 87;	///<该股东账户不存在对应司法冻结记录
	static const ATPRejectReasonCodeType kJudicialFreezeRecordNotMatch = 88;	///<不存在对应司法冻结记录
	static const ATPRejectReasonCodeType kOutOffTheMaxFreezeQty = 89;	///<超过最大可冻结数量
	static const ATPRejectReasonCodeType kUnknowDeclarationType = 90;	///<未知申报类型
	static const ATPRejectReasonCodeType kTriggerRulesOfContraTrade = 91;	///<触发对敲风控指标规则
	static const ATPRejectReasonCodeType kInvestorUnderlyingSecurityNotInWhiteList = 92;	///<投资者账户标的白名单不存在，无法进行开仓交易
	static const ATPRejectReasonCodeType kAvailableAmtLowerThanLimit = 93;	///<可用保证金低于设置的下限值
	static const ATPRejectReasonCodeType kInvestorAllPositionLimitOut = 94;	///<客户累计持仓超过限额
	static const ATPRejectReasonCodeType kInvestorAllLongPositionLimitOut = 95;	///<客户多头持仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorAllShortPositionLimitOut = 96;	///<客户空头持仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorAllBothPositionLimitOut = 97;	///<客户双边持仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorContractAllPositionLimitOut = 98;	///<客户标的总持仓数量超过限额
	static const ATPRejectReasonCodeType kBasicFieldsError = 99;	///<订单基础字段的格式检查不通过
	static const ATPRejectReasonCodeType kExchangeReject = 100;	///<订单被交易所拒绝，详细的交易所撤单代码见OrdRejReason字段
	static const ATPRejectReasonCodeType kRejSwitchOff = 101;	///<网关开关关闭
	static const ATPRejectReasonCodeType kRejAccountid = 102;	///<无法从路由表中找到股东账户对应的TE分区号
	static const ATPRejectReasonCodeType kRejBusinessType = 103;	///<业务类型错误
	static const ATPRejectReasonCodeType kRejMarketID = 104;	///<市场字段错误
	static const ATPRejectReasonCodeType kRejSide = 105;	///<买卖方向错误
	static const ATPRejectReasonCodeType kRejOrdType = 106;	///<订单类型错误
	static const ATPRejectReasonCodeType kRejTimeInForce = 107;	///<订单有效期错误
	static const ATPRejectReasonCodeType kRejCashMargin = 108;	///<信用标识错误
	static const ATPRejectReasonCodeType kRejPositionEffect = 109;	///<开平仓标识错误
	static const ATPRejectReasonCodeType kRejCoveredOrUncovered = 110;	///<备兑标识错误
	static const ATPRejectReasonCodeType kRejDesignationTransType = 111;	///<转托管注册类型错误
	static const ATPRejectReasonCodeType kRejSecurityid = 112;	///<证券代码错误
	static const ATPRejectReasonCodeType kRejOrderQty = 113;	///<订单申报数量错误
	static const ATPRejectReasonCodeType kRejPrice = 114;	///<价格错误
	static const ATPRejectReasonCodeType kRejWhitList = 115;	///<登录用户的IP地址未在白名单内
	static const ATPRejectReasonCodeType kRejCurrency = 116;	///<币种错误
	static const ATPRejectReasonCodeType kRejTransferWay = 117;	///<划转方式错误
	static const ATPRejectReasonCodeType kQtyTypeError = 118;	///<订单数量类型错误
	static const ATPRejectReasonCodeType kTpaRejGlobalRateLimit = 119;	///<全局流速限制
	static const ATPRejectReasonCodeType kTpaRejIpMacRateLimit = 120;	///<终端流速限制
	static const ATPRejectReasonCodeType kTpaRejCustIdNotInWhiteList = 121;	///<客户号不在白名单
	static const ATPRejectReasonCodeType kTpaRejCustIdInBlackList = 122;	///<客户号在黑名单
	static const ATPRejectReasonCodeType kTpaRejFuncCodeNotFound = 123;	///<功能号权限限制
	static const ATPRejectReasonCodeType kTpaRejMsgType = 124;	///<Rgw不处理委托
	static const ATPRejectReasonCodeType kTpaRejNeedFollowTheSessionProcess = 125;	///<未按照三方接入会话流程进行接入
	static const ATPRejectReasonCodeType kRejAlreadyLogin = 126;	///<该账户已经登录
	static const ATPRejectReasonCodeType kRejNotLoggedin = 127;	///<该账户未登录
	static const ATPRejectReasonCodeType kRejIPAddress = 128;	///<IP被禁止
	static const ATPRejectReasonCodeType kRejT2SDK = 200;	///<恒生API错误
	static const ATPRejectReasonCodeType kRejFundAccountId = 201;	///<恒生T2SDK FundAccountId错误
	static const ATPRejectReasonCodeType kRejCheckCloseOutFlag = 202;	///<了结标识错误
	static const ATPRejectReasonCodeType kRejLegSideErr = 203;	///<持仓类型错误
	static const ATPRejectReasonCodeType kRejSecondaryOrderIdIsErr = 204;	///<组合解除时，组合编码不能为空
	static const ATPRejectReasonCodeType kRejNotExistThePartition = 205;	///<无法在委托路由表中找到账户对应的分区号
	static const ATPRejectReasonCodeType kRejNotExistTxEndpointPartitions = 206;	///<账户无法在该分区的网关报送消息，总线管理服务中未配置网关到引擎的主题分区
	static const ATPRejectReasonCodeType kTransferFunctionHasClosed = 207;	///<转账功能已关闭
	static const ATPRejectReasonCodeType kRejCheckSZCirculationType = 208;	///<深圳流通类型错误
	static const ATPRejectReasonCodeType kCannotFindAccountID = 209;	///<不具备股东账户权限
	static const ATPRejectReasonCodeType kNoMsgTypeNoRight = 210;	///<不具备功能号权限
	static const ATPRejectReasonCodeType kRejStrategyIdIsErr = 211;	///<构建组合时，组合策略编码不能为空
	static const ATPRejectReasonCodeType kTpaRejCustIDLimit = 212;	///<客户号超过了流速上限
	static const ATPRejectReasonCodeType kTpaRejFuncCodeNotAllow = 213;	///<功能号被禁止
	static const ATPRejectReasonCodeType kRejRepayWay = 214;	///<还款方式错误
	static const ATPRejectReasonCodeType kCustLoginMode = 215;	///<客户号登录模式错误
	static const ATPRejectReasonCodeType kCustLoggingin = 216;	///<客户号正在登录
	static const ATPRejectReasonCodeType kCustLoggedin = 217;	///<客户号已经登录
	static const ATPRejectReasonCodeType kCustLoginTimeout = 218;	///<客户号登录超时
	static const ATPRejectReasonCodeType kRejPartitionNo = 219;	///<分区号没找到
	static const ATPRejectReasonCodeType kClientVersionStringError = 220;	///<API版本串校验不通过
	static const ATPRejectReasonCodeType kCustIDIsNotExist = 221;	///<API版本串校验客户号找不到
	static const ATPRejectReasonCodeType kClientVersionStringIsEmpty = 222;	///<API版本串不能为空
	static const ATPRejectReasonCodeType kRiskControlTimout = 223;	///<风控超时
	static const ATPRejectReasonCodeType kRiskControlReject = 224;	///<风控拒绝
	static const ATPRejectReasonCodeType kFlowAccessCustIDAndProductIDSpeedReject = 225;	///<客户号和产品号流速超过限制
	static const ATPRejectReasonCodeType kFlowAccessCustIDAndProductIDSumReject = 226;	///<客户号和产品号流量超过限制
	static const ATPRejectReasonCodeType kFlowAccessUserIDSpeedReject = 227;	///<用户流速超过限制
	static const ATPRejectReasonCodeType kFlowAccessUserIDSumReject = 228;	///<用户流量超过限制
	static const ATPRejectReasonCodeType kFlowAccessAgwSpeedReject = 229;	///<agw流速超过限制
	static const ATPRejectReasonCodeType kFlowAccessAgwSumReject = 230;	///<agw流量超过限制
	static const ATPRejectReasonCodeType kFlowAccessAgwSwitchReject = 231;	///<agw接入开关关闭
	static const ATPRejectReasonCodeType kFlowAccessUserIDSwitchReject = 232;	///<用户接入开关关闭
	static const ATPRejectReasonCodeType kFlowAccessCustIDAndProductIDSwitchReject = 233;	///<客户号和产品号接入开关关闭
	static const ATPRejectReasonCodeType kRiskControlNoConnectionReject = 234;	///<风控无连接拒绝
	static const ATPRejectReasonCodeType kFuncNoBlock = 235;	///<功能号被阻断
	static const ATPRejectReasonCodeType kSystemBusy = 236;	///<当前系统繁忙，请稍后重试
	static const ATPRejectReasonCodeType kGwCustPasswordVerifyFailed = 237;	///<客户密码逐笔验证失败
	static const ATPRejectReasonCodeType kMalfunctionTePartitionReject = 238;	///<系统异常拒单
	static const ATPRejectReasonCodeType kRepeateUserOnlineLimit = 239;	///<网关用户同时在线数超限,登录失败
	static const ATPRejectReasonCodeType kConnectionRetentionFailed = 240;	///<会话恢复失败
	static const ATPRejectReasonCodeType kRejOther = 999;	///<其他错误
	static const ATPRejectReasonCodeType kInvestorContractRightPositionLimitOut = 1001;	///<客户标的权利仓持仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorContractBuyOpenLimitOut = 1002;	///<客户标的当日累计买入开仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorContractSellOpenLimitOut = 1003;	///<客户标的当日累计卖出开仓数量超过限额
	static const ATPRejectReasonCodeType kInvestorContractOpenLimitOut = 1004;	///<客户标的当日累计开仓数量超过限额
	static const ATPRejectReasonCodeType kFundAccountRightDenied = 1005;	///<资金账户限制该业务
	static const ATPRejectReasonCodeType kFundAccountNoOrderWay = 1006;	///<资金账户无对应委托方式
	static const ATPRejectReasonCodeType kCannotFindCancelableOrder = 1007;	///<找不到可撤销的订单
	static const ATPRejectReasonCodeType kOrderAmtRangeOut = 1008;	///<委托需要的资金超过金额上下限
	static const ATPRejectReasonCodeType kBrokerUnsupportedSecurityID = 1009;	///<券商无代理该ETF申赎业务资格
	static const ATPRejectReasonCodeType kCustIDNonExist = 1010;	///<客户号在客户信息表中不存在
	static const ATPRejectReasonCodeType kCustPWDError = 1011;	///<客户密码错误
	static const ATPRejectReasonCodeType kCustIDNotEnable = 1012;	///<客户号未启用
	static const ATPRejectReasonCodeType kQuoteOrderQtyFailureToMeetTheRequirement = 1013;	///<报价单未达到最小数量要求
	static const ATPRejectReasonCodeType kQuoteOrderPriceFailureToMeetTheRequirement = 1014;	///<报价单未达到最大买卖价差要求
	static const ATPRejectReasonCodeType kInvestPeridNotMatchBiz = 1015;	///<客户投资期限不满足业务要求
	static const ATPRejectReasonCodeType kAccountSecurityStatus8NonPrivileged = 1016;	///<客户未开通交易退市整理期证券的权限
	static const ATPRejectReasonCodeType kAccountSecurityStatus9NonPrivileged = 1017;	///<客户未开通买入退市后协议转让证券的权限
	static const ATPRejectReasonCodeType kStandardSecurityUsageRateNoPassed = 1018;	///<出库标准券使用率高于给定值
	static const ATPRejectReasonCodeType kFinancingDebtRatioNoPassed = 1019;	///<债券质押负债率过高
	static const ATPRejectReasonCodeType kRepoAmplMulNoPassed = 1020;	///<债券质押回购放大倍数过高
	static const ATPRejectReasonCodeType kPledgeInNotAllowed = 1021;	///<该证券不允许入库
	static const ATPRejectReasonCodeType kPledgeOutNotAllowed = 1022;	///<该证券不允许出库
	static const ATPRejectReasonCodeType kContractMultiplieIsZero = 1023;	///<该证券的折算率为零
	static const ATPRejectReasonCodeType kPledgeCentrationNoPassed = 1024;	///<入库集中度高于给定值
	static const ATPRejectReasonCodeType kStandardSecurityShareInsufficient = 1025;	///<标准券股份不足
	static const ATPRejectReasonCodeType kPledgeSecurityShareInsufficient = 1026;	///<质押券股份不足
	static const ATPRejectReasonCodeType kPledgeInOrOutCancelNotAllowed = 1027;	///<质押出入库不允许撤单
	static const ATPRejectReasonCodeType kQuoteOrderBidPxOverPriceDeviateRate = 1028;	///<报价单买价超过价格偏离度
	static const ATPRejectReasonCodeType kQuoteOrderOfferPxOverPriceDeviateRate = 1029;	///<报价单卖价超过价格偏离度
	static const ATPRejectReasonCodeType kMarginOccupyOverUpperLimit = 1030;	///<实时保证金占用超过限额
	static const ATPRejectReasonCodeType kQuoteOrderSellPriceLowerThanBuyPrice = 1031;	///<报价单买卖价格倒挂
	static const ATPRejectReasonCodeType kBeyondFinancingAmountLimit = 1032;	///<债券质押回购融资额度高于阈值
	static const ATPRejectReasonCodeType kUnlimitMarketOrderNoPremiumRate = 1033;	///<无涨跌幅限制的证券未配置溢价率
	static const ATPRejectReasonCodeType kNotPermitUnlimitMarketOrder = 1034;	///<不允许无涨跌幅限制的证券交易
	static const ATPRejectReasonCodeType kNotFoundETFComponentSecurity = 1035;	///<未找到etf的成分股基础证券信息
	static const ATPRejectReasonCodeType kBondDebtRatioOverRange = 1036;	///<融资负债率过高
	static const ATPRejectReasonCodeType kFundTransferAgwSeqIdRepeat = 1037;	///<重复的请求
	static const ATPRejectReasonCodeType kFundTransferFundIsMultiple = 1038;	///<资金账号存在多个分区
	static const ATPRejectReasonCodeType kFundTransferFundIsUnKnow = 1039;	///<资金信息不存在
	static const ATPRejectReasonCodeType kFundTransferCurrencyNotSupport = 1040;	///<不支持的货币类型
	static const ATPRejectReasonCodeType kFundTransferAmountMustMoreThanZero = 1041;	///<金额必须大于0
	static const ATPRejectReasonCodeType kFundTransferAmountFormatErr = 1042;	///<金额格式不正确
	static const ATPRejectReasonCodeType kFundTransferAmountNotInValidRange = 1043;	///<调整金额或调整后的金额不在有效金额范围
	static const ATPRejectReasonCodeType kFundTransferAmountInsufficient = 1044;	///<转出资金不足
	static const ATPRejectReasonCodeType kThirdPartyReject = 1045;	///<外部系统拒绝
	static const ATPRejectReasonCodeType kSendThirdPartyFail = 1046;	///<发送外部系统失败
	static const ATPRejectReasonCodeType kFundTransferThirdPartyRollbackFail = 1047;	///<发生冲正且冲正失败
	static const ATPRejectReasonCodeType kFundTransferThirdPartyImplRollback = 1048;	///<发生冲正且冲正成功
	static const ATPRejectReasonCodeType kNotHKThroughSecuritySellNotAllowed = 1049;	///<非港股通证券代码不允许买入
	static const ATPRejectReasonCodeType kZeroSellTradeQtyMoreThanOneHand = 1050;	///<零股买卖委托数量大于一手
	static const ATPRejectReasonCodeType kZeroSellQtyNotEqualAva = 1051;	///<零股卖出失败,委托数量不等于持仓量
	static const ATPRejectReasonCodeType kTibOrderPriceRangOut = 1052;	///<科创板股票市价委托的保护价在价格范围之外
	static const ATPRejectReasonCodeType kPriceLowMustOddType = 1053;	///<申报价格小于0.01,必须以零股类型订单申报
	static const ATPRejectReasonCodeType kZeroShareBuyNotAllowed = 1054;	///<不允许零股买入
	static const ATPRejectReasonCodeType kZeroShareSellNotAllowed = 1055;	///<不允许零股卖出
	static const ATPRejectReasonCodeType kWholeShareBuyNotAllowed = 1056;	///<不允许整手买入
	static const ATPRejectReasonCodeType kWholeShareSellNotAllowed = 1057;	///<不允许整手卖出
	static const ATPRejectReasonCodeType kHKStocksThroughBizInfoNotFound = 1058;	///<港股通业务信息未找到
	static const ATPRejectReasonCodeType kHKStocksThroughExchangeRateNotFound = 1059;	///<港股通汇率配置信息未找到
	static const ATPRejectReasonCodeType kHKStocksThroughNotTradeDay = 1060;	///<非港股通交易日,不允许交易
	static const ATPRejectReasonCodeType kWholeSellOrderQtyStallError = 1061;	///<整手卖出失败,委托数量不是数量单位整数倍
	static const ATPRejectReasonCodeType kZeroBuyTradeQtyMoreThanOneHand = 1062;	///<零股买入失败,委托数量大于等于一手
	static const ATPRejectReasonCodeType kHKVotingNoNonExist = 1063;	///<港股通投票证券代码与其对应的公告编号、投票议案，在港股通信息表中未找到
	static const ATPRejectReasonCodeType kHKVotingDateNotAllowed = 1064;	///<投票单不在港股通投票期内
	static const ATPRejectReasonCodeType kHKVotingAllowNotAllowed = 1065;	///<当前议案不容许投赞成票
	static const ATPRejectReasonCodeType kHKVotingOpposeNotAllowed = 1066;	///<当前议案不容许投反对票
	static const ATPRejectReasonCodeType kHKVotingWaiverNotAllowed = 1067;	///<当前议案不容许投弃权票
	static const ATPRejectReasonCodeType kNotCrdCollateral = 1068;	///<非担保品标的
	static const ATPRejectReasonCodeType kHKCorporateActionIDNotExist = 1069;	///<公司行为代码不存在
	static const ATPRejectReasonCodeType kNotDividendStockOption = 1070;	///<不属于股票选择权红利
	static const ATPRejectReasonCodeType kHKCorporateActionNotInAviliableRange = 1071;	///<公司行为不在申报范围内
	static const ATPRejectReasonCodeType kHKStocksThroughPriceTickNotFound = 1072;	///<未找到对应的价格档位
	static const ATPRejectReasonCodeType kHKVotingQueryFieldError = 1073;	///<港股投票查询的3种投票数量必须为空
	static const ATPRejectReasonCodeType kHKVotingQueryMustBeMember = 1074;	///<港股投票查询必须由券商发起
	static const ATPRejectReasonCodeType kHKEntitlementDTInsufficient = 1075;	///<港股权益转托管数量不足
	static const ATPRejectReasonCodeType kHKDTIDBothInsufficient = 1076;	///<港股转托管委托不能同时选择证券编码和权益编码
	static const ATPRejectReasonCodeType kHKEntitlementNumberNotFound = 1077;	///<港股通转托管权益编码未找到
	static const ATPRejectReasonCodeType kHKCorporateActionQueryMustBeMember = 1078;	///<港股公司行为必须由会员发起
	static const ATPRejectReasonCodeType kHKCorporateActionHistoryCOrderCantNotCancel = 1079;	///<港股公司行为历史撤单委托不能撤单
	static const ATPRejectReasonCodeType kCrdUnderlyingNonExist = 1080;	///<标的清单中证券代码不存在
	static const ATPRejectReasonCodeType kCreditInfoNonExist = 1081;	///<客户信息在信用资料表中不存在
	static const ATPRejectReasonCodeType kSpecialAccountNonExist = 1082;	///<融资融券专户不存在
	static const ATPRejectReasonCodeType kNotCrdBuyUnderlying = 1083;	///<非融资标的
	static const ATPRejectReasonCodeType kCreditLimitNotExist = 1084;	///<信用额度信息不存在
	static const ATPRejectReasonCodeType kCreditLimitNotEnough = 1085;	///<信用额度不足
	static const ATPRejectReasonCodeType kCreditInfoUnNomal = 1086;	///<信用账户合同状态不正常
	static const ATPRejectReasonCodeType kOutOfAvalibalQty = 1087;	///<偿还数量超过最大可委托数量
	static const ATPRejectReasonCodeType kNotCreditAccount = 1088;	///<非信用账户
	static const ATPRejectReasonCodeType kNotSpecialAccount = 1089;	///<非融资融券专户
	static const ATPRejectReasonCodeType kCreditAccountForbid = 1090;	///<信用账户业务被黑名单限制
	static const ATPRejectReasonCodeType kMarginBalanceNotEnough = 1091;	///<客户融资融券保证金可用余额不足
	static const ATPRejectReasonCodeType kProtectiveStrategyCheckFailed = 1092;	///<认沽期权一级投资者买入开仓标的持仓不足
	static const ATPRejectReasonCodeType kRiskForbidTradeRights = 1093;	///<风控限制交易权限
	static const ATPRejectReasonCodeType kNotCrdSellUnderlying = 1094;	///<非融券标的
	static const ATPRejectReasonCodeType kMarginRatioNotExist = 1095;	///<担保率不存在
	static const ATPRejectReasonCodeType kAgwSeqIdDuplicate = 1096;	///<重复的请求
	static const ATPRejectReasonCodeType kOutOfAvalibalAmt = 1097;	///<偿还金额超过最大可偿还金额
	static const ATPRejectReasonCodeType kOrderWayNonPrivileged = 1098;	///<该委托方式禁止此业务
	static const ATPRejectReasonCodeType kOptionStrategyKeyNonExist = 1100;	///<策略编码不存在
	static const ATPRejectReasonCodeType kCombinationDateOut = 1101;	///<构建的日期不在允许范围
	static const ATPRejectReasonCodeType kRemoveCombinationPositionInsufficient = 1102;	///<期权解除策略保证金组合持仓不足
	static const ATPRejectReasonCodeType kCombinationContractPositionInsufficient = 1103;	///<期权组合策略保证金合约持仓不足
	static const ATPRejectReasonCodeType kCombinationContractRuleError = 1104;	///<期权组合策略保证金订单中的合约腿跟交易所策略不匹配
	static const ATPRejectReasonCodeType kOptionMarginFundIDNonExist = 1105;	///<保证金账号不存在
	static const ATPRejectReasonCodeType kCombinationContractUnitDiff = 1106;	///<成分合约单位不相等
	static const ATPRejectReasonCodeType kOrderLegsDiffSecurityRule = 1107;	///<委托带的腿数与规则表中的腿数不相同
	static const ATPRejectReasonCodeType kContractUnderlyingStrategyError = 1108;	///<合约标的不在此策略可使用范围内
	static const ATPRejectReasonCodeType kContractStrategyError = 1109;	///<合约不在此策略可使用范围内
	static const ATPRejectReasonCodeType kContractDiffUnderlyingID = 1110;	///<腿组合合约标的不一致
	static const ATPRejectReasonCodeType kContractSameUnderlyingID = 1111;	///<腿组合合约标的一致
	static const ATPRejectReasonCodeType kContractLastTradeDateDiff = 1112;	///<腿组合合约到期日不完全相同
	static const ATPRejectReasonCodeType kContractLastTradeDateSame = 1113;	///<腿组合合约到期日完全相同
	static const ATPRejectReasonCodeType kCoverContractNotAllow = 1114;	///<备兑合约不能做组合
	static const ATPRejectReasonCodeType kCoveredOpenForbid = 1115;	///<备兑开仓存在缺口不允许开仓
	static const ATPRejectReasonCodeType kCoveredPositionLimitOut = 1116;	///<备兑持仓数不足
	static const ATPRejectReasonCodeType kCoveredLockLimitOut = 1117;	///<锁定数不足
	static const ATPRejectReasonCodeType kCoveredPutForbid = 1118;	///<备兑不支持认沽合约
	static const ATPRejectReasonCodeType kCombinationLegNotEnough = 1119;	///<期权合并行权腿数量错误
	static const ATPRejectReasonCodeType kCombinationLegCallOrPutConsistent = 1120;	///<期权合并行权腿认沽认购类型不能相同
	static const ATPRejectReasonCodeType kComninationPutExPriceMustUpCallExPrice = 1121;	///<期权合并行权认沽行权价格必须高于认购行权价格
	static const ATPRejectReasonCodeType kCombinationModCardinalityNotZero = 1122;	///<期权合并行权委托数量必须为申报数量基数的整数倍
	static const ATPRejectReasonCodeType kCumExerciseQtyThanNetPosition = 1123;	///<权利仓的净持仓量不足以进行合并行权
	static const ATPRejectReasonCodeType kCanNotFindMntGuaranteeRatio = 1124;	///<未查询到维持担保比例设置
	static const ATPRejectReasonCodeType kLowerOfExtractLine = 1125;	///<维持担保比例低于提取线
	static const ATPRejectReasonCodeType kFundTransferCustCentralSysCfgError = 1126;	///<无法获取客户在集中交易的配置信息
	static const ATPRejectReasonCodeType kTurnToDisposalSecurityIDInvalid = 1127;	///<转处置标的证券不是被处置账户应收证券
	static const ATPRejectReasonCodeType kTurnToDisposalOrderQtyOutRange = 1128;	///<转处置申报数量大于被处置账户应收证券数量
	static const ATPRejectReasonCodeType kGageConcentrationRatioExceedLimit = 1130;	///<担保品证券集中度超限
	static const ATPRejectReasonCodeType kOutOfExtraSecurityOut = 1131;	///<余券划转累计数量超过应归还投资者余券数量
	static const ATPRejectReasonCodeType kNoGageInWithZeroGageRatio = 1132;	///<不得转入或买入担保品折算率为零的证券
	static const ATPRejectReasonCodeType kPasswordDecryptFail = 1133;	///<密码解密失败
	static const ATPRejectReasonCodeType kNoExtraSecurityNeedOut = 1134;	///<当前账户指定标的无余券，不需要划转
	static const ATPRejectReasonCodeType kNoCRDTradeParam = 1135;	///<两融通用交易参数不存在
	static const ATPRejectReasonCodeType kAccountIDNotMatchedCustID = 1136;	///<客户号与证券帐户不匹配
	static const ATPRejectReasonCodeType kTurnToDisposalUnderlyingSecurityIDInvalid = 1137;	///<转处置代码非期权标的代码
	static const ATPRejectReasonCodeType kQtyAndFundLowLimited = 1138;	///<委托数量或总额度小于最低限制
	static const ATPRejectReasonCodeType kNotExistThePartition = 1139;	///<报盘交易单元无法在该分区的引擎报单，总线管理服务中未配置引擎到报盘的主题分区
	static const ATPRejectReasonCodeType kTransactionCancelOrderNotAllowed = 1140;	///<大宗交易定价成交申报无法撤单
	static const ATPRejectReasonCodeType kCashUnderlyingPosLimitOut = 1141;	///<现货标的当日累计持仓数量超过限额
	static const ATPRejectReasonCodeType kCashOrderPxOverPriceDeviateRate = 1142;	///<现货标的价格超过价格偏离度
	static const ATPRejectReasonCodeType kTurnToDisposalReturnOrderQtyOutRange = 1143;	///<转处置返还数量大于剩余转处置数量
	static const ATPRejectReasonCodeType kInvestorContractDutyPositionLimitOut = 1144;	///<客户标的义务仓持仓数量超过限额
	static const ATPRejectReasonCodeType kReduceShareOverRange = 1145;	///<超过可减持的额度
	static const ATPRejectReasonCodeType kContractStatusNonPrivileged = 1146;	///<合同状态异常禁止此业务
	static const ATPRejectReasonCodeType kLegSideError = 1148;	///<组合策略构成头寸方向不符合要求
	static const ATPRejectReasonCodeType kExercisrPriceError = 1149;	///<组合策略构成合约行权价格不符合要求
	static const ATPRejectReasonCodeType kExercisrDateError = 1150;	///<组合策略构成合约到期日不符合要求
	static const ATPRejectReasonCodeType kLegCallOrPutError = 1151;	///<组合策略构成合约认沽认购类型不符合要求
	static const ATPRejectReasonCodeType kLegCoveredOrUncoveredError = 1152;	///<组合策略构成合约备兑标签不符合要求
	static const ATPRejectReasonCodeType kCombinationStrategyIsNotStandard = 1153;	///<组合策略合约为非标合约
	static const ATPRejectReasonCodeType kCumReduceShareOverRange = 1154;	///<累计减持额度超过限制
	static const ATPRejectReasonCodeType kCreNormalAccMapNotFound = 1155;	///<信用与普通账户对应关系不存在
	static const ATPRejectReasonCodeType kLegSideOrderError = 1156;	///<组合策略成分合约顺序不符合规则
	static const ATPRejectReasonCodeType kNotShareLiability = 1157;	///<不存在股份负债
	static const ATPRejectReasonCodeType kSpecialAccountNotAllowTheBusiness = 1158;	///<两融专户禁止该交易业务
	static const ATPRejectReasonCodeType kFinanSpecialAccountNotAllowTheBusiness = 1159;	///<融资专户禁止该交易业务
	static const ATPRejectReasonCodeType kRemoveCombinationPositionNotExitOrNotRight = 1160;	///<组合持仓不存在或组合持仓与合约账户不匹配
	static const ATPRejectReasonCodeType kRemoveCombinationPositionNotExit = 1161;	///<该组合持仓不存在
	static const ATPRejectReasonCodeType kDutyPositionLimitOut = 1162;	///<普通义务仓数量不足
	static const ATPRejectReasonCodeType kCombinationIDError = 1163;	///<组合策略编码不存在
	static const ATPRejectReasonCodeType kNotFoundBuyPrivateAmount = 1164;	///<融资保底额度信息不存在
	static const ATPRejectReasonCodeType kNotFoundSellPrivateAmount = 1165;	///<融券保底额度信息不存在
	static const ATPRejectReasonCodeType kMarginSpecialFundNotEnough = 1166;	///<融资专户资金余额不足
	static const ATPRejectReasonCodeType kMarginSpecialShareNotEnough = 1167;	///<融券专户股份余额不足
	static const ATPRejectReasonCodeType kLegPutNotAllowCoverOpen = 1168;	///<认沽合约不能做备兑开仓
	static const ATPRejectReasonCodeType kNotHaveRestrictShare = 1169;	///<卖出股份不含受限股
	static const ATPRejectReasonCodeType kReduceHoldingQuoteSwithchOff = 1170;	///<大宗减持业务开关被关闭
	static const ATPRejectReasonCodeType kNotExtraFoundSellPrivateAmount = 1171;	///<融券保底额度额外信息不存在
	static const ATPRejectReasonCodeType kCustFeeConfigError = 1173;	///<未获取到汇率信息
	static const ATPRejectReasonCodeType kFundNotSupportTheCurrency = 1174;	///<资金账号不支持该币种
	static const ATPRejectReasonCodeType kCouponSwapOrderQtyOutRange = 1175;	///<垫券还券业务垫券申报数量超过投资人行权净应付数量
	static const ATPRejectReasonCodeType kCouponSwapOrderQtyOutRangeCashAvailable = 1176;	///<垫券还券业务还券申报数量超过投资人现货证券账户可用数量
	static const ATPRejectReasonCodeType kCouponSwapSecurityIDInvalid = 1177;	///<投资人没有该证券行权净应付证券
	static const ATPRejectReasonCodeType kDeductionOrderQtyOutRange = 1178;	///<待清偿申报数量大于行权标的证券净应收数量
	static const ATPRejectReasonCodeType kDeductionAccountTypeError = 1179;	///<待清偿申报账户类型错误
	static const ATPRejectReasonCodeType kDeductionAccountNoPaidPos = 1180;	///<该投资人没有该证券行权净应收证券
	static const ATPRejectReasonCodeType kCloseStatusNonPrivilegedCreditTrade = 1181;	///<平仓状态禁止此业务
	static const ATPRejectReasonCodeType kLessThanMinSellQtyMustSellAll = 1182;	///<可用数量小于最低卖出数量必须全部卖出
	static const ATPRejectReasonCodeType kSellZeroNotEqualAvailableZero = 1183;	///<卖出零股数量与可用数量的零股数量不相同
	static const ATPRejectReasonCodeType kCancelTimeOverlimit = 1184;	///<撤单次数超限
	static const ATPRejectReasonCodeType kFundFeeInfoFail = 1185;	///<客户费用信息不存在
	static const ATPRejectReasonCodeType kGetSecurityVarietyFail = 1186;	///<证券类型对应的证券品种不存在
	static const ATPRejectReasonCodeType kSellFundLessThanFeeProhibitd = 1187;	///<卖出所得资金小于费用，不允许交易
	static const ATPRejectReasonCodeType kSellFundLessThanFeeAlarm = 1188;	///<卖出资金小于费用，请谨慎操作（仅提示作用，委托正常处理）
	static const ATPRejectReasonCodeType kTurnToDisposalAccountInvalid = 1189;	///<转处置账户或交易单元校验失败
	static const ATPRejectReasonCodeType kCreditAccountNotAllowTheBusiness = 1190;	///<信用账户禁止该交易业务
	static const ATPRejectReasonCodeType kShanghaiTransUncoveredUnsupported = 1195;	///<上海不支持备兑转普通
	static const ATPRejectReasonCodeType kCoverOpenOnlySupportSell = 1196;	///<备兑开仓只支持卖方向
	static const ATPRejectReasonCodeType kCoverCloseOnlySupportBuy = 1197;	///<备兑平仓只支持买方向
	static const ATPRejectReasonCodeType kThanSplitUpLimit = 1198;	///<超过系统最大可拆单数
	static const ATPRejectReasonCodeType kSplitOrderQtyError = 1199;	///<拆单数量参数错误
	static const ATPRejectReasonCodeType kBatchTypeError = 1200;	///<批量类型错误
	static const ATPRejectReasonCodeType kUnknownThirdSystemType = 1201;	///<融资融券未知三方系统类型
	static const ATPRejectReasonCodeType kRepoUnexpiredInfoNotUpdate = 1202;	///<客户未到期融资负债尚未更新
	static const ATPRejectReasonCodeType kUnableToCancellMemberOrder = 1203;	///<券商发起委托, 投资人不能撤单
	static const ATPRejectReasonCodeType kEncryptFail = 1204;	///<客户交易密码加密失败
	static const ATPRejectReasonCodeType kDecryptFail = 1205;	///<客户交易密码解密失败
	static const ATPRejectReasonCodeType kStrengthCheckFail = 1206;	///<客户交易密码强度校验失败
	static const ATPRejectReasonCodeType kNoClosePriceAndNoPriceLimit = 1208;	///<未收到收盘价且证券价格无涨跌幅，无法冻结资金
	static const ATPRejectReasonCodeType kLessDirectionalConvertibleBondsMin = 1209;	///<小于定向可转债协议转让数量门槛（张）
	static const ATPRejectReasonCodeType kRedeliverTypeDeny = 1210;	///<当前不支持的直接还券类型
	static const ATPRejectReasonCodeType kAccountSecurityStatus12NonPrivileged = 1211;	///<客户未开通买入退市整理期首日证券的权限
	static const ATPRejectReasonCodeType kPeakOverRiskExposure = 1212;	///<融券卖出规模超过限额
	static const ATPRejectReasonCodeType kRootNumNotExist = 1213;	///<交易节点不存在
	static const ATPRejectReasonCodeType kCombinationBuyCloseContractInvaild = 1230;	///<组合单边平仓合约不是组合持仓中的义务仓
	static const ATPRejectReasonCodeType kCombinationPositionInsufficient = 1231;	///<组合持仓数量不足
	static const ATPRejectReasonCodeType kMsgBusinessForbid = 1232;	///<消息业务被禁用
	static const ATPRejectReasonCodeType kLowerThanSpecExtractLine = 1233;	///<不满足特殊维保比例检查项
	static const ATPRejectReasonCodeType kBranchIDIsNull = 1234;	///<营业部为空
	static const ATPRejectReasonCodeType kTradeReportTypeError = 1235;	///<成交申报类型错误
	static const ATPRejectReasonCodeType kOrderCanceled = 1236;	///<订单已撤销
	static const ATPRejectReasonCodeType kNoFindForwardInfo = 1237;	///<没有对应的转发信息
	static const ATPRejectReasonCodeType kOrderProcessed = 1238;	///<该笔订单已经处理
	static const ATPRejectReasonCodeType kBondConsultTradeCounterpartyNoCanceled = 1239;	///<债券协商成交对手方不支持撤单
	static const ATPRejectReasonCodeType kNoFindOurPartyTradeInfo = 1240;	///<没有找到对应本方交易信息表信息
	static const ATPRejectReasonCodeType kBusinessTypeToCodeNotExist = 1241;	///<适当性业务检查参数配置不存在
	static const ATPRejectReasonCodeType kInvestorTypeError = 1242;	///<交易主体类型错误
	static const ATPRejectReasonCodeType kAccountNoReductionQuoteInfo = 1243;	///<账户无减持额度信息
	static const ATPRejectReasonCodeType kMultiRedeliverType = 1244;	///<已配置多种还券类型，请选择还券类型后再次提交委托
	static const ATPRejectReasonCodeType kExtShareTransferFail = 1245;	///<外部系统股份划拨失败
	static const ATPRejectReasonCodeType kDesignateContractInvalid = 1246;	///<无效指定偿还合约
	static const ATPRejectReasonCodeType kDesignateContractRedeliverTypeDeny = 1247;	///<当前还券类型不支持偿还指定合约
	static const ATPRejectReasonCodeType kTransferResultQtyOverRange = 1248;	///<划拨后的数量超过上下限
	static const ATPRejectReasonCodeType kNewBondNoAllowOrder = 1249;	///<此业务新指定账户不允许交易
	static const ATPRejectReasonCodeType kNotAllowedInNightOrderWhiteList = 1250;	///<该业务类别或证券类别不在夜市委托业务白名单中
	static const ATPRejectReasonCodeType kNightStatusNotAllowMarketOrder = 1251;	///<夜市委托不允许市价委托
	static const ATPRejectReasonCodeType kNightStatusNotAllowReduceLimitedShare = 1252;	///<夜市委托不允许减持受限股
	static const ATPRejectReasonCodeType kShanghaiCombinationOneLegClosedUnsupported = 1253;	///<上海市场不支持组合单边平仓
	static const ATPRejectReasonCodeType kDesignateContractRepayControlNotSet = 1254;	///<指定合约偿还未配置指定合约偿还参数
	static const ATPRejectReasonCodeType kBankSecuritiesTransferStreamQueryFailed = 1255;	///<银证转账流水查询失败
	static const ATPRejectReasonCodeType kBankSecuritiesTransferSuccessAndFundTransferInFailed = 1256;	///<银证转账成功,资金划入失败
	static const ATPRejectReasonCodeType kFundTransferOutSuccessAndBankSecuritiesTransferFailed = 1257;	///<资金划出成功,银证转账失败
	static const ATPRejectReasonCodeType kBankSecuritiesTransferOutFailed = 1258;	///<银证转入失败

};
/**
* @brief 回报同步结束消息结果错误码
*/
struct TRADE_API ATPReasonCodeConst
{
	static const ATPReasonCodeType kSuccess = 0;	///<正常返回
	static const ATPReasonCodeType kIndexError = 1;	///<索引错误
	static const ATPReasonCodeType kSessionNotReady = 2;	///<会话未就绪

};
/**
* @brief 交易所拒绝原因错误码
*/
struct TRADE_API ATPOrdRejReasonConst
{
	static const ATPOrdRejReasonType kBizError;	///<业务错误
	static const ATPOrdRejReasonType kMarketClosed;	///<市场休市
	static const ATPOrdRejReasonType kFieldError;	///<非法消息
	static const ATPOrdRejReasonType kConnectionError;	///<无法申报
	static const ATPOrdRejReasonType kOfferwayError;	///<报盘通道不存在
	static const ATPOrdRejReasonType kSwitchReject;	///<报盘开关已关闭
	static const ATPOrdRejReasonType kCosBizError;	///<业务错误
	static const ATPOrdRejReasonType kCosOfferwayError;	///<报盘通道不存在
	static const ATPOrdRejReasonType kCosSwitchReject;	///<报盘开关已关闭

};
/**
* @brief 错误码类型枚举
*/
struct TRADE_API ATPErrorCodeTypeConst
{
	static const ATPErrorCodeTypeType kOrdRejReason = 1;	///<交易所拒绝原因错误码类型
	static const ATPErrorCodeTypeType kPermissonErrorCode = 2;	///<客户登录错误码类型
	static const ATPErrorCodeTypeType kQueryResultCode = 3;	///<查询结果错误码类型
	static const ATPErrorCodeTypeType kReasonCode = 4;	///<回报同步结束消息结果错误码类型
	static const ATPErrorCodeTypeType kRejectReasonCode = 5;	///<拒绝原因错误码类型
	static const ATPErrorCodeTypeType kErrorCode = 6;	///<API错误码类型
	static const ATPErrorCodeTypeType kExternalInsErrorCode = 7;	///<外部指令拒绝原因错误码类型
	static bool IsValid(const ATPErrorCodeTypeType& value);

};
/**
* @brief 直接还券类型
*/
struct TRADE_API ATPRedeliverTypeConst
{
	static const ATPRedeliverType Standard = 0;	///<标准还券
	static const ATPRedeliverType Will = 1;	///<意向还券
	static const ATPRedeliverType Realtime = 2;	///<即时还券
	static const ATPRedeliverType Default = 255;	///<默认值
	static bool IsValid(const ATPRedeliverType& value);

};
/**
* @brief 母单状态枚举
*/
struct TRADE_API ATPAlgoStatusConst
{
	static const ATPAlgoStatusType New = 0;	///<已确认
	static const ATPAlgoStatusType NewPending = 1;	///<确认中
	static const ATPAlgoStatusType NewRejected = 2;	///<已拒绝
	static const ATPAlgoStatusType PartiallyFilled = 3;	///<部分成交
	static const ATPAlgoStatusType Filled = 4;	///<全部成交
	static const ATPAlgoStatusType Cancelled = 5;	///<已撤销
	static const ATPAlgoStatusType CancelPending = 6;	///<撤销中
	static const ATPAlgoStatusType CancelReject = 7;	///<撤销拒绝
	static const ATPAlgoStatusType Expired = 8;	///<已过期
	static const ATPAlgoStatusType ExpirePending = 9;	///<过期中
	static const ATPAlgoStatusType Paused = 10;	///<已暂停
	static const ATPAlgoStatusType OperateReject = 11;	///<操作拒绝
	static bool IsValid(const ATPAlgoStatusType& value);

};
/**
* @brief 证券类型枚举
*/
struct TRADE_API ATPSecurityTypeConst
{
	static const ATPSecurityTypeType Default = 0;	///<默认类型
	static const ATPSecurityTypeType MainStockA = 1;	///<主板A股
	static const ATPSecurityTypeType SmallStock = 2;	///<中小板股票
	static const ATPSecurityTypeType GrowthStock = 3;	///<核准制创业板股票
	static const ATPSecurityTypeType MainStockB = 4;	///<主板B股
	static const ATPSecurityTypeType NationalBond = 5;	///<国债（含地方债）
	static const ATPSecurityTypeType CorporateBond = 6;	///<企业债
	static const ATPSecurityTypeType CompanyBond = 7;	///<公司债
	static const ATPSecurityTypeType ConvertibleBond = 8;	///<可转债
	static const ATPSecurityTypeType PrivateBond = 9;	///<私募债
	static const ATPSecurityTypeType ConvertiblePrivateBond = 10;	///<可交换私募债
	static const ATPSecurityTypeType SecurityCompanySubBond = 11;	///<证券公司次级债
	static const ATPSecurityTypeType PledgeRepo = 12;	///<质押式回购
	static const ATPSecurityTypeType ABS = 13;	///<资产支持证券
	static const ATPSecurityTypeType StockETF = 14;	///<股票ETF
	static const ATPSecurityTypeType CMTStockETF = 15;	///<跨市场股票ETF
	static const ATPSecurityTypeType CMTETF = 16;	///<跨境ETF
	static const ATPSecurityTypeType PhysicalBondETF = 17;	///<实物债券ETF
	static const ATPSecurityTypeType CashBondETF = 18;	///<现金债券ETF
	static const ATPSecurityTypeType GoldETF = 19;	///<黄金ETF
	static const ATPSecurityTypeType CurrencyETF = 20;	///<货币ETF
	static const ATPSecurityTypeType LeverETF = 21;	///<杠杆ETF
	static const ATPSecurityTypeType CommodityFuturesETF = 22;	///<商品期货ETF
	static const ATPSecurityTypeType StandardLOF = 23;	///<标准LOF
	static const ATPSecurityTypeType GradSubFund = 24;	///<分级子基金
	static const ATPSecurityTypeType CloseFund = 25;	///<封闭式基金
	static const ATPSecurityTypeType OnlyRedemptionFund = 26;	///<仅申赎基金
	static const ATPSecurityTypeType RightsIssueWarrant = 27;	///<配售权证
	static const ATPSecurityTypeType Warrant = 28;	///<权证
	static const ATPSecurityTypeType StockOption = 29;	///<个股期权
	static const ATPSecurityTypeType ETFOption = 30;	///<ETF期权
	static const ATPSecurityTypeType FundManageProduct = 31;	///<资管产品
	static const ATPSecurityTypeType OfferRepo = 32;	///<报价回购
	static const ATPSecurityTypeType PreferredStock = 33;	///<优先股
	static const ATPSecurityTypeType SecurityCompanyShortBond = 34;	///<证券公司短期债
	static const ATPSecurityTypeType ConvertibleCompanyBond = 35;	///<可交换公司债
	static const ATPSecurityTypeType BondSell = 36;	///<债券分销
	static const ATPSecurityTypeType NationalBondSell = 37;	///<国债分销
	static const ATPSecurityTypeType OtherBond = 38;	///<其他债券
	static const ATPSecurityTypeType CMFund = 39;	///<跨市场/跨境资金
	static const ATPSecurityTypeType OtherFund = 40;	///<其他基金
	static const ATPSecurityTypeType InternationalStock = 41;	///<国际版股票
	static const ATPSecurityTypeType OtherStock = 42;	///<其他股票
	static const ATPSecurityTypeType BuyoutBondRepo = 43;	///<买断式债券回购
	static const ATPSecurityTypeType GrowthConvertibleBond = 44;	///<创业板可转债
	static const ATPSecurityTypeType DepositoryReceipt = 45;	///<存托凭证
	static const ATPSecurityTypeType SHLondonCDR = 46;	///<沪伦通CDR
	static const ATPSecurityTypeType InnovativeEnterpriseStock = 47;	///<创新企业股票
	static const ATPSecurityTypeType PledgeCode = 48;	///<质押代码
	static const ATPSecurityTypeType RealTimeCurrencyFund = 49;	///<实时申赎货币基金
	static const ATPSecurityTypeType StockBondIssue = 50;	///<新股新债发行
	static const ATPSecurityTypeType TIBStock = 51;	///<科创板Science and technology innovation board
	static const ATPSecurityTypeType HKStock = 52;	///<港股
	static const ATPSecurityTypeType TIBCDR = 53;	///<科创板CDR
	static const ATPSecurityTypeType OBSTOCK = 54;	///<期权股票
	static const ATPSecurityTypeType OBETF = 55;	///<期权ETF
	static const ATPSecurityTypeType RegistryGrowthStock = 56;	///<注册制创业板股票
	static const ATPSecurityTypeType RegistryInnovativeEnterpriseStock = 57;	///<注册制创新企业股票
	static const ATPSecurityTypeType RegistryDepositoryReceipt = 58;	///<注册制存托凭证
	static const ATPSecurityTypeType ChinextInnovativeEnterpriseStock = 59;	///<创业板创新企业股票
	static const ATPSecurityTypeType RegistryChinextInnovativeEnterpriseStock = 60;	///<注册制创业板创新企业股票
	static const ATPSecurityTypeType ChinextDepositoryReceipt = 61;	///<创业板存托凭证
	static const ATPSecurityTypeType RegistryChinextDepositoryReceipt = 62;	///<注册制创业板存托凭证
	static const ATPSecurityTypeType TIBETF = 63;	///<科创板ETF
	static const ATPSecurityTypeType TIBLOF = 64;	///<科创板相关LOF
	static const ATPSecurityTypeType InfrastructureFund = 65;	///<基础设施基金
	static const ATPSecurityTypeType DirectionalConvertibleBonds = 66;	///<定向可转债
	static const ATPSecurityTypeType TibConvertibleBond = 67;	///<科创板可转债
	static bool IsValid(const ATPSecurityTypeType& value);

};
/**
* @brief 证券状态枚举bit位枚举
*/
struct TRADE_API ATPSecurityStatusConst
{
	static const ATPSecurityStatusType Normal = 0;	///<正常
	static const ATPSecurityStatusType Suspended = 1;	///<停牌
	static const ATPSecurityStatusType Closer = 2;	///<临近到期日
	static const ATPSecurityStatusType Last5Adjusted = 3;	///<最近5个交易日内发生调整
	static const ATPSecurityStatusType ExRights = 4;	///<除权
	static const ATPSecurityStatusType ExDividend = 5;	///<除息
	static const ATPSecurityStatusType RiskWarning = 6;	///<风险警示（ST或*ST）
	static const ATPSecurityStatusType ListOnTheFirstDay = 7;	///<上市首日
	static const ATPSecurityStatusType DelisingPeriod = 8;	///<退市整理期
	static const ATPSecurityStatusType TransferAfterDelisting = 9;	///<退市后协议转让
	static const ATPSecurityStatusType DoubleToSingleAdjustment = 10;	///<实施双转单调整
	static const ATPSecurityStatusType ListingEarly = 11;	///<上市初期
	static const ATPSecurityStatusType FirstDayDelisingPeriod = 12;	///<退市整理期首日
	static bool IsValid(const ATPSecurityStatusType& value);

};
/**
* @brief 股份性质枚举
*/
struct TRADE_API ATPShareNegotiabilityConst
{
	static const ATPShareNegotiabilityType CirculationStock;	///<流通股
	static const ATPShareNegotiabilityType RestrictedStock;	///<首发后限售股
	static bool IsValid(const ATPShareNegotiabilityType& value);

};
/**
* @brief ETF成交回报类型枚举
*/
struct TRADE_API ATPETFTradeReportTypeConst
{
	static const ATPETFTradeReportTypeType ConfirmReport = 0;	///<订单确认
	static const ATPETFTradeReportTypeType SSETrading = 1;	///<上交所竞价撮合平台
	static const ATPETFTradeReportTypeType SSEATP = 2;	///<上交所综合业务平台
	static const ATPETFTradeReportTypeType SZSENonTrading = 3;	///<深交所非交易平台
	static const ATPETFTradeReportTypeType CSDAOpenEndFunds = 4;	///<中登开放式基金平台
	static bool IsValid(const ATPETFTradeReportTypeType& value);

};
/**
* @brief 认沽认购枚举
*/
struct TRADE_API ATPCallOrPutConst
{
	static const ATPCallOrPutType kNULL = ' ';	///<空
	static const ATPCallOrPutType kCall = 'C';	///<认购
	static const ATPCallOrPutType kPut = 'P';	///<认沽
	static bool IsValid(const ATPCallOrPutType& value);

};
/**
* @brief 融资融券合约信用标识枚举
*/
struct TRADE_API ATPCreditIdentificationConst
{
	static const ATPCreditIdentificationType kCrdBuyContract = 1;	///<融资合约
	static const ATPCreditIdentificationType kCrdSellContract = 2;	///<融券合约
	static const ATPCreditIdentificationType kCrdBuyInterest = 3;	///<融资利息
	static const ATPCreditIdentificationType kCrdSellFee = 4;	///<融券费用
	static const ATPCreditIdentificationType kCapitalRightsCompensation = 5;	///<资金权益补偿
	static const ATPCreditIdentificationType kShareRightsCompensation = 6;	///<股份权益补偿
	static const ATPCreditIdentificationType kOverdueInterest = 7;	///<逾期罚息
	static const ATPCreditIdentificationType kBadDebtInterest = 8;	///<坏账罚息
	static const ATPCreditIdentificationType kCapitalOccupationFee = 9;	///<资金占用费
	static const ATPCreditIdentificationType kManagementFee = 10;	///<管理费
	static bool IsValid(const ATPCreditIdentificationType& value);

};
/**
* @brief 是否输出融券专户可用数量枚举
*/
struct TRADE_API ATPIsQueryMarginQtyConst
{
	static const ATPIsQueryMarginQtyType UnOutPut = 0;	///<不输出：记录都输出且融券专户可用数量默认0
	static const ATPIsQueryMarginQtyType OutPut = 1;	///<输出：记录都输出且融券专户可用数量输出实际计算值
	static const ATPIsQueryMarginQtyType OutPutSpecial = 2;	///<输出：可用数量大于0的记录都输出，可用数量为0的记录（当查询指定证券代码时输出记录，不指定证券代码时不输出）
	static bool IsValid(const ATPIsQueryMarginQtyType& value);

};
/**
* @brief 订单标识枚举
*/
struct TRADE_API ATPOrderFlagConst
{
	static const ATPOrderFlagType kDefault = ' ';	///<默认值
	static const ATPOrderFlagType kNormal = '0';	///<普通订单
	static const ATPOrderFlagType kParent = '1';	///<批量母单
	static const ATPOrderFlagType kChildren = '2';	///<批量子单
	static bool IsValid(const ATPOrderFlagType& value);

};
/**
* @brief 实时划转标志枚举
*/
struct TRADE_API ATPImmediateEffectConst
{
	static const ATPImmediateEffectType Standard = 0;	///<非实时
	static const ATPImmediateEffectType Realtime = 1;	///<实时
	static bool IsValid(const ATPImmediateEffectType& value);

};
/**
* @brief 发行方式枚举
*/
struct TRADE_API ATPIssueTypeConst
{
	static const ATPIssueTypeType Seo = 1;	///<增发发行
	static const ATPIssueTypeType Ipo = 2;	///<首次发行
	static const ATPIssueTypeType Fund = 3;	///<基金发行
	static bool IsValid(const ATPIssueTypeType& value);

};
/**
* @brief 是否先用保底额度枚举
*/
struct TRADE_API ATPFirstUsePrivateAmountConst
{
	static const ATPFirstUsePrivateAmountType No = 0;	///<否
	static const ATPFirstUsePrivateAmountType Yes = 1;	///<是
	static const ATPFirstUsePrivateAmountType Null = 2;	///<未设置
	static bool IsValid(const ATPFirstUsePrivateAmountType& value);

};
/**
* @brief 成交申报模式枚举
*/
struct TRADE_API ATPTradeHandlingInstrConst
{
	static const ATPTradeHandlingInstrType TransReport = 1;	///<成交报告
	static const ATPTradeHandlingInstrType ProtocolMatch = 2;	///<协议配对
	static const ATPTradeHandlingInstrType TransRequest = 3;	///<成交请求
	static bool IsValid(const ATPTradeHandlingInstrType& value);

};
/**
* @brief 结算方式枚举
*/
struct TRADE_API ATPSettlTypeConst
{
	static const ATPSettlTypeType MultLateralNetting = 103;	///<多边净额
	static const ATPSettlTypeType OneByOneFullAmount = 104;	///<逐笔全额
	static bool IsValid(const ATPSettlTypeType& value);

};
/**
* @brief 交易主体类型枚举
*/
struct TRADE_API ATPInvestorTypeConst
{
	static const ATPInvestorTypeType Self;	///<自营
	static const ATPInvestorTypeType Asset;	///<资管
	static const ATPInvestorTypeType Institutional;	///<机构经纪
	static const ATPInvestorTypeType Personal;	///<个人经纪
	static bool IsValid(const ATPInvestorTypeType& value);

};
/**
* @brief 成交申报事务类别枚举
*/
struct TRADE_API ATPTransAffairTypeConst
{
	static const ATPTransAffairTypeType New = 0;	///<普通申报
	static const ATPTransAffairTypeType Cancel = 1;	///<主动撤单
	static const ATPTransAffairTypeType Replace = 2;	///<接收申报
	static bool IsValid(const ATPTransAffairTypeType& value);

};
/**
* @brief 成交申报类型枚举
*/
struct TRADE_API ATPTradeReportTypeConst
{
	static const ATPTradeReportTypeType SubmitTransaction = 0;	///<提交成交申报
	static const ATPTradeReportTypeType AcceptTransaction = 2;	///<接受成交申报
	static const ATPTradeReportTypeType RefuseTransaction = 3;	///<拒绝成交申报
	static bool IsValid(const ATPTradeReportTypeType& value);

};
/**
* @brief 算法母单订单类型枚举
*/
struct TRADE_API ATPAlgoOrdTypeConst
{
	static const ATPAlgoOrdTypeType kDefault = ' ';	///<默认值
	static const ATPAlgoOrdTypeType kMarket = '1';	///<母单市价
	static const ATPAlgoOrdTypeType kFixed = '2';	///<母单限价
	static bool IsValid(const ATPAlgoOrdTypeType& value);

};
/**
* @brief 算法母单操作枚举
*/
struct TRADE_API ATPAlgoOperationConst
{
	static const ATPAlgoOperationType Resume = 1;	///<恢复母单
	static const ATPAlgoOperationType Pause = 2;	///<暂停母单
	static const ATPAlgoOperationType PauseAndCancel = 3;	///<暂停母单并撤销子单
	static bool IsValid(const ATPAlgoOperationType& value);

};
/**
* @brief 持仓划拨方式枚举
*/
struct TRADE_API ATPTransferTypeConst
{
	static const ATPTransferTypeType kDefault = 0xFF;	///<默认值
	static const ATPTransferTypeType kIn = 1;	///<划入
	static const ATPTransferTypeType kOut = 2;	///<划出
	static bool IsValid(const ATPTransferTypeType& value);

};
/**
* @brief 消息订阅枚举
*/
struct TRADE_API ATPMultiChannelFlagTypeConst
{
	static const ATPMultiChannelFlagType kDefault = 255;	///<默认值
	static const ATPMultiChannelFlagType kSubscribe = 1;	///<订阅
	static const ATPMultiChannelFlagType kNoSubscribe = 0;	///<不订阅
	static bool IsValid(const ATPMultiChannelFlagType& value);

};
/**
* @brief 订阅结果枚举
*/
struct TRADE_API ATPMultiChannelResultTypeConst
{
	static const ATPMultiChannelResultType kSubscribed = 1;	///<已订阅
	static const ATPMultiChannelResultType kNoSubscribed = 2;	///<未订阅
	static const ATPMultiChannelResultType kOverSubscriptions = 3;	///<未订阅，超订阅连接数上限
	static const ATPMultiChannelResultType kUnknow = 4;	///<未知，后台版本过低
	static bool IsValid(const ATPMultiChannelResultType& value);

};
/**
* @brief 业务状态枚举
*/
struct TRADE_API ATPBusinessStatusConst
{
	static const ATPBusinessStatusType kDefault = 255;	///<默认值
	static const ATPBusinessStatusType kSubmitted = 0;	///<已提交
	static const ATPBusinessStatusType kWaitCheck = 1;	///<待审核
	static const ATPBusinessStatusType kCheckRejected = 2;	///<审核驳回
	static const ATPBusinessStatusType kCheckSuccess = 3;	///<办理成功
	static const ATPBusinessStatusType kCheckFail = 4;	///<办理失败
	static const ATPBusinessStatusType kCheckAbnormal = 5;	///<处理异常
	static bool IsValid(const ATPBusinessStatusType& value);

};
/**
* @brief 是否返回ETF实时申赎成交枚举
*/
struct TRADE_API ATPEtfPurchaseRedemptionFlagConst
{
	static const ATPEtfPurchaseRedemptionFlagType kNoReturn = 0;	///<不返回
	static const ATPEtfPurchaseRedemptionFlagType kReturn = 1;	///<返回
	static bool IsValid(const ATPEtfPurchaseRedemptionFlagType& value);

};


#endif