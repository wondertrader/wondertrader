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


    def on_tick(self, context:Context, code:str, newTick:dict):
        '''
        逐笔数据进来时调用\n
        生产环境中，每笔行情进来就直接调用\n
        回测环境中，是模拟的逐笔数据\n
        @context    策略运行上下文\n
        @code       合约代码
        @newTick    最新逐笔
        '''
        return

    def on_bar(self, context:Context, code:str, period:str, newBar:dict):
        '''
        K线闭合时回调
        @context    策略上下文\n
        @code       合约代码
        @period     K线周期
        @newBar     最新闭合的K线
        '''
        return