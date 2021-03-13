## **WonderTrader**数据存储方式
&emsp;&emsp;环境准备好了以后，还需要确定我们准备使用什么方式存储数据。**WonderTrader**实盘环境下支持两种数据存储方式：**文件存储**和**数据库存储**。而回测环境下，还支持直接从`csv`读取数据（仅限于历史K线数据）。

### 文件存储
&emsp;&emsp;历史K线数据文件，采用`zstd`压缩后存放。高频历史数据，包括`tick`数据，股票`level2`的委托明细、成交明细、委托队列，也采用压缩存放的方式。A股全市场一天的`level2`数据，压缩以后也就是大概**2G不到**的样子，对于硬盘来说是相当友好的。
&emsp;&emsp;实时数据文件，因为需要实时读写，所以不压缩数据结构，并采用**mmap**的方式映射到内存中，直接对文件进行读写，提高读写效率。

### 数据库存储
&emsp;&emsp;数据库存储的方式，**只针对历史K线数据**。实时数据和高频数据的存储方式还是和文件存储模式一致的。
&emsp;&emsp;主要考虑到高频数据量非常巨大，如果采用数据库，整个数据库的运行效率会大大降低。而实时数据对延迟要求非常高，数据库则不适合这样的应用场景了。目前支持的数据库是`mysql`/`mariadb`，以后如果有需求的话，可能会扩展到一些`nosql`数据库。

### csv历史数据
&emsp;&emsp;很多用户通过各种渠道获取到的历史数据，供应商为了便于用户直接查看数据，一般都会提供`csv`格式的。但是`csv`文件格式的数据，占用空间非常大，而且直接从`csv`文件读取数据的开销也是非常大的。
&emsp;&emsp;**WonderTrader**的回测框架为了尽量减少这种不必要的开销，在处理`csv`文件时，第一次会直接从`csv`文件读取，将读取的数据转成**WonderTrader**内部数据结构之后，会将数据转储为**WonderTrader**自有的压缩存放格式。这样下次在使用该数据的时候，读取压缩存放的数据以后，直接解压就可以得到结构化的历史数据，这样就可以直接进行访问了。

## **datahelper**模块
&emsp;&emsp;**datahelper**模块位于`wtpy.apps`子模块下，采用工厂模式进行封装，最大限度的降低了使用难度，将各种`API`的差异全部封装起来，用户在使用的时候只需要调用工厂创建即可，而不用担心因为每个数据源`API`不同而导致的各种问题。

### 数据源
**datahelper**模块目前已封装的数据源包括[tushare](https://tushare.pro/)、[baostock](http://baostock.com/)、[RQData](https://www.ricequant.com/doc/rqdata/python/)。

* `tushare`是知名度较高的**免费**数据源，**数据比较全**，使用的人也很多。但是`tushare`有些数据需要积分才能下载，下载速度也较慢
* `baostock`是一个**免费**、开源的**证券数据**平台，**无需注册**，并且**下载速度也非常快**，可以拿到**5分钟线**数据
* `RQData`是米筐开发的一个基于`Python`的金融数据工具包，是一个**收费**数据源，**数据很全，数据质量也很高**。对于一些有1分钟线甚至更高频数据的需求，免费的数据源就无法提供了，`RQData`可能也是一个不错的选择。

### 数据接口
**datahelper**模块主要**帮助用户进行历史数据的下载**，以及一些**基础数据的获取**。主要包括3种接口：
* 获取代码列表
* 获取除权因子
* 获取历史K线

&emsp;&emsp;对于财务数据，**WonderTrader**暂时没有从平台层面做标准化的工作。一方面财务数据相对静态，可以相对容易的从不同的渠道拿到。另一方面只有股票才需要财务数据，而现在最流行的选股框架还是多因子框架。相对**WonderTrader**而言，多因子框架几乎是另一个维度的，所以**WonderTrader**暂时就不涉及财务数据这块了。

数据辅助模块各个接口的详细定义如下：
```py
class BaseDataHelper:

    def __init__(self):
        self.isAuthed = False
        pass

    def __check__(self):
        if not self.isAuthed:
            raise Exception("This module has not authorized yet!")

    def auth(self, **kwargs):
        '''
        模块认证
        '''
        pass

    def dmpCodeListToFile(self, filename:str, hasIndex:bool=True, hasStock:bool=True):
        '''
        将代码列表导出到文件\n
        @filename   要输出的文件名，json格式\n
        @hasIndex   是否包含指数\n
        @hasStock   是否包含股票\n
        '''
        pass

    def dmpAdjFactorsToFile(self, codes:list, filename:str):
        '''
        将除权因子导出到文件\n
        @codes  股票列表，格式如["SSE.600000","SZSE.000001"]\n
        @filename   要输出的文件名，json格式
        '''
        pass

    def dmpBarsToFile(self, folder:str, codes:list, start_date:datetime=None, end_date:datetime=None, period:str="day"):
        '''
        将K线导出到指定的目录下的csv文件，文件名格式如SSE.600000_d.csv\n
        @folder 要输出的文件夹\n
        @codes  股票列表，格式如["SSE.600000","SZSE.000001"]\n
        @start_date 开始日期，datetime类型，传None则自动设置为1990-01-01\n
        @end_date   结束日期，datetime类型，传None则自动设置为当前日期\n
        @period K线周期，支持day、min1、min5\n
        '''
        pass

    def dmpAdjFactorsToDB(self, dbHelper:DBHelper, codes:list):
        '''
        将除权因子导出到数据库\n
        @codes  股票列表，格式如["SSE.600000","SZSE.000001"]\n
        @dbHelper   数据库辅助模块
        '''
        pass

    def dmpBarsToDB(self, dbHelper:DBHelper, codes:list, start_date:datetime=None, end_date:datetime=None, period:str="day"):
        '''
        将K线导出到数据库\n
        @dbHelper 数据库辅助模块\n
        @codes  股票列表，格式如["SSE.600000","SZSE.000001"]\n
        @start_date 开始日期，datetime类型，传None则自动设置为1990-01-01\n
        @end_date   结束日期，datetime类型，传None则自动设置为当前日期\n
        @period K线周期，支持day、min1、min5\n
        '''
        pass
```

## 数据下载
本文将以`tushare`数据源为例，演示一下数据辅助模块的基本用法。

### 模块初始化
* 首先，创建`tushare`对应的数据辅助模块：
    ```py
    from **wtpy**.apps.datahelper import DHFactory as DHF

    hlper = DHF.createHelper("tushare")
    ```

* 创建好了以后，对`tushare`进行认证：
    ```py
    hlper.auth(**{"token":"your token of tushare","use_pro":True})
    ```
&emsp;&emsp;值得一提的是，上面的代码中没有一个参数`use_pro`，该参数不是`tushare`认证需要的，而是用于控制`tushare`调用的接口的，如果`use_pro`为`True`，那么就调用`tushare`的`pro_bar`接口读取历史K线数据，否则就调用老版本的接口`get_k_data`读取历史K线数据。之所以这样，是因为`pro_bar`接口获取分钟数据的时候需要积分的，但是老的接口是不需要积分的。

### 下载数据到文件中
* 然后调用不同的接口获取数据，下面的代码演示了将数据下载到指定的文件中：
    ```py
    # 将代码列表下载到文件中
    hlper.dmpCodeListToFile(filename = 'codes.json', hasStock = True, hasIndex = True)

    # 将除权因子下载到文件中
    hlper.dmpAdjFactorsToFile(codes=['SSE.600000','SZSE.000001'], filename="./adjfactors.json")

    # 将K线下载到指定目录
    hlper.dmpBarsToFile("./", codes = ['SSE.600000','SZSE.000001'], period="day")
    ```

代码列表下载截图
![代码列表下载截图](https://segmentfault.com/img/bVcO4ze)

代码列表文件示意
```json
{
    "SSE": {
        "000001": {
            "code": "000001",
            "exchg": "SSE",
            "name": "上证指数",
            "product": "IDX"
        },
        "600000": {
            "code": "600000",
            "exchg": "SSE",
            "name": "浦发银行",
            "product": "STK"
        }
    },
    "SZSE": {
        "000001": {
            "code": "000001",
            "exchg": "SZSE",
            "name": "平安银行",
            "product": "STK"
        }
        "399001": {
            "code": "399001",
            "exchg": "SZSE",
            "name": "深证成指",
            "product": "IDX"
        }
    }
}
```

除权因子下载截图
![除权因子下载截图](https://segmentfault.com/img/bVcO4zM)


除权因子文件示意
```json
{
    "SSE": {
        "600000": [
            {
                "date": "20160623",
                "factor": 9.267
            },
            {
                "date": "20170525",
                "factor": 12.201
            },
            {
                "date": "20180713",
                "factor": 12.33
            },
            {
                "date": "20190611",
                "factor": 12.713
            },
            {
                "date": "20200723",
                "factor": 13.405
            }
        ]
    },
    "SZSE": {
        "000001": [
            {
                "date": "20160616",
                "factor": 104.758
            },
            {
                "date": "20170721",
                "factor": 106.309
            },
            {
                "date": "20180712",
                "factor": 108.031
            },
            {
                "date": "20190626",
                "factor": 109.169
            },
            {
                "date": "20200528",
                "factor": 111.048
            }
        ]
    }
}
```

K线数据下载截图<br>
![K线数据下载截图](https://segmentfault.com/img/bVcO4Ad)

K线数据示意
```csv
date,time,open,high,low,close,volume,turnover
20000112,0,26.0,26.0,24.8,25.12,35274900.0,88986734.0
20000113,0,25.0,25.25,24.8,24.9,7975600.0,19924430.4
20000114,0,24.88,25.0,23.91,24.2,17861900.0,43420608.7
20000117,0,24.08,24.44,23.75,24.4,8101500.0,19477696.400000002
20000118,0,24.5,24.57,23.88,24.14,7693300.0,18509168.400000002
20000119,0,24.14,24.29,23.98,24.13,4658400.0,11232523.799999999
20000120,0,24.12,24.65,24.1,24.44,5114400.0,12466817.600000001
20000121,0,24.51,24.63,24.08,24.27,8138700.0,19752321.5
20000124,0,24.2,24.25,23.95,24.18,9250300.0,22240443.5
......
20210210,0,10.67,10.85,10.56,10.69,105092240.99999999,112396135.6
20210218,0,10.8,11.02,10.74,10.83,143397923.0,155850481.1
20210219,0,10.83,11.12,10.77,10.97,122926300.0,135129039.1
20210222,0,10.92,10.95,10.7,10.71,127379413.99999999,137353338.2
20210223,0,10.71,10.99,10.71,10.78,93327786.0,101090131.6
20210224,0,10.81,10.89,10.55,10.6,97783999.0,104747027.89999999
20210225,0,10.66,10.85,10.6,10.8,89855495.0,96473275.5
20210226,0,10.72,10.82,10.54,10.54,85386093.0,90929810.5

```
### 数据库初始化
* 如果是用数据库存储历史数据，**首先要将数据库初始化**：
```py
# 创建一个数据库辅助模块，并根据需要初始化数据表格
dbHelper = MysqlHelper(host="127.0.0.1", user="root", pwd="", dbname="data_db", port=5306)
dbHelper.initDB()
```

创建空数据库截图<br>
![创建空数据库](https://segmentfault.com/img/bVcO4AL)

初始化好的数据库截图<br>
![初始化好的数据库截图](https://segmentfault.com/img/bVcO4AZ)

### 下载数据到数据库中
* 数据库初始化好了以后，就可以进行数据下载了：
    ```py
    # 下载除权因子并保存到数据库中
    hlper.dmpAdjFactorsToDB(dbHelper, codes=["SSE.600000",'SSE.600001'])

    # 下载历史K线并保存到数据库中
    hlper.dmpBarsToDB(dbHelper, codes=["SSE.600000",'SSE.600001'], period="day")
    ```

下载除权因子到数据库截图
![下载除权因子到数据库截图](https://segmentfault.com/img/bVcO4A6)

数据库中除权因子表截图
![数据库中除权因子表截图](https://segmentfault.com/img/bVcO4Bc)

下载日K线到数据库截图
![下载除权因子到数据库截图](https://segmentfault.com/img/bVcO4Bq)

数据库中日K线表截图
![数据库中除权因子表截图](https://segmentfault.com/img/bVcO4Bz)

## 数据的后续处理
&emsp;&emsp;上面演示了**datahelper**模块的用法，该模块能够帮助用户快速拉取**WonderTrader**可以直接使用的历史数据，可以有效的降低用户初次使用**WonderTrader**进行策略回测的门槛。

&emsp;&emsp;不过在实盘的过程中，还有很多实施的细节，本文也做一个大概的梳理。

### 标的代码的规则
- 期货合约代码，标准格式为`CFFEX.IF.2103`，其中**郑商所的合约，月份也要扩展为4位**
- 期货主力合约，标准格式为`CFFEX.IF.HOT`，**WonderTrader**会根据一个主力合约规则文件自动映射到分月合约
- 证券代码，股票的标准格式为`SSE.STK.600000`，指数的标准格式为`SZSE.IDX.399001`，支持简写格式`SSE.600000`

### 主力合约
&emsp;&emsp;主力合约映射的规则，需要每天维护，**WonderTrader**会根据规则自动处理映射，用户只需要使用`.HOT`代码就可以了。主力合约规则文件如下：
```json
{
    "CFFEX": {
        "IC": [
            {
                "date": 20191018,
                "from": "IC1910",
                "newclose": 4923.6,
                "oldclose": 5028.2,
                "to": "IC1912"
            },
            {
                "date": 20191219,
                "from": "IC1912",
                "newclose": 5208.6,
                "oldclose": 5244.4,
                "to": "IC2003"
            },
            {
                "date": 20200320,
                "from": "IC2003",
                "newclose": 5099.6,
                "oldclose": 5147.4,
                "to": "IC2004"
            }
        ]
    }
}
```
&emsp;&emsp;**WonderTrader**在读取主力合约的历史数据时，会优先读取直接对应的历史数据文件。如存储模式为文件时会先读取名为`CFFEX.IF_HOT.dsb`的文件，然后再根据主力合约规则读取分月合约的数据进行拼接。而如果存储模式为数据库，则会优先读取代码为`xx.HOT`的数据，然后再根据主力合约规则读取分月合约的数据。

### 收盘作业
&emsp;&emsp;顾名思义，在每个交易日结束以后，会对行情数据做一个盘后处理，这个处理的过程就叫做收盘作业。收盘作业主要包括以下工作：
- 将实时高频数据按天按代码压缩存放(`tick`和`level2`高频数据)
- 将当日的K线数据(`min1`和`min5`)合并到历史K线数据中
- 根据当天最新的`tick`数据，生成当天的日K线数据并合并到历史日K线数据中

&emsp;&emsp;正是因为有收盘作业这么一个机制，所以**WonderTrader**目前还不能很好的适应7×24小时交易的品种，如数字货币。所以**WonderTrader**对于数据货币的支持的最大的问题，还是7×24小时交易机制的数据处理问题。