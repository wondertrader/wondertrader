//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maStrategyApi.h
// 模块名称：微架构C++语言策略运行的接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2017-04-13
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2017-04-13    001.000.000   何万刚          原创
// 2019-04-13    001.000.001   林振益          IStrategyApi增加MessageWithTime函数
//----------------------------------------------------------------------------
#if !defined(__MA_STRATEGY_API_H__)
#define __MA_STRATEGY_API_H__

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
  #if defined MA_TRADE_API_EXPORTS
    #define MATRADEAPI __declspec(dllexport)
  #else
    #define MATRADEAPI __declspec(dllimport)
  #endif
#elif defined(OS_IS_LINUX)
  #define MATRADEAPI
#endif

#define BGN_NAMESPACE_MACLI   namespace macli {
#define END_NAMESPACE_MACLI   }
#define USE_NAMESPACE_MACLI   using namespace macli;

// 策略dll/so导出函数原型
//int MATRADEAPI CreateStrategySpi(void **p_ppclSpi, void *p_pclApi);
//int MATRADEAPI ReleaseStrategySpi(void *p_pclSpi);

BGN_NAMESPACE_MACLI

// 组解包接口类
class MATRADEAPI IStrategyFmt
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

// 策略响应接口类
class MATRADEAPI IStrategySpi
{
public:
  // 启动响应
  virtual int OnStartup(void) {return 0;}

  // 退出响应
  virtual int OnShutdown(void) {return 0;}

  // 断开响应(与服务器的连接首次断开时触发)
  virtual int OnDisconnect(void) {return 0;}

  // 重连响应(与服务器的连接重建成功时触发)
  virtual int OnReconnect(void) {return 0;}

  // 应答响应
  virtual int OnResponse(unsigned char *p_pszRspData, int p_iDataLen, LONGLONG  p_llRequestId, const char *p_pszFuncId) {return 0;}

  // 通知响应
  virtual int OnNotify(unsigned char *p_pszNtfData, int p_iDataLen, const char *p_pszTopic) {return 0;}
};

// 管理交互接口类
class MATRADEAPI IStrategyApi
{
public:
  // 申请Format实例
  virtual IStrategyFmt* CreateFormat(void) = 0;

  // 销毁Format实例
  virtual void ReleaseFormat(IStrategyFmt *p_pclFmt) = 0;

  // 发送请求信息(p_llRequestID唯一递增)
  virtual int Request(unsigned char *p_pszReqData, int p_nDataLen, LONGLONG p_llRequestID, const char *p_pszFuncId) = 0;

  // 发送运行信息
  virtual int Message(int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText) = 0; 

  // 发送自定义时间的运行信息(llTime格式:YYmmddHHMMSSnnn)
  virtual int MessageWithTime(int p_iMsgCode, int p_iMsgLevel, const char *p_pszMsgText,  long long llTime) = 0;

  // 获取最后错误信息
  virtual const char* GetLastErrorText(void) = 0;
};

END_NAMESPACE_MACLI

#endif  // __MA_STRATEGY_API_H__
