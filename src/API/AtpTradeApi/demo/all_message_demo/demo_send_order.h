#ifndef _DEMO_SEND_ORDER_H_
#define _DEMO_SEND_ORDER_H_
#include <iostream>
//#include <chrono>
#include <map>
#include <vector>
//#include <thread>
#include <string>
#include <stdint.h>
//#include <atomic>


/**
	* @brief 获取系列号
	*/
uint64_t GenerateClientSeqID();
/*
* @brief 发送现货集中竞价业务委托消息
*/
void SendCashOrder();
/*
* @brief 发送融资融券交易委托消息
*/
void SendCreditOrder();
/*
* @brief 发送融资融券非交易业务消息
*/
void SendMarginNoTradeOrder();
/*
* @brief 发送融资融券交易偿还业务消息
*/
void SendMarginRepayOrder();

/*
* @brief 发送质押式回购集中竞价业务委托消息
*/
void SendRepoAuction();
/*
* @brief 发送债券分销业务委托消息
*/
void SendBondDistribution();
/*
* @brief 发送期权集中竞价业务委托消息
*/
void SendOptionAuction();

/*
* @brief 发送期权报价业务委托消息
*/
void SendOptionQuoteOrder();
/*
* @brief 发送期权组合策略保证金业务委托消息
*/
void SendOptionCombinationOrder();
/*
* @brief 发送期权合并行权业务委托消息
*/
void SendOptionCombinationExerciseOrder();

/*
* @brief 发送ETF申赎业务委托消息 
*/
void SendETFRedemption();
/*
* @brief 发送网上发行业务委托消息
*/
void SendIssue();
/*
* @brief 发送配售业务委托消息
*/
void SendRightsIssue();
/*
* @brief 发送债券转股回售业务委托消息
*/
void SendSwapPutback();
/*
* @brief 发送期权行权业务委托消息
*/
void SendOptionExercise();

//备兑锁定解锁业务委托消息
void SendCoveredLock();

//转处置
void SendTurnToDisposal();
/*
* @brief 发送转托管业务委托消息
*/
void SendDesignation();
/*
* @brief 发送指定交易业务委托消息
*/
void SendDesignatedTransaction();
/*
* @brief 发送开放式基金申赎业务委托消息
*/
void SendLOF();
/*
* @brief 发送要约收购业务委托消息
*/
void SendTenderOffer();
/*
* @brief 发送网络投票业务委托消息
*/
void SendNetVoting();
/*
* @brief 发送密码服务业务委托消息
*/
void SendPasswordService();
/*
* @brief 发送权证行权业务委托消息
*/
void SendWarrantExercise();
/*
* @brief 发送分级基金实时分拆合并业务委托消息
*/
void SendStructuredFund();

/*
* @brief 发送盘后定价(科创板、创业板)业务委托消息
*/
void SendTibAfterHourAuction();

/*
* @brief 质押出入库业务委托消息
*/
void SendPledgeInOrOut();

//外部期权资金账户资金划拨请求
void SendExtOptionFundTransfer();
/*
* @brief 发送通用撤单消息
*/
void SendCancel();

//高管额度划拨请求
void SendReduceHoldingTransfer();
/*
* @brief 高管额度划转撤单消息
*/
void SendReduceHoldingTransferCancel();

/*
* @brief 发送一键撤单消息
*/
void SendBatchCancel();
/*
* @brief 发送FDEP公共撤单消息
*/
void SendFDEPCancel();
/*
* @brief 发送期权撤单消息
*/
void SendOptionCancel();

//期权最大可委托数查询
void SendCoverMaxOrderQtyQuery();
/*
* @brief 发送订单查询消息
*/
void SendOrderQuery();
/*
* @brief 发送成交查询消息
*/
void SendTradeOrderQuery();
/*
* @brief 发送融资融券合约明细查询消息
*/
void SendExtQueryContractSpecifications();
/*
* @brief 发送融资融券信用额度查询消息
*/
void SendExtQueryCreditAmount();
/*
* @brief 发送融资融券保证金可用查询消息
*/
void SendExtQueryMarginUseable();
/*
* @brief 发送融资融券资金负债查询消息
*/
void SendExtQueryFundLiability();
/*
* @brief 发送融资融券股份负债查询消息
*/
void SendExtQueryShareLiability();
/*
* @brief 发送融资融券合约管理查询消息
*/
void SendExtQueryContractSpecificationsManager();
/*
* @brief 发送融资融券标的清单查询消息
*/
void SendExtQueryMarginUnderlying();
/*
* @brief 发送融资融券客户信用资料查询消息
*/
void SendExtQueryCreditInfo();
/*
* @brief 发送融资融券资产信息查询消息
*/
void SendExtQueryFundAssetsInfo();
/*
* @brief 发送资金查询
*/
void SendFundQuery();

/*
* @brief 增强成交查询
*/

void SendTradeOrderExQuery();

/*
* @brief 发送股份持仓查询
*/
void SendShareQuery();
/*
* @brief 发送新股发行申购额度查询消息
*/
void SendIPOQtyQuery();
/*
* @brief 发送合约账户持仓查询消息
*/
void SendAccountContractShareQuery();

//行权预估盈亏查询消息
void SendExtQueryExerciseProfitLoss();
/*
* @brief 发送指定合约账户资金查询消息
*/
void SendAccountContractFundQuery();
/*
* @brief 发送成交汇总查询消息
*/
void SendTradeCollectQuery();
/*
* @brief 发送科创板新股发行申购额度查询消息
*/
void SendTIBIPOQtyQuery();
/*
* @brief 组合策略持仓查询消息
*/
void SendOptionCombinationQuery();
/*
* @brief 报价订单查询消息
*/
void SendOptionQuoteQuery();
/*
* @brief 发送增强型最大可委托数查询消息
*/
void SendNewMaxOrderQtyQuery();
/*
* @brief 发送客户登入验证消息
*/
void SendCustLogin();
/*
* @brief 发送客户登出消息
*/
void SendCustLogout();
/*
* @brief 发送外部资金划拨请求
*/
void SendExtFundTransfer();
/*
* @brief 发送融资融券直接还款请求
*/
void SendExtRepay();
/*
* @brief 发送业务层回报同步消息
*/
void SendSyncBiz();
/*
* @brief 发送港股通业务委托消息
*/
void SendHKStocksThroughAuction();
/*
* @brief 发送港股通投票业务委托消息
*/
void SendHKVoting();
/*
* @brief 发送港股通公司行为业务委托消息
*/
void SendHKCorporateAction();
/*
* @brief 发送港股通转托管业务委托消息
*/
void SendHKDesignationTransfer();
/*
* @brief 发送港股通公司行为撤单消息
*/
void SendHKCorporateActionCancel();
/*
* @brief 发送 ///< 期权普通与备兑互转订单
*/
void SendAgwMsgOptionCoveredTransUncoveredOrder();
/*
* @brief 发送 ///< 合并行权最大可委托数查询消息
*/
void SendAgwMsgExtQueryCombinExerciseMaxOrderQty();
/*
* @brief 发送 ///< 合并行权预估盈亏查询消息
*/
void SendAgwMsgExtQueryCombinExerciseProfitLoss();
/*
* @brief 发送 ///< 期权组合策略最大可委托数查询消息
*/
void SendAgwMsgExternalQueryOptionCombinationMaxOrderQty();

/*
* @brief 发送大宗盘后定价委托消息
*/
void SendAfterHoursPricing();

/*
* @brief 发送大宗意向申报委托消息
*/
void SendBlockTradeIntention();

/*
* @brief 发送大宗成交申报委托消息
*/
void SendBlockTradeTransaction();

/*
* @brief 发送大宗定价申报委托消息
*/
void SendBlockTradePricing();

/*
* @brief 发送大宗交易撤单消息
*/
void SendBlockTradeCancel();
/*
* @brief 发送 ///< 客户密码修改请求
*/
void SendAgwMsgCustPasswdModify();
/*
* @brief 发送 ///< 按证券账户划转资金请求消息
*/
void SendAgwExternalInsTETransFundReq();
/*
* @brief 发送 ///< 增强资金查询请求消息
*/
void SendAgwMsgExtQueryFundExReq();

/*
* @brief 发送 ///< 透传第三方OES通用接口
*/
void SendAgwMsgThirdPartPassThrough();

/*
* @brief 发送 ///< 银证转账请求消息
*/
void SendBankSecuritiesFundTransfer();

#endif
 
 
