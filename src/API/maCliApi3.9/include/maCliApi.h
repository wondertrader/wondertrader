//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maCliApi.h
// 模块名称：ma微架构客户端(Client)的C语言API接口
// 模块描述：
// 开发作者：何万刚
// 创建日期：2012-10-10
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2012-10-10    1.0          何万刚          原创
// 2012-10-17    1.1          陆明其          实现具体功能
// 2013-09-16    1.2          董建军          调整具体功能
// 2013-12-27    1.3          郭玉臻          增加加密函数
// 2014-11-21    1.4          何万刚          增加客户端日志功能
// 2015-01-26    1.5          何万刚          设置默认回调函数与推送全部成交信息
// 2015-03-28    1.6          何万刚          增加回调响应函数消耗监视/
// 2015-07-17    1.7          何万刚          增加日志模式
// 2016-08-16    2.0          张东方          支持SSL通信加密
// 2019-03-23    3.0          何万刚          增加文件传输功能
// 2019-08-15    3.1          卜文军          删除通信类型1-KCXP/2-0MQ/4-SHM类型
// 2019-08-15    3.2          卜文军          增加MACLI_OPTION_LPORT、MACLI_OPTION_IPORT选项
// 2022-11-10    3.9          何万刚          增加MACLI_HEAD_FID_REQTIME、MACLI_HEAD_FID_ANSTIME、MACLI_HEAD_FID_TRACEID、MACLI_HEAD_FID_SPANDID通信包头字段域，用于链路跟踪
//----------------------------------------------------------------------------
#if !defined(__MA_CLI_API_H__)
#define __MA_CLI_API_H__

#if defined(WIN32) || defined(WIN64) || defined(OS_IS_WINDOWS)
  #if defined(MA_CLIAPI_EXPORTS)
    #define MACLIAPI __declspec(dllexport)
  #else
    #define MACLIAPI __declspec(dllimport)
  #endif
  #define MACLI_STDCALL __stdcall
  #define MACLI_EXPORTS __declspec(dllexport)
#else
  #define MACLIAPI
  #define MACLI_STDCALL
  #define MACLI_EXPORTS
  #if !defined __int64
    #define __int64 long long
  #endif
#endif

//////////////////////////////////////////////////////////////////////////
//通信包头字段域定义
#define MACLI_HEAD_FID_PKT_LEN              0         //(只读)  p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int)
#define MACLI_HEAD_FID_PKT_CRC              1         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >4
#define MACLI_HEAD_FID_PKT_ID               2         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >4
#define MACLI_HEAD_FID_PKT_VER              3         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >2
#define MACLI_HEAD_FID_PKT_TYPE             4         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_MSG_TYPE             5         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_RESEND_FLAG          6         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_TIMESTAMP            7         //(只读)  p_pvdValuePtr=(void *)(__int64 *)         p_iValueSize=sizeof(__int64)
#define MACLI_HEAD_FID_MSG_ID               8         //(读写)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32 [写] strlen(p_pvdValuePtr)
#define MACLI_HEAD_FID_CORR_ID              9         //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32
//#define MACLI_HEAD_FID_MSG_INDEX          10        //(只读)
#define MACLI_HEAD_FID_FUNC_ID              11        //(读写)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32 [写] strlen(p_pvdValuePtr)
#define MACLI_HEAD_FID_SRC_NODE             12        //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >4
#define MACLI_HEAD_FID_DEST_NODE            13        //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >4
#define MACLI_HEAD_FID_PAGE_FLAG            14        //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_PAGE_NO              15        //(只读)  p_pvdValuePtr=(void *)(int  *)            p_iValueSize=sizeof(int)
#define MACLI_HEAD_FID_PAGE_CNT             16        //(只读)  p_pvdValuePtr=(void *)(int  *)            p_iValueSize=sizeof(int)
#define MACLI_HEAD_FID_BODY_LEN             21        //(只读)  p_pvdValuePtr=(void *)(int  *)            p_iValueSize=sizeof(int)
#define MACLI_HEAD_FID_PKT_HEAD_END         25        //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >4
#define MACLI_HEAD_FID_PKT_HEAD_LEN         35        //(只读)  p_pvdValuePtr=(void *)(int  *)            p_iValueSize=sizeof(int)
#define MACLI_HEAD_FID_PKT_HEAD_MSG         41        //(只读)  p_pvdValuePtr=(void *)(char  *)           p_iValueSize=[读] > 通过MACLI_HEAD_FID_PKT_HEAD_END获取得到的数值
#define MACLI_HEAD_FID_PKT_BODY_MSG         42        //(只读)  p_pvdValuePtr=(void *)(char  *)           p_iValueSize=[读] > 通过MACLI_HEAD_FID_PKT_LEN获取得到的数值 - 通过MACLI_HEAD_FID_PKT_HEAD_END获取得到的数值
#define MACLI_HEAD_FID_PKT_MSG              43        //(只读)  p_pvdValuePtr=(void *)(char  *)           p_iValueSize=[读] > 通过MACLI_HEAD_FID_PKT_LEN获取得到的数值
#define MACLI_HEAD_FID_REQTIME              48        //(只读)  p_pvdValuePtr=(void *)(__int64 *)         p_iValueSize=sizeof(__int64)
#define MACLI_HEAD_FID_ANSTIME              49        //(只读)  p_pvdValuePtr=(void *)(__int64 *)         p_iValueSize=sizeof(__int64)
#define MACLI_HEAD_FID_TRACEID              50        //(读写)  p_pvdValuePtr=(void *)(__int64 *)         p_iValueSize=sizeof(__int64)
#define MACLI_HEAD_FID_SPANDID              51        //(读写)  p_pvdValuePtr=(void *)(__int64 *)         p_iValueSize=sizeof(__int64)
#define MACLI_HEAD_FID_FUNC_TYPE            1052672   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_BIZ_CHANNEL          1052674   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >2
#define MACLI_HEAD_FID_TOKEN_FLAG           1069056   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >1
#define MACLI_HEAD_FID_PUB_TOPIC            1073152   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >12
#define MACLI_HEAD_FID_PUB_KEY1             1073153   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32
#define MACLI_HEAD_FID_PUB_KEY2             1073154   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32
#define MACLI_HEAD_FID_PUB_KEY3             1073155   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >32
#define MACLI_HEAD_FID_USER_SESSION         1871872   //(只读)  p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >64

//////////////////////////////////////////////////////////////////////////
//选项索引
#define MACLI_OPTION_CONNECT_PARAM          1         //(读写) p_pvdValuePtr=(ST_MACLI_CONNECT_OPTION *) p_iValueSize=sizeof(ST_MACLI_CONNECT_OPTION)
#define MACLI_OPTION_SYNCCALL_TIMEOUT       2         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int)
#define MACLI_OPTION_ASYNCALL_TIMEOUT       3         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int)
#define MACLI_OPTION_WRITELOG_LEVEL         4         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int)
#define MACLI_OPTION_WRITELOG_PATH          5         //(读写) p_pvdValuePtr=(void *)(char *)            p_iValueSize=[读] >256 [写] strlen(p_pvdValuePtr)
#define MACLI_OPTION_WRITELOG_SIZE          6         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int) [单位MB]
#define MACLI_OPTION_WRITELOG_TIME          7         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int) [秒]
#define MACLI_OPTION_WRITELOG_MODE          8         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int)
#define MACLI_OPTION_SSL_CONNECTION         9         //(读写) p_pvdValuePtr=(int *)                     p_iValueSize=sizeof(int) [是否支持SSL加密通信,1加密0为非加密]
#define MACLI_OPTION_LIP_ADDR               10        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_MAC_ADDR               11        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_APP_NAME               12        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_HD_ID                  13        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_CPU_INFO               14        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_CPU_ID                 15        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_PC_NAME                16        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_HD_PART                17        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_SYS_VOL                18        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_OS_VER                 19        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_SIP_ADDR               20        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_LPORT                  21        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256
#define MACLI_OPTION_IPORT                  22        //(读)  p_pvdValuePtr=(char *)                     p_iValueSize=[读] >256

//////////////////////////////////////////////////////////////////////////
// 日志模式定义:默认为循环日志
#define MACLI_WRITELOG_MODE_LOOP            0         //循环模式（日志文件达到指定大小，清空再重写此日志文件)[maCliApi.log]
#define MACLI_WRITELOG_MODE_FORWARD         1         //递增模式（日志文件达到指定大小，另写一个日志文件）   [maCliApixxxx.log] xxxx从0000到9999递增

//////////////////////////////////////////////////////////////////////////
// 日志级别定义（采用叠加方式）:例如 调试信息[1] + 错误信息[8] = 9
#define MACLI_WRITELOG_LEVEL_NOLOG          0         //无日志
#define MACLI_WRITELOG_LEVEL_ALLLOG         63        //所有日志
#define MACLI_WRITELOG_LEVEL_DEBUG          1         //调试信息(收发包具体数据内容)
#define MACLI_WRITELOG_LEVEL_INFO           2         //运行信息(收发包简要内容：功能号、包大小)
#define MACLI_WRITELOG_LEVEL_WARN           4         //警告信息(收发包通信断开重连、服务器切换、消息超时等)
#define MACLI_WRITELOG_LEVEL_ERROR          8         //错误信息(收发包通信失败)
#define MACLI_WRITELOG_LEVEL_FATAL          16        //严重错误(程序异常退出)
#define MACLI_WRITELOG_LEVEL_IMPORTANT      32        //重要信息(连接建立、连接关闭等)


//////////////////////////////////////////////////////////////////////////
// 通信类型定义
//#define COMM_TYPE_KCXP                    1       // 作废(maCliApi 3.1)
//#define COMM_TYPE_ZMQ                     2       // 作废(maCliApi 3.1)
#define COMM_TYPE_SOCKET                    3
//#define COMM_TYPE_SHM                     4       // 作废(maCliApi 3.1)

//////////////////////////////////////////////////////////////////////////
// 常用功能定义
#define MA_FUNC_SUBSCRIBE                   "00102012"  //订阅
#define MA_FUNC_UNSUBSCRIBE                 "00102013"  //取消订阅
#define MA_FUNC_PUB_CONTENT                 "00102020"  //发布
#define MA_FUNC_LOGIN_API                   "10301105"  //用户登录(API)
#define MA_FUNC_LOGIN_ACCREDIT              "10301104"  //客户授信登录

//////////////////////////////////////////////////////////////////////////
// 网络连接状态定义
#define MA_NET_CONNET                       "0"       //连接成功
#define MA_NET_BREAK                        "1"       //连接断开

//////////////////////////////////////////////////////////////////////////
//回调申明
typedef void *MACLIHANDLE;
// 推送内容函数原型如:OnPulish(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen) 
// 异步应答函数原型如:OnAnswer(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen) 
// 文件上传函数原型如:OnFileUp(const char *p_pszFileName, const unsigned char *p_pszError, int p_iPercent)   // p_iPercent > 0 正常传输  p_iPercent < 0 异常
// 文件下载函数原型如:OnFileDown(const char *p_pszFileName, const unsigned char *p_pszError, int p_iPercent) // p_iPercent > 0 正常传输  p_iPercent < 0 异常
typedef void (MACLI_Callback) (const char *, const unsigned char *, int);
typedef MACLI_Callback *MACLI_NOTIFY;

#define MACLI_SERVERNAME_MAX                32
#define MACLI_ADRRESS_MAX                   128
#define MACLI_PROXY_MAX                     128
#define MACLI_SSL_MAX                       256
#define MACLI_MSG_ID_SIZE                   32
#define MACLI_TOPIC_MAX                     12
#define MACLI_TERM_CODE                     32
#define MACLI_USERDATA_MAX                  256
#define MACLI_USERINFO_MAX                  64
#define MACLI_FUNC_ID_SIZE                  8   //功能号
#define MACLI_PUB_TOPIC_SIZE                12  //主题

#if (defined(OS_IS_AIX) && defined(__xlC__))
#pragma options align = packed
#else
#pragma pack(1)
#endif

typedef struct
{
  char szServerName[MACLI_SERVERNAME_MAX + 1];  //服务器别名
  int nCommType;                                //通信类型：1-KCXP 2-0MQ 3-SOCKET 4-SHM
  int nProtocal;                                //通信协议：1-TCP 2-UDP 3-EPGM 4-IPC
  char szSvrAddress[MACLI_ADRRESS_MAX + 1];     //服务器地址
  int nSvrPort;                                 //服务器端口
  char szLocalAddress[MACLI_ADRRESS_MAX + 1];   //本地地址
  int nLocalPort;                               //本地端口

  int nReqId;                                   //请求标识：通信类型为4-SHM时为请求队列编号
  int nAnsId;                                   //应答标识：通信类型为4-SHM时为应答队列编号

  char szSubId[MACLI_SERVERNAME_MAX + 1];       //订阅标识: 通信类型为KCXP时为订阅队列

  char szProxy[MACLI_PROXY_MAX + 1];            //代理服务器(保留)
  char szSSL[MACLI_SSL_MAX + 1];                //SSL(保留)
  char szTermCode[MACLI_TERM_CODE + 1];         //终端特征码

  char szReqName[MACLI_SERVERNAME_MAX + 1];     //通信类型为4-SHM时填写共享内存请求队列名称
  char szAnsName[MACLI_SERVERNAME_MAX + 1];     //通信类型为4-SHM时填写共享内存应答队列名称
  char szReqConnstr[MACLI_ADRRESS_MAX + 1];     //通信类型为4-SHM时填写共享内存请求队列连接串, 例如IPC/@@IP
  char szAnsConnstr[MACLI_ADRRESS_MAX + 1];     //通信类型为4-SHM时填写共享内存应答队列连接串

  char szMonitorReqName[MACLI_SERVERNAME_MAX + 1];     //监听请求队列名称
  char szMonitorAnsName[MACLI_SERVERNAME_MAX + 1];     //监听应答队列名称
  char szMonitorReqConnstr[MACLI_ADRRESS_MAX + 1];     //监听请求队列连接串, 例如IPC/@@IP
  char szMonitorAnsConnstr[MACLI_ADRRESS_MAX + 1];     //监听应答队列连接串

  char szSocketSubConnstr[MACLI_ADRRESS_MAX + 1];      //SOCKET订阅队列连接串

  int nReqMaxDepth;                             //通信类型为4-SHM时填写共享内存请求队列最大深度
  int nAnsMaxDepth;                             //通信类型为4-SHM时填写共享内存应答队列最大深度
} ST_MACLI_CONNECT_OPTION;

typedef struct
{
  char szFuncId[MACLI_FUNC_ID_SIZE + 1];        //功能号
  MACLI_NOTIFY pfnCallback;                     //异步应答回调函数指针
} ST_MACLI_ARCALLBACK;

typedef struct
{
  char szTopic[MACLI_PUB_TOPIC_SIZE + 1];       //主题
  MACLI_NOTIFY pfnCallback;                     //发布内容回调函数指针
} ST_MACLI_PSCALLBACK;

typedef struct
{
  MACLI_NOTIFY pfnCallback;                     //连接信息回调函数指针
} ST_MACLI_NETCALLBACK;

typedef struct
{
  char szServerName[MACLI_USERINFO_MAX + 1];    //连接名称
  char szUserId    [MACLI_USERINFO_MAX + 1];    //用户名  
  char szPassword  [MACLI_USERINFO_MAX + 1];    //密码    
  char szAppId     [MACLI_USERINFO_MAX + 1];    //APP标识 
  char szAuthCode  [MACLI_USERINFO_MAX + 1];    //认证码  
} ST_MACLI_USERINFO;

typedef struct
{
  char szFuncId[MACLI_FUNC_ID_SIZE + 1];        //功能号
  char szMsgId[MACLI_MSG_ID_SIZE + 1];          //消息ID
  int nTimeout;                                 //调用超时
  char szUserData1[MACLI_USERDATA_MAX + 1];     //用户数据1
  char szUserData2[MACLI_USERDATA_MAX + 1];     //用户数据2
} ST_MACLI_SYNCCALL;

typedef struct
{
  char szFuncId[MACLI_FUNC_ID_SIZE + 1];        //功能号
  char szMsgId[MACLI_MSG_ID_SIZE + 1];          //消息ID
  int nTimeout;                                 //调用超时
  char szUserData1[MACLI_USERDATA_MAX + 1];     //用户数据1
  char szUserData2[MACLI_USERDATA_MAX + 1];     //用户数据2
} ST_MACLI_ASYNCALL;

typedef struct
{
  char szTopic[MACLI_PUB_TOPIC_SIZE + 1];       //主题
  char szAcceptSn[MACLI_MSG_ID_SIZE + 1];       //订阅受理号
  int nTimeout;                                 //调用超时
  char szUserData1[MACLI_USERDATA_MAX + 1];     //用户数据1
  char szUserData2[MACLI_USERDATA_MAX + 1];     //用户数据2
} ST_MACLI_PUBCALL;


#if (defined(OS_IS_AIX) && defined(__xlC__))
#pragma options align = reset
#else
#pragma pack()
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//初始化/清理
MACLIAPI int MACLI_STDCALL maCli_Init(MACLIHANDLE *p_phHandle);
MACLIAPI int MACLI_STDCALL maCli_Exit(MACLIHANDLE p_hHandle);

//版本/参数
MACLIAPI int MACLI_STDCALL maCli_GetVersion(MACLIHANDLE p_hHandle, char *p_pszVer, int p_iVerSize);
MACLIAPI int MACLI_STDCALL maCli_GetOptions(MACLIHANDLE p_hHandle, int p_iOptionIdx, void *p_pvdValuePtr, int p_iValueSize);
MACLIAPI int MACLI_STDCALL maCli_SetOptions(MACLIHANDLE p_hHandle, int p_iOptionIdx, void *p_pvdValuePtr, int p_iValueSize);
MACLIAPI int MACLI_STDCALL maCli_SetArCallback(MACLIHANDLE p_hHandle, ST_MACLI_ARCALLBACK *p_pstArCallback);
MACLIAPI int MACLI_STDCALL maCli_SetPsCallback(MACLIHANDLE p_hHandle, ST_MACLI_PSCALLBACK *p_pstPsCallback);
MACLIAPI int MACLI_STDCALL maCli_SetNetCallback(MACLIHANDLE p_hHandle, ST_MACLI_NETCALLBACK *p_pstNetCallback);

//连接
MACLIAPI int MACLI_STDCALL maCli_Open(MACLIHANDLE p_hHandle, ST_MACLI_USERINFO *p_pstUserInfo);
MACLIAPI int MACLI_STDCALL maCli_MonitorOpen(MACLIHANDLE p_hHandle, ST_MACLI_USERINFO *p_pstUserInfo);
MACLIAPI int MACLI_STDCALL maCli_Close(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_ReOpen(MACLIHANDLE p_hHandle);

//调用1
MACLIAPI int MACLI_STDCALL maCli_SyncCall(MACLIHANDLE p_hHandle, ST_MACLI_SYNCCALL *p_pstSyncCall);
MACLIAPI int MACLI_STDCALL maCli_AsynCall(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall);
MACLIAPI int MACLI_STDCALL maCli_AsynGetReply(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall);
MACLIAPI int MACLI_STDCALL maCli_GetPsContent(MACLIHANDLE p_hHandle, ST_MACLI_PUBCALL *p_pstPubCall);

//调用2
MACLIAPI int MACLI_STDCALL maCli_SyncCall2(MACLIHANDLE p_hHandle, ST_MACLI_SYNCCALL *p_pstSyncCall,
  const unsigned char *p_pszReqData, int p_iReqDataLen, unsigned char **p_ppszAnsData, int *p_piAnsDataLen);
MACLIAPI int MACLI_STDCALL maCli_AsynCall2(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall,
  const unsigned char *p_pszReqData, int p_iReqDataLen);
MACLIAPI int MACLI_STDCALL maCli_AsynGetReply2(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall,
  unsigned char **p_ppszAnsData, int *p_piAnsDataLen);
MACLIAPI int MACLI_STDCALL maCli_GetPsContent2(MACLIHANDLE p_hHandle, ST_MACLI_PUBCALL *p_pstPubCall,
  unsigned char **p_ppszPubData, int *p_piPubDataLen);

MACLIAPI int MACLI_STDCALL maCli_AsynMonitorCall2(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall,
  const unsigned char *p_pszReqData, int p_iReqDataLen);
MACLIAPI int MACLI_STDCALL maCli_AsynMonitorGetReply2(MACLIHANDLE p_hHandle, ST_MACLI_ASYNCALL *p_pstAsynCall,
  unsigned char **p_ppszAnsData, int *p_piAnsDataLen);

//组包(请求)
MACLIAPI int MACLI_STDCALL maCli_Make(MACLIHANDLE p_hHandle, unsigned char **p_ppszReqData, int *p_piReqDataLen);
MACLIAPI int MACLI_STDCALL maCli_BeginWrite(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_EndWrite(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_CreateTable(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_AddRow(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_SaveRow(MACLIHANDLE p_hHandle);

MACLIAPI int MACLI_STDCALL maCli_SetHdrValue(MACLIHANDLE p_hHandle, const unsigned char *p_pszValue, int p_iValLen, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetHdrValueS(MACLIHANDLE p_hHandle, const char *p_pszValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetHdrValueN(MACLIHANDLE p_hHandle, int p_iValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetHdrValueC(MACLIHANDLE p_hHandle, char p_chValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetHdrValueD(MACLIHANDLE p_hHandle, double p_dValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetHdrValueL(MACLIHANDLE p_hHandle, __int64 p_i64Value, int p_iFieldIdx);

MACLIAPI int MACLI_STDCALL maCli_SetValue(MACLIHANDLE p_hHandle, const unsigned char *p_pszValue, int p_iValLen, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetValueS(MACLIHANDLE p_hHandle, const char *p_pszValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetValueN(MACLIHANDLE p_hHandle, int p_iValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetValueC(MACLIHANDLE p_hHandle, char p_chValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetValueD(MACLIHANDLE p_hHandle, double p_dValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_SetValueL(MACLIHANDLE p_hHandle, __int64 p_i64Value, const char *p_pszFieldIdx);

//解包(应答)
MACLIAPI int MACLI_STDCALL maCli_Parse(MACLIHANDLE p_hHandle, const unsigned char *p_pszAnsData, int p_iAnsDataLen);
MACLIAPI int MACLI_STDCALL maCli_GetTableCount(MACLIHANDLE p_hHandle, int *p_piTableCount);
MACLIAPI int MACLI_STDCALL maCli_OpenTable(MACLIHANDLE p_hHandle, int p_iTableIndex);
MACLIAPI int MACLI_STDCALL maCli_GetRowCount(MACLIHANDLE p_hHandle, int *p_piRowCount);
MACLIAPI int MACLI_STDCALL maCli_ReadRow(MACLIHANDLE p_hHandle, int p_iRowIndex);

MACLIAPI int MACLI_STDCALL maCli_GetHdrValue(MACLIHANDLE p_hHandle, unsigned char *p_pszValue, int p_iValSize, int *p_piValLen, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetHdrValueS(MACLIHANDLE p_hHandle, char *p_pszValue, int p_iValSize, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetHdrValueN(MACLIHANDLE p_hHandle, int *p_piValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetHdrValueC(MACLIHANDLE p_hHandle, char *p_pchValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetHdrValueD(MACLIHANDLE p_hHandle, double *p_pdValue, int p_iFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetHdrValueL(MACLIHANDLE p_hHandle, __int64 *p_pi64Value, int p_iFieldIdx);

MACLIAPI int MACLI_STDCALL maCli_GetValue(MACLIHANDLE p_hHandle, unsigned char *p_pszValue, int p_iValSize, int *p_piValLen, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetValueS(MACLIHANDLE p_hHandle, char *p_pszValue, int p_iValSize, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetValueN(MACLIHANDLE p_hHandle, int *p_piValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetValueC(MACLIHANDLE p_hHandle, char *p_pchValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetValueD(MACLIHANDLE p_hHandle, double *p_pdValue, const char *p_pszFieldIdx);
MACLIAPI int MACLI_STDCALL maCli_GetValueL(MACLIHANDLE p_hHandle, __int64 *p_pi64Value, const char *p_pszFieldIdx);

//辅助
MACLIAPI int MACLI_STDCALL maCli_GetUuid(MACLIHANDLE p_hHandle, char *p_pszUuid, int p_iUuidSize);
MACLIAPI int MACLI_STDCALL maCli_RestorePsList(MACLIHANDLE p_hHandle);
MACLIAPI int MACLI_STDCALL maCli_GetLastErrorCode(MACLIHANDLE p_hHandle, int *p_piErrorCode);
MACLIAPI int MACLI_STDCALL maCli_GetLastErrorMsg(MACLIHANDLE p_hHandle, char *p_pszErrorMsg, int p_iMsgSize);
MACLIAPI int MACLI_STDCALL maCli_ComEncrypt(MACLIHANDLE p_hHandle, char *p_pszOutput, int p_iSize, const char *p_pszInput, const char *p_pszKey);
MACLIAPI int MACLI_STDCALL maCli_ComDecrypt(MACLIHANDLE p_hHandle, char *p_pszOutput, int p_iSize, const char *p_pszInput, const char *p_pszKey);
MACLIAPI int MACLI_STDCALL maCli_GetConnIpAddr(MACLIHANDLE p_hHandle, char *p_pszIpAddr, int p_iIpAddrSize);
MACLIAPI int MACLI_STDCALL maCli_GetConnMac(MACLIHANDLE p_hHandle, char *p_pszMac, int p_iMacSize);
MACLIAPI int MACLI_STDCALL maCli_FileUpload(MACLIHANDLE p_hHandle, const char *p_pszFileId, const char *p_pszLocalFileName, MACLI_NOTIFY p_pfnCbProgress);
MACLIAPI int MACLI_STDCALL maCli_FileDownload(MACLIHANDLE p_hHandle, const char *p_pszFileId, const char *p_pszLocalFileName, MACLI_NOTIFY p_pfnCbProgress);
MACLIAPI int MACLI_STDCALL maCli_FolderUpload(MACLIHANDLE p_hHandle, const char *p_pszFileId, const char *p_pszLocalFolder, MACLI_NOTIFY p_pfnCbProgress);
MACLIAPI int MACLI_STDCALL maCli_FolderDownload(MACLIHANDLE p_hHandle, const char *p_pszFileId, const char *p_pszLocalFolder, MACLI_NOTIFY p_pfnCbProgress);

#ifdef __cplusplus
}
#endif

#endif  //__MA_CLI_API_H__
