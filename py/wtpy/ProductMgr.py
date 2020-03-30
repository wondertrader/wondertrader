import json

class ProductInfo:
    '''
    品种信息
    '''

    def __init__(self):
        self.exchg = ''     #交易所
        self.product = ''   #品种代码
        self.name = ''      #品种名称
        self.session = ''   #交易时段名
        self.covermode = 0  #平仓模式
        self.pricemode = 0  #价格模式
        self.precision = 0  #精度
        self.pricetick = 0  #价格变动单位
        self.volscale = 1   #数量乘数

class ProductMgr:
    '''
    品种信息管理器
    '''
    def __init__(self):
        self.__products__ = dict()
        return

    def load(self, fname:str):
        '''
        从文件加载品种信息
        '''
        f = open(fname, 'r', encoding="gbk")
        content = f.read()
        f.close()

        exchgMap = json.loads(content)
        for exchg in exchgMap:
            exchgObj = exchgMap[exchg]
            for pid in exchgObj:
                pObj = exchgObj[pid]
                pInfo = ProductInfo()
                pInfo.exchg = exchg
                pInfo.product = pid
                pInfo.name = pObj["name"]
                pInfo.session = pObj["session"]
                pInfo.covermode = int(pObj["covermode"])
                pInfo.pricemode = int(pObj["pricemode"])
                pInfo.precision = int(pObj["precision"])
                pInfo.volscale = int(pObj["volscale"])
                pInfo.pricetick = float(pObj["pricetick"])

                key = "%s.%s" % (exchg, pid)
                self.__products__[key] = pInfo


    def getProductInfo(self, pid:str):
        #pid形式可能为SHFE.ag.HOT，或者SHFE.ag.1912，或者SHFE.ag
        items = pid.split(".")
        key = items[0] + "." + items[1]
        if key not in self.__products__:
            return None

        return self.__products__[key]