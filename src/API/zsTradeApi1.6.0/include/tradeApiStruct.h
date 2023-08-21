#if !defined(__ZS_STK_TRADE_API_STRUCT_H__)
#define __ZS_STK_TRADE_API_STRUCT_H__

#include "baseDefine.h"

//-------------------------------策略交易注册------------------------------------
struct STReqAcctRegister
{
    char          szUserName[16];               // 用户名
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspAcctRegister
{
    char          szSessionId[128];             // 会话凭证
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chLoginStatus;                // 登录状态
    int           iAllowStrategy;               // 是否允许策略交易
};

//-------------------------------资产账户登录------------------------------------
struct STReqTradeLogin
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chAcctType;                   // 账户类型 '0':股票 '1':信用
    char          szAuthData[64];               // 认证数据-密码
};

struct STRspTradeLogin
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

//-------------------------------用户登录------------------------------------
struct STReqLogin
{
    char          szUserName[16];               // 用户名
    char          chLoginType;                  // 类型 'X':登出 其他：登录
    char          szPassword[256];              // 密码
};

struct STRspLogin
{
};

//-------------------------------股东账户查询--------------------------
struct STReqQryHolder
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspQryHolder
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szTrdacct[10 + 1];            // 证券账户
};

//-------------------------------委托下单------------------------------------
struct STReqOrder
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块 SZ SH
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    int           iStkBiz;                      // 证券业务 买(100)卖(101)
    short         iStrategySn;                  // 策略编号
};

struct STRspOrder
{
    int           iOrderBsn;                    // 委托批号
    char          szOrderId[10 + 1];            // 合同序号
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 冻结金额
    char          szStkbd[2 + 1];               // 交易板块 SZ SH
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    int           iStkBiz;                      // 证券业务 买(100)卖(101)
    char          szTrdacct[10 + 1];            // 证券账户
    short         iStrategySn;                  // 策略编号
};

//-------------------------------委托撤单------------------------------------
struct STReqCancelOrder
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块  SZ SH
    char          szOrderId[10 + 1];            // 合同序号
    int           iOrderBsn;                    // 委托批号
};

struct STRspCancelOrder
{
    int           iOrderBsn;                    // 委托批号
    char          szOrderId[10 + 1];            // 合同序号
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 冻结金额
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    int           iStkBiz;                      // 证券业务 买(100)卖(101)
    char          szTrdacct[10 + 1];            // 证券账户
    char          szMsgOk[32 + 1];              // 内撤信息
    char          szCancelList[256 + 1];        // 撤单列表
};

//-------------------------------委托查询--------------------------
struct STReqQryOrder
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chQueryFlag;                  // 查询方向
    char          szQueryPos[32 + 1];           // 定位串
    int           iQueryNum;                    // 查询行数 最大值1000
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderId[10 + 1];            // 合同序号
    int           iOrderBsn;                    // 委托批号
    char          szTrdacct[10 + 1];            // 交易账户
    short         iStrategySn;                  // 策略编号
    char          chFlag;                       // 查询标志 '0':查询撤单委托信息 '1':查询正常委托信息 其他:全部
};

struct STRspQryOrder
{
    char          szQryPos[32 + 1];             // 定位串
    char          szOrderId[10 + 1];            // 合同序号
    char          chOrderStatus;                // 委托状态
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
    char          szOrderUfzAmt[21 + 1];        // 委托解冻金额
    LONGLONG      llWithdrawnQty;               // 已撤单数量
    LONGLONG      llMatchedQty;                 // 已成交数量
    char          chIsWithdraw;                 // 撤单标志
    char          chIsWithdrawn;                // 已撤单标志
    int           iStkBiz;                      // 证券业务 买(100)卖(101)
    int           iOrderBsn;                    // 委托批号
    char          szRawOrderId[10 + 1];         // 原合同序号
    short         iStrategySn;                  // 策略编号
    char          szOrderTime[32 + 1];          // 委托时间
    char          szTrdacct[10 + 1];            // 交易账户
};

//-------------------------------可撤单委托查询------------------------
struct STReqQryWithdrawableOrder
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderId[10 + 1];            // 合同序号
    short         iStrategySn;                  // 策略编号
    int           iOrderBsn;                    // 委托批号
};

struct STRspQryWithdrawableOrder
{
    char          szOrderId[10 + 1];            // 合同序号
    char          chOrderStatus;                // 委托状态
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderPrice[21 + 1];         // 委托价格
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
    char          szOrderUfzAmt[21 + 1];        // 委托解冻金额
    char          szOrderTime[32 + 1];          // 委托时间
    LONGLONG      llWithdrawnQty;               // 已撤单数量
    LONGLONG      llMatchedQty;                 // 已成交数量
    char          chIsWithdraw;                 // 撤单标志
    char          chIsWithdrawn;                // 已撤单标志
    int           iStkBiz;                      // 证券业务
    int           iOrderBsn;                    // 委托批号
    char          szMatchedAmt[21 + 1];         // 成交金额
    char          szTrdacct[10 + 1];            // 交易账户
    short         iStrategySn;                  // 策略编号
};

//-------------------------------成交查询--------------------------------
struct STReqQryFill
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderId[10 + 1];            // 合同序号
    int           iOrderBsn;                    // 委托批号
    char          szTrdacct[10 + 1];            // 交易账户
    char          chQueryFlag;                  // 查询方向
    char          szQueryPos[32 + 1];           // 定位串
    int           iQueryNum;                    // 查询行数
    short         iStrategySn;                  // 策略编号
    char          chFlag;                       // 查询标志 '0':撤单委托的成交 '1':正常委托的成交 其他:全部
};

struct STRspQryFill
{
    char          szQryPos[32 + 1];             // 定位串
    char          szMatchedTime[8 + 1];         // 成交时间
    int           iOrderDate;                   // 委托日期
    int           iOrderSn;                     // 委托序号
    int           iOrderBsn;                    // 委托批号
    char          szOrderId[10 + 1];            // 合同序号
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szTrdacct[10 + 1];            // 证券账户
    int           iStkBiz;                      // 证券业务
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    char          chCurrency;                   // 货币代码
    char          szBondInt[21 + 1];            // 债券利息
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
    char          szMatchedSn[16 + 1];          // 成交编号
    char          szMatchedPrice[21 + 1];       // 成交价格
    char          szMatchedQty[21 + 1];         // 已成交数量
    char          szMatchedAmt[21 + 1];         // 已成交金额
    char          chMatchedType;                // 成交类型
    char          chIsWithdraw;                 // 撤单标志
    char          chOrderStatus;                // 委托状态
    short         iStrategySn;                  // 策略编号
};


//-------------------------------资金查询--------------------------
struct STReqQryMoney
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspQryMoney
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szFundBln[21 + 1];            // 资金余额
    char          szFundAvl[21 + 1];            // 资金可用金额
    char          szFundTrdFrz[21 + 1];         // 资金交易冻结金额
    char          szFundTrdOtd[21 + 1];         // 资金交易在途金额
    char          szTotalAssets[21 + 1];        // 资产总值
    char          szFundValue[21 + 1];          // 资金资产
    char          szMarketValue[21 + 1];        // 市值
};

//-------------------------------股份查询--------------------------------
struct STReqQryHolding
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szTrdacct[10 + 1];            // 交易账户
};

struct STRspQryHolding
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szTrdacct[10 + 1];            // 交易账户
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    LONGLONG      llStkBln;                     // 证券余额
    LONGLONG      llStkAvl;                     // 证券可用数量
    LONGLONG      llStkFrz;                     // 证券冻结数量
    LONGLONG      llStkTrdOtd;                  // 证券交易在途数量

    char          szCostPrice[21 + 1];          // 成本价格
    char          szStkBcostRlt[21 + 1];        // 证券买入成本（实时）
    char          szMktVal[21 + 1];             // 市值
    char          szProIncome[21 + 1];          // 参考盈亏
    char          szProfitRate[21 + 1];         // 盈亏比例
    LONGLONG      llStkQty;                     // 当前拥股数
};

//-------------------------------最大可交易数计算----------------------------
struct STReqMaxTradeQty
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderPrice[21 + 1];         // 委托价格
    int           iStkBiz;                      // 证券业务
    char          szTrdacct[10 + 1];            // 证券账户
};

struct STRspMaxTradeQty
{
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    LONGLONG      llOrderQty;                   // 委托数量
    char          szTrdacct[10 + 1];            // 证券账户
};






//-------------------------------成交回报推送------------------------------------
struct STRtnOrderFill
{
    char          szMatchedSn[16 + 1];        // 成交编号
    char          szStkCode[8 + 1];           // 证券代码
    char          szOrderId[10 + 1];          // 合同序号
    char          szTrdacct[16 + 1];          // 交易账户
    LONGLONG      llMatchedQty;               // 本次成交数量
    char          szMatchedPrice[11 + 1];     // 本次成交价格
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          szRltSettAmt[21 + 1];       // 实时清算金额
    char          szFundAvl[21 + 1];          // 资金可用金额（成交后）
    LONGLONG      llStkAvl;                   // 证券可用数量（成交后）
    int           iMatchedDate;               // 成交日期
    char          szMatchedTime[8 + 1];       // 成交时间
    char          chIsWithdraw;               // 撤单标志 'F':正常 'T':撤单
    LONGLONG      llCuacctCode;               // 资产账户
    int           iOrderBsn;                  // 委托批号
    short         iStrategySn;                // 策略编号
    char          szStkbd[2 + 1];             // 交易板块
    char          chMatchedType;              // 成交类型 '1':非法委托撤单成交 '2':交易所撤单或撮合成交
    char          chOrderStatus;              // 委托状态 '0':未报 '2':已报 '6':已撤 '8':已成 '9':废单
    int           iStkBiz;                    // 证券业务
    char          szOfferRetMsg[64 + 1];      // 申报信息 交易所废单时返回废单原因
    LONGLONG      llOrderQty;                 // 委托数量
    LONGLONG      llWithdrawnQty;             // 已撤单数量
    LONGLONG      llTotalMatchedQty;          // 累计成交数量
    char          szTotalMatchedAmt[21 + 1];  // 累计成交金额
    LONGLONG      llStkQty;                   // 拥股数
    char          szMatchedAmt[21 + 1];       // 已成交金额
};

//-------------------------------确认回报推送------------------------------------
struct STRtnOrderConfirm
{
    char          szStkCode[8 + 1];           // 证券代码
    char          szOrderId[10 + 1];          // 合同序号
    char          szTrdacct[16 + 1];          // 交易账户
    char          chIsWithdraw;               // 撤单标志 'F':正常 'T':撤单
    LONGLONG      llCuacctCode;               // 资产账户
    int           iOrderBsn;                  // 委托批号
    short         iStrategySn;                // 策略编号
    char          szStkbd[2 + 1];             // 交易板块
    char          chOrderStatus;              // 委托状态 '0':未报 '2':已报 '6':已撤 '8':已成 '9':废单
    int           iStkBiz;                    // 证券业务
    int           iOrderDate;                 // 委托日期
    char          szOrderPrice[21 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
};




//-------------------------------融资融券标的券信息查询------------------------------
struct STReqQryUndlStkInfo
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块  
    char          szStkCode[8 + 1];             // 证券代码  
    char          chCurrEnableFi;               // 当日融资标志 '0':允许 '1':不允许
    char          chCurrEnableSl;               // 当日融券标志 '0':允许 '1':不允许 
    char          chQueryFlag;                  // 查询方向  
    char          szQueryPos[32 + 1];           // 定位串
    int           iQueryNum;                    // 查询行数
};

struct STRspQryUndlStkInfo
{
    char          szQryPos[32 + 1];             // 定位串        
    char          szStkbd[2 + 1];               // 交易板块      
    char          szStkCode[8 + 1];             // 证券代码      
    char          szStkName[16 + 1];            // 证券名称      
    char          szFiMarginRatio[21 + 1];      // 融资保证金比例
    char          szSlMarginRatio[21 + 1];      // 融券保证金比例
    char          chCurrEnableFi;               // 当日融资标志 '0':允许 '1':不允许
    char          chCurrEnableSl;               // 当日融券标志 '0':允许 '1':不允许
};

//-------------------------------融资融券担保证券信息查询----------------------------
struct STReqQryColStkInfo
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块  
    char          szStkCode[8 + 1];             // 证券代码  
    char          chQueryFlag;                  // 查询方向  
    char          szQueryPos[32 + 1];           // 定位串    
    int           iQueryNum;                    // 查询行数  
};

struct STRspQryColStkInfo
{
    char          szQryPos[32 + 1];             // 定位串       
    char          szStkbd[2 + 1];               // 交易板块    
    char          szStkCode[8 + 1];             // 证券代码    
    char          szStkName[16 + 1];            // 证券名称    
    char          szCollatRatio[21 + 1];        // 担保品折算率
    char          chCreditFundUseFlag;          // 信用资金使用标志
};

//-------------------------------融资融券委托------------------------------------
struct STReqOrderCredit
{   
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块 SZ SH
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    int           iStkBiz;                      // 证券业务 融资买(702)融券卖(703)
    short         iStrategySn;                  // 策略编号
    char          szRepayOrderId[10 + 1];       // 偿还合同序号
    int           iRepayOpeningDate;            // 偿还合约日期
    char          szRepayStkCode[8 + 1];        // 偿还证券代码
};

struct STRspOrderCredit
{
    int           iOrderBsn;                    // 委托批号
    char          szOrderId[10 + 1];            // 合同序号
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 冻结金额
    char          szStkbd[2 + 1];               // 交易板块 SZ SH
    char          szStkCode[8 + 1];             // 证券代码
    char          szStkName[16 + 1];            // 证券名称
    int           iStkBiz;                      // 证券业务 融资买(702)融券卖(703)
    char          szTrdacct[10 + 1];            // 证券账户
    short         iStrategySn;                  // 策略编号
};

//-------------------------------融资融券直接还款----------------------------
struct STReqRepay
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chRepayType;                  // 偿还类型 '0':偿还融资欠款 '1':偿还融券费用
    char          szRepayOrderId[10 + 1];       // 偿还合同序号
    int           iRepayOpeningDate;            // 偿还合约日期
    char          szRepayStkCode[8 + 1];        // 偿还证券代码
    char          szRepayContractAmt[21 + 1];   // 偿还金额
    char          chRepayAmtCls;                // 偿还金额类别 '0':全部归还 '1':仅归还本金 '2':仅归还利息
    char          szRemark[128 + 1];            // 备注
};

struct STRspRepay
{
    char          szRealRepayAmt[21 + 1];       // 实际还款金额
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szRepayContractAmt[21 + 1];   // 偿还金额
};

//-------------------------------融资融券持仓查询--------------------------------
struct STReqQryHoldingCredit
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 证券代码
    char          szTrdacct[10 + 1];            // 交易账户
    char          chContractFlag;               // 启用合约开关
    char          chBizFlag;                    // 业务标志 '0':当前账户股份 '1':当前信用账户对应的普通账户股份
    
};

struct STRspQryHoldingCredit
{
    char          szQryPos[32 + 1];             // 定位串                    
    LONGLONG      llCuacctCode;                 // 资产账户            
    char          szStkbd[2 + 1];               // 交易板块            
    char          szTrdacct[10 + 1];            // 交易账户          
    char          szStkCode[8 + 1];             // 证券代码            
    char          szStkName[16 + 1];            // 证券名称                       
    LONGLONG      llStkPrebln;                  // 证券昨日余额        
    LONGLONG      llStkBln;                     // 证券余额            
    LONGLONG      llStkAvl;                     // 证券可用数量        
    LONGLONG      llStkFrz;                     // 证券冻结数量        
    LONGLONG      llStkUfz;                     // 证券解冻数量        
    LONGLONG      llStkTrdFrz;                  // 证券交易冻结数量    
    LONGLONG      llStkTrdUfz;                  // 证券交易解冻数量    
    LONGLONG      llStkTrdOtd;                  // 证券交易在途数量    
    LONGLONG      llStkTrdBln;                  // 证券交易扎差数量
    LONGLONG      llStkQty;                     // 当前拥股数          
    LONGLONG      llStkRemain;                  // 余券可用数量        
    LONGLONG      llStkSale;                    // 卖出冻结数量
    char          chIsCollat;                   // 是否是担保品
    char          szCollatRatio[21 + 1];        // 担保品折算率
    LONGLONG      llMktQty;                     // 当前拥股数（账户）
    char          szAveragePrice[21 + 1];       // 买入均价
};

//-------------------------------融资融券合约查询----------------------------
struct STReqQryContract
{
    LONGLONG      llCuacctCode;                 // 资产账户
    int           iBgnDate;                     // 开始日期
    int           iEndDate;                     // 结束日期
    char          szOrderId[10 + 1];            // 合同序号
    char          chContractType;               // 合约类型 '0':融资 '1':融券
    char          szQueryPos[32 + 1];           // 定位串  
    int           iQueryNum;                    // 查询行数
    char          szStkbd[2 + 1];               // 交易板块
    char          szTrdacct[10 + 1];            // 交易账户
    char          szStkCode[8 + 1];             // 证券代码
    char          chContractStatus;             // 合约状态 '0':开仓未归还 '1':部分归还 '2':到期未平仓 '3':主动了结 '4':手工了结 '5':实时合约 '6':展期中 
    char          chRepayFlag;                  // 平仓状态 '0':全部 '1':未平仓 '2':已平仓
    char          chQueryFlag;                  // 查询方向
};

struct STRspQryContract
{
    char          szQryPos[32 + 1];             // 定位串
    LONGLONG      llCashNo;                     // 头寸编号            
    int           iTrdDate;                     // 交易日期            
    char          chContractType;               // 合约类型 '0':融资 '1':融券
    char          szTrdacct[10 + 1];            // 交易账户            
    char          szStkbd[2 + 1];               // 交易板块
    int           iOpeningDate;                 // 开仓日期
    char          szStkCode[8 + 1];             // 证券代码
    char          szOrderId[10 + 1];            // 合同序号
    char          szFiDebtsAmt[21 + 1];         // 融资负债金额        
    LONGLONG      llSlDebtsQty;                 // 融券负债数量        
    LONGLONG      llRepaidQty;                  // 融券已还数量        
    char          szRepaidAmt[21 + 1];          // 融资已还金额        
    char          chContractStatus;             // 合约状态 '0':开仓未归还 '1':部分归还 '2':合约已过期 '3':主动了结 '4':手工了结 '5':实时合约 '6':展期中 
    int           iContractExpireDate;          // 合约到期日          
    char          szMarginRatio[21 + 1];        // 保证金比例          
    char          szMarginAmt[21 + 1];          // 占用保证金          
    char          szRights[21 + 1];             // 未偿还权益金额      
    LONGLONG      llRightsQty;                  // 未偿还权益数量      
    char          szOverdueFee[21 + 1];         // 逾期未偿还息费      
    int           iLastRepayDate;               // 最后偿还日期        
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szOrderPrice[21 + 1];         // 委托价格
    LONGLONG      llOrderQty;                   // 委托数量
    char          szOrderAmt[21 + 1];           // 委托金额
    char          szOrderFrzAmt[21 + 1];        // 委托冻结金额        
    LONGLONG      llWithdrawnQty;               // 已撤单数量          
    LONGLONG      llMatchedQty;                 // 成交数量            
    char          szMatchedAmt[21 + 1];         // 成交金额            
    char          szRltSettAmt[21 + 1];         // 实时清算金额        
    char          szSlDebtsMktvalue[21 + 1];    // 融券负债市值        
    LONGLONG      llRltRepaidQty;               // 融券实时归还数量    
    char          szRltRepaidAmt[21 + 1];       // 融资实时归还金额    
    char          szMatchedAmtRepay[21 + 1];    // 还成交金额          
    int           iCalIntDate;                  // 开始计息日期        
    char          szContractInt[21 + 1];        // 合约利息            
    char          szContractIntAccrual[21 + 1]; // 利息积数            
    char          szOverRights[21 + 1];         // 逾期未偿还权益      
    char          szRightsRepay[21 + 1];        // 己偿还权益          
    char          szRightsRlt[21 + 1];          // 实时偿还权益        
    char          szOverRightsRlt[21 + 1];      // 实时偿还预期权益    
    LONGLONG      llOverRightsQty;              // 逾期未偿还权益数量  
    LONGLONG      llRightsQtyRepay;             // 已偿还权益数量      
    LONGLONG      llRightsQtyRlt;               // 实时偿还权益数量    
    LONGLONG      llOverRightsQtyRlt;           // 实时偿还逾期权益数量
    char          szContractFee[21 + 1];        // 融资融券息费        
    char          szFeeRepay[21 + 1];           // 己偿还息费          
    char          szFeeRlt[21 + 1];             // 实时偿还息费        
    char          szOverDuefeeRlt[21 + 1];      // 实时偿还逾期息费    
    char          szPuniDebts[21 + 1];          // 逾期本金罚息        
    char          szPuniDebtsRepay[21 + 1];     // 本金罚息偿还        
    char          szPuniDebtsRlt[21 + 1];       // 实时逾期本金罚息    
    char          szPuniFee[21 + 1];            // 利息产生的罚息      
    char          szPuniFeeRepay[21 + 1];       // 己偿还罚息          
    char          szPuniFeeRlt[21 + 1];         // 实时逾期息费罚息    
    char          szPuniRights[21 + 1];         // 逾期权益罚息        
    char          szPuniRightsRepay[21 + 1];    // 权益罚息偿还        
    char          szPuniRightsRlt[21 + 1];      // 实时逾期权益罚息    
    int           iClosingDate;                 // 合约了结日期        
    char          szClosingPrice[21 + 1];       // 合约了结价格        
    char          chContractCls;                // 合约类别 '0':实时合约 '1':已开仓合约
    char          szProIncome[21 + 1];          // 参考盈亏
    char          szPledgeCuacct[21 + 1];       // 抵押资产
    char          szFiRepayAmt[21 + 1];         // 融资偿还
    LONGLONG      llSlRepayQty;                 // 融券偿还
};

//-------------------------------信用客户资产负债查询------------------------
struct STReqQryCustDebts
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspQryCustDebts
{      
    LONGLONG      llCuacctCode;                 // 资产账户              
    char          szFiRate[21 + 1];             // 融资利率        
    char          szSlRate[21 + 1];             // 融券利率        
    char          szFreeIntRate[21 + 1];        // 罚息利率        
    char          chCreditStatus;               // 信用状态 '0':正常 '1':未激活 '2':黑名单 '3':冻结 '9':过期
    char          szMarginRate[21 + 1];         // 维持担保比例    
    char          szRealRate[21 + 1];           // 实时担保比例    
    char          szTotalAssert[21 + 1];        // 总资产          
    char          szTotalDebts[21 + 1];         // 总负债          
    char          szMarginValue[21 + 1];        // 保证金可用余额  
    char          szFundAvl[21 + 1];            // 资金可用金额    
    char          szFundBln[21 + 1];            // 资金余额        
    char          szSlAmt[21 + 1];              // 融券卖出所得资金
    char          szGuaranteOut[21 + 1];        // 可转出担保资产  
    char          szColMktVal[21 + 1];          // 担保证券市值    
    char          szFiAmt[21 + 1];              // 融资本金        
    char          szTotalFiFee[21 + 1];         // 融资息费        
    char          szFiTotalDebts[21 + 1];       // 融资负债合计    
    char          szSlMktVal[21 + 1];           // 应付融券市值    
    char          szTotalSlFee[21 + 1];         // 融券息费        
    char          szSlTotalDebts[21 + 1];       // 融券负债合计    
    char          szFiCredit[21 + 1];           // 融资授信额度    
    char          szFiCreditAvl[21 + 1];        // 融资可用额度    
    char          szFiCreditFrz[21 + 1];        // 融资额度冻结    
    char          szSlCredit[21 + 1];           // 融券授信额度    
    char          szSlCreditAvl[21 + 1];        // 融券可用额度    
    char          szSlCreditFrz[21 + 1];        // 融券额度冻结    
    char          szRights[21 + 1];             // 红利权益        
    char          szRightsUncomer[21 + 1];      // 红利权益（在途）
    LONGLONG      llRightsQty;                  // 红股权益        
    LONGLONG      llRightsQtyUncomer;           // 红股权益（在途）
    char          szTotalCredit[21 + 1];        // 总额度          
    char          szTotalCteditAvl[21 + 1];     // 总可用额度      
};

//-------------------------------证券市值额度查询------------------------------------
struct STReqQryMktQuota
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspQryMktQuota
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szTrdacct[10 + 1];            // 证券账户
    LONGLONG      llMktQuota;                   // 市值额度
    LONGLONG      llKcbMktQuota;                // 市值额度(科创板) 
};

//-------------------------------当日新股信息查询------------------------------------
struct STReqQryIpoInfo
{
    LONGLONG      llCuacctCode;                 // 资产账户
};

struct STRspQryIpoInfo
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szStkbd[2 + 1];               // 交易板块
    char          szStkCode[8 + 1];             // 申购代码
    char          szStkName[16 + 1];            // 申购代码名称
    char          szLinkStk[8 + 1];             // 正股代码
    char          chIssueType;                  // 发行方式  '0':申购 '1':配售
    int           iIssueDate;                   // 申购日期
    char          szFixPrice[21 + 1];           // 申购价格
    int           iBuyUnit;                     // 申购单位
    int           iMinQty;                      // 申购下限
    int           iMaxQty;                      // 申购上限
};

//转帐银行业务信息查询
struct STReqQryBankInfo
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chCurrency;                   // 货币代码
    char          szBankCode[4 + 1];            // 银行代码
};
struct STRspQryBankInfo
{
    char          szBankCode[4 + 1];            // 银行代码
    char          chSourceType;                 // 发起方    
    char          chBankTranType;               // 转帐方式  
    char          chFundPwdFlag;                // 资金密码校验
    char          chBankPwdFlag;                // 银行密码校验
    char          chCheckBankId;                // 银行帐号校验
    char          chCheckIdNo;                  // 证件校验标志  
    char          szOrgId[4 + 1];               // 机构编码
    char          chCurrency;                   // 货币代码
    char          chStatus;                     // 银行状态
};

//银证转帐
struct STReqBankStkTrans
{
    LONGLONG  llCuacctCode;                      //资金账号
    char      chCurrency;                        //货币代码
    char      szFundPwd[32 + 1];                 //资金密码
    char      szBankCode[4 + 1];                 //银行代码
    char      szBankPwd[32 + 1];                 //银行密码
    char      chBankTranType;                    //转帐类型 '1':银行转证券 '2':证券转银行
    char      szTransAmt[21 + 1];                //转帐金额                          
};
struct STRspBankStkTrans
{
    int  iSNo;                                  //委托序号
    int  iSysErrorId;                           //错误代码
    char szErrorMsg[64 + 1];                    //错误信息

};

//查询银行账户余额
struct STReqQryBankBalance
{
    LONGLONG      llCuacctCode;                 // 资金账号
    char          chCurrency;                   // 货币代码
    char          szFundPwd[32 + 1];            // 资金密码
    char          szBankCode[4 + 1];            // 银行代码
    char          szBankPwd[32 + 1];            // 银行密码
};
struct STRspQryBankBalance
{
    int           iSNo;                         // 委托序号
    char          szErrorMsg[64 + 1];           // 错误信息
    int           iSysErrId;                    // 错误代码
    char          szFundEffect[21 + 1];         // 银行余额
};

//银证转帐查询
struct STReqQryBankStkTransInfo
{
    LONGLONG      llCuacctCode;                 // 资金账号
    char          chCurrency;                   // 货币代码
    int           iSNo;                         // 委托序号
};
struct STRspQryBankStkTransInfo
{
    int           iOperDate;                    // 转帐日期
    int           iOperTime;                    // 转帐时间
    LONGLONG      llCuacctCode;                 // 资金账号
    char          chCurrency;                   // 货币代码
    char          szBankCode[4 + 1];            // 银行代码
    char          chBankTranId;                 // 业务类型
    int           iSNo;                         // 委托序号
    char          szFundEffect[21 + 1];         // 委托金额
    char          szFundBal[21 + 1];            // 余额
    char          szRemark[32 + 1];             // 备注信息
    char          chStatus;                     // 处理结果 
    char          chSourceType;                 // 发起方向 
    char          szBankMsgId[16 + 1];          // 外部信息代码
    char          szBankMsg[64 + 1];            // 外部信息内容
    char          szErrorMsg[64 + 1];           // 系统错误信息
    int           iSysErrId;                    // 系统错误代码
};

//修改交易密码
struct STReqModifyTradePwd
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          szNewPwd[32 + 1];             // 新密码
};
struct STRspModifyTradePwd
{
    char          szMsgOk[32 + 1];              // 成功信息
};

//修改资金密码
struct STReqModifyFundPwd
{
    LONGLONG      llCuacctCode;                 // 资金帐户
    char          szOldFundPwd[32 + 1];         // 老资金密码
    char          szNewFundPwd[32 + 1];         // 新资金密码
};
struct STRspModifyFundPwd
{
    char          szMsgOk[32 + 1];              // 成功信息
};

//集中交易柜台资金查询
struct STReqQryRpcFund
{
    LONGLONG      llCuacctCode;                 // 资产账户
    char          chCurrency;                   // 货币代码
};
struct STRspQryRpcFund
{
    LONGLONG      llCuacctCode;                 // 资金账户
    char          szOrgId[4 + 1];               // 机构编码
    char          chCurrency;                   // 货币代码
    char          szFundBal[21 + 1];            // 资金余额
    char          szFundAvl[21 + 1];            // 资金可用金额
    char          szMarketValue[21 + 1];        // 资产总值
    char          szFund[21 + 1];               // 资金资产
    char          szStkValue[21 + 1];           // 市值
    int           iFundSeq;                     // 主资金标志
};

//资金划拨
struct STReqFundTransfer
{
    char         szOrgId[4 + 1];                // 机构编码
    LONGLONG     llCuacctCode;                  // 资金账号
    char         chCurrency;                    // 货币代码
    char         szFundAvl[21 + 1];             // 资金可用
    char         chDirect;                      // 划拨方向 '0':普通节点划入、VIP 系统划出 '1':普通节点划出、VIP 系统划入
};
struct STRspFundTransfer
{
    int          iSNo;                          // 操作流水号
    int          iVipSno;                       // VIP操作流水号
};

//银证转账银行账号查询
struct STReqQryBankAcct
{
    char          szBankCode[4 + 1];            // 银行代码
    char          chCurrency;                   // 货币代码
    LONGLONG      llCuacctCode;                 // 资金账号
};
struct STRspQryBankAcct
{
    char          szOrgId[4 + 1];                // 机构编码
    char          szBankCode[4 + 1];             // 银行代码
    char          szBankName[32 + 1];            // 银行名称
    char          chCurrency;                    // 货币代码
    char          szBankId[32 + 1];              // 银行帐户
    LONGLONG      llCuacctCode;                  // 资金帐号
    char          chLinkFlag;                    // 转帐标识
    char          chSourceType;                  // 发起方向
};



//-------------------------------期权委托申报------------------------------------
struct STReqOrderOpt
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szOptNum[16 + 1];           // 合约编码
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    short         iStrategySn;                // 策略编号
};

struct STRspOrderOpt
{
    int           iOrderBsn;                  // 委托批号
    char          szOrderId[10 + 1];          // 合同序号
    LONGLONG      llCuacctCode;               // 资产账户
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    char          szOrderAmt[21 + 1];         // 委托金额
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 证券账户
    char          szSubacctCode[8 + 1];       // 证券账户子编码
    char          szOptTrdacct[18 + 1];       // 期权合约账户
    char          szOptNum[16 + 1];           // 合约编码
    char          szOptCode[32 + 1];          // 合约代码
    char          szOptName[32 + 1];          // 合约简称
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szOptUndlName[16 + 1];      // 标的证券名称
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    short         iStrategySn;                // 策略编号
};

//-------------------------------期权委托撤单------------------------------------
struct STReqOptCancelOrder
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szOrderId[10 + 1];          // 合同序号
    int           iOrderBsn;                  // 委托批号
};

struct STRspOptCancelOrder
{
    int           iOrderBsn;                  // 委托批号
    char          szOrderId[10 + 1];          // 合同序号
    LONGLONG      llCuacctCode;               // 资产账户
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    char          szOrderAmt[21 + 1];         // 委托金额
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 证券账户
    char          szSubacctCode[8 + 1];       // 证券账户子编码
    char          szOptTrdacct[18 + 1];       // 期权合约账户
    char          szStkCode[32 + 1];          // 证券代码
    char          szStkName[32 + 1];          // 证券名称
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    char          chCancelStatus;             // 内部撤单标志 1:内部撤单 非1:普通撤单
    short         iStrategySn;                // 策略编号
};


//-------------------------------期权可用资金查询--------------------------
struct STReqOptQryMoney
{
    LONGLONG        llCuacctCode;               // 资产账户 
};

struct STRspOptQryMoney
{
    LONGLONG        llCuacctCode;               // 资产账户 
    char            chCurrency;                 // 货币代码
    char            szMarketValue[21 + 1];      // 资产总值 客户资产总额（实时）
    char            szFundValue[21 + 1];        // 资金资产 资金资产总额
    char            szStkValue[21 + 1];         // 市值 非资金资产总额 = 市值
    char            szFundPrebln[21 + 1];       // 资金昨日余额 
    char            szFundBln[21 + 1];          // 资金余额 
    char            szFundAvl[21 + 1];          // 资金可用金额 
    char            szFundFrz[21 + 1];          // 资金冻结金额 
    char            szFundUfz[21 + 1];          // 资金解冻金额 
    char            szFundTrdFrz[21 + 1];       // 资金交易冻结金额 
    char            szFundTrdUfz[21 + 1];       // 资金交易解冻金额 
    char            szFundTrdOtd[21 + 1];       // 资金交易在途金额 
    char            szFundTrdBln[21 + 1];       // 资金交易轧差金额 
    char            chFundStatus;               // 资金状态
    char            szMarginUsed[21 + 1];       // 占用保证金 
    char            szMarginInclRlt[21 + 1];    // 已占用保证金(含未成交) 包含卖出开仓委托未成交冻结的保证金(按前结算价计算)
    char            szFundExeMargin[21 + 1];    // 行权锁定保证金 
    char            szFundExeFrz[21 + 1];       // 行权资金冻结金额 
    char            szFundFeeFrz[21 + 1];       // 资金费用冻结金额 
    char            szPaylater[21 + 1];         // 垫付资金 
    char            szPreadvaPay[21 + 1];       // 预计垫资金额 深圳ETF行权E+1日预交收使用
    char            szExpPenInt[21 + 1];        // 预计负债利息 
    char            szFundDraw[21 + 1];         // 资金可取金额 
    char            szFundAvlRlt[21 + 1];       // 资金动态可用 
    char            szMarginInclDyn[21 + 1];    // 动态占用保证金(含未成交) 包含卖出开仓委托未成交冻结的保证金(按实时价格计算)
    char            szDailyInAmt[21 + 1];       // 当日入金 
    char            szDailyOutAmt[21 + 1];      // 当日出金 
    char            szFundRealAvl[21 + 1];      // 资金实际可用 当金证股票期权柜台系统启用动态可用功能时，资金实际可用=min（资金可用金额，资金动态可用）；当不启用动态可用时，资金实际可用=资金可用金额
};

//-------------------------------期权持仓查询------------------------------------
struct STReqOptQryHolding
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 交易账户
    char          szOptNum[16 + 1];           // 合约编码
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          chOptSide;                  // 持仓方向 L-权力仓，S-义务仓，C-备兑策略持仓
    char          chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
    char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
};

struct STRspOptQryHolding
{
    LONGLONG        llCuacctCode;               // 资产账户 
    char            szStkbd[2 + 1];             // 交易板块
    char            szTrdacct[10 + 1];          // 证券账户 
    char            szSubacctCode[8 + 1];       // 证券账户子编码 
    char            szOptTrdacct[18 + 1];       // 期权合约账户 
    char            chCurrency;                 // 货币代码 
    char            szOptNum[16 + 1];           // 合约编码 
    char            szOptCode[32 + 1];          // 合约代码 
    char            szOptName[32 + 1];          // 合约简称 
    char            chOptType;                  // 合约类型 字典[OPT_TYPE]
    char            chOptSide;                  // 持仓方向 
    char            chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
    LONGLONG        llOptPrebln;                // 合约昨日余额 
    LONGLONG        llOptBln;                   // 合约余额 
    LONGLONG        llOptAvl;                   // 合约可用数量 
    LONGLONG        llOptFrz;                   // 合约冻结数量 
    LONGLONG        llOptUfz;                   // 合约解冻数量 
    LONGLONG        llOptTrdFrz;                // 合约交易冻结数量 
    LONGLONG        llOptTrdUfz;                // 合约交易解冻数量 
    LONGLONG        llOptTrdOtd;                // 合约交易在途数量 
    LONGLONG        llOptTrdBln;                // 合约交易轧差数量 
    LONGLONG        llOptClrFrz;                // 合约清算冻结数量 
    LONGLONG        llOptClrUfz;                // 合约清算解冻数量 
    LONGLONG        llOptClrOtd;                // 合约清算在途数量 
    char            szOptBcost[21 + 1];         // 合约买入成本 
    char            szOptBcostRlt[21 + 1];      // 合约买入成本（实时） 
    char            szOptPlamt[21 + 1];         // 合约盈亏金额 
    char            szOptPlamtRlt[21 + 1];      // 合约盈亏金额（实时） 
    char            szOptMktVal[21 + 1];        // 合约市值 
    char            szOptPremium[21 + 1];       // 权利金 
    char            szOptMargin[21 + 1];        // 保证金 
    LONGLONG        llOptCvdAsset;              // 备兑股份数量 
    char            szOptClsProfit[21 + 1];     // 当日平仓盈亏 
    char            szSumClsProfit[21 + 1];     // 累计平仓盈亏 
    char            szOptFloatProfit[21 + 1];   // 浮动盈亏 浮动盈亏=证券市值-买入成本
    char            szTotalProfit[21 + 1];      // 总盈亏 
    LONGLONG        llOptRealPosi;              // 合约实际持仓 
    LONGLONG        llOptClsUnmatched;          // 合约平仓挂单数量 即平仓委托未成交数量
    LONGLONG        llOptDailyOpenRlt;          // 当日累计开仓数量 
    char            szOptUndlCode[8 + 1];       // 标的证券代码 
    char            szExerciseVal[21 + 1];      // 行权价值 认购权利仓的行权价值：MAX((标的-行权价), 0) * 合约单位 * 合约张数 认沽权利仓的行权价值：MAX((行权价-标的价), 0) * 合约单位 * 合约张数
    LONGLONG        llCombedQty;                // 已组合合约数量 参与组合的期权合约持仓数量
    char            szCostPrice[22 + 1];        // 合约成本价 
};


//-------------------------------期权当日委托查询------------------------------------
struct STReqOptQryOrder
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 交易账户
    char          szOptNum[32 + 1];           // 合约编码
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szCombStraCode[16 + 1];     // 组合策略代码
    char          szOrderId[10 + 1];          // 合同序号
    int           iOrderBsn;                  // 委托批号 
    char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
    char          szQryPos[32 + 1];           // 定位串
    int           iQryNum;                    // 查询行数
};

struct STRspOptQryOrder
{
    char          szQryPos[32 + 1];           // 定位串
    int           iTrdDate;                   // 交易日期
    int           iOrderDate;                 // 委托日期
    char          szOrderTime[25 + 1];        // 委托时间
    int           iOrderBsn;                  // 委托批号
    char          szOrderId[10 + 1];          // 合同序号
    char          chOrderStatus;              // 委托状态
    char          chOrderValidFlag;           // 委托有效标志
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 证券账户
    char          szSubacctCode[8 + 1];       // 证券账户子编码
    char          szOptTrdacct[18 + 1];       // 期权合约账户
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    char          szOwnerType[3 + 1];         // 订单所有类型
    char          szOptNum[16 + 1];           // 合约编码
    char          szOptCode[32 + 1];          // 合约代码
    char          szOptName[32 + 1];          // 合约简称
    char          szCombNum[16 + 1];          // 组合编码
    char          szCombStraCode[16 + 1];     // 组合策略代码
    char          szLeg1Num[16 + 1];          // 成分一合约编码
    char          szLeg2Num[16 + 1];          // 成分二合约编码
    char          szLeg3Num[16 + 1];          // 成分三合约编码
    char          szLeg4Num[16 + 1];          // 成分四合约编码
    char          chCurrency;                 // 货币代码
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    char          szOrderAmt[21 + 1];         // 委托金额
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          szOrderUfzAmt[21 + 1];      // 委托解冻金额
    LONGLONG      llOfferQty;                 // 申报数量
    int           iOfferStime;                // 申报时间
    LONGLONG      llWithdrawnQty;             // 已撤单数量
    LONGLONG      llMatchedQty;               // 已成交数量
    char          szMatchedAmt[21 + 1];       // 已成交金额
    char          chIsWithdraw;               // 撤单标志
    char          chIsWithdrawn;              // 已撤单标志
    char          chOptUndlCls;               // 标的证券类别
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szOptUndlName[16 + 1];      // 标的证券名称
    LONGLONG      llUndlFrzQty;               // 标的券委托冻结数量
    LONGLONG      llUndlUfzQty;               // 标的券委托解冻数量
    LONGLONG      llUndlWthQty;               // 标的券已撤单数量
    char          szOfferRetMsg[64 + 1];      // 申报返回信息
    short         iStrategySn;                // 策略编号
    int           iOrderSn;                   // 委托序号
    char          szRawOrderId[10 + 1];       // 原合同序号
    char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
    char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
    char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
    char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
};

//-------------------------------期权当日成交查询------------------------------------
struct STReqOptQryFill
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
    char          szTrdacct[10 + 1];          // 交易账户
    char          szOptNum[32 + 1];           // 合约编码
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szCombStraCode[16 + 1];     // 组合策略代码
    char          szOrderId[10 + 1];          // 合同序号
    int           iOrderBsn;                  // 委托批号
    char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
    char          szQryPos[32 + 1];           // 定位串
    int           iQryNum;                    // 查询行数
};

struct STRspOptQryFill
{
    char          szQryPos[32 + 1];           // 定位串
    int           iTrdDate;                   // 交易日期
    char          szMatchedTime[8 + 1];       // 成交时间
    int           iOrderDate;                 // 委托日期
    int           iOrderSn;                   // 委托序号
    int           iOrderBsn;                  // 委托批号
    char          szOrderId[10 + 1];          // 合同序号
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szStkpbu[8 + 1];            // 交易单元
    char          szTrdacct[10 + 1];          // 证券账户
    char          szSubacctCode[8 + 1];       // 证券账户子编码
    char          szOptTrdacct[18 + 1];       // 期权合约账户
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    char          szOwnerType[3 + 1];         // 订单所有类型
    char          szOptNum[16 + 1];           // 合约编码
    char          szOptCode[32 + 1];          // 合约代码
    char          szOptName[32 + 1];          // 合约简称
    char          szCombNum[16 + 1];          // 组合编码
    char          szCombStraCode[16 + 1];     // 组合策略代码
    char          szLeg1Num[16 + 1];          // 成分一合约编码
    char          szLeg2Num[16 + 1];          // 成分二合约编码
    char          szLeg3Num[16 + 1];          // 成分三合约编码
    char          szLeg4Num[16 + 1];          // 成分四合约编码
    char          chCurrency;                 // 货币代码
    char          chOptUndlCls;               // 标的证券类别
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szOptUndlName[16 + 1];      // 标的证券名称
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    char          szOrderAmt[21 + 1];         // 委托金额
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          chIsWithdraw;               // 撤单标志
    char          chMatchedType;              // 成交类型
    char          szMatchedSn[32 + 1];        // 成交编号
    char          szMatchedPrice[11 + 1];     // 成交价格
    LONGLONG      llMatchedQty;               // 已成交数量
    char          szMatchedAmt[21 + 1];       // 已成交金额
    short         iStrategySn;                // 策略编号
    char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
    char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
    char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
    char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
    char          szMatchedFee[21 + 1];       // 成交费用
};


//-------------------------------期权可撤委托查询------------------------------------
struct STReqOptQryWithdrawableOrder
{
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 交易账户
    char          szOptNum[32 + 1];           // 合约编码
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szOrderId[10 + 1];          // 合同序号
    char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
    char          szQryPos[32 + 1];           // 定位串
    int           iQryNum;                    // 查询行数
};

struct STRspOptQryWithdrawableOrder
{
    char          szQryPos[32 + 1];           // 定位串
    int           iTrdDate;                   // 交易日期
    int           iOrderDate;                 // 委托日期
    char          szOrderTime[25 + 1];        // 委托时间
    int           iOrderBsn;                  // 委托批号
    char          szOrderId[10 + 1];          // 合同序号
    char          chOrderStatus;              // 委托状态
    char          chOrderValidFlag;           // 委托有效标志
    LONGLONG      llCuacctCode;               // 资产账户
    char          szStkbd[2 + 1];             // 交易板块
    char          szTrdacct[10 + 1];          // 证券账户
    char          szSubacctCode[8 + 1];       // 证券账户子编码
    char          szOptTrdacct[18 + 1];       // 期权合约账户
    int           iStkBiz;                    // 证券业务
    int           iStkBizAction;              // 证券业务行为
    char          szOwnerType[3 + 1];         // 订单所有类型
    char          szOptNum[16 + 1];           // 合约编码
    char          szOptCode[32 + 1];          // 合约代码
    char          szOptName[32 + 1];          // 合约简称
    char          chCurrency;                 // 货币代码
    char          szOrderPrice[11 + 1];       // 委托价格
    LONGLONG      llOrderQty;                 // 委托数量
    char          szOrderAmt[21 + 1];         // 委托金额
    char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
    char          szOrderUfzAmt[21 + 1];      // 委托解冻金额
    LONGLONG      llOfferQty;                 // 申报数量
    int           iOfferStime;                // 申报时间
    LONGLONG      llWithdrawnQty;             // 已撤单数量
    LONGLONG      llMatchedQty;               // 已成交数量
    char          szMatchedAmt[21 + 1];       // 已成交金额
    char          chIsWithdraw;               // 撤单标志
    char          chIsWithdrawn;              // 已撤单标志
    char          chOptUndlCls;               // 标的证券类别
    char          szOptUndlCode[8 + 1];       // 标的证券代码
    char          szOptUndlName[16 + 1];      // 标的证券名称
    LONGLONG      llUndlFrzQty;               // 标的券委托冻结数量
    LONGLONG      llUndlUfzQty;               // 标的券委托解冻数量
    LONGLONG      llUndlWthQty;               // 标的券已撤单数量
    char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
    char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
    char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
    char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
};
#endif  //__ZS_STK_TRADE_API_STRUCT_H__