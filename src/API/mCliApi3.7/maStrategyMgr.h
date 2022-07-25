//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maStrategyMgr.h
// 模块名称：微架构C++语言策略运行的管理接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2017-04-13
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2017-04-13    001.000.000   何万刚          原创
// 2019-09-02    001.000.001   林振益          StgyStartup增加p_bOldVer参数
//----------------------------------------------------------------------------
#if !defined(__MA_STRATEGY_MGR_H__)
#define __MA_STRATEGY_MGR_H__
#include "maStrategyApi.h"

// 管理dll/so导出函数原型
//int MATRADEAPI CreateStrategyMgr(void **p_ppclMgr, void *p_pclSpi);
//int MATRADEAPI ReleaseStrategyMgr(void *p_pclMgr);

BGN_NAMESPACE_MACLI

class IStrategyMgrSpi
{
public:
  virtual int OnRequest(unsigned char *p_pszData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;
  virtual int OnMessage(int p_iDate, int p_iTime, unsigned int p_iPid, unsigned int p_iTid, int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0;
};

class IStrategyMgrApi
{
public:
  // 申请Format实例
  virtual IStrategyFmt* CreateFormat(void) = 0;

  // 销毁Format实例
  virtual void ReleaseFormat(IStrategyFmt *p_pclFmt) = 0;

public:
  // 初始化与反初始化
  virtual int Initialize(void) = 0;
  virtual int Uninitialize(void) = 0;

  // 策略模块
  virtual const char* GetStgyModule(void) = 0;
  virtual int SetStgModule(const char* p_pszModule) = 0;
  virtual bool CheckModlue(void) = 0;

  // 交互队列
  virtual unsigned int GetQueMaxSize(void) = 0;
  virtual int SetQueMaxSize(unsigned int p_uiQueMaxSize) = 0;
  virtual unsigned int GetReqQueDepth(void) = 0;
  virtual unsigned int GetRepQueDepth(void) = 0;
  virtual unsigned int GetNtfQueDepth(void) = 0;

  // 策略运行
  virtual bool IsStgyRunning(void) = 0;
  virtual unsigned int GetStgyPid(void) = 0;
  virtual int StgyStartup(bool p_bDebug = false, bool p_bOldVer = false) = 0;
  virtual int StgyShutdown(void) = 0;

  // 应答通知
  virtual int Response(unsigned char *p_pszRspData, int p_iDataLen, LONGLONG  p_llRequestId, const char *p_pszFuncId) = 0;
  virtual int Notify(unsigned char *p_pszNtfData, int p_iDataLen, const char *p_pszTopic) = 0;

  // 错误信息
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_STRATEGY_MGR_H__
