/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpLev2MdApi.h
///@brief 定义了客户端接口
///@history 
/////////////////////////////////////////////////////////////////////////

#if !defined(_TORA_TSTPLEV2MDAPI_H)
#define _TORA_TSTPLEV2MDAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TORATstpLev2ApiStruct.h"


#ifdef MD_API_EXPORT
#ifdef WINDOWS
#define LEV2MD_API_DLL_EXPORT __declspec(dllexport)
#else
#define LEV2MD_API_DLL_EXPORT __attribute__ ((visibility("default")))
#endif
#else
#define LEV2MD_API_DLL_EXPORT 
#endif

namespace TORALEV2API
{
	class CTORATstpLev2MdSpi
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

		///错误应答
		virtual void OnRspError(CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///登录请求响应
		virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLogin, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///登出请求响应
		virtual void OnRspUserLogout(CTORATstpUserLogoutField *pUserLogout, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅行情应答(上海非债券类、深圳非债券类、深圳可转债)
		virtual void OnRspSubMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅行情应答(上海非债券类、深圳非债券类、深圳可转债)
		virtual void OnRspUnSubMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅指数行情应答
		virtual void OnRspSubIndex(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅指数行情应答
		virtual void OnRspUnSubIndex(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅逐笔成交应答(深圳非债券类、深圳可转债)
		virtual void OnRspSubTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅逐笔成交应答(深圳非债券类、深圳可转债)
		virtual void OnRspUnSubTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅逐笔委托应答(深圳非债券类、深圳可转债)
		virtual void OnRspSubOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅逐笔委托应答(深圳非债券类、深圳可转债)
		virtual void OnRspUnSubOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅盘后定价行情应答
		virtual void OnRspSubPHMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅盘后定价行情应答
		virtual void OnRspUnSubPHMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅盘后定价逐笔成交应答
		virtual void OnRspSubPHTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消盘后定价订阅逐笔成交应答
		virtual void OnRspUnSubPHTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅逐笔重传成交应答
		virtual void OnRspSubResendTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅逐笔重传成交应答
		virtual void OnRspUnSubResendTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅逐笔重传委托应答
		virtual void OnRspSubResendOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅逐笔重传委托应答
		virtual void OnRspUnSubResendOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅上海XTS债券行情应答
		virtual void OnRspSubXTSMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅上海XTS债券行情应答
		virtual void OnRspUnSubXTSMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅上海XTS债券逐笔数据应答
		virtual void OnRspSubXTSTick(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅上海XTS债券逐笔数据应答
		virtual void OnRspUnSubXTSTick(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅上海NGTS非债券逐笔数据应答
		virtual void OnRspSubNGTSTick(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅上海NGTS非债券逐笔数据应答
		virtual void OnRspUnSubNGTSTick(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅深圳债券（不包括可转债)行情应答
		virtual void OnRspSubBondMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅深圳债券（不包括可转债)行情应答
		virtual void OnRspUnSubBondMarketData(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅深圳债券（不包括可转债)逐笔成交应答
		virtual void OnRspSubBondTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅深圳债券（不包括可转债)逐笔成交应答
		virtual void OnRspUnSubBondTransaction(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///订阅深圳债券（不包括可转债)逐笔委托应答
		virtual void OnRspSubBondOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///取消订阅深圳债券（不包括可转债)逐笔委托应答
		virtual void OnRspUnSubBondOrderDetail(CTORATstpSpecificSecurityField *pSpecificSecurity, CTORATstpRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

		///行情通知(上海非债券类、深圳非债券类、深圳可转债)
		virtual void OnRtnMarketData(CTORATstpLev2MarketDataField *pMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]) {};

		//指数行情通知
		virtual void OnRtnIndex(CTORATstpLev2IndexField *pIndex) {};

		//逐笔成交通知(深圳非债券类、深圳可转债)
		virtual void OnRtnTransaction(CTORATstpLev2TransactionField *pTransaction) {};

		//逐笔委托通知(深圳非债券类、深圳可转债)
		virtual void OnRtnOrderDetail(CTORATstpLev2OrderDetailField *pOrderDetail) {};

		///盘后定价行情通知
		virtual void OnRtnPHMarketData(CTORATstpLev2PHMarketDataField *pPHMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]) {};

		//盘后定价逐笔成交通知
		virtual void OnRtnPHTransaction(CTORATstpLev2PHTransactionField *pTransaction) {};

		//逐笔重传成交通知
		virtual void OnRtnResendTransaction(CTORATstpLev2ResendTransactionField *pTransaction) {};

		//逐笔重传委托通知
		virtual void OnRtnResendOrderDetail(CTORATstpLev2ResendOrderDetailField *pOrderDetail) {};

		///上海XTS债券行情通知
		virtual void OnRtnXTSMarketData(CTORATstpLev2XTSMarketDataField *pMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]) {};

		///上海XTS债券逐笔数据通知
		virtual void OnRtnXTSTick(CTORATstpLev2XTSTickField *pTick) {};

		///上海NGTS非债券逐笔数据通知
		virtual void OnRtnNGTSTick(CTORATstpLev2NGTSTickField *pTick) {};

		///深圳债券（不包括可转债)行情通知
		virtual void OnRtnBondMarketData(CTORATstpLev2BondMarketDataField *pMarketData, const int FirstLevelBuyNum, const int FirstLevelBuyOrderVolumes[], const int FirstLevelSellNum, const int FirstLevelSellOrderVolumes[]) {};

		//深圳债券（不包括可转债)逐笔成交通知
		virtual void OnRtnBondTransaction(CTORATstpLev2BondTransactionField *pTransaction) {};

		//深圳债券（不包括可转债)逐笔委托通知
		virtual void OnRtnBondOrderDetail(CTORATstpLev2BondOrderDetailField *pOrderDetail) {};
	};

	class LEV2MD_API_DLL_EXPORT CTORATstpLev2MdApi
	{
	public:
		///创建MdApi
		///@return 创建出的MdApi
		static CTORATstpLev2MdApi *CreateTstpLev2MdApi(const TTORATstpMDSubModeType &eMDSubMode = TORA_TSTP_MST_TCP, bool bCachedMode = false);

		///获取API版本号
		///@return 版本号
		static const char* GetApiVersion();

		///删除接口对象本身
		///@remark 不再使用本接口对象时,调用该函数删除接口对象
		virtual void Release() = 0;

		///初始化
		///@param cpuCores：API内部线程绑核参数，默认不绑核运行
		//                  "0"表示API内部线程绑定到第0核上运行
		//					"0,5,18"表示API内部线程绑定到第0,第5，第18号核上运行                 
		///@remark 初始化运行环境,只有调用后,接口才开始工作
		virtual void Init(const char * cpuCores="") = 0;

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
		///@remark RegisterFront/RegisterMulticast与RegisterNameServer二选一
		virtual void RegisterNameServer(char *pszNsAddress) = 0;

		///注册组播地址
		///@param pszMulticastAddress:组播网络地址,如:"udp://224.3.9.110:34567"。
		///@param pszInterfaceIP:接收网卡地址,如:"127.0.0.1",填NULL则依次轮询尝试本机所有网卡加入组播组。
		///@param pszSourceIp:组播数据包源地址,如:"127.0.0.1",填NULL表示不校验数据包源。
		///@param pInterfaceName:接收网卡名,如enp101s0f1,efvi接收时有意义
		///@param rxqCapacity:接收队列大小,rxqCapacity<=0 则取默认值，（512，1024，2048，4096),efvi接收时有意义
		///@param bEFVI:EFVI接收模式开关(linux)
		///@remark 在使用UDP组播方式工作时调用
		virtual void RegisterMulticast(char *pszMulticastAddress, char *pszInterfaceIP, char *pszSourceIp, const char * pInterfaceName="", int rxqCapacity=0,bool bEFVI=false) = 0;


		///注册回调接口
		///@param pSpi 派生自回调接口类的实例
		virtual void RegisterSpi(CTORATstpLev2MdSpi *pSpi) = 0;

		///在init函数调用之前调用，用于提前声明后续需要订阅的行情类别集合。在某些情况下可提高行情过滤效率
		///@param mkSubTypes: 行情类别集合，每一个字符都是TTORATstpMKSubType类型。如"TO"表示后续会
		///调用SubscribeTransaction、SubscribeOrderDetail订阅具体代码级别的逐笔成交和逐笔委托。
		///如果程序调用此函数申明了需要订阅的行情类别，后续也需要调用对应的Subcriber***函数进行行情订阅
		virtual void DeclareMKSubTypes(const TTORATstpMKSubTypesType & mkSubTypes) =0;

		///订阅行情。（上海非债券类、深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订行情。（上海非债券类、深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅指数行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeIndex(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订指数行情。
		///@param ppInstrumentID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeIndex(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅逐笔成交。（深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订逐笔成交。（深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅逐笔委托。（深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订逐笔委托。（深圳非债券类、深圳可转债）
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅盘后定价行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribePHMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订盘后定价行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribePHMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅盘后定价逐笔成交。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribePHTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订盘后定价逐笔成交。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribePHTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅逐笔重传成交。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeResendTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订逐笔重传成交。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeResendTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅逐笔重传委托。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeResendOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订逐笔重传委托。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeResendOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅上海XTS债券行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeXTSMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订上海XTS债券行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeXTSMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅上海XTS债券逐笔行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeXTSTick(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订上海XTS债券逐笔行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeXTSTick(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅上海NGTS上海非债券类逐笔行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeNGTSTick(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订上海NGTS上海非债券类、逐笔行情。
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeNGTSTick(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅深圳债券(不包含可转债)快照行情
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeBondMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订深圳债券(不包含可转债)快照行情
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeBondMarketData(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅深圳债券(不包含可转债)逐笔成交
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeBondTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订深圳债券(不包含可转债)逐笔成交
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeBondTransaction(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///订阅深圳债券(不包含可转债)逐笔委托
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int SubscribeBondOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///退订深圳债券(不包含可转债)逐笔委托
		///@param ppSecurityID 合约ID  
		///@param nCount 要订阅/退订行情的合约个数
		///@remark 
		virtual int UnSubscribeBondOrderDetail(char *ppSecurityID[], int nCount, TTORATstpExchangeIDType ExchageID) = 0;

		///用户登录请求
		virtual int ReqUserLogin(CTORATstpReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

		///登出请求
		virtual int ReqUserLogout(CTORATstpUserLogoutField *pUserLogout, int nRequestID) = 0;
	protected:
		~CTORATstpLev2MdApi(){};
	};
}
#endif
