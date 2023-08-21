//
//  baseDefine.h
//  zsApi
//
//  Created by 贺哲 on 2018/4/24.
//  Copyright ? 2018年 中山证券有限责任公司. All rights reserved.
//
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2018-04-24    1.0           贺哲            新建
//----------------------------------------------------------------------------

#if !defined(__BASE_DEFINE_H__)
#define __BASE_DEFINE_H__

#if defined (_MSC_VER) && (_MSC_VER == 1200)
#define FORMAT_LONGLONG "%I64d"
#if defined(WIN32)
typedef __int64 LONGLONG;
#endif
#else
#define FORMAT_LONGLONG "%lld"
typedef long long LONGLONG;
#endif

#if defined(WIN32) || defined(WIN64)
  #if defined(ZSAPI_EXPORTS)
    #define ZSAPI __declspec(dllexport)
  #else
    #define ZSAPI __declspec(dllimport)
  #endif
  #define ZSAPI_STDCALL __stdcall
#else
  #define ZSAPI
  #define ZSAPI_STDCALL
  #if !defined __int64
    #define __int64 long long
  #endif
#endif


//////////////////////////////////////////////////////////////////////////
// 登录状态定义
#define LOGIN_OFFLINE                       '0'       //未登录
#define LOGIN_ONLINE                        '1'       //已登录


#define ADRRESS_MAX                   128
#define USER_MAX                      16
#define ERR_MSG_SIZE                  256


#pragma pack(4)

// 请求固定参数
struct STReqFix
{
    char          szUserName[USER_MAX];       // 用户名
    LONGLONG      llAcctCode;                 // 资产账户
    int           iFunction;                  // 功能代码
    char          chOpRole;                   // 用户角色
    char          szSession[128];             // 会话凭证
};

// 第1结果集内容
struct STFirstSet
{
    int           iCode;                      //错误码
    char          szText[ERR_MSG_SIZE];       //错误文本
};

#pragma pack()

#endif  //__BASE_DEFINE_H__