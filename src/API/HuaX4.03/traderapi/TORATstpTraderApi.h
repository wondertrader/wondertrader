/////////////////////////////////////////////////////////////////////////
///@company 上海泰琰信息科技有限公司
///@file TORATstpTraderApi.h
///@brief 定义了客户端接口
///@history 
/////////////////////////////////////////////////////////////////////////
#ifndef TORATSTPTRADERAPI_H__
#define TORATSTPTRADERAPI_H__

#include "TORATstpUserApiStruct.h"

#ifdef TRADER_API_EXPORT
#ifdef WINDOWS
#define TRADER_API_DLL_EXPORT __declspec(dllexport)
#else
#define TRADER_API_DLL_EXPORT __attribute__ ((visibility("default")))
#endif
#else
#define TRADER_API_DLL_EXPORT 
#endif

namespace TORASTOCKAPI
{
	class CTORATstpTraderSpi
	{
	public:
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected(){};
		
		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        -3 连接已断开
		///        -4 网络读失败
		///        -5 网络写失败
		///        -6 订阅流错误
		///        -7 流序号错误
		///        -8 错误的心跳报文
		///        -9 错误的报文
		///		  -15 网络读失败
		///		  -16 网络写失败
		virtual void OnFrontDisconnected(int nReason){};
		
		///错误应答
		virtual void OnRspError(CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {};
		
		///获取连接信息应答
		virtual void OnRspGetConnectionInfo(CTORATstpConnectionInfoField *pConnectionInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///登录响应
		virtual void OnRspUserLogin(CTORATstpRspUserLoginField *pRspUserLoginField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///登出响应
		virtual void OnRspUserLogout(CTORATstpUserLogoutField *pUserLogoutField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///修改密码响应
		virtual void OnRspUserPasswordUpdate(CTORATstpUserPasswordUpdateField *pUserPasswordUpdateField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///录入设备序列响应
		virtual void OnRspInputDeviceSerial(CTORATstpRspInputDeviceSerialField *pRspInputDeviceSerialField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///报单录入响应
		virtual void OnRspOrderInsert(CTORATstpInputOrderField *pInputOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///报单回报
		virtual void OnRtnOrder(CTORATstpOrderField *pOrderField) {};
		
		///报单错误回报
		virtual void OnErrRtnOrderInsert(CTORATstpInputOrderField *pInputOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///成交回报
		virtual void OnRtnTrade(CTORATstpTradeField *pTradeField) {};
		
		///撤单响应
		virtual void OnRspOrderAction(CTORATstpInputOrderActionField *pInputOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///撤单错误回报
		virtual void OnErrRtnOrderAction(CTORATstpInputOrderActionField *pInputOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///条件单录入响应
		virtual void OnRspCondOrderInsert(CTORATstpInputCondOrderField *pInputCondOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///条件单回报
		virtual void OnRtnCondOrder(CTORATstpConditionOrderField *pConditionOrderField) {};
		
		///条件单录入错误回报
		virtual void OnErrRtnCondOrderInsert(CTORATstpInputCondOrderField *pInputCondOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///条件单撤单响应
		virtual void OnRspCondOrderAction(CTORATstpInputCondOrderActionField *pInputCondOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///条件单撤单错误回报
		virtual void OnErrRtnCondOrderAction(CTORATstpInputCondOrderActionField *pInputCondOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///协议交易报单录入响应
		virtual void OnRspNegoOrderInsert(CTORATstpInputNegoOrderField *pInputNegoOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///协议交易报单回报
		virtual void OnRtnNegoOrder(CTORATstpNegoOrderField *pNegoOrderField) {};
		
		///协议交易报单错误回报
		virtual void OnErrRtnNegoOrderInsert(CTORATstpInputNegoOrderField *pInputNegoOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///协议交易成交回报
		virtual void OnRtnNegoTrade(CTORATstpNegoTradeField *pNegoTradeField) {};
		
		///协议交易报单操作录入响应
		virtual void OnRspNegoOrderAction(CTORATstpInputNegoOrderActionField *pInputNegoOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///协议交易报单操作错误回报
		virtual void OnErrRtnNegoOrderAction(CTORATstpInputNegoOrderActionField *pInputNegoOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///市场状态回报
		virtual void OnRtnMarketStatus(CTORATstpMarketStatusField *pMarketStatusField) {};
		
		///资金转移响应
		virtual void OnRspTransferFund(CTORATstpInputTransferFundField *pInputTransferFundField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///资金转移错误回报
		virtual void OnErrRtnTransferFund(CTORATstpInputTransferFundField *pInputTransferFundField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///资金转移回报
		virtual void OnRtnTransferFund(CTORATstpTransferFundField *pTransferFundField) {};
		
		///仓位转移响应
		virtual void OnRspTransferPosition(CTORATstpInputTransferPositionField *pInputTransferPositionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///仓位转移错误回报
		virtual void OnErrRtnTransferPosition(CTORATstpInputTransferPositionField *pInputTransferPositionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///仓位转移回报
		virtual void OnRtnTransferPosition(CTORATstpTransferPositionField *pTransferPositionField) {};
		
		///外围系统仓位转移回报
		virtual void OnRtnPeripheryTransferPosition(CTORATstpPeripheryTransferPositionField *pPeripheryTransferPositionField) {};
		
		///外围系统资金转移回报
		virtual void OnRtnPeripheryTransferFund(CTORATstpPeripheryTransferFundField *pPeripheryTransferFundField) {};
		
		///查询集中交易系统资金响应
		virtual void OnRspInquiryJZFund(CTORATstpRspInquiryJZFundField *pRspInquiryJZFundField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///查询银行账户余额响应
		virtual void OnRspInquiryBankAccountFund(CTORATstpRspInquiryBankAccountFundField *pRspInquiryBankAccountFundField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///交易通知回报
		virtual void OnRtnTradingNotice(CTORATstpTradingNoticeField *pTradingNoticeField) {};
		
		///查询最大报单量响应
		virtual void OnRspInquiryMaxOrderVolume(CTORATstpRspInquiryMaxOrderVolumeField *pRspInquiryMaxOrderVolumeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///交易成交集中度查询响应
		virtual void OnRspInquiryTradeConcentration(CTORATstpInquiryTradeConcentrationField *pInquiryTradeConcentrationField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///请求修改开仓成本响应
		virtual void OnRspModifyOpenPosCost(CTORATstpReqModifyOpenPosCostField *pReqModifyOpenPosCostField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///录入节点资金分配信息响应
		virtual void OnRspInputNodeFundAssignment(CTORATstpInputNodeFundAssignmentField *pInputNodeFundAssignmentField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///查询节点资金分配比例响应
		virtual void OnRspInquiryNodeFundAssignment(CTORATstpRspInquiryNodeFundAssignmentField *pRspInquiryNodeFundAssignmentField, CTORATstpRspInfoField *pRspInfoField, int nRequestID) {};
		
		///查询交易所响应
		virtual void OnRspQryExchange(CTORATstpExchangeField *pExchangeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询证券信息响应
		virtual void OnRspQrySecurity(CTORATstpSecurityField *pSecurityField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询新股信息响应
		virtual void OnRspQryIPOInfo(CTORATstpIPOInfoField *pIPOInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询用户响应
		virtual void OnRspQryUser(CTORATstpUserField *pUserField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询投资者响应
		virtual void OnRspQryInvestor(CTORATstpInvestorField *pInvestorField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询股东账户响应
		virtual void OnRspQryShareholderAccount(CTORATstpShareholderAccountField *pShareholderAccountField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询配股配债信息响应
		virtual void OnRspQryRationalInfo(CTORATstpRationalInfoField *pRationalInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询报单响应
		virtual void OnRspQryOrder(CTORATstpOrderField *pOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询撤单请求
		virtual void OnRspQryOrderAction(CTORATstpOrderActionField *pOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询成交响应
		virtual void OnRspQryTrade(CTORATstpTradeField *pTradeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询资金账户响应
		virtual void OnRspQryTradingAccount(CTORATstpTradingAccountField *pTradingAccountField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询投资者持仓响应
		virtual void OnRspQryPosition(CTORATstpPositionField *pPositionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询基础交易费率响应
		virtual void OnRspQryTradingFee(CTORATstpTradingFeeField *pTradingFeeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询佣金费率响应
		virtual void OnRspQryInvestorTradingFee(CTORATstpInvestorTradingFeeField *pInvestorTradingFeeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询新股申购额度响应
		virtual void OnRspQryIPOQuota(CTORATstpIPOQuotaField *pIPOQuotaField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询报单明细资金响应
		virtual void OnRspQryOrderFundDetail(CTORATstpOrderFundDetailField *pOrderFundDetailField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询资金转移流水响应
		virtual void OnRspQryFundTransferDetail(CTORATstpFundTransferDetailField *pFundTransferDetailField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询持仓转移流水响应
		virtual void OnRspQryPositionTransferDetail(CTORATstpPositionTransferDetailField *pPositionTransferDetailField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询外围系统仓位调拨流水响应
		virtual void OnRspQryPeripheryPositionTransferDetail(CTORATstpPeripheryPositionTransferDetailField *pPeripheryPositionTransferDetailField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询外围系统资金调拨流水响应
		virtual void OnRspQryPeripheryFundTransferDetail(CTORATstpPeripheryFundTransferDetailField *pPeripheryFundTransferDetailField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询债券转股信息响应
		virtual void OnRspQryBondConversionInfo(CTORATstpBondConversionInfoField *pBondConversionInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询债券回售信息响应
		virtual void OnRspQryBondPutbackInfo(CTORATstpBondPutbackInfoField *pBondPutbackInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询投资者条件单限制参数响应
		virtual void OnRspQryInvestorCondOrderLimitParam(CTORATstpInvestorCondOrderLimitParamField *pInvestorCondOrderLimitParamField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询条件单响应
		virtual void OnRspQryConditionOrder(CTORATstpConditionOrderField *pConditionOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询条件单撤单响应
		virtual void OnRspQryCondOrderAction(CTORATstpCondOrderActionField *pCondOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询交易通知响应
		virtual void OnRspQryTradingNotice(CTORATstpTradingNoticeField *pTradingNoticeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询新股申购配号结果响应
		virtual void OnRspQryIPONumberResult(CTORATstpIPONumberResultField *pIPONumberResultField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询新股申购中签结果响应
		virtual void OnRspQryIPOMatchNumberResult(CTORATstpIPOMatchNumberResultField *pIPOMatchNumberResultField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询交易协议响应
		virtual void OnRspQryShareholderSpecPrivilege(CTORATstpShareholderSpecPrivilegeField *pShareholderSpecPrivilegeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询市场响应
		virtual void OnRspQryMarket(CTORATstpMarketField *pMarketField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询ETF清单信息响应
		virtual void OnRspQryETFFile(CTORATstpETFFileField *pETFFileField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询ETF成份证券信息响应
		virtual void OnRspQryETFBasket(CTORATstpETFBasketField *pETFBasketField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询投资者限仓信息响应
		virtual void OnRspQryInvestorPositionLimit(CTORATstpInvestorPositionLimitField *pInvestorPositionLimitField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询深港通国际市场互联状态信息响应
		virtual void OnRspQrySZSEImcParams(CTORATstpSZSEImcParamsField *pSZSEImcParamsField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询深港通国际市场互联汇率信息响应
		virtual void OnRspQrySZSEImcExchangeRate(CTORATstpSZSEImcExchangeRateField *pSZSEImcExchangeRateField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询深港通最小价差信息响应
		virtual void OnRspQrySZSEHKPriceTickInfo(CTORATstpSZSEHKPriceTickInfoField *pSZSEHKPriceTickInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询LOF基金信息响应
		virtual void OnRspQryLofFundInfo(CTORATstpLofFundInfoField *pLofFundInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询投资者质押持仓响应
		virtual void OnRspQryPledgePosition(CTORATstpPledgePositionField *pPledgePositionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询证券质押信息响应
		virtual void OnRspQryPledgeInfo(CTORATstpPledgeInfoField *pPledgeInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询系统节点信息响应
		virtual void OnRspQrySystemNodeInfo(CTORATstpSystemNodeInfoField *pSystemNodeInfoField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询标准券额度响应
		virtual void OnRspQryStandardBondPosition(CTORATstpStandardBondPositionField *pStandardBondPositionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询未到期债券质押回购委托响应
		virtual void OnRspQryPrematurityRepoOrder(CTORATstpPrematurityRepoOrderField *pPrematurityRepoOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询协议交易报单响应
		virtual void OnRspQryNegoOrder(CTORATstpNegoOrderField *pNegoOrderField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询协议交易撤单响应
		virtual void OnRspQryNegoOrderAction(CTORATstpNegoOrderActionField *pNegoOrderActionField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询协议交易成交响应
		virtual void OnRspQryNegoTrade(CTORATstpNegoTradeField *pNegoTradeField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
		
		///查询协议交易参数响应
		virtual void OnRspQryNegotiationParam(CTORATstpNegotiationParamField *pNegotiationParamField, CTORATstpRspInfoField *pRspInfoField, int nRequestID, bool bIsLast) {}; 
			
	};
	
	class TRADER_API_DLL_EXPORT CTORATstpTraderApi
	{
	public:
		///创建TraderApi
		///@param pszFlowPath 存贮订阅信息文件的目录，默认为当前目录
		///@param bEncrpyt 网络数据是否加密传输，默认不加密
		///@return 创建出的TraderApi
		static CTORATstpTraderApi *CreateTstpTraderApi(const char *pszFlowPath = "", bool bEncrypt = false);
		
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
		///@remark RegisterNameServer和RegisterFront任选其一。
		virtual void RegisterNameServer(char *pszNsAddress) = 0;
		
		///注册名字服务器用户信息
		///@param pFensUserInfoField：用户信息。
		virtual void RegisterFensUserInfo(CTORATstpFensUserInfoField *pFensUserInfoField) = 0;
	
		///注册回调接口
		///@param pSpi 派生自回调接口类的实例
		virtual void RegisterSpi(CTORATstpTraderSpi *pSpi) = 0;
		
		///订阅私有流
		///@param nResumeType 私有流重传方式  
		///        TORA_TERT_RESTART:从本交易日开始重传
		///        TORA_TERT_RESUME:从上次收到的续传
		///        TORA_TERT_QUICK:只传送登录后私有流的内容
		///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
		virtual void SubscribePrivateTopic(TORA_TE_RESUME_TYPE nResumeType) = 0;
		
		///订阅公共流
		///@param nResumeType 公共流重传方式  
		///        TORA_TERT_RESTART:从本交易日开始重传
		///        TORA_TERT_RESUME:从上次收到的续传
		///        TORA_TERT_QUICK:只传送登录后公共流的内容
		///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
		virtual void SubscribePublicTopic(TORA_TE_RESUME_TYPE nResumeType) = 0;
		
		///获取连接信息
		virtual int ReqGetConnectionInfo(int nRequestID) = 0;
		
		///登录请求
		virtual int ReqUserLogin(CTORATstpReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;
		
		///登出请求
		virtual int ReqUserLogout(CTORATstpUserLogoutField *pUserLogoutField, int nRequestID) = 0;
		
		///修改密码请求
		virtual int ReqUserPasswordUpdate(CTORATstpUserPasswordUpdateField *pUserPasswordUpdateField, int nRequestID) = 0;
		
		///录入设备序列请求
		virtual int ReqInputDeviceSerial(CTORATstpReqInputDeviceSerialField *pReqInputDeviceSerialField, int nRequestID) = 0;
		
		///报单录入请求
		virtual int ReqOrderInsert(CTORATstpInputOrderField *pInputOrderField, int nRequestID) = 0;
		
		///撤单请求
		virtual int ReqOrderAction(CTORATstpInputOrderActionField *pInputOrderActionField, int nRequestID) = 0;
		
		///条件单录入请求
		virtual int ReqCondOrderInsert(CTORATstpInputCondOrderField *pInputCondOrderField, int nRequestID) = 0;
		
		///条件单撤单请求
		virtual int ReqCondOrderAction(CTORATstpInputCondOrderActionField *pInputCondOrderActionField, int nRequestID) = 0;
		
		///协议交易报单录入请求
		virtual int ReqNegoOrderInsert(CTORATstpInputNegoOrderField *pInputNegoOrderField, int nRequestID) = 0;
		
		///协议交易报单操作录入请求
		virtual int ReqNegoOrderAction(CTORATstpInputNegoOrderActionField *pInputNegoOrderActionField, int nRequestID) = 0;
		
		///资金转移请求
		virtual int ReqTransferFund(CTORATstpInputTransferFundField *pInputTransferFundField, int nRequestID) = 0;
		
		///仓位转移请求
		virtual int ReqTransferPosition(CTORATstpInputTransferPositionField *pInputTransferPositionField, int nRequestID) = 0;
		
		///查询集中交易系统资金请求
		virtual int ReqInquiryJZFund(CTORATstpReqInquiryJZFundField *pReqInquiryJZFundField, int nRequestID) = 0;
		
		///查询银行账户余额请求
		virtual int ReqInquiryBankAccountFund(CTORATstpReqInquiryBankAccountFundField *pReqInquiryBankAccountFundField, int nRequestID) = 0;
		
		///查询最大报单量请求
		virtual int ReqInquiryMaxOrderVolume(CTORATstpReqInquiryMaxOrderVolumeField *pReqInquiryMaxOrderVolumeField, int nRequestID) = 0;
		
		///查询交易成交集中度请求
		virtual int ReqInquiryTradeConcentration(CTORATstpInquiryTradeConcentrationField *pInquiryTradeConcentrationField, int nRequestID) = 0;
		
		///请求修改开仓成本请求
		virtual int ReqModifyOpenPosCost(CTORATstpReqModifyOpenPosCostField *pReqModifyOpenPosCostField, int nRequestID) = 0;
		
		///录入节点资金分配信息请求
		virtual int ReqInputNodeFundAssignment(CTORATstpInputNodeFundAssignmentField *pInputNodeFundAssignmentField, int nRequestID) = 0;
		
		///查询节点资金分配比例请求
		virtual int ReqInquiryNodeFundAssignment(CTORATstpReqInquiryNodeFundAssignmentField *pReqInquiryNodeFundAssignmentField, int nRequestID) = 0;
		
		///查询交易所请求
		virtual int ReqQryExchange(CTORATstpQryExchangeField *pQryExchangeField, int nRequestID) = 0;
		
		///查询证券信息请求
		virtual int ReqQrySecurity(CTORATstpQrySecurityField *pQrySecurityField, int nRequestID) = 0;
		
		///查询新股信息请求
		virtual int ReqQryIPOInfo(CTORATstpQryIPOInfoField *pQryIPOInfoField, int nRequestID) = 0;
		
		///查询用户请求
		virtual int ReqQryUser(CTORATstpQryUserField *pQryUserField, int nRequestID) = 0;
		
		///查询投资者请求
		virtual int ReqQryInvestor(CTORATstpQryInvestorField *pQryInvestorField, int nRequestID) = 0;
		
		///查询股东账户请求
		virtual int ReqQryShareholderAccount(CTORATstpQryShareholderAccountField *pQryShareholderAccountField, int nRequestID) = 0;
		
		///查询配股配债信息请求
		virtual int ReqQryRationalInfo(CTORATstpQryRationalInfoField *pQryRationalInfoField, int nRequestID) = 0;
		
		///查询报单请求
		virtual int ReqQryOrder(CTORATstpQryOrderField *pQryOrderField, int nRequestID) = 0;
		
		///查询撤单请求
		virtual int ReqQryOrderAction(CTORATstpQryOrderActionField *pQryOrderActionField, int nRequestID) = 0;
		
		///查询成交请求
		virtual int ReqQryTrade(CTORATstpQryTradeField *pQryTradeField, int nRequestID) = 0;
		
		///查询资金账户请求
		virtual int ReqQryTradingAccount(CTORATstpQryTradingAccountField *pQryTradingAccountField, int nRequestID) = 0;
		
		///查询投资者持仓请求
		virtual int ReqQryPosition(CTORATstpQryPositionField *pQryPositionField, int nRequestID) = 0;
		
		///查询基础交易费率请求
		virtual int ReqQryTradingFee(CTORATstpQryTradingFeeField *pQryTradingFeeField, int nRequestID) = 0;
		
		///查询佣金费率请求
		virtual int ReqQryInvestorTradingFee(CTORATstpQryInvestorTradingFeeField *pQryInvestorTradingFeeField, int nRequestID) = 0;
		
		///查询新股申购额度请求
		virtual int ReqQryIPOQuota(CTORATstpQryIPOQuotaField *pQryIPOQuotaField, int nRequestID) = 0;
		
		///查询报单明细资金请求
		virtual int ReqQryOrderFundDetail(CTORATstpQryOrderFundDetailField *pQryOrderFundDetailField, int nRequestID) = 0;
		
		///查询资金转移流水请求
		virtual int ReqQryFundTransferDetail(CTORATstpQryFundTransferDetailField *pQryFundTransferDetailField, int nRequestID) = 0;
		
		///查询持仓转移流水请求
		virtual int ReqQryPositionTransferDetail(CTORATstpQryPositionTransferDetailField *pQryPositionTransferDetailField, int nRequestID) = 0;
		
		///查询外围系统仓位调拨流水请求
		virtual int ReqQryPeripheryPositionTransferDetail(CTORATstpQryPeripheryPositionTransferDetailField *pQryPeripheryPositionTransferDetailField, int nRequestID) = 0;
		
		///查询外围系统资金调拨流水请求
		virtual int ReqQryPeripheryFundTransferDetail(CTORATstpQryPeripheryFundTransferDetailField *pQryPeripheryFundTransferDetailField, int nRequestID) = 0;
		
		///查询债券转股信息请求
		virtual int ReqQryBondConversionInfo(CTORATstpQryBondConversionInfoField *pQryBondConversionInfoField, int nRequestID) = 0;
		
		///查询债券回售信息请求
		virtual int ReqQryBondPutbackInfo(CTORATstpQryBondPutbackInfoField *pQryBondPutbackInfoField, int nRequestID) = 0;
		
		///查询投资者条件单限制参数请求
		virtual int ReqQryInvestorCondOrderLimitParam(CTORATstpQryInvestorCondOrderLimitParamField *pQryInvestorCondOrderLimitParamField, int nRequestID) = 0;
		
		///查询条件单请求
		virtual int ReqQryConditionOrder(CTORATstpQryConditionOrderField *pQryConditionOrderField, int nRequestID) = 0;
		
		///查询条件单撤单请求
		virtual int ReqQryCondOrderAction(CTORATstpQryCondOrderActionField *pQryCondOrderActionField, int nRequestID) = 0;
		
		///查询交易通知请求
		virtual int ReqQryTradingNotice(CTORATstpQryTradingNoticeField *pQryTradingNoticeField, int nRequestID) = 0;
		
		///查询新股申购配号结果请求
		virtual int ReqQryIPONumberResult(CTORATstpQryIPONumberResultField *pQryIPONumberResultField, int nRequestID) = 0;
		
		///查询新股申购中签结果请求
		virtual int ReqQryIPOMatchNumberResult(CTORATstpQryIPOMatchNumberResultField *pQryIPOMatchNumberResultField, int nRequestID) = 0;
		
		///查询交易协议请求
		virtual int ReqQryShareholderSpecPrivilege(CTORATstpQryShareholderSpecPrivilegeField *pQryShareholderSpecPrivilegeField, int nRequestID) = 0;
		
		///查询市场请求
		virtual int ReqQryMarket(CTORATstpQryMarketField *pQryMarketField, int nRequestID) = 0;
		
		///查询ETF清单信息请求
		virtual int ReqQryETFFile(CTORATstpQryETFFileField *pQryETFFileField, int nRequestID) = 0;
		
		///查询ETF成份证券信息请求
		virtual int ReqQryETFBasket(CTORATstpQryETFBasketField *pQryETFBasketField, int nRequestID) = 0;
		
		///查询投资者限仓信息请求
		virtual int ReqQryInvestorPositionLimit(CTORATstpQryInvestorPositionLimitField *pQryInvestorPositionLimitField, int nRequestID) = 0;
		
		///查询深港通国际市场互联状态信息请求
		virtual int ReqQrySZSEImcParams(CTORATstpQrySZSEImcParamsField *pQrySZSEImcParamsField, int nRequestID) = 0;
		
		///查询深港通国际市场互联汇率信息请求
		virtual int ReqQrySZSEImcExchangeRate(CTORATstpQrySZSEImcExchangeRateField *pQrySZSEImcExchangeRateField, int nRequestID) = 0;
		
		///查询深港通最小价差信息请求
		virtual int ReqQrySZSEHKPriceTickInfo(CTORATstpQrySZSEHKPriceTickInfoField *pQrySZSEHKPriceTickInfoField, int nRequestID) = 0;
		
		///查询LOF基金信息请求
		virtual int ReqQryLofFundInfo(CTORATstpQryLofFundInfoField *pQryLofFundInfoField, int nRequestID) = 0;
		
		///查询投资者质押持仓请求
		virtual int ReqQryPledgePosition(CTORATstpQryPledgePositionField *pQryPledgePositionField, int nRequestID) = 0;
		
		///查询证券质押信息请求
		virtual int ReqQryPledgeInfo(CTORATstpQryPledgeInfoField *pQryPledgeInfoField, int nRequestID) = 0;
		
		///查询系统节点信息请求
		virtual int ReqQrySystemNodeInfo(CTORATstpQrySystemNodeInfoField *pQrySystemNodeInfoField, int nRequestID) = 0;
		
		///查询标准券额度请求
		virtual int ReqQryStandardBondPosition(CTORATstpQryStandardBondPositionField *pQryStandardBondPositionField, int nRequestID) = 0;
		
		///查询未到期债券质押回购委托请求
		virtual int ReqQryPrematurityRepoOrder(CTORATstpQryPrematurityRepoOrderField *pQryPrematurityRepoOrderField, int nRequestID) = 0;
		
		///查询协议交易报单请求
		virtual int ReqQryNegoOrder(CTORATstpQryNegoOrderField *pQryNegoOrderField, int nRequestID) = 0;
		
		///查询协议交易撤单请求
		virtual int ReqQryNegoOrderAction(CTORATstpQryNegoOrderActionField *pQryNegoOrderActionField, int nRequestID) = 0;
		
		///查询协议交易成交请求
		virtual int ReqQryNegoTrade(CTORATstpQryNegoTradeField *pQryNegoTradeField, int nRequestID) = 0;
		
		///查询协议交易参数请求
		virtual int ReqQryNegotiationParam(CTORATstpQryNegotiationParamField *pQryNegotiationParamField, int nRequestID) = 0;
		
		
	protected:
		~CTORATstpTraderApi(){};
	};
}
#endif // TORATSTPTRADERAPI_H__