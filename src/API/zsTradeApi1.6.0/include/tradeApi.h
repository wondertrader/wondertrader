#if !defined(__STK_TRADE_API_H__)
#define __STK_TRADE_API_H__

#include "baseApi.h"
#include "tradeApiStruct.h"


class ZSAPI CTradeCallback : virtual public CBaseCallback
{
public:
    // 委托响应
    virtual int OnOrder(STFirstSet *p_pFirstSet, STRspOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 撤单响应
    virtual int OnCancelOrder(STFirstSet *p_pFirstSet, STRspCancelOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 最大可交易数计算响应
    virtual int OnMaxTradeQty(STFirstSet *p_pFirstSet, STRspMaxTradeQty *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 持仓查询响应
    virtual int OnQryHolding(STFirstSet *p_pFirstSet, STRspQryHolding *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 成交查询响应
    virtual int OnQryFill(STFirstSet *p_pFirstSet, STRspQryFill *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 资金查询响应
    virtual int OnQryMoney(STFirstSet *p_pFirstSet, STRspQryMoney *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 委托查询响应
    virtual int OnQryOrder(STFirstSet *p_pFirstSet, STRspQryOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 股东账户查询响应
    virtual int OnQryHolder(STFirstSet *p_pFirstSet, STRspQryHolder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 可撤单委托查询响应
    virtual int OnQryWithdrawableOrder(STFirstSet *p_pFirstSet, STRspQryWithdrawableOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 策略交易注册响应
    virtual int OnAcctRegister(STFirstSet *p_pFirstSet, STRspAcctRegister *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 交易账户登录响应
    virtual int OnTradeLogin(STFirstSet *p_pFirstSet, STRspTradeLogin *p_pRsp, LONGLONG  p_llReqId, int p_iNum) {return 0;}

    // 系统用户登录响应
    virtual int OnLogin(STFirstSet *p_pFirstSet, STRspLogin *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 确认回报
    virtual int OnRtnOrderConfirm(STRtnOrderConfirm *p_pRtnOrderConfirm) { return 0; }

    // 成交回报
    virtual int OnRtnOrderFill(STRtnOrderFill *p_pRtnOrderFill) { return 0; }

public:
    // 融资融券委托下单响应
    virtual int OnOrderCredit(STFirstSet *p_pFirstSet, STRspOrderCredit *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 融资融券直接还款响应
    virtual int OnRepay(STFirstSet *p_pFirstSet, STRspRepay *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 融资融券持仓查询响应
    virtual int OnQryHoldingCredit(STFirstSet *p_pFirstSet, STRspQryHoldingCredit *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 融资融券标的券信息查询响应
    virtual int OnQryUndlStkInfo(STFirstSet *p_pFirstSet, STRspQryUndlStkInfo *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 融资融券担保证券信息查询响应
    virtual int OnQryColStkInfo(STFirstSet *p_pFirstSet, STRspQryColStkInfo *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 融资融券合约查询响应
    virtual int OnQryContract(STFirstSet *p_pFirstSet, STRspQryContract *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 信用客户资产负债查询响应
    virtual int OnQryCustDebts(STFirstSet *p_pFirstSet, STRspQryCustDebts *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

public:
    // 证券市值额度查询响应
    virtual int OnQryMktQuota(STFirstSet *p_pFirstSet, STRspQryMktQuota *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 当日新股信息查询响应
    virtual int OnQryIpoInfo(STFirstSet *p_pFirstSet, STRspQryIpoInfo *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 转帐银行业务信息查询响应
    virtual int OnQryBankInfo(STFirstSet *p_pFirstSet, STRspQryBankInfo *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 银证转帐响应
    virtual int OnBankStkTrans(STFirstSet *p_pFirstSet, STRspBankStkTrans *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 银行账户余额查询响应
    virtual int OnQryBankBalance(STFirstSet *p_pFirstSet, STRspQryBankBalance *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 银证转帐查询响应
    virtual int OnQryBankStkTransInfo(STFirstSet *p_pFirstSet, STRspQryBankStkTransInfo *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 修改交易密码响应
    virtual int OnModifyTradePwd(STFirstSet *p_pFirstSet, STRspModifyTradePwd *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 修改资金密码响应
    virtual int OnModifyFundPwd(STFirstSet *p_pFirstSet, STRspModifyFundPwd *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 集中交易柜台资金查询响应
    virtual int OnQryRpcFund(STFirstSet *p_pFirstSet, STRspQryRpcFund *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 资金划拨请求响应
    virtual int OnFundTransfer(STFirstSet *p_pFirstSet, STRspFundTransfer *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 银证转账银行账号查询响应
    virtual int OnQryBankAcct(STFirstSet *p_pFirstSet, STRspQryBankAcct *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

public:
    // 期权委托申报响应
    virtual int OnOrderOpt(STFirstSet *p_pFirstSet, STRspOrderOpt *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权委托撤单响应
    virtual int OnOptCancelOrder(STFirstSet *p_pFirstSet, STRspOptCancelOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权可用资金查询响应
    virtual int OnOptQryMoney(STFirstSet *p_pFirstSet, STRspOptQryMoney *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权持仓查询响应
    virtual int OnOptQryHolding(STFirstSet *p_pFirstSet, STRspOptQryHolding *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权当日委托查询响应
    virtual int OnOptQryOrder(STFirstSet *p_pFirstSet, STRspOptQryOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权当日成交查询响应
    virtual int OnOptQryFill(STFirstSet *p_pFirstSet, STRspOptQryFill *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

    // 期权可撤委托查询响应
    virtual int OnOptQryWithdrawableOrder(STFirstSet *p_pFirstSet, STRspOptQryWithdrawableOrder *p_pRsp, LONGLONG  p_llReqId, int p_iNum) { return 0; }

private:

};

class ZSAPI CTradeApi : public CBaseApi
{
public:
    CTradeApi(void);
    virtual ~CTradeApi(void);

    // 委托请求
    int Order(STReqOrder *p_pReq, LONGLONG p_llReqId);

    // 撤单请求
    int CancelOrder(STReqCancelOrder *p_pReq, LONGLONG p_llReqId);

    // 最大可交易数计算请求
    int MaxTradeQty(STReqMaxTradeQty *p_pReq, LONGLONG p_llReqId);

    // 持仓查询请求
    int QryHolding(STReqQryHolding *p_pReq, LONGLONG p_llReqId);

    // 成交查询请求
    int QryFill(STReqQryFill *p_pReq, LONGLONG p_llReqId);

    // 资金查询请求
    int QryMoney(STReqQryMoney *p_pReq, LONGLONG p_llReqId);

    // 委托查询请求
    int QryOrder(STReqQryOrder *p_pReq, LONGLONG p_llReqId);

    // 股东账户查询请求
    int QryHolder(STReqQryHolder *p_pReq, LONGLONG p_llReqId);

    // 可撤单委托查询请求
    int QryWithdrawableOrder(STReqQryWithdrawableOrder *p_pReq, LONGLONG p_llReqId);

    // 策略交易注册请求
    int AcctRegister(STReqAcctRegister *p_pReq, LONGLONG p_llReqId);

    // 交易账户登录请求
    int TradeLogin(STReqTradeLogin *p_pReq, LONGLONG p_llReqId);

    // 系统用户登录请求
    int Login(STReqLogin *p_pReq, LONGLONG p_llReqId);

public:
    // 融资融券委托下单请求
    int OrderCredit(STReqOrderCredit *p_pReq, LONGLONG p_llReqId);

    // 融资融券直接还款请求
    int Repay(STReqRepay *p_pReq, LONGLONG p_llReqId);

    // 融资融券持仓查询请求
    int QryHoldingCredit(STReqQryHoldingCredit *p_pReq, LONGLONG p_llReqId);

    // 融资融券标的券信息查询请求
    int QryUndlStkInfo(STReqQryUndlStkInfo *p_pReq, LONGLONG p_llReqId);

    // 融资融券担保证券信息查询请求
    int QryColStkInfo(STReqQryColStkInfo *p_pReq, LONGLONG p_llReqId);

    // 融资融券合约查询请求
    int QryContract(STReqQryContract *p_pReq, LONGLONG p_llReqId);

    // 信用客户资产负债查询请求
    int QryCustDebts(STReqQryCustDebts *p_pReq, LONGLONG p_llReqId);

public:
    // 证券市值额度查询请求
    int QryMktQuota(STReqQryMktQuota *p_pReq, LONGLONG p_llReqId);

    // 当日新股信息查询请求
    int QryIpoInfo(STReqQryIpoInfo *p_pReq, LONGLONG p_llReqId);

    // 转帐银行业务信息查询请求
    int QryBankInfo(STReqQryBankInfo *p_pReq, LONGLONG p_llReqId);

    // 银证转帐请求
    int BankStkTrans(STReqBankStkTrans *p_pReq, LONGLONG p_llReqId);

    // 银行账户余额查询请求
    int QryBankBalance(STReqQryBankBalance *p_pReq, LONGLONG p_llReqId);

    // 银证转帐查询请求
    int QryBankStkTransInfo(STReqQryBankStkTransInfo *p_pReq, LONGLONG p_llReqId);

    // 修改交易密码请求
    int ModifyTradePwd(STReqModifyTradePwd *p_pReq, LONGLONG p_llReqId);

    // 修改资金密码请求
    int ModifyFundPwd(STReqModifyFundPwd *p_pReq, LONGLONG p_llReqId);

    // 集中交易柜台资金查询请求
    int QryRpcFund(STReqQryRpcFund *p_pReq, LONGLONG p_llReqId);

    // 资金划拨请求
    int FundTransfer(STReqFundTransfer *p_pReq, LONGLONG p_llReqId);

    // 银证转账银行账号查询请求
    int QryBankAcct(STReqQryBankAcct *p_pReq, LONGLONG p_llReqId);

public:
    // 期权委托申报请求
    int OrderOpt(STReqOrderOpt *p_pReq, LONGLONG p_llReqId);

    // 期权委托撤单请求
    int OptCancelOrder(STReqOptCancelOrder *p_pReq, LONGLONG p_llReqId);

    // 期权可用资金查询请求
    int OptQryMoney(STReqOptQryMoney *p_pReq, LONGLONG p_llReqId);

    // 期权持仓查询请求
    int OptQryHolding(STReqOptQryHolding *p_pReq, LONGLONG p_llReqId);

    // 期权当日委托查询请求
    int OptQryOrder(STReqOptQryOrder *p_pReq, LONGLONG p_llReqId);

    // 期权当日成交查询请求
    int OptQryFill(STReqOptQryFill *p_pReq, LONGLONG p_llReqId);

    // 期权可撤委托查询请求
    int OptQryWithdrawableOrder(STReqOptQryWithdrawableOrder *p_pReq, LONGLONG p_llReqId);

public:
    virtual void OnReqCallback(const int p_iMsgId, const void *p_pszDataBuff, int p_iDataLen, LONGLONG p_llReqId);
    void OnSubCallback(const char *p_pszMsg, const void *p_pszDataBuff, int p_iDataLen);

private:
    void OnAcctRegister(STFirstSet *p_pFirstSet, STRspAcctRegister *p_pRsp);
    void OnLogin(STFirstSet *p_pFirstSet, STRspLogin *p_pRsp);
    void OnTradeLogin(STFirstSet *p_pFirstSet, STRspTradeLogin *p_pRsp);
};


#endif  //__STK_TRADE_API_H__