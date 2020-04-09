# WonderTrader——组合盘管理的利器
**WonderTrader是一个针对组合盘管理的开源交易框架**。<br>
*WonderTrader*通过合理的解耦框架的各个模块，搭建了一个组合+多个通道的架构，从而实现轻松管理多策略多账户的目的。<br>
作者有10多年的证券期货行业系统的开发经验，尤其对于CTA策略的IT系统开发有丰富的经验。经过多年的沉淀，最终形成了*WonderTrader*。<br>
*WonderTrader*是作者现在团队正在使用的框架，在*WonderTrader*框架上管理的资金规模也非常可观。作者开源的目的，一方面是不想闭门造车，毕竟团队内的策略风格和交易方向都有限，另外一方面是希望能够通过*WonderTrader*这个开放式的框架，给一些策略研发人员和平台开发人员带来更多的便利。

# 功能特点
+ 核心逻辑 + 外部模块
    > *WonderTrader*合理解耦整个框架的各个模块，通过合理的设计模块接口，将外部功能模块与核心逻辑合理的剥离出来，保证了模块之间的独立性。使用者可以根据自己的需要自行开发各个功能模块，以满足不同的业务场景的不同需求。<br>
    最常见则是交易通道（Trader）和行情通道（Parser）的开发，另外还有执行器（Executer）、CTA策略、风控模块等。

+ 一个组合 + 多个通道
    > *WonderTrader*通过解耦策略和执行，再配合合理的控制，实现了一个组合管理多个账户的目标。<br>
    组合包含了多个独立的CTA策略实例，可以是不同的策略的不同实例，也可以是相同策略针对不同品种的实例。组合盘将各个策略的目标品种的目标仓位轧平以后，统一提交给执行模块（一个执行模块对应一个交易通道）。<br>
    一个组合盘作为基本资金单位（如50w），搭配多个执行器，每个执行器配置不同的手数倍数s（s≥1）。组合的目标仓位设置好以后（如A品种x手，B品种y手）执行器会根据不同的交易账户按照事先设置好的倍数，设置自己的目标仓位（A品种x\*s手，B品种y\*s手），然后按照自己的逻辑进行买卖，直到和目标仓位一致。

+ 内置模块 + 自主模块
    > *WonderTrader*作为一个完整的交易框架，各个模块都提供内置的成熟模块，如数据落地、CTP行情接入，CTP交易、执行模块。内置的各个功能模块，已经能够满足绝大部分的需求。<br>
    同时，*WonderTrader*作为开放的框架，也允许使用者自行开发自主的功能模块。因为内置的功能模块要兼顾大多数的需求，所以无法满足一些特定场景的特定需求。如果使用者有一些特定的场景有特殊的需求，就可以考虑自行开发功能模块。<br>
    内置模块+自主模块的方式，能够兼顾便利性和灵活性，给使用者提供了极大的选择空间。

+ C++底层 + Python
    > *WonderTrader*全部采用C++开发。<br>
    考虑到不同的用户熟悉的开发语言不同，所以整个*WonderTrader*在代码上采用的结构都是core + porter的结构。简单来说，一个核心的lib，搭配C接口的动态导出库。<br>
    C接口的导出库，主要是为了跨语言调用的。*WonderTrader*没有使用boost.python之类的库进行跨语言对接，而只是采用C接口导出，方便各种语言调用。<br>
    Python作为当下最流行的量化开发语言，*WonderTrader*专门提供了一个Python3的子框架***wtpy***。*wtpy*通过C接口的导出库，将整个*WonderTrader*的功能都平移到Python里，极大的方便的不同层次的使用者。目前Python只对接了CTA引擎（高频不建议在Python下开发策略）。

+ CTA为主 + 伪高频为辅
    > CTA策略一般是基于分钟线以上的低频数据来开发的，*WonderTrader*的CTA引擎通过接入的行情数据，统一的调度各个策略的计算周期，从而实现组合的目标仓位统一管理的目的。这样做的好处是：组合只保留净头寸，如果有两个策略交易同一个品种，头寸会进行自动轧平，从而从根本上杜绝自成交，并减少保证金的占用和手续费的支出。<br>
    为什么是伪高频呢？因为*WonderTrader*彻底剥离了策略和执行，但是很多高频策略都要自行管理订单，所以*WonderTrader*的架构则无法满足这样的需求，故称之为伪高频。换一种简单的说法，伪高频就是tick级别的CTA策略。*WonderTrader*可以提供tick级和分钟级的数据响应回调，从而驱动策略进行计算。但是*WonderTrader*暂时还没有针对低延时做过特别的优化，所以使用者如果有高频策略需要在*WonderTrader*上使用，请一定要做好充分的评估！

# 使用前的准备
+ 基础数据文件 基础数据文件决定了交易品种的属性和交易时间的控制
    > - 交易时间模板文件sessions.json，如下是股票的交易时间模板
    
        ```javascript
        {
            "SD0930":{
                "name":"股票白盘0930",
                "offset": 0,
                "auction":{
                    "from": 929,
                    "to": 930
                },
                "sections":[
                    {
                        "from": 930,
                        "to": 1130
                    },
                    {
                        "from": 1300,
                        "to": 1500
                    }
                ]
            }
        }
        ```
    > - 品种定义文件commodities.json，如下是国内股票的品种定义
     
        ```javascript
        {
            "SSE" : 
            {
                "STK" : 
                {
                    "category" : 0,
                    "covermode" : 0,
                    "exchg" : "SSE",
                    "holiday" : "CHINA",
                    "name" : "上证股票",
                    "precision" : 2,
                    "pricemode" : 1,
                    "pricetick" : 0.01,
                    "session" : "SD0930",
                    "volscale" : 1
                }
            },
            "SZSE" : 
            {
                "STK" : 
                {
                    "category" : 0,
                    "covermode" : 0,
                    "exchg" : "SZSE",
                    "holiday" : "CHINA",
                    "name" : "深证股票",
                    "precision" : 2,
                    "pricemode" : 1,
                    "pricetick" : 0.01,
                    "session" : "SD0930",
                    "volscale" : 1
                }
            }
        }
        ```
    > - 合约列表文件contracts.json，如下是截取的股票列表

        ```javascript
        {
            "SSE": {
                "SH000001": {
                    "code": "SH000001",
                    "exchg": "SSE",
                    "name": "上证综指",
                    "product": "IDX"
                }，
                "SH600000": {
                    "area": "上海",
                    "code": "SH600000",
                    "exchg": "SSE",
                    "indust": "银行",
                    "name": "浦发银行",
                    "product": "STK"
                }
            },
            "SZSE" : 
            {
                "SZ399001": {
                    "code": "SZ399001",
                    "exchg": "SZSE",
                    "name": "深证成指",
                    "product": "IDX"
                },
                "SZ000001": {
                    "area": "深圳",
                    "code": "SZ000001",
                    "exchg": "SZSE",
                    "indust": "银行",
                    "name": "平安银行",
                    "product": "STK"
                }
            }
        }
        ```
    > - 主力合约切换规则文件，期货交易的时候需要用到，如下

        ```javascript
        {
            "CFFEX": {
                "IC": [
                    {
                        "date": 20190920,
                        "from": "IC1909",
                        "newclose": 5150.6,
                        "oldclose": 5185.6,
                        "to": "IC1910"
                    },
                    {
                        "date": 20191018,
                        "from": "IC1910",
                        "newclose": 4923.6,
                        "oldclose": 5028.2,
                        "to": "IC1912"
                    }
                ],
                "IF": [
                    {
                        "date": 20191018,
                        "from": "IF1910",
                        "newclose": 3924.6,
                        "oldclose": 3926.8,
                        "to": "IF1911"
                    },
                    {
                        "date": 20191114,
                        "from": "IF1911",
                        "newclose": 3897.0,
                        "oldclose": 3900.0,
                        "to": "IF1912"
                    }
                ]
            }
        }
        ```
# 使用指南
详见demo，这里不做赘述

# 版权说明
MIT
