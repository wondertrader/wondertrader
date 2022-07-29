
#include <iostream>
#include "demo_handler.h"
#include <vector>
#include <atomic>


std::ostream& operator<<(std::ostream& out, const std::vector<APIExerciseProfitLossLegType>& v)
{
	for (uint32_t i = 0; i < v.size(); i++)
	{
		const APIExerciseProfitLossLegType& t = v[i];
		out << "\n\tAPIExerciseProfitLossLegType " << i << "\n";
		out << "\t\tleg_security_id:" << t.leg_security_id << "\n";
		out << "\t\tprofit_loss:" << t.profit_loss << "\n";
		out << "\t\tcontract_buy_cost:" << t.contract_buy_cost << "\n";
	};

	return out;
}

/**
* @brief 连接成功事件
* @param[in] reason 原因
*/
void DemoATPTradeHandler::OnConnected(const std::string& reason)  {}

/**
 * @brief 连接失败事件
 * @param[in] reason 失败原因
 */
void DemoATPTradeHandler::OnConnectFailure(const std::string& reason)  {}

/**
 * @brief 连接超时时间
 * @param[in] reason 超时原因
 */
void DemoATPTradeHandler::OnConnectTimeOut(const std::string& reason)  {}

/**
 * @brief 心跳超时事件
 * @param[in] reason 超时原因
 */
void DemoATPTradeHandler::OnHeartbeatTimeout(const std::string& reason)  {}

/**
 * @brief  成功登录AGW事件
 * @param[in] reason 成功原因
 */
void DemoATPTradeHandler::OnLogin(const std::string& reason) 
{
	m_login_succes = true;
}

/**
 * @brief  登出事件
 * @param[in] reason 登出原因
 */
void DemoATPTradeHandler::OnLogout(const std::string& reason) 
{
	m_login_succes = false;
}

/**
 * @brief 重连结束
 * @param[in] reason 重连结束原因
 * @note 重连结束后需要使用者主动在发起连接
 */
void DemoATPTradeHandler::OnEndOfConnection(const std::string& reason) 
{
	m_exit = true;
}
/**
 * @brief  API内部错误
 * @param[in] reason 错误原因
 */
void DemoATPTradeHandler::OnError(const std::string& reason)  {}

/**
 * @brief 连接关闭
 * @param[in] reason 关闭原因
 */
void DemoATPTradeHandler::OnClosed(const std::string& reason) 
{
	m_login_succes = false;
}

/**
* @brief API内部日志回调函数
* @param[in] level 日志级别
* @param[in] message 日志内容
*/
void DemoATPTradeHandler::OnLog(ATPLogLevel level, const std::string& message)  {}

//实现回调函数，将收到的AGW消息打印出来

 void DemoATPTradeHandler::OnRspOrderStatusInternalAck(const ATPRspOrderStatusAckMsg& order_status_ack)  
{
	std::cout << "order_status_ack : " << std::endl;
	std::cout << "partition : " << (int32_t)order_status_ack.partition <<
		" index : " << order_status_ack.index <<
		" business_type : " << (int32_t)order_status_ack.business_type <<
		" cl_ord_no : " << order_status_ack.cl_ord_no <<
		" security_id : " << order_status_ack.security_id <<
		" market_id : " << order_status_ack.market_id <<
		" exec_type : " << order_status_ack.exec_type <<
		" ord_status : " << (int32_t)order_status_ack.ord_status <<
		" cust_id : " << order_status_ack.cust_id <<
		" fund_account_id : " << order_status_ack.fund_account_id <<
		" account_id : " << order_status_ack.account_id <<
		" price : " << order_status_ack.price <<
		" order_qty : " << order_status_ack.order_qty <<
		" leaves_qty : " << order_status_ack.leaves_qty <<
		" cum_qty : " << order_status_ack.cum_qty <<
		" side : " << order_status_ack.side <<
		" transact_time : " << order_status_ack.transact_time <<
		" user_info : " << order_status_ack.user_info <<
		" order_id : " << order_status_ack.order_id <<
		" cl_ord_id : " << order_status_ack.cl_ord_id <<
		" client_seq_id : " << order_status_ack.client_seq_id <<
		" orig_cl_ord_no : " << order_status_ack.orig_cl_ord_no <<
		" frozen_trade_value : " << order_status_ack.frozen_trade_value <<
		" frozen_fee : " << order_status_ack.frozen_fee <<
		" reject_reason_code : " << order_status_ack.reject_reason_code <<
		" ord_rej_reason : " << order_status_ack.ord_rej_reason <<
		" order_type : " << order_status_ack.order_type <<
		" time_in_force : " << order_status_ack.time_in_force <<
		" position_effect : " << order_status_ack.position_effect <<
		" covered_or_uncovered : " << (int32_t)order_status_ack.covered_or_uncovered <<
		" account_sub_code : " << order_status_ack.account_sub_code << std::endl;

}


 void DemoATPTradeHandler::OnRspOrderStatusAck(const ATPRspOrderStatusAckMsg& order_status_ack) 
{
	std::cout << "order_status_ack : " << std::endl;
	std::cout << "partition : " << (int32_t)order_status_ack.partition <<
	 " index : " << order_status_ack.index <<
	 " business_type : " << (int32_t)order_status_ack.business_type <<
	 " cl_ord_no : " << order_status_ack.cl_ord_no <<
	 " security_id : " << order_status_ack.security_id <<
	 " market_id : " << order_status_ack.market_id <<
	 " exec_type : " << order_status_ack.exec_type <<
	 " ord_status : " << (int32_t)order_status_ack.ord_status <<
	 " cust_id : " << order_status_ack.cust_id <<
	 " fund_account_id : " << order_status_ack.fund_account_id <<
	 " account_id : " << order_status_ack.account_id <<
	 " price : " << order_status_ack.price <<
	 " order_qty : " << order_status_ack.order_qty <<
	 " leaves_qty : " << order_status_ack.leaves_qty <<
	 " cum_qty : " << order_status_ack.cum_qty <<
	 " side : " << order_status_ack.side <<
	 " transact_time : " << order_status_ack.transact_time <<
	 " user_info : " << order_status_ack.user_info <<
	 " order_id : " << order_status_ack.order_id <<
	 " cl_ord_id : " << order_status_ack.cl_ord_id <<
	 " client_seq_id : " << order_status_ack.client_seq_id <<
	 " orig_cl_ord_no : " << order_status_ack.orig_cl_ord_no <<
	 " frozen_trade_value : " << order_status_ack.frozen_trade_value <<
	 " frozen_fee : " << order_status_ack.frozen_fee <<
	 " reject_reason_code : " << order_status_ack.reject_reason_code <<
	 " ord_rej_reason : " << order_status_ack.ord_rej_reason <<
	 " order_type : " << order_status_ack.order_type <<
	 " time_in_force : " << order_status_ack.time_in_force <<
	 " position_effect : " << order_status_ack.position_effect <<
	 " covered_or_uncovered : " << (int32_t)order_status_ack.covered_or_uncovered <<
	 " account_sub_code : " << order_status_ack.account_sub_code << 
	 " quote_flag:"<<(int32_t)order_status_ack.quote_flag<< std::endl;
 
}
	
void DemoATPTradeHandler::OnRspQuoteStatusAck(const ATPRspQuoteStatusAckMsg& order_status_ack)  
{
	std::cout << "OnRspQuoteStatusAck : " << std::endl;
}
	
void DemoATPTradeHandler::OnRspNoLegsStatusInternalAck(const ATPRspNoLegsStatusAckMsg& order_status_ack)  
{
	std::cout << "OnRspNoLegsStatusInternalAck : " << std::endl;
}
	
void DemoATPTradeHandler::OnRspNoLegsStatusAck(const ATPRspNoLegsStatusAckMsg& order_status_ack)  
{
	std::cout << "OnRspNoLegsStatusAck : " << std::endl;
}

void DemoATPTradeHandler::OnRspQuoteOrderQueryResult(const ATPRspQuoteQueryResultMsg& result)  
{
	std::cout << "OnRspQuoteOrderQueryResult : " << std::endl;
}

void DemoATPTradeHandler::OnRspCombinationPositionQueryResult(const ATPRspCombinationQueryResultMsg& result)  
{
	std::cout << "OnRspCombinationPositionQueryResult : " << std::endl;
}


void DemoATPTradeHandler::OnRspCashAuctionTradeER(const ATPRspCashAuctionTradeERMsg& cash_auction_trade_er) 
{
	std::cout << "cash_auction_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)cash_auction_trade_er.partition <<
	 " index : " << cash_auction_trade_er.index <<
	 " business_type : " <<(int32_t)cash_auction_trade_er.business_type <<
	 " cl_ord_no : " << cash_auction_trade_er.cl_ord_no <<
	 " security_id : " << cash_auction_trade_er.security_id <<
	 " market_id : " << cash_auction_trade_er.market_id <<
	 " exec_type : " << cash_auction_trade_er.exec_type <<
	 " ord_status : " << (int32_t)cash_auction_trade_er.ord_status <<
	 " cust_id : " << cash_auction_trade_er.cust_id <<
	 " fund_account_id : " << cash_auction_trade_er.fund_account_id <<
	 " account_id : " << cash_auction_trade_er.account_id <<
	 " price : " << cash_auction_trade_er.price <<
	 " order_qty : " << cash_auction_trade_er.order_qty <<
	 " leaves_qty : " << cash_auction_trade_er.leaves_qty <<
	 " cum_qty : " << cash_auction_trade_er.cum_qty <<
	 " side : " << cash_auction_trade_er.side <<
	 " transact_time : " << cash_auction_trade_er.transact_time <<
	 " user_info : " << cash_auction_trade_er.user_info <<
	 " order_id : " << cash_auction_trade_er.order_id <<
	 " cl_ord_id : " << cash_auction_trade_er.cl_ord_id <<
	 " exec_id : " << cash_auction_trade_er.exec_id <<
	 " last_px : " << cash_auction_trade_er.last_px <<
	 " last_qty : " << cash_auction_trade_er.last_qty <<
	 " total_value_traded : " << cash_auction_trade_er.total_value_traded <<
	 " fee : " << cash_auction_trade_er.fee <<
	 " cash_margin : " << cash_auction_trade_er.cash_margin << std::endl;
 
}


void DemoATPTradeHandler::OnRspCreditTradeER(const ATPRspCreditTradeERMsg& credit_trade_er)
{
	std::cout << "credit_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)credit_trade_er.partition <<
		" index : " << credit_trade_er.index <<
		" business_type : " << (int32_t)credit_trade_er.business_type <<
		" cl_ord_no : " << credit_trade_er.cl_ord_no <<
		" security_id : " << credit_trade_er.security_id <<
		" market_id : " << credit_trade_er.market_id <<
		" exec_type : " << credit_trade_er.exec_type <<
		" ord_status : " << (int32_t)credit_trade_er.ord_status <<
		" cust_id : " << credit_trade_er.cust_id <<
		" fund_account_id : " << credit_trade_er.fund_account_id <<
		" account_id : " << credit_trade_er.account_id <<
		" price : " << credit_trade_er.price <<
		" order_qty : " << credit_trade_er.order_qty <<
		" leaves_qty : " << credit_trade_er.leaves_qty <<
		" cum_qty : " << credit_trade_er.cum_qty <<
		" side : " << credit_trade_er.side <<
		" transact_time : " << credit_trade_er.transact_time <<
		" user_info : " << credit_trade_er.user_info <<
		" order_id : " << credit_trade_er.order_id <<
		" cl_ord_id : " << credit_trade_er.cl_ord_id <<
		" exec_id : " << credit_trade_er.exec_id <<
		" last_px : " << credit_trade_er.last_px <<
		" last_qty : " << credit_trade_er.last_qty <<
		" total_value_traded : " << credit_trade_er.total_value_traded <<
		" fee : " << credit_trade_er.fee <<
		" cash_margin : " << credit_trade_er.cash_margin << std::endl;

}


void DemoATPTradeHandler::OnRspMarginRepayTradeER(const ATPRspMarginRepayTradeERMsg& margin_trade_repay_er)
{
	std::cout << "margin_trade_repay_er : " << std::endl;
	std::cout << "partition : " << (int32_t)margin_trade_repay_er.partition <<
		" index : " << margin_trade_repay_er.index <<
		" business_type : " << (int32_t)margin_trade_repay_er.business_type <<
		" cl_ord_no : " << margin_trade_repay_er.cl_ord_no <<
		" security_id : " << margin_trade_repay_er.security_id <<
		" market_id : " << margin_trade_repay_er.market_id <<
		" exec_type : " << margin_trade_repay_er.exec_type <<
		" ord_status : " << (int32_t)margin_trade_repay_er.ord_status <<
		" cust_id : " << margin_trade_repay_er.cust_id <<
		" fund_account_id : " << margin_trade_repay_er.fund_account_id <<
		" account_id : " << margin_trade_repay_er.account_id <<
		" price : " << margin_trade_repay_er.price <<
		" order_qty : " << margin_trade_repay_er.order_qty <<
		" leaves_qty : " << margin_trade_repay_er.leaves_qty <<
		" cum_qty : " << margin_trade_repay_er.cum_qty <<
		" side : " << margin_trade_repay_er.side <<
		" transact_time : " << margin_trade_repay_er.transact_time <<
		" user_info : " << margin_trade_repay_er.user_info <<
		" order_id : " << margin_trade_repay_er.order_id <<
		" cl_ord_id : " << margin_trade_repay_er.cl_ord_id <<
		" exec_id : " << margin_trade_repay_er.exec_id <<
		" last_px : " << margin_trade_repay_er.last_px <<
		" last_qty : " << margin_trade_repay_er.last_qty <<
		" total_value_traded : " << margin_trade_repay_er.total_value_traded <<
		" fee : " << margin_trade_repay_er.fee <<
		" cash_margin : " << margin_trade_repay_er.cash_margin << std::endl;

}

void DemoATPTradeHandler::OnRspOptionAuctionTradeER(const ATPRspOptionAuctionTradeERMsg& option_auction_trade_er)
{
	std::cout << "option_auction_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)option_auction_trade_er.partition <<
	 " index : " << option_auction_trade_er.index <<
	 " business_type : " << (int32_t)option_auction_trade_er.business_type <<
	 " cl_ord_no : " << option_auction_trade_er.cl_ord_no <<
	 " security_id : " << option_auction_trade_er.security_id <<
	 " market_id : " << option_auction_trade_er.market_id <<
	 " exec_type : " << option_auction_trade_er.exec_type <<
	 " ord_status : " <<(int32_t) option_auction_trade_er.ord_status <<
	 " cust_id : " << option_auction_trade_er.cust_id <<
	 " fund_account_id : " << option_auction_trade_er.fund_account_id <<
	 " account_id : " << option_auction_trade_er.account_id <<
	 " price : " << option_auction_trade_er.price <<
	 " order_qty : " << option_auction_trade_er.order_qty <<
	 " leaves_qty : " << option_auction_trade_er.leaves_qty <<
	 " cum_qty : " << option_auction_trade_er.cum_qty <<
	 " side : " << option_auction_trade_er.side <<
	 " transact_time : " << option_auction_trade_er.transact_time <<
	 " user_info : " << option_auction_trade_er.user_info <<
	 " order_id : " << option_auction_trade_er.order_id <<
	 " cl_ord_id : " << option_auction_trade_er.cl_ord_id <<
	 " exec_id : " << option_auction_trade_er.exec_id <<
	 " last_px : " << option_auction_trade_er.last_px <<
	 " last_qty : " << option_auction_trade_er.last_qty <<
	 " total_value_traded : " << option_auction_trade_er.total_value_traded <<
	 " fee : " << option_auction_trade_er.fee <<
	 " position_effect : " << option_auction_trade_er.position_effect <<
	 " covered_or_uncovered : " << (int32_t)option_auction_trade_er.covered_or_uncovered <<
	 " account_sub_code : " << option_auction_trade_er.account_sub_code << 
	 " quote_flag:"<<(int32_t)option_auction_trade_er.quote_flag<< std::endl;
}


 void DemoATPTradeHandler::OnRspRepoAuctionTradeER(const ATPRspRepoAuctionTradeERMsg& repo_auction_trade_er)  
{
	std::cout << "repo_auction_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)repo_auction_trade_er.partition <<
	 " index : " << repo_auction_trade_er.index <<
	 " business_type : " << (int32_t)repo_auction_trade_er.business_type <<
	 " cl_ord_no : " << repo_auction_trade_er.cl_ord_no <<
	 " security_id : " << repo_auction_trade_er.security_id <<
	 " market_id : " << repo_auction_trade_er.market_id <<
	 " exec_type : " << repo_auction_trade_er.exec_type <<
	 " ord_status : " << (int32_t)repo_auction_trade_er.ord_status <<
	 " cust_id : " << repo_auction_trade_er.cust_id <<
	 " fund_account_id : " << repo_auction_trade_er.fund_account_id <<
	 " account_id : " << repo_auction_trade_er.account_id <<
	 " price : " << repo_auction_trade_er.price <<
	 " order_qty : " << repo_auction_trade_er.order_qty <<
	 " leaves_qty : " << repo_auction_trade_er.leaves_qty <<
	 " cum_qty : " << repo_auction_trade_er.cum_qty <<
	 " side : " << repo_auction_trade_er.side <<
	 " transact_time : " << repo_auction_trade_er.transact_time <<
	 " user_info : " << repo_auction_trade_er.user_info <<
	 " order_id : " << repo_auction_trade_er.order_id <<
	 " cl_ord_id : " << repo_auction_trade_er.cl_ord_id <<
	 " exec_id : " << repo_auction_trade_er.exec_id <<
	 " last_px : " << repo_auction_trade_er.last_px <<
	 " last_qty : " << repo_auction_trade_er.last_qty <<
	 " total_value_traded : " << repo_auction_trade_er.total_value_traded <<
	 " fee : " << repo_auction_trade_er.fee <<
	 " maturity_date : " << repo_auction_trade_er.maturity_date << std::endl;
	 
}

	
 void DemoATPTradeHandler::OnRspETFRedemptionTradeER(const ATPRspETFRedemptionTradeERMsg& etf_redemption_trade_er) 
{
	std::cout << "etf_redemption_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)etf_redemption_trade_er.partition <<
	 " index : " << etf_redemption_trade_er.index <<
	 " business_type : " << (int32_t)etf_redemption_trade_er.business_type <<
	 " cl_ord_no : " << etf_redemption_trade_er.cl_ord_no <<
	 " security_id : " << etf_redemption_trade_er.security_id <<
	 " market_id : " << etf_redemption_trade_er.market_id <<
	 " exec_type : " << etf_redemption_trade_er.exec_type <<
	 " ord_status : " << (int32_t)etf_redemption_trade_er.ord_status <<
	 " cust_id : " << etf_redemption_trade_er.cust_id <<
	 " fund_account_id : " << etf_redemption_trade_er.fund_account_id <<
	 " account_id : " << etf_redemption_trade_er.account_id <<
	 " price : " << etf_redemption_trade_er.price <<
	 " order_qty : " << etf_redemption_trade_er.order_qty <<
	 " leaves_qty : " << etf_redemption_trade_er.leaves_qty <<
	 " cum_qty : " << etf_redemption_trade_er.cum_qty <<
	 " side : " << etf_redemption_trade_er.side <<
	 " transact_time : " << etf_redemption_trade_er.transact_time <<
	 " user_info : " << etf_redemption_trade_er.user_info <<
	 " order_id : " << etf_redemption_trade_er.order_id <<
	 " cl_ord_id : " << etf_redemption_trade_er.cl_ord_id <<
	 " exec_id : " << etf_redemption_trade_er.exec_id <<
	 " last_px : " << etf_redemption_trade_er.last_px <<
	 " last_qty : " << etf_redemption_trade_er.last_qty <<
	 " total_value_traded : " << etf_redemption_trade_er.total_value_traded <<
	 " fee : " << etf_redemption_trade_er.fee << std::endl;
	//std::vector<APIConstituentStockUnit>::iterator it;
	int i = 1;
	for (auto it = etf_redemption_trade_er.constituent_stock.begin();
		it != etf_redemption_trade_er.constituent_stock.end(); it++)
	{
		std::cout << " constituent_stock_" << i << " : " << std::endl;
		std::cout << " security_id : " << it->security_id <<
		 " market_id : " << it->market_id <<
		 " qty : " << it->qty <<
		 " amt : " << it->amt <<
		 " price : " << it->price <<
		 " etf_trade_report_type : " << (int32_t)it->etf_trade_report_type << std::endl;
		i++;
	}
	

}


 void DemoATPTradeHandler::OnRspBondDistributionTradeER(const ATPRspBondDistributionTradeERMsg& bond_distribution_trade_er) 
{
	std::cout << "bond_distribution_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)bond_distribution_trade_er.partition <<
	 " index : " << bond_distribution_trade_er.index <<
	 " business_type : " << (int32_t)bond_distribution_trade_er.business_type <<
	 " cl_ord_no : " << bond_distribution_trade_er.cl_ord_no <<
	 " security_id : " << bond_distribution_trade_er.security_id <<
	 " market_id : " << bond_distribution_trade_er.market_id <<
	 " exec_type : " << bond_distribution_trade_er.exec_type <<
	 " ord_status : " << (int32_t)bond_distribution_trade_er.ord_status <<
	 " cust_id : " << bond_distribution_trade_er.cust_id <<
	 " fund_account_id : " << bond_distribution_trade_er.fund_account_id <<
	 " account_id : " << bond_distribution_trade_er.account_id <<
	 " price : " << bond_distribution_trade_er.price <<
	 " order_qty : " << bond_distribution_trade_er.order_qty <<
	 " leaves_qty : " << bond_distribution_trade_er.leaves_qty <<
	 " cum_qty : " << bond_distribution_trade_er.cum_qty <<
	 " side : " << bond_distribution_trade_er.side <<
	 " transact_time : " << bond_distribution_trade_er.transact_time <<
	 " user_info : " << bond_distribution_trade_er.user_info <<
	 " order_id : " << bond_distribution_trade_er.order_id <<
	 " cl_ord_id : " << bond_distribution_trade_er.cl_ord_id <<
	 " exec_id : " << bond_distribution_trade_er.exec_id <<
	 " last_px : " << bond_distribution_trade_er.last_px <<
	 " last_qty : " << bond_distribution_trade_er.last_qty <<
	 " total_value_traded : " << bond_distribution_trade_er.total_value_traded <<
	 " fee : " << bond_distribution_trade_er.fee << std::endl;
}


 void DemoATPTradeHandler::OnRspStructuredFundTradeER(const ATPRspStructuredFundTradeERMsg& structured_fund_trade_er) 
{
	std::cout << "structured_fund_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)structured_fund_trade_er.partition<<
	 " index : " << structured_fund_trade_er.index<<
	 " business_type : " << (int32_t)structured_fund_trade_er.business_type<<
	 " cl_ord_no : " << structured_fund_trade_er.cl_ord_no<<
	 " security_id : " << structured_fund_trade_er.security_id<<
	 " market_id : " << structured_fund_trade_er.market_id<<
	 " exec_type : " << structured_fund_trade_er.exec_type<<
	 " ord_status : " << (int32_t)structured_fund_trade_er.ord_status<<
	 " cust_id : " << structured_fund_trade_er.cust_id<<
	 " fund_account_id : " << structured_fund_trade_er.fund_account_id<<
	 " account_id : " << structured_fund_trade_er.account_id<<
	 " price : " << structured_fund_trade_er.price<<
	 " order_qty : " << structured_fund_trade_er.order_qty<<
	 " leaves_qty : " << structured_fund_trade_er.leaves_qty<<
	 " cum_qty : " << structured_fund_trade_er.cum_qty<<
	 " side : " << structured_fund_trade_er.side<<
	 " transact_time : " << structured_fund_trade_er.transact_time<<
	 " user_info : " << structured_fund_trade_er.user_info<<
	 " order_id : " << structured_fund_trade_er.order_id<<
	 " cl_ord_id : " << structured_fund_trade_er.cl_ord_id<<
	 " exec_id : " << structured_fund_trade_er.exec_id<<
	 " last_px : " << structured_fund_trade_er.last_px<<
	 " last_qty : " << structured_fund_trade_er.last_qty<<
	 " total_value_traded : " << structured_fund_trade_er.total_value_traded<<
	 " fee : " << structured_fund_trade_er.fee << std::endl;
}


 void DemoATPTradeHandler::OnRspLOFTradeER(const ATPRspLOFTradeERMsg& lof_trade_er) 
{
	std::cout << "lof_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)lof_trade_er.partition<<
	 " index : " << lof_trade_er.index<<
	 " business_type : " << (int32_t)lof_trade_er.business_type<<
	 " cl_ord_no : " << lof_trade_er.cl_ord_no<<
	 " security_id : " << lof_trade_er.security_id<<
	 " market_id : " << lof_trade_er.market_id<<
	 " exec_type : " << lof_trade_er.exec_type<<
	 " ord_status : " << (int32_t)lof_trade_er.ord_status<<
	 " cust_id : " << lof_trade_er.cust_id<<
	 " fund_account_id : " << lof_trade_er.fund_account_id<<
	 " account_id : " << lof_trade_er.account_id<<
	 " price : " << lof_trade_er.price<<
	 " order_qty : " << lof_trade_er.order_qty<<
	 " leaves_qty : " << lof_trade_er.leaves_qty<<
	 " cum_qty : " << lof_trade_er.cum_qty<<
	 " side : " << lof_trade_er.side<<
	 " transact_time : " << lof_trade_er.transact_time<<
	 " user_info : " << lof_trade_er.user_info<<
	 " order_id : " << lof_trade_er.order_id<<
	 " cl_ord_id : " << lof_trade_er.cl_ord_id<<
	 " exec_id : " << lof_trade_er.exec_id<<
	 " last_px : " << lof_trade_er.last_px<<
	 " last_qty : " << lof_trade_er.last_qty<<
	 " total_value_traded : " << lof_trade_er.total_value_traded<<
	 " fee : " << lof_trade_er.fee << std::endl;
}


 void DemoATPTradeHandler::OnRspTibAfterHourAuctionTradeER(const ATPRspTibAfterHourAuctionTradeERMsg& tib_after_hour_auction_trade_er) 
{
	std::cout << "tib_after_hour_auction_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)tib_after_hour_auction_trade_er.partition<<
	 " index : " << tib_after_hour_auction_trade_er.index<<
	 " business_type : " << (int32_t)tib_after_hour_auction_trade_er.business_type<<
	 " cl_ord_no : " << tib_after_hour_auction_trade_er.cl_ord_no<<
	 " security_id : " << tib_after_hour_auction_trade_er.security_id<<
	 " market_id : " << tib_after_hour_auction_trade_er.market_id<<
	 " exec_type : " << tib_after_hour_auction_trade_er.exec_type<<
	 " ord_status : " << (int32_t)tib_after_hour_auction_trade_er.ord_status<<
	 " cust_id : " << tib_after_hour_auction_trade_er.cust_id<<
	 " fund_account_id : " << tib_after_hour_auction_trade_er.fund_account_id<<
	 " account_id : " << tib_after_hour_auction_trade_er.account_id<<
	 " price : " << tib_after_hour_auction_trade_er.price<<
	 " order_qty : " << tib_after_hour_auction_trade_er.order_qty<<
	 " leaves_qty : " << tib_after_hour_auction_trade_er.leaves_qty<<
	 " cum_qty : " << tib_after_hour_auction_trade_er.cum_qty<<
	 " side : " << tib_after_hour_auction_trade_er.side<<
	 " transact_time : " << tib_after_hour_auction_trade_er.transact_time<<
	 " user_info : " << tib_after_hour_auction_trade_er.user_info<<
	 " order_id : " << tib_after_hour_auction_trade_er.order_id<<
	 " cl_ord_id : " << tib_after_hour_auction_trade_er.cl_ord_id<<
	 " exec_id : " << tib_after_hour_auction_trade_er.exec_id<<
	 " last_px : " << tib_after_hour_auction_trade_er.last_px<<
	 " last_qty : " << tib_after_hour_auction_trade_er.last_qty<<
	 " total_value_traded : " << tib_after_hour_auction_trade_er.total_value_traded<<
	 " fee : " << tib_after_hour_auction_trade_er.fee<<
	 " cash_margin : " << tib_after_hour_auction_trade_er.cash_margin << std::endl;
 
}

 void DemoATPTradeHandler::OnRspPledgeInOrOutTradeER(const ATPRspPledgeInOrOutTradeERMsg& pledge_in_or_out_trade_er)
 {
	 std::cout << "pledge_in_or_out_trade_er : " << std::endl;
	 std::cout << "partition : " << (int32_t)pledge_in_or_out_trade_er.partition <<
		 " index : " << pledge_in_or_out_trade_er.index <<
		 " business_type : " << (int32_t)pledge_in_or_out_trade_er.business_type <<
		 " cl_ord_no : " << pledge_in_or_out_trade_er.cl_ord_no <<
		 " security_id : " << pledge_in_or_out_trade_er.security_id <<
		 " market_id : " << pledge_in_or_out_trade_er.market_id <<
		 " exec_type : " << pledge_in_or_out_trade_er.exec_type <<
		 " ord_status : " << (int32_t)pledge_in_or_out_trade_er.ord_status <<
		 " cust_id : " << pledge_in_or_out_trade_er.cust_id <<
		 " fund_account_id : " << pledge_in_or_out_trade_er.fund_account_id <<
		 " account_id : " << pledge_in_or_out_trade_er.account_id <<
		 " price : " << pledge_in_or_out_trade_er.price <<
		 " order_qty : " << pledge_in_or_out_trade_er.order_qty <<
		 " leaves_qty : " << pledge_in_or_out_trade_er.leaves_qty <<
		 " cum_qty : " << pledge_in_or_out_trade_er.cum_qty <<
		 " side : " << pledge_in_or_out_trade_er.side <<
		 " transact_time : " << pledge_in_or_out_trade_er.transact_time <<
		 " user_info : " << pledge_in_or_out_trade_er.user_info <<
		 " order_id : " << pledge_in_or_out_trade_er.order_id <<
		 " cl_ord_id : " << pledge_in_or_out_trade_er.cl_ord_id <<
		 " exec_id : " << pledge_in_or_out_trade_er.exec_id <<
		 " last_px : " << pledge_in_or_out_trade_er.last_px <<
		 " last_qty : " << pledge_in_or_out_trade_er.last_qty <<
		 " total_value_traded : " << pledge_in_or_out_trade_er.total_value_traded <<
		 " fee : " << pledge_in_or_out_trade_er.fee << std::endl;

 }


 void DemoATPTradeHandler::OnRspMaxOrderQueryResult(const ATPRspMaxOrderQueryResultMsg& max_order_query_result) 
{
	std::cout << "max_order_query_result : " << std::endl;
	std::cout << "cust_id : " << max_order_query_result.cust_id<<
	 " fund_account_id : " << max_order_query_result.fund_account_id<<
	 " account_id : " << max_order_query_result.account_id<<
	 " client_seq_id : " << max_order_query_result.client_seq_id<<
	 " query_result_code : " << max_order_query_result.query_result_code<<
	 " user_info : " << max_order_query_result.user_info<<
	 " max_order_qty : " << max_order_query_result.max_order_qty<<
	 " theoretical_order_qty : " << max_order_query_result.theoretical_order_qty << std::endl;
}


 void DemoATPTradeHandler::OnRspOrderQueryResult(const ATPRspOrderQueryResultMsg& order_query_result) 
{
	std::cout << "order_query_result : " << std::endl;
	std::cout << "cust_id : " << order_query_result.cust_id<<
	 " fund_account_id : " << order_query_result.fund_account_id<<
	 " account_id : " << order_query_result.account_id<<
	 " client_seq_id : " << order_query_result.client_seq_id<<
	 " query_result_code : " << order_query_result.query_result_code<<
	 " user_info : " << order_query_result.user_info<<
	 " last_index : " << order_query_result.last_index<<
	 " total_num : " << order_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = order_query_result.order_array.begin();
		it != order_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " business_type : " << (int32_t)it->business_type<<
		 " security_id : " << it->security_id<<
		 " security_symbol : " << it->security_symbol<<
		 " market_id : " << it->market_id<<
		 " account_id : " << it->account_id<<
		 " side : " << it->side<<
		 " ord_type : " << it->ord_type<<
		 " ord_status : " << (int32_t)it->ord_status<<
		 " transact_time : " << it->transact_time<<
		 " order_price : " << it->order_price<<
		 " exec_price : " << it->exec_price<<
		 " order_qty : " << it->order_qty<<
		 " leaves_qty : " << it->leaves_qty<<
		 " cum_qty : " << it->cum_qty<<
		 " cl_ord_no : " << it->cl_ord_no<<
		 " order_id : " << it->order_id<<
		 " cl_ord_id : " << it->cl_ord_id<<
		 " client_seq_id : " << it->client_seq_id<<
		 " orig_cl_ord_no : " << it->orig_cl_ord_no<<
		 " frozen_trade_value : " << it->frozen_trade_value<<
		 " frozen_fee : " << it->frozen_fee<<
		 " reject_reason_code : " << it->reject_reason_code<<
		 " ord_rej_reason : " << it->ord_rej_reason << std::endl;
		i++;
	}

}

void DemoATPTradeHandler::OnRspQueryContractSpecificationsQueryResult(const ATPRspQueryContractSpecificationsResultMsg& contract_specifications_query_result)
{
	std::cout << "contract_specifications_query_result : " << std::endl;
	std::cout << "cust_id : " << contract_specifications_query_result.cust_id <<
		" fund_account_id : " << contract_specifications_query_result.fund_account_id <<
		" account_id : " << contract_specifications_query_result.account_id <<
		" client_seq_id : " << contract_specifications_query_result.client_seq_id <<
		" query_result_code : " << contract_specifications_query_result.query_result_code <<
		" user_info : " << contract_specifications_query_result.user_info <<
		" last_index : " << contract_specifications_query_result.last_index <<
		" total_num : " << contract_specifications_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = contract_specifications_query_result.order_array.begin();
		it != contract_specifications_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " branch_id : " << it->branch_id <<
			" opening_date : " << it->opening_date <<
			" opening_serial_num : " << it->opening_serial_num <<
			" market_id : " << it->market_id <<
			" credit_identification : " << (int32_t)it->credit_identification <<
			" opening_amt : " << it->opening_amt <<
			" trading_amt : " << it->trading_amt <<
			" opening_qty : " << it->opening_qty <<
			" opening_price : " << it->opening_price <<
			" repayment_amt : " << it->repayment_amt <<
			" repayment_qty : " << it->repayment_qty <<
			" expiration_date : " << it->expiration_date <<
			" interest_rate_product : " << (int32_t)it->interest_rate_product <<
			" roll_over_number : " << it->roll_over_number <<
			" deposit_ration : " << it->deposit_ration <<
			" associate_serial_num : " << it->associate_serial_num <<
			" associate_date : " << it->associate_date <<
			" unsettled_interest : " << it->unsettled_interest <<
			" unsettled_default_interest : " << it->unsettled_default_interest << std::endl;
		i++;
	}

}

void DemoATPTradeHandler::OnRspQueryCreditAmountQueryResult(const ATPRspQueryCreditAmountResultMsg& credit_amount_query_result)
{
	std::cout << "credit_amount_query_result : " << std::endl;
	std::cout << "cust_id : " << credit_amount_query_result.cust_id <<
		" fund_account_id : " << credit_amount_query_result.fund_account_id <<
		" account_id : " << credit_amount_query_result.account_id <<
		" client_seq_id : " << credit_amount_query_result.client_seq_id <<
		" query_result_code : " << credit_amount_query_result.query_result_code <<
		" user_info : " << credit_amount_query_result.user_info <<
		" last_index : " << credit_amount_query_result.last_index <<
		" total_num : " << credit_amount_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = credit_amount_query_result.order_array.begin();
		it != credit_amount_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " branch_id : " << it->branch_id <<
			" financing_security_amount : " << it->financing_security_amount <<
			" financing_amount : " << it->financing_amount <<
			" security_amount : " << it->security_amount <<
			" financing_required_amount : " << it->financing_required_amount <<
			" security_required_amount : " << it->security_required_amount << std::endl;
		i++;
	}

}

void DemoATPTradeHandler::OnRspQueryMarginUseableQueryResult(const ATPRspQueryMarginUseableResultMsg& margin_useable_query_result)
{
	std::cout << "margin_useable_query_result : " << std::endl;
	std::cout << "cust_id : " << margin_useable_query_result.cust_id <<
		" fund_account_id : " << margin_useable_query_result.fund_account_id <<
		" account_id : " << margin_useable_query_result.account_id <<
		" client_seq_id : " << margin_useable_query_result.client_seq_id <<
		" query_result_code : " << margin_useable_query_result.query_result_code <<
		" user_info : " << margin_useable_query_result.user_info <<
		" branch_id : " << margin_useable_query_result.branch_id <<
		" margin_useable_balance : " << margin_useable_query_result.margin_useable_balance << std::endl;

}

void DemoATPTradeHandler::OnRspQueryFundLiabilityQueryResult(const ATPRspQueryFundLiabilityResultMsg& fund_liability_query_result)
{
	std::cout << "fund_liability_query_result : " << std::endl;
	std::cout << "cust_id : " << fund_liability_query_result.cust_id <<
		" fund_account_id : " << fund_liability_query_result.fund_account_id <<
		" account_id : " << fund_liability_query_result.account_id <<
		" client_seq_id : " << fund_liability_query_result.client_seq_id <<
		" query_result_code : " << fund_liability_query_result.query_result_code <<
		" user_info : " << fund_liability_query_result.user_info <<
		" branch_id : " << fund_liability_query_result.branch_id <<
		" currency : " << fund_liability_query_result.currency <<
		" real_time_balance : " << fund_liability_query_result.real_time_balance <<
		" init_balance : " << fund_liability_query_result.init_balance <<
		" settle_unpay_interest : " << fund_liability_query_result.settle_unpay_interest <<
		" settle_unpay_penalty : " << fund_liability_query_result.settle_unpay_penalty <<
		" unpay_interest : " << fund_liability_query_result.unpay_interest <<
		" unpay_penalty : " << fund_liability_query_result.unpay_penalty <<
		" today_open : " << fund_liability_query_result.today_open <<
		" today_repay : " << fund_liability_query_result.today_repay <<
		" crd_buy_required : " << fund_liability_query_result.crd_buy_required <<
		" crd_sell_required : " << fund_liability_query_result.crd_sell_required <<
		" crd_buy_un_trade : " << fund_liability_query_result.crd_buy_un_trade <<
		" crd_sell_un_trade : " << fund_liability_query_result.crd_sell_un_trade <<
		" temp_add : " << fund_liability_query_result.temp_add <<
		" temp_sub : " << fund_liability_query_result.temp_sub <<
		" te_partition_no : " << (int32_t)fund_liability_query_result.te_partition_no << 
		" private_amount_occupied: " << fund_liability_query_result.private_amount_occupied <<
		" public_amount_occupied: " << fund_liability_query_result.public_amount_occupied <<
		" private_amount_frozen: " << fund_liability_query_result.private_amount_frozen <<
		" public_amount_frozen: " << fund_liability_query_result.public_amount_frozen << std::endl;
}

void DemoATPTradeHandler::OnRspQueryShareLiabilityQueryResult(const ATPRspQueryShareLiabilityResultMsg& share_liability_query_result)
{
	std::cout << "share_liability_query_result : " << std::endl;
	std::cout << "cust_id : " << share_liability_query_result.cust_id <<
		" fund_account_id : " << share_liability_query_result.fund_account_id <<
		" account_id : " << share_liability_query_result.account_id <<
		" client_seq_id : " << share_liability_query_result.client_seq_id <<
		" query_result_code : " << share_liability_query_result.query_result_code <<
		" user_info : " << share_liability_query_result.user_info <<
		" last_index : " << share_liability_query_result.last_index <<
		" total_num : " << share_liability_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = share_liability_query_result.order_array.begin();
		it != share_liability_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " security_id : " << it->security_id <<
			" security_symbol : " << it->security_symbol <<
			" market_id : " << it->market_id <<
			" account_id : " << it->account_id <<
			" real_time_qty : " << it->real_time_qty <<
			" market_value : " << it->market_value <<
			" init_qty : " << it->init_qty <<
			" today_open : " << it->today_open <<
			" today_repay : " << it->today_repay <<
			" today_leaves : " << it->today_leaves <<
			" margin_required : " << it->margin_required <<
			" today_leaves : " << it->temp_add <<
			" margin_required : " << it->temp_sub << std::endl;

		i++;
	}

}

void DemoATPTradeHandler::OnRspQueryContractSpecificationsManagerQueryResult(const ATPRspQueryContractSpecificationsManagerResultMsg& contract_specifications_manager_query_result)
{
	std::cout << "contract_specifications_manager_query_result : " << std::endl;
	std::cout << "cust_id : " << contract_specifications_manager_query_result.cust_id <<
		" fund_account_id : " << contract_specifications_manager_query_result.fund_account_id <<
		" account_id : " << contract_specifications_manager_query_result.account_id <<
		" client_seq_id : " << contract_specifications_manager_query_result.client_seq_id <<
		" query_result_code : " << contract_specifications_manager_query_result.query_result_code <<
		" user_info : " << contract_specifications_manager_query_result.user_info <<
		" last_index : " << contract_specifications_manager_query_result.last_index <<
		" total_num : " << contract_specifications_manager_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = contract_specifications_manager_query_result.order_array.begin();
		it != contract_specifications_manager_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " branch_id : " << it->branch_id <<
			" opening_date : " << it->opening_date <<
			" opening_serial_num : " << it->opening_serial_num <<
			" market_id : " << it->market_id <<
			" credit_identification : " << (int32_t)it->credit_identification <<
			" opening_amt : " << it->opening_amt <<
			" trading_amt : " << it->trading_amt <<
			" opening_qty : " << it->opening_qty <<
			" opening_price : " << it->opening_price <<
			" repayment_amt : " << it->repayment_amt <<
			" repayment_qty : " << it->repayment_qty <<
			" expiration_date : " << it->expiration_date <<
			" interest_rate_product : " << (int32_t)it->interest_rate_product <<
			" roll_over_number : " << it->roll_over_number <<
			" deposit_ration : " << it->deposit_ration <<
			" associate_serial_num : " << it->associate_serial_num <<
			" associate_date : " << it->associate_date <<
			" unsettled_interest : " << it->unsettled_interest <<
			" unsettled_default_interest : " << it->unsettled_default_interest <<
			" currency : " << it->currency <<
			" total_principal_liability : " << it->total_principal_liability <<
			" total_interest_liability : " << it->total_interest_liability <<
			" repayment_interest : " << it->repayment_interest <<
			" margin_required : " << it->margin_required <<
			" total_liability_qty : " << it->total_liability_qty <<
			" total_liability_amt : " << it->total_liability_amt << std::endl;
		i++;
	}

}

void DemoATPTradeHandler::OnRspQueryMarginUnderlyingQueryResult(const ATPRspQueryMarginUnderlyingResultMsg& margin_underlying_query_result)
{
	std::cout << "margin_underlying_query_result : " << std::endl;
	std::cout << "cust_id : " << margin_underlying_query_result.cust_id <<
		" fund_account_id : " << margin_underlying_query_result.fund_account_id <<
		" account_id : " << margin_underlying_query_result.account_id <<
		" client_seq_id : " << margin_underlying_query_result.client_seq_id <<
		" query_result_code : " << margin_underlying_query_result.query_result_code <<
		" user_info : " << margin_underlying_query_result.user_info <<

		" last_index : " << margin_underlying_query_result.last_index <<
		" total_num : " << margin_underlying_query_result.total_num << std::endl;
	//std::vector<APIOrderUnit>::iterator it;
	int i = 1;
	for (auto it = margin_underlying_query_result.order_array.begin();
		it != margin_underlying_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " market_id : " << it->market_id <<
			" security_id : " << it->security_id <<
			//" underlying_type : " << (int32_t)it->underlying_type <<
			" translation_proportion : " << it->translation_proportion <<
			" business_switch : " << it->business_switch << std::endl;
		i++;
	}
}

void DemoATPTradeHandler::OnRspQueryCreditInfoQueryResult(const ATPRspQueryCreditInfoResultMsg& credit_info_query_result)
{
	std::cout << "credit_info_query_result : " << std::endl;
	std::cout << "cust_id : " << credit_info_query_result.cust_id <<
		" fund_account_id : " << credit_info_query_result.fund_account_id <<
		" account_id : " << credit_info_query_result.account_id <<
		" client_seq_id : " << credit_info_query_result.client_seq_id <<
		" query_result_code : " << credit_info_query_result.query_result_code <<
		" user_info : " << credit_info_query_result.user_info <<

		" branch_id : " << credit_info_query_result.branch_id <<
		" contrac_status : " << (int32_t)credit_info_query_result.contrac_status <<
		" credit_rating : " << (int32_t)credit_info_query_result.credit_rating <<
		" closeout_status : " << (int32_t)credit_info_query_result.closeout_status <<
		" interest_product : " << (int32_t)credit_info_query_result.interest_product <<
		" financing_guarantee_ratio : " << credit_info_query_result.financing_guarantee_ratio <<
		" security_guarantee_ratio : " << credit_info_query_result.security_guarantee_ratio << std::endl;
}

void DemoATPTradeHandler::OnRspQueryFundAssetsInfoQueryResult(const ATPRspQueryFundAssetsInfoResultMsg& fund_assets_info_query_result)
{
	std::cout << "fund_assets_info_query_result : " << std::endl;
	std::cout << "cust_id : " << fund_assets_info_query_result.cust_id <<
		" fund_account_id : " << fund_assets_info_query_result.fund_account_id <<
		" account_id : " << fund_assets_info_query_result.account_id <<
		" client_seq_id : " << fund_assets_info_query_result.client_seq_id <<
		" query_result_code : " << fund_assets_info_query_result.query_result_code <<
		" user_info : " << fund_assets_info_query_result.user_info <<

		" branch_id : " << fund_assets_info_query_result.branch_id <<
		" fund_balance : " << fund_assets_info_query_result.fund_balance <<
		" hold_market_value : " << fund_assets_info_query_result.hold_market_value <<
		" total_assets : " << fund_assets_info_query_result.total_assets <<
		" fund_debt : " << fund_assets_info_query_result.fund_debt <<
		" share_debt_market_value : " << fund_assets_info_query_result.share_debt_market_value <<
		" total_debt : " << fund_assets_info_query_result.total_debt <<
		" net_assets : " << fund_assets_info_query_result.net_assets <<
		" margin_required : " << fund_assets_info_query_result.margin_required <<
		" maintains_margin_ratio1 : " << fund_assets_info_query_result.maintains_margin_ratio1 <<
		" maintains_margin_ratio2 : " << fund_assets_info_query_result.maintains_margin_ratio2 << std::endl;
}

void DemoATPTradeHandler::OnRspTradeOrderQueryResult(const ATPRspTradeOrderQueryResultMsg& trade_order_query_result)
{
	std::cout << "trade_order_query_result : " << std::endl;
	std::cout << "cust_id : " << trade_order_query_result.cust_id<<
	 " fund_account_id : " << trade_order_query_result.fund_account_id<<
	 " account_id : " << trade_order_query_result.account_id<<
	 " client_seq_id : " << trade_order_query_result.client_seq_id<<
	 " user_info : " << trade_order_query_result.user_info <<
	 " query_result_code : " << trade_order_query_result.query_result_code <<
	 " last_index : " << trade_order_query_result.last_index<<
	 " total_num : " << trade_order_query_result.total_num << std::endl;

	int i = 1;
	for (auto it = trade_order_query_result.order_array.begin();
		it != trade_order_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " business_type : " << (int32_t)it->business_type<<
		 " security_id : " << it->security_id<<
		 " security_symbol : " << it->security_symbol<<
		 " market_id : " << it->market_id<<
		 " account_id : " << it->account_id<<
		 " side : " << it->side<<
		 " ord_type : " << it->ord_type<<
		 " exec_type : " << it->exec_type<<
		 " exec_id : " << it->exec_id<<
		 " cl_ord_no : " << it->cl_ord_no<<
		 " order_id : " << it->order_id<<
		 " cl_ord_id : " << it->cl_ord_id<<
		 " transact_time : " << it->transact_time<<
		 " last_px : " << it->last_px<<
		 " last_qty : " << it->last_qty<<
		 " total_value_traded : " << it->total_value_traded<<
		 " fee : " << it->fee << std::endl;
		i++;
	}

}



 void DemoATPTradeHandler::OnRspFundQueryResult(const ATPRspFundQueryResultMsg& fund_query_result) 
{
	std::cout << "fund_query_result : " << std::endl;
	std::cout << "cust_id : " << fund_query_result.cust_id <<
		" fund_account_id : " << fund_query_result.fund_account_id <<
		" account_id : " << fund_query_result.account_id <<
		" client_seq_id : " << fund_query_result.client_seq_id <<
		" query_result_code : " << fund_query_result.query_result_code <<
		" user_info : " << fund_query_result.user_info <<
		" leaves_value : " << fund_query_result.leaves_value <<
		" init_leaves_value : " << fund_query_result.init_leaves_value <<
		" available_t0 : " << fund_query_result.available_t0 <<
		" available_t1 : " << fund_query_result.available_t1 <<
		" available_t2 : " << fund_query_result.available_t2 <<
		" available_t3 : " << fund_query_result.available_t3 <<
		" available_tall : " << fund_query_result.available_tall <<
		" frozen_all : " << fund_query_result.frozen_all <<
		" te_partition_no : " << (int32_t)fund_query_result.te_partition_no <<
		" credit_sell_frozen : " << fund_query_result.credit_sell_frozen << std::endl;

}


 void DemoATPTradeHandler::OnRspShareQueryResult(const ATPRspShareQueryResultMsg& share_query_result) 
{
	std::cout << "share_query_result : " << std::endl;
	std::cout << "cust_id : " << share_query_result.cust_id<<
	 " fund_account_id : " << share_query_result.fund_account_id<<
	 " account_id : " << share_query_result.account_id<<
	 " client_seq_id : " << share_query_result.client_seq_id<<
	 " query_result_code : " << share_query_result.query_result_code<<
	 " user_info : " << share_query_result.user_info<<
	 " last_index : " << share_query_result.last_index<<
	 " total_num : " << share_query_result.total_num << std::endl;
	//std::vector<APIShareUnit>::iterator it;
	int i = 1;
	for (auto it = share_query_result.order_array.begin();
		it != share_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " security_id : " << it->security_id<<
		 " security_symbol : " << it->security_symbol<<
		 " market_id : " << it->market_id<<
		 " account_id : " << it->account_id<<
		 " init_qty : " << it->init_qty<<
		 " leaves_qty : " << it->leaves_qty<<
		 " available_qty : " << it->available_qty<<
		 " profit_loss : " << it->profit_loss<<
		 " market_value : " << it->market_value<<
		 " cost_price : " << it->cost_price << std::endl;
		i++;
	}

}


 void DemoATPTradeHandler::OnRspIPOQtyQueryResult(const ATPRspIPOQtyQueryResultMsg& ipo_qty_query_result) 
{
	std::cout << "ipo_qty_query_result : " << std::endl;
	std::cout << "cust_id : " << ipo_qty_query_result.cust_id<<
	 " fund_account_id : " << ipo_qty_query_result.fund_account_id<<
	 " account_id : " << ipo_qty_query_result.account_id<<
	 " client_seq_id : " << ipo_qty_query_result.client_seq_id<<
	 " query_result_code : " << ipo_qty_query_result.query_result_code<<
	 " user_info : " << ipo_qty_query_result.user_info << std::endl;
	//std::vector<APIIPOQtyUnit>::iterator it;
	int i = 1;
	for (auto it = ipo_qty_query_result.order_array.begin();
		it != ipo_qty_query_result.order_array.end(); it++)
	{
		std::cout << " order_array_" << i << " : " << std::endl;
		std::cout << " market_id : " << it->market_id<<
		 " subscription_quota : " << it->subscription_quota << std::endl;
		i++;
	}

}


 void DemoATPTradeHandler::OnRspAccountContractShareQueryResult(const ATPRspAccountContractShareQueryResultMsg& account_contract_share_query_result) 
{
	std::cout << "account_contract_share_query_result : " << std::endl;
	std::cout<<"cust_id:"<<account_contract_share_query_result.cust_id<<std::endl;
	std::cout<<"fund_account_id:"<<account_contract_share_query_result.fund_account_id<<std::endl;
	std::cout<<"account_id:"<<account_contract_share_query_result.account_id<<std::endl;
	std::cout<<"client_seq_id:"<<account_contract_share_query_result.client_seq_id<<std::endl;
	std::cout<<"query_result_code:"<<account_contract_share_query_result.query_result_code<<std::endl;
	std::cout<<"user_info:"<<account_contract_share_query_result.user_info<<std::endl;
	std::cout<<"extra_data:"<<account_contract_share_query_result.extra_data<<std::endl;
	std::cout<<"account_sub_code:"<<account_contract_share_query_result.account_sub_code<<std::endl;
	std::cout<<"last_index:"<<account_contract_share_query_result.last_index<<std::endl;
	std::cout<<"total_num:"<<account_contract_share_query_result.total_num<<std::endl;
	//std::vector<APIContractUnit>::iterator it;
	int i = 1;
	for (auto it = account_contract_share_query_result.contract_array.begin();
		it != account_contract_share_query_result.contract_array.end(); it++)
	{
		std::cout << " contract_array_" << i << " : " << std::endl;
		std::cout<<"security_id:"<<it->security_id<<std::endl;
		std::cout<<"market_id:"<<it->market_id<<std::endl;
		std::cout<<"buy_init_value:"<<it->buy_init_value<<std::endl;
		std::cout<<"buy_value:"<<it->buy_value<<std::endl;
		std::cout<<"buy_frozen_value:"<<it->buy_frozen_value<<std::endl;
		std::cout<<"sell_init_value:"<<it->sell_init_value<<std::endl;
		std::cout<<"sell_value:"<<it->sell_value<<std::endl;
		std::cout<<"sell_frozen_value:"<<it->sell_frozen_value<<std::endl;
		std::cout<<"position_side:"<<it->position_side<<std::endl;
		std::cout<<"init_buy_amt:"<<it->init_buy_amt<<std::endl;
		std::cout<<"init_total_position_value:"<<it->init_total_position_value<<std::endl;
		std::cout<<"init_position_value:"<<it->init_position_value<<std::endl;
		std::cout<<"tatol_position_value:"<<it->tatol_position_value<<std::endl;
		std::cout<<"position_value:"<<it->position_value<<std::endl;
		std::cout<<"temp_in:"<<it->temp_in<<std::endl;
		std::cout<<"temp_out:"<<it->temp_out<<std::endl;
		std::cout<<"open_position_value:"<<it->open_position_value<<std::endl;
		std::cout<<"close_position_value:"<<it->close_position_value<<std::endl;
		std::cout<<"buy_frozen_value:"<<it->buy_frozen_value<<std::endl;
		std::cout<<"exercise_qty:"<<it->exercise_qty<<std::endl;
		std::cout<<"buy_amt:"<<it->buy_amt<<std::endl;
		std::cout<<"init_position_used_amt:"<<it->init_position_used_amt<<std::endl;
		std::cout<<"position_used_amt:"<<it->position_used_amt<<std::endl;
		std::cout<<"market_value:"<<it->market_value<<std::endl;
		std::cout<<"close_profit:"<<it->close_profit<<std::endl;
		std::cout<<"profit:"<<it->profit<<std::endl;
		std::cout<<"occupy_margin_value:"<<it->occupy_margin_value<<std::endl;
		i++;
	}


}


 void DemoATPTradeHandler::OnRspAccountContractFundQueryResult(const ATPRspAccountContractFundQueryResultMsg& account_contract_fund_query_result) 
{
	std::cout << "account_contract_fund_query_result : " << std::endl;
	std::cout << "cust_id : " << account_contract_fund_query_result.cust_id<<
	 " fund_account_id : " << account_contract_fund_query_result.fund_account_id<<
	 " account_id : " << account_contract_fund_query_result.account_id<<
	 " client_seq_id : " << account_contract_fund_query_result.client_seq_id<<
	 " query_result_code : " << account_contract_fund_query_result.query_result_code<<
	 " user_info : " << account_contract_fund_query_result.user_info<<
	 " init_total_amt : " << account_contract_fund_query_result.init_total_amt<<
	 " init_available_amt : " << account_contract_fund_query_result.init_available_amt<<
	 " available_amt : " << account_contract_fund_query_result.available_amt<<
	 " frozen_amt : " << account_contract_fund_query_result.frozen_amt << std::endl;
}


 void DemoATPTradeHandler::OnRspCollectQueryResult(const ATPRspCollectQueryResultMsg& collect_query_result) 
{
	std::cout << "collect_query_result : " << std::endl;
	std::cout << "cust_id : " << collect_query_result.cust_id<<
	 " fund_account_id : " << collect_query_result.fund_account_id<<
	 " account_id : " << collect_query_result.account_id<<
	 " client_seq_id : " << collect_query_result.client_seq_id<<
	 " query_result_code : " << collect_query_result.query_result_code<<
	 " user_info : " << collect_query_result.user_info<<
	 " all_buy_avg_price : " << collect_query_result.all_buy_avg_price<<
	 " all_buy_cum_qty : " << collect_query_result.all_buy_cum_qty<<
	 " all_buy_total_value_traded : " << collect_query_result.all_buy_total_value_traded<<
	 " all_sell_avg_price : " << collect_query_result.all_sell_avg_price<<
	 " all_sell_cum_qty : " << collect_query_result.all_sell_cum_qty<<
	 " all_sell_total_value_traded : " << collect_query_result.all_sell_total_value_traded<<
	 " all_total_value_traded : " << collect_query_result.all_total_value_traded << std::endl;

	int i = 1;
	for (auto it = collect_query_result.trade_collect_array.begin();
		it != collect_query_result.trade_collect_array.end(); it++)
	{
		std::cout << " trade_collect_array_" << i << " : " << std::endl;
		std::cout << " business_type : " << (int32_t)it->business_type<<
		 " security_id : " << it->security_id<<
		 " security_symbol : " << it->security_symbol<<
		 " market_id : " << it->market_id<<
		 " account_id : " << it->account_id<<
		 " buy_avg_price : " << it->buy_avg_price<<
		 " buy_cum_qty : " << it->buy_cum_qty<<
		 " buy_total_value_traded : " << it->buy_total_value_traded<<
		 " sell_avg_price : " << it->sell_avg_price<<
		 " sell_cum_qty : " << it->sell_cum_qty<<
		 " sell_total_value_traded : " << it->sell_total_value_traded<<
		 " total_value_traded : " << it->total_value_traded << std::endl;
		i++;
	}

}


 void DemoATPTradeHandler::OnRspTIBIPOQtyQueryResult(const ATPRspTIBIPOQtyQueryResultMsg& tib_ipo_qty_query_result) 
{
	std::cout << "tib_ipo_qty_query_result : " << std::endl;
	std::cout << "cust_id : " << tib_ipo_qty_query_result.cust_id<<
	 " fund_account_id : " << tib_ipo_qty_query_result.fund_account_id<<
	 " account_id : " << tib_ipo_qty_query_result.account_id<<
	 " client_seq_id : " << tib_ipo_qty_query_result.client_seq_id<<
	 " query_result_code : " << tib_ipo_qty_query_result.query_result_code<<
	 " user_info : " << tib_ipo_qty_query_result.user_info << std::endl;
	//std::vector<APIIPOQtyUnit>::iterator it;
	int i = 1;
	for (auto it = tib_ipo_qty_query_result.ipo_qty_array.begin();
		it != tib_ipo_qty_query_result.ipo_qty_array.end(); it++)
	{
		std::cout << " ipo_qty_array_" << i << " : " << std::endl;
		std::cout << " market_id : " << it->market_id<<
		 " subscription_quota : " << it->subscription_quota << std::endl;
		i++;
	}

}


 void DemoATPTradeHandler::OnRspCoverMaxOrderQtyResultQueryResult(const ATPRspCoverMaxOrderQtyResultQueryResultMsg& cover_max_order_qty_result_query_result) 
{
	std::cout << "cover_max_order_qty_result_query_result : " << std::endl;
	std::cout << "cust_id : " << cover_max_order_qty_result_query_result.cust_id<<
	 " fund_account_id : " << cover_max_order_qty_result_query_result.fund_account_id<<
	 " account_id : " << cover_max_order_qty_result_query_result.account_id<<
	 " client_seq_id : " << cover_max_order_qty_result_query_result.client_seq_id<<
	 " query_result_code : " << cover_max_order_qty_result_query_result.query_result_code<<
	 " user_info : " << cover_max_order_qty_result_query_result.user_info<<
	 " max_order_qty : " << cover_max_order_qty_result_query_result.max_order_qty<<
	 " theoretical_order_qty : " << cover_max_order_qty_result_query_result.theoretical_order_qty << std::endl;
}


 void DemoATPTradeHandler::OnRspSyncBizFinish(const ATPRspSyncBizFinishOtherMsg& sync_biz_finish) 
{
	std::cout << "sync_biz_finish : " << std::endl;
	std::cout << "client_seq_id : " << sync_biz_finish.client_seq_id << std::endl;

}


 void DemoATPTradeHandler::OnRspCustLoginResp(const ATPRspCustLoginRespOtherMsg& cust_login_resp) 
{
	std::cout << "cust_login_resp : " << std::endl;
	std::cout << "client_seq_id : " << cust_login_resp.client_seq_id<<
	 " transact_time : " << cust_login_resp.transact_time<<
	 " cust_id : " << cust_login_resp.cust_id<<
	 " permisson_error_code : " << cust_login_resp.permisson_error_code<<
	 " user_info : " << cust_login_resp.user_info << std::endl;
	//std::vector<APIMsgCustLoginRespFundAccountUnit>::iterator it;
	//std::vector<APIMsgCustLoginRespAccountUnit>::iterator it_1;
	int i = 1,j = 1;
	for (auto it = cust_login_resp.fund_account_array.begin();
		it != cust_login_resp.fund_account_array.end(); it++)
	{
		std::cout << " fund_account_array_" << i << " : " << std::endl;
		std::cout << " branch_id : " << it->fund_account_id<<
		 " fund_account_id : " << it->branch_id << std::endl;
		for (auto it_1 = it->account_array.begin();
			it_1 != it->account_array.end(); it_1++)
		{
			std::cout << " account_array_" << i << "_" << j << " : " << std::endl;
			std::cout << " account_id : " << it_1->account_id<<
			 " market_id : " << it_1->market_id << std::endl;

			j++;
		}
		i++;
	}

}


 void DemoATPTradeHandler::OnRspCustLogoutResp(const ATPRspCustLogoutRespOtherMsg& cust_logout_resp) 
{
	std::cout << "cust_logout_resp : " << std::endl;
	std::cout << "time : " << cust_logout_resp.time<<
	 " client_seq_id : " << cust_logout_resp.client_seq_id<<
	 " cust_id : " << cust_logout_resp.cust_id<<
	 " fund_account_id : " << cust_logout_resp.fund_account_id<<
	 " branch_id : " << cust_logout_resp.branch_id<<
	 " account_id : " << cust_logout_resp.account_id<<
	 " permisson_error_code : " << (int32_t)cust_logout_resp.permisson_error_code<<
	 " user_info : " << cust_logout_resp.user_info << std::endl;

}


 void DemoATPTradeHandler::OnRspExtFundTransferResult(const ATPRspExtFundTransferResultOtherMsg& ext_fund_transfer_result)
{
	std::cout << "ext_fund_transfer_result : " << std::endl;
	std::cout << "partition : " << (int32_t)ext_fund_transfer_result.partition<<
	 " index : " << ext_fund_transfer_result.index<<
	 " reject_reason_code : " << ext_fund_transfer_result.reject_reason_code<<
	 " reject_desc : " << ext_fund_transfer_result.reject_desc<<
	 " transact_time : " << ext_fund_transfer_result.transact_time<<
	 " client_seq_id : " << ext_fund_transfer_result.client_seq_id<<
	 " fund_account_id : " << ext_fund_transfer_result.fund_account_id<<
	 " branch_id : " << ext_fund_transfer_result.branch_id<<
	 " account_id : " << ext_fund_transfer_result.account_id<<
	 " currency : " << ext_fund_transfer_result.currency<<
	 " init_amt : " << ext_fund_transfer_result.init_amt<<
	 " order_frozen : " << ext_fund_transfer_result.order_frozen<<
	 " buy_trade : " << ext_fund_transfer_result.buy_trade<<
	 " sell_trade : " << ext_fund_transfer_result.sell_trade<<
	 " unusual_frozen : " << ext_fund_transfer_result.unusual_frozen<<
	 " unusual_frozen_cancel : " << ext_fund_transfer_result.unusual_frozen_cancel<<
	 " fee_frozen : " << ext_fund_transfer_result.fee_frozen<<
	 " fee_trade : " << ext_fund_transfer_result.fee_trade<<
	 " today_in : " << ext_fund_transfer_result.today_in<<
	 " today_out : " << ext_fund_transfer_result.today_out<<
	 " temp_add : " << ext_fund_transfer_result.temp_add<<
	 " temp_sub : " << ext_fund_transfer_result.temp_sub<<
	 " temp_frozen : " << ext_fund_transfer_result.temp_frozen<<
	 " temp_frozen_cancel : " << ext_fund_transfer_result.temp_frozen_cancel<<
	 " pre_balance : " << ext_fund_transfer_result.pre_balance<<
	 " pre_available_t0 : " << ext_fund_transfer_result.pre_available_t0<<
	 " pre_on_the_way_t1 : " << ext_fund_transfer_result.pre_on_the_way_t1<<
	 " pre_on_the_way_t2 : " << ext_fund_transfer_result.pre_on_the_way_t2<<
	 " pre_on_the_way_t3 : " << ext_fund_transfer_result.pre_on_the_way_t3<<
	 " transfer_value : " << ext_fund_transfer_result.transfer_value<<
	 " user_info : " << ext_fund_transfer_result.user_info << std::endl;

}


void DemoATPTradeHandler::OnRspExtRepayResult(const ATPRspExtRepayResultOtherMsg& ext_repay_result)
{
	std::cout << "ext_repay_result : " << std::endl;
	std::cout << " reject_reason_code : " << ext_repay_result.reject_reason_code <<
		" transact_time : " << ext_repay_result.transact_time <<
		" client_seq_id : " << ext_repay_result.client_seq_id <<
		" user_info : " << ext_repay_result.user_info << std::endl;

}

void DemoATPTradeHandler::OnRspBizRejection(const ATPRspBizRejectionOtherMsg& biz_rejection)
{
	std::cout << "biz_rejection : " << std::endl;
	std::cout << " transact_time : " << biz_rejection.transact_time <<
		" client_seq_id : " << biz_rejection.client_seq_id <<
		" msg_type : " << biz_rejection.api_msg_type <<
		" reject_reason_code : " << biz_rejection.reject_reason_code <<
		" business_reject_text : " << biz_rejection.business_reject_text <<
		" user_info : " << biz_rejection.user_info << std::endl;

 }

 void DemoATPTradeHandler::OnRspAfterHoursPricingTradeER(const ATPRspAfterHoursPricingTradeERMsg& after_hours_pricing_trade_er)
 {
	 std::cout << "after_hours_pricing_trade_er : " << std::endl;
	 std::cout << "partition : " << (int32_t)after_hours_pricing_trade_er.partition <<
		 " index : " << after_hours_pricing_trade_er.index <<
		 " business_type : " << (int32_t)after_hours_pricing_trade_er.business_type <<
		 " cl_ord_no : " << after_hours_pricing_trade_er.cl_ord_no <<
		 " security_id : " << after_hours_pricing_trade_er.security_id <<
		 " market_id : " << after_hours_pricing_trade_er.market_id <<
		 " exec_type : " << after_hours_pricing_trade_er.exec_type <<
		 " ord_status : " << (int32_t)after_hours_pricing_trade_er.ord_status <<
		 " cust_id : " << after_hours_pricing_trade_er.cust_id <<
		 " fund_account_id : " << after_hours_pricing_trade_er.fund_account_id <<
		 " account_id : " << after_hours_pricing_trade_er.account_id <<
		 " price : " << after_hours_pricing_trade_er.price <<
		 " order_qty : " << after_hours_pricing_trade_er.order_qty <<
		 " leaves_qty : " << after_hours_pricing_trade_er.leaves_qty <<
		 " cum_qty : " << after_hours_pricing_trade_er.cum_qty <<
		 " side : " << after_hours_pricing_trade_er.side <<
		 " transact_time : " << after_hours_pricing_trade_er.transact_time <<
		 " user_info : " << after_hours_pricing_trade_er.user_info <<
		 " order_id : " << after_hours_pricing_trade_er.order_id <<
		 " cl_ord_id : " << after_hours_pricing_trade_er.cl_ord_id <<
		 " exec_id : " << after_hours_pricing_trade_er.exec_id <<
		 " last_px : " << after_hours_pricing_trade_er.last_px <<
		 " last_qty : " << after_hours_pricing_trade_er.last_qty <<
		 " total_value_traded : " << after_hours_pricing_trade_er.total_value_traded <<
		 " fee : " << after_hours_pricing_trade_er.fee <<
		 " price_property : " << (int32_t)after_hours_pricing_trade_er.price_property << 
		 " cash_margin : " << after_hours_pricing_trade_er.cash_margin << std::endl;

 }


 void DemoATPTradeHandler::OnRspBlockTradeTransactionTradeER(const ATPRspBlockTradeTransactionTradeERMsg& block_trade_transaction_trade_er)
 {
	 std::cout << "block_trade_transaction_trade_er : " << std::endl;
	 std::cout << "partition : " << (int32_t)block_trade_transaction_trade_er.partition <<
		 " index : " << block_trade_transaction_trade_er.index <<
		 " business_type : " << (int32_t)block_trade_transaction_trade_er.business_type <<
		 " cl_ord_no : " << block_trade_transaction_trade_er.cl_ord_no <<
		 " security_id : " << block_trade_transaction_trade_er.security_id <<
		 " market_id : " << block_trade_transaction_trade_er.market_id <<
		 " exec_type : " << block_trade_transaction_trade_er.exec_type <<
		 " ord_status : " << (int32_t)block_trade_transaction_trade_er.ord_status <<
		 " cust_id : " << block_trade_transaction_trade_er.cust_id <<
		 " fund_account_id : " << block_trade_transaction_trade_er.fund_account_id <<
		 " account_id : " << block_trade_transaction_trade_er.account_id <<
		 " price : " << block_trade_transaction_trade_er.price <<
		 " order_qty : " << block_trade_transaction_trade_er.order_qty <<
		 " leaves_qty : " << block_trade_transaction_trade_er.leaves_qty <<
		 " cum_qty : " << block_trade_transaction_trade_er.cum_qty <<
		 " side : " << block_trade_transaction_trade_er.side <<
		 " transact_time : " << block_trade_transaction_trade_er.transact_time <<
		 " user_info : " << block_trade_transaction_trade_er.user_info <<
		 " order_id : " << block_trade_transaction_trade_er.order_id <<
		 " cl_ord_id : " << block_trade_transaction_trade_er.cl_ord_id <<
		 " exec_id : " << block_trade_transaction_trade_er.exec_id <<
		 " last_px : " << block_trade_transaction_trade_er.last_px <<
		 " last_qty : " << block_trade_transaction_trade_er.last_qty <<
		 " total_value_traded : " << block_trade_transaction_trade_er.total_value_traded <<
		 " fee : " << block_trade_transaction_trade_er.fee <<
		 " counter_party_pbu_id : " << block_trade_transaction_trade_er.counter_party_pbu_id <<
		 " counter_party_account_id : " << block_trade_transaction_trade_er.counter_party_account_id <<
		 " trd_sub_type : " << (int32_t)block_trade_transaction_trade_er.trd_sub_type <<
		 " confirm_id : " << block_trade_transaction_trade_er.confirm_id <<
		 " cash_margin : " << block_trade_transaction_trade_er.cash_margin << std::endl;

 }

 void DemoATPTradeHandler::OnRspBlockTradePricingTradeER(const ATPRspBlockTradePricingTradeERMsg& block_trade_pricing_trade_er)
 {
	 std::cout << "block_trade_pricing_trade_er : " << std::endl;
	 std::cout << "partition : " << (int32_t)block_trade_pricing_trade_er.partition <<
		 " index : " << block_trade_pricing_trade_er.index <<
		 " business_type : " << (int32_t)block_trade_pricing_trade_er.business_type <<
		 " cl_ord_no : " << block_trade_pricing_trade_er.cl_ord_no <<
		 " security_id : " << block_trade_pricing_trade_er.security_id <<
		 " market_id : " << block_trade_pricing_trade_er.market_id <<
		 " exec_type : " << block_trade_pricing_trade_er.exec_type <<
		 " ord_status : " << (int32_t)block_trade_pricing_trade_er.ord_status <<
		 " cust_id : " << block_trade_pricing_trade_er.cust_id <<
		 " fund_account_id : " << block_trade_pricing_trade_er.fund_account_id <<
		 " account_id : " << block_trade_pricing_trade_er.account_id <<
		 " price : " << block_trade_pricing_trade_er.price <<
		 " order_qty : " << block_trade_pricing_trade_er.order_qty <<
		 " leaves_qty : " << block_trade_pricing_trade_er.leaves_qty <<
		 " cum_qty : " << block_trade_pricing_trade_er.cum_qty <<
		 " side : " << block_trade_pricing_trade_er.side <<
		 " transact_time : " << block_trade_pricing_trade_er.transact_time <<
		 " user_info : " << block_trade_pricing_trade_er.user_info <<
		 " order_id : " << block_trade_pricing_trade_er.order_id <<
		 " cl_ord_id : " << block_trade_pricing_trade_er.cl_ord_id <<
		 " exec_id : " << block_trade_pricing_trade_er.exec_id <<
		 " last_px : " << block_trade_pricing_trade_er.last_px <<
		 " last_qty : " << block_trade_pricing_trade_er.last_qty <<
		 " total_value_traded : " << block_trade_pricing_trade_er.total_value_traded <<
		 " fee : " << block_trade_pricing_trade_er.fee <<
		 " pricing : " << (int32_t)block_trade_pricing_trade_er.pricing <<
		 " confirm_id : " << block_trade_pricing_trade_er.confirm_id <<
		 " cash_margin : " << block_trade_pricing_trade_er.cash_margin << std::endl;

 }

 
 void DemoATPTradeHandler::OnRspExtOptionFundTransferResult(const ATPRspOptionFundTransferResultOtherMsg& result) 
 {
	 std::cout << "ATPRspOptionFundTransferResultOtherMsg : " << std::endl;
	 std::cout<<"reject_reason_code:"<<result.reject_reason_code<<std::endl;
	 std::cout<<"reject_desc:"<<result.reject_desc<<std::endl;
	 std::cout<<"transact_time:"<<result.transact_time<<std::endl;
	 std::cout<<"client_seq_id:"<<result.client_seq_id<<std::endl;
	 std::cout<<"cust_id:"<<result.cust_id<<std::endl;
	 std::cout<<"fund_account_id:"<<result.fund_account_id<<std::endl;
	 std::cout<<"branch_id:"<<result.branch_id<<std::endl;
	 std::cout<<"account_id:"<<result.account_id<<std::endl;
	 std::cout<<"market_id:"<<result.market_id<<std::endl;
	 std::cout<<"currency:"<<result.currency<<std::endl;
	 std::cout<<"init_total_amt:"<<result.init_total_amt<<std::endl;
	 std::cout<<"init_available_amt:"<<result.init_available_amt<<std::endl;
	 std::cout<<"init_margin:"<<result.init_margin<<std::endl;
	 std::cout<<"init_exercise_assign:"<<result.init_exercise_assign<<std::endl;
	 std::cout<<"init_exercise_frozen:"<<result.init_exercise_frozen<<std::endl;
	 std::cout<<"total_amt:"<<result.total_amt<<std::endl;
	 std::cout<<"available_amt:"<<result.available_amt<<std::endl;
	 std::cout<<"today_in:"<<result.today_in<<std::endl;
	 std::cout<<"today_out:"<<result.today_out<<std::endl;
	 std::cout<<"temp_in:"<<result.temp_in<<std::endl;
	 std::cout<<"temp_out:"<<result.temp_out<<std::endl;
	 std::cout<<"royalties_in:"<<result.royalties_in<<std::endl;
	 std::cout<<"royalties_out:"<<result.royalties_out<<std::endl;
	 std::cout<<"fee:"<<result.fee<<std::endl;
	 std::cout<<"temp_frozen:"<<result.temp_frozen<<std::endl;
	 std::cout<<"temp_subfrozen:"<<result.temp_subfrozen<<std::endl;
	 std::cout<<"Unusual_frozen:"<<result.Unusual_frozen<<std::endl;
	 std::cout<<"Unusual_subfrozen:"<<result.Unusual_subfrozen<<std::endl;
	 std::cout<<"margin_frozen:"<<result.margin_frozen<<std::endl;
	 std::cout<<"royalties_frozen:"<<result.royalties_frozen<<std::endl;
	 std::cout<<"exercise_frozen:"<<result.exercise_frozen<<std::endl;
	 std::cout<<"exercise_out:"<<result.exercise_out<<std::endl;
	 std::cout<<"fee_frozen:"<<result.fee_frozen<<std::endl;
	 std::cout<<"pre_balance:"<<result.pre_balance<<std::endl;
	 std::cout<<"value:"<<result.value<<std::endl;
	 std::cout<<"transfer_way:"<<result.transfer_way<<std::endl;
	 std::cout<<"can_transfer_in_amt:"<<result.can_transfer_in_amt<<std::endl;
	 std::cout<<"user_info:"<<result.user_info<<std::endl;
	 std::cout<<"extra_data:"<<result.extra_data<<std::endl;
 }
 
 void  DemoATPTradeHandler::OnRspExtQueryResultExerciseProfitLoss(const ATPRspExtQueryResultExerciseProfitLossMsg& result) 
 {
	 std::cout<<"ATPRspExtQueryResultExerciseProfitLossMsg\n";
	std::cout<<"cust_id:"<<result.cust_id<<std::endl;
std::cout<<"fund_account_id:"<<result.fund_account_id<<std::endl;
std::cout<<"account_id:"<<result.account_id<<std::endl;
std::cout<<"client_seq_id:"<<result.client_seq_id<<std::endl;
std::cout<<"query_result_code:"<<result.query_result_code<<std::endl;
std::cout<<"user_info:"<<result.user_info<<std::endl;
std::cout<<"account_sub_code:"<<result.account_sub_code<<std::endl;
std::cout<<"market_id:"<<result.market_id<<std::endl;
std::cout<<"security_id:"<<result.security_id<<std::endl;
std::cout<<"order_qty:"<<result.order_qty<<std::endl;
std::cout<<"exercise_profit_loss:"<<result.exercise_profit_loss<<std::endl;
std::cout<<"exercise_contract_buy_cost:"<<result.exercise_contract_buy_cost<<std::endl;
 }

void DemoATPTradeHandler::OnRspHKStocksThroughAuctionTradeER(const ATPRspHKStocksThroughAuctionTradeERMsg& hkstocksthrough_auction_trade_er)
{
	std::cout << "hkstocksthrough_auction_trade_er : " << std::endl;
	std::cout << "partition : " << (int32_t)hkstocksthrough_auction_trade_er.partition <<
	 " index : " << hkstocksthrough_auction_trade_er.index <<
	 " business_type : " << (int32_t)hkstocksthrough_auction_trade_er.business_type <<
	 " cl_ord_no : " << hkstocksthrough_auction_trade_er.cl_ord_no <<
	 " security_id : " << hkstocksthrough_auction_trade_er.security_id <<
	 " market_id : " << hkstocksthrough_auction_trade_er.market_id <<
	 " exec_type : " << hkstocksthrough_auction_trade_er.exec_type <<
	 " ord_status : " << (int32_t)hkstocksthrough_auction_trade_er.ord_status <<
	 " cust_id : " << hkstocksthrough_auction_trade_er.cust_id <<
	 " fund_account_id : " << hkstocksthrough_auction_trade_er.fund_account_id <<
	 " account_id : " << hkstocksthrough_auction_trade_er.account_id <<
	 " price : " << hkstocksthrough_auction_trade_er.price <<
	 " order_qty : " << hkstocksthrough_auction_trade_er.order_qty <<
	 " leaves_qty : " << hkstocksthrough_auction_trade_er.leaves_qty <<
	 " cum_qty : " << hkstocksthrough_auction_trade_er.cum_qty <<
	 " side : " << hkstocksthrough_auction_trade_er.side <<
	 " transact_time : " << hkstocksthrough_auction_trade_er.transact_time <<
	 " user_info : " << hkstocksthrough_auction_trade_er.user_info <<
	 " order_id : " << hkstocksthrough_auction_trade_er.order_id <<
	 " cl_ord_id : " << hkstocksthrough_auction_trade_er.cl_ord_id <<
	 " exec_id : " << hkstocksthrough_auction_trade_er.exec_id <<
	 " last_px : " << hkstocksthrough_auction_trade_er.last_px <<
	 " last_qty : " << hkstocksthrough_auction_trade_er.last_qty <<
	 " total_value_traded : " << hkstocksthrough_auction_trade_er.total_value_traded <<
	 " fee : " << hkstocksthrough_auction_trade_er.fee <<
	 " cny_net_total_value_trade : " << hkstocksthrough_auction_trade_er.cny_net_total_value_trade <<
	 " cny_fee:"<< hkstocksthrough_auction_trade_er.cny_fee << std::endl;
}

void DemoATPTradeHandler::OnRspExtQueryResultCombinExerciseMaxOrderQty(const ATPRspExtQueryResultCombinExerciseMaxOrderQtyMsg& result)
{
	std::cout << "OnRspExtQueryResultCombinExerciseMaxOrderQty:\n";
	std::cout << "cust_id:" << result.cust_id << std::endl;  ///< 客户号ID 
	std::cout << "fund_account_id:" << result.fund_account_id << std::endl;  ///< 资金账户ID
	std::cout << "account_id:" << result.account_id << std::endl;  ///< 账户ID 
	std::cout << "client_seq_id:" << result.client_seq_id << std::endl;  ///< 用户系统消息序号
	std::cout << "query_result_code:" << result.query_result_code << std::endl;  ///< 查询结果代码 
	std::cout << "user_info:" << result.user_info << std::endl;  ///< 用户私有信息 
	std::cout << "max_order_qty:" << result.max_order_qty << std::endl;  ///< 最大可委托数 
	std::cout << "theoretical_order_qty:" << result.theoretical_order_qty << std::endl;  ///< 理论可委托数 

}




void DemoATPTradeHandler::OnRspExtQueryResultCombinExerciseProfitLoss(const ATPRspExtQueryResultCombinExerciseProfitLossMsg& result)
{
	std::cout << "OnRspExtQueryResultCombinExerciseProfitLoss:\n";
	std::cout << "cust_id:" << result.cust_id << std::endl;  ///< 客户号ID 
	std::cout << "fund_account_id:" << result.fund_account_id << std::endl;  ///< 资金账户ID 
	std::cout << "account_id:" << result.account_id << std::endl;  ///< 账户ID 
	std::cout << "client_seq_id:" << result.client_seq_id << std::endl;  ///< 用户系统消息序号 
	std::cout << "query_result_code:" << result.query_result_code << std::endl;  ///< 查询结果代码 
	std::cout << "user_info:" << result.user_info << std::endl;  ///< 用户私有信息 
	std::cout << "market_id:" << result.market_id << std::endl;  ///< 证券所属市场 
	std::cout << "no_legs_array:";
	std::cout<<result.no_legs_array;  ///< 行权盈亏信息组合 

	std::cout << "order_qty:" << result.order_qty << std::endl;  ///< 行权申报数量 
	std::cout << "combin_exercise_profit_loss:" << result.combin_exercise_profit_loss << std::endl;  ///< 合并行权盈亏
	std::cout << "combin_exercise_contract_buy_cost:" << result.combin_exercise_contract_buy_cost << std::endl;  ///< 合并行权买入成本
}

void DemoATPTradeHandler::OnRspThirdPartPassThroughResult(const ATPRspThirdPartPassThroughResultMsg& result)
{
	std::cout << "OnRspThirdPartPassThroughResult:\n";
	std::cout << "user_info:" << result.user_info << std::endl;
	std::cout << "transact_time:" << result.transact_time << std::endl;
	std::cout << "client_seq_id:" << result.client_seq_id << std::endl;
	std::cout << "reject_reason_code:" << result.reject_reason_code << std::endl;
	std::cout << "data:" << result.data << std::endl;
}

void DemoATPTradeHandler::OnRspBankSecuritiesFundTransferResult(const ATPRspBankSecuritiesFundTransferResultOtherMsg& result)
{
	std::cout << "OnRspBankSecuritiesFundTransferResult:\n";
	std::cout << "user_info:" << result.user_info << std::endl;
	std::cout << "transact_time:" << result.transact_time << std::endl;
	std::cout << "transfer_serial_no:" << result.transfer_serial_no << std::endl;
	std::cout << "bank_transfer_serial_no:" << result.bank_transfer_serial_no << std::endl;
	std::cout << "client_seq_id:" << result.client_seq_id << std::endl;
	std::cout << "reject_reason_code:" << result.reject_reason_code << std::endl;
	std::cout << "extra_data:" << result.extra_data << std::endl;
}