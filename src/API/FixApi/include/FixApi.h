/*! @mainpage  东吴证券股份有限公司易驰统一接入SDK V 1.0
* @author 吴宏兵 wuhb@dwzq.com.cn
* @section 介绍
* - 接口介绍:\n
*      东吴证券易驰统一接入最大限度的适配了顶点Fix api C接口，用户参见顶点Fix api用户即可，如有需要，可直接对顶点的Fixapi组件进行替换，可更好的方便三方客户连接东吴证券核心交易系统。\n
* - 接口文件:\n
*   1. 东吴证券易驰统一接入接口头文件： FixApi.h\n
*   \n
* - 库文件:\n
*   3. 接入主组件：FixApi.lib FixApi.dll\n
*   4. 内部依赖运行组件：fixapitool.dll\n
* @example TestHandler.h
* @example TestHandler.cpp
* @example HttpConnection.h
* @example HttpConnection.cpp
* @example HttpsClient.h
* @example HttpsClient.cpp
*/
#if !defined(AFX_FIXAPI_H__VER2001__INCLUDED_)
#define AFX_FIXAPI_H__VER2001__INCLUDED_

#include <string>

#ifdef USE_FIXAPI_CLASS
#include <FixApi/FixConn.h>
#endif

#ifndef _MSC_VER
#define FIX_API_EXPORT extern "C"
#define FIX_UDT_EXPORT 
#else
#ifndef FIX_API_EXPORT
#define FIX_API_EXPORT extern "C" __declspec(dllimport)
#define FIX_UDT_EXPORT __declspec(dllimport)
#endif
#endif

#ifndef _MSC_VER
	#define FIX_INT64 long
	#define HANDLE_ID long
#else
	#define FIX_INT64 __int64
	#ifdef _WIN64
		#define HANDLE_ID __int64
	#else
		#define HANDLE_ID int
	#endif
#endif

//#ifdef _WIN64
//	#define FIX_INT64 __int64
//	#define HANDLE_ID long
//#else
//#endif

// 连接句柄
#define HANDLE_CONN     HANDLE_ID

// 会话句柄
#define HANDLE_SESSION  HANDLE_ID

// 服务句柄
#define HANDLE_SVC      HANDLE_ID
//#ifdef _WIN64
////连接句柄
//#define HANDLE_CONN     HANDLE_ID
//
////会话句柄
//#define HANDLE_SESSION  HANDLE_ID
//
////服务句柄
//#define HANDLE_SVC      HANDLE_ID
//#else
////连接句柄
//#define HANDLE_CONN     HANDLE_ID
//
////会话句柄
//#define HANDLE_SESSION  HANDLE_ID
//
////服务句柄
//#define HANDLE_SVC      HANDLE_ID
//#endif

/* 
函数名称: Fix_AutoLoadOpenSSL

表示接口库内部是否需要自动初始化OpenSSL，默认为初始化。
要求在Fix_Initialize之前调用。
针对一些应用系统本身已经初始化OpenSSL的情况下，再调用Fix_Initialize出错，建议先调用此函数。
如果应用系统本身未初始化OpenSSL的情况，无须调用此函数。

输入参数:
    bAuto:
        true  表示要接口库内部加载
        false 表示在接口库的外部应用程序中加载

返回数据:
    true  表示函数调用成功
    false 表示函数调用失败
*/
FIX_API_EXPORT bool Fix_AutoLoadOpenSSL(bool bAuto);

/**
* @brief 第三方接口库初始化。只有调用一次就可以，不能多次调用。
* @return 返回数据:
* 返回true表示初始化成功；false表示失败。
*/
FIX_API_EXPORT bool Fix_Initialize();
/**
* @brief 第三方接口库初始化。只有调用一次就可以，不能多次调用。
* @return 返回数据:
* 返回true表示初始化成功；false表示失败。
*/
FIX_API_EXPORT bool Fix_SetAppInfo(const char* sAppName, const char* sVer);

/**
* @brief 第三方接口库卸载。只有调用一次就可以，不能多次调用。
* @return 返回数据:
* 返回true表示初始化成功；false表示失败。
*/
FIX_API_EXPORT bool Fix_Uninitialize();

/*
* @brief 设置每个业务请求包缺省的头信息
* @param
*      pszUser     -- [in] 系统要求的柜员代码(八位的字符串)。
*      pszWTFS     -- [in] 委托方式。
*      pszFBDM     -- [in] 发生营业部的代码信息(四位的字符串)
*      pszDestFBDM -- [in] 目标营业部的代码信息(四位的字符串)
* @return 系统返回类型为HANDLE_CONN的连接句柄。如果连接失败则返回false; 成功不为false;
*/
FIX_API_EXPORT bool Fix_SetDefaultInfo(const char* sUser, const char* sWTFS,
    const char* sFBDM, const char* sDestFBDM);

/*
* @brief 设置缺省的区域代码
* @param 区域编码
*/
FIX_API_EXPORT bool Fix_SetDefaultQYBM(const char* sQYBM);
/*
*@brief 设置代理
*/
FIX_API_EXPORT bool Fix_SetProxy(int nType, const char *sAddr, int nPort,
                                 const char *sUser, const char *sPwd);
FIX_API_EXPORT bool Fix_SetCertFile(const char *sFileCert, const char *sCertPwd, 
    const char *sFileCA = "", const char *sProcotol = "", bool bVerify = false);
FIX_API_EXPORT bool Fix_SetZipName(const char *sName);
/**
* @brief 连接到东吴证券易驰统一接入服务器。
* @param 参数说明: (建议以下参数可配置)
*      pszAddr:服务器地址字符串,格式 [ip]@[port]/tcp 例如:127.0.0.1@9901/tcp
*      pszUser:通信用户名 (由客户提供,如没有设置通信用户，为空)
*      pszPwd: 通信密码 (由客户提供,如没有设置通信用户，为空)
*      pszFileCert: 客户端证书文件(PEM证书，由客户提供)
*      pszCertPwd: 客户端证书密码
*      pszProcotol:
*  	             "SSLv3":采用SSLv3协议（不支持）;
* 				 "TLSv1":采用TLSv1协议（支持，会采用TLSV1.2）;
* 				 为空或其他（为保持和顶点SDK接口一致，此接口请用默认值，目前SDK支持TLSV1以上，不支持低版本协议。）: pszFileCert不为空,证书标准安全认证方式
*  				             pszFileCert为空, 标准认证方式
* 	 pszFileCA:信任证书文件（PEM格式，如果不想传入此参数，请手动安装信任证书到系统信任证书列表）
* 	 bVerify:验证服务器证书; true表示验证; false:不验证服务器证书; 建议设置为 true
* 	 nTimeOUT:认证超时时间(单位秒)
* @return 返回数据:
	 系统返回类型为HANDLE_CONN的连接句柄。如果连接失败则返回0; 成功不为0;
 */
//a5没有了
//FIX_API_EXPORT HANDLE_CONN Fix_ConnectEx(const char* sAddr, const char* sUser,
//    const char* sPwd, const char* sFileCert, const char* sCertPwd, const char* sFileCA,
//    const char* sProcotol, bool bVerify, int nTimeOut);

/**
* @brief 连接到东吴证券易驰统一接入服务器。
* @param 参数说明:
*       pszAddr  -- [in] 为要连接的服务器地址; 格式为: "ip地址@端口/tcp"
*       pszUser  -- [in]  通信用户名称; 由客户提供。
*       pszPwd   -- [in] 通信用户的密码; 由客户提供。
*       nTimeOut -- [in] 连接超时的时间。单位为秒。
*
* @return 返回数据:
* 系统返回类型为HANDLE_CONN的连接句柄。如果连接失败则返回0; 成功不为0;
* 此连接默认启用断线自动重连机制，如果要关闭请在DWACC_Connect成功后调用DWACC_SetAutoReconnect函数
*/
FIX_API_EXPORT HANDLE_CONN Fix_Connect(
	const char* sAddr, 
	const char* sUser = "",
    const char* sPwd = "", 
	int nTimeOut = 0);

/**
* @brief 连接到东吴证券易驰统一接入服务器(系统全局的备份地址)。【暂不支持】
* @param 备份地址
*/
FIX_API_EXPORT bool Fix_AddBackupSvrAddr(const char* sAddr);

/**
* @brief 易驰统一接入连接关闭。
* @param 参数说明:
* conn -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由DWACC_Connect生成的。
* @return 返回数据:
*返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_Close(HANDLE_CONN conn);

/**
* @brief 根据一个连接句柄分配一个会话句柄，用于数据交互。
* 由用户自己在应用层通过调用DWACC_AllocateSession分配出来的对象必须由用户自己调用DWACC_ReleaseSession来释放。
* 反之则不必要。原则上遵循“谁分配谁释放”。这样就可以确保内存不泄露。
* @param 参数说明:
* conn -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由DWACC_Connect生成的。
* @return 返回数据:
* 返回值类型为HANDLE_SESSION的会话对象；如果对象值为空表示对象分配失败。否则表示成功。
*/
FIX_API_EXPORT HANDLE_SESSION Fix_AllocateSession(HANDLE_CONN conn);

/**
* @brief 释放会话句柄。
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话对象。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_ReleaseSession(HANDLE_SESSION sess);

/**
* @brief 设置会话交互的超时时间【和顶点接口的区别：此值影响连接数据交互时间】。
* @param 参数说明:
*  conn    -- [in] 类型为HANDLE_CONN的连接句柄。该句柄由DWACC_Connect生成的。
*  timeout -- [in] 业务应答超时时间；单位为秒，系统默认为30秒。
* @return 返回数据:
*  返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetTimeOut(HANDLE_SESSION sess, int timeout);

/*
* @brief 设置会话的委托方式.(必须在Fix_CreateHead函数之前调用)
* @param 参数说明:
*  sess -- [in] 类型为HANDLE_SESSION的会话句柄。
*  val  -- [in] 字符串类型。用于表示客户的接入方式；
比如电话委托，磁卡委托，互联网委托等。
返回数据:
返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetWTFS(HANDLE_SESSION sess, const char* sVal);

/*
* @brief 设置会话的发生营业部代码.(必须在Fix_CreateHead函数之前调用)
* @param 参数说明:
*  sess -- [in] 类型为HANDLE_SESSION的会话句柄。
*  val  -- [in] 字符串类型。用于表示客户业务发生的营业代码,必须是四位的营业部代码;
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetFBDM(HANDLE_SESSION sess, const char* sVal);

/*
* @brief 设置会话的发生营业部代码.(必须在Fix_CreateHead函数之前调用)
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话句柄。
* val  -- [in] 字符串类型。用于表示客户业务到达的目标的营业代码,必须是四位的营业部代码;
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetDestFBDM(HANDLE_SESSION sess, const char* sVal);

/*
* @brief 设置会话的业务发生的节点信息.(必须在Fix_CreateHead函数之前调用)
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话句柄。
* val  -- [in] 字符串类型。一般是客户网卡信息或IP地址。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetNode(HANDLE_SESSION sess, const char* sVal);

/*
* @brief 设置会话的业务发生的柜员代码信息.(必须在Fix_CreateHead函数之前调用)
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话句柄。
* val  -- [in] 字符串类型。对一些柜台特殊业务，需要用到柜员信息进行认证。
* @return 返回数据: 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetGYDM(HANDLE_SESSION sess, const char* sVal);
// 设置会话的附加用户自定义信息，需要在 Fix_CreateHead 前调用
FIX_API_EXPORT bool Fix_SetUserHeadInfo(HANDLE_SESSION sess, const char *sVal);
/**
* @brief 设置会话的业务功能号.具体的功能号的定义请参照【第三方接入业务接口文档】。
* @param 参数说明:
* sess  -- [in]  类型为HANDLE_SESSION的会话句柄。
* nFunc -- [in]  整型。即为系统提供的业务功能号。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_CreateHead(HANDLE_SESSION sess, const char* sFunc);
/**
* @brief 设置会话的业务功能号.具体的功能号的定义请参照【第三方接入业务接口文档】。
* @param 参数说明:
* sess  -- [in]  类型为HANDLE_SESSION的会话句柄。
* nFunc -- [in]  整型。即为系统提供的业务功能号。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_CreateReq(HANDLE_SESSION sess, const char* sFunc);

/**
* @brief 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。
* 注:( 服务应答域数据只是针对一些中间件业务需要第三处理机处理的情况下使用。
* 而且是要求通过注册服务实现; 一般的情况下不使用该方式接入。)
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* id   -- [in]  请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* val  -- [in]  字符串类型;对应于id的业务数据。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetItem(HANDLE_SESSION sess, int id, const char* val);
/**
* @brief 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。
* 注:( 服务应答域数据只是针对一些中间件业务需要第三处理机处理的情况下使用。
* 而且是要求通过注册服务实现; 一般的情况下不使用该方式接入。)
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* id   -- [in]  请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* val  -- [in]  字符串类型;对应于id的业务数据。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetString(HANDLE_SESSION sess, int id, const char* val);
/**
* @brief 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。
* 注:( 服务应答域数据只是针对一些中间件业务需要第三处理机处理的情况下使用。
* 而且是要求通过注册服务实现; 一般的情况下不使用该方式接入。)
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* id   -- [in]  请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* val  -- [in]  字符串类型;对应于id的业务数据。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetLong(HANDLE_SESSION sess, int id, FIX_INT64 val);
FIX_API_EXPORT bool Fix_SetInt(HANDLE_SESSION sess, int id, int val);
FIX_API_EXPORT bool Fix_SetInt64(HANDLE_SESSION sess, int id, FIX_INT64 val);
/**
* @brief 设置该会话要发送给中间件的业务的请求域数据 或服务应答域数据。
* 注:( 服务应答域数据只是针对一些中间件业务需要第三处理机处理的情况下使用。
* 而且是要求通过注册服务实现; 一般的情况下不使用该方式接入。)
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* id   -- [in]  请求域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* val  -- [in]  字符串类型;对应于id的业务数据。
* @return 返回数据:
* 返回值为1表示成功; 0表示失败。
*/
FIX_API_EXPORT bool Fix_SetDouble(HANDLE_SESSION sess, int id, double val, int precise = 6);
// 设置 RawData 字段
FIX_API_EXPORT bool Fix_SetItemBuf(HANDLE_SESSION sess, const char* val, int len);

/**
* @brief 提交业务数据，并等待全部的业务结果返回。
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回值为True表示服务业务处理成功; false表示失败,这个失败是表示业务通信上的失败；
* 失败后，可以通过DWACC_GetCode取出错误码(必定是一个负数)。通过DWACC_GetErrMsg取出错误的信息。
*/
FIX_API_EXPORT bool Fix_Run(HANDLE_SESSION sess);

/**
* @brief 直接放弃等待业务的应答数据。(在同步方式下使用)
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回值为true表示成功; false表示失败。
*/
FIX_API_EXPORT bool Fix_Cancel(HANDLE_SESSION sess);

/*
* @brief 异步提交业务数据，不等待待全部的业务结果返回。
* 和函数Fix_IsReplyed配合使用.
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回值为True表示服务业务处理成功; FALSE表示失败,这个失败是表示业务通信上的失败；
* 失败后，可以通过Fix_GetCode取出错误码(必定是一个负数)。通过Fix_GetErrMsg取出错误的信息。
*/
FIX_API_EXPORT bool Fix_AsyncRun(HANDLE_SESSION sess);

/*
* @brief 异步提交业务数据后，检查数据是否全部返回。【暂不支持】
* @param 参数说明:
* sess -- [in]  类型为HANDLE_SESSION的会话句柄。
* nMSec [in/option] 指定等待的时间，默认不等待。单位为毫秒。
* @return 返回数据:
* 返回值为True表示应答数据全部返回; FALSE表示需要继续等待。
*/
FIX_API_EXPORT bool Fix_IsReplyed(HANDLE_SESSION sess, int nMSec = -1);

/**
* @brief 读取业务提交失败的错误代码。
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回负整数。
*/
FIX_API_EXPORT int Fix_GetCode(HANDLE_SESSION sess);

/**
* @brief 读取业务提交失败的错误信息。
* @param 参数说明:
* sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
* out    -- [in/out] 用于输出错误信息的字符串.
* outlen -- [in] 参数out缓冲区的大小.
* @return 返回数据:
* 返回错误信息字符串。
*/
FIX_API_EXPORT char* Fix_GetErrMsg(HANDLE_SESSION sess, char* out, int& outlen);

/**
* @brief 读取中间件返回的业务应答数据的行数。
* @param 参数说明:
* sess -- [in] 类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回行数量。
*/
FIX_API_EXPORT int Fix_GetCount(HANDLE_SESSION sess);

/**
* @brief 1、从中间件的返回结果集的指定行中读取业务应答数据。
* 2、或者是读取中间件请求的业务请求。
* @param 参数说明:
* sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
* id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* out    -- [in/out] 用于输出业务信息的字符串.
* outlen -- [in] 指定out缓冲区的大小.
* row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
* 表示读取第一行的数据。
* @return 返回数据:
* 返回指定行的的字符串数据。
*/
FIX_API_EXPORT char* Fix_GetItem(HANDLE_SESSION sess, int id, char* out,
    int& outlen, int row = -1);

FIX_API_EXPORT FIX_INT64 Fix_GetInt64(HANDLE_SESSION sess, int id, int row = -1);

/**
* @brief 1、从中间件的返回结果集的指定行中读取业务应答数据。
* 2、或者是读取中间件请求的业务请求。
* @param 参数说明:
* sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
* id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
* 表示读取第一行的数据。
* @return 返回数据:
* 返回指定行的整型数据。
*/
FIX_API_EXPORT FIX_INT64 Fix_GetLong(HANDLE_SESSION sess, int id, int row = -1);

/*
函数名称: Fix_GetPublishId

推送回调函数,可以用这个函数读取序列号
*/
FIX_API_EXPORT FIX_INT64 Fix_GetPublishId(HANDLE_SESSION sess, int row = -1);

/**
* @brief 1、从中间件的返回结果集的指定行中读取业务应答数据。
* 2、或者是读取中间件请求的业务请求。
* @param 参数说明:
* sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
* id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
* 表示读取第一行的数据。
* @return 返回数据:
* 返回指定行的浮点数据。
*/
FIX_API_EXPORT double Fix_GetDouble(HANDLE_SESSION sess, int id, int row = -1);

/**
* @brief 1、检查中间件的返回结果集的指定行中是否含有指定域。
* 2、或者检查中间件请求的业务数据是否含有指定域。
* @param 参数说明:
* sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
* id     -- [in] 指定域的tag值；具体的定义值请参考【第三方接入业务接口文档】。
* row    -- [in][option] 指定返回数据结果集中的行数(行数索引值以0开始).如果不设置这个值，
* 表示读取第一行的数据。
* @return 返回数据:
* 返回1表示,含有指定的域数据；否则没有。
*/
FIX_API_EXPORT bool Fix_HaveItem(HANDLE_SESSION sess, int id, int row = -1);

/*
* @brief 简单加密函数(用户密码加密)【暂不支持】
* @param 参数说明:
* pData        -- [in] 加密数据。
* @return 返回数据:
* 返回加密后的数据。
*/
//FIX_API_EXPORT char* Fix_Encode(char *pData, int& io_len, int nDataLen = 0);

/*
* @brief 取出对于的令牌字符串(可选、与后台系统配置有关)【暂不支持】
*/
FIX_API_EXPORT char* Fix_GetToken(HANDLE_SESSION sess, char* out, int& outlen);

/*
* @brief 设置会话的令牌(可选、与后台系统配置有关)【暂不支持】
*/
FIX_API_EXPORT bool Fix_SetToken(HANDLE_SESSION sess, const char* val);

/*
*@brief 开启写通信日志功能
*/
FIX_API_EXPORT bool Fix_WriteLog(bool bWrite);

/*
* @brief 发出应答数据。
* 具体可以参照Sample2的代码。
* @param 参数说明:
* sess        -- [in] 类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回True表示成功；否则表示失败。
*/
FIX_API_EXPORT bool Fix_Write(HANDLE_SESSION sess);

/*
*@brief 启用/禁用HANDLE_CONN断线自动重连机制，默认启用，如果第三方采用自己管理HANDLE_CONN连接, 必须把自动重连关闭，避免不必要的资源浪费
*/ 
FIX_API_EXPORT bool Fix_SetAutoReconnect(HANDLE_CONN conn, bool bAutoReconnect);
/*
*@brief 获取连接HANDLE_CONN断线自动重连状态
*/
FIX_API_EXPORT bool Fix_GetAutoReconnect(HANDLE_CONN conn);

/*
函数名称: Fix_IsConnect

*@brief 判断连接状态

返回结果:
    true  连接正常; 
    false 连接中断
*/
FIX_API_EXPORT bool Fix_IsConnect(HANDLE_CONN conn);

/*
函数名称: Fix_SubscribeByCustomer

面向主交易系统的客户数据订阅函数
pFunc为回调函数指针,函数定义为:
bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, int64 nSubs, void *pData )
其中conn为连接句柄,
nSubs为Fix_MDBSubscribeByCustomer返回的值，用于指明某个订阅的数据。
pData   就是订阅时的指针(可以为NULL值)

输入参数:
    conn    连接句柄
    sFunc   服务号（功能码）
    pFunc   回调函数指针
    pData   附加数据的指针（可以为NULL）
    sKHH    客户号
    sPwd    客户密码[密码为明文]。

返回结果:
    返回值大于0,订阅成功，否则失败.
    返回值可用于退订函数(数据退订函数)的输入参数
*/
FIX_API_EXPORT bool Fix_SubscribeByCustomer(HANDLE_SESSION sess, FIX_INT64 &nSubs, const char *sKHH, const char *sPwd, void* pFunc, void *pData);

FIX_API_EXPORT bool Fix_SubscribeByToken(HANDLE_SESSION sess, FIX_INT64& nSubs, const char* sKHH, const char* stoken, void* pFunc, void* pData);
/*
    MDB客户数据订阅函数
    pFunc为回调函数指针,函数定义为: bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, int64 nSubs, void *pData )
    其中conn为连接句柄, nSvc表示服务号，nSubs为Fix_MDBSubscribeByCustomer还回的值，用于指明某个订阅的数据。
        pszKHH, pszPwd,分别为客户号与客户密码[密码为明文]。
    pData就是订阅时的指针(可以为NULL值)
        返回值大于0,订阅成功，否则失败.返回值可用于退订函数(数据退订函数)的输入参数
*/
FIX_API_EXPORT FIX_INT64 Fix_MDBSubscribeByCustomer(HANDLE_CONN conn, const char* sFunc, void* pFunc, void* pData, const char* sKHH, const char* sPwd);
FIX_API_EXPORT FIX_INT64 Fix_SubscribeL2IndexByCode(HANDLE_CONN conn, void* pFunc, void* pData, const char* sJYS, const char* sIndexCode);
FIX_API_EXPORT FIX_INT64 Fix_SubscribeL2QuoteByZQDM(HANDLE_CONN conn, void* pFunc, void* pData, const char* sJYS, const char* sZQDM);

/*
    MDB客户融资融券订阅函数
    pFunc为回调函数指针,函数定义为: bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, int64 nSubs, void *pData )
    其中conn为连接句柄, nSvc表示服务号，nSubs为Fix_MDBSubscribeByCustomer还回的值，用于指明某个订阅的数据。
        pszKHH, pszPwd,分别为客户号与客户密码[密码为明文]。
    pData就是订阅时的指针(可以为NULL值)
        返回值大于0,订阅成功，否则失败.返回值可用于退订函数(数据退订函数)的输入参数
*/
FIX_API_EXPORT FIX_INT64 Fix_MDBSubscribeByCustomer_RZRQ(HANDLE_CONN conn, const char* sFunc, void* pFunc, void* pData, const char* sKHH, const char* sPwd);

//数据退订函数
//nHandle: 订阅句柄；为Fix_MDBSubscribeByCustomer的返回值
FIX_API_EXPORT bool Fix_UnSubscribeByHandle(HANDLE_CONN conn, FIX_INT64 nSubs);
/*
*@brief 获取错误信息,原来长度取引用
*/
FIX_API_EXPORT char* Fix_GetLastErrMsg(char* out, int& outlen);

FIX_API_EXPORT bool Fix_SetSubsPacketId(HANDLE_SESSION sess, FIX_INT64 nPacketId);


FIX_API_EXPORT bool Fix_Publish(HANDLE_SESSION sess);
FIX_API_EXPORT bool Fix_AddRefSession(HANDLE_SESSION sess);
FIX_API_EXPORT bool Fix_ReleaseSession(HANDLE_SESSION sess);

FIX_API_EXPORT bool Fix_SetSessDataString(HANDLE_SESSION hSess, const char* sData);
FIX_API_EXPORT bool Fix_SetWriteString(HANDLE_SESSION hSess, const char* sData);

// 若要正常使用这两个函数，请使用C++方式链接，确保编译环境相同（VS2013,GCC4.8.5）
FIX_UDT_EXPORT std::string Fix_GetStringItem(HANDLE_SESSION sess, int id, int row = -1);
FIX_UDT_EXPORT std::string Fix_GetSessDataString(HANDLE_SESSION hSess);

/*
* @brief 设置该异步会话对应的应答回调函数
* @param 参数说明:
*	sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
*	pFunc  -- [in]
*	pFunc为应答回调函数, 用于异步处理以下原型
*	bool OnSessRely( HANDLE_CONN, HANDLE_SESSION, long )
 */
FIX_API_EXPORT bool Fix_RegReplyCallFunc(HANDLE_SESSION hSess, void* pFunc);

FIX_API_EXPORT bool Fix_SetLevel(HANDLE_SESSION hSess, int nVal);
FIX_API_EXPORT bool Fix_SetDataType(HANDLE_SESSION hSess, int nVal);


/**
* @brief 连接消息回调。
* @param 参数说明: 连接句柄
* @param 参数说明: pFunc 连接句柄（回调函数原型：bool Func(HANDLE hConn, int nEvent【1:掉线，0：连接成功】,void *pData)）
* @return 返回值 true:正常; false:中断
*/
FIX_API_EXPORT bool Fix_SetConnEvent(HANDLE_CONN conn, void *pFunc, void *pData);
FIX_API_EXPORT void Fix_UsePool(bool bVal);
FIX_API_EXPORT void Fix_SetParamEx(const char* sType, const char* sVal);
/*
    MDB客户行情订阅函数(个股期权)
    pFunc为回调函数指针,函数定义为: bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, int64 nSubs, void *pData )
    其中conn为连接句柄, nSvc表示服务号，nSubs为Fix_MDBSubscribeSopQuoteByCustomer还回的值，用于指明某个订阅的数据。
        pszKHH, pszPwd,分别为客户号与客户密码。
    pData就是订阅时的指针(可以为NULL值)
    pszJYS,pszHYDM,分别表示交易所、合约代码（多个代码以分号隔开）
        返回值大于0,订阅成功，否则失败.返回值可用于退订函数(数据退订函数)的输入参数
*/
FIX_INT64 Fix_MDBSubscribeSopQuoteByCustomer(HANDLE_CONN conn, const char* sFunc,
    void* pFunc, void* pData, const char* sKHH, const char* sPwd,
    const char* sJYS, const char* sHYDM);
/*
    MDB客户数据订阅函数(个股期权)
    pFunc为回调函数指针,函数定义为: bool CallBackFunc( HANDLE_CONN conn, HANDLE_SESS sess, int64 nSubs, void *pData )
    其中conn为连接句柄, nSvc表示服务号，nSubs为Fix_MDBSubscribeByCustomer_SOP还回的值，用于指明某个订阅的数据。
        pszKHH, pszPwd,分别为客户号与客户密码。
    pData就是订阅时的指针(可以为NULL值)
        返回值大于0,订阅成功，否则失败.返回值可用于退订函数(数据退订函数)的输入参数
*/
FIX_API_EXPORT FIX_INT64 Fix_MDBSubscribeByCustomer_SOP(HANDLE_CONN conn, const char* sFunc,
    void* pFunc, void* pData, const char* sKHH, const char* sPwd);

//订阅行情中指定证券代码最新行情信息
//pszJYS  订阅交易所范围，不同交易所使用分号分割譬如SH;SZ，空串表示订阅所有交易所
//pszZQDM 订阅证券范围，代码区间使用逗号分割，譬如订阅600100--600300表达成600100,600300，独立代码使用分号分割，譬如订阅600033和601166表达成600033;601166
FIX_API_EXPORT FIX_INT64 Fix_SubscribeQuoteByZQDM(HANDLE_CONN conn, void* pFunc,
    void* pData, const char* sJYS, const char* sZQDM);

//设置会话附加信息
FIX_API_EXPORT bool Fix_SetSessData(HANDLE_SESSION hSess, void* pData);

//读取会话附加信息
FIX_API_EXPORT void* Fix_GetSessData(HANDLE_SESSION hSess);

/*
*@brief 获取字段缓存
*/
FIX_API_EXPORT bool Fix_GetItemBuf(HANDLE_SESSION sess, char* out, int& outlen, int row);

// 订阅回调函数,可以用这个函数读取推送的包序号
FIX_API_EXPORT FIX_INT64 Fix_GetPublishPacketId(HANDLE_SESSION sess, int row = -1);

#ifndef FIX_PUBLISH_TYPE_DEF
#define FIX_PUBLISH_TYPE_DEF
enum  FIX_PUBLISH_TYPE
{
  FIX_PUB_TYPE_COMM = 0,
  FIX_PUB_TYPE_RESUBS_BEFORE = 1, //for Fix_SetPubPacketId/SetSubsCondi
  FIX_PUB_TYPE_RESUBS_AFTER_SUCC = 2,
  FIX_PUB_TYPE_RESUBS_AFTER_FAIL = 3,
  FIX_PUB_TYPE_DOWN = 4
};
#endif
FIX_API_EXPORT FIX_PUBLISH_TYPE Fix_GetPublishType(HANDLE_SESSION sess);

/*
函数名称: Fix_UniEncode

函数说明:
    A5 统一标准加密，加密类型 (数值: 5)
    注意加密后的长度根据输入内容的长短变化，送入足够的缓冲区大小。

输入参数:
    sKHH        -- [in] 客户号
    pData       -- [in] 待加密的明文缓冲区
    nDataLen    -- [in] 输入的明文长度，默认为0，自动计算长度
    io_len      -- [in] 输入：pData缓冲区大小
                        输出：加密后的密文长度，若因缓冲区长度不够加密失败，根据这个调整

返回数据:
    pData       -- 加密成功，返回加密后的密文（可显示字符），直接覆盖原来输入的数据（pData）
                -- 加密失败：传入指针为空或缓冲区长度不够，返回空字符串
*/
FIX_API_EXPORT char* Fix_UniEncode(const char *sKHH, char *pData, int& io_len, int nDataLen = 0);

//设置日志路径，默认在当前路径的log目录下
FIX_API_EXPORT bool Fix_SetLogPath(const char* sPath);

//设置日志级别 默认为0，表示记录连接日志 1，表示连接日志+通信报文日志
FIX_API_EXPORT bool Fix_SetLogLevel(int nLevel);

/**
* @brief  获取指定行数据的列数量
* @param 参数说明:
*    sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
*    nRow   -- [in] 行序号,从零开始
* @return 返回数据:
*    返回列数量。
*/
FIX_API_EXPORT int Fix_GetColumnCount(HANDLE_SESSION sess, int nRow);

/*
* @brief 获取指定行、指定列下的字段id号 以及相应的数据
* @param 参数说明:
*   sess   -- [in] 类型为HANDLE_SESSION的会话句柄。
*	nRow   -- [in] 行序号,从零开始(配合DWACC_GetCount)
*	nIndex -- [in] 列序号,从零开始(DWACC_GetColumnCount)
*	nFid   -- [out] 域id
*	out	-- [out] 域对应的数据(内存上层分配)
*	nSize	-- [in/out] out的大小，函数会输出相应的真实数据大小
* @return 返回数据:
*	true:成功
*	false:失败(比如序号越界或者out内存不足)
*/
FIX_API_EXPORT bool Fix_GetValWithIdByIndex(HANDLE_SESSION sess, int nRow, int nIndex, int& nFid, char* out, int& nSize);


/*券商专用函数*/
FIX_API_EXPORT FIX_INT64 Fix_MDBSubscribe_Sys(HANDLE_CONN conn, const char* sFunc,
    void* pFunc, void* pData, int nNodeId, const char* sKHH, const char* sPwd);

/*
 函数名称: Fix_RegisterSvc
 服务注册函数，用于处理中间件发起的业务请求。
 参数说明:
 conn         -- [in] 类型为HANDLE_CONN的连接句柄。
 func         -- [in] 整型；指定的服务名称。
 pFunc        -- [in] 指定的回调数据指针。
 回调函数的传参格式: bool CallBack( HANDLE_CONN conn, HANDLE_SVC svc, HANDLE_SESSION sess );
 pszCondition -- [in] 指定服务限制条件。比如银行发起的业务，要求指定只是做某个银行的业务。
 或只是处理的指定区域中心的的业务。
 返回数据:
 返回一个相对一个连接句柄下唯一的服务句柄。
 如果返回值为NULL,表示服务注册失败; 成功不为NULL。
 */
FIX_API_EXPORT HANDLE_SVC Fix_RegisterSvc(HANDLE_CONN conn, const char* sFunc, const char* sName,
    void* pFunc, const char* sCondition, void* pData = NULL);
/*
* @brief 创建服务应答数据;用于单行的数据应答。【暂不支持】
* @param 参数说明:
* sess        -- [in] 类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回1表示成功；否则表示失败。
*/
FIX_API_EXPORT bool Fix_CreateAnswer(HANDLE_SESSION sess);
/*
* @brief 创建服务应答数据;用于多行的数据应答。必须和Fix_CreateNextAnswer、Fix_CreateEndMark配合使用。【暂不支持】
* 具体可以参照Sample2的代码。
* @param 参数说明:
* sess        -- [in] 类型为HANDLE_SESSION的会话句柄。
* @return 返回数据:
* 返回1表示成功；否则表示失败。
*/
FIX_API_EXPORT bool Fix_CreateAnswerMul(HANDLE_SESSION sess);

FIX_API_EXPORT bool Fix_SetSystemId(HANDLE_SESSION sess, int nId);
FIX_API_EXPORT bool Fix_SetNodeId(HANDLE_SESSION sess, int nId);
FIX_API_EXPORT bool Fix_SetESB(HANDLE_SESSION sess, bool bESB);
FIX_API_EXPORT bool Fix_SetDebug(HANDLE_SESSION sess, bool bDebug);
FIX_API_EXPORT int Fix_GetSystemId(HANDLE_SESSION sess);
FIX_API_EXPORT int Fix_GetNodeId(HANDLE_SESSION sess);

FIX_API_EXPORT bool Fix_SetDefaultSystemId(int nId);
FIX_API_EXPORT bool Fix_SetDefaultNodeId(int nId);
FIX_API_EXPORT int Fix_GetDefaultSystemId();
FIX_API_EXPORT int Fix_GetDefaultNodeId();

FIX_API_EXPORT FIX_INT64 Fix_GetLastRecvTime(HANDLE_CONN conn);
FIX_API_EXPORT FIX_INT64 Fix_GetLastSendTime(HANDLE_CONN conn);
FIX_API_EXPORT char* Fix_GetConnectIp(HANDLE_CONN conn, char *out, int& io_len);

// for struct data
// req
FIX_API_EXPORT bool Fix_SetStructData(HANDLE_SESSION sess, unsigned char *pData, int nSize);
// ans
FIX_API_EXPORT bool Fix_IsStruct(HANDLE_SESSION sess, int nRow = 0);
FIX_API_EXPORT bool Fix_GetStructData(HANDLE_SESSION sess, unsigned char *pData, int &nSize, int nRow = 0);
//add the interface
//订阅函数
FIX_API_EXPORT bool Fix_Subscribe(HANDLE_SESSION sess, FIX_INT64& nSubs, void *pFunc, void *pData);
//取消订阅
FIX_API_EXPORT bool Fix_Unsubscribe(HANDLE_SESSION sess, FIX_INT64 nSubs);
//设置订阅条件
FIX_API_EXPORT bool Fix_SetSubsCondi(HANDLE_SESSION sess, const char* sCondi);
FIX_API_EXPORT char* Fix_UniEncode3rd(const char *sKHH, char *pData, int& io_len, int nDataLen = 0);
FIX_API_EXPORT bool Fix_GetBody(HANDLE_SESSION sess, int nRow, char*& out_buf, int& out_len);
//注意：该接口目前暂且只能在fix_run 或者 fix_asyncrun 后调用【暂不支持】
FIX_API_EXPORT bool Fix_GetReqBody(HANDLE_SESSION sess, char*& out_buf, int& out_len);
FIX_API_EXPORT bool Fix_SetCustomerForBindThread(HANDLE_CONN conn, const char *pszKHH);

// 设置会话请求附加信息
// SYSINFO_IP = 0 添加IP地址信息
// SYSINFO_MAC = 1 添加MAC地址信息
FIX_API_EXPORT bool Fix_SetSysInfo(HANDLE_SESSION sess, int ntype, int id);

#ifdef _WIN64
FIX_API_EXPORT unsigned char *Fix_GetReqStructData(HANDLE_SESSION sess, int &nSize);
FIX_API_EXPORT void Fix_RegGlobalStructPublishFunc(void *pFunc);
FIX_API_EXPORT unsigned char *Fix_GetStructDataEx(HANDLE_SESSION sess, int &nSize, int nRow = 0);
#else
FIX_UDT_EXPORT unsigned char *Fix_GetReqStructData(HANDLE_SESSION sess, int &nSize);
FIX_UDT_EXPORT void Fix_RegGlobalStructPublishFunc(void *pFunc);
FIX_UDT_EXPORT unsigned char *Fix_GetStructDataEx(HANDLE_SESSION sess, int &nSize, int nRow = 0);
#endif
/*
*@brief:东吴证券三方接入专用,设置登录类型,需要在手机验证时候使用
*@param 参数说明:
*   type -- [in]  类型
* @return 返回数据:
*	1:成功
*	0:失败(预留比如手机号不存在,手机号长度不正确的方式,暂时没用)
*/
//FIX_API_EXPORT int Fix_SetLoginType(/*HANDLE_SESSION sess,*/int type);
/*
*@brief:东吴证券三方接入专用,请求验证码
*@param 参数说明:
*   sess -- [in]  类型为HANDLE_SESSION的会话句柄。
*   type -- [in]  类型
*	 mobileNum   -- [in]手机号
* @return 返回数据:
*	1:成功
*	0:失败(预留比如手机号不存在,手机号长度不正确的方式,暂时没用)
*/
//FIX_API_EXPORT int Fix_ReqVerificationCode(HANDLE_SESSION sess,int type,const char* mobileNum);

/*
*@brief:登录核查
*@param 参数说明:
*   sess -- [in]  类型为HANDLE_SESSION的会话句柄。
*   param1--[in]如果是普通登录就是用户名和密码;如果是手机验证登录就是手机号和验证码
*   param2--[in]如果是普通登录就是用户名和密码;如果是手机验证登录就是手机号和验证码
*	 type    -- [in]登录方式,0:普通验证码登录;1:手机验证登录,2:暂时的其他登录,默认普通登录,
					需要在Fix_Connect或者Fix_ConnectEx之后调用
* @return 返回数据:
* 返回值为True表示服务业务处理成功; false表示失败,这个失败是表示业务通信上的失败；
* 失败后，可以通过DWACC_GetCode取出错误码(必定是一个负数)。通过DWACC_GetErrMsg取出错误的信息。
*/
//FIX_API_EXPORT int Fix_CheckLogin(
//	HANDLE_SESSION sess,
//	const char* param1,
//	const char* param2,
//	int type);

#endif // __APEX_FIXAPI_H__
