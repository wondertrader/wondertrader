#if !defined(__MA_CLI_FISL_TRADE_API_STRUCT_H__)
#define __MA_CLI_FISL_TRADE_API_STRUCT_H__
#include "maCliTradeApi.h"
BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10321001:标的券信息查询------------------------------
struct MATRADEAPI CReqFislQryUndlStkInfoField
{
  char          chStkex;                    // 证券交易所 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkCode[8 + 1];           // 证券代码 
  char          chCurrEnableFi;             // 当日融资 字典[CURR_ENABLE_FI]
  char          chCurrEnableSl;             // 当日融券 字典[CURR_ENABLE_SL]
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQueryPos[32 + 1];         // 定位串 第1次查询送空，后续查询必送 与查询方向配合使用
  int           iQueryNum;                  // 查询行数 最大2000
  LONGLONG      llCuacctCode;               // 资产账户 
};

struct MATRADEAPI CRspFislQryUndlStkInfoField
{
  char          szQryPos[32 + 1];           // 定位串 
  char          chStkex;                    // 证券交易所 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkCode[8 + 1];           // 证券代码 
  char          szStkName[16 + 1];          // 证券名称 
  char          szFiMarginRatio[21 + 1];    // 融资保证金比例 
  char          szSlMarginRatio[21 + 1];    // 融券保证金比例 
  char          chCurrEnableFi;             // 当日融资标志 字典[CURR_ENABLE_FI]
  char          chCurrEnableSl;             // 当日融券标志 字典[CURR_ENABLE_SL]
  char          szCollatRatio[21 + 1];      // 担保品折算率 
  char          chCreditFundCtl;            // 融资交易控制 
  char          chCreditStkCtl;             // 融券交易控制 
};

//-------------------------------10321010:担保证券信息查询----------------------------
struct MATRADEAPI CReqFislQryColStkInfoField
{
  char          chStkex;                      // 证券交易所
  char          szStkbd[2 + 1];               // 交易板块  
  char          szStkCode[8 + 1];             // 证券代码  
  char          chQueryFlag;                  // 查询方向  
  char          szQueryPos[32 + 1];           // 定位串    
  int           iQueryNum;                    // 查询行数  
};

struct MATRADEAPI CRspFislQryColStkInfoField
{
  char          szQryPos[32 + 1];             // 定位串      
  char          chStkex;                      // 证券交易所  
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  char          szCollatRatio[21 + 1];        // 担保品折算率
  char          chCreditFundUseFlag;          // 信用资金使用标志
  char          chCollatCls;                  // 担保类别 字典[COLLAT_CLS]
};

//-------------------------------10301022:LOF信息查询------------------------------------
struct MATRADEAPI CReqFislQryLofInfoField
{
  char szStkbd[2 + 1];                        // 交易板块
  char szLofCode[8 + 1];                      // LOF代码 
};

struct MATRADEAPI CRspFislQryLofInfoField
{
  char chStkex;                               // 证券交易所         
  char szStkbd[2 + 1];                        // 交易板块           
  char szLofCode[8 + 1];                      // LOF代码            
  char chLofCrFlag;                           // 申购赎回标志       
  LONGLONG llLofCrtnUplmt;                    // 申购上限           
  LONGLONG llLofCrtnLwlmt;                    // 申购下限           
  LONGLONG llLofCrtnUnit;                     // 申购单位           
  LONGLONG llLofRdtnUplmt;                    // 赎回上限           
  LONGLONG llLofRdtnlwlmt;                    // 赎回下限           
  LONGLONG llLofRdtnUnit;                     // 赎回单位           
  int iLofRdtnDlvyDays;                       // 赎回资金回转天数   
  char chLofType;                             // 基金类型           
  char chLofRtdnPreFlag;                      // LOF资金提前可用标志
};

//-------------------------------10302001:买卖委托------------------------------------
struct MATRADEAPI CReqFislOrderField
{
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szOrderPrice[21 + 1];         // 委托价格    
  LONGLONG      llOrderQty;                   // 委托数量    
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 业务行为    
  char          szStkpbu[8 + 1];              // 交易单元    
  int           iOrderBsn;                    // 委托批号    
  char          szOrderText[256 + 1];         // 委托扩展    
  char          szClientInfo[256 + 1];        // 终端信息    
  char          chSecurityLevel;              // 安全手段    
  char          szSecurityInfo[256 + 1];      // 安全信息    
  char          szComponetStkCode[8 + 1];     // 成份股代码  
  char          szComponetStkbd[2 + 1];       // 成份股板块  
  char          szStkbdLink[2 + 1];           // 关联板块    
  char          szTrdacct[20 + 1];            // 证券账户    
  int           iCuacctSn;                    // 账户序号    
  char          chPublishCtrlFlag;            // 股票风控标志
  char          szRepayOrderId[10 + 1];       // 偿还合同序号
  int           iRepayOpeningDate;            // 偿还合约日期
  char          szRepayStkCode[8 + 1];        // 偿还证券代码
  char          szTrdacctLink[20 + 1];        // 关联股东
  char          szInitTrdAmt[21 + 1];         // 初始交易金额
  int           iRepchDay;                    // 购回天数
  char          szLoanerTrdacct[20 + 1];      // 出借人证券账户
  char          szRepaySno[32 + 1];           // 合约编号
  int           iItemno;                      // 合约序号
  char          chAutoBuyFlag;                // 自动展期标志
  char          chCloseOutMode;               // 平仓方式
  char          chEtfMode;                    // ETF模式 字典ETF_MODE
  char          szMeetingSeq[10 + 1];         // 股东大会编码
  char          szVoteId[10 + 1];             // 议案编号
  char          szOppLofCode[3 + 1];          // 转入方基金编码
};

struct MATRADEAPI CRspFislOrderField
{
  int           iOrderBsn;                    // 委托批号
  char          szOrderId[10 + 1];            // 合同序号
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szOrderPrice[21 + 1];         // 委托价格
  LONGLONG      llOrderQty;                   // 委托数量
  char          szOrderAmt[21 + 1];           // 委托金额
  char          szOrderFrzAmt[21 + 1];        // 冻结金额
  char          szStkpbu[8 + 1];              // 交易单元
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szStkName[16 + 1];            // 证券名称
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          szTrdacct[20 + 1];            // 证券账户
  int           iCuacctSn;                    // 账户序号
};

//-------------------------------10302004:委托撤单------------------------------------
struct MATRADEAPI CReqFislCancelOrderField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szOrderId[10 + 1];            // 合同序号
  int           iOrderBsn;                    // 委托批号
  char          szClientInfo[256 + 1];        // 终端信息
};

struct MATRADEAPI CRspFislCancelOrderField
{
  int           iOrderBsn;                    // 委托批号
  char          szOrderId[10 + 1];            // 合同序号 原委托合同序号
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szOrderPrice[21 + 1];         // 委托价格
  LONGLONG      llOrderQty;                   // 委托数量
  char          szOrderAmt[21 + 1];           // 委托金额
  char          szOrderFrzAmt[21 + 1];        // 冻结金额
  char          szStkpbu[8 + 1];              // 交易单元
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szStkName[16 + 1];            // 证券名称
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          chCancelStatus;               // 内部撤单标志
  char          szTrdacct[20 + 1];            // 证券账户
  char          szMsgOk[32 + 1];              // 内撤信息 如果是内部撤单，修改返回信息为 "撤单已受理"
  char          szCancelList[256 + 1];        // 撤单列表 格式: 原委托序号#撤单委托序号#内部撤单标志#错误信息|......|  撤单委托序号=-1 表示撤单失败
  char          szWtOrderId[10 + 1];          // 委托合同号 撤单委托合同序号 
};

//-------------------------------10302010:最大可交易数计算----------------------------
struct MATRADEAPI CReqFislMaxTradeQtyField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            chHiqtyFlag;                // 最大上限标志 （接口预留，暂不实现）
  char            szStkbdLink[2 + 1];         // 关联板块 ETF实物申赎时送（接口预留，暂不实现）
  char            szTrdacctLink[10 + 1];      // 关联股东 ETF实物申赎时送（接口预留，暂不实现）
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szVoteId[10 + 1];           // 议案编号 
  char            chEtfMode;                  // ETF模式 字典ETF_MODE
  char            chStkBizCtvFlag;            // 业务转换标志 
  int             iTrdDate;                   // 交易日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szStkpbu[8 + 1];            // 交易席位 
  char            szStkprop[2 + 1];           // 股份性质 
  char            szSdStkType[2 + 1];         // 上登证券类别 
  char            szEquitType[2 + 1];         // 权益类型 
  char            chCirculType;               // 流通类型 
  char            szProdcode[12 + 1];         // 产品编码 
  char            szOrderText[256 + 1];       // 委托扩展 格式:关键字1=值1,关键字2=值2 适当性业务规则，关键字为PROP_RISK_NO，传入匹配流水号，例如： PROP_RISK_NO=流水号
};

struct MATRADEAPI CRspFislMaxTradeQtyField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llOrderQty;                 // 委托数量 可委托最大数量
  LONGLONG        llStkQty;                   // 成分股数量 
};

//-------------------------------10322001:融资融券直接还款----------------------------
struct MATRADEAPI CReqFislMarginRepayField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            chRepayType;                // 偿还类型 0:全部偿还 1:偿还融券费用
  char            szRepayOrderId[10 + 1];     // 偿还合同序号 
  int             iRepayOpeningDate;          // 偿还合约日期 
  char            szRepayStkCode[8 + 1];      // 偿还证券代码 
  char            szRepayContractAmt[21 + 1]; // 偿还金额 
  char            chRepayAmtCls;              // 偿还金额类别 0:全部归还 1:仅归还本金 2:仅归还利息
  char            szRemark[128 + 1];          // 备注 
  char            chCashType;                 // 头寸类型 
};

struct MATRADEAPI CRspFislMarginRepayField
{
  char            szRealRepayAmt[21 + 1];     // 实际还款金额 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szRepayContractAmt[21 + 1]; // 偿还金额 
};

//-------------------------------10303001:可用资金查询--------------------------------
struct MATRADEAPI CReqFislQryExpendableFundField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  int           iValueFlag;                   // 取值标志
};

struct MATRADEAPI CRspFislQryExpendableFundField
{
  LONGLONG      llCustCode;                   // 客户代码        
  LONGLONG      llCuacctCode;                 // 资产账户        
  char          chCurrency;                   // 货币代码        
  int           iIntOrg;                      // 内部机构        
  char          szMarketValue[21 + 1];        // 资产总值        
  char          szFundValue[21 + 1];          // 资金资产        
  char          szStkValue[21 + 1];           // 市值            
  char          szFundLoan[21 + 1];           // 融资总金额      
  char          szFundLend[21 + 1];           // 融券总金额      
  char          szFundPrebln[21 + 1];         // 资金昨日余额    
  char          szFundBln[21 + 1];            // 资金余额        
  char          szFundAvl[21 + 1];            // 资金可用金额    
  char          szFundFrz[21 + 1];            // 资金冻结金额    
  char          szFundUfz[21 + 1];            // 资金解冻金额    
  char          szFundTrdFrz[21 + 1];         // 资金交易冻结金额
  char          szFundTrdUfz[21 + 1];         // 资金交易解冻金额
  char          szFundTrdOtd[21 + 1];         // 资金交易在途金额
  char          szFundTrdBln[21 + 1];         // 资金交易轧差金额
  char          chFundStatus;                 // 资金状态        
  char          chCuacctAttr;                 // 资产账户属性    
  char          szFundBorrowBln[21 + 1];      // 资金内部拆借金额
  char          szHFundAvl[21 + 1];           // 港通资金可用
  char          szHFundTrdFrz[21 + 1];        // 港通资金交易冻结
  char          szHFundTrdUfz[21 + 1];        // 港通资金交易解冻
  char          szHFundTrdOtd[21 + 1];        // 港通资金交易在途
  char          szHFundTrdBln[21 + 1];        // 港通资金交易轧差
  char          szCreditFundBln[21 + 1];      // 融券卖出金额
  char          szCreditFundFrz[21 + 1];      // 融券卖出金额冻结
  char          szQryPos[32 + 1];             // 定位串
  char          chHgtFlag;                    // 港股通资金通用标志
  int           iExtOrg;                      // 外部机构
  char          szCashproval[21 + 1];         // 现金宝资产
  char          szTranAvl[21 + 1];            // 最大可取金额 
};

//-------------------------------10303002:可用股份查询--------------------------------
struct MATRADEAPI CReqFislQryExpendableSharesField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szStkpbu[8 + 1];              // 交易单元
  char          chQueryFlag;                  // 查询方向
  char          szTrdacct[20 + 1];            // 交易账户
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  char          chContractFlag;               // 启用合约开关
  char          chBizFlag;                    // 业务标志
  char          iIntOrg;                      // 内部机构
};

struct MATRADEAPI CRspFislQryExpendableSharesField
{
  char          szQryPos[32 + 1];             // 定位串              
  int           iIntOrg;                      // 内部机构            
  LONGLONG      llCustCode;                   // 客户代码            
  LONGLONG      llCuacctCode;                 // 资产账户            
  char          szStkbd[2 + 1];               // 交易板块            
  char          szStkpbu[8 + 1];              // 交易单元   
  char          szTrdacct[20 + 1];            // 交易账户
  char          chCurrency;                   // 货币代码            
  char          szStkCode[8 + 1];             // 证券代码            
  char          szStkName[16 + 1];            // 证券名称            
  char          chStkCls;                     // 证券类别            
  LONGLONG      llStkPrebln;                  // 证券昨日余额        
  LONGLONG      llStkBln;                     // 证券余额            
  LONGLONG      llStkAvl;                     // 证券可用数量        
  LONGLONG      llStkFrz;                     // 证券冻结数量        
  LONGLONG      llStkUfz;                     // 证券解冻数量        
  LONGLONG      llStkTrdFrz;                  // 证券交易冻结数量    
  LONGLONG      llStkTrdUfz;                  // 证券交易解冻数量    
  LONGLONG      llStkTrdOtd;                  // 证券交易在途数量    
  LONGLONG      llStkTrdBln;                  // 证券交易扎差数量
  char          szStkBcost[21 + 1];           // 证券买入成本        
  char          szStkBcostRlt[21 + 1];        // 证券买入成本（实时）
  char          szStkPlamt[21 + 1];           // 证券盈亏金额        
  char          szStkPlamtRlt[21 + 1];        // 证券盈亏金额（实时）
  char          szMktVal[21 + 1];             // 市值                
  char          szCostPrice[21 + 1];          // 成本价格            
  char          szProIncome[21 + 1];          // 参考盈亏            
  char          chStkCalMktval;               // 市值计算标识        
  LONGLONG      llStkQty;                     // 当前拥股数          
  char          szCurrentPrice[21 + 1];       // 最新价格            
  char          szProfitPrice[21 + 1];        // 参考成本价          
  LONGLONG      llStkDiff;                    // 可申赎数量          
  LONGLONG      llStkTrdUnfrz;                // 已申赎数量          
  char          szIncome[21 + 1];             // 盈亏                
  LONGLONG      llStkRemain;                  // 余券可用数量        
  LONGLONG      llStkSale;                    // 卖出冻结数量
  char          chIsCollat;                   // 是否是担保品
  char          szCollatRatio[21 + 1];        // 担保品折算率
  char          szMarketValue[21 + 1];        // 市值（账户）
  LONGLONG      llMktQty;                     // 当前拥股数（账户）
  char          szLastPrice[21 + 1];          // 最新价格（账户）
  char          szProfitRate[21 + 1];         // 盈亏比例
  char          szAveragePrice[21 + 1];       // 买入均价
  LONGLONG      llStkTrdEtfctn;               // ETF申购数量
  LONGLONG      llStkTrdEtfrmn;               // ETF赎回数量     
  LONGLONG      llStkIntradayQty;             // 当日普通买入股份
  LONGLONG      llFislIntradayQty;            // 当日融资买入股份
};

//-------------------------------10303003:当日委托查询--------------------------------
struct MATRADEAPI CReqFislQryCurrDayOrderField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szOrderId[10 + 1];            // 合同序号
  int           iOrderBsn;                    // 委托批号
  char          szTrdacct[20 + 1];            // 交易账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  int           iCuacctSn;                    // 账户序号
  char          chFlag;                       // 查询标志
};

struct MATRADEAPI CRspFislQryCurrDayOrderField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  int           iIntOrg;                      // 内部机构    
  int           iTrdDate;                     // 交易日期    
  int           iOrderDate;                   // 委托日期    
  char          szOrderTime[32 + 1];          // 委托时间    
  char          szOrderId[10 + 1];            // 合同序号    
  char          chOrderStatus;                // 委托状态    
  char          chOrderValidFlag;             // 委托有效标志
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkpbu[8 + 1];              // 交易单元    
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 业务行为    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  char          chCurrency;                   // 货币代码    
  char          szBondInt[21 + 1];            // 债券利息    
  char          szOrderPrice[21 + 1];         // 委托价格    
  LONGLONG      llOrderQty;                   // 委托数量    
  char          szOrderAmt[21 + 1];           // 委托金额    
  char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
  char          szOrderUfzAmt[21 + 1];        // 委托解冻金额
  LONGLONG      llOfferQty;                   // 申报数量    
  int           iOfferStime;                  // 申报时间    
  LONGLONG      llWithdrawnQty;               // 已撤单数量  
  LONGLONG      llMatchedQty;                 // 已成交数量  
  char          chIsWithdraw;                 // 撤单标志    
  char          chIsWithdrawn;                // 已撤单标志  
  int           iOrderBsn;                    // 委托批号    
  char          szMatchedAmt[21 + 1];         // 成交金额    
  char          szTrdacct[20 + 1];            // 交易账户    
  int           iCuacctSn;                    // 账户序号    
  char          szRawOrderId[10 + 1];         // 原合同序号  
  char          szOfferRetMsg[64 + 1];        // 申报信息    
  char          szOpSite[32 + 1];             // 操作站点    
  char          chChannel;                    // 操作渠道    
};

//-------------------------------10303004:当日成交查询--------------------------------
struct MATRADEAPI CReqFislQryCurrDayFillField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szOrderId[10 + 1];            // 合同序号
  int           iOrderBsn;                    // 委托批号
  char          szTrdacct[20 + 1];            // 交易账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  int           iCuacctSn;                    // 账户序号
  char          chFlag;                       // 查询标志
};

struct MATRADEAPI CRspFislQryCurrDayFillField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  char          szMatchedTime[8 + 1];         // 成交时间    
  int           iOrderDate;                   // 委托日期    
  int           iOrderSn;                     // 委托序号    
  int           iOrderBsn;                    // 委托批号    
  char          szOrderId[10 + 1];            // 合同序号    
  int           iIntOrg;                      // 内部机构    
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkpbu[8 + 1];              // 交易单元    
  char          szStkTrdacct[10 + 1];         // 证券账户    
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 证券业务行为
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  char          chCurrency;                   // 货币代码    
  char          szBondInt[21 + 1];            // 债券利息    
  char          szOrderPrice[21 + 1];         // 委托价格    
  LONGLONG      llOrderQty;                   // 委托数量    
  char          szOrderAmt[21 + 1];           // 委托金额    
  char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
  char          szMatchedSn[32 + 1];          // 成交编号    
  char          szMatchedPrice[21 + 1];       // 成交价格    
  char          szMatchedQty[21 + 1];         // 已成交数量  
  char          szMatchedAmt[21 + 1];         // 已成交金额  
  char          chMatchedType;                // 成交类型    
  int           iCuacctSn;                    // 账户序号    
  char          chIsWithdraw;                 // 撤单标志    
  char          chOrderStatus;                // 委托状态    
};

//-------------------------------10303005:股东账户查询--------------------------------
struct MATRADEAPI CReqFislQryStkholderAcctField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szTrdacct[20 + 1];            // 交易账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  char          szIntOrg[256 + 1];            // 内部机构 
  char          chTregStatus;                 // 指定状态 
};

struct MATRADEAPI CRspFislQryStkholderAcctField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          chStkex;                      // 交易市场    
  char          szStkbd[2 + 1];               // 交易板块    
  int           iIntOrg;                      // 内部机构    
  int           iTrdacctSn;                   // 账户序号    
  char          szTrdacctExid[20 + 1];        // 报盘账户    
  char          chTrdacctStatus;              // 账户状态    
  char          chTregStatus;                 // 指定状态    
  char          chBregStatus;                 // 回购状态    
  char          szStkpbu[8 + 1];              // 交易单元    
  char          szTrdacct[20 + 1];            // 证券账户    
  char          szIdType[2 + 1];              // 证件类型    
  char          szIdCode[48 + 1];             // 证件号码    
  char          szCustName[8 + 1];            // 客户名称    
  char          chTrdacctExcls;               // 交易账户类别
  char          chTrdacctMainFlag;            // 主交易账户标志 
  char          chLimitFlag;                  // 受限股东标志 
};

//-------------------------------10303006:可撤单委托查询------------------------------
struct MATRADEAPI CReqFislQryCanWithdrawOrderField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szOrderId[10 + 1];            // 合同序号
  char          iOrderBsn;                    // 委托批号
  char          szTrdacct[20 + 1];            // 交易账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  int           iCuacctSn;                    // 账户序号
};

struct MATRADEAPI CRspFislQryCanWithdrawOrderField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  int           iIntOrg;                      // 内部机构    
  int           iTrdDate;                     // 交易日期    
  int           iOrderDate;                   // 委托日期    
  char          szOrderTime[32 + 1];          // 委托时间    
  char          szOrderId[10 + 1];            // 合同序号    
  char          chOrderStatus;                // 委托状态    
  char          chOrderValidFlag;             // 委托有效标志
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkpbu[8 + 1];              // 交易单元    
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 业务行为    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  char          chCurrency;                   // 货币代码    
  char          szOrderPrice[21 + 1];         // 委托价格    
  LONGLONG      llOrderQty;                   // 委托数量    
  char          szOrderAmt[21 + 1];           // 委托金额    
  char          szOrderFrzAmt[21 + 1];        // 委托冻结金额
  char          szOrderUfzAmt[21 + 1];        // 委托解冻金额
  LONGLONG      llOfferQty;                   // 申报数量    
  int           iOfferStime;                  // 申报时间    
  LONGLONG      llWithdrawnQty;               // 已撤单数量  
  LONGLONG      llMatchedQty;                 // 已成交数量  
  char          chIsWithdraw;                 // 撤单标志    
  char          chIsWithdrawn;                // 已撤单标志  
  int           iOrderBsn;                    // 委托批号    
  char          szMatchedAmt[21 + 1];         // 成交金额    
  char          szTrdacct[20 + 1];            // 交易账户    
  int           iCuacctSn;                    // 账户序号    
};

//-------------------------------10303011:可用资金查询[快速]--------------------------
struct MATRADEAPI CReqFislQryExpendableFundExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
};

struct MATRADEAPI CRspFislQryExpendableFundExField
{
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          chCurrency;                   // 货币代码    
  int           iIntOrg;                      // 内部机构    
  char          chCuacctAttr;                 // 资产账户属性
  char          szFundPrebln[21 + 1];         // 资金昨日余额
  char          szFundBln[21 + 1];            // 资金余额
  char          szFundAvl[21 + 1];            // 资金可用金额
  char          szFundTrdFrz[21 + 1];         // 资金交易冻结金额
  char          szFundTrdUfz[21 + 1];         // 资金交易解冻金额
  char          szFundTrdOtd[21 + 1];         // 资金交易在途金额 
  char          szFundTrdBln[21 + 1];         // 资金交易轧差金额 
  char          chFundStatus;                 // 资金状态
};

//-------------------------------10303012:可用股份查询[快速]--------------------------
struct MATRADEAPI CReqFislQryExpendableSharesExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
};

struct MATRADEAPI CRspFislQryExpendableSharesExField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  int           iIntOrg;                      // 内部机构    
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkpbu[8 + 1];              // 交易单元    
  char          szTrdacct[20 + 1];            // 交易账户    
  char          chCurrency;                   // 货币代码    
  char          szStkCode[8 + 1];             // 证券代码    
  char          chStkCls;                     // 证券类别    
  LONGLONG      llStkAvl;                     // 证券可用数量
  LONGLONG      llStkIntradayQty;             // 当日普通买入股份
  LONGLONG      llFislIntradayQty;            // 当日融资买入股份
};

//-------------------------------10303013:当日委托查询[快速]--------------------------
struct MATRADEAPI CReqFislQryCurrDayOrderExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkpbu[8 + 1];              // 交易单元
  char          szOrderId[10 + 1];            // 合同序号
  int           iCuacctSn;                    // 账户序号
  char          chFlag;                       // 查询标志
};

struct MATRADEAPI CRspFislQryCurrDayOrderExField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  int           iIntOrg;                      // 内部机构    
  char          szOrderId[10 + 1];            // 合同序号    
  char          chOrderStatus;                // 委托状态    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  LONGLONG      llOrderQty;                   // 委托数量    
  LONGLONG      llWithdrawnQty;               // 已撤单数量  
  LONGLONG      llMatchedQty;                 // 已成交数量  
  char          chIsWithdraw;                 // 撤单标志    
  char          chIsWithdrawn;                // 已撤单标志  
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 证券业务行为
  int           iCuacctSn;                    // 账户序号    
  int           iOrderBsn;                    // 委托批号    
  char          szRawOrderId[10 + 1];         // 原合同序号  
};

//-------------------------------10303014:当日成交查询[快速]--------------------------
struct MATRADEAPI CReqFislQryCurrDayFillExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  int           iCuacctSn;                    // 账户序号
  char          szOrderId[10 + 1];            // 合同序号 
};

struct MATRADEAPI CRspFislQryCurrDayFillExField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  char          szOrderId[10 + 1];            // 合同序号    
  int           iIntOrg;                      // 内部机构    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkTrdacct[20 + 1];         // 证券账户    
  char          szStkpbu[8 + 1];              // 交易单元    
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 证券业务行为
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  LONGLONG      llOrderQty;                   // 委托数量    
  char          szMatchedSn[32 + 1];          // 成交编号    
  char          szMatchedPrice[21 + 1];       // 成交价格    
  char          szMatchedQty[21 + 1];         // 成交数量    
  char          szMatchedAmt[21 + 1];         // 成交金额    
  int           iCuacctSn;                    // 账户序号    
  char          chOrderStatus;                // 委托状态    
};

//-------------------------------10303015:股东账户查询[快速]--------------------------
struct MATRADEAPI CReqFislQryStkholderAcctExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
};

struct MATRADEAPI CRspFislQryStkholderAcctExField
{
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          chStkex;                      // 交易市场    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szTrdacct[20 + 1];            // 证券账户    
  int           iIntOrg;                      // 内部机构    
  char          szStkpbu[8 + 1];              // 交易单元    
  char          chTrdacctExcls;               // 交易账户类别
};

//-------------------------------10303016:可撤单委托查询[快速]------------------------
struct MATRADEAPI CReqFislQryCanWithdrawOrderExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串  
  int           iQueryNum;                    // 查询行数
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkpbu[8 + 1];              // 交易单元
  char          szOrderId[10 + 1];            // 合同序号
  int           iCuacctSn;                    // 账户序号
};

struct MATRADEAPI CRspFislQryCanWithdrawOrderExField
{
  char          szQryPos[32 + 1];             // 定位串查询  
  int           iIntOrg;                      // 内部机构    
  char          szOrderId[10 + 1];            // 合同序号    
  char          chOrderStatus;                // 委托状态    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkCode[8 + 1];             // 证券代码    
  char          szStkName[16 + 1];            // 证券名称    
  LONGLONG      llOrderQty;                   // 委托数量    
  LONGLONG      llWithdrawnQty;               // 已撤单数量  
  LONGLONG      llMatchedQty;                 // 已成交数量  
  char          chIsWithdraw;                 // 撤单标志    
  char          chIsWithdrawn;                // 已撤单标志  
  int           iStkBiz;                      // 证券业务    
  int           iStkBizAction;                // 证券业务行为
  int           iCuacctSn;                    // 账户序号    
};

//-------------------------------10321004:融资融券协议查询----------------------------
struct MATRADEAPI CReqFislQryMarginProtocolField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  int           iIntOrg;                      // 内部机构
  LONGLONG      llCashFiNo;                   // 融资头寸编号
  LONGLONG      llCashSlNo;                   // 融券头寸编号
};

struct MATRADEAPI CRspFislQryMarginProtocolField
{
  LONGLONG      llCashFiNo;                   // 融资头寸编号    
  LONGLONG      llCashSlNo;                   // 融券头寸编号    
  LONGLONG      llCustCode;                   // 客户代码        
  LONGLONG      llCuacctCode;                 // 资产账户        
  char          chCurrency;                   // 货币代码        
  int           iIntOrg;                      // 内部机构        
  int           iCreditIntGrp;                // 利率组编号      
  char          szCreditLine[21 + 1];         // 信用额度        
  char          chCreditType;                 // 信用额度类型    
  char          szMaxCredit[21 + 1];          // 最大信用额度上限
  char          szMinCredit[21 + 1];          // 最小信用额度下限
  int           iContractTerm;                // 合约期限        
  int           iAgtBgnDate;                  // 协议开始日期    
  int           iAgtEndDate;                  // 协议结束日期    
  char          chCalIntMode;                 // 利息计算模式    
  char          chDebtRepayMode;              // 负债偿还模式    
  char          chAgmtStatus;                 // 协议状态        
  char          szFiCredit[21 + 1];           // 融资信用额度    
  char          szSlCredit[21 + 1];           // 融券信用额度    
  char          szFundRightundRight[128 + 1]; // 资金权限
  char          szRemark[256 + 1];            // 备注
  char          chDebtsStatus;                // 负债状态
  char          szFiMarginrateInit[21 + 1];   // 客户初始融资保证金比例
  char          szSlMarginrateInit[21 + 1];   // 客户初始融券保证金比例
  char          szFiMarginrateAgio[21 + 1];   // 融资保证金比例折扣
  char          szSlMarginrateAgio[21 + 1];   // 融券保证金比例折扣
  char          szFiMarginrateLow[21 + 1];    // 融资保证金比例下限
  char          szSlMarginrateLow[21 + 1];    // 融券保证金比例下限
  LONGLONG      llFiShareQuoteNo;             // 融资共享额度编号
  LONGLONG      llSlShareQuoteNo;             // 融券共享额度编号
  char          szCreditFundUsed[21 + 1];     // 信用资金占用
};

//-------------------------------10323001:融资融券合约查询----------------------------
struct MATRADEAPI CReqFislQryMarginContractField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  int           iBgnDate;                     // 开始日期
  int           iEndDate;                     // 结束日期
  char          szOrderId[10 + 1];            // 合同序号
  char          chContractType;               // 合约类型
  char          szQueryPos[32 + 1];           // 定位串  
  LONGLONG      llQueryNum;                   // 查询行数
  LONGLONG      llCustCode;                   // 客户代码
  char          chStkex;                      // 交易市场
  char          szStkbd[2 + 1];               // 交易板块
  char          szTrdacct[20 + 1];            // 交易账户
  char          szStkCode[8 + 1];             // 证券代码
  int           iIntOrg;                      // 内部机构
  int           iOpOrg;                       // 操作机构
  char          chContractStatus;             // 合约状态
  char          chRepayFlag;                  // 平仓状态
  char          chQueryFlag;                  // 查询方向
};

struct MATRADEAPI CRspFislQryMarginContractField
{
  char          szQryPos[32 + 1];             // 定位串              
  LONGLONG      llCashNo;                     // 头寸编号            
  int           iTrdDate;                     // 交易日期            
  char          chContractType;               // 合约类型            
  char          szTrdacct[20 + 1];            // 交易账户            
  char          chStkex;                      // 交易市场            
  char          szStkbd[2 + 1];               // 交易板块            
  int           iOpeningDate;                 // 开仓日期            
  char          szStkCode[8 + 1];             // 证券代码            
  char          szOrderId[10 + 1];            // 合同序号            
  char          szFiDebtsAmt[21 + 1];         // 融资负债金额        
  LONGLONG      llSlDebtsQty;                 // 融券负债数量        
  LONGLONG      llRepaidQty;                  // 融券已还数量        
  char          szRepaidAmt[21 + 1];          // 融资已还金额        
  char          chContractStatus;             // 合约状态            
  int           iContractExpireDate;          // 合约到期日          
  char          szMarginRatio[21 + 1];        // 保证金比例          
  char          szMarginAmt[21 + 1];          // 占用保证金          
  char          szRights[21 + 1];             // 未偿还权益金额      
  LONGLONG      llRightsQty;                  // 未偿还权益数量      
  char          szOverdueFee[21 + 1];         // 逾期未偿还息费      
  int           iLastRepayDate;               // 最后偿还日期        
  LONGLONG      llCustCode;                   // 客户代码            
  LONGLONG      llCuacctCode;                 // 资产账户            
  char          chCurrency;                   // 货币代码            
  int           iIntOrg;                      // 内部机构            
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
  int           iOpOrg;                       // 操作机构            
  char          chContractCls;                // 合约类别            
  char          szProIncome[21 + 1];          // 参考盈亏            
  char          szPledgeCuacct[21 + 1];       // 抵押资产            
  char          szFiRepayAmt[21 + 1];         // 融资偿还            
  LONGLONG      llSlRepayQty;                 // 融券偿还            
};

//-------------------------------10323002:融资融券保证金可用余额查询------------------
struct MATRADEAPI CReqFislQryMarginDepositAvlField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  LONGLONG      llCustCode;                   // 客户代码
};

struct MATRADEAPI CRspFislQryMarginDepositAvlField
{
  LONGLONG      llCustCode;                   // 客户代码      
  LONGLONG      llCuacctCode;                 // 资产账户      
  char          chCurrency;                   // 货币代码      
  int           iIntOrg;                      // 内部机构      
  char          szMarginValue[21 + 1];        // 保证金可用余额
};

//-------------------------------10323003:融资融券信用可用额度查询--------------------
struct MATRADEAPI CReqFislQryMarginCreditAvlField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  char          chContractType;               // 合约类型
};

struct MATRADEAPI CRspFislQryMarginCreditAvlField
{
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          chCurrency;                   // 货币代码    
  int           iIntOrg;                      // 内部机构    
  char          chContractType;               // 合约类型    
  char          szCreditValue[21 + 1];        // 信用可用额度
};

//-------------------------------10323006:信用客户资产负债查询------------------------
struct MATRADEAPI CReqFislQryCreditCustDebtsField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
};

struct MATRADEAPI CRspFislQryCreditCustDebtsField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFiRate[21 + 1];           // 融资利率 
  char            szSlRate[21 + 1];           // 融券利率 
  char            szFreeIntRate[21 + 1];      // 罚息利率 
  char            chCreditStatus;             // 信用状态 0:正常 1:未激活 2:黑名单 3:冻结 9:过期
  char            szMarginRate[21 + 1];       // 维持担保比例 
  char            szRealRate[21 + 1];         // 实时担保比例 
  char            szTotalAssert[21 + 1];      // 总资产 
  char            szTotalDebts[21 + 1];       // 总负债 
  char            szMarginValue[21 + 1];      // 保证金可用余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szFundBln[21 + 1];          // 资金余额 
  char            szSlAmt[21 + 1];            // 融券卖出所得资金 
  char            szGuaranteOut[21 + 1];      // 可转出担保资产 
  char            szColMktVal[21 + 1];        // 担保证券市值 
  char            szFiAmt[21 + 1];            // 融资本金 
  char            szTotalFiFee[21 + 1];       // 融资息费 
  char            szFiTotalDebts[21 + 1];     // 融资负债合计 
  char            szSlMktVal[21 + 1];         // 应付融券市值 
  char            szTotalSlFee[21 + 1];       // 融券息费 
  char            szSlTotalDebts[21 + 1];     // 融券负债合计 
  char            szFiCredit[21 + 1];         // 融资授信额度 
  char            szFiCreditAvl[21 + 1];      // 融资可用额度 
  char            szFiCreditFrz[21 + 1];      // 融资额度冻结 
  char            szSlCredit[21 + 1];         // 融券授信额度 
  char            szSlCreditAvl[21 + 1];      // 融券可用额度 
  char            szSlCreditFrz[21 + 1];      // 融券额度冻结 
  char            szRights[21 + 1];           // 红利权益 
  char            szRightsUncomer[21 + 1];    // 红利权益（在途） 
  LONGLONG        llRightsQty;                // 红股权益 
  LONGLONG        llRightsQtyUncomer;         // 红股权益（在途） 
  char            szTotalCredit[21 + 1];      // 总额度 
  char            szTotalCteditAvl[21 + 1];   // 总可用额度 
  char            szAddMarginAmt[21 + 1];     // 需追加保证金额 
};

//-------------------------------10323008:融资融券资金头寸查询------------------------
struct MATRADEAPI CReqFislQryMarginFundCashField
{
  LONGLONG      llCashNo;                     // 头寸编号
};

struct MATRADEAPI CRspFislQryMarginFundCashField
{
  LONGLONG      llCashNo;                     // 头寸编号    
  char          chCashType;                   // 头寸类型    
  char          szCashBln[21 + 1];            // 头寸总额度  
  char          szCashAvl[21 + 1];            // 头寸可用额度
  char          chCurrency;                   // 货币代码    
};

//-------------------------------10323009:融资融券股份头寸查询------------------------
struct MATRADEAPI CReqFislQryMarginSharesCashField
{
  LONGLONG        llCashNo;                   // 头寸编号 
  char            chStkex;                    // 证券交易所 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chMeshFlag;                 // 合并标志 0为不合并，1为合并
  LONGLONG        llCuacctCode;               // 资产账户 
};

struct MATRADEAPI CRspFislQryMarginSharesCashField
{
  LONGLONG        llCashNo;                   // 头寸编号 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llAssetBln;                 // 头寸总额度 
  LONGLONG        llAssetAvl;                 // 头寸可用额度 
  char            chCashType;                 // 头寸类型 
  char            szSrkName[16 + 1];          // 证券名称 
};

//-------------------------------10323011:可偿还金额查询------------------------
struct MATRADEAPI CReqFislQryCanRepayField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCashType;                 // 头寸类型 
};

struct MATRADEAPI CRspFislQryCanRepayField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFiTotalDebts[21 + 1];     // 融资负债合计 
  char            szTotalSlFee[21 + 1];       // 融券息费 
  char            szCanRepayAmt[21 + 1];      // 可偿还金额 
  char            szMayrepay[21 + 1];         // 偿还可用金额 
};

//-------------------------------10301034:证券市值额度查询------------------------------------
struct MATRADEAPI CReqFislMktQuotaField
{
  char          szTrdacct[20 + 1];            // 证券账户
};

struct MATRADEAPI CRspFislMktQuotaField
{
  char            chStkex;                    // 交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkpbu[8 + 1];            // 交易单元 
  LONGLONG        llMktQuota;                 // 市值额度(主板) 
  LONGLONG        llKcbMktquota;              // 市值额度(科创板) 
};

//-------------------------------10323030:融资合约汇总查询------------------------------------
struct MATRADEAPI CReqFislQryFiContractSumField
{
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块
  char          szStkCode[8 + 1];           // 证券代码
};

struct MATRADEAPI CRspFislQryFiContractSumField
{
  LONGLONG      llCuacctCode;               // 资产账户      
  char          szStkbd[2 + 1];             // 交易板块      
  char          szStkCode[8 + 1];           // 证券代码      
  char          szSettAmt[21 + 1];          // 清算金额      
  LONGLONG      llOpenQty;                  // 开仓数量      
  char          szRepayAmt[21 + 1];         // 归还金额      
  char          szOrderAmt[21 + 1];         // 委托金额      
  LONGLONG      llOrderQty;                 // 委托数量      
  LONGLONG      llMatchedQty;               // 已成交数量    
  char          szRltSettAmt[21 + 1];       // 实时清算金额  
  char          szMarginAmt[21 + 1];        // 占用保证金    
  char          szCollatRatio[21 + 1];      // 担保品折算率  
  char          szFiMarginRatio[21 + 1];    // 融资保证金比例
  LONGLONG      llRltSettQty;               // 实时清算数量  
};

//-------------------------------10323031:融券合约汇总查询------------------------------------
struct MATRADEAPI CReqFislQrySlContractSumField
{
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块
  char          szStkCode[8 + 1];           // 证券代码
};

struct MATRADEAPI CRspFislQrySlContractSumField
{
  LONGLONG      llCuacctCode;               // 资产账户      
  char          szStkbd[2 + 1];             // 交易板块      
  char          szStkCode[8 + 1];           // 证券代码      
  LONGLONG      llOpenQty;                  // 开仓数量          
  LONGLONG      llRepayQty;                 // 归还数量          
  char          szSettAmt[21 + 1];          // 清算金额          
  char          szRepayAmt[21 + 1];         // 归还金额          
  LONGLONG      llOrderQty;                 // 委托数量          
  LONGLONG      llRightsQty;                // 未偿还权益数量    
  LONGLONG      llOverRightsQty;            // 逾期未偿还权益数量
  char          szCollatRatio[21 + 1];      // 担保品折算率      
  char          szSlMarginRatio[21 + 1];    // 融券保证金比例    
  LONGLONG      llRltOpenQty;               // 实时开仓数量      
  char          szRltSettAmt[21 + 1];       // 实时清算金额      
};

//资金划拨
struct MATRADEAPI CReqFislFundTransferField
{
  char         szOrgId[4 + 1];               // 机构编码
  LONGLONG     llCuacctCode;                 // 资金账号
  char         chCurrency;                   // 货币代码
  char         szFundAvl[21 + 1];            // 资金可用
  char         chDirect;                     // 划拨方向
};
struct MATRADEAPI CRspFislFundTransferField
{
  int          iSNo;                         // 操作流水号
  int          iVipSno;                      // VIP操作流水号
};

//-------------------------------10301105:用户登录------------------------------------
struct MATRADEAPI CReqFislUserLoginField
{
  char          szAcctType[2 + 1];            // 账户类型
  char          szAcctId[32 + 1];             // 账户标识
  char          chUseScope;                   // 使用范围
  char          szEncryptKey[32 + 1];         // 加密因子
  char          chAuthType;                   // 认证类型
  char          szAuthData[256 + 1];          // 认证数据
  char          szThirdParty[1024 + 1];       // 第三方
  char          chEncryptType;                // 加密方式 ‘0’:金证KBSS系统加密 ‘1’:金证Win版集中交易加密 ‘2’:金证Unix版集中交易加密
};

struct MATRADEAPI CRspFislUserLoginField
{
  LONGLONG      llCustCode;                   // 客户代码    
  LONGLONG      llCuacctCode;                 // 资产账户    
  char          chStkex;                      // 交易市场    
  char          szStkbd[2 + 1];               // 交易板块    
  char          szStkTrdacct[20 + 1];         // 证券账户    
  int           iTrdacctSn;                   // 账户序号    
  char          szTrdacctExid[20 + 1];        // 报盘账户    
  char          chTrdacctStatus;              // 账户状态    
  char          chTregStatus;                 // 指定状态    
  char          chBregStatus;                 // 回购状态    
  char          szStkpbu[8 + 1];              // 交易单元    
  char          szAcctType[2 + 1];            // 账户类型    
  char          szAcctId[32 + 1];             // 账户标识    
  char          szTrdacctName[32 + 1];        // 交易账户名称
  char          szSessionId[256 + 1];         // 会话凭证    
  int           iIntOrg;                      // 内部机构    
  char          chCuacctAttr;                 // 资产账户属性
  int           iFlashPort;                   // 软加速端口号
  char          szFlashStkBizes[64 + 1];      // 软加速业务
  char          chSzFlashMode;                // 软加速深圳市场加速模式
  char          chShFlashMode;                // 软加速上海市场加速模式
  char          chTrdacctExcls;               // 交易账户类别 '0' -- 个人账户；'1' -- 机构账户；'2' -- 个人信用账户；'3' -- 机构信用账户
};

//-------------------------------00102023:成交回报推送------------------------------------
struct MATRADEAPI CRtnFislOrderFillField
{
  char          szMatchedSn[32 + 1];        // 成交编号
  char          chStkex;                    // 交易市场
  char          szStkCode[8 + 1];           // 证券代码
  char          szOrderId[10 + 1];          // 合同序号
  char          szTrdacct[20 + 1];          // 交易账户
  LONGLONG      llMatchedQty;               // 本次成交数量
  char          szMatchedPrice[11 + 1];     // 本次成交价格
  char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
  char          szRltSettAmt[21 + 1];       // 实时清算金额
  char          szFundAvl[21 + 1];          // 资金可用金额（成交后）
  LONGLONG      llStkAvl;                   // 证券可用数量（成交后）
  char          szOpptStkpbu[8 + 1];        // 对方席位
  char          szOpptTrdacct[20 + 1];      // 对方交易账号
  int           iMatchedDate;               // 成交日期
  char          szMatchedTime[8 + 1];       // 成交时间
  char          chIsWithdraw;               // 撤单标志
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iOrderBsn;                  // 委托批号
  int           iCuacctSn;                  // 账户序号
  char          szStkbd[2 + 1];             // 交易板块
  char          chMatchedType;              // 成交类型
  char          chOrderStatus;              // 委托状态
  int           iStkBiz;                    // 证券业务
  int           iStkBizAction;              // 证券业务行为
  char          szOfferRetMsg[64 + 1];      // 申报信息 交易所废单时返回废单原因
  LONGLONG      llOrderQty;                 // 委托数量
  LONGLONG      llWithdrawnQty;             // 已撤单数量
  LONGLONG      llTotalMatchedQty;          // 累计成交数量
  char          chCuacctAttr;               // 账户属性
  char          szTotalMatchedAmt[21 + 1];  // 累计成交金额
  LONGLONG      llStkQty;                   // 拥股数
  char          szMatchedAmt[21 + 1];       // 已成交金额
  char          szHFundAvl[21 + 1];         // 港股资金可用
  char          szRemark1[32 + 1];          // 预留字段1
  char          szRemark2[32 + 1];          // 预留字段2
  char          szRemark3[32 + 1];          // 预留字段3
  char          szRemark4[32 + 1];          // 预留字段4
  char          szRemark5[32 + 1];          // 预留字段5
  char          szRemark6[32 + 1];          // 预留字段6
  char          szRemark7[32 + 1];          // 预留字段7
  char          szRemark8[32 + 1];          // 预留字段8
  char          szRemark9[32 + 1];          // 预留字段9
  char          szRemarkA[32 + 1];          // 预留字段A
  char          szOrderNo[10 + 1];          // 委托编号
  LONGLONG      llStkTrdEtfctn;             // ETF申购数量
  LONGLONG      llStkTrdEtfrmn;             // ETF赎回数量
};

//-------------------------------00102023:申赎成交推送------------------------------------
struct MATRADEAPI CRtnFislPefEtfFillField
{
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llSerialNo;                 // 流水序号 流水序号，唯一
  char            szOccurTime[32 + 1];        // 发生时间 
  char            szMatchedSn[16 + 1];        // 成交编号 上交所：cjbh,交易所唯一 深交所:无
  char            szMatchedSno[32 + 1];       // 成交批号 上交所：gdxm+bcye 深交所: 执行编号EXECID
  char            chStkex;                    // 交易市场 数据字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 数据字典[STKBD]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            chEtfCr;                    // 申赎方向 1.申购2.赎回
  char            chBizType;                  // 业务类型 1.非跨市组合证券2.跨市组合证券2.非跨市现金替代3.跨市现金替代4.全现金替代5.现金差额
  char            chSettlementMode;           // 交收方式 1.净额担保交收2.T+0逐笔全额非担保3.T+1逐笔全额非担保4.RTGS与T+0逐笔全额非担保5.代收代付
  char            szCustCode[16 + 1];         // 客户代码 客户代码
  char            szCuacctCode[16 + 1];       // 资产账户 （客户）资产账户
  char            szTrdacct[20 + 1];          // 交易账户 交易账户（股东账户）
  char            szBizAmt[21 + 1];           // 业务金额 当前发生金额
  char            szFundBln[21 + 1];          // 资金当前余额 （客户）资产账户当前余额
  char            szFundAvl[21 + 1];          // 资金可用金额 （客户）资产账户可用金额
  char            szFundFrz[21 + 1];          // 资金冻结金额 （客户）资产账户冻结金额
  char            szFundUfz[21 + 1];          // 资金解冻金额 （客户）资产账户解冻金额
  char            szFundTrdFrz[21 + 1];       // 资金交易冻结金额 （客户）资产账户交易冻结金额
  char            szFundTrdUfz[21 + 1];       // 资金交易解冻金额 （客户）资产账户交易解冻金额
  char            szFundTrdOtd[21 + 1];       // 资金交易在途金额 （客户）资产账户交易在途金额
  char            szFundTrdBln[21 + 1];       // 资金交易轧差金额 （客户）资产账户交易轧差金额
  char            szStkpbu[8 + 1];            // 交易单元 交易单元（席位）
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 成分股证券代码或二级市场代码
  char            chStkCls;                   // 证券类别 证券类别DD[STK_CLS]
  LONGLONG        llBizQty;                   // 业务数量 深交所：股份交付数量 上交所：cjsl
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkAvl;                   // 证券可用数量 
  LONGLONG        llStkTrdFrz;                // 证券交易冻结数量 
  LONGLONG        llStkTrdUfz;                // 证券交易解冻数量 
  LONGLONG        llStkTrdOtd;                // 证券交易在途数量 
  LONGLONG        llStkTrdBln;                // 证券交易轧差数量 
  LONGLONG        llStkTrdEtfctn;             // ETF申购数量 
  LONGLONG        llStkTrdEtfrmn;             // ETF赎回数量 
  char            szUncollectedAmt[21 + 1];   // 赎回待处理资金 
  char            szCollectedAmt[21 + 1];     // 赎回已处理资金 
  char            szQryPos[32 + 1];           // 定位串 查询定位串
  int             iMatchedDate;               // 成交日期 
  char            szMatchedTime[8 + 1];       // 成交时间 
  char            szMatchedPrice[21 + 1];     // 成交价格 上交所cjjg
  char            szApplyId[10 + 1];          // 申请编号 上交所Sqbh
  char            szInvTrdacct[20 + 1];       // 投资者证券账户 
  char            szInvPbu[8 + 1];            // 投资者交易单元 
  char            chExecType;                 // 订单执行状态
  char            szRltSettAmt[21 + 1];       // 实时清算金额 
};

//-------------------------------00102024:确认回报推送------------------------------------
struct MATRADEAPI CRtnFislOrderConfirmField
{
  char          chStkex;                    // 交易市场
  char          szStkCode[8 + 1];           // 证券代码
  char          szOrderId[10 + 1];          // 合同序号
  char          szTrdacct[20 + 1];          // 交易账户
  char          chIsWithdraw;               // 撤单标志
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iOrderBsn;                  // 委托批号
  int           iCuacctSn;                  // 账户序号
  char          szStkbd[2 + 1];             // 交易板块
  char          chOrderStatus;              // 委托状态
  int           iStkBiz;                    // 证券业务
  int           iStkBizAction;              // 业务行为
  char          chCuacctAttr;               // 账户属性
  int           iOrderDate;                 // 委托日期
  int           iOrderSn;                   // 委托序号
  int           iIntOrg;                    // 内部机构
  char          szStkpbu[8 + 1];            // 交易单元
  char          szOrderPrice[21 + 1];       // 委托价格
  LONGLONG      llOrderQty;                 // 委托数量
  char          szRemark1[32 + 1];          // 预留字段1
  char          szRemark2[32 + 1];          // 预留字段2
  char          szRemark3[32 + 1];          // 预留字段3
  char          szRemark4[32 + 1];          // 预留字段4
  char          szRemark5[32 + 1];          // 预留字段5
  char          szRemark6[32 + 1];          // 预留字段6
  char          szRemark7[32 + 1];          // 预留字段7
  char          szRemark8[32 + 1];          // 预留字段8
  char          szRemark9[32 + 1];          // 预留字段9
  char          szRemarkA[32 + 1];          // 预留字段A
};


//-------------------------------10103500:融资融券负债偿还流水查询[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcRepaymentDetailField
{
    LONGLONG      llFundid;             // 资金帐号
    char          szPaykind[128 + 1];   // 偿还方式
    char          szDebtstype[128 + 1]; // 负债类型
    char          szMarket[128 + 1];    // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szBegindate[8 + 1];   // 开始日期
    char          szEnddate[8 + 1];     // 结束日期
    int           iReladate;            // 负债日期
    int           iRelasno;             // 负债流水号
    LONGLONG      llPositionid;         // 头寸编号
    int           iRelaLogbizdate;      // 关联资金流水日期
    int           iRelaLogsno;          // 关联资金流水序号
    char          szPosstr[32 + 1];     // 定位串
    int           iQrynum;              // 请求行数
};
struct MATRADEAPI CRspFislQryRpcRepaymentDetailField
{
    int           iSysdate;                   // 交收日期
    int           iSno;                       // 流水号
    LONGLONG      llCustid;                   // 客户代码
    char          szCustname[32 + 1];         // 客户姓名
    LONGLONG      llFundid;                   // 资金帐号
    char          chMoneytype;                // 货币代码
    int           iReladate;                  // 合约日期
    int           iRelasno;                   // 合约编号
    char          chMarket;                   // 交易市场
    char          szStkcode[10 + 1];          // 证券代码
    char          szStkname[32 + 1];          // 证券名称
    char          chDirect;                   // 发生方向
    char          chPaykind;                  // 偿还方式
    char          szCreditdebtstype[128 + 1]; // 负债类型
    char          szFundeffect[21 + 1];       // 资金发生数
    int           iStkeffect;                 // 股份发生数
    int           iSrcLogbizdate;             // 来源资金流水日期
    int           iSrcLogsno;                 // 来源资金流水序号
    int           iRelaLogbizdate;            // 关联资金流水日期
    int           iRelaLogsno;                // 关联资金流水序号
    LONGLONG      llRfscontractno;            // 约定合约编号
    LONGLONG      llPositionid;               // 头寸编号
};
//-------------------------------10103501:资金查询[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcFundField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspFislQryRpcFundField
{
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账户
    char          szOrgid[4 + 1];        // 机构编码
    char          chMoneytype;           // 货币
    char          szFundbal[21 + 1];     // 资金余额
    char          szFundavl[21 + 1];     // 资金可用金额
    char          szMarketvalue[21 + 1]; // 资产总值
    char          szFund[21 + 1];        // 资金资产
    char          szStkvalue[21 + 1];    // 市值
    int           iFundseq;              // 主资金标志
    char          szMaxdraw[21 + 1];     // 可取金额
};
//-------------------------------10103502:当日资金流水查询[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcFundDetailField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcFundDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iCleardate;           // 清算日期
    int           iBizdate;             // 交收日期
    int           iOrderdate;           // 发生日期
    int           iOrdertime;           // 发生时间
    int           iDigestid;            // 业务代码
    char          szDigestname[16 + 1]; // 业务说明
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 机构代码
    LONGLONG      llFundid;             // 资金帐号
    char          chMoneytype;          // 货币代码
    char          chMarket;             // 市场代码
    char          szSecuid[32 + 1];     // 股东代码
    char          szFundeffect[21 + 1]; // 资金发生数
    char          szFundbal[21 + 1];    // 资金本次余额
    int           iStkbal;              // 股份本次余额
    char          szOrderid[10 + 1];    // 合同序号
    char          szStkcode[16 + 1];    // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    char          szBsflag[2 + 1];      // 买卖类别
    int           iMatchqty;            // 成交数量
    char          szMatchprice[21 + 1]; // 成交价格
    char          szMatchamt[21 + 1];   // 成交金额
};
//-------------------------------10103503:新股申购客户市值额度查询[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcIPOSubscriptionLimitField
{
    char          szMarket[10 + 1]; // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szOrgid[4 + 1];   // 机构编码
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcIPOSubscriptionLimitField
{
    char          szPosstr[64 + 1]; // 定位串
    int           iServerid;        // 节点编号
    int           iDbfrec;          // 序号
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 机构编码
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llCustquota;      // 客户市值额度
    int           iReceivedate;     // 接收日期
    char          szRemark[32 + 1]; // 备注
};
//-------------------------------10103504:银证转帐[信用]--------------------------
struct MATRADEAPI CReqFislRpcBankStkTransField
{
    LONGLONG      llFundid;          // 资金账号
    char          chMoneytype;       // 货币代码
    char          szFundpwd[32 + 1]; // 资金密码
    char          szBankcode[4 + 1]; // 银行代码
    char          szBankpwd[32 + 1]; // 银行密码
    char          chBanktrantype;    // 转帐类型
    char          szTranamt[21 + 1]; // 转帐金额
};
struct MATRADEAPI CRspFislRpcBankStkTransField
{
    int           iSno;             // 委托序号
    int           iSyserrid;        // 错误代码
    char          szErrmsg[64 + 1]; // 错误信息
};
//-------------------------------10103505:查询账户余额[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcFundBalaField
{
    LONGLONG      llFundid;          // 资金账号
    char          chMoneytype;       // 货币代码
    char          szFundpwd[32 + 1]; // 资金密码
    char          szBankcode[4 + 1]; // 银行代码
    char          szBankpwd[32 + 1]; // 银行密码
};
struct MATRADEAPI CRspFislQryRpcFundBalaField
{
    int           iSno;                 // 委托序号
    char          szErrmsg[64 + 1];     // 错误信息
    int           iSyserrid;            // 错误代码
    char          szFundeffect[21 + 1]; // 银行余额
};
//-------------------------------10103506:银证转帐查询[信用]--------------------------
struct MATRADEAPI CReqFislQryRpcBankStkTransInfoField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币代码
    int           iSno;             // 转帐序号
    int           iBegindate;       // 转账开始日期
    int           iEnddate;         // 转账结束日期
};
struct MATRADEAPI CRspFislQryRpcBankStkTransInfoField
{
    int           iOperdate;            // 转帐日期
    int           iOpertime;            // 转帐时间
    LONGLONG      llFundid;             // 资金账号
    char          chMoneytype;          // 货币代码
    LONGLONG      llCustid;             // 客户代码
    char          szBankcode[4 + 1];    // 银行代码
    char          chBanktranid;         // 业务类型
    int           iSno;                 // 合同序号
    char          szFundeffect[21 + 1]; // 委托金额
    char          szFundbal[21 + 1];    // 余额
    char          szRemark[32 + 1];     // 备注信息
    char          chStatus;             // 处理结果
    char          chSourcetype;         // 发起方向
    char          szBankmsgid[16 + 1];  // 外部信息代码
    char          szBankmsg[64 + 1];    // 外部信息内容
    char          szErrormsg[64 + 1];   // 系统错误信息
    int           iSyserrid;            // 系统错误代码
};
//-------------------------------10103507:修改资金密码[信用]--------------------------
struct MATRADEAPI CReqFislRpcChgFundPwdField
{
    LONGLONG      llFundid;             // 资金帐户
    char          szOldfundpwd[32 + 1]; // 老资金密码
    char          szNewfundpwd[32 + 1]; // 新资金密码
};
struct MATRADEAPI CRspFislRpcChgFundPwdField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103508:修改交易密码[信用]--------------------------
struct MATRADEAPI CReqFislRpcChgTrdPwdField
{
    char          szNewpwd[32 + 1]; // 新密码
};
struct MATRADEAPI CRspFislRpcChgTrdPwdField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103509:获取开通转帐(银行)账号(410601) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcBankAcctField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chMoneytype;       // 货币代码
    LONGLONG      llFundid;          // 资金帐户
};
struct MATRADEAPI CRspFislQryRpcBankAcctField
{
    LONGLONG      llCustid;           // 客户代码
    char          szOrgid[4 + 1];     // 机构编码
    char          szBankcode[4 + 1];  // 银行代码
    char          szBankname[32 + 1]; // 银行名称
    char          chMoneytype;        // 货币代码
    char          szBankid[32 + 1];   // 银行帐户
    LONGLONG      llFundid;           // 资金帐号
    char          chLinkflag;         // 转帐标识
    char          chSourcetype;       // 发起方向
};
//-------------------------------10103510:获取开通外部(银行)账号(410602) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcOtherBankAcctField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chMoneytype;       // 货币代码
    LONGLONG      llFundid;          // 资金帐号
};
struct MATRADEAPI CRspFislQryRpcOtherBankAcctField
{
    LONGLONG      llCustid;            // 客户代码
    char          szOrgid[4 + 1];      // 机构编码
    char          szBankcode[4 + 1];   // 银行代码
    char          szBankid[32 + 1];    // 银行帐户
    char          szSubbankid[32 + 1]; // 银行子帐户
    LONGLONG      llFundid;            // 资金帐号
};
//-------------------------------10103511:当日新股清单查询(411549) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcIPOField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    int           iIssuedate;       // 申购日期
};
struct MATRADEAPI CRspFislQryRpcIPOField
{
    char          chMarket;           // 交易市场
    char          szStkcode[8 + 1];   // 证券代码
    char          szStkname[16 + 1];  // 证券名称
    char          szLinkstk[8 + 1];   // 正股代码
    char          chIssuetype;        // 发行方式
    int           iIssuedate;         // 申购日期
    char          szFixprice[21 + 1]; // 申购价格
    int           iBuyunit;           // 申购单位
    int           iMinqty;            // 申购最小数量
    int           iMaxqty;            // 申购最大数量
};
//-------------------------------10103512:配号查询(411518) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcDistributionField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szSecuid[32 + 1];  // 股东代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcDistributionField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iBizdate;              // 配号日期
    char          chMarket;              // 交易市场
    char          szSecuid[32 + 1];      // 股东代码
    char          szStkname[16 + 1];     // 证券名称
    char          szStkcode[16 + 1];     // 证券代码
    int           iMatchqty;             // 成交数量
    char          szMateno[32 + 1];      // 申购配号
    char          szOrderid[10 + 1];     // 委托号
    int           iOrderdate;            // 委托日期
    char          szMateerrmsg[128 + 1]; // 备注
};
//-------------------------------10103513:市值配售中签查询(411547) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcMktCapPlacingField
{
    char          szSecuid[10 + 1]; // 股东代码
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          chIssuetype;      // 发行方式
};
struct MATRADEAPI CRspFislQryRpcMktCapPlacingField
{
    LONGLONG      llCustid;             // 客户代码
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    int           iOrderdate;           // 委托日期
    int           iMatchdate;           // 中签日期
    int           iHitqty;              // 中签数量
    int           iGiveupqty;           // 放弃数量
    char          szMatchprice[21 + 1]; // 价格
    char          szHitamt[21 + 1];     // 中签金额
    int           iPayqty;              // 已缴款数量
    char          szPayamt[21 + 1];     // 已缴款金额
    char          szFrzamt[21 + 1];     // 中签冻结金额
    int           iClearsno;            // 交收顺序
    char          chIssuetype;          // 发行方式
    char          chStatus;             // 流程状态
};
//-------------------------------10103514:中签历史查询(411560) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcWinningHistoryField
{
    char          szSecuid[10 + 1];  // 股东代码
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          chIssuetype;       // 发行方式
    int           iBegindate;        // 中签开始日期
    int           iEnddate;          // 中签结束日期
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcWinningHistoryField
{
    char          szPoststr[32 + 1];    // 定位串
    LONGLONG      llCustid;             // 客户代码
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    int           iOrderdate;           // 委托日期
    int           iMatchdate;           // 中签日期
    int           iHitqty;              // 中签数量
    int           iGiveupqty;           // 放弃数量
    char          szMatchprice[21 + 1]; // 价格
    char          szHitamt[21 + 1];     // 中签金额
    int           iPayqty;              // 已缴款数量
    char          szPayamt[21 + 1];     // 已缴款金额
    int           iClearsno;            // 交收顺序
    char          chIssuetype;          // 发行方式
    char          chStatus;             // 流程状态
};
//-------------------------------10103515:市值配售交收顺序修改（411548）[信用]--------------------------
struct MATRADEAPI CReqFislRpcChgMktCapDistDeliOrderField
{
    int           iMatchdate;       // 中签日期
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    int           iClearsno;        // 交收顺序
};
struct MATRADEAPI CRspFislRpcChgMktCapDistDeliOrderField
{
    int           iSerialno; // 操作流水号
};
//-------------------------------10103516:交割单查询(411520) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcDeliOrderField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcDeliOrderField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iCleardate;           // 清算日期
    int           iBizdate;             // 交收日期
    int           iOrderdate;           // 委托日期
    int           iOrdertime;           // 委托时间
    int           iDigestid;            // 业务代码
    char          szDigestname[16 + 1]; // 业务代码说明
    LONGLONG      llFundid;             // 资金帐户
    char          chMoneytype;          // 货币代码
    char          chMarket;             // 交易所代码
    char          szSecuid[32 + 1];     // 股东代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrderid[10 + 1];    // 合同序号
    char          szStkcode[16 + 1];    // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    char          szBsflag[2 + 1];      // 买卖类别
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交号码
    int           iMatchtimes;          // 成交笔数
    int           iMatchqty;            // 成交数量
    char          szMatchprice[21 + 1]; // 成交价格
    char          szMatchamt[21 + 1];   // 成交金额
    char          szFundeffect[21 + 1]; // 清算金额
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeJsxf[21 + 1];    // 标准手续费
    char          szFeeSxf[21 + 1];     // 手续费
    char          szFeeGhf[21 + 1];     // 过户费
    char          szFeeQsf[21 + 1];     // 清算费
    char          szFeeJygf[21 + 1];    // 交易规费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFeeJsf[21 + 1];     // 经手费
    char          szFeeZgf[21 + 1];     // 证管费
    char          szFundbal[21 + 1];    // 资金本次余额
    int           iStkbal;              // 股份本次余额
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          chSourcetype;         // 发起方
    char          szBankcode[4 + 1];    // 外部银行
    char          szBankid[32 + 1];     // 外部账户
};
//-------------------------------10103517:对帐查询(411521) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcChkAcctField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcChkAcctField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iCleardate;           // 清算日期
    int           iBizdate;             // 交收日期
    int           iSno;                 // 资金流水序号
    int           iOrderdate;           // 发生日期
    int           iOrdertime;           // 发生时间
    int           iDigestid;            // 业务代码
    char          szDigestname[16 + 1]; // 业务说明
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 机构代码
    LONGLONG      llFundid;             // 资金帐号
    char          chMoneytype;          // 货币代码
    char          chMarket;             // 市场代码
    char          szSecuid[32 + 1];     // 股东代码
    char          szFundeffect[21 + 1]; // 资金发生数
    char          szFundbal[21 + 1];    // 资金本次余额
    int           iStkbal;              // 股份本次余额
    char          szOrderid[10 + 1];    // 合同序号
    char          szStkcode[16 + 1];    // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    char          szBsflag[2 + 1];      // 买卖类别
    int           iMatchqty;            // 成交数量
    char          szMatchprice[21 + 1]; // 成交价格
    char          szMatchamt[21 + 1];   // 成交金额
};
//-------------------------------10103518:当日委托汇总(410514) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcOrderSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspFislQryRpcOrderSummaryField
{
    int           iOrderdate;            // 委托日期
    LONGLONG      llCustid;              // 客户代码
    char          szCustname[16 + 1];    // 客户姓名
    char          szOrgid[4 + 1];        // 机构编码
    char          szBsflag[2 + 1];       // 买卖类别
    char          chMarket;              // 交易市场
    char          szStkname[8 + 1];      // 证券名称
    char          szStkcode[8 + 1];      // 证券代码
    char          szOrderprice[21 + 1];  // 委托价格
    LONGLONG      llOrderqty;            // 委托数量
    char          szOrderfrzamt[21 + 1]; // 委托金额
    LONGLONG      llMatchqty;            // 成交数量
    LONGLONG      llCancelqty;           // 撤单数量
    char          szMatchamt[21 + 1];    // 成交金额
};
//-------------------------------10103519:当日成交汇总(410516) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcMatchSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspFislQryRpcMatchSummaryField
{
    int           iOperdate;            // 交易日期
    LONGLONG      llCustid;             // 客户代码
    char          szOrgid[4 + 1];       // 机构编码
    char          szBsflag[2 + 1];      // 买卖类别
    char          chMarket;             // 交易市场
    char          szStkname[8 + 1];     // 证券名称
    char          szStkcode[8 + 1];     // 证券代码
    char          szMatchprice[21 + 1]; // 成交价格
    LONGLONG      llMatchqty;           // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    char          szBankcode[4 + 1];    // 外部银行
    char          szBankid[32 + 1];     // 外部账户
};
//-------------------------------10103520:历史委托明细查询(411511) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcHistoryOrderDetailField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    char          chCreditmethod;    // 约定业务分类
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcHistoryOrderDetailField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iOrderdate;            // 委托日期
    LONGLONG      llCustid;              // 客户代码
    char          szCustname[16 + 1];    // 客户姓名
    LONGLONG      llFundid;              // 资金账户
    char          chMoneytype;           // 货币
    char          szOrgid[4 + 1];        // 机构编码
    char          szSecuid[10 + 1];      // 股东代码
    char          szBsflag[2 + 1];       // 买卖类别
    char          szOrderid[10 + 1];     // 申报合同序号
    int           iReporttime;           // 报盘时间
    int           iOpertime;             // 委托时间
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 证券名称
    char          szStkname[8 + 1];      // 证券代码
    char          szOrderprice[21 + 1];  // 委托价格
    int           iOrderqty;             // 委托数量
    char          szOrderfrzamt[21 + 1]; // 冻结金额
    int           iMatchqty;             // 成交数量
    char          szMatchamt[21 + 1];    // 成交金额
    int           iCancelqty;            // 撤单数量
    char          chOrderstatus;         // 委托状态
    char          szSeat[6 + 1];         // 交易席位
    int           iOperdate;             // 操作日期
    char          chCreditid;            // 信用交易
    char          chCreditflag;          // 信用交易类型
    char          chCreditmethod;        // 约定业务分类
};
//-------------------------------10103521:历史成交明细查询(411513) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcHistoryMatchDetailField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcHistoryMatchDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iBizdate;             // 成交日期
    char          szSecuid[10 + 1];     // 股东代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szOrderid[10 + 1];    // 申报合同序号
    int           iOrdersno;            // 委托序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    int           iStkbal;              // 股份本次余额
};
//-------------------------------10103522:转帐银行业务信息(410211) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcBankTransBasicInfoField
{
    char          chMoneytype;       // 货币代码
    char          szBankcode[4 + 1]; // 银行代码
};
struct MATRADEAPI CRspFislQryRpcBankTransBasicInfoField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chSourcetype;      // 发起方
    char          chBanktrantype;    // 转帐方式
    char          chFundpwdflag;     // 资金密码校验
    char          chBankpwdflag;     // 银行密码校验
    char          chCheckbankid;     // 银行帐号校验
    char          chCheckidno;       // 证件校验标志
    char          szOrgid[4 + 1];    // 机构编码
    char          chMoneytype;       // 货币代码
    char          chStatus;          // 银行状态
};
//-------------------------------10103523:对帐单查询(411525) [信用]--------------------------
struct MATRADEAPI CReqFislQryRpcStatementField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMoneytype;       // 货币代码
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcStatementField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iCleardate;           // 清算日期
    int           iBizdate;             // 交收日期
    int           iSno;                 // 资金流水序号
    int           iOrderdate;           // 发生日期
    int           iOrdertime;           // 发生时间
    int           iDigestid;            // 业务代码
    char          szDigestname[16 + 1]; // 业务说明
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 机构代码
    LONGLONG      llFundid;             // 资金帐号
    char          chMoneytype;          // 货币代码
    char          chMarket;             // 市场代码
    char          szSecuid[32 + 1];     // 股东代码
    char          szFundeffect[21 + 1]; // 资金发生数
    char          szFundbal[21 + 1];    // 资金本次余额
    int           iStkbal;              // 股份本次余额
    char          szOrderid[10 + 1];    // 合同序号
    char          szStkcode[16 + 1];    // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    char          szBsflag[2 + 1];      // 买卖类别
    int           iMatchqty;            // 成交数量
    char          szMatchprice[21 + 1]; // 成交价格
    char          szMatchamt[21 + 1];   // 成交金额
    char          szStkeffect[21 + 1];  // 股份发生数
};
//-------------------------------10103527:证券信息--------------------------
struct MATRADEAPI CReqFislRpcStkInfoField
{
    char          chMarket;         // 交易市场
    char          chStklevel;       // 证券级别
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislRpcStkInfoField
{
    char          chMarket;               // 交易市场
    char          chMoneytype;            // 货币
    char          szStkname[8 + 1];       // 证券名称
    char          szStkcode[8 + 1];       // 证券代码
    char          chStktype;              // 证券类别
    int           iPriceunit;             // 价位
    char          szMaxrisevalue[21 + 1]; // 涨停价格
    char          szMaxdownvalue[21 + 1]; // 跌停价格
    char          chStopflag;             // 停牌标志
    char          chMtkcalflag;           // 市值计算标识
    char          szBondintr[21 + 1];     // 债券应计利息
    int           iMaxqty;                // 最高数量
    int           iMinqty;                // 最低数量
    int           iBuyunit;               // 买入最小单位
    int           iSaleunit;              // 卖出最小单位
    char          chStklevel;             // 证券级别
    int           iQuitdate;              // 退市证券交易截止日期
    char          szMemotext[128 + 1];    // 退市证券提示信息
};
//-------------------------------10103528:机构编码信息(业务机构)--------------------------
struct MATRADEAPI CReqFislRpcOrgInfoField
{
    char          szQryorgid[4 + 1]; // 机构编码
};
struct MATRADEAPI CRspFislRpcOrgInfoField
{
    char          szOrgid[4 + 1];    // 机构编码
    char          szOrgname[32 + 1]; // 机构简称
};
//-------------------------------10103526:取字典信息--------------------------
struct MATRADEAPI CReqFislRpcDictInfoField
{
    char          szDictitem[8 + 1]; // 字典条目
    char          szSubitem[4 + 1];  // 字典子项
};
struct MATRADEAPI CRspFislRpcDictInfoField
{
    char          szOrgid[4 + 1];        // 机构代码
    char          szDictitem[8 + 1];     // 字典条目
    char          szSubitem[4 + 1];      // 字典子项
    char          szSubitemname[32 + 1]; // 子项名称
};
//-------------------------------10103525:查询当前系统状态--------------------------
struct MATRADEAPI CReqFislQryRpcCurSysStatField
{
};
struct MATRADEAPI CRspFislQryRpcCurSysStatField
{
    int           iSysdate;              // 系统日期
    int           iOrderdate;            // 委托日期
    char          chStatus;              // 系统运行状态
    char          szNightremark[32 + 1]; // 夜市状态注释
    int           iSystime;              // 物理时间
};
//-------------------------------10103530:用户登录--------------------------
struct MATRADEAPI CReqFislRpcLoginField
{
    char          chInputtype;         // 登录类型
    char          szInputid[64 + 1];   // 登录标识
    char          szProcName[128 + 1]; // 第三方厂商名称
    char          szLoginip[64 + 1];   // 登录ip
    char          szLoginmac[32 + 1];  // 登录mac
};
struct MATRADEAPI CRspFislRpcLoginField
{
    char          chCustprop;             // 客户性质
    char          chMarket;               // 交易市场
    char          szSecuid[10 + 1];       // 股东代码
    char          chSecuidtype;           // 股东帐户类型
    char          szName[16 + 1];         // 股东姓名
    LONGLONG      llFundid;               // 缺省资金帐户
    char          chFundidtype;           // 资金帐户类型
    LONGLONG      llCustid;               // 客户代码
    char          szCustname[16 + 1];     // 客户姓名
    char          szOrgid[4 + 1];         // 机构编码
    char          szBankcode[4 + 1];      // 银行代码
    char          szIdentitysign[64 + 1]; // 数字签名
    char          chTimeoutflag;          // 延时属性
    char          chAuthlevel;            // 认证方式/级别
    int           iPwderrtimes;           // 登录错误次数
    char          szCustcert[128 + 1];    // 客户证书
    char          szLastlogindate[8 + 1]; // 上次登录日期
    char          szLastlogintime[8 + 1]; // 上次登录时间
    char          szLastloginip[64 + 1];  // 上次登录ip
    char          szLastloginmac[32 + 1]; // 上次登录mac
};
//-------------------------------10103524:股东挂失--------------------------
struct MATRADEAPI CReqFislRpcTrdacctReportLossField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspFislRpcTrdacctReportLossField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103529:资金帐户挂失--------------------------
struct MATRADEAPI CReqFislRpcCuacctReportLossField
{
    LONGLONG      llFundid;          // 资金账户
    char          szFundpwd[32 + 1]; // 资金密码
};
struct MATRADEAPI CRspFislRpcCuacctReportLossField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103531:设置主股东帐户--------------------------
struct MATRADEAPI CReqFislRpcSetMainTrdacctField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspFislRpcSetMainTrdacctField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103532:设置主资产账号--------------------------
struct MATRADEAPI CReqFislRpcSetMainCuacctField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspFislRpcSetMainCuacctField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103533:修改联系方式(网上交易使用)--------------------------
struct MATRADEAPI CReqFislRpcChgContactField
{
    char          szAddr[64 + 1];     // 地址
    char          szPostid[16 + 1];   // 邮编
    char          szTelno[32 + 1];    // 电话
    char          szMobileno[32 + 1]; // 移动电话
    char          szEmail[32 + 1];    // EMAIL
    char          chContact;          // 联络方式
    char          chContactfrep;      // 联络频率
};
struct MATRADEAPI CRspFislRpcChgContactField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103534:客户资料查询--------------------------
struct MATRADEAPI CReqFislQryRpcCustInfoField
{
};
struct MATRADEAPI CRspFislQryRpcCustInfoField
{
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    char          szOrgid[4 + 1];     // 机构编码
    char          chSex;              // 性别
    char          szAddr[64 + 1];     // 地址
    char          szPostid[16 + 1];   // 邮编
    char          szTelno[32 + 1];    // 电话
    char          szMobileno[32 + 1]; // 移动电话
    char          szEmail[32 + 1];    // EMAIL
    int           iOpendate;          // 开户日期
    char          chContact;          // 联络方式
    char          chContactfrep;      // 联络频率
    char          szRemark[32 + 1];   // 备注信息
};
//-------------------------------10103535:修改代理权限--------------------------
struct MATRADEAPI CReqFislRpcChgProxyPermissonField
{
    LONGLONG      llFundid;              // 资金账户
    char          szAgentright[128 + 1]; // 代理权限
};
struct MATRADEAPI CRspFislRpcChgProxyPermissonField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103536:修改操作方式--------------------------
struct MATRADEAPI CReqFislRpcChgOperModeField
{
    LONGLONG      llFundid;              // 资金账户
    char          szNewoperway[128 + 1]; // 操作方式
};
struct MATRADEAPI CRspFislRpcChgOperModeField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103537:查询客户操作方式--------------------------
struct MATRADEAPI CReqFislQryRpcCustOperModeField
{
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspFislQryRpcCustOperModeField
{
    LONGLONG      llFundid;           // 资金帐户
    char          szOperway[128 + 1]; // 操作方式
};
//-------------------------------10103538:修改客户联系人信息--------------------------
struct MATRADEAPI CReqFislRpcChgCustInfoField
{
    LONGLONG      llCustid;           // 客户代码
    char          szLinkman[16 + 1];  // 联系人姓名
    char          chSex;              // 性别
    char          chOcctype;          // 职业
    char          chIdtype;           // 证件类型
    char          szIdno[32 + 1];     // 证件号码
    char          szTelno[32 + 1];    // 家庭电话
    char          szTelnoCo[32 + 1];  // 单位电话
    char          szMobileno[32 + 1]; // 移动电话
    char          szFax[32 + 1];      // 传真号码
    char          szEmail[32 + 1];    // EMAIL
    char          szAddr[64 + 1];     // 地址
    char          szPostid[16 + 1];   // 邮编
    char          szRemark[32 + 1];   // 备注信息
};
struct MATRADEAPI CRspFislRpcChgCustInfoField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103540:查询客户资金帐号权限--------------------------
struct MATRADEAPI CReqFislQryRpcCuacctPermissionField
{
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspFislQryRpcCuacctPermissionField
{
    char          szOrgid[4 + 1];       // 营业部代码
    LONGLONG      llFundid;             // 资金帐户
    char          szFundname[16 + 1];   // 资金姓名
    char          szFundright[512 + 1]; // 资金权限
};
//-------------------------------10103539:修改资金帐号权限--------------------------
struct MATRADEAPI CReqFislRpcChgCuacctPermissionField
{
    LONGLONG      llFundid;           // 资金账户
    char          szFundright[2 + 1]; // 资金权限
    char          chAction;           // 操作类型
};
struct MATRADEAPI CRspFislRpcChgCuacctPermissionField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103541:查询客户证券帐号权限--------------------------
struct MATRADEAPI CReqFislQryRpcTrdacctPermissionField
{
    char          szSecuid[10 + 1]; // 证券账号
    char          chMarket;         // 市场
};
struct MATRADEAPI CRspFislQryRpcTrdacctPermissionField
{
    char          chMarket;             // 市场
    char          szSecuid[10 + 1];     // 证券账号
    char          szSecuright[512 + 1]; // 证券权限
};
//-------------------------------10103542:设置客户证券帐号权限--------------------------
struct MATRADEAPI CReqFislRpcSetTrdacctPermissionField
{
    char          chAction;            // 操作类型
    char          chMarket;            // 市场
    char          szSecuid[10 + 1];    // 证券账号
    char          szFundright[2 + 1];  // 证券权限
    int           iSigndate;           // 签署日期
    int           iEffectdate;         // 权限生效日期
    char          szRemark[64 + 1];    // 备注信息
};
struct MATRADEAPI CRspFislRpcSetTrdacctPermissionField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103543:融资融券额度上限查询--------------------------
struct MATRADEAPI CReqFislQryRpcUpLimitQuotaField
{
    LONGLONG      llFundid;         // 资金账号
};
struct MATRADEAPI CRspFislQryRpcUpLimitQuotaField
{
    char          szLimitamt[21 + 1];     // 额度上限
    char          szFundlimitamt[21 + 1]; // 原融资额度
    char          szStklimitamt[21 + 1];  // 原融券额度
    char          szAlllimitamt[21 + 1];  // 原总额度
};
//-------------------------------10103544:融资融券额度变更--------------------------
struct MATRADEAPI CReqFislRpcChgLimitQuotaField
{
    LONGLONG      llFundid;                 // 资金账号
    char          szApFundlimitamt[21 + 1]; // 融资额度
    char          szApStklimitamt[21 + 1];  // 融券额度
    char          szApAlllimitamt[21 + 1];  // 总额度
};
struct MATRADEAPI CRspFislRpcChgLimitQuotaField
{
};
//-------------------------------10103545:额度变更申请--------------------------
struct MATRADEAPI CReqFislRpcChgLimitQuotaApplicationField
{
    LONGLONG      llFundid;                 // 资金账号
    char          szApFundlimitamt[21 + 1]; // 融资额度
    char          szApStklimitamt[21 + 1];  // 融券额度
    char          szApAlllimitamt[21 + 1];  // 总额度
};
struct MATRADEAPI CRspFislRpcChgLimitQuotaApplicationField
{
};
//-------------------------------10103546:额度变更申请取消--------------------------
struct MATRADEAPI CReqFislRpcChgLimitQuotaApplicationCancelField
{
    int           iSno;              // 申请编号
    char          szRemark[255 + 1]; // 备注
};
struct MATRADEAPI CRspFislRpcChgLimitQuotaApplicationCancelField
{
};
//-------------------------------10103547:额度变更申请查询--------------------------
struct MATRADEAPI CReqFislQryRpcChgLimitQuotaApplicationField
{
    LONGLONG      llFundid;          // 信用资金账号
    int           iSno;              // 申请编号
    int           iApplydatebegin;   // 申请日期开始
    int           iApplydateend;     // 申请日期结束
    char          szStatus[128 + 1]; // 申请状态
    int           iQrynum;           // 查询条数
    char          szPosstr[64 + 1];  // 定位串
};
struct MATRADEAPI CRspFislQryRpcChgLimitQuotaApplicationField
{
    int           iServerid;                // 核心编号
    int           iSno;                     // 申请编号
    int           iApplydate;               // 申请日期
    LONGLONG      llFundid;                 // 资金账号
    LONGLONG      llCustid;                 // 客户代码
    char          szOrgid[4 + 1];           // 机构编码
    char          szApFundlimitamt[21 + 1]; // 申请融资额度
    char          szApStklimitamt[21 + 1];  // 申请融券额度
    char          szApAlllimitamt[21 + 1];  // 申请总额度
    char          chStatus;                 // 申请状态
    int           iApprovaldate;            // 审批日期
    char          szChkremark[255 + 1];     // 审批备注
    char          szRemark[255 + 1];        // 备注
    char          szPosstr[64 + 1];         // 定位串
};
//-------------------------------10103571:取证券余额及当前最新价等,支持三方交易--------------------------
struct MATRADEAPI CReqFislRpcStkAvlAndLatestPriceField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金账户
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislRpcStkAvlAndLatestPriceField
{
    char          chMarket;            // 交易市场
    char          szStkcode[6 + 1];    // 证券代码
    char          szStkname[10 + 1];   // 证券名称
    char          szOrgid[4 + 1];      // 机构代码
    LONGLONG      llFundid;            // 资金帐户
    char          szSecuid[10 + 1];    // 股东代码
    char          chMoneytype;         // 货币代码
    char          szFundavl[21 + 1];   // 资金可用
    char          szFundbal[21 + 1];   // 资金余额
    int           iStkavl;             // 证券可用
    int           iStkmaxqty;          // 证券可卖数量
    char          szLastprice[21 + 1]; // 最近价格
    char          szUpprice[21 + 1];   // 涨停价格
    char          szDownprice[21 + 1]; // 跌停价格
    char          chHandflag;          // 股手标志
    char          szBondintr[21 + 1];  // 国债利息
};
//-------------------------------10103572:取最大交易数量,支持三方交易--------------------------
struct MATRADEAPI CReqFislRpcMaxStkQtyField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    LONGLONG      llFundid;          // 资金账户
    char          szStkcode[8 + 1];  // 证券代码
    char          chBsflag;          // 买卖类别
    char          szPrice[21 + 1];   // 价格
    char          szBankcode[4 + 1]; // 外部银行
    char          chCreditid;        // 信用交易
    char          chCreditflag;      // 信用交易类型
    char          chCreditmethod;    // 约定业务分类
    LONGLONG      llRfscontractno;   // 约定合约编号
    char          chHiqtyflag;       // 最大上限标志
    LONGLONG      llPositionid;      // 头寸编号
};
struct MATRADEAPI CRspFislRpcMaxStkQtyField
{
    int           iMaxstkqty; // 股份数量
};
//-------------------------------10103573:委托买卖业务,支持三方交易，外围约定融资融券委托--------------------------
struct MATRADEAPI CReqFislRpcOrderField
{
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    LONGLONG      llFundid;            // 资金账户
    char          szStkcode[8 + 1];    // 证券代码
    char          szBsflag[2 + 1];     // 买卖类别
    char          szPrice[21 + 1];     // 价格
    int           iQty;                // 数量
    int           iOrdergroup;         // 委托批号
    char          szBankcode[4 + 1];   // 外部银行
    char          szRemark[64 + 1];    // 备注信息
    char          szTargetseat[6 + 1]; // 对方席位
    int           iPromiseno;          // 约定号
    char          chCreditid;          // 信用交易
    char          chCreditflag;        // 信用交易类型
    char          chCreditmethod;      // 约定业务分类
    LONGLONG      llRfscontractno;     // 约定合约编号
    LONGLONG      llPositionid;        // 头寸编号
    char          szLinkman[12 + 1];   // 联系人
    char          szLinkway[30 + 1];   // 联系方式
};
struct MATRADEAPI CRspFislRpcOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10103574:委托撤单--------------------------
struct MATRADEAPI CReqFislRpcOrderCancelField
{
    int           iOrderdate;       // 委托日期
    LONGLONG      llFundid;         // 资金帐户
    int           iOrdersno;        // 委托序号
};
struct MATRADEAPI CRspFislRpcOrderCancelField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103575:委托撤单查询--------------------------
struct MATRADEAPI CReqFislQryRpcOrderCancelField
{
    int           iOrderdate;        // 委托日期
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcOrderCancelField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iOrdersno;            // 委托序号
    int           iOrdergroup;          // 委托批号
    char          szOrcderid[10 + 1];   // 合同序号
    int           iOrderdate;           // 委托日期
    int           iOpertime;            // 委托时间
    LONGLONG      llFundid;             // 资金帐户
    char          chMarket;             // 客户代码
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szOrderprice[21 + 1]; // 委托价格
    int           iOrderqty;            // 委托数量
    int           iMatchqty;            // 成交数量
    char          chOrderstatus;        // 委托状态
    char          chCreditid;           // 信用交易
    char          chCreditflag;         // 信用交易类型
};
//-------------------------------10103576:重置成本--------------------------
struct MATRADEAPI CReqFislRpcResetCostField
{
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    char          szStkcode[8 + 1];    // 证券代码
    LONGLONG      llFundid;            // 资金帐户
    char          szCostprice[21 + 1]; // 成本价格
};
struct MATRADEAPI CRspFislRpcResetCostField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10103577:盈亏费用计算--------------------------
struct MATRADEAPI CReqFislRpcCalcFeeField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金帐户
    char          szPrice[21 + 1];  // 成交价格
    int           iQty;             // 成交数量
};
struct MATRADEAPI CRspFislRpcCalcFeeField
{
    char          szTradefee[21 + 1]; // 总费用
};
//-------------------------------10103676:客户查询，查询客户股东代码--------------------------
struct MATRADEAPI CReqFislQryRpcTrdacctField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcTrdacctField
{
    char          szPoststr[32 + 1];  // 定位串
    LONGLONG      llCustid;           // 客户代码
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    char          szName[16 + 1];     // 股东姓名
    int           iSecuseq;           // 股东序号
    char          chSecuidtype;       // 股东帐户类型
    char          szNrsecuid[10 + 1]; // 普通股东代码
    char          szNrseat[6 + 1];    // 普通股东席位
};
//-------------------------------10103548:资金查询--------------------------
struct MATRADEAPI CReqFislQryRpcCuacctField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspFislQryRpcCuacctField
{
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账户
    char          szOrgid[4 + 1];        // 机构编码
    char          chMoneytype;           // 货币
    char          szFundbal[21 + 1];     // 资金余额
    char          szFundavl[21 + 1];     // 资金可用金额
    char          szMarketvalue[21 + 1]; // 资产总值
    char          szFund[21 + 1];        // 资金资产
    char          szStkvalue[21 + 1];    // 市值
    int           iFundseq;              // 主资金标志
    char          szCreditbal[21 + 1];   // 信用资金余额
    char          szCreditavl[21 + 1];   // 信用资金可用
};
//-------------------------------10103679:股份明细查询--------------------------
struct MATRADEAPI CReqFislQryRpcStkDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcStkDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    LONGLONG      llCustid;             // 客户代码
    LONGLONG      llFundid;             // 资金账户
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkname[8 + 1];     // 证券名称
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrgid[4 + 1];       // 机构编码
    char          chMoneytype;          // 货币
    int           iStkbal;              // 股份余额
    int           iStkavl;              // 股份可用
    int           iCreditstkbal;        // 融资买入股份余额
    int           iCreditstkavl;        // 融资买入股份可用
    int           iStkremain;           // 客户余券
    char          szBuycost[21 + 1];    // 当前成本
    char          szCostprice[21 + 1];  // 成本价格
    char          szMktval[21 + 1];     // 市值
    char          szIncome[21 + 1];     // 盈亏
    char          chMtkcalflag;         // 市值计算标识
    int           iStkqty;              // 当前拥股数
    char          szLastprice[21 + 1];  // 最新价格
    char          chStktype;            // 证券类型
    char          szProfitcost[21 + 1]; // 浮动盈亏
    LONGLONG      llStkdiff;            // 可申赎数量
    char          chIspledge;           // 是否为担保品
    char          szPledgerate[21 + 1]; // 担保品折算率
};
//-------------------------------10103680:股份汇总查询--------------------------
struct MATRADEAPI CReqFislQryRpcStkSummaryField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspFislQryRpcStkSummaryField
{
    LONGLONG      llCustid;              // 客户代码
    char          chMarket;              // 交易市场
    char          szStkname[8 + 1];      // 证券名称
    char          szStkcode[8 + 1];      // 证券代码
    char          chMoneytype;           // 货币
    int           iStkbal;               // 股份余额
    int           iStkavl;               // 股份可用
    int           iCreditstkbal;         // 融资买入股份余额
    int           iCreditstkavl;         // 融资买入股份可用
    char          szBuycost[21 + 1];     // 当前成本
    char          szCostprice[21 + 1];   // 成本价格
    char          szMktval[21 + 1];      // 市值
    char          szIncome[21 + 1];      // 盈亏
    char          chMtkcalflag;          // 市值计算标识
    int           iStkqty;               // 当前拥股数
    char          szLastprice[21 + 1];   // 最新价格
    char          chStktype;             // 证券类型
    char          szProincome[21 + 1];   // 参考盈亏
    char          szProfitcost[21 + 1];  // 参考成本
    char          szProfitprice[21 + 1]; // 参考成本价
};
//-------------------------------10103681:资金帐户查询--------------------------
struct MATRADEAPI CReqFislQryRpcCuacct2Field
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspFislQryRpcCuacct2Field
{
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    LONGLONG      llFundid;           // 资金账户
    char          szOrgid[4 + 1];     // 机构编码
    char          chMoneytype;        // 货币
    char          szFundavl[21 + 1];  // 资金可用
    char          szFundbal[21 + 1];  // 资金余额
};
//-------------------------------10103682:客户查询，根据股东内码，查询股东代码--------------------------
struct MATRADEAPI CReqFislQryRpcTrdacct2Field
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspFislQryRpcTrdacct2Field
{
    LONGLONG      llCustid;         // 客户代码
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
//-------------------------------10103683:客户查询，根据股东代码，资金帐户查询客户代码--------------------------
struct MATRADEAPI CReqFislQryRpcTrdacct3Field
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspFislQryRpcTrdacct3Field
{
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    char          szOrgid[4 + 1];     // 机构代码
    char          szBankcode[4 + 1];  // 银行代码
    LONGLONG      llFundid;           // 资金帐户
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
};
//-------------------------------10103684:当日委托明细查询--------------------------
struct MATRADEAPI CReqFislQryRpcOrderDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
    char          chCreditmethod;    // 约定业务分类
};
struct MATRADEAPI CRspFislQryRpcOrderDetailField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iOrderdate;            // 委托日期
    int           iOrdersno;             // 委托序号
    LONGLONG      llCustid;              // 客户代码
    char          szCustname[16 + 1];    // 客户姓名
    LONGLONG      llFundid;              // 资金账户
    char          chMoneytype;           // 货币
    char          szOrgid[4 + 1];        // 机构编码
    char          szSecuid[10 + 1];      // 股东代码
    char          szBsflag[2 + 1];       // 买卖类别
    char          szOrderid[10 + 1];     // 申报合同序号
    int           iReporttime;           // 报盘时间
    int           iOpertime;             // 委托时间
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 证券名称
    char          szStkname[8 + 1];      // 证券代码
    char          szOrderprice[21 + 1];  // 委托价格
    int           iOrderqty;             // 委托数量
    char          szOrderfrzamt[21 + 1]; // 冻结金额
    int           iMatchqty;             // 成交数量
    char          szMatchamt[21 + 1];    // 成交金额
    int           iCancelqty;            // 撤单数量
    char          chOrderstatus;         // 委托状态
    char          szSeat[6 + 1];         // 交易席位
    char          chCancelflag;          // 撤单标识
    int           iOperdate;             // 操作日期
    char          chCreditid;            // 信用交易
    char          chCreditflag;          // 融资开仓平仓强平
    char          chCreditmethod;        // 约定业务分类
};
//-------------------------------10103686:当日成交明细查询--------------------------
struct MATRADEAPI CReqFislQryRpcMatchDetailField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcMatchDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iTrddate;             // 成交日期
    char          szSecuid[10 + 1];     // 股东代码
    char          szBsflag[2 + 1];      // 买卖类别
    int           iOrdersno;            // 委托序号
    char          szOrderid[10 + 1];    // 申报合同序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
};
//-------------------------------10103689:历史委托汇总－按证券代码和买卖方向--------------------------
struct MATRADEAPI CReqFislRpcHisOrderSummaryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspFislRpcHisOrderSummaryField
{
    int           iOrderdate;            // 委托日期
    LONGLONG      llCustid;              // 客户代码
    char          szCustname[16 + 1];    // 客户姓名
    char          szOrgid[4 + 1];        // 机构编码
    char          szBsflag[2 + 1];       // 买卖类别
    char          chMarket;              // 交易市场
    char          szStkname[8 + 1];      // 证券名称
    char          szStkcode[8 + 1];      // 证券代码
    char          szOrderprice[21 + 1];  // 委托价格
    LONGLONG      llOrderqty;            // 委托数量
    char          szOrderfrzamt[21 + 1]; // 委托金额
    LONGLONG      llMatchqty;            // 成交数量
    LONGLONG      llCancelqty;           // 撤单数量
    char          szMatchamt[21 + 1];    // 成交金额
};
//-------------------------------10103690:历史成交汇总－按证券代码和买卖方向--------------------------
struct MATRADEAPI CReqFislRpcHisMatchSummaryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspFislRpcHisMatchSummaryField
{
    int           iBizdate;             // 发生日期
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 机构编码
    char          szBsflag[2 + 1];      // 买卖类别
    char          chMarket;             // 交易市场
    char          szStkname[8 + 1];     // 证券名称
    char          szStkcode[8 + 1];     // 证券代码
    char          szMatchprice[21 + 1]; // 成交价格
    LONGLONG      llMatchqty;           // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          szBankcode[4 + 1];    // 外部银行
    char          szBankid[32 + 1];     // 外部账户
};
//-------------------------------10103549:中签查询--------------------------
struct MATRADEAPI CReqFislQryRpcLotteryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szSecuid[32 + 1];  // 股东代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcLotteryField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iBizdate;             // 中签日期
    char          chMarket;             // 交易市场
    char          szSecuid[32 + 1];     // 股东代码
    char          szStkname[16 + 1];    // 证券名称
    char          szStkcode[16 + 1];    // 证券代码
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
};
//-------------------------------10103692:当日委托汇总ordergroup+stkcode+bsflag--------------------------
struct MATRADEAPI CReqFislRpcOrderSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspFislRpcOrderSummaryField
{
    int           iOrderdate;            // 委托日期
    int           iOrdergroup;           // 委托批号
    LONGLONG      llCustid;              // 客户代码
    char          szCustname[16 + 1];    // 客户姓名
    char          szOrgid[4 + 1];        // 机构编码
    char          szBsflag[2 + 1];       // 买卖类别
    char          chMarket;              // 交易市场
    char          szStkname[8 + 1];      // 证券名称
    char          szStkcode[8 + 1];      // 证券代码
    char          szOrderprice[21 + 1];  // 委托价格
    LONGLONG      llOrderqty;            // 委托数量
    char          szOrderfrzamt[21 + 1]; // 委托金额
    LONGLONG      llMatchqty;            // 成交数量
    LONGLONG      llCancelqty;           // 撤单数量
    char          szMatchamt[21 + 1];    // 成交金额
    LONGLONG      llQty;                 // 可撤数量
};
//-------------------------------10103550:批量委托查询【增加】--------------------------
struct MATRADEAPI CReqFislQryRpcBatchOrderField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    int           iOrdergroup;       // 委托批号
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcBatchOrderField
{
    char          szPoststr[64 + 1];    // 定位串
    char          szOrgid[4 + 1];       // 机构代码
    int           iOrdergroup;          // 委托批号
    int           iOrderdate;           // 委托日期
    int           iOrdersno;            // 委托序号
    char          szOrderid[10 + 1];    // 合同序号
    LONGLONG      llFundid;             // 资金帐户
    char          chMarket;             // 市场代码
    char          szSeat[10 + 1];       // 席位
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[6 + 1];     // 证券代码
    char          szStkname[10 + 1];    // 证券名称
    char          szBankcode[4 + 1];    // 银行代码
    char          chMoneytype;          // 货币代码
    char          chOperway;            // 操作方式
    char          szBsflag[2 + 1];      // 买卖标识
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          chOrderstatus;        // 委托状态
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          szMatchprice[21 + 1]; // 成交界个
    int           iOperdate;            // 操作日期
    int           iOpertime;            // 操作时间
    int           iReporttime;          // 申报时间
    int           iOrdernum;            // 委托笔数
};
//-------------------------------10103551:信用资金可取查询--------------------------
struct MATRADEAPI CReqFislQryRpcCreditCuacctCanDrawField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspFislQryRpcCreditCuacctCanDrawField
{
    LONGLONG      llCustid;          // 客户代码
    LONGLONG      llFundid;          // 资金账户
    char          szOrgid[4 + 1];    // 机构编码
    char          chMoneytype;       // 货币
    char          szMaxdraw[21 + 1]; // 可取金额
};
//-------------------------------10103552:股票查询行情--------------------------
struct MATRADEAPI CReqFislQryRpcMktField
{
    char          chExchangeType;     // 交易市场
    char          szStockCode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcMktField
{
    char          szStockName[8 + 1];        // 证券名称
    char          szStockCode[6 + 1];        // 证券代码
    char          chExchangeIndex;           // 指数
    char          szLastPrice[21 + 1];       // 最新价
    int           iMatchAmount;              // 成交数量
    char          szOpenPrice[21 + 1];       // 今开盘价
    char          szClosePrice[21 + 1];      // 前收盘价
    char          szHighPrice[21 + 1];       // 最高价
    char          szLowPrice[21 + 1];        // 最低价
    char          szBusinessBalance[21 + 1]; // 今成交金额
    int           iBusinessAmount;           // 成交数量
    char          szBuyPrice1[21 + 1];       // 申买价一
    char          szBuyPrice2[21 + 1];       // 申买价二
    char          szBuyPrice3[21 + 1];       // 申买价三
    char          szBuyPrice4[21 + 1];       // 申买价四
    char          szBuyPrice5[21 + 1];       // 申买价五
    char          szSalePrice1[21 + 1];      // 申卖价一
    char          szSalePrice2[21 + 1];      // 申卖价二
    char          szSalePrice3[21 + 1];      // 申卖价三
    char          szSalePrice4[21 + 1];      // 申卖价四
    char          szSalePrice5[21 + 1];      // 申卖价五
    int           iBuyAmount1;               // 申买量一
    int           iBuyAmount2;               // 申买量二
    int           iBuyAmount3;               // 申买量三
    int           iBuyAmount4;               // 申买量四
    int           iBuyAmount5;               // 申买量五
    int           iSaleAmount1;              // 申卖量一
};
//-------------------------------10103695:意向行情查询--------------------------
struct MATRADEAPI CReqFislQryRpcMktIntentionField
{
    char          chMarket;          // 市场
    char          szStkcode[6 + 1];  // 证券代码
    char          szBsflag[128 + 1]; // 业务类别
    int           iPromiseno;        // 约定号
    int           iQryrec;           // 查询记录数
    int           iPosstr;           // 定位串
};
struct MATRADEAPI CRspFislQryRpcMktIntentionField
{
    char          chMarket;              // 交易所代码
    int           iJysorderid;           // 意向申报序列号
    char          szStkcode[6 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
    char          szOrderqty[21 + 1];    // 意向申报数量
    char          szOrderprice[21 + 1];  // 意向申报价格
    char          szBsflag[2 + 1];       // 买卖类别
    char          szSeat[6 + 1];         // 席位
    char          szSecuid[10 + 1];      // 用户代码
    char          szSecuname[16 + 1];    // 用户名称
    char          szContactinfo[32 + 1]; // 联系方式
    int           iReporttime;           // 申报时间
    int           iPromiseno;            // 约定号
    int           iUpdtime;              // 行情刷新时间
    int           iPosstr;               // 定位串
};
//-------------------------------10103696:查询营业部某一业务类别的申报席位--------------------------
struct MATRADEAPI CReqFislQryRpcSeatField
{
    char          chMarket;         // 交易所代码
    char          szOrgid[4 + 1];   // 营 业 部
    char          szBsflag[2 + 1];  // 业务类别
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspFislQryRpcSeatField
{
    char          szSeat[6 + 1]; // 申报席位
};
//-------------------------------10103697:查询资金账户信息--------------------------
struct MATRADEAPI CReqFislQryRpcCuacctAcctInfoField
{
    LONGLONG      llFundid;         // 资金账户
};
struct MATRADEAPI CRspFislQryRpcCuacctAcctInfoField
{
    char          szFundname[16 + 1];        // 资金姓名
    char          chIdtype;                  // 证件类型
    char          szIdno[32 + 1];            // 证件号码
    char          szOperway[128 + 1];        // 操作方式
    char          szFundagentright[128 + 1]; // 资金代理权限
    char          szFundright[128 + 1];      // 资金权限
    char          chStatus;                  // 资金状态
};
//-------------------------------10103553:客户特殊手续费查询--------------------------
struct MATRADEAPI CReqFislQryRpcCustSpecialFeeField
{
    LONGLONG      llFundid;             // 资产账户
    char          szMarket[32 + 1];     // 市场代码
    char          szStktype[128 + 1];   // 证券类别
    char          szTrdid[128 + 1];     // 交易类型
    char          szMoneytype[128 + 1]; // 货币类型
    char          szBsflag[128 + 1];    // 买卖类别
};
struct MATRADEAPI CRspFislQryRpcCustSpecialFeeField
{
    int           iServerid;                // 机器编码
    char          szOrgid[4 + 1];           // 机构编码
    LONGLONG      llFundid;                 // 资金账号
    LONGLONG      llCustid;                 // 客户代码
    char          szCustname[32 + 1];       // 客户姓名
    char          chMarket;                 // 市场代码
    char          szBsflag[2 + 1];          // 买卖方向
    char          chPtOperway;              // 操作方式
    char          chMoneytype;              // 货币代码
    char          chStktype;                // 证券类别
    char          szTrdid[2 + 1];           // 交易类型
    char          szMinfee[21 + 1];         // 最低收费
    char          szMaxfee[21 + 1];         // 最高收额
    char          szCommissionrate[21 + 1]; // 比例
};
//-------------------------------10103554:股息红利税扣收明细查询--------------------------
struct MATRADEAPI CReqFislQryRpcDividendBonusTaxDetailField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          szStatus[128 + 1]; // 扣税状态
    int           iBegindate;        // 开始日期
    int           iEnddate;          // 结束日期
};
struct MATRADEAPI CRspFislQryRpcDividendBonusTaxDetailField
{
    int           iServerid;            // 机器编码
    char          chMarket;             // 交易市场
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 股东姓名
    int           iSenddate;            // 发送日期
    int           iNoticedate;          // 通知日期
    char          szSno[10 + 1];        // 计税序号
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[8 + 1];     // 证券代码
    int           iTaxdate;             // 计税日期
    int           iRegistdate;          // 权益登记日期
    int           iReducedate;          // 减持交割日期
    char          chStatus;             // 扣税状态
    char          szBusitype[2 + 1];    // 业务类别
    char          chMoneytype;          // 货币代码
    char          szTaxamt[21 + 1];     // 应扣税额
    char          szPaidtaxamt[21 + 1]; // 已扣税额
    LONGLONG      llTaxcount;           // 净减持股数
    char          szBfjaccount[10 + 1]; // 备付金账号
    char          szSeat[6 + 1];        // 席位代码
    char          szClearseat[5 + 1];   // 结算席位
    char          szClearsno[8 + 1];    // 清算编号
    int           iHolddate;            // 初始持有日期
    char          szPrice[21 + 1];      // 价格
    char          szTaxrate[21 + 1];    // 补税税率
    char          szAssistcode[10 + 1]; // 辅助代码
    char          szResultcode[4 + 1];  // 结果代码
    char          szRemarks[64 + 1];    // 备注
};
//-------------------------------10103555:设定定期转帐转出计划--------------------------
struct MATRADEAPI CReqFislRpcSetPeriodicalTransferPlanField
{
    LONGLONG      llFundid;             // 资金帐号
    char          szBankcode[4 + 1];    // 银行代码
    char          chMoneytype;          // 货币代码
    char          szFundeffect[21 + 1]; // 转账金额
    char          chPlantype;           // 定期类型
    int           iActiondate;          // 执行日
    char          szWflag[8 + 1];       // 通用标志
    char          szRemark[64 + 1];     // 备注
};
struct MATRADEAPI CRspFislRpcSetPeriodicalTransferPlanField
{
    int           iSno; // 操作流水号
};
//-------------------------------10103556:查询定期转帐转出计划--------------------------
struct MATRADEAPI CReqFislQryRpcPeriodicalTransferPlanField
{
    LONGLONG      llFundid;             // 资金帐号
    char          szBankcode[4 + 1];    // 银行代码
    char          chMoneytype;          // 货币代码
    char          szFundeffect[21 + 1]; // 转账金额
    int           iCount;               // 查询行数
    char          szPoststr[32 + 1];    // 定位串
};
struct MATRADEAPI CRspFislQryRpcPeriodicalTransferPlanField
{
    char          szPoststr[12 + 1];    // 序号
    int           iSno;                 // 计划编号
    char          szBankcode[4 + 1];    // 银行代码
    char          szBankname[32 + 1];   // 银行名称
    LONGLONG      llFundid;             // 资金帐号
    char          chMoneytype;          // 货币代码
    char          szFundeffect[21 + 1]; // 待转资金
    char          chPlantype;           // 计划类型
    int           iActiondate;          // 执行日
    char          szRemark[32 + 1];     // 备注
    char          chActionret;          // 执行日执行结果
    char          szOperdate[8 + 1];    // 操作日
    int           iLinksno;             // 相关转帐流水号
    char          szWflag[8 + 1];       // 通用标志
};
//-------------------------------10103557:删除定期转帐转出计划--------------------------
struct MATRADEAPI CReqFislRpcDelPeriodicalTransferPlanField
{
    LONGLONG      llFundid;         // 资金帐号
    int           iSno;             // 计划编号
};
struct MATRADEAPI CRspFislRpcDelPeriodicalTransferPlanField
{
    int           iSno; // 操作流水号
};
//-------------------------------10103558:担保证券查询--------------------------
struct MATRADEAPI CReqFislQryRpcGuaranteedStkField
{
    char          chMarket;          // 交易市场
    char          chQryflag;         // 查询方向
    char          szPoststr[32 + 1]; // 定位串
    int           iCount;            // 请求行数
};
struct MATRADEAPI CRspFislQryRpcGuaranteedStkField
{
    char          szPoststr[32 + 1];    // 定位串
    char          chMarket;             // 交易市场
    char          szStkcode[6 + 1];     // 证券代码
    char          szStkname[10 + 1];    // 证券名称
    char          szPledgerate[21 + 1]; // 折算率
};
//-------------------------------10103559:标的证券查询--------------------------
struct MATRADEAPI CReqFislQryRpcUnderlyingStkField
{
    char          chMarket;          // 交易市场
    char          chQryflag;         // 查询方向
    char          szPoststr[32 + 1]; // 定位串
    int           iCount;            // 请求行数
};
struct MATRADEAPI CRspFislQryRpcUnderlyingStkField
{
    char          szPoststr[32 + 1];        // 定位串
    char          chMarket;                 // 交易市场
    char          szStkcode[6 + 1];         // 证券代码
    char          szStkname[10 + 1];        // 证券名称
    char          chCreditfundctrl;         // 融资允许
    char          chCreditstkctrl;          // 融券允许
    char          szMarginratefund[21 + 1]; // 融资保证金比例
    char          szMarginratestk[21 + 1];  // 融券保证金比例
};
//-------------------------------10103560:融资信用数据查询--------------------------
struct MATRADEAPI CReqFislQryRpcFinancingCreditDataField
{
    LONGLONG      llFundid;         // 资金帐号
    char          chMoneytype;      // 币种
    char          chMarket;         // 交易市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcFinancingCreditDataField
{
    char          szMaxfamt[21 + 1];           // 最大融资金额
    char          szMarginavl[21 + 1];         // 可用保证金余额
    char          szFcreditbal[21 + 1];        // 融资授信额度
    char          szFcreditavl[21 + 1];        // 融资可用的额度
    char          szFmarginrate[21 + 1];       // 融资保证金比例
    char          szFunmatchamt[21 + 1];       // 融资未成交金额
    char          szFundavl[21 + 1];           // 资金可用数
    char          szFundavlfrz[21 + 1];        // 资金可用数冻结数
    char          szFmarketval[21 + 1];        // 担保证券的市值
    char          szGdismarketval[21 + 1];     // 担保证券的折算价值
    char          szFdebtbal[21 + 1];          // 融资总金额
    char          szTotaldisfdebtbal[21 + 1];  // ∑ 融资金额 * 折算率
    char          szTotalratefdebtbal[21 + 1]; // ∑ 融资金额 * 融资保证金比例
    char          szTotalffee[21 + 1];         // 融资的利息和费用的总和
    char          szTotaldisfmkval[21 + 1];    // ∑ 融资的市值 * 折算率
    char          chMarket;                    // 交易市场
    char          szStkcode[6 + 1];            // 证券代码
    char          szStkname[10 + 1];           // 证券名称
};
//-------------------------------10103561:融券信用数据查询--------------------------
struct MATRADEAPI CReqFislQryRpcMarginCreditDataField
{
    LONGLONG      llFundid;         // 资金帐号
    char          chMoneytype;      // 币种
    char          chMarket;         // 交易市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcMarginCreditDataField
{
    char          szMaxdamt[21 + 1];           // 最大融券金额
    char          szMarginavl[21 + 1];         // 可用保证金余额
    char          szDcreditbal[21 + 1];        // 融券授信额度
    char          szDcreditavl[21 + 1];        // 融券可用的额度
    char          chMarket;                    // 交易市场
    char          szStkcode[6 + 1];            // 证券代码
    char          szStkname[10 + 1];           // 证券名称
    char          szDmarginrate[21 + 1];       // 融券保证金比例
    char          szDunmatchamt[21 + 1];       // 融券未成交金额
    char          szFundavl[21 + 1];           // 资金可用数
    char          szFundavlfrz[21 + 1];        // 资金可用数冻结数
    char          szDsaleavl[21 + 1];          // 融券卖出金额可用数
    char          szDsaleavlfrz[21 + 1];       // 融券卖出金额可用数的冻结数
    char          szGmarketval[21 + 1];        // 担保证券的市值
    char          szGdismarketval[21 + 1];     // 担保证券的折算价值
    char          szDsalebal[21 + 1];          // 融券卖出资金
    char          szTotaldisdsalebal[21 + 1];  // ∑ 融券卖出资金 * 折算率
    char          szTotalratedsalebal[21 + 1]; // ∑ 融券卖出资金 * 融券保证金比例
    char          szTotaldfee[21 + 1];         // 融券的利息和费用的总和
    char          szTotaldmkval[21 + 1];       // 融券的市值
    char          szTotaldisdmkval[21 + 1];    // ∑ 融券的市值 * 折算率
    char          szTotalratdmkval[21 + 1];    // ∑ 融券的市值 * 融券保证金比例
};
//-------------------------------10103562:信用客户资产负债查询--------------------------
struct MATRADEAPI CReqFislQryRpcCreditCustBalanceField
{
    LONGLONG      llFundid;         // 资金帐号
    char          chMoneytype;      // 币种
};
struct MATRADEAPI CRspFislQryRpcCreditCustBalanceField
{
    char          chMoneytype;              // 币种
    LONGLONG      llFundid;                 // 资金帐号
    char          szFundintrrate[21 + 1];   // 融资利率
    char          szStkintrrate[21 + 1];    // 融券利率
    char          szPunishintrrate[21 + 1]; // 罚息利率
    char          chCreditstatus;           // 信用状态
    char          szMarginrates[21 + 1];    // 维持担保比例
    char          szRealrate[21 + 1];       // 实时担保比例
    char          szAsset[21 + 1];          // 总资产
    char          szLiability[21 + 1];      // 总负债
    char          szMarginavl[21 + 1];      // 保证金可用数
    char          szFundbal[21 + 1];        // 资金余额
    char          szFundavl[21 + 1];        // 资金可用数
    char          szDsaleamtbal[21 + 1];    // 融券卖出所得资金
    char          szGuaranteeout[21 + 1];   // 可转出担保资产
    char          szGagemktavl[21 + 1];     // 担保证券市值
    char          szFdealavl[21 + 1];       // 融资本金
    char          szFfee[21 + 1];           // 融资息费
    char          szFtotaldebts[21 + 1];    // 融资负债合计
    char          szDealfmktavl[21 + 1];    // 应付融券市值
    char          szDfee[21 + 1];           // 融券息费
    char          szDtotaldebts[21 + 1];    // 融券负债合计
    char          szFcreditbal[21 + 1];     // 融资授信额度
    char          szFcreditavl[21 + 1];     // 融资可用额度
    char          szFcreditfrz[21 + 1];     // 融资额度冻结
    char          szDcreditbal[21 + 1];     // 融券授信额度
    char          szDcreditavl[21 + 1];     // 融券可用额度
    char          szDcreditfrz[21 + 1];     // 融券额度冻结
    char          szRights[21 + 1];         // 红利权益
    char          szUncomerqrights[21 + 1]; // 红利权益(在途)
    int           iRightsqty;               // 红股权益
    int           iUncomerqrightsqty;       // 红股权益(在途)
    char          szAcreditbal[21 + 1];     // 总额度
    char          szAcreditavl[21 + 1];     // 总可用额度
};
//-------------------------------10103563:融资融券合约查询--------------------------
struct MATRADEAPI CReqFislQryRpcContractField
{
    LONGLONG      llFundid;               // 资金帐号
    char          chMoneytype;            // 币    种
    char          szCreditdirect[16 + 1]; // 融资融券方向
    char          szLifestatus[16 + 1];   // 状态
    char          szMarket[16 + 1];       // 交易市场
    char          szStkcode[8 + 1];       // 证券代码
    char          szBegindate[8 + 1];     // 开始日期
    char          szEnddate[8 + 1];       // 结束日期
    char          szPosstr[32 + 1];       // 定位串
    int           iCount;                 // 请求行数
    LONGLONG      llRfscontractno;        // 约定合约编号
    LONGLONG      llPositionid;           // 头寸编号
};
struct MATRADEAPI CRspFislQryRpcContractField
{
    char          szOrgid[4 + 1];             // 营业部
    LONGLONG      llFundid;                   // 资金帐号
    char          chMoneytype;                // 币    种
    char          chMarket;                   // 交易市场
    char          szStkcode[6 + 1];           // 证券代码
    char          szStkname[10 + 1];          // 证券名称
    int           iOrderdate;                 // 委托日期
    char          szOrdersno[10 + 1];         // 委 托 号
    char          chCreditdirect;             // 融资融券方向
    int           iOrderqty;                  // 委托数量
    int           iMatchqty;                  // 成交数量
    char          szOrderamt[21 + 1];         // 委托金额
    char          szOrderfrzamt[21 + 1];      // 委托冻结金额
    char          szMatchamt[21 + 1];         // 成交金额
    char          szClearamt[21 + 1];         // 清算金额
    char          chLifestatus;               // 合约状态
    int           iEnddate;                   // 负债截止日期
    int           iOldenddate;                // 原始的负债截止日期
    char          szCreditrepay[21 + 1];      // T日之前归还金额
    char          szCreditrepayunfrz[21 + 1]; // T日归还金额
    char          szFundremain[21 + 1];       // 应还金额
    int           iStkrepay;                  // T日之前归还数量
    int           iStkrepayunfrz;             // T日归还数量
    int           iStkremain;                 // 应还证券数量
    char          szStkremainvalue[21 + 1];   // 应还证券市值
    char          szFee[21 + 1];              // 融资融券息、费
    char          szOverduefee[21 + 1];       // 逾期未偿还息、费
    char          szFeeRepay[21 + 1];         // 己偿还息、费
    char          szPunifee[21 + 1];          // 利息产生的罚息
    char          szPunifeeRepay[21 + 1];     // 己偿还罚息
    char          szRights[21 + 1];           // 未偿还权益金额
    char          szOverduerights[21 + 1];    // 逾期未偿还权益
    char          szRightsRepay[21 + 1];      // 己偿还权益
    char          szLastprice[21 + 1];        // 最新价
    char          szProfitcost[21 + 1];       // 浮动盈亏
    char          szPoststr[32 + 1];          // 定位串
    int           iSysdate;                   // 系统日期
    int           iSno;                       // 合约编号
    int           iLastdate;                  // 最后一次计算息费日期
    int           iClosedate;                 // 合约全部偿还日期
    char          szPunidebts[21 + 1];        // 逾期本金罚息
    char          szPunidebtsRepay[21 + 1];   // 本金罚息偿还
    char          szPunidebtsunfrz[21 + 1];   // 逾期本金罚息
    char          szPunifeeunfrz[21 + 1];     // 逾期息费罚息
    char          szPunirights[21 + 1];       // 逾期权益罚息
    char          szPunirightsRepay[21 + 1];  // 权益罚息偿还
    char          szPunirightsunfrz[21 + 1];  // 逾期权益罚息
    char          szFeeunfrz[21 + 1];         // 实时偿还利息
    char          szOverduefeeunfrz[21 + 1];  // 实时偿还逾期利息
    int           iRightsqty;                 // 未偿还权益数量
    int           iOverduerightsqty;          // 逾期未偿还权益数量
    char          szSecuid[10 + 1];           // 证券账号
    char          chDebtsflag;                // 合约生成方式
    LONGLONG      llPositionid;               // 头寸编号
    int           iDefertimes;                // 展期次数
};
//-------------------------------10103564:合同信息查询--------------------------
struct MATRADEAPI CReqFislQryRpcAgreementInfoField
{
    LONGLONG      llFundid;         // 资金帐号
    char          chMoneytype;      // 币种
};
struct MATRADEAPI CRspFislQryRpcAgreementInfoField
{
    LONGLONG      llFundid;                 // 资金帐号
    char          chMoneytype;              // 币种
    int           iContractno;              // 合同编号
    int           iContractsno;             // 合同顺序号
    int           iContractlimit;           // 合同期限
    char          chAutodefer;              // 合同自动延期标志
    int           iBegindate;               // 合同起始日期
    int           iEnddate;                 // 合同结束日期
    char          chContractstatus;         // 合同状态
    char          szCreditbal[21 + 1];      // 信用额度
    char          szFcreditbal[21 + 1];     // 融资额度
    char          szFcreditrate[21 + 1];    // 融资年利率
    char          szFyearmgfee[21 + 1];     // 融资管理费率
    char          szFyearsrfee[21 + 1];     // 融资承诺费率
    char          szFprate[21 + 1];         // 融资年罚息利率
    char          szDcreditbal[21 + 1];     // 融券额度
    char          szDcreditrate[21 + 1];    // 融券年利率
    char          szDyearmgfee[21 + 1];     // 融券管理费率
    char          szDyearsrfee[21 + 1];     // 融券承诺费率
    char          szDprate[21 + 1];         // 融券年罚息利率
    char          szBaseloanrate[21 + 1];   // 央行基准贷款利率
    char          szBasepunishrate[21 + 1]; // 央行基准罚息率
    char          szFundintrrate[21 + 1];   // 融资利率(不含央行基准率)
    char          szStkintrrate[21 + 1];    // 融券利率(不含央行基准率)
};
//-------------------------------10103565:外围约定直接还款、直接还款--------------------------
struct MATRADEAPI CReqFislRpcDirectRepaymentField
{
    LONGLONG      llFundid;             // 信用资金帐号
    char          chMoneytype;          // 货币代码
    char          szFundeffect[21 + 1]; // 还款金额
    char          szRemark[32 + 1];     // 备注信息
    char          chCreditmethod;       // 约定业务分类
    LONGLONG      llRfscontractno;      // 约定合约编号
    int           iDebtdate;            // 负债合约日期
    int           iDebtsno;             // 负债合约编号
    char          szDebttype[128 + 1];  // 负债类型
};
struct MATRADEAPI CRspFislRpcDirectRepaymentField
{
    char          szFundeffect[21 + 1]; // 实际还款金额
};
//-------------------------------10103566:查询券商融券帐户头寸--------------------------
struct MATRADEAPI CReqFislQryRpcMarginAcctPosiField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcMarginAcctPosiField
{
    char          chMarket;         // 市    场
    char          szStkcode[8 + 1]; // 证券代码
    char          szStkname[8 + 1]; // 证券名称
    LONGLONG      llStkbal;         // 证券余额
    LONGLONG      llStkavl;         // 证券可用
};
//-------------------------------10103567:信用客户可转入担保证券查询--------------------------
struct MATRADEAPI CReqFislQryRpcCreditCustCanTransferInGuaranteedStkField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcCreditCustCanTransferInGuaranteedStkField
{
    char          szPoststr[32 + 1];    // 定位串
    LONGLONG      llCustid;             // 客户代码
    LONGLONG      llFundid;             // 资金账户
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkname[8 + 1];     // 证券名称
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrgid[4 + 1];       // 机构编码
    char          chMoneytype;          // 货币
    int           iStkbal;              // 股份余额
    int           iStkavl;              // 股份可用
    int           iStkremain;           // 客户余券
    char          szBuycost[21 + 1];    // 当前成本
    char          szCostprice[21 + 1];  // 成本价格
    char          szMktval[21 + 1];     // 市值
    char          szIncome[21 + 1];     // 盈亏
    char          chMtkcalflag;         // 市值计算标识
    int           iStkqty;              // 当前拥股数
    char          szLastprice[21 + 1];  // 最新价格
    char          chStktype;            // 证券类型
    char          chIspledge;           // 是否担保品
    char          szPledgerate[21 + 1]; // 担保品折算率
};
//-------------------------------10103568:投票方案查询--------------------------
struct MATRADEAPI CReqFislQryRpcVotingSchemeField
{
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szProjectid[21 + 1];  // 方案号
    char          szMeetingseq[12 + 1]; // 大会代码
};
struct MATRADEAPI CRspFislQryRpcVotingSchemeField
{
    char          chMarket;                // 交易市场
    char          szStkcode[8 + 1];        // 证券代码
    char          szStkname[8 + 1];        // 证券名称
    char          szProjectid[21 + 1];     // 方案号码
    char          szProjectname[128 + 1];  // 方案名称
    int           iBegindate;              // 投票起始日期
    int           iEnddate;                // 投票结束日期
    int           iRegdate;                // 股权登记日
    char          chProjectkind;           // 方案类型
    char          szRemark[64 + 1];        // 备注信息
    char          szMeetingseq[12 + 1];    // 大会代码
    char          szMeetingdesc[1024 + 1]; // 大会名称
};
//-------------------------------10103569:投票下单--------------------------
struct MATRADEAPI CReqFislRpcVotingOrderField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 证券帐号
    char          szOrgid[4 + 1];       // 营业部代码
    LONGLONG      llFundid;             // 资金账户
    LONGLONG      llAgentid;            // 代理人代码
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrderprice[21 + 1]; // 委托价格
    int           iOrderqty;            // 委托数量
    char          chHaverelation;       // 存在关系
    char          chProjectkind;        // 方案类型
    char          szMeetingseq[12 + 1]; // 大会代码
};
struct MATRADEAPI CRspFislRpcVotingOrderField
{
};
//-------------------------------10103570:投票结果查询--------------------------
struct MATRADEAPI CReqFislQryRpcVotingResultField
{
    char          szOrgid[4 + 1];       // 营业部代码
    LONGLONG      llFundid;             // 资金账号
    char          chMarket;             // 市场代码
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrderprice[21 + 1]; // 投票议案
    char          szMeetingseq[12 + 1]; // 大会代码
};
struct MATRADEAPI CRspFislQryRpcVotingResultField
{
    int           iSysdate;             // 交易日期
    int           iOperdate;            // 发生日期
    int           iOpertime;            // 发生时间
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 分支机构
    char          szBrhid[4 + 1];       // 机构分支
    LONGLONG      llFundid;             // 资金帐号
    char          chMoneytype;          // 货币代码
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    char          szLinkstk[8 + 1];     // 关联证券代码
    char          szLinkname[8 + 1];    // 关联证券名称
    char          szOrderprice[21 + 1]; // 委托价格
    int           iOrderqty;            // 委托数量
    LONGLONG      llStkbal;             // 股份余额
    char          chHaverelation;       // 存在关系
    char          chProjectkind;        // 方案类型
    char          szMeetingseq[12 + 1]; // 大会代码
};
//-------------------------------10103578:投票结果统计--------------------------
struct MATRADEAPI CReqFislRpcVotingResultStatisticsField
{
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 投票代码
    char          szOrderprice[21 + 1]; // 投票议案
    int           iBegindate;           // 开始时间
    int           iEnddate;             // 结束时间
    char          chHaverelation;       // 存在关系
    char          szMeetingseq[12 + 1]; // 大会代码
};
struct MATRADEAPI CRspFislRpcVotingResultStatisticsField
{
    int           iServerid;            // 节点编号
    char          chMarket;             // 市场代码
    char          szStkcode[8 + 1];     // 投票代码
    char          szOrderprice[21 + 1]; // 投票议案
    char          chProjectkind;        // 议案类型
    LONGLONG      llOrderqty;           // 看备注信息
    LONGLONG      llThisviewstkbal;     // 看备注信息
    int           iThisviewcustqty;     // 看备注信息
    int           iVotecustqty;         // 参与投票客户数
    int           iAllcustqty;          // 持有正股客户数
    LONGLONG      llAllorderqty;        // 看备注信息
    char          szRate[21 + 1];       // 看备注信息
    char          szMeetingseq[12 + 1]; // 大会代码
};
//-------------------------------10103579:客户可投票议案查询--------------------------
struct MATRADEAPI CReqFislQryRpcCanVoteBillField
{
    char          szOrgid[4 + 1];   // 营业部代码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 市场代码
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcCanVoteBillField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[8 + 1];     // 证券代码
    char          szVotecode[8 + 1];    // 投票代码
    char          szStkname[8 + 1];     // 证券名称
    char          szProjectid[21 + 1];  // 方案号码
    char          chProjectkind;        // 方案类型
    LONGLONG      llStkavl;             // 股份可用数
    LONGLONG      llStkbal;             // 股份余额
    LONGLONG      llHaveorderqty;       // 已投票数量
    char          szMeetingseq[12 + 1]; // 大会代码
};
//-------------------------------10103580:客户可投票指定议案的股份查询--------------------------
struct MATRADEAPI CReqFislQryRpcCanVoteBillShareField
{
    char          szOrgid[4 + 1];       // 营业部代码
    LONGLONG      llFundid;             // 资金账号
    char          chMarket;             // 市场代码
    char          szSecuid[10 + 1];     // 证券账号
    char          szStkcode[8 + 1];     // 证券代码
    char          szProjectid[21 + 1];  // 投票议案
    char          szMeetingseq[12 + 1]; // 大会代码
};
struct MATRADEAPI CRspFislQryRpcCanVoteBillShareField
{
    LONGLONG      llVotablestk; // 可投票数量
};
//-------------------------------10103581:股票历史股份余额查询--------------------------
struct MATRADEAPI CReqFislQryRpcHisShareBalanceField
{
    char          szOrgid[4 + 1];   // 营业部代码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 市场代码
    char          szSecuid[10 + 1]; // 证券账号
    char          szStkcode[8 + 1]; // 证券代码
    char          chDatekind;       // 查询日期类型
    int           iBizdate;         // 查询日期
};
struct MATRADEAPI CRspFislQryRpcHisShareBalanceField
{
    int           iHisstkbal; // 历史股份余额
};
//-------------------------------10103582:融资融券帐号自助激活--------------------------
struct MATRADEAPI CReqFislRpcSelfActivateField
{
    LONGLONG      llFundid;         // 资金账号
};
struct MATRADEAPI CRspFislRpcSelfActivateField
{
    char          chActiveflag;   // 账户激活标志
    char          chCreditstatus; // 账户当前状态
};
//-------------------------------10103583:合同信息修改--------------------------
struct MATRADEAPI CReqFislRpcChgAgreementInfoField
{
    LONGLONG      llFundid;         // 资金账号
    int           iContractsno;     // 合同顺序号
    char          chAutodefer;      // 合同自动延期标志
};
struct MATRADEAPI CRspFislRpcChgAgreementInfoField
{
};
//-------------------------------10103584:客户账户级费用查询--------------------------
struct MATRADEAPI CReqFislQryRpcAcctLvlFeeField
{
    LONGLONG      llFundid;         // 资金账号
};
struct MATRADEAPI CRspFislQryRpcAcctLvlFeeField
{
    LONGLONG      llFundid;                         // 资金账号
    char          szOrgid[4 + 1];                   // 机构编码
    char          chMoneytype;                      // 货币代码
    LONGLONG      llCustid;                         // 客户代码
    char          szFundprofee[21 + 1];             // 融资承诺费
    char          szStkprofee[21 + 1];              // 融券承诺费
    char          szFundmanagefee[21 + 1];          // 融资管理费
    char          szFundmanageoverdue[21 + 1];      // 融资逾期管理费
    char          szFundmanagepunintr[21 + 1];      // 融资逾期管理费罚息
    char          szStkmanagefee[21 + 1];           // 融券管理费
    char          szStkmanageoverdue[21 + 1];       // 融券逾期管理费
    char          szStkmanagepunintr[21 + 1];       // 融券逾期管理费罚息
    char          szFundprofeeunfrz[21 + 1];        // 当日偿还融资承诺费
    char          szStkprofeeunfrz[21 + 1];         // 当日偿还融券承诺费
    char          szFundmanagefeeunfrz[21 + 1];     // 当日偿还融资管理费
    char          szFundmanageoverdueunfrz[21 + 1]; // 当日偿还融资逾期管理费
    char          szFundmanagepunintrunfrz[21 + 1]; // 当日偿还的融资逾期管理费罚息
    char          szStkmanagefeeunfrz[21 + 1];      // 当日偿还融券管理费
    char          szStkmanageoverdueunfrz[21 + 1];  // 当日偿还融券逾期管理费
    char          szStkmanagepunintrunfrz[21 + 1];  // 当日偿还融券逾期管理费罚息
    char          szSpacctfee[21 + 1];              // 融券特殊专户占用费
    char          szSpacctfeeunfrz[21 + 1];         // 当日偿还融券特殊专户占用费
    char          szFundposifullfee[21 + 1];        // 融资头寸全额占用费
    char          szFundposifullfeeunfrz[21 + 1];   // 融资头寸全额占用费偿还
    char          szFundposiunusefee[21 + 1];       // 融资头寸空闲费
    char          szFundposiunusefeeunfrz[21 + 1];  // 融资头寸空闲费偿还
    char          szFundposiusefee[21 + 1];         // 融资头寸使用费
    char          szFundposiusefeeunfrz[21 + 1];    // 融资头寸使用费偿还
    char          szStkposifullfee[21 + 1];         // 融券头寸全额占用费
    char          szStkposifullfeeunfrz[21 + 1];    // 融券头寸全额占用费偿还
    char          szStkposiunusefee[21 + 1];        // 融券头寸空闲费
    char          szStkposiunusefeeunfrz[21 + 1];   // 融券头寸空闲费偿还
    char          szStkposiusefee[21 + 1];          // 融券头寸使用费
    char          szStkposiusefeeunfrz[21 + 1];     // 融券头寸使用费偿还
    int           iLastdate;                        // 最后计息日期
};
//-------------------------------10103585:融资融券自定义合约偿还顺序查询--------------------------
struct MATRADEAPI CReqFislQryRpcCustomContractRepayOrderField
{
    LONGLONG      llFundid;                // 资金账号
    char          szOrgid[4 + 1];          // 营业部代码
    LONGLONG      llCustid;                // 客户代码
    char          chMarket;                // 市场
    char          szStkcode[8 + 1];        // 证券代码
    char          szCreditdirect[128 + 1]; // 合约类型
    char          szLifestatus[128 + 1];   // 合约状态
    int           iSysdate;                // 负债日期
    int           iSno;                    // 负债合约编号
    int           iCount;                  // 查询行数
    char          szPoststr[128 + 1];      // 定位串
};
struct MATRADEAPI CRspFislQryRpcCustomContractRepayOrderField
{
    int           iPrior;                     // 偿还优先级
    int           iOperdate;                  // 优先级设置日期
    char          szOrgid[4 + 1];             // 营业部
    LONGLONG      llFundid;                   // 资金帐号
    char          chMoneytype;                // 币    种
    char          chMarket;                   // 交易市场
    char          szStkcode[6 + 1];           // 证券代码
    char          szStkname[10 + 1];          // 证券名称
    int           iOrderdate;                 // 委托日期
    char          szOrdersno[10 + 1];         // 委 托 号
    char          chCreditdirect;             // 融资融券方向
    int           iOrderqty;                  // 委托数量
    int           iMatchqty;                  // 成交数量
    char          szOrderamt[21 + 1];         // 委托金额
    char          szOrderfrzamt[21 + 1];      // 委托冻结金额
    char          szMatchamt[21 + 1];         // 成交金额
    char          szClearamt[21 + 1];         // 清算金额
    char          chLifestatus;               // 合约状态
    int           iEnddate;                   // 负债截止日期
    int           iOldenddate;                // 原始的负债截止日期
    char          szCreditrepay[21 + 1];      // T日之前归还金额
    char          szCreditrepayunfrz[21 + 1]; // T日归还金额
    int           iStkrepay;                  // T日之前归还数量
    int           iStkrepayunfrz;             // T日归还数量
};
//-------------------------------10103586:融资融券自定义合约偿还顺序设置--------------------------
struct MATRADEAPI CReqFislRpcSetCustomContractRepayOrderField
{
    LONGLONG      llFundid;         // 资金账号
    char          szOrgid[4 + 1];   // 营业部代码
    LONGLONG      llCustid;         // 客户代码
    int           iSysdate;         // 负债日期
    int           iSno;             // 负债合约编号
    int           iPrior;           // 偿还优先级
};
struct MATRADEAPI CRspFislRpcSetCustomContractRepayOrderField
{
    LONGLONG      llFundid;       // 资金账号
    char          szOrgid[4 + 1]; // 营业部代码
    LONGLONG      llCustid;       // 客户代码
    int           iDebtdate;      // 负债日期
    int           iDebtsno;       // 负债合约编号
    int           iPrior;         // 偿还优先级
    int           iOperdate;      // 操作日期
};
//-------------------------------10103587:融券特殊专户证券查询--------------------------
struct MATRADEAPI CReqFislQryRpcSpecialMarginAcctStkField
{
    char          chMarket;         // 市场代码
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspFislQryRpcSpecialMarginAcctStkField
{
    LONGLONG      llCustid;         // 客户代码
    LONGLONG      llSpcustid;       // 专户客户代码
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 市场代码
    char          szStkcode[6 + 1]; // 证券代码
    char          szStkname[8 + 1]; // 证券名称
    LONGLONG      llStkbal;         // 股份余额
    LONGLONG      llStklastbal;     // 股份上次余额
    LONGLONG      llStkavl;         // 股份可用
};
//-------------------------------10103588:融资融券历史已了结合约查询--------------------------
struct MATRADEAPI CReqFislQryRpcHisClosedContractField
{
    LONGLONG      llFundid;               // 资金帐号
    char          chMoneytype;            // 币    种
    char          szCreditdirect[16 + 1]; // 融资融券方向
    char          szMarket[16 + 1];       // 交易市场
    char          szStkcode[8 + 1];       // 证券代码
    char          szBegindate[8 + 1];     // 开始日期
    char          szEnddate[8 + 1];       // 结束日期
    char          szPoststr[32 + 1];      // 定位串
    int           iCount;                 // 请求行数
};
struct MATRADEAPI CRspFislQryRpcHisClosedContractField
{
    char          szOrgid[4 + 1];             // 营业部
    LONGLONG      llFundid;                   // 资金帐号
    char          chMoneytype;                // 币    种
    char          chMarket;                   // 交易市场
    char          szStkcode[6 + 1];           // 证券代码
    char          szStkname[10 + 1];          // 证券名称
    int           iOrderdate;                 // 委托日期
    char          szOrdersno[10 + 1];         // 委 托 号
    char          chCreditdirect;             // 融资融券方向
    int           iOrderqty;                  // 委托数量
    int           iMatchqty;                  // 成交数量
    char          szOrderamt[21 + 1];         // 委托金额
    char          szOrderfrzamt[21 + 1];      // 委托冻结金额
    char          szMatchamt[21 + 1];         // 成交金额
    char          szClearamt[21 + 1];         // 清算金额
    char          chLifestatus;               // 合约状态
    int           iEnddate;                   // 负债截止日期
    int           iOldenddate;                // 原始的负债截止日期
    char          szCreditrepay[21 + 1];      // T日之前归还金额
    char          szCreditrepayunfrz[21 + 1]; // T日归还金额
    int           iStkrepay;                  // T日之前归还数量
    int           iStkrepayunfrz;             // T日归还数量
    char          szFee[21 + 1];              // 融资融券息、费
    char          szOverduefee[21 + 1];       // 逾期未偿还息、费
    char          szFeeRepay[21 + 1];         // 己偿还息、费
    char          szPunifee[21 + 1];          // 利息产生的罚息
    char          szRights[21 + 1];           // 未偿还权益金额
    char          szOverduerights[21 + 1];    // 逾期未偿还权益
    char          szRightsRepay[21 + 1];      // 己偿还权益
    char          szLastprice[21 + 1];        // 最新价
    char          szPoststr[32 + 1];          // 定位串
    int           iSysdate;                   // 系统日期
    int           iSno;                       // 合约编号
    int           iLastdate;                  // 最后一次计算息费日期
    int           iClosedate;                 // 合约全部偿还日期
    char          szPunifeeRepay[21 + 1];     // 己偿还罚息
    char          szPunidebts[21 + 1];        // 逾期本金罚息
    char          szPunidebtsRepay[21 + 1];   // 本金罚息偿还
    char          szPunidebtsunfrz[21 + 1];   // 逾期本金罚息
    char          szPunifeeunfrz[21 + 1];     // 逾期息费罚息
    char          szPunirights[21 + 1];       // 逾期权益罚息
    char          szPunirightsRepay[21 + 1];  // 权益罚息偿还
    char          szPunirightsunfrz[21 + 1];  // 逾期权益罚息
    char          szFeeunfrz[21 + 1];         // 实时偿还利息
    char          szOverduefeeunfrz[21 + 1];  // 实时偿还逾期利息
    int           iRightsqty;                 // 未偿还权益数量
    int           iOverduerightsqty;          // 逾期未偿还权益数量
};
//-------------------------------10103589:外围信用交易属性查询--------------------------
struct MATRADEAPI CReqFislQryRpcCreditTradeAttributeField
{
    LONGLONG      llFundid;         // 信用资金账户
};
struct MATRADEAPI CRspFislQryRpcCreditTradeAttributeField
{
    char          chTradeflag;      // 信用交易属性
    char          chLimitflag;      // 额度释放标志
    LONGLONG      llPositionidFund; // 融资开仓头寸
    LONGLONG      llPositionidStk;  // 融券开仓头寸
    char          chCrdtlevel;      // 客户信用等级
};
//-------------------------------10103590:合约展期申请--------------------------
struct MATRADEAPI CReqFislRpcContractExtensionApplicationField
{
    int           iDebtsno;          // 负债流水号
    int           iDebtdate;         // 负债日期
    int           iDeferdays;        // 申请展期天数
    char          szRemark[255 + 1]; // 备注
};
struct MATRADEAPI CRspFislRpcContractExtensionApplicationField
{
    int           iSno;       // 申请流水号
    int           iApplydate; // 申请日期
};
//-------------------------------10103591:客户等比额度释放标志修改--------------------------
struct MATRADEAPI CReqFislRpcChgGeometricQuotaReleaseFlagField
{
    LONGLONG      llFundid;         // 资金账号
    char          chLimitflag;      // 额度释放标志
};
struct MATRADEAPI CRspFislRpcChgGeometricQuotaReleaseFlagField
{
    int           iServerid;          // 机器编码
    char          szOrgid[4 + 1];     // 机构编码
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    LONGLONG      llFundid;           // 资金账号
    char          chLimitflag;        // 额度释放标注
};
//-------------------------------10103592:合约展期申请查询--------------------------
struct MATRADEAPI CReqFislQryRpcContractExtensionApplicationField
{
    LONGLONG      llFundid;         // 资金账号
    int           iSno;             // 申请流水号
    int           iStartdate;       // 申请开始日期
    int           iEnddate;         // 申请截止日期
    char          chStatus;         // 申请状态
    int           iDebtdate;        // 负债日期
    int           iDebtsno;         // 负债编号
};
struct MATRADEAPI CRspFislQryRpcContractExtensionApplicationField
{
    int           iSno;                 // 申请流水号
    char          szOrgid[4 + 1];       // 机构编号
    LONGLONG      llFundid;             // 信用资金帐号
    int           iApplydate;           // 申请日期
    int           iDebtdate;            // 负债日期
    int           iDebtsno;             // 负债编号
    int           iDeferdays;           // 展期天数
    char          chStatus;             // 申请状态
    int           iApprovaldate;        // 审批日期
    char          szChkremark[255 + 1]; // 审批备注
    char          szRemark[255 + 1];    // 备注
};
//-------------------------------10103593:合约展期申请取消--------------------------
struct MATRADEAPI CReqFislRpcContractExtensionApplicationCancelField
{
    LONGLONG      llFundid;         // 资金账号
    int           iSno;             // 申请流水号
    int           iApplydate;       // 申请日期
};
struct MATRADEAPI CRspFislRpcContractExtensionApplicationCancelField
{
};
//-------------------------------10103594:外围风险测评类型查询--------------------------
struct MATRADEAPI CReqFislQryRpcRiskEvaluationModeField
{
};
struct MATRADEAPI CRspFislQryRpcRiskEvaluationModeField
{
    int           iServerid;   // 服务器编号
    LONGLONG      llCustid;    // 客户代码
    char          chRiskkind;  // 风险类型
    int           iAssessdate; // 测评时间
};
//-------------------------------10103595:展期规则查询--------------------------
struct MATRADEAPI CReqFislQryRpcExtensionRuleField
{
    char          szStatus[128 + 1]; // 规则状态
};
struct MATRADEAPI CRspFislQryRpcExtensionRuleField
{
    int           iRuleid;            // 规则代码
    char          szRulename[64 + 1]; // 规则名称
    char          chStatus;           // 规则状态
    char          szChkvalue[64 + 1]; // 规则值
};
//-------------------------------10103596:可展期合约查询--------------------------
struct MATRADEAPI CReqFislQryRpcCanExtendContractField
{
    LONGLONG      llFundid;               // 资金帐号
    char          szCreditdirect[16 + 1]; // 融资融券方向
    char          szMarket[16 + 1];       // 交易市场
    char          szStkcode[8 + 1];       // 证券代码
    char          szBegindate[8 + 1];     // 开始日期
    char          szEnddate[8 + 1];       // 结束日期
    int           iSno;                   // 合约编号
    LONGLONG      llPositionid;           // 头寸编号
    char          szPosstr[32 + 1];       // 定位串
    int           iCount;                 // 请求行数
};
struct MATRADEAPI CRspFislQryRpcCanExtendContractField
{
    char          szOrgid[4 + 1];             // 营业部
    LONGLONG      llFundid;                   // 资金帐号
    char          chMoneytype;                // 币    种
    char          chMarket;                   // 交易市场
    char          szStkcode[6 + 1];           // 证券代码
    char          szStkname[10 + 1];          // 证券名称
    int           iOrderdate;                 // 委托日期
    char          szOrdersno[10 + 1];         // 委 托 号
    char          chCreditdirect;             // 融资融券方向
    int           iOrderqty;                  // 委托数量
    int           iMatchqty;                  // 成交数量
    char          szOrderamt[21 + 1];         // 委托金额
    char          szOrderfrzamt[21 + 1];      // 委托冻结金额
    char          szMatchamt[21 + 1];         // 成交金额
    char          szClearamt[21 + 1];         // 清算金额
    char          chLifestatus;               // 合约状态
    int           iEnddate;                   // 负债截止日期
    int           iOldenddate;                // 原始的负债截止日期
    char          szCreditrepay[21 + 1];      // T日之前归还金额
    char          szCreditrepayunfrz[21 + 1]; // T日归还金额
    char          szFundremain[21 + 1];       // 应还金额
    int           iStkrepay;                  // T日之前归还数量
    int           iStkrepayunfrz;             // T日归还数量
    int           iStkremain;                 // 应还证券数量
    char          szStkremainvalue[21 + 1];   // 应还证券市值
    char          szFee[21 + 1];              // 融资融券息、费
    char          szOverduefee[21 + 1];       // 逾期未偿还息、费
    char          szFeeRepay[21 + 1];         // 己偿还息、费
    char          szPunifee[21 + 1];          // 利息产生的罚息
    char          szPunifeeRepay[21 + 1];     // 己偿还罚息
    char          szRights[21 + 1];           // 未偿还权益金额
    char          szOverduerights[21 + 1];    // 逾期未偿还权益
    char          szRightsRepay[21 + 1];      // 己偿还权益
    char          szLastprice[21 + 1];        // 最新价
    char          szProfitcost[21 + 1];       // 浮动盈亏
    char          szPoststr[32 + 1];          // 定位串
    int           iSysdate;                   // 系统日期
    int           iSno;                       // 合约编号
    int           iLastdate;                  // 最后一次计算息费日期
    int           iClosedate;                 // 合约全部偿还日期
    char          szPunidebts[21 + 1];        // 逾期本金罚息
    char          szPunidebtsRepay[21 + 1];   // 本金罚息偿还
    char          szPunidebtsunfrz[21 + 1];   // 逾期本金罚息
    char          szPunifeeunfrz[21 + 1];     // 逾期息费罚息
    char          szPunirights[21 + 1];       // 逾期权益罚息
    char          szPunirightsRepay[21 + 1];  // 权益罚息偿还
    char          szPunirightsunfrz[21 + 1];  // 逾期权益罚息
    char          szFeeunfrz[21 + 1];         // 实时偿还利息
    char          szOverduefeeunfrz[21 + 1];  // 实时偿还逾期利息
    int           iRightsqty;                 // 未偿还权益数量
    int           iOverduerightsqty;          // 逾期未偿还权益数量
    char          szSecuid[10 + 1];           // 证券账号
    char          chDebtsflag;                // 合约生成方式
    LONGLONG      llPositionid;               // 头寸编号
    int           iDefertimes;                // 展期次数
};
//-------------------------------10103597:客户担保权益调整明细查询--------------------------
struct MATRADEAPI CReqFislQryRpcGuaranteeInterestAdjustmentDetailField
{
};
struct MATRADEAPI CRspFislQryRpcGuaranteeInterestAdjustmentDetailField
{
    int           iServerid;             // 服务器编号
    LONGLONG      llCustid;              // 客户代码
    char          szOrgid[4 + 1];        // 机构编码
    LONGLONG      llFundid;              // 资金帐号
    char          chMoneytype;           // 货币代码
    char          chMarket;              // 交易市场
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkcode[8 + 1];      // 权益证券代码
    char          szRelastkcode[8 + 1];  // 正股证券代码
    char          chRightskind;          // 权益类型
    char          chAdjustflag;          // 调整方式
    LONGLONG      llAdjustqty;           // 股份调整数量
    char          szAdjustprice[21 + 1]; // 调整股份每股价格
    char          szAdjustamt[21 + 1];   // 调整金额
    char          szRemark[64 + 1];      // 备注
};
//-------------------------------10103598:客户历史维持担保比例查询--------------------------
struct MATRADEAPI CReqFislQryRpcHisMaintainGuaranteeRatioField
{
    LONGLONG      llFundid;           // 资金账号
    int           iBegindate;         // 开始日期
    int           iEnddate;           // 结束日期
    char          szPoststr[128 + 1]; // 定位串
    int           iCount;             // 请求行数
};
struct MATRADEAPI CRspFislQryRpcHisMaintainGuaranteeRatioField
{
    int           iSysdate;            // 系统日期
    char          szOrgid[4 + 1];      // 机构编码
    LONGLONG      llFundid;            // 资金帐号
    char          chMoneytype;         // 货币代码
    LONGLONG      llCustid;            // 客户代码
    char          szCustname[16 + 1];  // 客户姓名
    char          szKeeprate[21 + 1];  // 维持担保比例
    char          szMarginavl[21 + 1]; // 保证金
    char          chClosestatus;       // 平仓状态
    char          chCreditstatus;      // 信用状态
    char          chNoticestatus;      // 追缴状态
    char          szPoststr[8 + 1];    // 定位串
};
//-------------------------------10103600:融资头寸预约申请--------------------------
struct MATRADEAPI CReqFislRpcFinancingPosiSubApplicationField
{
    LONGLONG      llFundid;              // 资金账号
    char          szReqintrrate[21 + 1]; // 申请费率
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请结束日期
    char          szReqamtmin[21 + 1];   // 申请金额下限
    char          szReqamtmax[21 + 1];   // 申请金额上限
    char          szRemark[255 + 1];     // 备注
};
struct MATRADEAPI CRspFislRpcFinancingPosiSubApplicationField
{
    int           iReqsno; // 申请编号
};
//-------------------------------10103601:融资头寸预约申请查询--------------------------
struct MATRADEAPI CReqFislQryRpcFinancingPosiSubApplicationField
{
    int           iReqsno;           // 申请编号
    char          chReqtype;         // 申请类型
    LONGLONG      llFundid;          // 资金账号
    int           iBeginsysdate;     // 申请单日期开始
    int           iEndsysdate;       // 申请单日期结束
    int           iChkdatebegin;     // 审批开始日期
    int           iChkdateend;       // 审批结束日期
    int           iCheckdate;        // 审批日期
    char          szStatus[128 + 1]; // 申请状态
    int           iQrynum;           // 请求数量
    char          szPosstr[64 + 1];  // 定位串
};
struct MATRADEAPI CRspFislQryRpcFinancingPosiSubApplicationField
{
    int           iServerid;             // 服务器编号
    int           iReqsno;               // 申请编号
    int           iSysdate;              // 申请单日期
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请到期日期
    char          chReqtype;             // 申请类型
    char          szOrgid[4 + 1];        // 机构编码
    LONGLONG      llFundid;              // 资金账号
    char          szReqamtmin[21 + 1];   // 申请金额下限
    char          szReqamtmax[21 + 1];   // 申请金额上限
    char          szReqintrrate[21 + 1]; // 申请费率
    char          szChkamt[21 + 1];      // 审批金额
    char          szChkintrrate[21 + 1]; // 审批费率
    int           iChkdatebegin;         // 审批开始日期
    int           iChkdateend;           // 审批结束日期
    int           iCheckdate;            // 审批日期
    char          szChkremark[255 + 1];  // 审批备注
    LONGLONG      llLinkposition;        // 关联头寸
    int           iOperid;               // 审批柜员
    char          szStatus[128 + 1];     // 申请状态
    char          szRemark[255 + 1];     // 备注
};
//-------------------------------10103602:融资头寸变更预约申请--------------------------
struct MATRADEAPI CReqFislRpcChgFinancingPosiSubApplicationField
{
    LONGLONG      llFundid;              // 资金账号
    char          szReqintrrate[21 + 1]; // 申请费率
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请结束日期
    char          szReqamtmin[21 + 1];   // 申请金额下限
    char          szReqamtmax[21 + 1];   // 申请金额上限
    LONGLONG      llLinkposition;        // 关联头寸
    char          szRemark[255 + 1];     // 备注
};
struct MATRADEAPI CRspFislRpcChgFinancingPosiSubApplicationField
{
    int           iReqsno; // 申请编号
};
//-------------------------------10103603:取消融资头寸预约申请--------------------------
struct MATRADEAPI CReqFislRpcCancelFinancingPosiSubApplicationField
{
    int           iReqsno;           // 申请编号
    char          szRemark[255 + 1]; // 备注
};
struct MATRADEAPI CRspFislRpcCancelFinancingPosiSubApplicationField
{
};
//-------------------------------10103604:客户关联融资头寸查询--------------------------
struct MATRADEAPI CReqFislQryRpcLinkFinancingPosiField
{
    LONGLONG      llFundid;         // 资金账号
    LONGLONG      llPositionid;     // 头寸编号
    int           iQrynum;          // 请求数量
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspFislQryRpcLinkFinancingPosiField
{
    char          szOrgid[4 + 1];         // 机构代码
    LONGLONG      llFundid;               // 资金账号
    LONGLONG      llCustid;               // 客户代码
    char          szFundname[16 + 1];     // 客户名称
    LONGLONG      llPositionid;           // 头寸编码
    char          szPositionname[32 + 1]; // 头寸名称
    char          chPositype;             // 头寸类型
    char          szTotalamt[21 + 1];     // 总额
    char          szAmtlast[21 + 1];      // 昨日金额
    char          szAvlamt[21 + 1];       // 实时可用
    char          szUsedamt[21 + 1];      // 已使用
    char          szUsedamtreal[21 + 1];  // 实时已使用
    char          szRepayamt[21 + 1];     // 今日直接还款金额
    char          szRepayamtreal[21 + 1]; // 实时已偿还
    char          szFullrate[21 + 1];     // 全额占用费率
    char          szUnuserate[21 + 1];    // 空闲占用费率
    char          szUserate[21 + 1];      // 使用费率
    char          szIntrrate[21 + 1];     // 开仓费率
    char          szPunirate[21 + 1];     // 罚息费率
    char          chIntrkind;             // 计算央行利率
    int           iLastdate;              // 最后计息日
    int           iEnddate;               // 到期日期
    char          szTrdflag[128 + 1];     // 交易属性
    char          chStatus;               // 状态
    char          szPosstr[64 + 1];       // 定位串
};
//-------------------------------10103605:设置默认融资开仓头寸--------------------------
struct MATRADEAPI CReqFislRpcSetDefaultFinancingOpenPosiField
{
    LONGLONG      llFundid;         // 资金帐号
    LONGLONG      llPositionid;     // 资金头寸编号
};
struct MATRADEAPI CRspFislRpcSetDefaultFinancingOpenPosiField
{
};
//-------------------------------10103606:融券头寸预约申请--------------------------
struct MATRADEAPI CReqFislRpcMarginPosiSubApplicationField
{
    LONGLONG      llFundid;              // 资金账号
    char          chMarket;              // 市场代码
    char          szStkcode[8 + 1];      // 证券代码
    char          szReqintrrate[21 + 1]; // 申请费率
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请结束日期
    LONGLONG      llReqqtymin;           // 申请数量下限
    LONGLONG      llReqqtymax;           // 申请数量上限
    char          szRemark[255 + 1];     // 备注
};
struct MATRADEAPI CRspFislRpcMarginPosiSubApplicationField
{
    int           iReqsno; // 申请编号
};
//-------------------------------10103607:融券头寸变更申请--------------------------
struct MATRADEAPI CReqFislRpcChgMarginPosiApplicationField
{
    LONGLONG      llFundid;              // 资金账号
    char          chMarket;              // 市场代码
    char          szStkcode[8 + 1];      // 证券代码
    char          szReqintrrate[21 + 1]; // 申请费率
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请结束日期
    LONGLONG      llReqqtymin;           // 申请数量下限
    LONGLONG      llReqqtymax;           // 申请数量上限
    char          szRemark[255 + 1];     // 备注
};
struct MATRADEAPI CRspFislRpcChgMarginPosiApplicationField
{
    int           iReqsno; // 申请编号
};
//-------------------------------10103608:融券头寸预约查询--------------------------
struct MATRADEAPI CReqFislQryRpcMarginPosiSubField
{
    int           iReqsno;           // 申请编号
    char          chReqtype;         // 请求类型
    int           iSysdate;          // 申请日期
    LONGLONG      llFundid;          // 信用资金账号
    int           iDatebegin;        // 申请开始日期
    int           iDateend;          // 申请结束日期
    int           iChkdatebegin;     // 审批开始日期
    int           iChkdateend;       // 审批结束日期
    int           iCheckdate;        // 审批日期
    char          chStatus;          // 申请状态
    int           iQrynum;           // 查询条数
    char          szPosstr[64 + 1];  // 定位串
    char          szRemark[255 + 1]; // 备注
};
struct MATRADEAPI CRspFislQryRpcMarginPosiSubField
{
    int           iServerid;             // 核心编号
    int           iReqsno;               // 申请编号
    int           iSysdate;              // 申请日期
    char          chReqtype;             // 请求类型
    LONGLONG      llCustid;              // 客户代码
    char          szOrgid[5 + 1];        // 机构编码
    LONGLONG      llFundid;              // 资金账户
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 证券代码
    LONGLONG      llReqqtymin;           // 申请数量下限
    LONGLONG      llReqqtymax;           // 申请数量上限
    char          szReqintrrate[21 + 1]; // 申请费率
    int           iDatebegin;            // 申请开始日期
    int           iDateend;              // 申请到期日期
    LONGLONG      llChkqty;              // 审批数量
    char          szChkintrrate[21 + 1]; // 审批费率
    int           iChkdatebegin;         // 审批开始日期
    int           iChkdateend;           // 审批到期日期
    int           iCheckdate;            // 审批日期
    char          szChkremark[255 + 1];  // 审批意见
    LONGLONG      llLinkposition;        // 关联头寸
    int           iOperid;               // 审批人
    char          chStatus;              // 申请状态
    char          szRemark[255 + 1];     // 备注
};
//-------------------------------10103609:融券开仓头寸指定--------------------------
struct MATRADEAPI CReqFislRpcMarginOpenPosiAppointField
{
    LONGLONG      llFundid;         // 资金帐号
    LONGLONG      llPositionid;     // 融券头寸编号
};
struct MATRADEAPI CRspFislRpcMarginOpenPosiAppointField
{
};
//-------------------------------10103610:客户关联融券头寸查询--------------------------
struct MATRADEAPI CReqFislQryRpcLinkMarginPosiField
{
    LONGLONG      llFundid;            // 资金账号
    LONGLONG      llPositionid;        // 头寸编号
    char          szPositype[128 + 1]; // 头寸类型
    char          szMarket[128 + 1];   // 交易市场
    char          szStkcode[8 + 1];    // 证券代码
    int           iQrynum;             // 请求数量
    char          szPosstr[64 + 1];    // 定位串
};
struct MATRADEAPI CRspFislQryRpcLinkMarginPosiField
{
    char          szOrgid[4 + 1];         // 机构代码
    LONGLONG      llFundid;               // 资金账号
    LONGLONG      llCustid;               // 客户代码
    LONGLONG      llPositionid;           // 头寸编码
    char          szPositionname[32 + 1]; // 头寸名称
    char          chPositype;             // 头寸类型
    char          chMarket;               // 交易市场
    char          szStkcode[8 + 1];       // 证券代码
    char          szStkname[16 + 1];      // 证券名称
    LONGLONG      llStktotal;             // 头寸数量
    LONGLONG      llStklast;              // 昨日数量
    LONGLONG      llStkavl;               // 头寸实时可用
    LONGLONG      llStkused;              // 头寸已使用
    LONGLONG      llStkusedreal;          // 头寸实时使用
    LONGLONG      llStkrepayreal;         // 实时已偿还
    char          szFullrate[21 + 1];     // 全额占用费率
    char          szUnuserate[21 + 1];    // 空闲占用费率
    char          szUserate[21 + 1];      // 使用费率
    char          szIntrrate[21 + 1];     // 开仓费率
    char          szPunirate[21 + 1];     // 罚息费率
    char          chIntrkind;             // 计算央行利率
    int           iBegindate;             // 生效开始日期
    int           iEnddate;               // 生效截至日期
    char          chStatus;               // 状态
    char          szPosstr[64 + 1];       // 定位串
};
//-------------------------------10103611:融券头寸预约取消--------------------------
struct MATRADEAPI CReqFislRpcCancelMarginPosiSubField
{
    int           iReqsno;           // 申请编号
    int           iSysdate;          // 申请日期
    char          szRemark[255 + 1]; // 备注
};
struct MATRADEAPI CRspFislRpcCancelMarginPosiSubField
{
};
//-------------------------------10103612:解冻中签日预冻结的资金--------------------------
struct MATRADEAPI CReqFislRpcUnfreezePreFrozenFundsField
{
    LONGLONG      llFundid;         // 资金账号
    int           iMatchdate;       // 中签日期
};
struct MATRADEAPI CRspFislRpcUnfreezePreFrozenFundsField
{
    int           iSno;               // 解冻流水序号
    char          szUnfrzamt[21 + 1]; // 解冻金额
};
//-------------------------------10103613:放弃配售缴款, 支持三方交易--------------------------
struct MATRADEAPI CReqFislRpcCancelRationField
{
    int           iSno;              // 流水号
    int           iMatchdate;        // 交易日期
    char          chMarket;          // 交易市场
    char          szStkcode[6 + 1];  // 证券代码
    char          szSecuid[10 + 1];  // 股东代码
    LONGLONG      llFundid;          // 资金账户
    int           iGiveupqty;        // 放弃数量
    char          szBankcode[4 + 1]; // 外部银行
    char          szRemark[64 + 1];  // 备注信息
};
struct MATRADEAPI CRspFislRpcCancelRationField
{
};

//-------------------------------10349001:系统探测--------------------------
struct MATRADEAPI CReqFislBizStatusField
{
  char            szStkbd[2 + 1];             // 交易板块 不送时为系统探测，返回第一结果集； 送具体板块时，返回该板块是否可以下单以及交易日期等信息; 送*或者@时返回所有板块是否可下单信息
};

struct MATRADEAPI CRspFislBizStatusField
{
  char            szStkbd[2 + 1];             // 交易板块 
  int             iTrdDate;                   // 当前交易日 
  int             iPhysicalDate;              // 物理日期 
  char            chTrdStatus;                // 交易状态 0:禁止交易  1：允许交易
  char            szRemark[128 + 1];          // 备注 不可下单的理由
};

//-------------------------------10303008:质押股份明细查询(单客户)--------------------------
struct MATRADEAPI CReqFislQryPledgeSharesField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            chQueryFlag;                // 查询方向 
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
};

struct MATRADEAPI CRspFislQryPledgeSharesField
{
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  LONGLONG        llStkPrebln;                // 证券昨日余额 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkAvl;                   // 证券可用数量 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBondCvtrate[21 + 1];      // 债券折算率 
  LONGLONG        llStkStdAvl;                // 标准券可用 
};

//-------------------------------10309005:资金流水查询--------------------------
struct MATRADEAPI CReqFislQryFundFlowField
{
  LONGLONG        llCuacctCode;               // 资金账户 资金账户与客户代码不能同时为空
  LONGLONG        llCustCode;                 // 客户代码 资金账户与客户代码不能同时为空
  char            szIntOrg[256 + 1];          // 内部机构 
  int             iBizCodes;                  // 业务代码 1:订单划出 2:订单划入 3:股息红利划出 4:资金上账 5:资金冻结
  char            chQueryFlag;                // 查询方向 '0':向后取数据 '1':向前取数据 其他全部返回
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
  int             iTrdDate;                   // 交易日期 
  char            chReverseStatus;            // 冲正状态 
  LONGLONG        llSerialNo;                 // 流水序号 
  char            szTransStatuses[8 + 1];     // 转帐状态 '0':已经调用 '1':转帐成功 '2':银证转入失败 '3':超时 （复数）
};

struct MATRADEAPI CRspFislQryFundFlowField
{
  char            szQryPos[32 + 1];           // 定位串查询 
  LONGLONG        llSerialNo;                 // 流水序号 
  int             iTrdDate;                   // 交易日期 
  char            szSyncTime[32 + 1];         // 发生时间 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iBizCodes;                  // 业务代码 1:订单划出 2:订单划入 3:股息红利划出 4:资金上账 5:资金冻结
  char            szBizContext[128 + 1];      // 业务内容 
  LONGLONG        llBizSn;                    // 关联流水号 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szFundBlnEffect[21 + 1];    // 资金余额变化 
  char            szFundAvlEffect[21 + 1];    // 资金可用变化 
  char            szFundFrzEffect[21 + 1];    // 资金冻结变化 
  char            szFundUfzEffect[21 + 1];    // 资金解冻变化 
  char            chReverseStatus;            // 冲正状态 '1':已冲正
  char            chTransStatus;              // 转帐状态 '0':已经调用 初始状态，表示订单已经成功调用集中交易的银证转入接口。 '1':转帐成功 转账过程结束。 '2':银证转入失败 调用银证转入接口失败。 '3':超时 获取转账结果超时 
};

//-------------------------------10302002:买卖委托(快速)--------------------------
struct MATRADEAPI CReqFislOrderExField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szClientInfo[256 + 1];      // 终端信息
  int             iOrderBsn;                  // 委托批号
  int             iCuacctSn;                  // 账户序号
};

struct MATRADEAPI CRspFislOrderExField
{
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号
  int             iCuacctSn;                  // 账户序号
};

//-------------------------------10309007:股份流水查询--------------------------
struct MATRADEAPI CReqFislQryStockFlowField
{
  LONGLONG        llCuacctCode;               // 资金账户 
  char            szIntOrg[1024 + 1];         // 内部机构 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
  int             iTrdDate;                   // 交易日期 
  char            szStkbd[32 + 1];            // 交易板块 
  char            chReverseStatus;            // 冲正状态 
  int             iBizCode;                   // 业务代码 
};

struct MATRADEAPI CRspFislQryStockFlowField
{
  char            szQryPos[32 + 1];           // 定位串查询 
  LONGLONG        llSerialNo;                 // 流水序号 
  int             iTrdDate;                   // 交易日期 
  char            szSyncTime[32 + 1];         // 同步时间 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  int             iBizCode;                   // 业务代码 
  char            szBizContext[128 + 1];      // 业务内容 
  LONGLONG        llBizSn;                    // 关联流水号 
  LONGLONG        llStkAvl;                   // 股份可用金额 
  LONGLONG        llStkBlnEffect;             // 股份余额变化 
  LONGLONG        llStkAvlEffect;             // 股份可用变化 
  LONGLONG        llStkFrzEffect;             // 股份冻结变化 
  LONGLONG        llStkUfzEffect;             // 股份解冻变化 
  char            chReverseStatus;            // 冲正状态 '1' - 已冲正
  char            szSubsysSn[32 + 1];         // KCBP节点 
};
//-------------------------------10306028:日切交易日查询--------------------------
struct MATRADEAPI CReqFislQrySettDateField
{
};

struct MATRADEAPI CRspFislQrySettDateField
{
  int             iTrdDate;                   // 交易日期 
  int             iSettDate;                  // 下一交易日 
  int             iSubsysSn;                  // 子系统编号 
};

//-------------------------------10305015:资金划拨(融资融券)--------------------------
struct MATRADEAPI CReqFislFundTransferJzjyField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 ‘1’集中交易划出、订单系统划入 ‘2’集中交易划入、订单系统划出
  char            szBizAmt[21 + 1];           // 发生金额 
};

struct MATRADEAPI CRspFislFundTransferJzjyField
{
  LONGLONG        llSerialNo;                 // 流水序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chCurrency;                 // 货币代码 
  LONGLONG        llBizCode;                  // 摘要代码 
  LONGLONG        llBizSn;                    // 关联流水号 
  char            chDirect;                   // 划拨方向 
  char            szBizAmt[21 + 1];           // 发生金额 
};

//-------------------------------10321028:融资融券合约展期申请--------------------------
struct MATRADEAPI CReqFislContApplyField
{
  LONGLONG        llContDays;                 // 展期天数 
  int             iContStopDate;              // 展期截止日期 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkbd[2 + 1];             // 交易板块 
  int             iOpeningDate;               // 开仓日期 
};

struct MATRADEAPI CRspFislContApplyField
{
};

//-------------------------------10305036:银证转账--------------------------
struct MATRADEAPI CReqFislBankStkTransferField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 DD[CURRENCY]
  char            chTranType;                 // 转帐类型 ‘1’: 银行转入‘2’: 证券转银行
  char            szTranAmt[21 + 1];          // 转帐金额 
  char            szFundPwd[32 + 1];          // 资金密码 密码需要加密, 加密采用kbss_encrypt1加密 接口自动加密
  char            szBankCode[4 + 1];          // 银行代码 
  char            szBankPwd[32 + 1];          // 银行密码 明文
  char            chPwdFlag;                  // 资金密码校验标志 对于证券转银行,0表示不校验资金密码,1表示校验资金密码,不送默认为1
  char            szTrdPwd[32 + 1];           // 交易密码 需加密送入，加密采用kbss_encrypt1加密  接口自动加密
  char            chInitiator;                // 发起方 I-券商 O-银行 (目前只支持送I)
  int             iExtOrg;                    // 外部机构 U版需要送
  char            szTrdTermcode[20 + 1];      // 交易终端特征码 后台记录格式：MAC_ADDR+HD_ID+TRD_TERMCODE
};

struct MATRADEAPI CRspFislBankStkTransferField
{
  LONGLONG        llSerialNo;                 // 流水序号 
  char            szErrorMess[128 + 1];       // 错误信息 
};

//-------------------------------10321076:融券头寸明细查询--------------------------
struct MATRADEAPI CReqFislQryCashDetailField
{
  LONGLONG        llCashNo;                   // 头寸编号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkCode[8 + 1];           // 证券代码 
};

struct MATRADEAPI CRspFislQryCashDetailField
{
  LONGLONG        llCashNo;                   // 头寸编号 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chStkex;                    // 证券交易所 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llStkBln;                   // 头寸余额 
  LONGLONG        llStkRepayReal;             // 实时偿还数量 
  LONGLONG        llStkUsed;                  // 差额数量 
  int             iContractNo;                // 序号 
  int             iSlContractDate;            // 合约日期 
  char            szUserName[32 + 1];         // 用户名称 
  LONGLONG        llDeferQty;                 // 在途展期数量 
  LONGLONG        llRightsQty;                // 未偿还权益数量 
};

//-------------------------------10321012:查询偿还明细--------------------------
struct MATRADEAPI CReqFislQryRepayDetailField
{
  int             iTrdDate;                   // 交易日期 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iConTrdDate;                // 合约交易日期 
  char            chConStkex;                 // 合约市场 
  char            szConStkbd[32 + 1];         // 合约板块 
  char            szConOrderId[10 + 1];       // 合约合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szOrgId[1024 + 1];          // 分支机构 
};

struct MATRADEAPI CRspFislQryRepayDetailField
{
  int             iTrdDate;                   // 交易日期 
  int             iSno;                       // 流水号 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderDate;                 // 委托日期 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chRepayType;                // 偿还类型 0:偿还融资欠款 1:偿还融券费用
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iConTrdDate;                // 合约交易日期 
  char            chConStkex;                 // 合约市场 
  char            szConStkbd[2 + 1];          // 合约板块 
  char            szConOrderId[10 + 1];       // 合约合同序号 
  LONGLONG        llRltRepayAmt;              // 实时偿还金额 
  LONGLONG        llRltRepayInt;              // 实时偿还利息 
  char            szOrgId[4 + 1];             // 分支机构 
  char            chCreditdebtsType;          // 负债类型 
  LONGLONG        llFundEffect;               // 负债资金发生数 
  int             iRelaDate;                  // 关联负债日期 
  int             iRelasNo;                   // 关联负债流水号 
  char            szRemark[255 + 1];          // 备注 
  char            szStkName[8 + 1];           // 证券名称 
};

//-------------------------------10103614:从订单VIP系统资金划转--------------------------
struct MATRADEAPI CReqFislRpcFundTransferVipField
{
  LONGLONG        llFundid;                   // 资金账号 
  char            chMoneytype;                // 货币代码 
  char            szFundpwd[32 + 1];          // 资金密码 密码需要加密
  char            szBankcode[4 + 1];          // 银行代码 
  char            szBankpwd[32 + 1];          // 银行密码 密码需要加密
  char            chBanktrantype;             // 转帐类型 1:银行转证券 2:证券转银行
  char            szTranamt[21 + 1];          // 转帐金额 
};

struct MATRADEAPI CRspFislRpcFundTransferVipField
{
  int             iSno;                       // 委托序号 
};

//-------------------------------10103615:券源头寸合约查询--------------------------
struct MATRADEAPI CReqFislQryRpcPositionContractSrcField
{
  char            chQrytype;                  // 查询类型 0:当前 1:历史
  int             iSysdateBeg;                // 合约日期(起) 
  int             iSysdateEnd;                // 合约日期(止) 
  int             iSno;                       // 合约编号 
  char            szSourcetype[8 + 1];        // 券源类型 0:预约券(默认) 1:库存券申请 2:准库存券申请
  char            szOrgid[2048 + 1];          // 机构编码 
  LONGLONG        llFundid;                   // 资金帐号 
  char            szMarket[8 + 1];            // 交易市场 0:深A  1:沪A
  char            szStkcode[10 + 1];          // 证券代码 
  char            szStatus[8 + 1];            // 合约状态 0:未了结 1:已了结
  char            szRatediffbeg[21 + 1];      // 起始利率轧差比例 
  char            szRatediffend[21 + 1];      // 结束利率轧差比例 
  int             iQrynum;                    // 查询数目 
  char            szPosstr[128 + 1];          // 定位串 
};

struct MATRADEAPI CRspFislQryRpcPositionContractSrcField
{
  int             iSysdate;                   // 合约日期 
  int             iSno;                       // 合约编号 
  char            szSourcetypename[32 + 1];   // 券源类型名称 
  char            chSourcetype;               // 券源类型 
  char            szOrgid[4 + 1];             // 机构编码 
  LONGLONG        llFundid;                   // 资金账号 
  char            szFundname[16 + 1];         // 客户名称 
  char            szMarketname[32 + 1];       // 市场名称 
  char            chMarket;                   // 市场代码 
  char            szStkcode[10 + 1];          // 证券代码 
  char            szStkname[8 + 1];           // 证券名称 
  char            szInteratemodelname[16 + 1];// 计息模式名称 
  char            chInteratemodel;            // 计息模式 0:全额占用费 1:空闲费+负债使用 2:空闲费+头寸使用
  char            szFullrate[21 + 1];         // 全额占用费率 
  char            szUnuserate[21 + 1];        // 空闲占用费率 
  char            szUserate[21 + 1];          // 使用费率 
  char            szTermrate[21 + 1];         // 证金利率 
  char            szRatediff[21 + 1];         // 利率与证金利率扎差 
  int             iPositionid;                // 头寸编号 
  LONGLONG        llStktotal;                 // 头寸数量 
  LONGLONG        llStkused;                  // 已使用数量 
  LONGLONG        llStkavl;                   // 可用数量 
  LONGLONG        llUnlockqty;                // 已解锁数量 
  LONGLONG        llRightsqty;                // 权益数量 
  LONGLONG        llUnpaidrightsqty;          // 未付权益数量 
  int             iContractno;                // 证金合约编号 
  int             iEnddate;                   // 到期日期 
  int             iReqdate;                   // 申请日期 
  int             iReqsno;                    // 申请单号 
  int             iAssigndate;                // 分派日期 
  char            szPosistkprice[21 + 1];     // 计息价格 
  char            szFee[21 + 1];              // 累计息费 
  LONGLONG        llRegstkavl;                // 登记日未开仓数量 
  LONGLONG        llUnuserights;              // 累积未开仓部分权益金额 
  LONGLONG        llDeferqty;                 // 在途展期数量 
  int             iDefersysdate;              // 展期新合约日期 
  int             iDefersno;                  // 展期新合约编号 
  char            szStatusname[32 + 1];       // 合约状态名称 
  char            chStatus;                   // 合约状态 
  int             iClosedate;                 // 了结日期 
  int             iPreparedate;               // 筹券日期 
  char            szRemark[255 + 1];          // 备注 
  int             iLenderid;                  // 出借人代码 
  char            szLendername[64 + 1];       // 出借人名称 
  char            szPosstr[128 + 1];          // 定位串 
};

//-------------------------------10304013:认证信息设置(密码录入/修改)--------------------------
struct MATRADEAPI CReqFislAuthDataSetJzjyField
{
  LONGLONG        llUserCode;                 // 用户代码 
  char            szAuthData[256 + 1];        // 认证数据 明文
  char            szSrcAuthData[256 + 1];     // 原认证数据 明文
  char            chUseScope;                 // 使用范围 0: 股票交易密码 2：信用交易密码
};

struct MATRADEAPI CRspFislAuthDataSetJzjyField
{
  char            szMsgOk[32 + 1];            // 成功信息 
};

//-------------------------------10305019:资金划拨(单账户多节点) 订单节点之间划拨--------------------------
struct MATRADEAPI CReqFislFundTransferNodeField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 ‘0’划入 ‘1’划出
  char            szTransFund[21 + 1];        // 划拨金额 
};

struct MATRADEAPI CRspFislFundTransferNodeField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szTransFund[21 + 1];        // 划拨金额 
  LONGLONG        llSerialNo;                 // 流水序号 
  char            chTransStatus;              // 划拨状态 0-已调用 1-成功 2-失败 3-超时
  char            szRemark[256 + 1];          // 备注 划拨状态为2/3时返回错误原因
};

//-------------------------------10323074:融资融券资金和信用额度查询--------------------------
struct MATRADEAPI CReqFislQryFundAvlField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspFislQryFundAvlField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iIntOrg;                    // 内部机构 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szCreditFundBln[21 + 1];    // 融券卖出金额 
  char            szCreditFundUsed[21 + 1];   // 信用资金占用 
  char            szMarginValue[21 + 1];      // 保证金可用余额 
  char            szFiCreditAvl[21 + 1];      // 融资可用额度 
  char            szSlCreditAvl[21 + 1];      // 融券可用额度 
  LONGLONG        llCashFiNo;                 // 融资头寸编号 
  LONGLONG        llCashSlNo;                 // 融券头寸编号 
  char            szFiCashAvl[21 + 1];        // 融资头寸可用 
  char            szCreditLine[21 + 1];       // 信用额度 
  char            chCreditType;               // 信用额度类型 
  char            szFiCredit[21 + 1];         // 融资信用额度 
  char            szSlCredit[21 + 1];         // 融券信用额度 
};

//-------------------------------10103616:用户密码管理--------------------------
struct MATRADEAPI CReqFislAutuDataMngField
{
  char            chOperationType;            // 操作类型 1:修改 3:重置
  char            chUseScope;                 // 使用范围 0:交易密码
  char            chAuthType;                 // 认证类型 0:密码
  char            szOldAuthData[256 + 1];     // 原认证信息 
  char            szNewAuthData[256 + 1];     // 新认证信息 
  char            szOpRemark[128 + 1];        // 操作备注 
  char            chFislSameFlag;             // 同步普通标志 1:取密码表交易密码同步两融
  char            chSubsysFlag;               // 同步标示 0:本地和对接系统均更新 1:仅更新对接系统 2:仅更新本地
  char            chEncryptType;              // 密码类型 1:数据库存储算法 其他:通信加密算法
  LONGLONG        llCuacctCode;               // 资产账户 对接消费支付子系统密码维护必送
};

struct MATRADEAPI CRspFislAutuDataMngField
{
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FISL_TRADE_API_STRUCT_H__