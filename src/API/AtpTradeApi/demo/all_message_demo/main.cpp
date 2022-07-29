

#include <stdio.h>
#include <cstring>
#include <string>
#include <thread>
//#include <atomic>
#include <iostream>
#include <atp_trade_api.h>
#include "cmdline.h"
#include "demo_handler.h"
#include "demo_send_order.h"
#include <chrono>



//使用到的全局变量

int64_t						number;
uint16_t				    send_to;

std::string					user;

std::map<int32_t, int32_t> report_sync;//保存同步回报信息
volatile/*std::atomic<*/bool/*>*/          g_exit=false; //是否退出
volatile/*std::atomic<*/bool/*>*/          g_login_success=false; //是否登录成功
 
ATPTradeAPI					*atp_trade_api_client =nullptr ;

ATPCoveredOrUncoveredType	covered_or_uncovered;
ATPDesignationTransTypeType	designation_trans_type;
ATPBusinessTypeType			business_type;
ATPReturnSeqType        	return_seq;
ATPTransferWayType	        transfer_way;
ATPAgwPartitionType	        partition;
ATPOrderQueryConditionType 	order_query_condition;
ATPMarketIDType				market_id;
ATPValidationCodeType		validation_code;
ATPIndexType				g_index;
ATPPriceType				price;
ATPQtyType					order_qty;
ATPClOrdNOType				cl_ord_no;
ATPClOrdNOType				orig_cl_ord_no;
ATPAmtType					cash_order_qty;
ATPTotalNumType				return_num;
ATPAmtType					transfer_value;
ATPReportIndexType			query_index;
ATPGeneralMeetingSeqType	general_meeting_seq;
std::string					password;
std::string					security_id;
std::string					cust_id;
std::string					fund_account_id;
std::string					account_id;
std::string					dest_account_id;
std::string					transferee_pbu_id;
std::string					designated_pbu_id;
std::string					tenderer;
std::string					voting_proposal;
std::string					voting_segment;
std::string					currency;
std::string					remark;
std::string					branch_id;
ATPSideType					side;
ATPOrdTypeType				order_type;
ATPOrderWayType				order_way;
ATPEnforceFlagType			enforce_flag;
ATPPositionEffectType		position_effect;
ATPVotingPreferenceType		voting_preference;
ATPBoolType					is_account_partition;
ATPQtyType					min_qty;
ATPMaxPriceLevelsType		max_price_levels;
ATPTimeInForceType			time_in_force;
ATPPriceType				stop_px;
std::string 				remote_address_master,	remote_address_backup ;

ATPPositionEffectType		bid_position_effect,offer_position_effect;
ATPQtyType					offer_order_qty ;
ATPPriceType				offer_price ;
std::string					quote_req_id ;
std::string 				user_info ;
std::string					account_sub_code;
ATPLegSideType				leg_side1,leg_side2 ;
std::string 				strategy_id,secondary_order_id ;
std::string					leg_security_id1,leg_security_id2;
ATPPricePropertyType        price_property;
ATPTrdSubTypeType           trd_sub_type;
std::string                 contactor;
std::string                 contact_info;
std::string                 confirm_id;
std::string                 counter_party_pbu_id;
std::string 				counter_party_account_id;
ATPPricingType              pricing;

ATPMarketIDType leg_market_id1;
ATPCoveredOrUncoveredType  leg_covered_or_uncovered1; ///<腿备兑标签


std::string                 client_feature_code;
ATPLotTypeType				lot_type;
ATPAnnouncementNumberType	announcement_number;
ATPProxyFormNumberType		proxy_form_number;
ATPAffirmativeVotesType		affirmative_votes;
ATPNegativeVotesType		negative_votes;
ATPAbstentionVotesType		abstention_votes;
std::string					corporate_action_id;
std::string					entitlement_number;
std::string                 dest_pbu_id;
ATPRepayWayType				repay_way;
ATPAmtType					repay_value;
ATPCloseOutFlagType				close_out_flag;
std::string					cash_margin_set;
ATPDate8Type				opening_date;
std::string					opening_serial_num;
ATPQueryContentsType			query_contents;
ATPCRDUnderlyingTypeType	underlying_type;
std::string					password_msg;//避免登录密码和消息内部密码冲突
std::string disposal_pbu, disposal_account_id;

APIExerciseLegType  no_legs_array;   /////< 行权多腿信息组合

std::string old_password;
ATPMarketIDType                      fund_out_market_id;            ///< 资金转出市场ID (必填)
std::string                     fund_in_account_id;            ///< 资金转入账户ID (必填)
ATPMarketIDType                      fund_in_market_id;             ///< 资金转入市场ID (必填)
ATPAmtType                           value;                         ///< 划拨金额 (必填)
std::string					extra_data;
std::string					trade_root_num;

uint32_t no_legs=0;   /////< 成份合约个数
/**
 * @brief 业务类型枚举
 * @note 输入的业务类型
 */
namespace MsgTypeNum
{
	static const int64_t kCashAuction 				= 100101;	///< 现货集中竞价业务
	static const int64_t kRepoAuction 				= 100201;	///< 质押式回购集中竞价业务
	static const int64_t kPledgeInOrOut				= 101901;	///< 质押出入库业务
	static const int64_t kBondDistribution			= 100301;	///< 债券分销业业务
	static const int64_t kOptionAuction				= 100401;	///< 期权集中竞价业务
	static const int64_t kOptionAuctionForceClose	= 100402;	///< 期权强平业务
	static const int64_t kETFRedemption				= 101201;	///< ETF申赎业务
	static const int64_t kIssue 					= 101301;	///< 网上发行业务
	static const int64_t kRightsIssue				= 101401;	///< 配售业务
	static const int64_t kSwapPutback 				= 101501;	///< 债券转股回售业务
	static const int64_t kOptionExercise			= 101601;	///< 期权行权业务
	static const int64_t kOptionQuote				= 100405;	///< 期权报价交易委托消息
	static const int64_t kOptionCombination			= 103401;	///< 组合策略保证金多腿业务委托消息
	static const int64_t kOptionCombinationExercise	= 103301;	///< 合并行权委托
	static const int64_t kOptionQuoteQuery			= 300011;	///< 报价查询消息
	static const int64_t kOptionCombinationQuery	= 300014;	///< 组合保证金持仓查询消息
	static const int64_t kDesignation 				= 102001;   ///< 转托管业务
	static const int64_t kDesignatedTransaction		= 102201;   ///< 指定交易业务
	static const int64_t kLOF						= 101701;   ///< 开放式基金申赎业务
	static const int64_t kTenderOffer				= 101801;   ///< 要约收购业务
	static const int64_t kNetVoting					= 102101;   ///< 网络投票业务
	static const int64_t kPasswordService			= 102401;   ///< 密码服务业务
	static const int64_t kWarrantExercise			= 102301;   ///< 权证行权业务
	static const int64_t kStructuredFund			= 103101;   ///< 分级基金实时分拆合并业务
	static const int64_t kCoveredLock				= 103201;   ///< 锁定解锁订单
	static const int64_t kTibAfterHourAuction 		= 109701;   ///< 科创板股票盘后定价业务
	static const int64_t kCancel 					= 190001;   ///< 通用撤单
	static const int64_t kBatchCancel 				= 190002;   ///< 一键撤单
	static const int64_t kFDEPCancel				= 190003;   ///< FDEP公共撤单
	static const int64_t kOptionCancel				= 190401;   ///< 期权撤单
	static const int64_t kMaxOrderQtyQuery 			= 300001;   ///< 最大可委托数查询 
	static const int64_t kOrderQuery 				= 300002;   ///< 订单查询
	static const int64_t kTradeOrderQuery 			= 300003;   ///< 成交查询
	static const int64_t kFundQuery 				= 300004;   ///< 资金查询
	static const int64_t kShareQuery 				= 300005;   ///< 股份持仓查询
	static const int64_t kIPOQtyQuery 				= 300006;   ///< 新股发行申购额度查询
	static const int64_t kAccountContractShareQuery = 300007;   ///< 合约账户持仓查询

	static const int64_t kAccountContractFundQuery	= 300008;   ///< 指定合约账户资金查询
	static const int64_t kTradeCollectQuery 		= 300009;   ///< 成交汇总查询
	static const int64_t kATPReqTIBIPOQtyQuery 		= 300010;   ///< 科创板新股发行申购额度查询
	static const int64_t kCoverMaxOrderQtyQuery		= 300012;   ///< 期权最大可委托数查询
	static const int64_t kNewMaxOrderQtyQuery 		= 300013;   ///< 增强型最大可委托数查询
	static const int64_t kCustLogin					= 800005;   ///< 客户登入验证
	static const int64_t kCustLogout				= 800007;   ///< 客户登出
	static const int64_t kExtFundTransfer			= 800009;   ///< 外部资金划拨请求
	static const int64_t kSyncBiz					= 800011;   ///< 业务层回报同步
	static const int64_t kAfterHoursPricing			= 1006001;  ///< 大宗盘后定价委托
	static const int64_t kBlockTradeIntention		= 1005009;  ///< 大宗意向申报委托
	static const int64_t kBlockTradeTransaction		= 1005003;  ///< 大宗成交申报委托
	static const int64_t kBlockTradePricing			= 1005001;  ///< 大宗定价申报委托
	static const int64_t kBlockTradeCancel			= 19000005; ///< 大宗交易撤单
	static const int64_t kTurnToDisposal            = 1027001;  ///< 转处置业务订单
	static const int64_t kExtOptionFundTransfer     = 8000002;	///< 外部期权资金账户资金划拨请求
	static const int64_t kExtQueryExerciseProfitLoss = 3000004;	///< 行权预估盈亏查询消息
	static const int64_t kReduceHoldingTransfer 	= 1036001;///< 高管额度划转业务
	static const int64_t kReduceHoldingTransferCancel = 19000006;///< 高管额度划转撤单

	static const int64_t kExtExtRepay = 8000106;   ///< 融资融券直接还款请求
	static const int64_t kCredit = 1099001;  ///< 融资融券交易委托
	static const int64_t kMarginNoTrade = 1100001;	///< 融资融券非交易业务消息
	static const int64_t kMarginRepay = 1101001;	///< 融资融券偿还业务
	static const int64_t kExtQueryContractSpecifications = 3000101;	///< 融资融券合约明细查询消息
	static const int64_t kExtQueryCreditAmount = 3000102;	///< 融资融券信用额度查询消息
	static const int64_t kExtQueryMarginUseable = 3000103;	///< 融资融券保证金可用查询消息
	static const int64_t kExtQueryFundLiability = 3000107;	///< 融资融券资金负债查询消息
	static const int64_t kExtQueryShareLiability = 3000108;	///< 融资融券股份负债查询消息
	static const int64_t kExtQueryContractSpecificationsManager = 3000109;	///< 融资融券合约管理查询消息
	static const int64_t kExtQueryMarginUnderlying = 3000119;	///< 融资融券标的清单查询消息
	static const int64_t kExtQueryCreditInfo = 3000120;	///< 融资融券客户信用资料查询消息
	static const int64_t kAgwMsgExtQueryFundAssetsInfo = 3000121; ///< 融资融券资产信息查询
	static const int64_t kHKStocksThrough			= 106301;	///< 港股通业务委托消息
	static const int64_t kHKVoting					= 106401;	///< 港股通投票业务委托消息
	static const int64_t kHKCorporateAction			= 106201;	///< 港股通公司行为业务委托消息
	static const int64_t kHKDesignationTransfer		= 106101;	///< 港股通转托管业务委托消息
	static const int64_t kHKCorporateActionCancel	= 190004;	///< 港股通公司行为撤单消息

	static const int64_t kAgwMsgOptionCoveredTransUncoveredOrder = 103501; ///< 期权普通与备兑互转订单
	static const int64_t kAgwMsgExtQueryCombinExerciseMaxOrderQty = 3000023; ///< 合并行权最大可委托数查询消息
	static const int64_t kAgwMsgExtQueryCombinExerciseProfitLoss = 3000022; ///< 合并行权预估盈亏查询消息
	static const int64_t kAgwMsgExternalQueryOptionCombinationMaxOrderQty = 3000027; ///< 期权组合策略最大可委托数查询消息
	static const int64_t kAgwMsgCustPasswdModify = 8000004; ///< 客户密码修改请求
	static const int64_t kAgwExternalInsTETransFundReq = 810001; ///< 按证券账户划转资金请求消息

	static const int64_t kAgwMsgExtQueryFundExReq = 3000033; ///< 增强资金查询请求消息
	static const int64_t kTradeOrderExQuery 		= 3000035;   ///< 增强成交查询
	static const int64_t  kAgwMsgThirdPartPassThrough = 8000027; ///<第三方通用转发接口
	static const int64_t kBankSecuritiesFundTransfer = 8000030;	///< 银证转账
};

template<size_t N>
void SET(char(&a)[N], const std::string& str)
{
	strncpy(a, str.c_str(), N);
}

template<typename T1, typename T2>
void  SET(T1& t1, const T2& t2)
{
	static_assert(std::is_same<T1, T2>::value, "Types should be  compatible");
	t1 = t2;
}

int32_t Connect()
{
	std::vector<std::string> locations;

	locations.push_back(remote_address_master);
	locations.push_back(remote_address_backup);

	//设置连接信息
	ATPConnectProperty prop;
	prop.user = user;//登录AGW的账户
	prop.password = password;//登录AGW的密码
	prop.locations = locations;//AGW IP地址 格式为："ip:port"
	prop.heartbeat_interval_milli = 5000;//发送心跳的时间间隔,单位：毫秒，默认值：5000
	prop.connect_timeout_milli = 5000;//连接超时时间，单位：毫秒，默认值：5000 
	prop.reconnect_time = 10;//  重试连接次数，默认值：连接地址数,
	prop.report_sync = report_sync;//回报同步数据map<int32_t, int32_t>，默认值：空

								  
	// API连接
	ATPRetCodeType ec = atp_trade_api_client->Connect(prop, new DemoATPTradeHandler(g_login_success, g_exit));
	if (ec != ErrorCode::kSuccess)
	{
		std::cout << "Connect error:" << ec << std::endl;
		return -1;
	}
	return 0 ;
}


void InitFeatureCode(TerminalFeatureCode &feature_code)
{
//	feature_code.mode = 1;
	feature_code.station_name = "";
}

int main(int argc, char* argv[])
{

	std::string tmp_side;
	std::string	tmp_order_type;
	std::string	tmp_order_way;
	std::string	tmp_enforce_flag;
	std::string	tmp_position_effect;
	std::string	tmp_voting_preference;
	std::string	tmp_time_in_force;
	std::string	tmp_lot_type;
	std::string tmp_bid_position_effect, tmp_offer_position_effect, tmp_leg_side1, tmp_leg_side2;
	std::string tmp_close_out_flag;
	std::string tmp_query_contents;

	
	//解析命令行参数
	try {
		cmdline::parser cmd_parse;
		cmd_parse.add<uint16_t>("send_to", 0 ,"witch version send to(4=V220,2=V217,3=V212,1=V210),default 4", false, 4);
		cmd_parse.add<int64_t>("number", 0 ,"the send msg_type value,default 100000", false, 100000);
		cmd_parse.add<std::string>("account_id", 0 ,"account_id, default 000000",false,"000000");
		cmd_parse.add<std::string>("dest_account_id", 0, "dest_account_id, default 000000", false, "");
		cmd_parse.add<std::string>("branch_id", 0 ,"branch_id,default 00000",false,"00000");
		cmd_parse.add<int32_t>("business_type", 0 ,"business_type,default 200",false,200);
		cmd_parse.add<ATPAmtType>("cash_order_qty", 0 ,"cash_order_qty,default 100000",false,100000);
		cmd_parse.add<ATPClOrdNOType>("cl_ord_no", 0 ,"cl_ord_no,default 0", false, 0);
		cmd_parse.add<std::string>("client_feature_code", 0 ,"client_feature_code ,default 0000", false, "0000");
		cmd_parse.add<int32_t>("covered_or_uncovered", 0 ,"covered_or_uncovered, default 0",false,0);
		cmd_parse.add<std::string>("currency", 0 ,"currency,only CNY",false ,"CNY");
		cmd_parse.add<std::string>("cust_id", 0 ,"cust_id, default 00000000",false,"00000000");
		cmd_parse.add<std::string>("designated_pbu_id", 0 ,"designated_pbu_id,default 0000",false,"0000");
		cmd_parse.add<int32_t>("designation_trans_type", 0 ,"designation_trans_type,default 1",false,1);
		cmd_parse.add<std::string>("enforce_flag", 0 ,"enforce_flag, default 0",false,"0");
		cmd_parse.add<std::string>("fund_account_id", 0 ,"fund_account_id, default 0000000",false,"0000000");
		cmd_parse.add<ATPGeneralMeetingSeqType>("general_meeting_seq", 0 ,"general_meeting_seq,default 100001",false, 100001);
		cmd_parse.add<ATPIndexType>("g_index", 0 ,"index,default 0", false, 0);
		cmd_parse.add<std::string>("remote_address_master", 0 ,"default 127.0.0.1:32002", false, "127.0.0.1:32002");
		cmd_parse.add<std::string>("remote_address_backup", 0 ,"default 127.0.0.1:32002", false, "127.0.0.1:32002");
		cmd_parse.add<ATPBoolType>("is_account_partition", 0 ,"is_account_partition,default 1", false, 1);
		cmd_parse.add<ATPMarketIDType>("market_id", 0 ,"market_id, default kNull",false, ATPMarketIDConst::kNULL);
		cmd_parse.add<ATPMaxPriceLevelsType>("max_price_levels", 0, "max_price_levels,default 0", false, 0);
		cmd_parse.add<ATPQtyType>("min_qty", 0, "min_qty,default 0", false, 0);
		cmd_parse.add<ATPClOrdNOType>("orig_cl_ord_no", 0 ,"orig_cl_ord_no,default 00001",false, 00001);
		cmd_parse.add<std::string>("order_way", 0 ,"order_way, default 0",false,"0");
		cmd_parse.add<ATPQtyType>("order_qty", 0 ,"order_qty, default 10000",false, 10000);
		cmd_parse.add<int32_t>("order_query_condition", 0 ,"order_query_condition,default 0", false, 0);
		cmd_parse.add<std::string>("order_type", 0 ,"order_type,default 0", false, "0");
		cmd_parse.add<int32_t>("partition", 0 ,"partition,default 0", false, 0);
		cmd_parse.add<std::string>("password", 0 ,"password,default password1", false, "password1");
		cmd_parse.add<std::string>("position_effect", 0 ,"position_effect, default C ",false,"C");
		cmd_parse.add<ATPPriceType>("price", 0 ,"price,default 100000",false, 100000);
		cmd_parse.add<ATPReportIndexType>("query_index", 0 ,"query_index,default 0", false, 0);
		cmd_parse.add<int32_t>("return_seq", 0 ,"return_seq,default 0", false, 1);
		cmd_parse.add<ATPTotalNumType>("return_num", 0 ,"return_num,default 0", false, 0);
		cmd_parse.add<std::string>("remark", 0 ,"remark,default 00000", false, "00000");
		cmd_parse.add<std::string>("security_id", 0 ,"security_id, default 000001",false,"000001");
		cmd_parse.add<std::string>("side", 0 ,"side, default 0,1,2...",false,"1");
		cmd_parse.add<ATPPriceType>("stop_px", 0 ,"stop_px, default 10000",false,10000);
		cmd_parse.add<std::string>("time_in_force", 0, "time_in_force,default 0", false, "0");
		cmd_parse.add<int32_t>("transfer_way", 0 ,"transfer_way,default 1", false, 1);
		cmd_parse.add<ATPAmtType>("transfer_value", 0 ,"transfer_value,default 0", false, 0);
		cmd_parse.add<std::string>("transferee_pbu_id", 0 ,"transferee_pbu_id,default 0000",false,"0000");
		cmd_parse.add<std::string>("tenderer", 0 ,"tenderer,default 00000",false,"00000");
		cmd_parse.add<std::string>("user", 0 ,"user,default user1", false, "user1");
		cmd_parse.add<std::string>("voting_proposal", 0 ,"voting_proposal,default 00000",false,"00000");
		cmd_parse.add<std::string>("voting_preference", 0 ,"voting_preference,default 1",false,"1");
		cmd_parse.add<std::string>("voting_segment", 0 ,"voting_segment,default 00000",false,"00000");
		cmd_parse.add<ATPValidationCodeType>("validation_code", 0 ,"validation_code,default 0",false, 0);
		cmd_parse.add<std::string>("bid_position_effect",0,"bid_position_effect default O",false,"O");
		cmd_parse.add<std::string>("offer_position_effect",0,"offer_position_effect default O",false,"O");
		cmd_parse.add<ATPQtyType>("offer_order_qty", 0 ,"offer_order_qty, default 10000",false, 10000);
		cmd_parse.add<ATPPriceType>("offer_price", 0 ,"offer_price,default 100000",false, 100000);
		cmd_parse.add<std::string>("quote_req_id",0,"quote_req_id,default 0",false,"0");
		cmd_parse.add<std::string>("user_info",0,"user_info,default api demo",false,"api demo");
		cmd_parse.add<std::string>("account_sub_code",0,"account_sub_code,default 888",false,"888");
		cmd_parse.add<std::string>("leg_side1",0,"leg_side1,default 1",false,"1");
		cmd_parse.add<std::string>("leg_side2",0,"leg_side2,default 2",false,"2");
		cmd_parse.add<std::string>("strategy_id",0,"strategy_id,default 0",false,"0");
		cmd_parse.add<std::string>("secondary_order_id",0,"secondary_order_id default 0",false,"0");
		cmd_parse.add<std::string>("leg_security_id1",0,"leg_security_id1,default 0",false,"0");
		cmd_parse.add<std::string>("leg_security_id2",0,"leg_security_id2 default 0",false,"0");
		cmd_parse.add<int32_t>("price_property",0,"price_property default 1",false,1);
		cmd_parse.add<int32_t>("trd_sub_type",0,"trd_sub_type default 1",false,1);
		cmd_parse.add<std::string>("contactor",0,"contactor default contactor",false,"contactor");
		cmd_parse.add<std::string>("contact_info",0,"contact_info default contactinfo",false,"contactinfo");
		cmd_parse.add<std::string>("confirm_id",0,"confirm_id default 123",false,"123");
		cmd_parse.add<std::string>("counter_party_pbu_id",0,"counter_party_pbu_id default 123",false,"123");
		cmd_parse.add<std::string>("counter_party_account_id",0,"counter_party_account_id default 123",false,"123");
		cmd_parse.add<int32_t>("pricing",0,"pricing default 1",false,1);
		cmd_parse.add<ATPMarketIDType>("leg_market_id1", 0, "leg_market_id1 default 0", false, 0);
		cmd_parse.add<ATPCoveredOrUncoveredType>("leg_covered_or_uncovered1", 0, "leg_covered_or_uncovered1 default 0", false, 0);
		cmd_parse.add<std::string>("dest_pbu_id", 0, "dest_pbu_id,default ", false, "");
		cmd_parse.add<std::string>("disposal_pbu", 0, "disposal_pbu,default ", false, " ");
		cmd_parse.add<std::string>("disposal_account_id", 0, "disposal_account_id default ", false, " ");
		cmd_parse.add<ATPRepayWayType>("repay_way", 0, "repay_way,default 0", false, 0);
		cmd_parse.add<std::string>("close_out_flag", 0, "close_out_flag,default 0,1,2", false, "0");
		cmd_parse.add<std::string>("cash_margin_set", 0, "cash_margin_set,default 0", false, "");
		cmd_parse.add<ATPDate8Type>("opening_date", 0, "opening_date,default 0", false, 0);
		cmd_parse.add<std::string>("opening_serial_num", 0, "opening_serial_num,default ", false, "");
		cmd_parse.add<std::string>("query_contents", 0, "query_contents,default 0,1", false, "1");
		cmd_parse.add<ATPCRDUnderlyingTypeType>("underlying_type", 0, "underlying_type,default 0,1,2,3", false, 0);
		cmd_parse.add<ATPAmtType>("repay_value", 0, "repay_value,default 0", false, 0);
		cmd_parse.add<std::string>("password_msg", 0, "password_msg,default ", false, "");
		cmd_parse.add<std::string>("lot_type", 0, "lot_type default ", false, "2");
		cmd_parse.add<ATPAnnouncementNumberType>("announcement_number", 0, "announcement_number default ", false, " ");
		cmd_parse.add<ATPProxyFormNumberType>("proxy_form_number", 0, "proxy_form_number default ", false, " ");
		cmd_parse.add<ATPAffirmativeVotesType>("affirmative_votes", 0, "affirmative_votes default ", false, " ");
		cmd_parse.add<ATPNegativeVotesType>("negative_votes", 0, "negative_votes default ", false, " ");
		cmd_parse.add<ATPAbstentionVotesType>("abstention_votes", 0, "abstention_votes default ", false, " ");
		cmd_parse.add<std::string>("corporate_action_id", 0, "corporate_action_id default ", false, " ");
		cmd_parse.add<std::string>("entitlement_number", 0, "entitlement_number default ", false, " ");

		cmd_parse.add<std::string>("old_password", 0, "old_password default ", false, " ");
		cmd_parse.add<ATPMarketIDType>("fund_out_market_id", 0, "fund_out_market_id default 0", false, 0);
		cmd_parse.add<std::string>("fund_in_account_id", 0, "fund_in_account_id default 0", false, " ");
		cmd_parse.add<ATPMarketIDType>("fund_in_market_id", 0, "fund_in_market_id default ", false, 0);
		cmd_parse.add<ATPAmtType>("value", 0, "value default 10000", false, 10000);
		cmd_parse.add<std::string>("extra_data", 0, "extra_data default ", false, " ");
		cmd_parse.add<std::string>("trade_root_num", 0, "trade_root_num default ", false, " ");

		

		cmd_parse.parse_check(argc, argv);

		send_to = cmd_parse.get<uint16_t>("send_to");
		remote_address_master = cmd_parse.get<std::string>("remote_address_master");
		remote_address_backup = cmd_parse.get<std::string>("remote_address_backup");
		user = cmd_parse.get<std::string>("user");
		password = cmd_parse.get<std::string>("password");
		security_id = cmd_parse.get<std::string>("security_id");
		market_id = cmd_parse.get<ATPMarketIDType>("market_id");
		cust_id = cmd_parse.get<std::string>("cust_id");
		fund_account_id = cmd_parse.get<std::string>("fund_account_id");
		account_id = cmd_parse.get<std::string>("account_id");
		tmp_side = cmd_parse.get<std::string>("side");
		price = cmd_parse.get<ATPPriceType>("price");
		order_qty = cmd_parse.get<ATPQtyType>("order_qty");
		tmp_order_way = cmd_parse.get<std::string>("order_way");
		tmp_enforce_flag = cmd_parse.get<std::string>("enforce_flag");
		tmp_position_effect = cmd_parse.get<std::string>("position_effect");
		covered_or_uncovered = cmd_parse.get<int32_t>("covered_or_uncovered");
		orig_cl_ord_no = cmd_parse.get<ATPClOrdNOType>("orig_cl_ord_no");
		cl_ord_no = cmd_parse.get<ATPClOrdNOType>("cl_ord_no");
		client_feature_code = cmd_parse.get<std::string>("client_feature_code");
		designation_trans_type = cmd_parse.get<int32_t>("designation_trans_type");
		transferee_pbu_id = cmd_parse.get<std::string>("transferee_pbu_id");
		designated_pbu_id = cmd_parse.get<std::string>("designated_pbu_id");
		cash_order_qty = cmd_parse.get<ATPAmtType>("cash_order_qty");
		tenderer = cmd_parse.get<std::string>("tenderer");
		general_meeting_seq = cmd_parse.get<ATPGeneralMeetingSeqType>("general_meeting_seq");
		voting_proposal = cmd_parse.get<std::string>("voting_proposal");
		tmp_voting_preference = cmd_parse.get<std::string>("voting_preference");
		voting_segment = cmd_parse.get<std::string>("voting_segment");
		validation_code = cmd_parse.get<ATPValidationCodeType>("validation_code");
		branch_id = cmd_parse.get<std::string>("branch_id");
		business_type = cmd_parse.get<int32_t>("business_type");
		number = cmd_parse.get<int64_t>("number");
		order_query_condition = cmd_parse.get<int32_t>("order_query_condition");
		query_index = cmd_parse.get<ATPReportIndexType>("query_index");
		return_num = cmd_parse.get<ATPTotalNumType>("return_num");
		return_seq = cmd_parse.get<int32_t>("return_seq");
		currency = cmd_parse.get<std::string>("currency");
		tmp_order_type = cmd_parse.get<std::string>("order_type");
		is_account_partition = cmd_parse.get<ATPBoolType>("is_account_partition");
		transfer_way = cmd_parse.get<int32_t>("transfer_way");
		transfer_value = cmd_parse.get<ATPAmtType>("transfer_value");
		remark = cmd_parse.get<std::string>("remark");
		partition = cmd_parse.get<int32_t>("partition");
		g_index = cmd_parse.get<ATPIndexType>("g_index");
		min_qty = cmd_parse.get<ATPQtyType>("min_qty");
		max_price_levels = cmd_parse.get<ATPMaxPriceLevelsType>("max_price_levels");
		tmp_time_in_force = cmd_parse.get<std::string>("time_in_force");
		stop_px = cmd_parse.get<ATPPriceType>("stop_px");
		repay_value = cmd_parse.get<ATPAmtType>("repay_value");
		password_msg = cmd_parse.get<std::string>("password_msg");

		tmp_bid_position_effect = cmd_parse.get<std::string>("bid_position_effect");
		tmp_offer_position_effect = cmd_parse.get<std::string>("offer_position_effect");
		offer_order_qty = cmd_parse.get<ATPQtyType>("offer_order_qty");
		offer_price = cmd_parse.get<ATPPriceType>("offer_price");
		quote_req_id = cmd_parse.get<std::string>("quote_req_id");
		user_info = cmd_parse.get<std::string>("user_info");
		account_sub_code = cmd_parse.get<std::string>("account_sub_code");
		tmp_leg_side1 = cmd_parse.get<std::string>("leg_side1");
		tmp_leg_side2 = cmd_parse.get<std::string>("leg_side2");
		strategy_id = cmd_parse.get<std::string>("strategy_id");
		secondary_order_id = cmd_parse.get<std::string>("secondary_order_id");
		leg_security_id1 = cmd_parse.get<std::string>("leg_security_id1");
		leg_security_id2 = cmd_parse.get<std::string>("leg_security_id2");	

		leg_market_id1  = cmd_parse.get<ATPMarketIDType>("leg_market_id1");
		leg_covered_or_uncovered1 = cmd_parse.get<ATPCoveredOrUncoveredType>("leg_covered_or_uncovered1");

		price_property = cmd_parse.get<int32_t>("price_property");
		trd_sub_type = cmd_parse.get<int32_t>("trd_sub_type");
		contactor = cmd_parse.get<std::string>("contactor");
		contact_info = cmd_parse.get<std::string>("contact_info");
		confirm_id = cmd_parse.get<std::string>("confirm_id");
		counter_party_pbu_id = cmd_parse.get<std::string>("counter_party_pbu_id");
		counter_party_account_id = cmd_parse.get<std::string>("counter_party_account_id");
		pricing = cmd_parse.get<int32_t>("pricing");

		disposal_pbu = cmd_parse.get<std::string>("disposal_pbu");
		disposal_account_id = cmd_parse.get<std::string>("disposal_account_id");
		tmp_lot_type = cmd_parse.get<std::string>("lot_type");
		announcement_number = cmd_parse.get<ATPAnnouncementNumberType>("announcement_number");
		proxy_form_number = cmd_parse.get<ATPProxyFormNumberType>("proxy_form_number");
		affirmative_votes = cmd_parse.get<ATPAffirmativeVotesType>("affirmative_votes");
		negative_votes = cmd_parse.get<ATPNegativeVotesType>("negative_votes");
		abstention_votes = cmd_parse.get<ATPAbstentionVotesType>("abstention_votes");
		corporate_action_id = cmd_parse.get<std::string>("corporate_action_id");
		entitlement_number = cmd_parse.get<std::string>("entitlement_number");
		dest_pbu_id = cmd_parse.get<std::string>("dest_pbu_id");
		dest_account_id = cmd_parse.get<std::string>("dest_account_id");
		repay_way = cmd_parse.get<ATPRepayWayType>("repay_way");
		tmp_close_out_flag = cmd_parse.get<std::string>("close_out_flag");
		cash_margin_set = cmd_parse.get<std::string>("cash_margin_set");
		opening_date = cmd_parse.get<ATPDate8Type>("opening_date");
		opening_serial_num = cmd_parse.get<std::string>("opening_serial_num");
		tmp_query_contents = cmd_parse.get<std::string>("query_contents");
		underlying_type = cmd_parse.get<ATPCRDUnderlyingTypeType>("underlying_type");

		old_password = cmd_parse.get<std::string>("old_password");
		fund_out_market_id = cmd_parse.get<ATPMarketIDType>("fund_out_market_id");
		fund_in_account_id = cmd_parse.get<std::string>("fund_in_account_id");
		fund_in_market_id = cmd_parse.get<ATPMarketIDType>("fund_in_market_id");
		value = cmd_parse.get<ATPAmtType>("value");
		extra_data = cmd_parse.get<std::string>("extra_data");
		trade_root_num = cmd_parse.get<std::string>("trade_root_num");
	}
	catch (cmdline::cmdline_error& e)
	{
		std::cout << e.what() << "\n";
		return -1;
	}
	catch (...)
	{
		std::cout << "parse cmdline error\n";
		return -1;
	}

	//char 型值在解析命令行时会报错，故使用string类型值tmp_xxx暂时存储解析的命令行的值
	side = tmp_side[0];
	order_type = tmp_order_type[0];
	order_way =	tmp_order_way[0];
	enforce_flag = tmp_enforce_flag[0];
	position_effect = tmp_position_effect[0];
	voting_preference = tmp_voting_preference[0];
	time_in_force = tmp_time_in_force[0];
	lot_type = tmp_lot_type[0];
	close_out_flag = tmp_close_out_flag[0];
	query_contents = tmp_query_contents[0];

	bid_position_effect = tmp_bid_position_effect[0];
	offer_position_effect = tmp_offer_position_effect[0];
	leg_side1 = tmp_leg_side1[0];
	leg_side2 = tmp_leg_side2[0];

	std::cout << "remote_address_1:" << remote_address_master << "\n";
	std::cout << "remote_address_2:" << remote_address_backup << "\n";
	std::cout << "user:" << user << "\n";
	std::cout << "password:" << password << "\n";
	std::cout << "start......" << std::endl;

	//Init()初始化API

	TerminalFeatureCode feature_code;

    const std::string station_name = ""; // 站点信息，该字段已经不使用
    const std::string cfg_path=".";      // 配置文件路径
    const std::string log_dir_path = ""; // 日志路径
    bool record_all_flag = true;         // 是否记录所有委托信息
    std::unordered_map<std::string,std::string> encrypt_cfg; // 加密库配置
    bool connection_retention_flag=false;   // 是否启用会话保持

    // encrypt_cfg参数填写：
    encrypt_cfg["ENCRYPT_SCHEMA"]="0";              // 字符 0 表示 不对消息中的所有 password 加密
    encrypt_cfg["ATP_ENCRYPT_PASSWORD"]="";         // 除登入及密码修改外其他消息的密码字段加密算法
    encrypt_cfg["ATP_LOGIN_ENCRYPT_PASSWORD"]="";   // 登入及密码修改消息中密码字段的加密算法so路径
    encrypt_cfg["GM_SM2_PUBLIC_KEY_PATH"]="";       // 采用国密算法时，通过该key配置 GM算法配置加密使用的公钥路径
    encrypt_cfg["RSA_PUBLIC_KEY_PATH"]="";          // 如果使用rsa算法加密，通过该key配置 rsa算法配置加密使用的公钥路径

    ATPRetCodeType ec = ATPTradeAPI::Init(station_name,cfg_path,log_dir_path,record_all_flag,encrypt_cfg,connection_retention_flag);
    if (ec != ErrorCode::kSuccess)
    {
        std::cout << "Init failed: " << ec <<std::endl;
        return -1;
    }

	ATPTradeAPI::SetFeatureCode(feature_code);
	
	
	int32_t result = 0 ;

	switch (send_to)
	{
	case ATPVersionTypeConst::kV210://V210版本老现货版本
	{
		atp_trade_api_client = new ATPTradeAPI(ATPVersionTypeConst::kV210);
		break;
	}
	case ATPVersionTypeConst::kV212: // V212 期权做市版本
	{
		atp_trade_api_client = new ATPTradeAPI(ATPVersionTypeConst::kV212);
		break;
	}
	case ATPVersionTypeConst::kV217:
	{
		atp_trade_api_client = new ATPTradeAPI(ATPVersionTypeConst::kV217);
		break;
	}
	case ATPVersionTypeConst::kV220:
	{
		atp_trade_api_client = new ATPTradeAPI(ATPVersionTypeConst::kV220);
		
		break;
	}
	case ATPVersionTypeConst::kV222:
	{
		atp_trade_api_client = new ATPTradeAPI(ATPVersionTypeConst::kV222);
		
		break;
	}
	default:
		std::cout << "unknow Version "<<send_to << " return "<< std::endl;
		return  -1;
		break;
	}

	
	result = Connect();
	if(result == -1)
	{
		g_exit=true ;
	}

	while (!g_exit)
	{
		bool sended = false ;
		if (g_login_success)
		{
			std::cout<<"g_login_success"<<std::endl;
			switch (number)
			{
			//ob 期权经济业务
			case MsgTypeNum::kOptionAuction:
			{
				SendOptionAuction();
				break;
			}
			case MsgTypeNum::kOptionExercise:
			{
				SendOptionExercise();
				break;
			}

			case MsgTypeNum::kCoveredLock:
			{
				SendCoveredLock();
				break;
			}
			
			case MsgTypeNum::kTurnToDisposal:
			{
				SendTurnToDisposal();
				break;
			}

			case MsgTypeNum::kCoverMaxOrderQtyQuery:
			{
				SendCoverMaxOrderQtyQuery();
				break;
			}
			
			case MsgTypeNum::kExtOptionFundTransfer:
			{
				SendExtOptionFundTransfer();
				break;
			}
				
			case MsgTypeNum::kAccountContractShareQuery:
			{
				SendAccountContractShareQuery();
				break;
			}
			case MsgTypeNum::kAccountContractFundQuery:
			{
				SendAccountContractFundQuery();
				break;
			}
			case MsgTypeNum::kExtQueryExerciseProfitLoss:
			{
				SendExtQueryExerciseProfitLoss();
				break;
			}
				
			//ob end
			case MsgTypeNum::kCashAuction:
			{
				SendCashOrder();
				break;
			}
			case MsgTypeNum::kCredit:
			{
				SendCreditOrder();
				break;
			}
			case MsgTypeNum::kMarginNoTrade:
			{
				SendMarginNoTradeOrder();
				break;
			}
			case MsgTypeNum::kMarginRepay:
			{
				SendMarginRepayOrder();
				break;
			}

			case MsgTypeNum::kRepoAuction:
			{
				SendRepoAuction();
				break;
			}
			case MsgTypeNum::kBondDistribution:
			{
				SendBondDistribution();
				break;
			}
			case MsgTypeNum::kETFRedemption:
			{
				SendETFRedemption();
				break;
			}
			case MsgTypeNum::kIssue:
			{
				SendIssue();
				break;
			}
			case MsgTypeNum::kRightsIssue:
			{
				SendRightsIssue();
				break;
			}
			case MsgTypeNum::kSwapPutback:
			{
				SendSwapPutback();
				break;
			}
		
			case MsgTypeNum::kDesignation:
			{
				SendDesignation();
				break;
			}
			case MsgTypeNum::kDesignatedTransaction:
			{
				SendDesignatedTransaction();
				break;
			}
			case MsgTypeNum::kLOF:
			{
				SendLOF();
				break;
			}
			case MsgTypeNum::kTenderOffer:
			{
				SendTenderOffer();
				break;
			}
			case MsgTypeNum::kNetVoting:
			{
				SendNetVoting();
				break;
			}
			case MsgTypeNum::kPasswordService:
			{
				SendPasswordService();
				break;
			}
			case MsgTypeNum::kWarrantExercise:
			{
				SendWarrantExercise();
				break;
			}
			case MsgTypeNum::kStructuredFund:
			{
				SendStructuredFund();
				break;
			}
	
			case MsgTypeNum::kTibAfterHourAuction:
			{
				SendTibAfterHourAuction();
				break;
			}
			case MsgTypeNum::kPledgeInOrOut:
			{
				SendPledgeInOrOut();
				break;
			}
			case MsgTypeNum::kAfterHoursPricing:
			{
				SendAfterHoursPricing();
				break;
			}
			case MsgTypeNum::kBlockTradeIntention:
			{
				SendBlockTradeIntention();
				break;
			}
			case MsgTypeNum::kBlockTradeTransaction:
			{
				SendBlockTradeTransaction();
				break;
			}
			case MsgTypeNum::kBlockTradePricing:
			{
				SendBlockTradePricing();
				break;
			}
			case MsgTypeNum::kCancel:
			{
				SendCancel();
				break;
			}
			case MsgTypeNum::kBatchCancel:
			{
				SendBatchCancel();
				break;
			}
			case MsgTypeNum::kFDEPCancel:
			{
				SendFDEPCancel();
				break;
			}
			case MsgTypeNum::kOptionCancel:
			{
				SendOptionCancel();
				break;
			}
			case MsgTypeNum::kBlockTradeCancel:
			{
				SendBlockTradeCancel();
				break;
			}
		
			case MsgTypeNum::kOrderQuery:
			{
				SendOrderQuery();
				break;
			}
			case MsgTypeNum::kTradeOrderQuery:
			{
				SendTradeOrderQuery();
				break;
			}
			case MsgTypeNum::kExtQueryContractSpecifications:
			{
				SendExtQueryContractSpecifications();
				break;
			}
			case MsgTypeNum::kExtQueryCreditAmount:
			{
				SendExtQueryCreditAmount();
				break;
			}
			case MsgTypeNum::kExtQueryMarginUseable:
			{
				SendExtQueryMarginUseable();
				break;
			}
			case MsgTypeNum::kExtQueryFundLiability:
			{
				SendExtQueryFundLiability();
				break;
			}
			case MsgTypeNum::kExtQueryShareLiability:
			{
				SendExtQueryShareLiability();
				break;
			}
			case MsgTypeNum::kExtQueryContractSpecificationsManager:
			{
				SendExtQueryContractSpecificationsManager();
				break;
			}
			case MsgTypeNum::kExtQueryMarginUnderlying:
			{
				SendExtQueryMarginUnderlying();
				break;
			}
			case MsgTypeNum::kExtQueryCreditInfo:
			{
				SendExtQueryCreditInfo();
				break;
			}
			case MsgTypeNum::kAgwMsgExtQueryFundAssetsInfo:
			{
				SendExtQueryFundAssetsInfo();
				break;
			}
			case MsgTypeNum::kFundQuery:
			{
				SendFundQuery();
				break;
			}
			case MsgTypeNum::kTradeOrderExQuery:
			{
				SendTradeOrderExQuery();
				break;
			}
			case MsgTypeNum::kShareQuery:
			{
				SendShareQuery();
				break;
			}
			case MsgTypeNum::kIPOQtyQuery:
			{
				SendIPOQtyQuery();
				break;
			}
		
			case MsgTypeNum::kTradeCollectQuery:
			{
				SendTradeCollectQuery();
				break;
			}
			case MsgTypeNum::kATPReqTIBIPOQtyQuery:
			{
				SendTIBIPOQtyQuery();
				break;
			}
			case MsgTypeNum::kNewMaxOrderQtyQuery:
			case MsgTypeNum::kMaxOrderQtyQuery:
			{
				SendNewMaxOrderQtyQuery();
				break;
			}
			case MsgTypeNum::kCustLogin:
			{
				std::cout<<"MsgTypeNum::kCustLogin"<<std::endl;
				SendCustLogin();
				break;
			}
			case MsgTypeNum::kCustLogout:
			{
				SendCustLogout();
				break;
			}
			case MsgTypeNum::kExtFundTransfer:
			{
				SendExtFundTransfer();
				break;
			}
			case MsgTypeNum::kExtExtRepay:
			{
				SendExtRepay();
				break;
			}
			case MsgTypeNum::kSyncBiz:
			{
				SendSyncBiz();
				break;
			}
			case MsgTypeNum::kOptionQuote:
			{
				SendOptionQuoteOrder();
				break;
			}
			case MsgTypeNum::kOptionCombination:
			{
				SendOptionCombinationOrder();
				break;
			}
			case MsgTypeNum::kOptionCombinationExercise:
			{
				SendOptionCombinationExerciseOrder();
				break;
			}
			case MsgTypeNum::kOptionQuoteQuery:
			{
				SendOptionQuoteQuery();
				break;
			}
			case MsgTypeNum::kOptionCombinationQuery:
			{
				SendOptionCombinationQuery();
				break;
			}
			case MsgTypeNum::kReduceHoldingTransfer:
			{
				SendReduceHoldingTransfer();
				break;
			}
			case MsgTypeNum::kReduceHoldingTransferCancel:
			{
				SendReduceHoldingTransferCancel();
				break;
			}
			case MsgTypeNum::kHKStocksThrough:
			{
				SendHKStocksThroughAuction();
				break;
			}
			case MsgTypeNum::kHKVoting:
			{
				SendHKVoting();
				break;
			}
			case MsgTypeNum::kHKCorporateAction:
			{
				SendHKCorporateAction();
				break;
			}
			case MsgTypeNum::kHKDesignationTransfer:
			{
				SendHKDesignationTransfer();
				break;
			}
			case MsgTypeNum::kHKCorporateActionCancel:
			{
				SendHKCorporateActionCancel();
				break;
			}
			case MsgTypeNum::kAgwMsgOptionCoveredTransUncoveredOrder:        // ///< 期权普通与备兑互转订单
			{
				SendAgwMsgOptionCoveredTransUncoveredOrder();
				break;
			}
			case MsgTypeNum::kAgwMsgExtQueryCombinExerciseMaxOrderQty:        // ///< 合并行权最大可委托数查询消息
			{
				SendAgwMsgExtQueryCombinExerciseMaxOrderQty();
				break;
			}
			case MsgTypeNum::kAgwMsgExtQueryCombinExerciseProfitLoss:        // ///< 合并行权预估盈亏查询消息
			{
				SendAgwMsgExtQueryCombinExerciseProfitLoss();
				break;
			}
			case MsgTypeNum::kAgwMsgExternalQueryOptionCombinationMaxOrderQty:        // ///< 期权组合策略最大可委托数查询消息
			{
				SendAgwMsgExternalQueryOptionCombinationMaxOrderQty();
				break;
			}
			case MsgTypeNum::kAgwMsgCustPasswdModify:        // ///< 客户密码修改请求
			{
				SendAgwMsgCustPasswdModify();
				break;
			}
			case MsgTypeNum::kAgwExternalInsTETransFundReq:        // ///< 按证券账户划转资金请求消息
			{
				SendAgwExternalInsTETransFundReq();
				break;
			}
			case MsgTypeNum::kAgwMsgExtQueryFundExReq:        // ///< 增强资金查询请求消息
			{
				SendAgwMsgExtQueryFundExReq();
				break;
			}
			case MsgTypeNum::kAgwMsgThirdPartPassThrough:
			{
				SendAgwMsgThirdPartPassThrough();
				break;
			}
			case MsgTypeNum::kBankSecuritiesFundTransfer:
			{
				SendBankSecuritiesFundTransfer();
				break;
			}
			default:
				std::cout<<"unknow msg_type:"<< number <<std::endl;
			}
			sended = true ;
		}

		if(sended)
		{
			break ;
		}
		
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));// 等待消息接收后关闭连接
	
	system("pause");
	std::cout << "client will close " << g_exit << std::endl;
	//主动关闭连接
	if(NULL != atp_trade_api_client)
	{
		atp_trade_api_client->Close();
	}
	printf("atp trade api demo exit \n");
	//std::this_thread::sleep_for(std::chrono::seconds(1));

	ATPTradeAPI::Stop(); 

	return 0;
}
