/*!
 * \file IParserApi.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 行情解析模块接口定义文件
 */
#pragma once
#include <string>
#include <stdint.h>
#include "WTSTypes.h"
#include "FasterDefs.h"

typedef faster_hashset<std::string> CodeSet;

NS_OTP_BEGIN
class WTSTickData;
class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;
class WTSParams;
class WTSArray;
class IBaseDataMgr;

/*
 *	行情解析模块回调接口
 */
class IParserSpi
{
public:
	/*
	 *	处理模块事件
	 *	@e	事件类型,如连接、断开、登录、登出
	 *	@ec	错误码,0为没有错误
	 */
	virtual void handleEvent(WTSParserEvent e, int32_t ec){}

	/*
	 *	处理合约列表
	 *	@aySymbols	合约列表,基础元素为WTSContractInfo,WTSArray的用法请参考定义
	 */
	virtual void handleSymbolList(const WTSArray* aySymbols)		= 0;

	/*
	 *	处理实时行情
	 *	@quote		实时行情
	 *	@bNeedSlice	是否需要切片,如果是从外部接入的快照行情数据,则需要切片,如果是内部广播的就不需要切片
	 */
	virtual void handleQuote(WTSTickData *quote, bool bNeedSlice)	= 0;

	/*
	 *	处理委托队列数据（股票level2）
	 *	@ordQueData	委托对垒数据
	 */
	virtual void handleOrderQueue(WTSOrdQueData* ordQueData){}

	/*
	 *	处理逐笔委托数据（股票level2）
	 *	@ordDetailData	逐笔委托数据
	 */
	virtual void handleOrderDetail(WTSOrdDtlData* ordDetailData){}

	/*
	 *	处理逐笔成交数据
	 *	@transData	逐笔成交数据
	 */
	virtual void handleTransaction(WTSTransData* transData){}

	/*
	 *	处理解析模块的日志
	 *	@ll			日志级别
	 *	@message	日志内容
	 */
	virtual void handleParserLog(WTSLogLevel ll, const char* format, ...)	= 0;

public:
	virtual IBaseDataMgr*	getBaseDataMgr()	= 0;
};

/*
 *	行情解析模块接口
 */
class IParserApi
{
public:
	virtual ~IParserApi(){}

public:
	/*
	 *	初始化解析模块
	 *	@config	模块配置
	 *	返回值	是否初始化成功
	 */
	virtual bool init(WTSParams* config)			= 0;

	/*
	 *	释放解析模块
	 *	用于退出时
	 */
	virtual void release()							= 0;

	/*
	 *	开始连接服务器
	 *	@返回值	连接命令是否发送成功
	 */
	virtual bool connect()							= 0;

	/*
	 *	断开连接
	 *	@返回值	命令是否发送成功
	 */
	virtual bool disconnect()						= 0;

	/*
	 *	是否已连接
	 *	@返回值	是否已连接
	 */
	virtual bool isConnected()						= 0;

	/*
	 *	订阅合约列表
	 */
	virtual void subscribe(const CodeSet& setCodes)		= 0;

	/*
	 *	退订合约列表
	 */
	virtual void unsubscribe(const CodeSet& setCodes)	= 0;

	/*
	 *	注册回调接口
	 */
	virtual void registerSpi(IParserSpi* listener)		= 0;
};

NS_OTP_END

//获取IDataMgr的函数指针类型
typedef otp::IParserApi* (*FuncCreateParser)();
typedef void(*FuncDeleteParser)(otp::IParserApi* &parser);