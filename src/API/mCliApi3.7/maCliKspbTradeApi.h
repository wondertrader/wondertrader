#if !defined(__MA_CLI_KSPB_TRADE_API_H__)
#define __MA_CLI_KSPB_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliKspbTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliKspbTradeSpi : virtual public CCliTradeSpi
{
public:
  // 确认回报
  virtual int OnRtnOrderConfirm(CRtnKspbRpcOrderConfirmField *p_pRtnOrderConfirmField) {return 0;}
  // 成交回报
  virtual int OnRtnOrderFill(CRtnKspbRpcOrderFillField *p_pRtnOrderFillField) {return 0;}
  // 用户登陆
  virtual int OnRspRpcLogin(CFirstSetField *p_pFirstSetField, CRspKspbRpcLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 买卖委托
  virtual int OnRspRpcOrder(CFirstSetField *p_pFirstSetField, CRspKspbRpcOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托撤单
  virtual int OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, CRspKspbRpcOrderCancelField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股份查询
  virtual int OnRspRpcQryStkDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryStkDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金查询
  virtual int OnRspRpcQryFund(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryFundField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 委托查询
  virtual int OnRspRpcQryOrderDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryOrderDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 成交查询
  virtual int OnRspRpcQryMatchDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryMatchDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 证券信息查询
  virtual int OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, CRspKspbRpcStkInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 股东查询
  virtual int OnRspRpcQryTrdacct(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryTrdacctField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银行信息查询
  virtual int OnRspRpcQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankTransBasicInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 银证转账
  virtual int OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankStkTransField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 转账流水查询
  virtual int OnRspRpcQryBankStkTrans(CFirstSetField *p_pFirstSetField, CRspKspbRpcBankStkTransInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史委托查询
  virtual int OnRspRpcQryHistoryOrderDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryHistoryOrderDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 历史成交查询
  virtual int OnRspRpcQryHistoryMatchDetail(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryHistoryMatchDetailField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 资金股份流水查询
  virtual int OnRspRpcQryStatement(CFirstSetField *p_pFirstSetField, CRspKspbRpcStatementField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 交割单查询
  virtual int OnRspRpcQryDeliOrder(CFirstSetField *p_pFirstSetField, CRspKspbRpcQryDeliOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliKspbTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliKspbTradeApi(void);

  // 析构函数
  virtual ~CCliKspbTradeApi(void);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);
  
  // 用户登陆
  int ReqRpcLogin(CReqKspbRpcLoginField *p_pReqField, LONGLONG p_llRequestId);
  // 买卖委托
  int ReqRpcOrder(CReqKspbRpcOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 委托撤单
  int ReqRpcOrderCancel(CReqKspbRpcOrderCancelField *p_pReqField, LONGLONG p_llRequestId);
  // 股份查询
  int ReqRpcQryStkDetail(CReqKspbRpcQryStkDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 资金查询
  int ReqRpcQryFund(CReqKspbRpcQryFundField *p_pReqField, LONGLONG p_llRequestId);
  // 委托查询
  int ReqRpcQryOrderDetail(CReqKspbRpcQryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 成交查询
  int ReqRpcQryMatchDetail(CReqKspbRpcQryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 证券信息查询
  int ReqRpcStkInfo(CReqKspbRpcStkInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 股东查询
  int ReqRpcQryTrdacct(CReqKspbRpcQryTrdacctField *p_pReqField, LONGLONG p_llRequestId);
  // 银行信息查询
  int ReqRpcQryBankTransBasicInfo(CReqKspbRpcBankTransBasicInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 银证转账
  int ReqRpcBankStkTrans(CReqKspbRpcBankStkTransField *p_pReqField, LONGLONG p_llRequestId);
  // 转账流水查询
  int ReqRpcQryBankStkTrans(CReqKspbRpcBankStkTransInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 历史委托查询
  int ReqRpcQryHistoryOrderDetail(CReqKspbRpcQryHistoryOrderDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 历史成交查询
  int ReqRpcQryHistoryMatchDetail(CReqKspbRpcQryHistoryMatchDetailField *p_pReqField, LONGLONG p_llRequestId);
  // 资金股份流水查询
  int ReqRpcQryStatement(CReqKspbRpcStatementField *p_pReqField, LONGLONG p_llRequestId);
  // 交割单查询
  int ReqRpcQryDeliOrder(CReqKspbRpcQryDeliOrderField *p_pReqField, LONGLONG p_llRequestId);

private:
  // 用户登陆
  void OnRspRpcLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 买卖委托
  void OnRspRpcOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托撤单
  void OnRspRpcOrderCancel(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股份查询
  void OnRspRpcQryStkDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金查询
  void OnRspRpcQryFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 委托查询
  void OnRspRpcQryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 成交查询
  void OnRspRpcQryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 证券信息查询
  void OnRspRpcStkInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股东查询
  void OnRspRpcQryTrdacct(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银行信息查询
  void OnRspRpcQryBankTransBasicInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 银证转账
  void OnRspRpcBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 转账流水查询
  void OnRspRpcQryBankStkTrans(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史委托查询
  void OnRspRpcQryHistoryOrderDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史成交查询
  void OnRspRpcQryHistoryMatchDetail(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 资金股份流水查询
  void OnRspRpcQryStatement(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 交割单查询
  void OnRspRpcQryDeliOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_KSPB_TRADE_API_H__