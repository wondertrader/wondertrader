#if !defined(__BASE_API_H__)
#define __BASE_API_H__

#include "baseDefine.h"

class ZSAPI CBaseCallback
{
public:
    virtual int OnConnected(void) = 0;
    virtual int OnDisconnected(int p_nReason, const char *p_pszErrInfo) = 0;
};

class ZSAPI CBaseApi
{
public:
    CBaseApi(void);
    virtual ~CBaseApi(void);

    // 初始化
    virtual int Init(int iNoSub = 0);

    // 退出
    virtual int Exit(void);

    // 设置服务器地址和端口
    virtual int RegisterServer(const char *p_pszIp, int p_iPort, unsigned int uiTimeout = 0);

    ///注册回调接口
    virtual int RegisterCallback(CBaseCallback *p_pCallback);

    //设置鉴权方式
    virtual void SetAuthType(int iAuthType, const char *szSecretKey = 0);

public:
    virtual void OnReqCallback(const int p_iMsgId, const void *p_pszDataBuff, int p_iDataLen, LONGLONG p_llReqId) = 0;
    virtual void OnSubCallback(const char *p_pszMsg, const void *p_pszDataBuff, int p_iDataLen) = 0;
    const char* GetLastError(void);

protected:
    char                      m_szServerIP[ADRRESS_MAX];
    int                       m_nPort;
    unsigned int              m_uiTimeout;
    STReqFix                  m_stReqFix;
    CBaseCallback            *m_pCallback;
    char                      m_szLastError[1024+1];
};





#endif  //__BASE_API_H__