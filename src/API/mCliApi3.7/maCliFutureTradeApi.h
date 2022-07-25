#if !defined(__MA_CLI_FUTURE_TRADE_API_H__)
#define __MA_CLI_FUTURE_TRADE_API_H__

#include "maCliApi.h"
#include "maCliTradeApi.h"
#include "maCliFutureTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliFutureTradeSpi : virtual public CCliTradeSpi
{
public:
  // 用户登录请求响应
  virtual int OnRspUserLogin(CFirstSetField *p_pFirstSetField, CRspFutureUserLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //期货登出
  virtual int OnRspUserLogout(CFirstSetField *p_pFirstSet, CRspFutureUserLogoutField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //报单录入
  virtual int OnRspOrder(CFirstSetField *p_pFirstSet, CRspFutureOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //报单操作
  virtual int OnRspChgOrder(CFirstSetField *p_pFirstSet, CRspFutureChgOrderField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //报单查询
  virtual int OnRspQryOrderInfo(CFirstSetField *p_pFirstSet, CRspFutureOrderInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //成交查询
  virtual int OnRspQryMatchInfo(CFirstSetField *p_pFirstSet, CRspFutureMatchInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //投资者查询
  virtual int OnRspQryCustInfo(CFirstSetField *p_pFirstSet, CRspFutureCustInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //投资者持仓查询
  virtual int OnRspQryPosInfo(CFirstSetField *p_pFirstSet, CRspFuturePosInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //合约查询
  virtual int OnRspQryContractInfo(CFirstSetField *p_pFirstSet, CRspFutureContractInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //投资者资金查询
  virtual int OnRspQryFundInfo(CFirstSetField *p_pFirstSet, CRspFutureFundInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //最大可开仓数量
  virtual int OnRspQryAvlQty(CFirstSetField *p_pFirstSet, CRspFutureAvlQtyField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //最大可平仓数量
  virtual int OnRspQryAvlPos(CFirstSetField *p_pFirstSet, CRspFutureAvlPosField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //客户量查询
  virtual int OnRspQryUsersOnline(CFirstSetField *p_pFirstSet, CRspFutureUsersOnlineField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //手续费率查询
  virtual int OnRspQryFee(CFirstSetField *p_pFirstSet, CRspFutureFeeField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //保证金率查询
  virtual int OnRspQryMargin(CFirstSetField *p_pFirstSet, CRspFutureMarginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //交易日期查询
  virtual int OnRspQryDateInfo(CFirstSetField *p_pFirstSet, CRspFutureDateInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //委托推送
  virtual int OnRtnOrder(CRtnFutureOrderField* p_pField){return 0;}
  //成交推送
  virtual int OnRtnMatch(CRtnFutureMatchField* p_pField){return 0;}
private:

};

class MATRADEAPI CCliFutureTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliFutureTradeApi(void);
  // 析构函数
  virtual ~CCliFutureTradeApi(void);
  
  // 用户登录请求
  virtual int ReqUserLogin(CReqFutureUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //期货登出
  virtual int ReqUserLogout(CReqFutureUserLogoutField *p_pReqField, LONGLONG p_llRequestId);  
  //报单录入
  virtual int ReqOrder(CReqFutureOrderField *p_pReqField, LONGLONG p_llRequestId);  
  //报单操作
  virtual int ReqChgOrder(CReqFutureChgOrderField *p_pReqField, LONGLONG p_llRequestId);  
  //报单查询
  virtual int ReqQryOrderInfo(CReqFutureOrderInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //成交查询
  virtual int ReqQryMatchInfo(CReqFutureMatchInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //投资者查询
  virtual int ReqQryCustInfo(CReqFutureCustInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //投资者持仓查询
  virtual int ReqQryPosInfo(CReqFuturePosInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //合约查询
  virtual int ReqQryContractInfo(CReqFutureContractInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //投资者资金查询
  virtual int ReqQryFundInfo(CReqFutureFundInfoField *p_pReqField, LONGLONG p_llRequestId);  
  //最大可开仓数量
  virtual int ReqQryAvlQty(CReqFutureAvlQtyField *p_pReqField, LONGLONG p_llRequestId);  
  //最大可平仓数量
  virtual int ReqQryAvlPos(CReqFutureAvlPosField *p_pReqField, LONGLONG p_llRequestId);  
  //客户量查询
  virtual int ReqQryUsersOnline(CReqFutureUsersOnlineField *p_pReqField, LONGLONG p_llRequestId);  
  //手续费率查询
  virtual int ReqQryFee(CReqFutureFeeField *p_pReqField, LONGLONG p_llRequestId);  
  //保证金率查询
  virtual int ReqQryMargin(CReqFutureMarginField *p_pReqField, LONGLONG p_llRequestId);  
  //交易日期查询
  virtual int ReqQryDateInfo(CReqFutureDateInfoField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);
  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);
  
private:
  // 用户登录请求响应
  void OnRspUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);  
  //期货登出
  void OnRspUserLogout(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //报单录入
  void OnRspOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //报单操作
  void OnRspChgOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //报单查询
  void OnRspQryOrderInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //成交查询
  void OnRspQryMatchInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //投资者查询
  void OnRspQryCustInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //投资者持仓查询
  void OnRspQryPosInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //合约查询
  void OnRspQryContractInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //投资者资金查询
  void OnRspQryFundInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //最大可开仓数量
  void OnRspQryAvlQty(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //最大可平仓数量
  void OnRspQryAvlPos(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //客户量查询
  void OnRspQryUsersOnline(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //手续费率查询
  void OnRspQryFee(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //保证金率查询
  void OnRspQryMargin(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);  
  //交易日期查询
  void OnRspQryDateInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

private:

};

END_NAMESPACE_MACLI

#endif//__MA_CLI_FUTURE_TRADE_API_H__