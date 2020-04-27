from wtpy.backtest import WtBtWrapper
from wtpy.Context import Context
from wtpy.BaseDefs import BaseStrategy
from wtpy.ExtDefs import BaseIndexWriter

from .ProductMgr import ProductMgr
from .SessionMgr import SessionMgr

import os
import json

def singleton(cls):
    instances = {}
    def getinstance(*args,**kwargs):
        if cls not in instances:
            instances[cls] = cls(*args,**kwargs)
        return instances[cls]
    return getinstance


@singleton
class WtBtEngine:

    def __init__(self):
        self.__wrapper__ = WtBtWrapper()  #api接口转换器
        self.__context__ = None      #策略ctx映射表
        self.__config__ = dict()        #框架配置项
        self.__cfg_commited__ = False   #配置是否已提交

        self.__idx_writer__ = None  #指标输出模块

        self.__wrapper__.initialize(self)   #初始化api接口        

    def __check_config__(self):
        '''
        检查设置项\n
        主要会补充一些默认设置项
        '''
        if "replayer" not in self.__config__:
            self.__config__["replayer"] = dict()
            self.__config__["replayer"]["basefiles"] = dict()

        if "replayer" not in self.__config__:
            self.__config__["replayer"] = dict()
            self.__config__["replayer"]["mode"] = "csv"
            self.__config__["replayer"]["path"] = "./storage/"

        if "env" not in self.__config__:
            self.__config__["env"] = dict()
            self.__config__["env"]["mocker"] = "cta"

    def set_writer(self, writer:BaseIndexWriter):
        '''
        设置指标输出模块
        '''
        self.__writer__ = writer

    def write_indicator(self, id, tag, time, data):
        '''
        写入指标数据
        '''
        if self.__writer__ is not None:
            self.__writer__.write_indicator(id, tag, time, data)

    def init(self, folder:str, cfgfile:str = "configbt.json", commfile:str="commodities.json", contractfile:str="contracts.json"):
        '''
        初始化\n
        @folder     基础数据文件目录，\\结尾\n
        @cfgfile    配置文件，json格式
        '''
        f = open(cfgfile, "r")
        content =f.read()
        self.__config__ = json.loads(content)
        f.close()

        self.__check_config__()

        self.__config__["replayer"]["basefiles"]["commodity"] = folder + commfile
        self.__config__["replayer"]["basefiles"]["contract"] = folder + contractfile
        self.__config__["replayer"]["basefiles"]["holiday"] = folder + "holidays.json"
        self.__config__["replayer"]["basefiles"]["session"] = folder + "sessions.json"
        self.__config__["replayer"]["basefiles"]["hot"] = folder + "hots.json"

        self.productMgr = ProductMgr()
        self.productMgr.load(folder + commfile)

        self.sessionMgr = SessionMgr()
        self.sessionMgr.load(folder + "sessions.json")

    def configMocker(self, name:str):
        '''
        设置模拟器
        '''
        self.__config__["env"]["mocker"] = name

    def configBacktest(self, stime:int, etime:int):
        '''
        配置回测设置项\n
        @stime  开始时间\n
        @etime  结束时间\n
        @period 回测单步周期\n
        @session    回测用的会话ID\n
        @time   周期倍数
        '''
        self.__config__["replayer"]["stime"] = int(stime)
        self.__config__["replayer"]["etime"] = int(etime)

    def configBTStorage(self, mode:str, path:str):
        '''
        配置数据存储\n
        @mode   存储模式，csv-表示从csv直接读取，一般回测使用，wtp-表示使用wt框架自带数据存储
        '''
        self.__config__["replayer"]["mode"] = mode
        self.__config__["replayer"]["path"] = path

    def commitBTConfig(self):
        '''
        提交配置\n
        只有第一次调用会生效，不可重复调用\n
        如果执行run之前没有调用，run会自动调用该方法
        '''
        if self.__cfg_commited__:
            return

        cfgfile = "config_runbt.json"
        f = open(cfgfile, "w")
        f.write(json.dumps(self.__config__, indent=4, sort_keys=True))
        f.close()
        self.__wrapper__.config_backtest(cfgfile)
        #os.remove(cfgfile)
        self.__cfg_commited__ = True

    def getSessionByCode(self, code:str):
        '''
        通过合约代码获取交易时间模板\n
        @code   合约代码，格式如SHFE.rb.HOT
        '''
        pInfo = self.productMgr.getProductInfo(code)
        if pInfo is None:
            return None

        return self.sessionMgr.getSession(pInfo.session)

    def getSessionByName(self, sname:str):
        '''
        通过模板名获取交易时间模板\n
        @sname  模板名
        '''
        return self.sessionMgr.getSession(sname)

    def getProductInfo(self, code:str):
        '''
        获取品种信息\n
        @code   合约代码，格式如SHFE.rb.HOT
        '''
        return self.productMgr.getProductInfo(code)

    def set_strategy(self, strategy:BaseStrategy):
        '''
        添加策略\n
        @strategy   策略对象
        '''
        self.__wrapper__.init_cta_mocker(strategy.name())
        self.__context__ = Context(0, strategy, self.__wrapper__, self)

    def get_context(self):
        return self.__context__

    def run_backtest(self):
        '''
        运行框架
        '''
        if not self.__cfg_commited__:   #如果配置没有提交，则自动提交一下
            self.commitBTConfig()

        self.__wrapper__.run_backtest()

    def release_backtest(self):
        '''
        释放框架
        '''
        self.__wrapper__.release_backtest()

    def dump_kline(self, code:str, period:str, filename:str):
        '''
        将K线导出到文件\n
        @code   合约代码，格式如SHFE.rb.HOT\n
        @period 周期，一般使用d/m1/m5
        '''
        self.__wrapper__.dump_kline(code, period, filename)

    def trans_mc_bars(self, csvFolder:str, binFolder:str, period:str):
        '''
        将MC导出的csv数据文件转换成wt格式\n
        @csvFolder  csv文件目录\n
        @binFolder  wt格式数据文件输出目录\n
        @period     K线周期
        '''
        self.__wrapper__.trans_mc_bars(csvFolder, binFolder, period)