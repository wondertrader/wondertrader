### 策略基本结构
---
`WonderTrader`的策略非常简单，一共就4个接口函数`on_init`、`on_bar`、`on_tick`、`on_calculate`，核心逻辑只需要写入on_calculate里即可。

```py
from wtpy import Context

class BaseStrategy:
    '''
    策略基础类，所有的策略都从该类派生\n
    包含了策略的基本开发框架
    '''
    def __init__(self, name):
        self.__name__ = name
        
    
    def name(self):
        return self.__name__


    def on_init(self, context:Context):
        '''
        策略初始化，启动的时候调用\n
        用于加载自定义数据\n
        @context    策略运行上下文
        '''
        return

    
    def on_calculate(self, context:Context):
        '''
        K线闭合时调用，一般作为策略的核心计算模块\n
        @context    策略运行上下文
        '''
        return


    def on_tick(self, context:Context, stdCode:str, newTick:dict):
        '''
        逐笔数据进来时调用\n
        生产环境中，每笔行情进来就直接调用\n
        回测环境中，是模拟的逐笔数据\n
        @context    策略运行上下文\n
        @code       合约代码
        @newTick    最新逐笔
        '''
        return

    def on_bar(self, context:Context, stdCode:str, period:str, newBar:dict):
        '''
        K线闭合时回调
        @context    策略上下文\n
        @code       合约代码
        @period     K线周期
        @newBar     最新闭合的K线
        '''
        return
```

### 回调函数详解
---
为了详细的介绍各个接口函数的作用，我们以demo中的DualThrust策略为例，来参照介绍
```py
from wtpy import BaseStrategy
from wtpy import Context

class StraDualThrust(BaseStrategy):
    
    def __init__(self, name:str, code:str, barCnt:int, period:str, days:int, k1:float, k2:float, isForStk:bool = False):
        BaseStrategy.__init__(self, name)

        self.__days__ = days
        self.__k1__ = k1
        self.__k2__ = k2

        self.__period__ = period
        self.__bar_cnt__ = barCnt
        self.__code__ = code

        self.__is_stk__ = isForStk

    def on_init(self, context:Context):
        code = self.__code__    #品种代码
        if self.__is_stk__:
            code = code + "Q"

        #这里演示了品种信息获取的接口
        #pInfo = context.stra_get_comminfo(code)
        #print(pInfo)

        context.stra_get_bars(code, self.__period__, self.__bar_cnt__, isMain = True)
        context.stra_log_text("DualThrust inited")

        #读取存储的数据
        self.xxx = context.user_load_data('xxx',1)

    
    def on_calculate(self, context:Context):
        code = self.__code__    #品种代码

        trdUnit = 1
        if self.__is_stk__:
            trdUnit = 100

        #读取最近50条1分钟线(dataframe对象)
        theCode = code
        if self.__is_stk__:
            theCode = theCode + "Q"
        df_bars = context.stra_get_bars(theCode, self.__period__, self.__bar_cnt__, isMain = True)

        #把策略参数读进来，作为临时变量，方便引用
        days = self.__days__
        k1 = self.__k1__
        k2 = self.__k2__

        #平仓价序列、最高价序列、最低价序列
        closes = df_bars["close"]
        highs = df_bars["high"]
        lows = df_bars["low"]

        #读取days天之前到上一个交易日位置的数据
        hh = highs[-days:-1].max()
        hc = closes[-days:-1].max()
        ll = lows[-days:-1].min()
        lc = closes[-days:-1].min()

        #读取今天的开盘价、最高价和最低价
        lastBar = df_bars.iloc[-1]
        openpx = lastBar["open"]
        highpx = lastBar["high"]
        lowpx = lastBar["low"]

        '''
        !!!!!这里是重点
        1、首先根据最后一条K线的时间，计算当前的日期
        2、根据当前的日期，对日线进行切片,并截取所需条数
        3、最后在最终切片内计算所需数据
        '''

        #确定上轨和下轨
        upper_bound = openpx + k1* max(hh-lc,hc-ll)
        lower_bound = openpx - k2* max(hh-lc,hc-ll)

        #读取当前仓位
        curPos = context.stra_get_position(code)/trdUnit

        if curPos == 0:
            if highpx >= upper_bound:
                context.stra_enter_long(code, 1*trdUnit, 'enterlong')
                context.stra_log_text("向上突破%.2f>=%.2f，多仓进场" % (highpx, upper_bound))
                #修改并保存
                self.xxx = 1
                context.user_save_data('xxx', self.xxx)
                return

            if lowpx <= lower_bound and not self.__is_stk__:
                context.stra_enter_short(code, 1*trdUnit, 'entershort')
                context.stra_log_text("向下突破%.2f<=%.2f，空仓进场" % (lowpx, lower_bound))
                return
        elif curPos > 0:
            if lowpx <= lower_bound:
                context.stra_exit_long(code, 1*trdUnit, 'exitlong')
                context.stra_log_text("向下突破%.2f<=%.2f，多仓出场" % (lowpx, lower_bound))
                #raise Exception("except on purpose")
                return
        else:
            if highpx >= upper_bound and not self.__is_stk__:
                context.stra_exit_short(code, 1*trdUnit, 'exitshort')
                context.stra_log_text("向上突破%.2f>=%.2f，空仓出场" % (highpx, upper_bound))
                return


    def on_tick(self, context:Context, stdCode:str, newTick:dict):
        return

    def on_bar(self, context:Context, stdCode:str, period:str, newBar:dict):
        return
```

* 初始化接口 on_init
    初始化接口，是一个重要的接口，最大的作用是通过拉取系统所需要的历史K线，向系统注册一个主K线的闭合回调，以及tick数据的回调。
    ```py
    def on_init(self, context:Context):
        code = self.__code__    #品种代码
        if self.__is_stk__:
            code = code + "Q"

        context.stra_get_bars(code, self.__period__, self.__bar_cnt__, isMain = True)
        context.stra_log_text("DualThrust inited")
    ```
    从上面的代码片段可以看到，`DualThrust`策略在初始化的时候，拉取了K线数据。`WonderTrader`里的K线数据都不用自己管理，开发策略的时候，直接调用接口就可以了。**`WonderTrader`后台做了一系列的工作，保证K线处理的速度和准确度，用户不需要担心数据拷贝等细节问题**。
    调用了拉取历史K线的接口以后，系统就会自动注册对应K线的闭合事件回调以及tick数据回调，即激活了`on_bar`和`on_tick`事件。

* K线闭合接口 on_bar
    K线闭合接口，K线闭合的时候系统会自动调用该接口。对于多条同周期K线的策略，比如配对交易，用到了两个关联品种的1分钟线K线。那么系统在1分钟线闭合的时候会自动触发两次`on_bar`事件，通知策略K线闭合。
    如果策略对于K线闭合有特殊的响应，则可以重写该接口。K线闭合的闭合，一般来说按照`on_init`里读取K线数据的先后顺序来触发响应。如果主K线本轮也闭合了，当所有闭合K线回调完成以后，系统就会调用`on_calculate`接口触发策略主逻辑重算。
    ```py
    def on_bar(self, context:Context, stdCode:str, period:str, newBar:dict):
        return
    ```
    * `context`是策略的环境，系统提供的所有接口都在`context`里
    * `stdCode`是标准品种代码，如`CFFEX.IF.HOT`，这个代码和用户使用的代码一致，即使是主力合约代码，也不会转换成分月合约，方便用户处理
    * `period`是K线周期，和用户拉取的K线周期一致，格式如`m1`、`m3`、`d1`
    * `newBar`是闭合的K线的数据，是一个`dict`，字段如下
    ```py
    {
        "time":202005121055,  #如果是日线，就是yyyyMMdd格式
        "open":3963.2,
        "high":3986.4,
        "low":3956.6,
        "close":3974.0,
        "volumn":1342
    }
    ```

* tick回调接口 on_tick
    tick回调接口在收到新的tick数据的时候触发。对于使用分钟线的策略来说，一个交易日内K线会多次闭合触发`on_bar`。但是对于使用日线的策略来说，因为当日的K线并没有闭合，所以历史K线中是没有当日K线的数据的，这个时候就需要在`on_tick`中来读取最新交易日的数据了。
    ```py
    def on_tick(self, context:Context, stdCode:str, newTick:dict):
        return
    ```
    * `context`是策略的环境，和on_bar一致的
    * `stdCode`是标准合约代码，同on_bar中的stdCode
    * `newTick`是最新的tick数据，是一个`dict`，字段如下
    ```py
    {
        "time": 20200512111053700,  #tick时间，精确到毫秒，yyyyMMddhhmmssmmm
        "open": 3963.2,
        "high":3986.4,
        "low":3956.6,
        "price":3974.0
    }
    ```

* 策略重算接口 on_calculate
    `on_calculate`是策略的核心逻辑所在，是整个策略的重中之重。`on_calculate`是以主K线的闭合来触发的。当主K线闭合后，并且所有的K线的闭合事件都已经回调以后，就会触发`on_calculate`进行主逻辑重算。`context`具体接口和用法我们在后面会详细介绍，这里就不作赘述了。
    ```py
    def on_calculate(self, context:Context):
        '''
        主K线闭合后调用，一般作为策略的核心计算模块\n
        @context    策略运行上下文
        '''
        return
    ```