
#include "demo_send_order.h"
#include <string.h>
#include <cstring>
//#include <chrono>
#include <sstream>

#include <atp_trade_api.h>

//保存每个业务消息的容器
std::map<ATPClientSeqIDType, ATPReqCashAuctionOrderMsg*> seq_cash_auction_map;
std::map<ATPClientSeqIDType, ATPReqCreditOrderMsg*> seq_credit_map;
std::map<ATPClientSeqIDType, ATPReqMarginNoTradeOrderMsg*> seq_margin_no_trade_map;
std::map<ATPClientSeqIDType, ATPReqMarginRepayOrderMsg*> seq_margin_repay_map;
std::map<ATPClientSeqIDType, ATPReqRepoAuctionOrderMsg*> seq_repo_auction_map;
std::map<ATPClientSeqIDType, ATPReqBondDistributionOrderMsg*> seq_bond_distribution_map;
std::map<ATPClientSeqIDType, ATPReqOptionAuctionOrderMsg*> seq_option_auction_map;

std::map<ATPClientSeqIDType, ATPReqOptionQuoteOrderMsg*> seq_quote_order_map;
std::map<ATPClientSeqIDType, ATPReqETFRedemptionOrderMsg*> seq_etf_redemption_map;
std::map<ATPClientSeqIDType, ATPReqIssueOrderMsg*> seq_issue_map;
std::map<ATPClientSeqIDType, ATPReqRightsIssueOrderMsg*> seq_rights_issue_map;
std::map<ATPClientSeqIDType, ATPReqSwapPutbackOrderMsg*> seq_swap_putback_map;
std::map<ATPClientSeqIDType, ATPReqOptionExerciseOrderMsg*> seq_option_exercise_map;
std::map<ATPClientSeqIDType, ATPReqCoveredLockOrderMsg*> seq_covered_lock_order_map;
std::map<ATPClientSeqIDType, ATPReqTurnToDisposalOrderMsg*> seq_turn_to_disposal_map;

std::map<ATPClientSeqIDType, ATPReqDesignationOrderMsg*> seq_designation_map;
std::map<ATPClientSeqIDType, ATPReqDesignatedTransactionOrderMsg*> seq_designated_transaction_map;
std::map<ATPClientSeqIDType, ATPReqLOFOrderMsg*> seq_lof_map;
std::map<ATPClientSeqIDType, ATPReqTenderOfferOrderMsg*> seq_tender_offer_map;
std::map<ATPClientSeqIDType, ATPReqNetVotingOrderMsg*> seq_net_voting_map;
std::map<ATPClientSeqIDType, ATPReqPasswordServiceOrderMsg*> seq_password_service_map;
std::map<ATPClientSeqIDType, ATPReqWarrantExerciseOrderMsg*> seq_warrant_exercise_map;
std::map<ATPClientSeqIDType, ATPReqStructuredFundOrderMsg*> seq_structured_fund_map;
std::map<ATPClientSeqIDType, ATPReqOptionCombinationOrderMsg*> seq_combination_map;
std::map<ATPClientSeqIDType, ATPReqTibAfterHourAuctionOrderMsg*> seq_tib_after_hour_auction_map;
std::map<ATPClientSeqIDType, ATPReqPledgeInOrOutOrderMsg*> seq_pledge_in_or_out_map;
std::map<ATPClientSeqIDType, ATPReqCancelOrderMsg*> seq_cancel_map;
std::map<ATPClientSeqIDType, ATPReqBatchCancelOrderMsg*> seq_batch_cancel_map;
std::map<ATPClientSeqIDType, ATPReqFDEPCancelOrderMsg*> seq_fdep_cancel_map;
std::map<ATPClientSeqIDType, ATPReqOptionCancelOrderMsg*> seq_option_cancel_map;
std::map<ATPClientSeqIDType, ATPReqCoverMaxOrderQtyQueryMsg*> seq_cover_max_order_qty_query_map;
std::map<ATPClientSeqIDType, ATPReqExtOptionFundTransfeOtherMsg*> seq_ext_option_fund_transfe_other_map;
std::map<ATPClientSeqIDType, ATPReqOrderQueryMsg*> seq_order_query_map;
std::map<ATPClientSeqIDType, ATPReqTradeOrderQueryMsg*> seq_trade_order_query_map;
std::map<ATPClientSeqIDType, ATPReqFundQueryMsg*> seq_fund_query_map;
std::map<ATPClientSeqIDType, ATPReqShareQueryMsg*> seq_share_query_map;
std::map<ATPClientSeqIDType, ATPReqIPOQtyQueryMsg*> seq_ipo_qty_query_map;
std::map<ATPClientSeqIDType, ATPReqAccountContractShareQueryMsg*> seq_account_contract_share_query_map;
std::map<ATPClientSeqIDType, ATPReqAccountContractFundQueryMsg*> seq_account_contract_fund_query_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryExerciseProfitLossMsg*> seq_ext_query_exercise_profit_loss_map;
std::map<ATPClientSeqIDType, ATPReqTradeCollectQueryMsg*> seq_trade_collect_query_map;
std::map<ATPClientSeqIDType, ATPReqTIBIPOQtyQueryMsg*> seq_tib_ipo_qty_query_map;
std::map<ATPClientSeqIDType, ATPReqOptionCombinationExerciseOrderMsg*> seq_combination_exercise_map;
std::map<ATPClientSeqIDType, ATPReqNewMaxOrderQtyQueryMsg*> seq_new_max_order_qty_query_map;
std::map<ATPClientSeqIDType, ATPReqCustLoginOtherMsg*> seq_cust_login_map;
std::map<ATPClientSeqIDType, ATPReqCustLogoutOtherMsg*> seq_cust_logout_map;
std::map<ATPClientSeqIDType, ATPReqExtFundTransferOtherMsg*> seq_ext_fund_transfer_map;
std::map<ATPClientSeqIDType, ATPReqExtRepayOtherMsg*> seq_ext_repay_map;
std::map<ATPClientSeqIDType, ATPReqSyncBizOtherMsg*> seq_sync_biz_map;
std::map<ATPClientSeqIDType, ATPReqQuoteQueryMsg*> seq_quote_query_map;
std::map<ATPClientSeqIDType, ATPReqCombinationQueryMsg*> seq_combination_query_map;
std::map<ATPClientSeqIDType, ATPReqAfterHoursPricingOrderMsg*> seq_after_hours_pricing_map;
std::map<ATPClientSeqIDType, ATPReqBlockTradeIntentionOrderMsg*> seq_block_trade_intention_map;
std::map<ATPClientSeqIDType, ATPReqBlockTradeTransactionOrderMsg*> seq_block_trade_transaction_map;
std::map<ATPClientSeqIDType, ATPReqBlockTradePricingOrderMsg*> seq_block_trade_pricing_map;
std::map<ATPClientSeqIDType, ATPReqBlockTradeCancelOrderMsg*> seq_block_trade_cancel_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryContractSpecificationsMsg*> seq_ext_query_contract_specifications_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryCreditAmountMsg*> seq_ext_query_credit_amount_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryMarginUseableMsg*> seq_ext_query_margin_useable_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryFundLiabilityMsg*> seq_ext_query_fund_liability_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryShareLiabilityMsg*> seq_ext_query_share_liability_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryContractSpecificationsManagerMsg*> seq_ext_query_contract_specifications_manager_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryMarginUnderlyingMsg*> seq_ext_query_margin_underlying_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryCreditInfoMsg*> seq_ext_query_credit_info_map;
std::map<ATPClientSeqIDType, ATPReqExtQueryFundAssetsInfoMsg*> seq_ext_query_fund_assets_info_map;
std::map<ATPClientSeqIDType, ATPReqHKStocksThroughAuctionOrderMsg*> seq_hkstocksthrough_auction_map;
std::map<ATPClientSeqIDType, ATPReqHKVotingOrderMsg*> seq_hk_voting_map;
std::map<ATPClientSeqIDType, ATPReqHKCorporateActionOrderMsg*> seq_hk_corporate_action_map;
std::map<ATPClientSeqIDType, ATPReqHKDesignationTransferOrderMsg*> seq_hk_designation_transfer_map;
std::map<ATPClientSeqIDType, ATPReqHKCorporateActionCancelOrderMsg*> seq_hk_corporate_action_cancel_map;


std::map<ATPClientSeqIDType, ATPReqOptionCoveredTransUncoveredOrderMsg*> seq_optioncoveredtransuncoveredorder_map;    // ///< 期权普通与备兑互转订单
std::map<ATPClientSeqIDType, ATPReqExtQueryCombinExerciseMaxOrderQtyMsg*> seq_extquerycombinexercisemaxorderqty_map;    // ///< 合并行权最大可委托数查询消息
std::map<ATPClientSeqIDType, ATPReqExtQueryCombinExerciseProfitLossMsg*> seq_extquerycombinexerciseprofitloss_map;    // ///< 合并行权预估盈亏查询消息
std::map<ATPClientSeqIDType, ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg*> seq_externalqueryoptioncombinationmaxorderqty_map;    // ///< 期权组合策略最大可委托数查询消息

std::map<ATPClientSeqIDType, ATPReqCustPasswdModifyMsg*> seq_custpasswdmodify_map;    // ///< 客户密码修改请求
std::map<ATPClientSeqIDType, ATPReqExternalInsTETransFundReqMsg*> seq_externalinstetransfundreq_map;    // ///< 按证券账户划转资金请求消息
std::map<ATPClientSeqIDType, ATPReqExtFundQueryMsg*> seq_extfundqueryreq_map;    // ///< 增强资金查询请求消息
std::map<ATPClientSeqIDType, ATPReqThirdPartPassThroughMsg*> seq_third_part_pass_through_map;
std::map<ATPClientSeqIDType, ATPReqTradeOrderExQueryMsg*> seq_trade_order_ex_query_map;
std::map<ATPClientSeqIDType, ATPReqBankSecuritiesFundTransferOtherMsg*> seq_bank_securities_fund_transfer_other_map;
//使用到的全局变量
//使用到的全局变量

extern	int64_t						number;
extern	uint16_t				    send_to;
extern	std::string					user;

extern	std::map<int32_t, int32_t> report_sync;//保存同步回报信息

extern	/*std::atomic<*/bool/*>*/          g_exit; //是否登录成功
extern	/*std::atomic<*/bool/*>*/          g_login_succes;
extern  ATPTradeAPI					*atp_trade_api_client ;
/*
 ATPTradeAPI 对象
 */

extern	ATPCoveredOrUncoveredType	covered_or_uncovered;
extern	ATPDesignationTransTypeType	designation_trans_type;
extern	ATPBusinessTypeType			business_type;
extern	ATPReturnSeqType        	return_seq;
extern	ATPTransferWayType	        transfer_way;
extern	ATPAgwPartitionType	        partition;
extern	ATPOrderQueryConditionType 	order_query_condition;
extern	ATPMarketIDType				market_id;
extern	ATPValidationCodeType		validation_code;
extern	ATPIndexType				g_index;
extern	ATPPriceType				price;
extern	ATPQtyType					order_qty;
extern	ATPClOrdNOType				cl_ord_no;
extern	ATPClOrdNOType				orig_cl_ord_no;
extern	ATPAmtType					cash_order_qty;
extern	ATPTotalNumType				return_num;
extern	ATPAmtType					transfer_value;
extern	ATPReportIndexType			query_index;
extern	ATPGeneralMeetingSeqType	general_meeting_seq;
extern	std::string					password;
extern	std::string					security_id;
extern	std::string					cust_id;
extern	std::string					fund_account_id;
extern	std::string					account_id;
extern	std::string					dest_account_id;
extern	std::string					transferee_pbu_id;
extern	std::string					designated_pbu_id;
extern	std::string					tenderer;
extern	std::string					voting_proposal;
extern	std::string					voting_segment;
extern	std::string					currency;
extern	std::string					remark;
extern	std::string					branch_id;
extern	ATPSideType					side;
extern	ATPOrdTypeType				order_type;
extern	ATPOrderWayType				order_way;
extern	ATPEnforceFlagType			enforce_flag;
extern	ATPPositionEffectType		position_effect;
extern	ATPVotingPreferenceType		voting_preference;
extern	ATPBoolType					is_account_partition;
extern  ATPQtyType					min_qty;
extern  ATPMaxPriceLevelsType		max_price_levels;
extern  ATPTimeInForceType			time_in_force;
extern  ATPPriceType				stop_px;
extern  ATPPositionEffectType		bid_position_effect,offer_position_effect;
extern	ATPQtyType					offer_order_qty ;
extern	ATPPriceType				offer_price ;
extern  std::string					quote_req_id ;
extern	std::string 				user_info ;
extern	std::string					account_sub_code;
extern	ATPLegSideType				leg_side1,leg_side2 ;
extern  std::string 				strategy_id,secondary_order_id ;
extern 	std::string					leg_security_id1,leg_security_id2;
extern  ATPPricePropertyType        price_property;
extern  ATPTrdSubTypeType           trd_sub_type;
extern  std::string                 contactor;
extern  std::string                 contact_info;
extern  std::string                 confirm_id;
extern  std::string                 counter_party_pbu_id;
extern 	std::string					counter_party_account_id;
extern  ATPPricingType              pricing;
extern  ATPMarketIDType leg_market_id1;
extern  ATPSideType   leg_side1;
extern  ATPCoveredOrUncoveredType  leg_covered_or_uncovered1; ///<腿备兑标签

extern 	std::string					client_feature_code;
extern	ATPLotTypeType				lot_type;
extern	ATPAnnouncementNumberType	announcement_number;
extern	ATPProxyFormNumberType		proxy_form_number;
extern	ATPAffirmativeVotesType		affirmative_votes;
extern	ATPNegativeVotesType		negative_votes;
extern	ATPAbstentionVotesType		abstention_votes;
extern	std::string					corporate_action_id;
extern	std::string 				entitlement_number;
extern  std::string                 dest_pbu_id;
extern  ATPRepayWayType				repay_way;
extern  ATPAmtType					repay_value;

extern	ATPCloseOutFlagType				close_out_flag;
extern	std::string					cash_margin_set;
extern	ATPDate8Type				opening_date;
extern	std::string					opening_serial_num;
extern	ATPQueryContentsType			query_contents;
extern	ATPCRDUnderlyingTypeType	underlying_type;
extern	std::string					password_msg;
extern  std::string disposal_pbu, disposal_account_id;
extern  APIExerciseLegType  no_legs_array; //< 行权多腿信息组合
extern uint32_t no_legs;

extern std::string old_password;
extern ATPMarketIDType                      fund_out_market_id;            ///< 资金转出市场ID (必填)
extern std::string                     fund_in_account_id;            ///< 资金转入账户ID (必填)
extern ATPMarketIDType                      fund_in_market_id;             ///< 资金转入市场ID (必填)
extern ATPAmtType                           value;                         ///< 划拨金额 (必填)
extern std::string					extra_data;
extern std::string					trade_root_num;

#define SENDMESSAGE(function_name,p) \
auto ec = atp_trade_api_client->function_name(p); \
if (ec != kSuccess) \
{ \
	std::cout << "send Error" << ec << std::endl; \
	/*std::this_thread::sleep_for(std::chrono::seconds(5)); \*/\
} 

template<size_t N>
void SET(char (&a)[N],const std::string& str)
{
	strncpy(a, str.c_str(), N);
}

template<typename T1,typename T2>
void  SET(T1& t1,const T2& t2)
{
	static_assert(std::is_same<T1, T2>::value, "Types should be  compatible");
	t1 = t2;
}

/**
* @brief 获取系列号
*/
uint64_t GenerateClientSeqID()
{
	//std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	//std::chrono::time_point< std::chrono::system_clock, std::chrono::nanoseconds>  ns =
	//	std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
	//return ns.time_since_epoch().count();
	static uint64_t  i=0;
	return ++i;
}

/**
 * @brief 调用V217版本的上行消息函数
 * @@param[in] ord_type 目标业务类型
 */


//将收到的消息发送至ATPTradeAPI进行处理

void SendCashOrder()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCashAuctionOrderMsg* p = new ATPReqCashAuctionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_type = order_type;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->stop_px = stop_px;
	p->min_qty = min_qty;
	p->max_price_levels = max_price_levels;
	p->time_in_force = time_in_force;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_cash_auction_map[seq_id] = p;

	SENDMESSAGE(ReqCashAuctionOrder,p);

}

void SendCreditOrder()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCreditOrderMsg* p = new ATPReqCreditOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->order_type = order_type;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user_info.c_str(), 17);
	memset(p->password, 0x0, 129);
	strncpy(p->password, password_msg.c_str(), 129);
	p->enforce_flag = enforce_flag;
	p->stop_px = stop_px;
	p->min_qty = min_qty;
	p->max_price_levels = max_price_levels;
	p->time_in_force = time_in_force;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_credit_map[seq_id] = p;

	SENDMESSAGE(ReqCreditOrder,p);

}

void SendMarginNoTradeOrder()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqMarginNoTradeOrderMsg* p = new ATPReqMarginNoTradeOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user_info.c_str(), 17);
	strncpy(p->password, password_msg.c_str(), 129);
	p->enforce_flag = enforce_flag;
	strncpy(p->dest_account_id, dest_account_id.c_str(), 13);
	strncpy(p->dest_pbu_id, dest_pbu_id.c_str(), 7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_margin_no_trade_map[seq_id] = p;

	SENDMESSAGE(ReqMarginNoTradeOrder,p);

}

void SendMarginRepayOrder()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqMarginRepayOrderMsg* p = new ATPReqMarginRepayOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->order_type = order_type;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user_info.c_str(), 17);
	strncpy(p->password, password_msg.c_str(), 129);
	p->enforce_flag = enforce_flag;
	p->stop_px = stop_px;
	p->min_qty = min_qty;
	p->max_price_levels = max_price_levels;
	p->time_in_force = time_in_force;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_margin_repay_map[seq_id] = p;

	SENDMESSAGE(ReqMarginRepayOrder,p);

}

void SendRepoAuction()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqRepoAuctionOrderMsg* p = new ATPReqRepoAuctionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_repo_auction_map[seq_id] = p;

	SENDMESSAGE(ReqRepoAuctionOrder,p);	
}

void SendBondDistribution()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBondDistributionOrderMsg* p = new ATPReqBondDistributionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_bond_distribution_map[seq_id] = p;

	SENDMESSAGE(ReqBondDistributionOrder,p);


	
}

void SendOptionAuction()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionAuctionOrderMsg* p = new ATPReqOptionAuctionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->side = side;
	p->order_type = order_type;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user_info.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->stop_px = stop_px;
	p->min_qty = min_qty;
	p->max_price_levels = max_price_levels;
	p->time_in_force = time_in_force;
	p->position_effect = position_effect;
	p->covered_or_uncovered = covered_or_uncovered;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_option_auction_map[seq_id] = p;

	SENDMESSAGE(ReqOptionAuctionOrder,p);


	
}

void SendOptionQuoteOrder()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionQuoteOrderMsg* p = new ATPReqOptionQuoteOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user_info.c_str(),17);

	strncpy(p->quote_req_id,quote_req_id.c_str(),11);
	p->bid_price = price;
	p->bid_order_qty = order_qty;
	p->bid_position_effect = bid_position_effect;
	p->offer_price = offer_price;
	p->offer_order_qty = offer_order_qty;
	p->offer_position_effect = offer_position_effect ;
	
	

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_quote_order_map[seq_id] = p;

	SENDMESSAGE(ReqOptionQuoteOrder,p);

}

void SendETFRedemption()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqETFRedemptionOrderMsg* p = new ATPReqETFRedemptionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_etf_redemption_map[seq_id] = p;

	SENDMESSAGE(ReqETFRedemptionOrder,p);

	
}

void SendIssue()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqIssueOrderMsg* p = new ATPReqIssueOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_issue_map[seq_id] = p;

	SENDMESSAGE(ReqIssueOrder,p);

	
}

void SendRightsIssue()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqRightsIssueOrderMsg* p = new ATPReqRightsIssueOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_rights_issue_map[seq_id] = p;

	SENDMESSAGE(ReqRightsIssueOrder,p);

	
}

void SendSwapPutback()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqSwapPutbackOrderMsg* p = new ATPReqSwapPutbackOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_swap_putback_map[seq_id] = p;


	SENDMESSAGE(ReqSwapPutbackOrder,p);
	
}

void SendOptionExercise()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionExerciseOrderMsg* p = new ATPReqOptionExerciseOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_option_exercise_map[seq_id] = p;

	SENDMESSAGE(ReqOptionExerciseOrder,p);
	
}

void SendCoveredLock()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCoveredLockOrderMsg* p = new ATPReqCoveredLockOrderMsg;

	SET(p->cust_id, cust_id);
	SET(p->fund_account_id, fund_account_id);
	SET(p->account_id, account_id);
	SET(p->client_seq_id, seq_id);
	SET(p->order_way, order_way);
	SET(p->user_info, user_info);
	SET(p->password, password);

	SET(p->security_id, security_id);
	SET(p->side, side);
	SET(p->order_qty, order_qty);
	SET(p->enforce_flag, enforce_flag);
	SET(p->account_sub_code, account_sub_code);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_covered_lock_order_map[seq_id] = p;

	SENDMESSAGE(ReqCoveredLockOrder,p);
}

void SendTurnToDisposal()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTurnToDisposalOrderMsg* p = new ATPReqTurnToDisposalOrderMsg;

	SET(p->security_id, security_id);
	
	SET(p->cust_id, cust_id);
	SET(p->fund_account_id, fund_account_id);
	SET(p->account_id, account_id);
	SET(p->side, side);
	SET(p->price, price);
	SET(p->order_qty, order_qty);
	SET(p->client_seq_id, seq_id);
	SET(p->order_way, order_way);
	SET(p->user_info, user_info);
	SET(p->password, password);
	SET(p->enforce_flag, enforce_flag);
	SET(p->account_sub_code, account_sub_code);
	SET(p->disposal_pbu, disposal_pbu);
	SET(p->disposal_account_id, disposal_account_id);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_turn_to_disposal_map[seq_id] = p;

	SENDMESSAGE(ReqTurnToDisposalOrder,p);
}

void SendDesignation()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqDesignationOrderMsg* p = new ATPReqDesignationOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->orig_cl_ord_no = orig_cl_ord_no;
	p->designation_trans_type = designation_trans_type;
	strncpy(p->transferee_pbu_id, transferee_pbu_id.c_str(),7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_designation_map[seq_id] = p;

	SENDMESSAGE(ReqDesignationOrder,p);

	
}

void SendDesignatedTransaction()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqDesignatedTransactionOrderMsg* p = new ATPReqDesignatedTransactionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	strncpy(p->designated_pbu_id,designated_pbu_id.c_str(),7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_designated_transaction_map[seq_id] = p;

	SENDMESSAGE(ReqDesignatedTransactionOrder,p);

	
}

void SendLOF()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqLOFOrderMsg* p = new ATPReqLOFOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->cash_order_qty = cash_order_qty;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_lof_map[seq_id] = p;

	SENDMESSAGE(ReqLOFOrder,p);
	
}

void SendTenderOffer()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTenderOfferOrderMsg* p = new ATPReqTenderOfferOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	strncpy(p->tenderer,tenderer.c_str(),7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_tender_offer_map[seq_id] = p;

	SENDMESSAGE(ReqTenderOfferOrder,p);

	
}

void SendNetVoting()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqNetVotingOrderMsg* p = new ATPReqNetVotingOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->general_meeting_seq = general_meeting_seq;
	strncpy(p->voting_proposal, voting_proposal.c_str(),11);
	p->voting_preference = voting_preference;
	strncpy(p->voting_segment, voting_segment.c_str(),3);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_net_voting_map[seq_id] = p;

	SENDMESSAGE(ReqNetVotingOrder,p);

	
}

void SendPasswordService()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqPasswordServiceOrderMsg* p = new ATPReqPasswordServiceOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->validation_code = validation_code;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_password_service_map[seq_id] = p;

	SENDMESSAGE(ReqPasswordServiceOrder,p);


	
}

void SendWarrantExercise()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqWarrantExerciseOrderMsg* p = new ATPReqWarrantExerciseOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_warrant_exercise_map[seq_id] = p;

	SENDMESSAGE(ReqWarrantExerciseOrder,p);
	
}

void SendStructuredFund()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqStructuredFundOrderMsg* p = new ATPReqStructuredFundOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_structured_fund_map[seq_id] = p;

	SENDMESSAGE(ReqStructuredFundOrder,p);

	
}

void SendOptionCombinationOrder()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionCombinationOrderMsg* p = new ATPReqOptionCombinationOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	//p->order_way = order_way;
	strncpy(p->user_info, user_info.c_str(),17);
	//strncpy(p->password, password.c_str(),129);
	//p->enforce_flag = enforce_flag;
	p->market_id = market_id ;
	p->side = side ;
	strncpy(p->strategy_id,strategy_id.c_str(),9);
	strncpy(p->secondary_order_id,secondary_order_id.c_str(),17);

	APINoLegsOrderUnit unit1,unit2 ;
	strncpy(unit1.leg_security_id,leg_security_id1.c_str(),9);
	unit1.market_id = market_id ;
	unit1.leg_side = leg_side1 ;
	unit1.leg_covered_or_uncovered = ATPCoveredOrUncoveredConst::kUnCovered;
	strncpy(unit2.leg_security_id,leg_security_id2.c_str(),9);
	unit2.market_id = market_id ;
	unit2.leg_side = leg_side2 ;
	unit2.leg_covered_or_uncovered = ATPCoveredOrUncoveredConst::kUnCovered;
	
	p->no_legs_array.push_back(unit1);
	p->no_legs_array.push_back(unit2);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_combination_map[seq_id] = p;

	SENDMESSAGE(ReqOptionCombinationOrder,p);
}

void SendOptionCombinationExerciseOrder()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionCombinationExerciseOrderMsg* p = new ATPReqOptionCombinationExerciseOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->market_id = market_id ;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	
	strncpy(p->user_info, user.c_str(),17);
	
	APIExerciseLeg unit1,unit2 ;
	strncpy(unit1.leg_security_id,leg_security_id1.c_str(),9) ;
	strncpy(unit2.leg_security_id,leg_security_id2.c_str(),9) ;
	p->no_legs_array.push_back(unit1);
	p->no_legs_array.push_back(unit2);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_combination_exercise_map[seq_id] = p;

	SENDMESSAGE(ReqOptionCombinationExerciseOrder,p);
}

void SendOptionCombinationQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCombinationQueryMsg* p = new ATPReqCombinationQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->market_id = market_id ;
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->secondary_order_id,secondary_order_id.c_str(),17);
	p->query_index = query_index ;
	p->return_num = return_num ;
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_combination_query_map[seq_id] = p;

	SENDMESSAGE(ReqOptionCombinationPositionQuery,p);
}

void SendOptionQuoteQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqQuoteQueryMsg* p = new ATPReqQuoteQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user_info.c_str(),17);
	
	p->cl_ord_no = cl_ord_no ;
	p->market_id = market_id ;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type ;
	p->side = side;
	p->order_query_condition = order_query_condition ;
	p->query_index = query_index ;
	p->return_num = return_num ;
	p->return_seq = return_seq ;
	
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_quote_query_map[seq_id] = p;

	SENDMESSAGE(ReqOptionQuoteOrderQuery,p);
}

void SendTibAfterHourAuction()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTibAfterHourAuctionOrderMsg* p = new ATPReqTibAfterHourAuctionOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_tib_after_hour_auction_map[seq_id] = p;

	SENDMESSAGE(ReqTibAfterHourAuctionOrder,p);

	
}
void SendPledgeInOrOut()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqPledgeInOrOutOrderMsg* p = new ATPReqPledgeInOrOutOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;

	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->enforce_flag = enforce_flag;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_pledge_in_or_out_map[seq_id] = p;

	SENDMESSAGE(ReqPledgeInOrOutOrder,p);
}
void SendCancel()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCancelOrderMsg* p = new ATPReqCancelOrderMsg;

	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->orig_cl_ord_no = orig_cl_ord_no;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_cancel_map[seq_id] = p;

	SENDMESSAGE( ReqCancelOrder, p);
}


void SendReduceHoldingTransfer()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqReduceHoldingTransferOrderMsg* p = new ATPReqReduceHoldingTransferOrderMsg;

	strncpy(p->security_id, security_id.c_str(),9);
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	strncpy(p->counter_party_account_id,counter_party_account_id.c_str(),13);
	strncpy(p->counter_party_pbu_id,counter_party_pbu_id.c_str(),7);
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	//seq_tib_after_hour_auction_map[seq_id] = p;

	SENDMESSAGE( ReqReduceHoldingTransferOrder, p);	
}

void SendReduceHoldingTransferCancel()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqReduceHoldingTransferCancelOrderMsg* p = new ATPReqReduceHoldingTransferCancelOrderMsg;

	//p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->orig_cl_ord_no = orig_cl_ord_no;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	//seq_cancel_map[seq_id] = p;

	SENDMESSAGE( ReqReduceHoldingTransferCancelOrder, p);
}



void SendBatchCancel()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBatchCancelOrderMsg* p = new ATPReqBatchCancelOrderMsg;

	p->business_type = business_type;
	strncpy(p->security_id, security_id.c_str(),9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	strncpy(p->branch_id, branch_id.c_str(),11);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_batch_cancel_map[seq_id] = p;

	SENDMESSAGE(ReqBatchCancelOrder,p);
	
}

void SendFDEPCancel()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqFDEPCancelOrderMsg* p = new ATPReqFDEPCancelOrderMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->orig_cl_ord_no = orig_cl_ord_no;
	//p->market_id = market_id;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_fdep_cancel_map[seq_id] = p;

	SENDMESSAGE(ReqFDEPCancelOrder,p);

	
}

void SendOptionCancel()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOptionCancelOrderMsg* p = new ATPReqOptionCancelOrderMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->orig_cl_ord_no = orig_cl_ord_no;
	p->market_id = market_id;

    
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_option_cancel_map[seq_id] = p;

	SENDMESSAGE(ReqOptionCancelOrder,p);

	
}

void SendCoverMaxOrderQtyQuery()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCoverMaxOrderQtyQueryMsg* p = new ATPReqCoverMaxOrderQtyQueryMsg;

	SET(p->cust_id,cust_id);
	SET(p->fund_account_id,fund_account_id);
	SET(p->account_id,account_id);
	SET(p->client_seq_id,seq_id);
	SET(p->user_info,user_info);
	SET(p->password,password);

	SET(p->market_id,market_id);
	SET(p->security_id,security_id);
	SET(p->business_type,business_type);
	SET(p->side,side);
	SET(p->price,price);
	SET(p->account_sub_code,account_sub_code);
	SET(p->position_effect,position_effect);
	SET(p->covered_or_uncovered,covered_or_uncovered);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_cover_max_order_qty_query_map[seq_id] = p;

	SENDMESSAGE(ReqCoverMaxOrderQtyQuery,p);
}


void SendExtOptionFundTransfer()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtOptionFundTransfeOtherMsg* p = new ATPReqExtOptionFundTransfeOtherMsg;

	SET(p->cust_id,cust_id);
	SET(p->fund_account_id,fund_account_id);
	SET(p->branch_id,branch_id);
	SET(p->account_id,account_id);
	SET(p->market_id,market_id);
	SET(p->currency,currency);
	SET(p->transfer_way,transfer_way);
	SET(p->transfer_value,transfer_value);
	SET(p->remark,remark);
	SET(p->password,password);
	SET(p->client_seq_id,seq_id);
	SET(p->user_info,user_info);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_option_fund_transfe_other_map[seq_id] = p;

	SENDMESSAGE(ReqExtOptionFundTransferOther,p);	
}

void SendOrderQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqOrderQueryMsg* p = new ATPReqOrderQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->cl_ord_no = cl_ord_no;
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type;
	p->side = side;
	p->business_abstract_type = 1;
	p->order_query_condition = order_query_condition;
	p->query_index = query_index;
	p->return_num = return_num;
	p->return_seq = return_seq;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_order_query_map[seq_id] = p;

	SENDMESSAGE(ReqOrderQuery,p);

	
}

void SendTradeOrderQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTradeOrderQueryMsg* p = new ATPReqTradeOrderQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type;
	p->business_abstract_type = 1;
	p->query_index = query_index;
	p->return_num = return_num;
	p->return_seq = return_seq;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_trade_order_query_map[seq_id] = p;

	SENDMESSAGE(ReqTradeOrderQuery,p);

}

void SendExtQueryContractSpecifications()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryContractSpecificationsMsg* p = new ATPReqExtQueryContractSpecificationsMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);
	p->close_out_flag = close_out_flag;
	strncpy(p->cash_margin_set, cash_margin_set.c_str(), 40);
	p->opening_date = opening_date;
	strncpy(p->opening_serial_num, opening_serial_num.c_str(), 32);
	p->query_index = query_index;
	p->return_num = return_num;
	p->return_seq = return_seq;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_contract_specifications_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryContractSpecifications,p);

}

void SendExtQueryCreditAmount()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryCreditAmountMsg* p = new ATPReqExtQueryCreditAmountMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_credit_amount_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryCreditAmount,p);

}

void SendExtQueryMarginUseable()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryMarginUseableMsg* p = new ATPReqExtQueryMarginUseableMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);
	p->query_contents = query_contents;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_margin_useable_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryMarginUseable,p);

}

void SendExtQueryFundLiability()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryFundLiabilityMsg* p = new ATPReqExtQueryFundLiabilityMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);
	strncpy(p->currency, currency.c_str(), 5);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_fund_liability_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryFundLiability,p);

}

void SendExtQueryShareLiability()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryShareLiabilityMsg* p = new ATPReqExtQueryShareLiabilityMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);

	char* str = new char[strlen(account_id.c_str()) + 1];
	strcpy(str, account_id.c_str());
	const char* split = ",";
	char* p2 = strtok(str, split);
	while (p2 != nullptr)
	{
		APIAccountIDUnit api_account_unit;
		strncpy(api_account_unit.account_id, p2, 13);
		p->account_id_array.push_back(api_account_unit);
		p2 = strtok(NULL, split);
	}
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(), 9);
	p->query_index = query_index;
	p->return_num = return_num;
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_share_liability_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryShareLiability,p);

}

void SendExtQueryContractSpecificationsManager()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryContractSpecificationsManagerMsg* p = new ATPReqExtQueryContractSpecificationsManagerMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	strncpy(p->branch_id, branch_id.c_str(), 11);
	strncpy(p->security_id, security_id.c_str(), 9);
	p->close_out_flag = close_out_flag;
	strncpy(p->cash_margin_set, cash_margin_set.c_str(), 41);
	p->opening_date = opening_date;
	strncpy(p->opening_serial_num, opening_serial_num.c_str(), 33);
	p->query_index = query_index;
	p->return_num = return_num;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_contract_specifications_manager_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryContractSpecificationsManager,p);

}

void SendExtQueryMarginUnderlying()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryMarginUnderlyingMsg* p = new ATPReqExtQueryMarginUnderlyingMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);

	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(), 9);
	//p->underlying_type = underlying_type;

	p->query_index = query_index;
	p->return_num = return_num;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_margin_underlying_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryMarginUnderlying,p);
}

void SendExtQueryCreditInfo()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryCreditInfoMsg* p = new ATPReqExtQueryCreditInfoMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);

	strncpy(p->branch_id, branch_id.c_str(), 11);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_credit_info_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryCreditInfo,p);

}

void SendExtQueryFundAssetsInfo()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryFundAssetsInfoMsg* p = new ATPReqExtQueryFundAssetsInfoMsg;

	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);

	strncpy(p->branch_id, branch_id.c_str(), 11);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_fund_assets_info_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryFundAssetsInfo,p);
}


void SendTradeOrderExQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTradeOrderExQueryMsg* p = new ATPReqTradeOrderExQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->cl_ord_no = cl_ord_no;
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type;
	p->query_index = query_index;
	p->return_num = return_num;
	p->return_seq = return_seq;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_trade_order_ex_query_map[seq_id] = p;

	SENDMESSAGE( ReqTradeOrderExQuery, p);
	
}

void SendFundQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqFundQueryMsg* p = new ATPReqFundQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	strncpy(p->currency, currency.c_str(),5);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_fund_query_map[seq_id] = p;

	SENDMESSAGE(ReqFundQuery,p);

	
}

void SendFundQueryExtra()
{
#if 0
    ATPClientSeqIDType seq_id = GenerateClientSeqID();
    ATPReqFundExQueryMsg* p = new ATPReqFundExQueryMsg;

    strncpy(p->cust_id, cust_id.c_str(), 17);
    strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
    strncpy(p->account_id, account_id.c_str(), 13);
    p->client_seq_id = seq_id;
    strncpy(p->user_info, user.c_str(), 17);
    strncpy(p->password, password.c_str(), 129);
    strncpy(p->currency, currency.c_str(), 5);
	strncpy(p->branch_id, branch_id.c_str(), 11);

    std::cout << "send seq_id of order : " << seq_id << std::endl;

    seq_fund_query_ex_map[seq_id] = p;

    SENDMESSAGE(ReqFundQueryExtra, p);
#endif
}

void SendExtrenalFundQuery()
{

#if 0
    ATPClientSeqIDType seq_id = GenerateClientSeqID();
    ATPReqExternalFundQueryMsg* p = new ATPReqExternalFundQueryMsg;

    strncpy(p->cust_id, cust_id.c_str(), 17);
    strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
    strncpy(p->account_id, account_id.c_str(), 13);
    p->client_seq_id = seq_id;
    strncpy(p->user_info, user.c_str(), 17);
    strncpy(p->password, password.c_str(), 129);
    strncpy(p->currency, currency.c_str(), 5);

    std::cout << "send seq_id of order : " << seq_id << std::endl;

    seq_external_fund_query_map[seq_id] = p;

    SENDMESSAGE(ReqExternalFundQuery, p);
#endif

}

void SendShareQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqShareQueryMsg* p = new ATPReqShareQueryMsg;
	
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);

	APIAccountIDUnit api_account_unit;
	strncpy(api_account_unit.account_id,account_id.c_str(),13);
	p->account_id_array.push_back(api_account_unit);

	p->business_type = business_type;
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->query_index = query_index;
	p->return_num = return_num;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_share_query_map[seq_id] = p;

	SENDMESSAGE( ReqShareQuery, p);
}
#if 0
void SendShareQueryExtra()
{

    ATPClientSeqIDType seq_id = GenerateClientSeqID();
    ATPReqShareQueryMsg* p = new ATPReqShareQueryMsg;

    strncpy(p->cust_id, cust_id.c_str(), 17);
    strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
    strncpy(p->account_id, account_id.c_str(), 13);
    p->client_seq_id = seq_id;
    strncpy(p->user_info, user.c_str(), 17);
    strncpy(p->password, password.c_str(), 129);

    APIAccountIDUnit api_account_unit;
    strncpy(api_account_unit.account_id, account_id.c_str(), 13);
    p->account_id_array.push_back(api_account_unit);

    p->business_type = business_type;
    p->market_id = market_id;
    strncpy(p->security_id, security_id.c_str(), 9);
    p->query_index = query_index;
    p->return_num = return_num;

    std::cout << "send seq_id of order : " << seq_id << std::endl;

    seq_share_query_map[seq_id] = p;

    SENDMESSAGE(ReqShareQueryExtra, p);
}
#endif
void SendIPOQtyQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqIPOQtyQueryMsg* p = new ATPReqIPOQtyQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ipo_qty_query_map[seq_id] = p;

	SENDMESSAGE(ReqIPOQtyQuery,p);

	
}

void SendAccountContractShareQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqAccountContractShareQueryMsg* p = new ATPReqAccountContractShareQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->account_sub_code,account_sub_code.c_str(),7);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->query_index = query_index;
	p->return_num = return_num;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_account_contract_share_query_map[seq_id] = p;

	SENDMESSAGE(ReqAccountContractShareQuery,p);

	
}

void SendAccountContractFundQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqAccountContractFundQueryMsg* p = new ATPReqAccountContractFundQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_account_contract_fund_query_map[seq_id] = p;

	SENDMESSAGE(ReqAccountContractFundQuery,p);

	
}

void SendExtQueryExerciseProfitLoss()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtQueryExerciseProfitLossMsg* p = new ATPReqExtQueryExerciseProfitLossMsg;

	SET(p->cust_id,cust_id);
	SET(p->fund_account_id,fund_account_id);
	SET(p->account_id,account_id);
	SET(p->client_seq_id,seq_id);
	SET(p->user_info,user_info);
	SET(p->password,password);
	SET(p->account_sub_code,account_sub_code);
	SET(p->market_id,market_id);
	SET(p->security_id,security_id);
	SET(p->order_qty,order_qty);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_query_exercise_profit_loss_map[seq_id] = p;

	SENDMESSAGE(ReqExtQueryExerciseProfitLoss,p);
}

void SendTradeCollectQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTradeCollectQueryMsg* p = new ATPReqTradeCollectQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_trade_collect_query_map[seq_id] = p;

	SENDMESSAGE(ReqTradeCollectQuery,p);
}

void SendTIBIPOQtyQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqTIBIPOQtyQueryMsg* p = new ATPReqTIBIPOQtyQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_tib_ipo_qty_query_map[seq_id] = p;

	SENDMESSAGE(ReqTIBIPOQtyQuery,p);

	
}



void SendNewMaxOrderQtyQuery()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqNewMaxOrderQtyQueryMsg* p = new ATPReqNewMaxOrderQtyQueryMsg;

	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->market_id = market_id;
	strncpy(p->security_id, security_id.c_str(),9);
	p->business_type = business_type;
	p->side = side;
	p->price = price;
	p->order_type = order_type;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_new_max_order_qty_query_map[seq_id] = p;

	

	SENDMESSAGE(ReqNewMaxOrderQtyQuery,p);


	
}

void SendCustLogin()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCustLoginOtherMsg* p = new ATPReqCustLoginOtherMsg;

	p->client_seq_id = seq_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->branch_id, branch_id.c_str(),11);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password_msg.c_str(),129);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_cust_login_map[seq_id] = p;

	SENDMESSAGE(ReqCustLoginOther,p);

	
}

void SendCustLogout()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqCustLogoutOtherMsg* p = new ATPReqCustLogoutOtherMsg;

	p->client_seq_id = seq_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->branch_id,branch_id.c_str(),11);
	strncpy(p->account_id, account_id.c_str(),13);
	strncpy(p->user_info, user.c_str(),17);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_cust_logout_map[seq_id] = p;

	SENDMESSAGE(ReqCustLogoutOther,p);

	
}

void SendExtFundTransfer()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtFundTransferOtherMsg* p = new ATPReqExtFundTransferOtherMsg;

	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->branch_id,branch_id.c_str(),11);
	strncpy(p->account_id, account_id.c_str(),13);
	p->is_account_partition = is_account_partition;
	strncpy(p->currency, currency.c_str(),5);
	p->transfer_way = transfer_way;
	p->transfer_value = transfer_value;
	strncpy(p->remark, remark.c_str(),128);
	strncpy(p->password, password.c_str(),129);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user.c_str(),17);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_fund_transfer_map[seq_id] = p;

	SENDMESSAGE(ReqExtFundTransferOther,p);


}

void SendExtRepay()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqExtRepayOtherMsg* p = new ATPReqExtRepayOtherMsg;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	strncpy(p->user_info, user_info.c_str(), 17);

	strncpy(p->password, password_msg.c_str(), 129);
	p->order_way = order_way;
	strncpy(p->currency, currency.c_str(), 5);
	p->repay_value = repay_value;
	p->market_id = market_id;
	strncpy(p->branch_id, branch_id.c_str(), 11);
	p->repay_way = repay_way;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_ext_repay_map[seq_id] = p;

	SENDMESSAGE(ReqExtRepayOther,p);


}

void SendSyncBiz()
{

	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqSyncBizOtherMsg* p = new ATPReqSyncBizOtherMsg;

	p->client_seq_id = seq_id;
	APINoPartitionUnit api_no_partition_array;
	api_no_partition_array.partition = partition;
	api_no_partition_array.index = g_index;
	p->no_parition_array.push_back(api_no_partition_array);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_sync_biz_map[seq_id] = p;

	SENDMESSAGE(ReqSyncBiz,p);

	
}


void SendAfterHoursPricing()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqAfterHoursPricingOrderMsg* p = new ATPReqAfterHoursPricingOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	//p->order_type = order_type;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->enforce_flag = enforce_flag;
	p->price_property = price_property;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_after_hours_pricing_map[seq_id] = p;

	SENDMESSAGE(ReqAfterHoursPricingOrder, p);

}

void SendBlockTradeIntention()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBlockTradeIntentionOrderMsg* p = new ATPReqBlockTradeIntentionOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->enforce_flag = enforce_flag;
	p->trd_sub_type = trd_sub_type;
	strncpy(p->contactor, contactor.c_str(), 13);
	strncpy(p->contact_info, contact_info.c_str(), 31);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_block_trade_intention_map[seq_id] = p;

	SENDMESSAGE(ReqBlockTradeIntentionOrder, p);

}

void SendBlockTradeTransaction()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBlockTradeTransactionOrderMsg* p = new ATPReqBlockTradeTransactionOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->enforce_flag = enforce_flag;
	strncpy(p->confirm_id, confirm_id.c_str(), 9);
	p->trd_sub_type = trd_sub_type;
	strncpy(p->counter_party_pbu_id, counter_party_pbu_id.c_str(), 7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_block_trade_transaction_map[seq_id] = p;

	SENDMESSAGE(ReqBlockTradeTransactionOrder, p);

}

void SendBlockTradePricing()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBlockTradePricingOrderMsg* p = new ATPReqBlockTradePricingOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->side = side;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->enforce_flag = enforce_flag;
	p->pricing = pricing;
	strncpy(p->confirm_id, confirm_id.c_str(), 9);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_block_trade_pricing_map[seq_id] = p;

	SENDMESSAGE(ReqBlockTradePricingOrder, p);

}

void SendBlockTradeCancel()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBlockTradeCancelOrderMsg* p = new ATPReqBlockTradeCancelOrderMsg;

	p->market_id = market_id;
	strncpy(p->cust_id, cust_id.c_str(), 17);
	strncpy(p->fund_account_id, fund_account_id.c_str(), 17);
	strncpy(p->account_id, account_id.c_str(), 13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(), 17);
	strncpy(p->password, password.c_str(), 129);
	p->orig_cl_ord_no = orig_cl_ord_no;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_block_trade_cancel_map[seq_id] = p;

	SENDMESSAGE(ReqBlockTradeCancelOrder, p);

}



void SendHKStocksThroughAuction()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqHKStocksThroughAuctionOrderMsg* p = new ATPReqHKStocksThroughAuctionOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->ord_type = order_type;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->price = price;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	p->stop_px = stop_px;
	p->min_qty = min_qty;
	p->max_price_levels = max_price_levels;
	p->time_in_force = time_in_force;
	p->lot_type = lot_type;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_hkstocksthrough_auction_map[seq_id] = p;

	SENDMESSAGE(ReqHKStocksThroughAuctionOrder,p);
}

void SendHKVoting()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqHKVotingOrderMsg* p = new ATPReqHKVotingOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->announcement_number = announcement_number;
	p->proxy_form_number = proxy_form_number;
	p->affirmative_votes = affirmative_votes;
	p->negative_votes = negative_votes;
	p->abstention_votes = abstention_votes;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_hk_voting_map[seq_id] = p;

	SENDMESSAGE(ReqHKVotingOrder,p);
}

void SendHKCorporateAction()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqHKCorporateActionOrderMsg* p = new ATPReqHKCorporateActionOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	strncpy(p->corporate_action_id, corporate_action_id.c_str(),21);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_hk_corporate_action_map[seq_id] = p;

	SENDMESSAGE(ReqHKCorporateActionOrder,p);
}

void SendHKDesignationTransfer()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqHKDesignationTransferOrderMsg* p = new ATPReqHKDesignationTransferOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->side = side;
	p->order_qty = order_qty;
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->enforce_flag = enforce_flag;
	p->orig_cl_ord_no = orig_cl_ord_no;
	p->designation_trans_type = designation_trans_type;
	strncpy(p->entitlement_number, entitlement_number.c_str(),11);
	strncpy(p->transferee_pbu_id, transferee_pbu_id.c_str(),7);

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_hk_designation_transfer_map[seq_id] = p;

	SENDMESSAGE(ReqHKDesignationTransferOrder,p);
}

void SendHKCorporateActionCancel()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqHKCorporateActionCancelOrderMsg* p = new ATPReqHKCorporateActionCancelOrderMsg;

	strncpy(p->security_id, security_id.c_str(), 9);
	p->market_id = market_id;
	strncpy(p->cust_id,cust_id.c_str(),17);
	strncpy(p->fund_account_id, fund_account_id.c_str(),17);
	strncpy(p->account_id, account_id.c_str(),13);
	p->client_seq_id = seq_id;
	p->order_way = order_way;
	strncpy(p->user_info, user.c_str(),17);
	strncpy(p->password, password.c_str(),129);
	p->orig_cli_ord_no = orig_cl_ord_no;

	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_hk_corporate_action_cancel_map[seq_id] = p;

	SENDMESSAGE(ReqHKCorporateActionCancelOrder,p);
}

// ///< 期权普通与备兑互转订单
void SendAgwMsgOptionCoveredTransUncoveredOrder()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqOptionCoveredTransUncoveredOrderMsg* p = new ATPReqOptionCoveredTransUncoveredOrderMsg;

	//SET(p->,);
	SET(p->security_id, security_id);   /////< 证券代码(必填)
	SET(p->market_id, market_id);   /////< 市场ID(必填)
	SET(p->cust_id, cust_id);   /////< 客户号(必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金帐号(必填)
	SET(p->account_id, account_id);   /////< 账户id(必填)
	SET(p->side, side);   /////< 买卖方向(必填)买卖方向，普通转备兑填2,备兑转普通填1   
	SET(p->price, price);   /////< 委托价格(必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号(必填)
	SET(p->client_feature_code, client_feature_code);   /////< 终端识别码（保留）。【该字段启用需特殊版本】 （非必填）
	SET(p->order_way, order_way);   /////< 委托方式(必填)  V217以后有效
	SET(p->user_info, user_info);   /////< 用户私有信息(必填)
	SET(p->password, password);   /////< 密码(必填) V217以后有效
	SET(p->account_sub_code, account_sub_code);   /////< 账户子编码(必填)

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_optioncoveredtransuncoveredorder_map[client_seq_id] = p;

	SENDMESSAGE(ReqOptionCoveredTransUncoveredOrder,p);
}

// ///< 合并行权最大可委托数查询消息
void SendAgwMsgExtQueryCombinExerciseMaxOrderQty()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqExtQueryCombinExerciseMaxOrderQtyMsg* p = new ATPReqExtQueryCombinExerciseMaxOrderQtyMsg;

	//SET(p->,);
	SET(p->cust_id, cust_id);   /////< 客户号ID(必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金账户ID(必填)
	SET(p->account_id, account_id);   /////< 账户ID(必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号(必填)
	SET(p->user_info, user_info);   /////< 用户私有信息(必填)
	SET(p->password, password);   /////< 客户密码(必填)
	SET(p->client_feature_code, client_feature_code);   /////< 终端识别码（保留）。【该字段启用需特殊版本】 （非必填）
	SET(p->market_id, market_id);   /////< 市场ID(必填)

	APIExerciseLegType item;
	SET(item.leg_security_id, leg_security_id1);
	
	p->no_legs_array.push_back(item);   /////< 行权多腿信息组合(必填)

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_extquerycombinexercisemaxorderqty_map[client_seq_id] = p;

	SENDMESSAGE(ReqExtQueryCombinExerciseMaxOrderQty,p);
}

// ///< 合并行权预估盈亏查询消息
void SendAgwMsgExtQueryCombinExerciseProfitLoss()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqExtQueryCombinExerciseProfitLossMsg* p = new ATPReqExtQueryCombinExerciseProfitLossMsg;

	//SET(p->,);
	SET(p->cust_id, cust_id);   /////< 客户号ID(必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金账户ID(必填)
	SET(p->account_id, account_id);   /////< 账户ID(必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号(必填)
	SET(p->user_info, user_info);   /////< 用户私有信息(必填)
	SET(p->password, password);   /////< 客户密码(必填)
	SET(p->client_feature_code, client_feature_code);   /////< 终端识别码（保留）。【该字段启用需特殊版本】 （非必填）
	SET(p->market_id, market_id);   /////< 证券所属市场(必填)
	APIExerciseLegType item;
	SET(item.leg_security_id, leg_security_id1);

	p->no_legs_array.push_back(item);   /////< 行权多腿信息组合(必填)

	SET(p->order_qty, order_qty);   /////< 行权申报数量(必填)

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_extquerycombinexerciseprofitloss_map[client_seq_id] = p;

	SENDMESSAGE(ReqExtQueryCombinExerciseProfitLoss,p);
}

// ///< 期权组合策略最大可委托数查询消息
void SendAgwMsgExternalQueryOptionCombinationMaxOrderQty()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg* p = new ATPReqExternalQueryOptionCombinationMaxOrderQtyMsg;

	//SET(p->,);
	SET(p->cust_id, cust_id);   /////< 客户号ID (必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金账户ID (必填)
	SET(p->account_id, account_id);   /////< 账户ID (必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号 (必填)
	SET(p->client_feature_code, client_feature_code);   /////< 终端识别码 (必填)
	SET(p->user_info, user_info);   /////< 用户私有信息 (必填)
	SET(p->password, password);   /////< 客户密码 (必填)
	SET(p->market_id, market_id);   /////< 证券所属市场 (必填)
	SET(p->strategy_id, strategy_id);   /////< 策略编码 (必填)
	SET(p->secondary_order_id, secondary_order_id);   /////< 组合编号 (必填)
	SET(p->side, side);   /////< 买卖方向 (必填)
	SET(p->no_legs, no_legs);   /////< 成份合约个数 (必填)
	
	APINoLegsOrderUnitType item; ///< 单腿信息组
	SET(item.leg_security_id,leg_security_id1);///< 成分合约代码
	SET(item.market_id, leg_market_id1);      ///< 市场
	SET(item.leg_side,leg_side1);       ///< 持仓类型
	SET(item.leg_covered_or_uncovered, leg_covered_or_uncovered1); ///<腿备兑标签
	if (!ATPCoveredOrUncoveredConst::IsValid(leg_covered_or_uncovered1))
	{
		std::cout <<(int)leg_covered_or_uncovered1<<" leg_covered_or_uncovered1 not Valid\n";
	}
	p->no_legs_array.push_back(item);   /////< 单腿信息组

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_externalqueryoptioncombinationmaxorderqty_map[client_seq_id] = p;

	SENDMESSAGE(ReqExternalQueryOptionCombinationMaxOrderQty,p);
}

// ///< 客户密码修改请求
void SendAgwMsgCustPasswdModify()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqCustPasswdModifyMsg* p = new ATPReqCustPasswdModifyMsg;

	//SET(p->,);
	//SET(p->extra_data, extra_data);   /////< 扩展字段 （非必填）
	SET(p->cust_id, cust_id);   /////< 客户号 (必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金账户ID (必填)
	SET(p->branch_id, branch_id);   /////< origid+营业部ID,各占5个字节 (必填)
	SET(p->old_password, old_password);   /////< 原密码 (必填)
	SET(p->password, password);   /////< 新密码 (必填)
	SET(p->order_way, order_way);   /////< 委托方式 (必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号 (必填)
	SET(p->user_info, user_info);   /////< 用户私有信息 (非必填)
	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_custpasswdmodify_map[client_seq_id] = p;

	SENDMESSAGE(ReqCustPasswdModify,p);
}

// ///< 按证券账户划转资金请求消息
void SendAgwExternalInsTETransFundReq()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqExternalInsTETransFundReqMsg* p = new ATPReqExternalInsTETransFundReqMsg;

	//SET(p->,);
	//SET(p->extra_data, extra_data);   /////< 扩展字段 （非必填）
	SET(p->cust_id, cust_id);   /////< 客户号 (必填)
	SET(p->password, password);   /////< 客户密码 (必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金帐号 (必填)
	SET(p->branch_id, branch_id);   /////< 资金转出账户ID (必填)
	SET(p->fund_out_market_id, fund_out_market_id);   /////< 资金转出市场ID (必填)
	SET(p->fund_in_account_id, fund_in_account_id);   /////< 资金转入账户ID (必填)
	SET(p->fund_in_market_id, fund_in_market_id);   /////< 资金转入市场ID (必填)
	SET(p->value, value);   /////< 划拨金额 (必填)
	SET(p->currency, currency);   /////< 货币种类 (必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号 (必填)
	SET(p->client_feature_code, client_feature_code);   /////< 终端识别码（保留）。【该字段启用需特殊版本】 （非必填）
	SET(p->user_info, user_info);   /////< 用户私有信息 （非必填，默认值 "api demo"）

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_externalinstetransfundreq_map[client_seq_id] = p;

	SENDMESSAGE(ReqExternalInsTETransFundReq,p);
}

// ///< 增强资金查询请求消息
void SendAgwMsgExtQueryFundExReq()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqExtFundQueryMsg* p = new ATPReqExtFundQueryMsg;

	//SET(p->,);
	//SET(p->extra_data, extra_data);   /////< 扩展字段 （非必填）
	SET(p->cust_id, cust_id);   /////< 客户号 (必填)
	SET(p->fund_account_id, fund_account_id);   /////< 资金帐号 (必填)
	SET(p->account_id, account_id);   /////< 资金转出账户ID (必填)
	SET(p->client_seq_id, client_seq_id);   /////< 用户系统消息序号 (必填)
	SET(p->password, password);   /////< 客户密码 (必填)
	SET(p->currency, currency);   /////< 货币种类 (必填)
	SET(p->user_info, user_info);   /////< 用户私有信息 （非必填，默认值 "api demo"）

	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_extfundqueryreq_map[client_seq_id] = p;

	SENDMESSAGE(ReqExtFundQuery,p);
}

///< 透传第三方OES通用接口
void SendAgwMsgThirdPartPassThrough()
{
	ATPClientSeqIDType client_seq_id = GenerateClientSeqID();

	ATPReqThirdPartPassThroughMsg* p = new ATPReqThirdPartPassThroughMsg;
	SET(p->partition, partition);
	SET(p->cust_id, cust_id);
	SET(p->fund_account_id, fund_account_id);
	SET(p->account_id, account_id);
	SET(p->client_seq_id, client_seq_id);
	SET(p->user_info, user_info);
	SET(p->client_feature_code, client_feature_code);
	SET(p->market_id, market_id);
	SET(p->branch_id, branch_id);
	SET(p->trade_root_num, trade_root_num);
	SET(p->data, extra_data);
	std::cout << "send seq_id of order : " << client_seq_id << std::endl;

	seq_third_part_pass_through_map[client_seq_id] = p;

	SENDMESSAGE(ReqThirdPartPassThrough, p);
}

void SendBankSecuritiesFundTransfer()
{
	ATPClientSeqIDType seq_id = GenerateClientSeqID();
	ATPReqBankSecuritiesFundTransferOtherMsg* p = new ATPReqBankSecuritiesFundTransferOtherMsg;

	SET(p->cust_id,cust_id);
	SET(p->fund_account_id,fund_account_id);
	SET(p->branch_id,branch_id);
	SET(p->account_id,account_id);
	SET(p->market_id,market_id);
	SET(p->currency,currency);
	SET(p->transfer_way,transfer_way);
	SET(p->transfer_value,transfer_value);
	SET(p->remark,remark);
	SET(p->password,password_msg);
	SET(p->client_seq_id,seq_id);
	SET(p->user_info,user_info);
	SET(p->extra_data,extra_data);
	std::cout << "send seq_id of order : " << seq_id << std::endl;

	seq_bank_securities_fund_transfer_other_map[seq_id] = p;

	SENDMESSAGE(ReqBankSecuritiesFundTransferOther,p);	
}
