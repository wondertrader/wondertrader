import json
import time
from threading import Thread

import urllib.request
import io
import gzip

def fileToJson(filename, encoding="utf-8"):
    f = open(filename, 'r')
    content = f.read()
    f.close()
    try:
        return json.loads(content)
    except:
        return None

def httpPost(url, datas, encoding='utf-8'):
    headers = {
        'User-Agent': 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)',
        'Accept-encoding': 'gzip'
    }
    datas = bytes(datas,'utf-8')
    request = urllib.request.Request(url, datas, headers)
    try:
        f = urllib.request.urlopen(request)
        ec = f.headers.get('Content-Encoding')
        if ec == 'gzip':
            cd = f.read()
            cs = io.BytesIO(cd)
            f = gzip.GzipFile(fileobj=cs)

        return f.read().decode(encoding)
    except:
        return ""

TaskReportRTData        = 1
TaskReportSettleData    = 2
TaskReportInitData      = 3

class DataReporter:
    '''
    数据报告器
    '''

    def __init__(self, id:str):
        self.__inited__ = False
        self.__id__ = id
        return

    def init(self, url:str):
        self.__url__ = url
        self.__inited__ = True
        self.__thrd_task__ = None
        self.__tasks__ = list()
        self.__stopped__ = False

        #读取策略标记
        filename = "./generated/marker.json"
        obj = fileToJson(filename)
        if obj is not None:
            self.stra_names = obj["marks"]


    def __do_report_rt_data__(self):
        print("settle data reporter triggered")
        # 第一步，提交组合数据，读取portfolio
        filename = "./generated/portfolio/datas.json"
        objPort = fileToJson(filename)
        # 开始提交组合数据
        objPort["pid"] = self.__id__
        url = self.__url__ + "/rpt/portrtdata"
        httpPost(url, json.dumps(objPort))

        # 第二步，提交策略数据
        for sname in self.stra_names:
            filename = "./generated/stradata/" + sname + ".json"
            objStra = fileToJson(filename)
            objStra["pid"] = self.__id__
            objStra["sid"] = sname
            # print(objStra)
            #开始提交策略数据
            url = self.__url__ + "/rpt/strartdata"
            # print(url)
            httpPost(url, json.dumps(objStra))

    def __task_loop__(self):
        while not self.__stopped__:
            if len(self.__tasks__) == 0:
                time.sleep(1)
                continue
            else:
                taskid = self.__tasks__.pop(0)                
                if taskid == TaskReportRTData:
                    self.__do_report_rt_data__()
                elif taskid == TaskReportSettleData:
                    self.__do_report_settle_data__()
                elif taskid == TaskReportInitData:
                    self.__do_report_init_data__()

    def __start__(self):
        if self.__thrd_task__ is None:
            self.__thrd_task__ = Thread(target=self.__task_loop__, name="reportthread")
            # self.__thrd_task__.setDaemon(True)
            self.__thrd_task__.start()
            print("report thread started")

    def report_rt_data(self):
        print("rt data reporter triggered")
        self.__tasks__.append(TaskReportRTData)
        if self.__thrd_task__ is None:
            self.__start__()

    def __do_report_init_data__(self):
        objInitData = dict()
        objInitData["pid"] = self.__id__
        objInitData["strategies"] = self.stra_names
        url = self.__url__ + "/rpt/portinitdata"
        # print(url)
        httpPost(url, json.dumps(objInitData))

    def __do_report_settle_data__(self):
        print("settle data reporter triggered")
        # 第一步，提交组合数据，读取portfolio
        # 第二步，提交策略数据
        # 第三步，提交交易数据

    def report_settle_data(self):
        self.__tasks__.append(TaskReportSettleData)
        if self.__thrd_task__ is None:
            self.__start__()

    def report_init_data(self):
        self.__tasks__.append(TaskReportInitData)
        if self.__thrd_task__ is None:
            self.__start__()
        