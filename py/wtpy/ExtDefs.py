import json
import time
from threading import Thread

def fileToJson(filename, encoding="utf-8"):
    f = open(filename, 'r')
    content = f.read()
    f.close()
    try:
        return json.loads(content)
    except:
        return None

class BaseIndexWriter:
    '''
    基础指标输出工具
    '''

    def __init__(self):
        return

    def write_indicator(self, id, tag, time, data):
        '''
        将指标数据出\n
        @id     指标ID\n
        @tag    数据标记\n
        @time   指标时间\n
        @data   数据对象，一个dict
        '''
        raise Exception("Basic writer cannot output index data to any media")


class BaseDataReporter:
    '''
    数据报告器
    '''
    TaskReportRTData        = 1
    TaskReportSettleData    = 2
    TaskReportInitData      = 3

    def __init__(self, id:str):
        self.__inited__ = False
        self.__id__ = id
        return

    def init(self):
        self.__inited__ = True
        self.__thrd_task__ = None
        self.__tasks__ = list()
        self.__stopped__ = False

        #读取策略标记
        filename = "./generated/marker.json"
        obj = fileToJson(filename)
        if obj is not None:
            self.stra_names = obj["marks"]

    def rpt_portfolio_rt_data_impl(self, rtData):
        raise Exception("this method has not been implemented")

    def rpt_strategy_rt_data_impl(self, rtData):
        raise Exception("this method has not been implemented")

    def rpt_init_data_impl(self, initData):
        raise Exception("this method has not been implemented")

    def __do_report_rt_data__(self):
        print("settle data reporter triggered")
        # 第一步，提交组合数据，读取portfolio
        filename = "./generated/portfolio/datas.json"
        objPort = fileToJson(filename)
        objPort["pid"] = self.__id__
        # 开始提交组合数据
        self.rpt_portfolio_rt_data_impl(objPort)

        # 第二步，提交策略数据
        for sname in self.stra_names:
            filename = "./generated/stradata/" + sname + ".json"
            objStra = fileToJson(filename)
            objStra["pid"] = self.__id__
            objStra["sid"] = sname

            self.rpt_strategy_rt_data_impl(objStra)

    def __task_loop__(self):
        while not self.__stopped__:
            if len(self.__tasks__) == 0:
                time.sleep(1)
                continue
            else:
                taskid = self.__tasks__.pop(0)                
                if taskid == self.TaskReportRTData:
                    self.__do_report_rt_data__()
                elif taskid == self.TaskReportSettleData:
                    self.__do_report_settle_data__()
                elif taskid == self.TaskReportInitData:
                    self.__do_report_init_data__()

    def __start__(self):
        if self.__thrd_task__ is None:
            self.__thrd_task__ = Thread(target=self.__task_loop__, name="reportthread")
            # self.__thrd_task__.setDaemon(True)
            self.__thrd_task__.start()
            print("report thread started")

    def __do_report_init_data__(self):
        objInitData = dict()
        objInitData["pid"] = self.__id__
        objInitData["strategies"] = self.stra_names
        self.rpt_init_data_impl(objInitData)

    def __do_report_settle_data__(self):
        print("settle data reporter triggered")

    def report_rt_data(self):
        print("rt data reporter triggered")
        self.__tasks__.append(self.TaskReportRTData)
        if self.__thrd_task__ is None:
            self.__start__()

    def report_settle_data(self):
        self.__tasks__.append(self.TaskReportSettleData)
        if self.__thrd_task__ is None:
            self.__start__()

    def report_init_data(self):
        self.__tasks__.append(self.TaskReportInitData)
        if self.__thrd_task__ is None:
            self.__start__()
        