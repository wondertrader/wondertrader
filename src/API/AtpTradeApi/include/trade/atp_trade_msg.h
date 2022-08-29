// code_generator_tag
// messages:545e8f3461ca9459a3a3c925e674a6b7 template:d5e676b48a16d8942e0c6cea656a6229 code:e7eef0b3e567570c5abb0173c23b98d7
#ifndef ATP_TRADE_MSG_H
#define ATP_TRADE_MSG_H
#include "atp_trade_types.h"
#include "atp_trade_constants.h"

/**
*	@brief 终端信息
*/
struct TRADE_API TerminalFeatureCode
{
	TerminalFeatureCode();
	std::string station_name; ///<站点信息
	std::string terminal_type; ///<终端类型
	std::string i_ip; ///<公网IP地址
	std::string i_port; ///<公网端口
	std::string p_cn; ///<PC终端设备名
	std::string s_no; ///<PC终端设备序列号
	std::string hd; ///<硬盘序列号
	std::string cpu_n; ///<CPU序列号
	std::string pi; ///<硬盘分区信息
	std::string vol; ///<系统盘卷标号
	std::string terminal_name; ///<交易终端软件
	std::string imei; ///<IMEI
	std::string idfv; ///<供应商标识符
	std::string iccid; ///<集成电路卡识别码
	std::string imsi; ///<国际移动用户识别码
	std::string rmpn; ///<注册手机号码
	std::string umpn; ///<实际使用手机号码
	std::string osv; ///<操作系统版本
	std::string tel; ///<电话号码
	std::string bsn; ///<银行流水号
	std::string expand_info; ///<扩展信息
	
};

/**
*	@brief 连接设置参数
*/
struct TRADE_API ATPConnectProperty
{
	std::string					user;						///< 登录AGW的账户
	std::string					password;					///< 登录AGW的密码
	std::vector<std::string>	locations;					///< AGW IP地址 格式为："ip:port"
	int32_t						heartbeat_interval_milli;	///< 发送心跳的时间间隔,单位：毫秒，默认值：5000
	int32_t						connect_timeout_milli;      ///< 连接超时时间，单位：毫秒，默认值：5000 
	int32_t						reconnect_time;             ///< 重试连接次数，默认值：连接地址数
	std::map<int32_t, int32_t>	report_sync;			    ///< 回报同步数据map<int32_t, int32_t>，默认值：空
	std::string 				client_name ;				///< 客户端名称
	std::string 				client_version;				///< 客户端版本号
	int32_t                     mode; ///< 模式mode为1时快速登录，不处理回报同步，API与AGW的tcp连接断开情况下存在委托丢失.默认0进行回报同步处理

	ATPConnectProperty();
};

/**
*	@brief 客户连接设置参数
*/
struct TRADE_API ATPConnectCustProperty
{
	std::vector<std::string>	locations;					///< AGW IP地址 格式为："ip:port"
	int32_t						heartbeat_interval_milli;	///< 发送心跳的时间间隔,单位：毫秒，默认值：5000
	int32_t						connect_timeout_milli;      ///< 连接超时时间，单位：毫秒，默认值：5000 
	int32_t						reconnect_time;             ///< 重试连接次数，默认值：连接地址数

	std::string 				client_name;				///< 客户端名称
	std::string 				client_version;				///< 客户端版本号
    std::map<int32_t, int32_t> report_sync;                 ///< 回报同步数据map<int32_t, int32_t>，默认值：空
	int32_t                     mode; ///< 模式mode为1时快速登录，不处理回报同步，API与AGW的tcp连接断开情况下存在委托丢失.默认0进行回报同步处理
	ATPConnectCustProperty();
};



/**
 *  @brief 待查询持仓股东单元
 */
struct TRADE_API APIAccountIDUnit
{
    ATPAccountIDType account_id; ///证券账户ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    
    APIAccountIDUnit();
};


/**
 *  @brief 行权合约单元信息
 */
struct TRADE_API APIExerciseLegType
{
    ATPSecurityIDType leg_security_id; ///成分合约代码
    
    APIExerciseLegType();
};


/**
 *  @brief 行权盈亏信息
 */
struct TRADE_API APIExerciseProfitLossLegType
{
    ATPSecurityIDType leg_security_id; ///成分合约代码
    ATPAmtType profit_loss; ///成分合约行权预估盈亏N18(4)
    ATPAmtType contract_buy_cost; ///成分合约行权合约买入成本N18(4)
    
    APIExerciseProfitLossLegType();
};


/**
 *  @brief 多腿信息委托单元
 */
struct TRADE_API APINoLegsOrderUnitType
{
    ATPSecurityIDType leg_security_id; ///成分合约代码
    ATPMarketIDType market_id; ///市场
    ATPLegSideType leg_side; ///持仓类型
    ATPCoveredOrUncoveredType leg_covered_or_uncovered; ///腿备兑标签
    
    APINoLegsOrderUnitType();
};


/**
 *  @brief 现货交易委托消息
 */
struct TRADE_API ATPReqCashAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    
    ATPReqCashAuctionOrderMsg();
};


/**
 *  @brief 证券委托组合
 */
struct TRADE_API APIBatchCashAuctionOrderUnit
{
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量 N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    
    APIBatchCashAuctionOrderUnit();
};


/**
 *  @brief 批量现货交易委托消息
 */
struct TRADE_API ATPReqBatchCashAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4)
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    ATPBatchType batch_type; ///批量委托类型 
    ATPQtyType decrease_qty; ///递减数量N15(2)
    ATPQtyType max_order_qty; ///单笔上限N15(2)
    ATPQtyType min_order_qty; ///单笔下限（已弃用）
    std::vector<APIBatchCashAuctionOrderUnit> order_array; ///多证券委托组合 
    
    ATPReqBatchCashAuctionOrderMsg();
};


/**
 *  @brief 融资融券交易委托消息
 */
struct TRADE_API ATPReqCreditOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 ‘S’=  融券卖出、‘B’= 融资买入
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    
    ATPReqCreditOrderMsg();
};


/**
 *  @brief 融资融券非交易业务消息
 */
struct TRADE_API ATPReqMarginNoTradeOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID (投资者信用账户)
    ATPFundAccountIDType fund_account_id; ///资金账户ID (投资者信用账户)
    ATPBranchIDType branch_id; ///营业部ID (投资者信用账户)
    ATPAccountIDType account_id; ///证券账户ID (投资者信用账户)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段（指定合约偿还列表，ATP3.1.11开始支持；具体填写规则可参考引言中的接口约定） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向（投资者：'L'= 担保品返还、'T'= 直接还券、'V'= 担保品提交）
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPbuIDType dest_pbu_id; ///对方申报交易单元 （买卖方向 = ‘L’、‘V’时：深市必传，沪市传空）
    ATPAccountIDType dest_account_id; ///对方证券账户 （买卖方向 = ‘L’、‘V’时：深市必传，沪市传空）
    ATPRedeliverType redeliver_type; ///直接还券类型（默认值为255，表示不指定）,ATP3.1.10.1开始支持非必填
    
    ATPReqMarginNoTradeOrderMsg();
};


/**
 *  @brief 融资融券交易偿还业务委托消息
 */
struct TRADE_API ATPReqMarginRepayOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段（指定合约偿还列表，ATP3.1.11开始支持；具体填写规则可参考引言中的接口约定） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向（投资者：'N'= 卖券还款、'U'= 买券还券）
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量 N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    
    ATPReqMarginRepayOrderMsg();
};


/**
 *  @brief 质押式回购集中竞价业务委托消息
 */
struct TRADE_API ATPReqRepoAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '3'=正回购，'4'=逆回购
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqRepoAuctionOrderMsg();
};


/**
 *  @brief 债券分销业务委托消息
 */
struct TRADE_API ATPReqBondDistributionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '1'=认购  '2'=分销
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqBondDistributionOrderMsg();
};


/**
 *  @brief 期权集中竞价业务委托消息
 */
struct TRADE_API ATPReqOptionAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2)（张） 
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPOrdTypeType order_type; ///订单类型 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPQtyType min_qty; ///最低成交数量（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    ATPPositionEffectType position_effect; ///开平仓标志 
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码 
    
    ATPReqOptionAuctionOrderMsg();
};


/**
 *  @brief 期权报价业务委托消息
 */
struct TRADE_API ATPReqOptionQuoteOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType bid_order_qty; ///买申报数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2) 
    ATPPriceType bid_price; ///买委托价格N13(4) 填0时，即可进行买委托的撤单
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPQuoteReqIDType quote_req_id; ///报价请求序号 
    ATPPriceType offer_price; ///卖报价N13(4) 
    ATPQtyType offer_order_qty; ///卖数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2) 填0时，即可进行卖委托的撤单
    ATPPositionEffectType bid_position_effect; ///买平仓标志 
    ATPPositionEffectType offer_position_effect; ///卖平仓标志 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    
    ATPReqOptionQuoteOrderMsg();
};


/**
 *  @brief 期权强平业务委托消息
 */
struct TRADE_API ATPReqOptionAuctionForceCloseOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPOrdTypeType order_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量 N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签 
    ATPPositionEffectType position_effect; ///开平仓标志 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPBatchNoType batch_no; ///强平批次 
    
    ATPReqOptionAuctionForceCloseOrderMsg();
};


/**
 *  @brief 组合保证金腿信息单元
 */
struct TRADE_API APINoLegsOrderUnit
{
    ATPSecurityIDType leg_security_id; ///成份合约代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPLegSideType leg_side; ///持仓类型 
    ATPCoveredOrUncoveredType leg_covered_or_uncovered; ///腿备兑标签 
    ATPQtyType leg_order_qty; ///成份合约数量 此字段响应时返回；委托不需要填（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    APINoLegsOrderUnit();
};


/**
 *  @brief 期权组合保证金业务委托消息
 */
struct TRADE_API ATPReqOptionCombinationOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '1':解除，'2'：组合
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPSecurityIDType strategy_id; ///策略编码 
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码，买卖方向为解除时需要填，组合时不需要填
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    std::vector<APINoLegsOrderUnit> no_legs_array; ///多腿信息组合
    
    ATPReqOptionCombinationOrderMsg();
};


/**
 *  @brief 期权合并行权成份合约单元
 */
struct TRADE_API APIExerciseLeg
{
    ATPSecurityIDType leg_security_id; ///成份合约信息 
    
    APIExerciseLeg();
};


/**
 *  @brief 期权合并行权业务委托消息
 */
struct TRADE_API ATPReqOptionCombinationExerciseOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    std::vector<APIExerciseLeg> no_legs_array; ///成份合约
    
    ATPReqOptionCombinationExerciseOrderMsg();
};


/**
 *  @brief ETF申赎业务委托消息
 */
struct TRADE_API ATPReqETFRedemptionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///基金证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 'D'=申购  'E'=赎回  'P'=实物申购  'R'=实物赎回
    ATPQtyType order_qty; ///申赎基金份额N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqETFRedemptionOrderMsg();
};


/**
 *  @brief 网上发行业务委托消息
 */
struct TRADE_API ATPReqIssueOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType order_qty; ///申报数量N15(2)（股） 
    ATPPriceType price; ///委托价格N13(4)（元） 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqIssueOrderMsg();
};


/**
 *  @brief 配售业务委托消息
 */
struct TRADE_API ATPReqRightsIssueOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///配售权证代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向（沪市填 '2'=卖出，深市填'1'=买入 ）
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4)
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqRightsIssueOrderMsg();
};


/**
 *  @brief 债券转股回售业务委托消息
 */
struct TRADE_API ATPReqSwapPutbackOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '5'=转股  '6'=回售 '9'=回售撤销
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （已弃用）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPShareNegotiabilityType share_negotiability; ///股份性质，证券类型为定向可转债业务时必填
    
    ATPReqSwapPutbackOrderMsg();
};


/**
 *  @brief 期权行权业务委托消息
 */
struct TRADE_API ATPReqOptionExerciseOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    
    ATPReqOptionExerciseOrderMsg();
};


/**
 *  @brief 转托管业务委托消息
 */
struct TRADE_API ATPReqDesignationOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码（默认值为空字符串，表示该证券账户下的全部证券） 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号，撤销转托管时填
    ATPDesignationTransTypeType designation_trans_type; ///转托管注册类型 
    ATPPBUIDType transferee_pbu_id; ///转入托管单元代码 
    
    ATPReqDesignationOrderMsg();
};


/**
 *  @brief 指定交易业务委托消息
 */
struct TRADE_API ATPReqDesignatedTransactionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 "799998"=撤销指定交易  "799999"=登记指定交易
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPBUIDType designated_pbu_id; ///指定的交易单元 
    
    ATPReqDesignatedTransactionOrderMsg();
};


/**
 *  @brief 开放式基金申赎业务委托消息
 */
struct TRADE_API ATPReqLOFOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///基金证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 'D'=申购  'E'=赎回
    ATPQtyType order_qty; ///赎回基金份额N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPAmtType cash_order_qty; ///申购金额N18(4) 
    
    ATPReqLOFOrderMsg();
};


/**
 *  @brief 要约收购业务委托消息
 */
struct TRADE_API ATPReqTenderOfferOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '7'=预受要约  '8'= 解除预受要约
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPTendererType tenderer; ///要约收购参与人编码 
    
    ATPReqTenderOfferOrderMsg();
};


/**
 *  @brief 网络投票业务委托消息
 */
struct TRADE_API ATPReqNetVotingOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///投票代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType order_qty; ///申报数量N15(2) 累积投票议案时，OrderQty字段填写投票数量，其他情况填0
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPGeneralMeetingSeqType general_meeting_seq; ///股东大会编码 
    ATPVotingProposalType voting_proposal; ///投票议案编号及子议案编号（沪深通用），议案和子议案用小数点分隔。例如：2.01，表示议案编号2，子议案编号1 
    ATPVotingPreferenceType voting_preference; ///投票意向（当为非累积投票议案时） 
    ATPVotingSegmentType voting_segment; ///分段统计段号（预留）（不填）
    
    ATPReqNetVotingOrderMsg();
};


/**
 *  @brief 密码服务业务委托消息
 */
struct TRADE_API ATPReqPasswordServiceOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 。 'A'= Activation 密码激活 ，'C'= Cancellation 密码注销
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPValidationCodeType validation_code; ///密码(可变长字段)激活校验码 
    
    ATPReqPasswordServiceOrderMsg();
};


/**
 *  @brief 权证行权业务委托消息
 */
struct TRADE_API ATPReqWarrantExerciseOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 ，固定填深圳市场代码
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqWarrantExerciseOrderMsg();
};


/**
 *  @brief 分级基金实时分拆合并业务委托消息
 */
struct TRADE_API ATPReqStructuredFundOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 ，固定填深圳市场代码
    ATPSideType side; ///买卖方向 'O'=Open 分拆 , 'M'=Merge 合并
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqStructuredFundOrderMsg();
};


/**
 *  @brief 锁定解锁订单
 */
struct TRADE_API ATPReqCoveredLockOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPSideType side; ///买卖方向 'Z'=锁定  'Y'= 解锁
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqCoveredLockOrderMsg();
};


/**
 *  @brief 盘后定价(科创板、创业板)业务委托消息
 */
struct TRADE_API ATPReqTibAfterHourAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPSideType side; ///买卖方向 '1'=买 ，'2'=卖
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPMarketIDType market_id; ///市场代码 
    
    ATPReqTibAfterHourAuctionOrderMsg();
};


/**
 *  @brief 转处置业务订单
 */
struct TRADE_API ATPReqTurnToDisposalOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPBUIDType disposal_pbu; ///划入待处置券的交易单元 
    ATPAccountIDType disposal_account_id; ///划入待处置券的证券账户 
    
    ATPReqTurnToDisposalOrderMsg();
};


/**
 *  @brief 质押出入库业务委托消息
 */
struct TRADE_API ATPReqPledgeInOrOutOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 ‘H’表示入库‘I’表示出库
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    
    ATPReqPledgeInOrOutOrderMsg();
};


/**
 *  @brief 通用撤单消息
 */
struct TRADE_API ATPReqCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cl_ord_no; ///原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no 
    
    ATPReqCancelOrderMsg();
};


/**
 *  @brief 一键撤单消息
 */
struct TRADE_API ATPReqBatchCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID，如果证券账号为空，则按客户号进行撤单，如果证券账户和客户号均为空则指令无效
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID ，填空则按照客户号一键撤单，填写了证券账户，则优先按证券账户进行撤单
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBusinessTypeType business_type; ///业务类型,撤所有业务类型时，填0
    ATPSecurityIDType security_id; ///证券代码（填空表示撤销所有） 
    ATPMarketIDType market_id; ///市场代码 
    ATPBranchIDType branch_id; ///营业部ID 
    BusinessAbstractType business_abstract_type; ///抽象业务编码 
    
    ATPReqBatchCancelOrderMsg();
};


/**
 *  @brief FDEP公共撤单消息
 */
struct TRADE_API ATPReqFDEPCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cl_ord_no; ///原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no
    
    ATPReqFDEPCancelOrderMsg();
};


/**
 *  @brief 期权撤单消息
 */
struct TRADE_API ATPReqOptionCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cl_ord_no; ///原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    
    ATPReqOptionCancelOrderMsg();
};


/**
 *  @brief 委托状态响应消息
 */
struct TRADE_API ATPRspOrderStatusAckMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态,（对于撤单委托，填写被撤委托的当前状态，找不到原始委托时，固定填8= Reject ）
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间（如果是ATP的Ack则回报时间为ATP的，如果是交易所的确认则回报时间为交易所的。同理，成交回报都是交易所的回报时间）
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExtraData extra_data; ///扩展字段
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_cl_ord_no; ///对于撤单返回的状态响应，为原始委托的客户合同号，指示被撤消订单的cl_ord_no; 对于委托确认，取值为0
    ATPAmtType frozen_trade_value; ///冻结交易金额N18(4)
    ATPAmtType frozen_fee; ///冻结费用N18(4)
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPOrdTypeType order_type; ///订单类型
    ATPTimeInForceType time_in_force; ///订单有效时间类型（无意义）
    ATPPositionEffectType position_effect; ///开平仓标志（用于期权业务）
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签（用于期权业务）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（无意义）
    ATPQuoteFlagType quote_flag; ///是否报价订单（无意义）
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码（用于期权业务）
    ATPClOrdNOType parent_cl_ord_no; ///批量订单母单编号
    ATPParticularFlagsType particular_flags; ///特殊委托标识，右起第一个bit位是夜市委托标志位：0-正常委托、1-夜市委托（ATP3.2.0开始支持）
    
    ATPRspOrderStatusAckMsg();
};


/**
 *  @brief 批量委托状态响应消息
 */
struct TRADE_API ATPRspBatchOrderStatusAckMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType parent_cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态,（对于撤单委托，填写被撤委托的当前状态，找不到原始委托时，固定填8= Reject ）
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)
    ATPQtyType leaves_qty; ///未成交部分的数量
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_parent_cl_ord_no; ///原始委托的客户合同号，指示被撤消批量订单的cl_ord_no;
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPOrdTypeType order_type; ///订单类型
    ATPTimeInForceType time_in_force; ///订单有效时间类型（无意义）
    ATPPositionEffectType position_effect; ///开平仓标志
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签
    ATPAccountSubCodeType account_sub_code; ///账户子编码（无意义）
    ATPQuoteFlagType quote_flag; ///是否报价订单（无意义）
    ATPQtyType success_order_qty; ///成功数量|笔数
    ATPQtyType failed_order_qty; ///失败数量|笔数
    ATPParticularFlagsType particular_flags; ///特殊委托标识，右起第一个bit位是夜市委托标志位：0-正常委托、1-夜市委托（ATP3.2.0开始支持）
    
    ATPRspBatchOrderStatusAckMsg();
};


/**
 *  @brief 期权报价委托状态响应消息
 */
struct TRADE_API ATPRspQuoteStatusAckMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPQuoteReqIDType quote_req_id; ///报价请求序号（保留字段），返回原报价单字段
    ATPQuoteStatusType quote_status; ///报价单状态
    ATPExecTypeType bid_exec_type; ///买订单当前执行类型
    ATPOrdStatusType bid_ord_status; ///买订单当前申报的状态 当不存在买委托时，返回99表示无意义状态
    ATPExecTypeType offer_exec_type; ///卖订单当前执行类型
    ATPOrdStatusType offer_ord_status; ///卖订单当前申报的状态 当不存在卖委托时，返回99表示无意义状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType bid_price; ///买委托价格N13(4)
    ATPQtyType bid_order_qty; ///买委托数量，当为报价撤单时为0（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType bid_leaves_qty; ///买未成交部分的数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType bid_cum_qty; ///买累计成交数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPPriceType offer_price; ///卖委托价格N13(4)
    ATPQtyType offer_order_qty; ///卖委托数量，当为报价撤单时为0（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType offer_leaves_qty; ///卖未成交部分的数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType offer_cum_qty; ///卖累计成交数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPTimeStampType transact_time; ///回报时间（如果是TE的Ack则回报时间为TE的，如果是交易所的确认则回报时间为交易所的。同理，成交回报都是交易所的回报时间）
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPAmtType bid_frozen_trade_value; ///买冻结交易金额N18(4)
    ATPAmtType bid_frozen_fee; ///买冻结费用N18(4)
    ATPAmtType offer_frozen_trade_value; ///卖冻结交易金额N18(4)
    ATPAmtType offer_frozen_fee; ///卖冻结费用N18(4)
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPPositionEffectType bid_position_effect; ///买委托开仓/平仓,'C'=平仓,'O'=开仓 报价撤单时为空
    ATPPositionEffectType offer_position_effect; ///卖委托开仓/平仓,'C'=平仓,'O'=开仓 报价撤单时为空
    ATPAccountSubCodeType account_sub_code; ///账户子编码（无意义）
    
    ATPRspQuoteStatusAckMsg();
};


/**
 *  @brief 多腿委托状态响应消息
 */
struct TRADE_API ATPRspNoLegsStatusAckMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态,（对于撤单委托，填写被撤委托的当前状态，找不到原始委托时，固定填8= Reject ）
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间。如果是TE的Ack则回报时间为TE的，如果是交易所的确认则回报时间为交易所的。同理，成交回报都是交易所的回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExtraData extra_data; ///扩展字段
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_cl_ord_no; ///对于撤单返回的状态响应，为原始委托的客户合同号，指示被撤消订单的cl_ord_no; 对于委托确认，取值为0
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPAccountSubCodeType account_sub_code; ///账户子编码（无意义）
    ATPSecurityIDType strategy_id; ///策略编号
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码
    std::vector<APINoLegsOrderUnit> no_legs_array; ///成份合约信息
    
    ATPRspNoLegsStatusAckMsg();
};


/**
 *  @brief 现货集中竞价业务成交回报
 */
struct TRADE_API ATPRspCashAuctionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPCashMarginType cash_margin; ///信用标识
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPClOrdNOType parent_cl_ord_no; ///母单订单编号
    ATPParticularFlagsType particular_flags; ///特殊委托标识，右起第一个bit位是夜市委托标志位：0-正常委托、1-夜市委托（ATP3.2.0开始支持）
    
    ATPRspCashAuctionTradeERMsg();
};


/**
 *  @brief 融资融券交易委托成交回报
 */
struct TRADE_API ATPRspCreditTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）（注意，上交所回报该字段含义：对应申报是市价转撤单的时候，这里填写自动撤单的数量；对应申报是市价转限价的时候，这里填写转为限价订单的数量；当对应申报是限价订单时，填写撤单成功的数量；缺省填0；深交所回报该字段含义：订单未成交部分的数量）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPCashMarginType cash_margin; ///信用标识
    
    ATPRspCreditTradeERMsg();
};


/**
 *  @brief 融资融券交易偿还业务成交回报
 */
struct TRADE_API ATPRspMarginRepayTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）（注意，上交所回报该字段含义：对应申报是市价转撤单的时候，这里填写自动撤单的数量；对应申报是市价转限价的时候，这里填写转为限价订单的数量；当对应申报是限价订单时，填写撤单成功的数量；缺省填0；深交所回报该字段含义：订单未成交部分的数量）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPCashMarginType cash_margin; ///信用标识
    
    ATPRspMarginRepayTradeERMsg();
};


/**
 *  @brief 期权集中竞价业务成交回报
 */
struct TRADE_API ATPRspOptionAuctionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPPositionEffectType position_effect; ///开平仓标志
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签
    ATPAccountSubCodeType account_sub_code; ///合约账户子编码（无意义）
    ATPQuoteFlagType quote_flag; ///是否报价订单（无意义）
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPQtyType position_value; ///可用持仓
    ATPQtyType total_position_value; ///总持仓
    ATPAmtType available_amt; ///可用资金
    ATPAmtType total_amt; ///资金总额
    ATPSecuritySymbolType security_symbol; ///证券名称
    ATPOrdTypeType ord_type; ///委托类型
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码
    
    ATPRspOptionAuctionTradeERMsg();
};


/**
 *  @brief 质押式回购集中竞价业务成交回报
 */
struct TRADE_API ATPRspRepoAuctionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '3'=正回购， '4'=逆回购
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPDate8Type maturity_date; ///到期日
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspRepoAuctionTradeERMsg();
};


/**
 *  @brief ETF成分股单元
 */
struct TRADE_API APIConstituentStockUnit
{
    ATPSecurityIDType security_id; ///成分股证券代码，例外：1-沪市资金代码、2-深市资金代码、3-港市资金代码、9-非沪深资金代码（全额现金申赎）
    ATPMarketIDType market_id; ///市场代码，当security_id为资金代码时为空
    ATPQtyType qty; ///成交数量，当security_id为资金代码时为0（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType amt; ///成交金额N18(4)，对于沪市ETF申赎成交回报，security_id为资金代码时有值，否则为0；对于深市ETF，原逻辑不变。
    ATPPriceType price; ///成交价格N13(4)，对于沪市ETF申赎成交回报，此字段为0
    ATPETFTradeReportTypeType etf_trade_report_type; ///ETF成交回报类型
    ATPExecIDType exec_id; ///执行编号
    
    APIConstituentStockUnit();
};


/**
 *  @brief ETF申赎业务成交回报
 */
struct TRADE_API ATPRspETFRedemptionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)固定为0
    ATPQtyType order_qty; ///委托数量N15(2)，原申赎份额（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)，一但有成交则认为全部成交，所以此处为0（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)，原申赎份额（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 'D'=申购  'E'=赎回
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)，固定为0
    ATPAmtType fee; ///费用N18(4)，固定为0
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    std::vector<APIConstituentStockUnit> constituent_stock; ///成分股与资金信息
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspETFRedemptionTradeERMsg();
};


/**
 *  @brief 债券分销业务成交回报
 */
struct TRADE_API ATPRspBondDistributionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）单位：元（精确到0.0001元）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=认购  '2'=分销
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspBondDistributionTradeERMsg();
};


/**
 *  @brief 分级基金实时分拆合并业务成交回报
 */
struct TRADE_API ATPRspStructuredFundTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向  'O'= Open 分拆 ，'M'= Merge 合并
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspStructuredFundTradeERMsg();
};


/**
 *  @brief 开放式基金申赎业务成交回报
 */
struct TRADE_API ATPRspLOFTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向  'D'=申购  'E'=赎回
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspLOFTradeERMsg();
};


/**
 *  @brief 盘后定价(科创板、创业板)业务成交回报
 */
struct TRADE_API ATPRspTibAfterHourAuctionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向  '1'=买  '2' =卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPCashMarginType cash_margin; ///信用标识
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspTibAfterHourAuctionTradeERMsg();
};


/**
 *  @brief 质押入出库业务成交回报
 */
struct TRADE_API ATPRspPledgeInOrOutTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向  'H'=入库  'I' =出库
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspPledgeInOrOutTradeERMsg();
};


/**
 *  @brief 订单查询消息
 */
struct TRADE_API ATPReqOrderQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType cl_ord_no; ///客户订单编号 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSideType side; ///买卖方向 （默认值为'0'，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPOrderQueryConditionType order_query_condition; ///委托查询条件 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    BusinessAbstractType business_abstract_type; ///抽象业务编码 
    
    ATPReqOrderQueryMsg();
};


/**
 *  @brief 批量订单查询消息
 */
struct TRADE_API ATPReqBatchOrderQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType parent_cl_ord_no; ///客户订单编号 
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqBatchOrderQueryMsg();
};


/**
 *  @brief 报价订单信息
 */
struct TRADE_API APIQuoteUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向
    ATPOrdTypeType ord_type; ///订单类别
    ATPQuoteReqIDType quote_req_id; ///报价请求序号
    ATPQuoteStatusType quote_ord_status; ///原订单当前状态
    ATPTimeStampType transact_time; ///回报时间。如果是TE的Ack则回报时间为TE的，如果是交易所的确认则回报时间为交易所的。同理，成交回报都是交易所的回报时间
    ATPPriceType bid_order_price; ///买委托价格N13(4)
    ATPPriceType bid_exec_price; ///买委托平均成交价格N13(4)
    ATPQtyType bid_order_qty; ///买委托数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType bid_leaves_qty; ///买未成交部分的数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType bid_cum_qty; ///买累计成交数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType bid_frozen_trade_value; ///买冻结交易金额N18(4)
    ATPAmtType bid_frozen_fee; ///买冻结费用N18(4)
    ATPOrdStatusType bid_ord_status; ///买订单当前的状态
    ATPPositionEffectType bid_position_effect; ///买开仓/平仓
    ATPPriceType offer_order_price; ///卖委托价格N13(4)
    ATPPriceType offer_exec_price; ///卖委托平均成交价格N13(4)
    ATPQtyType offer_order_qty; ///卖委托数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType offer_leaves_qty; ///卖未成交部分的数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType offer_cum_qty; ///卖累计成交数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType offer_frozen_trade_value; ///卖冻结交易金额N18(4)
    ATPAmtType offer_frozen_fee; ///卖冻结费用N18(4)
    ATPOrdStatusType offer_ord_status; ///卖订单当前的状态
    ATPPositionEffectType offer_position_effect; ///卖开仓/平仓
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPOrderIDType order_id; ///交易所订单编号, 取值为数字
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
    
    APIQuoteUnit();
};


/**
 *  @brief 报价订单查询结果
 */
struct TRADE_API ATPRspQuoteQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIQuoteUnit> order_array; ///查询结果组合
    
    ATPRspQuoteQueryResultMsg();
};


/**
 *  @brief 报价查询消息
 */
struct TRADE_API ATPReqQuoteQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType cl_ord_no; ///客户订单编号 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSideType side; ///买卖方向 （默认值为'0'，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPOrderQueryConditionType order_query_condition; ///委托查询条件 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    
    ATPReqQuoteQueryMsg();
};


/**
 *  @brief 期权组合策略保证金组合持仓查询结果组合
 */
struct TRADE_API APICombinationUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPAccountSubCodeType account_sub_code; ///证券子编码
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码
    ATPSecurityIDType strategy_id; ///策略编码
    ATPQtyType init_combi_position; ///日初组合持仓量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType combi_position; ///组合实时持仓数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType frozen_combi_position; ///组合解除冻结数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType init_combi_occupy_amt; ///日初组合占用保证金N18(4)
    ATPAmtType combi_occupy_amt; ///组合占用保证金N18(4)
    ATPAmtType init_origin_occupy_amt; ///日初组合前占用保证金N18(4)
    ATPAmtType origin_occupy_amt; ///组合前占用保证金N18(4)
    std::vector<APINoLegsOrderUnit> position_array; ///成份合约代码
    
    APICombinationUnit();
};


/**
 *  @brief 期权组合策略保证金查询结果
 */
struct TRADE_API ATPRspCombinationQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APICombinationUnit> order_array; ///查询结果组合
    
    ATPRspCombinationQueryResultMsg();
};


/**
 *  @brief 组合策略保证金持仓查询消息
 */
struct TRADE_API ATPReqCombinationQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqCombinationQueryMsg();
};


/**
 *  @brief 成交查询消息
 */
struct TRADE_API ATPReqTradeOrderQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    BusinessAbstractType business_abstract_type; ///抽象业务编码 
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPClOrdIDType cl_ord_id; ///报盘合同号
    ATPExecIDType exec_id; ///执行编号
    ATPEtfPurchaseRedemptionFlagType etf_purchase_redemption_flag; ///是否返回ETF实时申赎成交：0-不返回，1-返回（ATP3.2.1开始支持）
    
    ATPReqTradeOrderQueryMsg();
};


/**
 *  @brief 融资融券合约明细查询消息
 */
struct TRADE_API ATPReqExtQueryContractSpecificationsMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPCloseOutFlagType close_out_flag; ///了结标识 （默认值为'0'，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPCashMarginSetType cash_margin_set; ///信用标识集合 （默认值为空字符串，表示查所有；标志位之间请用逗号隔开，如需查询融券合约与融资利息两种合约明细请填写"2,3" 具体填写规则可参考引言中的接口约定）
    ATPDate8Type opening_date; ///开仓日期 （默认值为0，格式为YYYYMMDD，如20210514）
    ATPOpeningSerialNumType opening_serial_num; ///开仓流水号 （默认值为空字符串）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    
    ATPReqExtQueryContractSpecificationsMsg();
};


/**
 *  @brief 融资融券信用额度查询消息
 */
struct TRADE_API ATPReqExtQueryCreditAmountMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    
    ATPReqExtQueryCreditAmountMsg();
};


/**
 *  @brief 融资融券保证金可用查询消息
 */
struct TRADE_API ATPReqExtQueryMarginUseableMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPQueryContentsType query_contents; ///查询内容 '0'= 只查询余额，'1'= 查询余额及明细
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）（ATP3.1.10.1开始支持）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）（ATP3.1.10.1开始支持）
    
    ATPReqExtQueryMarginUseableMsg();
};


/**
 *  @brief 融资融券资金负债查询消息
 */
struct TRADE_API ATPReqExtQueryFundLiabilityMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPCurrencyType currency; ///货币种类 （仅支持人民币）
    
    ATPReqExtQueryFundLiabilityMsg();
};


/**
 *  @brief 信用账户融资保底额度查询消息
 */
struct TRADE_API ATPReqExtQueryMarginTradingPrivateAmoutMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPCurrencyType currency; ///货币种类 （仅支持人民币）
    
    ATPReqExtQueryMarginTradingPrivateAmoutMsg();
};


/**
 *  @brief 融券保底额度查询消息
 */
struct TRADE_API ATPReqExtQuerySecuritiesLendingPrivateAmountMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQuerySecuritiesLendingPrivateAmountMsg();
};


/**
 *  @brief 融资融券股份负债查询消息
 */
struct TRADE_API ATPReqExtQueryShareLiabilityMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    std::vector<APIAccountIDUnit> account_id_array; ///待查证券账户组 
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryShareLiabilityMsg();
};


/**
 *  @brief 融资融券合约管理查询消息
 */
struct TRADE_API ATPReqExtQueryContractSpecificationsManagerMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPCloseOutFlagType close_out_flag; ///了结标识 （默认值为'0'，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPCashMarginSetType cash_margin_set; ///信用标识集合（该接口仅支持1-融资合约,2-融券合约两种类型，标志位之间用逗号隔开;具体填写规则可参考引言中的接口约定）
    ATPDate8Type opening_date; ///开仓日期 (默认值为0，格式为YYYYMMDD，如20210514）
    ATPOpeningSerialNumType opening_serial_num; ///开仓流水号 （默认值为空字符串）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryContractSpecificationsManagerMsg();
};


/**
 *  @brief 融资融券标的清单查询消息
 */
struct TRADE_API ATPReqExtQueryMarginUnderlyingMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPCRDUnderlyingTypeType underlying_query_type; ///标的类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPIsQueryMarginQtyType is_query_margin_qty; ///是否输出融券专户可用数量（默认值为0，表示不输出：记录都输出且融券专户可用数量默认0）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryMarginUnderlyingMsg();
};


/**
 *  @brief 融资融券客户信用资料查询消息
 */
struct TRADE_API ATPReqExtQueryCreditInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    
    ATPReqExtQueryCreditInfoMsg();
};


/**
 *  @brief 融资融券资产信息查询消息
 */
struct TRADE_API ATPReqExtQueryFundAssetsInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID 
    
    ATPReqExtQueryFundAssetsInfoMsg();
};


/**
 *  @brief 增强成交查询消息
 */
struct TRADE_API ATPReqTradeOrderExQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType cl_ord_no; ///客户订单编号 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    BusinessAbstractType business_abstract_type; ///抽象业务编码 
    ATPClOrdIDType cl_ord_id; ///报盘合同号
    ATPExecIDType exec_id; ///执行编号
    ATPEtfPurchaseRedemptionFlagType etf_purchase_redemption_flag; ///是否返回ETF实时申赎成交：0-不返回，1-返回（ATP3.2.1开始支持）
    
    ATPReqTradeOrderExQueryMsg();
};


/**
 *  @brief ETF申赎成交查询
 */
struct TRADE_API ATPReqETFQueryOrderExMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType cl_ord_no; ///客户订单编号 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型（已弃用）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    BusinessAbstractType business_abstract_type; ///抽象业务编码（已弃用）
    
    ATPReqETFQueryOrderExMsg();
};


/**
 *  @brief 资金查询
 */
struct TRADE_API ATPReqFundQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID （ATP3.1.7版本开始支持此字段非必填）
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPCurrencyType currency; ///货币种类（ATP3.1.8.1开始支持非必填）（默认值为空串表示查询CNY）
    
    ATPReqFundQueryMsg();
};


/**
 *  @brief 股份持仓查询
 */
struct TRADE_API ATPReqShareQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID（ATP3.1.7版本开始支持此字段非必填） 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    std::vector<APIAccountIDUnit> account_id_array; ///待查证券账户组（ATP3.1.6版本开始支持非必填）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）（ATP3.1.6版本开始支持此字段非必填）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqShareQueryMsg();
};


/**
 *  @brief 新股发行申购额度查询消息
 */
struct TRADE_API ATPReqIPOQtyQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码
    
    ATPReqIPOQtyQueryMsg();
};


/**
 *  @brief 合约账户持仓查询消息
 */
struct TRADE_API ATPReqAccountContractShareQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    
    ATPReqAccountContractShareQueryMsg();
};


/**
 *  @brief 指定合约账户资金查询消息
 */
struct TRADE_API ATPReqAccountContractFundQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID（一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPBranchIDType branch_id; ///营业部 
    ATPExtraData extra_data; ///扩展字段 
    
    ATPReqAccountContractFundQueryMsg();
};


/**
 *  @brief 成交汇总查询消息
 */
struct TRADE_API ATPReqTradeCollectQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 ,填0查所有
    ATPSecurityIDType security_id; ///证券代码 
    ATPBusinessTypeType business_type; ///业务类型,填0查所有
    BusinessAbstractType business_abstract_type; ///抽象业务编码（未启用）
    
    ATPReqTradeCollectQueryMsg();
};


/**
 *  @brief 科创新股发行申购额度查询消息
 */
struct TRADE_API ATPReqTIBIPOQtyQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码（仅支持101-上海市场）
    
    ATPReqTIBIPOQtyQueryMsg();
};


/**
 *  @brief 期权最大可委托数查询消息
 */
struct TRADE_API ATPReqCoverMaxOrderQtyQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPBusinessTypeType business_type; ///业务类型 
    ATPSideType side; ///买卖方向 
    ATPPriceType price; ///委托价格N13(4) 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    ATPPositionEffectType position_effect; ///开平仓标志 
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签 
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码 
    
    ATPReqCoverMaxOrderQtyQueryMsg();
};


/**
 *  @brief 港股通转托管最大可委托数查询消息
 */
struct TRADE_API ATPReqHKExtQueryMaxQueryOrderQtyMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPBusinessTypeType business_type; ///业务类型 
    ATPSideType side; ///买卖方向 
    ATPPriceType price; ///委托价格 N13(4) 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPEntitlementType entitlement_number; ///权益编码 
    ATPBranchIDType branch_id; ///营业部ID 
    
    ATPReqHKExtQueryMaxQueryOrderQtyMsg();
};


/**
 *  @brief 最大可委托数查询消息
 */
struct TRADE_API ATPReqMaxOrderQtyQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段（指定合约偿还列表，ATP3.1.11开始支持；具体填写规则可参考引言中的接口约定） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPBusinessTypeType business_type; ///业务类型 
    ATPSideType side; ///买卖方向 
    ATPPriceType price; ///委托价格N13(4) 
    ATPImmediateEffectType immediate_effect; ///实时划转标志 
    ATPRedeliverType redeliver_type; ///直接还券类型（默认值为255，表示不指定）,ATP3.1.10.1开始支持非必填 
    
    ATPReqMaxOrderQtyQueryMsg();
};


/**
 *  @brief 增强型最大可委托数查询消息
 */
struct TRADE_API ATPReqNewMaxOrderQtyQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPBusinessTypeType business_type; ///业务类型 
    ATPSideType side; ///买卖方向 
    ATPPriceType price; ///委托价格N13(4) 
    ATPOrdTypeType order_type; ///订单类型 
    ATPTrdSubTypeType trd_sub_type; ///是否受限 （仅适用于大宗交易）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqNewMaxOrderQtyQueryMsg();
};


/**
 *  @brief 账户登入消息
 */
struct TRADE_API ATPReqCustLoginOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID （当登录模式为1-客户号登录模式时，该字段必填）
    ATPFundAccountIDType fund_account_id; ///资金账户ID （当登录模式为2-资金账户登录模式时，该字段必填）
    ATPAccountIDType account_id; ///证券账户ID （当登录模式为3-证券账户登录模式时，该字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPPasswordType password; ///客户密码 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID （当登录模式为2-资金账户登录模式时，该字段是否必填请咨询券商）
    ATPLoginModeType login_mode; ///客户号登录模式 
    
    ATPReqCustLoginOtherMsg();
};


/**
 *  @brief 账户登出消息
 */
struct TRADE_API ATPReqCustLogoutOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID（是否必填请咨询券商）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqCustLogoutOtherMsg();
};


/**
 *  @brief 外部资金划拨请求
 */
struct TRADE_API ATPReqExtFundTransferOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPBoolType is_account_partition; ///账户是否按证券账户分区（已弃用）
    ATPCurrencyType currency; ///货币种类 
    ATPTransferWayType transfer_way; ///划拨方式 ：1=转入；2=转出
    ATPAmtType transfer_value; ///划拨金额 N18(4) 
    ATPBranchIDType branch_id; ///营业部ID（ATP3.1.9开始支持非必填）
    ATPRemarkType remark; ///备注 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqExtFundTransferOtherMsg();
};


/**
 *  @brief 融资融券直接还款请求
 */
struct TRADE_API ATPReqExtRepayOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段（指定合约偿还列表，ATP3.1.11开始支持；具体填写规则可参考引言中的接口约定） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPCurrencyType currency; ///货币种类（默认值 "CNY"）
    ATPAmtType repay_value; ///还款金额 N18(4) 
    ATPMarketIDType market_id; ///市场代码 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPRepayWayType repay_way; ///还款方式 
    
    ATPReqExtRepayOtherMsg();
};


/**
 *  @brief 最大可委托数查询结果
 */
struct TRADE_API ATPRspMaxOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPQtyType max_order_qty; ///最大可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType theoretical_order_qty; ///理论可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    ATPRspMaxOrderQueryResultMsg();
};


/**
 *  @brief 融资融券合约明细信息单元
 */
struct TRADE_API APIContractSpecificationsUnit
{
    ATPBranchIDType branch_id; ///营业部ID
    ATPDate8Type opening_date; ///开仓日期
    ATPOpeningSerialNumType opening_serial_num; ///开仓流水号
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPCreditIdentificationType credit_identification; ///融资融券合约信用标识
    ATPAmtType opening_amt; ///开仓金额N18(4)
    ATPAmtType trading_amt; ///成交金额N18(4)
    ATPQtyType opening_qty; ///开仓数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType opening_price; ///开仓价格N13(4)
    ATPAmtType repayment_amt; ///已偿还金额N18(4)
    ATPQtyType repayment_qty; ///已偿还数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPDate8Type expiration_date; ///到期日
    ATPInterestRateProductType interest_rate_product; ///利率产品
    ATPRollOverNumberType roll_over_number; ///已展期次数
    ATPDepositRatioType deposit_ration; ///保证金比例N3(4)
    ATPOpeningSerialNumType associate_serial_num; ///关联流水号
    ATPDate8Type associate_date; ///关联日期
    ATPAmtType unsettled_interest; ///未结利息N18(4)
    ATPAmtType unsettled_default_interest; ///未结罚息N18(4)
    ATPFirstUsePrivateAmountType first_use_private_amount; ///是否先用保底额度
    
    APIContractSpecificationsUnit();
};


/**
 *  @brief 融资融券合约明细查询结果
 */
struct TRADE_API ATPRspQueryContractSpecificationsResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIContractSpecificationsUnit> order_array; ///查询结果组合
    
    ATPRspQueryContractSpecificationsResultMsg();
};


/**
 *  @brief 融资融券信用额度信息单元
 */
struct TRADE_API APICreditAmountUnit
{
    ATPBranchIDType branch_id; ///营业部ID
    ATPAmtType financing_security_amount; ///授信总额度N18(4)
    ATPAmtType financing_amount; ///融资额度N18(4)
    ATPAmtType security_amount; ///融券额度N18(4)
    ATPAmtType financing_required_amount; ///融资占用额度N18(4)
    ATPAmtType security_required_amount; ///融券占用额度N18(4)
    
    APICreditAmountUnit();
};


/**
 *  @brief 融资融券信用额度查询结果
 */
struct TRADE_API ATPRspQueryCreditAmountResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数(单客户查询为1)
    std::vector<APICreditAmountUnit> order_array; ///查询结果组合
    
    ATPRspQueryCreditAmountResultMsg();
};


/**
 *  @brief 融资融券保证金可用查询结果
 */
struct TRADE_API ATPRspQueryMarginUseableResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPBranchIDType branch_id; ///营业部ID
    ATPAmtType margin_useable_balance; ///保证金可用余额N18(4)
    
    ATPRspQueryMarginUseableResultMsg();
};


/**
 *  @brief 融资融券资金负债查询结果
 */
struct TRADE_API ATPRspQueryFundLiabilityResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPBranchIDType branch_id; ///营业部ID
    ATPCurrencyType currency; ///货币种类
    ATPAmtType real_time_balance; ///实时余额N18(4)
    ATPAmtType init_balance; ///期初余额N18(4)
    ATPAmtType settle_unpay_interest; ///已结未付利息N18(4)
    ATPAmtType settle_unpay_penalty; ///已结未付罚息N18(4)
    ATPAmtType unpay_interest; ///未结利息N18(4)
    ATPAmtType unpay_penalty; ///未结罚息N18(4)
    ATPAmtType today_open; ///当日开仓N18(4)
    ATPAmtType today_repay; ///当日偿还N18(4)
    ATPAmtType crd_buy_required; ///融资保证金占用N18(4)
    ATPAmtType crd_sell_required; ///融券保证金占用N18(4)
    ATPAmtType crd_buy_un_trade; ///融资未成交金额N18(4)
    ATPAmtType crd_sell_un_trade; ///融券未成交金额N18(4)
    ATPAmtType temp_add; ///临时调增N18(4)
    ATPAmtType temp_sub; ///临时调减N18(4)
    ATPPartitionNoType te_partition_no; ///所属分区号
    ATPAmtType private_amount_occupied; ///保底额度占用N18(4)
    ATPAmtType public_amount_occupied; ///公用额度占用N18(4)
    ATPAmtType private_amount_frozen; ///保底额度冻结N18(4)
    ATPAmtType public_amount_frozen; ///公用额度冻结N18(4)
    ATPAmtType today_designate_repay_amt; ///当日指定偿还金额,ATP3.1.11开始支持
    
    ATPRspQueryFundLiabilityResultMsg();
};


/**
 *  @brief 融资融券股份查询结果信息单元
 */
struct TRADE_API APIShareLiabilityUnit
{
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPQtyType real_time_qty; ///实时数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType market_value; ///市值N18(4)
    ATPQtyType init_qty; ///期初数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType today_open; ///当日开仓N15(2)
    ATPQtyType today_repay; ///当日偿还N15(2)
    ATPQtyType today_leaves; ///当日余券N15(2)
    ATPAmtType margin_required; ///保证金占用N18(4)
    ATPQtyType temp_add; ///临时调增N15(2)
    ATPQtyType temp_sub; ///临时调减N15(2)
    ATPQtyType today_designate_repay_qty; ///当日指定偿还数量，ATP3.1.11开始支持
    
    APIShareLiabilityUnit();
};


/**
 *  @brief 融资融券股份负债查询结果
 */
struct TRADE_API ATPRspQueryShareLiabilityResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///合约持仓记录总数
    std::vector<APIShareLiabilityUnit> order_array; ///查询结果组合
    ATPPartitionNoType te_partition_no; ///所属分区号
    
    ATPRspQueryShareLiabilityResultMsg();
};


/**
 *  @brief 融资融券合约管理信息单元
 */
struct TRADE_API APIContractSpecificationsManagerUnit
{
    ATPBranchIDType branch_id; ///营业部ID
    ATPDate8Type opening_date; ///开仓日期
    ATPOpeningSerialNumType opening_serial_num; ///开仓流水号
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPCreditIdentificationType credit_identification; ///融资融券合约信用标识
    ATPAmtType opening_amt; ///开仓金额N18(4)
    ATPAmtType trading_amt; ///成交金额N18(4)
    ATPQtyType opening_qty; ///开仓数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType opening_price; ///开仓价格N13(4)
    ATPAmtType repayment_amt; ///已偿还金额N18(4)
    ATPQtyType repayment_qty; ///已偿还数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPDate8Type expiration_date; ///到期日
    ATPInterestRateProductType interest_rate_product; ///利率产品
    ATPRollOverNumberType roll_over_number; ///已展期次数
    ATPDepositRatioType deposit_ration; ///保证金比例N3(4)
    ATPOpeningSerialNumType associate_serial_num; ///关联流水号
    ATPDate8Type associate_date; ///关联日期
    ATPAmtType unsettled_interest; ///未结利息N18(4)
    ATPAmtType unsettled_default_interest; ///未结罚息N18(4)
    ATPCurrencyType currency; ///货币种类
    ATPAmtType total_principal_liability; ///总本金负债N18(4)
    ATPAmtType total_interest_liability; ///总利息负债N18(4)
    ATPAmtType repayment_interest; ///已偿还利息N18(4)
    ATPAmtType margin_required; ///保证金占用N18(4)
    ATPQtyType total_liability_qty; ///总负债数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_liability_amt; ///总负债金额N18(4)
    ATPFirstUsePrivateAmountType first_use_private_amount; ///是否先用保底额度
    
    APIContractSpecificationsManagerUnit();
};


/**
 *  @brief 融资融券合约管理查询结果
 */
struct TRADE_API ATPRspQueryContractSpecificationsManagerResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIContractSpecificationsManagerUnit> order_array; ///查询结果组合
    
    ATPRspQueryContractSpecificationsManagerResultMsg();
};


/**
 *  @brief 融资融券标的清单信息单元
 */
struct TRADE_API APIMarginUnderlyingUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    UnderlyingTypeType underlying_type; ///标的类型（多个权限之间逗号分隔：1-担保品；2-融券；3-融资）
    ATPCRDUnderlyingRateType translation_proportion; ///折算率N13(4)
    ATPCRDBusinessSwitchType business_switch; ///业务开关（多个开关之间逗号分隔：1-暂停提交；2-允许展期；3-集中度控制；4-高流通性证券）
    ATPDepositRatioType financing_guarantee_ratio; ///融资保证金比率N3(4)（例：508表示50.8%）
    ATPDepositRatioType security_guarantee_ratio; ///融券保证金比率N3(4)（例：508表示50.8%）
    ATPQtyType margin_qty; ///融券专户可用数量N15(2)
    
    APIMarginUnderlyingUnit();
};


/**
 *  @brief 融资融券标的清单查询结果
 */
struct TRADE_API ATPRspQueryMarginUnderlyingResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIMarginUnderlyingUnit> order_array; ///查询结果组合
    
    ATPRspQueryMarginUnderlyingResultMsg();
};


/**
 *  @brief 融资融券客户信用资料查询结果
 */
struct TRADE_API ATPRspQueryCreditInfoResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPBranchIDType branch_id; ///营业部ID
    ATPContractStatusType contrac_status; ///合同状态（字典项请咨询券商）
    ATPCreditRatingType credit_rating; ///信用级别（字典项请咨询券商）
    ATPCloseoutStatusType closeout_status; ///平仓状态（字典项请咨询券商）
    ATPInterestRateProductType interest_product; ///利率产品（字典项请咨询券商）
    ATPDepositRatioType financing_guarantee_ratio; ///融资保证金比例N3(4)
    ATPDepositRatioType security_guarantee_ratio; ///融券保证金比例N3(4)
    
    ATPRspQueryCreditInfoResultMsg();
};


/**
 *  @brief 融资融券资产信息查询结果
 */
struct TRADE_API ATPRspQueryFundAssetsInfoResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPBranchIDType branch_id; ///营业部ID
    ATPAmtType fund_balance; ///资金余额N18(4)
    ATPAmtType hold_market_value; ///持仓市值N18(4)
    ATPAmtType total_assets; ///总资产N18(4)
    ATPAmtType fund_debt; ///资金负债N18(4)
    ATPAmtType share_debt_market_value; ///股份负债市值N18(4)
    ATPAmtType total_debt; ///总负债N18(4)
    ATPAmtType net_assets; ///净资产N18(4)
    ATPAmtType margin_required; ///保证金占用N18(4)
    ATPMtnGuaranteeRatioType maintains_margin_ratio1; ///维持担保比例1 N7(4)
    ATPMtnGuaranteeRatioType maintains_margin_ratio2; ///维持担保比例2 N7(4)
    ATPAmtType unfilled_crd_sell_amt; ///融券卖出未成交金额N18(4)
    ATPAmtType unfilled_crd_buy_amt; ///融资买入未成交金额N18(4)
    ATPAmtType margin_pre_required; ///保证金预占用（未成交合约占用）N18(4)
    ATPAmtType withdrawable_amt; ///可取资金（考虑了提取线）N18(4)
    
    ATPRspQueryFundAssetsInfoResultMsg();
};


/**
 *  @brief 订单信息单元
 */
struct TRADE_API APIOrderUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向
    ATPOrdTypeType ord_type; ///订单类别
    ATPOrdStatusType ord_status; ///当前申报的状态，（对于撤单订单，填写被撤订单的当前状态，找不到原始订单时，固定填8 = Reject ）
    ATPTimeStampType transact_time; ///客户委托时间
    ATPPriceType order_price; ///委托价格N13(4)
    ATPPriceType exec_price; ///平均成交价格N13(4)
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_cl_ord_no; ///对于撤单订单，为原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no; 对于普通订单，取值为0
    ATPAmtType frozen_trade_value; ///冻结交易金额N18(4)
    ATPAmtType frozen_fee; ///冻结费用N18(4)
    ATPCurrencyType currency; ///货币种类
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPAmtType order_entrusted_amt; ///委托金额N18(4)
    ATPAmtType order_cum_transaction_amt; ///成交金额N18(4)
    ATPExecTypeType exec_type; ///执行类型（ATP3.1.8 开始支持）
    ATPSecurityTypeType security_type; ///证券类别（ATP3.1.8 开始支持）
    ATPQtyType canceled_qty; ///已撤单数量N15(2)（ATP3.1.9 开始支持）
    ATPOrderFlagType order_flag; ///订单标识（ATP3.1.9 开始支持）（返回默认值表示当前版本不支持）
    ATPParticularFlagsType particular_flags; ///特殊委托标识，右起第一个bit位是夜市委托标志位：0-正常委托、1-夜市委托（ATP3.2.0开始支持）
    
    APIOrderUnit();
};


/**
 *  @brief 订单查询结果
 */
struct TRADE_API ATPRspOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（ATP3.1.9 开始支持）
    std::vector<APIOrderUnit> order_array; ///查询结果组合
    
    ATPRspOrderQueryResultMsg();
};


/**
 *  @brief 批量订单查询结果
 */
struct TRADE_API ATPRspBatchOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPClOrdNOType parent_cl_ord_no; ///客户订单编号
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号，当 “LastIndex +1 == TotalNum”时，表示已经查到最后的记录
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOrderUnit> order_array; ///查询结果组合
    
    ATPRspBatchOrderQueryResultMsg();
};


/**
 *  @brief 增强资金查询消息
 */
struct TRADE_API ATPReqExtFundQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID （ATP3.1.7版本开始支持此字段非必填）
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPCurrencyType currency; ///货币种类 （ATP3.1.7起开始支持非必填，表示查所有）
    
    ATPReqExtFundQueryMsg();
};


/**
 *  @brief 资金查询结果
 */
struct TRADE_API APIExtFundUnit
{
    ATPAmtType leaves_value; ///资金余额N18(4)
    ATPAmtType init_leaves_value; ///日初资金金额N18(4)
    ATPAmtType available_t0; ///T+0可用资金N18(4)
    ATPAmtType available_t1; ///T+1可用资金N18(4)
    ATPAmtType available_t2; ///T+2可用资金N18(4)
    ATPAmtType available_t3; ///T+3可用资金N18(4)
    ATPAmtType available_tall; ///T+All可用资金N18(4)
    ATPAmtType frozen_all; ///当前所有冻结N18(4)
    ATPCurrencyType currency; ///货币种类
    
    
};


/**
 *  @brief 增强资金查询结果
 */
struct TRADE_API ATPRspExtFundQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPPartitionType te_partition_no; ///所属分区号
    std::vector<APIExtFundUnit> ext_query_fund_array; ///增强资金查询结果组合
    
    
};


/**
 *  @brief 成交信息单元
 */
struct TRADE_API APITradeOrderUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码（对于ETF实时申赎成交，上海市场：1-沪市资金代码、2-深市资金代码、3-港市资金代码、9-非沪深资金代码）
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向（对于ETF实时申赎成交，此字段为'D'=申购 'E'=赎回 'P'=实物申购 'R'=实物赎回）
    ATPOrdTypeType ord_type; ///取原订单的订单类型（对于撤单订单，取被撤订单的订单类型）
    ATPExecTypeType exec_type; ///当前订单执行类型（若为交易所确认撤单的委托，exec_type=4-Cancelled表示已撤销）
    ATPExecIDType exec_id; ///执行编号（对于交易所确认撤单的委托，exec_id为空）
    ATPClOrdNOType cl_ord_no; ///客户订单编号（原订单的客户订单编号，若此笔记录为撤单，是为撤单的原订单编号）
    ATPOrderIDType order_id; ///交易所订单编号（对于交易所确认撤单的委托，order_id为空）
    ATPClOrdIDType cl_ord_id; ///申报合同号（上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓）
    ATPTimeStampType transact_time; ///成交时间（对于交易所确认撤单的委托，取交易所确认撤单的回报时间）
    ATPPriceType last_px; ///成交数量N13(4)（对于交易所确认撤单的委托，取0；对于ETF实时申赎成交，取0）
    ATPQtyType last_qty; ///成交数量N15(2)（对于交易所确认撤单的委托，取每笔撤单的数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）；对于ETF实时申赎成交，上海市场当security_id为资金代码时取0）
    ATPAmtType total_value_traded; ///成交金额N18(4)（对于交易所确认撤单的委托，取0；对于ETF实时申赎成交，上海市场当security_id不为资金代码时取0，深圳市场取现金代替金额）
    ATPAmtType fee; ///成交费用N18(4)（对于交易所确认撤单的委托，取0；对于ETF实时申赎成交，取0）
    ATPCurrencyType currency; ///货币种类
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPSecurityTypeType security_type; ///证券类别（ATP3.1.8 开始支持）
    ATPQtyType order_qty; ///委托数量N15(2)（ATP3.1.10开始支持；对于ETF实时申赎成交，同last_qty）
    ATPParticularFlagsType particular_flags; ///特殊委托标识，右起第一个bit位是夜市委托标志位：0-正常委托、1-夜市委托（ATP3.2.0开始支持）
    
    APITradeOrderUnit();
};


/**
 *  @brief ETF成分股信息
 */
struct TRADE_API APIShareRecordETFArray
{
    ATPSecurityIDType security_id; ///证券代码，例外：1-沪市资金代码、2-深市资金代码、3-港市资金代码、9-非沪深资金代码（全额现金申赎）
    ATPMarketIDType market_id; ///市场代码，当security_id为沪市资金代码时为空
    ATPQtyType qty; ///成交数量或资金比例因子N15(2)，当security_id为沪市资金代码时为0
    ATPAmtType amt; ///成交金额N18(4)，对于沪市ETF申赎成交回报，security_id为资金代码时有值，否则为空；对于深市ETF，原逻辑不变。
    ATPPriceType price; ///成交价格N13(4)，对于沪市ETF申赎成交回报，此字段为空
    ATPETFTradeReportTypeType etf_trade_report_type; ///ETF成交回报类型
    ATPExecIDType exec_id; ///执行编号
    
    APIShareRecordETFArray();
};


/**
 *  @brief ETF申赎成交信息单元
 */
struct TRADE_API APIETFTradeOrderUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向
    ATPOrdTypeType ord_type; ///取原订单的订单类型，对于撤单订单，取被撤订单的订单类型
    ATPExecTypeType exec_type; ///当前订单执行类型,若为交易所确认撤单的exec_type=4-Cancelled表示已撤销
    ATPExecIDType exec_id; ///执行编号 ,对于交易所确认撤单的委托执行编号为空
    ATPClOrdNOType cl_ord_no; ///客户订单编号,原订单的客户订单编号，若此笔记录为撤单，则应该是撤单的原订单
    ATPOrderIDType order_id; ///交易所订单编号,交易所订单编号，对于交易所确认撤单的记录，order_id为空
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
    ATPTimeStampType transact_time; ///成交时间 ,成交时间，对于交易所确认撤单，取交易所确认撤单的回报时间
    ATPPriceType last_px; ///成交价格，对于交易所确认撤单，取0N13(4)
    ATPQtyType last_qty; ///成交数量 ,对于交易所确认撤单，取每笔撤单的数量（股票为股、基金为份、上海债券为手，其他为张）N15(2)
    ATPAmtType total_value_traded; ///成交金额，N18(4)对于交易所确认撤单的委托，取0
    ATPAmtType fee; ///成交费用 ,N18(4)对于交易所确认撤单的委托，取0
    ATPCurrencyType currency; ///货币种类
    ATPSecurityTypeType security_type; ///证券类别（ATP3.1.8 开始支持）
    std::vector<APIShareRecordETFArray> order_array; ///ETF成份股记录组
    
    APIETFTradeOrderUnit();
};


/**
 *  @brief 成交查询结果
 */
struct TRADE_API ATPRspTradeOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APITradeOrderUnit> order_array; ///查询结果组合
    
    ATPRspTradeOrderQueryResultMsg();
};


/**
 *  @brief EFT申赎成交查询结果
 */
struct TRADE_API ATPRspETFTradeOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIETFTradeOrderUnit> order_array; ///查询结果组合
    
    ATPRspETFTradeOrderQueryResultMsg();
};


/**
 *  @brief 资金查询结果
 */
struct TRADE_API ATPRspFundQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPAmtType leaves_value; ///资金余额N18(4)
    ATPAmtType init_leaves_value; ///日初资金金额N18(4)
    ATPAmtType available_t0; ///T+0可用资金N18(4)，对于两融类业务：包含昨日清算后融券冻结资金，不包含当日融券卖出冻结资金
    ATPAmtType available_t1; ///T+1可用资金N18(4)，对于两融类业务：包含融券卖出冻结资金（买高流通股票、买券还券时可全额使用，买非高流通股份、直接还款时可用资金为：T+1可用资金（available_t1）减去融券卖出冻结金额（credit_sell_frozen））
    ATPAmtType available_t2; ///T+2可用资金N18(4)
    ATPAmtType available_t3; ///T+3可用资金N18(4)
    ATPAmtType available_tall; ///T+All可用资金N18(4)
    ATPAmtType frozen_all; ///当前所有冻结N18(4)
    ATPPartitionType te_partition_no; ///所属分区号
    ATPAmtType credit_sell_frozen; ///融券卖出冻结金额N18(4)、买高流通股票、买券还券时可全额使用
    ATPAmtType credit_sell_occupied; ///融券卖出款占用N18(4)
    ATPAmtType credit_sell_pre_occupied; ///融券卖出款预占用N18(4)
    ATPAmtType init_credit_sell; ///日初融券卖出款N18(4)
    
    ATPRspFundQueryResultMsg();
};


/**
 *  @brief 股份信息单元
 */
struct TRADE_API APIShareUnit
{
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPQtyType init_qty; ///日初持仓量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）；对于两融类业务：不含日初融券买入量（init_crd_sell_buy_share_qty）
    ATPQtyType leaves_qty; ///剩余股份数量N15(2)（包含当日买入部分,拆分合并、申购赎回）（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType available_qty; ///可用股份数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType profit_loss; ///浮动盈亏N18(4)
    ATPAmtType market_value; ///市值N18(4)
    ATPAmtType cost_price; ///成本价N18(4)
    ATPCurrencyType currency; ///货币种类
    ATPQtyType init_crd_sell_buy_share_qty; ///日初融券买入量N15(2)；对于两融类业务:与日初持仓量(init_qty)相加才是实际的昨结算后股份持仓数量）
    ATPAmtType init_crd_sell_occupied_amt; ///日初融券占用金额N18(4)
    ATPQtyType cur_crd_sell_occupied_qty; ///当前融券买入量N15(2)
    ATPAmtType cur_crd_sell_occupied_amt; ///当前融券占用金额N18(4)
    ATPSecurityTypeType security_type; ///证券类别（ATP3.1.8 开始支持）
    ATPQtyType etf_redemption_qty; ///对于ETF基金表示可赎回数量，对于其他证券表示可用于ETF申购的数量（ATP3.2.1开始支持）
    
    APIShareUnit();
};


/**
 *  @brief 股份查询结果
 */
struct TRADE_API ATPRspShareQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIShareUnit> order_array; ///查询结果组合
    
    ATPRspShareQueryResultMsg();
};


/**
 *  @brief 科创板新股额度信息单元
 */
struct TRADE_API APIIPOQtyUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPQtyType subscription_quota; ///科创板申购额度（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    APIIPOQtyUnit();
};


/**
 *  @brief 新股额度信息单元
 */
struct TRADE_API APIIPOQtyExUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPQtyType subscription_quota; ///申购额度（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType tib_subscription_quota; ///科创板申购额度（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    APIIPOQtyExUnit();
};


/**
 *  @brief 新股申购额度查询结果
 */
struct TRADE_API ATPRspIPOQtyQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    std::vector<APIIPOQtyExUnit> order_array; ///查询结果组合
    
    ATPRspIPOQtyQueryResultMsg();
};


/**
 *  @brief 合约账户持仓信息单元
 */
struct TRADE_API APIContractUnit
{
    ATPSecurityIDType security_id; ///合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPQtyType buy_init_value; ///买开仓日初持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType buy_value; ///买开仓可用持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType buy_frozen_value; ///委托冻结
    ATPQtyType sell_init_value; ///卖开仓日初持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType sell_value; ///卖开仓可用持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType sell_frozen_value; ///卖开仓冻结量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType cover_init_value; ///备兑仓日初持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType cover_value; ///备兑仓可用持仓量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType cover_frozen_value; ///备兑仓冻结量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType accum_exercise_qty; ///普通行权申报数量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType combi_exercise_qty; ///合并行权申报数量，无意义（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPPositionSideType position_side; ///持仓方向（0-权利仓，1-义务仓，2-备兑仓）
    ATPAmtType init_buy_amt; ///日初占用买入额度N18(4)
    ATPQtyType init_total_position_value; ///日初持仓量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType init_position_value; ///日初可用持仓量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType init_combinationed_value; ///日初已组合数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType tatol_position_value; ///当前持仓量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType position_value; ///当前可用持仓量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType combinationed_value; ///已组合数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType temp_in; ///临时调增（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType temp_out; ///临时调减（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType open_position_value; ///开仓数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType close_position_value; ///平仓数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType exercise_qty; ///行权申报数量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType buy_amt; ///占用买入额度N18(4)
    ATPAmtType init_position_used_amt; ///日初合约买入成本N18(4)
    ATPAmtType position_used_amt; ///合约买入成本N18(4)
    ATPAmtType market_value; ///市值N18(4)
    ATPAmtType close_profit; ///平仓盈亏N18(4)
    ATPAmtType profit; ///浮动盈亏N18(4)
    ATPAmtType occupy_margin_value; ///普通义务仓占用保证金N18(4)
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPAmtType net_contract_qty; ///合约交易轧差数量
    ATPAmtType cover_used_qty; ///备兑股份数量
    ATPAmtType total_profit; ///总盈亏
    
    APIContractUnit();
};


/**
 *  @brief 合约账户持仓信息查询结果
 */
struct TRADE_API ATPRspAccountContractShareQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPAccountSubCodeType account_sub_code; ///账户子编码
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///合约持仓记录总数
    std::vector<APIContractUnit> contract_array; ///组合结果
    
    ATPRspAccountContractShareQueryResultMsg();
};


/**
 *  @brief 指定合约账户资金查询结果
 */
struct TRADE_API ATPRspAccountContractFundQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPAmtType init_total_amt; ///日初总资金N18(4)
    ATPAmtType init_available_amt; ///日初可用资金N18(4)
    ATPAmtType available_amt; ///可用资金N18(4)
    ATPAmtType frozen_amt; ///冻结资金N18(4)，无意义
    ATPAmtType init_margin; ///日初占用保证金N18(4)
    ATPAmtType init_exercise_assign; ///行权指派保证金N18(4)
    ATPAmtType init_exercise_frozen; ///行权冻结资金N18(4)
    ATPAmtType total_amt; ///余额N18(4)
    ATPAmtType today_in; ///当日入金N18(4)
    ATPAmtType today_out; ///当日出金N18(4)
    ATPAmtType temp_in; ///临时调增N18(4)
    ATPAmtType temp_out; ///临时调减N18(4)
    ATPAmtType royalties_in; ///保证金收入N18(4)
    ATPAmtType royalties_out; ///保证金支出N18(4)
    ATPAmtType fee; ///费用支出N18(4)
    ATPAmtType temp_frozen; ///临时冻结N18(4)
    ATPAmtType temp_subfrozen; ///临时解冻N18(4)
    ATPAmtType Unusual_frozen; ///异常冻结N18(4)
    ATPAmtType Unusual_subfrozen; ///异常解冻N18(4)
    ATPAmtType margin_frozen; ///占用保证金N18(4)
    ATPAmtType royalties_frozen; ///权利金委托冻结N18(4)
    ATPAmtType exercise_frozen; ///行权委托冻结N18(4)
    ATPAmtType exercise_out; ///行权申报支出N18(4)
    ATPAmtType fee_frozen; ///费用冻结资金N18(4)
    ATPAmtType close_profit; ///平仓盈亏N18(4)
    ATPAmtType profit; ///浮动盈亏N18(4)
    ATPAmtType can_out_amt; ///可取资金N18(4)
    ATPAmtType total_assets; ///资产总值
    ATPAmtType total_market_value; ///合约持仓市值
    ATPAmtType total_frozen_amt; ///总冻结金额
    ATPAmtType netting_trade_amt; ///当日交易轧差金额
    
    ATPRspAccountContractFundQueryResultMsg();
};


/**
 *  @brief 成交汇总信息单元
 */
struct TRADE_API APITradeCollectUnit
{
    ATPBusinessTypeType business_type; ///业务代码
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType buy_avg_price; ///买入均价N13(4)
    ATPQtyType buy_cum_qty; ///买入累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType buy_total_value_traded; ///买入成交总金额，精确到3位小数N18(4)
    ATPPriceType sell_avg_price; ///卖出均价N13(4)
    ATPQtyType sell_cum_qty; ///卖出累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType sell_total_value_traded; ///卖出成交金额，精确到3位小数N18(4)
    ATPAmtType total_value_traded; ///买卖合计成交金额，精确到3位小数N18(4)
    ATPCurrencyType currency; ///货币种类
    ATPSecurityTypeType security_type; ///证券类别（ATP3.1.8 开始支持）
    
    APITradeCollectUnit();
};


/**
 *  @brief 成交汇总查询结果
 */
struct TRADE_API ATPRspCollectQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPPriceType all_buy_avg_price; ///总买入均价N13(4)
    ATPQtyType all_buy_cum_qty; ///总买入累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType all_buy_total_value_traded; ///总买入成交总金额，精确到3位小数N18(4)
    ATPPriceType all_sell_avg_price; ///总卖出均价N13(4)
    ATPQtyType all_sell_cum_qty; ///总卖出累计总量（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPAmtType all_sell_total_value_traded; ///总卖出成交金额，精确到3位小数N18(4)
    ATPAmtType all_total_value_traded; ///总买卖合计成交金额，精确到3位小数N18(4)
    std::vector<APITradeCollectUnit> trade_collect_array; ///成交汇总查询结果组合
    
    ATPRspCollectQueryResultMsg();
};


/**
 *  @brief 科创板股票新股发行申购额度查询结果
 */
struct TRADE_API ATPRspTIBIPOQtyQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    std::vector<APIIPOQtyUnit> ipo_qty_array; ///成交汇总查询结果组合
    
    ATPRspTIBIPOQtyQueryResultMsg();
};


/**
 *  @brief 期权最大可委托数查询结果消息
 */
struct TRADE_API ATPRspCoverMaxOrderQtyResultQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPQtyType max_order_qty; ///最大可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType theoretical_order_qty; ///理论可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    ATPRspCoverMaxOrderQtyResultQueryResultMsg();
};


/**
 *  @brief 分区索引信息单元
 */
struct TRADE_API APINoPartitionUnit
{
    ATPPartitionType partition; ///分区号 
    ATPAgwIndexType index; ///回报记录号 
    
    APINoPartitionUnit();
};


/**
 *  @brief 业务层回报同步消息
 */
struct TRADE_API ATPReqSyncBizOtherMsg
{
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    std::vector<APINoPartitionUnit> no_parition_array; ///分区记录号信息
    
    ATPReqSyncBizOtherMsg();
};


/**
 *  @brief 同步客户回报结束消息
 */
struct TRADE_API ATPRspSyncBizFinishOtherMsg
{
    ATPSeqNumType client_seq_id; ///用户系统消息序
    ATPReasonCodeType reason_code; ///结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-4 回报同步结束消息结果错误码类型，即可查询到错误描述信息
    ATPStringType reason_message; ///错误原因
    
    ATPRspSyncBizFinishOtherMsg();
};


/**
 *  @brief 股东账户信息单元
 */
struct TRADE_API APIMsgCustLoginRespAccountUnit
{
    ATPAccountIDType account_id; ///证券账户ID
    ATPMarketIDType market_id; ///市场代码
    ATPAccountRole account_role; ///所属账户角色
    ATPPartitionType partition_no; ///所属分区号（默认值为0，ATP3.1.9开始支持，返回默认值表示该版本不支持）
    
    APIMsgCustLoginRespAccountUnit();
};


/**
 *  @brief 客户号登录响应资金账户信息单元
 */
struct TRADE_API APIMsgCustLoginRespFundAccountUnit
{
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    std::vector<APIMsgCustLoginRespAccountUnit> account_array; ///登入应答证券账户重复组
    
    APIMsgCustLoginRespFundAccountUnit();
};


/**
 *  @brief 账户登录应答消息
 */
struct TRADE_API ATPRspCustLoginRespOtherMsg
{
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPCustIDType cust_id; ///客户号ID
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPTimeStampType transact_time; ///处理时间
    ATPPermissonErrorCodeType permisson_error_code; ///错误码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-2客户登录错误码类型，即可查询到错误描述信息
    std::vector<APIMsgCustLoginRespFundAccountUnit> fund_account_array; ///登入应答资金账户重复组
    ATPBoolType is_multi_partitions; ///是否为一户两地账户
    ATPMultiChannelResultType multi_channel_result; ///回报消息订阅结果
    
    ATPRspCustLoginRespOtherMsg();
};


/**
 *  @brief 账户登出应答消息
 */
struct TRADE_API ATPRspCustLogoutRespOtherMsg
{
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPCustIDType cust_id; ///客户号ID
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPTimeType time; ///处理登出时间
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPermissonErrorCodeType permisson_error_code; ///错误码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-2客户登录错误码类型，即可查询到错误描述信息
    
    ATPRspCustLogoutRespOtherMsg();
};


/**
 *  @brief 资金划拨(转入、转出)应答消息
 */
struct TRADE_API ATPRspExtFundTransferResultOtherMsg
{
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPCustIDType cust_id; ///客户号ID
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPStringType reject_desc; ///拒绝原因描述
    ATPTimeStampType transact_time; ///回报时间
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPCurrencyType currency; ///货币种类
    ATPAmtType init_amt; ///日初可用余额N18(4)
    ATPAmtType order_frozen; ///委托预冻结N18(4)
    ATPAmtType buy_trade; ///买入成交N18(4)
    ATPAmtType sell_trade; ///卖出成交N18(4)
    ATPAmtType unusual_frozen; ///异常冻结N18(4)
    ATPAmtType unusual_frozen_cancel; ///异常冻结取消N18(4)
    ATPAmtType fee_frozen; ///冻结费用N18(4)
    ATPAmtType fee_trade; ///成交费用N18(4)
    ATPAmtType today_in; ///当日入金N18(4)
    ATPAmtType today_out; ///当日出金N18(4)
    ATPAmtType temp_add; ///临时调增N18(4)
    ATPAmtType temp_sub; ///临时调减N18(4)
    ATPAmtType temp_frozen; ///临时冻结N18(4)
    ATPAmtType temp_frozen_cancel; ///临时冻结取消N18(4)
    ATPAmtType pre_balance; ///调整前资金余额N18(4)
    ATPAmtType pre_available_t0; ///调整前T+0可用资金N18(4)
    ATPAmtType pre_on_the_way_t1; ///调整前T+1在途可用资金N18(4)
    ATPAmtType pre_on_the_way_t2; ///调整前T+2在途可用资金N18(4)
    ATPAmtType pre_on_the_way_t3; ///调整前T+3在途可用资金N18(4)
    ATPAmtType transfer_value; ///转移金额N18(4)
    
    ATPRspExtFundTransferResultOtherMsg();
};


/**
 *  @brief 融资融券直接还款应答消息
 */
struct TRADE_API ATPRspExtRepayResultOtherMsg
{
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPTimeStampType transact_time; ///回报时间
    
    ATPRspExtRepayResultOtherMsg();
};


/**
 *  @brief 外部业务拒绝消息
 */
struct TRADE_API ATPRspBizRejectionOtherMsg
{
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPTimeStampType transact_time; ///回报时间
    ATPMsgTypeType api_msg_type; ///被拒绝消息的消息类型（该字段忽略）
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPBusinessRejectTextType business_reject_text; ///拒绝原因描述
    
    ATPRspBizRejectionOtherMsg();
};


/**
 *  @brief 大宗盘后定价委托消息
 */
struct TRADE_API ATPReqAfterHoursPricingOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) （主板市价委托填0，科创板股票市价委托必须输入保护价）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPricePropertyType price_property; ///价格类型 
    
    ATPReqAfterHoursPricingOrderMsg();
};


/**
 *  @brief 大宗意向申报委托消息
 */
struct TRADE_API ATPReqBlockTradeIntentionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPTrdSubTypeType trd_sub_type; ///是否受限 0：非受限，1:受限(仅上海)
    ATPContactorType contactor; ///联系人 
    ATPContactInfoType contact_info; ///联系人信息 
    
    ATPReqBlockTradeIntentionOrderMsg();
};


/**
 *  @brief 大宗成交申报委托消息
 */
struct TRADE_API ATPReqBlockTradeTransactionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '1'：买，'2'：卖
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPConfirmIDType confirm_id; ///约定号 
    ATPTrdSubTypeType trd_sub_type; ///是否受限 0：非受限股份； 1：受限股份
    ATPPBUIDType counter_party_pbu_id; ///对手方PBUID 
    
    ATPReqBlockTradeTransactionOrderMsg();
};


/**
 *  @brief 大宗定价申报委托消息
 */
struct TRADE_API ATPReqBlockTradePricingOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 (仅支持深圳)
    ATPSideType side; ///买卖方向 '1'：买，'2'：卖
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPricingType pricing; ///是否协议交易定价 1=协议交易定价申报(约定号不用填) 2=协议交易定价成交申报(约定号必填且不可撤单）
    ATPConfirmIDType confirm_id; ///约定号 定价成交申报时必填
    
    ATPReqBlockTradePricingOrderMsg();
};


/**
 *  @brief 大宗交易撤单消息
 */
struct TRADE_API ATPReqBlockTradeCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cl_ord_no; ///客户订单编号 ，指示被撤消订单的cl_ord_no
    
    ATPReqBlockTradeCancelOrderMsg();
};


/**
 *  @brief 批量订单撤单消息
 */
struct TRADE_API ATPReqCancelBatchOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType parent_cl_ord_no; ///原始交易客户方（券商）订单编号，指被撤消订单的cl_ord_no（非算法母单编号）
    
    ATPReqCancelBatchOrderMsg();
};


/**
 *  @brief 大宗盘后定价业务成交回报
 */
struct TRADE_API ATPRspAfterHoursPricingTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPPricePropertyType price_property; ///价格类型
    ATPCashMarginType cash_margin; ///信用标识
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspAfterHoursPricingTradeERMsg();
};


/**
 *  @brief 大宗成交申报委托业务成交回报
 */
struct TRADE_API ATPRspBlockTradeTransactionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPPBUIDType counter_party_pbu_id; ///对手方PBUID
    ATPAccountIDType counter_party_account_id; ///对手方股东账户
    ATPTrdSubTypeType trd_sub_type; ///是否受限
    ATPConfirmIDType confirm_id; ///约定号
    ATPCashMarginType cash_margin; ///信用标识
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspBlockTradeTransactionTradeERMsg();
};


/**
 *  @brief 大宗定价申报委托业务成交回报
 */
struct TRADE_API ATPRspBlockTradePricingTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓；
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPExtraData extra_data; ///扩展字段
    ATPPricingType pricing; ///是否协议交易定价
    ATPConfirmIDType confirm_id; ///约定号
    ATPCashMarginType cash_margin; ///信用标识
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspBlockTradePricingTradeERMsg();
};


/**
 *  @brief 高管额度划转业务委托消息
 */
struct TRADE_API ATPReqReduceHoldingTransferOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPQtyType order_qty; ///申报数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPAccountIDType counter_party_account_id; ///对手方股东账户 
    ATPPBUIDType counter_party_pbu_id; ///对手方PBUID 
    
    ATPReqReduceHoldingTransferOrderMsg();
};


/**
 *  @brief 高管额度划转撤单消息
 */
struct TRADE_API ATPReqReduceHoldingTransferCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cl_ord_no; ///客户订单编号 ，指示被撤消订单的cl_ord_no
    
    ATPReqReduceHoldingTransferCancelOrderMsg();
};


/**
 *  @brief 外部期权资金账户资金划拨请求
 */
struct TRADE_API ATPReqExtOptionFundTransfeOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPFundAccountIDType account_id; ///证券账户ID 
    ATPMarketIDType market_id; ///市场代码 
    ATPCurrencyType currency; ///货币种类 
    ATPTransferWayType transfer_way; ///划拨方式 ：1-转入；2-转出；4-可划入资金查询
    ATPAmtType transfer_value; ///划拨金额 N18(4) 
    ATPRemarkType remark; ///备注 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqExtOptionFundTransfeOtherMsg();
};


/**
 *  @brief 外部期权资金账户资金划拨消息应答
 */
struct TRADE_API ATPRspOptionFundTransferResultOtherMsg
{
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPBusinessRejectTextType reject_desc; ///拒绝原因描述
    ATPTimeType transact_time; ///回报时间
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPFundAccountIDType account_id; ///证券账户ID
    ATPMarketIDType market_id; ///市场代码
    ATPCurrencyType currency; ///货币种类
    ATPAmtType init_total_amt; ///日初总资金N18(4)
    ATPAmtType init_available_amt; ///日初可用资金N18(4)
    ATPAmtType init_margin; ///日初占用保证金N18(4)
    ATPAmtType init_exercise_assign; ///行权指派保证金N18(4)
    ATPAmtType init_exercise_frozen; ///行权冻结资金N18(4)
    ATPAmtType total_amt; ///余额N18(4)
    ATPAmtType available_amt; ///可用余额N18(4)
    ATPAmtType today_in; ///当日入金N18(4)
    ATPAmtType today_out; ///当日出金N18(4)
    ATPAmtType temp_in; ///临时调增N18(4)
    ATPAmtType temp_out; ///临时调减N18(4)
    ATPAmtType royalties_in; ///保证金收入N18(4)
    ATPAmtType royalties_out; ///保证金支出N18(4)
    ATPAmtType fee; ///费用支出N18(4)
    ATPAmtType temp_frozen; ///临时冻结N18(4)
    ATPAmtType temp_subfrozen; ///临时解冻N18(4)
    ATPAmtType Unusual_frozen; ///异常冻结N18(4)
    ATPAmtType Unusual_subfrozen; ///异常解冻N18(4)
    ATPAmtType margin_frozen; ///占用保证金N18(4)
    ATPAmtType royalties_frozen; ///权利金委托冻结N18(4)
    ATPAmtType exercise_frozen; ///行权委托冻结N18(4)
    ATPAmtType exercise_out; ///行权申报支出N18(4)
    ATPAmtType fee_frozen; ///费用冻结资金N18(4)
    ATPAmtType pre_balance; ///调整前资金余额N18(4)
    ATPAmtType value; ///资金发生金额N18(4)
    ATPTransferWayType transfer_way; ///划拨方式：1-转入；2-转出；4-可划入资金查询
    ATPAmtType can_transfer_in_amt; ///可划入资金，仅当划拨方式等于4时有效N18(4)
    ATPUserInfoType user_info; ///用户私有信息
    ATPSeqNumType serial_no; ///外部系统流水号
    ATPExtraData extra_data; ///扩展字段
    
    ATPRspOptionFundTransferResultOtherMsg();
};


/**
 *  @brief 行权预估盈亏查询消息
 */
struct TRADE_API ATPReqExtQueryExerciseProfitLossMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPAccountIDType account_sub_code; ///账户子编码（已弃用）
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType security_id; ///证券代码 
    ATPQtyType order_qty; ///行权申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    
    ATPReqExtQueryExerciseProfitLossMsg();
};


/**
 *  @brief 行权预估盈亏查询消息结果
 */
struct TRADE_API ATPRspExtQueryResultExerciseProfitLossMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPAccountSubCodeType account_sub_code; ///账户子编码
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPQtyType order_qty; ///行权申报数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType exercise_profit_loss; ///行权预估盈亏N18(4)
    ATPAmtType exercise_contract_buy_cost; ///行权合约买入成本N18(4)
    ATPExtraData extra_data; ///扩展字段
    
    ATPRspExtQueryResultExerciseProfitLossMsg();
};


/**
 *  @brief 港股通业务委托消息
 */
struct TRADE_API ATPReqHKStocksThroughAuctionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///委托价格N13(4) 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPriceType stop_px; ///止损价N13(4) 
    ATPOrdTypeType ord_type; ///订单类型 
    ATPQtyType min_qty; ///最低成交数量 N15(2)（已弃用）
    ATPMaxPriceLevelsType max_price_levels; ///最多成交价位数（已弃用）
    ATPTimeInForceType time_in_force; ///订单有效时间类型（已弃用）
    ATPLotTypeType lot_type; ///订单数量类型 ，'1' = 零股订单，'2' = 整手订单
    
    ATPReqHKStocksThroughAuctionOrderMsg();
};


/**
 *  @brief 港股通投票业务委托消息
 */
struct TRADE_API ATPReqHKVotingOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 '1': 申报 'Q'：查询
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPAnnouncementNumberType announcement_number; ///公告编号 
    ATPProxyFormNumberType proxy_form_number; ///议案编号 
    ATPAffirmativeVotesType affirmative_votes; ///赞成数量 
    ATPNegativeVotesType negative_votes; ///反对数量 
    ATPAbstentionVotesType abstention_votes; ///弃权数量 
    
    ATPReqHKVotingOrderMsg();
};


/**
 *  @brief 港股通转托管业务委托消息
 */
struct TRADE_API ATPReqHKDesignationTransferOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 'J'=DesignationTransfer，转托管 'K'=RevertDesignationTransfer，反向转托管
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPClOrdNOType orig_cl_ord_no; ///原委托的订单编号 
    ATPDesignationTransTypeType designation_trans_type; ///转托管注册类型 
    ATPEntitlementNumberType entitlement_number; ///权益编号 
    ATPPBUIDType transferee_pbu_id; ///转托管，转入交易单元代码；反向转托管，转出交易单元 
    
    ATPReqHKDesignationTransferOrderMsg();
};


/**
 *  @brief 港股通公司行为业务委托消息
 */
struct TRADE_API ATPReqHKCorporateActionOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPQtyType order_qty; ///申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPCorporateActionIDType corporate_action_id; ///公司行为代码 
    
    ATPReqHKCorporateActionOrderMsg();
};


/**
 *  @brief 港股通公司行为公共撤单消息
 */
struct TRADE_API ATPReqHKCorporateActionCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPClOrdNOType orig_cli_ord_no; ///原委托的订单编号 
    
    ATPReqHKCorporateActionCancelOrderMsg();
};


/**
 *  @brief 港股通业务成交回报
 */
struct TRADE_API ATPRspHKStocksThroughAuctionTradeERMsg
{
    ATPPartitionType partition; ///分区号
    ATPIndexType index; ///回报记录号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPSecurityIDType security_id; ///证券代码/合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType ord_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPPriceType price; ///委托价格N13(4)（上交所市价转限价申报时，这里填写转为限价订单的价格，其他订单填0）
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPSideType side; ///买卖方向 '1'=买，'2'=卖
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号 上交所：以QP1开头，表示为交易所保证金强制平仓；以CV1开头，表示为交易所备兑强制平仓
    ATPExecIDType exec_id; ///成交编号
    ATPPriceType last_px; ///成交价格N13(4)
    ATPQtyType last_qty; ///成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType total_value_traded; ///成交金额N18(4)
    ATPAmtType fee; ///费用N18(4)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPAmtType cny_net_total_value_trade; ///成交金额，人民币N18(4)
    ATPAmtType cny_fee; ///成交费用，人民币N18(4)
    ATPClOrdNOType orig_cl_ord_no; ///原委托的客户订单编号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    
    ATPRspHKStocksThroughAuctionTradeERMsg();
};


/**
 *  @brief 期权普通与备兑互转订单
 */
struct TRADE_API ATPReqOptionCoveredTransUncoveredOrderMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID 
    ATPSideType side; ///买卖方向 买卖方向，备兑转普通填1，普通转备兑填2
    ATPPriceType price; ///委托价格N13(4) 
    ATPQtyType qty; ///委托数量 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPAccountSubCodeType account_sub_code; ///账户子编码（已弃用）
    
    ATPReqOptionCoveredTransUncoveredOrderMsg();
};


/**
 *  @brief 合并行权最大可委托数查询消息
 */
struct TRADE_API ATPReqExtQueryCombinExerciseMaxOrderQtyMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    std::vector<APIExerciseLegType> no_legs_array; ///行权多腿信息组合 
    
    ATPReqExtQueryCombinExerciseMaxOrderQtyMsg();
};


/**
 *  @brief 合并行权最大可委托数查询结果
 */
struct TRADE_API ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPQtyType max_order_qty; ///最大可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    ATPQtyType theoretical_order_qty; ///理论可委托数（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）N15(2)
    
    ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg();
};


/**
 *  @brief 合并行权预估盈亏查询消息
 */
struct TRADE_API ATPReqExtQueryCombinExerciseProfitLossMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 
    std::vector<APIExerciseLegType> no_legs_array; ///行权多腿信息组合 
    ATPQtyType order_qty; ///行权申报数量N15(2) （股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    
    ATPReqExtQueryCombinExerciseProfitLossMsg();
};


/**
 *  @brief 合并行权预估盈亏查询结果
 */
struct TRADE_API ATPRspExtQueryResultCombinExerciseProfitLossMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPMarketIDType market_id; ///市场代码
    std::vector<APIExerciseProfitLossLegType> no_legs_array; ///行权盈亏信息组合
    ATPQtyType order_qty; ///行权申报数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPAmtType combin_exercise_profit_loss; ///合并行权盈亏N18(4)
    ATPAmtType combin_exercise_contract_buy_cost; ///合并行权买入成本N18(4)
    
    ATPRspExtQueryResultCombinExerciseProfitLossMsg();
};


/**
 *  @brief 期权组合策略最大可委托数查询消息
 */
struct TRADE_API ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPMarketIDType market_id; ///市场代码 
    ATPSecurityIDType strategy_id; ///策略编码 
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码 
    ATPSideType side; ///买卖方向 
    ATPLengthType no_legs; ///成份合约个数 
    std::vector<APINoLegsOrderUnitType> no_legs_array; ///单腿信息组 
    
    ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg();
};


/**
 *  @brief 按证券账户划转资金请求消息
 */
struct TRADE_API ATPReqExternalInsTETransFundReqMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType fund_out_account_id; ///资金转出账户ID 
    ATPMarketIDType fund_out_market_id; ///资金转出市场代码 
    ATPAccountIDType fund_in_account_id; ///资金转入账户ID 
    ATPMarketIDType fund_in_market_id; ///资金转入市场代码 
    ATPAmtType value; ///划拨金额 N18(4)
    ATPCurrencyType currency; ///货币种类 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    
    ATPReqExternalInsTETransFundReqMsg();
};


/**
 *  @brief 按证券账户划转资金请求消息
 */
struct TRADE_API ATPReqOptionExternalInsTETransFundReqMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType fund_out_account_id; ///资金转出账户ID 
    ATPMarketIDType fund_out_market_id; ///资金转出市场代码 
    ATPAccountIDType fund_in_account_id; ///资金转入账户ID 
    ATPMarketIDType fund_in_market_id; ///资金转入市场代码 
    ATPAmtType value; ///划拨金额 N18(4)
    ATPCurrencyType currency; ///货币种类 
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    
    ATPReqOptionExternalInsTETransFundReqMsg();
};


/**
 *  @brief 按证券账户划转资金响应消息
 */
struct TRADE_API ATPRspExternalInsTETransFundRespMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ExternalInsErrorCodeType external_ins_error_code; ///外部指令错误代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-7 外部指令拒绝原因错误码类型，即可查询到错误描述信息
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    
    ATPRspExternalInsTETransFundRespMsg();
};


/**
 *  @brief 期权按证券账户划转资金响应消息
 */
struct TRADE_API ATPRspOptionExternalInsTETransFundRespMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ExternalInsErrorCodeType external_ins_error_code; ///外部指令错误代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-7 外部指令拒绝原因错误码类型，即可查询到错误描述信息
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    
    ATPRspOptionExternalInsTETransFundRespMsg();
};


/**
 *  @brief 客户密码修改请求
 */
struct TRADE_API ATPReqCustPasswdModifyMsg
{
    ATPExtraData extra_data; ///扩展字段 
    ATPCustIDType cust_id; ///客户号ID （cust_id、fund_account_id+branch_id两者必选填其一）
    ATPFundAccountIDType fund_account_id; ///资金账户ID （cust_id、fund_account_id+branch_id两者必选填其一）
    ATPBranchIDType branch_id; ///营业部ID （cust_id、fund_account_id+branch_id两者必选填其一）
    ATPPasswordType old_password; ///原密码 
    ATPPasswordType password; ///新密码 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    
    ATPReqCustPasswdModifyMsg();
};


/**
 *  @brief 客户密码修改应答
 */
struct TRADE_API ATPRspCustPasswdModifyResultMsg
{
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPStringType reject_desc; ///拒绝原因描述
    ATPTimeStampType transact_time; ///回报时间
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPUserInfoType user_info; ///用户私有信息
    
    ATPRspCustPasswdModifyResultMsg();
};


/**
 *  @brief 垫券还券业务订单
 */
struct TRADE_API ATPReqCouponSwapOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2)（张） 
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPOrdTypeType order_type; ///订单类型 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPBUIDType lender_pbu; ///出借券交易单元
    ATPAccountIDType lender_account_id; ///出借券证券账户
    
    ATPReqCouponSwapOrderMsg();
};


/**
 *  @brief 待清偿委托业务订单
 */
struct TRADE_API ATPReqDeductionToBePaidOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码) 
    ATPMarketIDType market_id; ///市场代码 
    ATPSideType side; ///买卖方向 
    ATPQtyType order_qty; ///申报数量N15(2)（张） 
    ATPPriceType price; ///委托价格N13(4) （市价委托填0）
    ATPOrdTypeType order_type; ///订单类型 
    ATPEnforceFlagType enforce_flag; ///风险强制标识（已弃用）
    ATPPBUIDType deduction_pbu; ///待清偿划扣证券的交易单元 
    ATPAccountIDType deduction_account_id; ///待清偿划扣证券的证券账户 
    ATPAccountTypeType deduction_account_type; ///待清偿划扣账户账户类型 
    
    ATPReqDeductionToBePaidOrderMsg();
};


/**
 *  @brief 标的证券备兑占用查询消息
 */
struct TRADE_API ATPReqExternalQueryLockedAccountContractQtyMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码 
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExternalQueryLockedAccountContractQtyMsg();
};


/**
 *  @brief 锁定委托业务订单结果
 */
struct TRADE_API ATPRspExternalQueryLockedAccountContractQtyMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPOrderWayType order_way; ///委托方式
    ATPUserInfoType user_info; ///用户私有信息
    ATPPasswordType password; ///客户密码 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPAccountSubCodeType account_sub_code; ///账户子编码
    ATPTotalNumType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///合约持仓记录总数
    
    ATPRspExternalQueryLockedAccountContractQtyMsg();
};


/**
 *  @brief 客户保证金风险信息查询
 */
struct TRADE_API ATPReqExternalQueryOptionMarginRiskInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID
    
    ATPReqExternalQueryOptionMarginRiskInfoMsg();
};


/**
 *  @brief 客户保证金风险信息查询结果
 */
struct TRADE_API ATPRspExternalQueryOptionMarginRiskInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPRiskRatio_def risk_level; ///风险等级
    ATPRiskRatio_def risk_value0; ///实时风险值0
    ATPRiskRatio_def risk_value1; ///实时风险值1
    ATPRiskRatio_def risk_value2; ///实时风险值2
    ATPRiskRatio_def min_risk_value0; ///当日最低风险值0
    ATPRiskRatio_def min_risk_value1; ///当日最低风险值1
    ATPRiskRatio_def min_risk_value2; ///当日最低风险值2
    ATPRiskRatio_def last_risk_value1; ///昨日风险值1
    ATPRiskRatio_def last_risk_value2; ///昨日风险值2
    ATPAmtType exchange_margin_amt; ///交易所保证金
    ATPAmtType member_margin_amt; ///券商保证金
    ATPAmtType today_in_amt; ///当日入金
    ATPAmtType risk_margin_amt0; ///对冲后券商实时保证金（含未成交）
    ATPAmtType risk_margin_amt1; ///对冲后券商实时保证金（不含未成交）
    ATPAmtType risk_margin_amt2; ///对冲后交易所实时保证金
    
    ATPRspExternalQueryOptionMarginRiskInfoMsg();
};


/**
 *  @brief 期权基础信息查询消息
 */
struct TRADE_API ATPReqExternalQueryOptionSecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID(ATP3.1.8开始该字段非必填) 
    ATPFundAccountIDType fund_account_id; ///资金账户ID (ATP3.1.8开始该字段非必填)
    ATPAccountIDType account_id; ///证券账户ID (ATP3.1.8开始该字段非必填)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码)（填空表示查所有）
    ATPMarketIDType market_id; ///市场代码（填0表示查所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExternalQueryOptionSecurityInfoMsg();
};


/**
 *  @brief 期权标的证券基础信息查询消息
 */
struct TRADE_API ATPReqExternalQueryOptionUnderlyingSecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID (ATP3.1.8开始该字段非必填)
    ATPFundAccountIDType fund_account_id; ///资金账户ID(ATP3.1.8开始该字段非必填) 
    ATPAccountIDType account_id; ///证券账户ID(ATP3.1.8开始该字段非必填) 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码)（填0表示查所有）
    ATPMarketIDType market_id; ///市场代码（填0表示查所有）
    ATPReportIndexType query_index; ///起始查询索引号（默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量（默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExternalQueryOptionUnderlyingSecurityInfoMsg();
};


/**
 *  @brief 期权合约基础信息查询结果组合
 */
struct TRADE_API APIOptionSecurityInfoUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///合约编码
    ATPSecuritySymbolType contract_symbol; ///期权合约简称
    ATPSecurityIDType underlying_security; ///标的证券代码
    ATPSecuritySymbolType underlying_symbol; ///基础证券证券名称
    ATPSecurityTypeType underlying_security_type; ///标的证券类别
    ATPCallOrPutType call_or_put; ///认购认沽
    ATPPriceType exercise_price; ///期权行权价
    APTContractUnitType contract_unit; ///合约单位
    ATPDate8Type last_trade_day; ///最后交易日
    ATPDate8Type exercise_begin_date; ///期权行权日
    ATPDate8Type delivery_day; ///行权交割日
    ATPQtyType contract_position; ///当前合约未平仓数
    ATPPriceType prev_clearing_price; ///合约前结算价
    ATPPriceType underlying_close_px; ///标的证券前收盘
    ATPAmtType sell_open_margin; ///今卖开每张单位保证金
    ATPAmtType last_sell_open_margin; ///昨卖开每张单位保证金
    ATPMarginParamType param1; ///保证金比例计算参数一
    ATPMarginParamType param2; ///保证金比例计算参数二
    ATPPriceType price_upper_limit; ///涨幅上限价格
    ATPPriceType price_lower_limit; ///跌幅下限价格
    ATPQtyType buy_qty_unit; ///买数量单位
    ATPQtyType buy_qty_lower_limit; ///单笔限价申报下限
    ATPQtyType buy_qty_upper_limit; ///单笔限价申报上限
    ATPQtyType market_order_buy_qty_lower_limit; ///单笔市价申报下限
    ATPQtyType market_order_buy_qty_upper_limit; ///单笔市价申报上限
    ATPPriceType price_tick; ///价格档位
    
    APIOptionSecurityInfoUnit();
};


/**
 *  @brief 期权合约基础信息查询结果
 */
struct TRADE_API ATPRspExternalQueryOptionSecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOptionSecurityInfoUnit> order_array; ///查询结果组合
    
    ATPRspExternalQueryOptionSecurityInfoMsg();
};


/**
 *  @brief 期权标的证券基础信息查询结果组合
 */
struct TRADE_API APIOptionUnderlyingSecurityInfoUnit
{
    ATPSecurityIDType security_id; ///合约编码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPSecurityTypeType security_type; ///证券类别
    ATPMarketIDType market_id; ///市场代码
    ATPListDateType list_date; ///挂牌日期
    ATPCurrencyType currency; ///货币种类
    ATPQtyType out_standing_share; ///发行总量
    ATPPriceType par_value; ///面值
    ATPQtyType public_float_share_quantity; ///流通量
    ATPBoolType day_trading; ///是否支持回转交易
    ATPPriceType prev_close_price; ///昨收盘价
    
    APIOptionUnderlyingSecurityInfoUnit();
};


/**
 *  @brief 期权标的证券基础信息查询结果
 */
struct TRADE_API ATPRspExternalQueryOptionUnderlyingSecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOptionUnderlyingSecurityInfoUnit> order_array; ///查询结果组合
    
    ATPRspExternalQueryOptionUnderlyingSecurityInfoMsg();
};


/**
 *  @brief 期权五档行情查询消息
 */
struct TRADE_API ATPReqExternalQueryOptionMdSnapShotArrayInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID(ATP3.1.8开始该字段非必填)
    ATPFundAccountIDType fund_account_id; ///资金账户ID(ATP3.1.8开始该字段非必填) 
    ATPAccountIDType account_id; ///证券账户ID(ATP3.1.8开始该字段非必填) 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码(合约编码)（填0表示查所有）
    ATPMarketIDType market_id; ///市场代码（填0表示查所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExternalQueryOptionMdSnapShotArrayInfoMsg();
};


/**
 *  @brief 期权五档行情查询结果组合
 */
struct TRADE_API APIOptionMdSnapShatArrayInfoUnit
{
    ATPMarketIDType market_id; ///市场代码
    ATPTimeType orig_time; ///数据发布时间戳 DateTime_def
    ATPSecurityIDType security_id; ///合约编码
    ATPPriceType last_price; ///最近价格
    ATPPriceType limit_up; ///涨停价格
    ATPPriceType limit_down; ///跌停价格
    ATPPriceType open_price; ///开盘价格
    ATPPriceType close_price; ///收盘价格
    ATPPriceType high_price; ///最高价格
    ATPPriceType low_price; ///最低价格
    ATPPriceType buy_price1; ///买一价
    ATPPriceType buy_price2; ///买二价
    ATPPriceType buy_price3; ///买三价
    ATPPriceType buy_price4; ///买四价
    ATPPriceType buy_price5; ///买五价
    ATPPriceType sell_price1; ///卖一价
    ATPPriceType sell_price2; ///卖二价
    ATPPriceType sell_price3; ///卖三价
    ATPPriceType sell_price4; ///卖四价
    ATPPriceType sell_price5; ///卖五价
    ATPQtyType buy_qty1; ///买盘一
    ATPQtyType buy_qty2; ///买盘二
    ATPQtyType buy_qty3; ///买盘三
    ATPQtyType buy_qty4; ///买盘四
    ATPQtyType buy_qty5; ///买盘五
    ATPQtyType sell_qty1; ///卖盘一
    ATPQtyType sell_qty2; ///卖盘二
    ATPQtyType sell_qty3; ///卖盘三
    ATPQtyType sell_qty4; ///卖盘四
    ATPQtyType sell_qty5; ///卖盘五
    
    APIOptionMdSnapShatArrayInfoUnit();
};


/**
 *  @brief 期权五档行情查询结果
 */
struct TRADE_API ATPRspExternalQueryOptionMdSnapShotArrayInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOptionMdSnapShatArrayInfoUnit> order_array; ///查询结果组合
    
    ATPRspExternalQueryOptionMdSnapShotArrayInfoMsg();
};


/**
 *  @brief 成交查询消息
 */
struct TRADE_API ATPReqOptionTradeOrderQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    
    ATPReqOptionTradeOrderQueryMsg();
};


/**
 *  @brief 期权成交信息单元
 */
struct TRADE_API APIOptionTradeOrderUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向 填'0'查所有
    ATPOrdTypeType ord_type; ///订单类别
    ATPExecTypeType exec_type; ///当前订单执行类型
    ATPExecIDType exec_id; ///执行编号
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓
    ATPTimeStampType transact_time; ///客户委托时间
    ATPPriceType last_px; ///成交价格，单位：元(精确到0.0001)
    ATPQtyType last_qty; ///成交数量
    ATPAmtType total_value_traded; ///成交金额，精确到三位小数
    ATPAmtType fee; ///费用
    ATPPositionEffectType position_effect; ///开平仓标志
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签
    ATPCallOrPutType call_or_put; ///认购C， 认沽P
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码
    
    APIOptionTradeOrderUnit();
};


/**
 *  @brief 期权成交查询结果
 */
struct TRADE_API ATPRspOptionTradeOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOptionTradeOrderUnit> order_array; ///查询结果组合
    
    ATPRspOptionTradeOrderQueryResultMsg();
};


/**
 *  @brief 期权订单查询消息
 */
struct TRADE_API ATPReqOptionOrderQueryMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType cl_ord_no; ///客户订单编号 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSideType side; ///买卖方向 （默认值为'0'，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPOrderQueryConditionType order_query_condition; ///委托查询条件 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    
    ATPReqOptionOrderQueryMsg();
};


/**
 *  @brief 期权订单信息单元
 */
struct TRADE_API APIOptionOrderUnit
{
    ATPBusinessTypeType business_type; ///业务类型
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///市场代码
    ATPAccountIDType account_id; ///证券账户ID
    ATPSideType side; ///买卖方向
    ATPOrdTypeType ord_type; ///订单类别
    ATPOrdStatusType ord_status; ///当前申报的状态，（对于撤单订单，填写被撤订单的当前状态，找不到原始订单时，固定填8 = Reject ）
    ATPTimeStampType transact_time; ///客户委托时间
    ATPPriceType order_price; ///委托价格N13(4)
    ATPPriceType exec_price; ///平均成交价格N13(4)
    ATPQtyType order_qty; ///委托数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType leaves_qty; ///未成交部分的数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPQtyType cum_qty; ///累计成交数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPClOrdNOType cl_ord_no; ///客户订单编号
    ATPOrderIDType order_id; ///交易所订单编号
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType orig_cl_ord_no; ///对于撤单订单，为原始交易客户方（券商）订单编号，指示被撤消订单的cl_ord_no; 对于普通订单，取值为0
    ATPAmtType frozen_trade_value; ///冻结交易金额N18(4)
    ATPAmtType frozen_fee; ///冻结费用N18(4)
    ATPPositionEffectType position_effect; ///开平仓标志
    ATPCoveredOrUncoveredType covered_or_uncovered; ///备兑标签
    ATPSecurityIDType strategy_id; ///组合策略编码
    ATPSecondaryOrderIDType secondary_order_id; ///组合编码
    ATPSecurityIDType leg_security_id1; ///成份1合约编码
    ATPSecurityIDType leg_security_id2; ///成份2合约编码
    ATPSecurityIDType leg_security_id3; ///成份3合约编码
    ATPSecurityIDType leg_security_id4; ///成份4合约编码
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPOrdRejReasonType ord_rej_reason; ///拒绝原因描述信息；当交易所拒绝时，这里填写交易所返回的拒绝原因代码。可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-1交易所拒绝原因错误码类型，即可查询错误描述信息
    ATPCallOrPutType call_or_put; ///认购C， 认沽P
    ATPAmtType order_entrusted_amt; ///委托金额N18(4)
    ATPAmtType order_cum_transaction_amt; ///累计成交金额N18(4)
    
    APIOptionOrderUnit();
};


/**
 *  @brief 期权订单查询结果
 */
struct TRADE_API ATPRspOptionOrderQueryResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APIOptionOrderUnit> order_array; ///查询结果组合
    
    ATPRspOptionOrderQueryResultMsg();
};


/**
 *  @brief 标的周期内该备兑占用结果组合
 */
struct TRADE_API APIContractQtyArray
{
    ATPSecurityIDType security_id; ///合约编码
    ATPMarketIDType market_id; ///市场代码
    ATPQtyType locked_qty; ///已锁定数量
    ATPQtyType init_covered_occupy; ///昨备兑占用
    ATPQtyType init_covered_gap; ///日初备兑缺口
    ATPQtyType cur_covered_occupy; ///当前备兑占用
    ATPQtyType cur_covered_gap; ///当前预估备兑缺口
    ATPQtyType covered_close_free_qty; ///备兑平仓释放数量
    ATPQtyType covered_open_avai; ///可用于备兑开仓数量
    
    APIContractQtyArray();
};


/**
 *  @brief 标的证券备兑占用查询结果
 */
struct TRADE_API ATPRspExtQueryResultLockedAccountContractQtyMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPAccountSubCodeType account_sub_code; ///账户子编码
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///合约持仓记录总数
    std::vector<APIContractQtyArray> contract_array; ///组合结果
    
    ATPRspExtQueryResultLockedAccountContractQtyMsg();
};


/**
 *  @brief 透传第三方OES通用接口
 */
struct TRADE_API ATPReqThirdPartPassThroughMsg
{
    ATPPartitionType partition; ///分区号
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码
    ATPBranchIDType branch_id; ///营业部ID
    ATPTradeRootNumType trade_root_num; ///集中交易节点号
    ATPExtraData data; ///数据(可变长字段：根据业务功能自定义)
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqThirdPartPassThroughMsg();
};


/**
 *  @brief 透传第三方OES通用接口应答
 */
struct TRADE_API ATPRspThirdPartPassThroughResultMsg
{
    ATPUserInfoType user_info; ///用户私有信息
    ATPTimeStampType transact_time; ///回报时间
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPExtraData data; ///数据(可变长字段：根据业务功能自定义)
    
    ATPRspThirdPartPassThroughResultMsg();
};


/**
 *  @brief 融资保底额度查询结果
 */
struct TRADE_API ATPRspQueryMarginTradingPrivateAmoutResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPBranchIDType branch_id; ///营业部ID
    ATPAmtType private_amount; ///保底额度N18(4)
    ATPAmtType private_amount_occupied; ///保底额度占用N18(4)
    ATPAmtType public_amount_occupied; ///公用额度占用N18(4)
    ATPFirstUsePrivateAmountType first_use_private_amount; ///是否先用保底额度
    
    ATPRspQueryMarginTradingPrivateAmoutResultMsg();
};


/**
 *  @brief 融券保底额度单元
 */
struct TRADE_API APISecuritiesLendingPrivateAmountUnit
{
    ATPMarketIDType market_id; ///交易所
    ATPSecurityIDType security_id; ///证券代码
    ATPQtyType private_amount; ///保底额度N15(2)
    ATPQtyType private_amount_occupied; ///保底额度占用N15(2)
    ATPQtyType public_amount_occupied; ///公用额度占用N15(2)
    ATPFirstUsePrivateAmountType first_use_private_amount; ///是否先用保底额度
    
    APISecuritiesLendingPrivateAmountUnit();
};


/**
 *  @brief 信用账户融券保底额度查询结果
 */
struct TRADE_API ATPRspQuerySecuritiesLendingPrivateAmountResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<APISecuritiesLendingPrivateAmountUnit> data_array; ///查询结果组合
    
    ATPRspQuerySecuritiesLendingPrivateAmountResultMsg();
};


/**
 *  @brief 银证转账请求
 */
struct TRADE_API ATPReqBankSecuritiesFundTransferOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPFundAccountIDType account_id; ///证券账户ID 
    ATPMarketIDType market_id; ///市场代码 
    ATPCurrencyType currency; ///货币种类 （默认值 "CNY"）
    ATPTransferWayType transfer_way; ///划拨方式 ：1-银行转入；2-银行转出
    ATPAmtType transfer_value; ///划拨金额 N18(4) 
    ATPRemarkType remark; ///备注 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqBankSecuritiesFundTransferOtherMsg();
};


/**
 *  @brief 银证转账请求应答
 */
struct TRADE_API ATPRspBankSecuritiesFundTransferResultOtherMsg
{
    ATPTimeStampType transact_time; ///处理时间
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPTransferWayType transfer_way; ///划拨方式
    ATPSeqNumType transfer_serial_no; ///外部系统统一划拨流水号
    ATPSeqNumType bank_transfer_serial_no; ///外部系统银证转账流水号
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///数据(可变长字段：根据业务功能自定义)
    
    ATPRspBankSecuritiesFundTransferResultOtherMsg();
};


/**
 *  @brief 一步式银证转账请求
 */
struct TRADE_API ATPReqCashBankSecuritiesFundTransferOtherMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPFundAccountIDType account_id; ///证券账户ID 
    ATPMarketIDType market_id; ///市场代码 
    ATPCurrencyType currency; ///货币种类 （默认值 "CNY"）
    ATPTransferWayType transfer_way; ///划拨方式 ：1-银证转入；2-银证转出
    ATPAmtType transfer_value; ///划拨金额 N18(4) 
    ATPRemarkType remark; ///备注 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    
    ATPReqCashBankSecuritiesFundTransferOtherMsg();
};


/**
 *  @brief 一步式银证转账请求应答
 */
struct TRADE_API ATPRspCashBankSecuritiesFundTransferResultOtherMsg
{
    ATPTimeStampType transact_time; ///处理时间
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPTransferWayType transfer_way; ///划拨方式
    ATPSeqNumType transfer_serial_no; ///外部系统统一划拨流水号
    ATPSeqNumType bank_transfer_serial_no; ///外部系统银证转账流水号
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///数据(可变长字段：根据业务功能自定义)
    ATPSerialNoType serial_no; ///流水号（ATP3.2.1开始支持）
    ATPBusinessStatusType business_status; ///业务状态（ATP3.2.1开始支持）
    
    ATPRspCashBankSecuritiesFundTransferResultOtherMsg();
};


/**
 *  @brief 外部实时资金查询消息
 */
struct TRADE_API ATPReqExtRealtimeFundQueryMsg
{
    ATPBranchIDType branch_id; ///营业部ID 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPCurrencyType currency; ///货币种类 
    ATPExtraData extra_data; ///扩展字段 
    
    ATPReqExtRealtimeFundQueryMsg();
};


/**
 *  @brief 外部资金查询结果
 */
struct TRADE_API ATPRspExtRealtimeFundQueryResultMsg
{
    ATPBranchIDType branch_id; ///营业部ID
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPCurrencyType currency; ///货币种类
    ATPExtraData extra_data; ///扩展字段
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPAmtType fund_balance; ///资金余额N18(4)
    ATPAmtType fund_available; ///可用资金N18(4)
    
    ATPRspExtRealtimeFundQueryResultMsg();
};


/**
 *  @brief 新股清单查询消息
 */
struct TRADE_API ATPReqExtQueryNewShareInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID(ATP3.1.8开始该字段非必填) 
    ATPFundAccountIDType fund_account_id; ///资金账户ID(ATP3.1.8开始该字段非必填) 
    ATPAccountIDType account_id; ///证券账户ID (ATP3.1.8开始该字段非必填)
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryNewShareInfoMsg();
};


/**
 *  @brief 新股清单查询组合
 */
struct TRADE_API ApiNewShareInfoArray
{
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///申购代码
    ATPSecuritySymbolType security_symbol; ///申购代码名称
    ATPSecurityIDType underlying_security_id; ///正股代码
    ATPIssueTypeType issue_type; ///发行方式
    ATPPriceType price_upper_limit; ///发行价上限N13(4)
    ATPPriceType price_lower_limit; ///发行价下限N13(4)
    ATPQtyType purchase_unit; ///申购单位N15(2)
    ATPQtyType max_purchase_qty; ///最大申购量N15(2)
    ATPQtyType init_issued_total_qty; ///网上初始发行股数N15(2)
    ATPBoolType relate_to_investor_quota; ///是否与投资者额度关联
    ATPBoolType is_repeat_purchase; ///是否允许重复认购
    ATPBoolType is_revoked_order; ///是否允许撤单
    ATPBoolType is_checkfund; ///是否资金检查
    ATPDate8Type start_date; ///发行起始日
    ATPDate8Type end_date; ///发行结束日
    ATPSecurityTypeType security_type; ///证券类别
    
    
};


/**
 *  @brief 新股清单查询结果消息
 */
struct TRADE_API ATPRspExtQueryResultNewShareInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///组合个数
    std::vector<ApiNewShareInfoArray> new_share_info_array; ///查询组合
    
    ATPRspExtQueryResultNewShareInfoMsg();
};


/**
 *  @brief 证券信息查询消息
 */
struct TRADE_API ATPReqExtQuerySecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID(ATP3.1.8开始该字段非必填) 
    ATPFundAccountIDType fund_account_id; ///资金账户ID(ATP3.1.8开始该字段非必填) 
    ATPAccountIDType account_id; ///证券账户ID(ATP3.1.8开始该字段非必填)
    ATPSeqNumType client_seq_id; ///用户系统消息序号 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQuerySecurityInfoMsg();
};


/**
 *  @brief 证券信息查询组合
 */
struct TRADE_API ApiSecurityInfoArray
{
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券代码名称
    ATPSecurityTypeType security_type; ///证券类别
    ATPPriceType prev_close_price; ///昨日收盘价N13(4)
    ATPPriceType price_upper_limit; ///涨停价N13(4)
    ATPPriceType price_lower_limit; ///跌停价N13(4)
    ATPPriceTickType price_tick; ///最小变动价位N13(4)
    ATPQtyType buy_qty_lower_limit; ///最小买数量下限N15(2)
    ATPQtyType sell_qty_lower_limit; ///最小卖数量下限N15(2)
    ATPBoolType day_trading; ///是否支持回转交易
    ATPBoolType has_price_limit; ///涨跌幅限制
    ATPSecurityStatusType security_status; ///证券状态（按bit位设置对应的证券状态，例：129表示该证券状态为0-正常、7-上市首日）（ATP3.1.10.2开始支持）
    ATPQtyType market_buy_qty_upper_limit; ///市价买数量上限N15(2)（ATP3.1.10开始支持）
    ATPQtyType market_buy_qty_lower_limit; ///市价买数量下限N15(2)（ATP3.1.10开始支持）
    ATPQtyType market_sell_qty_upper_limit; ///市价卖数量上限N15(2)（ATP3.1.10开始支持）
    ATPQtyType market_sell_qty_lower_limit; ///市价卖数量下限N15(2)（ATP3.1.10开始支持）
    ATPQtyType buy_qty_upper_limit; ///限价买数量上限N15(2)（ATP3.1.10开始支持）
    ATPQtyType sell_qty_upper_limit; ///限价卖数量上限N15(2)（ATP3.1.10开始支持）
    
    
};


/**
 *  @brief 证券信息查询结果消息
 */
struct TRADE_API ATPRspExtQueryResultSecurityInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPLengthType total_num; ///组合个数
    std::vector<ApiSecurityInfoArray> security_info_array; ///查询组合
    
    ATPRspExtQueryResultSecurityInfoMsg();
};


/**
 *  @brief 增强股份查询消息
 */
struct TRADE_API ATPReqExtQueryShareExMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    std::vector<APIAccountIDUnit> account_id_array; ///待查证券账户组 
    ATPBusinessTypeType business_type; ///业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPMarketIDType market_id; ///市场代码 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryShareExMsg();
};


/**
 *  @brief 增强股份查询结果组合
 */
struct TRADE_API ApiShareArrayEx
{
    ATPSecurityIDType security_id; ///证券代码
    ATPSecuritySymbolType security_symbol; ///证券简称
    ATPMarketIDType market_id; ///证券所属市场
    ATPAccountIDType account_id; ///证券账户
    ATPQtyType init_qty; ///日初持仓量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType leaves_qty; ///剩余股份数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)（包含当日买入部分）
    ATPQtyType available_qty; ///可用股份数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPAmtType profit_loss; ///浮动盈亏,N18(4)
    ATPAmtType market_value; ///市值,N18(4)
    ATPAmtType cost_price; ///成本价,N18(4)
    ATPPriceType last_price; ///最新价,N13(4)
    ATPSecurityTypeType security_type; ///证券类型
    ATPQtyType stock_buy; ///股份买入解冻(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType stock_sale; ///股份卖出冻结(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部代码
    ATPQtyType etf_redemption_qty; ///对于ETF基金表示可赎回数量，对于其他证券表示可用于ETF申购的数量（ATP3.2.1开始支持）
    
    ApiShareArrayEx();
};


/**
 *  @brief 增强股份查询结果
 */
struct TRADE_API ATPRspExtQueryResultShareExMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///记录总数
    std::vector<ApiShareArrayEx> order_array; ///查询结果组合
    
    ATPRspExtQueryResultShareExMsg();
};


/**
 *  @brief 增强资产查询消息
 */
struct TRADE_API ATPReqExtQueryAssetExMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///证券账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPCurrencyType currency; ///货币种类 
    
    ATPReqExtQueryAssetExMsg();
};


/**
 *  @brief 增强资产查询结果
 */
struct TRADE_API ATPRspExtQueryResultAssetExMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPUserInfoType user_info; ///用户私有信息
    ATPAmtType leaves_value; ///资金余额,N18(4)
    ATPAmtType init_leaves_value; ///日初资金金额,N18(4)
    ATPAmtType available_t0; ///T+0可用资金,N18(4)
    ATPAmtType available_t1; ///T+1可用资金,N18(4)
    ATPAmtType available_t2; ///T+2可用资金,N18(4)
    ATPAmtType available_t3; ///T+3可用资金,N18(4)
    ATPAmtType available_tall; ///T+All可用资金,N18(4)
    ATPAmtType frozen_all; ///当前所有冻结,N18(4)
    ATPAgwPartitionType te_partition_no; ///所属分区号
    ATPBranchIDType branch_id; ///origid+营业部ID,各占5个字节
    ATPAmtType credit_sell_frozen; ///融券卖出冻结金额,N18(4)
    ATPAmtType credit_sell_occupied; ///融券卖出款占用
    ATPAmtType credit_sell_pre_occupied; ///融券卖出款预占用
    ATPAmtType init_credit_sell; ///日初融券卖出款
    ATPAmtType total_asset; ///资产总值
    ATPAmtType market_value; ///总市值
    ATPAmtType fund_buy; ///买入冻结
    ATPAmtType fund_sale; ///卖出解冻
    
    ATPRspExtQueryResultAssetExMsg();
};


/**
 *  @brief 算法母单下达委托
 */
struct TRADE_API ATPReqAlgoParentOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场ID  
    ATPSideType side; ///买卖方向  
    ATPQtyType order_qty; ///母单数量N15(2)（股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张）
    ATPPriceType price; ///母单的限价N13(4) ;表示算法子单委托价格的买入上限或卖出下限；填0表示母单市价，即不限制子单价格上下限
    ATPAlgoOrdTypeType order_type; ///订单类型  ，配合price一起使用
    ATPTimeInForceType time_in_force; ///订单有效时间类型  ;'0'=当日有效
    ATPStringType algo_name; ///算法名称  ，例如：TWAP、VWAP等
    ATPTimeType start_time; ///算法开始时间 （填0表示从当前时间开始）格式YYYYMMDDHHmmSSsss;例：20201124093000000
    ATPTimeType end_time; ///算法结束时间 （填0表示结束时间由“执行时长”参数决定）格式YYYYMMDDHHmmSSsss;例：20201124150000000
    ATPAlgoDurationSecType duration_sec; ///算法执行长度(单位秒) （当结束时间为0时，此参数生效），算法系统会自动剔除非交易时间。如开始时间对应9:00:00，此参数为300秒，对应的有效执行时间段是9:30:00~9:35:00
    ATPAlgoPartRateType part_rate; ///算法参与比例  N5(4) ，值域[0,10000]，如值2000表示量比20%，0或10000表示没有参与比例限制
    ATPStringType extra_params; ///算法扩展参数 ;Json格式，如设置单笔委托最小金额为10000元，对应值: {“min_amount”:10000}
    ATPStringType basket_id; ///篮子ID 
    ATPAlgoBasketCountType basket_count; ///一篮子算法母单个数 
    
    ATPReqAlgoParentOrderMsg();
};


/**
 *  @brief 算法母单查询请求
 */
struct TRADE_API ATPReqExternalQueryAlgoParentMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType parent_cl_ord_no; ///算法母单编号 ,填0查所有
    ATPMarketIDType market_id; ///证券所属市场 ,填0查所有
    ATPSecurityIDType security_id; ///证券代码 ,填空查所有
    ATPBusinessTypeType business_type; ///业务类型 ,填0查所有
    ATPSideType side; ///买卖方向 ,填'0'查所有
    ATPReportIndexType query_index; ///起始查询索引号 ,填0表示从头开始查
    ATPTotalNumType return_num; ///指定查询返回数量 ,填 0表示不指定
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 ,例如，1=按时间正序，2=按时间倒序
    
    ATPReqExternalQueryAlgoParentMsg();
};


/**
 *  @brief 算法母单执行回报响应
 */
struct TRADE_API ATPRspAlgoParentExecReportMsg
{
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///账户ID
    ATPUserInfoType user_info; ///用户私有信息
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClOrdNOType parent_cl_ord_no; ///算法母单编号
    ATPPbuIDType offer_pbu_id; ///申报交易单元
    ATPCustIDType cust_id; ///客户号ID
    ATPOwnerType owner_type; ///订单所有者类型
    ATPBranchIDType offer_branch_id; ///报盘营业部代码(交易所编码)
    ATPPbuIDType pbu_id; ///投资者所属交易单元
    ATPBranchIDType branch_id; ///营业部代码(券商编码)
    ATPMarketIDType market_id; ///证券所属市场
    ATPSecurityIDType security_id; ///证券代码
    ATPBusinessTypeType business_type; ///业务类型
    ATPSideType side; ///买卖方向
    ATPPriceType price; ///母单限价
    ATPQtyType order_qty; ///母单数量(张)
    ATPStringType algo_name; ///算法名称
    ATPTimeType start_time; ///算法开始时间
    ATPTimeType end_time; ///算法结束时间
    ATPAlgoDurationSecType duration_sec; ///算法执行长度(单位秒)
    ATPAlgoPartRateType part_rate; ///算法参与比例
    ATPStringType extra_params; ///算法扩展参数, Json格式
    ATPStringType basket_id; ///篮子ID
    ATPAlgoBasketCountType basket_count; ///一篮子算法母单个数
    ATPAlgoStatusType algo_status; ///母单状态
    ATPStringType algo_text; ///母单信息
    ATPTimeStampType transact_time; ///母单委托时间
    ATPTimeStampType last_updated_time; ///母单最新更新时间
    ATPQtyType leaves_qty; ///剩余未委托数量
    ATPQtyType cum_sent_qty; ///累计成交数量 + 在途数量N15(2)
    ATPQtyType cum_qty; ///累计成交数量N15(2)
    ATPPriceType cum_avg_price; ///累计成交均价N13(4)
    ATPStringType extra_fields; ///母单回报的json扩展字段
    ATPIndexType index; ///回报记录号
    
    ATPRspAlgoParentExecReportMsg();
};


/**
 *  @brief 算法母单查询结果
 */
struct TRADE_API ATPRspExternalQueryResultAlgoParentMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码
    ATPUserInfoType user_info; ///用户私有信息
    ATPExtraData extra_data; ///扩展字段
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号，当 “LastIndex +1 == TotalNum”时，表示已经查到最后的记录；组合个数为0时，该值为-1
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<ATPRspAlgoParentExecReportMsg> parent_array; ///查询结果组合
    
    ATPRspExternalQueryResultAlgoParentMsg();
};


/**
 *  @brief 算法母单撤销委托
 */
struct TRADE_API ATPReqCancelAlgoParentOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///账户ID 
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息  
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPClOrdNOType orig_parent_cl_ord_no; ///原算法母单编号 
    
    ATPReqCancelAlgoParentOrderMsg();
};


/**
 *  @brief 算法子单查询消息
 */
struct TRADE_API ATPReqExternalQueryAlgoOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID  
    ATPAccountIDType account_id; ///证券账户ID（一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPExtraData extra_data; ///扩展字段 
    ATPClOrdNOType parent_cl_ord_no; ///算法母单编号
    ATPClOrdNOType cl_ord_no; ///客户订单编号,同一个客户当天唯一,填0查所有
    ATPMarketIDType market_id; ///证券所属市场 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSecurityIDType security_id; ///证券代码证券代码 （默认值为空字符串，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPBusinessTypeType business_type; ///业务类型业务类型 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPSideType side; ///买卖方向 
    ATPOrderQueryConditionType order_query_condition; ///委托查询条件 （默认值为0，表示查所有，一户两地账户表示查单个证券账户下所有）
    ATPReportIndexType query_index; ///起始查询索引号 （默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///查询返回数量 （默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    ATPReturnSeqType return_seq; ///返回顺序，按时间排序 （默认值为1，表示按时间正序排序）
    
    ATPReqExternalQueryAlgoOrderMsg();
};


/**
 *  @brief 算法子单查询结果
 */
struct TRADE_API ATPRspExternalQueryResultAlgoOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID 
    ATPAccountIDType account_id; ///账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPClientFeatureCodeType client_feature_code; ///终端识别码
    ATPUserInfoType user_info; ///用户私有信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号，当 “LastIndex +1 == TotalNum”时，表示已经查到最后的记录；组合个数为0时，该值为-1
    ATPTotalNumType total_num; ///订单记录总数
    ATPClOrdNOType parent_cl_ord_no; ///算法母单编号
    std::vector<APIOrderUnit> order_array; ///查询结果组合
    
    ATPRspExternalQueryResultAlgoOrderMsg();
};


/**
 *  @brief 债券协商成交委托
 */
struct TRADE_API ATPReqBondConsultTradeOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户id
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商）
    ATPSecurityIDType security_id; ///证券代码
    ATPMarketIDType market_id; ///市场代码（只支持102-深圳市场）
    ATPSideType side; ///买卖方向
    ATPPriceType price; ///委托价格,N13(4)
    ATPQtyType order_qty; ///申报数量N15(2)(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPTradeReportTypeType trade_report_type; ///成交申报类型（默认值为0，表示提交成交申报）
    ATPTransAffairTypeType trade_report_trans_type; ///成交申报事务类别（默认值为0，表示普通申报）
    ATPClOrdIDType trade_report_ref_id; ///原客户成交申报编号（成交申报类型trade_report_type为2-接受成交申报、3-拒绝成交申报时，该字段必填）
    ATPMemberIDType counterparty_member_id; ///对手方交易商代码
    ATPInvestorTypeType counterparty_investor_type; ///对手方交易主体类型
    ATPInvestorIDType counterparty_investor_id; ///对手方交易主体代码
    ATPInvestorNameType counterparty_investor_name; ///对手方客户名称
    ATPTraderCodeType counterparty_trader_code; ///对手方交易员代码
    ATPAccountIDType counterparty_account_id; ///对手方证券账户
    ATPMemoType memo; ///备注
    
    ATPReqBondConsultTradeOrderMsg();
};


/**
 *  @brief 债券协商成交委托撤单委托
 */
struct TRADE_API ATPReqBondConsultTradeCancelOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金帐号ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商）
    ATPMarketIDType market_id; ///市场代码
    ATPOrderWayType order_way; ///委托方式（该字段是否必填以及字典项请咨询券商）
    ATPClOrdNOType orig_cli_ord_no; ///原委托的客户订单编号
    
    ATPReqBondConsultTradeCancelOrderMsg();
};


/**
 *  @brief 债券协商成交委托成交回报
 */
struct TRADE_API ATPRspBondConsultTradeERMsg
{
    ATPAgwPartitionType partition; ///分区号
    ATPAgwIndexType index; ///索引号
    ATPBusinessTypeType business_type; ///业务类型
    ATPClOrdNOType client_order_no; ///客户订单号
    ATPSecurityIDType security_id; ///证券代码
    ATPMarketIDType market_id; ///市场ID
    ATPExecTypeType exec_type; ///当前委托执行类型
    ATPOrdStatusType order_status; ///当前申报的状态
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///投资者帐号
    ATPPriceType price; ///委托价格,N13(4)
    ATPQtyType order_qty; ///委托数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType leaves_qty; ///未成交数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPQtyType cum_qty; ///累计成交数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPSideType side; ///买卖方向
    ATPTimeStampType transact_time; ///回报时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPOrderIDType order_id; ///交易所订单编号, 取值为数字
    ATPClOrdIDType cl_ord_id; ///申报合同号,上交所：以QP1开头,表示为交易所保证金强制平仓；以CV1开头,表示为交易所备兑强制平仓；
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPExecIDType exec_id; ///成交编号
    ATPAmtType last_px; ///成交价格,N18(4)
    ATPQtyType last_qty; ///成交数量(股票为股、基金为份、上海债券默认为张（使用时请务必与券商确认），其他为张)N15(2)
    ATPAmtType total_value_traded; ///成交金额,N18(4)
    ATPAmtType fee; ///费用,N18(4)
    ATPMemberIDType member_id; ///本方交易商代码
    ATPInvestorTypeType investor_type; ///本方交易主体类型
    ATPInvestorIDType investor_id; ///本方交易主体代码
    ATPInvestorNameType investor_name; ///本方客户名称
    ATPTraderCodeType trader_code; ///本方交易员代码
    ATPMemberIDType counterparty_member_id; ///对手方交易商代码
    ATPInvestorTypeType counterparty_investor_type; ///对手方交易主体类型
    ATPInvestorIDType counterparty_investor_id; ///对手方交易主体代码
    ATPInvestorNameType counterparty_investor_name; ///对手方客户名称
    ATPTraderCodeType counterparty_trader_code; ///对手方交易员代码
    ATPSettlTypeType settl_type; ///结算方式
    ATPSettlPeriodType settl_period; ///结算周期
    ATPCashMarginType cash_margin; ///信用标识
    ATPMemoType memo; ///备注
    
    ATPRspBondConsultTradeERMsg();
};


/**
 *  @brief 债券协商成交转发成交申报查询消息
 */
struct TRADE_API ATPReqExtQueryOrderForwardInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商）
    ATPBranchIDType branch_id; ///营业部ID
    ATPMarketIDType market_id; ///证券所属市场（只支持102-深圳市场）
    ATPAccountIDType counterparty_account_id; ///对手方证券账户（默认值为空，表示查所有）
    ATPClOrdIDType orig_cl_ord_id; ///原客户成交申报编号（默认值为空，表示查所有）
    ATPReportIndexType query_index; ///起始查询索引号（默认值为0，表示从头开始查）
    ATPTotalNumType return_num; ///指定查询返回数量（默认值为0，表示按能返回的最大数量返回，具体数量请咨询券商）
    
    ATPReqExtQueryOrderForwardInfoMsg();
};


/**
 *  @brief 债券协商成交转发成交申报重复组信息
 */
struct TRADE_API ATPAgwOrderForwardInfoArray
{
    ATPOrdStatusType ord_status; ///订单当前申报的状态
    ATPSecurityIDType security_id; ///证券代码
    ATPOwnerType owner_type; ///订单所有者类型
    ATPTimeStampType transact_time; ///订单处理时间
    ATPUserInfoType user_info; ///用户私有信息
    ATPClOrdIDType trade_report_id; ///客户成交申报编号
    ATPTradeReportTypeType trade_report_type; ///成交申报类型
    ATPTransAffairTypeType trade_report_trans_type; ///成交申报事务类别
    ATPTradeHandlingInstrType trade_handling_instr; ///成交申报模式
    ATPClOrdIDType trade_report_ref_id; ///原客户成交申报编号
    ATPPriceType last_px; ///价格,单位：元(精确到0.0001元)
    ATPQtyType last_qty; ///数量
    ATPSideType side; ///买卖方向
    ATPPBUIDType pbu_id; ///本方交易单元
    ATPAccountIDType account_id; ///本方证券账户
    ATPBranchIDType branch_id; ///本方营业部代码
    ATPMemberIDType member_id; ///本方交易商代码
    ATPInvestorTypeType investor_type; ///本方交易主体类型
    ATPInvestorIDType investor_id; ///本方交易主体代码
    ATPInvestorNameType investor_name; ///本方客户名称
    ATPTraderCodeType trader_code; ///本方交易员代码
    ATPMemberIDType counterparty_member_id; ///对手方交易商代码
    ATPInvestorTypeType counterparty_investor_type; ///对手方交易主体类型
    ATPInvestorIDType counterparty_investor_id; ///对手方交易主体代码
    ATPInvestorNameType counterparty_investor_name; ///对手方客户名称
    ATPTraderCodeType counterparty_trader_code; ///对手方交易员代码
    ATPSettlTypeType settl_type; ///结算方式
    ATPSettlPeriodType settl_period; ///结算周期
    ATPCashMarginType cash_margin; ///信用标识
    ATPMemoType memo; ///备注
    
    ATPAgwOrderForwardInfoArray();
};


/**
 *  @brief 债券协商成交转发成交申报查询结果消息
 */
struct TRADE_API ATPRspExtQueryResultOrderForwardInfoMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///账户ID
    ATPSeqNumType client_seq_id; ///用户系统消息序号
    ATPQueryResultCodeType query_result_code; ///查询结果代码
    ATPUserInfoType user_info; ///用户私有信息
    ATPReportIndexType last_index; ///最后一笔查询结果的索引号
    ATPTotalNumType total_num; ///订单记录总数
    std::vector<ATPAgwOrderForwardInfoArray> order_forward_info_array; ///查询组合
    
    ATPRspExtQueryResultOrderForwardInfoMsg();
};


/**
 *  @brief 算法母单暂停恢复
 */
struct TRADE_API ATPReqOperateAlgoParentOrderMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID  
    ATPAccountIDType account_id; ///证券账户ID（一户两地账户此字段必填）（ATP3.1.7版本开始支持此字段非必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPClOrdNOType parent_cl_ord_no; ///算法母单编号
    ATPAlgoOperationType operation; ///操作类型
    
    ATPReqOperateAlgoParentOrderMsg();
};


/**
 *  @brief 外部持仓查询消息
 */
struct TRADE_API ATPReqExtStockQueryMsg
{
    ATPBranchIDType branch_id; ///营业部ID 
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPAccountIDType account_id; ///证券账户ID （一户两地账户此字段必填）
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPExtraData extra_data; ///扩展字段 
    
    ATPReqExtStockQueryMsg();
};


/**
 *  @brief 外部持仓查询结果
 */
struct TRADE_API ATPRspExtStockQueryResultMsg
{
    ATPBranchIDType branch_id; ///营业部ID
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPMarketIDType market_id; ///市场代码
    ATPSecurityIDType security_id; ///证券代码
    ATPExtraData extra_data; ///扩展字段
    ATPQueryResultCodeType query_result_code; ///查询结果代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型 -3 查询结果错误码类型，即可查询到错误描述信息
    ATPQtyType qty; ///委托数量 
    
    ATPRspExtStockQueryResultMsg();
};


/**
 *  @brief 外部持仓划拨请求
 */
struct TRADE_API ATPReqExtShareTransferMsg
{
    ATPCustIDType cust_id; ///客户号ID 
    ATPFundAccountIDType fund_account_id; ///资金账户ID 
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID 
    ATPSecurityIDType security_id; ///证券代码 
    ATPMarketIDType market_id; ///市场代码
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号 
    ATPOrderWayType order_way; ///委托方式（是否必填以及字典项请咨询券商）
    ATPUserInfoType user_info; ///用户私有信息 
    ATPClientFeatureCodeType client_feature_code; ///终端识别码（该字段是否必填请咨询券商）
    ATPPasswordType password; ///客户密码（该字段是否必填请咨询券商） 
    ATPTransferTypeType transfer_type; ///划拨方式 ：1=转入；2=转出
    ATPQtyType value; ///划拨数量 N15(2) 
    ATPRemarkType remark; ///备注 
    ATPExtraData extra_data; ///扩展字段 
    
    ATPReqExtShareTransferMsg();
};


/**
 *  @brief 外部持仓划拨结果
 */
struct TRADE_API ATPRspExtShareTransferResultMsg
{
    ATPCustIDType cust_id; ///客户号ID
    ATPFundAccountIDType fund_account_id; ///资金账户ID
    ATPBranchIDType branch_id; ///营业部ID
    ATPAccountIDType account_id; ///证券账户ID
    ATPMarketIDType market_id; ///市场代码
    ATPClientSeqIDType client_seq_id; ///用户系统消息序号
    ATPUserInfoType user_info; ///用户私有信息
    ATPRejectReasonCodeType reject_reason_code; ///拒绝原因代码；可调用GetErrorInfo错误码查询函数，传入错误码及错误码类型-5 拒绝原因错误码类型，即可查询到错误描述信息
    ATPStringType reject_desc; ///拒绝原因描述
    ATPSecurityIDType security_id; ///证券代码 
    ATPQtyType value; ///划拨数量N15(2)
    ATPExtraData extra_data; ///扩展字段
    
    ATPRspExtShareTransferResultMsg();
};




#endif // ATP_TRADE_MSG_H
