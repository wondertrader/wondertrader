#if !defined(__MA_CLI_COS_TRADE_API_H__)
#define __MA_CLI_COS_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliCosTradeApiStruct.h"
#include "maCliStkTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliCosTradeSpi : virtual public CCliTradeSpi
{
public:
  // 股票用户登录请求响应
  virtual int OnRspStkUserLogin(CFirstSetField *p_pFirstSetField, CRspStkUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 信用用户登录请求响应
  virtual int OnRspFislUserLogin(CFirstSetField *p_pFirstSetField, CRspFislUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 期权用户登录请求响应
  virtual int OnRspOptUserLogin(CFirstSetField *p_pFirstSetField, CRspOptUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 期权用户登录请求(新)响应
  virtual int OnRspOptRpcUserLogin(CFirstSetField *p_pFirstSetField, CRspOptRpcUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 期货用户登录请求响应
  virtual int OnRspFutureUserLogin(CFirstSetField *p_pFirstSetField, CRspFutureUserLoginField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //子系统查询
  virtual int OnRspQryConf(CFirstSetField *p_pFirstSet, CRspCosConfField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //子系统设置
  virtual int OnRspChgConf(CFirstSetField *p_pFirstSet, CRspCosChgConfField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化委托
  virtual int OnRspOrder(CFirstSetField *p_pFirstSet, CRspCosOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //委托撤单
  virtual int OnRspCancelOrder(CFirstSetField *p_pFirstSet, CRspCosCancelOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //委托改单
  virtual int OnRspChgOrder(CFirstSetField *p_pFirstSet, CRspCosChgOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化组合委托
  virtual int OnRspCombOrder(CFirstSetField *p_pFirstSet, CRspCosCombOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //节点变更
  virtual int OnRspChgNode(CFirstSetField *p_pFirstSet, CRspCosChgNodeField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //节点变更上线
  virtual int OnRspChgNodeOnline(CFirstSetField *p_pFirstSet, CRspCosChgNodeOnlineField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化委托查询
  virtual int OnRspQryOrderInfo(CFirstSetField *p_pFirstSet, CRspCosOrderInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //客户所在节点编号查询
  virtual int OnRspQryNodeInfo(CFirstSetField *p_pFirstSet, CRspCosNodeInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //节点的客户查询
  virtual int OnRspQryNodeCustInfo(CFirstSetField *p_pFirstSet, CRspCosNodeCustInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //接口配置查询
  virtual int OnRspQryItfInfo(CFirstSetField *p_pFirstSet, CRspCosItfInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //接口配置设置
  virtual int OnRspChgItf(CFirstSetField *p_pFirstSet, CRspCosChgItfField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化可撤单委托查询
  virtual int OnRspQryCanWithdrawOrder(CFirstSetField *p_pFirstSet, CRspCosCanWithdrawOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化成交查询
  virtual int OnRspQryMatch(CFirstSetField *p_pFirstSet, CRspCosMatchField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //量化子委托查询
  virtual int OnRspQrySubOrder(CFirstSetField *p_pFirstSet, CRspCosSubOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //历史委托查询
  virtual int OnRspQryHisOrderInfo(CFirstSetField *p_pFirstSet, CRspCosHisOrderInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //历史成交查询
  virtual int OnRspQryHisFillInfo(CFirstSetField *p_pFirstSet, CRspCosHisFillInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //组合委托查询
  virtual int OnRspQryCombOrderInfo(CFirstSetField *p_pFirstSet, CRspCosCombOrderInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //资产用户查询
  virtual int OnRspQryCuacctCodeInfo(CFirstSetField *p_pFirstSet, CRspCosCuacctCodeInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //节点账户查询
  virtual int OnRspQryNodeUsers(CFirstSetField *p_pFirstSet, CRspCosNodeUsersField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //根据组合委托编号查询组合委托
  virtual int OnRspQryCombOrderInfoEx(CFirstSetField *p_pFirstSet, CRspCosCombOrderInfoExField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //行情处理快照
  virtual int OnRspSnapshot(CFirstSetField *p_pFirstSet, CRspCosSnapshotField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //行情回放功能
  virtual int OnRspReplay(CFirstSetField *p_pFirstSet, CRspCosReplayField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //深度行情查询委托
  virtual int OnRspQryMktDataInfo(CFirstSetField *p_pFirstSet, CRspCosMktDataInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //行情记录功能
  virtual int OnRspRecordMktData(CFirstSetField *p_pFirstSet, CRspCosRecordMktDataField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //行情回放进度查询
  virtual int OnRspQryReplayInfo(CFirstSetField *p_pFirstSet, CRspCosReplayInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //历史行情查询委托
  virtual int OnRspQryHisMktData(CFirstSetField *p_pFirstSet, CRspCosHisMktDataField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //数据同步
  virtual int OnRspSync(CFirstSetField *p_pFirstSet, CRspCosSyncField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //资产账户开户
  virtual int OnRspOpenAccount(CFirstSetField *p_pFirstSet, CRspCosOpenAccountField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //资产账户销户
  virtual int OnRspCloseAccount(CFirstSetField *p_pFirstSet, CRspCosCloseAccountField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //资金同步
  virtual int OnRspFundSync(CFirstSetField *p_pFirstSet, CRspCosFundSyncField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //股份同步
  virtual int OnRspStkSync(CFirstSetField *p_pFirstSet, CRspCosStkSyncField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //交易日切
  virtual int OnRspCutoff(CFirstSetField *p_pFirstSet, CRspCosCutoffField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 可用资金查询响应
  virtual int OnRspQryExpendableFund(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableFundField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 可用股份查询响应
  virtual int OnRspQryExpendableShares(CFirstSetField *p_pFirstSetField, CRspStkQryExpendableSharesField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  //成交信息包括四类: 
  //  1.正常委托的成交信息
  //   2.撤单所产生的成交信息（委托已申报至交易所）
  //   3.内部撤单的成交信息（委托未申报至交易所）
  //   4.非法委托的撤单成交信息（委托被交易所拒绝）
  virtual int OnRtnOrder(CRtnOrderField * p_pOrderField){return 0;}
  //量化委托确认回报
  virtual int OnRtnTsuOrder(CRtnTsuOrderField* p_pOrderField){return 0;}
  ///////////////////////////////////////////////////////////////////////  
  //内部接口功能
  //成交回报数据
  virtual int OnRspMatchReport(CFirstSetField *p_pFirstSet, CRspCosMatchReportField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //委托应答数据
  virtual int OnRspOrderAnswer(CFirstSetField *p_pFirstSet, CRspCosOrderAnswerField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  //自选股编辑
  virtual int OnRspPortfolio(CFirstSetField *p_pFirstSet, CRspCosPortfolioField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //自选股查询
  virtual int OnRspQryPortfolioInfo(CFirstSetField *p_pFirstSet, CRspCosPortfolioInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //ETF篮子委托
  virtual int OnRspEtfBasketOrder(CFirstSetField *p_pFirstSet, CRspCosEtfBasketOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //自定义篮子委托
  virtual int OnRspCustomBasketOrder(CFirstSetField *p_pFirstSet, CRspCosCustomBasketOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //证券信息查询
  virtual int OnRspQryStkInfo(CFirstSetField *p_pFirstSet, CRspCosQryStkInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //算法单委托响应
  virtual int OnRspOrderStrategy(CFirstSetField *p_pFirstSet, CRspCosOrderStrategyField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //策略启停响应
  virtual int OnRspStrategySwitch(CFirstSetField *p_pFirstSet, CRspCosStrategySwitchField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //日切交易日查询响应
  virtual int OnRspQrySettDate(CFirstSetField *p_pFirstSetField, CRspStkQrySettDateField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //获取下一交易日历响应
  virtual int OnRspQryTradeDate(CFirstSetField *p_pFirstSetField, CRspCosQryTradeDateField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //期权组合保证金和行权指令合并委托响应
  virtual int OnRspCombStraAndExerciseOrder(CFirstSetField *p_pFirstSetField, CRspCosCombStraAndExerciseOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //期权信息查询响应
  virtual int OnRspQryOptInfo(CFirstSetField *p_pFirstSetField, CRspCosQryOptInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //期货信息查询响应
  virtual int OnRspQryFutInfo(CFirstSetField *p_pFirstSetField, CRspCosQryFutInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //现货期权期货信息查询响应
  virtual int OnRspQryStkOptFutInfo(CFirstSetField *p_pFirstSetField, CRspCosQryStkOptFutInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //历史子委托查询响应
  virtual int OnRspQryHisSubOrder(CFirstSetField *p_pFirstSetField, CRspCosQryHisSubOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //股本变迁信息查询响应
  virtual int OnRspQryChgShareCapital(CFirstSetField *p_pFirstSetField, CRspCosQryChgShareCapitalField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  //交易时间片查询响应
  virtual int OnRspQryTrdTimeSlice(CFirstSetField *p_pFirstSetField, CRspCosQryTrdTimeSliceField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 策略延期响应
  virtual int OnRspStrategyExtension(CFirstSetField *p_pFirstSetField, CRspCosStrategyExtensionField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  // 策略委托查询(通用)响应
  virtual int OnRspQryStrategyOrder(CFirstSetField *p_pFirstSetField, CRspCosQryStrategyOrderField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
};

class MATRADEAPI CCliCosTradeApi : virtual public CCliTradeApi
{
public:
  // 默认构造函数
  CCliCosTradeApi(void);
  // 析构函数
  virtual ~CCliCosTradeApi(void);
  
  //用户现货登录请求
  virtual int ReqStkUserLogin(CReqStkUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //用户信用登录请求
  virtual int ReqFislUserLogin(CReqFislUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //用户期权登录请求
  virtual int ReqOptUserLogin(CReqOptUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //用户期权登录请求(新)
  virtual int ReqOptRpcUserLogin(CReqOptRpcUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //用户期货登录请求
  virtual int ReqFutureUserLogin(CReqFutureUserLoginField *p_pReqField, LONGLONG p_llRequestId);
  //子系统查询
  virtual int ReqQryConf(CReqCosConfField *p_pReqField, LONGLONG p_llRequestId);
  //子系统设置
  virtual int ReqChgConf(CReqCosChgConfField *p_pReqField, LONGLONG p_llRequestId);
  //量化委托
  virtual int ReqOrder(CReqCosOrderField *p_pReqField, LONGLONG p_llRequestId);
  //委托撤单
  virtual int ReqCancelOrder(CReqCosCancelOrderField *p_pReqField, LONGLONG p_llRequestId);
  //委托改单
  virtual int ReqChgOrder(CReqCosChgOrderField *p_pReqField, LONGLONG p_llRequestId);
  //量化组合委托
  virtual int ReqCombOrder(CReqCosCombOrderField *p_pReqField, LONGLONG p_llRequestId);
  //节点变更
  virtual int ReqChgNode(CReqCosChgNodeField *p_pReqField, LONGLONG p_llRequestId);
  //节点变更上线
  virtual int ReqChgNodeOnline(CReqCosChgNodeOnlineField *p_pReqField, LONGLONG p_llRequestId);
  //量化委托查询
  virtual int ReqQryOrderInfo(CReqCosOrderInfoField *p_pReqField, LONGLONG p_llRequestId);
  //客户所在节点编号查询
  virtual int ReqQryNodeInfo(CReqCosNodeInfoField *p_pReqField, LONGLONG p_llRequestId);
  //节点的客户查询
  virtual int ReqQryNodeCustInfo(CReqCosNodeCustInfoField *p_pReqField, LONGLONG p_llRequestId);
  //接口配置查询
  virtual int ReqQryItfInfo(CReqCosItfInfoField *p_pReqField, LONGLONG p_llRequestId);
  //接口设置
  virtual int ReqChgItf(CReqCosChgItfField *p_pReqField, LONGLONG p_llRequestId);
  //量化可撤单委托查询
  virtual int ReqQryCanWithdrawOrder(CReqCosCanWithdrawOrderField *p_pReqField, LONGLONG p_llRequestId);
  //量化成交查询
  virtual int ReqQryMatch(CReqCosMatchField *p_pReqField, LONGLONG p_llRequestId);
  //量化子委托查询
  virtual int ReqQrySubOrder(CReqCosSubOrderField *p_pReqField, LONGLONG p_llRequestId);
  //历史委托查询
  virtual int ReqQryHisOrderInfo(CReqCosHisOrderInfoField *p_pReqField, LONGLONG p_llRequestId);
  //历史成交查询
  virtual int ReqQryHisFillInfo(CReqCosHisFillInfoField *p_pReqField, LONGLONG p_llRequestId);
  //组合委托查询
  virtual int ReqQryCombOrderInfo(CReqCosCombOrderInfoField *p_pReqField, LONGLONG p_llRequestId);
  //资产用户查询
  virtual int ReqQryCuacctCodeInfo(CReqCosCuacctCodeInfoField *p_pReqField, LONGLONG p_llRequestId);
  //节点账户查询
  virtual int ReqQryNodeUsers(CReqCosNodeUsersField *p_pReqField, LONGLONG p_llRequestId);
  //根据组合委托编号查询组合委托
  virtual int ReqQryCombOrderInfoEx(CReqCosCombOrderInfoExField *p_pReqField, LONGLONG p_llRequestId);
  //行情处理快照
  virtual int ReqSnapshot(CReqCosSnapshotField *p_pReqField, LONGLONG p_llRequestId);
  //行情回放功能
  virtual int ReqReplay(CReqCosReplayField *p_pReqField, LONGLONG p_llRequestId);
  //深度行情查询委托
  virtual int ReqQryMktDataInfo(CReqCosMktDataInfoField *p_pReqField, LONGLONG p_llRequestId);
  //行情记录功能
  virtual int ReqRecordMktData(CReqCosRecordMktDataField *p_pReqField, LONGLONG p_llRequestId);
  //行情回放进度查询
  virtual int ReqQryReplayInfo(CReqCosReplayInfoField *p_pReqField, LONGLONG p_llRequestId);
  //历史行情查询委托
  virtual int ReqQryHisMktData(CReqCosHisMktDataField *p_pReqField, LONGLONG p_llRequestId);
  //数据同步
  virtual int ReqSync(CReqCosSyncField *p_pReqField, LONGLONG p_llRequestId);
  //资产账户开户
  virtual int ReqOpenAccount(CReqCosOpenAccountField *p_pReqField, LONGLONG p_llRequestId);
  //资产账户销户
  virtual int ReqCloseAccount(CReqCosCloseAccountField *p_pReqField, LONGLONG p_llRequestId);
  //资金同步
  virtual int ReqFundSync(CReqCosFundSyncField *p_pReqField, LONGLONG p_llRequestId);
  //股份同步
  virtual int ReqStkSync(CReqCosStkSyncField *p_pReqField, LONGLONG p_llRequestId);
  //交易日切
  virtual int ReqCutoff(CReqCosCutoffField *p_pReqField, LONGLONG p_llRequestId);
  // 可用资金查询请求
  virtual int ReqQryExpendableFund(CReqStkQryExpendableFundField *p_pReqField, LONGLONG p_llRequestId);
  // 可用股份查询请求
  virtual int ReqQryExpendableShares(CReqStkQryExpendableSharesField *p_pReqField, LONGLONG p_llRequestId);

  //////////////////////////////////////////////////////////////////////////////////////////////
  //内部接口功能
  //成交回报数据
  virtual int ReqMatchReport(CReqCosMatchReportField *p_pReqField, LONGLONG p_llRequestId);
  //委托应答数据
  virtual int ReqOrderAnswer(CReqCosOrderAnswerField *p_pReqField, LONGLONG p_llRequestId);
  
  //自选股编辑
  virtual int ReqPortfolio(CReqCosPortfolioField *p_pReqField, LONGLONG p_llRequestId);
  //自选股查询
  virtual int ReqQryPortfolioInfo(CReqCosPortfolioInfoField *p_pReqField, LONGLONG p_llRequestId);
  //ETF篮子委托
  virtual int ReqEtfBasketOrder(CReqCosEtfBasketOrderField *p_pReqField, LONGLONG p_llRequestId);
  //自定义篮子委托
  virtual int ReqCustomBasketOrder(CReqCosCustomBasketOrderField *p_pReqField, LONGLONG p_llRequestId);
  //证券信息查询
  virtual int ReqQryStkInfo(CReqCosQryStkInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 算法单委托请求
  virtual int ReqOrderStrategy(CReqCosOrderStrategyField *p_pReqField, LONGLONG p_llRequestId);
  // 策略启停请求
  virtual int ReqStrategySwitch(CReqCosStrategySwitchField *p_pReqField, LONGLONG p_llRequestId);
  // 日切交易日查询请求
  virtual int ReqQrySettDate(CReqStkQrySettDateField *p_pReqField, LONGLONG p_llRequestId);
  // 获取下一交易日历请求
  virtual int ReqQryTradeDate(CReqCosQryTradeDateField *p_pReqField, LONGLONG p_llRequestId);
  // 期权组合保证金和行权指令合并委托请求
  virtual int ReqCombStraAndExerciseOrder(CReqCosCombStraAndExerciseOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 期权信息查询请求
  virtual int ReqQryOptInfo(CReqCosQryOptInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 期货信息查询请求
  virtual int ReqQryFutInfo(CReqCosQryFutInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 交易品种信息查询[快速]请求
  virtual int ReqQryStkOptFutInfo(CReqCosQryStkOptFutInfoField *p_pReqField, LONGLONG p_llRequestId);
  // 历史子委托查询请求
  virtual int ReqQryHisSubOrder(CReqCosQryHisSubOrderField *p_pReqField, LONGLONG p_llRequestId);
  // 股本变迁信息查询请求
  virtual int ReqQryChgShareCapital(CReqCosQryChgShareCapitalField *p_pReqField, LONGLONG p_llRequestId);
  // 交易时间片查询请求
  virtual int ReqQryTrdTimeSlice(CReqCosQryTrdTimeSliceField *p_pReqField, LONGLONG p_llRequestId);
  // 策略延期请求
  virtual int ReqStrategyExtension(CReqCosStrategyExtensionField *p_pReqField, LONGLONG p_llRequestId);
  // 策略委托查询(通用)请求
  virtual int ReqQryStrategyOrder(CReqCosQryStrategyOrderField *p_pReqField, LONGLONG p_llRequestId);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);
  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
  // 用户现货登录请求响应
  void OnRspStkUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户信用登录请求响应
  void OnRspFislUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户期权登录请求响应
  void OnRspOptUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户期权登录请求(新)响应
  void OnRspOptRpcUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 用户期货登录请求响应
  void OnRspFutureUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  //子系统查询
  void OnRspQryConf(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //子系统设置
  void OnRspChgConf(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化委托
  void OnRspOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化撤单
  void OnRspCancelOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //委托改单
  void OnRspChgOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //组合委托
  void OnRspCombOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //节点变更
  void OnRspChgNode(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //节点变更上线
  void OnRspChgNodeOnline(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化委托查询
  void OnRspQryOrderInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //客户所在节点编号查询
  void OnRspQryNodeInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //节点的客户查询
  void OnRspQryNodeCustInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //接口配置查询
  void OnRspQryItfInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //接口配置设置
  void OnRspChgItf(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化可撤单委托查询
  void OnRspQryCanWithdrawOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化成交查询
  void OnRspQryMatch(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //量化子委托查询
  void OnRspQrySubOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //历史委托查询
  void OnRspQryHisOrderInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //历史成交查询
  void OnRspQryHisFillInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //组合委托查询
  void OnRspQryCombOrderInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //资产用户查询
  void OnRspQryCuacctCodeInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //节点账户查询
  void OnRspQryNodeUsers(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //根据组合委托编号查询组合委托
  void OnRspQryCombOrderInfoEx(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //行情处理快照
  void OnRspSnapshot(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //行情回放功能
  void OnRspReplay(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //深度行情查询委托
  void OnRspQryMktDataInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //行情记录功能
  void OnRspRecordMktData(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //行情回放进度查询
  void OnRspQryReplayInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //历史行情查询委托
  void OnRspQryHisMktData(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //数据同步
  void OnRspSync(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //资产账户开户
  void OnRspOpenAccount(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //资产账户销户
  void OnRspCloseAccount(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //资金同步
  void OnRspFundSync(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //股份同步
  void OnRspStkSync(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //交易日切
  void OnRspCutoff(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可用资金查询响应
  void OnRspQryExpendableFund(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 可用股份查询响应
  void OnRspQryExpendableShares(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  /////////////////////////////////////////////////////////////////////////
  //内部接口功能
  //成交回报数据
  void OnRspMatchReport(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //委托应答数据
  void OnRspOrderAnswer(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  
  //自选股编辑
  void OnRspPortfolio(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //自选股查询
  void OnRspQryPortfolioInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //ETF篮子委托
  void OnRspEtfBasketOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //自定义篮子委托
  void OnRspCustomBasketOrder(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  //证券信息查询
  void OnRspQryStkInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  // 算法单委托响应
  void OnRspOrderStrategy(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 策略启停响应
  void OnRspStrategySwitch(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 日切交易日查询响应
  void OnRspQrySettDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 获取下一交易日历响应
  void OnRspQryTradeDate(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 期权组合保证金和行权指令合并委托响应
  void OnRspCombStraAndExerciseOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 期权信息查询请求
  void OnRspQryOptInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 期货信息查询请求
  void OnRspQryFutInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 现货期权期货信息查询请求
  void OnRspQryStkOptFutInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 历史子委托查询请求
  void OnRspQryHisSubOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 股本变迁信息查询请求
  void OnRspQryChgShareCapital(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 交易时间片查询请求
  void OnRspQryTrdTimeSlice(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 策略延期响应
  void OnRspStrategyExtension(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  // 策略委托查询(通用)响应
  void OnRspQryStrategyOrder(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
};

END_NAMESPACE_MACLI
#endif //__MA_CLI_COS_TRADE_API_H__