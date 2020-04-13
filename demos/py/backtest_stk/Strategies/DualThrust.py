from wtpy import BaseStrategy


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

    def on_init(self, context):
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

    
    def on_calculate(self, context):
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


    def on_tick(self, context, stdCode, newTick):
        #context.stra_log_text ("on tick fired")
        return