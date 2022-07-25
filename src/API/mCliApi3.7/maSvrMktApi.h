//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maSvrMktApi.h
// 模块名称：微架构C++语言外部接入行情源的接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2019-06-20
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2019-06-20    001.000.000   何万刚          原创
//----------------------------------------------------------------------------
#if !defined(__MA_SVRMKTAPI_API_H__)
#define __MA_SVRMKTAPI_API_H__
#include "maMktData.h"

#if defined(LONGLONG)
#undef LONGLONG;
#endif

#if defined(ULONGLONG)
#undef ULONGLONG;
#endif

#if defined (_MSC_VER) && (_MSC_VER == 1200)
  #define FORMAT_LONGLONG "%I64d"
  #if defined(WIN32)
  typedef __int64 LONGLONG;
  typedef unsigned __int64 ULONGLONG;
  #endif
#else
  #define FORMAT_LONGLONG "%lld"
  typedef long long LONGLONG;
  typedef unsigned long long ULONGLONG;
#endif


#if defined(WIN32) || defined(WIN64) || defined(OS_IS_WINDOWS)
  #if defined MA_MKTDATA_API_EXPORTS
    #define MAMKTDATAAPI __declspec(dllexport)
  #else
    #define MAMKTDATAAPI __declspec(dllimport)
  #endif
#elif defined(OS_IS_LINUX)
  #define MAMKTDATAAPI
#endif


#define BGN_NAMESPACE_MACLI   namespace macli {
#define END_NAMESPACE_MACLI   }
#define USE_NAMESPACE_MACLI   using namespace macli;

// 外接行情源dll/so导出函数原型
//int MACLIAPI CreateSvrMktSpi(void **p_ppclSpi, void *p_pclApi);
//int MACLIAPI ReleaseSvrMktSpi(void *p_pclSpi);

BGN_NAMESPACE_MACLI

// 交互消息类型定义
enum EN_SVRMKT_MSG_TYPE
{
  SVRMKT_MSG_TYPE_REQ = 1,  // 请求消息
  SVRMKT_MSG_TYPE_REP = 2,  // 响应消息
  SVRMKT_MSG_TYPE_PUB = 3,  // 推送消息
  SVRMKT_MSG_TYPE_RTM = 4,  // 运行消息
  SVRMKT_MSG_TYPE_CON = 5,  // 连接消息
  SVRMKT_MSG_TYPE_OTH = 9,  // 其他消息
};

// 行情类型
enum EN_SVRMKT_TYPE
{
  SVRMKT_TYPE_DATA = '1',
  SVRMKT_TYPE_DATA2 = '2',
  SVRMKT_TYPE_DATA3 = '3',
  SVRMKT_TYPE_MATCH = '4',
  SVRMKT_TYPE_ORDER = '5',
  SVRMKT_TYPE_INDEX = '6',
  SVRMKT_TYPE_ORDER_QUEUE = '7',
  SVRMKT_TYPE_TRANSACTION = '8',
};

// 组解包接口类
class MAMKTDATAAPI ISvrMktApiFmt
{
public:
  // 组包
  virtual int BeginWrite(void) = 0;
  virtual int SetValue(const char *p_pszValue, int p_iValueLen, const char *p_pszField) = 0;
  virtual int EndWrite(void) = 0;
  virtual int Make(unsigned char **ppszData, int &p_refiDataLen) = 0;
  virtual int CreateTable(void) = 0;
  virtual int AddRow(void) = 0;
  virtual int SaveRow(void) = 0;

  // 解包
  virtual int Parse(unsigned char *p_pszData, int p_iDataLen) = 0;
  virtual int GetTableCount(int &p_refiTableCount) = 0;
  virtual int OpenTable(int p_iTableIdx) = 0;
  virtual int GetRowCount(int &p_refiRowCount) = 0;
  virtual int ReadRow(int p_iRowIdx) = 0;
  virtual int GetValue(char *p_pszValue, int p_iValueSize, int &p_refiValueLen, const char *p_pszField) = 0;

  // 重载
  virtual int GetValue(char *p_pszValue, int p_iValueSize, const char *p_pszField) = 0;
  virtual int SetValue(const char *p_pszValue, const char *p_pszField) = 0;

  virtual int GetValue(char &p_refchValue,  const char *p_pszField) = 0;
  virtual int SetValue(char p_chValue, const char *p_pszField) = 0;

  virtual int GetValue(short &p_refsValue,  const char *p_pszField) = 0;
  virtual int SetValue(short p_sValue, const char *p_pszField) = 0;

  virtual int GetValue(unsigned short &p_refusValue, const char *p_pszField) = 0;
  virtual int SetValue(unsigned short p_usValue, const char *p_pszField) = 0;

  virtual int GetValue(int &p_refiValue, const char *p_pszField) = 0;
  virtual int SetValue(int p_iValue, const char *p_pszField) = 0;

  virtual int GetValue(unsigned int &p_refuiValue, const char *p_pszField) = 0;
  virtual int SetValue(unsigned int p_uiValue, const char *p_pszField) = 0;

  virtual int GetValue(LONGLONG &p_refllValue, const char *p_pszField) = 0;
  virtual int SetValue(LONGLONG p_llValue, const char *p_pszField) = 0;

  virtual int GetValue(ULONGLONG &p_refullValue, const char *p_pszField) = 0;
  virtual int SetValue(ULONGLONG p_ullValue, const char *p_pszField) = 0;

  virtual int GetValue(double &p_refdValue, const char *p_pszField) = 0;
  virtual int SetValue(double p_dValue, const char *p_pszField) = 0;

  // 错误信息
  virtual const char* GetLastErrorText(void) = 0;
};

// 外接行情源响应接口类
class MAMKTDATAAPI ISvrMktSpi
{
public:
  // 设置响应
  virtual int OnSetParam(const char* p_pszParam) {return 0;}

  // 启动响应
  virtual int OnStartup(void) {return 0;}

  // 退出响应
  virtual int OnShutdown(void) {return 0;}

  // 请求响应
  virtual int OnRequest(unsigned char *p_pszReqData, int p_iDataLen, LONGLONG  p_llRequestId, const char *p_pszFuncId) {return 0;}
};

// 管理交互接口类
class MAMKTDATAAPI ISvrMktApi
{
public:
  // 申请Format实例
  virtual ISvrMktApiFmt* CreateFormat(void) = 0;

  // 销毁Format实例
  virtual void ReleaseFormat(ISvrMktApiFmt *p_pclFmt) = 0;

  // 发送响应信息(p_llRequestID唯一递增)
  virtual int Response(unsigned char *p_pszRspData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;

  // 推送行情信息
  virtual int Publish(ST_MKT_DATA2 &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_DATA3 &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_MATCH &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_TRANSACTION &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_ORDER &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_ORDER_QUEUE &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_INDEX &refstData, LONGLONG p_llAcceptSn) = 0;
  virtual int Publish(ST_MKT_THOUNSAND_DATA &refstData, LONGLONG p_llAcceptSn) = 0;

  // 发送运行信息
  virtual int Message(int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0; 

  // 获取最后错误信息
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_SVRMKTAPI_API_H__
