from pandas import DataFrame as df
import pandas as pd
import os
import json

from wtpy.porter import WtWrapper

class Context:
    '''
    Context是策略可以直接访问的唯一对象\n
    策略所有的接口都通过Context对象调用\n
    Context类包括以下几类接口：\n
    1、时间接口（日期、时间等），接口格式如：stra_xxx\n
    2、数据接口（K线、财务等），接口格式如：stra_xxx\n
    3、下单接口（设置目标仓位、直接下单等），接口格式如：stra_xxx\n
    '''

    def __init__(self, id, stra, wrapper: WtWrapper, engine):
        self.__stra_info__ = stra   #策略对象，对象基类BaseStrategy.py
        self.__wrapper__ = wrapper  #底层接口转换器
        self.__id__ = id            #策略ID
        self.__bar_cache__ = dict() #K线缓存
        self.__tick_cache__ = dict()    #tTick缓存，每次都重新去拉取，这个只做中转用，不在python里维护副本
        self.__sname__ = stra.name()    
        self.__engine__ = engine          #交易环境

    def write_indicator(self, tag, time, data):
        '''
        输出指标数据
        @tag    指标标签
        @time   输出时间
        @data   输出的指标数据，dict类型，会转成json以后保存
        '''
        self.__engine__.write_indicator(self.__stra_info__.name(), tag, time, data)

    def on_init(self):
        '''
        初始化，一般用于系统启动的时候
        '''
        self.__stra_info__.on_init(self)

    def on_getticks(self, code:str, curTick:dict, isLast:bool):
        key = code
        if key not in self.__tick_cache__:
            self.__tick_cache__[key] = list()
        elif not isinstance(self.__tick_cache__[key], list):
            self.__tick_cache__[key] = list()

        ticks = self.__tick_cache__[key]
            
        if curTick is not None:          
            ticks.append(curTick)

        if isLast:
            localTicks = df(ticks)
            tickTime = localTicks["time"]
            localTicks.insert(0,'ticktime', tickTime)
            localTicks = localTicks.set_index("time") 
            self.__tick_cache__[key] = localTicks

    def on_getbars(self, code:str, period:str, curBar:dict, isLast:bool):
        key = "%s#%s" % (code, period)
        if key not in self.__bar_cache__:
            self.__bar_cache__[key] = list()
        elif not isinstance(self.__bar_cache__[key], list):
            self.__bar_cache__[key] = list()

        bars = self.__bar_cache__[key]
            
        if curBar is not None:          
            bars.append(curBar)

        if isLast:
            localBars = df(bars)
            barTime = localBars["time"]
            localBars.insert(0,'bartime', barTime)
            localBars = localBars.set_index("time") 
            self.__bar_cache__[key] = localBars


    def on_tick(self, code:str, newTick):
        self.__stra_info__.on_tick(self, code, newTick)


    def on_bar(self, code:str, period:str, newBar:dict):
        '''
        K线闭合事件响应
        @code   品种代码
        @period K线基础周期
        @times  周期倍数
        @newBar 最新K线
        '''        
        key = "%s#%s" % (code, period)

        if key not in self.__bar_cache__:
            return

        try:
            self.__bar_cache__[key].loc[newBar["bartime"]] = pd.Series(newBar)
            self.__bar_cache__[key].closed = True
            self.__stra_info__.on_bar(self, code, period, newBar)
        except ValueError as ve:
            print(ve)
        else:
            return


    def on_calculate(self):
        self.__stra_info__.on_calculate(self)

    def stra_log_text(self, message:str):
        '''
        输出日志
        @message    消息内容\n
        '''
        self.__wrapper__.ctx_str_log_text(self.__id__, message)
        
    def stra_get_date(self):
        '''
        获取当前日期\n
        @return int，格式如20180513
        '''
        return self.__wrapper__.ctx_str_get_date()

    def stra_get_position_avgpx(self, code:str = ""):
        '''
        获取当前持仓均价\n
        @code   合约代码
        @return 持仓均价
        '''
        return self.__wrapper__.ctx_str_get_position_avgpx(self.__id__, code)

    def stra_get_position_profit(self, code:str = ""):
        '''
        获取持仓浮动盈亏
        @code   合约代码，为None时读取全部品种的浮动盈亏
        @return 浮动盈亏
        '''
        return self.__wrapper__.ctx_str_get_position_profit(self.__id__, code)

    def stra_get_time(self):
        '''
        获取当前时间，24小时制，精确到分\n
        @return int，格式如1231
        '''
        return self.__wrapper__.ctx_str_get_time()

    def stra_get_price(self, code):
        '''
        获取最新价格，一般在获取了K线以后再获取该价格
        @return 最新价格
        '''
        return self.__wrapper__.ctx_str_get_price(code)

    def stra_get_bars(self, code:str, period:str, count:int, isMain:bool = False):
        '''
        获取历史K线
        @code   合约代码
        @period K线周期，如m3/d7
        @count  要拉取的K线条数
        @isMain 是否是主K线
        '''
        key = "%s#%s" % (code, period)

        if key in self.__bar_cache__:
            #这里做一个数据长度处理
            return self.__bar_cache__[key].iloc[-count:]

        cnt =  self.__wrapper__.ctx_str_get_bars(self.__id__, code, period, count, isMain)
        if cnt == 0:
            return None

        df_bars = self.__bar_cache__[key]
        df_bars.closed = False

        return df_bars

    def stra_get_ticks(self, code:str, count:int):
        '''
        获取tick数据
        @code   合约代码
        @count  要拉取的tick数量
        '''
        cnt = self.__wrapper__.ctx_str_get_ticks(self.__id__, code, count)
        if cnt == 0:
            return None
        
        df_ticks = self.__tick_cache__[code]
        return df_ticks

    def stra_get_position(self, code:str = "", usertag:str = ""):
        '''
        读取当前仓位\n
        @code       合约/股票代码\n
        @usertag    入场标记
        @return     正为多仓，负为空仓
        '''
        return self.__wrapper__.ctx_str_get_position(self.__id__, code, usertag)

    def stra_set_position(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        设置仓位\n
        @code   合约/股票代码\n
        @qty    目标仓位，正为多仓，负为空仓\n
        @return 设置结果TRUE/FALSE
        '''
        self.__wrapper__.ctx_str_set_position(self.__id__, code, qty, usertag, limitprice, stopprice)
        

    def stra_enter_long(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        多仓进场，如果有空仓，则平空再开多\n
        @code   品种代码\n
        @qty    数量\n
        @limitprice 限价，默认为0\n
        @stopprice  止价，默认为0
        '''
        self.__wrapper__.ctx_str_enter_long(self.__id__, code, qty, usertag, limitprice, stopprice)

    def stra_exit_long(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        多仓出场，如果剩余多仓不够，则全部平掉即可\n
        @code   品种代码\n
        @qty    数量\n
        @limitprice 限价，默认为0\n
        @stopprice  止价，默认为0
        '''
        self.__wrapper__.ctx_str_exit_long(self.__id__, code, qty, usertag, limitprice, stopprice)

    def stra_enter_short(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        空仓进场，如果有多仓，则平多再开空\n
        @code   品种代码\n
        @qty    数量\n
        @limitprice 限价，默认为0\n
        @stopprice  止价，默认为0
        '''
        self.__wrapper__.ctx_str_enter_short(self.__id__, code, qty, usertag, limitprice, stopprice)

    def stra_exit_short(self, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        空仓出场，如果剩余空仓不够，则全部平掉即可\n
        @code   品种代码\n
        @qty    数量\n
        @limitprice 限价，默认为0\n
        @stopprice  止价，默认为0
        '''
        self.__wrapper__.ctx_str_exit_short(self.__id__, code, qty, usertag, limitprice, stopprice)

    def stra_get_last_entrytime(self, code:str):
        '''
        获取当前持仓最后一次进场时间\n
        @code   品种代码\n
        @return 返回最后一次开仓的时间，格式如201903121047
        '''
        return self.__wrapper__.ctx_str_get_last_entertime(self.__id__, code)

    def stra_get_first_entrytime(self, code:str):
        '''
        获取当前持仓第一次进场时间\n
        @code   品种代码\n
        @return 返回最后一次开仓的时间，格式如201903121047
        '''
        return self.__wrapper__.ctx_str_get_first_entertime(self.__id__, code)


    def user_save_data(self, key:str, val):
        '''
        保存用户数据
        @key    数据id
        @val    数据值，可以直接转换成str的数据均可
        '''
        self.__wrapper__.ctx_str_save_user_data(self.__id__, key, str(val))

    def user_load_data(self, key:str, defVal = None, vType = float):
        '''
        读取用户数据
        @key    数据id
        @defVal 默认数据，如果找不到则返回改数据，默认为None
        @return 返回值，默认处理为float数据
        '''
        ret = self.__wrapper__.ctx_str_load_user_data(self.__id__, key, "")
        if ret == "":
            return defVal

        return vType(ret)

    def stra_get_detail_profit(self, code:str, usertag:str, flag:int = 0):
        '''
        获取指定标记的持仓的盈亏
        @code       合约代码\n
        @usertag    进场标记\n
        @flag       盈亏记号，0-浮动盈亏，1-最大浮盈，2-最大亏损（负数）
        @return     盈亏 
        '''
        return self.__wrapper__.ctx_str_get_detail_profit(self.__id__, code, usertag, flag)

    def stra_get_detail_cost(self, code:str, usertag:str):
        '''
        获取指定标记的持仓的开仓价
        @code       合约代码\n
        @usertag    进场标记\n
        @return     开仓价 
        '''
        return self.__wrapper__.ctx_str_get_detail_cost(self.__id__, code, usertag)

    def stra_get_detail_entertime(self, code:str, usertag:str):
        '''
        获取指定标记的持仓的进场时间\n
        @code       合约代码\n
        @usertag    进场标记\n
        @return     进场时间，格式如201907260932 
        '''
        return self.__wrapper__.ctx_str_get_detail_entertime(self.__id__, code, usertag)

    def stra_get_comminfo(self, code:str):
        '''
        获取品种详情\n
        @code   合约代码如SHFE.ag.HOT，或者品种代码如SHFE.ag\n
        @return 品种信息，结构请参考ProductMgr中的ProductInfo
        '''
        if self.__engine__ is None:
            return None
        return self.__engine__.getProductInfo(code)