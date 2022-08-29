/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpXMdApi.h
///@brief 定义了客户端接口
///@history 
/////////////////////////////////////////////////////////////////////////

#ifndef TORATSTPXMDAPI_H__
#define TORATSTPXMDAPI_H__

#include "TORATstpXMdApiStruct.h"


#ifdef XMD_API_EXPORT
	#ifdef WINDOWS
		#define XMD_API_DLL_EXPORT __declspec(dllexport)
	#else
		#define XMD_API_DLL_EXPORT __attribute__ ((visibility("default")))
	#endif
#else
	#define XMD_API_DLL_EXPORT
#endif


namespace TORALEV1API
{
	class CTORATstpXMdSpi
	{
	public:
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected(){};

		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///        -3 连接已断开
		///        -4 网络读失败
		///        -5 网络写失败
		///        -6 订阅流错误
		///        -7 流序号错误
		///        -8 错误的心跳报文
		///        -9 错误的报文
		virtual void OnFrontDisconnected(int nReason){};

		///获取连接信息应答
		virtual void OnRspGetConnectionInfo(CTORATstpConnectionInfoField *pConnectionInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};

		///登录应答
		virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLoginField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};

		///登出应答
		virtual void OnRspUserLogout(CTORATstpUserLogoutField *pUserLogoutField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};

		///订阅行情应答
		virtual void OnRspSubMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订行情应答
		virtual void OnRspUnSubMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅盘后行情应答
		virtual void OnRspSubPHMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订盘后行情应答
		virtual void OnRspUnSubPHMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅特定行情应答
		virtual void OnRspSubSpecialMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订特定行情应答
		virtual void OnRspUnSubSpecialMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅简易行情应答
		virtual void OnRspSubSimplifyMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订简易行情应答
		virtual void OnRspUnSubSimplifyMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅合约状态应答
		virtual void OnRspSubSecurityStatus(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订合约状态应答
		virtual void OnRspUnSubSecurityStatus(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅市场状态应答
		virtual void OnRspSubMarketStatus(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订市场状态应答
		virtual void OnRspUnSubMarketStatus(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅国际市场互联状态应答
		virtual void OnRspSubImcParams(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订国际市场互联状态应答
		virtual void OnRspUnSubImcParams(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///查询行情快照应答
		virtual void OnRspInquiryMarketDataMirror(CTORATstpMarketDataField *pMarketDataField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {};

		///查询盘后行情快照应答
		virtual void OnRspInquiryPHMarketDataMirror(CTORATstpPHMarketDataField *pPHMarketDataField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {};

		///查询特定行情快照应答
		virtual void OnRspInquirySpecialMarketDataMirror(CTORATstpSpecialMarketDataField *pMarketDataField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {};

		///订阅期权行情应答
		virtual void OnRspSubSPMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订期权行情应答
		virtual void OnRspUnSubSPMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅期权简易行情应答
		virtual void OnRspSubSPSimplifyMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订期权简易行情应答
		virtual void OnRspUnSubSPSimplifyMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅期权合约状态应答
		virtual void OnRspSubSPSecurityStatus(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订期权合约状态应答
		virtual void OnRspUnSubSPSecurityStatus(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///订阅期权市场状态应答
		virtual void OnRspSubSPMarketStatus(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订期权市场状态应答
		virtual void OnRspUnSubSPMarketStatus(CTORATstpSpecificMarketField *pSpecificMarketField, CTORATstpRspInfoField *pRspInfoField) {};

		///查询期权行情快照应答
		virtual void OnRspInquirySPMarketDataMirror(CTORATstpMarketDataField *pMarketDataField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {};

		///行情通知
		virtual void OnRtnMarketData(CTORATstpMarketDataField *pMarketDataField) {};

		///盘后行情通知
		virtual void OnRtnPHMarketData(CTORATstpPHMarketDataField *pPHMarketDataField) {};

		///特定行情通知
		virtual void OnRtnSpecialMarketData(CTORATstpSpecialMarketDataField *pSpecialMarketDataField) {};

		///简易行情通知
		virtual void OnRtnSimplifyMarketData(CTORATstpSimplifyMarketDataField *pSimplifyMarketDataField) {};

		///合约状态
		virtual void OnRtnSecurityStatus(CTORATstpSecurityStatusField *pSecurityStatusField) {};

		///市场状态
		virtual void OnRtnMarketStatus(CTORATstpMarketStatusField *pMarketStatusField) {};

		///国际市场互联状态
		virtual void OnRtnImcParams(CTORATstpImcParamsField *pImcParamsField) {};

		///期权行情通知
		virtual void OnRtnSPMarketData(CTORATstpMarketDataField *pMarketDataField) {};

		///期权简易行情通知
		virtual void OnRtnSPSimplifyMarketData(CTORATstpSimplifyMarketDataField *pSimplifyMarketDataField) {};

		///期权合约状态通知
		virtual void OnRtnSPSecurityStatus(CTORATstpSecurityStatusField *pSecurityStatusField) {};

		///期权市场状态通知
		virtual void OnRtnSPMarketStatus(CTORATstpMarketStatusField *pMarketStatusField) {};


		///订阅合成快照应答(衍生服务)
		virtual void OnRspSubRapidMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订合成快照应答(衍生服务)
		virtual void OnRspUnSubRapidMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///合成快照通知(衍生服务)
		virtual void OnRtnRapidMarketData(CTORATstpRapidMarketDataField *pRapidMarketDataField) {};

		///订阅资金流向行情应答(衍生服务)
		virtual void OnRspSubFundsFlowMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///退订资金流向行情应答(衍生服务)
		virtual void OnRspUnSubFundsFlowMarketData(CTORATstpSpecificSecurityField *pSpecificSecurityField, CTORATstpRspInfoField *pRspInfoField) {};

		///资金流向行情通知(衍生服务)
		virtual void OnRtnFundsFlowMarketData(CTORATstpFundsFlowMarketDataField *pFundsFlowMarketDataField) {};
	};

	class XMD_API_DLL_EXPORT CTORATstpXMdApi
	{
	public:
		///创建XMdApi
		///@return 创建出的XMdApi
		///@param eMDSubMode:行情订阅模式，目前支持TCP和UDP组播两种方式
		///@param eDeriveMDSubMode:衍生服务合成快照行情订阅模式，目前支持TCP和UDP组播两种方式
		static CTORATstpXMdApi *CreateTstpXMdApi(const TTORATstpMDSubModeType &eMDSubMode = TORA_TSTP_MST_TCP, const TTORATstpMDSubModeType &eDeriveMDSubMode = TORA_TSTP_MST_TCP);

		///获取API版本号
		///@return 版本号
		static const char* GetApiVersion();

		///删除接口对象本身
		///@remark 不再使用本接口对象时,调用该函数删除接口对象
		virtual void Release() = 0;

		///初始化
		///@remark 初始化运行环境,只有调用后,接口才开始工作
		virtual void Init() = 0;

		///等待接口线程结束运行
		///@return 线程退出代码
		virtual int Join() = 0;

		///注册前置机网络地址
		///@param pszFrontAddress：前置机网络地址。
		///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
		///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
		virtual void RegisterFront(char *pszFrontAddress) = 0;

		///注册名字服务器网络地址
		///@param pszNsAddress：名字服务器网络地址。
		///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:12001”。 
		///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”12001”代表服务器端口号。
		///@remark RegisterNameServer优先于RegisterFront
		virtual void RegisterNameServer(char *pszNsAddress) = 0;

		///注册名字服务器用户信息
		///@param pFensUserInfoField：用户信息。
		virtual void RegisterFensUserInfo(CTORATstpFensUserInfoField *pFensUserInfoField) = 0;
	
		///注册组播地址
		///@param pszMulticastAddress:组播网络地址,如:"udp://224.3.9.110:34567"。
		///@param pszInterfaceIP:接收网卡地址,如:"127.0.0.1",填NULL则依次轮询尝试本机所有网卡加入组播组。
		///@param pszSourceIp:组播数据包源地址,如:"127.0.0.1",填NULL表示不校验数据包源。
		///@remark 仅在使用UDP组播方式工作时有效
		virtual void RegisterMulticast(char *pszMulticastAddress, char *pszInterfaceIP, char *pszSourceIp) = 0;

		///注册衍生服务服务器网络地址
		///@param pszNsAddress：衍生服务服务器网络地址。
		///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:15001”。 
		///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”15001”代表服务器端口号。
		virtual void RegisterDeriveServer(char *pszDeriveAddress) = 0;

		///注册衍生服务服务器组播地址
		///@param pszMulticastAddress:组播网络地址,如:"udp://224.3.9.110:34567"。
		///@param pszInterfaceIP:接收网卡地址,如:"127.0.0.1",填NULL则依次轮询尝试本机所有网卡加入组播组。
		///@param pszSourceIp:组播数据包源地址,如:"127.0.0.1",填NULL表示不校验数据包源。
		///@remark 仅在使用UDP组播方式工作时有效
		virtual void RegisterDeriveMulticast(char * pszMulticastAddress, char * pszInterfaceIP, char * pszSourceIp) = 0;

		///注册回调接口
		virtual void RegisterSpi(CTORATstpXMdSpi *pSpi) = 0;

		///获取连接信息
		virtual int ReqGetConnectionInfo(int nRequestID) = 0;

		///用户登录请求
		virtual int ReqUserLogin(CTORATstpReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

		///登出请求
		virtual int ReqUserLogout(CTORATstpUserLogoutField *pUserLogoutField, int nRequestID) = 0;

		///订阅行情
		virtual int SubscribeMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订行情
		virtual int UnSubscribeMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅盘后行情
		virtual int SubscribePHMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订盘后行情
		virtual int UnSubscribePHMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅特定行情
		virtual int SubscribeSpecialMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订特定行情
		virtual int UnSubscribeSpecialMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅简易行情
		virtual int SubscribeSimplifyMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订简易行情
		virtual int UnSubscribeSimplifyMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅合约状态
		virtual int SubscribeSecurityStatus(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订合约状态
		virtual int UnSubscribeSecurityStatus(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅市场状态
		virtual int SubscribeMarketStatus(TTORATstpMarketIDType MarketID) = 0;

		///退订市场状态
		virtual int UnSubscribeMarketStatus(TTORATstpMarketIDType MarketID) = 0;

		///订阅国际市场互联状态
		virtual int SubscribeImcParams(TTORATstpMarketIDType MarketID) = 0;

		///退订国际市场互联状态
		virtual int UnSubscribeImcParams(TTORATstpMarketIDType MarketID) = 0;

		///查询行情快照
		virtual int ReqInquiryMarketDataMirror(CTORATstpInquiryMarketDataField *pInquiryMarketDataField, int nRequestID) = 0;

		///查询盘后行情快照
		virtual int ReqInquiryPHMarketDataMirror(CTORATstpInquiryMarketDataField *pInquiryMarketDataField, int nRequestID) = 0;

		///查询特定行情快照
		virtual int ReqInquirySpecialMarketDataMirror(CTORATstpInquirySpecialMarketDataField *pInquirySpecialMarketDataField, int nRequestID) = 0;

		///订阅期权行情
		virtual int SubscribeSPMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订期权行情
		virtual int UnSubscribeSPMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅期权简易行情
		virtual int SubscribeSPSimplifyMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订期权简易行情
		virtual int UnSubscribeSPSimplifyMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅期权合约状态
		virtual int SubscribeSPSecurityStatus(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订期权合约状态
		virtual int UnSubscribeSPSecurityStatus(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅期权市场状态
		virtual int SubscribeSPMarketStatus(TTORATstpMarketIDType MarketID) = 0;

		///退订期权市场状态
		virtual int UnSubscribeSPMarketStatus(TTORATstpMarketIDType MarketID) = 0;

		///查询期权行情快照
		virtual int ReqInquirySPMarketDataMirror(CTORATstpInquiryMarketDataField *pInquiryMarketDataField, int nRequestID) = 0;


		///订阅合成快照(衍生服务)
		virtual int SubscribeRapidMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订合成快照(衍生服务)
		virtual int UnSubscribeRapidMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///订阅资金流向行情(衍生服务)
		virtual int SubscribeFundsFlowMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

		///退订资金流向行情(衍生服务)
		virtual int UnSubscribeFundsFlowMarketData(char **ppSecurityID, int nCount, TTORATstpExchangeIDType ExchangeID) = 0;

	protected:
		~CTORATstpXMdApi(){};
	};
}

#endif // TORATSTPXMDAPI_H__