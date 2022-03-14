### 0.9.0(重大版本)
* 将数据读写模块WtDataReader和WtDataWriter统一整合为WtDataStorage，并将回测框架和WtDtServo中的随机读取模块整合到该模块中
* （**重要**）新增一个基于LMDB实现的数据存储引擎WtDataStorageAD，主要针对7*24小时交易品种的存储场景
* （**重要**）重构了底层数据结构WTSTickStruct和WTSBarStruct，修改了数据结构对齐方式以及部分字段类型，以兼容更多的交易品种，并且将所有读写的地方做了新老数据结构的兼容处理
* 重构了CodeHelper模块，不再通过CodeHelper解析代码来判断合约类型，全部改成从WTSCommodityInfo来判断，并且将代码中按照种类处理代码格式的逻辑全部改成从一些文本规则转换标准代码，以兼容更多的代码规则
* （**重要**）优化了WTSBaseDataMgr加载合约的流程，支持将合约直接当成品种进行处理，主要为了适应一些每个合约的交易规则都不同的场景
* 修改了WTSKlineSlice和WTSTickSlice，改为多数据块的模式，使用更加灵活
* 重构WTSLogger，全面改成兼容fmtlib的格式化语法
* （**重要**）配置文件全面兼容yaml和json两种格式，并实现了一个WTSCfgLoader模块自动处理
* 完善了对股票复权数据的处理
* （**重要**）新增一个极速交易引擎UFTEngine(WtUftCore、WtUftStraFact、WtUftRunner三个工程)，独立于其他几个引擎，不做过多兼容处理，只为了针对极速交易场景
* （**重要**）速度优化（时间函数、字符串函数、hash、对象池等优化），将UFTEngine的系统延迟优化到175纳秒以内，HFTEngine的系统延迟优化到1.5微秒以内
* （**重要**）新增两个延迟测试工具WtLatencyHFT和WtLatencyUFT，分别用于测试HFTEngine和UFTEngine的系统延迟
* （**重要**）内部细节调整，完善了对7*24小时品种的支持
* （**重要**）TraderAdapter新增了自成交熔断机制
* 新对接了易达交易柜台(TraderYD、PaserYD)
* 新对接了艾克朗科行情接口(ParserXeleSkt)
* 新增了一个绑核辅助模块CpuHelper
* 其他细节优化和Bug修正


### 0.8.0(大版本)
* （**重要**）实现了ExtDataLoder的机制，实盘和回测框架都可以通过应用层的扩展数据加载器加载历史数据（详见WtBtCore/HisDataReplayer和WtDataStorage/WtDtReader）
* （**重要**）实现了ExtDataDumper的机制，如果向datakit注册了ExtDataDumper，在收盘作业的时候，就会通过ExtDataDumper将实时数据转储
* （**重要**）代码级重构，解决了CMakeLists和GCC编译的兼容问题，可以在win下使用MinGW编译	
* （**重要**）完善了对T+1交易机制的支持
* （**重要**）统一了实盘框架策略的理论成交价和组合的理论成交价，方便测算执行的滑点
* （**重要**）上一期主力合约自动清理的机制改成可配置，默认不打开，可以设置includes和excludes，可以更好地兼容多个组合在同一个账户下交易的时候，同时清理过期合约的场景
* （**重要**）优化了内代码解析的辅助模块，将股票代码也强制标准化为SSE.STK.600000，不再支持SSE.600000的简写格式
* 将WtDataReader和WtDataWriter合并成WtDataStorage，把相同存储机制的reader和writer放在一个工程下，方便后面扩展别的存储机制的模块
* 去掉了C++底层对Mysql的支持，改用ExtDataLoader和ExtDataDumper来实现
* 去掉了对内置的易盛iTap9.0交易接口，因为内置版本较老，以后后需要再重写
* 新增一个单元测试项目TestUnits，基于gtest实现，以后逐步完善
* 郑商所行情发现1秒4笔的情况，原来对于时间戳的兼容处理，做了相应的修改
* WtDtHelper模块中，将trans_bars和trans_ticks接口取消，改用store_bars和store_ticks代替，采用连续整块内存转储的方式
* 其他细节优化和bug修正

### 0.7.1
* 回测框架的CTAMocker和HFTMocker都增加了单步调度切入机制，用于单步控制回测的进度（该机制的主要目的是针对强化学习的通用接口提供对接方式）
* WtDtPorter支持了扩展Parser，用户可以用wtpy中直接向datakit底层推送实时行情
* 执行单元WtExecUnit新增一个接口clear_all_position，用于清理某个合约的全部持仓（净头寸机制下，全部锁仓时，正常执行单元不会平仓，该接口就是用于告知执行单元，多空头寸也要全部清掉）
* 其他的细节优化和Bug修正

### 0.7.0
* 新增一个WtDtServo模块，用于提供直接访问WonderTrader落地的数据文件的接口，主要包括按照时间区间获取数据get_bars_by_range、get_ticks_by_range，以及按照条数获取数据get_bars_by_count、get_ticks_by_count两大类接口
* 根据WtDtServo的需求，优化了WtDataWriter模块数据处理流程
* 回测框架支持重复回测，即在同一个进程中可以多次调用run_backtest接口。可以通过调用clear_cache释放已加载的历史数据，如果不释放，在下一次回测之前只会将数据访问标记重置，从而减少多次回测的IO开销
* 新增一个基于nanomsg封装的pub/sub消息队列WtMsgQue模块，该模块采用C接口封装，可以单独使用，主要作为实盘和回测消息通知模块EventNotifier的基础组件使用。提供了MQServer和MQClient两个类，可以很方便上手
* 实盘框架的EventNotifier，调用WtMsgQue的MQServer，向外推送日志、成交、风控等消息
* 回测框架的EventNotifier，调用WtMsgQue的MQServer，向外推送回测进度、每日结算的资金信息等消息
* WtExecFact新增一个最小冲击算法执行单元WtMinImpactExeUnit，对大单进行拆单发送，可以设定时间间隔、每次发单的固定数量或按照对价挂单量的比例
* 改造了回测框架的一些数据的落地，主要针对远程回测监控提供一些基础特性
* 开平策略管理器ActionPolicyMgr，新增了一个净今仓和净昨仓的检查，主要针对中金所的特殊规则（如果同事有今仓和昨仓，中金所处理平仓一定是优先平今的，这样会推高手续费）做的兼容性设置
* WtPorter和WtBtPorter获取数据的回调接口，从原来的每条数据回调一次，改成直接整块数据块回调，减少回调函数的调用次数，数据读取性能大概能提升10%左右
* 实盘框架完善了自动清理过期主力合约的机制。在净头寸的机制下，普通的清理方案，只能让净头寸降到0，实际上过期主力合约要求双边持仓都要降到0，所以这里要专门单独处理。
* 实现了股票后复权数据的兼容处理，当获取历史数据代码如SSE.600000H，则是后复权数据，如果后缀为Q，则是前复权数据。后复权数据要求最新的K线数据要实时复权，所以内部数据处理的机制也做了一些兼容性调整
* WtsTools工程下，新增一个CsvReader类，方便从csv文件加载历史数据
* 回测框架支持异步回测，同时提供stop_backtest接口，可以从外部手动停止回测，主要是为了适配强化学习框架gym的训练环境
* 其他细节优化和Bug修正

### 0.6.5
* 将CTPLoader、CTPOptLoader和MiniLoader改成动态库，方便应用层调用
* 交易通道合规风控代码细节优化
* 支持模块名自动根据平台补全后缀
* 本地执行器WtLocalExecuter支持针对品种设置执行策略
* WtDtHelper模块新增一个resamle_bars接口，用于对K线进行重采样
* 内部逻辑增加了对次主力合约的支持，代码格式如CFFEX.IF.2ND
* 完善了回测框架CTA策略回测时对于数据的兼容性
* 完善了tick回测时对于tick数据不够时的兼容性
* 完善了回测框架条件单处理的细节
* WtBtPorter的dump_bars接口修改为通过回调返回数据，而不是直接导出到csv
* WtPorter新增dump_bars接口，用法同WtBtPorter
* 其他Bug修正和细节优化

### 0.6.4
* 性能调优
* 调整了CTAMocker中的一些细节
* 优化了C接口模块获取数据的接口如cta_get_bars、cta_get_ticks，可以提升调用层(wtpy)的性能
* TraderCTPOpt模块中CashIn为权利金净收入，合并到平仓盈亏字段中
* WtDtHelper模块增加了trans_bars和trans_ticks，用于通过C接口直接将数据转储到dsb文件
* WtPorter新增了ExpParser和ExpExecuter两个类，用于与C接口的外接行情接入模块和执行模块进行交互
* WtPorter模块接口增加向底层推送行情数据的接口parser_push_quote
* 完善了WtDtCore中行情接入模块订阅过滤规则，统一改成filter参数
* WtBtPorter模块和WtDtHelper模块，从CSV导入K线数据，添加了对更多字段的支持
* CAT策略新增获取资金数据接口stra_get_fund_data
* CTAMocker和SelMocker暴露设置滑点的地方
* 增加自动补全动态库文件名的机制，目前主要适配好了风控模块、行情解析模块、交易模块、数据读写模块
* WtDataWriter增加一些配置项，用于控制不同类型的数据是否落地
* 将配置文件中的traders和parsers全部拆分成独立的配置文件
* 其他细节优化


### 0.6.3
* 完善了回测框架中CTA仿真器CTAMocker中对条件单的处理
* 新增一个tsl的robin_map库，替换unordered_map和unordered_set，处理速度提升5倍以上（robin_map官方宣传）
* 增加了堆栈打印的模块，可以在崩溃时把堆栈信息输出到日志中，方便调试

### 0.6.2
* 将日志全部翻译成英文
* 内置简单执行单元WtSimpExeUnit增加了涨跌停价的修正逻辑
* 内置执行单元工厂WtExeFact中的订单管理模块WtOrdMon，检查订单超时时，会根据是否是涨跌停价挂单，如果是涨跌停价挂单，则不进行撤单重挂
* CTPMini2对接模块ParserCTPMini和TraderCTPMini进行的细节完善，并接入实盘
* 文档做了一次更新
* 其他代码细节完善

### 0.6.1
* 新增一个/dist目录，用于发布一些程序的执行环境的配置文件
* 将CTA、HFT和SEL引擎的策略新增on_session_begin和on_session_end用于向策略推送交易日开始和交易日结束的事件
* 完善了CTPLoader和MiniLoader，主要优化了对期权合约的支持
* 新增一个CTPOptLoader工程，主要用于CTP股票期权API接入
* 添加了CTP期权接口的行情接入模块ParserCTPOpt以及交易模块TraderCTPOpt
* 初步扩充了交易接口中的期权业务接口，同时修改了一些接口函数命名规则
* 完善了平台中对ETF期权和个股期权的支持，主要修改的点是，ETF期权和个股期权只支持标准代码格式，即SSE.ETFO.10003045，而简写格式如SSE.600000只针对股票
* WtDtHelper增加两个接口,read_dsb_ticks用于读取dsb格式的历史tick数据，read_dsb_bars用于读取dsb格式的历史K线数据
* 创建HFT策略的时候增加一个是否托管数据的参数agent，用于控制是否将持仓、成交等数据放在底层进行管理，默认是托管
* CTA引擎新增一个获取最后一次出场时间的接口stra_get_last_exittime
* 同步更新/demos下的代码
* 其他代码细节的完善


### 0.6.0(大版本)
* `CTA`策略接口新增一个获取交易日的`API`，`cta_get_tdate`和`sel_get_tdate`
* 回测引擎和实盘引擎的C接口，新增`cta_get_all_position`和`sel_get_all_position`两个策略接口，用于获取策略全部持仓
* 修复了`ParserUDP`的一些`bug`
* `CTA`引擎设置目标仓位时，同时订阅`tick`数据，主要针对标的不确定的策略，例如截面因子`CTA`策略
* 内置执行单元`WtSimpExeUnit`新增一个根据`microprice`来确定委托价格的方式
* 将内置执行模块`WtExeFact`中的订单管理模块独立出来，方便调用
* `CTA`回测引擎中，输出的平仓明细中新增“最大潜在收益”和“最大潜在亏损”两个字段
* 回测框架中，将`ExecMocker`中的模拟撮合逻辑剥离出来，放到一个单独的`MatchEngine`中，方便以后的优化
* **`HFT`引擎的回测进行了一次彻底的整理实现，基本满足了`HFT`策略回测的需求（已测试）**
* 初步完成了`HFT`引擎对股票`Level2`数据（`orderqueue`,`orderdetail`,`transaction`）的访问接口（尚未充分测试）
* `WtPorter`和`WtBtPorter`两个C接口粘合模块，初步完成了C接口对股票`Level2`数据的支持
* 其他代码细节的完善

### 0.5.4
* `WtBtPorter`、`WtPorter`、`WtExecMon`的初始化接口，全部改成支持传文件名和文件内容两种方式
* `CTA`实盘引擎中，策略发出信号的时候，新增了一个订阅`tick`的操作，主要针对策略`交易未订阅K线的品种`的需求
* 优化了`Windows`下`dmp`文件生成的路径，方便调试`bug`
* 回测引擎中，成交明细和平仓明细，新增了一个`BarNumber`的字段，主要用于统计每个交易回合的周期数，`BarNumber`指的是主K线的`BarNumber`，并且是一个相对开始回测的第一条K线的编号。
* 回测引擎中，针对`CTA`策略`交易未订阅K线的品种`的需求做了一些优化
* 全平台中，将能部分`boost`库改成`std`的库，减少对`boost`的依赖
* 新增一个`WtDtHelper`模块，主要提供数据辅助功能，目前主要是提供`csv`和二进制文件的互转，后面还会加入数据库、二进制、`csv`的互转接口
* 将平台版本号从`WTSMarcos.h`迁移到`WTSVersion.h`中，减少修改版本号引起的重编译

### 0.5.3
* 回测引擎增加了设置成交滑点的参数选项，不设置则为0
* 修正了`C++demo`中的一些代码的细节问题
* 执行模块为搭建分布式执行框架做了一些预先调整
* `ParserUDP`模块接收缓存改成`8M`
* 增加了一个`MiniLoader`工程，用于从`CTPMini`接口拉取合约列表
* 将`linux`下编译的`boost`依赖从动态库改成静态库
* 其他细节完善

### 0.5.2
* 修改`CTPLoader`为显式加载`CTP`模块，方便设置`CTP`模块的路径
* 将所有的通道对接模块（行情、交易）改成显示加载三方依赖库，并统一检查了版本的一致性
* 修正了股票`Level2`数据落地的一些细节问题
* 修正了风控触发时，限制手数比例为小数时，没有进行四舍五入导致的问题
* 历史数据新增了对`Mysql`数据库的支持，涉及到的模块包括数据读取模块`WtDataReader`、数据落地模块`WtDataWriter`、回测框架`WtBtCore`，`Mysql`库版本为`6.11`
* 修正了`UDP`广播模块中，中转的广播消息对象对`WTSObject`的处理的`bug`

### 0.5.1
* 实盘引擎（`CTA`、`HFT`、`SEL`）在启动的时候，将策略列表和交易通道列表输出到一个配置文件中，方便监控服务读取查看
* 新增一个事件通知组件`EventNotifier`，主要作用是通过`UDP`通道，向指定的接受端发送成交回报、订单回报，以后还会扩展到其他盘中需要监控的数据
* 回测引擎，回测过程中产生的数据记录（成交、信号、平仓、资金）不在回测过程中写入文件，而是到了回测结束以后统一写入文件
* 完善了系统中合约代码标准化对股指期权IO的处理

### 0.5.0
* 股票数据新增前复权因子处理逻辑，直接从未复权数据转换成前复权数据
* `demos`目录下，将`python`的demo迁移到`wtpy`之下，然后新增一个`C++`版本demo的解决方案，提供了cta策略、高频策略、执行单元、选股策略四个demo，并提供了一个回测入口程序，用于本地调试
* 日志模块调整,主要是利用`fmtlib`优化了一些日志输出的细节
* 完成了高频策略引擎C接口导出

### 0.4.0
* 新增一个**选股调度引擎**，用于调度应用层的选股策略，得到目标组合以后，提供自动执行服务，暂时只支持日级别以上的调度周期，执行会放到第二天
* 因为新增了选股调度引擎，所以全面重构`WtPorter`和`WtBtPorter`导出的接口函数，以便调用的时候区分
* 新增一个**独立的执行器模块**`WtExecMon`，并导出C接口提供服务。主要是剥离了策略引擎逻辑，提供单纯的执行服务，方便作为单纯的执行通道，嫁接到其他框架之下
* `Windows`下的开发环境从`vs2013`升级到`vs2017`，`boost1.72`和`curl`需要同步升级

### 0.3.6
* 执行器使用线程池，减少对网络线程的时间占用
* 增加了一个实盘仿真模块TraderMocker，可以满足目前已经支持的股票和期货的仿真交易
* 更多接口支持（飞马、tap、CTPMini）
* 内置执行算法增加TWAP
* 继续完善文档

### 0.3.5
* 把手数相关的都从整数改成浮点数，主要目的是为了以后兼容虚拟币交易(虚拟币交易数量都是小数单位)
* 优化手数改成浮点数以后带来的日志输出不简洁的问题(浮点数打印会显示很多“0000”)
* 逐步完善文档
* XTP实盘适配，主要是修复bug

### 0.3.4
* 正式开源的第一个版本











