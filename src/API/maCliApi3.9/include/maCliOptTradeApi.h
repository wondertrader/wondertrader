//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maCliOptTradeApi.h
// 模块名称：股票期权交易API C++ Class
// 模块描述：
// 开发作者：何万刚
// 创建日期：2012-12-23
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2012-12-23    1.0          何万刚          初创
//----------------------------------------------------------------------------
#if !defined(__MA_CLI_OPT_TRADE_API_H__)
#define __MA_CLI_OPT_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliOptTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliOptTradeSpi : virtual public CCliTradeSpi
{
public:
  // 用户登录请求响应
  virtual int OnRspUserLogin(CFirstSetField* p_pFirstSetField, CRspOptUserLoginField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 期权基础信息查询响应
  virtual int OnRspQryBaseInfo(CFirstSetField* p_pFirstSetField, CRspOptBaseInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权委托申报响应
  virtual int OnRspOrder(CFirstSetField* p_pFirstSetField, CRspOptOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 个股期权委托撤单响应
  virtual int OnRspCancelOrder(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权委托申报响应(极速)
  virtual int OnRspOrderFlash(CFirstSetField* p_pFirstSetField, CRspOptOrderFlashField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权委托撤单响应(极速)
  virtual int OnRspCancelOrderFlash(CFirstSetField* p_pFirstSetField, CRspOptCancelOrderFlashField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权标的证券锁定解锁响应
  virtual int OnRspUndlStkLock(CFirstSetField* p_pFirstSetField, CRspOptUndlStkLockField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 计算个股期权最大可交易数量响应
  virtual int OnRspMaxTradeQty(CFirstSetField* p_pFirstSetField, CRspOptMaxTradeQtyField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应
  virtual int OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, CRspOptExpendableFundField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用合约资产查询响应
  virtual int OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, CRspOptExpendableCuField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用备兑股份查询响应
  virtual int OnRspQryExpendableCvdStk(CFirstSetField* p_pFirstSetField, CRspOptExpendableCvdStkField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权当日委托查询响应
  virtual int OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, CRspOptCurrDayOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权当日成交查询响应
  virtual int OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, CRspOptCurrDayFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权合约账户查询响应
  virtual int OnRspQryAcct(CFirstSetField* p_pFirstSetField, CRspOptAcctField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 个股期权可撤委托查询响应
  virtual int OnRspQryCanWithdrawOrder(CFirstSetField* p_pFirstSetField, CRspOptCanWithdrawOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnOptOrderConfirmField* p_pRtnField) {return 0;}

  // 成交回报
  virtual int OnRtnOrderFill(CRtnOptOrderFillField* p_pRtnField) { return 0; }

  // 成交回报--委托信息
  virtual int OnRtnOrder(CRtnOptOrderField* p_pRtnField) {return 0;}

  // 成交回报--合约信息
  virtual int OnRtnContract(CRtnOptContractField* p_pRtnField) {return 0;}

  // 确认回报(极速)
  virtual int OnRtnOrderConfirmFlash(CRtnOptOrderConfirmFlashField* p_pRtnField) {return 0;}

  // 成交回报(极速)
  virtual int OnRtnOrderFillFlash(CRtnOptOrderFillFlashField* p_pRtnField) {return 0;}

  //////////////////////////////////////////////////////////////////////////////
  
  //组合策略文件查询响应 2015/8/3
  virtual int OnRspQryCombStra(CFirstSetField* p_pFirstSetField,CRspOptCombStraField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //组合保证金委托响应 2015/8/5
  virtual int OnRspCombStraOrder(CFirstSetField* p_pFirstSetField,CRspOptCombStraOrderField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //可用组合持仓查询响应 2015/8/5
  virtual int OnRspQryCombStraPos(CFirstSetField* p_pFirstSetField,CRspOptCombStraPosField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //可用组合持仓明细查询响应 2015/8/5
  virtual int OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField,CRspOptCombStraPosDetailField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //个股期权初始保证金 2015/8/10
  virtual int OnRspIniMargin(CFirstSetField* p_pFirstSetField,CRspOptIniMarginField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //行权指派欠资     2015/8/10
  virtual int OnRspQryExeDebt(CFirstSetField* p_pFirstSetField,CRspOptExeDebtField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //行权指派欠券       2015/8/10
  virtual int OnRspQryExeStkDebt(CFirstSetField* p_pFirstSetField,CRspOptExeStkDebtField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //客户风险度         2015/8/10
  virtual int OnRspQryRiskLvl(CFirstSetField* p_pFirstSetField,CRspOptRiskLvlField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //客户结算单查询 2015/8/6
  virtual int OnRspQrySettList(CFirstSetField* p_pFirstSetField,CRspOptSettListField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //客户结算单确认 2015/8/6
  virtual int OnRspSettListConfirm(CFirstSetField* p_pFirstSetField,CRspOptSettListConfirmField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //银衍转账响应 2015/8/3
  virtual int OnRspBankDeriTrans(CFirstSetField* p_pFirstSetField,CRspOptBankDeriTransField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}
  
  //查询银衍交易配置信息响应 2015/8/7
  virtual int OnRspBankDeriInfo(CFirstSetField* p_pFirstSetField,CRspOptBankDeriInfoField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //查询客户签约银行代码响应 2015/8/7
  virtual int OnRspUserBank(CFirstSetField* p_pFirstSetField,CRspOptUserBankField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //对账单行权交割流水查询响应 2015/8/7
  virtual int OnRspStateDeliHis(CFirstSetField* p_pFirstSetField,CRspOptStateDeliHisField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //银衍转账流水查询响应 2015/8/7
  virtual int OnRspBankDeriTransHis(CFirstSetField* p_pFirstSetField,CRspOptBankDeriTransHisField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //银行余额查询响应 2015/8/7
  virtual int OnRspBankBala(CFirstSetField* p_pFirstSetField,CRspOptBankBalaField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  //密码修改响应 2015/8/3
  virtual int OnRspChangePwd(CFirstSetField* p_pFirstSetField,CRspOptChangePwdField* p_pRspField,LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex){return 0;}

  // 可锁定股份查询
  virtual int OnRspQryCanLockStk(CFirstSetField* p_pFirstSetField, CRspOptCanLockStkField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户适当性匹配产品信息查询
  virtual int OnRspQryCustAppropriateMatchInfo(CFirstSetField* p_pFirstSetField, CRspOptQryCustAppropriateMatchInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户适当性信息查询
  virtual int OnRspQryCustAppropriateInfo(CFirstSetField* p_pFirstSetField, CRspOptQryCustAppropriateInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户风险警示书签署
  virtual int OnRspCustRiskSign(CFirstSetField* p_pFirstSetField, CRspOptCustRiskSignField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权行权指令合并申报委托响应
  virtual int OnRspCombExe(CFirstSetField *p_pFirstSetField, CRspOptCombExeField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 行权指令合并申报成份合约查询响应
  virtual int OnRspQryCombExeLegNum(CFirstSetField *p_pFirstSetField, CRspOptQryCombExeLegNumField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股票期权当日委托查询[优化]响应
  virtual int OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, CRspOptQryCurrDayOrderExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 股票期权当日成交查询[优化]响应
  virtual int OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, CRspOptQryCurrDayFillExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询[优化]响应
  virtual int OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableFundExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可取资金查询[优化]响应
  virtual int OnRspQryDrawFundEx(CFirstSetField *p_pFirstSetField, CRspOptQryDrawFundExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用合约资产查询[优化]响应
  virtual int OnRspQryExpendableCuEx(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableCuExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 历史委托查询(RPC)响应
  virtual int OnRspQryHisOrder(CFirstSetField* p_pFirstSetField, CRspOptQryHisOrderField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 历史成交查询(RPC)响应
  virtual int OnRspQryHisFill(CFirstSetField* p_pFirstSetField, CRspOptQryHisFillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户结算单(文件)查询响应
  virtual int OnRspQrySettListFile(CFirstSetField* p_pFirstSetField, CRspOptQrySettListFileField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 个股期权委托申报(优化)响应
  virtual int OnRspOrderEx(CFirstSetField* p_pFirstSetField, CRspOptOrderExField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 期权做市双边报价（深圳）响应
  virtual int OnRspBilateralQuote(CFirstSetField* p_pFirstSetField, CRspOptBilateralQuoteField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 期权做市双边报价撤单（深圳）响应
  virtual int OnRspBilateralQuotationKill(CFirstSetField* p_pFirstSetField, CRspOptBilateralQuotationKillField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 证券信息查询（期权）响应
  virtual int OnRspQryStkInfo(CFirstSetField* p_pFirstSetField, CRspOptQryStkInfoField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 期权客户登录(新)响应
  virtual int OnRspRpcUserLogin(CFirstSetField* p_pFirstSetField, CRspOptRpcUserLoginField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 快速订单资金划拨响应
  virtual int OnRspRpcFundingTransfer(CFirstSetField* p_pFirstSetField, CRspOptRpcFundingTransferField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 快速订单可划入资金查询响应
  virtual int OnRspQryRpcFundingTransfer(CFirstSetField* p_pFirstSetField, CRspOptQryRpcFundingTransferField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 快速订单资金划拨流水查询响应
  virtual int OnRspQryRpcFundingTransferFlow(CFirstSetField* p_pFirstSetField, CRspOptQryRpcFundingTransferFlowField* p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 临近到期合约资产查询响应
  virtual int OnRspQryExpendableCuOfCloseToExpireDate(CFirstSetField *p_pFirstSetField, CRspOptQryExpendableCuOfCloseToExpireDateField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权行权指派明细查询响应
  virtual int OnRspQryRpcExeDetail(CFirstSetField *p_pFirstSetField, CRspOptQryRpcExeDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权当日风险通知查询响应
  virtual int OnRspQryRpcCurrDayRiskNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcCurrDayRiskNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权历史风险通知查询响应
  virtual int OnRspQryRpcHisRiskNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcHisRiskNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权当日风险通知查询（风控系统专用）响应
  virtual int OnRspQryRpcCurrDayRiskSysNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcCurrDayRiskSysNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 股票期权历史风险通知查询（风控系统专用）响应
  virtual int OnRspQryRpcHisRiskSysNotify(CFirstSetField *p_pFirstSetField, CRspOptQryRpcHisRiskSysNotifyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 客户限仓额度查询响应
  virtual int OnRspQryPositionLimit(CFirstSetField *p_pFirstSetField, CRspOptQryPositionLimitField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) { return 0; }
};

class MATRADEAPI CCliOptTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliOptTradeApi(void);

  // 析构函数
  virtual ~CCliOptTradeApi(void);

  // 用户登录请求
  virtual int ReqUserLogin(CReqOptUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 期权基础信息查询
  virtual int ReqQryBaseInfo(CReqOptBaseInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权委托申报
  virtual int ReqOrder(CReqOptOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权委托撤单
  virtual int ReqCancelOrder(CReqOptCancelOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权委托申报(极速)
  virtual int ReqOrderFlash(CReqOptOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权委托撤单(极速)
  virtual int ReqCancelOrderFlash(CReqOptCancelOrderFlashField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权标的证券锁定解锁
  virtual int ReqUndlStkLock(CReqOptUndlStkLockField *p_pReqField, LONGLONG p_llRequestId);

  // 计算个股期权最大可交易数量
  virtual int ReqMaxTradeQty(CReqOptMaxTradeQtyField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询
  virtual int ReqQryExpendableFund(CReqOptExpendableFundField *p_pReqField, LONGLONG p_llRequestId);

  // 可用合约资产查询
  virtual int ReqQryExpendableCu(CReqOptExpendableCuField *p_pReqField, LONGLONG p_llRequestId);

  // 可用备兑股份查询
  virtual int ReqQryExpendableCvdStk(CReqOptExpendableCvdStkField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权当日委托查询
  virtual int ReqQryCurrDayOrder(CReqOptCurrDayOrderField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权当日成交查询
  virtual int ReqQryCurrDayFill(CReqOptCurrDayFillField *p_pReqField, LONGLONG p_llRequestId);
 
  // 个股期权合约账户查询
  virtual int ReqQryAcct(CReqOptAcctField *p_pReqField, LONGLONG p_llRequestId);

  // 个股期权可撤委托查询
  virtual int ReqQryCanWithdrawOrder(CReqOptCanWithdrawOrderField *p_pReqField, LONGLONG p_llRequestId);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  //组合策略文件查询 2015/8/3
  virtual int ReqQryCombStra(CReqOptCombStraField * p_pReqField,LONGLONG p_llRequestId);

  //组合保证金委托 2015/8/5 
  virtual int ReqCombStraOrder(CReqOptCombStraOrderField* p_pReqField,LONGLONG p_llRequestId);
  
  //可用组合持仓查询 2015/8/5 
  virtual int ReqQryCombStraPos(CReqOptCombStraPosField* p_pReqField,LONGLONG p_llRequestId);
 
  //可用组合持仓明细查询 2015/8/5 
  virtual int ReqQryCombStraPosDetail(CReqOptCombStraPosDetailField* p_pReqField,LONGLONG p_llRequestId);
  
  //个股期权初始保证金 2015/8/10
  virtual int ReqIniMargin(CReqOptIniMarginField* p_pReqField,LONGLONG p_llRequestId);

  //行权指派欠资     2015/8/10
  virtual int ReqQryExeDebt(CReqOptExeDebtField* p_pReqField,LONGLONG p_llRequestId);

  //行权指派欠券       2015/8/10
  virtual int ReqQryExeStkDebt(CReqOptExeStkDebtField* p_pReqField,LONGLONG p_llRequestId);

  //客户风险度         2015/8/10
  virtual int ReqQryRiskLvl(CReqOptRiskLvlField* p_pReqField,LONGLONG p_llRequestId);
  
  //客户结算单查询 2015/8/6
  virtual int ReqQrySettList(CReqOptSettListField* p_pReqField,LONGLONG p_llRequestId);

  //客户结算单确认 2015/8/6
  virtual int ReqSettListConfirm(CReqOptSettListConfirmField* p_pReqField,LONGLONG p_llRequestId);

  //银衍转账 2015/8/3
  virtual int ReqBankDeriTrans(CReqOptBankDeriTransField* p_pReqField,LONGLONG p_llRequestId);
  
  //查询银衍交易配置信息 2015/8/7
  virtual int ReqBankDeriInfo(CReqOptBankDeriInfoField* p_pReqField,LONGLONG p_llRequestId);

  //查询客户签约银行代码 2015/8/7
  virtual int ReqUserBank(CReqOptUserBankField* p_pReqField,LONGLONG p_llRequestId);

  //对账单行权交割流水查询 2015/8/7
  virtual int ReqStateDeliHis(CReqOptStateDeliHisField* p_pReqField,LONGLONG p_llRequestId);

  //银衍转账流水查询 2015/8/7
  virtual int ReqBankDeriTransHis(CReqOptBankDeriTransHisField* p_pReqField,LONGLONG p_llRequestId);

  //银行余额查询 2015/8/7
  virtual int ReqBankBala(CReqOptBankBalaField* p_pReqField,LONGLONG p_llRequestId);

  //密码修改 2015/8/3
  virtual int ReqChangePwd(CReqOptChangePwdField* p_pReqField,LONGLONG p_llRequestId);

  // 可锁定股份查询
  virtual int ReqQryCanLockStk(CReqOptCanLockStkField* p_pReqField, LONGLONG p_llRequestId);

  // 客户适当性匹配产品信息查询
  virtual int ReqQryCustAppropriateMatchInfo(CReqOptQryCustAppropriateMatchInfoField* p_pReqField, LONGLONG p_llRequestId);

  // 客户适当性信息查询
  virtual int ReqQryCustAppropriateInfo(CReqOptQryCustAppropriateInfoField* p_pReqField, LONGLONG p_llRequestId);

  // 客户风险警示书签署
  virtual int ReqCustRiskSign(CReqOptCustRiskSignField* p_pReqField, LONGLONG p_llRequestId);

  // 股票期权行权指令合并申报委托请求
  virtual int ReqCombExe(CReqOptCombExeField *p_pReqField, LONGLONG p_llRequestId);

  // 行权指令合并申报成份合约查询请求
  virtual int ReqQryCombExeLegNum(CReqOptQryCombExeLegNumField *p_pReqField, LONGLONG p_llRequestId);

  // 股票期权当日委托查询[优化]请求
  virtual int ReqQryCurrDayOrderEx(CReqOptQryCurrDayOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // 股票期权当日成交查询[优化]请求
  virtual int ReqQryCurrDayFillEx(CReqOptQryCurrDayFillExField *p_pReqField, LONGLONG p_llRequestId);

  // 可用资金查询[优化]请求
  virtual int ReqQryExpendableFundEx(CReqOptQryExpendableFundExField *p_pReqField, LONGLONG p_llRequestId);

  // 可取资金查询[优化]请求
  virtual int ReqQryDrawFundEx(CReqOptQryDrawFundExField *p_pReqField, LONGLONG p_llRequestId);

  // 可用合约资产查询[优化]请求
  virtual int ReqQryExpendableCuEx(CReqOptQryExpendableCuExField *p_pReqField, LONGLONG p_llRequestId);

  // 历史委托查询(RPC)请求
  virtual int ReqQryHisOrder(CReqOptQryHisOrderField* p_pRspField, LONGLONG p_llRequestId);

  // 历史成交查询(RPC)请求
  virtual int ReqQryHisFill(CReqOptQryHisFillField* p_pRspField, LONGLONG p_llRequestId);

  // 客户结算单(文件)查询请求
  virtual int ReqQrySettListFile(CReqOptQrySettListFileField* p_pRspField, LONGLONG p_llRequestId);

  // 个股期权委托申报(优化)
  virtual int ReqOrderEx(CReqOptOrderExField *p_pReqField, LONGLONG p_llRequestId);

  // 期权做市双边报价（深圳）
  virtual int ReqBilateralQuote(CReqOptBilateralQuoteField *p_pReqField, LONGLONG p_llRequestId);

  // 期权做市双边报价撤单（深圳）
  virtual int ReqBilateralQuotationKill(CReqOptBilateralQuotationKillField *p_pReqField, LONGLONG p_llRequestId);

  // 证券信息查询（期权）
  virtual int ReqQryStkInfo(CReqOptQryStkInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 期权客户登录(新)
  virtual int ReqRpcUserLogin(CReqOptRpcUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 快速订单资金划拨请求
  virtual int ReqRpcFundingTransfer(CReqOptRpcFundingTransferField *p_pReqField, LONGLONG p_llRequestId);

  // 快速订单可划入资金查询请求
  virtual int ReqQryRpcFundingTransfer(CReqOptQryRpcFundingTransferField *p_pReqField, LONGLONG p_llRequestId);
  
  // 快速订单资金划拨流水查询请求
  virtual int ReqQryRpcFundingTransferFlow(CReqOptQryRpcFundingTransferFlowField *p_pReqField, LONGLONG p_llRequestId);
  
  // 临近到期合约资产查询请求
  virtual int ReqQryExpendableCuOfCloseToExpireDate(CReqOptQryExpendableCuOfCloseToExpireDateField *p_pReqField, LONGLONG p_llRequestId);
  
  // 股票期权行权指派明细查询请求
  virtual int ReqQryRpcExeDetail(CReqOptQryRpcExeDetailField *p_pReqField, LONGLONG p_llRequestId);
  
  // 股票期权当日风险通知查询请求
  virtual int ReqQryRpcCurrDayRiskNotify(CReqOptQryRpcCurrDayRiskNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // 股票期权历史风险通知查询请求
  virtual int ReqQryRpcHisRiskNotify(CReqOptQryRpcHisRiskNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // 股票期权当日风险通知查询（风控系统专用）请求
  virtual int ReqQryRpcCurrDayRiskSysNotify(CReqOptQryRpcCurrDayRiskSysNotifyField *p_pReqField, LONGLONG p_llRequestId);
  
  // 股票期权历史风险通知查询（风控系统专用）请求
  virtual int ReqQryRpcHisRiskSysNotify(CReqOptQryRpcHisRiskSysNotifyField *p_pReqField, LONGLONG p_llRequestId);

  // 客户限仓额度查询请求
  virtual int ReqQryPositionLimit(CReqOptQryPositionLimitField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // 用户登录请求响应
  void OnRspUserLogin(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 期权基础信息查询响应
  void OnRspQryBaseInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权委托申报响应
  void OnRspOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权委托撤单响应
  void OnRspCancelOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权标的证券锁定解锁响应
  void OnRspUndlStkLock(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 计算个股期权最大可交易数量响应
  void OnRspMaxTradeQty(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用资金查询响应
  void OnRspQryExpendableFund(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用合约资产查询响应
  void OnRspQryExpendableCu(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用备兑股份查询响应
  void OnRspQryExpendableCvdStk(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权当日委托查询响应
  void OnRspQryCurrDayOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权当日成交查询响应
  void OnRspQryCurrDayFill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权合约账户查询响应
  void OnRspQryAcct(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权可撤委托查询响应
  void OnRspQryCanWithdrawOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  ////////////////////////////////////////////////////////////////////////////////////////
  //组合策略文件查询响应  2015/8/5
  void OnRspQryCombStra(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //组合保证金委托响应  2015/8/3
  void OnRspCombStraOrder(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //可用组合持仓查询响应  2015/8/5
  void OnRspQryCombStraPos(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //可用组合持仓明细查询响应  2015/8/5
  void OnRspQryCombStraPosDetail(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
  
  //个股期权初始保证金 2015/8/10
  void OnRspIniMargin(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //行权指派欠资查询     2015/8/10
  void OnRspQryExeDebt(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //行权指派欠券查询      2015/8/10
  void OnRspQryExeStkDebt(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //客户风险度查询         2015/8/10
  void OnRspQryRiskLvl(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //客户结算单查询 2015/8/6
  void OnRspQrySettList(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
  
  //客户结算单确认 2015/8/6
  void OnRspSettListConfirm(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum); 
   
  //银衍转账响应 2015/8/3
  void OnRspBankDeriTrans(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);
  
  //查询银衍交易配置信息响应 2015/8/7
  void OnRspBankDeriInfo(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //查询客户签约银行代码响应 2015/8/7
  void OnRspUserBank(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //对账单行权交割流水查询响应 2015/8/7
  void OnRspStateDeliHis(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //银衍转账流水查询响应 2015/8/7
  void OnRspBankDeriTransHis(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //银行余额查询响应 2015/8/7
  void OnRspBankBala(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  //密码修改响应 2015/8/3
  void OnRspChangePwd(CFirstSetField* p_pFirstSetField,LONGLONG p_llRequestId,int p_iFieldNum);

  // 可锁定股份查询响应
  void OnRspQryCanLockStk(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 客户适当性匹配产品信息查询响应
  void OnRspQryCustAppropriateMatchInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 客户适当性信息查询响应
  void OnRspQryCustAppropriateInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 客户风险警示书签署响应
  void OnRspCustRiskSign(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 股票期权行权指令合并申报委托响应
  void OnRspCombExe(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 行权指令合并申报成份合约查询响应
  void OnRspQryCombExeLegNum(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 股票期权当日委托查询[优化]响应
  void OnRspQryCurrDayOrderEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 股票期权当日成交查询[优化]响应
  void OnRspQryCurrDayFillEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用资金查询[优化]响应
  void OnRspQryExpendableFundEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可取资金查询[优化]响应
  void OnRspQryDrawFundEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 可用合约资产查询[优化]响应
  void OnRspQryExpendableCuEx(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 历史委托查询(RPC)响应
  void OnRspQryHisOrder(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 历史成交查询(RPC)响应
  void OnRspQryHisFill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 客户结算单(文件)查询响应
  void OnRspQrySettListFile(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 个股期权委托申报(优化)响应
  void OnRspOrderEx(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 期权做市双边报价（深圳）响应
  void OnRspBilateralQuote(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 期权做市双边报价撤单（深圳）响应
  void OnRspBilateralQuotationKill(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 证券信息查询（期权）响应
  void OnRspQryStkInfo(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 期权客户登录(新)响应
  void OnRspRpcUserLogin(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 快速订单资金划拨响应
  void OnRspRpcFundingTransfer(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 快速订单可划入资金查询响应
  void OnRspQryRpcFundingTransfer(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 快速订单资金划拨流水查询响应
  void OnRspQryRpcFundingTransferFlow(CFirstSetField* p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 临近到期合约资产查询响应
  void OnRspQryExpendableCuOfCloseToExpireDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 股票期权行权指派明细查询响应
  void OnRspQryRpcExeDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 股票期权当日风险通知查询响应
  void OnRspQryRpcCurrDayRiskNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 股票期权历史风险通知查询响应
  void OnRspQryRpcHisRiskNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 股票期权当日风险通知查询（风控系统专用）响应
  void OnRspQryRpcCurrDayRiskSysNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 股票期权历史风险通知查询（风控系统专用）响应
  void OnRspQryRpcHisRiskSysNotify(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 客户限仓额度查询响应
  void OnRspQryPositionLimit(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_OPT_TRADE_API_H__