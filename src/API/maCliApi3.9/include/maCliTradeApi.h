//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maCliTradeApi.h
// 模块名称：ma微架构C++语言API接口的类CTP封装接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2015-06-25
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2015-06-25    001.000.000   何万刚          原创
//----------------------------------------------------------------------------
#if !defined(__MA_CLI_TRADE_API_H__)
#define __MA_CLI_TRADE_API_H__

#if defined (_MSC_VER) && (_MSC_VER == 1200)
  #define FORMAT_LONGLONG "%I64d"
  #if defined(WIN32)
  typedef __int64 LONGLONG;
  #endif
#else
  #define FORMAT_LONGLONG "%lld"
  typedef long long LONGLONG;
#endif

#if defined(WIN32) || defined(WIN64) || defined(OS_IS_WINDOWS)
  #if defined MA_TRADE_API_EXPORTS
    #define MATRADEAPI __declspec(dllexport)
  #else
    #define MATRADEAPI __declspec(dllimport)
  #endif
#else
  #define MATRADEAPI
#endif

#if !defined(BGN_NAMESPACE_MACLI)
  #define BGN_NAMESPACE_MACLI   namespace macli {
  #define END_NAMESPACE_MACLI   }
  #define USE_NAMESPACE_MACLI   using namespace macli;  
#endif

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

// 系统连接参数
struct MATRADEAPI CReqSysField
{
  char          szSvrAddress[39 + 1];       // 服务器IP地址
  int           nSvrPort;                   // 服务器端口
  char          szLocalAddress[39 + 1];     // 本地IP地址
  int           nLocalPort;                 // 本地端口
  int           iSsl;                       // 加密通信 0:非加密 1:加密 -1:按配置
  char          szPktVer[2 + 1];            // 报文协议版本 01:Jason 02:PB
  unsigned int  uiTimeout;                  // 超时时长(秒)
  char          chDataSet;                  // 推送自动订阅 '0':按连接 '1':按账户 0:不订阅
  int           iFlashModule;               // 软加速模块类型 0:默认连接 1:普通连接 2:TCP_DIRECT连接[Lnx64支持] 4:RDMA连接[Lnx64支持]
  char          chFlashPub;                 // 软加速推送模式 '0':按账户普通  '1':按账户精简  '2':按连接普通 '3':按连接精简
};

// 请求固定参数
struct MATRADEAPI CReqFixField
{
  char          szOpUser[32 + 1];           // 操作用户代码
  char          chOpRole;                   // 操作用户角色
  char          szOpSite[256 + 1];          // 操作站点
  char          chChannel;                  // 操作渠道
  char          szSession[128 + 1];         // 会话凭证
  char          szFunction[16 + 1];         // 功能代码
  char          szRuntime[32 + 1];          // 调用时间 YYYY-MM-DD HH24:MI:SS.nnnnnn
  int           iOpOrg;                     // 操作机构
  char          szCuacctCode[32 + 1];       // 操作资产账户
  char          chCuacctType;               // 操作账户类型
  char          szTrdPwd[32 + 1];           // 交易密码 集中交易接口使用
  char          szSerChannel[8 + 1];        // 服务渠道
};

// 终端信息参数
struct MATRADEAPI CReqClientField
{
  char          szIip[39 + 1];              // 公网IP
  char          szIport[5 + 1];             // 公网IP端口号
  char          szLip[39 + 1];              // 内网IP
  char          szMac[18 + 1];              // MAC地址
  char          szHd[32 + 1];               // 硬盘序列号
  char          szPcn[20 + 1];              // PC终端设备名
  char          szCpu[20 + 1];              // CPU序列号
  char          szPi[20 + 1];               // 硬盘分区信息
  char          szVol[10 + 1];              // 系统盘卷标号
  char          szExt[53 + 1];              // 扩展信息 以‘;’进行分隔
};

// 第1结果集内容
struct MATRADEAPI CFirstSetField
{
  int           iMsgCode;                   // 信息代码
  char          chMsgLevel;                 // 信息级别
  char          szMsgText[256 + 1];         // 信息内容
  char          szMsgDebug[1024 + 1];       // 后台跟踪信息
};

//-------------------------------00102012:订阅主题------------------------------------
struct MATRADEAPI CReqSubTopicField
{
  char          szTopic[12 + 1];            // 主题
  char          szFilter[512 + 1];          // 过滤条件
  char          chDataSet;                  // 成交数据集 '0':仅含本连接发送委托的成交信息 '1':含所有连接发送委托的成交信息
};

struct MATRADEAPI CRspSubTopicField
{
  char          szTopic[12 + 1];            // 主题
  char          szFilter[512 + 1];          // 过滤条件
  LONGLONG      llAcceptSn;                 // 订阅受理号
  char          szChannel[64 + 1];          // 推送通道
  char          chDataSet;                  // 成交数据集 '0':仅含本连接发送委托的成交信息 '1':含所有连接发送委托的成交信息
};

//-------------------------------00102013:退订主题------------------------------------
struct MATRADEAPI CReqUnsubTopicField
{
  char          szTopic[12 + 1];            // 主题
  LONGLONG      llAcceptSn;                 // 订阅受理号
};

struct MATRADEAPI CRspUnsubTopicField
{
  char          szTopic[12 + 1];            // 主题
  LONGLONG      llAcceptSn;                 // 订阅受理号
};

//-------------------------------00102014:上传文件功能------------------------------------
struct MATRADEAPI CReqFileUploadField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szLocalFileName[512 + 1];   // 本地文件名
};

struct MATRADEAPI CRspFileUploadField
{
  char            szFileName[512 + 1];        // 本地文件名
};

//-------------------------------00102015:下载文件功能------------------------------------
struct MATRADEAPI CReqFileDownloadField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szLocalFileName[512 + 1];   // 本地文件名
};

struct MATRADEAPI CRspFileDownloadField
{
  char            szFileName[512 + 1];        // 本地文件名
};

//-------------------------------00102014:上传目录文件功能------------------------------------
struct MATRADEAPI CReqFolderUploadField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szLocalFolder[512 + 1];     // 相对目录路径
};

struct MATRADEAPI CRspFolderUploadField
{
  char            szFileName[512 + 1];        // 相对路径文件名
};

//-------------------------------00102015:下载目录文件功能------------------------------------
struct MATRADEAPI CReqFolderDownloadField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szLocalFolder[512 + 1];     // 相对目录路径
};

struct MATRADEAPI CRspFolderDownloadField
{
  char            szFileName[512 + 1];        // 相对路径文件名
};

//-------------------------------00102016:文件信息查询------------------------------------
struct MATRADEAPI CReqQryFileInfoField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szFilePath[512 + 1];        // 相对路径目录
  char            szFileFilter[32 + 1];       // 文件过滤  例如 *.dll 默认为空
};

struct MATRADEAPI CRspQryFileInfoField
{
  char            szFileId[32 + 1];           // 文件ID
  LONGLONG        llFileSize;                 // 文件大小
  char            szFileMd5[32 + 1];          // 文件MD5
  char            szFileTime[32 + 1];         // 更新时间
  char            szFileName[512 + 1];        // 相对路径文件名
};

//-------------------------------00102017:策略启动功能------------------------------------
struct MATRADEAPI CReqStgyStartField
{
  char            szFileId[32 + 1];           // 文件ID
  char            szCuacctCode[32 + 1];       // 资金账户
  char            szStgyId[32 + 1];           // 策略ID
  char            szStgyParam[512 + 1];       // 策略参数
};

struct MATRADEAPI CRspStgyStartField
{
  char            szRunningId[32 + 1];        // 运行实例
  char            szFileId[32 + 1];           // 文件ID
  char            szCuacctCode[32 + 1];       // 资金账户
  char            szStgyId[32 + 1];           // 策略ID
  char            chRunningStatus;            // 运行状态
};

//-------------------------------00102018:策略停止功能------------------------------------
struct MATRADEAPI CReqStgyStopField
{
  char            szRunningId[32 + 1];        // 运行实例
  char            szFileId[32 + 1];           // 文件ID
  char            szCuacctCode[32 + 1];       // 资金账户
  char            szStgyId[32 + 1];           // 策略ID
};

struct MATRADEAPI CRspStgyStopField
{
  char            szRunningId[32 + 1];        // 运行实例
  char            szFileId[32 + 1];           // 文件ID
  char            szCuacctCode[32 + 1];       // 资金账户
  char            szStgyId[32 + 1];           // 策略ID
  char            chRunningStatus;            // 运行状态
};

//-------------------------------00102019:策略状态查询------------------------------------
struct MATRADEAPI CReqQryStgyStatField
{
  char            szRunningId[32 + 1];        // 运行实例
  char            szFileId[32 + 1];           // 文件ID
  char            szStgyId[32 + 1];           // 策略ID
};

struct MATRADEAPI CRspQryStgyStatField
{
  char            szRunningId[32 + 1];        // 运行实例
  char            szFileId[32 + 1];           // 文件ID
  char            szCuacctCode[32 + 1];       // 资金账户
  char            szStgyId[32 + 1];           // 策略ID
  char            chRunningStatus;            // 运行状态
};

//-------------------------------00103003:连接保活------------------------------------
struct MATRADEAPI CReqHeartBeatField
{
  ;
};

struct MATRADEAPI CRspHeartBeatField
{
  char          szServerLocalNodeId[32 + 1];  // 本节点编号
  char          szServerNodeId[32 + 1];       // 节点编号
  char          szServerNodeGid[32 + 1];      // 节点组号
  char          szServerSiteName[32 + 1];     // 接入站点名
  char          szServerNodeType[32 + 1];     // 节点类型
  char          szServerSiteIP[32 + 1];       // 站点IP
  char          szServerNodePwd[128 + 1];     // 节点路径
  char          szServerNodeUse[4 + 1];       // 节点用途 ‘n’:待机节点 ‘s’:备机节点 ‘a’:主用节点
  char          szServerBackIp[32 + 1];       // 备份站点IP
};

//-------------------------------00103004:安全检查------------------------------------
struct MATRADEAPI CReqSafeCheckField
{
  char          szCuacctCode[32 + 1];         // 资金账户
  char          chPlainType;                  // 明文类型 ‘2’:客户端IP ‘3’:连接MAC ‘4’:主程序APP
  char          szCipherText[128 + 1];        // 密文内容 RSA加密密文
};

struct MATRADEAPI CRspSafeCheckField
{
  int           iEffectDate;                  // 生效日期
  int           iExpireDate;                  // 失效日期
};

//-------------------------------00102028:风控信息推送内容------------------------------------
struct MATRADEAPI CRtnTradeRiskInfoField
{
  char            chRiskCls;                  // 风险类别 
  char            szRiskId[2 + 1];            // 风险标识 
  char            chRiskIdLevel;              // 风险级别 
  char            szRiskName[256 + 1];        // 风险名称 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 证券板块 字典[STKBD]
  char            szContent[2048 + 1];        // 通知内容 
};

//-------------------------------00102021:系统状态推送内容------------------------------------
struct MATRADEAPI CRtnSystemInfoField
{
  int             iSubsys;                    // 系统类型 
  int             iSubsysSn;                  // 系统节点编号 账户所属系统编号
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            chTrdStatus;                // 交易状态 ’1’:允许下单 ’0’:禁止下单
  char            szRemark[128 + 1];          // 状态说明 
};

//-------------------------------00102029:账户通知推送内容--------------------------
struct MATRADEAPI CRtnNoticeInfoField
{
  int             iSubsys;                    // 系统类型 
  char            chNotyType;                 // 通知类型 ’0’:到期提醒 ’1’:到期终止 ’2’:股价变动终止
  char            szNotyInfo[256 + 1];        // 通知内容 
  int             iSettDate;                  // 清算日期 
  int             iOrderDate;                 // 委托日期 
  int             iOrderBsn;                  // 委托批号 
  LONGLONG        llOrderNo;                  // 委托编号 
  LONGLONG        llOrderGroupNo;             // 组合编号 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            chExeStatus;                // 委托状态 
  int             iTrdDate;                   // 交易日期 
  int             iOrderValidDate;            // 有效日期 
  char            szCustCode[16 + 1];         // 客户代码 
  char            szCuacctCode[16 + 1];       // 资产账户 
  char            chCuacctType;               // 账户类型 
  char            szTrdCode[30 + 1];          // 品种代码 
  int             iTrdBiz;                    // 交易业务 
  int             iTrdBizAcction;             // 业务活动 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderPrice[21 + 1];       // 委托价格 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chStrategyType;             // 策略类型 
  char            szStrategyName[32 + 1];     // 策略名称 
  char            chChannel;                  // 操作渠道
};

//-------------------------------10388750:COS系统登录功能--------------------------
struct MATRADEAPI CReqCosLoginField
{
  char            szUserCode[16 + 1];         // 用户代码 也可以是手机号登录
  char            szAuthData[256 + 1];        // 认证数据 
  char            szEncryptKey[32 + 1];       // 加密因子 
  char            szThirdParty[1024 + 1];     // 第三方 使用第三方客户端的用户代码必输,字段内容由密钥dll指定接口获取;
  char            chChannel;                  // 操作渠道
  char            szSerChannel[8 + 1];        // 服务渠道
};

struct MATRADEAPI CRspCosLoginField
{
  char            szUserCode[16 + 1];         // 用户代码 
  char            szPhoneNum[16 + 1];         // 手机号 
  char            szUserGroupRight[8 + 1];    // 用户组权限 
  char            szSessionId[128 + 1];       // 会话凭证 客户端需保存，后续请求填写到固定入参会话凭证(8814)中，否则后续请求将被拒绝。
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

class MATRADEAPI CCliTradeSpi
{
public:
  // 客户端与服务器成功建立通信连接后，该方法被调用
  virtual int OnConnected(void) {return 0;}

  // 客户端与服务器成功的通信连接断开时，该方法被调用
  virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) {return 0;}

  // 客户端上传文件时，该方法被调用 p_nResult:进度百分比
  virtual int OnFileUpload(const char *p_pszFileName, int p_nResult) {return 0;}

  // 客户端下载文件时，该方法被调用 p_nResult:进度百分比
  virtual int OnFileDownload(const char *p_pszFileName, int p_nResult) {return 0;}

  // 客户端上传目录时，每个文件上传时该方法被调用 p_nResult:进度百分比
  virtual int OnFolderUpload(const char *p_pszFileName, int p_nResult) {return 0;}

  // 客户端下载目录时，每个文件下载时该方法被调用 p_nResult:进度百分比
  virtual int OnFolderDownload(const char *p_pszFileName, int p_nResult) {return 0;}

  // 连接保活响应
  virtual int OnRspHeartBeat(CFirstSetField *p_pFirstSetField, CRspHeartBeatField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 安全检查响应
  virtual int OnRspSafeCheck(CFirstSetField *p_pFirstSetField, CRspSafeCheckField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 文件信息查询响应
  virtual int OnRspQryFileInfo(CFirstSetField *p_pFirstSetField, CRspQryFileInfoField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 策略启动响应
  virtual int OnRspStgyStart(CFirstSetField *p_pFirstSetField, CRspStgyStartField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 策略停止响应
  virtual int OnRspStgyStop(CFirstSetField *p_pFirstSetField, CRspStgyStopField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 策略状态查询响应
  virtual int OnRspQryStgyStat(CFirstSetField *p_pFirstSetField, CRspQryStgyStatField *p_pRspField, LONGLONG p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // COS系统登录功能响应
  virtual int OnRspCosLogin(CFirstSetField *p_pFirstSet, CRspCosLoginField *p_pRspField, LONGLONG  p_llRequestId, int p_iFieldNum, int p_iFieldIndex) {return 0;}

  // 订阅响应
  virtual int OnRtnSubTopic(CRspSubTopicField* p_pRspField) {return 0;}

  // 取消订阅响应
  virtual int OnRtnUnsubTopic(CRspUnsubTopicField* p_pRspField) {return 0;}

  // 风控信息推送消息 主题TRADERISK+X
  virtual int OnRtnTradeRisk(CRtnTradeRiskInfoField* p_pRspField) {return 0;}

  // 系统状态推送消息 主题SYSTEM00
  virtual int OnRtnSystemInfo(CRtnSystemInfoField* p_pRspField) {return 0;}

  // 账户通知推送消息 主题NOTICE+XX
  virtual int OnRtnNoticeInfo(CRtnNoticeInfoField* p_pRspField) {return 0;}

};

class MATRADEAPI CCliTradeApi
{
public:
  // 默认构造函数
  CCliTradeApi(void);

  // 析构函数
  virtual ~CCliTradeApi(void);

  // 初始化
  virtual int Init(void);

  // 退出
  virtual int Exit(void);

  // 注册连接信息
  virtual int RegisterServer(const char *p_pszIp, int p_iPort, unsigned int p_uiTimeout = 1, const char* p_pszPktVer = "01", bool p_bCos = false, int p_iSsl = -1);

  // 注册连接信息
  virtual int RegisterServer(CReqSysField* p_pstSysField);

  ///注册回调接口
  virtual int RegisterSpi(CCliTradeSpi *p_pSpi);

  // 注册账号信息
  virtual int RegisterAcct(char p_chChannel, const char* p_pszOpSite = 0, int p_iSiteLen = 0);

  // 注册账号信息
  virtual int RegisterAcct(CReqFixField* p_pstFixField);

  // 注册终端信息
  virtual int RegisterClient(CReqClientField* p_pstClientField);

  // 连接保活请求
  virtual int ReqHeartBeat(CReqHeartBeatField *p_pReqField, LONGLONG p_llRequestId);

  // 安全检查请求
  virtual int ReqSafeCheck(CReqSafeCheckField *p_pReqField, LONGLONG p_llRequestId);

  // 上传文件请求
  virtual int ReqFileUpload(CReqFileUploadField *p_pReqField, LONGLONG p_llRequestId);

  // 下载文件请求
  virtual int ReqFileDownload(CReqFileDownloadField *p_pReqField, LONGLONG p_llRequestId);

  // 上传目录请求
  virtual int ReqFolderUpload(CReqFolderUploadField *p_pReqField, LONGLONG p_llRequestId);

  // 下载目录请求
  virtual int ReqFolderDownload(CReqFolderDownloadField *p_pReqField, LONGLONG p_llRequestId);

  // 文件信息查询请求
  virtual int ReqQryFileInfo(CReqQryFileInfoField *p_pReqField, LONGLONG p_llRequestId);

  // 策略启动请求
  virtual int ReqStgyStart(CReqStgyStartField *p_pReqField, LONGLONG p_llRequestId);

  // 策略停止请求
  virtual int ReqStgyStop(CReqStgyStopField *p_pReqField, LONGLONG p_llRequestId);

  // 策略状态查询请求
  virtual int ReqQryStgyStat(CReqQryStgyStatField *p_pReqField, LONGLONG p_llRequestId);

  // COS系统登录功能请求
  virtual int ReqCosLogin(CReqCosLoginField *p_pReqField, LONGLONG p_llRequestId);

  // 设置业务包头信息
  int SetBizPackHead(const char *p_pszFuncId, LONGLONG p_llRequestId, char *p_pszMsgId, char p_chPkgType, char p_chPkgVer = '2', char p_chFuncType = 0);

  // 订阅主题
  int SubTopic(const char *p_pszTopic, const char *p_pszFilter, char p_chDataSet = '0');

  // 退订主题
  int UnsubTopic(const char *p_pszTopic, LONGLONG llAcceptSn = 0);

  // 订阅请求响应
  void OnRspSubTopic(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 退订请求响应
  void OnRspUnsubTopic(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 连接保活请求响应
  void OnRspHeartBeat(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 安全检查请求响应
  void OnRspSafeCheck(CFirstSetField *p_pFirstSetField, LONGLONG  p_llRequestId, int p_iFieldNum);

  // 文件信息查询请求响应
  void OnRspQryFileInfo(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 策略启动请求响应
  void OnRspStgyStart(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 策略停止请求响应
  void OnRspStgyStop(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 策略状态查询请求响应
  void OnRspQryStgyStat(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 量化系统注册功能响应
  void OnRspCosLogin(CFirstSetField *p_pFirstSetField, LONGLONG p_llRequestId, int p_iFieldNum);

  // 获取最后错误信息
  const char* GetLastErrorText(void);

public:
  // 异步回调函数
  virtual void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 发布回调函数
  virtual void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 连接回调函数
  virtual void OnNetCallback(const char *p_pszNetState, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 文件上传回调
  virtual void OnFileUpCallback(const char *p_pszFileName, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 文件下载回调
  virtual void OnFileDownCallback(const char *p_pszFileName, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 目录上传回调
  virtual void OnFolderUpCallback(const char *p_pszFileName, const unsigned char *p_pszDataBuff, int p_iDataLen);

  // 目录下载回调
  virtual void OnFolderDownCallback(const char *p_pszFileName, const unsigned char *p_pszDataBuff, int p_iDataLen);

protected:
  int OnBeginCall(void);
  int OnEndCall(void);
  int OnDoCall(void* p_pvAsynCall);
  int OnCheckPointer(void* p_pVar, const char* p_pszVar);
  
protected:
  void             *m_hAsynCall;                 // 系统调用句柄
  void             *m_hAnsParse;                 // 应答解包句柄
  void             *m_hPubParse;                 // 推送解包句柄
  CReqSysField      m_stSysField;                // 系统连接参数
  CReqFixField      m_stReqFixField;             // 请求固定参数
  CReqClientField   m_stReqClientField;          // 请求终端参数
  CCliTradeSpi     *m_pTradeSpi;                 // 回调实例指针
  char              m_szLastErrorText[1024 + 1]; // 最后错误信息
  char              m_szClientInfo[256 + 1];     // 请求终端信息
  void             *m_pAcctInfo;                 // 登录账户信息
};

END_NAMESPACE_MACLI

#endif  // __MA_CLI_TRADE_API_H__