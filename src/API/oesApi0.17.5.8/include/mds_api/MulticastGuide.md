# 组播行情对接说明

一、概述
-----------------------------------

因为目前同时在线的连接数量较多，导致TCP行情延迟较大，而且延迟抖动很大，会对交易效果产生不利影响。
而测试发现在当前环境下组播行情的延迟相比TCP行情有明显优势，特别是在并发连接数量越多、行情数据量越大时，组播行情的优势更加明显。而且组播行情的丢包率很低，在多个完整交易日的测试过程中，组播行情的丢包率均为0。
因此建议实盘下优先对接组播行情，以降低行情延迟、提升交易效果。
*TCP行情与组播行情的具体差异情况参见：“二、客户端优化建议”。*

宽睿API同时支持对接TCP行情和组播行情，如果使用的是异步API（MdsAsyncApi），可以很方便的对接组播行情。
*组播行情的具体对接方法参见：“三、对接组播行情的具体过程”。*

如果当前使用的还是同步API，建议切换为使用异步API。切换的工作量较小，只需要修改初始化方法即可，无需修改负责行情数据处理的回调方法。
*异步API的具体切换方法参见：“附录A：从同步API切换到异步API的具体过程”。*


二、客户端优化建议
-----------------------------------

* **建议优先对接组播行情，组播行情延迟和延迟抖动相比TCP行情优秀很多：**
	- 因为同时在线的连接数量较多，行情延迟会相差一个数量级，特别是在行情数据量大时，组播行情相对TCP行情的优势更加明显；
	- 组播行情的平均端到端延迟为8微秒左右，TCP行情的平均端到端延迟为80微秒左右；
	- 9:30开盘时刻的组播行情最大端到端延迟为0.8~1.6毫秒，TCP行情开盘时刻最大端到端延迟为25毫秒；
	- TCP行情存在因为网络事件导致最大延迟偶尔达到580毫秒的异常情况，而组播行情不存在这样的问题。

* **如果必须对接TCP行情，则需要注意以下方面：**
	- 减少订阅的行情数据量有助于降低延迟抖动，建议尽量减少订阅的证券数量以降低行情数据量；
	- 盘中连接或重建连接会带来很大的延迟抖动，需要尽量避免盘中创建或重建连接；
	- 如果对延迟不是很敏感（例如通过图形客户端对接行情，或者只是为了落地行情数据），建议优先对接压缩行情端口。这样即可以降低行情推送的压力，又可以节约网络带宽。


三、对接组播行情的具体过程
-----------------------------------

### 1、修改配置文件中的相关配置项
#### 1.1 组播地址配置
组播行情划分为以下4个频道，可以同时对接所有频道，也可以按需要仅对接个别频道：
		
	- udpServer.Snap1: 快照-频道1, 上海L1/L2快照
	- udpServer.Snap2: 快照-频道2, 深圳L1/L2快照
	- udpServer.Tick1: 逐笔-频道1, 上海逐笔成交/逐笔委托
	- udpServer.Tick2: 逐笔-频道2, 深圳逐笔成交/逐笔委托

具体配置示例如下：

```
[mds_client]
# 上海L1/L2快照
udpServer.Snap1 = udp-mcast://232.200.151.102:5301
# 深圳L1/L2快照
udpServer.Snap2 = udp-mcast://232.200.152.102:5302
# 上海逐笔成交/逐笔委托
udpServer.Tick1 = udp-mcast://232.200.153.102:5303
# 深圳逐笔成交/逐笔委托
udpServer.Tick2 = udp-mcast://232.200.154.102:5304
```

#### 1.2 指定用于接收组播数据的网络设备接口

指定用于接收组播数据的本地网口的IP地址，以避免对接不到组播数据。

```
[mds_client]
# 用于接收组播数据的网络设备接口的IP地址
mcastInterfaceIp = 10.33.42.xxx
```

#### 1.3 查询服务地址配置

对接组播行情时，也可以同时对接查询服务。建议使用异步API的内置查询通道，这样就无需进行显式的管理，只在配置文件中配置好查询服务地址即可。

具体配置示例如下：

```
[mds_client]
qryServer = tcp://行情主机IP:5201, tcp://行情备机IP:5202
```

#### 1.4 行情订阅相关的配置
组播行情也支持行情订阅和过滤功能 (仅异步API支持)。
要启用组播行情的本地行情订阅和过滤功能，需要开启 isUdpFilterable 配置参数。

```
[mds_client.async_api]
# 是否启用对UDP行情数据的本地行情订阅和过滤功能
# - 如果该参数为TRUE, 则允许对UDP行情设置订阅条件, 并在API端完成对行情数据的过滤
isUdpFilterable = yes
```

行情订阅的订阅参数和订阅接口和TCP行情完全相同。行情订阅相关的配置项如下：

```
# 上交所股票(包含基金和债券)的订阅标志
sse.stock.enable = true
# 订阅的产品代码列表, 为空表示订阅所有产品
#sse.stock.instrs = 600096, 601933

# 上交所指数的订阅标志
sse.index.enable = true
sse.index.instrs =

# 上交所期权的订阅标志
sse.option.enable = true
#sse.option.instrs = 10000001, 11001996

# 深交所股票(包含基金和债券)的订阅标志
szse.stock.enable = true
szse.stock.instrs =

# 深交所指数的订阅标志
szse.index.enable = true
szse.index.instrs =

# 深交所期权的订阅标志
szse.option.enable = true
szse.option.instrs =

# 数据模式 (0: 订阅最新快照并跳过过时数据, 1: 订阅最新快照并立即发送, 2: 订阅所有时点的行情快照(对组播行情不适用))
mktData.tickType = 1

# 逐笔数据的数据重建标识 (标识是否订阅重建到的逐笔数据. 0: 不订阅重建到的逐笔数据, 1: 订阅重建到的逐笔数据, 2: 只订阅重建到的逐笔数据(对组播行情不适用))
mktData.tickRebuildFlag = 0

# 订阅的数据种类
# (0:所有, 1:L1快照/指数/期权, 2:L2快照, 4:L2委托队列, 8:逐笔成交,
#  0x10:深交所逐笔委托, 0x20:上交所逐笔委托, 0x40:L2市场总览,
#  0x100:市场状态, 0x200:证券实时状态, 0x400:指数行情, 0x800:期权行情)
# 要订阅多个数据种类, 可以用逗号或空格分隔, 或者设置为并集值, 如:
# "mktData.dataTypes = 1,2,4" 或等价的 "mktData.dataTypes = 0x07"
mktData.dataTypes = 0
```


### 2、调用API初始化接口

具体示例代码可以参见样例文件：03_mds_async_udp_sample.c

大致过程如下:

1) 初始化异步API的运行时环境 (MdsAsyncApi_CreateContext)

```
    pAsyncContext = MdsAsyncApi_CreateContext(CONFIG_FILE_NAME);
    if (! pAsyncContext) {
        SLOG_ERROR("创建异步API的运行时环境失败!");
        return -1;
    }
```

2) 逐个添加待对接的组播频道/通道信息 (MdsAsyncApi_AddChannel)，并同时指定相关的回调函数
	
```
    /*
     * 从配置文件中加载组播频道1的通道配置 - udpServer.Snap1 (快照-频道1, 上海L1/L2快照)
     *
     * @note 关于 OnConnect, OnDisconnect 回调函数:
     * - OnConnect 回调函数可以为空, 若不指定 OnConnect 回调函数, 则会使用通道配置中
     *   默认的订阅参数订阅行情数据
     * - OnDisconnect 回调函数仅用于通知客户端连接已经断开, 异步线程会自动尝试重建连接
     *
     * @note 关于 OnMsg 回调函数:
     * - 也可以为不同的通道分别指定不同的消息处理函数
     */
    pAsyncChannel = MdsAsyncApi_AddChannelFromFile(
            pAsyncContext, "udp_snap1_SH",
            CONFIG_FILE_NAME, MDSAPI_CFG_DEFAULT_SECTION,
            MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP1,
            _MdsAsyncUdpSample_HandleMsg, NULL,
            _MdsAsyncUdpSample_OnConnect, NULL,
            _MdsAsyncUdpSample_OnDisconnect, NULL);
    if (! pAsyncChannel) {
        SLOG_ERROR("从配置文件中加载通道配置失败! channelTag[%s]",
                MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP1);
        goto ON_ERROR;
    }

    /* 从配置文件中加载组播频道2的通道配置 - udpServer.Snap2 (快照-频道2, 深圳L1/L2快照) */
    pAsyncChannel = MdsAsyncApi_AddChannelFromFile(
            pAsyncContext, "udp_snap2_SZ",
            CONFIG_FILE_NAME, MDSAPI_CFG_DEFAULT_SECTION,
            MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP2,
            _MdsAsyncUdpSample_HandleMsg, NULL,
            _MdsAsyncUdpSample_OnConnect, NULL,
            _MdsAsyncUdpSample_OnDisconnect, NULL);
    if (! pAsyncChannel) {
        SLOG_ERROR("从配置文件中加载通道配置失败! channelTag[%s]",
                MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_SNAP2);
        goto ON_ERROR;
    }

    /* 从配置文件中加载组播频道3的通道配置 - udpServer.Tick1 (逐笔-频道1, 上海逐笔成交/逐笔委托) */
    pAsyncChannel = MdsAsyncApi_AddChannelFromFile(
            pAsyncContext, "udp_tick1_SH",
            CONFIG_FILE_NAME, MDSAPI_CFG_DEFAULT_SECTION,
            MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK1,
            _MdsAsyncUdpSample_HandleMsg, NULL,
            _MdsAsyncUdpSample_OnConnect, NULL,
            _MdsAsyncUdpSample_OnDisconnect, NULL);
    if (! pAsyncChannel) {
        SLOG_ERROR("从配置文件中加载通道配置失败! channelTag[%s]",
                MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK1);
        goto ON_ERROR;
    }

    /* 从配置文件中加载组播频道4的通道配置 - udpServer.Tick2 (逐笔-频道2, 深圳逐笔成交/逐笔委托) */
    pAsyncChannel = MdsAsyncApi_AddChannelFromFile(
            pAsyncContext, "udp_tick2_SZ",
            CONFIG_FILE_NAME, MDSAPI_CFG_DEFAULT_SECTION,
            MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK2,
            _MdsAsyncUdpSample_HandleMsg, NULL,
            _MdsAsyncUdpSample_OnConnect, NULL,
            _MdsAsyncUdpSample_OnDisconnect, NULL);
    if (! pAsyncChannel) {
        SLOG_ERROR("从配置文件中加载通道配置失败! channelTag[%s]",
                MDSAPI_CFG_DEFAULT_KEY_UDP_ADDR_TICK2);
        goto ON_ERROR;
    }
```

3) 启动异步API线程 (MdsAsyncApi_Start)

```
    if (! MdsAsyncApi_Start(pAsyncContext)) {
        SLOG_ERROR("启动异步API线程失败!");
        goto ON_ERROR;
    }
```

4) 终止异步API线程 (MdsAsyncApi_Stop)

```
	MdsAsyncApi_Stop(pAsyncContext);
```


四、性能相关的配置项
-----------------------------------

### 1、异步API提供的性能相关选项

```
[mds_client.async_api]
# 异步队列的大小 (可缓存的消息数量)
asyncQueueSize = 100000
# 是否优先使用大页内存来创建异步队列
isHugepageAble = yes
# 是否启动独立的回调线程来执行回调处理 (否则将直接在通信线程下执行回调处理)
isAsyncCallbackAble = yes
# 是否使用忙等待模式 (TRUE:延迟更低但CPU会被100%占用; FALSE:延迟和CPU使用率相对均衡)
isBusyPollAble = yes
```

### 2、CPU亲和性配置

```
[cpuset]
# CPU亲和性配置的使能标志
enable = yes
# 默认的CPU绑定配置, 建议和通信线程、回调线程在一个NUMA节点
default = 1

# 异步API线程的CPU绑定配置
# - 通信线程 (mdsapi_communication): 关键线程, 需要分配一个独立的CPU核
# - 回调线程 (mdsapi_callback): 关键线程, 需要分配一个独立的CPU核
# - 连接管理线程 (mdsapi_connect): 辅助线程(默认关闭), 无需显式配置, 使用默认的亲和性配置就可以 (和通信线程、回调线程在一个NUMA节点)
# - 异步I/O线程 (mdsapi_io_thread): 辅助线程(默认关闭), 分配到一个公共的CPU核, 避免影响关键线程就可以
mdsapi_communication = 3
mdsapi_callback = 5
mdsapi_connect = 1
mdsapi_io_thread = 4
```


五、注意事项
-----------------------------------

### 1、对接组播行情的前提条件

1. 网络环境良好

   - 使用部署在托管机房内的物理机，而非虚拟化环境
   - 配备万兆低延迟网卡

2. 申请开通组播网络，并做好组播数据对接的配置和调试工作，特别是留意以下会影响组播数据对接的方面：

   - 关闭 selinux
   - 关闭防火墙
   - 禁用反向路径检查（rp_filter）
     - net.ipv4.conf.default.rp_filter = 0
     - net.ipv4.conf.all.rp_filter = 0
     - net.ipv4.conf.XXX.rp_filter = 0 (XXX 为对接组播数据的网口, 例如: p5p2)

3. API配置文件中需要指定指定用于接收组播数据的网络设备接口：

```
	[mds_client]
	# 用于接收组播数据的网络设备接口的IP地址
	mcastInterfaceIp = 10.33.42.xxx
```

### 2、影响性能的注意事项

1. 做好基础的服务器和网络优化，包括但不限于以下方面：

   - 优化BIOS参数，关闭节能选项，关闭超线程，启用高性能模式或相关选项
   - 禁用防火墙、selinux，并关闭不需要的服务
   - 优化内核启动参数（grub.cfg），例如：
     - selinux=0 ipv6.disable=1 transparent_hugepage=never intel_idle.max_cstate=0 processor.max_cstate=0 intel_pstate=disabled idle=poll iommu=off intel_iommu=off pcie_aspm=performance mce=ignore_ce audit=0 

   - 优化内核运行参数（sysctl.conf），例如：
     - net.core.netdev_max_backlog = 65536
     - net.ipv4.tcp_mem = 524288 1048576 2621440
     - net.ipv4.udp_mem = 524288 1048576 2621440
     - net.ipv4.tcp_rmem = 16384 524288 11960320
     - net.ipv4.tcp_wmem = 16384 524288 11960320
     - net.ipv4.udp_rmem_min = 16384
     - net.ipv4.udp_wmem_min = 16384
     - net.core.busy_poll = 50
     - net.core.busy_read = 50

   - 优化系统运行参数，例如（RedHat/CentOS下）：
     - tuned-adm profile network-latency
	  - x86_energy_perf_policy performance

   - 优化网卡参数，例如：
     - ethtool -G p7p2 rx 4096 tx 2048
     - ethtool -C p7p2 adaptive-rx off rx-usecs 0 rx-usecs-irq 0

   - 建议关闭端口聚合
   - 推荐使用 Linux 平台

2. 设置CPU亲和性时，需要绑定到物理核，不能绑定到超线程的逻辑核。实际上对于追求低延迟的场景，建议关闭超线程；
3. 建议启用低延迟网卡的内核旁路等优化选项（例如 Solarflare 网卡的 onload 驱动）。


附录A：从同步API切换到异步API的具体过程
-----------------------------------

参考第三章第二节："2、调用API初始化接口"，以及样例代码：

- 01_mds_async_tcp_sample.c （基于异步API的TCP行情对接的样例代码）
- 03_mds_async_udp_sample.c （基于异步API的UDP行情对接的样例代码）
- 04_mds_sync_tcp_sample.c  （基于同步API的TCP行情对接的样例代码）

通过比对同步API样例代码（04_mds_sync_tcp_sample.c）和异步API样例代码（01_mds_async_tcp_sample.c），可以看出两者的差异。

同步API的执行过程如下:

1. 通过 MdsApi_InitAll 接口初始化客户端环境并建立好连接通道
2. 通过 MdsApi_SubscribeByString 接口订阅行情
3. 直接在主线程中轮询等待行情消息到达（MdsApi_WaitOnMsg）, 并通过回调函数对行情消息进行处理

异步API的执行过程如下:

1. 初始化异步API的运行时环境 (MdsAsyncApi_CreateContext)
2. 添加通道配置信息 (MdsAsyncApi_AddChannel)
   - 指定执行行情消息处理的回调函数
     - **消息处理回调函数与同步API完全相同**
   - 指定连接完成后的回调函数, 并在该回调函数中完成行情订阅处理
   	  - 行情订阅接口名称改为 **MdsAsyncApi_SubscribeByString** 即可
   	  - 除了第一个参数，其它参数和同步接口一致
3. 启动异步API线程 (MdsAsyncApi_Start)
4. 终止异步API线程 (MdsAsyncApi_Stop)

由同步API切换到异步API的大致过程如下：

1. 将初始化方法改为使用异步API的相关初始化接口
   - 初始化异步API的运行时环境 (MdsAsyncApi_CreateContext)
   - 添加通道配置信息 (MdsAsyncApi_AddChannel)
2. 将行情订阅操作迁移到异步API的 OnConnect 回调函数中执行
3. 删除轮询等待行情消息到达（MdsApi_WaitOnMsg）的代码，行情消息处理回调函数将由API自动调度执行
4. 调用 MdsAsyncApi_Start 方法启动异步API线程
5. 在需要停止运行时，调用 MdsAsyncApi_Stop 方法终止异步API线程


附录B：异步API与同步API的区别
-----------------------------------

同步接口是最小化的封装，不会有任何额外的消耗，所以利用同步接口可以达到最极致的性能，不过这些都需要客户端自行实现，API没有额外的支持。

异步接口的底层也是基于同步接口实现的，相对于同步接口主要是易用性和功能方面的提升，包括：

1. 异常处理方面：内置了异常检测和连接重建机制；
2. 性能方面：内置了之前需要客户端实现的优化技术，包括CPU绑定、忙等待、大页内存、异步回调处理、异步延迟统计和异步数据落地等；
3. 易用性方面：提供更加简单的接口，特别是更易于对接多频道的组播行情；
4. 功能方面：针对组播行情，提供了本地行情订阅功能，可以使用和TCP相同的接口来对组播行情进行订阅和过滤。

总的来说，异步接口是在不牺牲性能的前提下，最大程度的平衡和满足性能和易用性方面的需要。
推荐使用异步API接口。
