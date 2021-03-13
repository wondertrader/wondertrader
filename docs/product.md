# 实盘完整攻略

本文将完整的介绍一下，如果搭建一个可以直接用于生产环境的完整的流程。
本文将以配置股票仿真盘为例，使用`DualThrust`策略，通过XTP行情通道落地数据，并通过XTP仿真交易通道来进行仿真交易。

### 安装wtpy
---
* 安装python3.6以上的版本（32位、64位），安装完成以后输入以下命令，可以检查python的版本号
    ``` shell
    $ python
    ```

* 打开命令行，输入以下指令直接安装
    ``` shell
    $ pip install wtpy --upgrade
    ```
    或者直接下载whl文件到本地进行安装
    阿里云镜像地址：<https://mirrors.aliyun.com/pypi/simple/wtpy/>
    pipy地址：<https://pypi.org/project/wtpy/#files>

* 安装完成以后，输入以下命令，可以查看wtpy的版本号
    ``` shell
    $ pip show wtpy
    ```

### 准备数据组件
---
* 复制股票数据组件demo<https://github.com/wondertrader/wtpy/tree/master/demos/datakit_stk>

* 打开配置文件`dtcfg.json`，配置XTP仿真行情通道
    ```json
    "parsers":[
        {
            "active":true,
            "module":"ParserXTP.dll",
            "host":"120.27.164.138",    //XTP仿真行情通道地址
            "port":"6002",
            "user":"********",  //XTP仿真账号
            "pass":"********",  //XTP仿真密码
            "protocol":1,       //XTP通道参数，可以忽略
            "clientid":1,       //XTP通道参数，可以忽略
            "hbinterval":15,    //XTP通道参数，可以忽略
            "buffsize":128,     //XTP通道参数，可以忽略
            "code":"SSE.000001,SSE.600009,SSE.600036,SSE.600276,SZSE.000001"
        }
    ]
    ```

* 打开配置文件`dtcfg.json`，配置数据落地目录
    ```json
     "writer":{
        "path":"./STK_Data",    //数据落地目录
        "savelog":false,        //是否将tick同时输出csv文件
        "async":false,          //异步开关，如果股票个数比较多，建议开启
        "groupsize":20          //数据落地分组大小，用于控制日志显示
    }
    ```

* 打开配置文件`dtcfg.json`，配置广播端口
    ```json
    "broadcaster":{
        "active":true,  //是否开启广播
        "bport":3997,   //订阅端口
        "broadcast":[
            {
                "host":"255.255.255.255",   //广播地址
                "port":9001,                //广播端口
                "type":2                    //数据格式，固定为2
            }
        ]
    }
    ```

* 完成上述工作以后，就可以执行`runDT.py`启动数据组件了

### 准备环境
---
* 复制股票实盘demo<https://github.com/wondertrader/wtpy/tree/master/demos/cta_stk>

* 打开配置文件`config.json`，修改环境配置
    ```json
    "env":{
        "name":"cta",
        "mode": "product",
        "product":{
            "session":"SD0930"
        },
        "filters":"filters.json",   //人工干预配置文件，暂时忽略
        "fees":"fees_stk.json",     //佣金模板文件
    }
    ```

* 打开配置文件`config.json`，修改数据读取路径
    ```json
    "data":{
        "store":{
            "path":"./STK_Data/"    //这里修改为之前数据组件落地的路径，注意区分相对路径和绝对路径
        }
    }
    ```

* 打开配置文件`config.json`，修改XTP交易通道配置
    ```json
    "traders":[
        {
            "active":true,
            "id":"simnow",              //交易通道ID
            "module":"TraderXTP.dll",   
            "host":"120.27.164.69",     //XTP仿真交易地址
            "port":"6001",
            "user":"********",          //XTP仿真账号
            "pass":"********",
            "acckey":"********",        //XTP认证key
			"protocol":1,
			"clientid":1,
			"hbinterval":15,
			"buffsize":128,
            "quick":true,
        }
    ]
    ```

* 打开配置文件`config.json`，修改行情通道配置
    ```json
    "parsers":[
        {
            "active":true,
            "id":"parser1",
            "module":"ParserUDP.dll",
            "host":"127.0.0.1",
            "bport":9001,               //数据组件广播端口
            "sport":3997,               //数据组件查询端口
            "filter":""
        }
    ]
    ```

* 至此，`config.json`里面需要用户修改的修改项基本就修改完成了，其他的配置项保持即可。

### 准备数据
---
假设我们需要使用上证指数(SSE.000001)、浦发银行(SSE.600000)、深证成指(SZSE.399001)、平安银行(SZSE.000001)等4个品种的5分钟数据和日线数据。

* 从Multicharts导出SSE.000001和SZSE.399001两个指数的历史日线数据和历史5分钟线数据（截止到最近一个交易日）到csv文件

* 从Multicharts导出SSE.000001和SZSE.399001两个指数的历史日线前复权数据和历史5分钟线前复权数据（截止到最近一个交易日）到csv文件。个股要考虑数据的连续性，所以要使用前复权数据。

* 将sv文件按照数据周期不同放到不同的文件夹下面，然后利用回测引擎，将csv数据转换成wt平台的数据格式
    ```python
    from wtpy import WtBtEngine

    if __name__ == "__main__":
        #创建一个运行环境，并加入策略
        env = WtBtEngine()
        
        env.trans_mc_bars(".\\csv_m5\\",".\\bin_m5\\", "m5")
        env.trans_mc_bars(".\\csv_d\\",".\\bin_d\\", "d")

        kw = input('press any key to exit\n')
    ```

* 将生成的数据文件(`*.dsb`)重命名后，复制到数据组件存储路径对应的目录下，如：
    * 从MC导出的历史数据文件名为`SH600009Q_m5.csv`
    * 转成wt内部数据格式以后为`SH600009Q_m5.dsb`
    * 将dsb文件名改成`600009Q.dsb`
    * 然后将`600009Q.dsb`复制到`./STK_Data/his/min5/SSE/`下面

* 历史数据准备完成以后，对于个股数据来说，wt会自动将历史复权数据和最新的实时数据拼接起来，从而形成一份完整连续的个股复权数据。

### 修改策略
---
`DualThrust`策略，在编写的时候，就考虑到了对股票的支持，所以加了一个参数`isForStk`。对于一般期货策略逻辑，要改写成支持股票，主要需要注意以下几点：
* 获取历史数据的时候，一定要将股票代码加`Q`
    ```python
    #读取最近50条1分钟线(dataframe对象)
    theCode = code
    if self.__is_stk__:
        theCode = theCode + "Q" #历史数据一定要用复权数据
    df_bars = context.stra_get_bars(theCode, self.__period__, self.__bar_cnt__, isMain = True)
    ```

* 对于国内市场来说，股票买入必须按照1手100股来挂单
    ```python
    trdUnit = 1
    if self.__is_stk__:
        trdUnit = 100   #交易股票的时候需要设定交易单位为100
    ```

* 最后发出信号的时候，品种代码一定不能带`Q`

* 策略逻辑要注意股票和期货的区别
    ```python
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

        if lowpx <= lower_bound and not self.__is_stk__: #不是股票才能做空
            context.stra_enter_short(code, 1*trdUnit, 'entershort')
            context.stra_log_text("向下突破%.2f<=%.2f，空仓进场" % (lowpx, lower_bound))
            return
    elif curPos > 0:
        if lowpx <= lower_bound:
            context.stra_exit_long(code, 1*trdUnit, 'exitlong')
            context.stra_log_text("向下突破%.2f<=%.2f，多仓出场" % (lowpx, lower_bound))
            return
    else:
        if highpx >= upper_bound and not self.__is_stk__: #不是股票才能做空
            context.stra_exit_short(code, 1*trdUnit, 'exitshort')
            context.stra_log_text("向上突破%.2f>=%.2f，空仓出场" % (highpx, upper_bound))
            return
    ```

* 策略修改完成以后，还可以放到回测环境下，做一次回测。回测的教程可以参考[回测自己的策略](mystrategy.md)这一章。


### 启动实盘
---
* 检查run.py是否正确配置
    ```python
    from wtpy import WtEngine
    from Strategies.DualThrust import StraDualThrust

    if __name__ == "__main__":
        #创建一个运行环境，并加入策略
        engine = WtEngine()
        engine.init('.\\Common\\', "config.json")
        
        straInfo = StraDualThrust(name='pydt_SH600000', code="SSE.600000", barCnt=50, period="m5", days=30, k1=0.1, k2=0.1, isForStk=True)
        engine.add_strategy(straInfo)

        straInfo = StraDualThrust(name='pydt_SZ000001', code="SZSE.000001", barCnt=50, period="m5", days=30, k1=0.1, k2=0.1, isForStk=True)
        engine.add_strategy(straInfo)
        
        engine.run()

        kw = input('press any key to exit\n')
    ```

* 开盘前启动数据组件，比如9:20

* 执行run.py