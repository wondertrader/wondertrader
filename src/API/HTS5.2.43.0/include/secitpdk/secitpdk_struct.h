#pragma once
#pragma pack(push,1)
#define  HEADER_VER 5243

struct stStructMsg
{
    int16       nStructFunc;           //功能号
    uint8       nStructVer;            //请求报文版本
    uint64      nStructToken;          //安全令牌
    int16       nStructRetCode;        //返回值
    char        AccountId[13];         //客户号
    char        FundAccount[31];       //资金账号
    int64       OrderId;               //委托号 
    char        Department[5];         //营业部
    char        Market[3];             //交易所
    char        SecuAccount[11];       //股东号 
    char        BPSecuAccount[11];     //股东号 
    char        StockCode[9];          //证券代码
    char        StockType[9];          //证券类别
    char        MarketingUnit[7];      //交易单元
    char        TransType[3];          //业务类别
    char        WithdrawFlag[2];       //撤销标志
    char        MatchSerialNo[31];     //成交编号
    char        MatchTime[13];         //成交时间
    char        MoneyType[4];          //币种
    float64     OrderPrice;            //委托价格
    int64       OrderQty;              //委托数量
    int64       WithdrawQty;           //撤单数量
    float64     FrozenBalance;         //冻结资金
    int64       MatchQty;              //本次成交数量
    float64     MatchPrice;            //本次成交价格
    float64     MatchAmt;              //本次成交金额
    int64       TotalMatchQty;         //委托总成交数量
    float64     TotalMatchAmt;         //委托总成交金额
    float64     SettleBalance;         //清算资金
    uint8       EntrustType;           //交易类别
    int32       OrderType;             //订单类型
    int32       OrderStatus;           //申报结果
    char        ResultInfo[61];        //结果说明
    int64       CXOrderId;             //撤销委托号
    int64       BatchNo;               //委托批次号(原样回送客户端参数)
    char        OrderClerk[9];         //委托柜员
    char        ETPSort[11];           //交易所产品分类
    char        ETPOrderNo[11];        //交易所产品成交序号
    char        KFSDM[7];
    char        KFSBDBH[17];
    char        MsgCode[8];
    char        ConfirmTime[13];        //确认时间
    stStructMsg()
    {
        memset(this, 0, sizeof(stStructMsg));
    }
};
typedef void(*pStructMessageCallbackMethod)(const char* pTime, stStructMsg &stMsg, int nType);
//证券行情
typedef struct
{
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    char     PhoneticCode[8];     //拼音代码
    char     StockType[4];        //证券类别
    double   PriceTick;           //交易价位
    double   TradeUnit;           //交易单位
    int      MaxTradeAmt;         //委托上限
    int      MinTradeAmt;         //委托下限
    char     MoneyType[4];        //币种
    double   LastPrice;           //最新价
    double   HighLimitPrice;      //最高报价--涨停价
    double   LowLimitPrice;       //最低报价--跌停价
    int      NetPriceFlag;        //国债净价标志
    int      QtyTick;             //整手数--港股
    double   LastClosePrice;      //昨收盘
    double   OpenPrice;           //今开盘
    int      SuspendFlag;         //停牌标志
    double   MatchQty;            //成交数量
    double   MatchAmt;            //成交金额
    double   HighPrice;           //最高价
    double   LowPrice;            //最低价
    double   BidPrice1;           //买入价格1
    double   BidPrice2;           //买入价格2
    double   BidPrice3;           //买入价格3
    double   BidPrice4;           //买入价格4
    double   BidPrice5;           //买入价格5
    double   AskPrice1;           //卖出价格1
    double   AskPrice2;           //卖出价格2
    double   AskPrice3;           //卖出价格3
    double   AskPrice4;           //卖出价格4
    double   AskPrice5;           //卖出价格5
    int      BidQty1;             //买入数量1
    int      BidQty2;             //买入数量2
    int      BidQty3;             //买入数量3
    int      BidQty4;             //买入数量4
    int      BidQty5;             //买入数量5
    int      AskQty1;             //卖出数量1
    int      AskQty2;             //卖出数量2
    int      AskQty3;             //卖出数量3
    int      AskQty4;             //卖出数量4
    int      AskQty5;             //卖出数量5
    double   InterestPrice;       //利息报价
    double   FloatShareQty;       //流通股数量
    int      ListDate;            //发行日期
    int      PaymentDate;         //兑付日期
    double   StrikePrice;         //行权价格--期权
    char     StrikeCode[16];      //行权代码--期权
    char     OrderTypeAvl[64];    //可用订单类型
    double   UpdateTime;          //更新时间
} ITPDK_ZQHQ;
//客户信息
typedef struct
{
    char     AccountId[16];          //客户号
    char     AccountName[32];        //客户姓名
    char     CertificateNo[32];      //证件编号
    int      CertificateType;        //证件类别
    char     OrgCode[8];             //机构代码
    int      OrgAccFlag;             //机构账户标志
    int      EntrustWayAvl;          //允许委托方式
    int      ServiceAvl;             //开通产品服务
} ITPDK_KHH;
//数据字典-市场
typedef struct
{
    char     Market[4];           //交易所
    char     MarketName[16];      //交易所简称
    char     MarketFullName[32];  //交易所全称
} ITPDK_JYS;
//数据字典-证券代码
typedef struct
{
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    char     StockType[4];        //证券类别
    int      TradingStatus;       //交易状态
    double   LastClosePrice;      //昨收盘
    int64    MaxTradeAmt;         //委托上限
    int      MinTradeAmt;         //委托下限
    int64    TradeUnit;           //交易单位
    double   PriceTick;           //交易价位
    char     MoneyType[4];        //币种
    double   HighLimitPrice;      //最高报价--涨停价
    double   LowLimitPrice;       //最低报价--跌停价
    char     BrowIndex[16];       //分页查询定位串
    char     BoardCode[4];        //板块标识
    int64    SJMaxTradeAmt;       //市价委托上限
    int      SJMinTradeAmt;       //市价委托下限
    int64    PHMaxTradeAmt;       //盘后委托上限
    int      PHMinTradeAmt;       //盘后委托下限
} ITPDK_ZQDM;
//数据字典-投票信息
typedef struct
{
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockType[4];        //证券类别
    char     CompanyCode[12];     //公司代码
    char     MeetingCode[22];     //大会编码
    char     MeetingName[100];    //大会名称
    char     MotionCode[12];      //议案编号
    char     MotionName[1024];    //议案名称
    char     MotionType[3];       //议案类型
    int      AlloweAbstention;    //允许弃权
    int      BeginDate;           //开始日期
    int      EndDate;             //结束日期
    int64    BrowIndex;           //分页查询定位串
} ITPDK_TPXX;
//数据字典-汇率参数
typedef struct
{
    char     MoneyType1[4];       //币种1
    char     MoneyType2[4];       //币种2
    double   CashBuyRate;         //现钞买入价
    double   CashAskRate;         //现钞卖出价
    double   RemitBuyRate;        //现汇买入价
    double   RemitAskRate;        //现汇卖出价
    double   GSBL;                //估算比例
    double   JSPJJ;               //今收盘
    int      UpdateTime;          //更新日期
} ITPDK_HLCS;
//数据字典-系统代码
typedef struct
{
    char     GroupId[32];   //分类代码
    char     Code[8];       //编码
    char     CodeName[64];  //编码说明
} ITPDK_XTDM;
//数据字典-交易类别
typedef struct
{
    char     EntrustType[4];         //交易类别
    char     EntrustTypeName[16];    //类别名称
} ITPDK_JYLB;
//数据字典-证券交易属性
typedef struct
{
    char     Market[4];        //交易所
    char     StockType[4];     //证券类别
    int      EntrustType;      //交易类别
    int      QtyTick;          //交易基数
    int      EntrustWay;       //委托方式
} ITPDK_ZQJYSX;
//币种
typedef struct
{
    char     MoneyType[4];        //币种
    char     MoneyTypeName[16];   //币种名称
} ITPDK_BZ;
//数据字典-机构代码
typedef struct
{
    char     OrgCode[8];    //机构代码
    char     OrgName[32];   //机构名称
} ITPDK_JGDM;
//数据字典-银行参数
typedef struct
{
    char     BankCode[8];         //银行代码
    char     MoneyType[4];        //币种
    char     BusinessAvl[64];     //允许证券发起的业务类别
    char     CheckFundPwd[64];    //需要校验资金密码的业务类别
    char     CheckBankPwd[64];    //需要校验银行密码的业务类别
} ITPDK_YHCS;
//本日新股申购
typedef struct
{
    int      Flag;             //类别 1.当日新股发行 2.当日可转债、可交换债发行 （A5无该字段）
    char     Market[4];        //交易所
    char     StockCode[12];    //证券代码
    char     StockName[31];    //证券名称
    char     RGCode[12];       //认购代码
    int      RGRQ;             //认购日期
    double   IssuePrice;       //发行价格
    int64    MaxTradeAmt;      //委托上限
    int      MinTradeAmt;      //委托下限（A5无该字段）
    char     MoneyType[4];     //币种（A5无该字段）
    int      UpdateTime;       //更新时间
} ITPDK_XGSG;
//数据字典-港股交易价位
typedef struct
{
    char     Market[4];        //交易所
    char     StockType[4];     //证券类别
    double   BeginPrice;       //开始价格
    double   EndPrice;         //结束价格
    double   PriceTick;        //适用价位
    int      UpdateTime;       //更新日期
} ITPDK_JYJW;
//数据字典-ETF基本信息
typedef struct
{
    char     Market[4];              //交易所
    char     FundCode[8];            //基金代码
    char     FundName[16];           //基金名称
    char     SubsAndRedeCode[8];     //申购赎回代码
    int      TradingDate;            //交易日期
    int64    SubsAndRedeUnit;        //申购赎回单位
    double   CashDiff;               //预估现金差额
    double   MaxCashRatio;           //现金替代比例上限
    int64    SubsAndRedeState;       //申购赎回状态，-1=无申赎资格 0=禁止申赎 1=允许申赎 2=允许申购，禁止赎回 3=禁止申购，允许赎回
    double   NetAssetValue;          //T-1基金单位净值
    double   NAVperCU;               //申购赎回单位净值
    double   CashComponent;          //T日每个篮子现金差额
} ITPDK_ETFXX;
//数据字典-ETF成分股信息
typedef struct
{
    char     FundCode[8];         //基金代码
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    int64    ComponentQty;        //成份股数量
    double   PremiumRatio;        //溢价比例
    double   CashSubstitute;      //替代金额
    int      SubstituteFlag;      //替代标志，0=禁止现金替代 1=允许现金替代 2=必须现金替代
    char     StockType[4];        //证券类别
} ITPDK_ETFMX;
//数据字典-基金代码
typedef struct
{
    char     Market[4];                 //交易所
    char     StockCode[12];             //证券代码
    char     FundCode[12];              //基金代码
    char     FundName[44];              //基金名称
    int      DivideAndMergeStatus;      //基金拆分状态
    double   NetValue;                  //基金净值
    double   TotalNAV;                  //累计净值
    int      DivideRate;                //拆分比例
    double   MinMergeQty;               //最低合并数量
    double   MinDivideQty;              //最低拆分数量
} ITPDK_JJHQ;
//分级基金信息
typedef struct
{
    char     Market[4];                 //交易所
    char     FundCode_M[12];            //母基代码
    char     FundName_M[20];            //母基名称
    double   FundNetValue_M;            //母基净值
    double   FundPrice_M;               //母基价格
    char     IndexCode[12];             //指数代码
    char     IndexName[12];             //指数名称
    double   IndexRate;                 //指数涨跌幅
    char     FundManager[36];           //基金管理人
    double   SubscribeFeeRate;          //申购费率
    double   RedeemFeeRate;             //赎回费率
    char     DivideRate[20];            //AB比率
    int      DivideRate_A;              //A比率
    int      DivideRate_B;              //B比率
    int      RegularConvertFlag;        //定折标志
    double   UpConvertLimit;            //上折阀值
    double   DownConvertLimit;          //下折阀值
    char     NextConvertDate[12];       //下次定折
    int      ExpiryDate;                //终止日期
    char     FundCode_A[12];            //A基代码
    char     FundName_A[20];            //A基名称
    double   FundPrice_A;               //A基现价
    double   FundNetValue_A;            //A基净值
    char     InterestRateRule[20];      //利率规则
    double   CurrentInterestRate;       //本期利率
    double   NextInterestRate;          //本期利率
    double   ForwardInterestRate;       //下期利率
    int      TradableFlag;              //可交易标志
    char     TradePrompt[260];          //交易特别提示
    int      NetValueDate;              //净值日期
    int      MinMergeQty;               //最低组合数量
    int      MinDivideQty;              //最低拆分数量
    char     FundCode_B[8];             //B基代码
    char     FundName_B[20];            //A基名称
    double   FundNetValue_B;            //A基净值
    double   FundProce_B;               //A基现价
    int      UpdateTime;                //更新时间
    int      PreTradeVolume_M;          //T-1母基交易量
    int      PreTradeVolume_A;          //T-1A基交易量
    int      PreTradeVolume_B;          //T-1B基交易量
    double   FundShares;                //份额
    double   DateIncFundShares;         //本日增加份额
    double   WholePremiumRate;          //T-1整体溢价率
    double   PreWholePremiumRate;       //T-2整体溢价率
    char     SubscribeStatus[68];       //基金申购状态
    char     RedeemStatus[68];          //基金赎回状态
    int      Status;                    //基金状态
    double   PreNetValue;               //T-1基金净值
} ITPDK_FJJJ;
//股东号
typedef struct
{
    char     AccountId[16];    //客户号
    char     Market[4];        //交易所
    char     SecuAccount[12];  //股东号
    char     HolderName[16];   //股东姓名
    char     FundAccount[16];  //资金帐号
    char     OrgCode[8];       //机构代码--结算银行
    char     MoneyType[4];     //币种
    char     TradeAccess[16];  //交易权限
    int      HolderType;       //股东类别
    int      EquityQty;        //权益数量(新股申购)
} ITPDK_GDH;
//配售权益
typedef struct
{
    char     AccountId[16];    //客户号
    char     Market[4];        //交易所
    char     SecuAccount[12];  //股东号
    int64    StarQty;          //科创版数量
    int64    EquityQty;        //证券权益数量
    int      SettleMentDate;   //清算日期
} ITPDK_PSQY;
//资金信息
typedef struct
{
    char     AccountId[16];             //客户号
    char     FundAccount[16];           //资金账号
    char     MoneyType[4];              //币种
    char     OrgCode[8];                //机构代码
    int      MasterFlag;                //主帐户标志
    int      AccountType;               //帐户类别
    double   LastBalance;               //上日余额
    double   CurrentBalance;            //账户余额
    double   FrozenBalance;             //冻结资金
    double   UF_FrozenBalance;          //冻结资金（未完结部分UnFinished）
    double   T2_FrozenBalance;          //T+2冻结金额
    double   FundAvl;                   //可用资金
    double   T2_FundAvl;                //T+2可用资金
    double   TotalAsset;                //总资产
    double   MarketValue;               //最新市值
    double   DebtAmt;                   //负债金额
    double   CreditQuota;               //信用额度
    double   CreditQuotaAvl;            //可用信用额度
    double   UncomeBalance;             //未交收资金
    double   CashBalance;               //现金余额
    double   CashAsset;                 //现金资产
    double   OtherAsset;                //其他资产
    double   FetchBalance;              //可取资金
    double   DateProfit;                //本日盈亏
    double   UnclearProfit;             //浮动盈亏
    double   DiluteUnclearProfit;       //摊薄浮动盈亏
    double   UpdateTime;                //更新时间
    double   SettleBalance;             //实时清算资金
    double   ContractPosiValue;         //合约持仓市值(期权扩展查询)
    double   LastPositionValue;         //昨日持仓市值(期权扩展查询)
} ITPDK_ZJZH;
//资产汇总
typedef struct
{
    char     AccountId[16];          //客户号
    char     MoneyType[4];           //币种
    char     CollectItem[16];        //产品项目
    char     AccountCode[16];        //帐户代码
    double   AssetAmt;               //资产余额
    double   DebtAmt;                //负债余额
    double   NetAsset;               //净资产
    double   NoNegotiableValue;      //非流通市值
    double   UpdateTime;             //更新时间
} ITPDK_ZCXX;
//银行账号
typedef struct
{
    char     AccountId[16];    //客户号
    char     BankAccount[32];  //银行账号
    char     FundAccount[16];  //资金账号
    char     BankCode[8];      //银行代码
    char     BankName[32];     //银行名称
    char     MoneyType[4];     //币种
} ITPDK_YHZH;
//查询银证业务流水入参结构体
typedef struct
{
    char lpKhh[16];     //客户号
    char lpZjzh[16];    //资金账号
    char lpJymm[40];    //交易密码
    char lpScdm[4];     //市场代码
    char lpYyb[5];      //营业部
    char lpBz[4];       //币种
    char lpSqh[30];     //申请号
    char lpYhdm[8];     //银行代码
    char lpYhzh[41];    //银行账号
    int nRowCount;      //查询行数（<=200）
    char lpBrowindex[64];//索引流水号
    int nExFlag;        //是否取辅账户流水（0:否 1:是，默认为 0）
}ITPDK_REQ_YZYWLS;
//查询银证业务流水出参结构体
typedef struct
{
    char    sWTRQ[9];    //发生日期
    char    sWTSJ[13];   //发生时间
    char    sZJZH[16];   //资金账号
    char    sBZ[4];      //币种
    char    sJGDM[8];    //银行代码
    char    sCLJG[30];   //处理结果
    char    sJGSM[128];  //结果说明
    char    sSQH[30];    //申请号
    double  dZHYE;       //账户余额
    char    sYWLB[8];    //业务类别
    char    sYWLBMC[32]; //业务类别名称
    double  dFSJE;       //发生金额
    char    sYHMC[32];   //银行名称
}ITPDK_YZYWLS;
//查询银行代码入参结构体
typedef struct
{
    char lpKhh[16];     //客户号
    char lpJymm[40];    //交易密码
    char lpScdm[4];     //市场代码
    char lpYyb[5];      //营业部
    int nYHLB;          //银行类别
    int nExFlag;        //是否取辅账户流水（0:否 1:是，默认为 0）
    char lpWtfs[5];     //委托方式
    char lpBz[4];       //币种
    char lpYhdm[8];     //银行代码
}ITPDK_REQ_YHDM;
//查询银行代码出参结构体
typedef struct
{
    char    sYHDM[7];    //银行代码
    char    sYHMC[81];   //银行名称
    char    sYHZH[41];   //银行账号
    char    sYHBZ[22];   //银行标志
    char    sBZ[4];      //币种
    char    sZZYMMXX[2];    //证转银密码选项(0：都不输，1：只输资金密码，2：只输银行密码，3：两个密码都要输)
    char    sYZZMMXX[2];    //银转证密码选项(0：都不输，1：只输资金密码，2：只输银行密码，3：两个密码都要输)
    char    sZHH[11];    //主/辅账户号
    char    sZHLB[2];       //账户类别(0-主账户，1-子账户)
    char    sZHLBMC[31]; //账户类别名称
    int     nKSQX;       //是否支持快速取现(1-支持，0 不支持)
    char    sKHH[16];    //客户号
    char    sZJZH[16];   //资金账号
    char    sWBJGDM[8];  //外部机构代码
}ITPDK_YHDM;
//查询银行余额入参结构体
typedef struct
{
    char lpKhh[16];     //客户号
    char lpJymm[40];    //资金账号
    char lpZjzh[16];    //交易密码
    char lpZjmm[40];    //资金密码
    char lpBz[4];       //币种
    char lpYhzh[41];    //银行帐号
    char lpYhdm[8];     //银行代码
    char lpYhmm[40];    //银行密码
    char lpWbzhmm[40];  //外部账号密码
}ITPDK_REQ_YHYE;
//查询银行余额出参结构体
typedef struct
{
    char    sLSH[31];    //流水号
    double  dFSJE;       //账户余额
}ITPDK_YHYE;
//查询新股中签入参结构体
typedef struct
{
    char lpYybdm[5];    //营业部代码
    char lpScdm[4];     //市场代码
    char lpKhh[16];     //客户号
    char lpJymm[40];    //交易密码
    char lpZqdm[9];     //证券代码（为空就查询所有）
    char lpQsrq[9];     //起始日期
    char lpJsrq[9];     //结束日期
    int nRowCount;      //查询行数（<=200）
    char lpSyrq[9];     //索引日期
    char lpSyYybdm[5];  //索引营业部代码
    char lpBrowindex[64];//索引流水号
    char lpWtfs[5];     //委托方式
    char sFXFS[2];      //发行方式
}ITPDK_REQ_XGZQ;
//查询新股中签出参结构体
typedef struct
{
    char    sFSRQ[9];    //发生日期
    char    sKHYYB[5];   //开户营业部
    char    sKHH[16];    //客户号
    char    sGDH[16];    //股东代码
    char    sGDXM[21];   //股东姓名
    char    sSCDM[4];    //市场代码
    char    sSCMC[21];   //市场名称
    char    sZQDM[9];    //证券代码
    char    sZQMC[41];   //证券名称
    int64   nSGSL;       //申购数量
    double  dCJJG;       //成交价格
    double  dCJJE;       //成交金额
    int     nZQSL;       //证券数量
    char    sLSH[31];    //流水号
    char    sKKRQ[9];    //扣款日期
    char    sZQLB[4];    //证券类别
    char    sCLZT[3];    //处理状态
    char    sTQDJBZ;     //提前冻结标志
    double  dSJZQSL;     //实际中签数量
    double  dSJZQJE;     //实际中签金额
    double  dFQSL;       //放弃数量
    double  dFQJE;       //放弃金额
    char    sWTRQ[9];    //委托日期
    int     nPGSL;       //配股数量
    double  dPGJKJE;     //配股缴款金额
    int     nJSSX;       //交收顺序
    char    sFXFS[2];    //发行方式
    char    sLCZT[2];    //流程状态
}ITPDK_XGZQ;
//配号查询入参结构体
typedef struct
{
    char lpYybdm[5];    //营业部代码
    char lpScdm[4];     //市场代码
    char lpKhh[16];     //客户号
    char lpJymm[40];    //交易密码
    char lpGddm[16];    //股东代码
    char lpPhdm[9];     //配号代码
    char lpQsrq[9];     //起始日期
    char lpJsrq[9];     //结束日期
    int nRowCount;      //查询行数（<=200）
    char lpBrowindex[64];//索引流水号
    char lpWtfs[5];     //委托方式
    char lpZjzh[16];    //资金账号
}ITPDK_REQ_PHCX;
//配号查询出参结构体
typedef struct
{
    char    sKHH[16];    //客户号
    char    sSCDM[4];    //市场代码
    char    sSCMC[21];   //市场名称
    char    sGDH[16];    //股东账号
    char    sPHDM[9];    //配号代码
    char    sPHMC[41];   //配号名称
    char    sQSPH[13];   //起始配号
    int64   nPHGS;       //配号个数
    char    sPHRQ[9];    //配号日期
    char    sLSH[31];    //流水号
    char    sCLXX[81];   //处理信息
    char    sWTRQ[9];    //委托日期
}ITPDK_PHCX;
//银证转账入参结构体
typedef struct
{
    char lpKhh[16];     //客户号
    int nYwlb;          //业务类别（YWLB_YHTOZQ银行转证券、YWLB_ZQTOYH证券转银行）
    char lpZjzh[16];    //资金账号
    char lpJymm[40];    //交易密码
    char lpZjmm[40];    //资金密码
    char lpBz[4];       //币种
    char lpYhzh[41];    //银行帐号
    double lfZzje;      //转账金额
    char lpYhdm[8];     //银行代码
    char lpYhmm[40];    //银行密码
}ITPDK_REQ_YZZZ;
//证券持仓
typedef struct
{
    char     AccountId[16];             //客户号
    char     Market[4];                 //交易所
    char     StockCode[12];             //证券代码
    char     SecuAccount[12];           //股东号
    char     FundAccount[16];           //资金账号
    int      AccountType;               //账户类别
    char     MoneyType[4];              //币种
    char     StockName[31];             //证券名称
    int64    CurrentQty;                //今持仓量
    int64    QtyAvl;                    //可卖出数量
    double   LastPrice;                 //最新价
    double   MarketValue;               //最新市值
    double   DateProfit;                //本日盈亏
    double   CostPrice;                 //持仓均价
    double   UnclearProfit;             //浮动盈亏
    double   DividendAmt;               //红利金额
    double   RealizeProfit;             //实现盈亏
    int64    PreQty;                    //昨持仓量
    int64    FrozenQty;                 //冻结数量
    int64    UncomeQty;                 //未交收数量
    double   CostBalance;               //持仓成本
    double   DiluteCostPrice;           //摊薄成本价
    double   KeepCostPrice;             //保本价
    double   AvgBuyPrice;               //买入均价
    int64    AllotmentQty;              //配股数量
    int64    SubscribeQty;              //申购数量
    char     OpenDate[12];              //开仓日期
    double   InterestPrice;             //利息报价
    double   Dilutekeep_CostPrice;      //摊薄保本价
    double   DiluteUnclearProfit;       //摊薄浮动盈亏
    int64    TradeUnit;                 //交易单位
    int64    SubscribableQty;           //可申购数量
    int64    RedeemableQty;             //可赎回数量
    int64    RealSubsQty;               //申购成交数量
    int64    RealRedeQty;               //赎回成交数量
    int64    TotalSellQty;              //累计卖出数量
    int64    TotalBuyQty;               //累计买入数量
    double   TotalSellAmt;              //卖出金额
    double   TotalBuyAmt;               //买入金额
    double   AllotmentAmt;              //配股金额
    int64    RealBuyQty;                //当日买入成交数量
    int64    RealSellQty;               //当日卖出成交数量
    double   RealBuyBalance;            //当日买入成交金额
    double   RealSellBalance;           //当日卖出成交金额
    int64    BrowIndex;                 //分页查询定位串
    int64    OrderFrozenQty;            //委托冻结数量
} ITPDK_ZQGL;
//当日委托
typedef struct
{
    char     AccountId[16];       //客户号
    int64    OrderId;             //委托号
    int64    CXOrderId;           //撤销委托号
    char     SBWTH[17];           //申报委托号
    int64    KFSBDBH;             //开发商本地编号
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    char     StockType[4];        //证券类别
    int      EntrustType;         //交易类别
    double   OrderPrice;          //委托价格
    int64    OrderQty;            //委托数量
    double   MatchPrice;          //成交价格
    int64    MatchQty;            //成交数量
    int64    WithdrawQty;         //撤单数量
    char     SecuAccount[12];     //股东号
    int64    BatchNo;             //委托批次号
    int      EntrustDate;         //委托日期
    int64    SerialNo;            //流水号
    int      OrderType;           //订单类型
    int      OrderType_HK;        //港股订单类型
    double   StopPrice;           //止损限价
    int      OrderStatus;         //申报结果
    char     EntrustNode[48];     //交易站点
    char     EntrustTime[13];     //委托时间
    char     ReportTime[13];      //申报时间
    char     MatchTime[13];       //成交时间
    char     WithdrawFlag[4];     //撤销标志
    char     ResultInfo[128];     //结果说明
    double   MatchAmt;            //成交金额
    double   FrozenBalance;       //冻结资金
    double   BailBalance;         //冻结保证金
    double   HandingFee;          //冻结手继费
    int64    BrowIndex;           //分页查询定位串
} ITPDK_DRWT;
//分笔成交
typedef struct
{
    char     AccountId[16];       //客户号
    char     Market[4];           //交易所
    char     SecuAccount[12];     //股东号
    char     MatchSerialNo[32];   //成交编号
    int64    OrderId;             //委托号
    int      EntrustType;         //交易类别
    char     MatchTime[13];       //成交时间
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    int64    MatchQty;            //成交数量
    double   MatchPrice;          //成交价格
    double   MatchAmt;            //成交金额
    char     MoneyType[4];        //币种
    double   ClearBalance;        //清算金额
    int64    BatchNo;             //委托批次号
    int      EntrustDate;         //委托日期
    int64    BrowIndex;           //起始记录索引值
    char     WithdrawFlag[4];     //撤销标志
    int64    KFSBDBH;             //开发商本地编号
} ITPDK_SSCJ;
//转账申请
typedef struct
{
    char     AccountId[16];    //客户号
    char     OrgCode[8];       //银行代码
    char     BankAccount[32];  //银行帐号
    char     MoneyType[4];     //币种
    char     ReportNo[16];     //申请编号
    char     TransType[8];     //业务类别
    int      ResultCode;       //处理结果
    double   OccurAmt;         //发生金额
    double   FundBalance;      //本次资金余额
    char     ResultInfo[64];   //结果说明
    char     ResultTime[13];   //处理时间
    char     ApplyTime[13];    //申请时间
} ITPDK_ZZSQ;
//交割流水
typedef struct
{
    char     AccountId[16];       //客户号
    int      OccurDate;           //成交日期
    char     browindex[64];       //分页查询定位串
    char     MatchTime[13];       //成交时间
    char     Market[4];           //交易所
    char     StockName[31];       //证券名称
    char     StockCode[12];       //证券代码
    int      EntrustType;         //交易类别
    int64    MatchQty;            //成交数量
    double   MatchPrice;          //成交价格
    double   MatchAmt;            //成交金额
    double   HolderQty;           //股份余额
    double   FundBalance;         //资金余额
    double   clearBalance;        //清算资金
    double   HandingFee;          //佣金
    double   StampTax;            //印花税
    double   TransferFee;         //过户费
    double   FinalValueFee;       //成交费
    double   ClearingFee;         //结算费
    double   RegulatoryFee;       //监管规费
    char     MatchSerialNo[20];   //成交编号
    char     SecuAccount[12];     //股东号
    int64    OrderId;             //委托号
    char     MoneyType[4];        //币种
} ITPDK_JGMX;
//历史委托
typedef struct
{
    char     AccountId[16];       //客户号
    int      OccurDate;           //委托日期
    char     browindex[64];       //分页查询定位串
    char     EntrustTime[13];     //委托时间
    int      OrderId;             //委托号
    char     ResultInfo[64];      //结果说明
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[31];       //证券名称
    int      EntrustType;         //交易类别
    double   OrderPrice;          //委托价格
    int      OrderQty;            //委托数量
    double   MatchPrice;          //成交价格
    int      MatchQty;            //成交数量
    int      WithdrawQty;         //撤单数量
    char     SecuAccount[12];     //股东号
    char     EntrustNode[48];     //委托地址
    int      OrderType;           //订单类型
    int      OrderType_HK;        //港股订单类型
    double   StopPrice;           //止损限价
    int      OrderStatus;         //申报结果
    char     WithdrawFlag[4];     //撤销标志
} ITPDK_WTLS;
//资金流水
typedef struct
{
    int64   SerialNo;           //流水号
    char    AccountId[16];      //客户号
    char    FundAccount[16];    //资金账号
    char    MoneyType[4];       //币种
    int     OccurDate;          //发生日期
    int     FrozenType;         //冻结类别
    double  FrozenBalance;      //冻结金额
    char    ApplyTime[13];      //发生时间
    char    Summary[256];       //摘要
    int64   BrowIndex;          //起始记录索引值
} ITPDK_ZJLS;
//柜台资金流水
typedef struct
{
    char    AccountId[16];      //客户号
    char    FundAccount[16];    //资金账号
    char    MoneyType[5];       //币种
    int     OccurDate;          //发生日期
    char    ApplyTime[13];      //发生时间
    int     FrozenType;         //冻结类别
    double  FrozenBalance;      //冻结金额
    char    Summary[128];       //摘要
    char    BrowIndex[25];      //分页查询定位串
} ITPDK_GTZJLS;
//中签缴款
typedef struct
{
    char     AccountId[16];       //客户号
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     SecuAccount[12];     //股东号
    char     StockName[31];       //证券名称
    int      LuckyQty;            //中签数量
    double   IssuePrice;          //发行价格
    int      PaymentQty;          //已预缴款数量
    int      PaymentQtyAvl;       //可补缴款数量
    int      T3_PaymentQty;       //T+3有效补缴数量
    int      T3_CancelQty;        //T+3确认放弃数量
} ITPDK_ZQJK;

/////////////////////////////接口出入参结构/////////////////////////////
//接口请求 - 客户信息
struct ITPDK_CusReqInfo
{
    char     AccountId[16];       //客户号
    char     SecuAccount[12];     //股东号
    char     Password[40];        //交易密码
    int      TradeNodeID;         //交易节点
    char     EntrustWay[10];       //委托方式
    char     NodeInfo[256];       //操作站点
    char     DevelCode[7];        //开发商代码
    int64    Token;               //登录令牌

    int64    RetCode;             //返回值
    char     ErrMsg[256];         //错误信息
    char     sKZLSH[9];           //扩展流水号
    ITPDK_CusReqInfo()
        :AccountId{ 0 }
        , SecuAccount{ 0 }
        , Password{ 0 }
        , TradeNodeID(-1)
        , EntrustWay{ 0 }
        , NodeInfo{ 0 }
        , DevelCode{ 0 }
        , Token(-1)
        , RetCode(0)
        , ErrMsg{ 0 }
        , sKZLSH{ 0 }
    {
    }
};
//客户节点信息
struct ITPDK_KHJD
{
    char     AccountId[16];       //客户号
    int      SystemType;          //系统类型
    char     Market[4];           //交易所
    int      NodeID;              //节点编号
};
//普通交易批量委托
struct BatchOrderInfo
{
    char    Jys[3];     //交易所
    char    Zqdm[7];    //证券代码
    int     Jylb;       //交易类别(JYLB_BUY、JYLB_SALE)
    double  Wtjg;       //委托价格
    int64   Wtsl;       //委托数量
    int     Ddlx;       //订单类型(DDLX_XJWT,DDLX_SJWT)
    char    Gdh[11];

    //以下为处理结果返回字段
    int64   Wth;        //成功时为委托号>0，失败时为错误码<=0
    char    Msg[128];   //失败信息
};

//ETF篮子股买卖
struct ETFBasket
{
    int64   Wth;           //成功时为委托号>0，失败时为错误码<=0
    char    Cfgdm[10];     //成分股代码
    char    Jys[3];        //交易所
    int64   Wtsl;          //委托数量
    char    Msg[128];      //失败信息
};

//配售权益信息
struct ITPDK_PSQYInfo
{
    char    AccountId[16];      //客户号
    char    Market[4];          //交易所
    char    SecuAccount[12];    //股东号
    int64   BallotQty;          //新股额度
    int64   StartQty;           //科创板额度
};

//一户两地-划拨策略信息
struct ITPDK_ZJHBCL
{
    char    AccountId[16];     //客户号
    char    FundAccount[16];   //资金账号
    char    MoneyType[4];      //币种
    int64   NodeId;            //节点编号
    char    Market[4];         //交易所
    double  Rate;              //费率
};

//一户两地-节点间资金划拨明细
struct ITPDK_JDJZJHBMX
{
    char    AccountId[16];      //客户号
    char    FundAccount[16];    //资金账号
    char    MoneyType[4];       //币种
    int     NodeId1;            //划出节点
    int     NodeId2;            //划入节点
    int     TradeDate;          //交易日期
    int     TransDate;          //划拨日期
    char    ApplyTime[13];      //申请时间
    double  OccurAmt;           //发生金额
    char    Summary[128];       //摘要
    char    BrowIndex[25];      //分页查询定位串
};


#pragma pack(pop)

