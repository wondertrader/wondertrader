#if !defined(__MA_CLI_STK_MKT_DATA_H__)
#define __MA_CLI_STK_MKT_DATA_H__

#include "maMktData.h"
#include "maCliTradeApi.h"
#include "maCliMktDataStruct.h"
#include "maCliStkTradeApiStruct.h"
#include "maCliOptTradeApiStruct.h"
#include "maCliFislTradeApiStruct.h"
#include "maCliFutureTradeApiStruct.h"
#include "maCliCosTradeApiStruct.h"

BGN_NAMESPACE_MACLI

class MACLIAPI CCliMktDataSpi
{
public:
  // 客户端与服务器成功建立通信连接后，该方法被调用
  virtual int OnConnected(void) {return 0;}

  // 客户端与服务器成功的通信连接断开时，该方法被调用
  virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) {return 0;}
  
  // 登录后台量化系统
  virtual int OnRspCosLogin(CFirstSetField *p_pFirstSet, CRspCosLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 现货/信用用户登录请求响应
  virtual int OnRspStkUserLogin(CFirstSetField *p_pFirstSetField, CRspStkUserLoginField *p_pRsqField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 期权用户登录请求响应
  virtual int OnRspOptUserLogin(CFirstSetField *p_pFirstSetField, CRspOptUserLoginField *p_pRsqField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 期货用户登录请求响应
  virtual int OnRspFutureUserLogin(CFirstSetField *p_pFirstSetField, CRspFutureUserLoginField *p_pRsqField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 心跳检测请求响应
  virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSetField, CRspHeartBeatField *p_pRsqField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 行情回放请求响应
  virtual int OnRspPlayBack(CFirstSetField *p_pFirstSetField, CRspPlayBackField *p_pRsqField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 订阅响应
  virtual int OnRtnSubMktData(CRspSubTopicField* p_pRspSubField) {return 0;}

  // 取消订阅响应
  virtual int OnRtnUnsubMktData(CRspUnsubTopicField* p_pRspUnsubField) {return 0;}

  // 自选股编辑
  virtual int OnRspPortfolio(CFirstSetField *p_pFirstSet, CRspMktDataPortfolioField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 自选股查询
  virtual int OnRspQryPortfolioInfo(CFirstSetField *p_pFirstSet, CRspMktDataPortfolioInfoField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // k线数据
  virtual int OnRspQryKData(CFirstSetField *p_pFirstSet, CRspMktDataKDataField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // tick快照
  virtual int OnRspQryTickSnapshot(CFirstSetField *p_pFirstSet, CRspMktDataTickSnapshotField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 千档行情
  virtual int OnRspQryThounsandData(CFirstSetField *p_pFirstSet, CRspMktDataThounsandDataField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 指数估值查询响应
  virtual int OnRspQryIndexVal(CFirstSetField *p_pFirstSetField, CRspMktDataQryIndexValField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}
  
  // 云指标计算请求响应
  virtual int OnRspQryCloudIndex(CFirstSetField *p_pFirstSetField, CRspMktDataQryCloudIndexField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 快照行情响应(期权、现货、期货)
  virtual int OnRtnMktData(ST_MKT_DATA* p_pMktData) {return 0;}

  // 快照行情响应(期权、现货、期货)
  virtual int OnRtnMktData(ST_MKT_DATA2* p_pMktData) {return 0;}

  // 快照行情响应(期权、现货、期货)
  virtual int OnRtnMktData(ST_MKT_DATA3* p_pMktData) {return 0;}

  // 分时成交响应
  virtual int OnRtnMktMatch(ST_MKT_MATCH* p_pMktData) {return 0;}
  
  // 逐笔委托响应
  virtual int OnRtnMktOrder(ST_MKT_ORDER* p_pMktData) {return 0;}
  
  // 指标数据响应
  virtual int OnRtnMktIndex(ST_MKT_INDEX* p_pMktData) {return 0;}

  // 逐笔成交响应
  virtual int OnRtnMktTransaction(ST_MKT_TRANSACTION* p_pMktData) {return 0;}

  // 委托队列响应
  virtual int OnRtnMktOrderQueue(ST_MKT_ORDER_QUEUE* p_pMktData) {return 0;}

  // 千档行情响应
  virtual int OnRtnMktThounsandData(ST_MKT_THOUNSAND_DATA* p_pMktData) {return 0;}
};

class MACLIAPI CCliMktDataApi
{
public:
  // 默认构造函数
  CCliMktDataApi(void);

  // 析构函数
  virtual ~CCliMktDataApi(void);

  // 初始化
  virtual int Init(void);

  // 退出
  virtual int Exit(void);

  // 注册连接信息
  virtual int RegisterServer(const char *p_pszIp, int p_iPort, bool p_bCos = false, int p_iSsl = -1);

  // 注册连接信息
  virtual int RegisterServer(CReqSysField* p_pstSysField);

  // 注册回调接口
  virtual int RegisterSpi(CCliMktDataSpi *p_pSpi);

  // 注册账号信息
  virtual int RegisterAcct(char p_chChannel, const char *p_pszSession);

  // 注册账号信息
  virtual int RegisterAcct(CReqFixField* p_pstFixField);

  // 量化系统注册功能请求
  virtual int ReqCosLogin(CReqCosLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 用户现货登录请求
  virtual int ReqStkUserLogin(CReqStkUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 用户期权登录请求
  virtual int ReqOptUserLogin(CReqOptUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 用户期货登录请求
  virtual int ReqFutureUserLogin(CReqFutureUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 用户信用登录请求
  virtual int ReqFislUserLogin(CReqFislUserLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 心跳检测请求
  virtual int ReqHeartBeat(CReqHeartBeatField *p_pReqField, LONGLONG p_llRequestId);

  // 行情回放请求
  virtual int ReqPlayBack(CReqPlayBackField * p_pPlayBack,LONGLONG p_llRequestID);

  // 自选股编辑
  virtual int ReqPortfolio(CReqMktDataPortfolioField *p_pReqField, LONGLONG p_llRequestId);

  // 自选股查询
  virtual int ReqQryPortfolioInfo(CReqMktDataPortfolioInfoField *p_pReqField, LONGLONG p_llRequestId);

  // k线数据
  virtual int ReqQryKData(CReqMktDataKDataField *p_pReqField, LONGLONG p_llRequestId);

  // tick快照
  virtual int ReqQryTickSnapshot(CReqMktDataTickSnapshotField *p_pReqField, LONGLONG p_llRequestId);

  // 千档行情查询
  virtual int ReqQryThounsandData(CReqMktDataThounsandDataField *p_pReqField, LONGLONG p_llRequestId);
  
  // 指数估值查询请求
  virtual int ReqQryIndexVal(CReqMktDataQryIndexValField *p_pReqField, LONGLONG p_llRequestId);
  
  // 云指标计算请求请求
  virtual int ReqQryCloudIndex(CReqMktDataQryCloudIndexField *p_pReqField, LONGLONG p_llRequestId);

  // 获取最后错误信息
  virtual const char* GetLastErrorText(void);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 订阅主题
  int SubMktData(CReqSubTopicField* p_pReqSubField);

  // 退订主题
  int UnsubMktData(CReqUnsubTopicField* p_pReqUnsubField);

private:
  // 设置业务包头信息
  int SetBizPackHead(const char *p_pszFuncId, LONGLONG p_llRequestId, char *p_pszMsgId, char p_chPkgType);

  // 订阅请求响应
  void OnRspSubMktData(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 退订请求响应
  void OnRspUnsubMktData(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 量化系统注册功能响应
  void OnRspCosLogin(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

  // 用户现货/信用登录请求响应
  void OnRspStkUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 用户期权登录请求响应
  void OnRspOptUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 用户期货登录请求响应
  void OnRspFutureUserLogin(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 心跳检测请求响应
  void OnRspHeartBeat(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 行情回放请求响应
  void OnRspPlayBack(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 自选股编辑
  void OnRspPortfolio(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

  // 自选股查询
  void OnRspQryPortfolioInfo(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

  // k线数据
  void OnRspQryKData(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

  // tick快照
  void OnRspQryTickSnapshot(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);

  // 千档行情
  void OnRspQryThounsandData(CFirstSetField *p_pFirstSet, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 指数估值查询响应
  void OnRspQryIndexVal(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);
  
  // 云指标计算请求响应
  void OnRspQryCloudIndex(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

private:
  void*              m_hAsynCall;        // 系统调用句柄
  void*              m_hAnsParse;        // 应答解包句柄
  void*              m_hPubParse;        // 推送解包句柄
  CReqSysField       m_stSysField;       // 系统连接参数
  CReqFixField       m_stReqFixField;    // 请求固定参数
  CCliMktDataSpi    *m_pMktDataSpi;      // 回调实例指针
  bool               m_bIsLogin;
  bool               m_bIsCos;
  char               m_szLastErrorText[1024+1]; // 最后错误信息
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_STK_MKT_DATA_H__