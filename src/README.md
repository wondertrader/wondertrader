# wt > src
这是wondertrader的C++底层源码

## 开发环境
+ Windows	
	> `Visual Studio 2019` + `Windows 7`
+ Linux	
	> `Gcc v7.5.0` + `Ubuntu 18.04.3 LTS` + `cmake 3.10.2`

## 依赖库
+ boost 1.72
+ rapidjson
+ spdlog

## 解决方案结构
+ ***Backtest***
	backtest包含了回测相关的项目
	- WtBtCore		回测框架核心代码
	- WtBtPorter	回测框架C接口导出模块
	- WtBtRunner	回测框架纯C++环境运行入口程序
	- TestBtPorter	C接口导出模块（*WtBtPorer*）的测试程序，方便在C++环境调试
+ ***BaseLibs***
	BaseLibs包含了整个解决方案的基础库
	- Share			包含了整个框架的基础数据结构、基础对象以及所有接口的定义，也包含了一些公共方法的封装
	- WTSUtilsLib	包含了整个框架用到的第三方组件，如*pugixml*、*zstdlib*、*base64*、*md5*等
	- WTSToolsLib	框架的通用工具库，很多基础工具都在该项目里定义，如日志模块*WTSLogger*、基础数据模模块*WTSBaseDataMgr*等
+ ***DataKit***
	数据组件包含了数据接入以及落地的项目代码
	- WtDtCore		数据组件核心库，包含了整个数据接入落地的核心逻辑
	- WtDtPorter	数据组件C接口导出库，主要用于跨语言调用
	- QuoteFactory	这是一个C++可执行程序，作为C++环境的数据落地程序的入口
+ ***Parsers***
	Parsers包含了所有的行情解析器模块代码
	- ParserCTP		对接CTP行情通道的行情解析器
	- ParserXTP		对接XTP行情通道的行情解析器
	- ParserUT		对接兴业证券提供的股票L2行情通道的行情解析器
	- ParserUDP		对接数据组件*UDP*广播的行情数据通道的解析器
+ ***Plugins***
	Plugins包含了交易框架外部插件的项目代码
	- WtExeFact		内置的执行器工厂，提供了一个简单的对价执行单元和一个简单的*alpha*拆单执行单元
	- WtRiskMonFact	内置的风控单元工厂，提供了一个简单的组合盘资金风控的风控单元
+ ***Porter***
	Porter包含了实盘的核心项目，是整个解决方案的核心
	- WtCore		实盘交易核心库，包含了整个交易框架的核心逻辑
	- WtPorter		交易框架C接口导出库，主要用于跨语言调用
	- WtRunner		同*WtBtRunner*，实盘框架纯C++环境运行的入口
	- TestPorter	实盘C接口导出模块（*WtPorter*)的测试程序，方便在C++调试
+ ***Traders***
	Traders包含了所有的交易通道的模块代码
	- TraderCTP		CTP柜台交易通道对接模块
	- TraderCTPMini	CTPMini柜台交易通道对接模块
	- TraderFemas	飞马柜台交易通道对接模块
	- TraderXTP		XTP柜台交易通道对接模块