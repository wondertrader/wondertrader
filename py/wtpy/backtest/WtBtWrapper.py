from ctypes import cdll, c_int, c_char_p, c_longlong, c_bool, c_void_p, c_ulong, c_uint, c_uint64, c_double
from wtpy.common.WtDefine import CB_STRATEGY_INIT, CB_STRATEGY_TICK, CB_STRATEGY_CALC, CB_STRATEGY_BAR, CB_STRATEGY_GET_BAR, CB_STRATEGY_GET_TICK
import platform
import os
import sys

theEngine = None

def isPythonX64():
    ret = platform.architecture()
    return (ret[0] == "64bit")

def isWindows():
    if "windows" in platform.system().lower():
        return True

    return False

#回调函数
def on_strategy_init(id):
    engine = theEngine
    ctx = engine.get_context()
    if ctx is not None:
        ctx.on_init()
    return

def on_strategy_tick(id, code, newTick):
    engine = theEngine
    ctx = engine.get_context()

    realTick = newTick.contents
    tick = dict()
    tick["time"] = realTick.action_date * 1000000000 + realTick.action_time
    tick["open"] = realTick.open
    tick["high"] = realTick.high
    tick["low"] = realTick.low
    tick["price"] = realTick.price

    if ctx is not None:
        ctx.on_tick(code, tick)
    return

def on_strategy_calc(id):
    engine = theEngine
    ctx = engine.get_context()
    if ctx is not None:
        ctx.on_calculate()
    return

def on_strategy_bar(id, code, period, newBar):
    period = bytes.decode(period)
    engine = theEngine
    ctx = engine.get_context()
    newBar = newBar.contents
    curBar = dict()
    if period[0] == 'd':
        curBar["time"] = newBar.date
    else:
        curBar["time"] = 1990*100000000 + newBar.time
    curBar["bartime"] = curBar["time"]
    curBar["open"] = newBar.open
    curBar["high"] = newBar.high
    curBar["low"] = newBar.low
    curBar["close"] = newBar.close
    curBar["volumn"] = newBar.vol
    if ctx is not None:
        ctx.on_bar(bytes.decode(code), period, curBar)
    return


def on_strategy_get_bar(id, code, period, curBar, isLast):
    '''
    获取K线回调，该回调函数因为是python主动发起的，需要同步执行，所以不走事件推送\n
    @id     策略id\n
    @code   合约代码\n
    @period K线周期\n
    @curBar 最新一条K线\n
    @isLast 是否是最后一条
    '''
    engine = theEngine
    ctx = engine.get_context()
    realBar = None
    if curBar:
        realBar = curBar.contents

    period = bytes.decode(period)

    bar = None
    if realBar is not None:
        bar = dict()
        if period[0] == 'd':
            bar["time"] = realBar.date
        else:
            bar["time"] = 1990*100000000 + realBar.time
        bar["open"] = realBar.open
        bar["high"] = realBar.high
        bar["low"] = realBar.low
        bar["close"] = realBar.close
        bar["volumn"] = realBar.vol

    if ctx is not None:
        ctx.on_getbars(bytes.decode(code), period, bar, isLast)
    return

def on_strategy_get_tick(id, code, curTick, isLast):
    '''
    获取Tick回调，该回调函数因为是python主动发起的，需要同步执行，所以不走事件推送\n
    @id         策略id\n
    @code       合约代码\n
    @curTick    最新一笔Tick\n
    @isLast     是否是最后一条
    '''

    engine = theEngine
    ctx = engine.get_context()
    realTick = None
    if curTick:
        realTick = curTick.contents

    tick = None
    if realTick is not None:
        tick = dict()
        tick["time"] = realTick.action_date * 1000000000 + realTick.action_time
        tick["open"] = realTick.open
        tick["high"] = realTick.high
        tick["low"] = realTick.low
        tick["price"] = realTick.price

    if ctx is not None:
        ctx.on_getticks(bytes.decode(code), tick, isLast)
    return

'''
将回调函数转换成C接口识别的函数类型
''' 
cb_strategy_init = CB_STRATEGY_INIT(on_strategy_init)
cb_strategy_tick = CB_STRATEGY_TICK(on_strategy_tick)
cb_strategy_calc = CB_STRATEGY_CALC(on_strategy_calc)
cb_strategy_bar = CB_STRATEGY_BAR(on_strategy_bar)
cb_strategy_get_bar = CB_STRATEGY_GET_BAR(on_strategy_get_bar)
cb_strategy_get_tick = CB_STRATEGY_GET_TICK(on_strategy_get_tick)

# Python对接C接口的库
class WtBtWrapper:
    '''
    Wt平台C接口底层对接模块
    '''

    # api可以作为公共变量
    api = None
    ver = "Unknown"
    
    # 构造函数，传入动态库名
    def __init__(self):
        paths = os.path.split(__file__)
        if isWindows(): #windows平台
            if isPythonX64():
                dllname = "x64/WtBtPorter.dll"
                a = (paths[:-1] + (dllname,))
                _path = os.path.join(*a)
                self.api = cdll.LoadLibrary(_path)
            else:
                dllname = "x86/WtBtPorter.dll"
                a = (paths[:-1] + (dllname,))
                _path = os.path.join(*a)
                self.api = cdll.LoadLibrary(_path)
        else:#Linux平台
            dllname = "linux/libWtBtPorter.so"
            a = (paths[:-1] + (dllname,))
            _path = os.path.join(*a)
            print(_path)
            self.api = cdll.LoadLibrary(_path)
            print(self.api)
        self.api.get_version.restype = c_char_p
        self.api.ctx_str_get_last_entertime.restype = c_uint64
        self.api.ctx_str_get_first_entertime.restype = c_uint64
        self.api.ctx_str_get_detail_entertime.restype = c_uint64
        self.api.ctx_str_enter_long.argtypes = [c_ulong, c_char_p, c_double, c_char_p, c_double, c_double]
        self.api.ctx_str_enter_short.argtypes = [c_ulong, c_char_p, c_double, c_char_p, c_double, c_double]
        self.api.ctx_str_exit_long.argtypes = [c_ulong, c_char_p, c_double, c_char_p, c_double, c_double]
        self.api.ctx_str_exit_short.argtypes = [c_ulong, c_char_p, c_double, c_char_p, c_double, c_double]
        self.api.ctx_str_set_position.argtypes = [c_ulong, c_char_p, c_double, c_char_p, c_double, c_double]
        self.ver = bytes.decode(self.api.get_version())

        self.api.ctx_str_save_userdata.argtypes = [c_ulong, c_char_p, c_char_p]
        self.api.ctx_str_load_userdata.argtypes = [c_ulong, c_char_p, c_char_p]
        self.api.ctx_str_load_userdata.restype = c_char_p

        self.api.ctx_str_get_position.restype = c_double
        self.api.ctx_str_get_position_profit.restype = c_double
        self.api.ctx_str_get_position_avgpx.restype = c_double
        self.api.ctx_str_get_detail_cost.restype = c_double
        self.api.ctx_str_get_detail_profit.restype = c_double

    def run_backtest(self):
        self.api.run_backtest()

    def release_backtest(self):
        self.api.release_backtest()

    def write_log(self, level, message:str, catName:str = ""):
        self.api.write_log(level, bytes(message, encoding = "utf8").decode('utf-8').encode('gbk'), bytes(catName, encoding = "utf8"))

    def config_backtest(self, cfgfile:str = 'config.json'):
        self.api.config_backtest(bytes(cfgfile, encoding = "utf8"))

    def initialize(self, engine, logProfile:str = "logcfgbt.json"):
        '''
        C接口初始化
        '''
        global theEngine
        theEngine = engine
        try:
            self.api.register_callbacks(cb_strategy_init, cb_strategy_tick, cb_strategy_calc, cb_strategy_bar)
            self.api.init_backtest(bytes(logProfile, encoding = "utf8"))
        except OSError as oe:
            print(oe)

        self.write_log(102, "Wt回测框架已初始化完成，框架版本号：%s" % (self.ver))

    def dump_kline(self, stdCode:str, period:str, filename:str):
        self.api.dump_bars(bytes(stdCode, encoding = "utf8"), bytes(period, encoding = "utf8"), bytes(filename, encoding = "utf8"))

    def trans_mc_bars(self, csvFolder:str, binFolder:str, period:str):
        self.api.trans_mc_bars(bytes(csvFolder, encoding = "utf8"),bytes(binFolder, encoding = "utf8"),bytes(period, encoding = "utf8"))

    def init_cta_mocker(self, name:str):
        '''
        创建策略环境\n
        @name      策略名称
        @return    系统内策略ID 
        '''
        return self.api.init_cta_mocker(bytes(name, encoding = "utf8") )

    def ctx_str_enter_long(self, id:int, code:str, qty:float, usertag:str, limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        开多\n
        @id     策略id\n
        @code   合约代码\n
        @qty    手数，大于等于0\n
        '''
        self.api.ctx_str_enter_long(id, bytes(code, encoding = "utf8"), qty, bytes(usertag, encoding = "utf8"), limitprice, stopprice)

    def ctx_str_exit_long(self, id:int, code:str, qty:float, usertag:str, limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        平多\n
        @id     策略id\n
        @code   合约代码\n
        @qty    手数，大于等于0\n
        '''
        self.api.ctx_str_exit_long(id, bytes(code, encoding = "utf8"), qty, bytes(usertag, encoding = "utf8"), limitprice, stopprice)

    def ctx_str_enter_short(self, id:int, code:str, qty:float, usertag:str, limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        开空\n
        @id     策略id\n
        @code   合约代码\n
        @qty    手数，大于等于0\n
        '''
        self.api.ctx_str_enter_short(id, bytes(code, encoding = "utf8"), qty, bytes(usertag, encoding = "utf8"), limitprice, stopprice)

    def ctx_str_exit_short(self, id:int, code:str, qty:float, usertag:str, limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        平空\n
        @id     策略id\n
        @code   合约代码\n
        @qty    手数，大于等于0\n
        '''
        self.api.ctx_str_exit_short(id, bytes(code, encoding = "utf8"), qty, bytes(usertag, encoding = "utf8"), limitprice, stopprice)

    def ctx_str_get_bars(self, id:int, code:str, period:str, count:int, isMain:bool):
        '''
        读取K线\n
        @id     策略id\n
        @code   合约代码\n
        @period 周期，如m1/m3/d1等\n
        @count  条数\n
        @isMain 是否主K线
        '''
        return self.api.ctx_str_get_bars(id, bytes(code, encoding = "utf8"), bytes(period, encoding = "utf8"), count, isMain, cb_strategy_get_bar)

    def ctx_str_get_ticks(self, id:int, code:str, count:int):
        '''
        读取Tick\n
        @id     策略id\n
        @code   合约代码\n
        @count  条数\n
        '''
        return self.api.ctx_str_get_ticks(id, bytes(code, encoding = "utf8"), count, cb_strategy_get_tick)

    def ctx_str_get_position_profit(self, id:int, code:str):
        '''
        获取浮动盈亏\n
        @id     策略id\n
        @code   合约代码\n
        @return 指定合约的浮动盈亏
        '''
        return self.api.ctx_str_get_position_profit(id, bytes(code, encoding = "utf8"))

    def ctx_str_get_position_avgpx(self, id:int, code:str):
        '''
        获取持仓均价\n
        @id     策略id\n
        @code   合约代码\n
        @return 指定合约的持仓均价
        '''
        return self.api.ctx_str_get_position_avgpx(id, bytes(code, encoding = "utf8"))
    
    def ctx_str_get_position(self, id:int, code:str, usertag:str = ""):
        '''
        获取持仓\n
        @id     策略id\n
        @code   合约代码\n
        @usertag    进场标记，如果为空则获取该合约全部持仓\n
        @return 指定合约的持仓手数，正为多，负为空
        '''
        return self.api.ctx_str_get_position(id, bytes(code, encoding = "utf8"), bytes(usertag, encoding = "utf8"))

    def ctx_str_get_price(self, code:str):
        '''
        @code   合约代码\n
        @return 指定合约的最新价格 
        '''
        return self.api.ctx_str_get_price(bytes(code, encoding = "utf8"))

    def ctx_str_set_position(self, id:int, code:str, qty:float, usertag:str = "", limitprice:float = 0.0, stopprice:float = 0.0):
        '''
        设置目标仓位\n
        @id     策略id
        @code   合约代码\n
        @qty    目标仓位，正为多，负为空
        '''
        self.api.ctx_str_set_position(id, bytes(code, encoding = "utf8"), qty, bytes(usertag, encoding = "utf8"), limitprice, stopprice)

    def ctx_str_get_date(self):
        '''
        获取当前日期\n
        @return    当前日期 
        '''
        return self.api.ctx_str_get_date()

    def ctx_str_get_time(self):
        '''
        获取当前时间\n
        @return    当前时间 
        '''
        return self.api.ctx_str_get_time()

    def ctx_str_get_first_entertime(self, id:int, code:str):
        '''
        获取当前持仓的首次进场时间\n
        @code       合约代码\n
        @return     进场时间，格式如201907260932 
        '''
        return self.api.ctx_str_get_first_entertime(id, bytes(code, encoding = "utf8"))

    def ctx_str_get_last_entertime(self, id:int, code:str):
        '''
        获取当前持仓的最后进场时间\n
        @code       合约代码\n
        @return     进场时间，格式如201907260932 
        '''
        return self.api.ctx_str_get_last_entertime(id, bytes(code, encoding = "utf8"))

    def ctx_str_log_text(self, id:int, message:str):
        '''
        日志输出\n
        @id         策略ID\n
        @message    日志内容
        '''
        self.api.ctx_str_log_text(id, bytes(message, encoding = "utf8").decode('utf-8').encode('gbk'))

    def ctx_str_get_detail_entertime(self, id:int, code:str, usertag:str):
        '''
        获取指定标记的持仓的进场时间\n
        @id         策略id\n
        @code       合约代码\n
        @usertag    进场标记\n
        @return     进场时间，格式如201907260932 
        '''
        return self.api.ctx_str_get_detail_entertime(id, bytes(code, encoding = "utf8"), bytes(usertag, encoding = "utf8")) 

    def ctx_str_get_detail_cost(self, id:int, code:str, usertag:str):
        '''
        获取指定标记的持仓的开仓价\n
        @id         策略id\n
        @code       合约代码\n
        @usertag    进场标记\n
        @return     开仓价 
        '''
        return self.api.ctx_str_get_detail_cost(id, bytes(code, encoding = "utf8"), bytes(usertag, encoding = "utf8")) 

    def ctx_str_get_detail_profit(self, id:int, code:str, usertag:str, flag:int):
        '''
        获取指定标记的持仓的盈亏\n
        @id         策略id\n
        @code       合约代码\n
        @usertag    进场标记\n
        @flag       盈亏记号，0-浮动盈亏，1-最大浮盈，2-最大亏损（负数）
        @return     盈亏 
        '''
        return self.api.ctx_str_get_detail_profit(id, bytes(code, encoding = "utf8"), bytes(usertag, encoding = "utf8"), flag) 

    def ctx_str_save_user_data(self, id:int, key:str, val:str):
        '''
        保存用户数据\n
        @id         策略id\n
        @key        数据名
        @val        数据值
        '''
        self.api.ctx_str_save_userdata(id, bytes(key, encoding = "utf8"), bytes(val, encoding = "utf8"))

    def ctx_str_load_user_data(self, id:int, key:str, defVal:str  = ""):
        '''
        加载用户数据\n
        @id         策略id\n
        @key        数据名
        @defVal     默认值
        '''
        ret = self.api.ctx_str_load_userdata(id, bytes(key, encoding = "utf8"), bytes(defVal, encoding = "utf8"))
        return bytes.decode(ret)
  
