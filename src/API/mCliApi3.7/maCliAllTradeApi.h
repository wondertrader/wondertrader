#if !defined(__MA_CLI_ALL_TRADE_API_H__)
#define __MA_CLI_ALL_TRADE_API_H__

#include "maCliTradeApi.h"
#include "maCliStkTradeApi.h"
#include "maCliFislTradeApi.h"
#include "maCliOptTradeApi.h"
#include "maCliFutureTradeApi.h"
#include "maCliCosTradeApi.h"

BGN_NAMESPACE_MACLI

class MATRADEAPI CCliAllTradeSpi : public CCliStkTradeSpi, public CCliFislTradeSpi, public CCliOptTradeSpi, public CCliFutureTradeSpi, public CCliCosTradeSpi
{
public:
  // 默认构造函数
  CCliAllTradeSpi() {}
  // 析构函数
  virtual ~CCliAllTradeSpi() {}

};

class MATRADEAPI CCliAllTradeApi : public CCliStkTradeApi, public CCliFislTradeApi, public CCliOptTradeApi, public CCliFutureTradeApi, public CCliCosTradeApi
{
public:
  // 默认构造函数
  CCliAllTradeApi(void);
  // 析构函数
  virtual ~CCliAllTradeApi(void);
  // 注册连接信息
  virtual int RegisterServer(CReqSysField* p_pstSysField);

public:
  // 异步回调函数
  void OnArCallback(const char *p_pszMsgId, const unsigned char *p_pszDataBuff, int p_iDataLen);
  // 发布回调函数
  void OnPsCallback(const char *p_pszAcceptSn, const unsigned char *p_pszDataBuff, int p_iDataLen);

private:
};

END_NAMESPACE_MACLI
#endif //__MA_CLI_ALL_TRADE_API_H__