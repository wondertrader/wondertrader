from wtpy.porter import WtWrapper
from wtpy.Context import Context
from wtpy.BaseDefs import BaseStrategy
from wtpy.ExtDefs import BaseIndexWriter
from wtpy.ExtDefs import BaseDataReporter

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
class WtEngine:

    def __init__(self):
        self.__wrapper__ = WtWrapper()  #api接口转换器
        self.__ctx_dict__ = dict()      #策略ctx映射表
        self.__config__ = dict()        #框架配置项
        self.__cfg_commited__ = False   #配置是否已提交

        self.__writer__ = None          #指标输出模块
        self.__reporter__ = None        #数据提交模块

        self.__wrapper__.initialize(self)   #初始化api接口        

    def __check_config__(self):
        '''
        检查设置项\n
        主要会补充一些默认设置项
        '''
        if "basefiles" not in self.__config__:
            self.__config__["basefiles"] = dict()

        if "env" not in self.__config__:
            self.__config__["env"] = dict()
            self.__config__["env"]["name"] = "cta"
            self.__config__["env"]["mode"] = "product"
            self.__config__["env"]["product"] = {
                "session":"TRADING"
            }
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

    def set_data_reporter(self, reporter:BaseDataReporter):
        '''
        设置数据报告器
        '''
        self.__reporter__ = reporter

    def init(self, folder:str, cfgfile:str = "config.json", commfile:str="commodities.json", contractfile:str="contracts.json"):
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

        self.__config__["basefiles"]["commodity"] = folder + commfile
        self.__config__["basefiles"]["contract"] = folder + contractfile
        self.__config__["basefiles"]["holiday"] = folder + "holidays.json"
        self.__config__["basefiles"]["session"] = folder + "sessions.json"
        self.__config__["basefiles"]["hot"] = folder + "hots.json"

        self.productMgr = ProductMgr()
        self.productMgr.load(folder + commfile)

        self.sessionMgr = SessionMgr()
        self.sessionMgr.load(folder + "sessions.json")

    def configEngine(self, name:str, mode:str = "product"):
        '''
        设置引擎和运行模式
        '''
        self.__config__["env"]["name"] = name
        self.__config__["env"]["mode"] = mode

    def addExternalCtaStrategy(self, id:str, params:dict):
        '''
        添加外部的CTA策略
        '''
        if "strategies" not in self.__config__:
            self.__config__["strategies"] = dict()

        if "cta" not in self.__config__["strategies"]:
            self.__config__["strategies"]["cta"] = list()

        params["id"] = id
        self.__config__["strategies"]["cta"].append(params)

    def addExternalHftStrategy(self, id:str, params:dict):
        '''
        添加外部的HFT策略
        '''
        if "strategies" not in self.__config__:
            self.__config__["strategies"] = dict()

        if "hft" not in self.__config__["strategies"]:
            self.__config__["strategies"]["hft"] = list()

        params["id"] = id
        self.__config__["strategies"]["hft"].append(params)

    def configStorage(self, mode:str, path:str):
        '''
        配置数据存储\n
        @mode   存储模式，csv-表示从csv直接读取，一般回测使用，wtp-表示使用wt框架自带数据存储
        '''
        self.__config__["store"]["mode"] = mode
        self.__config__["store"]["path"] = path

    def commitConfig(self):
        '''
        提交配置\n
        只有第一次调用会生效，不可重复调用\n
        如果执行run之前没有调用，run会自动调用该方法
        '''
        if self.__cfg_commited__:
            return

        cfgfile = "config_run.json"
        f = open(cfgfile, "w")
        f.write(json.dumps(self.__config__, indent=4, sort_keys=True))
        f.close()
        self.__wrapper__.config(cfgfile)
        #os.remove(cfgfile)
        self.__cfg_commited__ = True

    def regCtaStraFactories(self, factFolder:str):
        '''
        向底层模块注册CTA工厂模块目录\n
        !!!CTA策略只会被CTA引擎加载!!!\n
        @factFolder 工厂模块所在的目录
        '''
        return self.__wrapper__.reg_cta_factories(factFolder)

    def regHftStraFactories(self, factFolder:str):
        '''
        向底层模块注册HFT工厂模块目录\n
        !!!HFT策略只会被HFT引擎加载!!!\n
        @factFolder 工厂模块所在的目录
        '''
        return self.__wrapper__.reg_hft_factories(factFolder)

    def regExecuterFactories(self, factFolder:str):
        '''
        向底层模块注册执行器模块目录\n
        !!!执行器只在CTA引擎有效!!!\n
        @factFolder 工厂模块所在的目录
        '''
        return self.__wrapper__.reg_exe_factories(factFolder)

    def addExecuter(self, id:str, trader:str, policies:dict, scale:int = 1):
        if "executers" not in self.__config__:
            self.__config__["executers"] = list()

        exeItem = {
            "active":True,
            "id": id,
            "scale": scale,
            "policy": policies,
            "trader":trader
        }

        self.__config__["executers"].append(exeItem)

    def addTrader(self, id:str, params:dict):
        if "traders" not in self.__config__:
            self.__config__["traders"] = list()

        tItem = params
        tItem["active"] = True
        tItem["id"] = id

        self.__config__["traders"].append(tItem)

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

    def add_strategy(self, strategy:BaseStrategy):
        '''
        添加策略\n
        @strategy   策略对象
        '''
        id = self.__wrapper__.create_context(strategy.name())
        self.__ctx_dict__[id] = Context(id, strategy, self.__wrapper__, self)

    def get_context(self, id:int):
        '''
        根据ID获取策略上下文\n
        @id     上下文id，一般添加策略的时候会自动生成一个唯一的上下文id
        '''
        if id not in self.__ctx_dict__:
            return None

        return self.__ctx_dict__[id]

    def run(self):
        '''
        运行框架
        '''
        if not self.__cfg_commited__:   #如果配置没有提交，则自动提交一下
            self.commitConfig()

        self.__wrapper__.run()

    def release(self):
        '''
        释放框架
        '''
        self.__wrapper__.release()

    def set_strategy_position(self, straname:str, positions:list, outdir:str = ""):
        '''
        手动设置策略初始仓位，主要为了防止手动编辑出错\n
        @straname   策略名\n
        @posdetails 持仓明细，list()
        '''
        stradata = {
            "positions": positions
        }

        if outdir[-1] != "\\":
            outdir += "\\"

        if not os.path.exists(outdir):
            os.makedirs(outdir)

        filename = outdir + "%s.json" % (straname)
        f = open(filename, "w")
        f.write(json.dumps(stradata, indent=4))
        f.close()

    def on_init(self):
        if self.__reporter__ is not None:
            self.__reporter__.report_init_data()
        return

    def on_schedule(self, date:int, time:int):
        # print("engine scheduled")
        if self.__reporter__ is not None:
            self.__reporter__.report_rt_data()

    def on_session_begin(self, date:int):
        # print("session begin")
        return

    def on_session_end(self, date:int):
        if self.__reporter__ is not None:
            self.__reporter__.report_settle_data()
        return
