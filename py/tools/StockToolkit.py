import tushare as ts
import json
import os
import datetime

import urllib.request
import io
import gzip

import struct
import time
import re

def create_dirs(path):
    lst = path.replace("/","\\").split("\\")
    dir = lst[0] + "/"
    if not os.path.exists(dir):
        os.mkdir(dir)

    for idx in range(1,len(lst)):
        dir += lst[idx] + "//"
        if not os.path.exists(dir):
            os.mkdir(dir)
    
    
def httpGet(url, encoding='utf-8'):
    request = urllib.request.Request(url)
    request.add_header('Accept-encoding', 'gzip')
    request.add_header(
        'User-Agent', 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)')
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
        
class StockToolkit:

    def __init__(self, tushare_toke:str):
        self.__pro__ = ts.pro_api(tushare_toke)
        self.__stocks__ = dict()
        self.__indice__ = dict()


    def dmpStksFromTS(self, filename:str = "", hasIndice:bool = True):
        stocks = {
            "SSE":{},
            "SZSE":{}
        }

        #个股列表
        df_stocks = self.__pro__.stock_basic(exchange='', list_status='L', fields='ts_code,symbol,name,area,industry,list_date')
        for idx, row in df_stocks.iterrows():
            code = row["ts_code"]
            rawcode = row["symbol"]
            sInfo = dict()
            pid = "STK"
            if code[-2:] == "SH":
                sInfo["exchg"] = "SSE"
            else:
                sInfo["exchg"] = "SZSE"
            code = rawcode #code[-2:] + rawcode
            sInfo["code"] = code
            sInfo["name"] = row["name"]
            sInfo["indust"] = row["industry"]
            sInfo["area"] = row["area"]
            sInfo["product"] = pid            
            
            stocks[sInfo["exchg"]][code] = sInfo

            self.__stocks__[code] = sInfo

        if hasIndice:
            #上证指数列表
            df_stocks = self.__pro__.index_basic(market='SSE')
            for idx, row in df_stocks.iterrows():
                code = row["ts_code"]
                rawcode = code[:6]
                sInfo = dict()
                sInfo["exchg"] = "SSE"
                code = rawcode #"SH" + rawcode
                sInfo["code"] = code
                sInfo["name"] = row["name"]
                sInfo["product"] = "IDX"            
                
                stocks[sInfo["exchg"]][code] = sInfo
                self.__indice__[code] = sInfo

            #深证指数列表
            df_stocks = self.__pro__.index_basic(market='SZSE')
            for idx, row in df_stocks.iterrows():
                code = row["ts_code"]
                rawcode = code[:6]
                sInfo = dict()
                sInfo["exchg"] = "SZSE"
                code = rawcode  #"SZ" + rawcode
                sInfo["code"] = code
                sInfo["name"] = row["name"]
                sInfo["product"] = "IDX"            
                
                stocks[sInfo["exchg"]][code] = sInfo
                self.__indice__[code] = sInfo

        if len(filename) > 0:
            f = open(filename, 'w')
            f.write(json.dumps(stocks, sort_keys=True, indent=4, ensure_ascii=False))
            f.close()

    def dmpAdjFromSINA(self, filename:str):
        #https://finance.sina.com.cn/realstock/company/sh600000/qfq.js
        total = len(self.__stocks__.keys())
        if total == 0:
            raise Exception("stock list has not been initialized")

        stocks = {
            "SSE":{},
            "SZSE":{}
        }

        count = 0
        for code in self.__stocks__:
            count += 1
            url = "https://finance.sina.com.cn/realstock/company/%s/qfq.js" % (code)
            print("正在拉取%s[%d/%d]的除权因子……" % (code, count, total))
            cInfo = self.__stocks__[code]
            exchg = cInfo["exchg"]
            content = httpGet(url.lower())
            if len(content) == 0:
                continue

            if code[2:] not in stocks[exchg]:
                stocks[exchg][code[2:]] = list()

            content = content[16:]
            content = content.split("\n")[0]
            resObj = json.loads(content)
            for item in resObj["data"]:
                date = int(item["d"].replace("-",""))
                factor = float(item["f"])
                stocks[exchg][code[2:]].append({
                    "date": date,
                    "factor": factor
                })

        f = open(filename, 'w+')
        f.write(json.dumps(stocks, sort_keys=True, indent=4, ensure_ascii=False))
        f.close()

    def dmpStkDaysFromTS(self, folder:str, sdate:str="1990-01-01", edate:str="", codes:list = None, bNeedAdj:bool = True, isIndex:bool = False):

        if edate == "":
            edate = datetime.datetime.now().strftime('%Y-%m-%d')

        if not os.path.exists(folder + 'day/'):
            create_dirs(folder + 'day/')
    
        if not os.path.exists(folder + 'min5/'):
            create_dirs(folder + 'min5/')

        if codes is None:
            if isIndex:
                codes = self.__indice__.keys()
            else:
                codes = self.__stocks__.keys()

        total = len(codes)

        count = 0
        for code in codes:
            count += 1
            print("正在拉取%s的日线[%d/%d]……" % (code, count, total))
            exchg = ""
            if code[:2] == "SH":
                exchg = "SSE"
            else:
                exchg = "SZSE"

            thisFolder = folder + 'day/' + exchg + "/"
            if not os.path.exists(thisFolder):
                create_dirs(thisFolder)

            thisFolder = folder + 'min5/' + exchg + "/"
            if not os.path.exists(thisFolder):
                create_dirs(thisFolder)
            
            df_bars = None
            try:
                # get_h_data貌似也可以
                df_bars = ts.get_k_data(code[2:], start=sdate, end=edate, ktype='D', autype='qfq' if bNeedAdj else None, index=isIndex)

                csvpath = (folder + 'day/%s/%s%s.csv') % (exchg, code[2:], "Q" if bNeedAdj and not isIndex else "")
                isNewFile = True
                if os.path.exists(csvpath):
                    isNewFile = False

                f = open(csvpath, 'w')
                f.write("date, time, open, high, low, close, volumn\n")

                for idx, row in df_bars.iterrows():
                    f.write(datetime.datetime.strptime(str(row["date"]), '%Y-%m-%d').strftime("%Y/%m/%d") + ", ")
                    f.write("0, ")
                    f.write(str(row["open"]) + ", ")
                    f.write(str(row["high"]) + ", ")
                    f.write(str(row["low"]) + ", ")
                    f.write(str(row["close"]) + ", ")
                    f.write(str(row["volume"]) + "\n")
                f.close()
            except:
                print("正在拉取%s的日线异常" % (code))

            print("正在拉取%s的5分钟线[%d/%d]……" % (code, count, total))
            df_bars = None
            try:
                df_bars = ts.get_k_data(code[2:], sdate, edate, ktype='5', autype='qfq' if bNeedAdj else None)

                csvpath = (folder + 'min5/%s/%s%s.csv') % (exchg, code[2:], "Q" if bNeedAdj and not isIndex else "")
                isNewFile = True
                if os.path.exists(csvpath):
                    isNewFile = False

                f = open(csvpath, 'w')
                f.write("date, time, open, high, low, close, volumn\n")

                for idx, row in df_bars.iterrows():
                    curDt = datetime.datetime.strptime(str(row["date"]), '%Y-%m-%d %H:%M')
                    f.write(curDt.strftime("%Y/%m/%d") + ", ")
                    f.write(curDt.strftime("%H:%M") + ", ")
                    f.write(str(row["open"]) + ", ")
                    f.write(str(row["high"]) + ", ")
                    f.write(str(row["low"]) + ", ")
                    f.write(str(row["close"]) + ", ")
                    f.write(str(row["volume"]) + "\n")
                f.close()
            except:
                print("正在拉取%s的5分钟线异常" % (code))

    def dmpStkDaysFrom163(self, folder:str, sdate:int=19900101, edate:int=0, codes:list = None, isIndex:bool = False):
        if edate == 0:
            edate = int(datetime.datetime.now().strftime('%Y%m%d'))

        if codes is None:
            if isIndex:
                codes = self.__indice__.keys()
            else:
                codes = self.__stocks__.keys()
        total = len(codes)

        count = 0
        for code in codes:
            count += 1
            rawcode = code
            if code[:2] == "SH":
                code = "0" + code[2:]
            else:
                code = "1" + code[2:]

            url = "http://quotes.money.163.com/service/chddata.html?code=%s&start=%d&end=%d&fields=TCLOSE;HIGH;LOW;TOPEN;LCLOSE;CHG;PCHG;TURNOVER;VOTURNOVER;VATURNOVER;TCAP;MCAP" % (code, sdate, edate)
            print("正在拉取%s[%d/%d]的日线数据[%d-%d]……" % (rawcode, count, total, sdate, edate))
            exchg = ""
            if rawcode[:2] == "SH":
                exchg = "SSE"
            else:
                exchg = "SZSE"
            content = httpGet(url, "gbk")
            if len(content) == 0:
                print("股票%s没有拉取到日线数据" % (rawcode))
                continue
            lines = content.split("\r\n")

            if not os.path.exists(folder + 'day/' + exchg + "/"):
                create_dirs(folder + 'day/' + exchg + "/")

            if isIndex:
                csvpath = (folder + 'day/%s/%s.csv') % (exchg, rawcode[2:])
            else:
                csvpath = (folder + 'day/%s/%sQ.csv') % (exchg, rawcode[2:])
            f = open(csvpath, 'w+')
            f.write("date, time, open, high, low, close, volumn, turnover\n")

            for idx in range(1,len(lines)):
                line = lines[-idx].strip()
                if len(line) == 0:
                    continue

                items = line.split(",")
                if items[6]=="0.0" and items[4]=="0.0" and items[5]=="0.0":#过滤停牌数据
                    continue

                f.write(datetime.datetime.strptime(items[0], '%Y-%m-%d').strftime("%Y/%m/%d") + ", ")
                f.write("0, ")
                f.write(items[6] + ", ")
                f.write(items[4] + ", ")
                f.write(items[5] + ", ")
                f.write(items[3] + ", ")
                f.write(items[11] + ",")
                f.write(items[12] + "\n")
            f.close()
        return