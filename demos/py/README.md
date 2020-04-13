# Python Demos

Python下的demo目前大概罗列了6个，主要是演示不同环境下不同组件的使用<br>
提供了一个示例策略，DualThrust，股票和期货都用这个策略
+ backtest_fut  期货回测demo
+ backtest_stk  股票回测demo
+ datakit_fut   期货数据组件demo
+ datakit_stk   股票数据组件demo
+ product_fut   期货实盘demo，配置的是SIMNOW通道
+ product_stk   股票实盘demo，配置的是XTP的仿真通道

# 如何使用这些demo
+ 首先确认本地安装的是 *Python3.6* 以上的版本，32位、64位都可以，wtpy子框架会根据Python的版本自动选择对应的底层
+ 然后安装*WonderTrader*上的*Python*子框架[***wtpy***](https://pypi.org/project/wtpy/)(version >= v0.3.2)
+ 如何运行回测demo
    > 直接运行目录下的 *run.py* 即可
+ 如何运行实盘demo
    - 首先运行数据组件
        > 修改配置文件dtcfg.json中的解析器配置<br>
        以 *simnow* 通道为例，将********改成自己的simnow账号，然后修改*code*字段为自己要订阅的合约，合约代码规则为"市场代码.合约代码"
        >```json
        >"parsers":[
        >    {
        >        "active":true, //是否启用
        >        "module":"ParserCTP.dll",  //模块文件名，linux下为libParserCTP.so
        >        "front":"tcp://180.168.146.187:10111", //行情前置
        >        "broker":"9999", 
        >        "user":"********",
        >        "pass":"********",
        >        "code":"CFFEX.IF2005,SHFE.au2012"
        >    }
        >]
        >```
        > 修改数据dtcfg.json中落地模块配置
        >```json
        >"writer":{
        >    "path":"E:/FUT_Data",  //数据存储的路径
        >    "savelog":true,        //是否同时输出csv格式的数据
        >    "async":true,          //是否异步，异步会把数据提交到缓存队列，然后由独立线程进行处理
        >    "groupsize":100        //数据条数分组大小，每处理这么多条就会输出一条日志
        >}
        >```
        > 修改dtcfg.json中的udp广播配置（目前只开放了内存块直接广播）
        >```json
        >"broadcaster":{
        >    "active":true, //是否启用
        >    "bport":3997,  //udp监听端口
        >    "broadcast":[
        >        {
        >            "host":"255.255.255.255",  //广播地址
        >            "port":9001,               //广播端口
        >            "type":2                   //广播类型,0-纯文本格式,1-json格式,2-内存块直接广播
        >        }
        >    ]
        >}
        >```
        > 最后运行runDT.py
    - 然后修改实盘demo的配置文件*config.json*, 再运行实盘demo下的 *run.py*
        > 将数据读取配置中的路径修改为数据组件里配置的路径
        >```json
        >"data":{
        >    "store":{
        >        "path":"./STK_Data/"   //这里改为数据组件的存储路径
        >    }
        >}
        >```
        > 修改行情通道中的接收端口和接收地址
        >```json
        >"parsers":[
        >    {
        >        "active":true,
        >        "id":"parser1",
        >        "module":"ParserUDP.dll",
        >        "host":"127.0.0.1",    //udp广播的地址
        >        "bport":9001,          //udp广播的端口
        >        "sport":3997,          //udp监听的端口
        >        "filter":""            //市场过滤器，根据需要配置，如CFFEX,SHFE，可针对不同的市场配置不同的行情通道
        >    }
        >]
        >```
        > 修改交易通道的配置
        >```json
        >"traders":[
        >    {
        >        "active":true,
        >        "id":"simnow",
        >        "module":"TraderXTP.dll",
        >
        >        "host":"120.27.164.69",    //以下为交易模块专用配置
        >        "port":"6001",
        >        "user":"********",
        >        "pass":"********",
        >        "protocol":1,
        >        "clientid":1,
        >        "hbinterval":15,
        >        "buffsize":128,
        >        "quick":true,
        >
        >        "riskmon": //交易通道风控配置
        >        {
        >            "active":true,
        >            "policy":
        >            {
        >                "default": //默认策略，可以针对品种进行专门设置，格式如CFFEX.IF
        >                {
        >                    "order_times_boundary": 20,    //单位时间最高下单次数，如10s内下单20次
        >                    "order_stat_timespan": 10,     //下单次数统计时间，单位秒
        >                    
        >                    "cancel_times_boundary": 20,   //单位时间最高撤单次数，如10s内撤单20次
        >                    "cancel_stat_timespan": 10,    //撤单次数统计时间
        >                    "cancel_total_limits": 470     //总的最大撤单次数
        >                }
        >            }
        >        }
        >    }
        >]
        >```
        > 修改执行器的配置，可以根据需要配置多个执行器，执行器和交易通道一对一绑定
        >```json
        >"executers":[
        >    {
        >        "active":true, //是否启用
        >        "id":"exe0",
        >        "scale": 1,    //放大倍数 
        >        "policy":      //执行策略
        >        {
        >            "default":{    //默认执行单元，也可以根据品种设置如CFFEX.IF
        >                "name":"WtExeFact.WtSimpExeUnit",  //执行单元名（工厂名.执行单元名）
        >                "offset": 0,   //下单价格与基准价的偏移量，买入+，卖出-
        >                "expire": 40,  //未成交订单超时时间，单位秒
        >                "opposite": true   //是否使用对手价作为基准价，false时为最新价
        >            }
        >        },
        >        "trader":"simnow"  //绑定的交易通道id
        >    }
        >]
        >```
        > 修改交易环境的的配置
        >```json
        >"env":{
        >    "name":"cta",              //确定使用cta引擎，还是hft引擎
        >    "product":{                //生产环境的配置
        >        "session":"SD0930"     //!!cta总调度的会话时间模板，可以在sessions.json里找到
        >    },
        >    "filters":"filters.json",  //组合过滤器，用于临时控制某个策略或品种的信号
        >    "fees":"fees_stk.json",    //佣金模板
        >    "riskmon":{                //组合风控设置
        >        "active":true,
        >        "module":"WtRiskMonFact.dll",  //风控策略模板名
        >        "name":"SimpleRiskMon",        //风控策略名，框架会根据这个策略名创建风控策略实例
        >        "calc_span":5,                 //以下是风控策略自己的参数
        >        "risk_span": 30,
        >        "risk_scale": 0.3,
        >        "basic_ratio": 101,
        >        "inner_day_fd":20.0,
        >        "inner_day_active":true,
        >        "multi_day_fd":60.0,
        >        "multi_day_active":false,
        >        "base_amount": 5000000
        >    }
        >}
        >```
        > 修改自动开平策略的配置文件*actpolicy.json*<br>
        >以股指为例，股指平今手续费很高，所以开平优先级顺序为：平昨>开仓>平今<br>
        >如果是上期黄金和白银等品种，平今为0，则优先级顺序为：平今>平昨>开仓
        >```json
        >"stockindex":{ //开平策略名称，随意定，不要重复
		>	"order":[   //开平顺序设定
		>		{
		>			"action":"closeyestoday",   //首先平昨，这样仓位不会一直增加，减少保证金占用
		>			"limit":0                   //平昨手数限制，0为不限制
		>		},
		>		{
		>			"action":"open",            //然后再开仓
		>			"limit":500                 //开仓限制为500手
		>		},
		>		{
		>			"action":"closetoday",      //最后平今
		>			"limit":0
		>		}
		>	],
		>	"filters":["CFFEX.IF","CFFEX.IC","CFFEX.IH"]    //适用品种为IF、IC、IH
		>}
        >```
