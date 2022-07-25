#if !defined(__MA_CLI_FS2_TRADE_API_H__)
#define __MA_CLI_FS2_TRADE_API_H__

#include "maCliFs2Api.h"
#include "maCliFs2TradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFs2TradeSpi : virtual public CCliFs2Spi
{
public:
  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnFsOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // 成交回报
  virtual int OnRtnOrderFill(CRtnFsOrderFillField *p_pRtnOrderFillField) {return 0;}
  // 机构信息查询响应
  virtual int OnRspQryOrgInfo(CFirstSetField *p_pFirstSetField, CRspFsQryOrgInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券信息查询响应
  virtual int OnRspQryTrdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryTrdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 字典信息查询响应
  virtual int OnRspQryDictInfo(CFirstSetField *p_pFirstSetField, CRspFsQryDictInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 系统状态查询响应
  virtual int OnRspQrySysStat(CFirstSetField *p_pFirstSetField, CRspFsQrySysStatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转账响应
  virtual int OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, CRspFsBankStkTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 券商发起查银行余额响应
  virtual int OnRspQryInitiatorOrderBankBal(CFirstSetField *p_pFirstSetField, CRspFsQryInitiatorOrderBankBalField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询银证业务流水响应
  virtual int OnRspQryBankStkDetail(CFirstSetField *p_pFirstSetField, CRspFsQryBankStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户内现金划转响应
  virtual int OnRspCustCashTransfer(CFirstSetField *p_pFirstSetField, CRspFsCustCashTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 账户资金归集响应
  virtual int OnRspQryAcctConcentration(CFirstSetField *p_pFirstSetField, CRspFsQryAcctConcentrationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 查询转帐银行帐户响应
  virtual int OnRspQryTransferAcct(CFirstSetField *p_pFirstSetField, CRspFsQryTransferAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户资金划拨响应
  virtual int OnRspQryFortuneAcctTransfer(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctTransferField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户划拨流水查询响应
  virtual int OnRspQryFortuneAcctTransferDetail(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctTransferDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户登录响应
  virtual int OnRspQryCustLogin(CFirstSetField *p_pFirstSetField, CRspFsQryCustLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资产账户查询响应
  virtual int OnRspQryAssetsAcct(CFirstSetField *p_pFirstSetField, CRspFsQryAssetsAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 财富账户查询响应
  virtual int OnRspQryFortuneAcct(CFirstSetField *p_pFirstSetField, CRspFsQryFortuneAcctField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东查询响应
  virtual int OnRspQrySecu(CFirstSetField *p_pFirstSetField, CRspFsQrySecuField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东交易协议查询响应
  virtual int OnRspQrySecuTransferAgreement(CFirstSetField *p_pFirstSetField, CRspFsQrySecuTransferAgreementField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 适当性匹配检查响应
  virtual int OnRspPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 客户适当性查询响应
  virtual int OnRspQryCustPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsQryCustPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 产品适当性查询响应
  virtual int OnRspQryProdPropriatenceCheck(CFirstSetField *p_pFirstSetField, CRspFsQryProdPropriatenceCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询响应
  virtual int OnRspQryFund(CFirstSetField *p_pFirstSetField, CRspFsQryFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 总资产查询响应
  virtual int OnRspQryTotalAssets(CFirstSetField *p_pFirstSetField, CRspFsQryTotalAssetsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份资产查询响应
  virtual int OnRspQryShareAssets(CFirstSetField *p_pFirstSetField, CRspFsQryShareAssetsField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日委托查询响应
  virtual int OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交查询响应
  virtual int OnRspQryCurrDayMatch(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayMatchField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 当日成交统计响应
  virtual int OnRspQryCurrDayMatchCount(CFirstSetField *p_pFirstSetField, CRspFsQryCurrDayMatchCountField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 最大可交易数量计算响应
  virtual int OnRspQryMaxLimitTransferCount(CFirstSetField *p_pFirstSetField, CRspFsQryMaxLimitTransferCountField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易买卖委托申报响应
  virtual int OnRspTrdTransferOrder(CFirstSetField *p_pFirstSetField, CRspFsTrdTransferOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易委托撤单响应
  virtual int OnRspTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, CRspFsTrdTransferPurchaseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可撤单委托查询响应
  virtual int OnRspQryAvlTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, CRspFsQryAvlTrdTransferPurchaseCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券交易批量撤单响应
  virtual int OnRspTrdQuantityCancel(CFirstSetField *p_pFirstSetField, CRspFsTrdQuantityCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 重置成本响应
  virtual int OnRspResetCost(CFirstSetField *p_pFirstSetField, CRspFsResetCostField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行持仓明细查询响应
  virtual int OnRspQryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, CRspFsQryBondsPreIssuePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行客户平仓明细查询响应
  virtual int OnRspQryCustBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, CRspFsQryCustBondsPreIssueClosePosiField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 国债预发行产品信息查询响应
  virtual int OnRspQryBondsPreProdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryBondsPreProdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股配号查询响应
  virtual int OnRspQryNewShareDistribution(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareDistributionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股中签查询响应
  virtual int OnRspQryNewShareWinning(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareWinningField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股申购市值额度查询响应
  virtual int OnRspQryNewShareSubscriptionLimit(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareSubscriptionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 新股信息查询响应
  virtual int OnRspQryNewShareInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNewShareInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购品种信息查询响应
  virtual int OnRspQryQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押物折算率信息查询响应
  virtual int OnRspQryQuoteRepurchaseProductInfoConversionRate(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductInfoConversionRateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押物查询响应
  virtual int OnRspQryQuoteRepurchaseProduct(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseProductField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购合约展期响应
  virtual int OnRspQuoteRepurchaseContflag(CFirstSetField *p_pFirstSetField, CRspFsQuoteRepurchaseContflagField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购质押资金查询响应
  virtual int OnRspQryQuoteRepurchasePledgeFund(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchasePledgeFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约流水查询响应
  virtual int OnRspQryQuoteRepurchaseReservationDetail(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseReservationDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购预约响应
  virtual int OnRspQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, CRspFsQuoteRepurchaseReservationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可提前购回合约查询响应
  virtual int OnRspQryQuoteRepurchaseValPreRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValPreRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可展期取消合约查询响应
  virtual int OnRspQryQuoteRepurchaseValFlagCancel(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValFlagCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可调整自动展期合约查询响应
  virtual int OnRspQryQuoteRepurchaseAutoValFlagCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAutoValFlagContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可取消提前终止预约合约查询响应
  virtual int OnRspQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可取消初始交易预约合约查询响应
  virtual int OnRspQryQuoteRepurchaseAvlCancelStaTransferPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlCancelStaTransferPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可展期合约查询响应
  virtual int OnRspQryQuoteRepurchaseValFlagCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseValFlagContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 报价回购可提前终止预约合约查询响应
  virtual int OnRspQryQuoteRepurchaseAvlAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, CRspFsQryQuoteRepurchaseAvlAdvanceEndPreContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权直接还款响应
  virtual int OnRspFinancingRightDirectRepayment(CFirstSetField *p_pFirstSetField, CRspFsFinancingRightDirectRepaymentField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权合约查询响应
  virtual int OnRspQryFinancingRightcontract(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightcontractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权可还负债查询响应
  virtual int OnRspQryFinancingRightCanrePayDebt(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightCanrePayDebtField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 融资行权偿还明细查询响应
  virtual int OnRspQryFinancingRightPayDetail(CFirstSetField *p_pFirstSetField, CRspFsQryFinancingRightPayDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票股东大会信息查询响应
  virtual int OnRspQryNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingTrdacctsConferenceInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票投票结果查询响应
  virtual int OnRspQryNetworkVotingResult(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingResultField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票议案信息查询响应
  virtual int OnRspQryNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingMotionInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 网络投票产品信息查询响应
  virtual int OnRspQryNetworkVotingProdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryNetworkVotingProdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通价差信息查询响应
  virtual int OnRspQryHkCuaccDifferencet(CFirstSetField *p_pFirstSetField, CRspFsQryHkCuaccDifferencetField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通汇率信息查询响应
  virtual int OnRspQryHkRationInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkRationInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通市场状态信息查询响应
  virtual int OnRspQryHkShareMarketInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareMarketInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通标的证券信息查询响应
  virtual int OnRspQryHkShareCommTrdInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareCommTrdInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通交易日历查询响应
  virtual int OnRspQryHkShareTransferDate(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareTransferDateField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 港股通资金查询响应
  virtual int OnRspQryHkShareFund(CFirstSetField *p_pFirstSetField, CRspFsQryHkShareFundField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 大宗交易行情信息查询响应
  virtual int OnRspQryBlockTradingMkt(CFirstSetField *p_pFirstSetField, CRspFsQryBlockTradingMktField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 盘后行情查询响应
  virtual int OnRspQrylosingMktSecondboard(CFirstSetField *p_pFirstSetField, CRspFsQrylosingMktSecondboardField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 受限投资者可交易证券查询响应
  virtual int OnRspQryRestrictedInvestorValTransferTrd(CFirstSetField *p_pFirstSetField, CRspFsQryRestrictedInvestorValTransferTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转分层信息查询响应
  virtual int OnRspQryStkTranLayeringInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranLayeringInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转询价确认信息查询响应
  virtual int OnRspQryStkTranInquiryConfirmInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranInquiryConfirmInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股转发行业务信息查询响应
  virtual int OnRspQryStkTranIssuingBusinessInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkTranIssuingBusinessInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押合约总表查询响应
  virtual int OnRspQryStkPledgeContract(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押合约明细查询响应
  virtual int OnRspQryStkPledgeContractDetail(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeContractDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押初始交易参考金额查询响应
  virtual int OnRspQryStkPledgeInitTrdReferAmt(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeInitTrdReferAmtField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押股份额度信息查询响应
  virtual int OnRspQryStkPledgeQuotaInfo(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeQuotaInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股票质押标的证券查询响应
  virtual int OnRspQryStkPledgeTrd(CFirstSetField *p_pFirstSetField, CRspFsQryStkPledgeTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 出借人股票质押额度查询响应
  virtual int OnRspQryLenderStkPledgeQuota(CFirstSetField *p_pFirstSetField, CRspFsQryLenderStkPledgeQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳债券质押式协议回购合约查询响应
  virtual int OnRspQryszBondPledgeRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryszBondPledgeRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押式协议回购标的证券查询响应
  virtual int OnRspQryBondPledgeRepurchaseTrd(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeRepurchaseTrdField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购行情查询申报响应
  virtual int OnRspshContRepurchaseMarket(CFirstSetField *p_pFirstSetField, CRspFsshContRepurchaseMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购非公开报价行情查询响应
  virtual int OnRspQryshContRepurchaseClsMarket(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseClsMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购公开报价行情查询响应
  virtual int OnRspQryshContRepurchaseOpenMarket(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseOpenMarketField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购未结算协议查询响应
  virtual int OnRspQryshContRepurchaseUnsettledBill(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseUnsettledBillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海协议回购合约查询响应
  virtual int OnRspQryshContRepurchaseContract(CFirstSetField *p_pFirstSetField, CRspFsQryshContRepurchaseContractField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳债券协议回购购回金额查询响应
  virtual int OnRspQryszBondContRepurchaseQuota(CFirstSetField *p_pFirstSetField, CRspFsQryszBondContRepurchaseQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押股份明细查询响应
  virtual int OnRspQryBondPledgeStkDetail(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeStkDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 债券质押式回购合约查询响应
  virtual int OnRspQryBondPledgeStkRepurchaseCont(CFirstSetField *p_pFirstSetField, CRspFsQryBondPledgeStkRepurchaseContField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请撤单响应
  virtual int OnRspETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, CRspFsETFOfflineApplicationCancelField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF篮子委托响应
  virtual int OnRspETFOrder(CFirstSetField *p_pFirstSetField, CRspFsETFOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF网下认购申请查询响应
  virtual int OnRspQryETFOfflineApplication(CFirstSetField *p_pFirstSetField, CRspFsQryETFOfflineApplicationField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF成分股持仓查询响应
  virtual int OnRspQryETFCotPosition(CFirstSetField *p_pFirstSetField, CRspFsQryETFCotPositionField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // ETF信息查询响应
  virtual int OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, CRspFsQryEtfInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海LOF基金分拆合并信息查询响应
  virtual int OnRspQryshLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspFsQryshLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳LOF基金分拆合并信息查询响应
  virtual int OnRspQryszLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, CRspFsQryszLOFFundSplitMergeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股权人员信息查询响应
  virtual int OnRspQrySecurInfo(CFirstSetField *p_pFirstSetField, CRspFsQrySecurInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 所得税策略信息维护响应
  virtual int OnRspQryTaxInfoMaintenance(CFirstSetField *p_pFirstSetField, CRspFsQryTaxInfoMaintenanceField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 要约收购转换关系表查询响应
  virtual int OnRspQryTenderSgConverRelat(CFirstSetField *p_pFirstSetField, CRspFsQryTenderSgConverRelatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 上海减持股份额度查询响应
  virtual int OnRspQryshReduceStkQuota(CFirstSetField *p_pFirstSetField, CRspFsQryshReduceStkQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳减持股份额度查询响应
  virtual int OnRspQryszReduceStkQuota(CFirstSetField *p_pFirstSetField, CRspFsQryszReduceStkQuotaField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 权证信息查询响应
  virtual int OnRspQryWarrantInfo(CFirstSetField *p_pFirstSetField, CRspFsQryWarrantInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 三方回购篮子信息查询响应
  virtual int OnRspQryThreeRePurchaseInfo(CFirstSetField *p_pFirstSetField, CRspFsQryThreeRePurchaseInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 三方回购质押表查询响应
  virtual int OnRspQryThreeRePurchasepledge(CFirstSetField *p_pFirstSetField, CRspFsQryThreeRePurchasepledgeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购转发成交信息表查询响应
  virtual int OnRspQryszThreeRePurchaseMatchInfo(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseMatchInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购合约总表查询响应
  virtual int OnRspQryszThreeRePurchaseContTotal(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseContTotalField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 深圳三方回购合约明细查询响应
  virtual int OnRspQryszThreeRePurchaseContDetail(CFirstSetField *p_pFirstSetField, CRspFsQryszThreeRePurchaseContDetailField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H股全流通股东账户信息查询响应
  virtual int OnRspQryHStkFullCirAcctInfo(CFirstSetField *p_pFirstSetField, CRspFsQryHStkFullCirAcctInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // H股境外证券代码对应关系查询响应
  virtual int OnRspQryHStkForTrdIdCorrespond(CFirstSetField *p_pFirstSetField, CRspFsQryHStkForTrdIdCorrespondField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 询价交易非公开报价行情查询响应
  virtual int OnRspQryInquireTransferCloseMarketInfo(CFirstSetField *p_pFirstSetField, CRspFsQryInquireTransferCloseMarketInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交查询响应
  virtual int OnRspQryHisFill(CFirstSetField *p_pFirstSetField, CRspFsQryHisFillField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托查询响应
  virtual int OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, CRspFsQryHisOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史交割单查询响应
  virtual int OnRspQryHisDeliOrder(CFirstSetField *p_pFirstSetField, CRspFsQryHisDeliOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliFs2TradeApi : virtual public CCliFs2Api
{
public:
  // 默认构造函数
  CCliFs2TradeApi(void);

  // 析构函数
  virtual ~CCliFs2TradeApi(void);

  // 机构信息查询请求
  virtual int ReqQryOrgInfo(CReqFsQryOrgInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 证券信息查询请求
  virtual int ReqQryTrdInfo(CReqFsQryTrdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 字典信息查询请求
  virtual int ReqQryDictInfo(CReqFsQryDictInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 系统状态查询请求
  virtual int ReqQrySysStat(CReqFsQrySysStatField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转账请求
  virtual int ReqBankStkTransfer(CReqFsBankStkTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 券商发起查银行余额请求
  virtual int ReqQryInitiatorOrderBankBal(CReqFsQryInitiatorOrderBankBalField *p_pReqField, LONGLONG p_llRequestId);
  // 查询银证业务流水请求
  virtual int ReqQryBankStkDetail(CReqFsQryBankStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 客户内现金划转请求
  virtual int ReqCustCashTransfer(CReqFsCustCashTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 账户资金归集请求
  virtual int ReqQryAcctConcentration(CReqFsQryAcctConcentrationField *p_pReqField, LONGLONG p_llRequestId);
  // 查询转帐银行帐户请求
  virtual int ReqQryTransferAcct(CReqFsQryTransferAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户资金划拨请求
  virtual int ReqQryFortuneAcctTransfer(CReqFsQryFortuneAcctTransferField *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户划拨流水查询请求
  virtual int ReqQryFortuneAcctTransferDetail(CReqFsQryFortuneAcctTransferDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 客户登录请求
  virtual int ReqQryCustLogin(CReqFsQryCustLoginField *p_pReqField, LONGLONG p_llRequestId);
  // 资产账户查询请求
  virtual int ReqQryAssetsAcct(CReqFsQryAssetsAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 财富账户查询请求
  virtual int ReqQryFortuneAcct(CReqFsQryFortuneAcctField *p_pReqField, LONGLONG p_llRequestId);
  // 股东查询请求
  virtual int ReqQrySecu(CReqFsQrySecuField *p_pReqField, LONGLONG p_llRequestId);
  // 股东交易协议查询请求
  virtual int ReqQrySecuTransferAgreement(CReqFsQrySecuTransferAgreementField *p_pReqField, LONGLONG p_llRequestId);
  // 适当性匹配检查请求
  virtual int ReqPropriatenceCheck(CReqFsPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 客户适当性查询请求
  virtual int ReqQryCustPropriatenceCheck(CReqFsQryCustPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 产品适当性查询请求
  virtual int ReqQryProdPropriatenceCheck(CReqFsQryProdPropriatenceCheckField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询请求
  virtual int ReqQryFund(CReqFsQryFundField *p_pReqField, LONGLONG p_llRequestId);
  // 总资产查询请求
  virtual int ReqQryTotalAssets(CReqFsQryTotalAssetsField *p_pReqField, LONGLONG p_llRequestId);
  // 股份资产查询请求
  virtual int ReqQryShareAssets(CReqFsQryShareAssetsField *p_pReqField, LONGLONG p_llRequestId);
  // 当日委托查询请求
  virtual int ReqQryCurrDayOrder(CReqFsQryCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交查询请求
  virtual int ReqQryCurrDayMatch(CReqFsQryCurrDayMatchField *p_pReqField, LONGLONG p_llRequestId);
  // 当日成交统计请求
  virtual int ReqQryCurrDayMatchCount(CReqFsQryCurrDayMatchCountField *p_pReqField, LONGLONG p_llRequestId);
  // 最大可交易数量计算请求
  virtual int ReqQryMaxLimitTransferCount(CReqFsQryMaxLimitTransferCountField *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易买卖委托申报请求
  virtual int ReqTrdTransferOrder(CReqFsTrdTransferOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易委托撤单请求
  virtual int ReqTrdTransferPurchaseCancel(CReqFsTrdTransferPurchaseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 可撤单委托查询请求
  virtual int ReqQryAvlTrdTransferPurchaseCancel(CReqFsQryAvlTrdTransferPurchaseCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 证券交易批量撤单请求
  virtual int ReqTrdQuantityCancel(CReqFsTrdQuantityCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 重置成本请求
  virtual int ReqResetCost(CReqFsResetCostField *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行持仓明细查询请求
  virtual int ReqQryBondsPreIssuePosi(CReqFsQryBondsPreIssuePosiField *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行客户平仓明细查询请求
  virtual int ReqQryCustBondsPreIssueClosePosi(CReqFsQryCustBondsPreIssueClosePosiField *p_pReqField, LONGLONG p_llRequestId);
  // 国债预发行产品信息查询请求
  virtual int ReqQryBondsPreProdInfo(CReqFsQryBondsPreProdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 新股配号查询请求
  virtual int ReqQryNewShareDistribution(CReqFsQryNewShareDistributionField *p_pReqField, LONGLONG p_llRequestId);
  // 新股中签查询请求
  virtual int ReqQryNewShareWinning(CReqFsQryNewShareWinningField *p_pReqField, LONGLONG p_llRequestId);
  // 新股申购市值额度查询请求
  virtual int ReqQryNewShareSubscriptionLimit(CReqFsQryNewShareSubscriptionLimitField *p_pReqField, LONGLONG p_llRequestId);
  // 新股信息查询请求
  virtual int ReqQryNewShareInfo(CReqFsQryNewShareInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购品种信息查询请求
  virtual int ReqQryQuoteRepurchaseProductInfo(CReqFsQryQuoteRepurchaseProductInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押物折算率信息查询请求
  virtual int ReqQryQuoteRepurchaseProductInfoConversionRate(CReqFsQryQuoteRepurchaseProductInfoConversionRateField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押物查询请求
  virtual int ReqQryQuoteRepurchaseProduct(CReqFsQryQuoteRepurchaseProductField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购合约展期请求
  virtual int ReqQuoteRepurchaseContflag(CReqFsQuoteRepurchaseContflagField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购质押资金查询请求
  virtual int ReqQryQuoteRepurchasePledgeFund(CReqFsQryQuoteRepurchasePledgeFundField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约流水查询请求
  virtual int ReqQryQuoteRepurchaseReservationDetail(CReqFsQryQuoteRepurchaseReservationDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购预约请求
  virtual int ReqQuoteRepurchaseReservation(CReqFsQuoteRepurchaseReservationField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可提前购回合约查询请求
  virtual int ReqQryQuoteRepurchaseValPreRepurchaseCont(CReqFsQryQuoteRepurchaseValPreRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可展期取消合约查询请求
  virtual int ReqQryQuoteRepurchaseValFlagCancel(CReqFsQryQuoteRepurchaseValFlagCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可调整自动展期合约查询请求
  virtual int ReqQryQuoteRepurchaseAutoValFlagCont(CReqFsQryQuoteRepurchaseAutoValFlagContField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可取消提前终止预约合约查询请求
  virtual int ReqQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CReqFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可取消初始交易预约合约查询请求
  virtual int ReqQryQuoteRepurchaseAvlCancelStaTransferPreCont(CReqFsQryQuoteRepurchaseAvlCancelStaTransferPreContField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可展期合约查询请求
  virtual int ReqQryQuoteRepurchaseValFlagCont(CReqFsQryQuoteRepurchaseValFlagContField *p_pReqField, LONGLONG p_llRequestId);
  // 报价回购可提前终止预约合约查询请求
  virtual int ReqQryQuoteRepurchaseAvlAdvanceEndPreCont(CReqFsQryQuoteRepurchaseAvlAdvanceEndPreContField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权直接还款请求
  virtual int ReqFinancingRightDirectRepayment(CReqFsFinancingRightDirectRepaymentField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权合约查询请求
  virtual int ReqQryFinancingRightcontract(CReqFsQryFinancingRightcontractField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权可还负债查询请求
  virtual int ReqQryFinancingRightCanrePayDebt(CReqFsQryFinancingRightCanrePayDebtField *p_pReqField, LONGLONG p_llRequestId);
  // 融资行权偿还明细查询请求
  virtual int ReqQryFinancingRightPayDetail(CReqFsQryFinancingRightPayDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票股东大会信息查询请求
  virtual int ReqQryNetworkVotingTrdacctsConferenceInfo(CReqFsQryNetworkVotingTrdacctsConferenceInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票投票结果查询请求
  virtual int ReqQryNetworkVotingResult(CReqFsQryNetworkVotingResultField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票议案信息查询请求
  virtual int ReqQryNetworkVotingMotionInfo(CReqFsQryNetworkVotingMotionInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 网络投票产品信息查询请求
  virtual int ReqQryNetworkVotingProdInfo(CReqFsQryNetworkVotingProdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通价差信息查询请求
  virtual int ReqQryHkCuaccDifferencet(CReqFsQryHkCuaccDifferencetField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通汇率信息查询请求
  virtual int ReqQryHkRationInfo(CReqFsQryHkRationInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通市场状态信息查询请求
  virtual int ReqQryHkShareMarketInfo(CReqFsQryHkShareMarketInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通标的证券信息查询请求
  virtual int ReqQryHkShareCommTrdInfo(CReqFsQryHkShareCommTrdInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通交易日历查询请求
  virtual int ReqQryHkShareTransferDate(CReqFsQryHkShareTransferDateField *p_pReqField, LONGLONG p_llRequestId);
  // 港股通资金查询请求
  virtual int ReqQryHkShareFund(CReqFsQryHkShareFundField *p_pReqField, LONGLONG p_llRequestId);
  // 大宗交易行情信息查询请求
  virtual int ReqQryBlockTradingMkt(CReqFsQryBlockTradingMktField *p_pReqField, LONGLONG p_llRequestId);
  // 盘后行情查询请求
  virtual int ReqQrylosingMktSecondboard(CReqFsQrylosingMktSecondboardField *p_pReqField, LONGLONG p_llRequestId);
  // 受限投资者可交易证券查询请求
  virtual int ReqQryRestrictedInvestorValTransferTrd(CReqFsQryRestrictedInvestorValTransferTrdField *p_pReqField, LONGLONG p_llRequestId);
  // 股转分层信息查询请求
  virtual int ReqQryStkTranLayeringInfo(CReqFsQryStkTranLayeringInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 股转询价确认信息查询请求
  virtual int ReqQryStkTranInquiryConfirmInfo(CReqFsQryStkTranInquiryConfirmInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 股转发行业务信息查询请求
  virtual int ReqQryStkTranIssuingBusinessInfo(CReqFsQryStkTranIssuingBusinessInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押合约总表查询请求
  virtual int ReqQryStkPledgeContract(CReqFsQryStkPledgeContractField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押合约明细查询请求
  virtual int ReqQryStkPledgeContractDetail(CReqFsQryStkPledgeContractDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押初始交易参考金额查询请求
  virtual int ReqQryStkPledgeInitTrdReferAmt(CReqFsQryStkPledgeInitTrdReferAmtField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押股份额度信息查询请求
  virtual int ReqQryStkPledgeQuotaInfo(CReqFsQryStkPledgeQuotaInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 股票质押标的证券查询请求
  virtual int ReqQryStkPledgeTrd(CReqFsQryStkPledgeTrdField *p_pReqField, LONGLONG p_llRequestId);
  // 出借人股票质押额度查询请求
  virtual int ReqQryLenderStkPledgeQuota(CReqFsQryLenderStkPledgeQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳债券质押式协议回购合约查询请求
  virtual int ReqQryszBondPledgeRepurchaseCont(CReqFsQryszBondPledgeRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押式协议回购标的证券查询请求
  virtual int ReqQryBondPledgeRepurchaseTrd(CReqFsQryBondPledgeRepurchaseTrdField *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购行情查询申报请求
  virtual int ReqshContRepurchaseMarket(CReqFsshContRepurchaseMarketField *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购非公开报价行情查询请求
  virtual int ReqQryshContRepurchaseClsMarket(CReqFsQryshContRepurchaseClsMarketField *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购公开报价行情查询请求
  virtual int ReqQryshContRepurchaseOpenMarket(CReqFsQryshContRepurchaseOpenMarketField *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购未结算协议查询请求
  virtual int ReqQryshContRepurchaseUnsettledBill(CReqFsQryshContRepurchaseUnsettledBillField *p_pReqField, LONGLONG p_llRequestId);
  // 上海协议回购合约查询请求
  virtual int ReqQryshContRepurchaseContract(CReqFsQryshContRepurchaseContractField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳债券协议回购购回金额查询请求
  virtual int ReqQryszBondContRepurchaseQuota(CReqFsQryszBondContRepurchaseQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押股份明细查询请求
  virtual int ReqQryBondPledgeStkDetail(CReqFsQryBondPledgeStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 债券质押式回购合约查询请求
  virtual int ReqQryBondPledgeStkRepurchaseCont(CReqFsQryBondPledgeStkRepurchaseContField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请撤单请求
  virtual int ReqETFOfflineApplicationCancel(CReqFsETFOfflineApplicationCancelField *p_pReqField, LONGLONG p_llRequestId);
  // ETF篮子委托请求
  virtual int ReqETFOrder(CReqFsETFOrderField *p_pReqField, LONGLONG p_llRequestId);
  // ETF网下认购申请查询请求
  virtual int ReqQryETFOfflineApplication(CReqFsQryETFOfflineApplicationField *p_pReqField, LONGLONG p_llRequestId);
  // ETF成分股持仓查询请求
  virtual int ReqQryETFCotPosition(CReqFsQryETFCotPositionField *p_pReqField, LONGLONG p_llRequestId);
  // ETF信息查询请求
  virtual int ReqQryEtfInfo(CReqFsQryEtfInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 上海LOF基金分拆合并信息查询请求
  virtual int ReqQryshLOFFundSplitMerge(CReqFsQryshLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳LOF基金分拆合并信息查询请求
  virtual int ReqQryszLOFFundSplitMerge(CReqFsQryszLOFFundSplitMergeField *p_pReqField, LONGLONG p_llRequestId);
  // 股权人员信息查询请求
  virtual int ReqQrySecurInfo(CReqFsQrySecurInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 所得税策略信息维护请求
  virtual int ReqQryTaxInfoMaintenance(CReqFsQryTaxInfoMaintenanceField *p_pReqField, LONGLONG p_llRequestId);
  // 要约收购转换关系表查询请求
  virtual int ReqQryTenderSgConverRelat(CReqFsQryTenderSgConverRelatField *p_pReqField, LONGLONG p_llRequestId);
  // 上海减持股份额度查询请求
  virtual int ReqQryshReduceStkQuota(CReqFsQryshReduceStkQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳减持股份额度查询请求
  virtual int ReqQryszReduceStkQuota(CReqFsQryszReduceStkQuotaField *p_pReqField, LONGLONG p_llRequestId);
  // 权证信息查询请求
  virtual int ReqQryWarrantInfo(CReqFsQryWarrantInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 三方回购篮子信息查询请求
  virtual int ReqQryThreeRePurchaseInfo(CReqFsQryThreeRePurchaseInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 三方回购质押表查询请求
  virtual int ReqQryThreeRePurchasepledge(CReqFsQryThreeRePurchasepledgeField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购转发成交信息表查询请求
  virtual int ReqQryszThreeRePurchaseMatchInfo(CReqFsQryszThreeRePurchaseMatchInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购合约总表查询请求
  virtual int ReqQryszThreeRePurchaseContTotal(CReqFsQryszThreeRePurchaseContTotalField *p_pReqField, LONGLONG p_llRequestId);
  // 深圳三方回购合约明细查询请求
  virtual int ReqQryszThreeRePurchaseContDetail(CReqFsQryszThreeRePurchaseContDetailField *p_pReqField, LONGLONG p_llRequestId);
  // H股全流通股东账户信息查询请求
  virtual int ReqQryHStkFullCirAcctInfo(CReqFsQryHStkFullCirAcctInfoField *p_pReqField, LONGLONG p_llRequestId);
  // H股境外证券代码对应关系查询请求
  virtual int ReqQryHStkForTrdIdCorrespond(CReqFsQryHStkForTrdIdCorrespondField *p_pReqField, LONGLONG p_llRequestId);
  // 询价交易非公开报价行情查询请求
  virtual int ReqQryInquireTransferCloseMarketInfo(CReqFsQryInquireTransferCloseMarketInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交查询请求
  virtual int ReqQryHisFill(CReqFsQryHisFillField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托查询请求
  virtual int ReqQryHisOrder(CReqFsQryHisOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 历史交割单查询请求
  virtual int ReqQryHisDeliOrder(CReqFsQryHisDeliOrderField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // 机构信息查询响应
  void OnRspQryOrgInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券信息查询响应
  void OnRspQryTrdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 字典信息查询响应
  void OnRspQryDictInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 系统状态查询响应
  void OnRspQrySysStat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转账响应
  void OnRspBankStkTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 券商发起查银行余额响应
  void OnRspQryInitiatorOrderBankBal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询银证业务流水响应
  void OnRspQryBankStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户内现金划转响应
  void OnRspCustCashTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 账户资金归集响应
  void OnRspQryAcctConcentration(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 查询转帐银行帐户响应
  void OnRspQryTransferAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户资金划拨响应
  void OnRspQryFortuneAcctTransfer(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户划拨流水查询响应
  void OnRspQryFortuneAcctTransferDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户登录响应
  void OnRspQryCustLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资产账户查询响应
  void OnRspQryAssetsAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 财富账户查询响应
  void OnRspQryFortuneAcct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东查询响应
  void OnRspQrySecu(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东交易协议查询响应
  void OnRspQrySecuTransferAgreement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 适当性匹配检查响应
  void OnRspPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 客户适当性查询响应
  void OnRspQryCustPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 产品适当性查询响应
  void OnRspQryProdPropriatenceCheck(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询响应
  void OnRspQryFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 总资产查询响应
  void OnRspQryTotalAssets(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份资产查询响应
  void OnRspQryShareAssets(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日委托查询响应
  void OnRspQryCurrDayOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交查询响应
  void OnRspQryCurrDayMatch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 当日成交统计响应
  void OnRspQryCurrDayMatchCount(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 最大可交易数量计算响应
  void OnRspQryMaxLimitTransferCount(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易买卖委托申报响应
  void OnRspTrdTransferOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易委托撤单响应
  void OnRspTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可撤单委托查询响应
  void OnRspQryAvlTrdTransferPurchaseCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券交易批量撤单响应
  void OnRspTrdQuantityCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 重置成本响应
  void OnRspResetCost(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行持仓明细查询响应
  void OnRspQryBondsPreIssuePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行客户平仓明细查询响应
  void OnRspQryCustBondsPreIssueClosePosi(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 国债预发行产品信息查询响应
  void OnRspQryBondsPreProdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股配号查询响应
  void OnRspQryNewShareDistribution(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股中签查询响应
  void OnRspQryNewShareWinning(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股申购市值额度查询响应
  void OnRspQryNewShareSubscriptionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 新股信息查询响应
  void OnRspQryNewShareInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购品种信息查询响应
  void OnRspQryQuoteRepurchaseProductInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押物折算率信息查询响应
  void OnRspQryQuoteRepurchaseProductInfoConversionRate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押物查询响应
  void OnRspQryQuoteRepurchaseProduct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购合约展期响应
  void OnRspQuoteRepurchaseContflag(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购质押资金查询响应
  void OnRspQryQuoteRepurchasePledgeFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约流水查询响应
  void OnRspQryQuoteRepurchaseReservationDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购预约响应
  void OnRspQuoteRepurchaseReservation(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可提前购回合约查询响应
  void OnRspQryQuoteRepurchaseValPreRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可展期取消合约查询响应
  void OnRspQryQuoteRepurchaseValFlagCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可调整自动展期合约查询响应
  void OnRspQryQuoteRepurchaseAutoValFlagCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可取消提前终止预约合约查询响应
  void OnRspQryQuoteRepurchaseAvlCancelAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可取消初始交易预约合约查询响应
  void OnRspQryQuoteRepurchaseAvlCancelStaTransferPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可展期合约查询响应
  void OnRspQryQuoteRepurchaseValFlagCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 报价回购可提前终止预约合约查询响应
  void OnRspQryQuoteRepurchaseAvlAdvanceEndPreCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权直接还款响应
  void OnRspFinancingRightDirectRepayment(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权合约查询响应
  void OnRspQryFinancingRightcontract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权可还负债查询响应
  void OnRspQryFinancingRightCanrePayDebt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 融资行权偿还明细查询响应
  void OnRspQryFinancingRightPayDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票股东大会信息查询响应
  void OnRspQryNetworkVotingTrdacctsConferenceInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票投票结果查询响应
  void OnRspQryNetworkVotingResult(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票议案信息查询响应
  void OnRspQryNetworkVotingMotionInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 网络投票产品信息查询响应
  void OnRspQryNetworkVotingProdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通价差信息查询响应
  void OnRspQryHkCuaccDifferencet(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通汇率信息查询响应
  void OnRspQryHkRationInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通市场状态信息查询响应
  void OnRspQryHkShareMarketInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通标的证券信息查询响应
  void OnRspQryHkShareCommTrdInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通交易日历查询响应
  void OnRspQryHkShareTransferDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 港股通资金查询响应
  void OnRspQryHkShareFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 大宗交易行情信息查询响应
  void OnRspQryBlockTradingMkt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 盘后行情查询响应
  void OnRspQrylosingMktSecondboard(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 受限投资者可交易证券查询响应
  void OnRspQryRestrictedInvestorValTransferTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转分层信息查询响应
  void OnRspQryStkTranLayeringInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转询价确认信息查询响应
  void OnRspQryStkTranInquiryConfirmInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股转发行业务信息查询响应
  void OnRspQryStkTranIssuingBusinessInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押合约总表查询响应
  void OnRspQryStkPledgeContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押合约明细查询响应
  void OnRspQryStkPledgeContractDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押初始交易参考金额查询响应
  void OnRspQryStkPledgeInitTrdReferAmt(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押股份额度信息查询响应
  void OnRspQryStkPledgeQuotaInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股票质押标的证券查询响应
  void OnRspQryStkPledgeTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 出借人股票质押额度查询响应
  void OnRspQryLenderStkPledgeQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳债券质押式协议回购合约查询响应
  void OnRspQryszBondPledgeRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押式协议回购标的证券查询响应
  void OnRspQryBondPledgeRepurchaseTrd(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购行情查询申报响应
  void OnRspshContRepurchaseMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购非公开报价行情查询响应
  void OnRspQryshContRepurchaseClsMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购公开报价行情查询响应
  void OnRspQryshContRepurchaseOpenMarket(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购未结算协议查询响应
  void OnRspQryshContRepurchaseUnsettledBill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海协议回购合约查询响应
  void OnRspQryshContRepurchaseContract(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳债券协议回购购回金额查询响应
  void OnRspQryszBondContRepurchaseQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押股份明细查询响应
  void OnRspQryBondPledgeStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 债券质押式回购合约查询响应
  void OnRspQryBondPledgeStkRepurchaseCont(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请撤单响应
  void OnRspETFOfflineApplicationCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF篮子委托响应
  void OnRspETFOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF网下认购申请查询响应
  void OnRspQryETFOfflineApplication(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF成分股持仓查询响应
  void OnRspQryETFCotPosition(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // ETF信息查询响应
  void OnRspQryEtfInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海LOF基金分拆合并信息查询响应
  void OnRspQryshLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳LOF基金分拆合并信息查询响应
  void OnRspQryszLOFFundSplitMerge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股权人员信息查询响应
  void OnRspQrySecurInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 所得税策略信息维护响应
  void OnRspQryTaxInfoMaintenance(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 要约收购转换关系表查询响应
  void OnRspQryTenderSgConverRelat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 上海减持股份额度查询响应
  void OnRspQryshReduceStkQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳减持股份额度查询响应
  void OnRspQryszReduceStkQuota(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 权证信息查询响应
  void OnRspQryWarrantInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 三方回购篮子信息查询响应
  void OnRspQryThreeRePurchaseInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 三方回购质押表查询响应
  void OnRspQryThreeRePurchasepledge(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购转发成交信息表查询响应
  void OnRspQryszThreeRePurchaseMatchInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购合约总表查询响应
  void OnRspQryszThreeRePurchaseContTotal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 深圳三方回购合约明细查询响应
  void OnRspQryszThreeRePurchaseContDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H股全流通股东账户信息查询响应
  void OnRspQryHStkFullCirAcctInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // H股境外证券代码对应关系查询响应
  void OnRspQryHStkForTrdIdCorrespond(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 询价交易非公开报价行情查询响应
  void OnRspQryInquireTransferCloseMarketInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交查询响应
  void OnRspQryHisFill(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托查询响应
  void OnRspQryHisOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史交割单查询响应
  void OnRspQryHisDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
private:
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FS2_TRADE_API_H__