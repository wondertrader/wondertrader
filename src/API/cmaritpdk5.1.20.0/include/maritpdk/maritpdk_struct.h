#pragma once
#pragma pack(push,1)
namespace MarItpdk
{


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
   char     MarketingUnit[8]; //交易单元
   int64    HolderType;       //股东类别
} ITPDK_XYGDH;


//配售权益
typedef struct
{
   char     AccountId[16];    //客户号
   char     Market[4];        //交易所
   char     SecuAccount[12];  //股东号
   int      StarQty;          //科创版数量
   int      EquityQty;        //证券权益数量
   int      SettleMentDate;   //清算日期
} ITPDK_XYPSQY;



//两融证券代码查询
typedef struct
{
    char     Market[4];           //交易所
    char     MoneyType[4];        //币种
    char     StockCode[12];       //证券代码
    char     StockName[16];       //证券名称
    char     StockType[4];        //证券类别
    double   PriceTick;           //交易价位
    double   TradeUnit;           //交易单位
    int64    MaxMarketTradeAmt;        //市价委托上限
    int64    MinMarketTradeAmt;        //市价委托下限
    int64      MaxTradeAmt;         //委托上限
    int64      MinTradeAmt;         //委托下限
    double   LastClosePrice;      //昨收盘
    double   HighLimitPrice;      //最高报价--涨停价
    double   LowLimitPrice;       //最低报价--跌停价
    int64      NetPriceFlag;        //国债净价标志
    double   InterestPrice;       //利息报价
    int64     BrowIndex;           //分页查询定位串
   
}ITPDK_XYZQDM;


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
   double 	SettleBalance;				// 实时清算资金
   double   ContractPosiValue;				//合约持仓市值(扩展查询)
   double   LastPositionValue;			//昨日持仓市值(扩展查询)
} ITPDK_XYZJZH;


//证券持仓
typedef struct
{
   char     AccountId[16];             //客户号
   char     Market[4];                 //交易所
   char     StockCode[12];             //证券代码
   char     SecuAccount[12];           //股东号
   char     FundAccount[16];           //资金账号
   int64      AccountType;               //账户类别
   char     MoneyType[4];              //币种
   char     StockName[16];             //证券名称
   int64      CurrentQty;                //今持仓量
   int64      QtyAvl;                    //可卖出数量
   double   LastPrice;                 //最新价
   double   MarketValue;               //最新市值
   double   DateProfit;                //本日盈亏
   double   CostPrice;                 //持仓均价
   double   UnclearProfit;             //浮动盈亏
   double   DividendAmt;               //红利金额
   double   RealizeProfit;             //实现盈亏
   int64   PreQty;                    //昨持仓量
   int64   FrozenQty;                 //冻结数量
   int64    OrderFrozenQty;             //委托冻结数量
   int64   UncomeQty;                 //未交收数量
   double   CostBalance;               //持仓成本
   double   DiluteCostPrice;           //摊薄成本价
   double   KeepCostPrice;             //保本价
   double   AvgBuyPrice;               //买入均价
   int64   AllotmentQty;              //配股数量
   int64   SubscribeQty;              //申购数量
   char     OpenDate[12];              //开仓日期
   double   InterestPrice;             //利息报价
   double   Dilutekeep_CostPrice;      //摊薄保本价
   double   DiluteUnclearProfit;       //摊薄浮动盈亏
   int64      TradeUnit;                 //交易单位
   int64      SubscribableQty;           //可申购数量
   int64      RedeemableQty;             //可赎回数量
   int64      RealSubsQty;               //申购成交数量
   int64      RealRedeQty;               //赎回成交数量
   int64   TotalSellQty;              //累计卖出数量
   int64   TotalBuyQty;               //累计买入数量
   double   TotalSellAmt;              //卖出金额
   double   TotalBuyAmt;               //买入金额
   double   AllotmentAmt;              //配股金额
   int64      RealBuyQty;                //当日买入成交数量
   int64      RealSellQty;               //当日卖出成交数量
   double   RealBuyBalance;            //当日买入成交金额
   double   RealSellBalance;           //当日卖出成交金额
   int64      BrowIndex;                 //分页查询定位串
} ITPDK_XYZQGL;
//当日委托
typedef struct
{
   char     AccountId[16];       //客户号
   int      OrderId;             //委托号
   int      CancelOrderId;       //撤销委托号
   char     SBWTH[17];           //申报委托号
   char     Market[4];           //交易所
   char     StockCode[12];       //证券代码
   char     StockName[16];       //证券名称
   int      EntrustType;         //交易类别
   double   OrderPrice;          //委托价格
   int      OrderQty;            //委托数量
   double   MatchPrice;          //成交价格
   int   MatchQty;            //成交数量
   int      WithdrawQty;         //撤单数量
   char     SecuAccount[12];     //股东号
   int      BatchNo;             //委托批次号
   int      EntrustDate;         //委托日期
   int      SerialNo;            //流水号
   int      OrderType;           //订单类型
   int      OrderType_HK;        //港股订单类型
   double   StopPrice;           //止损限价
   int      OrderStatus;         //申报结果
   char     EntrustNode[48];     //交易站点
   char     EntrustTime[13];     //委托时间
   char     ReportTime[13];      //申报时间
   char     MatchTime[13];       //成交时间
   char     KfsOrderNum[24];       //开发商报单编号
   char     WithdrawFlag[4];     //撤销标志
   char     ResultInfo[128];     //结果说明
   double   MatchAmt;            //成交金额
   double   FrozenBalance;       //冻结资金
   double   BailBalance;         //冻结保证金
   double   HandingFee;          //冻结手继费
   int      BrowIndex;           //分页查询定位串
} ITPDK_XYDRWT;

//分笔成交
typedef struct
{
   char     AccountId[16];       //客户号
   int64      ReportSerialNo;      //回报序号
   char     Market[4];           //交易所
   char     SecuAccount[12];     //股东号
   char     MatchSerialNo[32];   //成交编号
   int64      OrderId;             //委托号
   int64      EntrustType;         //交易类别
   char     MatchTime[13];       //成交时间
   char     StockCode[12];       //证券代码
   char     StockName[16];       //证券名称
   char     KfsOrderNum[24];       //开发商报单编号
   int64   MatchQty;            //成交数量
   double   MatchPrice;          //成交价格
   double   MatchAmt;            //成交金额
   char     MoneyType[4];        //币种
   double   clearBalance;        //清算金额
   int64      BatchNo;             //委托批次号
   int64      EntrustDate;         //委托日期
   int64      BrowIndex;           //起始记录索引值
   char     WithdrawFlag[4];     //撤销标志
} ITPDK_XYSSCJ;


//资格证券
typedef struct
{
    char     Market[4];           //交易所
    char     StockCode[12];       //证券代码
    char     StockName[16];       //证券名称
    double   ConversionRate;      //折算率
    double   MarginBailRate;      //融资保证金比例
    double   ShortBailRate;       //融券保证金比例
    int      StockProperty;       //证券属性 1	融资标的, 2	融券标的, 4	担保物, 7 融资融券担保标的
    int      TradeStatus;         //交易状态  0-正常，1 禁止融资 2 禁止融券 3 禁止融资融券
    int      CollateralStatus;    //担保状态 0-正常 1-暂停 2-作废 3-同标准设置
    int      Type;                //类型，0-公司，1 个人
    int64    BrowIndex;           //分页查询定位串
} ITPDK_ZGZQ;

//客户可融券信息
typedef struct
{
    char        AccountId[16];       //客户号
    int32       nTCXZ;      //头寸性质（1普通头寸 2专项头寸）
    char        szTCBH[21];  //头寸编号
    char        sJYS[3];    //交易所
    char        sZQDM[11];  //证券代码
    char        sZQMC[21];  //证券名称
    int64       nZQSL;      //证券数量
    int64       nDJSL;      //冻结数量
    int64       nMCWTSL;    //卖出委托数量
    int64       nDRCHSL;    //当日偿还数量
    int64       nRCSL;      //已融出数量
    int64       nYYSL;      //预约数量
    int64       nDRCJSL;    //当日成交数量
    int64       nKYSL;      //可用数量
    int64       nHCCHSL;    //划出偿还数量
    float64     dRQBL;      //融券比例
    float64     dZSL;       //折算率
    int32       nJYZT;      //交易状态
    int64       BrowIndex;           //分页查询定位串

} ITPDK_RQZQ;

//客户可融资金信息
typedef struct
{
    char        AccountId[16];       //客户号
    int32       nTCXZ;      //头寸性质（1普通头寸 2专项头寸）
    char        szTCBH[21];  //头寸编号
    double      dTCGM;      //头寸规模
    double      dYRCJE;     //已融出金额
    double      dYYJE;      //预约金额
    double      dRCDJJE;    //当日融出冻结金额
    double      dDRCHJE;    //当日偿还金额
    double      dDJJE;      //临时冻结金额
    double      dKYED;      //可用金额

} ITPDK_KRZJXX;

//信用资产
typedef struct
{
    char   AccountId[16];                //客户号
    double dKYBZJ;       //可用保证金
    double dDBBL;       //担保比例
    double dYJBL1;       //预计维持担保比例1，考虑担保物划入划出
    double dYJBL2;         //预计维持担保比例2，考虑担保物划出，不考虑划入
    double dZHYE;       //账户余额
    double dKYZJ;       //可用资金
    double dQSZJ;       //清算资金
    double dKMDBPZJ;	//可买担保品资金

    double dZQSZ_BD;    //标的证券市值
    double dZQSZ;		    //证券市值_计算担保比例使用
    double dZQSZ_DB;    //担保证券市值
    double dZQSZ_DBZS;	//担保证券折算市值_计算可用保证金使用(已扣融资)
    double dZQSZ_DB_BD;//担保证券变动折算市值_计算可用保证金使用
    double dZQSZ_DBZS_BD;//担保证券变动折算市值_计算可用保证金使用
    double dZQSZ_YJZC;          //担保物返回未成交_预估资产

    double dRZFZ;       //融资负债
    double dRQFZ;       //融券负债
    double dRZFZ2;      //融资负债2
    double dRQFZ2;      //融券负债2
    double dHQWT2;      //还券委托未成
    double dXJCE;       //应该追补到比例线的资金差额
    double dPCJE;       //强制平仓资金 = (总负债*平仓到位比例 - 总资产)/(平仓到位比例 -1)
    double dZDPCJE;     //主动平仓资金 = (总负债*追保到位比例 - 总资产)/(追保到位比例 -1)
    double dKQBZ;       //可提资产标准
    double dKQZJ;       //可取资金

    double dRZLX;       //尚欠利息_融资
    double dRQLX;       //尚欠利息_融券
    double dYJLX;       //预计利息/费用
    double dZFZ;        //总负债
    double dZZC;        //总资产
    double dJZC;        //净资产
    //double dZYBZJ;    //占用保证金
    double dZYBZJ_RZ;   //占用保证金_融资
    double dZYBZJ_RQ;   //占用保证金_融券
    double dRZYK;       //融资盈亏
    double dRQYK;       //融券盈亏
    double dRQSYZJ;     //融券剩余资金
    double dRZSXF;      //融资费用
    double dRQSXF;      //融券费用
    double dDBSZ_SH;    //上海担保证券市值
    double dDBSZ_SZ;    //深圳担保证券市值
    double dRZFYZS;     //融资浮盈折算
    double dRZFK;       //融资浮亏
    double dRQFYZS;     //融券浮盈折算
    double dRQFK;       //融券浮亏
    double dRQMCJE;     //融券金额_负债

    double dRZDQSZ;     //融资当前市值
    double dRQDQSZ;     //融券当前市值

    double dSYZQSZ;     //所有证券市值
						
						
						// FALG 送1  扩展字段
    int    nJSRQ;		//合同到期日
    double dRZED;		//融资授信额度
    double dRQED;       //融券授信额度
    double dRZZY;       //融资已用额度
    double dRQZY;       //融券已用额度
    double dHTED;       //合同额度
    double dQTZY;		//其它占用
    double dKYDBZJ;		//可买担保品资金
    double dQTFY;       //其他费用
    double dZZCJZD;     //持仓占总资产集中度
    double dJZCJZD;     //净资产集中度
    double dZZCJZD_BK;  //科创板+创业板板块持仓占总资产集中度
    double dJZCJZD_BK;  //科创板+创业板板块净持仓集中度
    double dZZCJZD_BKCDR;//科创板+创业板+科创板CDR+创业板CDR板块持仓占总资产集中度

    double dRZFZ_PT;       //融资负债(普通头寸)
    double dRZSXF_PT;      //融资费用(普通头寸)
    double dRZLX_PT;       //尚欠利息_融资(普通头寸)

    double dRQMCQSJE;      //融券卖出冻结清算金额（按实际成交的统计）
    double dBY1;           //备用1
    double dBY2;           //备用2
   
} ITPDK_XYZC;
//信用合约
typedef struct
{
    char    AccountId[16];             //客户号
    char    m_szHYBH[25];  // HYBH 合约编号
    char    m_szHTBH[17];  // HTBH 合同编号
    int   	m_nWTH;        // WTH	订单编号
    int   	m_nFSRQ;       // FSRQ	发生日期
    char    m_szFSSJ[13];  // FSSJ 发生时间
    int     m_nJYRQ;       // JYRQ  交易日期
    int     m_nDQRQ;		// 到期日期
    int     m_nHYQX;        // 合约期限
    int     m_nQXRQ;        // 起息日期
    int     m_nHYLB;        // 合约类别
    int     m_nHYLX;        // 合约类型
    char    m_szJYS[3];    // JYS 交易所
    char    m_szGDH[11];      //股东号
    char    m_szZQDM[11];  // ZQDM 证券代码
    char    m_szZQMC[21];  // ZQMC 证券名称
    int     m_nWTSL;       // WTSL 委托数量
    double  m_dWTJG;     // WTJG	数托价格
    int     m_nCJSL;       // CJSL	成交数量
    double  m_dCJJE;     // CJJE	成交金额
    double  m_dCJJG;     // CJJG 	成交价格
    double  m_dSXF;      // SXF	成交手续费
    double  m_dYCHJE;    // YCHJE	 已偿还合约金额
    double  m_dYCHSXF;   // YCHSXF 已偿还交易费用
    int     m_nYCHSL;      // YCHSL 已偿还数量
    double  m_dYCHLX;    // YCHLX	已偿还利息
    double  m_dYCHFX;    // YCHFX	已偿还罚息
    double  m_dQCHYJE;   // QCHYJE	期初合约金额
    double  m_dQCSXF;    // QCSXF	期初合约交易费用
    double  m_dQCLX;     // QCLX	期初合约利息
    double  m_dQCFX;     // QCFX	期初罚息金额
    int     m_nQCSL;       // QCSL	期初合约数量
    double  m_dSSHYJE;   // SSHYJE	实时合约金额
    double  m_dSSSXF;    // SSSXF	实时合约交易费用
    double  m_dSSLX;     // SSLX	实时合约利息
    double  m_dSSFX;     // SSFX	实时罚息金额
    int     m_nSSSL;       // SSSL	实时合约数量
    double  m_dHYLL;     // HYLL	利率/费率
    double  m_dFXFL;     // FXFL	罚息利率
    double  m_dLXJS;     // LXJS	利息基数
    double  m_dFDLX;     // FDLX	分段利息
    double  m_dYJLX;     // YJLX	预计利息
    double  m_dFXLXJS;   // FXLXJS	罚息利息积数
    double  m_dFXYJLX;   // FXYJLX	罚息预计利息
    double  m_dFXFDLX;   // FXFDLX	罚息分段利息
    double  m_dBZJBL;    // BZJBL	保证金比例
    int     m_nZQCS;       // ZQCS	展期次数
    int     m_nEDDJBS;     // EDDJBS额度冻结标识
    char    m_szYHYBH[25];           // YHYBH	原合约编号
    char    m_szTCBH[21];             // TCBH	头寸编号
    int     m_nBCSL;       // BCSL	补偿数量
    double  m_dBCJE;     // BCJE	补偿金额
    int     m_nLXLJFS;     // LXLJFS	利息了结方式
    int     m_nFLBH;       // FLBH	费率编号
    char    m_szDQRQHYBH[29];        // DQRQHYBH
    int     m_nQKBZ;       // QKBZ 欠款标志
    int     m_nQQBZ;       // QQBZ	欠券标志
    int     m_nZT;			//检查负债了结状态：0-未了结  1-已了结
    char    m_szBrowIndex[25];//分页索引值
} ITPDK_XYFZ;
//当日信用合约变动
typedef struct
{
    char    AccountId[16];       //客户号
    int     m_nLSH;        // LSH 流水号
    char    m_szHYBH[25];  // HYBH 合约编号
    int     m_nWTH;        // WTH	订单编号
    int     m_nHBXH;       // HBXH  回报序号
    char    m_szJYS[3];    // JYS	交易所
    char    m_szZQDM[11];  // ZQDM	证券代码
    int     m_nJYRQ;       // JYRQ	交易日期
    int     m_nFSRQ;       // FSRQ	发生日期
    char    m_szFSSJ[13];  // FSSJ	发生时间
    int     m_nHYBDLB;     // HYBDLB 合约变动类别
    int     m_nJYLB;       // JYLB	交易类别
    int     m_nHYLB;       // HYLB	合约类别
    char    m_szHYZQDM[11];// HYZQDM	合约证券代码
    char    m_szHYJYS[3];  // HYJYS		合约交易所
    int     m_nFSHYSL;     // FSHYSL	发生合约数量
    int     m_nHYSL;       // HYSL		后余数量
    double  m_dFSHYJE;     // FSHYJE	发生合约金额
    double  m_dHYJE;       // HYJE		后余金额
    double  m_dFSFY;       // FSFY		发生费用
    double  m_dHYFY;       // HYFY		后余费用
    double  m_dFSLX;       // FSLX		发生利息
    double  m_dHYLX;       // HYLX		后余利息
    double  m_dFSFX;       // FSFX		发生罚息
    double  m_dHYFX;       // HYFX		后余罚息
    int     m_nZDBZ;       // ZDBZ		指定还款标志
    int     m_nQPBZ;       // QPBZ		强平标志
    char    m_szYHYBH[25];    // YHYBH	原合约编号
    char    m_szTCBH[21];      // TCBH	头寸编号
    char    m_szZY[129];      // ZY		摘要
    int     m_nCXBZ;         // CXBZ	撤销标志
    int     m_nQSBZ;         // QSBZ	清算标志
} ITPDK_XYDRBD;
//当日信用融券负债汇总
typedef struct
{
    char    AccountId[16];       //客户号
    char    m_szJYS[3];         //交易所
    char    m_szZQDM[11];       //证券代码
    int     m_nFZSL;            //负债数量
    int     m_nDRRQCJSL;        //当日融券成交数量
    int     m_nHQWTSL;          //还券委托数量
    int     m_nHQCJSL;          //还券成交数量
    int     m_nYQWTSL;          //余券委托数量
    int     m_nYQSL;            //余券数量
    int     m_nBrowIndex;       //分页索引
} ITPDK_XYRQFZHZ;

//A股可划拨数量查询
typedef struct
{
	char    AccountId[16];  //客户号
	char    m_szJYS[3];     //交易所
	char    m_szGDH[12];     //股东号
	char    m_szZQDM[11];   //证券代码
	char    m_szZQMC[21];	//证券名称
	int64     m_nKWTSL;     //可划拨数量
	int64     m_nKMCSL;     //可卖出数量
	int64     m_nMRCJSL;    //当天买入数量

} ITPDK_XYDBHRSL;

//专项头寸证券调拨
typedef struct
{
    int64   m_nSQH;         //申请号
    int64   m_nLSH;         //流水号
    int64   m_nJCCL;        //今持仓量
    int64   m_nKMCSL;       //可卖出数量
    int64   m_nBDSL;        //变动数量
    int64   m_nFSSL;        //发生数量
}ITPDK_ZXTCZQDB;

//查询证券黑名单
typedef struct
{
    int64   m_nTYPE;        //类型
    char    m_szJYS[3];     //交易所
    char    m_szZQZBZ[64];  //证券指标值 ,在这里是证券代码
    int64   m_nJYLB;        //交易类别
    char    m_szZQLB[4];    //证券类别
    char    m_szXYSX[4];    //信用属性 "01" - 集中度  "02" - 黑名单
    char    m_szXYSXZ[64];  //信用属性值，在这里是交易限制，即交易类别拼接的字符串
}ITPDK_ZQHMD;

//本日新股申购
typedef struct
{
    char     m_szJYS[4];        //交易所
    char     m_szZQDM[12];      //证券代码
    char     m_szZQMC[16];      //证券名称
    int      m_nFXFS;           //发行方式
    double   m_dRGJG;           //认购价格
    char     m_szRGDM[12];      //认购代码
    int      m_nRGRQ;           //认购日期
    int      m_nWTSX;           //委托上限
    int      m_nBDRQ;           //变动日期
} ITPDK_XGSG;

//负债变动明细
typedef struct
{
    char    AccountId[16];      // KHH      客户号
    int     m_nLSH;             // LSH      流水号
    char    m_szHYBH[25];       // HYBH     合约编号
    int     m_nWTH;             // WTH	    订单编号
    int     m_nHBXH;            // HBXH     回报序号
    char    m_szJYS[3];         // JYS	    交易所
    char    m_szZQDM[11];       // ZQDM	    证券代码
    int     m_nJYRQ;            // JYRQ	    交易日期
    int     m_nFSRQ;            // FSRQ	    发生日期
    char    m_szFSSJ[13];       // FSSJ	    发生时间
    int     m_nHYBDLB;          // HYBDLB   合约变动类别
    int     m_nJYLB;            // JYLB	    交易类别
    int     m_nHYLB;            // HYLB	    合约类别
    char    m_szHYZQDM[11];     // HYZQDM	合约证券代码
    char    m_szHYJYS[3];       // HYJYS	合约交易所
    int     m_nFSHYSL;          // FSHYSL	发生合约数量
    int     m_nHYSL;            // HYSL		后余数量
    double  m_dFSHYJE;          // FSHYJE	发生合约金额
    double  m_dHYJE;            // HYJE		后余金额
    double  m_dFSFY;            // FSFY		发生费用
    double  m_dHYFY;            // HYFY		后余费用
    double  m_dFSLX;            // FSLX		发生利息
    double  m_dHYLX;            // HYLX		后余利息
    double  m_dFSFX;            // FSFX		发生罚息
    double  m_dHYFX;            // HYFX		后余罚息
    int     m_nZDBZ;            // ZDBZ		指定还款标志
    int     m_nQPBZ;            // QPBZ		强平标志
    char    m_szYHYBH[25];      // YHYBH	原合约编号
    char    m_szTCBH[21];       // TCBH	    头寸编号
    char    m_szZY[129];        // ZY		摘要
    int     m_nCXBZ;            // CXBZ	    撤销标志
    int     m_nQSBZ;            // QSBZ	    清算标志
    int     m_nBrowIndex;       // BrwoIndex 分页索引
} ITPDK_FZBDMX;

//融资仓单明细查询（光大）
typedef struct
{
    char    m_sKCLSH[13];       //开仓流水号
    char    m_sKCRQ[9];         //开仓日期
    char    m_sCDDQR[9];        //仓单到期日
    char    m_sZQDM[9];         //证券代码
    char    m_sZQMC[9];         //证券名称
    double  m_dRZMRJE;          //融资买入金额
    double  m_dYCHJE;           //已偿还金额
    double  m_dRZFZJE;          //融资负债金额
    double  m_dRZMRJ;           //融资买入价
    double  m_dRZFZSZ;          //融资负债市值
    double  m_dRZLXYJ;          //融资利息已计
    double  m_dRZLXYH;          //融资利息已还
    double  m_dYQFXYJ;          //逾期罚息已计
    double  m_dYQFXYH;          //逾期罚息已还
    int     m_nCDZQCS;          //仓单展期次数
    double  m_dHYNLL;           //合约年利率
    char    m_sLJBS[2];         //了结标识
    double  m_dWJXF;            //未结息费
    double  m_dYJWFXF;          //已结未付息费
    double  m_dRZSL;            //融资数量
}ITPDK_RZCDMX;

//融券仓单明细查询（光大）
typedef struct
{
    char    m_sKCLSH[13];       //开仓流水号
    char    m_sKCRQ[9];         //开仓日期
    char    m_sCDDQR[9];        //仓单到期日
    char    m_sZQDM[9];         //证券代码
    char    m_sZQMC[9];         //证券名称
    int     m_dRQMCGS;          //融券卖出股数
    int     m_dYCHGS;           //已偿还股数
    double  m_dRQFZGS;          //融券负债股数
    double  m_dRQMCJ;           //融券卖出价
    double  m_dRQFZSZ;          //融券负债市值
    double  m_dRQFYYJ;          //融券费用已计
    double  m_dRQFYYH;          //融券费用已还
    double  m_dYQFXYJ;          //逾期罚息已计
    double  m_dYQFXYH;          //逾期罚息已还
    char    m_sQYBCFS[30];      //权益补偿方式(第一位标示“配股偿还标志”，第二位标示“配债偿还标志”，第三位标示“派发权证偿还标志)
                                //标志含义：0或空格:提前了结  1:现金补偿
    int     m_nCDZQCS;          //仓单展期次数
    double  m_dHYNLL;           //合约年利率
    double  m_dWJXF;            //未结息费
    double  m_dYJWFXF;          //已结未付息费
}ITPDK_RQCDMX;

//仓单变更申请拒绝原因查询（光大）
typedef struct
{
    char    m_sSQRQ[9];         //申请日期
    char    m_sKHH[16];         //客户号
    char    m_sSQXH[13];        //申请序号
    char    m_sCDLSH[13];       //仓单流水号
    char    m_sCDFSR[9];        //仓单发生日
    char    m_sCDLX[2];         //仓单类型（1:融资  2:融券）
    char    m_sTZLX[2];         //调整类型（0:利率调整  1:仓单展期）
    char    m_sYYB[4];          //营业部
    char    m_sJYS[3];          //市场代码
    char    m_sZQDM[9];         //证券代码
    char    m_sJJYY[255];       //拒绝原因
    char    m_sLSSCSJ[27];      //流水生成时间
}ITPDK_CDBGSQJJYYCX;

//仓单变更申请查询（光大）
typedef struct
{
    char    m_sSQRQ[9];         //申请日期
    char    m_sKHH[16];         //客户号
    char    m_sSQLSH[11];       //申请流水号
    char    m_sTZLX[2];         //调整类型（0:利率调整  1:仓单展期）
    char    m_sHYLX[2];         //合约类型（1:融资  2:融券）
    char    m_sYYB[4];          //营业部
    char    m_sSQZG[7];         //申请职工
    char    m_sSQSJ[9];         //申请时间
    char    m_sSHZG[7];         //审核职工
    char    m_sSHZT[2];         //审核状态（0:待处理  1:通过  2:拒绝  3:处理失败）
    char    m_sSHRQ[9];         //审核日期
    char    m_sSHSJ[9];         //审核时间
    char    m_sKCKSRQ[9];       //开仓开始日期
    char    m_sKCJSRQ[9];       //开仓结束日期
    char    m_sJYS[3];          //交易所
    char    m_sZQDM[9];         //证券代码
    int     m_dCDBS;            //仓单笔数
    int     m_dCLBS;            //处理笔数
    char    m_sFJSJ[41];        //附加数据（利率调整时存放新产品编号，展期时存放预展期天数）
    char    m_sWTLY[81];        //委托来源
    char    m_sBZ[256];         //备注（审核状态为 3:处理失败 时记录失败原因）
}ITPDK_CDBGSQCX;

//新股中签查询（光大）
typedef struct
{
    char    m_sFSRQ[9];         //发生日期
    char    m_sKHYYB[5];        //开户营业部
    char    m_sKHH[11];         //客户号
    char    m_sGDH[16];         //股东代码
    char    m_sGDXM[21];        //股东姓名
    char    m_sSCDM[2];         //市场代码
    char    m_sSCMC[21];        //市场名称
    char    m_sZQDM[9];         //证券代码
    char    m_sZQMC[41];        //证券名称
    int     m_nZQSL;            //中签数量
    double  m_dCJJG;            //成交价格
    double  m_dCJJE;            //成交金额
    int     m_nZQGS;            //中签个数
    char    m_sLSH[31];         //流水号
    char    m_sKKRQ[9];         //扣款日期
    char    m_sZQLB[4];         //证券类别（00:其他，01::股票，02:债券）
    char    m_sCLZT[3];         //处理状态（0:未处理，1:全部中签扣款，2:部分中签扣款，3:全部放弃）
    char    m_sTQDJBZ[2];       //提前冻结标志（0:未预冻结，1:预冻结，2:中签处理）
    double  m_dSJZQSL;          //实际中签数量
    double  m_dSJZQJE;          //实际中签金额
    double  m_dFQSL;            //放弃数量
    double  m_dFQJE;            //放弃金额
}ITPDK_XGZQ;

//配号查询（光大）
typedef struct
{
    char    m_sSCDM[2];         //市场代码
    char    m_sSCMC[21];        //市场名称
    char    m_sGDH[16];         //股东账号
    char    m_sPHDM[9];         //配号代码
    char    m_sPHMC[41];        //配号名称
    char    m_sQSPH[13];        //起始配号
    int     m_nPHGS;            //配号个数
    char    m_sPHRQ[9];         //配号日期
    char    m_sLSH[31];         //流水号
    char    m_sCLXX[256];       //处理信息
}ITPDK_PHCX;

//查询银证业务流水（光大）
typedef struct
{
    char    m_sYWLB[3];         //转账类别
    char    m_sYWLBMC[21];      //转账类别名称
    double  m_dFSJE;            //金额
    char    m_sCLJG[13];        //转账返回代码
    char    m_sJGSM[256];       //转账确认信息
    char    m_sLSH[16];         //流水号
    char    m_sYHMC[81];        //银行名称
    char    m_sYHDM[7];         //银行代码
    char    m_sWTSJ[9];         //转账时间
    char    m_sWTRQ[9];         //发生日期
}ITPDK_YZYWLS;

//查询银行代码（光大）
typedef struct
{
    char    m_sYHDM[7];         //银行代码
    char    m_sYHJC[81];        //银行简称
    char    m_sYHZH[41];        //银行账号
    char    m_sYHBZ[22];        //银行标志
    char    m_sHBDM[4];         //货币代码
    char    m_sZZYMMXX[2];      //证券转银行密码选项（0:都不输，1:只输资金密码，2:只输银行密码，3:两个密码都要输）
    char    m_sYZZMMXX[2];      //银行转证券密码选项（0:都不输，1:只输资金密码，2:只输银行密码，3:两个密码都要输）
    char    m_sZHH[11];         //主/辅账户号
    char    m_sZHLB[2];         //账户类别（0:主账户，1:子账户）
    char    m_sZHLBMC[31];      //账户类别名称
    int     m_nKSQX;            //是否支持快速取现（0:不支持，1:支持）
}ITPDK_YHDM;

//查询银行余额（光大）
typedef struct
{
    char    m_sLSH[16];         //流水号
    double  m_dYHYE;            //银行余额           
}ITPDK_YHYE;

//查询客户信息
typedef struct
{
    char    m_sKHH[16];         //客户号
    char    m_sKHXM[32];        //客户姓名
    char    m_sYYB[8];          //营业部
    int     m_nWTFS;            //委托方式
}ITPDK_KHXX;

//查询资金调拨明细
typedef struct
{
    char    m_sKHH[16];         //客户号
    int64   m_nLSH;             //流水号
    char    m_sZJZH[15];        //资金账号
    char    m_sBZ[4];           //币种
    int     m_nFSRQ;            //发生日期
    char    m_sFSSJ[13];        //发生时间
    int     m_nDJLB;            //冻结类别
    double  m_dDJJE;            //冻结金额
    int64   m_nBrowIndex;       //分页索引值
    char    m_sZY[61];          //摘要
}ITPDK_ZJDJMX;

//A5资格证券
typedef struct
{
    char    m_sJYS[4];          //交易所
    char    m_sZQDM[12];        //证券代码
    char    m_sZQMC[25];        //证券名称
    int     m_nRZZT;            //融资状态
    int     m_nBZRZZT;          //标准融资状态
    double  m_dBZRZBL;          //标准融资比例
    double  m_dRZBL;            //融资保证金比例
    int     m_nRQZT;            //融券状态
    int     m_nBZRQZT;          //标准融券状态
    double  m_dBZRQBL;          //标准融券比例
    double  m_dRQBL;            //融券保证金比例
    int     m_nZQQYBZ;          //允许展期
    int     m_nZQCS;            //最大展期次数
    int     m_nQTSX;            //其他属性
    int     m_nFXKZSX;          //风险控制属性
    int     m_nYXRQ;            //有效日期
    int     m_nDJRQ;            //登记日期
    int     m_nBDRQ;            //修改日期 
    char    m_sBrowIndex[10];   //分页查询定位串
} ITPDK_A5ZGZQ;

//A5担保证券
typedef struct
{
    char    m_sJYS[4];          //交易所
    char    m_sZQDM[12];        //证券代码
    char    m_sZQMC[25];        //证券名称
    int     m_nJYZT;            //交易状态
    int     m_nBZJYZT;          //标准交易状态
    double  m_dDBZSL;           //担保折算率
    double  m_dBZZSL;           //标准折算率
    int     m_nGYQYBZ;          //是否启用公允价
    double  m_dGYJG;            //公允价格
    int     m_nQTSX;            //其他属性
    int     m_nFXKZSX;          //风险控制属性
    int     m_nYXRQ;            //有效日期
    int     m_nDJRQ;            //登记日期
    int     m_nBDRQ;            //修改日期 
    char    m_sBrowIndex[10];   //分页查询定位串
} ITPDK_A5DBZQ;

/////////////////////////////接口出入参结构/////////////////////////////
//接口请求 - 客户信息
struct ITPDK_CusReqInfo
{
    char     AccountId[16];       //客户号
    char     FundAccount[16];      //资金号
    char     SecuAccount[12];     //股东号
    char     Password[40];        //交易密码
    char     OrgCode[8];          //机构代码
    int      TradeNodeID;         //交易节点
    char     EntrustWay[5];       //委托方式
    char     NodeInfo[256];       //操作站点
    char     OppSecuAccount[12];      //对方股东号
    char     OppSeat[12];           //对方席位
    int64    Token;               //登录令牌

    int64    RetCode;             //返回值
    char     ErrMsg[256];         //错误信息

    ITPDK_CusReqInfo()
        :AccountId{ 0 }
        , FundAccount{ 0 }
        , SecuAccount{ 0 }
        , Password{ 0 }
        , OrgCode{ 0}
        , TradeNodeID(-1)
        , EntrustWay{ 0 }
        , NodeInfo{ 0 }
        , OppSecuAccount{ 0 }
        , OppSeat{ 0 }
        , Token(-1)
        , RetCode(0)
        , ErrMsg{ 0 }
    {
    }
};

};  //MarItpdk
#pragma pack(pop)
