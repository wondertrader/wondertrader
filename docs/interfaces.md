### 接口概述
---
策略API接口主要分成4类：基础数据接口、交易数据接口、信号接口和其他接口。
* 基础数据接口  主要提供一些系统基础数据
* 交易数据接口  主要用于获取行情相关数据和交易数据
* 信号接口      即用于下单的接口
* 其他接口      即不便归类的其他接口

### 基础数据接口
---

* 获取品种信息
    ```py
    def stra_get_comminfo(self, code:str):
    ```
    参数说明
    ```
    code    品种代码，如CFFEX.IF.HOT
    返回值  ProductInfo对象，可以参考ProductMgr.py模块
    ```
* 获取当前日期
    ```py
    def stra_get_date(self):
    ```
    参数说明
    ```
    返回值  当天日期yyyyMMdd，如果是回测模式下，则为回测当时的日期
    ```
* 获取当前时间
    ```py
    def stra_get_time(self):
    ```
    参数说明
    ```
    返回值  当前时间，精确到分钟。
    ```
    需要注意的是这里的时间指的是当前所属的1分钟K线对应的时间，而WonderTrader里对分钟线的处理，是以尾部时间为准。如9:30:15，这个时候调用该接口，获取到的时间为931。
* 输出日志
    ```py
    def stra_log_text(self, message:str):
    ```
    参数说明
    ```
    message 要输出的日志内容
    ```
    调用API输出日志会记录在日志文件中，系统会自动根据策略ID生成日志文件，这样用户不用自己搭建日志模块，就可以轻松管理好策略的日志。

### 交易数据接口
---
* 获取K线数据
    ```py
    def stra_get_bars(self, code:str, period:str, count:int, isMain:bool = False):
    ```
    参数说明
    ```
    code   合约代码
    period K线周期，如m3/d7
    count  要拉取的K线条数
    isMain 是否是主K线
    返回值  K线矩阵，DataFrame结构，字段包括bartime,open,high,low,close,volumn，索引为时间
    ```
* 获取tick数据
    ```py
    def stra_get_ticks(self, code:str, count:int):
    ```
    参数说明
    ```
    code   合约代码
    count  要拉取的tick数量
    返回值  Tick矩阵，DataFrame结构，字段包括ticktime,open,high,low,price，索引为时间
    ```
* 获取最新价格
    ```py
    def stra_get_price(self, code):
    ```
    参数说明
    ```
    code   合约代码
    返回值  品种最新的价格。回测时为当时的价格
    ```
* 获取持仓部位
    ```py
    def stra_get_position(self, code:str = "", usertag:str = ""):
    ```
    参数说明
    ```
    code       品种代码，不传则自动设置为主K线合约
    usertag    入场标记，默认为空，不为空时只读取对应标记的部位
    返回值      持仓部位，正数则是多头，负数则是空头，0则没有仓位
    ```
* 获取持仓均价
    ```py
    def stra_get_position_avgpx(self, code:str):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    返回值  持仓均价
    ```
* 获取持仓盈亏
    ```py
    def stra_get_position_profit(self, code:str):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    返回值  持仓盈亏
    ```
* 获取最后入场时间
    ```py
    def stra_get_last_entrytime(self, code:str):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    返回值  最后一次开仓的时间，格式如201903121047，如果没有持仓，则返回0xffffffffffffffffULL
    ```
* 获取指定信号的进场价格
    ```py
    def stra_get_detail_cost(self, code:str, usertag:str):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    usertag 用户标记，用于指定入场信号
    返回值  入场价格
    ```
    如果策略内有连续多次开仓的逻辑，且需要知道某个特定进场信号的价格，则可以调用该接口。如果没有连续开仓的逻辑，但是需要知道最后一次进场信号的价格，也可以使用该接口。
* 获取指定信号的进场时间
    ```py
    def stra_get_detail_entertime(self, code:str, usertag:str):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    usertag 用户标记，用于指定入场信号
    返回值  进场时间，格式如yyyyMMddhhmm
    ```
    如果策略内有连续多次开仓的逻辑，且需要知道某个特定进场信号的分钟线时间，则可以调用该接口。如果没有连续开仓的逻辑，但是需要知道最后一次进场信号的分钟线时间，也可以使用该接口。
* 获取指定信号的持仓盈亏
    ```py
    def stra_get_detail_profit(self, code:str, usertag:str, flag:int = 0):
    ```
    参数说明
    ```
    code    品种代码，不传则自动设置为主K线合约
    usertag 用户标记，用于指定入场信号
    flag    盈亏记号，0-浮动盈亏，1-最大浮盈，2-最大亏损（负数）
    返回值  进场时间，格式如yyyyMMddhhmm
    ```
    如果策略内有连续多次开仓的逻辑，且需要知道某个特定进场信号的持仓盈亏，则可以调用该接口。如果没有连续开仓的逻辑，但是需要知道最后一次进场信号的持仓盈亏，也可以使用该接口。
    flag的意义重点在于风控，最大浮盈和最大亏损都是该笔进场的最大潜在盈利和最大潜在亏损，很多策略可以通过该数据确定一些风控的逻辑。

### 信号接口
---
* 多头进场
    ```py
    def stra_enter_long(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
    ```
    参数说明
    ```
    code   品种代码
    qty    数量
    limitprice 限价，默认为0
    stopprice  止价，默认为0
    ```
    进场接口有一个需要注意的地方，就是**当持仓头寸为反向时，进场接口要先平掉反向头寸，再开仓到参数中指定的仓位**，即qty手。这样的好处就是方便用户在开发策略的时候，只需要关心目的，而不需要考虑过程，简化了代码逻辑，还减少了出错的可能。
    limitprice是下单的时候设置一个限价触发条件，而stopprice则是设置一个止价触发条件。当条件满足的时候，才会产生信号。另外，limitprice和stopprice为0时，则不会设置限价单和止价单，但是**如果同时不为0的时候，只有限价单生效**。
* 空头进场
    ```py
    def stra_enter_short(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
    ```
    参数说明
    ```
    code   品种代码
    qty    数量
    limitprice 限价，默认为0
    stopprice  止价，默认为0
    ```
    接口使用同上
* 多头出场
    ```py
    def stra_exit_long(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
    ```
    参数说明
    ```
    code   品种代码
    qty    数量
    limitprice 限价，默认为0
    stopprice  止价，默认为0
    ```
    出场接口用法和入场接口相同。但是出场也有一个需要注意的地方：**如果持仓方向和出场方向不一致，即持有空头，而调用多头出场，系统将不会执行；如果持仓数量不够时，全部出场以后也不会再反向开仓。**
* 空头出场
    ```py
    def stra_exit_short(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
    ```
    参数说明
    ```
    code   品种代码
    qty    数量
    limitprice 限价，默认为0
    stopprice  止价，默认为0
    ```
* 设置目标部位
    ```py
    def stra_set_position(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
    ```
    参数说明
    ```
    code   品种代码
    qty    数量
    limitprice 限价，默认为0
    stopprice  止价，默认为0
    ```
    除了上述几个信号接口，wtpy还提供了一个非常直接的信号接口：即直接设置目标部位。用户可以不用关心多头空头进场出场的细节问题，只需要直接设置目标仓位即可。

### 其他接口
---
* 读取用户数据
    ```py
    def user_load_data(self, key:str, defVal = None, vType = float):
    ```
    参数说明
    ```
    key     用户数据key，用户数据是kv形式存储的，所以读取的时候需要传入key
    defVal  默认值，默认为None，为了防止读取后处理时出现异常，可以传入自定义默认值
    vType   数据类型，存储的时候是以字符串存储的，读取的时候根据自己的需要转为指定类型
    返回值  读取到的用户数据
    ```
* 保存用户数据
    ```py
    def user_save_data(self, key:str, val):
    ```
    参数说明
    ```
    key    用户数据key，用户数据是kv形式存储的，所以读取的时候需要传入key
    val    用户数据，存储时会自动转成字符串
    ```
* 输出指标数据
    ```py
    def write_indicator(self, tag, time, data):
    ```
    参数说明
    ```
    tag    指标标签
    time   输出时间
    data   输出的指标数据，dict类型，会转成json以后保存
    ```
