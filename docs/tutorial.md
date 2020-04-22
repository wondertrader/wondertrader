#### 安装wtpy
---
* 安装python3.6以上的版本（32位、64位），安装完成以后输入以下命令，可以检查python的版本号
    ```
    $ python
    ```

* 打开命令行，输入以下指令直接安装
    ```
    $ pip install wtpy
    ```
    或者直接下载whl文件到本地进行安装
    阿里云镜像地址：<https://mirrors.aliyun.com/pypi/simple/wtpy/>
    pipy地址：<https://pypi.org/project/wtpy/#files>

* 安装完成以后，输入以下命令，可以查看wtpy的版本号
    ```
    $ pip show wtpy
    ```

### 选择要运行的demo
---
* github上提供了python下的几个demo
    * 期货回测 [backtest_fut](https://github.com/wondertrader/wondertrader/tree/master/demos/py/backtest_fut)
    * 股票回测 [backtest_stk](https://github.com/wondertrader/wondertrader/tree/master/demos/py/backtest_stk)
    * 期货数据组件 [datakit_fut](https://github.com/wondertrader/wondertrader/tree/master/demos/py/datakit_fut)
    * 股票数据组件 [datakit_stk](https://github.com/wondertrader/wondertrader/tree/master/demos/py/datakit_stk)
    * 期货实盘 [product_fut](https://github.com/wondertrader/wondertrader/tree/master/demos/py/product_fut)
    * 股票实盘 [product_stk](https://github.com/wondertrader/wondertrader/tree/master/demos/py/product_stk)

* 选择需要的demo，如期货回测demo

### 修改demo的配置
---
* 打开回测配置文件configbt.json
    ```json
    {
        "replayer":{
            "mode":"csv",           //数据类型，csv或者bin
            "path":"./storage/",    //数据存储路径
            "stime":201909010900,   //回测开始时间，分钟线yyyyMMddhhmm，日线yyyyMMdd
            "etime":201912011500,   //回测结束时间，格式同开始时间
            "basefiles":{
                "session":"./common/sessions.json",     //交易时间模板
                "commodity":"./common/commodities.json",//品种列表
                "contract":"./common/contracts.json",   //合约列表
                "holiday":"./common/holidays.json",     //节假日
                "hot":"./common/hots.json"              //期货主力切换规则
            },
            "fees":"fees.json"  //佣金模板
        },
        "env":{
            "mocker":"cta"      //回测撮合，CTA模式
        }
    }
    ```

### 回测及结果分析
---
* 打开回测入口文件runBT.py，配置好回测引擎，加入要测试的策略，以及设置好绩效分析的参数
    ```python
    from wtpy import WtBtEngine
    from wtpy.backtest import WtBtAnalyst

    from Strategies.DualThrust import StraDualThrust

    if __name__ == "__main__":
        #创建一个运行环境，并加入策略
        engine = WtBtEngine()
        engine.init('.\\Common\\', "configbt.json")
        engine.configBacktest(201909100930,201912011500)
        engine.configBTStorage(mode="csv", path=".\\storage\\")
        engine.commitBTConfig() #代码里的配置项，会覆盖配置文件configbt.json里的配置项

        '''
        创建DualThrust策略的一个实例
        name    策略实例名称
        code    回测使用的合约代码
        barCnt  要拉取的K线条数
        period  要使用的K线周期，m表示分钟线
        days    策略算法参数，算法引用的历史数据条数
        k1      策略算法参数，上边界系数
        k2      策略算法参数，下边界系数
        isForStk    DualThrust策略用于控制交易品种的代码
        '''
        straInfo = StraDualThrust(name='pydt_IF', code="CFFEX.IF.HOT", barCnt=50, period="m5", days=30, k1=0.1, k2=0.1, isForStk=False)
        engine.set_strategy(straInfo)

        #开始运行回测
        engine.run_backtest()

        #创建绩效分析模块
        analyst = WtBtAnalyst()
        #将回测的输出数据目录传递给绩效分析模块
        analyst.add_strategy("pydt_IF", folder="./outputs_bt/pydt_IF/", init_capital=500000, rf=0.02, annual_trading_days=240)
        #运行绩效模块
        analyst.run()

        kw = input('press any key to exit\n')
        engine.release_backtest()
    ```
* 然后启动runBT.py进行回测，回测的执行如下
    ![alt 回测示例图](http://q92ex9g0c.bkt.clouddn.com/snapshots/bt_fut_snapshot.jpg)

* 回测完成以后，打开生成的绩效分析报表(xxxx.xlsx)，则可以查看策略的回测绩效
    回测绩效概览
    ![alt 回测绩效概览](http://q92ex9g0c.bkt.clouddn.com/snapshots/bt_fut_pnl_summary.png)
    回测收益详情
    ![alt 回测绩效详情](http://q92ex9g0c.bkt.clouddn.com/snapshots/ana_fut_snapshot.jpg)
    从上图的绩效分析可以看出，DualThrust策略，针对股指期货主力合约，在20190919到20191201这段时间内，这组参数下进行回测，一直处于回撤的状态，累计亏损达20%。可见，这不是一组好的参数。

* 接下来我们调整参数，将K1和K2改成0.5和0.3，再进行回测和分析，得到如下结果
    调整后的回测绩效总览
    ![alt 回测绩效概览](http://q92ex9g0c.bkt.clouddn.com/snapshots/bt_fut_pnl_summary2.png)
    从上图可以看出，参数调整过以后的绩效远远好于调整之前。

* 最后进行回测分析总结
    让我们简单的分析一下参数调整前和参数调整后绩效悬殊的原因。
    熟悉DualThrust的朋友肯定知道：DualThrus的信号原理其实就是价格突破。当上边界系数k1和下边界系数k2较小的时候，信号出现较多，但是容易遇到假突破，从而导致交易成本上升，最终出现净亏损；当k1和k2较大的时候，信号变少，但是又容易错失交易机会。
    调整之前的k1和k2较小，调整以后的k1和k2也不是特别大，从而达到了过滤大部分假突破，降低交易成本，并保证一定的灵敏度的效果，所以才会有较大幅度的改善。
    大家有兴趣的话，可以自行尝试一下，将k1和k2改成不同的组合，相信能够有更好的回测结果。
    