#if !defined(__MA_CLI_STK_TRADE_API_H__)
#define __MA_CLI_STK_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliStkTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliStkTradeSpi : virtual public CCliTradeSpi
{
public:
  // 证券信息查询响应[快速]
  virtual int OnRspQryStkInfoEx(CFirstSetField *p_pFirstSetField, CRspStkQryInfoExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 证券信息查询响应
  virtual int OnRspQryStkInfo(CFirstSetField *p_pFirstSetField, CRspStkQryStkInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 盘后行情查询(上海科创板)响应
  virtual int OnRspQryClosingMkt(CFirstSetField *p_pFirstSetField, CRspStkQryClosingMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // LOF信息查询
  virtual int OnRspQryLofInfo(CFirstSetField *p_pFirstSetField, CRspStkQryLofInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ETF信息查询
  virtual int OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, CRspStkQryEtfInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ETF篮信息查询
  virtual int OnRspQryEtfBasket(CFirstSetField *p_pFirstSetField, CRspStkQryEtfBasketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 买卖委托响应
  virtual int OnRspOrder(CFirstSetField *p_pFirstSetField, CRspStkOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 组合委托响应
  virtual int OnRspCombOrder(CFirstSetField *p_pFirstSetField, CRspStkCombOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 委托撤单响应
  virtual int OnRspCancelOrder(CFirstSetField *p_pFirstSetField, CRspStkCancelOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 最大可交易数计算响应
  virtual int OnRspMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspStkMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 批量委托响应
  virtual int OnRspQuantityOrder(CFirstSetField *p_pFirstSetField, CRspStkQuantityOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 批量撤单响应
  virtual int OnRspQuantityCancelOrder(CFirstSetField *p_pFirstSetField, CRspStkQuantityCancelOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 买卖委托响应(极速)
  virtual int OnRspOrderFlash(CFirstSetField *p_pFirstSetField, CRspStkOrderFlashField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 委托撤单响应(极速)
  virtual int OnRspCancelOrderFlash(CFirstSetField *p_pFirstSetField, CRspStkCancelOrderFlashField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 组合委托响应(极速)
  virtual int OnRspCombOrderFlash(CFirstSetField *p_pFirstSetField, CRspStkCombOrderFlashField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ETF 篮子委托响应
  virtual int OnRspEtfOrder(CFirstSetField *p_pFirstSetField, CRspStkEtfOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 篮子委托最大可交易数计算响应
  virtual int OnRspEtfMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspStkEtfMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应
  virtual int OnRspQryExpendableFund(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用股份查询响应
  virtual int OnRspQryExpendableShares(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableSharesField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日委托查询响应
  virtual int OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日成交查询响应
  virtual int OnRspQryCurrDayFill(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股东账户查询响应
  virtual int OnRspQryStkholderAcct(CFirstSetField *p_pFirstSetField, CRspStkQryStkholderAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可撤单委托查询响应
  virtual int OnRspQryCanWithdrawOrder(CFirstSetField *p_pFirstSetField, CRspStkQryCanWithdrawOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应[快速]
  virtual int OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableFundExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用股份查询响应[快速]
  virtual int OnRspQryExpendableSharesEx(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableSharesExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日委托查询响应[快速]
  virtual int OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日成交查询响应[快速]
  virtual int OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayFillExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股东账户查询响应[快速]
  virtual int OnRspQryStkholderAcctEx(CFirstSetField *p_pFirstSetField, CRspStkQryStkholderAcctExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可撤单委托查询响应[快速]
  virtual int OnRspQryCanWithdrawOrderEx(CFirstSetField *p_pFirstSetField, CRspStkQryCanWithdrawOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // ETF 成分股持仓查询响应
  virtual int OnRspQryEtfCotPosition(CFirstSetField *p_pFirstSetField, CRspStkQryEtfCotPositionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通汇率信息查询响应
  virtual int OnRspQryRateInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRateInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通市场状态信息查询响应
  virtual int OnRspQryMktStatusInfo(CFirstSetField *p_pFirstSetField, CRspStkQryMktStatusInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通标的证券状态查询响应
  virtual int OnRspQryUndlStkStatus(CFirstSetField *p_pFirstSetField, CRspStkQryUndlStkStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通二级资金台账查询响应
  virtual int OnRspQryFundAcctLvlTwo(CFirstSetField *p_pFirstSetField, CRspStkQryFundAcctLvlTwoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通交易最小价差查询响应
  virtual int OnRspQryTradeMinSpread(CFirstSetField *p_pFirstSetField, CRspStkQryTradeMinSpreadField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 港股通客户负债查询响应
  //virtual int OnRspQryCustDebts(CFirstSetField *p_pFirstSetField, CRspStkQryCustDebtsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 回购合约查询响应
  virtual int OnRspQryRepchContract(CFirstSetField *p_pFirstSetField, CRspStkQryRepchContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户适当性查询响应
  virtual int OnRspQryCustAppropriate(CFirstSetField *p_pFirstSetField, CRspStkQryCustAppropriateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 产品适当性查询响应
  virtual int OnRspQryProdAppropriate(CFirstSetField *p_pFirstSetField, CRspStkQryProdAppropriateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 适当性匹配检查响应
  virtual int OnRspAppropriateCheck(CFirstSetField *p_pFirstSetField, CRspStkAppropriateCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 适当性匹配结果记录响应
  virtual int OnRspAppropriateResult(CFirstSetField *p_pFirstSetField, CRspStkAppropriateResultField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 回购资金占用天数查询响应
  virtual int OnRspQryRepchFund(CFirstSetField *p_pFirstSetField, CRspStkQryRepchFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 用户登录请求响应
  virtual int OnRspUserLogin(CFirstSetField *p_pFirstSetField, CRspStkUserLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 历史委托查询响应
  virtual int OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, CRspStkQryHisOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 历史成交查询响应
  virtual int OnRspQryHisFill(CFirstSetField *p_pFirstSetField, CRspStkQryHisFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日/历史委托查询响应
  virtual int OnRspQryCurrDayHisOrder(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayHisOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 当日/历史成交查询响应
  virtual int OnRspQryCurrDayHisFill(CFirstSetField *p_pFirstSetField, CRspStkQryCurrDayHisFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnStkOrderConfirmField *p_pRtnField) {return 0;}

  // 成交回报
  virtual int OnRtnOrderFill(CRtnStkOrderFillField *p_pRtnField) {return 0;}

  // 申赎成交推送
  virtual int OnRtnOrderFill(CRtnStkPefEtfFillField *p_pRtnField) {return 0;}

  // 确认回报(极速)
  virtual int OnRtnOrderConfirmFlash(CRtnStkOrderConfirmFlashField *p_pRtnField) {return 0;}

  // 成交回报(极速)
  virtual int OnRtnOrderFillFlash(CRtnStkOrderFillFlashField *p_pRtnField) {return 0;}

  //转帐银行业务信息    2015/8/12
  virtual int OnRspQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, CRspStkBankTransBasicInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //银证转帐       2015/8/12
  virtual int OnRspBankStkTrans(CFirstSetField *p_pFirstSetField, CRspStkBankStkTransField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //查询账户余额   2015/8/12
  virtual int OnRspQryFundBala(CFirstSetField *p_pFirstSetField, CRspStkFundBalaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //银证转帐查询   2015/8/12
  virtual int OnRspQryBankStkTrans(CFirstSetField *p_pFirstSetField, CRspStkBankStkTransInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //修改交易密码   2015/8/12
  virtual int OnRspChgTrdPwd(CFirstSetField *p_pFirstSetField, CRspStkChgTrdPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //修改资金密码   2015/8/12
  virtual int OnRspChgFundPwd(CFirstSetField *p_pFirstSetField, CRspStkChgFundPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //对帐单查询     2015/8/12
  virtual int OnRspQryStatement(CFirstSetField *p_pFirstSetField, CRspStkStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //资金划拨
  virtual int OnRspQryFundTransfer(CFirstSetField *p_pFirstSetField, CRspStkFundTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 证券市值额度查询响应
  virtual int OnRspQryMktQuota(CFirstSetField *p_pFirstSetField, CRspStkMktQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 上证LOF信息查询响应
  virtual int OnRspQryScLofInfo(CFirstSetField *p_pFirstSetField, CRspStkQryScLofInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 历史委托明细查询响应
  virtual int OnRspQryRpcHistoryOrderDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHistoryOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交明细查询响应
  virtual int OnRspQryRpcHistoryMatchDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHistoryMatchDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日新股清单查询响应
  virtual int OnRspQryRpcIPO(CFirstSetField *p_pFirstSetField, CRspStkQryRpcIPOField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围新股申购市值额度表查询响应
  virtual int OnRspQryRpcIPOSubscriptionLimit(CFirstSetField *p_pFirstSetField, CRspStkQryRpcIPOSubscriptionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 配号查询响应
  virtual int OnRspQryRpcDistribution(CFirstSetField *p_pFirstSetField, CRspStkQryRpcDistributionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 市值配售中签查询响应
  virtual int OnRspQryRpcMktCapPlacing(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMktCapPlacingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 中签历史查询响应
  virtual int OnRspQryRpcWinningHistory(CFirstSetField *p_pFirstSetField, CRspStkQryRpcWinningHistoryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 市值配售交收顺序修改响应
  virtual int OnRspRpcChgMktCapDistDeliOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcChgMktCapDistDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 交割单查询响应
  virtual int OnRspQryRpcDeliOrder(CFirstSetField *p_pFirstSetField, CRspStkQryRpcDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 对帐查询响应
  virtual int OnRspQryRpcChkAcct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcChkAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 获取开通转帐(银行)账号查询响应
  virtual int OnRspQryRpcBankAcct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBankAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 获取开通外部(银行)账号查询响应
  virtual int OnRspQryRpcOtherBankAcct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOtherBankAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询响应
  virtual int OnRspQryRpcFund(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日资金流水查询响应
  virtual int OnRspQryRpcFundDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFundDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托汇总(410514)响应
  virtual int OnRspQryRpcOrderSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrderSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交汇总(410516)响应
  virtual int OnRspQryRpcMatchSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMatchSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 查询当前系统状态响应
  virtual int OnRspQryRpcCurSysStat(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCurSysStatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取字典信息响应
  virtual int OnRspRpcDictInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcDictInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券信息响应
  virtual int OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcStkInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询机构信息响应
  virtual int OnRspQryRpcOrgInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrgInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证通转帐银行业务信息响应
  virtual int OnRspRpcBankStkTransferBankBizInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcBankStkTransferBankBizInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 用户登陆响应
  virtual int OnRspRpcLogin(CFirstSetField *p_pFirstSetField, CRspStkRpcLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户总资产查询(新)响应
  virtual int OnRspQryRpcTotalAssets(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTotalAssetsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改联系方式（网上交易使用）响应
  virtual int OnRspRpcChgContact(CFirstSetField *p_pFirstSetField, CRspStkRpcChgContactField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户资料查询响应
  virtual int OnRspQryRpcCustInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改操作方式响应
  virtual int OnRspRpcChgOperMode(CFirstSetField *p_pFirstSetField, CRspStkRpcChgOperModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户开通的操作方式响应
  virtual int OnRspQryRpcCustOperMode(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustOperModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改客户联系人信息响应
  virtual int OnRspRpcChgCustInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcChgCustInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户的扩展属性响应
  virtual int OnRspRpcSetCustAttrEx(CFirstSetField *p_pFirstSetField, CRspStkRpcSetCustAttrExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置数字签名响应
  virtual int OnRspRpcSetDigitalSign(CFirstSetField *p_pFirstSetField, CRspStkRpcSetDigitalSignField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围修改资金帐号权限响应
  virtual int OnRspRpcChgCuacctPermission(CFirstSetField *p_pFirstSetField, CRspStkRpcChgCuacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金帐号开通资金权限查询响应
  virtual int OnRspQryRpcCuacctPermission(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券帐号开通权限查询响应
  virtual int OnRspQryRpcTrdacctPermission(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTrdacctPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户权限有效期信息查询响应
  virtual int OnRspQryRpcCustPermissionValidPeriod(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustPermissionValidPeriodField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围激活客户权限响应
  virtual int OnRspRpcActivateCustPermission(CFirstSetField *p_pFirstSetField, CRspStkRpcActivateCustPermissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户特殊手续费查询响应
  virtual int OnRspQryRpcCustSpecialFee(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustSpecialFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 一次性服务佣金收取响应
  virtual int OnRspRpcOneTimeServiceCommission(CFirstSetField *p_pFirstSetField, CRspStkRpcOneTimeServiceCommissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 一次性服务佣金撤单响应
  virtual int OnRspRpcCancelOneTimeServiceCommission(CFirstSetField *p_pFirstSetField, CRspStkRpcCancelOneTimeServiceCommissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固定期限服务佣金办理响应
  virtual int OnRspRpcFixedTermServiceCommission(CFirstSetField *p_pFirstSetField, CRspStkRpcFixedTermServiceCommissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固定期限服务佣金退款响应
  virtual int OnRspRpcFixedTermServiceCommissionRefund(CFirstSetField *p_pFirstSetField, CRspStkRpcFixedTermServiceCommissionRefundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 一次性服务佣金设置查询响应
  virtual int OnRspQryRpcOneTimeServiceCommissionSetting(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOneTimeServiceCommissionSettingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 一次性服务佣金收取查询响应
  virtual int OnRspQryRpcOneTimeServiceCommissions(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOneTimeServiceCommissionsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询咨询付费类别响应
  virtual int OnRspQryRpcConsultationPaymentCategory(CFirstSetField *p_pFirstSetField, CRspStkQryRpcConsultationPaymentCategoryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 增加咨询付费类别响应
  virtual int OnRspRpcAddConsultationPaymentCategory(CFirstSetField *p_pFirstSetField, CRspStkRpcAddConsultationPaymentCategoryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 维护客户订阅状态响应
  virtual int OnRspRpcMaintainCustSubStatus(CFirstSetField *p_pFirstSetField, CRspStkRpcMaintainCustSubStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户订阅流水响应
  virtual int OnRspQryRpcCustSubStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustSubStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户订阅设置响应
  virtual int OnRspRpcCustSubSetting(CFirstSetField *p_pFirstSetField, CRspStkRpcCustSubSettingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 第三方收费结构响应
  virtual int OnRspRpcThirdPartyChargingStructure(CFirstSetField *p_pFirstSetField, CRspStkRpcThirdPartyChargingStructureField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 投资咨询产品费用保证金支付响应
  virtual int OnRspRpcInvestmentAdvisoryProductFee(CFirstSetField *p_pFirstSetField, CRspStkRpcInvestmentAdvisoryProductFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 密码校验响应
  virtual int OnRspRpcCuacctPwdCheck(CFirstSetField *p_pFirstSetField, CRspStkRpcCuacctPwdCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金资料查询响应
  virtual int OnRspQryRpcCuacctInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 获取客户资料核查状态响应
  virtual int OnRspRpcCustInfoCheck(CFirstSetField *p_pFirstSetField, CRspStkRpcCustInfoCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户登录校验信息响应
  virtual int OnRspRpcSetLoginCheckMode(CFirstSetField *p_pFirstSetField, CRspStkRpcSetLoginCheckModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户资料核查标识响应
  virtual int OnRspRpcSetCustInfoCheckMode(CFirstSetField *p_pFirstSetField, CRspStkRpcSetCustInfoCheckModeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户消息状态响应
  virtual int OnRspRpcSetCustMsgStatus(CFirstSetField *p_pFirstSetField, CRspStkRpcSetCustMsgStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户消息服务响应
  virtual int OnRspQryRpcCustMsgService(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustMsgServiceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 修改银行密码响应
  virtual int OnRspRpcChgBankPwd(CFirstSetField *p_pFirstSetField, CRspStkRpcChgBankPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金台帐间转帐响应
  virtual int OnRspRpcTransferBetweenCuacct(CFirstSetField *p_pFirstSetField, CRspStkRpcTransferBetweenCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围客户交易帐户资金归集响应
  virtual int OnRspRpcCuacctCashConcentration(CFirstSetField *p_pFirstSetField, CRspStkRpcCuacctCashConcentrationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取同一客户交易台帐、转帐台帐响应
  virtual int OnRspRpcCuacctInfo2(CFirstSetField *p_pFirstSetField, CRspStkRpcCuacctInfo2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金可取数查询响应
  virtual int OnRspQryRpcCuacctMaxDraw(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctMaxDrawField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户可取资金查询响应
  virtual int OnRspQryRpcCuacctMaxDrawForTencentPay(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctMaxDrawForTencentPayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取证券余额及当前最新价等,支持三方交易响应
  virtual int OnRspRpcStkAvlAndLatestPrice(CFirstSetField *p_pFirstSetField, CRspStkRpcStkAvlAndLatestPriceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取最大交易数量,支持三方交易响应
  virtual int OnRspRpcMaxStkQty(CFirstSetField *p_pFirstSetField, CRspStkRpcMaxStkQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 买卖委托业务,支持三方交易响应
  virtual int OnRspRpcOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单响应
  virtual int OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcOrderCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单查询响应
  virtual int OnRspQryRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrderCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 重置成本响应
  virtual int OnRspRpcResetCost(CFirstSetField *p_pFirstSetField, CRspStkRpcResetCostField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盈亏费用计算响应
  virtual int OnRspRpcCalcFee(CFirstSetField *p_pFirstSetField, CRspStkRpcCalcFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托买卖业务,支持三方交易（新增外围调用）响应
  virtual int OnRspRpcOrderForTencentPay(CFirstSetField *p_pFirstSetField, CRspStkRpcOrderForTencentPayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资管份额转让T+0委托响应
  virtual int OnRspRpcTransferCapitalManagementShare(CFirstSetField *p_pFirstSetField, CRspStkRpcTransferCapitalManagementShareField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 放弃配售缴款, 支持三方交易响应
  virtual int OnRspRpcCancelRation(CFirstSetField *p_pFirstSetField, CRspStkRpcCancelRationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 放弃配售缴款, 支持三方交易响应
  virtual int OnRspQryRpcCancelRation(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCancelRationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户摇号查询响应
  virtual int OnRspQryRpcCustLottery(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustLotteryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户从外围解冻中签预冻结资金响应
  virtual int OnRspRpcUnfreezePreFrozenFunds(CFirstSetField *p_pFirstSetField, CRspStkRpcUnfreezePreFrozenFundsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 产品信息查询响应
  virtual int OnRspQryRpcStkPledgeProductInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeProductInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可质押标的证券查询响应
  virtual int OnRspQryRpcStkPledgeNegotiableSecurity(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeNegotiableSecurityField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户质押标的折算率查询响应
  virtual int OnRspQryRpcStkPledgePledgeConversionRate(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgePledgeConversionRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户贷款额度信息查询响应
  virtual int OnRspQryRpcStkPledgeLoanQuota(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeLoanQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可质押证券持仓查询响应
  virtual int OnRspQryRpcStkPledgeNegotiableSecurityPosi(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeNegotiableSecurityPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 最大可贷款金额查询响应
  virtual int OnRspQryRpcStkPledgeMaxLoanAmt(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeMaxLoanAmtField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 贷款申请响应
  virtual int OnRspRpcStkPledgeLoanApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeLoanApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 还款申请响应
  virtual int OnRspRpcStkPledgeRepaymentApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeRepaymentApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 补充证券质押物申请响应
  virtual int OnRspRpcStkPledgeSupplementaryPledgeSecurityApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeSupplementaryPledgeSecurityApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约还款日延期申请响应
  virtual int OnRspRpcStkPledgeDeferredContractRepaymentApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeDeferredContractRepaymentApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约需补充质押数量查询响应
  virtual int OnRspQryRpcStkPledgeInsufficientPledge(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeInsufficientPledgeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 业务申请情况查询响应
  virtual int OnRspQryRpcStkPledgeBizApplicationStatus(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeBizApplicationStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 贷款合约查询响应
  virtual int OnRspQryRpcStkPledgeLoanContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeLoanContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 贷款合约质押明细查询响应
  virtual int OnRspQryRpcStkPledgeLoanContractPledgeDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeLoanContractPledgeDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押额度修改响应
  virtual int OnRspRpcStkPledgeChgStkPledgeQuota(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeChgStkPledgeQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 部分还款申请响应
  virtual int OnRspRpcStkPledgePartialRepaymentApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgePartialRepaymentApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 出借人额度查询响应
  virtual int OnRspQryRpcStkPledgeLenderQuota(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeLenderQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询合约违约状态为追保的合约响应
  virtual int OnRspQryRpcStkPledgeAdditionalMarginContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeAdditionalMarginContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询追保合约需要追保的数量响应
  virtual int OnRspQryRpcStkPledgeAdditionalMarginQty(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeAdditionalMarginQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 贷款本金偿还响应
  virtual int OnRspRpcStkPledgeLoanPrincipalRepayment(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeLoanPrincipalRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可用于还款资金查询响应
  virtual int OnRspQryRpcStkPledgeAvailableQtyForRepayment(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgeAvailableQtyForRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份部分解除质押申请响应
  virtual int OnRspRpcStkPledgePartialCancellationPledgeApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgePartialCancellationPledgeApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户个性化设置利率查询响应
  virtual int OnRspQryRpcStkPledgePersonalizedInterestRate(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkPledgePersonalizedInterestRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约利息偿还申请响应
  virtual int OnRspRpcStkPledgeRepaymentContractInterestApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcStkPledgeRepaymentContractInterestApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 约定购回客户额度查询响应
  virtual int OnRspQryRpcAgreementRepurchaseDebtContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcAgreementRepurchaseDebtContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 约定购回客户额度查询响应
  virtual int OnRspQryRpcAgreementRepurchaseQuota(CFirstSetField *p_pFirstSetField, CRspStkQryRpcAgreementRepurchaseQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 约定购回初始交易响应
  virtual int OnRspRpcAgreementRepurchaseInitialTransaction(CFirstSetField *p_pFirstSetField, CRspStkRpcAgreementRepurchaseInitialTransactionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 约定购回购回交易响应
  virtual int OnRspRpcAgreementRepurchaseRepurchaseTransaction(CFirstSetField *p_pFirstSetField, CRspStkRpcAgreementRepurchaseRepurchaseTransactionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 约定购回延期购回申请响应
  virtual int OnRspRpcAgreementRepurchaseDelayedRepurchaseApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcAgreementRepurchaseDelayedRepurchaseApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取权证信息响应
  virtual int OnRspRpcWarrantInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcWarrantInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户自主行权委托预计所得税查询响应
  virtual int OnRspQryRpcExpectedIncomeTax(CFirstSetField *p_pFirstSetField, CRspStkQryRpcExpectedIncomeTaxField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 合约查询(410983)响应
  virtual int OnRspQryRpcContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权委托查询(410989)响应
  virtual int OnRspQryRpcFinancingRightCommission(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingRightCommissionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股权激励人员信息查询（410990）响应
  virtual int OnRspQryRpcEquityIncentivePersonnelInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcEquityIncentivePersonnelInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权参数信息查询（410991）响应
  virtual int OnRspQryRpcFinancingExerciseParams(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingExerciseParamsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权资产负债查询（410992）响应
  virtual int OnRspQryRpcFinancingExerciseAssetsLiability(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingExerciseAssetsLiabilityField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权客户额度信息查询（410993）响应
  virtual int OnRspQryRpcFinancingExerciseCustQuotaInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingExerciseCustQuotaInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 直接还款(410984)响应
  virtual int OnRspRpcDirectRepayment(CFirstSetField *p_pFirstSetField, CRspStkRpcDirectRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权业务(410985)响应
  virtual int OnRspRpcFinancingExerciseBiz(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancingExerciseBizField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 取融资行权可委托数量(410986)响应
  virtual int OnRspRpcFinancingExerciseMaxOrderQty(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancingExerciseMaxOrderQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权委托撤单(410987)响应
  virtual int OnRspRpcFinancingExerciseCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancingExerciseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权担保证券查询(410988)响应
  virtual int OnRspQryRpcFinancingExerciseGuaranteedSecurityInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingExerciseGuaranteedSecurityInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF成份股清单信息查询响应
  virtual int OnRspQryRpcETFStkList(CFirstSetField *p_pFirstSetField, CRspStkQryRpcETFStkListField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请（现金认购，股票认购）响应
  virtual int OnRspRpcETFOfflineApplication(CFirstSetField *p_pFirstSetField, CRspStkRpcETFOfflineApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请撤单响应
  virtual int OnRspRpcETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcETFOfflineApplicationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请查询响应
  virtual int OnRspQryRpcETFOfflineApplication(CFirstSetField *p_pFirstSetField, CRspStkQryRpcETFOfflineApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳LOF基金分拆合并信息查询响应
  virtual int OnRspQryRpcSzLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海LOF基金分拆合并信息查询响应
  virtual int OnRspQryRpcShLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盘后基金分拆/合并根据昨日申购额得到的建议值响应
  virtual int OnRspRpcAfterCloseFundSplitMergeRecommendedValue(CFirstSetField *p_pFirstSetField, CRspStkRpcAfterCloseFundSplitMergeRecommendedValueField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户国债预发行持仓明细响应
  virtual int OnRspQryRpcTreasuryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTreasuryBondsPreIssuePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户国债预发行平仓明细响应
  virtual int OnRspQryRpcTreasuryBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTreasuryBondsPreIssueClosePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户国债预发行应付履约金明细响应
  virtual int OnRspQryRpcTreasuryBondsPreIssuePerformanceBond(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTreasuryBondsPreIssuePerformanceBondField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票股东大会信息查询(440001)响应
  virtual int OnRspQryRpcNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcNetworkVotingTrdacctsConferenceInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票议案信息查询(440002)响应
  virtual int OnRspQryRpcNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcNetworkVotingMotionInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票结果查询(440003)响应
  virtual int OnRspQryRpcNetworkVotingResult(CFirstSetField *p_pFirstSetField, CRspStkQryRpcNetworkVotingResultField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票(440101)响应
  virtual int OnRspRpcNetworkVoting(CFirstSetField *p_pFirstSetField, CRspStkRpcNetworkVotingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票委托查询(440103)响应
  virtual int OnRspQryRpcNetworkVoting(CFirstSetField *p_pFirstSetField, CRspStkQryRpcNetworkVotingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 标准券汇总查询响应
  virtual int OnRspQryRpcStdBondSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStdBondSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 质押库标准券明细查询响应
  virtual int OnRspQryRpcPledgeBankStdBondDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcPledgeBankStdBondDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 回购资金占用天数查询响应
  virtual int OnRspQryRpcRepurchaseFundOccupancyDays(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRepurchaseFundOccupancyDaysField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 未到期回购查询响应
  virtual int OnRspQryRpcUndueRepurchase(CFirstSetField *p_pFirstSetField, CRspStkQryRpcUndueRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户融资回购风险查询响应
  virtual int OnRspQryRpcFinancingRepurchaseRisk(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancingRepurchaseRiskField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户报价回购品种信息响应
  virtual int OnRspQryRpcShQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShQuoteRepurchaseProductInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 提前购回响应
  virtual int OnRspRpcShQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, CRspStkRpcShQuoteRepurchaseAdvanceRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约响应
  virtual int OnRspRpcShQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, CRspStkRpcShQuoteRepurchaseReservationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约流水查询响应
  virtual int OnRspQryRpcShQuoteRepurchaseReservationStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShQuoteRepurchaseReservationStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户未到期的报价回购信息响应
  virtual int OnRspQryRpcShQuoteRepurchaseUndueRepurchase(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShQuoteRepurchaseUndueRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购对应质押物明细响应
  virtual int OnRspRpcShQuoteRepurchasePledgeDetail(CFirstSetField *p_pFirstSetField, CRspStkRpcShQuoteRepurchasePledgeDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海报价回购质权信息查询响应
  virtual int OnRspQryRpcShQuoteRepurchasePledgeInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShQuoteRepurchasePledgeInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购批量委托保留额度查询响应
  virtual int OnRspQryRpcShQuoteRepurchaseBatchOrderRetentionLimit(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShQuoteRepurchaseBatchOrderRetentionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购批量委托保留额度设置响应
  virtual int OnRspRpcShQuoteRepurchaseSetBatchOrderRetentionLimit(CFirstSetField *p_pFirstSetField, CRspStkRpcShQuoteRepurchaseSetBatchOrderRetentionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购产品信息查询响应
  virtual int OnRspQryRpcSzQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseAdvanceRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购预约响应
  virtual int OnRspRpcSzQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseReservationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购预约取消响应
  virtual int OnRspRpcSzQuoteRepurchaseReservationCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseReservationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购预约执行响应
  virtual int OnRspRpcSzQuoteRepurchaseReservationExecution(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseReservationExecutionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购提前购回响应
  virtual int OnRspRpcSzQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseAdvanceRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约流水查询响应
  virtual int OnRspQryRpcSzQuoteRepurchaseReservationStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseReservationStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购合约数量调整响应
  virtual int OnRspRpcSzQuoteRepurchaseChgContractQty(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseChgContractQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购展期取消响应
  virtual int OnRspRpcSzQuoteRepurchaseExtensionCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseExtensionCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询深圳客户未到期的报价回购信息响应
  virtual int OnRspQryRpcSzQuoteRepurchaseUndueRepurchase(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseUndueRepurchaseField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询深圳报价回购质押折算率响应
  virtual int OnRspQryRpcSzQuoteRepurchasePledgeConversionRate(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchasePledgeConversionRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购现金质押查询响应
  virtual int OnRspQryRpcSzQuoteRepurchaseCashPledge(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseCashPledgeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳报价回购质权信息查询响应
  virtual int OnRspQryRpcSzQuoteRepurchaseMortgage(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseMortgageField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询深圳报价回购客户特定报价响应
  virtual int OnRspQryRpcSzQuoteRepurchaseSpecificOffer(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchaseSpecificOfferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户基金投资适当性信息维护响应
  virtual int OnRspRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriateness(CFirstSetField *p_pFirstSetField, CRspStkRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriatenessField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押物查询响应
  virtual int OnRspQryRpcSzQuoteRepurchasePledge(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzQuoteRepurchasePledgeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券协议回购委托申报响应
  virtual int OnRspRpcBondAgreementRepurchaseOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcBondAgreementRepurchaseOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券协议回购委托撤单申报响应
  virtual int OnRspRpcBondAgreementRepurchaseCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcBondAgreementRepurchaseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券协议回购委托流水查询响应
  virtual int OnRspQryRpcBondAgreementRepurchaseStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondAgreementRepurchaseStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券协议回购成交查询响应
  virtual int OnRspQryRpcBondAgreementRepurchaseMatch(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondAgreementRepurchaseMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券协议回购合约查询响应
  virtual int OnRspQryRpcBondAgreementRepurchaseContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondAgreementRepurchaseContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 密码服务委托申报响应
  virtual int OnRspRpcPwdServiceOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcPwdServiceOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东查询，查询客户对应的股东代码响应
  virtual int OnRspQryRpcTrdacct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTrdacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询(含创新业务)响应
  virtual int OnRspQryRpcCuacct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资产明细查询响应
  virtual int OnRspQryRpcAssetDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcAssetDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份明细查询响应
  virtual int OnRspQryRpcStkDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份汇总查询响应
  virtual int OnRspQryRpcStkSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 外围资金帐户资金情况查询响应
  virtual int OnRspQryRpcCuacct2(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacct2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户查询，根据股东内码，查询股东代码响应
  virtual int OnRspQryRpcTrdacct2(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTrdacct2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户查询，根据股东代码，资金帐户查询客户代码响应
  virtual int OnRspQryRpcTrdacct3(CFirstSetField *p_pFirstSetField, CRspStkQryRpcTrdacct3Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托查询响应
  virtual int OnRspQryRpcOrderDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托查询(含创新业务)响应
  virtual int OnRspQryRpcOrder2(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrder2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交明细查询响应
  virtual int OnRspQryRpcMatchDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMatchDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交查询(含创新业务)响应
  virtual int OnRspQryRpcMatch(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交查询（微证券专用）响应
  virtual int OnRspQryRpcMatchForTencentPay(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMatchForTencentPayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托汇总查询, 按证券代码和买卖方向响应
  virtual int OnRspQryRpcHisOrderSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHisOrderSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 成交汇总查询, 按证券代码和买卖方向响应
  virtual int OnRspQryRpcHisMatchSummary(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHisMatchSummaryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 交割单查询，根据柜台普通对帐的摘要配置打印信息打印单据。微证券专用响应
  virtual int OnRspQryRpcDeliveryForTencentPay(CFirstSetField *p_pFirstSetField, CRspStkQryRpcDeliveryForTencentPayField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托汇总查询,ordergroup+stkcode+bsflag响应
  virtual int OnRspQryRpcOrderSummary2(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrderSummary2Field *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户机构查询响应
  virtual int OnRspQryRpcOrg(CFirstSetField *p_pFirstSetField, CRspStkQryRpcOrgField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户属性和资金帐号属性响应
  virtual int OnRspQryRpcAcctInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcAcctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 意向行情查询响应
  virtual int OnRspQryRpcMktIntention(CFirstSetField *p_pFirstSetField, CRspStkQryRpcMktIntentionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询营业部某一业务类别的申报席位响应
  virtual int OnRspQryRpcSeat(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSeatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询资金账户信息响应
  virtual int OnRspQryRpcCuacctAcctInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCuacctAcctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 非流通股份余额查询（沪市）响应
  virtual int OnRspQryRpcShNonCirculatingShareBalance(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShNonCirculatingShareBalanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳非流通股份余额查询响应
  virtual int OnRspQryRpcSzNonCirculatingShareBalance(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzNonCirculatingShareBalanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股息红利扣税查询响应
  virtual int OnRspQryRpcDividendWithholdingTax(CFirstSetField *p_pFirstSetField, CRspStkQryRpcDividendWithholdingTaxField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户所属机构响应
  virtual int OnRspQryRpcCustOrg(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCustOrgField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询存管资金账号响应
  virtual int OnRspQryRpcDepositoryCuacct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcDepositoryCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 场内基金申赎失败查询响应
  virtual int OnRspQryRpcFloorFundRedemptionFailure(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFloorFundRedemptionFailureField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 黑名单证券查询响应
  virtual int OnRspQryRpcBlacklistStk(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBlacklistStkField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳大宗交易行情查询响应
  virtual int OnRspQryRpcSzBlockTradingMkt(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzBlockTradingMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海大宗交易行情查询响应
  virtual int OnRspQryRpcShBlockTradingMkt(CFirstSetField *p_pFirstSetField, CRspStkQryRpcShBlockTradingMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳场内基金分红方式查询响应
  virtual int OnRspQryRpcSzFloorFundDividends(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSzFloorFundDividendsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 受限股东减持额度查询响应
  virtual int OnRspQryRpcLimitedTrdacctReductionLimit(CFirstSetField *p_pFirstSetField, CRspStkQryRpcLimitedTrdacctReductionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通资金资产查询响应
  virtual int OnRspQryRpcHkCuacct(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkCuacctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通客户负债查询响应
  virtual int OnRspQryRpcHkLiability(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkLiabilityField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 权益查询响应
  virtual int OnRspQryRpcHkEquity(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkEquityField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通开户准备响应
  virtual int OnRspRpcHkPreparationForOpeningAccount(CFirstSetField *p_pFirstSetField, CRspStkRpcHkPreparationForOpeningAccountField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通未交收明细查询响应
  virtual int OnRspQryRpcHkUndeliveryDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkUndeliveryDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通证券组合费交收明细响应
  virtual int OnRspRpcHkStkCombinationFeeDeliveryDetail(CFirstSetField *p_pFirstSetField, CRspStkRpcHkStkCombinationFeeDeliveryDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交割单查询响应
  virtual int OnRspQryRpcHkDelivery(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkDeliveryField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通对帐单查询响应
  virtual int OnRspQryRpcHkStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通最小价差查询响应
  virtual int OnRspQryRpcHkMinSpread(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkMinSpreadField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通参考汇率查询响应
  virtual int OnRspQryRpcHkExchangeRate(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkExchangeRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通市场交易状态查询响应
  virtual int OnRspQryRpcHkMarketStatus(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkMarketStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通标的证券查询响应
  virtual int OnRspQryRpcHkUnderlyingSecurity(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkUnderlyingSecurityField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易日历查询响应
  virtual int OnRspQryRpcHkCalendar(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkCalendarField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通委托买卖交易响应
  virtual int OnRspRpcHkOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcHkOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通撤单委托响应
  virtual int OnRspRpcHkCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcHkCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单查询响应
  virtual int OnRspQryRpcHkCancel(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通最大可交易数量查询响应
  virtual int OnRspQryRpcHkMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易委托明细查询响应
  virtual int OnRspQryRpcHkOrderDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易成交查询响应
  virtual int OnRspQryRpcHkMatch(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易历史委托查询响应
  virtual int OnRspQryRpcHkHisOrder(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkHisOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易成交历史查询响应
  virtual int OnRspQryRpcHkHisMatch(CFirstSetField *p_pFirstSetField, CRspStkQryRpcHkHisMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转系统证券委托申报响应
  virtual int OnRspRpcStkTranSysStkOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcStkTranSysStkOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转系统协议转让行情查询响应
  virtual int OnRspQryRpcStkTranSysAgreementTransferMkt(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkTranSysAgreementTransferMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转系统取最大交易数量,支持三方交易响应
  virtual int OnRspRpcStkTranSysMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspStkRpcStkTranSysMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转系统受限投资者查询响应
  virtual int OnRspQryRpcStkTranSysRestrictedInvestor(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkTranSysRestrictedInvestorField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转市场非流通股份余额查询响应
  virtual int OnRspQryRpcStkTranSysNonCirculatingSharesBalance(CFirstSetField *p_pFirstSetField, CRspStkQryRpcStkTranSysNonCirculatingSharesBalanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台交易委托明细查询响应
  virtual int OnRspQryRpcFixedIncomeSysOrderDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFixedIncomeSysOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台交易历史委托明细查询响应
  virtual int OnRspQryRpcFixedIncomeSysHisOrderDetail(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFixedIncomeSysHisOrderDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台交易成交查询响应
  virtual int OnRspQryRpcFixedIncomeSysMatch(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFixedIncomeSysMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台最大可交易数量查询响应
  virtual int OnRspQryRpcFixedIncomeSysMaxTradeQty(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFixedIncomeSysMaxTradeQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台委托买卖交易响应
  virtual int OnRspRpcFixedIncomeSysOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcFixedIncomeSysOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 固收平台撤单委托响应
  virtual int OnRspRpcFixedIncomeSysCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcFixedIncomeSysCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单查询响应
  virtual int OnRspQryRpcFixedIncomeSysCancel(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFixedIncomeSysCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券基本信息查询响应
  virtual int OnRspQryRpcBondBasicInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondBasicInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台债券行情查询响应
  virtual int OnRspQryRpcBondMkt(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品份额转让撤单响应
  virtual int OnRspRpcFinancialProductShareTransferCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancialProductShareTransferCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品份额转让业务响应
  virtual int OnRspRpcFinancialProductShareTransfer(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancialProductShareTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品转让联系人信息查询响应
  virtual int OnRspQryRpcFinancialProductShareTransferContactInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancialProductShareTransferContactInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品份额转让费用计算响应
  virtual int OnRspRpcFinancialProductShareTransferFee(CFirstSetField *p_pFirstSetField, CRspStkRpcFinancialProductShareTransferFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台债券参数信息查询响应
  virtual int OnRspQryRpcBondParamInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondParamInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 意向委托流水查询响应
  virtual int OnRspQryRpcBondIntentionOrderStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcBondIntentionOrderStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品行情查询响应
  virtual int OnRspQryRpcFinancialProductMarket(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancialProductMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 柜台理财产品委托查询响应
  virtual int OnRspQryRpcFinancialProductOrder(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFinancialProductOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券出借委托外围接口响应
  virtual int OnRspRpcRefinancingStkLendingOrder(CFirstSetField *p_pFirstSetField, CRspStkRpcRefinancingStkLendingOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券出借委托撤单外围接口响应
  virtual int OnRspRpcRefinancingStkLendingCancel(CFirstSetField *p_pFirstSetField, CRspStkRpcRefinancingStkLendingCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托查询外围接口响应
  virtual int OnRspQryRpcRefinancingOrder(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 提前索还外围接口响应
  virtual int OnRspRpcRefinancingAdvanceRevert(CFirstSetField *p_pFirstSetField, CRspStkRpcRefinancingAdvanceRevertField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 同意展期外围接口响应
  virtual int OnRspRpcRefinancingExtentionApprove(CFirstSetField *p_pFirstSetField, CRspStkRpcRefinancingExtentionApproveField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券出借可撤记录查询外围接口响应
  virtual int OnRspQryRpcRefinancingStkLendingCanWithdraw(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingStkLendingCanWithdrawField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 转融券未了结合约查询外围接口响应
  virtual int OnRspQryRpcRefinancingOpenContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingOpenContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可出借标的券查询外围接口响应
  virtual int OnRspQryRpcRefinancingUnderlyingStkForLending(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingUnderlyingStkForLendingField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 出借人信息查询外围接口响应
  virtual int OnRspQryRpcRefinancingLenderInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingLenderInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史出借委托查询响应
  virtual int OnRspQryRpcRefinancingHisLendingOrder(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingHisLendingOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券出借已了结合约查询响应
  virtual int OnRspQryRpcRefinancingStkLendingClosedContract(CFirstSetField *p_pFirstSetField, CRspStkQryRpcRefinancingStkLendingClosedContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户的预留信息响应
  virtual int OnRspQryRpcSysSecurityCustReservedInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSysSecurityCustReservedInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 设置客户预留信息响应
  virtual int OnRspRpcSysSecuritySetCustReservedInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecuritySetCustReservedInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 同步客户动态令牌响应
  virtual int OnRspRpcSysSecuritySyncDynamicToken(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecuritySyncDynamicTokenField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户证书获取随机码响应
  virtual int OnRspRpcSysSecurityCertificateRandomCode(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecurityCertificateRandomCodeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户动态令牌状态设置响应
  virtual int OnRspRpcSysSecuritySetDynamicTokenStatus(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecuritySetDynamicTokenStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户获取一次性动态口令响应
  virtual int OnRspRpcSysSecurityOneTimeDynamicPwd(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecurityOneTimeDynamicPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户登陆绑定信息设置响应
  virtual int OnRspRpcSysSecuritySetLoginBindingInfo(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecuritySetLoginBindingInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户登陆绑定信息查询响应
  virtual int OnRspQryRpcSysSecurityLoginBindingInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcSysSecurityLoginBindingInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 校验客户交易密码响应
  virtual int OnRspRpcSysSecurityVerifyTrdPwd(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecurityVerifyTrdPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户获取手机一次性动态口令响应
  virtual int OnRspRpcSysSecurityMobileOneTimeDynamicPwd(CFirstSetField *p_pFirstSetField, CRspStkRpcSysSecurityMobileOneTimeDynamicPwdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询客户ETF篮各成分股的持仓数量响应
  virtual int OnRspQryRpcETFConstituentStkPosi(CFirstSetField *p_pFirstSetField, CRspStkQryRpcETFConstituentStkPosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 普通节点和VIP系统间证券划拨响应
  virtual int OnRspRpcCommVIPStkTransfer(CFirstSetField *p_pFirstSetField, CRspStkRpcCommVIPStkTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 现金资产转银行响应
  virtual int OnRspRpcCashAssetBankTransfer(CFirstSetField *p_pFirstSetField, CRspStkRpcCashAssetBankTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金划拨流水查询响应
  virtual int OnRspQryRpcCapitalTransferStatement(CFirstSetField *p_pFirstSetField, CRspStkQryRpcCapitalTransferStatementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // Vip客户资金资产同步表查询响应
  virtual int OnRspQryRpcVIPCuacctSyncTable(CFirstSetField *p_pFirstSetField, CRspStkQryRpcVIPCuacctSyncTableField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 场内产品适当性信息查询响应
  virtual int OnRspQryRpcFloorTradingProductAppropriatenessInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcFloorTradingProductAppropriatenessInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户适当性信息查询响应
  virtual int OnRspQryRpcAppropriatenessInfo(CFirstSetField *p_pFirstSetField, CRspStkQryRpcAppropriatenessInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户与场内产品适当性匹配校验响应
  virtual int OnRspRpcAppropriatenessMatchingCheck(CFirstSetField *p_pFirstSetField, CRspStkRpcAppropriatenessMatchingCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 记录场内业务风险揭示书签署流水响应
  virtual int OnRspRpcFloorTradingRiskDisclosureStatementSignedRecordFlow(CFirstSetField *p_pFirstSetField, CRspStkRpcFloorTradingRiskDisclosureStatementSignedRecordFlowField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 系统探测响应
  virtual int OnRspBizStatus(CFirstSetField *p_pFirstSetField, CRspStkBizStatusField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 质押股份明细查询(单客户)响应
  virtual int OnRspQryPledgeShares(CFirstSetField *p_pFirstSetField, CRspStkQryPledgeSharesField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金流水查询响应
  virtual int OnRspQryFundFlow(CFirstSetField *p_pFirstSetField, CRspStkQryFundFlowField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 买卖委托(快速)响应
  virtual int OnRspOrderEx(CFirstSetField *p_pFirstSetField, CRspStkOrderExField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份流水查询响应
  virtual int OnRspQryStockFlow(CFirstSetField *p_pFirstSetField, CRspStkQryStockFlowField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 日切交易日查询响应
  virtual int OnRspQrySettDate(CFirstSetField *p_pFirstSetField, CRspStkQrySettDateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盘后基金行情查询响应
  virtual int OnRspQryLofMktInfo(CFirstSetField *p_pFirstSetField, CRspStkQryLofMktInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金划拨(集中交易)响应
  virtual int OnRspFundTransferJzjy(CFirstSetField *p_pFirstSetField, CRspStkFundTransferJzjyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可用资金查询(集中交易)响应
  virtual int OnRspQryExpendableFundJzjy(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableFundJzjyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 认证信息设置(密码录入/修改)响应
  virtual int OnRspAuthDataSetJzjy(CFirstSetField *p_pFirstSetField, CRspStkAuthDataSetJzjyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金划拨(单账户多节点) 订单节点之间划拨响应
  virtual int OnRspFundTransferNode(CFirstSetField *p_pFirstSetField, CRspStkFundTransferNodeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转账响应
  virtual int OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, CRspStkBankStkTransferField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 要约收购收购人信息查询响应
  virtual int OnRspQryTenderSgrInfo(CFirstSetField *p_pFirstSetField, CRspStkQryTenderSgrInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盘后行情查询(创业板)响应
  virtual int OnRspQryClosingMktSecondboard(CFirstSetField *p_pFirstSetField, CRspStkQryClosingMktSecondboardField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请撤单响应
  virtual int OnRspEtfOfflineCancelOrder(CFirstSetField *p_pFirstSetField, CRspStkEtfOfflineCancelOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请查询响应
  virtual int OnRspQryEtfOfflineOrder(CFirstSetField *p_pFirstSetField, CRspStkQryEtfOfflineOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股日历查询响应
  virtual int OnRspQryNewStkCal(CFirstSetField *p_pFirstSetField, CRspStkQryNewStkCalField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 共享内存品种信息查询响应
  virtual int OnRspQryStkInfoShm(CFirstSetField *p_pFirstSetField, CRspStkQryStkInfoShmField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券竞买业务预约委托撤单响应
  virtual int OnRspReserveOrderCancel(CFirstSetField *p_pFirstSetField, CRspStkReserveOrderCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳债券报价行情查询响应
  virtual int OnRspQryBondQuote(CFirstSetField *p_pFirstSetField, CRspStkQryBondQuoteField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 协商成交回报行情信息查询响应
  virtual int OnRspQryReportMktInfo(CFirstSetField *p_pFirstSetField, CRspStkQryReportMktInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券现券交易竞买预约委托信息查询响应
  virtual int OnRspQryCashBondReserveOrderInfo(CFirstSetField *p_pFirstSetField, CRspStkQryCashBondReserveOrderInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //委托汇总查询响应
  virtual int OnRspQryOrderSummary(CFirstSetField *p_pFirstSetField, CRspStkQryOrderSummaryField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //成功汇总查询响应
  virtual int OnRspQryMatchSummary(CFirstSetField *p_pFirstSetField, CRspStkQryMatchSummaryField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //债券现券交易竞买预约信息查询响应
  virtual int OnRspQryCashBondTradeReservationInfo(CFirstSetField *p_pFirstSetField, CRspStkQryCashBondTradeReservationInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //债券现券交易业务参考信息查询响应
  virtual int OnRspQryCashBondTradeProfitInfo(CFirstSetField *p_pFirstSetField, CRspStkQryCashBondTradeProfitInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //债券通用质押式回购业务参考信息查询响应
  virtual int OnRspQryBondPledgeRepchProfitInfo(CFirstSetField *p_pFirstSetField, CRspStkQryBondPledgeRepchProfitInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
private:
};

class MATRADEAPI CCliStkTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliStkTradeApi(void);

  // 析构函数
  virtual ~CCliStkTradeApi(void);

  // 证券信息查询请求[快速]
  virtual int ReqQryStkInfo(CReqStkQryInfoExField *p_pReqField, LONGLONG p_llRequestId);

  // 证券信息查询请求
  virtual int ReqQryStkInfo(CReqStkQryStkInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 盘后行情查询(上海科创板)请求
  virtual int ReqQryClosingMkt(CReqStkQryClosingMktField *p_pReqField, LONGLONG p_llRequestId);

  // LOF信息查询请求
  virtual int ReqQryLofInfo(CReqStkQryLofInfoField *p_pReqField, LONGLONG p_llRequestId);

  // ETF信息查询请求
  virtual int ReqQryEtfInfo(CReqStkQryEtfInfoField *p_pReqField, LONGLONG p_llRequestId);

  // ETF篮信息查询请求
  virtual int ReqQryEtfBasket(CReqStkQryEtfBasketField *p_pReqField, LONGLONG p_llRequestId);

  // 买卖委托请求
  virtual int ReqOrder(CReqStkOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 组合委托请求
  virtual int ReqCombOrder(CReqStkCombOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 委托撤单请求
  virtual int ReqCancelOrder(CReqStkCancelOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 最大可交易数计算请求
  virtual int ReqMaxTradeQty(CReqStkMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);

  // 批量委托请求
  virtual int ReqQuantityOrder(CReqStkQuantityOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 批量撤单请求
  virtual int ReqQuantityCancelOrder(CReqStkQuantityCancelOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 买卖委托请求(极速)
  virtual int ReqOrderFlash(CReqStkOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // 委托撤单请求(极速)
  virtual int ReqCancelOrderFlash(CReqStkCancelOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // 组合委托请求(极速)
  virtual int ReqCombOrderFlash(CReqStkCombOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // ETF 篮子委托请求
  virtual int ReqEtfOrder(CReqStkEtfOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 篮子委托最大可交易数计算请求
  virtual int ReqEtfMaxTradeQty(CReqStkEtfMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询请求
  virtual int ReqQryExpendableFund(CReqStkQryExpendableFundField *p_pReqField, LONGLONG p_llRequestId);

  // 可用股份查询请求
  virtual int ReqQryExpendableShares(CReqStkQryExpendableSharesField *p_pReqField, LONGLONG p_llRequestId);

  // 当日委托查询请求
  virtual int ReqQryCurrDayOrder(CReqStkQryCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 当日成交查询请求
  virtual int ReqQryCurrDayFill(CReqStkQryCurrDayFillField *p_pReqField, LONGLONG p_llRequestId);

  // 股东账户查询请求
  virtual int ReqQryStkholderAcct(CReqStkQryStkholderAcctField *p_pReqField, LONGLONG p_llRequestId);

  // 可撤单委托查询请求
  virtual int ReqQryCanWithdrawOrder(CReqStkQryCanWithdrawOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询请求[快速]
  virtual int ReqQryExpendableFundEx(CReqStkQryExpendableFundExField *p_pReqField, LONGLONG p_llRequestId);

  // 可用股份查询请求[快速]
  virtual int ReqQryExpendableSharesEx(CReqStkQryExpendableSharesExField *p_pReqField, LONGLONG p_llRequestId);

  // 当日委托查询请求[快速]
  virtual int ReqQryCurrDayOrderEx(CReqStkQryCurrDayOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // 当日成交查询请求[快速]
  virtual int ReqQryCurrDayFillEx(CReqStkQryCurrDayFillExField *p_pReqField, LONGLONG p_llRequestId);

  // 股东账户查询请求[快速]
  virtual int ReqQryStkholderAcctEx(CReqStkQryStkholderAcctExField *p_pReqField, LONGLONG p_llRequestId);

  // 可撤单委托查询请求[快速]
  virtual int ReqQryCanWithdrawOrderEx(CReqStkQryCanWithdrawOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // ETF 成分股持仓查询请求
  virtual int ReqQryEtfCotPosition(CReqStkQryEtfCotPositionField *p_pReqField, LONGLONG p_llRequestId);

  // 用户登录请求
  virtual int ReqUserLogin(CReqStkUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 历史委托查询请求
  virtual int ReqStkQryHisOrder(CReqStkQryHisOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 历史成交查询请求
  virtual int ReqStkQryHisFill(CReqStkQryHisFillField *p_pReqField, LONGLONG p_llRequestId);

  // 当日/历史委托查询请求
  virtual int ReqStkQryCurrDayHisOrder(CReqStkQryCurrDayHisOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 当日/历史成交查询请求
  virtual int ReqStkQryCurrDayHisFill(CReqStkQryCurrDayHisFillField *p_pReqField, LONGLONG p_llRequestId);

  //转帐银行业务信息 2015/8/12
  virtual int ReqQryBankTransBasicInfo(CReqStkBankTransBasicInfoField *p_pReqField, LONGLONG p_llRequestId);

  //银证转帐       2015/8/12
  virtual int ReqBankStkTrans(CReqStkBankStkTransField *p_pReqField, LONGLONG p_llRequestId);

  //查询账户余额   2015/8/12
  virtual int ReqQryFundBala(CReqStkFundBalaField *p_pReqField, LONGLONG p_llRequestId);

  //银证转帐查询   2015/8/12
  virtual int ReqQryBankStkTrans(CReqStkBankStkTransInfoField *p_pReqField, LONGLONG p_llRequestId);

  //修改交易密码   2015/8/12
  virtual int ReqChgTrdPwd(CReqStkChgTrdPwdField *p_pReqField, LONGLONG p_llRequestId);

  //修改资金密码   2015/8/12
  virtual int ReqChgFundPwd(CReqStkChgFundPwdField *p_pReqField, LONGLONG p_llRequestId);

  //对帐单查询     2015/8/12
  virtual int ReqQryStatement(CReqStkStatementField *p_pReqField, LONGLONG p_llRequestId);

  //资金划拨
  virtual int ReqQryFundTransfer(CReqStkFundTransferField *p_pReqField, LONGLONG p_llRequestId);

  // 证券市值额度查询
  virtual int ReqQryMktQuota(CReqStkMktQuotaField *p_pReqField, LONGLONG p_llRequestId);

  // 港股通汇率信息查询请求
  virtual int ReqQryRateInfo(CReqStkQryRateInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 港股通市场状态信息查询请求
  virtual int ReqQryMktStatusInfo(CReqStkQryMktStatusInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 港股通标的证券状态查询请求
  virtual int ReqQryUndlStkStatus(CReqStkQryUndlStkStatusField *p_pReqField, LONGLONG p_llRequestId);

  // 港股通二级资金台账查询请求
  virtual int ReqQryFundAcctLvlTwo(CReqStkQryFundAcctLvlTwoField *p_pReqField, LONGLONG p_llRequestId);

  // 港股通交易最小价差查询请求
  virtual int ReqQryTradeMinSpread(CReqStkQryTradeMinSpreadField *p_pReqField, LONGLONG p_llRequestId);

  // 回购合约查询请求
  virtual int ReqQryRepchContract(CReqStkQryRepchContractField *p_pReqField, LONGLONG p_llRequestId);

  // 客户适当性查询
  virtual int ReqQryCustAppropriate(CReqStkQryCustAppropriateField *p_pReqField, LONGLONG p_llRequestId);

  // 产品适当性查询
  virtual int ReqQryProdAppropriate(CReqStkQryProdAppropriateField *p_pReqField, LONGLONG p_llRequestId);

  // 适当性匹配检查
  virtual int ReqAppropriateCheck(CReqStkAppropriateCheckField *p_pReqField, LONGLONG p_llRequestId);

  // 适当性匹配结果记录
  virtual int ReqAppropriateResult(CReqStkAppropriateResultField *p_pReqField, LONGLONG p_llRequestId);

  // 回购资金占用天数查询
  virtual int ReqQryRepchFund(CReqStkQryRepchFundField *p_pReqField, LONGLONG p_llRequestId);

  // 上证LOF信息查询
  virtual int ReqQryScLofInfo(CReqStkQryScLofInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托明细查询请求
  virtual int ReqQryRpcHistoryOrderDetail(CReqStkQryRpcHistoryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交明细查询请求
  virtual int ReqQryRpcHistoryMatchDetail(CReqStkQryRpcHistoryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 当日新股清单查询请求
  virtual int ReqQryRpcIPO(CReqStkQryRpcIPOField *p_pReqField, LONGLONG p_llRequestId);
  // 外围新股申购市值额度表查询请求
  virtual int ReqQryRpcIPOSubscriptionLimit(CReqStkQryRpcIPOSubscriptionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 配号查询请求
  virtual int ReqQryRpcDistribution(CReqStkQryRpcDistributionField *p_pReqField, LONGLONG p_llRequestId);
  // 市值配售中签查询请求
  virtual int ReqQryRpcMktCapPlacing(CReqStkQryRpcMktCapPlacingField *p_pReqField, LONGLONG p_llRequestId);
  // 中签历史查询请求
  virtual int ReqQryRpcWinningHistory(CReqStkQryRpcWinningHistoryField *p_pReqField, LONGLONG p_llRequestId);
  // 市值配售交收顺序修改请求
  virtual int ReqRpcChgMktCapDistDeliOrder(CReqStkRpcChgMktCapDistDeliOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 交割单查询请求
  virtual int ReqQryRpcDeliOrder(CReqStkQryRpcDeliOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 对帐查询请求
  virtual int ReqQryRpcChkAcct(CReqStkQryRpcChkAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 获取开通转帐(银行)账号查询请求
  virtual int ReqQryRpcBankAcct(CReqStkQryRpcBankAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 获取开通外部(银行)账号查询请求
  virtual int ReqQryRpcOtherBankAcct(CReqStkQryRpcOtherBankAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询请求
  virtual int ReqQryRpcFund(CReqStkQryRpcFundField *p_pReqField, LONGLONG p_llRequestId);
  // 当日资金流水查询请求
  virtual int ReqQryRpcFundDetail(CReqStkQryRpcFundDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托汇总(410514)请求
  virtual int ReqQryRpcOrderSummary(CReqStkQryRpcOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交汇总(410516)请求
  virtual int ReqQryRpcMatchSummary(CReqStkQryRpcMatchSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 查询当前系统状态请求
  virtual int ReqQryRpcCurSysStat(CReqStkQryRpcCurSysStatField *p_pReqField, LONGLONG p_llRequestId);
  // 取字典信息请求
  virtual int ReqRpcDictInfo(CReqStkRpcDictInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 证券信息请求
  virtual int ReqRpcStkInfo(CReqStkRpcStkInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 查询机构信息请求
  virtual int ReqQryRpcOrgInfo(CReqStkQryRpcOrgInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 银证通转帐银行业务信息请求
  virtual int ReqRpcBankStkTransferBankBizInfo(CReqStkRpcBankStkTransferBankBizInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 用户登陆请求
  virtual int ReqRpcLogin(CReqStkRpcLoginField *p_pReqField, LONGLONG p_llRequestId);
  // 客户总资产查询(新)请求
  virtual int ReqQryRpcTotalAssets(CReqStkQryRpcTotalAssetsField *p_pReqField, LONGLONG p_llRequestId);
  // 修改联系方式（网上交易使用）请求
  virtual int ReqRpcChgContact(CReqStkRpcChgContactField *p_pReqField, LONGLONG p_llRequestId);
  // 客户资料查询请求
  virtual int ReqQryRpcCustInfo(CReqStkQryRpcCustInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 修改操作方式请求
  virtual int ReqRpcChgOperMode(CReqStkRpcChgOperModeField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户开通的操作方式请求
  virtual int ReqQryRpcCustOperMode(CReqStkQryRpcCustOperModeField *p_pReqField, LONGLONG p_llRequestId);
  // 修改客户联系人信息请求
  virtual int ReqRpcChgCustInfo(CReqStkRpcChgCustInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户的扩展属性请求
  virtual int ReqRpcSetCustAttrEx(CReqStkRpcSetCustAttrExField *p_pReqField, LONGLONG p_llRequestId);
  // 设置数字签名请求
  virtual int ReqRpcSetDigitalSign(CReqStkRpcSetDigitalSignField *p_pReqField, LONGLONG p_llRequestId);
  // 外围修改资金帐号权限请求
  virtual int ReqRpcChgCuacctPermission(CReqStkRpcChgCuacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 资金帐号开通资金权限查询请求
  virtual int ReqQryRpcCuacctPermission(CReqStkQryRpcCuacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 证券帐号开通权限查询请求
  virtual int ReqQryRpcTrdacctPermission(CReqStkQryRpcTrdacctPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 客户权限有效期信息查询请求
  virtual int ReqQryRpcCustPermissionValidPeriod(CReqStkQryRpcCustPermissionValidPeriodField *p_pReqField, LONGLONG p_llRequestId);
  // 外围激活客户权限请求
  virtual int ReqRpcActivateCustPermission(CReqStkRpcActivateCustPermissionField *p_pReqField, LONGLONG p_llRequestId);
  // 客户特殊手续费查询请求
  virtual int ReqQryRpcCustSpecialFee(CReqStkQryRpcCustSpecialFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 一次性服务佣金收取请求
  virtual int ReqRpcOneTimeServiceCommission(CReqStkRpcOneTimeServiceCommissionField *p_pReqField, LONGLONG p_llRequestId);
  // 一次性服务佣金撤单请求
  virtual int ReqRpcCancelOneTimeServiceCommission(CReqStkRpcCancelOneTimeServiceCommissionField *p_pReqField, LONGLONG p_llRequestId);
  // 固定期限服务佣金办理请求
  virtual int ReqRpcFixedTermServiceCommission(CReqStkRpcFixedTermServiceCommissionField *p_pReqField, LONGLONG p_llRequestId);
  // 固定期限服务佣金退款请求
  virtual int ReqRpcFixedTermServiceCommissionRefund(CReqStkRpcFixedTermServiceCommissionRefundField *p_pReqField, LONGLONG p_llRequestId);
  // 一次性服务佣金设置查询请求
  virtual int ReqQryRpcOneTimeServiceCommissionSetting(CReqStkQryRpcOneTimeServiceCommissionSettingField *p_pReqField, LONGLONG p_llRequestId);
  // 一次性服务佣金收取查询请求
  virtual int ReqQryRpcOneTimeServiceCommissions(CReqStkQryRpcOneTimeServiceCommissionsField *p_pReqField, LONGLONG p_llRequestId);
  // 查询咨询付费类别请求
  virtual int ReqQryRpcConsultationPaymentCategory(CReqStkQryRpcConsultationPaymentCategoryField *p_pReqField, LONGLONG p_llRequestId);
  // 增加咨询付费类别请求
  virtual int ReqRpcAddConsultationPaymentCategory(CReqStkRpcAddConsultationPaymentCategoryField *p_pReqField, LONGLONG p_llRequestId);
  // 维护客户订阅状态请求
  virtual int ReqRpcMaintainCustSubStatus(CReqStkRpcMaintainCustSubStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户订阅流水请求
  virtual int ReqQryRpcCustSubStatement(CReqStkQryRpcCustSubStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 客户订阅设置请求
  virtual int ReqRpcCustSubSetting(CReqStkRpcCustSubSettingField *p_pReqField, LONGLONG p_llRequestId);
  // 第三方收费结构请求
  virtual int ReqRpcThirdPartyChargingStructure(CReqStkRpcThirdPartyChargingStructureField *p_pReqField, LONGLONG p_llRequestId);
  // 投资咨询产品费用保证金支付请求
  virtual int ReqRpcInvestmentAdvisoryProductFee(CReqStkRpcInvestmentAdvisoryProductFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 密码校验请求
  virtual int ReqRpcCuacctPwdCheck(CReqStkRpcCuacctPwdCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 资金资料查询请求
  virtual int ReqQryRpcCuacctInfo(CReqStkQryRpcCuacctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 获取客户资料核查状态请求
  virtual int ReqRpcCustInfoCheck(CReqStkRpcCustInfoCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户登录校验信息请求
  virtual int ReqRpcSetLoginCheckMode(CReqStkRpcSetLoginCheckModeField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户资料核查标识请求
  virtual int ReqRpcSetCustInfoCheckMode(CReqStkRpcSetCustInfoCheckModeField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户消息状态请求
  virtual int ReqRpcSetCustMsgStatus(CReqStkRpcSetCustMsgStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户消息服务请求
  virtual int ReqQryRpcCustMsgService(CReqStkQryRpcCustMsgServiceField *p_pReqField, LONGLONG p_llRequestId);
  // 修改银行密码请求
  virtual int ReqRpcChgBankPwd(CReqStkRpcChgBankPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 资金台帐间转帐请求
  virtual int ReqRpcTransferBetweenCuacct(CReqStkRpcTransferBetweenCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 外围客户交易帐户资金归集请求
  virtual int ReqRpcCuacctCashConcentration(CReqStkRpcCuacctCashConcentrationField *p_pReqField, LONGLONG p_llRequestId);
  // 取同一客户交易台帐、转帐台帐请求
  virtual int ReqRpcCuacctInfo2(CReqStkRpcCuacctInfo2Field *p_pReqField, LONGLONG p_llRequestId);
  // 资金可取数查询请求
  virtual int ReqQryRpcCuacctMaxDraw(CReqStkQryRpcCuacctMaxDrawField *p_pReqField, LONGLONG p_llRequestId);
  // 客户可取资金查询请求
  virtual int ReqQryRpcCuacctMaxDrawForTencentPay(CReqStkQryRpcCuacctMaxDrawForTencentPayField *p_pReqField, LONGLONG p_llRequestId);
  // 取证券余额及当前最新价等,支持三方交易请求
  virtual int ReqRpcStkAvlAndLatestPrice(CReqStkRpcStkAvlAndLatestPriceField *p_pReqField, LONGLONG p_llRequestId);
  // 取最大交易数量,支持三方交易请求
  virtual int ReqRpcMaxStkQty(CReqStkRpcMaxStkQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 买卖委托业务,支持三方交易请求
  virtual int ReqRpcOrder(CReqStkRpcOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单请求
  virtual int ReqRpcOrderCancel(CReqStkRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单查询请求
  virtual int ReqQryRpcOrderCancel(CReqStkQryRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 重置成本请求
  virtual int ReqRpcResetCost(CReqStkRpcResetCostField *p_pReqField, LONGLONG p_llRequestId);
  // 盈亏费用计算请求
  virtual int ReqRpcCalcFee(CReqStkRpcCalcFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 委托买卖业务,支持三方交易（新增外围调用）请求
  virtual int ReqRpcOrderForTencentPay(CReqStkRpcOrderForTencentPayField *p_pReqField, LONGLONG p_llRequestId);
  // 资管份额转让T+0委托请求
  virtual int ReqRpcTransferCapitalManagementShare(CReqStkRpcTransferCapitalManagementShareField *p_pReqField, LONGLONG p_llRequestId);
  // 放弃配售缴款, 支持三方交易请求
  virtual int ReqRpcCancelRation(CReqStkRpcCancelRationField *p_pReqField, LONGLONG p_llRequestId);
  // 放弃配售缴款, 支持三方交易请求
  virtual int ReqQryRpcCancelRation(CReqStkQryRpcCancelRationField *p_pReqField, LONGLONG p_llRequestId);
  // 客户摇号查询请求
  virtual int ReqQryRpcCustLottery(CReqStkQryRpcCustLotteryField *p_pReqField, LONGLONG p_llRequestId);
  // 客户从外围解冻中签预冻结资金请求
  virtual int ReqRpcUnfreezePreFrozenFunds(CReqStkRpcUnfreezePreFrozenFundsField *p_pReqField, LONGLONG p_llRequestId);
  // 产品信息查询请求
  virtual int ReqQryRpcStkPledgeProductInfo(CReqStkQryRpcStkPledgeProductInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 可质押标的证券查询请求
  virtual int ReqQryRpcStkPledgeNegotiableSecurity(CReqStkQryRpcStkPledgeNegotiableSecurityField *p_pReqField, LONGLONG p_llRequestId);
  // 客户质押标的折算率查询请求
  virtual int ReqQryRpcStkPledgePledgeConversionRate(CReqStkQryRpcStkPledgePledgeConversionRateField *p_pReqField, LONGLONG p_llRequestId);
  // 客户贷款额度信息查询请求
  virtual int ReqQryRpcStkPledgeLoanQuota(CReqStkQryRpcStkPledgeLoanQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 可质押证券持仓查询请求
  virtual int ReqQryRpcStkPledgeNegotiableSecurityPosi(CReqStkQryRpcStkPledgeNegotiableSecurityPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 最大可贷款金额查询请求
  virtual int ReqQryRpcStkPledgeMaxLoanAmt(CReqStkQryRpcStkPledgeMaxLoanAmtField *p_pReqField, LONGLONG p_llRequestId);
  // 贷款申请请求
  virtual int ReqRpcStkPledgeLoanApplication(CReqStkRpcStkPledgeLoanApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 还款申请请求
  virtual int ReqRpcStkPledgeRepaymentApplication(CReqStkRpcStkPledgeRepaymentApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 补充证券质押物申请请求
  virtual int ReqRpcStkPledgeSupplementaryPledgeSecurityApplication(CReqStkRpcStkPledgeSupplementaryPledgeSecurityApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 合约还款日延期申请请求
  virtual int ReqRpcStkPledgeDeferredContractRepaymentApplication(CReqStkRpcStkPledgeDeferredContractRepaymentApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 合约需补充质押数量查询请求
  virtual int ReqQryRpcStkPledgeInsufficientPledge(CReqStkQryRpcStkPledgeInsufficientPledgeField *p_pReqField, LONGLONG p_llRequestId);
  // 业务申请情况查询请求
  virtual int ReqQryRpcStkPledgeBizApplicationStatus(CReqStkQryRpcStkPledgeBizApplicationStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 贷款合约查询请求
  virtual int ReqQryRpcStkPledgeLoanContract(CReqStkQryRpcStkPledgeLoanContractField *p_pReqField, LONGLONG p_llRequestId);
  // 贷款合约质押明细查询请求
  virtual int ReqQryRpcStkPledgeLoanContractPledgeDetail(CReqStkQryRpcStkPledgeLoanContractPledgeDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押额度修改请求
  virtual int ReqRpcStkPledgeChgStkPledgeQuota(CReqStkRpcStkPledgeChgStkPledgeQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 部分还款申请请求
  virtual int ReqRpcStkPledgePartialRepaymentApplication(CReqStkRpcStkPledgePartialRepaymentApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 出借人额度查询请求
  virtual int ReqQryRpcStkPledgeLenderQuota(CReqStkQryRpcStkPledgeLenderQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 查询合约违约状态为追保的合约请求
  virtual int ReqQryRpcStkPledgeAdditionalMarginContract(CReqStkQryRpcStkPledgeAdditionalMarginContractField *p_pReqField, LONGLONG p_llRequestId);
  // 查询追保合约需要追保的数量请求
  virtual int ReqQryRpcStkPledgeAdditionalMarginQty(CReqStkQryRpcStkPledgeAdditionalMarginQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 贷款本金偿还请求
  virtual int ReqRpcStkPledgeLoanPrincipalRepayment(CReqStkRpcStkPledgeLoanPrincipalRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // 可用于还款资金查询请求
  virtual int ReqQryRpcStkPledgeAvailableQtyForRepayment(CReqStkQryRpcStkPledgeAvailableQtyForRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // 股份部分解除质押申请请求
  virtual int ReqRpcStkPledgePartialCancellationPledgeApplication(CReqStkRpcStkPledgePartialCancellationPledgeApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 客户个性化设置利率查询请求
  virtual int ReqQryRpcStkPledgePersonalizedInterestRate(CReqStkQryRpcStkPledgePersonalizedInterestRateField *p_pReqField, LONGLONG p_llRequestId);
  // 合约利息偿还申请请求
  virtual int ReqRpcStkPledgeRepaymentContractInterestApplication(CReqStkRpcStkPledgeRepaymentContractInterestApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 约定购回客户额度查询请求
  virtual int ReqQryRpcAgreementRepurchaseDebtContract(CReqStkQryRpcAgreementRepurchaseDebtContractField *p_pReqField, LONGLONG p_llRequestId);
  // 约定购回客户额度查询请求
  virtual int ReqQryRpcAgreementRepurchaseQuota(CReqStkQryRpcAgreementRepurchaseQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 约定购回初始交易请求
  virtual int ReqRpcAgreementRepurchaseInitialTransaction(CReqStkRpcAgreementRepurchaseInitialTransactionField *p_pReqField, LONGLONG p_llRequestId);
  // 约定购回购回交易请求
  virtual int ReqRpcAgreementRepurchaseRepurchaseTransaction(CReqStkRpcAgreementRepurchaseRepurchaseTransactionField *p_pReqField, LONGLONG p_llRequestId);
  // 约定购回延期购回申请请求
  virtual int ReqRpcAgreementRepurchaseDelayedRepurchaseApplication(CReqStkRpcAgreementRepurchaseDelayedRepurchaseApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 取权证信息请求
  virtual int ReqRpcWarrantInfo(CReqStkRpcWarrantInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户自主行权委托预计所得税查询请求
  virtual int ReqQryRpcExpectedIncomeTax(CReqStkQryRpcExpectedIncomeTaxField *p_pReqField, LONGLONG p_llRequestId);
  // 合约查询(410983)请求
  virtual int ReqQryRpcContract(CReqStkQryRpcContractField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权委托查询(410989)请求
  virtual int ReqQryRpcFinancingRightCommission(CReqStkQryRpcFinancingRightCommissionField *p_pReqField, LONGLONG p_llRequestId);
  // 股权激励人员信息查询（410990）请求
  virtual int ReqQryRpcEquityIncentivePersonnelInfo(CReqStkQryRpcEquityIncentivePersonnelInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权参数信息查询（410991）请求
  virtual int ReqQryRpcFinancingExerciseParams(CReqStkQryRpcFinancingExerciseParamsField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权资产负债查询（410992）请求
  virtual int ReqQryRpcFinancingExerciseAssetsLiability(CReqStkQryRpcFinancingExerciseAssetsLiabilityField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权客户额度信息查询（410993）请求
  virtual int ReqQryRpcFinancingExerciseCustQuotaInfo(CReqStkQryRpcFinancingExerciseCustQuotaInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 直接还款(410984)请求
  virtual int ReqRpcDirectRepayment(CReqStkRpcDirectRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权业务(410985)请求
  virtual int ReqRpcFinancingExerciseBiz(CReqStkRpcFinancingExerciseBizField *p_pReqField, LONGLONG p_llRequestId);
  // 取融资行权可委托数量(410986)请求
  virtual int ReqRpcFinancingExerciseMaxOrderQty(CReqStkRpcFinancingExerciseMaxOrderQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权委托撤单(410987)请求
  virtual int ReqRpcFinancingExerciseCancel(CReqStkRpcFinancingExerciseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权担保证券查询(410988)请求
  virtual int ReqQryRpcFinancingExerciseGuaranteedSecurityInfo(CReqStkQryRpcFinancingExerciseGuaranteedSecurityInfoField *p_pReqField, LONGLONG p_llRequestId);
  // ETF成份股清单信息查询请求
  virtual int ReqQryRpcETFStkList(CReqStkQryRpcETFStkListField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请（现金认购，股票认购）请求
  virtual int ReqRpcETFOfflineApplication(CReqStkRpcETFOfflineApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请撤单请求
  virtual int ReqRpcETFOfflineApplicationCancel(CReqStkRpcETFOfflineApplicationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请查询请求
  virtual int ReqQryRpcETFOfflineApplication(CReqStkQryRpcETFOfflineApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳LOF基金分拆合并信息查询请求
  virtual int ReqQryRpcSzLOFFundSplitMerge(CReqStkQryRpcSzLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // 上海LOF基金分拆合并信息查询请求
  virtual int ReqQryRpcShLOFFundSplitMerge(CReqStkQryRpcShLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // 盘后基金分拆/合并根据昨日申购额得到的建议值请求
  virtual int ReqRpcAfterCloseFundSplitMergeRecommendedValue(CReqStkRpcAfterCloseFundSplitMergeRecommendedValueField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户国债预发行持仓明细请求
  virtual int ReqQryRpcTreasuryBondsPreIssuePosi(CReqStkQryRpcTreasuryBondsPreIssuePosiField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户国债预发行平仓明细请求
  virtual int ReqQryRpcTreasuryBondsPreIssueClosePosi(CReqStkQryRpcTreasuryBondsPreIssueClosePosiField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户国债预发行应付履约金明细请求
  virtual int ReqQryRpcTreasuryBondsPreIssuePerformanceBond(CReqStkQryRpcTreasuryBondsPreIssuePerformanceBondField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票股东大会信息查询(440001)请求
  virtual int ReqQryRpcNetworkVotingTrdacctsConferenceInfo(CReqStkQryRpcNetworkVotingTrdacctsConferenceInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票议案信息查询(440002)请求
  virtual int ReqQryRpcNetworkVotingMotionInfo(CReqStkQryRpcNetworkVotingMotionInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票结果查询(440003)请求
  virtual int ReqQryRpcNetworkVotingResult(CReqStkQryRpcNetworkVotingResultField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票(440101)请求
  virtual int ReqRpcNetworkVoting(CReqStkRpcNetworkVotingField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票委托查询(440103)请求
  virtual int ReqQryRpcNetworkVoting(CReqStkQryRpcNetworkVotingField *p_pReqField, LONGLONG p_llRequestId);
  // 标准券汇总查询请求
  virtual int ReqQryRpcStdBondSummary(CReqStkQryRpcStdBondSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 质押库标准券明细查询请求
  virtual int ReqQryRpcPledgeBankStdBondDetail(CReqStkQryRpcPledgeBankStdBondDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 回购资金占用天数查询请求
  virtual int ReqQryRpcRepurchaseFundOccupancyDays(CReqStkQryRpcRepurchaseFundOccupancyDaysField *p_pReqField, LONGLONG p_llRequestId);
  // 未到期回购查询请求
  virtual int ReqQryRpcUndueRepurchase(CReqStkQryRpcUndueRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 客户融资回购风险查询请求
  virtual int ReqQryRpcFinancingRepurchaseRisk(CReqStkQryRpcFinancingRepurchaseRiskField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户报价回购品种信息请求
  virtual int ReqQryRpcShQuoteRepurchaseProductInfo(CReqStkQryRpcShQuoteRepurchaseProductInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 提前购回请求
  virtual int ReqRpcShQuoteRepurchaseAdvanceRepurchase(CReqStkRpcShQuoteRepurchaseAdvanceRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约请求
  virtual int ReqRpcShQuoteRepurchaseReservation(CReqStkRpcShQuoteRepurchaseReservationField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约流水查询请求
  virtual int ReqQryRpcShQuoteRepurchaseReservationStatement(CReqStkQryRpcShQuoteRepurchaseReservationStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户未到期的报价回购信息请求
  virtual int ReqQryRpcShQuoteRepurchaseUndueRepurchase(CReqStkQryRpcShQuoteRepurchaseUndueRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购对应质押物明细请求
  virtual int ReqRpcShQuoteRepurchasePledgeDetail(CReqStkRpcShQuoteRepurchasePledgeDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 上海报价回购质权信息查询请求
  virtual int ReqQryRpcShQuoteRepurchasePledgeInfo(CReqStkQryRpcShQuoteRepurchasePledgeInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购批量委托保留额度查询请求
  virtual int ReqQryRpcShQuoteRepurchaseBatchOrderRetentionLimit(CReqStkQryRpcShQuoteRepurchaseBatchOrderRetentionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购批量委托保留额度设置请求
  virtual int ReqRpcShQuoteRepurchaseSetBatchOrderRetentionLimit(CReqStkRpcShQuoteRepurchaseSetBatchOrderRetentionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购产品信息查询请求
  virtual int ReqQryRpcSzQuoteRepurchaseAdvanceRepurchase(CReqStkQryRpcSzQuoteRepurchaseAdvanceRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购预约请求
  virtual int ReqRpcSzQuoteRepurchaseReservation(CReqStkRpcSzQuoteRepurchaseReservationField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购预约取消请求
  virtual int ReqRpcSzQuoteRepurchaseReservationCancel(CReqStkRpcSzQuoteRepurchaseReservationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购预约执行请求
  virtual int ReqRpcSzQuoteRepurchaseReservationExecution(CReqStkRpcSzQuoteRepurchaseReservationExecutionField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购提前购回请求
  virtual int ReqRpcSzQuoteRepurchaseAdvanceRepurchase(CReqStkRpcSzQuoteRepurchaseAdvanceRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约流水查询请求
  virtual int ReqQryRpcSzQuoteRepurchaseReservationStatement(CReqStkQryRpcSzQuoteRepurchaseReservationStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购合约数量调整请求
  virtual int ReqRpcSzQuoteRepurchaseChgContractQty(CReqStkRpcSzQuoteRepurchaseChgContractQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购展期取消请求
  virtual int ReqRpcSzQuoteRepurchaseExtensionCancel(CReqStkRpcSzQuoteRepurchaseExtensionCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 查询深圳客户未到期的报价回购信息请求
  virtual int ReqQryRpcSzQuoteRepurchaseUndueRepurchase(CReqStkQryRpcSzQuoteRepurchaseUndueRepurchaseField *p_pReqField, LONGLONG p_llRequestId);
  // 查询深圳报价回购质押折算率请求
  virtual int ReqQryRpcSzQuoteRepurchasePledgeConversionRate(CReqStkQryRpcSzQuoteRepurchasePledgeConversionRateField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购现金质押查询请求
  virtual int ReqQryRpcSzQuoteRepurchaseCashPledge(CReqStkQryRpcSzQuoteRepurchaseCashPledgeField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳报价回购质权信息查询请求
  virtual int ReqQryRpcSzQuoteRepurchaseMortgage(CReqStkQryRpcSzQuoteRepurchaseMortgageField *p_pReqField, LONGLONG p_llRequestId);
  // 查询深圳报价回购客户特定报价请求
  virtual int ReqQryRpcSzQuoteRepurchaseSpecificOffer(CReqStkQryRpcSzQuoteRepurchaseSpecificOfferField *p_pReqField, LONGLONG p_llRequestId);
  // 客户基金投资适当性信息维护请求
  virtual int ReqRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriateness(CReqStkRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriatenessField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押物查询请求
  virtual int ReqQryRpcSzQuoteRepurchasePledge(CReqStkQryRpcSzQuoteRepurchasePledgeField *p_pReqField, LONGLONG p_llRequestId);
  // 债券协议回购委托申报请求
  virtual int ReqRpcBondAgreementRepurchaseOrder(CReqStkRpcBondAgreementRepurchaseOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 债券协议回购委托撤单申报请求
  virtual int ReqRpcBondAgreementRepurchaseCancel(CReqStkRpcBondAgreementRepurchaseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 债券协议回购委托流水查询请求
  virtual int ReqQryRpcBondAgreementRepurchaseStatement(CReqStkQryRpcBondAgreementRepurchaseStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 债券协议回购成交查询请求
  virtual int ReqQryRpcBondAgreementRepurchaseMatch(CReqStkQryRpcBondAgreementRepurchaseMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 债券协议回购合约查询请求
  virtual int ReqQryRpcBondAgreementRepurchaseContract(CReqStkQryRpcBondAgreementRepurchaseContractField *p_pReqField, LONGLONG p_llRequestId);
  // 密码服务委托申报请求
  virtual int ReqRpcPwdServiceOrder(CReqStkRpcPwdServiceOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 股东查询，查询客户对应的股东代码请求
  virtual int ReqQryRpcTrdacct(CReqStkQryRpcTrdacctField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询(含创新业务)请求
  virtual int ReqQryRpcCuacct(CReqStkQryRpcCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 资产明细查询请求
  virtual int ReqQryRpcAssetDetail(CReqStkQryRpcAssetDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 股份明细查询请求
  virtual int ReqQryRpcStkDetail(CReqStkQryRpcStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 股份汇总查询请求
  virtual int ReqQryRpcStkSummary(CReqStkQryRpcStkSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 外围资金帐户资金情况查询请求
  virtual int ReqQryRpcCuacct2(CReqStkQryRpcCuacct2Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户查询，根据股东内码，查询股东代码请求
  virtual int ReqQryRpcTrdacct2(CReqStkQryRpcTrdacct2Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户查询，根据股东代码，资金帐户查询客户代码请求
  virtual int ReqQryRpcTrdacct3(CReqStkQryRpcTrdacct3Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托查询请求
  virtual int ReqQryRpcOrderDetail(CReqStkQryRpcOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托查询(含创新业务)请求
  virtual int ReqQryRpcOrder2(CReqStkQryRpcOrder2Field *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交明细查询请求
  virtual int ReqQryRpcMatchDetail(CReqStkQryRpcMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交查询(含创新业务)请求
  virtual int ReqQryRpcMatch(CReqStkQryRpcMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交查询（微证券专用）请求
  virtual int ReqQryRpcMatchForTencentPay(CReqStkQryRpcMatchForTencentPayField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托汇总查询, 按证券代码和买卖方向请求
  virtual int ReqQryRpcHisOrderSummary(CReqStkQryRpcHisOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 成交汇总查询, 按证券代码和买卖方向请求
  virtual int ReqQryRpcHisMatchSummary(CReqStkQryRpcHisMatchSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 交割单查询，根据柜台普通对帐的摘要配置打印信息打印单据。微证券专用请求
  virtual int ReqQryRpcDeliveryForTencentPay(CReqStkQryRpcDeliveryForTencentPayField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托汇总查询,ordergroup+stkcode+bsflag请求
  virtual int ReqQryRpcOrderSummary2(CReqStkQryRpcOrderSummary2Field *p_pReqField, LONGLONG p_llRequestId);
  // 客户机构查询请求
  virtual int ReqQryRpcOrg(CReqStkQryRpcOrgField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户属性和资金帐号属性请求
  virtual int ReqQryRpcAcctInfo(CReqStkQryRpcAcctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 意向行情查询请求
  virtual int ReqQryRpcMktIntention(CReqStkQryRpcMktIntentionField *p_pReqField, LONGLONG p_llRequestId);
  // 查询营业部某一业务类别的申报席位请求
  virtual int ReqQryRpcSeat(CReqStkQryRpcSeatField *p_pReqField, LONGLONG p_llRequestId);
  // 查询资金账户信息请求
  virtual int ReqQryRpcCuacctAcctInfo(CReqStkQryRpcCuacctAcctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 非流通股份余额查询（沪市）请求
  virtual int ReqQryRpcShNonCirculatingShareBalance(CReqStkQryRpcShNonCirculatingShareBalanceField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳非流通股份余额查询请求
  virtual int ReqQryRpcSzNonCirculatingShareBalance(CReqStkQryRpcSzNonCirculatingShareBalanceField *p_pReqField, LONGLONG p_llRequestId);
  // 股息红利扣税查询请求
  virtual int ReqQryRpcDividendWithholdingTax(CReqStkQryRpcDividendWithholdingTaxField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户所属机构请求
  virtual int ReqQryRpcCustOrg(CReqStkQryRpcCustOrgField *p_pReqField, LONGLONG p_llRequestId);
  // 查询存管资金账号请求
  virtual int ReqQryRpcDepositoryCuacct(CReqStkQryRpcDepositoryCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 场内基金申赎失败查询请求
  virtual int ReqQryRpcFloorFundRedemptionFailure(CReqStkQryRpcFloorFundRedemptionFailureField *p_pReqField, LONGLONG p_llRequestId);
  // 黑名单证券查询请求
  virtual int ReqQryRpcBlacklistStk(CReqStkQryRpcBlacklistStkField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳大宗交易行情查询请求
  virtual int ReqQryRpcSzBlockTradingMkt(CReqStkQryRpcSzBlockTradingMktField *p_pReqField, LONGLONG p_llRequestId);
  // 上海大宗交易行情查询请求
  virtual int ReqQryRpcShBlockTradingMkt(CReqStkQryRpcShBlockTradingMktField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳场内基金分红方式查询请求
  virtual int ReqQryRpcSzFloorFundDividends(CReqStkQryRpcSzFloorFundDividendsField *p_pReqField, LONGLONG p_llRequestId);
  // 受限股东减持额度查询请求
  virtual int ReqQryRpcLimitedTrdacctReductionLimit(CReqStkQryRpcLimitedTrdacctReductionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通资金资产查询请求
  virtual int ReqQryRpcHkCuacct(CReqStkQryRpcHkCuacctField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通客户负债查询请求
  virtual int ReqQryRpcHkLiability(CReqStkQryRpcHkLiabilityField *p_pReqField, LONGLONG p_llRequestId);
  // 权益查询请求
  virtual int ReqQryRpcHkEquity(CReqStkQryRpcHkEquityField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通开户准备请求
  virtual int ReqRpcHkPreparationForOpeningAccount(CReqStkRpcHkPreparationForOpeningAccountField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通未交收明细查询请求
  virtual int ReqQryRpcHkUndeliveryDetail(CReqStkQryRpcHkUndeliveryDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通证券组合费交收明细请求
  virtual int ReqRpcHkStkCombinationFeeDeliveryDetail(CReqStkRpcHkStkCombinationFeeDeliveryDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交割单查询请求
  virtual int ReqQryRpcHkDelivery(CReqStkQryRpcHkDeliveryField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通对帐单查询请求
  virtual int ReqQryRpcHkStatement(CReqStkQryRpcHkStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通最小价差查询请求
  virtual int ReqQryRpcHkMinSpread(CReqStkQryRpcHkMinSpreadField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通参考汇率查询请求
  virtual int ReqQryRpcHkExchangeRate(CReqStkQryRpcHkExchangeRateField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通市场交易状态查询请求
  virtual int ReqQryRpcHkMarketStatus(CReqStkQryRpcHkMarketStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通标的证券查询请求
  virtual int ReqQryRpcHkUnderlyingSecurity(CReqStkQryRpcHkUnderlyingSecurityField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易日历查询请求
  virtual int ReqQryRpcHkCalendar(CReqStkQryRpcHkCalendarField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通委托买卖交易请求
  virtual int ReqRpcHkOrder(CReqStkRpcHkOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通撤单委托请求
  virtual int ReqRpcHkCancel(CReqStkRpcHkCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单查询请求
  virtual int ReqQryRpcHkCancel(CReqStkQryRpcHkCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通最大可交易数量查询请求
  virtual int ReqQryRpcHkMaxTradeQty(CReqStkQryRpcHkMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易委托明细查询请求
  virtual int ReqQryRpcHkOrderDetail(CReqStkQryRpcHkOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易成交查询请求
  virtual int ReqQryRpcHkMatch(CReqStkQryRpcHkMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易历史委托查询请求
  virtual int ReqQryRpcHkHisOrder(CReqStkQryRpcHkHisOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易成交历史查询请求
  virtual int ReqQryRpcHkHisMatch(CReqStkQryRpcHkHisMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 股转系统证券委托申报请求
  virtual int ReqRpcStkTranSysStkOrder(CReqStkRpcStkTranSysStkOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 股转系统协议转让行情查询请求
  virtual int ReqQryRpcStkTranSysAgreementTransferMkt(CReqStkQryRpcStkTranSysAgreementTransferMktField *p_pReqField, LONGLONG p_llRequestId);
  // 股转系统取最大交易数量,支持三方交易请求
  virtual int ReqRpcStkTranSysMaxTradeQty(CReqStkRpcStkTranSysMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 股转系统受限投资者查询请求
  virtual int ReqQryRpcStkTranSysRestrictedInvestor(CReqStkQryRpcStkTranSysRestrictedInvestorField *p_pReqField, LONGLONG p_llRequestId);
  // 股转市场非流通股份余额查询请求
  virtual int ReqQryRpcStkTranSysNonCirculatingSharesBalance(CReqStkQryRpcStkTranSysNonCirculatingSharesBalanceField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台交易委托明细查询请求
  virtual int ReqQryRpcFixedIncomeSysOrderDetail(CReqStkQryRpcFixedIncomeSysOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台交易历史委托明细查询请求
  virtual int ReqQryRpcFixedIncomeSysHisOrderDetail(CReqStkQryRpcFixedIncomeSysHisOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台交易成交查询请求
  virtual int ReqQryRpcFixedIncomeSysMatch(CReqStkQryRpcFixedIncomeSysMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台最大可交易数量查询请求
  virtual int ReqQryRpcFixedIncomeSysMaxTradeQty(CReqStkQryRpcFixedIncomeSysMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台委托买卖交易请求
  virtual int ReqRpcFixedIncomeSysOrder(CReqStkRpcFixedIncomeSysOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 固收平台撤单委托请求
  virtual int ReqRpcFixedIncomeSysCancel(CReqStkRpcFixedIncomeSysCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单查询请求
  virtual int ReqQryRpcFixedIncomeSysCancel(CReqStkQryRpcFixedIncomeSysCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 债券基本信息查询请求
  virtual int ReqQryRpcBondBasicInfo(CReqStkQryRpcBondBasicInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台债券行情查询请求
  virtual int ReqQryRpcBondMkt(CReqStkQryRpcBondMktField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品份额转让撤单请求
  virtual int ReqRpcFinancialProductShareTransferCancel(CReqStkRpcFinancialProductShareTransferCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品份额转让业务请求
  virtual int ReqRpcFinancialProductShareTransfer(CReqStkRpcFinancialProductShareTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品转让联系人信息查询请求
  virtual int ReqQryRpcFinancialProductShareTransferContactInfo(CReqStkQryRpcFinancialProductShareTransferContactInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品份额转让费用计算请求
  virtual int ReqRpcFinancialProductShareTransferFee(CReqStkRpcFinancialProductShareTransferFeeField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台债券参数信息查询请求
  virtual int ReqQryRpcBondParamInfo(CReqStkQryRpcBondParamInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 意向委托流水查询请求
  virtual int ReqQryRpcBondIntentionOrderStatement(CReqStkQryRpcBondIntentionOrderStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品行情查询请求
  virtual int ReqQryRpcFinancialProductMarket(CReqStkQryRpcFinancialProductMarketField *p_pReqField, LONGLONG p_llRequestId);
  // 柜台理财产品委托查询请求
  virtual int ReqQryRpcFinancialProductOrder(CReqStkQryRpcFinancialProductOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 证券出借委托外围接口请求
  virtual int ReqRpcRefinancingStkLendingOrder(CReqStkRpcRefinancingStkLendingOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 证券出借委托撤单外围接口请求
  virtual int ReqRpcRefinancingStkLendingCancel(CReqStkRpcRefinancingStkLendingCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 委托查询外围接口请求
  virtual int ReqQryRpcRefinancingOrder(CReqStkQryRpcRefinancingOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 提前索还外围接口请求
  virtual int ReqRpcRefinancingAdvanceRevert(CReqStkRpcRefinancingAdvanceRevertField *p_pReqField, LONGLONG p_llRequestId);
  // 同意展期外围接口请求
  virtual int ReqRpcRefinancingExtentionApprove(CReqStkRpcRefinancingExtentionApproveField *p_pReqField, LONGLONG p_llRequestId);
  // 证券出借可撤记录查询外围接口请求
  virtual int ReqQryRpcRefinancingStkLendingCanWithdraw(CReqStkQryRpcRefinancingStkLendingCanWithdrawField *p_pReqField, LONGLONG p_llRequestId);
  // 转融券未了结合约查询外围接口请求
  virtual int ReqQryRpcRefinancingOpenContract(CReqStkQryRpcRefinancingOpenContractField *p_pReqField, LONGLONG p_llRequestId);
  // 可出借标的券查询外围接口请求
  virtual int ReqQryRpcRefinancingUnderlyingStkForLending(CReqStkQryRpcRefinancingUnderlyingStkForLendingField *p_pReqField, LONGLONG p_llRequestId);
  // 出借人信息查询外围接口请求
  virtual int ReqQryRpcRefinancingLenderInfo(CReqStkQryRpcRefinancingLenderInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 历史出借委托查询请求
  virtual int ReqQryRpcRefinancingHisLendingOrder(CReqStkQryRpcRefinancingHisLendingOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 证券出借已了结合约查询请求
  virtual int ReqQryRpcRefinancingStkLendingClosedContract(CReqStkQryRpcRefinancingStkLendingClosedContractField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户的预留信息请求
  virtual int ReqQryRpcSysSecurityCustReservedInfo(CReqStkQryRpcSysSecurityCustReservedInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 设置客户预留信息请求
  virtual int ReqRpcSysSecuritySetCustReservedInfo(CReqStkRpcSysSecuritySetCustReservedInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 同步客户动态令牌请求
  virtual int ReqRpcSysSecuritySyncDynamicToken(CReqStkRpcSysSecuritySyncDynamicTokenField *p_pReqField, LONGLONG p_llRequestId);
  // 客户证书获取随机码请求
  virtual int ReqRpcSysSecurityCertificateRandomCode(CReqStkRpcSysSecurityCertificateRandomCodeField *p_pReqField, LONGLONG p_llRequestId);
  // 客户动态令牌状态设置请求
  virtual int ReqRpcSysSecuritySetDynamicTokenStatus(CReqStkRpcSysSecuritySetDynamicTokenStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 客户获取一次性动态口令请求
  virtual int ReqRpcSysSecurityOneTimeDynamicPwd(CReqStkRpcSysSecurityOneTimeDynamicPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 客户登陆绑定信息设置请求
  virtual int ReqRpcSysSecuritySetLoginBindingInfo(CReqStkRpcSysSecuritySetLoginBindingInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户登陆绑定信息查询请求
  virtual int ReqQryRpcSysSecurityLoginBindingInfo(CReqStkQryRpcSysSecurityLoginBindingInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 校验客户交易密码请求
  virtual int ReqRpcSysSecurityVerifyTrdPwd(CReqStkRpcSysSecurityVerifyTrdPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 客户获取手机一次性动态口令请求
  virtual int ReqRpcSysSecurityMobileOneTimeDynamicPwd(CReqStkRpcSysSecurityMobileOneTimeDynamicPwdField *p_pReqField, LONGLONG p_llRequestId);
  // 查询客户ETF篮各成分股的持仓数量请求
  virtual int ReqQryRpcETFConstituentStkPosi(CReqStkQryRpcETFConstituentStkPosiField *p_pReqField, LONGLONG p_llRequestId);
  // 普通节点和VIP系统间证券划拨请求
  virtual int ReqRpcCommVIPStkTransfer(CReqStkRpcCommVIPStkTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 现金资产转银行请求
  virtual int ReqRpcCashAssetBankTransfer(CReqStkRpcCashAssetBankTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 资金划拨流水查询请求
  virtual int ReqQryRpcCapitalTransferStatement(CReqStkQryRpcCapitalTransferStatementField *p_pReqField, LONGLONG p_llRequestId);
  // Vip客户资金资产同步表查询请求
  virtual int ReqQryRpcVIPCuacctSyncTable(CReqStkQryRpcVIPCuacctSyncTableField *p_pReqField, LONGLONG p_llRequestId);
  // 场内产品适当性信息查询请求
  virtual int ReqQryRpcFloorTradingProductAppropriatenessInfo(CReqStkQryRpcFloorTradingProductAppropriatenessInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户适当性信息查询请求
  virtual int ReqQryRpcAppropriatenessInfo(CReqStkQryRpcAppropriatenessInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 客户与场内产品适当性匹配校验请求
  virtual int ReqRpcAppropriatenessMatchingCheck(CReqStkRpcAppropriatenessMatchingCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 记录场内业务风险揭示书签署流水请求
  virtual int ReqRpcFloorTradingRiskDisclosureStatementSignedRecordFlow(CReqStkRpcFloorTradingRiskDisclosureStatementSignedRecordFlowField *p_pReqField, LONGLONG p_llRequestId);
  // 系统探测请求
  virtual int ReqBizStatus(CReqStkBizStatusField *p_pReqField, LONGLONG p_llRequestId);
  // 质押股份明细查询(单客户)请求
  virtual int ReqQryPledgeShares(CReqStkQryPledgeSharesField *p_pReqField, LONGLONG p_llRequestId);
  // 资金流水查询请求
  virtual int ReqQryFundFlow(CReqStkQryFundFlowField *p_pReqField, LONGLONG p_llRequestId);
  // 买卖委托请求
  virtual int ReqOrderEx(CReqStkOrderExField *p_pReqField, LONGLONG p_llRequestId);
  // 股份流水查询请求
  virtual int ReqQryStockFlow(CReqStkQryStockFlowField *p_pReqField, LONGLONG p_llRequestId);
  // 日切交易日查询请求
  virtual int ReqQrySettDate(CReqStkQrySettDateField *p_pReqField, LONGLONG p_llRequestId);
  // 盘后基金行情查询请求
  virtual int ReqQryLofMktInfo(CReqStkQryLofMktInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 资金划拨(集中交易)请求
  virtual int ReqFundTransferJzjy(CReqStkFundTransferJzjyField *p_pReqField, LONGLONG p_llRequestId);
  // 可用资金查询(集中交易)请求
  virtual int ReqQryExpendableFundJzjy(CReqStkQryExpendableFundJzjyField *p_pReqField, LONGLONG p_llRequestId);
  // 认证信息设置(密码录入/修改)请求
  virtual int ReqAuthDataSetJzjy(CReqStkAuthDataSetJzjyField *p_pReqField, LONGLONG p_llRequestId);
  // 资金划拨(单账户多节点) 订单节点之间划拨请求
  virtual int ReqFundTransferNode(CReqStkFundTransferNodeField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转账请求
  virtual int ReqBankStkTransfer(CReqStkBankStkTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 要约收购收购人信息查询请求
  virtual int ReqQryTenderSgrInfo(CReqStkQryTenderSgrInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 盘后行情查询(创业板)请求
  virtual int ReqQryClosingMktSecondboard(CReqStkQryClosingMktSecondboardField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请撤单请求
  virtual int ReqEtfOfflineCancelOrder(CReqStkEtfOfflineCancelOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请查询请求
  virtual int ReqQryEtfOfflineOrder(CReqStkQryEtfOfflineOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 新股日历查询请求
  virtual int ReqQryNewStkCal(CReqStkQryNewStkCalField *p_pReqField, LONGLONG p_llRequestId);
  // 共享内存品种信息查询请求
  virtual int ReqQryStkInfoShm(CReqStkQryStkInfoShmField *p_pReqField, LONGLONG p_llRequestId);
  // 债券竞买业务预约委托撤单请求
  virtual int ReqReserveOrderCancel(CReqStkReserveOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳债券报价行情查询请求
  virtual int ReqQryBondQuote(CReqStkQryBondQuoteField *p_pReqField, LONGLONG p_llRequestId);
  // 协商成交回报行情信息查询请求
  virtual int ReqQryReportMktInfo(CReqStkQryReportMktInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 债券现券交易竞买预约委托信息查询请求
  virtual int ReqQryCashBondReserveOrderInfo(CReqStkQryCashBondReserveOrderInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 委托汇总查询请求
  virtual int ReqQryOrderSummary(CReqStkQryOrderSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 成交汇总查询请求
  virtual int ReqQryMatchSummary(CReqStkQryMatchSummaryField *p_pReqField, LONGLONG p_llRequestId);
  // 债券现券交易竞买预约信息查询请求
  virtual int ReqQryCashBondTradeReservationInfo(CReqStkQryCashBondTradeReservationInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 债券现券交易业务参考信息查询请求
  virtual int ReqQryCashBondTradeProfitInfo(CReqStkQryCashBondTradeProfitInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 债券通用质押式回购业务参考信息查询请求
  virtual int ReqQryBondPledgeRepchProfitInfo(CReqStkQryBondPledgeRepchProfitInfoField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // 证券信息查询响应[快速]
  void OnRspQryStkInfoEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 证券信息查询响应
  void OnRspQryStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 盘后行情查询(上海科创板)响应
  void OnRspQryClosingMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // LOF信息查询
  void OnRspQryLofInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ETF信息查询
  void OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // ETF篮信息查询
  void OnRspQryEtfBasket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

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
  void OnRspQryEtfCotPosition(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通汇率信息查询响应
  void OnRspQryRateInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通市场状态信息查询响应
  void OnRspQryMktStatusInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通标的证券状态查询响应
  void OnRspQryUndlStkStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通二级资金台账查询响应
  void OnRspQryFundAcctLvlTwo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通交易最小价差查询响应
  void OnRspQryTradeMinSpread(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 港股通客户负债查询响应
  //void OnRspQryCustDebts(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 回购合约查询响应
  void OnRspQryRepchContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 客户适当性查询响应
  void OnRspQryCustAppropriate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 产品适当性查询响应
  void OnRspQryProdAppropriate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 适当性匹配检查响应
  void OnRspAppropriateCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 适当性匹配结果记录响应
  void OnRspAppropriateResult(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 回购资金占用天数查询响应
  void OnRspQryRepchFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 用户登录请求响应
  void OnRspUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 历史委托查询响应
  void OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 历史成交查询响应
  void OnRspQryHisFill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日/历史委托查询响应
  void OnRspQryCurrDayHisOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 当日/历史成交查询响应
  void OnRspQryCurrDayHisFill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  //转帐银行业务信息 2015/8/12
  void OnRspQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //银证转帐       2015/8/12
  void OnRspBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //查询账户余额   2015/8/12
  void OnRspQryFundBala(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //银证转帐查询   2015/8/12
  void OnRspQryBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //修改交易密码   2015/8/12
  void OnRspChgTrdPwd(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //修改资金密码   2015/8/12
  void OnRspChgFundPwd(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //对帐单查询     2015/8/12
  void OnRspQryStatement(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  //资金划拨
  void OnRspQryFundTransfer(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 证券市值额度查询响应
  void OnRspQryMktQuota(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 上证LOF信息查询响应
  void OnRspQryScLofInfo(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);
  
  // 历史委托明细查询响应
  void OnRspQryRpcHistoryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交明细查询响应
  void OnRspQryRpcHistoryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日新股清单查询响应
  void OnRspQryRpcIPO(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围新股申购市值额度表查询响应
  void OnRspQryRpcIPOSubscriptionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 配号查询响应
  void OnRspQryRpcDistribution(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 市值配售中签查询响应
  void OnRspQryRpcMktCapPlacing(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 中签历史查询响应
  void OnRspQryRpcWinningHistory(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 市值配售交收顺序修改响应
  void OnRspRpcChgMktCapDistDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 交割单查询响应
  void OnRspQryRpcDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 对帐查询响应
  void OnRspQryRpcChkAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取开通转帐(银行)账号查询响应
  void OnRspQryRpcBankAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取开通外部(银行)账号查询响应
  void OnRspQryRpcOtherBankAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询响应
  void OnRspQryRpcFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日资金流水查询响应
  void OnRspQryRpcFundDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托汇总(410514)响应
  void OnRspQryRpcOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交汇总(410516)响应
  void OnRspQryRpcMatchSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询当前系统状态响应
  void OnRspQryRpcCurSysStat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取字典信息响应
  void OnRspRpcDictInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券信息响应
  void OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询机构信息响应
  void OnRspQryRpcOrgInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证通转帐银行业务信息响应
  void OnRspRpcBankStkTransferBankBizInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户登陆响应
  void OnRspRpcLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户总资产查询(新)响应
  void OnRspQryRpcTotalAssets(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改联系方式（网上交易使用）响应
  void OnRspRpcChgContact(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户资料查询响应
  void OnRspQryRpcCustInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改操作方式响应
  void OnRspRpcChgOperMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户开通的操作方式响应
  void OnRspQryRpcCustOperMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改客户联系人信息响应
  void OnRspRpcChgCustInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户的扩展属性响应
  void OnRspRpcSetCustAttrEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置数字签名响应
  void OnRspRpcSetDigitalSign(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围修改资金帐号权限响应
  void OnRspRpcChgCuacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金帐号开通资金权限查询响应
  void OnRspQryRpcCuacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券帐号开通权限查询响应
  void OnRspQryRpcTrdacctPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户权限有效期信息查询响应
  void OnRspQryRpcCustPermissionValidPeriod(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围激活客户权限响应
  void OnRspRpcActivateCustPermission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户特殊手续费查询响应
  void OnRspQryRpcCustSpecialFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 一次性服务佣金收取响应
  void OnRspRpcOneTimeServiceCommission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 一次性服务佣金撤单响应
  void OnRspRpcCancelOneTimeServiceCommission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固定期限服务佣金办理响应
  void OnRspRpcFixedTermServiceCommission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固定期限服务佣金退款响应
  void OnRspRpcFixedTermServiceCommissionRefund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 一次性服务佣金设置查询响应
  void OnRspQryRpcOneTimeServiceCommissionSetting(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 一次性服务佣金收取查询响应
  void OnRspQryRpcOneTimeServiceCommissions(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询咨询付费类别响应
  void OnRspQryRpcConsultationPaymentCategory(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 增加咨询付费类别响应
  void OnRspRpcAddConsultationPaymentCategory(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 维护客户订阅状态响应
  void OnRspRpcMaintainCustSubStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户订阅流水响应
  void OnRspQryRpcCustSubStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户订阅设置响应
  void OnRspRpcCustSubSetting(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 第三方收费结构响应
  void OnRspRpcThirdPartyChargingStructure(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 投资咨询产品费用保证金支付响应
  void OnRspRpcInvestmentAdvisoryProductFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 密码校验响应
  void OnRspRpcCuacctPwdCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金资料查询响应
  void OnRspQryRpcCuacctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取客户资料核查状态响应
  void OnRspRpcCustInfoCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户登录校验信息响应
  void OnRspRpcSetLoginCheckMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户资料核查标识响应
  void OnRspRpcSetCustInfoCheckMode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户消息状态响应
  void OnRspRpcSetCustMsgStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户消息服务响应
  void OnRspQryRpcCustMsgService(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 修改银行密码响应
  void OnRspRpcChgBankPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金台帐间转帐响应
  void OnRspRpcTransferBetweenCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围客户交易帐户资金归集响应
  void OnRspRpcCuacctCashConcentration(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取同一客户交易台帐、转帐台帐响应
  void OnRspRpcCuacctInfo2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金可取数查询响应
  void OnRspQryRpcCuacctMaxDraw(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户可取资金查询响应
  void OnRspQryRpcCuacctMaxDrawForTencentPay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取证券余额及当前最新价等,支持三方交易响应
  void OnRspRpcStkAvlAndLatestPrice(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取最大交易数量,支持三方交易响应
  void OnRspRpcMaxStkQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 买卖委托业务,支持三方交易响应
  void OnRspRpcOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单响应
  void OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单查询响应
  void OnRspQryRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 重置成本响应
  void OnRspRpcResetCost(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盈亏费用计算响应
  void OnRspRpcCalcFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托买卖业务,支持三方交易（新增外围调用）响应
  void OnRspRpcOrderForTencentPay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资管份额转让T+0委托响应
  void OnRspRpcTransferCapitalManagementShare(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 放弃配售缴款, 支持三方交易响应
  void OnRspRpcCancelRation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 放弃配售缴款, 支持三方交易响应
  void OnRspQryRpcCancelRation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户摇号查询响应
  void OnRspQryRpcCustLottery(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户从外围解冻中签预冻结资金响应
  void OnRspRpcUnfreezePreFrozenFunds(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 产品信息查询响应
  void OnRspQryRpcStkPledgeProductInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可质押标的证券查询响应
  void OnRspQryRpcStkPledgeNegotiableSecurity(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户质押标的折算率查询响应
  void OnRspQryRpcStkPledgePledgeConversionRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户贷款额度信息查询响应
  void OnRspQryRpcStkPledgeLoanQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可质押证券持仓查询响应
  void OnRspQryRpcStkPledgeNegotiableSecurityPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 最大可贷款金额查询响应
  void OnRspQryRpcStkPledgeMaxLoanAmt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 贷款申请响应
  void OnRspRpcStkPledgeLoanApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 还款申请响应
  void OnRspRpcStkPledgeRepaymentApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 补充证券质押物申请响应
  void OnRspRpcStkPledgeSupplementaryPledgeSecurityApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约还款日延期申请响应
  void OnRspRpcStkPledgeDeferredContractRepaymentApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约需补充质押数量查询响应
  void OnRspQryRpcStkPledgeInsufficientPledge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 业务申请情况查询响应
  void OnRspQryRpcStkPledgeBizApplicationStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 贷款合约查询响应
  void OnRspQryRpcStkPledgeLoanContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 贷款合约质押明细查询响应
  void OnRspQryRpcStkPledgeLoanContractPledgeDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押额度修改响应
  void OnRspRpcStkPledgeChgStkPledgeQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 部分还款申请响应
  void OnRspRpcStkPledgePartialRepaymentApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 出借人额度查询响应
  void OnRspQryRpcStkPledgeLenderQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询合约违约状态为追保的合约响应
  void OnRspQryRpcStkPledgeAdditionalMarginContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询追保合约需要追保的数量响应
  void OnRspQryRpcStkPledgeAdditionalMarginQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 贷款本金偿还响应
  void OnRspRpcStkPledgeLoanPrincipalRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可用于还款资金查询响应
  void OnRspQryRpcStkPledgeAvailableQtyForRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份部分解除质押申请响应
  void OnRspRpcStkPledgePartialCancellationPledgeApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户个性化设置利率查询响应
  void OnRspQryRpcStkPledgePersonalizedInterestRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约利息偿还申请响应
  void OnRspRpcStkPledgeRepaymentContractInterestApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 约定购回客户额度查询响应
  void OnRspQryRpcAgreementRepurchaseDebtContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 约定购回客户额度查询响应
  void OnRspQryRpcAgreementRepurchaseQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 约定购回初始交易响应
  void OnRspRpcAgreementRepurchaseInitialTransaction(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 约定购回购回交易响应
  void OnRspRpcAgreementRepurchaseRepurchaseTransaction(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 约定购回延期购回申请响应
  void OnRspRpcAgreementRepurchaseDelayedRepurchaseApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取权证信息响应
  void OnRspRpcWarrantInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户自主行权委托预计所得税查询响应
  void OnRspQryRpcExpectedIncomeTax(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 合约查询(410983)响应
  void OnRspQryRpcContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权委托查询(410989)响应
  void OnRspQryRpcFinancingRightCommission(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股权激励人员信息查询（410990）响应
  void OnRspQryRpcEquityIncentivePersonnelInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权参数信息查询（410991）响应
  void OnRspQryRpcFinancingExerciseParams(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权资产负债查询（410992）响应
  void OnRspQryRpcFinancingExerciseAssetsLiability(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权客户额度信息查询（410993）响应
  void OnRspQryRpcFinancingExerciseCustQuotaInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 直接还款(410984)响应
  void OnRspRpcDirectRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权业务(410985)响应
  void OnRspRpcFinancingExerciseBiz(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 取融资行权可委托数量(410986)响应
  void OnRspRpcFinancingExerciseMaxOrderQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权委托撤单(410987)响应
  void OnRspRpcFinancingExerciseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权担保证券查询(410988)响应
  void OnRspQryRpcFinancingExerciseGuaranteedSecurityInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF成份股清单信息查询响应
  void OnRspQryRpcETFStkList(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请（现金认购，股票认购）响应
  void OnRspRpcETFOfflineApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请撤单响应
  void OnRspRpcETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请查询响应
  void OnRspQryRpcETFOfflineApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳LOF基金分拆合并信息查询响应
  void OnRspQryRpcSzLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海LOF基金分拆合并信息查询响应
  void OnRspQryRpcShLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盘后基金分拆/合并根据昨日申购额得到的建议值响应
  void OnRspRpcAfterCloseFundSplitMergeRecommendedValue(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户国债预发行持仓明细响应
  void OnRspQryRpcTreasuryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户国债预发行平仓明细响应
  void OnRspQryRpcTreasuryBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户国债预发行应付履约金明细响应
  void OnRspQryRpcTreasuryBondsPreIssuePerformanceBond(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票股东大会信息查询(440001)响应
  void OnRspQryRpcNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票议案信息查询(440002)响应
  void OnRspQryRpcNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票结果查询(440003)响应
  void OnRspQryRpcNetworkVotingResult(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票(440101)响应
  void OnRspRpcNetworkVoting(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票委托查询(440103)响应
  void OnRspQryRpcNetworkVoting(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 标准券汇总查询响应
  void OnRspQryRpcStdBondSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 质押库标准券明细查询响应
  void OnRspQryRpcPledgeBankStdBondDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 回购资金占用天数查询响应
  void OnRspQryRpcRepurchaseFundOccupancyDays(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 未到期回购查询响应
  void OnRspQryRpcUndueRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户融资回购风险查询响应
  void OnRspQryRpcFinancingRepurchaseRisk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户报价回购品种信息响应
  void OnRspQryRpcShQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 提前购回响应
  void OnRspRpcShQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约响应
  void OnRspRpcShQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约流水查询响应
  void OnRspQryRpcShQuoteRepurchaseReservationStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户未到期的报价回购信息响应
  void OnRspQryRpcShQuoteRepurchaseUndueRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购对应质押物明细响应
  void OnRspRpcShQuoteRepurchasePledgeDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海报价回购质权信息查询响应
  void OnRspQryRpcShQuoteRepurchasePledgeInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购批量委托保留额度查询响应
  void OnRspQryRpcShQuoteRepurchaseBatchOrderRetentionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购批量委托保留额度设置响应
  void OnRspRpcShQuoteRepurchaseSetBatchOrderRetentionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购产品信息查询响应
  void OnRspQryRpcSzQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购预约响应
  void OnRspRpcSzQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购预约取消响应
  void OnRspRpcSzQuoteRepurchaseReservationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购预约执行响应
  void OnRspRpcSzQuoteRepurchaseReservationExecution(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购提前购回响应
  void OnRspRpcSzQuoteRepurchaseAdvanceRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约流水查询响应
  void OnRspQryRpcSzQuoteRepurchaseReservationStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购合约数量调整响应
  void OnRspRpcSzQuoteRepurchaseChgContractQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购展期取消响应
  void OnRspRpcSzQuoteRepurchaseExtensionCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询深圳客户未到期的报价回购信息响应
  void OnRspQryRpcSzQuoteRepurchaseUndueRepurchase(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询深圳报价回购质押折算率响应
  void OnRspQryRpcSzQuoteRepurchasePledgeConversionRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购现金质押查询响应
  void OnRspQryRpcSzQuoteRepurchaseCashPledge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳报价回购质权信息查询响应
  void OnRspQryRpcSzQuoteRepurchaseMortgage(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询深圳报价回购客户特定报价响应
  void OnRspQryRpcSzQuoteRepurchaseSpecificOffer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户基金投资适当性信息维护响应
  void OnRspRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriateness(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押物查询响应
  void OnRspQryRpcSzQuoteRepurchasePledge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券协议回购委托申报响应
  void OnRspRpcBondAgreementRepurchaseOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券协议回购委托撤单申报响应
  void OnRspRpcBondAgreementRepurchaseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券协议回购委托流水查询响应
  void OnRspQryRpcBondAgreementRepurchaseStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券协议回购成交查询响应
  void OnRspQryRpcBondAgreementRepurchaseMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券协议回购合约查询响应
  void OnRspQryRpcBondAgreementRepurchaseContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 密码服务委托申报响应
  void OnRspRpcPwdServiceOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东查询，查询客户对应的股东代码响应
  void OnRspQryRpcTrdacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询(含创新业务)响应
  void OnRspQryRpcCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资产明细查询响应
  void OnRspQryRpcAssetDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份明细查询响应
  void OnRspQryRpcStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份汇总查询响应
  void OnRspQryRpcStkSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 外围资金帐户资金情况查询响应
  void OnRspQryRpcCuacct2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户查询，根据股东内码，查询股东代码响应
  void OnRspQryRpcTrdacct2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户查询，根据股东代码，资金帐户查询客户代码响应
  void OnRspQryRpcTrdacct3(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托查询响应
  void OnRspQryRpcOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托查询(含创新业务)响应
  void OnRspQryRpcOrder2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交明细查询响应
  void OnRspQryRpcMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交查询(含创新业务)响应
  void OnRspQryRpcMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交查询（微证券专用）响应
  void OnRspQryRpcMatchForTencentPay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托汇总查询, 按证券代码和买卖方向响应
  void OnRspQryRpcHisOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 成交汇总查询, 按证券代码和买卖方向响应
  void OnRspQryRpcHisMatchSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 交割单查询，根据柜台普通对帐的摘要配置打印信息打印单据。微证券专用响应
  void OnRspQryRpcDeliveryForTencentPay(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托汇总查询,ordergroup+stkcode+bsflag响应
  void OnRspQryRpcOrderSummary2(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户机构查询响应
  void OnRspQryRpcOrg(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户属性和资金帐号属性响应
  void OnRspQryRpcAcctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 意向行情查询响应
  void OnRspQryRpcMktIntention(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询营业部某一业务类别的申报席位响应
  void OnRspQryRpcSeat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询资金账户信息响应
  void OnRspQryRpcCuacctAcctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 非流通股份余额查询（沪市）响应
  void OnRspQryRpcShNonCirculatingShareBalance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳非流通股份余额查询响应
  void OnRspQryRpcSzNonCirculatingShareBalance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股息红利扣税查询响应
  void OnRspQryRpcDividendWithholdingTax(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户所属机构响应
  void OnRspQryRpcCustOrg(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询存管资金账号响应
  void OnRspQryRpcDepositoryCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 场内基金申赎失败查询响应
  void OnRspQryRpcFloorFundRedemptionFailure(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 黑名单证券查询响应
  void OnRspQryRpcBlacklistStk(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳大宗交易行情查询响应
  void OnRspQryRpcSzBlockTradingMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海大宗交易行情查询响应
  void OnRspQryRpcShBlockTradingMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳场内基金分红方式查询响应
  void OnRspQryRpcSzFloorFundDividends(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 受限股东减持额度查询响应
  void OnRspQryRpcLimitedTrdacctReductionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通资金资产查询响应
  void OnRspQryRpcHkCuacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通客户负债查询响应
  void OnRspQryRpcHkLiability(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 权益查询响应
  void OnRspQryRpcHkEquity(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通开户准备响应
  void OnRspRpcHkPreparationForOpeningAccount(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通未交收明细查询响应
  void OnRspQryRpcHkUndeliveryDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通证券组合费交收明细响应
  void OnRspRpcHkStkCombinationFeeDeliveryDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交割单查询响应
  void OnRspQryRpcHkDelivery(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通对帐单查询响应
  void OnRspQryRpcHkStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通最小价差查询响应
  void OnRspQryRpcHkMinSpread(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通参考汇率查询响应
  void OnRspQryRpcHkExchangeRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通市场交易状态查询响应
  void OnRspQryRpcHkMarketStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通标的证券查询响应
  void OnRspQryRpcHkUnderlyingSecurity(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易日历查询响应
  void OnRspQryRpcHkCalendar(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通委托买卖交易响应
  void OnRspRpcHkOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通撤单委托响应
  void OnRspRpcHkCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单查询响应
  void OnRspQryRpcHkCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通最大可交易数量查询响应
  void OnRspQryRpcHkMaxTradeQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易委托明细查询响应
  void OnRspQryRpcHkOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易成交查询响应
  void OnRspQryRpcHkMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易历史委托查询响应
  void OnRspQryRpcHkHisOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易成交历史查询响应
  void OnRspQryRpcHkHisMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转系统证券委托申报响应
  void OnRspRpcStkTranSysStkOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转系统协议转让行情查询响应
  void OnRspQryRpcStkTranSysAgreementTransferMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转系统取最大交易数量,支持三方交易响应
  void OnRspRpcStkTranSysMaxTradeQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转系统受限投资者查询响应
  void OnRspQryRpcStkTranSysRestrictedInvestor(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转市场非流通股份余额查询响应
  void OnRspQryRpcStkTranSysNonCirculatingSharesBalance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台交易委托明细查询响应
  void OnRspQryRpcFixedIncomeSysOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台交易历史委托明细查询响应
  void OnRspQryRpcFixedIncomeSysHisOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台交易成交查询响应
  void OnRspQryRpcFixedIncomeSysMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台最大可交易数量查询响应
  void OnRspQryRpcFixedIncomeSysMaxTradeQty(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台委托买卖交易响应
  void OnRspRpcFixedIncomeSysOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 固收平台撤单委托响应
  void OnRspRpcFixedIncomeSysCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单查询响应
  void OnRspQryRpcFixedIncomeSysCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券基本信息查询响应
  void OnRspQryRpcBondBasicInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台债券行情查询响应
  void OnRspQryRpcBondMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品份额转让撤单响应
  void OnRspRpcFinancialProductShareTransferCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品份额转让业务响应
  void OnRspRpcFinancialProductShareTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品转让联系人信息查询响应
  void OnRspQryRpcFinancialProductShareTransferContactInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品份额转让费用计算响应
  void OnRspRpcFinancialProductShareTransferFee(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台债券参数信息查询响应
  void OnRspQryRpcBondParamInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 意向委托流水查询响应
  void OnRspQryRpcBondIntentionOrderStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品行情查询响应
  void OnRspQryRpcFinancialProductMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 柜台理财产品委托查询响应
  void OnRspQryRpcFinancialProductOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券出借委托外围接口响应
  void OnRspRpcRefinancingStkLendingOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券出借委托撤单外围接口响应
  void OnRspRpcRefinancingStkLendingCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托查询外围接口响应
  void OnRspQryRpcRefinancingOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 提前索还外围接口响应
  void OnRspRpcRefinancingAdvanceRevert(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 同意展期外围接口响应
  void OnRspRpcRefinancingExtentionApprove(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券出借可撤记录查询外围接口响应
  void OnRspQryRpcRefinancingStkLendingCanWithdraw(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 转融券未了结合约查询外围接口响应
  void OnRspQryRpcRefinancingOpenContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可出借标的券查询外围接口响应
  void OnRspQryRpcRefinancingUnderlyingStkForLending(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 出借人信息查询外围接口响应
  void OnRspQryRpcRefinancingLenderInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史出借委托查询响应
  void OnRspQryRpcRefinancingHisLendingOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券出借已了结合约查询响应
  void OnRspQryRpcRefinancingStkLendingClosedContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户的预留信息响应
  void OnRspQryRpcSysSecurityCustReservedInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 设置客户预留信息响应
  void OnRspRpcSysSecuritySetCustReservedInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 同步客户动态令牌响应
  void OnRspRpcSysSecuritySyncDynamicToken(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户证书获取随机码响应
  void OnRspRpcSysSecurityCertificateRandomCode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户动态令牌状态设置响应
  void OnRspRpcSysSecuritySetDynamicTokenStatus(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户获取一次性动态口令响应
  void OnRspRpcSysSecurityOneTimeDynamicPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户登陆绑定信息设置响应
  void OnRspRpcSysSecuritySetLoginBindingInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户登陆绑定信息查询响应
  void OnRspQryRpcSysSecurityLoginBindingInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 校验客户交易密码响应
  void OnRspRpcSysSecurityVerifyTrdPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户获取手机一次性动态口令响应
  void OnRspRpcSysSecurityMobileOneTimeDynamicPwd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询客户ETF篮各成分股的持仓数量响应
  void OnRspQryRpcETFConstituentStkPosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 普通节点和VIP系统间证券划拨响应
  void OnRspRpcCommVIPStkTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 现金资产转银行响应
  void OnRspRpcCashAssetBankTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金划拨流水查询响应
  void OnRspQryRpcCapitalTransferStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // Vip客户资金资产同步表查询响应
  void OnRspQryRpcVIPCuacctSyncTable(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 场内产品适当性信息查询响应
  void OnRspQryRpcFloorTradingProductAppropriatenessInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户适当性信息查询响应
  void OnRspQryRpcAppropriatenessInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户与场内产品适当性匹配校验响应
  void OnRspRpcAppropriatenessMatchingCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 记录场内业务风险揭示书签署流水响应
  void OnRspRpcFloorTradingRiskDisclosureStatementSignedRecordFlow(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
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
  // 盘后基金行情查询响应
  void OnRspQryLofMktInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金划拨(集中交易)响应
  void OnRspFundTransferJzjy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可用资金查询(集中交易)响应
  void OnRspQryExpendableFundJzjy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 认证信息设置(密码录入/修改)响应
  void OnRspAuthDataSetJzjy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金划拨(单账户多节点) 订单节点之间划拨响应
  void OnRspFundTransferNode(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转账响应
  void OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 要约收购收购人信息查询响应
  void OnRspQryTenderSgrInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盘后行情查询(创业板)查询响应
  void OnRspQryClosingMktSecondboard(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请撤单响应
  void OnRspEtfOfflineCancelOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请查询响应
  void OnRspQryEtfOfflineOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股日历查询响应
  void OnRspQryNewStkCal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 共享内存品种信息查询响应
  void OnRspQryStkInfoShm(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券竞买业务预约委托撤单响应
  void OnRspReserveOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳债券报价行情查询响应
  void OnRspQryBondQuote(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 协商成交回报行情信息查询响应
  void OnRspQryReportMktInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券现券交易竞买预约委托信息查询响应
  void OnRspQryCashBondReserveOrderInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托汇总查询响应
  void OnRspQryOrderSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 成交汇总查询响应
  void OnRspQryMatchSummary(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券现券交易竞买预约信息查询响应
  void OnRspQryCashBondTradeReservationInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券现券交易业务参考信息查询响应
  void OnRspQryCashBondTradeProfitInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券通用质押式回购业务参考信息查询响应
  void OnRspQryBondPledgeRepchProfitInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_STK_TRADE_API_H__