//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maSvrMktMgr.h
// 模块名称：微架构C++语言外部接入行情源的管理接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2019-06-20
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2019-06-20    001.000.000   何万刚          原创
//----------------------------------------------------------------------------
#if !defined(__MA_SVRMKT_MGR_H__)
#define __MA_SVRMKT_MGR_H__
#include "maSvrMktApi.h"

// 管理dll/so导出函数原型
//int MAMKTDATAAPI CreateSvrMktMgr(void **p_ppclMgr, void *p_pclSpi);
//int MAMKTDATAAPI ReleaseSvrMktMgr(void *p_pclMgr);

BGN_NAMESPACE_MACLI

class ISvrMktMgrSpi
{
public:
  // 断开响应(与服务器的连接成功时触发)
  virtual int OnConnect(void) = 0;

  // 断开响应(与服务器的连接首次断开时触发)
  virtual int OnDisconnect(int p_iErrorCode, const char *p_pszErrorMsg) = 0;

  // 重连响应(与服务器的连接重建成功时触发)
  virtual int OnReconnect(void) = 0;

  // 应答消息响应
  virtual int OnResponse(unsigned char *p_pszData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;

  // 推送消息响应
  virtual int OnPublish(unsigned char *p_pszPubData, int p_iDataLen, LONGLONG p_llAcceptSn, const char *p_pszTopic) = 0;

  // 提示信息响应
  virtual int OnMessage(int p_iDate, int p_iTime, unsigned int p_iPid, unsigned int p_iTid, int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0;
};

class ISvrMktMgrApi
{
public:
  // 申请Format实例
  virtual ISvrMktApiFmt* CreateFormat(void) = 0;

  // 销毁Format实例
  virtual void ReleaseFormat(ISvrMktApiFmt *p_pclFmt) = 0;

public:
  // 初始化与反初始化
  virtual int Initialize(void) = 0;
  virtual int Uninitialize(void) = 0;

  // 策略模块
  virtual const char* GetModule(void) = 0;
  virtual int SetModule(const char* p_pszModule) = 0;
  virtual bool CheckModlue(void) = 0;

  // 交互队列
  virtual unsigned int GetQueMaxSize(void) = 0;
  virtual int SetQueMaxSize(unsigned int p_uiQueMaxSize) = 0;
  virtual unsigned int GetReqQueDepth(void) = 0;
  virtual unsigned int GetRepQueDepth(void) = 0;
  virtual unsigned int GetPubQueDepth(void) = 0;

  // 运行及状态
  virtual bool IsRunning(void) = 0;
  virtual unsigned int GetPid(void) = 0;
  virtual int Startup(bool p_bDebug = false) = 0;
  virtual int Shutdown(void) = 0;

  // 请求通知
  virtual int Request(unsigned char *p_pszReqData, int p_iDataLen, LONGLONG p_llRequestId, const char *p_pszFuncId) = 0;

  // 错误信息
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_SVRMKT_MGR_H__
