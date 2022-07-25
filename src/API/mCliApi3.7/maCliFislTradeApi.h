#if !defined(__MA_CLI_FISL_TRADE_API_H__)
#define __MA_CLI_FISL_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliFislTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFislTradeSpi : virtual public CCliTradeSpi
{
public:
  // 标的券信息查询响应
  virtual int OnRspQryUndlStkInfo(CFirstSetField *p_pFirstSetField, CRspFislQryUndlStkInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 担保证券信息查询响应
  virtual int OnRspQryColStkInfo(CFirstSetField *p_pFirstSetField, CRspFislQryColStkInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // LOF信息查询
  virtual int OnRspQryLofInfo(CFirstSetField *p_pFirstSetField, CRspFislQryLofInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 买卖委托响应
  virtual int OnRspOrder(CFirstSetField *p_pFirstSetField, CRspFislOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 委托撤单响应
  virtual int OnRspCancelOrder(CFirstSetField *p_pFirstSetField, CRspFislCancelOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 最大可交易数计算响应
  virtual int OnRspMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspFislMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券直接还款响应
  virtual int OnRspMarginRepay(CFirstSetField *p_pFirstSetField, CRspFislMarginRepayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应
  virtual int OnRspQryExpendableFund(CFirstSetField *p_pFirstSetField, CRspFislQryExpendableFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用股份查询响应
  virtual int OnRspQryExpendableShares(CFirstSetField *p_pFirstSetField, CRspFislQryExpendableSharesField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日委托查询响应
  virtual int OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspFislQryCurrDayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日成交查询响应
  virtual int OnRspQryCurrDayFill(CFirstSetField *p_pFirstSetField, CRspFislQryCurrDayFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股东账户查询响应
  virtual int OnRspQryStkholderAcct(CFirstSetField *p_pFirstSetField, CRspFislQryStkholderAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可撤单委托查询响应
  virtual int OnRspQryCanWithdrawOrder(CFirstSetField *p_pFirstSetField, CRspFislQryCanWithdrawOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应[快速]
  virtual int OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, CRspFislQryExpendableFundExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用股份查询响应[快速]
  virtual int OnRspQryExpendableSharesEx(CFirstSetField *p_pFirstSetField, CRspFislQryExpendableSharesExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日委托查询响应[快速]
  virtual int OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, CRspFislQryCurrDayOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日成交查询响应[快速]
  virtual int OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, CRspFislQryCurrDayFillExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股东账户查询响应[快速]
  virtual int OnRspQryStkholderAcctEx(CFirstSetField *p_pFirstSetField, CRspFislQryStkholderAcctExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可撤单委托查询响应[快速]
  virtual int OnRspQryCanWithdrawOrderEx(CFirstSetField *p_pFirstSetField, CRspFislQryCanWithdrawOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券协议查询响应
  virtual int OnRspQryMarginProtocol(CFirstSetField *p_pFirstSetField, CRspFislQryMarginProtocolField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券合约查询响应
  virtual int OnRspQryMarginContract(CFirstSetField *p_pFirstSetField, CRspFislQryMarginContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券保证金可用余额查询响应
  virtual int OnRspQryMarginDepositAvl(CFirstSetField *p_pFirstSetField, CRspFislQryMarginDepositAvlField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券信用可用额度查询响应
  virtual int OnRspQryMarginCreditAvl(CFirstSetField *p_pFirstSetField, CRspFislQryMarginCreditAvlField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 信用客户资产负债查询响应
  virtual int OnRspQryCreditCustDebts(CFirstSetField *p_pFirstSetField, CRspFislQryCreditCustDebtsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券资金头寸查询响应
  virtual int OnRspQryMarginFundCash(CFirstSetField *p_pFirstSetField, CRspFislQryMarginFundCashField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资融券股份头寸查询响应
  virtual int OnRspQryMarginSharesCash(CFirstSetField *p_pFirstSetField, CRspFislQryMarginSharesCashField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 可偿还金额查询响应
  virtual int OnRspQryCanRepay(CFirstSetField *p_pFirstSetField, CRspFislQryCanRepayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 证券市值额度查询响应
  virtual int OnRspQryMktQuota(CFirstSetField *p_pFirstSetField, CRspFislMktQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融资合约汇总查询响应
  virtual int OnRspQryFiContractSum(CFirstSetField *p_pFirstSetField, CRspFislQryFiContractSumField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 融券合约汇总查询响应
  virtual int OnRspQrySlContractSum(CFirstSetField *p_pFirstSetField, CRspFislQrySlContractSumField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 资金划拨响应
  virtual int OnRspQryFundTransfer(CFirstSetField *p_pFirstSetField, CRspFislFundTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 用户登录请求响应
  virtual int OnRspUserLogin(CFirstSetField *p_pFirstSetField, CRspFislUserLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnFislOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}

  // 成交回报
  virtual int OnRtnOrderFill(CRtnFislOrderFillField *p_pRtnOrderFillField) {return 0;}

  // 申赎成交推送
  virtual int OnRtnOrderFill(CRtnFislPefEtfFillField *p_pRtnOrderFillField) {return 0;}

  // 融资融券负债偿还流水查询[信用]响应
  virtual int OnRspQryRpcRepaymentDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcRepaymentDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询[信用]响应
  virtual int OnRspQryRpcFund(CFirstSetField *p_pFirstSetField, CRspFislQryRpcFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日资金流水查询[信用]响应
  virtual int OnRspQryRpcFundDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcFundDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股申购客户市值额度查询[信用]响应
  virtual int OnRspQryRpcIPOSubscriptionLimit(CFirstSetField *p_pFirstSetField, CRspFislQryRpcIPOSubscriptionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转帐[信用]响应
  virtual int OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, CRspFislRpcBankStkTransField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询账户余额[信用]响应
  virtual int OnRspQryRpcFundBala(CFirstSetField *p_pFirstSetField, CRspFislQryRpcFundBalaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转帐查询[信用]响应
  virtual int OnRspQryRpcBankStkTransInfo(CFirstSetField *p_pFirstSetField, CRspFislQryRpcBankStkTransInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改资金密码[信用]响应
  virtual int OnRspRpcChgFundPwd(CFirstSetField *p_pFirstSetField, CRspFislRpcChgFundPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改交易密码[信用]响应
  virtual int OnRspRpcChgTrdPwd(CFirstSetField *p_pFirstSetField, CRspFislRpcChgTrdPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 获取开通转帐(银行)账号(410601) [信用]响应
  virtual int OnRspQryRpcBankAcct(CFirstSetField *p_pFirstSetField, CRspFislQryRpcBankAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 获取开通外部(银行)账号(410602) [信用]响应
  virtual int OnRspQryRpcOtherBankAcct(CFirstSetField *p_pFirstSetField, CRspFislQryRpcOtherBankAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日新股清单查询(411549) [信用]响应
  virtual int OnRspQryRpcIPO(CFirstSetField *p_pFirstSetField, CRspFislQryRpcIPOField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 配号查询(411518) [信用]响应
  virtual int OnRspQryRpcDistribution(CFirstSetField *p_pFirstSetField, CRspFislQryRpcDistributionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 市值配售中签查询(411547) [信用]响应
  virtual int OnRspQryRpcMktCapPlacing(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMktCapPlacingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 中签历史查询(411560) [信用]响应
  virtual int OnRspQryRpcWinningHistory(CFirstSetField *p_pFirstSetField, CRspFislQryRpcWinningHistoryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 市值配售交收顺序修改（411548）[信用]响应
  virtual int OnRspRpcChgMktCapDistDeliOrder(CFirstSetField *p_pFirstSetField, CRspFislRpcChgMktCapDistDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 交割单查询(411520) [信用]响应
  virtual int OnRspQryRpcDeliOrder(CFirstSetField *p_pFirstSetField, CRspFislQryRpcDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 对帐查询(411521) [信用]响应
  virtual int OnRspQryRpcChkAcct(CFirstSetField *p_pFirstSetField, CRspFislQryRpcChkAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托汇总(410514) [信用]响应
  virtual int OnRspQryRpcOrderSummary(CFirstSetField *p_pFirstSetField, CRspFislQryRpcOrderSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交汇总(410516) [信用]响应
  virtual int OnRspQryRpcMatchSummary(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMatchSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托明细查询(411511) [信用]响应
  virtual int OnRspQryRpcHistoryOrderDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcHistoryOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交明细查询(411513) [信用]响应
  virtual int OnRspQryRpcHistoryMatchDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcHistoryMatchDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 转帐银行业务信息(410211) [信用]响应
  virtual int OnRspQryRpcBankTransBasicInfo(CFirstSetField *p_pFirstSetField, CRspFislQryRpcBankTransBasicInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 对帐单查询(411525) [信用]响应
  virtual int OnRspQryRpcStatement(CFirstSetField *p_pFirstSetField, CRspFislQryRpcStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券信息响应
  virtual int OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, CRspFislRpcStkInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 机构编码信息(业务机构)响应
  virtual int OnRspRpcOrgInfo(CFirstSetField *p_pFirstSetField, CRspFislRpcOrgInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取字典信息响应
  virtual int OnRspRpcDictInfo(CFirstSetField *p_pFirstSetField, CRspFislRpcDictInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询当前系统状态响应
  virtual int OnRspQryRpcCurSysStat(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCurSysStatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 用户登录响应
  virtual int OnRspRpcLogin(CFirstSetField *p_pFirstSetField, CRspFislRpcLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东挂失响应
  virtual int OnRspRpcTrdacctReportLoss(CFirstSetField *p_pFirstSetField, CRspFislRpcTrdacctReportLossField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金帐户挂失响应
  virtual int OnRspRpcCuacctReportLoss(CFirstSetField *p_pFirstSetField, CRspFislRpcCuacctReportLossField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置主股东帐户响应
  virtual int OnRspRpcSetMainTrdacct(CFirstSetField *p_pFirstSetField, CRspFislRpcSetMainTrdacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置主资产账号响应
  virtual int OnRspRpcSetMainCuacct(CFirstSetField *p_pFirstSetField, CRspFislRpcSetMainCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改联系方式(网上交易使用)响应
  virtual int OnRspRpcChgContact(CFirstSetField *p_pFirstSetField, CRspFislRpcChgContactField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户资料查询响应
  virtual int OnRspQryRpcCustInfo(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCustInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改代理权限响应
  virtual int OnRspRpcChgProxyPermisson(CFirstSetField *p_pFirstSetField, CRspFislRpcChgProxyPermissonField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改操作方式响应
  virtual int OnRspRpcChgOperMode(CFirstSetField *p_pFirstSetField, CRspFislRpcChgOperModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户操作方式响应
  virtual int OnRspQryRpcCustOperMode(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCustOperModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改客户联系人信息响应
  virtual int OnRspRpcChgCustInfo(CFirstSetField *p_pFirstSetField, CRspFislRpcChgCustInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户资金帐号权限响应
  virtual int OnRspQryRpcCuacctPermission(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCuacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改资金帐号权限响应
  virtual int OnRspRpcChgCuacctPermission(CFirstSetField *p_pFirstSetField, CRspFislRpcChgCuacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户证券帐号权限响应
  virtual int OnRspQryRpcTrdacctPermission(CFirstSetField *p_pFirstSetField, CRspFislQryRpcTrdacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户证券帐号权限响应
  virtual int OnRspRpcSetTrdacctPermission(CFirstSetField *p_pFirstSetField, CRspFislRpcSetTrdacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券额度上限查询响应
  virtual int OnRspQryRpcUpLimitQuota(CFirstSetField *p_pFirstSetField, CRspFislQryRpcUpLimitQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券额度变更响应
  virtual int OnRspRpcChgLimitQuota(CFirstSetField *p_pFirstSetField, CRspFislRpcChgLimitQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 额度变更申请响应
  virtual int OnRspRpcChgLimitQuotaApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcChgLimitQuotaApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 额度变更申请取消响应
  virtual int OnRspRpcChgLimitQuotaApplicationCancel(CFirstSetField *p_pFirstSetField, CRspFislRpcChgLimitQuotaApplicationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 额度变更申请查询响应
  virtual int OnRspQryRpcChgLimitQuotaApplication(CFirstSetField *p_pFirstSetField, CRspFislQryRpcChgLimitQuotaApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取证券余额及当前最新价等,支持三方交易响应
  virtual int OnRspRpcStkAvlAndLatestPrice(CFirstSetField *p_pFirstSetField, CRspFislRpcStkAvlAndLatestPriceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取最大交易数量,支持三方交易响应
  virtual int OnRspRpcMaxStkQty(CFirstSetField *p_pFirstSetField, CRspFislRpcMaxStkQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托买卖业务,支持三方交易，外围约定融资融券委托响应
  virtual int OnRspRpcOrder(CFirstSetField *p_pFirstSetField, CRspFislRpcOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单响应
  virtual int OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspFislRpcOrderCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单查询响应
  virtual int OnRspQryRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspFislQryRpcOrderCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 重置成本响应
  virtual int OnRspRpcResetCost(CFirstSetField *p_pFirstSetField, CRspFislRpcResetCostField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盈亏费用计算响应
  virtual int OnRspRpcCalcFee(CFirstSetField *p_pFirstSetField, CRspFislRpcCalcFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户查询，查询客户股东代码响应
  virtual int OnRspQryRpcTrdacct(CFirstSetField *p_pFirstSetField, CRspFislQryRpcTrdacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询响应
  virtual int OnRspQryRpcCuacct(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份明细查询响应
  virtual int OnRspQryRpcStkDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份汇总查询响应
  virtual int OnRspQryRpcStkSummary(CFirstSetField *p_pFirstSetField, CRspFislQryRpcStkSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金帐户查询响应
  virtual int OnRspQryRpcCuacct2(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCuacct2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户查询，根据股东内码，查询股东代码响应
  virtual int OnRspQryRpcTrdacct2(CFirstSetField *p_pFirstSetField, CRspFislQryRpcTrdacct2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户查询，根据股东代码，资金帐户查询客户代码响应
  virtual int OnRspQryRpcTrdacct3(CFirstSetField *p_pFirstSetField, CRspFislQryRpcTrdacct3Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托明细查询响应
  virtual int OnRspQryRpcOrderDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交明细查询响应
  virtual int OnRspQryRpcMatchDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMatchDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托汇总－按证券代码和买卖方向响应
  virtual int OnRspRpcHisOrderSummary(CFirstSetField *p_pFirstSetField, CRspFislRpcHisOrderSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交汇总－按证券代码和买卖方向响应
  virtual int OnRspRpcHisMatchSummary(CFirstSetField *p_pFirstSetField, CRspFislRpcHisMatchSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 中签查询响应
  virtual int OnRspQryRpcLottery(CFirstSetField *p_pFirstSetField, CRspFislQryRpcLotteryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托汇总ordergroup+stkcode+bsflag响应
  virtual int OnRspRpcOrderSummary(CFirstSetField *p_pFirstSetField, CRspFislRpcOrderSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 批量委托查询【增加】响应
  virtual int OnRspQryRpcBatchOrder(CFirstSetField *p_pFirstSetField, CRspFislQryRpcBatchOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 信用资金可取查询响应
  virtual int OnRspQryRpcCreditCuacctCanDraw(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCreditCuacctCanDrawField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票查询行情响应
  virtual int OnRspQryRpcMkt(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 意向行情查询响应
  virtual int OnRspQryRpcMktIntention(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMktIntentionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询营业部某一业务类别的申报席位响应
  virtual int OnRspQryRpcSeat(CFirstSetField *p_pFirstSetField, CRspFislQryRpcSeatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询资金账户信息响应
  virtual int OnRspQryRpcCuacctAcctInfo(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCuacctAcctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户特殊手续费查询响应
  virtual int OnRspQryRpcCustSpecialFee(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCustSpecialFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股息红利税扣收明细查询响应
  virtual int OnRspQryRpcDividendBonusTaxDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcDividendBonusTaxDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设定定期转帐转出计划响应
  virtual int OnRspRpcSetPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, CRspFislRpcSetPeriodicalTransferPlanField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询定期转帐转出计划响应
  virtual int OnRspQryRpcPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, CRspFislQryRpcPeriodicalTransferPlanField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 删除定期转帐转出计划响应
  virtual int OnRspRpcDelPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, CRspFislRpcDelPeriodicalTransferPlanField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 担保证券查询响应
  virtual int OnRspQryRpcGuaranteedStk(CFirstSetField *p_pFirstSetField, CRspFislQryRpcGuaranteedStkField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 标的证券查询响应
  virtual int OnRspQryRpcUnderlyingStk(CFirstSetField *p_pFirstSetField, CRspFislQryRpcUnderlyingStkField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资信用数据查询响应
  virtual int OnRspQryRpcFinancingCreditData(CFirstSetField *p_pFirstSetField, CRspFislQryRpcFinancingCreditDataField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券信用数据查询响应
  virtual int OnRspQryRpcMarginCreditData(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMarginCreditDataField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 信用客户资产负债查询响应
  virtual int OnRspQryRpcCreditCustBalance(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCreditCustBalanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券合约查询响应
  virtual int OnRspQryRpcContract(CFirstSetField *p_pFirstSetField, CRspFislQryRpcContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合同信息查询响应
  virtual int OnRspQryRpcAgreementInfo(CFirstSetField *p_pFirstSetField, CRspFislQryRpcAgreementInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围约定直接还款、直接还款响应
  virtual int OnRspRpcDirectRepayment(CFirstSetField *p_pFirstSetField, CRspFislRpcDirectRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询券商融券帐户头寸响应
  virtual int OnRspQryRpcMarginAcctPosi(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMarginAcctPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 信用客户可转入担保证券查询响应
  virtual int OnRspQryRpcCreditCustCanTransferInGuaranteedStk(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCreditCustCanTransferInGuaranteedStkField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 投票方案查询响应
  virtual int OnRspQryRpcVotingScheme(CFirstSetField *p_pFirstSetField, CRspFislQryRpcVotingSchemeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 投票下单响应
  virtual int OnRspRpcVotingOrder(CFirstSetField *p_pFirstSetField, CRspFislRpcVotingOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 投票结果查询响应
  virtual int OnRspQryRpcVotingResult(CFirstSetField *p_pFirstSetField, CRspFislQryRpcVotingResultField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 投票结果统计响应
  virtual int OnRspRpcVotingResultStatistics(CFirstSetField *p_pFirstSetField, CRspFislRpcVotingResultStatisticsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户可投票议案查询响应
  virtual int OnRspQryRpcCanVoteBill(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCanVoteBillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户可投票指定议案的股份查询响应
  virtual int OnRspQryRpcCanVoteBillShare(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCanVoteBillShareField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票历史股份余额查询响应
  virtual int OnRspQryRpcHisShareBalance(CFirstSetField *p_pFirstSetField, CRspFislQryRpcHisShareBalanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券帐号自助激活响应
  virtual int OnRspRpcSelfActivate(CFirstSetField *p_pFirstSetField, CRspFislRpcSelfActivateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合同信息修改响应
  virtual int OnRspRpcChgAgreementInfo(CFirstSetField *p_pFirstSetField, CRspFislRpcChgAgreementInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户账户级费用查询响应
  virtual int OnRspQryRpcAcctLvlFee(CFirstSetField *p_pFirstSetField, CRspFislQryRpcAcctLvlFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券自定义合约偿还顺序查询响应
  virtual int OnRspQryRpcCustomContractRepayOrder(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCustomContractRepayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券自定义合约偿还顺序设置响应
  virtual int OnRspRpcSetCustomContractRepayOrder(CFirstSetField *p_pFirstSetField, CRspFislRpcSetCustomContractRepayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券特殊专户证券查询响应
  virtual int OnRspQryRpcSpecialMarginAcctStk(CFirstSetField *p_pFirstSetField, CRspFislQryRpcSpecialMarginAcctStkField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券历史已了结合约查询响应
  virtual int OnRspQryRpcHisClosedContract(CFirstSetField *p_pFirstSetField, CRspFislQryRpcHisClosedContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围信用交易属性查询响应
  virtual int OnRspQryRpcCreditTradeAttribute(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCreditTradeAttributeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约展期申请响应
  virtual int OnRspRpcContractExtensionApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcContractExtensionApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户等比额度释放标志修改响应
  virtual int OnRspRpcChgGeometricQuotaReleaseFlag(CFirstSetField *p_pFirstSetField, CRspFislRpcChgGeometricQuotaReleaseFlagField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约展期申请查询响应
  virtual int OnRspQryRpcContractExtensionApplication(CFirstSetField *p_pFirstSetField, CRspFislQryRpcContractExtensionApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约展期申请取消响应
  virtual int OnRspRpcContractExtensionApplicationCancel(CFirstSetField *p_pFirstSetField, CRspFislRpcContractExtensionApplicationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围风险测评类型查询响应
  virtual int OnRspQryRpcRiskEvaluationMode(CFirstSetField *p_pFirstSetField, CRspFislQryRpcRiskEvaluationModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 展期规则查询响应
  virtual int OnRspQryRpcExtensionRule(CFirstSetField *p_pFirstSetField, CRspFislQryRpcExtensionRuleField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可展期合约查询响应
  virtual int OnRspQryRpcCanExtendContract(CFirstSetField *p_pFirstSetField, CRspFislQryRpcCanExtendContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户担保权益调整明细查询响应
  virtual int OnRspQryRpcGuaranteeInterestAdjustmentDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRpcGuaranteeInterestAdjustmentDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户历史维持担保比例查询响应
  virtual int OnRspQryRpcHisMaintainGuaranteeRatio(CFirstSetField *p_pFirstSetField, CRspFislQryRpcHisMaintainGuaranteeRatioField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资头寸预约申请响应
  virtual int OnRspRpcFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcFinancingPosiSubApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资头寸预约申请查询响应
  virtual int OnRspQryRpcFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, CRspFislQryRpcFinancingPosiSubApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资头寸变更预约申请响应
  virtual int OnRspRpcChgFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcChgFinancingPosiSubApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取消融资头寸预约申请响应
  virtual int OnRspRpcCancelFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcCancelFinancingPosiSubApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户关联融资头寸查询响应
  virtual int OnRspQryRpcLinkFinancingPosi(CFirstSetField *p_pFirstSetField, CRspFislQryRpcLinkFinancingPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置默认融资开仓头寸响应
  virtual int OnRspRpcSetDefaultFinancingOpenPosi(CFirstSetField *p_pFirstSetField, CRspFislRpcSetDefaultFinancingOpenPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券头寸预约申请响应
  virtual int OnRspRpcMarginPosiSubApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcMarginPosiSubApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券头寸变更申请响应
  virtual int OnRspRpcChgMarginPosiApplication(CFirstSetField *p_pFirstSetField, CRspFislRpcChgMarginPosiApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券头寸预约查询响应
  virtual int OnRspQryRpcMarginPosiSub(CFirstSetField *p_pFirstSetField, CRspFislQryRpcMarginPosiSubField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券开仓头寸指定响应
  virtual int OnRspRpcMarginOpenPosiAppoint(CFirstSetField *p_pFirstSetField, CRspFislRpcMarginOpenPosiAppointField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户关联融券头寸查询响应
  virtual int OnRspQryRpcLinkMarginPosi(CFirstSetField *p_pFirstSetField, CRspFislQryRpcLinkMarginPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券头寸预约取消响应
  virtual int OnRspRpcCancelMarginPosiSub(CFirstSetField *p_pFirstSetField, CRspFislRpcCancelMarginPosiSubField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 解冻中签日预冻结的资金响应
  virtual int OnRspRpcUnfreezePreFrozenFunds(CFirstSetField *p_pFirstSetField, CRspFislRpcUnfreezePreFrozenFundsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 放弃配售缴款, 支持三方交易响应
  virtual int OnRspRpcCancelRation(CFirstSetField *p_pFirstSetField, CRspFislRpcCancelRationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 系统探测响应
  virtual int OnRspBizStatus(CFirstSetField *p_pFirstSetField, CRspFislBizStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 质押股份明细查询(单客户)响应
  virtual int OnRspQryPledgeShares(CFirstSetField *p_pFirstSetField, CRspFislQryPledgeSharesField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金流水查询响应
  virtual int OnRspQryFundFlow(CFirstSetField *p_pFirstSetField, CRspFislQryFundFlowField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 买卖委托(快速)响应
  virtual int OnRspOrderEx(CFirstSetField *p_pFirstSetField, CRspFislOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份流水查询响应
  virtual int OnRspQryStockFlow(CFirstSetField *p_pFirstSetField, CRspFislQryStockFlowField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 日切交易日查询响应
  virtual int OnRspQrySettDate(CFirstSetField *p_pFirstSetField, CRspFislQrySettDateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金划拨(融资融券)响应
  virtual int OnRspFundTransferJzjy(CFirstSetField *p_pFirstSetField, CRspFislFundTransferJzjyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券合约展期申请响应
  virtual int OnRspContApply(CFirstSetField *p_pFirstSetField, CRspFislContApplyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转账响应
  virtual int OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, CRspFislBankStkTransferField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融券头寸明细查询响应
  virtual int OnRspQryCashDetail(CFirstSetField *p_pFirstSetField, CRspFislQryCashDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询偿还明细响应
  virtual int OnRspQryRepayDetail(CFirstSetField *p_pFirstSetField, CRspFislQryRepayDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 从订单VIP系统资金划转响应
  virtual int OnRspRpcFundTransferVip(CFirstSetField *p_pFirstSetField, CRspFislRpcFundTransferVipField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 券源头寸合约查询响应
  virtual int OnRspQryRpcPositionContractSrc(CFirstSetField *p_pFirstSetField, CRspFislQryRpcPositionContractSrcField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 认证信息设置(密码录入/修改)响应
  virtual int OnRspAuthDataSetJzjy(CFirstSetField *p_pFirstSetField, CRspFislAuthDataSetJzjyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金划拨(单账户多节点) 订单节点之间划拨响应
  virtual int OnRspFundTransferNode(CFirstSetField *p_pFirstSetField, CRspFislFundTransferNodeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资融券资金和信用额度查询响应
  virtual int OnRspQryFundAvl(CFirstSetField *p_pFirstSetField, CRspFislQryFundAvlField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 用户密码管理响应
  virtual int OnRspAutuDataMng(CFirstSetField *p_pFirstSetField, CRspFislAutuDataMngField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
private:
};

class MATRADEAPI CCliFislTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliFislTradeApi(void);

  // 析构函数
  virtual ~CCliFislTradeApi(void);

  // 标的券信息查询请求
  virtual int ReqQryUndlStkInfo(CReqFislQryUndlStkInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 担保证券信息查询请求
  virtual int ReqQryColStkInfo(CReqFislQryColStkInfoField *p_pReqField, LONGLONG p_llRequestId);

  // LOF信息查询请求
  virtual int ReqQryLofInfo(CReqFislQryLofInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 买卖委托请求
  virtual int ReqOrder(CReqFislOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 委托撤单请求
  virtual int ReqCancelOrder(CReqFislCancelOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 最大可交易数计算请求
  virtual int ReqMaxTradeQty(CReqFislMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券直接还款请求
  virtual int ReqMarginRepay(CReqFislMarginRepayField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询请求
  virtual int ReqQryExpendableFund(CReqFislQryExpendableFundField *p_pReqField, LONGLONG p_llRequestId);

  // 可用股份查询请求
  virtual int ReqQryExpendableShares(CReqFislQryExpendableSharesField *p_pReqField, LONGLONG p_llRequestId);

  // 当日委托查询请求
  virtual int ReqQryCurrDayOrder(CReqFislQryCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 当日成交查询请求
  virtual int ReqQryCurrDayFill(CReqFislQryCurrDayFillField *p_pReqField, LONGLONG p_llRequestId);

  // 股东账户查询请求
  virtual int ReqQryStkholderAcct(CReqFislQryStkholderAcctField *p_pReqField, LONGLONG p_llRequestId);

  // 可撤单委托查询请求
  virtual int ReqQryCanWithdrawOrder(CReqFislQryCanWithdrawOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询请求[快速]
  virtual int ReqQryExpendableFundEx(CReqFislQryExpendableFundExField *p_pReqField, LONGLONG p_llRequestId);

  // 可用股份查询请求[快速]
  virtual int ReqQryExpendableSharesEx(CReqFislQryExpendableSharesExField *p_pReqField, LONGLONG p_llRequestId);

  // 当日委托查询请求[快速]
  virtual int ReqQryCurrDayOrderEx(CReqFislQryCurrDayOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // 当日成交查询请求[快速]
  virtual int ReqQryCurrDayFillEx(CReqFislQryCurrDayFillExField *p_pReqField, LONGLONG p_llRequestId);

  // 股东账户查询请求[快速]
  virtual int ReqQryStkholderAcctEx(CReqFislQryStkholderAcctExField *p_pReqField, LONGLONG p_llRequestId);

  // 可撤单委托查询请求[快速]
  virtual int ReqQryCanWithdrawOrderEx(CReqFislQryCanWithdrawOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券协议查询请求
  virtual int ReqQryMarginProtocol(CReqFislQryMarginProtocolField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券合约查询请求
  virtual int ReqQryMarginContract(CReqFislQryMarginContractField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券保证金可用余额查询请求
  virtual int ReqQryMarginDepositAvl(CReqFislQryMarginDepositAvlField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券信用可用额度查询请求
  virtual int ReqQryMarginCreditAvl(CReqFislQryMarginCreditAvlField *p_pReqField, LONGLONG p_llRequestId);

  // 信用客户资产负债查询请求
  virtual int ReqQryCreditCustDebts(CReqFislQryCreditCustDebtsField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券资金头寸查询请求
  virtual int ReqQryMarginFundCash(CReqFislQryMarginFundCashField *p_pReqField, LONGLONG p_llRequestId);

  // 融资融券股份头寸查询请求
  virtual int ReqQryMarginSharesCash(CReqFislQryMarginSharesCashField *p_pReqField, LONGLONG p_llRequestId);
  
  // 可偿还金额查询
  virtual int ReqQryCanRepay(CReqFislQryCanRepayField *p_pReqField, LONGLONG p_llRequestId);

  // 证券市值额度查询
  virtual int ReqQryMktQuota(CReqFislMktQuotaField *p_pReqField, LONGLONG p_llRequestId);

  // 融资合约汇总查询
  virtual int ReqQryFiContractSum(CReqFislQryFiContractSumField *p_pReqField, LONGLONG  p_llRequestId);

  // 融券合约汇总查询
  virtual int ReqQrySlContractSum(CReqFislQrySlContractSumField *p_pReqField, LONGLONG  p_llRequestId);

  //资金划拨
  virtual int ReqQryFundTransfer(CReqFislFundTransferField *p_pReqField, LONGLONG p_llRequestId);

  // 用户登录请求
  virtual int ReqUserLogin(CReqFislUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  
  // 融资融券负债偿还流水查询[信用]请求
  virtual int ReqQryRpcRepaymentDetail(CReqFislQryRpcRepaymentDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询[信用]请求
  virtual int ReqQryRpcFund(CReqFislQryRpcFundField *p_pReqField, LONGLONG p_llRequestId);
  // 当日资金流水查询[信用]请求
  virtual int ReqQryRpcFundDetail(CReqFislQryRpcFundDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 新股申购客户市值额度查询[信用]请求
  virtual int ReqQryRpcIPOSubscriptionLimit(CReqFislQryRpcIPOSubscriptionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转帐[信用]请求
  virtual int ReqRpcBankStkTrans(CReqFislRpcBankStkTransField *p_pReqField, LONGLONG p_llRequestId);
  // 查询账户余额[信用]请求
  virtual int ReqQryRpcFundBala(CReqFislQryRpcFundBalaField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转帐查询[信用]请求
  virtual int ReqQryRpcBankStkTransInfo(CReqFislQryRpcBankStkTransInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 修改资金密码[信用]请求
  virtual int ReqRpcChgFundPwd(CReqFislRpcChgFundPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 修改交易密码[信用]请求
  virtual int ReqRpcChgTrdPwd(CReqFislRpcChgTrdPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 获取开通转帐(银行)账号(410601) [信用]请求
  virtual int ReqQryRpcBankAcct(CReqFislQryRpcBankAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 获取开通外部(银行)账号(410602) [信用]请求
  virtual int ReqQryRpcOtherBankAcct(CReqFislQryRpcOtherBankAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 当日新股清单查询(411549) [信用]请求
  virtual int ReqQryRpcIPO(CReqFislQryRpcIPOField *p_pReqField, LONGLONG p_llRequestId);
  // 配号查询(411518) [信用]请求
  virtual int ReqQryRpcDistribution(CReqFislQryRpcDistributionField *p_pReqField, LONGLONG p_llRequestId);
  // 市值配售中签查询(411547) [信用]请求
  virtual int ReqQryRpcMktCapPlacing(CReqFislQryRpcMktCapPlacingField *p_pReqField, LONGLONG p_llRequestId);
  // 中签历史查询(411560) [信用]请求
  virtual int ReqQryRpcWinningHistory(CReqFislQryRpcWinningHistoryField *p_pReqField, LONGLONG p_llRequestId);
  // 市值配售交收顺序修改（411548）[信用]请求
  virtual int ReqRpcChgMktCapDistDeliOrder(CReqFislRpcChgMktCapDistDeliOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 交割单查询(411520) [信用]请求
  virtual int ReqQryRpcDeliOrder(CReqFislQryRpcDeliOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 对帐查询(411521) [信用]请求
  virtual int ReqQryRpcChkAcct(CReqFislQryRpcChkAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托汇总(410514) [信用]请求
  virtual int ReqQryRpcOrderSummary(CReqFislQryRpcOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交汇总(410516) [信用]请求
  virtual int ReqQryRpcMatchSummary(CReqFislQryRpcMatchSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托明细查询(411511) [信用]请求
  virtual int ReqQryRpcHistoryOrderDetail(CReqFislQryRpcHistoryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交明细查询(411513) [信用]请求
  virtual int ReqQryRpcHistoryMatchDetail(CReqFislQryRpcHistoryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 转帐银行业务信息(410211) [信用]请求
  virtual int ReqQryRpcBankTransBasicInfo(CReqFislQryRpcBankTransBasicInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 对帐单查询(411525) [信用]请求
  virtual int ReqQryRpcStatement(CReqFislQryRpcStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 证券信息请求
  virtual int ReqRpcStkInfo(CReqFislRpcStkInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 机构编码信息(业务机构)请求
  virtual int ReqRpcOrgInfo(CReqFislRpcOrgInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 取字典信息请求
  virtual int ReqRpcDictInfo(CReqFislRpcDictInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 查询当前系统状态请求
  virtual int ReqQryRpcCurSysStat(CReqFislQryRpcCurSysStatField *p_pReqField, LONGLONG p_llRequestId);
  // 用户登录请求
  virtual int ReqRpcLogin(CReqFislRpcLoginField *p_pReqField, LONGLONG p_llRequestId);
  // 股东挂失请求
  virtual int ReqRpcTrdacctReportLoss(CReqFislRpcTrdacctReportLossField *p_pReqField, LONGLONG p_llRequestId);
  // 资金帐户挂失请求
  virtual int ReqRpcCuacctReportLoss(CReqFislRpcCuacctReportLossField *p_pReqField, LONGLONG p_llRequestId);
  // 设置主股东帐户请求
  virtual int ReqRpcSetMainTrdacct(CReqFislRpcSetMainTrdacctField *p_pReqField, LONGLONG p_llRequestId);
  // 设置主资产账号请求
  virtual int ReqRpcSetMainCuacct(CReqFislRpcSetMainCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 修改联系方式(网上交易使用)请求
  virtual int ReqRpcChgContact(CReqFislRpcChgContactField *p_pReqField, LONGLONG p_llRequestId);
  // 客户资料查询请求
  virtual int ReqQryRpcCustInfo(CReqFislQryRpcCustInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 修改代理权限请求
  virtual int ReqRpcChgProxyPermisson(CReqFislRpcChgProxyPermissonField *p_pReqField, LONGLONG p_llRequestId);
  // 修改操作方式请求
  virtual int ReqRpcChgOperMode(CReqFislRpcChgOperModeField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户操作方式请求
  virtual int ReqQryRpcCustOperMode(CReqFislQryRpcCustOperModeField *p_pReqField, LONGLONG p_llRequestId);
  // 修改客户联系人信息请求
  virtual int ReqRpcChgCustInfo(CReqFislRpcChgCustInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户资金帐号权限请求
  virtual int ReqQryRpcCuacctPermission(CReqFislQryRpcCuacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 修改资金帐号权限请求
  virtual int ReqRpcChgCuacctPermission(CReqFislRpcChgCuacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户证券帐号权限请求
  virtual int ReqQryRpcTrdacctPermission(CReqFislQryRpcTrdacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户证券帐号权限请求
  virtual int ReqRpcSetTrdacctPermission(CReqFislRpcSetTrdacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券额度上限查询请求
  virtual int ReqQryRpcUpLimitQuota(CReqFislQryRpcUpLimitQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券额度变更请求
  virtual int ReqRpcChgLimitQuota(CReqFislRpcChgLimitQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 额度变更申请请求
  virtual int ReqRpcChgLimitQuotaApplication(CReqFislRpcChgLimitQuotaApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 额度变更申请取消请求
  virtual int ReqRpcChgLimitQuotaApplicationCancel(CReqFislRpcChgLimitQuotaApplicationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 额度变更申请查询请求
  virtual int ReqQryRpcChgLimitQuotaApplication(CReqFislQryRpcChgLimitQuotaApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 取证券余额及当前最新价等,支持三方交易请求
  virtual int ReqRpcStkAvlAndLatestPrice(CReqFislRpcStkAvlAndLatestPriceField *p_pReqField, LONGLONG p_llRequestId);
  // 取最大交易数量,支持三方交易请求
  virtual int ReqRpcMaxStkQty(CReqFislRpcMaxStkQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 委托买卖业务,支持三方交易，外围约定融资融券委托请求
  virtual int ReqRpcOrder(CReqFislRpcOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单请求
  virtual int ReqRpcOrderCancel(CReqFislRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单查询请求
  virtual int ReqQryRpcOrderCancel(CReqFislQryRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 重置成本请求
  virtual int ReqRpcResetCost(CReqFislRpcResetCostField *p_pReqField, LONGLONG p_llRequestId);
  // 盈亏费用计算请求
  virtual int ReqRpcCalcFee(CReqFislRpcCalcFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 客户查询，查询客户股东代码请求
  virtual int ReqQryRpcTrdacct(CReqFislQryRpcTrdacctField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询请求
  virtual int ReqQryRpcCuacct(CReqFislQryRpcCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 股份明细查询请求
  virtual int ReqQryRpcStkDetail(CReqFislQryRpcStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 股份汇总查询请求
  virtual int ReqQryRpcStkSummary(CReqFislQryRpcStkSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 资金帐户查询请求
  virtual int ReqQryRpcCuacct2(CReqFislQryRpcCuacct2Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户查询，根据股东内码，查询股东代码请求
  virtual int ReqQryRpcTrdacct2(CReqFislQryRpcTrdacct2Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户查询，根据股东代码，资金帐户查询客户代码请求
  virtual int ReqQryRpcTrdacct3(CReqFislQryRpcTrdacct3Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托明细查询请求
  virtual int ReqQryRpcOrderDetail(CReqFislQryRpcOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交明细查询请求
  virtual int ReqQryRpcMatchDetail(CReqFislQryRpcMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托汇总－按证券代码和买卖方向请求
  virtual int ReqRpcHisOrderSummary(CReqFislRpcHisOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交汇总－按证券代码和买卖方向请求
  virtual int ReqRpcHisMatchSummary(CReqFislRpcHisMatchSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 中签查询请求
  virtual int ReqQryRpcLottery(CReqFislQryRpcLotteryField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托汇总ordergroup+stkcode+bsflag请求
  virtual int ReqRpcOrderSummary(CReqFislRpcOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 批量委托查询【增加】请求
  virtual int ReqQryRpcBatchOrder(CReqFislQryRpcBatchOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 信用资金可取查询请求
  virtual int ReqQryRpcCreditCuacctCanDraw(CReqFislQryRpcCreditCuacctCanDrawField *p_pReqField, LONGLONG p_llRequestId);
  // 股票查询行情请求
  virtual int ReqQryRpcMkt(CReqFislQryRpcMktField *p_pReqField, LONGLONG p_llRequestId);
  // 意向行情查询请求
  virtual int ReqQryRpcMktIntention(CReqFislQryRpcMktIntentionField *p_pReqField, LONGLONG p_llRequestId);
  // 查询营业部某一业务类别的申报席位请求
  virtual int ReqQryRpcSeat(CReqFislQryRpcSeatField *p_pReqField, LONGLONG p_llRequestId);
  // 查询资金账户信息请求
  virtual int ReqQryRpcCuacctAcctInfo(CReqFislQryRpcCuacctAcctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户特殊手续费查询请求
  virtual int ReqQryRpcCustSpecialFee(CReqFislQryRpcCustSpecialFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 股息红利税扣收明细查询请求
  virtual int ReqQryRpcDividendBonusTaxDetail(CReqFislQryRpcDividendBonusTaxDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 设定定期转帐转出计划请求
  virtual int ReqRpcSetPeriodicalTransferPlan(CReqFislRpcSetPeriodicalTransferPlanField *p_pReqField, LONGLONG p_llRequestId);
  // 查询定期转帐转出计划请求
  virtual int ReqQryRpcPeriodicalTransferPlan(CReqFislQryRpcPeriodicalTransferPlanField *p_pReqField, LONGLONG p_llRequestId);
  // 删除定期转帐转出计划请求
  virtual int ReqRpcDelPeriodicalTransferPlan(CReqFislRpcDelPeriodicalTransferPlanField *p_pReqField, LONGLONG p_llRequestId);
  // 担保证券查询请求
  virtual int ReqQryRpcGuaranteedStk(CReqFislQryRpcGuaranteedStkField *p_pReqField, LONGLONG p_llRequestId);
  // 标的证券查询请求
  virtual int ReqQryRpcUnderlyingStk(CReqFislQryRpcUnderlyingStkField *p_pReqField, LONGLONG p_llRequestId);
  // 融资信用数据查询请求
  virtual int ReqQryRpcFinancingCreditData(CReqFislQryRpcFinancingCreditDataField *p_pReqField, LONGLONG p_llRequestId);
  // 融券信用数据查询请求
  virtual int ReqQryRpcMarginCreditData(CReqFislQryRpcMarginCreditDataField *p_pReqField, LONGLONG p_llRequestId);
  // 信用客户资产负债查询请求
  virtual int ReqQryRpcCreditCustBalance(CReqFislQryRpcCreditCustBalanceField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券合约查询请求
  virtual int ReqQryRpcContract(CReqFislQryRpcContractField *p_pReqField, LONGLONG p_llRequestId);
  // 合同信息查询请求
  virtual int ReqQryRpcAgreementInfo(CReqFislQryRpcAgreementInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 外围约定直接还款、直接还款请求
  virtual int ReqRpcDirectRepayment(CReqFislRpcDirectRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // 查询券商融券帐户头寸请求
  virtual int ReqQryRpcMarginAcctPosi(CReqFislQryRpcMarginAcctPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 信用客户可转入担保证券查询请求
  virtual int ReqQryRpcCreditCustCanTransferInGuaranteedStk(CReqFislQryRpcCreditCustCanTransferInGuaranteedStkField *p_pReqField, LONGLONG p_llRequestId);
  // 投票方案查询请求
  virtual int ReqQryRpcVotingScheme(CReqFislQryRpcVotingSchemeField *p_pReqField, LONGLONG p_llRequestId);
  // 投票下单请求
  virtual int ReqRpcVotingOrder(CReqFislRpcVotingOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 投票结果查询请求
  virtual int ReqQryRpcVotingResult(CReqFislQryRpcVotingResultField *p_pReqField, LONGLONG p_llRequestId);
  // 投票结果统计请求
  virtual int ReqRpcVotingResultStatistics(CReqFislRpcVotingResultStatisticsField *p_pReqField, LONGLONG p_llRequestId);
  // 客户可投票议案查询请求
  virtual int ReqQryRpcCanVoteBill(CReqFislQryRpcCanVoteBillField *p_pReqField, LONGLONG p_llRequestId);
  // 客户可投票指定议案的股份查询请求
  virtual int ReqQryRpcCanVoteBillShare(CReqFislQryRpcCanVoteBillShareField *p_pReqField, LONGLONG p_llRequestId);
  // 股票历史股份余额查询请求
  virtual int ReqQryRpcHisShareBalance(CReqFislQryRpcHisShareBalanceField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券帐号自助激活请求
  virtual int ReqRpcSelfActivate(CReqFislRpcSelfActivateField *p_pReqField, LONGLONG p_llRequestId);
  // 合同信息修改请求
  virtual int ReqRpcChgAgreementInfo(CReqFislRpcChgAgreementInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户账户级费用查询请求
  virtual int ReqQryRpcAcctLvlFee(CReqFislQryRpcAcctLvlFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券自定义合约偿还顺序查询请求
  virtual int ReqQryRpcCustomContractRepayOrder(CReqFislQryRpcCustomContractRepayOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券自定义合约偿还顺序设置请求
  virtual int ReqRpcSetCustomContractRepayOrder(CReqFislRpcSetCustomContractRepayOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 融券特殊专户证券查询请求
  virtual int ReqQryRpcSpecialMarginAcctStk(CReqFislQryRpcSpecialMarginAcctStkField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券历史已了结合约查询请求
  virtual int ReqQryRpcHisClosedContract(CReqFislQryRpcHisClosedContractField *p_pReqField, LONGLONG p_llRequestId);
  // 外围信用交易属性查询请求
  virtual int ReqQryRpcCreditTradeAttribute(CReqFislQryRpcCreditTradeAttributeField *p_pReqField, LONGLONG p_llRequestId);
  // 合约展期申请请求
  virtual int ReqRpcContractExtensionApplication(CReqFislRpcContractExtensionApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 客户等比额度释放标志修改请求
  virtual int ReqRpcChgGeometricQuotaReleaseFlag(CReqFislRpcChgGeometricQuotaReleaseFlagField *p_pReqField, LONGLONG p_llRequestId);
  // 合约展期申请查询请求
  virtual int ReqQryRpcContractExtensionApplication(CReqFislQryRpcContractExtensionApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 合约展期申请取消请求
  virtual int ReqRpcContractExtensionApplicationCancel(CReqFislRpcContractExtensionApplicationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 外围风险测评类型查询请求
  virtual int ReqQryRpcRiskEvaluationMode(CReqFislQryRpcRiskEvaluationModeField *p_pReqField, LONGLONG p_llRequestId);
  // 展期规则查询请求
  virtual int ReqQryRpcExtensionRule(CReqFislQryRpcExtensionRuleField *p_pReqField, LONGLONG p_llRequestId);
  // 可展期合约查询请求
  virtual int ReqQryRpcCanExtendContract(CReqFislQryRpcCanExtendContractField *p_pReqField, LONGLONG p_llRequestId);
  // 客户担保权益调整明细查询请求
  virtual int ReqQryRpcGuaranteeInterestAdjustmentDetail(CReqFislQryRpcGuaranteeInterestAdjustmentDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 客户历史维持担保比例查询请求
  virtual int ReqQryRpcHisMaintainGuaranteeRatio(CReqFislQryRpcHisMaintainGuaranteeRatioField *p_pReqField, LONGLONG p_llRequestId);
  // 融资头寸预约申请请求
  virtual int ReqRpcFinancingPosiSubApplication(CReqFislRpcFinancingPosiSubApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 融资头寸预约申请查询请求
  virtual int ReqQryRpcFinancingPosiSubApplication(CReqFislQryRpcFinancingPosiSubApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 融资头寸变更预约申请请求
  virtual int ReqRpcChgFinancingPosiSubApplication(CReqFislRpcChgFinancingPosiSubApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 取消融资头寸预约申请请求
  virtual int ReqRpcCancelFinancingPosiSubApplication(CReqFislRpcCancelFinancingPosiSubApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 客户关联融资头寸查询请求
  virtual int ReqQryRpcLinkFinancingPosi(CReqFislQryRpcLinkFinancingPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 设置默认融资开仓头寸请求
  virtual int ReqRpcSetDefaultFinancingOpenPosi(CReqFislRpcSetDefaultFinancingOpenPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 融券头寸预约申请请求
  virtual int ReqRpcMarginPosiSubApplication(CReqFislRpcMarginPosiSubApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 融券头寸变更申请请求
  virtual int ReqRpcChgMarginPosiApplication(CReqFislRpcChgMarginPosiApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 融券头寸预约查询请求
  virtual int ReqQryRpcMarginPosiSub(CReqFislQryRpcMarginPosiSubField *p_pReqField, LONGLONG p_llRequestId);
  // 融券开仓头寸指定请求
  virtual int ReqRpcMarginOpenPosiAppoint(CReqFislRpcMarginOpenPosiAppointField *p_pReqField, LONGLONG p_llRequestId);
  // 客户关联融券头寸查询请求
  virtual int ReqQryRpcLinkMarginPosi(CReqFislQryRpcLinkMarginPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 融券头寸预约取消请求
  virtual int ReqRpcCancelMarginPosiSub(CReqFislRpcCancelMarginPosiSubField *p_pReqField, LONGLONG p_llRequestId);
  // 解冻中签日预冻结的资金请求
  virtual int ReqRpcUnfreezePreFrozenFunds(CReqFislRpcUnfreezePreFrozenFundsField *p_pReqField, LONGLONG p_llRequestId);
  // 放弃配售缴款, 支持三方交易请求
  virtual int ReqRpcCancelRation(CReqFislRpcCancelRationField *p_pReqField, LONGLONG p_llRequestId);
  // 系统探测请求
  virtual int ReqBizStatus(CReqFislBizStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 质押股份明细查询(单客户)请求
  virtual int ReqQryPledgeShares(CReqFislQryPledgeSharesField *p_pReqField, LONGLONG p_llRequestId);
  // 资金流水查询请求
  virtual int ReqQryFundFlow(CReqFislQryFundFlowField *p_pReqField, LONGLONG p_llRequestId);
  // 买卖委托请求
  virtual int ReqOrderEx(CReqFislOrderExField *p_pReqField, LONGLONG p_llRequestId);
  // 股份流水查询请求
  virtual int ReqQryStockFlow(CReqFislQryStockFlowField *p_pReqField, LONGLONG p_llRequestId);
  // 日切交易日查询请求
  virtual int ReqQrySettDate(CReqFislQrySettDateField *p_pReqField, LONGLONG p_llRequestId);
  // 资金划拨(融资融券)请求
  virtual int ReqFundTransferJzjy(CReqFislFundTransferJzjyField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券合约展期申请请求
  virtual int ReqContApply(CReqFislContApplyField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转账请求
  virtual int ReqBankStkTransfer(CReqFislBankStkTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 融券头寸明细查询请求
  virtual int ReqQryCashDetail(CReqFislQryCashDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 查询偿还明细请求
  virtual int ReqQryRepayDetail(CReqFislQryRepayDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 从订单VIP系统资金划转请求
  virtual int ReqRpcFundTransferVip(CReqFislRpcFundTransferVipField *p_pReqField, LONGLONG p_llRequestId);
  // 券源头寸合约查询请求
  virtual int ReqQryRpcPositionContractSrc(CReqFislQryRpcPositionContractSrcField *p_pReqField, LONGLONG p_llRequestId);
  // 认证信息设置(密码录入/修改)请求
  virtual int ReqAuthDataSetJzjy(CReqFislAuthDataSetJzjyField *p_pReqField, LONGLONG p_llRequestId);
  // 资金划拨(单账户多节点) 订单节点之间划拨请求
  virtual int ReqFundTransferNode(CReqFislFundTransferNodeField *p_pReqField, LONGLONG p_llRequestId);
  // 融资融券资金和信用额度查询请求
  virtual int ReqQryFundAvl(CReqFislQryFundAvlField *p_pReqField, LONGLONG p_llRequestId);
  // 用户密码管理请求
  virtual int ReqAutuDataMng(CReqFislAutuDataMngField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);
  
private:
  // 证券信息查询响应[快速]
  void OnRspQryStkInfoEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 标的券信息查询响应
  void OnRspQryUndlStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 担保证券信息查询响应
  void OnRspQryColStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // LOF信息查询
  void OnRspQryLofInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 买卖委托响应
  void OnRspOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 组合委托响应
  void OnRspCombOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 委托撤单响应
  void OnRspCancelOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 最大可交易数计算响应
  void OnRspMaxTradeQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 批量委托响应
  void OnRspQuantityOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 批量撤单响应
  void OnRspQuantityCancelOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ETF 篮子委托响应
  void OnRspEtfOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 篮子委托最大可交易数计算响应
  void OnRspEtfMaxTradeQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券直接还款响应
  void OnRspMarginRepay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用资金查询响应
  void OnRspQryExpendableFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用股份查询响应
  void OnRspQryExpendableShares(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日委托查询响应
  void OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日成交查询响应
  void OnRspQryCurrDayFill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 股东账户查询响应
  void OnRspQryStkholderAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可撤单委托查询响应
  void OnRspQryCanWithdrawOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用资金查询响应[快速]
  void OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用股份查询响应[快速]
  void OnRspQryExpendableSharesEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日委托查询响应[快速]
  void OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日成交查询响应[快速]
  void OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 股东账户查询响应[快速]
  void OnRspQryStkholderAcctEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可撤单委托查询响应[快速]
  void OnRspQryCanWithdrawOrderEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ETF 成分股持仓查询响应
  void OnRspQryEftCotPosition(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券协议查询响应
  void OnRspQryMarginProtocol(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券合约查询响应
  void OnRspQryMarginContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券保证金可用余额查询响应
  void OnRspQryMarginDepositAvl(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券信用可用额度查询响应
  void OnRspQryMarginCreditAvl(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 信用客户资产负债查询响应
  void OnRspQryCreditCustDebts(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券资金头寸查询响应
  void OnRspQryMarginFundCash(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 融资融券股份头寸查询响应
  void OnRspQryMarginSharesCash(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 可偿还金额查询响应
  void OnRspQryCanRepay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 证券市值额度查询响应
  void OnRspQryMktQuota(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 融资合约汇总查询响应
  void OnRspQryFiContractSum(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 融券合约汇总查询响应
  void OnRspQrySlContractSum(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 资金划拨响应
  void OnRspQryFundTransfer(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 用户登录请求响应
  void OnRspUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);
  // 融资融券负债偿还流水查询[信用]响应
  void OnRspQryRpcRepaymentDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询[信用]响应
  void OnRspQryRpcFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日资金流水查询[信用]响应
  void OnRspQryRpcFundDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股申购客户市值额度查询[信用]响应
  void OnRspQryRpcIPOSubscriptionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转帐[信用]响应
  void OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询账户余额[信用]响应
  void OnRspQryRpcFundBala(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转帐查询[信用]响应
  void OnRspQryRpcBankStkTransInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改资金密码[信用]响应
  void OnRspRpcChgFundPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改交易密码[信用]响应
  void OnRspRpcChgTrdPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取开通转帐(银行)账号(410601) [信用]响应
  void OnRspQryRpcBankAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取开通外部(银行)账号(410602) [信用]响应
  void OnRspQryRpcOtherBankAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日新股清单查询(411549) [信用]响应
  void OnRspQryRpcIPO(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 配号查询(411518) [信用]响应
  void OnRspQryRpcDistribution(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 市值配售中签查询(411547) [信用]响应
  void OnRspQryRpcMktCapPlacing(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 中签历史查询(411560) [信用]响应
  void OnRspQryRpcWinningHistory(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 市值配售交收顺序修改（411548）[信用]响应
  void OnRspRpcChgMktCapDistDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 交割单查询(411520) [信用]响应
  void OnRspQryRpcDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 对帐查询(411521) [信用]响应
  void OnRspQryRpcChkAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托汇总(410514) [信用]响应
  void OnRspQryRpcOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交汇总(410516) [信用]响应
  void OnRspQryRpcMatchSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托明细查询(411511) [信用]响应
  void OnRspQryRpcHistoryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交明细查询(411513) [信用]响应
  void OnRspQryRpcHistoryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 转帐银行业务信息(410211) [信用]响应
  void OnRspQryRpcBankTransBasicInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 对帐单查询(411525) [信用]响应
  void OnRspQryRpcStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券信息响应
  void OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 机构编码信息(业务机构)响应
  void OnRspRpcOrgInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取字典信息响应
  void OnRspRpcDictInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询当前系统状态响应
  void OnRspQryRpcCurSysStat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户登录响应
  void OnRspRpcLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东挂失响应
  void OnRspRpcTrdacctReportLoss(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金帐户挂失响应
  void OnRspRpcCuacctReportLoss(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置主股东帐户响应
  void OnRspRpcSetMainTrdacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置主资产账号响应
  void OnRspRpcSetMainCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改联系方式(网上交易使用)响应
  void OnRspRpcChgContact(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户资料查询响应
  void OnRspQryRpcCustInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改代理权限响应
  void OnRspRpcChgProxyPermisson(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改操作方式响应
  void OnRspRpcChgOperMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户操作方式响应
  void OnRspQryRpcCustOperMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改客户联系人信息响应
  void OnRspRpcChgCustInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户资金帐号权限响应
  void OnRspQryRpcCuacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改资金帐号权限响应
  void OnRspRpcChgCuacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户证券帐号权限响应
  void OnRspQryRpcTrdacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户证券帐号权限响应
  void OnRspRpcSetTrdacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券额度上限查询响应
  void OnRspQryRpcUpLimitQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券额度变更响应
  void OnRspRpcChgLimitQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 额度变更申请响应
  void OnRspRpcChgLimitQuotaApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 额度变更申请取消响应
  void OnRspRpcChgLimitQuotaApplicationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 额度变更申请查询响应
  void OnRspQryRpcChgLimitQuotaApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取证券余额及当前最新价等,支持三方交易响应
  void OnRspRpcStkAvlAndLatestPrice(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取最大交易数量,支持三方交易响应
  void OnRspRpcMaxStkQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托买卖业务,支持三方交易，外围约定融资融券委托响应
  void OnRspRpcOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单响应
  void OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单查询响应
  void OnRspQryRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 重置成本响应
  void OnRspRpcResetCost(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盈亏费用计算响应
  void OnRspRpcCalcFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户查询，查询客户股东代码响应
  void OnRspQryRpcTrdacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询响应
  void OnRspQryRpcCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份明细查询响应
  void OnRspQryRpcStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份汇总查询响应
  void OnRspQryRpcStkSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金帐户查询响应
  void OnRspQryRpcCuacct2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户查询，根据股东内码，查询股东代码响应
  void OnRspQryRpcTrdacct2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户查询，根据股东代码，资金帐户查询客户代码响应
  void OnRspQryRpcTrdacct3(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托明细查询响应
  void OnRspQryRpcOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交明细查询响应
  void OnRspQryRpcMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托汇总－按证券代码和买卖方向响应
  void OnRspRpcHisOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交汇总－按证券代码和买卖方向响应
  void OnRspRpcHisMatchSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 中签查询响应
  void OnRspQryRpcLottery(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托汇总ordergroup+stkcode+bsflag响应
  void OnRspRpcOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 批量委托查询【增加】响应
  void OnRspQryRpcBatchOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 信用资金可取查询响应
  void OnRspQryRpcCreditCuacctCanDraw(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票查询行情响应
  void OnRspQryRpcMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 意向行情查询响应
  void OnRspQryRpcMktIntention(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询营业部某一业务类别的申报席位响应
  void OnRspQryRpcSeat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询资金账户信息响应
  void OnRspQryRpcCuacctAcctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户特殊手续费查询响应
  void OnRspQryRpcCustSpecialFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股息红利税扣收明细查询响应
  void OnRspQryRpcDividendBonusTaxDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设定定期转帐转出计划响应
  void OnRspRpcSetPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询定期转帐转出计划响应
  void OnRspQryRpcPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 删除定期转帐转出计划响应
  void OnRspRpcDelPeriodicalTransferPlan(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 担保证券查询响应
  void OnRspQryRpcGuaranteedStk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 标的证券查询响应
  void OnRspQryRpcUnderlyingStk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资信用数据查询响应
  void OnRspQryRpcFinancingCreditData(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券信用数据查询响应
  void OnRspQryRpcMarginCreditData(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 信用客户资产负债查询响应
  void OnRspQryRpcCreditCustBalance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券合约查询响应
  void OnRspQryRpcContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合同信息查询响应
  void OnRspQryRpcAgreementInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围约定直接还款、直接还款响应
  void OnRspRpcDirectRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询券商融券帐户头寸响应
  void OnRspQryRpcMarginAcctPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 信用客户可转入担保证券查询响应
  void OnRspQryRpcCreditCustCanTransferInGuaranteedStk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 投票方案查询响应
  void OnRspQryRpcVotingScheme(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 投票下单响应
  void OnRspRpcVotingOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 投票结果查询响应
  void OnRspQryRpcVotingResult(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 投票结果统计响应
  void OnRspRpcVotingResultStatistics(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户可投票议案查询响应
  void OnRspQryRpcCanVoteBill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户可投票指定议案的股份查询响应
  void OnRspQryRpcCanVoteBillShare(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票历史股份余额查询响应
  void OnRspQryRpcHisShareBalance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券帐号自助激活响应
  void OnRspRpcSelfActivate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合同信息修改响应
  void OnRspRpcChgAgreementInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户账户级费用查询响应
  void OnRspQryRpcAcctLvlFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券自定义合约偿还顺序查询响应
  void OnRspQryRpcCustomContractRepayOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券自定义合约偿还顺序设置响应
  void OnRspRpcSetCustomContractRepayOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券特殊专户证券查询响应
  void OnRspQryRpcSpecialMarginAcctStk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券历史已了结合约查询响应
  void OnRspQryRpcHisClosedContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围信用交易属性查询响应
  void OnRspQryRpcCreditTradeAttribute(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约展期申请响应
  void OnRspRpcContractExtensionApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户等比额度释放标志修改响应
  void OnRspRpcChgGeometricQuotaReleaseFlag(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约展期申请查询响应
  void OnRspQryRpcContractExtensionApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约展期申请取消响应
  void OnRspRpcContractExtensionApplicationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围风险测评类型查询响应
  void OnRspQryRpcRiskEvaluationMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 展期规则查询响应
  void OnRspQryRpcExtensionRule(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可展期合约查询响应
  void OnRspQryRpcCanExtendContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户担保权益调整明细查询响应
  void OnRspQryRpcGuaranteeInterestAdjustmentDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户历史维持担保比例查询响应
  void OnRspQryRpcHisMaintainGuaranteeRatio(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资头寸预约申请响应
  void OnRspRpcFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资头寸预约申请查询响应
  void OnRspQryRpcFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资头寸变更预约申请响应
  void OnRspRpcChgFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取消融资头寸预约申请响应
  void OnRspRpcCancelFinancingPosiSubApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户关联融资头寸查询响应
  void OnRspQryRpcLinkFinancingPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置默认融资开仓头寸响应
  void OnRspRpcSetDefaultFinancingOpenPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券头寸预约申请响应
  void OnRspRpcMarginPosiSubApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券头寸变更申请响应
  void OnRspRpcChgMarginPosiApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券头寸预约查询响应
  void OnRspQryRpcMarginPosiSub(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券开仓头寸指定响应
  void OnRspRpcMarginOpenPosiAppoint(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户关联融券头寸查询响应
  void OnRspQryRpcLinkMarginPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券头寸预约取消响应
  void OnRspRpcCancelMarginPosiSub(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 解冻中签日预冻结的资金响应
  void OnRspRpcUnfreezePreFrozenFunds(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 放弃配售缴款, 支持三方交易响应
  void OnRspRpcCancelRation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 系统探测响应
  void OnRspBizStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 质押股份明细查询(单客户)响应
  void OnRspQryPledgeShares(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金流水查询响应
  void OnRspQryFundFlow(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 买卖委托(快速)响应
  void OnRspOrderEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份流水查询响应
  void OnRspQryStockFlow(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 日切交易日查询响应
  void OnRspQrySettDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金划拨(融资融券)响应
  void OnRspFundTransferJzjy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券合约展期申请响应
  void OnRspContApply(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转账响应
  void OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融券头寸明细查询响应
  void OnRspQryCashDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询偿还明细响应
  void OnRspQryRepayDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 从订单VIP系统资金划转响应
  void OnRspRpcFundTransferVip(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 券源头寸合约查询响应
  void OnRspQryRpcPositionContractSrc(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 认证信息设置(密码录入/修改)响应
  void OnRspAuthDataSetJzjy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金划拨(单账户多节点) 订单节点之间划拨响应
  void OnRspFundTransferNode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资融券资金和信用额度查询响应
  void OnRspQryFundAvl(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户密码管理响应
  void OnRspAutuDataMng(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FISL_TRADE_API_H__