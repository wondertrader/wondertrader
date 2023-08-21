# wt > src
这是wondertrader的C++底层源码

## 开发环境
+ Windows	
	> `Visual Studio 2017` + `Windows 10`
+ Linux	
	> `Gcc v8.4.0` + `cmake 3.17.5`

## 依赖库
+ [boost 1.72](https://www.boost.org/)
+ [rapidjson 1.0.2](https://github.com/Tencent/rapidjson)
+ [spdlog 1.9.2](https://github.com/gabime/spdlog)
+ [nanomsg 1.1.5](https://github.com/nanomsg/nanomsg)

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
	- WtDataStorage	数据读取组件，用于读取WT自有格式的行情数据
	- WtDataStorageAD	数据落地组件，用于将行情数据落地到文件中
	- WtDtHelper	数据辅助工具，用于提供将数据转换成wt标准数据的接口
	- WtDtServo		数据伺服器，用于提供实时的数据随机访问接口
+ ***Parsers***
	Parsers包含了所有的行情解析器模块代码
	- ParserCTP		(期)对接CTP行情通道的行情解析器
	- ParserCTPMini	(期)对接CTPMini行情通道的行情解析器
	- ParserFemas	(期)对接飞马行情通道的行情解析器
	- ParserYD		(期)对接易达期货行情通道的行情解析器
	- ParserXeleSkt	(期)对接艾克朗科组播行情通道的行情解析器	
	- ParserCTPOpt	(权)对接CTPOpt期权行情通道的行情解析器
	- ParserMA		(权)对接金证期权maOpt期权行情通道的行情解析器
	- ParserAres	(权)对接QWIN期权行情通道的行情解析器
	- ParserHuax	(股)对接华鑫证券奇点行情接口的行情解析器
	- ParserXTP		(股)对接XTP行情通道的行情解析器
	- ParserOES		(股)对接宽睿行情通道的行情解析器
	- ParserUDP		对接数据组件*UDP*广播的行情数据通道的解析器
+ ***Plugins***
	Plugins包含了交易框架外部插件的项目代码
	- WtExeFact		内置的执行器工厂，提供了一个简单的执行单元
	- WtRiskMonFact	内置的风控单元工厂，提供了一个简单的组合盘资金风控的风控单元
	- WtCtaStraFact	一个示例的CTA策略工厂，内置一个C++版本的DualThrust策略
	- WtHftStraFact	一个示例的HFT策略工厂
	- WtSelStraFact 一个示例的SEL策略工厂
+ ***Product***
	Porter包含了实盘的核心项目，是整个解决方案的核心
	- WtCore		实盘交易核心库，包含了整个交易框架的核心逻辑
	- WtPorter		交易框架C接口导出库，主要用于跨语言调用
	- WtExecMon		独立执行器C接口导出库，作为独立执行器的入口
	- WtRunner		同*WtBtRunner*，实盘框架纯C++环境运行的入口
+ ***Traders***
	Traders包含了所有的交易通道的模块代码
	- TraderCTP		(期)CTP柜台交易通道对接模块
	- TraderCTPMini	(期)CTPMini柜台交易通道对接模块
	- TraderFemas	(期)飞马柜台交易通道对接模块
	- TraderYD		(期)易达柜台交易通道对接模块
	- TraderCTPOpt	(权)CTPOpt交易通道对接模块
	- TraderAresClt	(权)QWIN交易通道对接模块
	- TraderMAOpt	(权)金证期权maOpt交易通道对接模块
	- TraderXTP		(股)XTP柜台交易通道对接模块
	- TraderXTPXAlgo	(股)XTP算法交易通道对接模块
	- TraderATP		(股)华锐交易通道对接模块
	- TraderOES		(股)宽睿交易通道对接模块
	- TraderHuaX	(股)华鑫奇点交易通道对接模块
	- TraderMocker	纯本地仿真撮合模块，广泛适用于各种品种的仿真交易，减少对仿真环境的依赖，只需要接入行情就可以进行仿真交易测试
+ ***UltraFT***
	- WtUftCore		超高频引擎核心模块
	- WtUftStraFact	超高频引起示例策略工厂
	- WtUftRunner	超高频引擎实盘运行入口程序
+ ***Tools***
	- CTPLoader		CTP合约加载模块
	- MiniLoader	CTPMini合约加载模块
	- CTPOptLoader	CTPOpt合约加载模块
	- LoaderRunner	合约加载器模块运行入口程序
	- TraderDumper	交易数据落地模块，主要用于实时转储交易接口的数据
	- WtMsgQue		消息队列MQ模块，将nanomsg封装成了C接口，便于调用