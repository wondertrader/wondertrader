// code_generator_tag
// responses:ccb91ced49101285a659c98389d90f6a template:646dd51ff6f508acc67bffc451828387 code:499be7f1573804da80b0033b8799da5c
#ifndef ATP_TRADE_HANDLER_H
#define ATP_TRADE_HANDLER_H

#include <string>
#include "atp_trade_msg.h"
#include "atp_trade_export.h"

/**
 * @brief TradeAPI处理句柄
 */

class TRADE_API ATPTradeHandler
{
public:
    virtual ~ATPTradeHandler() {}
    /**
	 * @brief 连接成功事件
	 * @param[in] reason 原因
     * @type session
	 */
    virtual void OnConnected(const std::string &reason) {}

    /**
	 * @brief 连接失败事件
	 * @param[in] reason 失败原因
     * @type session
	 */
    virtual void OnConnectFailure(const std::string &reason) {}

    /**
	 * @brief 连接超时时间
	 * @param[in] reason 超时原因
     * @type session
	 */
    virtual void OnConnectTimeOut(const std::string &reason) {}

    /**
	 * @brief 心跳超时事件
	 * @param[in] reason 超时原因
     * @type session
	 */
    virtual void OnHeartbeatTimeout(const std::string &reason) {}

    /**
	 * @brief  成功登录AGW事件
	 * @param[in] reason 成功原因
     * @type session
	 */
    virtual void OnLogin(const std::string &reason) {}

    /**
	 * @brief  登出事件
	 * @param[in] reason 登出原因
     * @type session
	 */
    virtual void OnLogout(const std::string &reason) {}

    /**
	 * @brief 重连结束
	 * @param[in] reason 重连结束原因 
	 * @note 重连结束后需要使用者主动在发起连接
     * @type session
	 */
    virtual void OnEndOfConnection(const std::string &reason) {}

    /**
	 * @brief  API内部错误
	 * @param[in] reason 错误原因
     * @note 内部发生错误时触发的回调
     * @type other
     * @business common
	 */
    virtual void OnError(const std::string &reason) {}

    /**
	 * @brief 连接关闭
	 * @param[in] reason 关闭原因
     * @type session
	 */
    virtual void OnClosed(const std::string &reason) {}

    /**
	 * @brief API内部日志回调函数
	 * @param[in] level 日志级别
	 * @param[in] message 日志内容 
     * @note API内部日志回调，可在Init函数中设置日志级别
     * @type other
     * @business common
	 */
    virtual void OnLog(ATPLogLevel level, const std::string &message) {}

    /**
	 * @brief  成功恢复AGW事件
	 * @param[in] reason 成功原因
     * @type session
	 */
    virtual void OnRecovered(const std::string &reason) {}

/**
     * @brief 委托状态内部响应
     * @param[in] ATPRspOrderStatusAckMsg
     * @type ack
     */
    virtual void OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg &msg){}

/**
     * @brief 委托状态响应
     * @param[in] ATPRspOrderStatusAckMsg
     * @type ack
     */
    virtual void OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg &msg){}

/**
     * @brief 报价委托状态响应
     * @param[in] ATPRspQuoteStatusAckMsg
     * @type ack
     */
    virtual void OnRspQuoteStatusAck(const ATPRspQuoteStatusAckMsg &msg){}

/**
     * @brief 期权多腿委托状态内部响应
     * @param[in] ATPRspNoLegsStatusAckMsg
     * @type ack
     */
    virtual void OnRspNoLegsStatusInternalAck(const ATPRspNoLegsStatusAckMsg &msg){}

/**
     * @brief 期权多腿委托状态响应
     * @param[in] ATPRspNoLegsStatusAckMsg
     * @type ack
     */
    virtual void OnRspNoLegsStatusAck(const ATPRspNoLegsStatusAckMsg &msg){}

/**
     * @brief 期权集中竞价业务成交回报
     * @param[in] ATPRspOptionAuctionTradeERMsg
     * @type deal
     */
    virtual void OnRspOptionAuctionTradeER(const ATPRspOptionAuctionTradeERMsg &msg){}

/**
     * @brief 融资融券交易成交回报
     * @param[in] ATPRspCreditTradeERMsg
     * @type deal
     */
    virtual void OnRspCreditTradeER(const ATPRspCreditTradeERMsg &msg){}

/**
     * @brief 融资融券交易偿还成交回报
     * @param[in] ATPRspMarginRepayTradeERMsg
     * @type deal
     */
    virtual void OnRspMarginRepayTradeER(const ATPRspMarginRepayTradeERMsg &msg){}

/**
     * @brief 现货成交回报
     * @param[in] ATPRspCashAuctionTradeERMsg
     * @type deal
     */
    virtual void OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg &msg){}

/**
     * @brief 质押式回购集中竞价业务成交回报
     * @param[in] ATPRspRepoAuctionTradeERMsg
     * @type deal
     */
    virtual void OnRspRepoAuctionTradeER(const ATPRspRepoAuctionTradeERMsg &msg){}

/**
     * @brief ETF申赎业务成交回报
     * @param[in] ATPRspETFRedemptionTradeERMsg
     * @type deal
     */
    virtual void OnRspETFRedemptionTradeER(const ATPRspETFRedemptionTradeERMsg &msg){}

/**
     * @brief 债券分销业务成交回报
     * @param[in] ATPRspBondDistributionTradeERMsg
     * @type deal
     */
    virtual void OnRspBondDistributionTradeER(const ATPRspBondDistributionTradeERMsg &msg){}

/**
     * @brief 分级基金实时分拆合并业务成交回报
     * @param[in] ATPRspStructuredFundTradeERMsg
     * @type deal
     */
    virtual void OnRspStructuredFundTradeER(const ATPRspStructuredFundTradeERMsg &msg){}

/**
     * @brief 开放式基金申赎业务成交回报
     * @param[in] ATPRspLOFTradeERMsg
     * @type deal
     */
    virtual void OnRspLOFTradeER(const ATPRspLOFTradeERMsg &msg){}

/**
     * @brief 盘后定价(科创板、创业板)业务成交回报
     * @param[in] ATPRspTibAfterHourAuctionTradeERMsg
     * @type deal
     */
    virtual void OnRspTibAfterHourAuctionTradeER(const ATPRspTibAfterHourAuctionTradeERMsg &msg){}

/**
     * @brief 质押入出库业务成交回报
     * @param[in] ATPRspPledgeInOrOutTradeERMsg
     * @type deal
     */
    virtual void OnRspPledgeInOrOutTradeER(const ATPRspPledgeInOrOutTradeERMsg &msg){}

/**
     * @brief 最大可委托数查询结果
     * @param[in] ATPRspMaxOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspMaxOrderQueryResult(const ATPRspMaxOrderQueryResultMsg &msg){}

/**
     * @brief 订单查询结果
     * @param[in] ATPRspOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg &msg){}

/**
     * @brief 融资融券合约明细查询结果
     * @param[in] ATPRspQueryContractSpecificationsResultMsg
     * @type query
     */
    virtual void OnRspQueryContractSpecificationsQueryResult(const ATPRspQueryContractSpecificationsResultMsg &msg){}

/**
     * @brief 融资融券信用额度查询结果
     * @param[in] ATPRspQueryCreditAmountResultMsg
     * @type query
     */
    virtual void OnRspQueryCreditAmountQueryResult(const ATPRspQueryCreditAmountResultMsg &msg){}

/**
     * @brief 融资融券保证金可用查询结果
     * @param[in] ATPRspQueryMarginUseableResultMsg
     * @type query
     */
    virtual void OnRspQueryMarginUseableQueryResult(const ATPRspQueryMarginUseableResultMsg &msg){}

/**
     * @brief 融资融券资金负债查询结果
     * @param[in] ATPRspQueryFundLiabilityResultMsg
     * @type query
     */
    virtual void OnRspQueryFundLiabilityQueryResult(const ATPRspQueryFundLiabilityResultMsg &msg){}

/**
     * @brief 融资融券股份负债查询结果
     * @param[in] ATPRspQueryShareLiabilityResultMsg
     * @type query
     */
    virtual void OnRspQueryShareLiabilityQueryResult(const ATPRspQueryShareLiabilityResultMsg &msg){}

/**
     * @brief 融资融券合约管理查询结果
     * @param[in] ATPRspQueryContractSpecificationsManagerResultMsg
     * @type query
     */
    virtual void OnRspQueryContractSpecificationsManagerQueryResult(const ATPRspQueryContractSpecificationsManagerResultMsg &msg){}

/**
     * @brief 融资融券标的清单查询结果
     * @param[in] ATPRspQueryMarginUnderlyingResultMsg
     * @type query
     */
    virtual void OnRspQueryMarginUnderlyingQueryResult(const ATPRspQueryMarginUnderlyingResultMsg &msg){}

/**
     * @brief 融资融券客户信用资料查询结果
     * @param[in] ATPRspQueryCreditInfoResultMsg
     * @type query
     */
    virtual void OnRspQueryCreditInfoQueryResult(const ATPRspQueryCreditInfoResultMsg &msg){}

/**
     * @brief 融资融券资产信息查询结果
     * @param[in] ATPRspQueryFundAssetsInfoResultMsg
     * @type query
     */
    virtual void OnRspQueryFundAssetsInfoQueryResult(const ATPRspQueryFundAssetsInfoResultMsg &msg){}

/**
     * @brief 报价订单查询结果
     * @param[in] ATPRspQuoteQueryResultMsg
     * @type query
     */
    virtual void OnRspQuoteOrderQueryResult(const ATPRspQuoteQueryResultMsg &msg){}

/**
     * @brief 期权组合策略保证金持仓查询结果
     * @param[in] ATPRspCombinationQueryResultMsg
     * @type query
     */
    virtual void OnRspCombinationPositionQueryResult(const ATPRspCombinationQueryResultMsg &msg){}

/**
     * @brief ETF申赎成交查询结果
     * @param[in] ATPRspETFTradeOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspEtfTradeOrderQueryResult(const ATPRspETFTradeOrderQueryResultMsg &msg){}

/**
     * @brief 成交查询结果
     * @param[in] ATPRspTradeOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg &msg){}

/**
     * @brief 资金查询结果
     * @param[in] ATPRspFundQueryResultMsg
     * @type query
     */
    virtual void OnRspFundQueryResult(const ATPRspFundQueryResultMsg &msg){}

/**
     * @brief 股份查询结果
     * @param[in] ATPRspShareQueryResultMsg
     * @type query
     */
    virtual void OnRspShareQueryResult(const ATPRspShareQueryResultMsg &msg){}

/**
     * @brief 新股发行申购额度查询结果
     * @param[in] ATPRspIPOQtyQueryResultMsg
     * @type query
     */
    virtual void OnRspIPOQtyQueryResult(const ATPRspIPOQtyQueryResultMsg &msg){}

/**
     * @brief 合约账户持仓查询结果
     * @param[in] ATPRspAccountContractShareQueryResultMsg
     * @type query
     */
    virtual void OnRspAccountContractShareQueryResult(const ATPRspAccountContractShareQueryResultMsg &msg){}

/**
     * @brief 指定合约账户资金查询结果
     * @param[in] ATPRspAccountContractFundQueryResultMsg
     * @type query
     */
    virtual void OnRspAccountContractFundQueryResult(const ATPRspAccountContractFundQueryResultMsg &msg){}

/**
     * @brief 成交汇总查询结果
     * @param[in] ATPRspCollectQueryResultMsg
     * @type query
     */
    virtual void OnRspCollectQueryResult(const ATPRspCollectQueryResultMsg &msg){}

/**
     * @brief 科创板股票新股发行申购额度查询结果
     * @param[in] ATPRspTIBIPOQtyQueryResultMsg
     * @type query
     */
    virtual void OnRspTIBIPOQtyQueryResult(const ATPRspTIBIPOQtyQueryResultMsg &msg){}

/**
     * @brief 期权最大可委托数查询结果
     * @param[in] ATPRspCoverMaxOrderQtyResultQueryResultMsg
     * @type query
     */
    virtual void OnRspCoverMaxOrderQtyResultQueryResult(const ATPRspCoverMaxOrderQtyResultQueryResultMsg &msg){}

/**
     * @brief 同步客户回报结束
     * @param[in] ATPRspSyncBizFinishOtherMsg
     * @type other
     */
    virtual void OnRspSyncBizFinish(const ATPRspSyncBizFinishOtherMsg &msg){}

/**
     * @brief 账户登入应答函数
     * @param[in] ATPRspCustLoginRespOtherMsg
     * @type session
     */
    virtual void OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg &msg){}

/**
     * @brief 账户登出应答函数
     * @param[in] ATPRspCustLogoutRespOtherMsg
     * @type session
     */
    virtual void OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg &msg){}

/**
     * @brief 资金划拨(转入、转出)应答函数
     * @param[in] ATPRspExtFundTransferResultOtherMsg
     * @type other
     */
    virtual void OnRspExtFundTransferResult(const ATPRspExtFundTransferResultOtherMsg &msg){}

/**
     * @brief 融资融券直接还款应答函数
     * @param[in] ATPRspExtRepayResultOtherMsg
     * @type ack
     */
    virtual void OnRspExtRepayResult(const ATPRspExtRepayResultOtherMsg &msg){}

/**
     * @brief 外部业务拒绝函数
     * @param[in] ATPRspBizRejectionOtherMsg
     * @type other
     */
    virtual void OnRspBizRejection(const ATPRspBizRejectionOtherMsg &msg){}

/**
     * @brief 大宗盘后定价业务成交回报函数
     * @param[in] ATPRspAfterHoursPricingTradeERMsg
     * @type deal
     */
    virtual void OnRspAfterHoursPricingTradeER(const ATPRspAfterHoursPricingTradeERMsg &msg){}

/**
     * @brief 大宗成交申报委托业务成交回报函数
     * @param[in] ATPRspBlockTradeTransactionTradeERMsg
     * @type deal
     */
    virtual void OnRspBlockTradeTransactionTradeER(const ATPRspBlockTradeTransactionTradeERMsg &msg){}

/**
     * @brief 大宗定价申报委托业务成交回报函数
     * @param[in] ATPRspBlockTradePricingTradeERMsg
     * @type deal
     */
    virtual void OnRspBlockTradePricingTradeER(const ATPRspBlockTradePricingTradeERMsg &msg){}

/**
     * @brief 期权资金划拨应答函数
     * @param[in] ATPRspOptionFundTransferResultOtherMsg
     * @type other
     */
    virtual void OnRspExtOptionFundTransferResult(const ATPRspOptionFundTransferResultOtherMsg &msg){}

/**
     * @brief 行权预估盈亏查询消息结果
     * @param[in] ATPRspExtQueryResultExerciseProfitLossMsg
     * @type query
     */
    virtual void OnRspExtQueryResultExerciseProfitLoss(const ATPRspExtQueryResultExerciseProfitLossMsg &msg){}

/**
     * @brief 港股通业务成交回报
     * @param[in] ATPRspHKStocksThroughAuctionTradeERMsg
     * @type deal
     */
    virtual void OnRspHKStocksThroughAuctionTradeER(const ATPRspHKStocksThroughAuctionTradeERMsg &msg){}

/**
     * @brief 合并行权最大可委托数查询结果
     * @param[in] ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg
     * @type query
     */
    virtual void OnRspExtQueryResultCombinExerciseMaxOrderQty(const ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg &msg){}

/**
     * @brief 合并行权预估盈亏查询结果
     * @param[in] ATPRspExtQueryResultCombinExerciseProfitLossMsg
     * @type query
     */
    virtual void OnRspExtQueryResultCombinExerciseProfitLoss(const ATPRspExtQueryResultCombinExerciseProfitLossMsg &msg){}

/**
     * @brief 按证券账户划转资金响应
     * @param[in] ATPRspExternalInsTETransFundRespMsg
     * @type other
     */
    virtual void OnRspExternalInsTETransFundResp(const ATPRspExternalInsTETransFundRespMsg &msg){}

/**
     * @brief 期权按证券账户划转资金响应
     * @param[in] ATPRspOptionExternalInsTETransFundRespMsg
     * @type other
     */
    virtual void OnRspOptionExternalInsTETransFundResp(const ATPRspOptionExternalInsTETransFundRespMsg &msg){}

/**
     * @brief 客户密码修改应答
     * @param[in] ATPRspCustPasswdModifyResultMsg
     * @type other
     */
    virtual void OnRspCustPasswdModifyResult(const ATPRspCustPasswdModifyResultMsg &msg){}

/**
     * @brief 增强资金查询结果
     * @param[in] ATPRspExtFundQueryResultMsg
     * @type query
     */
    virtual void OnRspExtFundQueryResult(const ATPRspExtFundQueryResultMsg &msg){}

/**
     * @brief 现货竞价批量委托状态响应
     * @param[in] ATPRspBatchOrderStatusAckMsg
     * @type ack
     */
    virtual void OnRspBatchOrderStatusAck(const ATPRspBatchOrderStatusAckMsg &msg){}

/**
     * @brief 现货竞价批量委托查询结果
     * @param[in] ATPRspBatchOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspBatchOrderQueryResult(const ATPRspBatchOrderQueryResultMsg &msg){}

/**
     * @brief 期权组合策略保证金订单查询结果
     * @param[in] ATPRspCombinationQueryResultMsg
     * @type query
     */
    virtual void OnRspCombinationOrderQueryResult(const ATPRspCombinationQueryResultMsg &msg){}

/**
     * @brief 客户保证金风险信息查询结果
     * @param[in] ATPRspExternalQueryOptionMarginRiskInfoMsg
     * @type query
     */
    virtual void OnRspExternalQueryOptionMarginRiskInfoMsg(const ATPRspExternalQueryOptionMarginRiskInfoMsg &msg){}

/**
     * @brief 期权合约基础信息查询结果
     * @param[in] ATPRspExternalQueryOptionSecurityInfoMsg
     * @type query
     */
    virtual void OnRspExternalQueryOptionSecurityInfoMsg(const ATPRspExternalQueryOptionSecurityInfoMsg &msg){}

/**
     * @brief 期权标的证券基础信息查询结果
     * @param[in] ATPRspExternalQueryOptionUnderlyingSecurityInfoMsg
     * @type query
     */
    virtual void OnRspExternalQueryOptionUnderlyingSecurityInfoMsg(const ATPRspExternalQueryOptionUnderlyingSecurityInfoMsg &msg){}

/**
     * @brief 期权五档行情查询结果
     * @param[in] ATPRspExternalQueryOptionMdSnapShotArrayInfoMsg
     * @type query
     */
    virtual void OnRspExternalQueryOptionMdSnapShotArrayInfoMsg(const ATPRspExternalQueryOptionMdSnapShotArrayInfoMsg &msg){}

/**
     * @brief 锁定委托业务订单结果
     * @param[in] ATPRspExternalQueryLockedAccountContractQtyMsg
     * @type query
     */
    virtual void OnRspExternalQueryLockedAccountContractQtyMsg(const ATPRspExternalQueryLockedAccountContractQtyMsg &msg){}

/**
     * @brief 期权订单查询结果
     * @param[in] ATPRspOptionOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspOptionOrderQueryResult(const ATPRspOptionOrderQueryResultMsg &msg){}

/**
     * @brief 期权成交查询结果
     * @param[in] ATPRspOptionTradeOrderQueryResultMsg
     * @type query
     */
    virtual void OnRspOptionTradeOrderQueryResult(const ATPRspOptionTradeOrderQueryResultMsg &msg){}

/**
     * @brief 标的证券备兑占用查询结果
     * @param[in] ATPRspExtQueryResultLockedAccountContractQtyMsg
     * @type query
     */
    virtual void OnRspLockedAccountContractQtyQueryResult(const ATPRspExtQueryResultLockedAccountContractQtyMsg &msg){}

/**
     * @brief 透传第三方OES通用接口应答
     * @param[in] ATPRspThirdPartPassThroughResultMsg
     * @type other
     */
    virtual void OnRspThirdPartPassThroughResult(const ATPRspThirdPartPassThroughResultMsg &msg){}

/**
     * @brief 信用账户融资保底额度查询结果
     * @param[in] ATPRspQueryMarginTradingPrivateAmoutResultMsg
     * @type query
     */
    virtual void OnRspQueryResultMarginTPAResult(const ATPRspQueryMarginTradingPrivateAmoutResultMsg &msg){}

/**
     * @brief 信用账户融券保底额度查询结果
     * @param[in] ATPRspQuerySecuritiesLendingPrivateAmountResultMsg
     * @type query
     */
    virtual void OnRspQuerySecuritiesLendingPrivateAmountQueryResult(const ATPRspQuerySecuritiesLendingPrivateAmountResultMsg &msg){}

/**
     * @brief 银证转账应答函数
     * @param[in] ATPRspBankSecuritiesFundTransferResultOtherMsg
     * @type other
     */
    virtual void OnRspBankSecuritiesFundTransferResult(const ATPRspBankSecuritiesFundTransferResultOtherMsg &msg){}

/**
     * @brief 一步式银证转账应答函数
     * @param[in] ATPRspCashBankSecuritiesFundTransferResultOtherMsg
     * @type other
     */
    virtual void OnRspCashBankSecuritiesFundTransferResult(const ATPRspCashBankSecuritiesFundTransferResultOtherMsg &msg){}

/**
     * @brief 外部实时资金查询结果
     * @param[in] ATPRspExtRealtimeFundQueryResultMsg
     * @type query
     */
    virtual void OnRspExtRealtimeFundQueryResult(const ATPRspExtRealtimeFundQueryResultMsg &msg){}

/**
     * @brief 新股清单查询结果
     * @param[in] ATPRspExtQueryResultNewShareInfoMsg
     * @type query
     */
    virtual void OnRspExtQueryResultNewShareInfo(const ATPRspExtQueryResultNewShareInfoMsg &msg){}

/**
     * @brief 证券信息查询结果
     * @param[in] ATPRspExtQueryResultSecurityInfoMsg
     * @type query
     */
    virtual void OnRspExtQueryResultSecurityInfo(const ATPRspExtQueryResultSecurityInfoMsg &msg){}

/**
     * @brief 增强股份查询结果
     * @param[in] ATPRspExtQueryResultShareExMsg
     * @type query
     */
    virtual void OnRspExtQueryResultShareEx(const ATPRspExtQueryResultShareExMsg &msg){}

/**
     * @brief 增强资产查询结果
     * @param[in] ATPRspExtQueryResultAssetExMsg
     * @type query
     */
    virtual void OnRspExtQueryResultAssetEx(const ATPRspExtQueryResultAssetExMsg &msg){}

/**
     * @brief 算法母单执行回报响应
     * @param[in] ATPRspAlgoParentExecReportMsg
     * @type ack
     */
    virtual void OnRspAlgoParentExecReport(const ATPRspAlgoParentExecReportMsg &msg){}

/**
     * @brief 算法母单查询结果
     * @param[in] ATPRspExternalQueryResultAlgoParentMsg
     * @type query
     */
    virtual void OnRspExternalQueryResultAlgoParent(const ATPRspExternalQueryResultAlgoParentMsg &msg){}

/**
     * @brief 算法子单查询结果
     * @param[in] ATPRspExternalQueryResultAlgoOrderMsg
     * @type query
     */
    virtual void OnRspExternalQueryResultAlgoOrder(const ATPRspExternalQueryResultAlgoOrderMsg &msg){}

/**
     * @brief 债券协商成交转发成交申报查询结果
     * @param[in] ATPRspExtQueryResultOrderForwardInfoMsg
     * @type query
     */
    virtual void OnRspExtQueryResultOrderForwardInfo(const ATPRspExtQueryResultOrderForwardInfoMsg &msg){}

/**
     * @brief 债券协商成交委托成交回报
     * @param[in] ATPRspBondConsultTradeERMsg
     * @type deal
     */
    virtual void OnRspBondConsultTradeER(const ATPRspBondConsultTradeERMsg &msg){}

/**
     * @brief 外部持仓查询结果
     * @param[in] ATPRspExtStockQueryResultMsg
     * @type query
     */
    virtual void OnRspExtStockQueryResult(const ATPRspExtStockQueryResultMsg &msg){}

/**
     * @brief 外部持仓划拨应答函数
     * @param[in] ATPRspExtShareTransferResultMsg
     * @type other
     */
    virtual void OnRspExtShareTransferResult(const ATPRspExtShareTransferResultMsg &msg){}
};

#endif