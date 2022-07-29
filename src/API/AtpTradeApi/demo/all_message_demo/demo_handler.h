
#ifndef _DEMO_HANDLER_H_
#define _DEMO_HANDLER_H_

#include <atp_trade_api.h>
#include <atomic>


//回调处理函数,派生自 ATPTradeHandler
class DemoATPTradeHandler :public ATPTradeHandler
{
	
public:

	DemoATPTradeHandler(volatile/*std::atomic<*/bool/*>*/&login_succes, volatile/*std::atomic<*/bool/*>*/&exit)
		:m_login_succes(login_succes),m_exit(exit)
	{
	 
	}

	/**
	 * @brief 连接成功事件
	 * @param[in] reason 原因
	 */
	virtual void OnConnected(const std::string& reason) override;

	/**
	 * @brief 连接失败事件
	 * @param[in] reason 失败原因
	 */
	virtual void OnConnectFailure(const std::string& reason) override ;

	/**
	 * @brief 连接超时时间
	 * @param[in] reason 超时原因
	 */
	virtual void OnConnectTimeOut(const std::string& reason) override ;

	/**
	 * @brief 心跳超时事件
	 * @param[in] reason 超时原因
	 */
	virtual void OnHeartbeatTimeout(const std::string& reason) override ;

	/**
	 * @brief  成功登录AGW事件
	 * @param[in] reason 成功原因
	 */
	virtual void OnLogin(const std::string& reason) override ;

	/**
	 * @brief  登出事件
	 * @param[in] reason 登出原因
	 */
	virtual void OnLogout(const std::string& reason) override ;

	/**
	 * @brief 重连结束
	 * @param[in] reason 重连结束原因
	 * @note 重连结束后需要使用者主动在发起连接
	 */
	virtual void OnEndOfConnection(const std::string& reason) override ;

	/**
	 * @brief  API内部错误
	 * @param[in] reason 错误原因
	 */
	virtual void OnError(const std::string& reason) override ;

	/**
	 * @brief 连接关闭
	 * @param[in] reason 关闭原因
	 */
	virtual void OnClosed(const std::string& reason) override ;

	/**
	 * @brief API内部日志回调函数
	 * @param[in] level 日志级别
	 * @param[in] message 日志内容
	 */
	virtual void OnLog(ATPLogLevel level, const std::string& message) override ;

	/**
	 * @brief 委托状态内部响应
	 * @param[in] order_status_ack& 确认消息
	 */
	virtual void OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack) override ;
	
	/**
	 * @brief 委托状态响应
	 * @param[in] order_status_ack& 确认消息
	 */
	virtual void OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack) override ;
	
	/**
	 * @brief 报价委托状态响应
	 * @param[in] order_status_ack& 确认消息  
	 */
	virtual void OnRspQuoteStatusAck(const ATPRspQuoteStatusAckMsg& order_status_ack) override ;
	
	/**
	 * @brief 期权多腿委托状态内部响应
	 * @param[in] order_status_ack& 确认消息  
	 */
	virtual void OnRspNoLegsStatusInternalAck(const ATPRspNoLegsStatusAckMsg& order_status_ack) override ;
	
	/**
	 * @brief 期权多腿委托状态响应
	 * @param[in] order_status_ack& 确认消息  
	 */
	virtual void OnRspNoLegsStatusAck(const ATPRspNoLegsStatusAckMsg& order_status_ack) override ;
	
	/**
	 * @brief 现货成交回报
	 * @param[in] cash_auction_trade_er 现货成交回报结果消息
	 */
	virtual void OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er) override ;

	/**
	 * @brief 融资融券交易成交回报
	 * @param[in] credit_trade_er 融资融券交易成交回报结果消息
	 */
	virtual void OnRspCreditTradeER(const ATPRspCreditTradeERMsg& credit_trade_er) override;

	/**
	 * @brief 融资融券交易偿还成交回报
	 * @param[in] margin_trade_repay_er 融资融券交易成交回报结果消息
	 */
	virtual void OnRspMarginRepayTradeER(const ATPRspMarginRepayTradeERMsg& margin_trade_repay_er) override;

	/**
	 * @brief 期权集中竞价业务成交回报
	 * @param[in] option_auction_trade_er 期权集中竞价业务成交回报结果消息
	 */
	virtual void OnRspOptionAuctionTradeER(const ATPRspOptionAuctionTradeERMsg& option_auction_trade_er) override;

	/**
	 * @brief 质押式回购集中竞价业务成交回报
	 * @param[in] repo_auction_trade_er 质押式回购集中竞价业务成交回报结果消息
	 */
	virtual void OnRspRepoAuctionTradeER(const ATPRspRepoAuctionTradeERMsg& repo_auction_trade_er) override ;
	
	/**
	 * @brief ETF申赎业务成交回报
	 * @param[in] etf_redemption_trade_er ETF申赎业务成交回报结果消息
	 */
	virtual void OnRspETFRedemptionTradeER(const ATPRspETFRedemptionTradeERMsg& etf_redemption_trade_er) override ;
	
	/**
	 * @brief 债券分销业务成交回报
	 * @param[in] bond_distribution_trade_er 债券分销业务成交回报结果消息
	 */
	virtual void OnRspBondDistributionTradeER(const ATPRspBondDistributionTradeERMsg& bond_distribution_trade_er) override ;
	
	/**
	 * @brief 分级基金实时分拆合并业务成交回报
	 * @param[in] structured_fund_trade_er 分级基金实时分拆合并业务成交回报结果消息
	 */
	virtual void OnRspStructuredFundTradeER(const ATPRspStructuredFundTradeERMsg& structured_fund_trade_er) override ;
	
	/**
	 * @brief 开放式基金申赎业务成交回报
	 * @param[in] lof_trade_er 开放式基金申赎业务成交回报结果消息
	 */
	virtual void OnRspLOFTradeER(const ATPRspLOFTradeERMsg& lof_trade_er) override ;
	
	/**
	 * @brief 科创板股票盘后定价业务成交回报
	 * @param[in] tib_after_hour_auction_trade_er 科创板股票盘后定价业务成交回报结果消息
	 */
	virtual void OnRspTibAfterHourAuctionTradeER(const ATPRspTibAfterHourAuctionTradeERMsg& tib_after_hour_auction_trade_er) override ;

	/**
	 * @brief 质押入出库业务成交回报
	 * @param[in] pledge_in_or_out_trade_er 质押入出库业务成交回报结果消息
	 */
	virtual void OnRspPledgeInOrOutTradeER(const ATPRspPledgeInOrOutTradeERMsg& pledge_in_or_out_trade_er) override;

	/**
	 * @brief 最大可委托数查询结果
	 * @param[in] max_order_query_result 最大可委托数查询结果消息
	 */
	virtual void OnRspMaxOrderQueryResult(const ATPRspMaxOrderQueryResultMsg& max_order_query_result) override ;


	/**
	 * @brief 订单查询结果
	 * @param[in] order_query_result 订单查询结果消息
	 */
	virtual void OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg& order_query_result) override ;

	/**
	 * @brief 融资融券合约明细查询结果
	 * @param[in] contract_specifications_query_result 融资融券合约明细查询结果消息
	 */
	virtual void OnRspQueryContractSpecificationsQueryResult(const ATPRspQueryContractSpecificationsResultMsg& contract_specifications_query_result) override;


	/**
	 * @brief 融资融券信用额度查询结果
	 * @param[in] credit_amount_query_result 融资融券信用额度查询结果消息
	 */
	virtual void OnRspQueryCreditAmountQueryResult(const ATPRspQueryCreditAmountResultMsg& credit_amount_query_result) override;

	/**
	 * @brief 融资融券保证金可用查询结果
	 * @param[in] margin_useable_query_result 融资融券保证金可用查询结果消息
	 */
	virtual void OnRspQueryMarginUseableQueryResult(const ATPRspQueryMarginUseableResultMsg& margin_useable_query_result) override;

	/**
	 * @brief 融资融券资金负债查询结果
	 * @param[in] fund_liability_query_result 融资融券资金负债查询结果消息
	 */
	virtual void OnRspQueryFundLiabilityQueryResult(const ATPRspQueryFundLiabilityResultMsg& fund_liability_query_result) override;

	/**
	 * @brief 融资融券股份负债查询结果
	 * @param[in] share_liability_query_result 融资融券股份负债查询结果消息
	 */
	virtual void OnRspQueryShareLiabilityQueryResult(const ATPRspQueryShareLiabilityResultMsg& share_liability_query_result) override;

	/**
	 * @brief 融资融券合约管理查询结果
	 * @param[in] contract_specifications_manager_query_result 融资融券合约管理查询结果消息
	 */
	virtual void OnRspQueryContractSpecificationsManagerQueryResult(const ATPRspQueryContractSpecificationsManagerResultMsg& contract_specifications_manager_query_result) override;

	/**
	 * @brief 融资融券标的清单查询结果
	 * @param[in] margin_underlying_query_result 融资融券标的清单查询结果消息
	 */
	virtual void OnRspQueryMarginUnderlyingQueryResult(const ATPRspQueryMarginUnderlyingResultMsg& margin_underlying_query_result) override;

	/**
	 * @brief 融资融券客户信用资料查询结果
	 * @param[in] credit_info_query_result 融资融券客户信用资料查询结果消息
	 */
	virtual void OnRspQueryCreditInfoQueryResult(const ATPRspQueryCreditInfoResultMsg& credit_info_query_result) override;

	/**
	 * @brief 融资融券资产信息查询结果
	 * @param[in] fund_assets_info_query_result 融资融券资产信息查询结果消息
	 */
	virtual void OnRspQueryFundAssetsInfoQueryResult(const ATPRspQueryFundAssetsInfoResultMsg& fund_assets_info_query_result) override;

	/**
	 * @brief 成交查询结果
	 * @param[in] trade_order_query_result 成交查询结果消息
	 */
	virtual void OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg& trade_order_query_result) override ;


	/**
	 * @brief 资金查询结果
	 * @param[in] fund_query_result 资金查询结果消息
	 */
	virtual void OnRspFundQueryResult(const ATPRspFundQueryResultMsg& fund_query_result) override ;


	/**
	 * @brief 股份查询结果
	 * @param[in] share_query_result 股份查询结果消息
	 */
	virtual void OnRspShareQueryResult(const ATPRspShareQueryResultMsg& share_query_result) override ;


	/**
	 * @brief 新股发行申购额度查询结果
	 * @param[in] ipo_qty_query_result 新股发行申购额度查询结果消息
	 */
	virtual void OnRspIPOQtyQueryResult(const ATPRspIPOQtyQueryResultMsg& ipo_qty_query_result) override ;


	/**
	 * @brief 合约账户持仓查询结果
	 * @param[in] account_contract_share_query_result 合约账户持仓查询结果消息
	 */
	virtual void OnRspAccountContractShareQueryResult(const ATPRspAccountContractShareQueryResultMsg& account_contract_share_query_result) override ;


	/**
	 * @brief 指定合约账户资金查询结果
	 * @param[in] account_contract_fund_query_result 指定合约账户资金查询结果消息
	 */
	virtual void OnRspAccountContractFundQueryResult(const ATPRspAccountContractFundQueryResultMsg& account_contract_fund_query_result) override ;

	/**
	 * @brief 报价订单查询结果
	 * @param[in] result 结果消息 
	 */
	virtual void OnRspQuoteOrderQueryResult(const ATPRspQuoteQueryResultMsg& result) override ;

	/**
	 * @brief 期权组合策略保证金组合查询结果
	 * @param[in] result 结果消息 
	 */
	virtual void OnRspCombinationPositionQueryResult(const ATPRspCombinationQueryResultMsg& result) override ;

	/**
	 * @brief 成交汇总查询结果
	 * @param[in] collect_query_result 成交汇总查询结果消息
	 */
	virtual void OnRspCollectQueryResult(const ATPRspCollectQueryResultMsg& collect_query_result) override ;


	/**
	 * @brief 科创板股票新股发行申购额度查询结果
	 * @param[in] tib_ipo_qty_query_result 科创板股票新股发行申购额度查询结果消息
	 */
	virtual void OnRspTIBIPOQtyQueryResult(const ATPRspTIBIPOQtyQueryResultMsg& tib_ipo_qty_query_result) override ;


	/**
	 * @brief 期权最大可委托数查询结果
	 * @param[in] cover_max_order_qty_result_query_result 期权最大可委托数查询结果消息消息
	 */
	virtual void OnRspCoverMaxOrderQtyResultQueryResult(const ATPRspCoverMaxOrderQtyResultQueryResultMsg& cover_max_order_qty_result_query_result) override ;


	/**
	 * @brief 同步客户回报结束
	 * @param[in] sync_biz_finish 同步客户回报结束消息
	 */
	virtual void OnRspSyncBizFinish(const ATPRspSyncBizFinishOtherMsg& sync_biz_finish) override ;

	/**
	* @brief 客户登入验证应答函数
	* @param[in]  cust_login_resp 客户登入验证应答消息
	*/
	virtual void OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg& cust_login_resp) override ;

	/**
	* @brief 客户登出应答函数
	* @param[in]  cust_logout_resp 客户登出应答消息
	*/
	virtual void OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg& cust_logout_resp) override ;

	/**
	* @brief 资金划拨(转入、转出)应答函数
	* @param[in]  ext_fund_transfer_result 资金划拨(转入、转出)应答消息
	*/
	virtual void OnRspExtFundTransferResult(const ATPRspExtFundTransferResultOtherMsg& ext_fund_transfer_result) override ;

	/**
	* @brief 融资融券直接还款应答函数
	* @param[in]  ext_repay_result 融资融券直接还款应答消息
	*/
	virtual void OnRspExtRepayResult(const ATPRspExtRepayResultOtherMsg& ext_repay_result) override;

	/**
	* @brief 外部业务拒绝函数
	* @param[in]  biz_rejection 外部业务拒绝消息
	*/
	virtual void OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection) override ;
	
	/**
	* @brief 大宗盘后定价业务成交回报函数
	* @param[in]  after_hours_pricing_trade_er 大宗盘后定价业务成交回报消息
	*/
	virtual void OnRspAfterHoursPricingTradeER(const ATPRspAfterHoursPricingTradeERMsg& after_hours_pricing_trade_er) override ;

	
	/**
	* @brief 大宗成交申报委托业务成交回报函数
	* @param[in]  block_trade_transaction_trade_er 大宗成交申报委托业务成交回报消息
	*/
	virtual void OnRspBlockTradeTransactionTradeER(const ATPRspBlockTradeTransactionTradeERMsg& block_trade_transaction_trade_er) override ;
	
	/**
	* @brief 大宗定价申报委托业务成交回报函数
	* @param[in]  block_trade_pricing_trade_er 大宗定价申报委托业务成交回报消息
	*/
	virtual void OnRspBlockTradePricingTradeER(const ATPRspBlockTradePricingTradeERMsg& block_trade_pricing_trade_er) override ;
	
	/**
	* @brief 期权资金划拨应答函数
	* @param[in]  ext_option_fund_transfer_result 资金划拨应答消息
	*/
	virtual void OnRspExtOptionFundTransferResult(const ATPRspOptionFundTransferResultOtherMsg& ext_option_fund_transfer_result) override;
	             
	/**
	* @brief 行权预估盈亏查询消息结果
	* @param[in]  result 行权预估盈亏查询应答消息
	*/
	virtual void  OnRspExtQueryResultExerciseProfitLoss(const ATPRspExtQueryResultExerciseProfitLossMsg& result) override;

	/**
	 * @brief 港股通业务成交回报
	 * @param[in] hkstocksthrough_auction_trade_er 港股通业务成交回报结果消息 
	 */
	virtual void OnRspHKStocksThroughAuctionTradeER(const ATPRspHKStocksThroughAuctionTradeERMsg& hkstocksthrough_auction_trade_er) override;

	/**
	* @brief 合并行权最大可委托数查询结果
	* @param[in]  result 合并行权最大可委托数查询结果
	*/
	virtual void  OnRspExtQueryResultCombinExerciseMaxOrderQty(const ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg& result) override;

	/**
	* @brief 合并行权预估盈亏查询结果
	* @param[in]  result 合并行权预估盈亏查询结果
	*/
	virtual void  OnRspExtQueryResultCombinExerciseProfitLoss(const ATPRspExtQueryResultCombinExerciseProfitLossMsg& result) override;
	/**
	* @brief 		透传第三方OES通用接口应答
	* @param[in]	result 透传第三方OES通用接口应答
	*/
	virtual void OnRspThirdPartPassThroughResult(const ATPRspThirdPartPassThroughResultMsg& result) override;
	/**
	* @brief 		银证转账应答函数
	* @param[in]	result 银证转账应答
	*/
	virtual void OnRspBankSecuritiesFundTransferResult(const ATPRspBankSecuritiesFundTransferResultOtherMsg& result) override;

	private:
		volatile/*std::atomic<*/bool/*>*/&m_login_succes;
		volatile/*std::atomic<*/bool/*>*/&m_exit;
};


#endif
