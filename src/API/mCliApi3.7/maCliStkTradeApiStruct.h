#if !defined(__MA_CLI_STK_TRADE_API_STRUCT_H__)
#define __MA_CLI_STK_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10301036:证券信息查询[快速]--------------------------
struct MATRADEAPI CReqStkQryInfoExField
{
  char            szStkbd[2 + 1];             // 交易板块
  char            szStkCode[8 + 1];           // 证券代码
  char            chQueryFlag;                // 查询方向
  char            szQueryPos[32 + 1];         // 定位串
  int             iQueryNum;                  // 查询行数
  char            szCuacctCode[16 + 1];       // 资产账户 COS系统使用
};

struct MATRADEAPI CRspStkQryInfoExField
{
  char            szQryPos[32 + 1];           // 定位串查询 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 
  char            chStkStatus;                // 证券状态 
  char            chCurrency;                 // 货币代码 
  char            szStkUplmtRatio[21 + 1];    // 涨停比例 
  char            szStkLwlmtRatio[21 + 1];    // 跌停比例 
  char            szStkUplmtPrice[21 + 1];    // 涨停价格 
  char            szStkLwlmtPrice[21 + 1];    // 跌停价格 
  LONGLONG        llStkUplmtQty;              // 限价数量上限 
  LONGLONG        llStkLwlmtQty;              // 限价数量下限 
  LONGLONG        llStkUplmtQtyMp;            // 市价数量上限 
  LONGLONG        llStkLwlmtQtyMp;            // 市价数量下限 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            chStkLotFlag;               // 单位标志 
  char            szStkSpread[21 + 1];        // 价位价差 
  char            chStkCalMktval;             // 市值标志 
  char            chStkSuspended;             // 停牌标志 
  char            szBondInt[21 + 1];          // 债券利息 
  int             iBuyUnit;                   // 买入最小单位 
  int             iSaleUnit;                  // 卖出最小单位 
  char            chStkLevel;                 // 证券级别 
  int             iStkDeadline;               // 交易截止日期 
  char            szRemindMessage[128 + 1];   // 退市提示信息 
  char            szStkBizs[512 + 1];         // 证券业务 DD[STK_BIZ]
  LONGLONG        llStkCirculation;           // 流通量 
  char            chIsReg;                    // 是否注册制 
  char            chIsVie;                    // 是否具有协议控制架构 
  char            chNoprofit;                 // 是否尚未盈利 
  char            chVoterights;               // 是否存在投票权差异 
};

//-------------------------------10301001:证券信息查询--------------------------
struct MATRADEAPI CReqStkQryStkInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkLevel;                 // 证券级别 字典[STK_LEVEL] 其中‘Z‘表示为交易所标示退市证券类别;不输入查全部
  char            chStkex;                    // 交易市场 当交易板块输入不合法时此字段有效
  char            szCuacctCode[16 + 1];       // 资产账户 COS系统使用
};

struct MATRADEAPI CRspStkQryStkInfoField
{
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 
  char            chStkStatus;                // 证券状态 
  char            chCurrency;                 // 货币代码 
  char            szStkUplmtRatio[21 + 1];    // 涨停比例 
  char            szStkLwlmtRatio[21 + 1];    // 跌停比例 
  char            szStkUplmtPrice[21 + 1];    // 涨停价格 
  char            szStkLwlmtPrice[21 + 1];    // 跌停价格 
  LONGLONG        llStkUplmtQty;              // 限价数量上限 
  LONGLONG        llStkLwlmtQty;              // 限价数量下限 
  LONGLONG        llStkUplmtQtyMp;            // 市价数量上限 
  LONGLONG        llStkLwlmtQtyMp;            // 市价数量下限 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            chStkLotFlag;               // 单位标志 
  char            szStkSpread[21 + 1];        // 价位价差 
  char            chStkCalMktval;             // 市值标志 
  char            chStkSuspended;             // 停牌标志 
  char            szStkIsin[16 + 1];          // 国际编码 
  char            chStkSubCls;                // 证券子类别 字典[STK_SUB_CLS]
  char            szStkBizes[512 + 1];        // 证券业务 字典[STK_BIZ]
  char            chStkCustodyMode;           // 托管模式 字典[STK_CUSTODY_MODE]
  char            szStkUndlCode[8 + 1];       // 正股代码 
  char            szStkFaceVal[21 + 1];       // 证券面值 
  char            szBondInt[21 + 1];          // 债券利息 
  int             iBuyUnit;                   // 买入最小单位 
  int             iSaleUnit;                  // 卖出最小单位 
  char            chStkLevel;                 // 证券级别 
  int             iStkDeadline;               // 交易截止日期 
  char            szRemindMessage[128 + 1];   // 退市提示信息 
  LONGLONG        llStkCirculation;           // 流通量 
  int             iStkCtrlExpDate;            // 监控截至日期 
  LONGLONG        llStkPublishCount;          // 总发行量 
  int             iDlvyCycle;                 // 交收周期 
  char            szStkFullName[128 + 1];     // 中文证券名称（长） 
  char            chIsReg;                    // 是否注册制 
  char            chIsVie;                    // 是否具有协议控制架构 
  char            chNoprofit;                 // 是否尚未盈利 
  char            chVoterights;               // 是否存在投票权差异 
  char            chSwapFlag;                 // 转股期标志 
  char            chPutbackFlag;              // 回售期标志 
  char            chPutbackCancelFlag;        // 回售撤销期标志 
  char            szStkLayerInfo[16 + 1];     // 证券分层信息 
  char            chStkLayerFlag;             // 分层信息标志 
};

//-------------------------------10303132:盘后行情查询(上海科创板)--------------------------
struct MATRADEAPI CReqStkQryClosingMktField
{
  char            chStkex;                    // 市场 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向前取数据 1:向后取数据
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
};

struct MATRADEAPI CRspStkQryClosingMktField
{
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 市场 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 
  char            szClosingPrice[21 + 1];     // 今日收盘价 
  LONGLONG        llStkMatchedQty;            // 成交数量 
  char            szStkMatchedAmt[21 + 1];    // 成交金额 
  LONGLONG        llStkBuyLeftQty;            // 买盘剩余数量 
  LONGLONG        llStkSaleLeftQty;           // 卖盘剩余数量 
  char            chStkSuspended;             // 停牌标识 
  char            szUpdTime[32 + 1];          // 更新时间 
  char            szRemark[56 + 1];           // 备注 
};

//-------------------------------10301022:LOF信息查询------------------------------------
struct MATRADEAPI CReqStkQryLofInfoField
{
  char szStkbd[2 + 1];                        // 交易板块
  char szLofCode[8 + 1];                      // LOF代码
};

struct MATRADEAPI CRspStkQryLofInfoField
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
struct MATRADEAPI CReqStkOrderField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 科创板市价委托填保护价
  LONGLONG        llOrderQty;                 // 委托数量 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  int             iOrderBsn;                  // 委托批号 
  char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等 格式： "PC;IIP=公网IP地址;IPORT=公网IP端口号;LIP=内网IP;MAC=MAC地址;HD=硬盘序列号;PCN=PC终端设备名;CPU=CPU序列号;PI=硬盘分区信息;VOL=系统盘卷标号@扩展信息1;扩展信息2;扩展信息3"
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iCuacctSn;                  // 账户序号 客户端标识，用于多个客户端共用同一个资产账号下单时，区分不同客户端的委托与成交信息，并提供了相关的接口查询功能。
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szOrderText[256 + 1];       // 委托扩展 格式:关键字1=值1,关键字2=值2 格式:关键字1=值1,关键字2=值2 业务适当性检查，关键字为PROP_RISK_NO（匹配流水号）例如：PROP_RISK_NO=匹配流水号 基金概要确认标识，关键字为OF_CTR_FLAGS 例如：OF_CTR_FLAGS=1，1：已确认，其它值：未确认
  char            chSecurityLevel;            // 安全手段 1:校验会话凭证
  char            szSecurityInfo[256 + 1];    // 安全信息 自动填写
  char            szComponetStkCode[8 + 1];   // 成份股代码 ETF网下股票认购启用
  char            szComponetStkbd[2 + 1];     // 成份股板块 ETF网下股票认购、ETF实物申赎启用
  char            szStkbdLink[2 + 1];         // 关联板块 ETF网下股票认购、ETF实物申赎启用
  char            chPublishCtrlFlag;          // 股票风控标志 0：不控制 1：控制股票买入持仓不超过总股本一定比例 仅限于限仓需求，并需配合后台的相关设置参数方可生效
  char            szRepayOrderId[10 + 1];     // 偿还合同序号 用于融资融券交易
  int             iRepayOpeningDate;          // 偿还合约日期 用于融资融券交易
  char            szRepayStkCode[8 + 1];      // 偿还证券代码 用于融资融券交易
  char            szTrdacctLink[20 + 1];      // 关联股东 用于跨市场ETF交易
  char            szInitTrdAmt[21 + 1];       // 初始交易金额 用于股票质押交易
  int             iRepchDay;                  // 购回天数 用于股票质押交易
  char            szLoanerTrdacct[20 + 1];    // 出借人证券账户 用于股票质押交易
  char            szRepaySno[32 + 1];         // 合约编号 用于股票质押交易
  int             iItemno;                    // 合约序号 用于股票质押交易
  //char          chAutoBuyFlag;                // 自动展期标志
  char            chOutputDelayFlag;          // 输出时延标志 0:不输出服务器处理延时 1:输出服务器处理延时 默认为0
  char            chCloseOutMode;             // 平仓方式 用于股票质押交易
  char            chEtfMode;                  // ETF模式 适用于ETF申购交易 字典[ETF_MODE]
  char            szMeetingSeq[10 + 1];       // 股东大会编码 用于网络投票交易
  char            szVoteId[10 + 1];           // 议案编号 用于网络投票交易
  char            szOppLofCode[3 + 1];        // 转入方基金编码 用于上证LOF基金转换交易
  LONGLONG        llMatchNo;                  // 成交约定号 
  char            szOpptStkpbu[8 + 1];        // 对手方交易单元 
  char            szOpptTrdacct[20 + 1];      // 对手方股东账户 
  char            szStkprop[2 + 1];           // 股份性质 
};

struct MATRADEAPI CRspStkOrderField
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
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          szTrdacct[20 + 1];            // 证券账户
  int           iCuacctSn;                    // 账户序号
  int           iOrderSn;                     // 委托序号
  char          chOrderStatus;                // 委托状态
  char          szStkName[16 + 1];            // 证券名称
};

//-------------------------------10302003:组合委托------------------------------------
struct MATRADEAPI CReqStkCombOrderField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  int           iOrderBsn;                    // 委托批号
  char          szStrategyText[32768 + 1];    // 策略信息
  char          szClientInfo[256 + 1];        // 终端信息
  char          szTrdTermcode[20 + 1];        // 终端特征码
  int           iCuacctSn;                    // 账户序号
  char          chSecurityLevel;              // 安全手段
  char          szSecurityInfo[256 + 1];      // 安全信息
  char          chErrorFlag;                  // 错误屏弊标志
  char          chPublishCtrlFlag;            // 股票风控标志
};

struct MATRADEAPI CRspStkCombOrderField
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
  int           iOrderRet;                    // 委托结果
  char          szRetInfo[256 + 1];           // 返回信息
};

//-------------------------------10302004:委托撤单------------------------------------
struct MATRADEAPI CReqStkCancelOrderField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szOrderId[10 + 1];            // 合同序号
  int           iOrderBsn;                    // 委托批号
  char          szClientInfo[256 + 1];        // 终端信息
};

struct MATRADEAPI CRspStkCancelOrderField
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
struct MATRADEAPI CReqStkMaxTradeQtyField
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

struct MATRADEAPI CRspStkMaxTradeQtyField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llOrderQty;                 // 委托数量 可委托最大数量
  LONGLONG        llStkQty;                   // 成分股数量 
};

//-------------------------------10302011:批量委托------------------------------------
struct MATRADEAPI CReqStkQuantityOrderField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  int           iTrdacctCnt;                  // 证券账户数量
  char          szStkCode[8 + 1];             // 证券代码
  char          szOrderPrice[21 + 1];         // 委托价格
  LONGLONG      llOrderQty;                   // 委托数量
  char          szOrderAmt[21 + 1];           // 委托金额
  LONGLONG      llTrdacctSaleLimit;           // 单账户卖出股份限额
  int           iTrdacctStartNo;              // 证券账户起始序号
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          szStkpbu[8 + 1];              // 交易单元
  int           iOrderBsn;                    // 委托批号
  char          szClientInfo[256 + 1];        // 终端信息
  char          chSecurityLevel;              // 安全手段
  char          szSecurityInfo[256 + 1];      // 安全信息
  int           iCuacctSn;                    // 账户序号
  char          chPublishCtrlFlag;            // 股票风控标志
};

struct MATRADEAPI CRspStkQuantityOrderField
{
  int           iOrderBsn;                    // 委托批号
  LONGLONG      llCuacctCode;                 // 资产账户
  int           iTrdacctSuccCnt;              // 证券账户委托成功累计
  LONGLONG      llOrderQtySum;                // 委托数量成功累计
  char          szStkpbu[8 + 1];              // 交易单元
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          szStkName[16 + 1];            // 证券名称
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  int           iCuacctSn;                    // 账户序号
};

//-------------------------------10302012:批量撤单------------------------------------
struct MATRADEAPI CReqStkQuantityCancelOrderField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  int           iOrderBsn;                    // 委托批号
  char          szClientInfo[256 + 1];        // 终端信息
};

struct MATRADEAPI CRspStkQuantityCancelOrderField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  int           iOrderBsn;                    // 委托批号
  char          szOrderId[10 + 1];            // 合同序号
  int           iCancelRet;                   // 撤单结果
  char          szRetInfo[256 + 1];           // 返回信息
};

//-------------------------------10302019:ETF 篮子委托--------------------------------
struct MATRADEAPI CReqStkEtfOrderField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szTrdacct[20 + 1];            // 证券账户
  char          szEtfCode[8 + 1];             // ETF代码
  LONGLONG      llOrderQty;                   // 委托数量
  char          szPriceInfo[10240 + 1];       // 价格信息
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          szStkpbu[8 + 1];              // 交易单元
  char          chBizType;                    // 交易类型
  char          szClientInfo[256 + 1];        // 终端信息
  char          chSecurityLevel;              // 安全手段
  char          szSecurityInfo[256 + 1];      // 安全信息
  char          szTrdacctLink[10 + 1];        // 关联股东
  char          chPublishCtrlFlag;            // 持仓比例控制
  int           iCuacctSn;                    // 账户序号
};

struct MATRADEAPI CRspStkEtfOrderField
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
  char          szTrdacct[20 + 1];            // 证券账户
  char          szStkCode[8 + 1];             // 证券代码
  char          szStkName[16 + 1];            // 证券名称
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  int           iCuacctSn;                    // 账户序号
};

//-------------------------------10302020:篮子委托最大可交易数计算--------------------
struct MATRADEAPI CReqStkEtfMaxTradeQtyField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szTrdacct[20 + 1];            // 证券账户
  char          szEtfCode[8 + 1];             // ETF代码
  char          szPriceInfo[10240 + 1];       // 价格信息
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  char          chBizType;                    // 交易类型
  char          szTrdacctLink[10 + 1];        // 关联股东
};

struct MATRADEAPI CRspStkEtfMaxTradeQtyField
{
  char          chStkex;                      // 交易市场
  char          szStkbd[2 + 1];               // 交易板块
  char          szTrdacct[20 + 1];            // 证券账户
  char          szStkCode[8 + 1];             // 证券代码
  LONGLONG      llOrderQty;                   // 委托数量
};

//-------------------------------10303001:可用资金查询--------------------------------
struct MATRADEAPI CReqStkQryExpendableFundField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  int           iValueFlag;                   // 取值标志
};

struct MATRADEAPI CRspStkQryExpendableFundField
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
struct MATRADEAPI CReqStkQryExpendableSharesField
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

struct MATRADEAPI CRspStkQryExpendableSharesField
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
struct MATRADEAPI CReqStkQryCurrDayOrderField
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

struct MATRADEAPI CRspStkQryCurrDayOrderField
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
  char          szOpSite[256 + 1];            // 操作站点
  char          chChannel;                    // 操作渠道
  char          szRltSettAmt[21 + 1];         // 实时清算金额 
};

//-------------------------------10303004:当日成交查询--------------------------------
struct MATRADEAPI CReqStkQryCurrDayFillField
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

struct MATRADEAPI CRspStkQryCurrDayFillField
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
  char          szStkTrdacct[20 + 1];         // 证券账户
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
  char          szRltSettAmt[21 + 1];         // 实时清算金额 
};

//-------------------------------10303005:股东账户查询--------------------------------
struct MATRADEAPI CReqStkQryStkholderAcctField
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

struct MATRADEAPI CRspStkQryStkholderAcctField
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
struct MATRADEAPI CReqStkQryCanWithdrawOrderField
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
};

struct MATRADEAPI CRspStkQryCanWithdrawOrderField
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
struct MATRADEAPI CReqStkQryExpendableFundExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
};

struct MATRADEAPI CRspStkQryExpendableFundExField
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
  char          szCreditFundBln[21 + 1];      // 信用资金余额
  char          szCreditFundFrz[21 + 1];      // 信用冻结金额
};

//-------------------------------10303012:可用股份查询[快速]--------------------------
struct MATRADEAPI CReqStkQryExpendableSharesExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串
  int           iQueryNum;                    // 查询行数
};

struct MATRADEAPI CRspStkQryExpendableSharesExField
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
  LONGLONG      llStkBln;                     // 证券余额
  LONGLONG      llStkTrdFrz;                  // 证券交易冻结数量
  LONGLONG      llStkTrdUfz;                  // 证券交易解冻数量
  LONGLONG      llStkTrdOtd;                  // 证券交易在途数量
  LONGLONG      llStkTrdBln;                  // 证券交易扎差数量
  LONGLONG      llStkQty;                     // 当前拥股数
  LONGLONG      llStkDiff;                    // 可申赎数量
  LONGLONG      llStkTrdUnfrz;                // 已申赎数量
  LONGLONG      llStkIntradayQty;             // 当日普通买入股份
  LONGLONG      llFislIntradayQty;            // 当日融资买入股份
};

//-------------------------------10303013:当日委托查询[快速]--------------------------
struct MATRADEAPI CReqStkQryCurrDayOrderExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串
  int           iQueryNum;                    // 查询行数
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkpbu [8 + 1];             // 交易单元
  char          szOrderId[10 + 1];            // 合同序号
  int           iCuacctSn;                    // 账户序号
  char          chFlag;                       // 查询标志
};

struct MATRADEAPI CRspStkQryCurrDayOrderExField
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
struct MATRADEAPI CReqStkQryCurrDayFillExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chQueryFlag;                  // 查询方向
  char          szQueryPos[32 + 1];           // 定位串
  int           iQueryNum;                    // 查询行数
  int           iCuacctSn;                    // 账户序号
  char          szOrderId[10 + 1];            // 合同序号 
};

struct MATRADEAPI CRspStkQryCurrDayFillExField
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
struct MATRADEAPI CReqStkQryStkholderAcctExField
{
  LONGLONG      llCuacctCode;                 // 资产账户
};

struct MATRADEAPI CRspStkQryStkholderAcctExField
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
struct MATRADEAPI CReqStkQryCanWithdrawOrderExField
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

struct MATRADEAPI CRspStkQryCanWithdrawOrderExField
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

//-------------------------------10303018:ETF 成分股持仓查询--------------------------
struct MATRADEAPI CReqStkQryEtfCotPositionField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szEtfCode[8 + 1];             // ETF代码
  char          szStkbd[2 + 1];               // 交易板块
};

struct MATRADEAPI CRspStkQryEtfCotPositionField
{
  char          szEtfCode[8 + 1];                // ETF代码
  char          szStkCode[8 + 1];                // 成分股代码
  char          szStkName[8 + 1];                // 成分股简称
  char          chStkMarket;                     // 成分股交易市场
  LONGLONG      llStkQty;                        // 成分股数量
  LONGLONG      llStkAvl;                        // 客户持仓数量
  char          chEtfInsteadFlag;                // 替代标志
  char          szEtfOverflowRate[21 + 1];       // 溢价比例
  char          szCreationInsteadAmt[21 + 1];    // 申购替代金额
  char          szRedemptionInsteadAmt[21 + 1];  // 赎回替代金额
  char          szStkbd[2 + 1];                  // 交易板块
};

//-------------------------------10301034:证券市值额度查询------------------------------------
struct MATRADEAPI CReqStkMktQuotaField
{
  char          szTrdacct[20 + 1];            // 证券账户
};

struct MATRADEAPI CRspStkMktQuotaField
{
  char            chStkex;                    // 交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkpbu[8 + 1];            // 交易单元 
  LONGLONG        llMktQuota;                 // 市值额度(主板) 
  LONGLONG        llKcbMktquota;              // 市值额度(科创板) 
};

//-------------------------------10301016:ETF信息查询------------------------------------
struct MATRADEAPI CReqStkQryEtfInfoField
{
  char          szStkbd[2 + 1];             // 交易板块
  char          szEtfCode[8 + 1];           // ETF代码
  char          chEtfMode;                  // ETF模式 字典ETF_MODE
  char          chEtfType;                  // ETF类型
};

struct MATRADEAPI CRspStkQryEtfInfoField
{
  char          chStkex;                    // 证券交易所
  char          szStkbd[2 + 1];             // 交易板块
  int           iTrdDate;                   // 交易日期
  int           iTrdPredate;                // 前交易日
  char          szEtfCode[8 + 1];           // ETF代码
  char          szEtfName[32 + 1];          // ETF名称
  char          chEtfPublish;               // ETF发布标志
  char          chEtfCrFlag;                // 申购赎回标志
  LONGLONG      llEtfCrUnit;                // 申购赎回单位
  char          szEtfCashRatio[21 + 1];     // 现金替代比例
  int           iEtfStkNum;                 // ETF股票篮股票数
  char          szEtfEstmCash[21 + 1];      // ETF股票篮现金差
  char          szEtfCashComp[21 + 1];      // 基准单位现金
  char          szEtfNavPercu[21 + 1];      // 基准单位净值
  char          szEtfNav[21 + 1];           // ETF单位净值
  char          chEtfType;                  // ETF类型
  LONGLONG      llCreationLimit;            // 申购上限
  LONGLONG      llRedemptionLimit;          // 赎回上限
  LONGLONG      llClimitPerUser;            // 单账户申购上限
  LONGLONG      llRlimitPerUser;            // 单账户赎回上限
  char          chEtfMode;                  // ETF模式 字典ETF_MODE
  char          chRiskLevel;                // 风险级别
  char          chQsEtfCrFlag;              // 券商申购赎回标志
  char          szEtfUndlCode[8 + 1];       // ETF二级市场代码
};

//-------------------------------10301018:ETF篮信息查询------------------------------------
struct MATRADEAPI CReqStkQryEtfBasketField
{
  char          szStkbd[2 + 1];                   // 交易板块
  char          szEtfCode[8 + 1];                 // ETF代码
  char          chEtfMode;                        // ETF模式 字典ETF_MODE
};

struct MATRADEAPI CRspStkQryEtfBasketField
{
  char          chStkex;                          // 证券交易所
  char          szStkbd[2 + 1];                   // 交易板块
  char          szEtfCode[8 + 1];                 // ETF代码
  char          szStkCode[8 + 1];                 // 证券代码
  char          szStkName[16 + 1];                // 证券名称
  LONGLONG      llStkQty;                         // 证券数量
  char          chEtfInsteadFlag;                 // 现金替代标志
  char          szEtfOverflowRate[21 + 1];        // 溢价比例
  char          szRedemptionInsteadAmt[21 + 1];   // 赎回现金替代金额
  char          chComponetStkex;                  // 成份股市场代码
  char          szCreationInsteadAmt[21 + 1];     // 申购现金替代金额
  char          chEtfMode;                        // ETF模式 字典ETF_MODE
  int           iUpdDate;                         // 更新日期 
  char          szEtfDiscountRate[21 + 1];        // 折价比例 
};

//-------------------------------10301053:港股通汇率信息查询------------------------------------
struct MATRADEAPI CReqStkQryRateInfoField
{
  char          szStkbd[2 + 1];              // 交易板块
  char          chCurrency;                  // 货币代码
};

struct MATRADEAPI CRspStkQryRateInfoField
{
  char          chStkex;                     // 交易市场
  char          szStkbd[2 + 1];              // 交易板块
  char          chCurrency;                  // 货币代码
  char          szBuyRate[21 + 1];           // 买入参考汇率
  char          szDayBuyRiseRate[21 + 1];    // 日间买入参考汇率浮动比例
  char          szNightBuyRiseRate[21 + 1];  // 夜市买入参考汇率浮动比例
  char          szSaleRate[21 + 1];          // 卖出参考汇率
  char          szDaySaleRiseRate[21 + 1];   // 日间卖出参考汇率浮动比例
  char          szNightSaleRiseRate[21 + 1]; // 夜市卖出参考汇率浮动比例
  char          szMidRate[21 + 1];           // 中间参考汇率
  char          szBuySettRate[21 + 1];       // 买入结算汇率
  int           iUpdDate;                    // 更新日期
  char          szRemark[256 + 1];           // 备注
};

//-------------------------------10301057:港股通市场状态信息查询------------------------------------
struct MATRADEAPI CReqStkQryMktStatusInfoField
{
  char          szStkbd[2 + 1];              // 交易板块
};

struct MATRADEAPI CRspStkQryMktStatusInfoField
{
  char          chStkex;                     // 交易市场
  char          szStkbd[2 + 1];              // 交易板块
  char          szThresholdQuota[21 + 1];    // 每日初始额度
  char          szPosQuota[21 + 1];          // 日中剩余额度
  char          chQuotaStatus;               // 额度状态
  char          chMktTrdStatus;              // 市场交易状态
  int           iUpdDate;                    // 更新日期
  char          szRemark[128 + 1];           // 备注
};

//-------------------------------10301059:港股通标的证券状态查询------------------------------------
struct MATRADEAPI CReqStkQryUndlStkStatusField
{
  char          szStkbd[2 + 1];              // 交易板块
  char          szStkCode[8 + 1];            // 证券代码
  char          chObjectFlag;                // 标的标志
};

struct MATRADEAPI CRspStkQryUndlStkStatusField
{
  char          chStkex;                     // 证券交易所
  char          szStkbd[2 + 1];              // 交易板块
  char          szStkCode[8 + 1];            // 证券代码
  char          chLotTrdStatus;              // 整手交易状态
  char          chOddTrdStatus;              // 零股交易状态
  char          chObjectFlag;                // 标的标志
  int           iUpdDate;                    // 更新日期
  char          szRemark[256 + 1];           // 备注
};

//-------------------------------10303027:港股通二级资金台账查询------------------------------------
struct MATRADEAPI CReqStkQryFundAcctLvlTwoField
{
  LONGLONG      llCuacctCode;                // 资产账户
  char          chCurrency;                  // 货币代码
};

struct MATRADEAPI CRspStkQryFundAcctLvlTwoField
{
  LONGLONG      llCuacctCode;                // 资产账户
  char          chCurrency;                  // 货币代码
  char          szFundDetailCls[2 + 1];      // 资金明细分类
  int           iDlvyCycle;                  // 交收周期
  char          szFundBln[21 + 1];           // 资金余额
  char          szFundAvl[21 + 1];           // 资金可用金额
  char          szFundFrz[21 + 1];           // 资金冻结金额
  char          szFundUfz[21 + 1];           // 资金解冻金额
  char          szFundTrdFrz[21 + 1];        // 资金交易冻结金额
  char          szFundTrdUfz[21 + 1];        // 资金交易解冻金额
  char          szFundTrdOtd[21 + 1];        // 资金交易在途金额
  char          szFundTrdBln[21 + 1];        // 资金交易轧差金额
  char          szFundBorrowBln[21 + 1];     // 内部拆借扎差
};

//-------------------------------10301055:港股通交易最小价差查询------------------------------------
struct MATRADEAPI CReqStkQryTradeMinSpreadField
{
  char          szStkbd[2 + 1];              // 交易板块
  char          chStkCls;                    // 证券类别
  char          szPrice[21 + 1];             // 价格
};

struct MATRADEAPI CRspStkQryTradeMinSpreadField
{
  char          chStkex;                     // 交易市场
  char          szStkbd[2 + 1];              // 交易板块
  char          chStkCls;                    // 证券类别
  char          szBgnPrice[21 + 1];          // 价格下限
  char          szEndPrice[21 + 1];          // 价格上限
  char          szStkSpread[21 + 1];         // 价差（价位）
  char          szMinBgnPrice[21 + 1];       // 最低价格下限
  char          szMaxEndPrice[21 + 1];       // 最高价格上限
  int           iPriceUnitNum;               // 价差数量
};

//-------------------------------10303024:回购合约查询------------------------------------
struct MATRADEAPI CReqStkQryRepchContractField
{
  LONGLONG      llCuacctCode;                // 资产账户
  LONGLONG      llCustCode;                  // 客户代码
  char          chFlag;                      // 查询标志
  char          szOrderId[10 + 1];           // 合同序号
  int           iRepchDate;                  // 回购日期
  int           iIntOrg;                     // 内部机构
  char          szStkbd[2 + 1];              // 交易板块
  char          szTrdacct[20 + 1];           // 交易账户
  char          szStkCode[8 + 1];            // 证券代码
  char          chDateType;                  // 日期类型
  int           iBgnDate;                    // 开始日期
  int           iEndDate;                    // 结束日期
  char          szQueryPos[32 + 1];          // 定位串
  int           iQueryNum;                   // 查询行数
  char          chQueryFlag;                 // 查询方向
};

struct MATRADEAPI CRspStkQryRepchContractField
{
  int           iRepchDate;                  // 到期日期
  int           iTrdDate;                    // 交易日期
  LONGLONG      llCuacctCode;                // 资产账户
  char          chStkex;                     // 交易市场
  char          szStkbd[2 + 1];              // 交易板块
  char          szOrderId[10 + 1];           // 合同序号
  LONGLONG      llCustCode;                  // 客户代码
  char          chCurrency;                  // 货币代码
  char          szTrdacct[20 + 1];           // 交易账户
  char          szStkCode[8 + 1];            // 证券代码
  char          szStkName[16 + 1];           // 证券名称
  int           iStkBiz;                     // 证券业务
  int           iStkBizAction;               // 业务行为
  char          szOrderPrice[21 + 1];        // 委托价格
  char          szMatchedPrice[21 + 1];      // 成交价格
  LONGLONG      llRepchQty;                  // 回购数量
  char          szRepchAmt[21 + 1];          // 到期金额
  char          szQryPos[32 + 1];            // 定位串
  int           iIntOrg;                     // 内部机构
  char          szCustName[32 + 1];          // 客户姓名
  char          chStkCls;                    // 证券类别
  LONGLONG      llOrderQty;                  // 委托数量
  LONGLONG      llMatchQty;                  // 成交数量
  char          szMatchAmt[21 + 1];          // 成交金额
  char          szStkpbu[8 + 1];             // 交易单元
  char          szMatchId[32 + 1];           // 成交编号
  char          chStkTrdExtcls;              // 交易方式分类
  char          chMpOrdertype;               // 市价委托类型
  char          chRepchStatus;               // 回购状态
  char          chChannel;                   // 操作渠道
  int           iActualUsedDays;             // 实际占用天数
};

//-------------------------------10304112:客户适当性查询------------------------------------
struct MATRADEAPI CReqStkQryCustAppropriateField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
};

struct MATRADEAPI CRspStkQryCustAppropriateField
{
  LONGLONG      llCustCode;                   // 客户代码
  char          chProfInvestorType;           // 专业投资者分类
  int           iProfExpDate;                 // 专业投资者到期日期
  char          chInvestLmt;                  // 投资期限
  char          szInvestPro[32 + 1];          // 投资品种
  char          chExpectIncome;               // 预期收益
  char          chCustLastriskFlag;           // 最低客户风险类别标示
  char          chCustRiskLevel;              // 客户风险等级
  int           iCustRiskLevelDate;           // 风险等级有效期
};

//-------------------------------10301147:产品适当性查询------------------------------------
struct MATRADEAPI CReqStkQryProdAppropriateField
{
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          chStkex;                      // 交易市场
  int           iStkBiz;                      // 证券业务
};

struct MATRADEAPI CRspStkQryProdAppropriateField
{
  char          chStkex;                      // 证券交易所
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  char          chStkRiskLevel;               // 产品风险等级
  char          chInvestPeriod ;              // 投资期限
  char          szInvestType[2 + 1];          // 投资品种
  char          chExincomeType;               // 预期收益
  int           iStkBiz;                      // 证券业务
};

//-------------------------------10301467:适当性匹配检查------------------------------------
struct MATRADEAPI CReqStkAppropriateCheckField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  int           iStkBiz;                      // 证券业务
  char          chRiskNoFlag;                 // 风险记录标志
};

struct MATRADEAPI CRspStkAppropriateCheckField
{
  LONGLONG      llPropRiskNo;                 // 适当性流水号
  LONGLONG      llCustCode;                   // 客户代码
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  int           iStkBiz;                      // 证券业务
  char          chCheckResult;                // 适当性匹配结果
  char          chCustRiskLevel;              // 客户风险等级
  char          chCustInvestLmt;              // 客户投资期限
  char          szCustInvestPro[128 + 1];     // 客户投资品种
  char          chCustExpectIncome;           // 客户预期收益
  char          chStkRiskLevel;               // 产品风险等级
  char          chStkInvestPeriod;            // 产品投资期限
  char          szStkInvestType[2 + 1];       // 产品投资品种
  char          chStkExincomeType;            // 产品预期收益
  char          chRiskLevelFitFlag;           // 风险等级匹配结果
  char          chInvestPeriodFitFlag;        // 投资期限匹配结果
  char          chInvestTypeFitFlag;          // 投资品种匹配结果
  char          chExincomeTypeFitFlag;        // 预期收益匹配结果
  char          szCheckDesc[512 + 1];         // 校验结果描述
  char          chCustLastriskFlag;           // 最低客户风险类别标示
  int           iCustRiskLevelDate;           // 风险等级有效期
};

//-------------------------------10301468:适当性匹配结果记录------------------------------------
struct MATRADEAPI CReqStkAppropriateResultField
{
  LONGLONG      llCustCode;                   // 客户代码
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  int           iStkBiz;                      // 证券业务
  char          szConfirmType[64 + 1];        // 确认类别
  LONGLONG      llPropRiskNo;                 // 适当性流水号
};

struct MATRADEAPI CRspStkAppropriateResultField
{
  LONGLONG      llPropRiskNo;                 // 适当性流水号
};

//-------------------------------10303095:回购资金占用天数查询------------------------------------
struct MATRADEAPI CReqStkQryRepchFundField
{
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  int           iTrdDate;                     // 交易日期
};

struct MATRADEAPI CRspStkQryRepchFundField
{
  int           iBeginUsedDate;               // 开始占用日期
  int           iUsedDate;                    // 占用天数
  int           iRepchDate;                   // 到期日期
};

//-------------------------------10301079:上证LOF信息查询------------------------------------
struct MATRADEAPI CReqStkQryScLofInfoField
{
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkLofCode[8 + 1];          // 上证LOF代码
};

struct MATRADEAPI CRspStkQryScLofInfoField
{
  char          chStkex;                      // 证券交易所
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkLofCode[8 + 1];          // 上证LOF代码
  char          szStkLofName[10 + 1];         // 上证LOF名称
  char          szStkLofStatusSh[8 + 1];      // 上证LOF状态
  int           iStkLofRatio;                 // 基金转换系数
  char          chStkLofType;                 // 基金类型
  char          chStkLofSubType;              // 基金子类型
  LONGLONG      llStkLofPmUnit;               // 拆分合并单位
  LONGLONG      llStkLofMergeFloor;           // 最小合并数
  LONGLONG      llStkLofPartitionFloor;       // 最小拆分数
  char          szStkLofMainCode[8 + 1];      // 母基金代码
  char          szStkLofMainName[10 + 1];     // 母基金名称
  int           iStkLofMainRatio;             // 母份额转换系数
  char          szStkLofMainStatus[8 + 1];    // 母基金状态
};

//-------------------------------10303028:历史委托查询--------------------------
struct MATRADEAPI CReqStkQryHisOrderField
{
  LONGLONG        llCustCode;                 // 客户代码
  int             iIntOrg;                    // 内部机构 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回 (当前支持0，其余值留作扩展用)
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
};

struct MATRADEAPI CRspStkQryHisOrderField
{
  int             iIntOrg;                    // 内部机构
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  char            chOrderValidFlag;           // 委托有效标志 字典[ORDER_VALID_FLAG]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szOrderUfzAmt[21 + 1];      // 委托解冻金额 
  LONGLONG        llOfferQty;                 // 申报数量 
  char            szOfferStime[32 + 1];       // 申报时间 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chIsWithdrawn;              // 已撤单标志 字典[IS_WITHDRAWN]
  int             iOrderBsn;                  // 委托批号 
  char            szQryPos[32 + 1];           // 定位串查询 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
};

//-------------------------------10303029:历史成交查询--------------------------
struct MATRADEAPI CReqStkQryHisFillField
{
  LONGLONG        llCustCode;                 // 客户代码 不能同时为空 当都不为空时按客户代码作为有效条件
  int             iIntOrg;                    // 内部机构 
  char            chQueryFlag;                // 查询方向 ‘0’:向后取数据 ‘1’:向前取数据 其他全部返回 (当前支持0，其余值留作扩展用)
  char            szQueryPos[32 + 1];         // 定位串 
  int             iQueryNum;                  // 查询行数 
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
};

struct MATRADEAPI CRspStkQryHisFillField
{
  char            szMatchedTime[8 + 1];       // 成交时间 
  int             iOrderDate;                 // 委托日期 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szMatchedSn[32 + 1];        // 成交编号 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
  char            szQryPos[32 + 1];           // 定位串查询 
};

//-------------------------------10303019:当日/历史委托查询--------------------------
struct MATRADEAPI CReqStkQryCurrDayHisOrderField
{
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD] 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回 
  char            szQueryPos[32 + 1];         // 定位串 对应为委托表中“QUERY_POS” 
  int             iQueryNum;                  // 查询行数 最大值为1000
  int             iCuacctSn;                  // 账户序号 
  char            chOrderMode;                // 委托模式 0-所有委托，1-可撤委托，2-非法委托，3-合法委托
};

struct MATRADEAPI CRspStkQryCurrDayHisOrderField
{
  char            szQryPos[32 + 1];           // 定位串查询
  int             iIntOrg;                    // 内部机构 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  char            chOrderValidFlag;           // 委托有效标志 字典[ORDER_VALID_FLAG]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBondInt[21 + 1];          // 债券利息 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szOrderUfzAmt[21 + 1];      // 委托解冻金额 
  LONGLONG        llOfferQty;                 // 申报数量 
  int             iOfferStime;                // 申报时间 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chIsWithdrawn;              // 已撤单标志 字典[IS_WITHDRAWN]
  int             iOrderBsn;                  // 委托批号 
  char            szMatchedAmt[21 + 1];       // 成交金额 
  int             iCuacctSn;                  // 账户序号 
  char            szRawOrderId[10 + 1];       // 原合同序号 撤单委托时有效，指被撤单的委托
  char            szOfferRetMsg[64 + 1];      // 申报返回信息 
};

//-------------------------------10303020:当日/历史成交查询--------------------------
struct MATRADEAPI CReqStkQryCurrDayHisFillField
{
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 客户代码与资产账户两项不能同时为空
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  char            chQueryFlag;                // 查询方向 ‘0’:向后取数据 ‘1’:向前取数据 其他全部返回 
  char            szQueryPos[32 + 1];         // 定位串 对应为成交表中“QUERY_POS” 
  int             iQueryNum;                  // 查询行数 最大值为1000
  int             iCuacctSn;                  // 账户序号 
};

struct MATRADEAPI CRspStkQryCurrDayHisFillField
{
  char            szQryPos[32 + 1];           // 定位串查询 
  char            szMatchedTime[8 + 1];       // 成交时间 
  int             iOrderDate;                 // 委托日期 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBondInt[21 + 1];          // 债券利息 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szMatchedSn[32 + 1];        // 成交编号 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  char            szMatchedQty[21 + 1];       // 已成交数量 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chMatchedType;              // 成交类型 ‘0’:未申报撤单成交 ‘1’:非法委托撤单成交 ‘2’:交易所成交
  char            szStkTrdacct[20 + 1];       // 证券账户 
  int             iCuacctSn;                  // 账户序号 
};

//-------------------------------10303008:质押股份明细查询(单客户)--------------------------
struct MATRADEAPI CReqStkQryPledgeSharesField
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

struct MATRADEAPI CRspStkQryPledgeSharesField
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
struct MATRADEAPI CReqStkQryFundFlowField
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

struct MATRADEAPI CRspStkQryFundFlowField
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

//-------------------------------10301105:用户登录------------------------------------
struct MATRADEAPI CReqStkUserLoginField
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

struct MATRADEAPI CRspStkUserLoginField
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
  char          chSzFlashMode;                // 软加速深圳市场加速模式 ‘0’-不加速 ‘1’-软件加速 ‘2’-FPGA加速
  char          chShFlashMode;                // 软加速上海市场加速模式 ‘0’-不加速 ‘1’-软件加速 ‘2’-FPGA加速
  char          chFlashEnableFlag;            // 加速启用标志 ‘0’-未启用 ‘1’-已启用
  char          chTrdacctExcls;               // 交易账户类别 '0' -- 个人账户；'1' -- 机构账户；'2' -- 个人信用账户；'3' -- 机构信用账户
};

//转帐银行业务信息 10360500
struct MATRADEAPI CReqStkBankTransBasicInfoField
{
  char chCurrency;    //货币代码  9080
  char szBankCode[4+1]; //银行代码  15
};
struct MATRADEAPI CRspStkBankTransBasicInfoField
{
  char szBankCode[4+1];        // 银行代码  BANK_CODE VARCHAR(4)  9080
  char chSourceType;           // 发起方    SOURCE_TYPE CHAR(1) 9108
  char chBankTranType;         // 转帐方式  BANK_TRAN_TYPE  CHAR(1) 9109
  char chFundPwdFlag;          // 资金密码校验  FUND_PWD_FLAG CHAR(1) 9081
  char chBankPwdFlag;          // 银行密码校验  BANK_PWD_FLAG CHAR(1) 9082
  char chCheckBankId;          // 银行帐号校验  CHECK_BANK_ID CHAR(1) 9083
  char chCheckIdNo;          // 证件校验标志  CHECK_ID_NO CHAR(1) 9084
  int  iIntOrg;            // 操作机构  INT_ORG SMALLINT  8821
  char chCurrency;           // 货币代码  CURRENCY  CHAR(1) 9085
  char chStatus;             // 银行状态  STATUS  CHAR(1) 9086

};
//银证转帐
struct MATRADEAPI CReqStkBankStkTransField
{
  LONGLONG llCuacctCode;      //资金账号  CUACCT_CODE     SMALLINT  √  8920
  char  chCurrency;       //货币代码  CURRENCY      CHAR(1)   √  15
  char  szFundPwd[32+1];      //资金密码  FUND_PWD      VARCHAR(32) X 9100
  char  szBankCode[4+1];      //银行代码  BANK_CODE     VARCHAR(4)  √  9101
  char  szBankPwd[32+1];      //银行密码  BANK_PWD      CHAR(32)  X 9102
  char  chBankTranType;     //转帐类型  BANK_TRAN_TYPE    CHAR(1)   √  9103
  char  szTransAmt[21+1];     //转帐金额  TRANS_AMT     CMONEY    √  9104
  char  chPwdFlag;          //资金密码校验标志  PWD_FLAG  CHAR(1)   X 9105
  //char szExtsNo[32+1];      //外部流水号  EXTS_NO       VARCHAR(32) X 9106
  //char          szTrdPwd[32 + 1];             // 交易密码2015-11-26添加 2016-1-15 去掉

};
struct MATRADEAPI CRspStkBankStkTransField
{
  int  iSNo;        //委托序号  S_NO      SMALLINT  9107
  int  iSysErrorId;   //错误代码  SYS_ERROR_ID  SMALLINT  9108
  char szErrorMsg[64+1];  //错误信息  ERROR_MSG VAARCHAR(64)  9109

};
//查询账户余额
struct MATRADEAPI CReqStkFundBalaField
{
  LONGLONG      llCuacctCode;                 // 资金账号     8920
  char          chCurrency;         // 货币代码     15
  char          szFundPwd[32 + 1];            // 资金密码     9100
  char          szBankCode[4 + 1];            // 银行代码     9101
  char          szBankPwd[32 + 1];            // 银行密码     9102
  //char          szTrdPwd[32 + 1];             // 交易密码2015-11-26添加 2016-1-15 去掉
};
struct MATRADEAPI CRspStkFundBalaField
{
  int           iSNo;                        // 委托序号      9107
  char          szErrorMsg[64 + 1];           // 错误信息     9108
  int           iSysErrId;                   // 错误代码      9109
  LONGLONG      llFundEffect;                 // 银行余额     9110
};

//银证转帐查询   2015/8/12
struct MATRADEAPI CReqStkBankStkTransInfoField
{
  LONGLONG      llCuacctCode;          // 资金账号     8920
  char          chCurrency;            // 货币代码   15
  char          szSNo[32 + 1];         // 转帐序号   9100
  //char          szTrdPwd[32 + 1];             // 交易密码2015-11-26添加 2016-1-15 去掉

};
struct MATRADEAPI CRspStkBankStkTransInfoField
{
  int           iOperDate;                    // 转帐日期      9100
  int           iOperTime;                    // 转帐时间      9101
  LONGLONG      llCuacctCode;                 // 资金账号      8920
  char          chCurrency;                   // 货币代码      15
  LONGLONG      llCustCode;                   // 客户代码      8902
  char          szBankCode[4 + 1];            // 银行代码      9102
  char          chBankTranId;                 // 业务类型      9103
  int           iSNo;                         // 合同序号      9104
  LONGLONG      llFundEffect;                 // 委托金额      9105
  LONGLONG      llFundBal;                    // 余额        9106
  char          szRemark[32 + 1];             // 备注信息      9107
  char          chStatus;                     // 处理结果      9108
  char          chSourceType;                 // 发起方向      9109
  char          szBankMsgId[16 + 1];          // 外部信息代码    9110
  char          szBankMsg[64 + 1];            // 外部信息内容    9111
  char          szErrorMsg[64 + 1];           // 系统错误信息    9112
  int           iSysErrId;                    // 系统错误代码    9113
  char          szFundEffect[23 + 1];         // 委托金额       9105
  char          szFundBal[23 + 1];            // 余额           9106
};
//修改交易密码   2015/8/12
struct MATRADEAPI CReqStkChgTrdPwdField
{
  char          szNewPwd[32 + 1];             // 新密码 9100
  //char          szOldPwd[32 + 1];             // 老交易密码 2015-11-26添加 2016-1-15 去掉
};
struct MATRADEAPI CRspStkChgTrdPwdField
{
  char          szMsgOk[32 + 1];              // 成功信息 9101
  char          szCustCert[128 + 1];          // 客户证书 9102
};
//修改资金密码   2015/8/12
struct MATRADEAPI CReqStkChgFundPwdField
{
  LONGLONG      llCuacctCode;                 // 资金帐户     8920
  char          szOldFundPwd[32 + 1];         // 老资金密码  9100
  char          szNewFundPwd[32 + 1];         // 新资金密码  9101
  //char          szTrdPwd[32 + 1];             // 交易密码 2015-11-26添加 2016-1-15 去掉

};
struct MATRADEAPI CRspStkChgFundPwdField
{
  char          szMsgOk[32 + 1];              // 成功信息   9101
};
//对帐单查询     2015/8/12
struct MATRADEAPI CReqStkStatementField
{
  int         iStrDate;                    // 起始日期       9100
  int         iEndDate;                    // 终止日期       9101
  LONGLONG     llCuacctCode;                 // 资金帐户       8920
  char          chCurrency;            // 货币代码         15
  char          chPrintFlag;           // 重打标识         9102
  char          chQryFlag;             // 查询方向         9080
  int         iQryNum;                     // 请求行数       8992
  char          szQryPos[32 + 1];             // 定位串       8991
  //char          szTrdPwd[32 + 1];             // 交易密码  2015-11-26添加

};
struct MATRADEAPI CRspStkStatementField
{
  char          szQryPos[32 + 1];             // 定位串       8991
  int         iClearDate;                     // 清算日期      8992
  int         iBizDate;                    // 交收日期       9103
  int         iOrderDate;                  // 发生日期       9104
  int         iOrderTime;                  // 发生时间       9105
  int         iDigestId;                   // 业务代码       9106
  char          szDigestName[16 + 1];         // 业务说明      9107
  LONGLONG     llCustId;                     // 客户代码       8902
  char          szCustName[16 + 1];           // 客户姓名      8910
  char          szIntOrg[4 + 1];              // 机构代码      8911
  LONGLONG     llCuacctCode;                 // 资金帐号       8920
  char          chCurrency;            // 货币代码         15
  char          chStkex;               // 市场代码         207
  char          szTrdacct[32 + 1];            // 股东代码      8927
  LONGLONG     llFundEffect;                 // 资金发生数      9108
  LONGLONG     llFundBal;                    // 资金本次余额     9109
  int         iStkBal;                     // 股份本次余额       9110
  char          szOrderId[10 + 1];            // 合同序号      11
  char          szStkCode[16 + 1];            // 证券代码      48
  char          szStkName[16 + 1];            // 证券名称      55
  char          szBsFlag[2 + 1];              // 买卖类别      9111
  int         iMatchedQty;                 // 成交数量       387
  LONGLONG     llMatchedPrice;               // 成交价格       8841
  LONGLONG     llMatchedAmt;                 // 成交金额       8504

};

//资金划拨
struct MATRADEAPI CReqStkFundTransferField
{
  char         szOrgId[4 + 1];               // 机构编码
  LONGLONG     llCuacctCode;                 // 资金账号
  char         chCurrency;                   // 货币代码
  char         szFundAvl[21 + 1];            // 资金可用
  char         chDirect;                     // 划拨方向
};
struct MATRADEAPI CRspStkFundTransferField
{
  int          iSNo;                         // 操作流水号
  int          iVipSno;                      // VIP操作流水号
};
//-------------------------------10101509:历史委托明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcHistoryOrderDetailField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    int           iOrdergroup;       // 委托批号
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
    char          szExtsno[32 + 1];  // 外部流水号
    char          chQryoperway;      // 委托渠道
};
struct MATRADEAPI CRspStkQryRpcHistoryOrderDetailField
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
    int           iOrdergroup;           // 委托批号
    int           iReporttime;           // 报盘时间
    int           iOpertime;             // 委托时间
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 证券名称
    char          szStkname[8 + 1];      // 证券代码
    char          szProdcode[12 + 1];    // 产品编码
    char          szOrderprice[21 + 1];  // 委托价格
    int           iOrderqty;             // 委托数量
    char          szOrderfrzamt[21 + 1]; // 冻结金额
    int           iMatchqty;             // 成交数量
    char          szMatchamt[21 + 1];    // 成交金额
    int           iCancelqty;            // 撤单数量
    char          chCancelflag;          // 撤单标志
    char          chOrderstatus;         // 委托状态
    char          szSeat[6 + 1];         // 交易席位
    char          szMatchprice[21 + 1];  // 成交价格
    int           iOperdate;             // 操作日期
    char          szBondintr[21 + 1];    // 债券应计利息
    char          chOperway;             // 委托渠道
    char          szRemark[64 + 1];      // 备注信息
};
//-------------------------------10101510:历史成交明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcHistoryMatchDetailField
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
struct MATRADEAPI CRspStkQryRpcHistoryMatchDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iBizdate;             // 交收日期
    int           iCleardate;           // 成交日期
    char          szSecuid[10 + 1];     // 股东代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szOrderid[10 + 1];    // 申报合同序号
    int           iOrdersno;            // 委托序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    char          szProdcode[12 + 1];   // 产品编码
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    int           iStkbal;              // 股份本次余额
    char          szFeeJsxf[21 + 1];    // 净手续费
    char          szFeeSxf[21 + 1];     // 手续费
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeGhf[21 + 1];     // 过户费
    char          szFeeQsf[21 + 1];     // 清算费
    char          szFeeJygf[21 + 1];    // 交易规费
    char          szFeeJsf[21 + 1];     // 经手费
    char          szFeeZgf[21 + 1];     // 证管费
    char          szFeeQtf[21 + 1];     // 其他费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFundeffect[21 + 1]; // 资金发生数
    char          szFundbal[21 + 1];    // 使用金额
    char          szBondintr[21 + 1];   // 债券应计利息
};
//-------------------------------10101511:当日新股清单查询--------------------------
struct MATRADEAPI CReqStkQryRpcIPOField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 申购代码
    int           iIssuedate;       // 申购日期
};
struct MATRADEAPI CRspStkQryRpcIPOField
{
    char          chMarket;           // 交易市场
    char          szStkcode[8 + 1];   // 申购代码
    char          szStkname[16 + 1];  // 申购代码名称
    char          szLinkstk[8 + 1];   // 正股代码
    char          chIssuetype;        // 发行方式
    int           iIssuedate;         // 申购日期
    char          szFixprice[21 + 1]; // 申购价格
    int           iBuyunit;           // 申购单位
    int           iMinqty;            // 申购最小数量
    int           iMaxqty;            // 申购最大数量
};
//-------------------------------10101512:外围新股申购市值额度表查询--------------------------
struct MATRADEAPI CReqStkQryRpcIPOSubscriptionLimitField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szOrgid[4 + 1];   // 机构编码
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcIPOSubscriptionLimitField
{
    char          szPosstr[64 + 1]; // 定位串
    int           iServerid;        // 节点编号
    int           iDbfrec;          // 序号
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 机构编码
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llCustquota;      // 客户市值额度
    LONGLONG      llKcCustquota;    // 科创板申购额度
    int           iReceivedate;     // 接收日期
    char          szRemark[32 + 1]; // 备注
};
//-------------------------------10101513:配号查询--------------------------
struct MATRADEAPI CReqStkQryRpcDistributionField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szSecuid[10 + 1];  // 股东代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcDistributionField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iBizdate;              // 配号日期
    char          chMarket;              // 交易市场
    char          szSecuid[32 + 1];      // 股东代码
    char          szStkname[16 + 1];     // 证券名称
    char          szStkcode[16 + 1];     // 证券代码
    int           iMatchqty;             // 配号数量
    char          szMateno[32 + 1];      // 申购配号
    char          szOrderid[10 + 1];     // 委托号
    int           iOrderdate;            // 委托日期
    char          szMateerrmsg[128 + 1]; // 备注
};
//-------------------------------10101514:市值配售中签查询--------------------------
struct MATRADEAPI CReqStkQryRpcMktCapPlacingField
{
    char          szSecuid[10 + 1]; // 股东代码
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          chIssuetype;      // 发行方式
};
struct MATRADEAPI CRspStkQryRpcMktCapPlacingField
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
    char          szFrzamt[21 + 1];     // 预冻结金额
    int           iClearsno;            // 交收顺序
    char          chIssuetype;          // 发行方式
    char          chStatus;             // 流程状态
};
//-------------------------------10101515:中签历史查询--------------------------
struct MATRADEAPI CReqStkQryRpcWinningHistoryField
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
struct MATRADEAPI CRspStkQryRpcWinningHistoryField
{
    char          szPoststr[32 + 1];    // 定位串
    LONGLONG      llCustid;             // 客户代码
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    int           iOrderdate;           // 委托日期
    int           iOrdersno;            // 委托序号
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
//-------------------------------10101516:市值配售交收顺序修改--------------------------
struct MATRADEAPI CReqStkRpcChgMktCapDistDeliOrderField
{
    int           iMatchdate;       // 中签日期
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    int           iClearsno;        // 交收顺序
};
struct MATRADEAPI CRspStkRpcChgMktCapDistDeliOrderField
{
    int           iSerialno; // 操作流水号
};
//-------------------------------10101517:交割单查询--------------------------
struct MATRADEAPI CReqStkQryRpcDeliOrderField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcDeliOrderField
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
    char          szFeeOneYhs[21 + 1];  // 一级印花税
    char          szFeeOneGhf[21 + 1];  // 一级过户费
    char          szFeeOneQsf[21 + 1];  // 一级清算费
    char          szFeeOneJygf[21 + 1]; // 一级交易规费
    char          szFeeOneJsf[21 + 1];  // 一级经手费
    char          szFeeOneZgf[21 + 1];  // 一级证管费
    char          szFeeOneQtf[21 + 1];  // 一级其他费
    char          szFeeOneFxj[21 + 1];  // 一级风险金
};
//-------------------------------10101518:对帐查询--------------------------
struct MATRADEAPI CReqStkQryRpcChkAcctField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcChkAcctField
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
    char          szFeeJsxf[21 + 1];    // 净佣金
    char          szFeeSxf[21 + 1];     // 佣金
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeGhf[21 + 1];     // 过户费
    char          szFeeQsf[21 + 1];     // 清算费
    char          szFeeJygf[21 + 1];    // 交易规费
    char          szFeeJsf[21 + 1];     // 经手费
    char          szFeeZgf[21 + 1];     // 证管费
    char          szFeeQtf[21 + 1];     // 其他费
    char          szFeefront[21 + 1];   // 前台费用
    char          szBankcode[16 + 1];   // 银行代码
    LONGLONG      llStkeffect;          // 股份发生数
    char          szMatchcode[32 + 1];  // 成交编号
};
//-------------------------------10101519:获取开通转帐(银行)账号查询--------------------------
struct MATRADEAPI CReqStkQryRpcBankAcctField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chMoneytype;       // 货币代码
    LONGLONG      llFundid;          // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcBankAcctField
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
//-------------------------------10101520:获取开通外部(银行)账号查询--------------------------
struct MATRADEAPI CReqStkQryRpcOtherBankAcctField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chMoneytype;       // 货币代码
    LONGLONG      llFundid;          // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcOtherBankAcctField
{
    LONGLONG      llCustid;            // 客户代码
    char          szOrgid[4 + 1];      // 机构编码
    char          szBankcode[4 + 1];   // 银行代码
    char          szBankid[32 + 1];    // 银行帐户
    char          szSubbankid[32 + 1]; // 银行子帐户
    LONGLONG      llFundid;            // 资金帐号
};
//-------------------------------10101521:资金查询--------------------------
struct MATRADEAPI CReqStkQryRpcFundField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspStkQryRpcFundField
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
    char          szFundloan[21 + 1];    // 融资总金额
    char          szFundbuy[21 + 1];     // 买入冻结
    char          szFundsale[21 + 1];    // 卖出解冻
    char          szFundfrz[21 + 1];     // 冻结总金额
    char          szFundlastbal[21 + 1]; // 昨日余额
};
//-------------------------------10101522:当日资金流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcFundDetailField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcFundDetailField
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
//-------------------------------10101523:当日委托汇总(410514)--------------------------
struct MATRADEAPI CReqStkQryRpcOrderSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspStkQryRpcOrderSummaryField
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
//-------------------------------10101524:当日成交汇总(410516)--------------------------
struct MATRADEAPI CReqStkQryRpcMatchSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspStkQryRpcMatchSummaryField
{
    int           iOperdate;            // 交易日期
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
    char          chMatchtype;          // 成交类型
    char          szBankcode[4 + 1];    // 外部银行
    char          szBankid[32 + 1];     // 外部账户
};

//-------------------------------10101525:查询当前系统状态--------------------------
struct MATRADEAPI CReqStkQryRpcCurSysStatField
{
};
struct MATRADEAPI CRspStkQryRpcCurSysStatField
{
    int           iSysdate;              // 系统日期
    int           iOrderdate;            // 委托日期
    char          chStatus;              // 系统运行状态
    char          szNightremark[32 + 1]; // 夜市状态注释
    int           iSystime;              // 物理时间
};
//-------------------------------10101526:取字典信息--------------------------
struct MATRADEAPI CReqStkRpcDictInfoField
{
    char          szDictitem[8 + 1]; // 字典条目
    char          szSubitem[4 + 1];  // 字典子项
};
struct MATRADEAPI CRspStkRpcDictInfoField
{
    char          szOrgid[4 + 1];        // 机构代码
    char          szDictitem[8 + 1];     // 字典条目
    char          szSubitem[4 + 1];      // 字典子项
    char          szSubitemname[32 + 1]; // 子项名称
    char          chDispflag;            // 显示标识
    char          chAddenable;           // 字典可否增加子项
    char          szItemname[32 + 1];    // 字典条目名称
};
//-------------------------------10101527:证券信息--------------------------
struct MATRADEAPI CReqStkRpcStkInfoField
{
    char          chMarket;          // 交易市场
    char          chStklevel;        // 证券级别
    char          szStkcode[8 + 1];  // 证券代码
    char          szPoststr[32 + 1]; // 定位串
    int           iRowcount;         // 查询行数
    char          chStktype;         // 证券类别
};
struct MATRADEAPI CRspStkRpcStkInfoField
{
    char          szPoststr[32 + 1];      // 定位串
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
    char          chStkstatus;            // 证券状态
    char          chStklevel;             // 证券级别
    char          chTrdid;                // 交易类型
    int           iQuitdate;              // 退市证券交易截止日期
    char          szFixprice[21 + 1];     // 系统定价
    char          chPriceflag;            // 委托价格标志
    char          szMemotext[128 + 1];    // 退市证券提示信息
};
//-------------------------------10101528:查询机构信息--------------------------
struct MATRADEAPI CReqStkQryRpcOrgInfoField
{
    char          szQryorgid[4 + 1]; // 机构编码
};
struct MATRADEAPI CRspStkQryRpcOrgInfoField
{
    char          szOrgid[4 + 1];    // 机构编码
    char          szOrgname[32 + 1]; // 机构简称
};
//-------------------------------10101529:银证通转帐银行业务信息--------------------------
struct MATRADEAPI CReqStkRpcBankStkTransferBankBizInfoField
{
    char          szBankcode[4 + 1]; // 银行代码
};
struct MATRADEAPI CRspStkRpcBankStkTransferBankBizInfoField
{
    char          szBankcode[4 + 1]; // 银行代码
    int           iBanktrdid;        // 银行交易类型
    char          chTrdpwdflag;      // 交易密码校验
    char          chCtrlflag;        // 证件校验标志
    char          chStatus;          // 银行状态
};
//-------------------------------10101530:用户登陆--------------------------
struct MATRADEAPI CReqStkRpcLoginField
{
    char          chInputtype;                // 登陆类型
    char          szInputid[64 + 1];          // 登陆标识
    char          szProcName[128 + 1];        // 第三方厂商名称
    char          szCert[4096 + 1];           // 证书
    char          chCerttype;                 // 证书类型
    char          chUserrole;                 // 用户角色
    char          szRandcode[32 + 1];         // 随机码
    char          szSignedrandcode[4096 + 1]; // 签名后内容
    char          szEtokenpin[33 + 1];        // 动态口令pin码
    char          szDynpwd[33 + 1];           // 动态口令
    char          szLoginsite[64 + 1];        // 委托站点
    char          szLoginip[64 + 1];          // 登录机器IP
    char          szMac[32 + 1];              // 登录机器MAC地址
    char          szCpusn[32 + 1];            // 登录机器cpu序列号
    char          szHddsn[32 + 1];            // 登录机器硬盘序列号
    char          chCheckauthflag;            // 安全认证检查标志
};
struct MATRADEAPI CRspStkRpcLoginField
{
    char          chCustprop;             // 客户性质
    char          chMarket;               // 交易市场
    char          szSecuid[10 + 1];       // 股东代码
    char          szName[16 + 1];         // 股东姓名
    LONGLONG      llFundid;               // 缺省资金帐户
    LONGLONG      llCustid;               // 客户代码
    char          szCustname[16 + 1];     // 客户姓名
    char          szOrgid[4 + 1];         // 机构编码
    char          szBankcode[4 + 1];      // 银行代码
    char          szIdentitysign[64 + 1]; // 数字签名
    char          chTimeoutflag;          // 延时属性
    char          chAuthlevel;            // 认证方式/级别
    int           iPwderrtimes;           // 登陆错误次数
    char          chSingleflag;           // 客户标志
    char          chCheckpwdflag;         // 密码有效标志
    char          szCustcert[128 + 1];    // 客户证书
    char          szTokenlen[8 + 1];      // 登录时输入的动态令牌长度
    char          szLastlogindate[8 + 1]; // 最近登录日期
    char          szLastlogintime[8 + 1]; // 最近登录时间
    char          szLastloginip[64 + 1];  // 最近登录IP
    char          szLastloginmac[32 + 1]; // 最近登录MAC
    char          chInputtype;            // 登陆类型
    char          szInputid[64 + 1];      // 登陆标识
    char          szTokenenddate[8 + 1];  // 客户动态令牌结束日期
    char          chBindflag;             // 硬件绑定信息标识
};
//-------------------------------10101531:客户总资产查询(新)--------------------------
struct MATRADEAPI CReqStkQryRpcTotalAssetsField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户帐号
    char          chMoneytype;      // 货币代码
    char          chAssetcategory;  // 资产类别
};
struct MATRADEAPI CRspStkQryRpcTotalAssetsField
{
    LONGLONG      llFundid;               // 资金帐号
    char          chMoneytype;            // 货币代码
    char          szSpecies[4 + 1];       // 资产科目代码
    char          szSpeciesname[32 + 1];  // 资产科目名称
    char          chAssetcategory;        // 资产类别
    char          szCategoryname[32 + 1]; // 资产类别名称
    char          szAsset[21 + 1];        // 资 产
};
//-------------------------------10101532:修改联系方式（网上交易使用）--------------------------
struct MATRADEAPI CReqStkRpcChgContactField
{
    char          chIdtype;           // 证件类型
    char          szIdno[32 + 1];     // 证件号码
    char          szAddr[64 + 1];     // 地址
    char          szPostid[16 + 1];   // 邮编
    char          szTelno[32 + 1];    // 电话
    char          szMobileno[32 + 1]; // 移动电话
    char          szEmail[32 + 1];    // EMAIL
    char          chContact;          // 联络方式
    char          chContactfrep;      // 联络频率
    char          chEdu;              // 学历
    char          chOcctype;          // 职业
};
struct MATRADEAPI CRspStkRpcChgContactField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101533:客户资料查询--------------------------
struct MATRADEAPI CReqStkQryRpcCustInfoField
{
};
struct MATRADEAPI CRspStkQryRpcCustInfoField
{
    LONGLONG      llCustid;                // 客户代码
    char          szCustname[16 + 1];      // 客户姓名
    char          szOrgid[4 + 1];          // 机构编码
    char          chSex;                   // 性别
    char          szAddr[64 + 1];          // 地址
    char          szPostid[16 + 1];        // 邮编
    char          szTelno[32 + 1];         // 电话
    char          szMobileno[32 + 1];      // 移动电话
    char          szEmail[32 + 1];         // EMAIL
    int           iOpendate;               // 开户日期
    char          chContact;               // 联络方式
    char          chContactfrep;           // 联络频率
    char          szRemark[32 + 1];        // 备注信息
    char          chIdtype;                // 证件类型
    char          szIdno[32 + 1];          // 证件号码
    int           iIdbegindate;            // 证件有效开始日期
    int           iIdenddate;              // 证件有效结束日期
    int           iYearchkdate;            // 年检日期
    int           iClosedate;              // 销户日期
    char          szPoliceorg[64 + 1];     // 发证单位
    char          chEdu;                   // 学历
    char          chOcctype;               // 职业
    char          szContractverno[32 + 1]; // 开户合同版本号
    char          szExtprop[32 + 1];       // 扩展属性
    int           iPwderrtimes;            // 客户密码校验错误次数
    int           iTimeout;                // 登陆错误延时
    char          chLockflag;              // 密码出错锁定
};
//-------------------------------10101534:修改操作方式--------------------------
struct MATRADEAPI CReqStkRpcChgOperModeField
{
    LONGLONG      llFundid;              // 资金账户
    char          szNewoperway[128 + 1]; // 操作方式
};
struct MATRADEAPI CRspStkRpcChgOperModeField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101535:查询客户开通的操作方式--------------------------
struct MATRADEAPI CReqStkQryRpcCustOperModeField
{
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcCustOperModeField
{
    LONGLONG      llFundid;           // 资金帐户
    char          szName[16 + 1];     // 资金姓名
    char          szOperway[128 + 1]; // 操作方式
};
//-------------------------------10101536:修改客户联系人信息--------------------------
struct MATRADEAPI CReqStkRpcChgCustInfoField
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
struct MATRADEAPI CRspStkRpcChgCustInfoField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101537:设置客户的扩展属性--------------------------
struct MATRADEAPI CReqStkRpcSetCustAttrExField
{
    char          szExtprop[32 + 1]; // 客户扩展属性
};
struct MATRADEAPI CRspStkRpcSetCustAttrExField
{
};
//-------------------------------10101538:设置数字签名--------------------------
struct MATRADEAPI CReqStkRpcSetDigitalSignField
{
    char          szIdentitysign[64 + 1]; // 数字签字
};
struct MATRADEAPI CRspStkRpcSetDigitalSignField
{
};
//-------------------------------10101539:外围修改资金帐号权限--------------------------
struct MATRADEAPI CReqStkRpcChgCuacctPermissionField
{
    LONGLONG      llFundid;          // 资金账户
    char          szRightid[2 + 1];  // 权限
    char          chMarket;          // 市场
    char          szSecuid[10 + 1];  // 证券账号
    char          szRemark[128 + 1]; // 备注
    char          chAction;          // 操作类型
};
struct MATRADEAPI CRspStkRpcChgCuacctPermissionField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101540:资金帐号开通资金权限查询--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctPermissionField
{
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcCuacctPermissionField
{
    char          szOrgid[4 + 1];       // 营业部代码
    LONGLONG      llFundid;             // 资金帐户
    char          szFundname[16 + 1];   // 资金姓名
    char          szFundright[512 + 1]; // 资金权限
};
//-------------------------------10101541:证券帐号开通权限查询--------------------------
struct MATRADEAPI CReqStkQryRpcTrdacctPermissionField
{
    char          szSecuid[10 + 1]; // 证券账号
    char          chMarket;         // 市场
};
struct MATRADEAPI CRspStkQryRpcTrdacctPermissionField
{
    char          chMarket;             // 市场
    char          szSecuid[10 + 1];     // 证券账号
    char          szSecuright[512 + 1]; // 证券权限
};
//-------------------------------10101542:客户权限有效期信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcCustPermissionValidPeriodField
{
    LONGLONG      llFundid;           // 资金账户
    char          chMarket;           // 市场
    char          szSecuid[10 + 1];   // 证券账号
    char          szRightid[512 + 1]; // 权限
};
struct MATRADEAPI CRspStkQryRpcCustPermissionValidPeriodField
{
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 营业部代码
    LONGLONG      llFundid;         // 资金帐户
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 证券账号
    char          szRightid[2 + 1]; // 权限
    int           iEffectdate;      // 生效日期
    int           iExpiredate;      // 失效日期
    char          szRemark[64 + 1]; // 备注
};
//-------------------------------10101543:外围激活客户权限--------------------------
struct MATRADEAPI CReqStkRpcActivateCustPermissionField
{
    LONGLONG      llFundid;          // 资金账户
    char          szRightid[2 + 1];  // 权限
    char          chMarket;          // 市场
    char          szSecuid[10 + 1];  // 证券账号
    int           iExpiredate;       // 失效日期
    char          szRemark[128 + 1]; // 备注
};
struct MATRADEAPI CRspStkRpcActivateCustPermissionField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101544:客户特殊手续费查询--------------------------
struct MATRADEAPI CReqStkQryRpcCustSpecialFeeField
{
    LONGLONG      llFundid;             // 资产账户
    char          szMarket[32 + 1];     // 市场代码
    char          szStktype[128 + 1];   // 证券类别
    char          szTrdid[128 + 1];     // 交易类型
    char          szMoneytype[128 + 1]; // 货币类型
    char          szBsflag[128 + 1];    // 买卖类别
};
struct MATRADEAPI CRspStkQryRpcCustSpecialFeeField
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
//-------------------------------10101545:一次性服务佣金收取--------------------------
struct MATRADEAPI CReqStkRpcOneTimeServiceCommissionField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llFundid;         // 资金账户
    char          chMoneytype;      // 币种
    int           iFeeno;           // 服务佣金编号
};
struct MATRADEAPI CRspStkRpcOneTimeServiceCommissionField
{
    char          szFee[21 + 1]; // 冻结金额
    int           iSno;          // 操作流水号
};
//-------------------------------10101546:一次性服务佣金撤单--------------------------
struct MATRADEAPI CReqStkRpcCancelOneTimeServiceCommissionField
{
    LONGLONG      llFundid;         // 资金账户
    char          chMoneytype;      // 币种
    int           iFeeno;           // 服务佣金编号
};
struct MATRADEAPI CRspStkRpcCancelOneTimeServiceCommissionField
{
    char          szReturnamt[21 + 1]; // 退款金额
    int           iSno;                // 操作流水号
};
//-------------------------------10101547:固定期限服务佣金办理--------------------------
struct MATRADEAPI CReqStkRpcFixedTermServiceCommissionField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llFundid;         // 资金账户
    int           iFeeno;           // 服务佣金编号
    char          chEffectmode;     // 生效方式
    char          chContinueflag;   // 自动续费模式
    int           iContinuetime;    // 自动续费次数
};
struct MATRADEAPI CRspStkRpcFixedTermServiceCommissionField
{
    char          szFreezefund[21 + 1]; // 冻结金额
    int           iEffectdate;          // 生效日期
    int           iSno;                 // 操作流水号
};
//-------------------------------10101548:固定期限服务佣金退款--------------------------
struct MATRADEAPI CReqStkRpcFixedTermServiceCommissionRefundField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llFundid;         // 资金账户
    int           iFeeno;           // 服务佣金编号
    char          chReturnmode;     // 退款方式
};
struct MATRADEAPI CRspStkRpcFixedTermServiceCommissionRefundField
{
    char          szReturnamt[21 + 1]; // 退款金额
    int           iSno;                // 操作流水号
};
//-------------------------------10101549:一次性服务佣金设置查询--------------------------
struct MATRADEAPI CReqStkQryRpcOneTimeServiceCommissionSettingField
{
    char          chMoneytype;      // 币种
};
struct MATRADEAPI CRspStkQryRpcOneTimeServiceCommissionSettingField
{
    char          szOrgid[4 + 1]; // 机构编码
    int           iFeeno;         // 服务佣金编号
    char          chMoneytype;    // 币种
    char          chFeetype;      // 服务佣金类型
    char          szFee[21 + 1];  // 费用
};
//-------------------------------10101550:一次性服务佣金收取查询--------------------------
struct MATRADEAPI CReqStkQryRpcOneTimeServiceCommissionsField
{
    LONGLONG      llFundid;         // 资金账户
};
struct MATRADEAPI CRspStkQryRpcOneTimeServiceCommissionsField
{
    int           iFeeno;        // 服务佣金编号
    char          chFeetype;     // 服务佣金类型
    char          chMoneytype;   // 币种
    char          szFee[21 + 1]; // 收取金额
    int           iTrddate;      // 交易日期
};
//-------------------------------10101551:查询咨询付费类别--------------------------
struct MATRADEAPI CReqStkQryRpcConsultationPaymentCategoryField
{
    char          szProccode[4 + 1]; // 产品编号
};
struct MATRADEAPI CRspStkQryRpcConsultationPaymentCategoryField
{
    char          szProccode[4 + 1];  // 产品编号
    char          szProcname[32 + 1]; // 产品名称
    char          chProducttype;      // 产品分类
    char          szFdOrgid[255 + 1]; // 禁止营业部
};
//-------------------------------10101552:增加咨询付费类别--------------------------
struct MATRADEAPI CReqStkRpcAddConsultationPaymentCategoryField
{
    char          szProccode[4 + 1];  // 产品编号
    char          szProcname[32 + 1]; // 产品名称
    char          chProducttype;      // 产品分类
    char          szFdOrgid[4 + 1];   // 禁止营业部
    char          szRemark[64 + 1];   // 备注信息
};
struct MATRADEAPI CRspStkRpcAddConsultationPaymentCategoryField
{
};
//-------------------------------10101553:维护客户订阅状态--------------------------
struct MATRADEAPI CReqStkRpcMaintainCustSubStatusField
{
    int           iOperdate;        // 发生日期
    int           iStrsno;          // 起始流水号
    int           iEndsno;          // 结束流水号
    LONGLONG      llFundid;         // 资金帐户
    char          chBusitype;       // 业务种类
    char          chCheckflag;      // 处理标志
};
struct MATRADEAPI CRspStkRpcMaintainCustSubStatusField
{
};
//-------------------------------10101554:查询客户订阅流水--------------------------
struct MATRADEAPI CReqStkQryRpcCustSubStatementField
{
    int           iStrdate;           // 开始日期
    int           iEnddate;           // 结束日期
    char          szProductid[4 + 1]; // 产品编号
    LONGLONG      llFundid;           // 资金帐户
    char          chCheckflag;        // 处理标志
    char          chQryflag;          // 查询方向
    int           iCount;             // 请求行数
    char          szPoststr[32 + 1];  // 定位串
};
struct MATRADEAPI CRspStkQryRpcCustSubStatementField
{
    char          szPoststr[32 + 1];      // 定位串
    int           iOperdate;              // 操作日期
    int           iSno;                   // 系统流水号
    char          szProductid[4 + 1];     // 产品编号
    char          szProcductname[32 + 1]; // 产品名称
    char          szOrgid[4 + 1];         // 机构编号
    char          szBrhid[4 + 1];         // 机构分支
    int           iOpertime;              // 发生时间
    LONGLONG      llCustid;               // 客户代码
    char          szName[16 + 1];         // 中文姓名
    LONGLONG      llFundid;               // 资金帐号
    char          chMoneytype;            // 货币代码
    char          chBusitype;             // 业务种类
    char          chCheckflag;            // 处理标志
    char          szNetaddr[12 + 1];      // 操作站点
    char          szOperorg[4 + 1];       // 操作营业部
    char          szRemark[64 + 1];       // 备注信息
};
//-------------------------------10101555:客户订阅设置--------------------------
struct MATRADEAPI CReqStkRpcCustSubSettingField
{
    char          szProductid[4 + 1];     // 产品编号
    char          szProcductname[32 + 1]; // 产品名称
    char          szBrhid[4 + 1];         // 机构分支
    LONGLONG      llFundid;               // 资金帐号
    char          chMoneytype;            // 货币代码
    char          chBusitype;             // 业务种类
};
struct MATRADEAPI CRspStkRpcCustSubSettingField
{
};
//-------------------------------10101556:第三方收费结构--------------------------
struct MATRADEAPI CReqStkRpcThirdPartyChargingStructureField
{
    LONGLONG      llFundid;           // 资金账户
    char          chMoneytype;        // 货币代码
    char          chFeetype;          // 费用标识
    char          szOtherfee[21 + 1]; // 三方费用
};
struct MATRADEAPI CRspStkRpcThirdPartyChargingStructureField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101557:投资咨询产品费用保证金支付--------------------------
struct MATRADEAPI CReqStkRpcInvestmentAdvisoryProductFeeField
{
    LONGLONG      llFundid;                // 资金账户
    char          szFundpwd[32 + 1];       // 资金密码
    char          chMoneytype;             // 货币代码
    char          szDepositpayfee[21 + 1]; // 支付金额
    char          szOrderid[10 + 1];       // 订单序号
    char          szRemark[64 + 1];        // 备注信息
};
struct MATRADEAPI CRspStkRpcInvestmentAdvisoryProductFeeField
{
    int           iSno;              // 操作流水号
    char          szFundavl[21 + 1]; // 资金可用
};
//-------------------------------10101558:密码校验--------------------------
struct MATRADEAPI CReqStkRpcCuacctPwdCheckField
{
    LONGLONG      llFundid;          // 资金账户
    char          szFundpwd[64 + 1]; // 资金密码
};
struct MATRADEAPI CRspStkRpcCuacctPwdCheckField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101559:资金资料查询--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctInfoField
{
    LONGLONG      llFundid;          // 资金账户
    char          szFundpwd[64 + 1]; // 资金密码
    char          chMoneytype;       // 货币代码
};
struct MATRADEAPI CRspStkQryRpcCuacctInfoField
{
    LONGLONG      llFundid;           // 资金帐户
    char          szOrgid[4 + 1];     // 机构代码
    char          szFundname[16 + 1]; // 资金姓名
    char          chIdtype;           // 证件类型
    char          szIdno[32 + 1];     // 证件号码
    int           iOpendate;          // 开户日期
    char          chStatus;           // 资金状态
    char          chMoneytype;        // 货币代码
    char          szFundbal[21 + 1];  // 资金余额
    char          szFundavl[21 + 1];  // 资金可用
};
//-------------------------------10101560:获取客户资料核查状态--------------------------
struct MATRADEAPI CReqStkRpcCustInfoCheckField
{
    LONGLONG      llCustid;         // 客户代码
};
struct MATRADEAPI CRspStkRpcCustInfoCheckField
{
    char          chVerifiedflag; // 客户资料核查标识
    int           iVerifieddate;  // 客户资料核查日期
};
//-------------------------------10101561:设置客户登录校验信息--------------------------
struct MATRADEAPI CReqStkRpcSetLoginCheckModeField
{
    char          chLockflag;       // 密码锁定
    int           iPwderrtimes;     // 客户密码校验错误次数
    int           iTimeout;         // 登陆错误延时
};
struct MATRADEAPI CRspStkRpcSetLoginCheckModeField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101562:设置客户资料核查标识--------------------------
struct MATRADEAPI CReqStkRpcSetCustInfoCheckModeField
{
    LONGLONG      llCustid;         // 客户代码
    char          chVerifiedflag;   // 客户核查标识
};
struct MATRADEAPI CRspStkRpcSetCustInfoCheckModeField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101563:设置客户消息状态--------------------------
struct MATRADEAPI CReqStkRpcSetCustMsgStatusField
{
    LONGLONG      llCustid;         // 客户代码
    int           iNewsid;          // 消息号
    char          chSendflag;       // 发送标志
};
struct MATRADEAPI CRspStkRpcSetCustMsgStatusField
{
};
//-------------------------------10101564:查询客户消息服务--------------------------
struct MATRADEAPI CReqStkQryRpcCustMsgServiceField
{
    LONGLONG      llCustid;         // 客户代码
    int           iNewsid;          // 消息号
};
struct MATRADEAPI CRspStkQryRpcCustMsgServiceField
{
    char          szOrgid[4 + 1];       // 机构编码
    char          szBrhid[4 + 1];       // 分支机构
    LONGLONG      llCustid;             // 客户代码
    int           iNewsid;              // 消息号
    char          szNewsname[32 + 1];   // 消息名称
    char          szNewstext[1024 + 1]; // 消息内容
    char          szRemark[255 + 1];    // 备注
};
//-------------------------------10101565:修改银行密码--------------------------
struct MATRADEAPI CReqStkRpcChgBankPwdField
{
    LONGLONG      llFundid;             // 资金账号
    char          chMoneytype;          // 货币代码
    char          szFundpwd[32 + 1];    // 资金密码
    char          szBankcode[4 + 1];    // 银行代码
    char          szBankpwd[32 + 1];    // 银行密码
    char          szNewbankpwd[32 + 1]; // 新银行密码
};
struct MATRADEAPI CRspStkRpcChgBankPwdField
{
    int           iSno;             // 委托序号
    char          chStatus;         // 处理结果
    char          szErrmsg[64 + 1]; // 错误信息
};
//-------------------------------10101566:资金台帐间转帐--------------------------
struct MATRADEAPI CReqStkRpcTransferBetweenCuacctField
{
    char          szOrgid[4 + 1];        // 机构代码
    LONGLONG      llOutfundid;           // 转出资金帐号
    char          chCheckpwdflag;        // 检查密码标志
    char          szOutpassword[32 + 1]; // 转出资金密码
    LONGLONG      llInfundid;            // 转入资金帐号
    char          chMoneytype;           // 货币代码
    char          szFundeffect[21 + 1];  // 转帐金额
    char          chCashcheck;           // 现金支票类型
};
struct MATRADEAPI CRspStkRpcTransferBetweenCuacctField
{
    int           iOutsno; // 转出流水号
    int           iInsno;  // 转入流水号
};
//-------------------------------10101567:外围客户交易帐户资金归集--------------------------
struct MATRADEAPI CReqStkRpcCuacctCashConcentrationField
{
    char          szOrgid[4 + 1];   // 机构代码
    LONGLONG      llFundid;         // 资金帐号
    char          chMoneytype;      // 货币代码
    char          chCashcheck;      // 现金支票类型
};
struct MATRADEAPI CRspStkRpcCuacctCashConcentrationField
{
    char          szFundeffect[21 + 1]; // 归集金额          　
    int           iRenum;               // 返回笔数
};
//-------------------------------10101568:取同一客户交易台帐、转帐台帐--------------------------
struct MATRADEAPI CReqStkRpcCuacctInfo2Field
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户代码
};
struct MATRADEAPI CRspStkRpcCuacctInfo2Field
{
    char          szOrgid[4 + 1];      // 机构代码
    LONGLONG      llFundid;            // 资金帐号
    char          chFundtype;          // 帐户类别
    char          chMoneytype;         // 货币代码
    char          szFundbal[21 + 1];   // 余额
    char          szFundavl[21 + 1];   // 可用
    char          szSfbankcode[4 + 1]; // 三方存管银行代码
};
//-------------------------------10101569:资金可取数查询--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctMaxDrawField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspStkQryRpcCuacctMaxDrawField
{
    LONGLONG      llCustid;          // 客户代码
    LONGLONG      llFundid;          // 资金账户
    char          szOrgid[4 + 1];    // 机构编码
    char          chMoneytype;       // 货币
    char          szMaxdraw[21 + 1]; // 可取金额
};
//-------------------------------10101570:客户可取资金查询--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctMaxDrawForTencentPayField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币代码
};
struct MATRADEAPI CRspStkQryRpcCuacctMaxDrawForTencentPayField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金代码
    char          chMoneytype;       // 货币代码
    char          szMaxdraw[21 + 1]; // 最大可取金额
};
//-------------------------------10101571:取证券余额及当前最新价等,支持三方交易--------------------------
struct MATRADEAPI CReqStkRpcStkAvlAndLatestPriceField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金账户
    char          szStkcode[8 + 1]; // 证券代码
    char          szPrice[21 + 1];  // 价格
    int           iQty;             // 数量
};
struct MATRADEAPI CRspStkRpcStkAvlAndLatestPriceField
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
//-------------------------------10101572:取最大交易数量,支持三方交易--------------------------
struct MATRADEAPI CReqStkRpcMaxStkQtyField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    LONGLONG      llFundid;             // 资金账户
    char          szStkcode[8 + 1];     // 证券代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szPrice[21 + 1];      // 价格
    char          szBankcode[4 + 1];    // 外部银行
    char          chHiqtyflag;          // 最大上限标志
    char          chCreditid;           // 信用产品标识
    char          chCreditflag;         // 特殊委托类型
    char          chLinkmarket;         // 关联市场
    char          szLinksecuid[10 + 1]; // 关联股东
    char          szSorttype[4 + 1];    // 品种类别
    char          chDzsaletype;         // 减持类型
    char          szProdcode[12 + 1];   // 产品编码
};
struct MATRADEAPI CRspStkRpcMaxStkQtyField
{
    int           iMaxstkqty; // 股份数量
};
//-------------------------------10101573:买卖委托业务,支持三方交易--------------------------
struct MATRADEAPI CReqStkRpcOrderField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    LONGLONG      llFundid;             // 资金账户
    char          szStkcode[8 + 1];     // 证券代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szPrice[21 + 1];      // 价格
    int           iQty;                 // 数量
    int           iOrdergroup;          // 委托批号
    char          szBankcode[4 + 1];    // 外部银行
    char          chCreditid;           // 信用产品标识
    char          chCreditflag;         // 特殊委托类型
    char          szRemark[64 + 1];     // 备注信息
    char          szTargetseat[6 + 1];  // 对方席位
    int           iPromiseno;           // 约定号
    int           iRisksno;             // 风险调查流水号
    char          chAutoflag;           // 自动展期标志
    int           iEnddate;             // 展期终止日期
    char          szLinkman[12 + 1];    // 联系人
    char          szLinkway[30 + 1];    // 联系方式
    char          chLinkmarket;         // 关联市场
    char          szLinksecuid[10 + 1]; // 关联股东
    char          szSorttype[3 + 1];    // 品种类别
    int           iMergematchcode;      // 合并管理的初始交易的成交编号
    int           iMergematchdate;      // 合并管理的初始交易的成交日期
    char          szOldorderid[24 + 1]; // 原合同序号
    char          szProdcode[12 + 1];   // 产品编码
    char          chPricetype;          // 报价类型
    char          chBlackflag;          // 是否允许购买黑名单证券
    char          chDzsaletype;         // 减持类型
    LONGLONG      llRisksignsno;        // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
};
//-------------------------------10101574:委托撤单--------------------------
struct MATRADEAPI CReqStkRpcOrderCancelField
{
    int           iOrderdate;       // 委托日期
    LONGLONG      llFundid;         // 资金帐户
    int           iOrdersno;        // 委托序号
    char          szBsflag[2 + 1];  // 买卖类别
};
struct MATRADEAPI CRspStkRpcOrderCancelField
{
    char          szMsgok[32 + 1]; // 成功信息
    char          chCancelStatus;  // 内部撤单标志
    int           iOrdersno;       // 撤单委托序号
};
//-------------------------------10101575:委托撤单查询--------------------------
struct MATRADEAPI CReqStkQryRpcOrderCancelField
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
struct MATRADEAPI CRspStkQryRpcOrderCancelField
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
};
//-------------------------------10101576:重置成本--------------------------
struct MATRADEAPI CReqStkRpcResetCostField
{
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    char          szStkcode[8 + 1];    // 证券代码
    LONGLONG      llFundid;            // 资金帐户
    char          szCostprice[21 + 1]; // 成本价格
    char          chReqtype;           // 重置类型
};
struct MATRADEAPI CRspStkRpcResetCostField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101577:盈亏费用计算--------------------------
struct MATRADEAPI CReqStkRpcCalcFeeField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金帐户
    char          szPrice[21 + 1];  // 成交价格
    int           iQty;             // 成交数量
};
struct MATRADEAPI CRspStkRpcCalcFeeField
{
    char          szTradefee[21 + 1]; // 总费用
};
//-------------------------------10101578:委托买卖业务,支持三方交易（新增外围调用）--------------------------
struct MATRADEAPI CReqStkRpcOrderForTencentPayField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    LONGLONG      llFundid;             // 资金账户
    char          szStkcode[8 + 1];     // 证券代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szPrice[21 + 1];      // 价格
    int           iQty;                 // 数量
    char          szOrdergroup[32 + 1]; // 委托批号
    char          szBankcode[4 + 1];    // 外部银行
    char          chCreditid;           // 信用产品标识
    char          chCreditflag;         // 特殊委托类型
    char          szRemark[64 + 1];     // 备注信息
    char          szTargetseat[6 + 1];  // 对方席位
    int           iPromiseno;           // 约定号
    int           iRisksno;             // 风险调查流水号
    char          chAutoflag;           // 自动展期标志
    int           iEnddate;             // 展期终止日期
    char          szLinkman[12 + 1];    // 联系人
    char          szLinkway[30 + 1];    // 联系方式
    char          chLinkmarket;         // 关联市场
    char          szLinksecuid[10 + 1]; // 关联股东
    char          szSorttype[3 + 1];    // 品种类别
    int           iMergematchcode;      // 合并管理的初始交易的成交编号
    int           iMergematchdate;      // 合并管理的初始交易的成交日期
    char          szOldorderid[24 + 1]; // 原合同序号
    char          szProdcode[12 + 1];   // 产品编码
    char          chPricetype;          // 报价类型
    char          szExtsno[32 + 1];     // 外部流水号
};
struct MATRADEAPI CRspStkRpcOrderForTencentPayField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
    char          szExtsno[32 + 1];  // 外部流水号
};
//-------------------------------10101579:资管份额转让T+0委托--------------------------
struct MATRADEAPI CReqStkRpcTransferCapitalManagementShareField
{
    char          chMarket;             // 交易市场
    LONGLONG      llFundid;             // 资金帐户
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szTaacc[12 + 1];      // 基金账号
    char          szOfcode[8 + 1];      // 基金代码
};
struct MATRADEAPI CRspStkRpcTransferCapitalManagementShareField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101580:放弃配售缴款, 支持三方交易--------------------------
struct MATRADEAPI CReqStkRpcCancelRationField
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
struct MATRADEAPI CRspStkRpcCancelRationField
{
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101581:放弃配售缴款查询--------------------------
struct MATRADEAPI CReqStkQryRpcCancelRationField
{
    int           iSno;              // 流水号
    int           iMatchdate;        // 中签日期
    char          chMarket;          // 交易市场
    char          szStkcode[6 + 1];  // 证券代码
    char          szSecuid[10 + 1];  // 股东代码
    LONGLONG      llFundid;          // 资金账户
    int           iGiveupqty;        // 放弃数量
    char          szBankcode[4 + 1]; // 外部银行
    char          szRemark[64 + 1];  // 备注信息
};
struct MATRADEAPI CRspStkQryRpcCancelRationField
{
    int           iMatchdate;           // 中签日期
    int           iSno;                 // 序号
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llFundid;             // 资金帐号
    LONGLONG      llCustid;             // 客户代码
    int           iOrderdate;           // 委托日期
    int           iSettlefundays;       // 缴款日期
    char          szBpsecuid[10 + 1];   // 报盘股东
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szSeat[6 + 1];        // 交易席位
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrderid[10 + 1];    // 合同序号
    int           iHitqty;              // 中签数量
    char          szMatchprice[21 + 1]; // 价格
    int           iPayqty;              // 缴款数量
    char          chSourcetype;         // 发起方
    char          szBankcode[4 + 1];    // 外部机构
    int           iGiveupqty;           // 放弃缴款数量
    char          chGiveupflag;         // 放弃委托发送标志
    int           iMktconfirmqty;       // 交易所放弃数量
};
//-------------------------------10101582:客户摇号查询--------------------------
struct MATRADEAPI CReqStkQryRpcCustLotteryField
{
    LONGLONG      llFundid;         // 资金帐户
    int           iLotteryno;       // 配号的期号
};
struct MATRADEAPI CRspStkQryRpcCustLotteryField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户代码
    LONGLONG      llFundid;         // 资金帐号
    int           iIssue;           // 配号的期号
    int           iLotteryno;       // 配号
    char          chHitflag;        // 配号中签标志
    char          chProcessflag;    // 客户兑奖标志
    char          szRemark[32 + 1]; // 备注
};
//-------------------------------10101583:客户从外围解冻中签预冻结资金--------------------------
struct MATRADEAPI CReqStkRpcUnfreezePreFrozenFundsField
{
    LONGLONG      llFundid;         // 资金帐户
    int           iMatchdate;       // 中签日期
};
struct MATRADEAPI CRspStkRpcUnfreezePreFrozenFundsField
{
    int           iSno;               // 操作流水号
    char          szUnfrzamt[21 + 1]; // 解冻预冻结金额
};
//-------------------------------10101584:产品信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeProductInfoField
{
    char          chProducttype;    // 产品类型
    int           iGhdays;          // 产品期限
};
struct MATRADEAPI CRspStkQryRpcStkPledgeProductInfoField
{
    char          chProducttype;         // 产品类型
    char          szProductname[64 + 1]; // 产品名称
    int           iGhdays;               // 产品期限
    char          chTermtype;            // 期限类型
    char          szDuerate[21 + 1];     // 年利率
    char          szPrerate[21 + 1];     // 提前购回利率
    char          szContrate[21 + 1];    // 延期利率
    char          szPunishrate[21 + 1];  // 罚息利率
    int           iMinintdays;           // 最小计息天数
    int           iMaxtimes;             // 最高延期次数
    int           iMaxghdays;            // 最长天数
    char          szWarningrate[21 + 1]; // 预警线
    char          szLowestrate[21 + 1];  // 最低线
    char          szEndingrate[21 + 1];  // 终止线
};
//-------------------------------10101585:可质押标的证券查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeNegotiableSecurityField
{
    char          chMarket;         // 交易市场
    char          szStkcode[6 + 1]; // 证券代码
    char          chTargetstktype;  // 标的券类型
};
struct MATRADEAPI CRspStkQryRpcStkPledgeNegotiableSecurityField
{
    char          chMarket;             // 交易市场
    char          szStkcode[6 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    char          szCloseprice[21 + 1]; // 昨收盘价
    char          szExchrate[21 + 1];   // 默认质押折算率
};
//-------------------------------10101586:客户质押标的折算率查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgePledgeConversionRateField
{
    char          chMarket;         // 交易市场
    char          szStkcode[6 + 1]; // 证券代码
    char          chProducttype;    // 产品类型
};
struct MATRADEAPI CRspStkQryRpcStkPledgePledgeConversionRateField
{
    char          chMarket;           // 交易市场
    char          szStkcode[6 + 1];   // 证券代码
    char          szStkname[8 + 1];   // 证券名称
    char          chProducttype;      // 产品类型
    char          szExchrate[21 + 1]; // 质押折算率
};
//-------------------------------10101587:客户贷款额度信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeLoanQuotaField
{
};
struct MATRADEAPI CRspStkQryRpcStkPledgeLoanQuotaField
{
    char          szOrgid[4 + 1];     // 机构代码
    LONGLONG      llCustid;           // 客户代码
    char          szTotalamt[21 + 1]; // 总额度
    char          szAvlamt[21 + 1];   // 剩余可用额度
};
//-------------------------------10101588:可质押证券持仓查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeNegotiableSecurityPosiField
{
    char          chProducttype;    // 产品类型
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkQryRpcStkPledgeNegotiableSecurityPosiField
{
    char          chMarket;              // 市场
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkcode[6 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
    LONGLONG      llStkavl;              // 可质押数量
    char          szLastprice[21 + 1];   // 行情最新价
    char          szMktvalue[21 + 1];    // 市值
    char          szPledgeprice[21 + 1]; // 质押品价格
    char          szExchrate[21 + 1];    // 折算率
    char          szMaxamt[21 + 1];      // 可贷金额
};
//-------------------------------10101589:最大可贷款金额查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeMaxLoanAmtField
{
    char          szOrgid[4 + 1];   // 机构代码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          szSecuid[10 + 1]; // 股东代码
    int           iOrderqty;        // 质押数量
    char          chProducttype;    // 品种
    int           iGhdays;          // 期限
};
struct MATRADEAPI CRspStkQryRpcStkPledgeMaxLoanAmtField
{
    char          szMaxamt[21 + 1]; // 最大可贷金额
};
//-------------------------------10101590:贷款申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeLoanApplicationField
{
    char          szOrgid[4 + 1];       // 机构代码
    LONGLONG      llFundid;             // 资金账号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szSecuid[10 + 1];     // 股东代码
    int           iOrderqty;            // 质押数量
    char          szOrderamt[21 + 1];   // 贷款金额
    char          chProducttype;        // 品种
    int           iGhdays;              // 期限
    char          szFundusetype[2 + 1]; // 资金用途类型
    char          szFunduse[100 + 1];   // 资金用途描述
    LONGLONG      llRisksignsno;        // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcStkPledgeLoanApplicationField
{
    char          szReqsno[32 + 1]; // 申请单号
};
//-------------------------------10101591:还款申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeRepaymentApplicationField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金账号
    char          szGpzysno[32 + 1]; // 合约编号
};
struct MATRADEAPI CRspStkRpcStkPledgeRepaymentApplicationField
{
    int           iReqsno; // 申请单号
};
//-------------------------------10101592:补充证券质押物申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeSupplementaryPledgeSecurityApplicationField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金账号
    char          szGpzysno[32 + 1]; // 合约编号
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szSecuid[10 + 1];  // 股东代码
    int           iOrderqty;         // 质押数量
};
struct MATRADEAPI CRspStkRpcStkPledgeSupplementaryPledgeSecurityApplicationField
{
    LONGLONG      llReqsno; // 申请单号
};
//-------------------------------10101593:合约还款日延期申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeDeferredContractRepaymentApplicationField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金账号
    char          szGpzysno[32 + 1]; // 合约编号
    int           iEnddate;          // 延期目标日
};
struct MATRADEAPI CRspStkRpcStkPledgeDeferredContractRepaymentApplicationField
{
    int           iReqsno; // 申请单号
};
//-------------------------------10101594:合约需补充质押数量查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeInsufficientPledgeField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金账号
    char          szGpzysno[32 + 1]; // 合约编号
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
};
struct MATRADEAPI CRspStkQryRpcStkPledgeInsufficientPledgeField
{
    LONGLONG      llQty;             // 需补充质押数量
    char          szNeedamt[21 + 1]; // 需补充金额
};
//-------------------------------10101595:业务申请情况查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeBizApplicationStatusField
{
    char          szOrgid[4 + 1];   // 机构代码
    LONGLONG      llFundid;         // 资金账号
    char          szReqbiz[8 + 1];  // 申请业务类型
    int           iBegindate;       // 日期(起)
    int           iEnddate;         // 日期(止)
};
struct MATRADEAPI CRspStkQryRpcStkPledgeBizApplicationStatusField
{
    char          szReqsno[32 + 1];     // 申请单号
    int           iReqdate;             // 申请日期
    char          szReqbiz[8 + 1];      // 申请业务类型
    char          chMarket;             // 市场
    char          szStkcode[6 + 1];     // 证券代码
    char          szSecuid[10 + 1];     // 股东代码
    LONGLONG      llOrderqty;           // 数量
    char          szOrderamt[21 + 1];   // 金额
    char          szDuerate[21 + 1];    // 利率
    char          chProducttype;        // 产品类型
    int           iGhdays;              // 期限
    int           iTargetdate;          // 到期日
    char          szFundusetype[2 + 1]; // 资金用途类型
    char          szFunduse[100 + 1];   // 资金用途描述
    char          chDealstatus;         // 处理状态
    char          szDealmsg[128 + 1];   // 处理结果
    char          szGpzysno[32 + 1];    // 合约编号
    char          szOrderid[10 + 1];    // 交易所合同序号
    char          szRemark[256 + 1];    // 备注
};
//-------------------------------10101596:贷款合约查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeLoanContractField
{
    char          szGpzysno[32 + 1]; // 合约编号
    char          chMarket;          // 市场代码
    int           iOrderbegindate;   // 贷款日期(起)
    int           iOrderenddate;     // 贷款日期(止)
    char          chEndflag;         // 了结情况
    char          chBusikind;        // 业务类型
    char          szPosstr[64 + 1];  // 定位串
};
struct MATRADEAPI CRspStkQryRpcStkPledgeLoanContractField
{
    int           iSysdate;                  // 合约日期
    char          szGpzysno[32 + 1];         // 合约编号
    char          chMarket;                  // 市场代码
    char          chProducttype;             // 产品类型
    int           iGhdays;                   // 期限
    char          szDuerate[21 + 1];         // 年利率
    char          szPrerate[21 + 1];         // 提前购回利率
    char          szContrate[21 + 1];        // 延期利率
    char          szPunishrate[21 + 1];      // 罚息利率
    int           iOldenddate;               // 初始购回日期
    int           iEnddate;                  // 合约到期日
    char          szMatchamt[21 + 1];        // 贷款金额
    char          szBackamt[21 + 1];         // 已还金额
    char          szBackamtReal[21 + 1];     // 日间实时偿还总额
    char          szCalbackamt[21 + 1];      // 预计购回金额
    char          szDueintr[21 + 1];         // 当前总利息
    char          szAwardamt[21 + 1];        // 利息基数
    char          szAwardintr[21 + 1];       // 已计利息
    char          szUncalintr[21 + 1];       // 当前未结利息
    char          szPayinterest[21 + 1];     // 已还利息
    char          szCallbackintr[21 + 1];    // 当前购回利息
    char          szSaferate[21 + 1];        // 昨日风险比例
    char          szRiskrate[21 + 1];        // 实时风险比例
    char          chTermtype;                // 期限类型
    char          szFundusetype[2 + 1];      // 资金用途类型
    char          szFundusedo[100 + 1];      // 资金用途描述
    char          chStatus;                  // 合约状态
    char          szOrderid[10 + 1];         // 合同序号
    char          chRiskstatus;              // 违约状态
    char          szWarningrate[21 + 1];     // 预警线
    char          szLowestrate[21 + 1];      // 最低线
    char          szEndingrate[21 + 1];      // 终止线
    char          szFundpay[21 + 1];         // 应补资金
    char          szPursueamt[21 + 1];       // 追报金额
    char          szPursueamtReal[21 + 1];   // 已追保金额
    char          szRemainpursueamt[21 + 1]; // 剩余追保金额
    int           iTimes;                    // 已延期次数
    int           iPursuedays;               // 追保天数
    char          szPosstr[64 + 1];          // 定位串
};
//-------------------------------10101597:贷款合约质押明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeLoanContractPledgeDetailField
{
    char          szGpzysno[32 + 1]; // 合约编号
    char          chBusikind;        // 业务类型
    char          chMarket;          // 市场代码
};
struct MATRADEAPI CRspStkQryRpcStkPledgeLoanContractPledgeDetailField
{
    char          szGpzysno[32 + 1];          // 合约编号
    int           iItemno;                    // 序号
    char          chMarket;                   // 市场代码
    char          szSecuid[10 + 1];           // 股东代码
    char          szStkcode[8 + 1];           // 证券代码
    LONGLONG      llMatchqty;                 // 抵押数量
    LONGLONG      llBonusqty;                 // 红股数量
    LONGLONG      llBackqty;                  // 己返还总数量
    LONGLONG      llBackqtyReal;              // 日间实时返还数量
    char          szBonusamt[21 + 1];         // 红利金额
    char          szBackbonusamt[21 + 1];     // 己返还红利总金额
    char          szBackbonusamtReal[21 + 1]; // 日间实时返还金额
    char          chStatus;                   // 合约状态
};
//-------------------------------10101598:股票质押额度修改--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeChgStkPledgeQuotaField
{
    char          szTotalamt[21 + 1]; // 总额度
};
struct MATRADEAPI CRspStkRpcStkPledgeChgStkPledgeQuotaField
{
    char          szTotalamt[21 + 1]; // 总额度
    char          szExchrate[21 + 1]; // 征信系数
};
//-------------------------------10101599:部分还款申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgePartialRepaymentApplicationField
{
    char          szOrgid[4 + 1];     // 机构代码
    LONGLONG      llFundid;           // 资金账号
    char          szGpzysno[32 + 1];  // 合约编号
    int           iItemno;            // 合约序号
    int           iOrderqty;          // 购回数量
    char          szOrderamt[21 + 1]; // 购回金额
};
struct MATRADEAPI CRspStkRpcStkPledgePartialRepaymentApplicationField
{
    int           iReqsno; // 申请单号
};
//-------------------------------10101600:出借人额度查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeLenderQuotaField
{
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 证券帐号
};
struct MATRADEAPI CRspStkQryRpcStkPledgeLenderQuotaField
{
    char          szOrgid[4 + 1];   // 机构代码
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szAvlamt[21 + 1]; // 剩余可用额度
    char          chStatus;         // 状态
    char          chLoanermodel;    // 出借人模式
};
//-------------------------------10101601:查询合约违约状态为追保的合约--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeAdditionalMarginContractField
{
    LONGLONG      llFundid;               // 资金账号
    char          szGpzysno[32 + 1];      // 合约编号
    char          szBusikind[128 + 1];    // 业务类型
    char          szProducttype[128 + 1]; // 产品类型
};
struct MATRADEAPI CRspStkQryRpcStkPledgeAdditionalMarginContractField
{
    int           iSysdate;                  // 合约日期
    char          szGpzysno[32 + 1];         // 合约编号
    LONGLONG      llFundid;                  // 资金帐号
    char          chMarket;                  // 市场代码
    char          szSecuid[10 + 1];          // 股东代码
    char          chProducttype;             // 产品类型
    int           iGhdays;                   // 期限
    char          szDuerate[21 + 1];         // 年利率
    char          szPrerate[21 + 1];         // 提前购回利率
    char          szContrate[21 + 1];        // 延期利率
    char          szPunishrate[21 + 1];      // 罚息利率
    int           iOldenddate;               // 初始购回日期
    int           iEnddate;                  // 合约到期日
    char          szMatchamt[21 + 1];        // 贷款金额
    char          szBackamt[21 + 1];         // 已还金额
    char          szBackamtReal[21 + 1];     // 日间实时偿还总额
    char          szCalbackamt[21 + 1];      // 预计购回金额
    char          szDueintr[21 + 1];         // 当前总利息
    char          szPayinterest[21 + 1];     // 已还利息
    char          szSaferate[21 + 1];        // 昨日风险比例
    char          chTermtype;                // 期限类型
    char          chStatus;                  // 合约状态
    char          chRiskstatus;              // 违约状态
    char          szWarningrate[21 + 1];     // 预警线
    char          szLowestrate[21 + 1];      // 最低线
    char          szEndingrate[21 + 1];      // 终止线
    char          szPursueamt[21 + 1];       // 追保金额
    char          szPursueamtReal[21 + 1];   // 已追保金额
    char          szRemainpursueamt[21 + 1]; // 剩余追保金额
};
//-------------------------------10101602:查询追保合约需要追保的数量--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeAdditionalMarginQtyField
{
    char          szGpzysno[32 + 1]; // 合约编号
    char          chMarket;          // 市场
    char          szStkcode[10 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcStkPledgeAdditionalMarginQtyField
{
    LONGLONG      llPursueqty; // 追保数量
};
//-------------------------------10101603:贷款本金偿还--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeLoanPrincipalRepaymentField
{
    char          szGpzysno[32 + 1]; // 合约编号
    char          szBackamt[21 + 1]; // 偿还金额
};
struct MATRADEAPI CRspStkRpcStkPledgeLoanPrincipalRepaymentField
{
    char          szReqsno[16 + 1]; // 申请单号
};
//-------------------------------10101604:可用于还款资金查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgeAvailableQtyForRepaymentField
{
    char          szGpzysno[32 + 1]; // 合约编号
};
struct MATRADEAPI CRspStkQryRpcStkPledgeAvailableQtyForRepaymentField
{
    char          szCanuseamt[21 + 1];     // 可使用金额
    char          szRemainbackamt[21 + 1]; // 待购回金额
};
//-------------------------------10101605:股份部分解除质押申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgePartialCancellationPledgeApplicationField
{
    char          szOrgid[4 + 1];    // 机构代码
    LONGLONG      llFundid;          // 资金账号
    char          szGpzysno[32 + 1]; // 合约编号
    int           iItemno;           // 合约序号
    int           iOrderqty;         // 解押数量
};
struct MATRADEAPI CRspStkRpcStkPledgePartialCancellationPledgeApplicationField
{
    LONGLONG      llReqsno; // 申请单号
};
//-------------------------------10101606:客户个性化设置利率查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkPledgePersonalizedInterestRateField
{
};
struct MATRADEAPI CRspStkQryRpcStkPledgePersonalizedInterestRateField
{
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llCustid;             // 客户代码
    char          chProducttype;        // 产品类别
    int           iGhdays;              // 产品期限
    char          chTermtype;           // 期限类型
    char          szDuerate[21 + 1];    // 年利率
    char          szPrerate[21 + 1];    // 提前购回利率
    char          szContrate[21 + 1];   // 延期利率
    char          szPunishrate[21 + 1]; // 罚息利率
    int           iStartdate;           // 起始日期
    int           iEnddate;             // 结束日期
    char          szRemark[32 + 1];     // 备注
};
//-------------------------------10101607:合约利息偿还申请--------------------------
struct MATRADEAPI CReqStkRpcStkPledgeRepaymentContractInterestApplicationField
{
    char          szGpzysno[32 + 1]; // 合约编号
    char          szBackamt[21 + 1]; // 偿还金额
};
struct MATRADEAPI CRspStkRpcStkPledgeRepaymentContractInterestApplicationField
{
    LONGLONG      llReqsno;          // 申请单号
    char          szBackamt[21 + 1]; // 实际偿还金额
};
//-------------------------------10101608:约定购回客户额度查询--------------------------
struct MATRADEAPI CReqStkQryRpcAgreementRepurchaseDebtContractField
{
    char          szYdghsno[32 + 1]; // 合约编号
    char          chMarket;          // 市场代码
    int           iOrderbegindate;   // 贷款日期(起)
    int           iOrderenddate;     // 贷款日期(止)
    char          chEndflag;         // 了结情况
    char          szPosstr[64 + 1];  // 定位串
    int           iQryrec;           // 查询行数
};
struct MATRADEAPI CRspStkQryRpcAgreementRepurchaseDebtContractField
{
    int           iSysdate;               // 合约日期
    char          szYdghsno[32 + 1];      // 合约编号
    char          chMarket;               // 市场代码
    char          szOrderid[10 + 1];      // 合同序号
    char          szMatchcode[20 + 1];    // 成交号码
    char          szDuerate[21 + 1];      // 年利率
    int           iEnddate;               // 合约到期日
    char          szMatchamt[21 + 1];     // 成交金额
    char          szBackamt[21 + 1];      // 已返还金额
    char          szBackamtUnfrz[21 + 1]; // 在途购回金额
    char          szBackamtReal[21 + 1];  // 日间购回金额
    LONGLONG      llMatchqty;             // 成交数量
    LONGLONG      llBonusqty;             // 红股数量
    LONGLONG      llBackqty;              // 已返还数量
    LONGLONG      llBackqtyUnfrz;         // 在途购回数量
    LONGLONG      llBackqtyReal;          // 日间购回数量
    char          szDueintr[21 + 1];      // 当前总利息
    char          szPayinterest[21 + 1];  // 已还利息
    char          szSaferate[21 + 1];     // 风险比例
    char          szFundusedo[100 + 1];   // 资金用途
    char          chStatus;               // 合约状态
    char          chRiskstatus;           // 违约状态
    char          szPosstr[64 + 1];       // 定位串
};
//-------------------------------10101609:约定购回客户额度查询--------------------------
struct MATRADEAPI CReqStkQryRpcAgreementRepurchaseQuotaField
{
};
struct MATRADEAPI CRspStkQryRpcAgreementRepurchaseQuotaField
{
    int           iServerid;          // 机器编码
    LONGLONG      llCustid;           // 客户代码
    char          szTotalamt[21 + 1]; // 总额度
    char          szAvlamt[21 + 1];   // 额度可用
};
//-------------------------------10101610:约定购回初始交易--------------------------
struct MATRADEAPI CReqStkRpcAgreementRepurchaseInitialTransactionField
{
    int           iFundid;               // 资金账号
    char          szMarket[128 + 1];     // 交易市场
    char          szStkcode[8 + 1];      // 证券代码
    char          szSecuid[10 + 1];      // 股东代码
    int           iOrderqty;             // 委托数量
    char          szOrderamt[21 + 1];    // 委托金额
    int           iGhdays;               // 购回天数
    char          szDuerate[21 + 1];     // 年费率
    char          szLinkydghsno[32 + 1]; // 关联合约编号
    char          chCycleflag;           // 利息扣除方式
    int           iCyclevalue;           // 利息扣收日
    char          szFundusedo[32 + 1];   // 资金用途
    LONGLONG      llRisksignsno;         // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcAgreementRepurchaseInitialTransactionField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 委托订单号
};
//-------------------------------10101611:约定购回购回交易--------------------------
struct MATRADEAPI CReqStkRpcAgreementRepurchaseRepurchaseTransactionField
{
    LONGLONG      llFundid;          // 资金账号
    char          szYdghsno[32 + 1]; // 合约编号
};
struct MATRADEAPI CRspStkRpcAgreementRepurchaseRepurchaseTransactionField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 委托订单号
};
//-------------------------------10101612:约定购回延期购回申请--------------------------
struct MATRADEAPI CReqStkRpcAgreementRepurchaseDelayedRepurchaseApplicationField
{
    LONGLONG      llFundid;          // 资金帐户
    char          szYdghsno[32 + 1]; // 合约编号
    int           iDelaydays;        // 延迟天数
};
struct MATRADEAPI CRspStkRpcAgreementRepurchaseDelayedRepurchaseApplicationField
{
    int           iSno; // 操作流水号
};
//-------------------------------10101613:取权证信息--------------------------
struct MATRADEAPI CReqStkRpcWarrantInfoField
{
    char          chMarket;         // 交易市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkRpcWarrantInfoField
{
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 行权代码
    char          szWarrantcode[8 + 1];  // 权证代码
    char          szTargetstk[8 + 1];    // 标的证券代码
    char          chExertype;            // 行权方式
    char          chWarrantkind;         // 期权类型
    char          chCleartype;           // 结算方式
    char          szExerprice[21 + 1];   // 行权价格
    char          szExerratio[21 + 1];   // 行权比例
    char          szTotquantity[32 + 1]; // 权证流通总余额
    char          szClearprice[21 + 1];  // 结算价格
    int           iExpdate;              // 到期日期
    int           iExerbegindate;        // 行权开始日期
    int           iExerenddate;          // 行权结束日期
};
//-------------------------------10101614:客户自主行权委托预计所得税查询--------------------------
struct MATRADEAPI CReqStkQryRpcExpectedIncomeTaxField
{
    LONGLONG      llCustid;           // 客户代码
    char          chMarket;           // 交易市场
    char          chStkcode;          // 证券代码
    char          szOrderqty[10 + 1]; // 委托数量
};
struct MATRADEAPI CRspStkQryRpcExpectedIncomeTaxField
{
    char          szIncometaxamt[21 + 1]; // 应纳税所得额
    char          szIncometax[21 + 1];    // 应缴税额
};
//-------------------------------10101615:合约查询(410983)--------------------------
struct MATRADEAPI CReqStkQryRpcContractField
{
    int           iSysdate;         // 交易日期
    char          szRzsno[64 + 1];  // 合约编号
    char          szMarket[32 + 1]; // 市场
    LONGLONG      llFundid;         // 资金账户
    char          szStkcode[6 + 1]; // 证券代码
    char          chStatus;         // 合约状态
    int           iEnddate;         // 到期日期
    int           iClosedate;       // 了结日期
};
struct MATRADEAPI CRspStkQryRpcContractField
{
    int           iSysdate;                  // 交易日期
    char          szRzsno[64 + 1];           // 合约编号
    char          szOrgid[4 + 1];            // 机构编码
    LONGLONG      llCustid;                  // 客户代码
    char          szCustname[32 + 1];        // 客户名称
    LONGLONG      llFundid;                  // 资金账户
    char          chMoneytype;               // 币种
    char          chMarket;                  // 市场
    char          szSeat[6 + 1];             // 席位
    char          szSecuid[10 + 1];          // 股东账户
    char          szOrderid[10 + 1];         // 委托序号
    char          szStkcode[8 + 1];          // 证券代码
    char          szStkname[32 + 1];         // 证券名称
    char          szTargetstk[8 + 1];        // 标的证券
    char          szOrderprice[21 + 1];      // 委托价格
    LONGLONG      llConqty;                  // 行权数量
    char          szConamt[21 + 1];          // 行权金额
    char          szPayconamt[21 + 1];       // 已归还本金
    char          szPayconamtReal[21 + 1];   // 实时归还本金
    char          szTaxamt[21 + 1];          // 扣税金额
    char          szPaytaxamt[21 + 1];       // 已还扣税金额
    char          szPaytaxamtReal[21 + 1];   // 实时归还扣税金额
    int           iIntcaldate;               // 上次计息日期
    char          szDuerate[21 + 1];         // 利息年利率
    char          szDueintr[21 + 1];         // 应付利息
    char          szPaydueintr[21 + 1];      // 已还利息
    char          szPaydueintrReal[21 + 1];  // 实时归还利息
    char          szPunirate[21 + 1];        // 违约日利率
    char          szPuniintr[21 + 1];        // 应付违约金
    char          szPaypuniintr[21 + 1];     // 已还违约金
    char          szPaypuniintrReal[21 + 1]; // 实时归还违约金
    int           iEnddate;                  // 到期日期
    int           iClosedate;                // 了结日期
    char          chStatus;                  // 合约状态
    char          szRzdebt[21 + 1];          // 客户需偿还负债
    char          szRemark[64 + 1];          // 备注
};
//-------------------------------10101616:融资行权委托查询(410989)--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingRightCommissionField
{
    int           iBegindate;         // 开始日期
    int           iEnddate;           // 结束日期
    char          szOrgid[4 + 1];     // 机构编码
    LONGLONG      llFundid;           // 资金账户
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    char          szStkcode[6 + 1];   // 证券代码
    char          szBsflag[2 + 1];    // 买卖类别
    char          szBsflagSub[2 + 1]; // 行权类型
    char          chCancelflag;       // 撤单标识
    char          chOrderstatus;      // 委托状态
    char          szPosstr[64 + 1];   // 定位串
    int           iQryrec;            // 请求行数
};
struct MATRADEAPI CRspStkQryRpcFinancingRightCommissionField
{
    int           iOrdersno;              // 委托序号
    int           iOrdergroup;            // 批量委托批号
    int           iOrdertime;             // 发生委托时间
    LONGLONG      llCustid;               // 客户代码
    char          szCustname[16 + 1];     // 客户姓名
    char          szOrgid[4 + 1];         // 分支机构
    char          szBrhid[4 + 1];         // 机构分支
    LONGLONG      llFundid;               // 资产帐户
    char          chMoneytype;            // 货币代码
    char          chFundkind;             // 资金类别
    char          chFundlevel;            // 资金级别
    char          chFundgroup;            // 资金分组
    char          szSeat[6 + 1];          // 报盘席位
    char          chMarket;               // 交易市场
    char          szTargetstk[6 + 1];     // 标的证券
    char          chChannel;              // 通道号
    char          szStkname[8 + 1];       // 证券名称
    char          chStktype;              // 证券类别
    int           iCancelqty;             // 撤单数量
    char          szOrderfrzamt[21 + 1];  // 委托冻结金额
    char          szTaxamt[21 + 1];       // 本次委托冻结所得税
    char          szBsflag[2 + 1];        // 买卖方向
    char          szBsflagSub[2 + 1];     // 行权类型
    char          chCancelflag;           // 撤单标志'F'
    char          chOrderstatus;          // 委托状态
    char          szOldorderid[10 + 1];   // 撤单序号
    int           iOperid;                // 操作柜员代码
    char          szNetaddr[64 + 1];      // 操作站点
    char          chOperway;              // 操作方式
    char          szRemark[128 + 1];      // 其他备注
    char          szClearmainseat[6 + 1]; // 非交易申报结算主席位,fjybs.bszjjs
    char          szBsywlb[2 + 1];        // 申报业务类别,fjybs.bsywlb
    char          chBswtlx;               // 报送委托类型,fjybs.bswtlx,'S'申报,'C'撤单
    char          szOrderid[10 + 1];      // 报送合同序号,fjybs.bswtxh
    char          szStkcode[6 + 1];       // 证券代码
    char          szSecuid[10 + 1];       // 股东代码
    char          szBsxwdm[6 + 1];        // 报送席位代码
    char          szBsywsj[32 + 1];       // 报送业务数据
    char          szBsgfxz[2 + 1];        // 报送股份性质
    int           iOrderqty;              // 委托数量
    char          szOrderamt[21 + 1];     // 委托金额
    char          szOrderprice[21 + 1];   // 委托价格
    char          chBsbz1;                // 报送标识1
    char          chBsbz2;                // 报送标识2
    char          szBsdfxw[6 + 1];        // 报送对方席位
    char          szBsdfgd[10 + 1];       // 报送对方股东
    char          szBsdfxz[2 + 1];        // 报送对方性质
    char          szBsnr1[64 + 1];        // 报送内容1
    char          szBsnr2[128 + 1];       // 报送内容2
    int           iOrderdate;             // 委托交易日期
    int           iReporttime;            // 报送时间
    char          chBsclbz;               // 报送处理标识
    char          chBsbyzd;               // 报送备用字段
};
//-------------------------------10101617:股权激励人员信息查询（410990）--------------------------
struct MATRADEAPI CReqStkQryRpcEquityIncentivePersonnelInfoField
{
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    char          szStkcode[6 + 1];   // 行权代码
    char          szTargetstk[6 + 1]; // 标的券代码
};
struct MATRADEAPI CRspStkQryRpcEquityIncentivePersonnelInfoField
{
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    char          szTargetstk[6 + 1];  // 标的券代码
    char          szStkcode[6 + 1];    // 行权代码
    LONGLONG      llCustid;            // 客户代码
    char          szCuatname[16 + 1];  // 客户名称
    char          chCusttype;          // 客户类别
    int           iProratetime;        // 工作月数
    int           iWarBegindate;       // 敏感期开始
    int           iWarEnddate;         // 敏感期结束
    char          chProctax;           // 是否处理所得税
    char          szBegcaitax[21 + 1]; // 所得税起征金额
    char          chFrzstatus;         // 是否冻结标的券
    char          chStatus;            // 状态
};
//-------------------------------10101618:融资行权参数信息查询（410991）--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingExerciseParamsField
{
    char          chCusttype;       // 客户类别
};
struct MATRADEAPI CRspStkQryRpcFinancingExerciseParamsField
{
    char          chCusttype;             // 客户类型
    int           iContractdate;          // 融资期限
    char          szStdrate[21 + 1];      // 基准融资利率(年)
    char          szFinerate[21 + 1];     // 违约利率(日)
    char          szMaxlimit[21 + 1];     // 融资额度上限(元)
    char          szTranrate[21 + 1];     // 履约担保比例提取线
    char          szWarnrate[21 + 1];     // 履约担保比例警戒线
    char          szCloserate[21 + 1];    // 履约担保比例平仓线
    char          szWarnrateSub[21 + 1];  // 二级履约担保比例警戒线
    char          szCloserateSub[21 + 1]; // 二级履约担保比例平仓线
    char          szSaferate[21 + 1];     // 履约担保比例安全线
    char          szRemark[64 + 1];       // 备注
};
//-------------------------------10101619:融资行权资产负债查询（410992）--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingExerciseAssetsLiabilityField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llFundid;         // 资金账号
};
struct MATRADEAPI CRspStkQryRpcFinancingExerciseAssetsLiabilityField
{
    char          szAsset[21 + 1];           // 担保资产
    char          szDebt[21 + 1];            // 总负债
    char          szMktval[21 + 1];          // 担保证券市值
    char          szKeeprate[21 + 1];        // 履约担保比例
    char          szKeeprateSub[21 + 1];     // 二级履约担保比例
    char          szTranrate[21 + 1];        // 履约担保比例提取线
    char          szWarnrate[21 + 1];        // 履约担保比例警戒线
    char          szCloserate[21 + 1];       // 履约担保比例平仓线
    char          szDueintr[21 + 1];         // 利息
    char          szPuniintr[21 + 1];        // 违约金
    char          szTaxamt[21 + 1];          // 扣税
    char          szStdrate[21 + 1];         // 融资利率
    char          szConamt[21 + 1];          // 融资本金
    char          szCustbondamt[21 + 1];     // 融资授信额度
    char          szCustlimitremain[21 + 1]; // 融资可用额度
    char          szCanrepaydebt[21 + 1];    // 客户可还负债
    char          szRepayamt[21 + 1];        // 实时还款金额
};
//-------------------------------10101620:融资行权客户额度信息查询（410993）--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingExerciseCustQuotaInfoField
{
};
struct MATRADEAPI CRspStkQryRpcFinancingExerciseCustQuotaInfoField
{
    LONGLONG      llCustid;                  // 客户代码
    char          szCustname[16 + 1];        // 客户名称
    char          szCustbondamt[21 + 1];     // 客户授信额度
    char          szCustusedamt[21 + 1];     // 客户已用额度
    char          szCustusedamtReal[21 + 1]; // 客户实时已用额度
    char          szCustfinrate[21 + 1];     // 信用等级调节系数
    char          szCanrepaydebt[21 + 1];    // 客户可还负债
    char          szRepayamt[21 + 1];        // 实时还款金额
};
//-------------------------------10101621:直接还款(410984)--------------------------
struct MATRADEAPI CReqStkRpcDirectRepaymentField
{
    char          szOrgid[4 + 1];      // 机构编码
    LONGLONG      llFundid;            // 资金账户
    char          chMoneytype;         // 币种
    char          szReturnamt[21 + 1]; // 还款金额
};
struct MATRADEAPI CRspStkRpcDirectRepaymentField
{
    char          szFundeffect[21 + 1]; // 发生金额数
};
//-------------------------------10101622:融资行权业务(410985)--------------------------
struct MATRADEAPI CReqStkRpcFinancingExerciseBizField
{
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    LONGLONG      llFundid;           // 资金账户
    char          szOrgid[4 + 1];     // 机构编码
    char          szStkcode[8 + 1];   // 证券代码
    char          szBsflag[2 + 1];    // 买卖类别
    char          szBsflagSub[2 + 1]; // 行权类型
    LONGLONG      llOrderqty;         // 数量
    LONGLONG      llRisksignsno;      // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcFinancingExerciseBizField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
};
//-------------------------------10101623:取融资行权可委托数量(410986)--------------------------
struct MATRADEAPI CReqStkRpcFinancingExerciseMaxOrderQtyField
{
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    LONGLONG      llFundid;           // 资金账户
    char          szStkcode[8 + 1];   // 证券代码
    char          szBsflag[2 + 1];    // 买卖类别
    char          szBsflagSub[2 + 1]; // 行权类型
};
struct MATRADEAPI CRspStkRpcFinancingExerciseMaxOrderQtyField
{
    LONGLONG      llOrderqty; // 股份数量
};
//-------------------------------10101624:融资行权委托撤单(410987)--------------------------
struct MATRADEAPI CReqStkRpcFinancingExerciseCancelField
{
    int           iOrderdate;       // 委托日期
    int           iOrdersno;        // 委托序号
    int           iOrdergroup;      // 委托批号
};
struct MATRADEAPI CRspStkRpcFinancingExerciseCancelField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
};
//-------------------------------10101625:融资行权担保证券查询(410988)--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingExerciseGuaranteedSecurityInfoField
{
};
struct MATRADEAPI CRspStkQryRpcFinancingExerciseGuaranteedSecurityInfoField
{
    char          chMarket;             // 交易市场
    char          szStkcode[10 + 1];    // 证券代码
    char          szStkname[32 + 1];    // 证券名称
    char          szPledgerate[21 + 1]; // 折算率
};
//-------------------------------10101626:ETF成份股清单信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcETFStkListField
{
    char          chMarket;         // 交易市场
    char          szOfcode[8 + 1];  // ETF基金代码
    char          szStkcode[8 + 1]; // 成份股代码
    char          chStkmarket;      // 成份股交易市场
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcETFStkListField
{
    char          szPosstr[64 + 1];        // 定位串
    int           iServerid;               // 节点编号
    char          chMarket;                // 交易市场
    char          szOfcode[8 + 1];         // ETF基金代码
    char          szOfname[16 + 1];        // ETF基金名称
    char          szStkcode[8 + 1];        // 成份股代码
    char          szStkname[8 + 1];        // 成份股简称
    char          chStkmarket;             // 成份股交易市场
    char          szStkmic[4 + 1];         // 成份股市场标识码
    int           iAmount;                 // 股票数量
    char          chReplaceflag;           // 替代标志
    char          szOverpricerate[21 + 1]; // 溢价比例
    char          szReplaceamtSg[21 + 1];  // 申购替代金额
    char          szReplaceamtSh[21 + 1];  // 赎回替代金额
    int           iIBy;                    // 备用
    char          szCBy[8 + 1];            // 备用
    char          szRemark[32 + 1];        // 备注
};
//-------------------------------10101627:ETF网下认购申请（现金认购，股票认购）--------------------------
struct MATRADEAPI CReqStkRpcETFOfflineApplicationField
{
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    LONGLONG      llFundid;             // 资金帐户
    char          szOfcode[8 + 1];      // 基金认购代码
    char          szBsflag[2 + 1];      // 认购类型
    char          szStkcode[8 + 1];     // 股票代码
    int           iOrderqty;            // 委托数量
    char          chSecumarket;         // 成份股市场
    char          chLinkmarket;         // 关联市场
    char          szLinksecuid[10 + 1]; // 关联股东
    LONGLONG      llRisksignsno;        // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcETFOfflineApplicationField
{
    int           iSno;                 // 申请序号
    char          szFundeffect[21 + 1]; // 发生金额
};
//-------------------------------10101628:ETF网下认购申请撤单--------------------------
struct MATRADEAPI CReqStkRpcETFOfflineApplicationCancelField
{
    LONGLONG      llFundid;         // 资金帐户
    int           iSysdate;         // 申请时系统日期
    int           iSno;             // 申请序号
};
struct MATRADEAPI CRspStkRpcETFOfflineApplicationCancelField
{
    int           iSno;                // 申请序号
    char          szErrormsg[128 + 1]; // 返回信息
};
//-------------------------------10101629:ETF网下认购申请查询--------------------------
struct MATRADEAPI CReqStkQryRpcETFOfflineApplicationField
{
    int           iSysdate;             // 系统日期
    int           iSno;                 // 申请序号
    char          szMarket[32 + 1];     // 交易市场
    LONGLONG      llFundid;             // 资金帐户
    char          szOfcode[8 + 1];      // 基金认购代码
    char          szBsflag[32 + 1];     // 认购类型
    char          szCancelflag[32 + 1]; // 撤单标识
    char          szStatus[32 + 1];     // 状态
};
struct MATRADEAPI CRspStkQryRpcETFOfflineApplicationField
{
    int           iSysdate;                 // 系统日期
    int           iSno;                     // 申请序号
    int           iOperdate;                // 申请物理日期
    int           iOpertime;                // 申请物理时间
    char          szOfcode[8 + 1];          // 基准基金代码
    char          szOfname[20 + 1];         // 基准基金名称
    char          chMoneytype;              // 币种
    char          chSecumarket;             // 股东市场
    char          szSecuid[10 + 1];         // 股东
    char          chMarket;                 // 基金市场
    char          szStkcode[8 + 1];         // 基金认购代码
    char          szStkname[10 + 1];        // 基金认购代码名称
    char          chInputbsflag;            // 认购类型
    int           iOrderqty;                // 委托数量
    char          szOrderfrzamt[21 + 1];    // 委托冻结金额
    char          szCommissionrate[21 + 1]; // 费用比例
    char          szBillfee[21 + 1];        // 单费
    char          chCancelflag;             // 撤单标识
    char          chStatus;                 // 处理状态
    char          szFrzcode[10 + 1];        // 股份认购冻结代码
    char          szFrzname[8 + 1];         // 股份认购冻结代码名称
    int           iOrderdate;               // 申购委托日期
    int           iOrdersno;                // 申购委托委托序号
};
//-------------------------------10101630:深圳LOF基金分拆合并信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzLOFFundSplitMergeField
{
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcSzLOFFundSplitMergeField
{
    char          chMarket;            // 市场
    char          szStkcode[6 + 1];    // 证券代码
    char          szStkname[8 + 1];    // 证券名称
    char          szMststkcode[6 + 1]; // 母基金证券代码
    char          szMststkname[8 + 1]; // 母基金证券名称
    char          chStatus;            // 分拆合并状态
    char          szComrate[21 + 1];   // 组合比例
    int           iUnitcomqty;         // 最小合并单位
    int           iUnitsegqty;         // 最小分拆单位
    int           iMincomqty;          // 最低合并数量
    int           iMinsegqty;          // 最低分拆数量
    LONGLONG      llMaxcomqty;         // 最高合并数量
    LONGLONG      llMaxsegqty;         // 最高分拆数量
};
//-------------------------------10101631:上海LOF基金分拆合并信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcShLOFFundSplitMergeField
{
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcShLOFFundSplitMergeField
{
    char          chMarket;            // 市场
    char          szStkcode[6 + 1];    // 证券代码
    char          szStkname[8 + 1];    // 证券名称
    char          szMststkcode[6 + 1]; // 母基金证券代码
    char          szMststkname[8 + 1]; // 母基金证券名称
    char          szBusistatus[8 + 1]; // 分拆合并状态
    int           iCoefficient;        // 转换系数
    int           iUnitqty;            // 分拆合并整手数
    int           iMincomqty;          // 最低合并数量
    int           iMinsegqty;          // 最低分拆数量
};
//-------------------------------10101632:盘后基金分拆/合并根据昨日申购额得到的建议值--------------------------
struct MATRADEAPI CReqStkRpcAfterCloseFundSplitMergeRecommendedValueField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金帐户
    char          szBsflag[2 + 1];  // 买卖类别
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkRpcAfterCloseFundSplitMergeRecommendedValueField
{
    LONGLONG      llAdviceqty; // 股份数量
    char          chIsmccust;  // 盲拆客户
};
//-------------------------------10101633:查询客户国债预发行持仓明细--------------------------
struct MATRADEAPI CReqStkQryRpcTreasuryBondsPreIssuePosiField
{
    char          chMarket;         // 市场代码
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金账号
    int           iTrddate;         // 开仓日期
    char          szPosstr[32 + 1]; // 定位串
    int           iCount;           // 请求行数
};
struct MATRADEAPI CRspStkQryRpcTreasuryBondsPreIssuePosiField
{
    char          szPosstr[32 + 1];        // 定位串
    int           iServerid;               // 机器编码
    int           iTrddate;                // 开仓日期
    char          szOrderid[10 + 1];       // 合同序号
    LONGLONG      llCustid;                // 客户代码
    char          chMoneytype;             // 货币代码
    LONGLONG      llFundid;                // 资金账号
    char          szSecuid[10 + 1];        // 股东代码
    int           iMatchtime;              // 成交时间
    char          szMatchcode[32 + 1];     // 成交编号
    char          chMarket;                // 市场代码
    char          szStkcode[8 + 1];        // 证券代码
    LONGLONG      llPosiqty;               // 原始持仓数量
    LONGLONG      llLastcloseqty;          // 前平仓累计数量
    LONGLONG      llRealcloseqty;          // 实时平仓累计数量
    LONGLONG      llDaycloseqty;           // 当日平仓累计数量
    LONGLONG      llRemianposiqty;         // 剩余持仓数量
    char          szPosiprice[21 + 1];     // 持仓价格
    char          szDeposit[21 + 1];       // 履约保证金
    char          szRemaindeposit[21 + 1]; // 剩余履约保证金
    char          szMarginamt[21 + 1];     // 价差保证金累计
    char          szCustrate[21 + 1];      // 原始履约金比例
};
//-------------------------------10101634:查询客户国债预发行平仓明细--------------------------
struct MATRADEAPI CReqStkQryRpcTreasuryBondsPreIssueClosePosiField
{
    char          chMarket;         // 市场代码
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金账号
    int           iClosedate;       // 平仓日期
    char          szPosstr[64 + 1]; // 定位串
    int           iCount;           // 请求行数
};
struct MATRADEAPI CRspStkQryRpcTreasuryBondsPreIssueClosePosiField
{
    char          szPosstr[32 + 1];       // 定位串
    int           iServerid;              // 机器编码
    char          szOrgid[4 + 1];         // 机构编码
    int           iOpendate;              // 开仓日期
    char          szOrderid[10 + 1];      // 开仓合同序号
    LONGLONG      llCustid;               // 客户号
    char          chMoneytype;            // 货币类型
    LONGLONG      llFundid;               // 资金账号
    char          szSecuid[10 + 1];       // 股东代码
    char          szMatchcode[32 + 1];    // 开仓成交编号
    char          chMarket;               // 市场代码
    char          szStkcode[8 + 1];       // 证券代码
    int           iClosedate;             // 平仓日期
    char          szCloseorderid[16 + 1]; // 平仓合同序号
    char          szClosecode[32 + 1];    // 平仓成交编号
    LONGLONG      llCloseqty;             // 平仓数量
    char          szCloseprice[21 + 1];   // 平仓价格
    char          szBackdeposit[21 + 1];  // 返还履约保证金
    char          szMarginamt[21 + 1];    // 价差保证金
    char          szProfitamt[21 + 1];    // 收益金额
};
//-------------------------------10101635:查询客户国债预发行应付履约金明细--------------------------
struct MATRADEAPI CReqStkQryRpcTreasuryBondsPreIssuePerformanceBondField
{
    LONGLONG      llFundid;         // 资产账户
    char          chMarket;         // 市场代码
    char          szStkcode[8 + 1]; // 证券代码
    char          szPosstr[64 + 1]; // 定位串
    int           iCount;           // 请求行数
};
struct MATRADEAPI CRspStkQryRpcTreasuryBondsPreIssuePerformanceBondField
{
    char          szPosstr[32 + 1];     // 定位串
    int           iServerid;            // 机器编码
    char          szOrgid[4 + 1];       // 机构编码
    char          chMarket;             // 市场代码
    char          szStkcode[8 + 1];     // 证券代码
    int           iFundid;              // 资产账户
    char          szOrderid[10 + 1];    // 合约号
    char          chInvitebids;         // 招标方式
    int           iRemainposiqty;       // 持仓数量
    char          szPaymentamt[21 + 1]; // 应支付履约金
};
//-------------------------------10101636:网络投票股东大会信息查询(440001)--------------------------
struct MATRADEAPI CReqStkQryRpcNetworkVotingTrdacctsConferenceInfoField
{
    char          chMarket;             // 市场
    char          szCompanycode[8 + 1]; // 公司代码
    char          szVotecode[8 + 1];    // 产品代码
};
struct MATRADEAPI CRspStkQryRpcNetworkVotingTrdacctsConferenceInfoField
{
    char          szCompanycode[8 + 1];      // 公司代码
    char          szCompanyinfo[64 + 1];     // 公司简称
    char          szMeetingdatebegin[8 + 1]; // 股东大会开始日
    char          szMeetingdateend[8 + 1];   // 股东大会结束日
    char          szMeetingtype[4 + 1];      // 股东大会类型
    char          szMeetingdesc[500 + 1];    // 股东大会名称
    char          chTpmarket;                // 投票市场
    char          szMeetingseq[10 + 1];      // 股东大会编码
    char          chMarket;                  // 交易市场
    char          szVotecode[8 + 1];         // 产品代码
    char          szSabbr[64 + 1];           // 股票简称
    char          szStype[24 + 1];           // 股份类别
    char          szRegdate[8 + 1];          // 股权登记日
    char          szLasttradedate[8 + 1];    // 最后交易日
};
//-------------------------------10101637:网络投票议案信息查询(440002)--------------------------
struct MATRADEAPI CReqStkQryRpcNetworkVotingMotionInfoField
{
    char          chTpmarket;        // 投票市场
    char          chMeetingseq;      // 股东大会编码
    char          szVid[10 + 1];     // 议案编号
    char          szVotecode[8 + 1]; // 产品代码
};
struct MATRADEAPI CRspStkQryRpcNetworkVotingMotionInfoField
{
    char          chTpmarket;           // 投票市场
    char          szMeetingseq[10 + 1]; // 股东大会编码
    char          szVid[10 + 1];        // 议案编号
    char          szVinfo[1024 + 1];    // 议案名称
    char          szVtype[4 + 1];       // 议案类型
    char          szVleijino[4 + 1];    // 累积投票应选数
    char          szVrole[50 + 1];      // 股东身份
    char          szVrelation[100 + 1]; // 议案关系
    char          szRefcode[255 + 1];   // 投票产品代码指引
};
//-------------------------------10101638:网络投票结果查询(440003)--------------------------
struct MATRADEAPI CReqStkQryRpcNetworkVotingResultField
{
    char          chQueryflag;       // 查询标识
    int           iBegindate;        // 归档开始日期
    int           iEnddate;          // 归档结束日期
    char          chTpmarket;        // 投票市场
    char          chMeetingseq;      // 股东大会编码
    char          szVid[10 + 1];     // 议案编号
    char          chMarket;          // 市场
    char          szVotecode[8 + 1]; // 产品代码
};
struct MATRADEAPI CRspStkQryRpcNetworkVotingResultField
{
    char          chTpmarket;        // 投票市场
    char          chMeetingseq;      // 股东大会编码
    LONGLONG      llCustid;          // 客户代码
    char          chMarket;          // 市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szVotecode[8 + 1]; // 产品代码
    LONGLONG      llVoteagree;       // 赞成
    LONGLONG      llVoteagainst;     // 反对
    LONGLONG      llVotecancel;      // 弃权
    char          szVid[10 + 1];     // 议案编号
    char          szVinfo[100 + 1];  // 议案名称
    char          szVtype[4 + 1];    // 议案类型
};
//-------------------------------10101639:网络投票(440101)--------------------------
struct MATRADEAPI CReqStkRpcNetworkVotingField
{
    LONGLONG      llFundid;         // 资金账户
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          chTpmarket;       // 投票市场
    char          chMeetingseq;     // 股东大会编码
    char          chVid;            // 议案编号
    LONGLONG      llQty;            // 投票数量
    char          chResult;         // 投票意见
    int           iOrdergroup;      // 委托批号
};
struct MATRADEAPI CRspStkRpcNetworkVotingField
{
};
//-------------------------------10101640:网络投票委托查询(440103)--------------------------
struct MATRADEAPI CReqStkQryRpcNetworkVotingField
{
    int           iBegindate;       // 开始日期
    int           iEnddate;         // 结束日期
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          chTpmarket;       // 投票市场
    char          chMeetingseq;     // 股东大会编码
    char          szVid[10 + 1];    // 议案编号
};
struct MATRADEAPI CRspStkQryRpcNetworkVotingField
{
    int           iOrdersno;            // 委托序号
    int           iOrdergroup;          // 批量委托批号
    char          szOrderid[10 + 1];    // 申报合同序号
    int           iOrderdate;           // 委托交易日期
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          szOrgid[4 + 1];       // 分支机构
    char          szBrhid[4 + 1];       // 机构分支
    LONGLONG      llFundid;             // 资产帐户
    char          chMoneytype;          // 货币代码
    char          szSecuid[10 + 1];     // 股东代码
    char          chMarket;             // 交易市场
    char          szSeat[6 + 1];        // 报盘席位
    char          chChannel;            // 通道号
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    char          chStktype;            // 证券类别
    char          szBsflag[2 + 1];      // 买卖方向
    char          chTpmarket;           // 投票市场
    char          szMeetingseq[12 + 1]; // 股东大会编码
    char          szVid[12 + 1];        // 投票议案编号
    char          chVoteresult;         // 投票意见
    LONGLONG      llVoteqty;            // 投票数量
    char          szSegment[4 + 1];     // 分段统计段号
    LONGLONG      llLongqty;            // 总股份数量，
    int           iReporttime;          // 申报时间
    char          chOrderstatus;        // 委托状态
    char          chSendstatus;         // 发送状态
    char          szRemark[128 + 1];    // 其他备注
};
//-------------------------------10101641:标准券汇总查询--------------------------
struct MATRADEAPI CReqStkQryRpcStdBondSummaryField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcStdBondSummaryField
{
    char          szPoststr[32 + 1];  // 定位串
    int           iServerid;          // 节点编号
    char          szOrgid[4 + 1];     // 机构编码
    LONGLONG      llCustid;           // 客户代码
    LONGLONG      llFundid;           // 资金账户
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    char          szStkcode[8 + 1];   // 证券代码
    char          chStktype;          // 证券类别
    char          chMoneytype;        // 货币代码
    char          szExchrate[21 + 1]; // 折算率
    LONGLONG      llBondassetavl;     // 质押券可用
    LONGLONG      llBondunfrz;        // 标准券解冻
    LONGLONG      llBondfrz;          // 标准券冻结
    LONGLONG      llRemainqty;        // 标准券可用
};
//-------------------------------10101642:质押库标准券明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcPledgeBankStdBondDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcPledgeBankStdBondDetailField
{
    char          szPoststr[32 + 1]; // 定位串
    char          szOrgid[4 + 1];    // 机构编码
};
//-------------------------------10101643:回购资金占用天数查询--------------------------
struct MATRADEAPI CReqStkQryRpcRepurchaseFundOccupancyDaysField
{
    char          szStkcode[8 + 1]; // 证券代码
    char          chMarket;         // 市场代码
};
struct MATRADEAPI CRspStkQryRpcRepurchaseFundOccupancyDaysField
{
    int           iJxdays;      // 计息天数
    int           iJxbegindate; // 开始计算日期
};
//-------------------------------10101644:未到期回购查询--------------------------
struct MATRADEAPI CReqStkQryRpcUndueRepurchaseField
{
    char          szOrgid[4 + 1];    // 机构编码
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chDatetype;        // 日期类型
    int           iBegindate;        // 开始日期
    int           iEnddate;          // 结束日期
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcUndueRepurchaseField
{
    char          szPoststr[32 + 1];    // 定位串
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llFundid;             // 资金账号
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    char          chMoneytype;          // 货币
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    char          szSeat[6 + 1];        // 交易席位
    char          szSecuid[10 + 1];     // 股东代码
    int           iOrderdate;           // 委托日期
    int           iMatchdate;           // 购回日期
    char          szOrderid[10 + 1];    // 合同序号
    char          szMatchcode[20 + 1];  // 成交号码
    char          szOrderprice[21 + 1]; // 委托价格
    int           iOrderqty;            // 委托数量
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          szClearedamt[21 + 1]; // 购回金额
    char          szBrhid[4 + 1];       // 机构分支
    char          chStktype;            // 证券类别
    char          szBsflag[2 + 1];      // 买卖类别
    char          chTrdid;              // 交易类型
    char          szBusitype[4 + 1];    // 业务类别
    char          chOperway;            // 操作方式
    char          chStatus;             // 状态
    char          szFeeSxf[21 + 1];     // 购回交易佣金
    char          chBreakflag;          // 违约标志
    char          szDiscratio[21 + 1];  // 折扣比例
    int           iBbMatchdate;         // 资金占用天数
};
//-------------------------------10101645:客户融资回购风险查询--------------------------
struct MATRADEAPI CReqStkQryRpcFinancingRepurchaseRiskField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          chMoneytype;      // 货币代码
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkQryRpcFinancingRepurchaseRiskField
{
    char          szOrgid[4 + 1];           // 机构编码
    char          szBrhid[4 + 1];           // 分支编码
    LONGLONG      llFundid;                 // 资金账号
    LONGLONG      llCustid;                 // 客户代码
    char          chMarket;                 // 交易市场
    char          chMoneytype;              // 货币代码
    char          szSecuid[10 + 1];         // 证券账号
    char          szRate[21 + 1];           // 标准券使用比例
    char          szScale[21 + 1];          // 融资放大倍数
    char          szBondbal[21 + 1];        // 融资质押总数
    char          szRemainqty[21 + 1];      // 标准券余额
    char          szClearmat[21 + 1];       // 未到期回购资金
    char          szFundassettotal[21 + 1]; // 客户总资产(总资产按客户代码统计，不以资金帐号统计)
    char          szMaxamtRz[21 + 1];       // 最大可再融资金额
    LONGLONG      llMaxamtCk;               // 最大可再出库数量
    char          szMaxamtZc[21 + 1];       // 最大可再转出金额
};
//-------------------------------10101646:查询客户报价回购品种信息--------------------------
struct MATRADEAPI CReqStkQryRpcShQuoteRepurchaseProductInfoField
{
    char          szStkcode[8 + 1];   // 证券代码
    char          chMarket;           // 交易市场
    char          szProdcode[12 + 1]; // 产品编码
};
struct MATRADEAPI CRspStkQryRpcShQuoteRepurchaseProductInfoField
{
    char          chMarket;                   // 交易市场
    char          szStkcode[8 + 1];           // 证券代码
    char          szStkname[8 + 1];           // 证券名称
    char          szProdcode[12 + 1];         // 产品编码
    char          szProdname[64 + 1];         // 产品名称
    int           iGhDays;                    // 期限(天)
    int           iBuyunit;                   // 交易单位(张)
    int           iMinqty;                    // 交易最低数量
    char          szLastrate[21 + 1];         // 最新到期收益率
    char          szCallrate[21 + 1];         // 最新提前终止收益率
    char          szMaxbondamt[21 + 1];       // 总额度
    char          szBondbalamt[21 + 1];       // 额度余额
    char          szBondavlamt[21 + 1];       // 可用额度
    LONGLONG      llLastcontract;             // 昨日未到期
    LONGLONG      llBondbackbal;              // 当日融券回转可用数
    LONGLONG      llBondbrk;                  // 当日累计终止
    LONGLONG      llBondbrkunfrz;             // 当日累计终止成交
    char          chAutobuyflag;              // 自动展期标志
    char          chBuyflag;                  // 交易标志
    int           iContmaxdays;               // 最大展期天数
    char          szMinprebookbrkamt[21 + 1]; // 预约购回额度起点
    char          szRemark[128 + 1];          // 备注信息
};
//-------------------------------10101647:提前购回--------------------------
struct MATRADEAPI CReqStkRpcShQuoteRepurchaseAdvanceRepurchaseField
{
    char          chMarket;            // 交易市场
    LONGLONG      llFundid;            // 资金帐户
    int           iBrkqty;             // 提前终止数量
    int           iMatchdate;          // 成交日期
    char          szMatchcode[32 + 1]; // 成交编号
    char          chAction;            // 操作标志
};
struct MATRADEAPI CRspStkRpcShQuoteRepurchaseAdvanceRepurchaseField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101648:报价回购预约--------------------------
struct MATRADEAPI CReqStkRpcShQuoteRepurchaseReservationField
{
    LONGLONG      llFundid;            // 资金帐户
    char          chMarket;            // 交易市场
    int           iMatchdate;          // 成交日期
    char          szMatchcode[32 + 1]; // 成交编号
    int           iExecdate;           // 执行日期
    char          chAction;            // 操作类型
    LONGLONG      llMatchqty;          // 委托数量
};
struct MATRADEAPI CRspStkRpcShQuoteRepurchaseReservationField
{
    int           iSno; // 操作流水号
};
//-------------------------------10101649:报价回购预约流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcShQuoteRepurchaseReservationStatementField
{
    LONGLONG      llFundid;            // 资金账号
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    char          szStkcode[8 + 1];    // 证券代码
    char          szProdcode[12 + 1];  // 产品编码
    char          chMoneytype;         // 货币代码
    char          szMatchcode[32 + 1]; // 成交编号
    char          chDatetype;          // 日期类型
    int           iBegindate;          // 起始日期
    int           iEnddate;            // 结束日期
    int           iCount;              // 请求行数
    char          szPosstr[64 + 1];    // 定位串
};
struct MATRADEAPI CRspStkQryRpcShQuoteRepurchaseReservationStatementField
{
    char          szPosstr[64 + 1];     // 定位串
    int           iServerid;            // 节点编号
    int           iSysdate;             // 系统日期
    int           iSno;                 // 流水号
    int           iOperdate;            // 申请预约日期
    int           iOpertime;            // 申请预约时间
    int           iExecdate;            // 提前终止执行日期
    char          szOrgid[4 + 1];       // 机构编码
    char          szBrhid[4 + 1];       // 分支机构
    LONGLONG      llFundid;             // 资金账号
    LONGLONG      llCustid;             // 客户代码
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 证券账号
    char          szStkcode[8 + 1];     // 证券代码
    char          chMoneytype;          // 货币代码
    LONGLONG      llMatchqty;           // 提前终止数量
    char          szMatchamt[21 + 1];   // 提前终止额度
    char          szPrebookamt[21 + 1]; // 预约额度本次余额
    int           iMatchdate;           // 回购成效日期
    char          szMatchcode[32 + 1];  // 回购成交编号
    char          szRemark[32 + 1];     // 备用
    char          chStatus;             // 状态
    int           iOrderdate;           // 委托日期
    int           iOrdersno;            // 委托序号
    char          szProdcode[12 + 1];   // 产品编码
};
//-------------------------------10101650:查询客户未到期的报价回购信息--------------------------
struct MATRADEAPI CReqStkQryRpcShQuoteRepurchaseUndueRepurchaseField
{
    char          szStkcode[8 + 1];   // 证券代码
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
    char          chDatetype;         // 日期类型
    int           iBegindate;         // 开始日期
    int           iEnddate;           // 结束日期
    char          chQrytype;          // 查询类型
    char          szProdcode[12 + 1]; // 产品编码
};
struct MATRADEAPI CRspStkQryRpcShQuoteRepurchaseUndueRepurchaseField
{
    char          chMarket;             // 交易市场
    char          szOrgid[4 + 1];       // 机构编码
    char          szBrhid[4 + 1];       // 分支机构
    LONGLONG      llCustid;             // 客户代码
    LONGLONG      llFundid;             // 资金账号
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    char          szProdcode[12 + 1];   // 产品编码
    char          szProdname[64 + 1];   // 产品名称
    char          szLastrate[21 + 1];   // 到期收益率
    char          szCallrate[21 + 1];   // 提前终止收益率
    char          szMaxbondamt[21 + 1]; // 总额度
    char          szBondbalamt[21 + 1]; // 额度余额
    char          szBondavlamt[21 + 1]; // 额度可用
    char          chBreakautoflag;      // 自动展期标志(预约)
    int           iBreakautodate;       // 展期终止日期(预约)
    int           iSno;                 // 预约流水号
    int           iSysdate;             // 预约日期
    char          szLastraten[21 + 1];  // 最新到期收益率
    char          szCallraten[21 + 1];  // 最新提前终止收益率
    LONGLONG      llMatchqty;           // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    int           iOrderdate;           // 委托日期
    int           iMatchdate;           // 到期日期
    char          szMatchcode[32 + 1];  // 成交编号
    char          chAutoflag;           // 自动展期标志
    LONGLONG      llRemainqty;          // 未提前终止数量
    LONGLONG      llContqty;            // 自动展期数量
    LONGLONG      llCancontqty;         // 可展期数量
    char          chBrkcontflag;        // 终止展期标志
    char          chBreakflag;          // 违约标志
    char          szDiscratio[21 + 1];  // 折扣比例
};
//-------------------------------10101651:报价回购对应质押物明细--------------------------
struct MATRADEAPI CReqStkRpcShQuoteRepurchasePledgeDetailField
{
    LONGLONG      llFundid;            // 资金账号
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 股东代码
    char          szStkcode[8 + 1];    // 证券代码
    int           iMatchdate;          // 成交日期
    char          szMatchcode[32 + 1]; // 成交编号
    char          szProdcode[12 + 1];  // 产品编码
    int           iCount;              // 请求行数
    char          szPosstr[64 + 1];    // 定位串
};
struct MATRADEAPI CRspStkRpcShQuoteRepurchasePledgeDetailField
{
    char          szPosstr[64 + 1];    // 定位串
    int           iServerid;           // 节点编号
    char          szOrgid[4 + 1];      // 机构编码
    char          szBrhid[4 + 1];      // 分支机构
    LONGLONG      llFundid;            // 资金账号
    LONGLONG      llCustid;            // 客户代码
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 证券账号
    char          szStkcode[8 + 1];    // 证券代码
    char          szProdcode[12 + 1];  // 产品编码
    char          szSeat[6 + 1];       // 席位代码
    char          szZywdm[8 + 1];      // 质押物代码
    char          chZywlx;             // 质押物类型
    LONGLONG      llStkbal;            // 股份余额
    int           iMatchdate;          // 成交日期
    char          szMatchcode[32 + 1]; // 成交编号
    char          szTargetseat[6 + 1]; // 清算编号
    char          szSjlx[3 + 1];       // 数据类型
    char          chLtlx;              // 流通类型
    char          szQylb[2 + 1];       // 权益类别
    char          szGpnf[4 + 1];       // 挂牌年份
    char          szFzdm[3 + 1];       // 辅助代码
    char          szText[64 + 1];      // 补充说明
    char          szRemark[32 + 1];    // 备用
};
//-------------------------------10101652:上海报价回购质权信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcShQuoteRepurchasePledgeInfoField
{
};
struct MATRADEAPI CRspStkQryRpcShQuoteRepurchasePledgeInfoField
{
    LONGLONG      llStdbond;          // 标准券总额
    char          szUndueamt[21 + 1]; // 未到期回购总额
};
//-------------------------------10101653:报价回购批量委托保留额度查询--------------------------
struct MATRADEAPI CReqStkQryRpcShQuoteRepurchaseBatchOrderRetentionLimitField
{
    char          chMarket;         // 交易市场
};
struct MATRADEAPI CRspStkQryRpcShQuoteRepurchaseBatchOrderRetentionLimitField
{
    int           iServerid;           // 节点编号
    char          szOrgid[4 + 1];      // 机构编码
    char          chMarket;            // 交易市场
    LONGLONG      llCustid;            // 客户代码
    char          szSecuid[10 + 1];    // 证券账号
    char          szReservamt[21 + 1]; // 保留金额
    int           iBegindate;          // 生效日期
    int           iEnddate;            // 失效日期
};
//-------------------------------10101654:报价回购批量委托保留额度设置--------------------------
struct MATRADEAPI CReqStkRpcShQuoteRepurchaseSetBatchOrderRetentionLimitField
{
    char          szReservamt[21 + 1]; // 保留金额
    char          szSecuid[10 + 1];    // 证券账号
    char          chMarket;            // 交易市场
    char          chAction;            // 操作类型
    int           iBegindate;          // 生效日期
    int           iEnddate;            // 失效日期
};
struct MATRADEAPI CRspStkRpcShQuoteRepurchaseSetBatchOrderRetentionLimitField
{
};
//-------------------------------10101655:深圳报价回购产品信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseAdvanceRepurchaseField
{
    char          szProdcode[12 + 1]; // 产品编码
    char          chIsvalid;          // 产品状态
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseAdvanceRepurchaseField
{
    char          chMarket;                   // 交易市场
    char          szStkcode[8 + 1];           // 证券代码
    char          szProdcode[12 + 1];         // 产品编码
    char          szProdname[64 + 1];         // 产品名称
    char          chProdmode;                 // 产品模式
    char          chProdtype;                 // 产品类型
    int           iGhDays;                    // 购回天数
    int           iBuyunit;                   // 买入单位
    int           iMinqty;                    // 交易最低数量
    char          szLastrate[21 + 1];         // 到期年收益率
    char          szCallrate[21 + 1];         // 提前终止收益率
    char          szMaxbondamt[21 + 1];       // 产品总发行额总额度
    char          szBondbalamt[21 + 1];       // 额度余额
    char          szBondavlamt[21 + 1];       // 可用额度
    char          szNextLastrate[21 + 1];     // 下轮到期年收益率
    char          szNextCallrate[21 + 1];     // 下轮提前终止收益率
    LONGLONG      llLastcontract;             // 己占用额度
    LONGLONG      llBondbackbal;              // 当日到期不展期数量累计
    LONGLONG      llBondbrkunfrz;             // 日间购回数量累计
    char          chAutobuyflag;              // 自动展期标志
    char          chBuyflag;                  // 交易标志
    char          szMinprebookbrkamt[21 + 1]; // 预约购回额度起点
    char          chLxzqFlag;                 // 利息展期标志
    LONGLONG      llRemainedminqty;           // 购回剩余最小数量
    int           iBegindate;                 // 发行日期
    int           iEnddate;                   // 到期日期
    int           iRemaindays;                // 剩余天数
    char          chIsbuy;                    // 是否可正常交易
    char          chIsprebook;                // 是否可预约
    char          szRemark[128 + 1];          // 备注信息
};
//-------------------------------10101656:深圳报价回购预约--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseReservationField
{
    LONGLONG      llFundid;            // 资金帐户
    char          chMarket;            // 交易市场
    char          szSecuid[10 + 1];    // 证券账号
    char          szStkcode[6 + 1];    // 证券代码
    char          szProdcode[12 + 1];  // 产品编码
    char          chPrebooktype;       // 预约类型
    LONGLONG      llMatchqty;          // 预约数量
    int           iMatchdate;          // 原合约成交日期
    char          szMatchcode[32 + 1]; // 原合约成交编号
    int           iExecdate;           // 执行日期
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseReservationField
{
    int           iSno; // 预约流水号
};
//-------------------------------10101657:深圳报价回购预约取消--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseReservationCancelField
{
    LONGLONG      llFundid;         // 资金帐户
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 证券账号
    int           iSysdate;         // 预约日期
    int           iSno;             // 预约流水号
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseReservationCancelField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101658:深圳报价回购预约执行--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseReservationExecutionField
{
    LONGLONG      llFundid;         // 资金帐户
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 证券账号
    int           iSysdate;         // 预约日期
    int           iSno;             // 预约流水号
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseReservationExecutionField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101659:深圳报价回购提前购回--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseAdvanceRepurchaseField
{
    char          chMarket;            // 交易市场
    LONGLONG      llFundid;            // 资金帐户
    int           iBrkqty;             // 提前终止数量
    int           iMatchdate;          // 成交日期
    char          szMatchcode[32 + 1]; // 成交编号
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseAdvanceRepurchaseField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101660:报价回购预约流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseReservationStatementField
{
    LONGLONG      llFundid;           // 资金账号
    char          szProdcode[12 + 1]; // 产品编码
    char          chPrebooktype;      // 预约类型
    char          szStatus[8 + 1];    // 预约状态
    int           iBegindate;         // 预约开始日期
    int           iEnddate;           // 预约结束日期
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseReservationStatementField
{
    int           iSysdate;           // 预约日期
    int           iSno;               // 预约流水号
    int           iExecdate;          // 预约执行日期
    char          szOrgid[4 + 1];     // 机构编码
    char          szBrhid[4 + 1];     // 分支机构
    LONGLONG      llFundid;           // 资金账号
    LONGLONG      llCustid;           // 客户代码
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 证券账号
    char          szStkcode[8 + 1];   // 证券代码
    char          szProdcode[12 + 1]; // 产品编码
    char          chPrebooktype;      // 预约类型
    LONGLONG      llMatchqty;         // 预约数量
    char          szMatchamt[21 + 1]; // 预约金额
    char          chStatus;           // 状态
};
//-------------------------------10101661:报价回购合约数量调整--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseChgContractQtyField
{
    char          chMarket;            // 交易市场
    char          szOrgid[4 + 1];      // 机构编码
    LONGLONG      llFundid;            // 资金账号
    char          szMatchcode[32 + 1]; // 成交编号
    int           iMatchdate;          // 成交日期
    LONGLONG      llAdjustqty;         // 调整数量
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseChgContractQtyField
{
};
//-------------------------------10101662:深圳报价回购展期取消--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseExtensionCancelField
{
    char          chMarket;            // 交易市场
    char          szOrgid[4 + 1];      // 机构编码
    LONGLONG      llFundid;            // 资金账号
    int           iMatchdate;          // 合约成交日期
    char          szMatchcode[32 + 1]; // 合约成交编号
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseExtensionCancelField
{
};
//-------------------------------10101663:查询深圳客户未到期的报价回购信息--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseUndueRepurchaseField
{
    LONGLONG      llFundid;           // 资金账号
    int           iBegindate;         // 合约生成日期
    int           iEnddate;           // 合约生成日期
    char          szContflag[16 + 1]; // 合约展期标志
    char          szProdcode[12 + 1]; // 产品编码
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseUndueRepurchaseField
{
    char          chMarket;              // 交易市场
    char          szOrgid[4 + 1];        // 机构编码
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账号
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkcode[8 + 1];      // 证券代码
    char          szProdcode[12 + 1];    // 产品编码
    char          szProdname[64 + 1];    // 产品名称
    char          szLastrate[21 + 1];    // 到期收益率
    char          szCallrate[21 + 1];    // 提前终止收益率
    int           iOrderdate;            // 委托日期
    int           iMatchdate;            // 成交日期
    char          szMatchcode[32 + 1];   // 成交编号
    int           iEnddate;              // 到期日期
    LONGLONG      llMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];    // 成交金额
    LONGLONG      llBackqty;             // 已购回数量
    LONGLONG      llBackqtyReal;         // 实时购回数量
    LONGLONG      llRemainqty;           // 未购回数量
    char          szRemainamt[21 + 1];   // 未购回金额
    char          szFundintCalc[21 + 1]; // 预计利息
    char          chContflag;            // 展期标志
    LONGLONG      llContqty;             // 可展期数量
};
//-------------------------------10101664:查询深圳报价回购质押折算率--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchasePledgeConversionRateField
{
    char          chMarket;         // 市场
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchasePledgeConversionRateField
{
    char          chMarket;           // 市场
    char          szStkcode[8 + 1];   // 证券代码
    char          szExchrate[21 + 1]; // 质押折算率
    char          szNav[21 + 1];      // 基金净值
};
//-------------------------------10101665:报价回购现金质押查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseCashPledgeField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户代码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseCashPledgeField
{
    int           iServerid;                // 机器编码
    char          szOrgid[4 + 1];           // 机构编码
    LONGLONG      llFundid;                 // 资金账号
    LONGLONG      llCustid;                 // 客户代码
    char          chMarket;                 // 交易市场
    char          szSecuid[10 + 1];         // 证券账号
    char          szBondamtlastbal[21 + 1]; // 上次余额
    char          szBondamtbal[21 + 1];     // 担保库余额
    char          szBondamtavl[21 + 1];     // 担保库可用
    char          szPosstr[64 + 1];         // 定位串
};
//-------------------------------10101666:深圳报价回购质权信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseMortgageField
{
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseMortgageField
{
    char          szStkvalue[21 + 1]; // 质押券总市值
    char          szFundavl[21 + 1];  // 入库资金余额
    LONGLONG      llStdbond;          // 标准券总额
    LONGLONG      llStdbondavl;       // 实时标准券余额
    char          szUndueamt[21 + 1]; // 未到期回购总额
};
//-------------------------------10101667:查询深圳报价回购客户特定报价--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchaseSpecificOfferField
{
    char          szOrgid[4 + 1];     // 机构编码
    LONGLONG      llFundid;           // 资金账号
    char          szProdcode[12 + 1]; // 产品编码
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchaseSpecificOfferField
{
    char          szOrgid[4 + 1];     // 机构编码
    LONGLONG      llFundid;           // 资金账号
    char          szProdcode[12 + 1]; // 产品编码
    char          szLastrate[21 + 1]; // 到期收益率
    char          szCallrate[21 + 1]; // 提前终止收益率
};
//-------------------------------10101668:客户基金投资适当性信息维护--------------------------
struct MATRADEAPI CReqStkRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriatenessField
{
    char          szOrgid[4 + 1];           // 机构编码
    LONGLONG      llCustid;                 // 客户代码
    char          chInvestmentperiod;       // 投资期限
    char          szInvestmenttype[32 + 1]; // 投资品种
    char          szRemark[128 + 1];        // 备注
};
struct MATRADEAPI CRspStkRpcSzQuoteRepurchaseMaintenanceFundInvestmentAppropriatenessField
{
};
//-------------------------------10101669:报价回购质押物查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzQuoteRepurchasePledgeField
{
    char          chMarket;          // 交易市场
    char          szOrgid[4 + 1];    // 机构编码
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcSzQuoteRepurchasePledgeField
{
    char          szPoststr[64 + 1];    // 定位串
    int           iServerid;            // 节点编号
    LONGLONG      llCustid;             // 客户代码
    char          szOrgid[5 + 1];       // 机构代码
    LONGLONG      llFundid;             // 资金帐户
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    char          szSeat[6 + 1];        // 席位代码
    char          szStkcode[8 + 1];     // 证券代码
    char          chStktype;            // 证券类别
    LONGLONG      llBondlastbal;        // 上次余额
    LONGLONG      llBondbal;            // 质押余额
    LONGLONG      llBondavl;            // 质押可用
    char          szExchrate[21 + 1];   // 折算率
    char          szStdbondavl[21 + 1]; // 折算成标准券可用
};
//-------------------------------10101670:债券协议回购委托申报--------------------------
struct MATRADEAPI CReqStkRpcBondAgreementRepurchaseOrderField
{
    char          chMarket;            // 交易市场
    LONGLONG      llFundid;            // 资产帐户
    char          szSecuid[10 + 1];    // 股东代码
    char          szStkcode[8 + 1];    // 证券代码
    char          szBsflag[2 + 1];     // 买卖类别
    LONGLONG      llOrderqty;          // 委托数量
    char          szOrderamt[21 + 1];  // 融资金额
    int           iGhdays;             // 产品期限
    char          szDuerate[21 + 1];   // 年利率
    char          szTargetseat[6 + 1]; // 对方交易单元
    char          szConfernum[8 + 1];  // 约定号
    char          szZqzysno[32 + 1];   // 初始合约号
    LONGLONG      llRisksignsno;       // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcBondAgreementRepurchaseOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101671:债券协议回购委托撤单申报--------------------------
struct MATRADEAPI CReqStkRpcBondAgreementRepurchaseCancelField
{
    char          chMarket;         // 交易市场
    int           iOrderdate;       // 委托日期
    int           iOrdersno;        // 委托序号
    LONGLONG      llFundid;         // 资金帐号
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkRpcBondAgreementRepurchaseCancelField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101672:债券协议回购委托流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondAgreementRepurchaseStatementField
{
    char          chMarket;         // 交易市场
    LONGLONG      llFundid;         // 资金帐号
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    int           iBegindate;       // 委托日期(起)
    int           iEnddate;         // 委托日期(止)
    int           iOrdersno;        // 委托序号
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcBondAgreementRepurchaseStatementField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iOrderdate;            // 委托日期
    int           iOrdersno;             // 委托序号
    char          szOrderid[10 + 1];     // 合同序号
    LONGLONG      llFundid;              // 资金账户
    char          szSecuid[10 + 1];      // 股东代码
    char          szSeat[6 + 1];         // 交易席位
    char          chMarket;              // 交易市场
    char          szStkcode[8 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
    char          chMoneytype;           // 货币代码
    char          szBsflag[2 + 1];       // 买卖类别
    char          chOrderstatus;         // 委托状态
    char          chCancelstatus;        // 撤单状态
    char          chCancelflag;          // 撤单标识
    char          szOldorderid[10 + 1];  // 原合同序号
    int           iOrderqty;             // 委托数量
    int           iMatchqty;             // 成交数量
    int           iCancelqty;            // 撤单数量
    char          szOrderamt[21 + 1];    // 委托金额
    char          szOrderfrzamt[21 + 1]; // 冻结金额
    char          szMatchamt[21 + 1];    // 成交金额
    int           iGhdays;               // 购回期限
    char          szDuerate[21 + 1];     // 年利率
    char          szTargetseat[6 + 1];   // 对方席位
    char          szConfernum[8 + 1];    // 约定号
    char          szZqzysno[32 + 1];     // 初始合约号
    int           iReporttime;           // 报盘时间
    char          chOperway;             // 委托渠道
    int           iOpertime;             // 委托时间
    char          szRemark[64 + 1];      // 备注信息
};
//-------------------------------10101673:债券协议回购成交查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondAgreementRepurchaseMatchField
{
    char          chMarket;         // 交易市场
    LONGLONG      llFundid;         // 资金帐号
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    int           iBegindate;       // 成交日期(起)
    int           iEnddate;         // 成交日期(止)
    int           iOrdersno;        // 委托序号
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcBondAgreementRepurchaseMatchField
{
    char          szPoststr[32 + 1];   // 定位串
    int           iSysdate;            // 成交日期
    char          szSecuid[10 + 1];    // 股东代码
    char          chMarket;            // 交易市场
    char          szStkcode[8 + 1];    // 证券代码
    char          szStkname[8 + 1];    // 证券名称
    char          szBsflag[2 + 1];     // 买卖类别
    int           iOrdersno;           // 委托序号
    char          szOrderid[10 + 1];   // 合同序号
    int           iOrderqty;           // 委托数量
    char          szMatchcode[20 + 1]; // 成交序号
    char          chMatchtype;         // 成交类型
    int           iMatchqty;           // 成交数量
    char          szMatchamt[21 + 1];  // 成交金额
    int           iMatchtime;          // 成交时间
};
//-------------------------------10101674:债券协议回购合约查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondAgreementRepurchaseContractField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐号
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          szZqzysno[32 + 1]; // 初始合约号
    char          chQueryflag;       // 查询标志
    int           iCount;            // 请求行数
    char          szPosstr[64 + 1];  // 定位串
};
struct MATRADEAPI CRspStkQryRpcBondAgreementRepurchaseContractField
{
    char          szPoststr[32 + 1];   // 定位串
    char          szZqzysno[32 + 1];   // 初始合约号
    int           iSysdate;            // 系统日期
    LONGLONG      llFundid;            // 资金账号
    char          szSecuid[10 + 1];    // 股东代码
    char          szSeat[6 + 1];       // 席位代码
    char          chMarket;            // 市场代码
    char          szStkcode[8 + 1];    // 证券代码
    char          szStkname[8 + 1];    // 证券名称
    char          chMoneytype;         // 货币代码
    char          szOrderid[10 + 1];   // 合同序号
    char          szConfernum[8 + 1];  // 约定号
    char          szTargetseat[6 + 1]; // 对方席位
    int           iGhdays;             // 产品期限
    int           iEnddate;            // 预计购回日期
    char          szDuerate[21 + 1];   // 年利率
    LONGLONG      llMatchqty;          // 初始质押数量
    char          szMatchamt[21 + 1];  // 初始金额
    char          szMatchcode[32 + 1]; // 成交编号
    char          chStatus;            // 合约状态
    LONGLONG      llDebtqty;           // 合约数量
    char          szDebtamt[21 + 1];   // 合约金额
    char          chPledgorkind;       // 资金融入/融出标志
    char          szRemark[128 + 1];   // 备注信息
};
//-------------------------------10101675:密码服务委托申报--------------------------
struct MATRADEAPI CReqStkRpcPwdServiceOrderField
{
    char          chMarket;         // 交易市场
    LONGLONG      llFundid;         // 资产帐户
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          chValidationtype; // 密码激活类型
    int           iValidationcode;  // 密码激活校验号
};
struct MATRADEAPI CRspStkRpcPwdServiceOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101676:股东查询，查询客户对应的股东代码--------------------------
struct MATRADEAPI CReqStkQryRpcTrdacctField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcTrdacctField
{
    char          szPoststr[32 + 1]; // 定位串
    LONGLONG      llCustid;          // 客户代码
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szName[16 + 1];    // 股东姓名
    int           iSecuseq;          // 股东序号
    char          chRegflag;         // 指定交易状态
};
//-------------------------------10101677:资金查询(含创新业务)--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
    char          szRemark[6 + 1];  // 备注
};
struct MATRADEAPI CRspStkQryRpcCuacctField
{
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账户
    char          szOrgid[4 + 1];        // 机构编码
    char          chMoneytype;           // 货币
    char          szFundbal[21 + 1];     // 资金余额
    char          szMarketvalue[21 + 1]; // 资产总值
    char          szFundavl[21 + 1];     // 资金可用金额
    char          szFund[21 + 1];        // 资金资产
    char          szStkvalue[21 + 1];    // 市值
    int           iFundseq;              // 主资金标志
    char          szFundloan[21 + 1];    // 融资总金额
    char          szFundbuy[21 + 1];     // 买入冻结
    char          szFundsale[21 + 1];    // 卖出解冻
    char          szFundfrz[21 + 1];     // 冻结总金额
    char          szFundlastbal[21 + 1]; // 昨日余额
    char          szFundpartavl[21 + 1]; // 沪港通二级可用资金
};
//-------------------------------10101678:资产明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcAssetDetailField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspStkQryRpcAssetDetailField
{
    LONGLONG      llCustid;                  // 客户代码
    LONGLONG      llFundid;                  // 资金账户
    char          szOrgid[4 + 1];            // 机构编码
    char          chMoneytype;               // 货币
    char          szFundassettotoal[21 + 1]; // 资产总值
    char          szFundassetpure[21 + 1];   // 资产净值
    char          szFundasset[21 + 1];       // 资金资产
    char          szSktasset[21 + 1];        // 证券资产总值
    char          szStkassetabg[21 + 1];     // AB股证券资产
    char          szStkassetgz[21 + 1];      // 股转证券资产
    char          szStkassethk[21 + 1];      // 港股证券资产
    char          szFundassetof[21 + 1];     // 开放式基金资产
    char          szFundassetrzjr[21 + 1];   // 融资借入资金
    char          szFundassetrqjc[21 + 1];   // 融券借出资金
    char          szStkassetzy[21 + 1];      // 抵押证券资产
    char          szFundassetofzj[21 + 1];   // 自建TA基金资产
    char          szFundassetofjs[21 + 1];   // 中登TA基金资产
};
//-------------------------------10101679:股份明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcStkDetailField
{
    char          szPoststr[32 + 1];     // 定位串
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账户
    char          chMarket;              // 交易市场
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkname[8 + 1];      // 证券名称
    char          szStkcode[8 + 1];      // 证券代码
    char          szOrgid[4 + 1];        // 机构编码
    char          chMoneytype;           // 货币
    int           iStkbal;               // 股份余额
    int           iStkavl;               // 股份可用
    char          szBuycost[21 + 1];     // 当前成本
    char          szCostprice[21 + 1];   // 成本价格
    char          szMktval[21 + 1];      // 市值
    char          szIncome[21 + 1];      // 盈亏
    char          szProincome[21 + 1];   // 参考盈亏
    char          chMtkcalflag;          // 市值计算标识
    int           iStkqty;               // 当前拥股数
    char          szLastprice[21 + 1];   // 最新价格
    char          chStktype;             // 证券类型
    char          szProfitcost[21 + 1];  // 参考成本
    char          szProfitprice[21 + 1]; // 参考成本价
    LONGLONG      llStkbuy;              // 股份买入解冻
    LONGLONG      llStksale;             // 股份卖出冻结
    LONGLONG      llStkdiff;             // 可申赎数量
    LONGLONG      llStkfrz;              // 股份冻结
    LONGLONG      llStktrdfrz;           // 买入申赎差
    LONGLONG      llStktrdunfrz;         // 申赎数量
    LONGLONG      llStkbuysale;          // 股份实时买卖差额
    LONGLONG      llStkuncomebuy;        // 在途买入
    LONGLONG      llStkuncomesale;       // 在途卖出
    char          szCostpriceEx[21 + 1]; // 成本价格
};
//-------------------------------10101680:股份汇总查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkSummaryField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcStkSummaryField
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
//-------------------------------10101681:外围资金帐户资金情况查询--------------------------
struct MATRADEAPI CReqStkQryRpcCuacct2Field
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspStkQryRpcCuacct2Field
{
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    LONGLONG      llFundid;           // 资金账户
    char          szOrgid[4 + 1];     // 机构编码
    char          chMoneytype;        // 货币
    char          szFundavl[21 + 1];  // 资金可用
    char          szFundbal[21 + 1];  // 资金余额
};
//-------------------------------10101682:客户查询，根据股东内码，查询股东代码--------------------------
struct MATRADEAPI CReqStkQryRpcTrdacct2Field
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkQryRpcTrdacct2Field
{
    LONGLONG      llCustid;         // 客户代码
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
};
//-------------------------------10101683:客户查询，根据股东代码，资金帐户查询客户代码--------------------------
struct MATRADEAPI CReqStkQryRpcTrdacct3Field
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金帐户
};
struct MATRADEAPI CRspStkQryRpcTrdacct3Field
{
    LONGLONG      llCustid;           // 客户代码
    char          szCustname[16 + 1]; // 客户姓名
    char          szOrgid[4 + 1];     // 机构代码
    char          szBankcode[4 + 1];  // 银行代码
    LONGLONG      llFundid;           // 资金帐户
    char          chMarket;           // 交易市场
    char          szSecuid[10 + 1];   // 股东代码
};
//-------------------------------10101684:当日委托查询--------------------------
struct MATRADEAPI CReqStkQryRpcOrderDetailField
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
    char          szExtsno[32 + 1];  // 外部流水号
    char          chQryoperway;      // 委托渠道
};
struct MATRADEAPI CRspStkQryRpcOrderDetailField
{
    char          szPoststr[32 + 1];     // 定位串
    int           iOrderdate;            // 委托日期
    int           iOrdersno;             // 委托序号
    int           iOrdergroup;           // 委托批号
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
    char          szProdcode[12 + 1];    // 产品编码
    char          szProdname[64 + 1];    // 产品名称
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
    char          szBondintr[21 + 1];    // 债券应计利息
    char          chOperway;             // 委托渠道
    char          szRemark[64 + 1];      // 备注信息
};
//-------------------------------10101685:当日委托查询(含创新业务)--------------------------
struct MATRADEAPI CReqStkQryRpcOrder2Field
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
    char          szExtsno[32 + 1];  // 外部流水号
    char          chQryoperway;      // 委托渠道
};
struct MATRADEAPI CRspStkQryRpcOrder2Field
{
    char          szPoststr[32 + 1];     // 定位串
    int           iOrderdate;            // 委托日期
    int           iOrdersno;             // 委托序号
    int           iOrdergroup;           // 委托批号
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
    char          szProdcode[12 + 1];    // 产品编码
    char          szProdname[64 + 1];    // 产品名称
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
    char          szBondintr[21 + 1];    // 债券应计利息
    char          chOperway;             // 委托渠道
    char          szRemark[64 + 1];      // 备注信息
};
//-------------------------------10101686:当日成交明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcMatchDetailField
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
    char          chQryoperway;      // 委托渠道
};
struct MATRADEAPI CRspStkQryRpcMatchDetailField
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
    char          szProdcode[12 + 1];   // 产品编码
    char          szProdname[64 + 1];   // 产品名称
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szBondintr[21 + 1];   // 债券应计利息
};
//-------------------------------10101687:当日成交查询(含创新业务)--------------------------
struct MATRADEAPI CReqStkQryRpcMatchField
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
    char          chQryoperway;      // 委托渠道
};
struct MATRADEAPI CRspStkQryRpcMatchField
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
    char          szProdcode[12 + 1];   // 产品编码
    char          szProdname[64 + 1];   // 产品名称
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szBondintr[21 + 1];   // 债券应计利息
};
//-------------------------------10101688:当日成交查询（微证券专用）--------------------------
struct MATRADEAPI CReqStkQryRpcMatchForTencentPayField
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
    char          chQryoperway;      // 委托渠道
    char          szExtsno[32 + 1];  // 外部流水号
};
struct MATRADEAPI CRspStkQryRpcMatchForTencentPayField
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
    char          szProdcode[12 + 1];   // 产品编码
    char          szProdname[64 + 1];   // 产品名称
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szBondintr[21 + 1];   // 债券应计利息
    char          szOrdergroup[32 + 1]; // 外部流水号
};
//-------------------------------10101689:历史委托汇总查询, 按证券代码和买卖方向--------------------------
struct MATRADEAPI CReqStkQryRpcHisOrderSummaryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspStkQryRpcHisOrderSummaryField
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
//-------------------------------10101690:成交汇总查询, 按证券代码和买卖方向--------------------------
struct MATRADEAPI CReqStkQryRpcHisMatchSummaryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
    char          chQryoperway;      // 交易渠道
};
struct MATRADEAPI CRspStkQryRpcHisMatchSummaryField
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
    char          szFeeJsxf[21 + 1];    // 净佣金
    char          szFeeSxf[21 + 1];     // 佣金
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeGhf[21 + 1];     // 过户费
    char          szFeeQsf[21 + 1];     // 清算费
    char          szFeeJygf[21 + 1];    // 交易规费
    char          szFeeJsf[21 + 1];     // 经手费
    char          szFeeZgf[21 + 1];     // 证管费
    char          szFeeQtf[21 + 1];     // 其他费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFundeffect[21 + 1]; // 资金发生数
};
//-------------------------------10101691:交割单查询，根据柜台普通对帐的摘要配置打印信息打印单据。微证券专用--------------------------
struct MATRADEAPI CReqStkQryRpcDeliveryForTencentPayField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
    char          szExtsno[32 + 1];  // 外部流水号
};
struct MATRADEAPI CRspStkQryRpcDeliveryForTencentPayField
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
    char          szFeeOneYhs[21 + 1];  // 一级印花税
    char          szFeeOneGhf[21 + 1];  // 一级过户费
    char          szFeeOneQsf[21 + 1];  // 一级清算费
    char          szFeeOneJygf[21 + 1]; // 一级交易规费
    char          szFeeOneJsf[21 + 1];  // 一级经手费
    char          szFeeOneZgf[21 + 1];  // 一级证管费
    char          szFeeOneQtf[21 + 1];  // 一级其他费
    char          szFeeOneFxj[21 + 1];  // 一级风险金
    char          szOrdergroup[32 + 1]; // 外部流水号
};
//-------------------------------10101692:当日委托汇总查询,ordergroup+stkcode+bsflag--------------------------
struct MATRADEAPI CReqStkQryRpcOrderSummary2Field
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBankcode[4 + 1]; // 外部银行
};
struct MATRADEAPI CRspStkQryRpcOrderSummary2Field
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
//-------------------------------10101693:客户机构查询--------------------------
struct MATRADEAPI CReqStkQryRpcOrgField
{
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkQryRpcOrgField
{
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 机构代码
    char          chMarket;         // 交易市值
    char          szSecuid[10 + 1]; // 股东代码
};
//-------------------------------10101694:查询客户属性和资金帐号属性--------------------------
struct MATRADEAPI CReqStkQryRpcAcctInfoField
{
    LONGLONG      llFundid;         // 资金账户
};
struct MATRADEAPI CRspStkQryRpcAcctInfoField
{
    LONGLONG      llCustid;              // 客户代码
    LONGLONG      llFundid;              // 资金账号
    char          chCustprop;            // 客户性质
    char          chCustkind;            // 客户类别
    char          chCustgroup;           // 客户分组
    char          chFundkind;            // 资金分类
    char          chFundlevel;           // 资金室号
    char          chFundgroup;           // 资金分组
    char          szRiskfactor[129 + 1]; // 风险要素
    char          szCriterion[129 + 1];  // 开户规范
};
//-------------------------------10101695:意向行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcMktIntentionField
{
    char          chMarket;          // 市场
    char          szStkcode[6 + 1];  // 证券代码
    char          szBsflag[128 + 1]; // 业务类别
    int           iPromiseno;        // 约定号
    int           iQryrec;           // 查询记录数
    int           iPosstr;           // 定位串
};
struct MATRADEAPI CRspStkQryRpcMktIntentionField
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
//-------------------------------10101696:查询营业部某一业务类别的申报席位--------------------------
struct MATRADEAPI CReqStkQryRpcSeatField
{
    char          chMarket;         // 交易所代码
    char          szOrgid[4 + 1];   // 营业部
    char          szBsflag[2 + 1];  // 业务类别
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkQryRpcSeatField
{
    char          szSeat[6 + 1]; // 申报席位
};
//-------------------------------10101697:查询资金账户信息--------------------------
struct MATRADEAPI CReqStkQryRpcCuacctAcctInfoField
{
    LONGLONG      llFundid;         // 资金账户
};
struct MATRADEAPI CRspStkQryRpcCuacctAcctInfoField
{
    char          szFundname[16 + 1];        // 资金姓名
    char          chIdtype;                  // 证件类型
    char          szIdno[32 + 1];            // 证件号码
    char          szOperway[128 + 1];        // 操作方式
    char          szFundagentright[128 + 1]; // 资金代理权限
    char          szFundright[128 + 1];      // 资金权限
    char          chStatus;                  // 资金状态
};
//-------------------------------10101698:非流通股份余额查询（沪市）--------------------------
struct MATRADEAPI CReqStkQryRpcShNonCirculatingShareBalanceField
{
    LONGLONG      llFundid;          // 资金帐号
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iCount;            // 请求行数
    char          szPoststr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcShNonCirculatingShareBalanceField
{
    LONGLONG      llCustid;          // 客户代码
    char          szOrgid[4 + 1];    // 机构编码
    LONGLONG      llFundid;          // 资金账号
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    LONGLONG      llStkbal;          // 委托数量
    char          szMktval[21 + 1];  // 市值
    char          szPathid[16 + 1];  // 交易路径类型
    char          szPoststr[64 + 1]; // 定位串
};
//-------------------------------10101699:深圳非流通股份余额查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzNonCirculatingShareBalanceField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iCount;            // 请求行数
    char          szPoststr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcSzNonCirculatingShareBalanceField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          szSeat[6 + 1];     // 席位
    char          szStkprop[2 + 1];  // 股份性质
    LONGLONG      llStkbal;          // 股份余额
    LONGLONG      llStkavl;          // 股份可用
    char          szMktval[21 + 1];  // 市值
    char          szPoststr[64 + 1]; // 定位串
};
//-------------------------------10101700:股息红利扣税查询--------------------------
struct MATRADEAPI CReqStkQryRpcDividendWithholdingTaxField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    int           iBegindate;       // 起始日期
    int           iEnddate;         // 结束日期
    char          chQryhis;         // 查询方式
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcDividendWithholdingTaxField
{
    char          chMarket;               // 交易市场
    char          szSecuid[10 + 1];       // 证券账号
    char          szStkcode[8 + 1];       // 证券代码
    LONGLONG      llTaxcount;             // 净减持股数
    int           iReducedate;            // 减持交割日期
    char          chMoneytype;            // 货币代码
    char          szTaxamt[21 + 1];       // 应扣税额
    char          szPaidtaxamt[21 + 1];   // 已扣税额
    char          szUnpaidtaxamt[21 + 1]; // 未扣税额
    char          chStatus;               // 扣税状态
    int           iTaxdate;               // 计税日期
};
//-------------------------------10101701:查询客户所属机构--------------------------
struct MATRADEAPI CReqStkQryRpcCustOrgField
{
    char          chInputtype;       // 登陆类型
    char          szInputid[64 + 1]; // 登陆标识
};
struct MATRADEAPI CRspStkQryRpcCustOrgField
{
    int           iServerid;      // 服务编码
    char          szOrgid[5 + 1]; // 机构代码
    char          szBrhid[5 + 1]; // 分支代码
    LONGLONG      llCustid;       // 客户代码
};
//-------------------------------10101702:查询存管资金账号--------------------------
struct MATRADEAPI CReqStkQryRpcDepositoryCuacctField
{
    char          szBankcode[4 + 1]; // 银行代码
    char          chMoneytype;       // 货币代码
    LONGLONG      llFundid;          // 资金帐户
    char          chIdtype;          // 证件类型
    char          szIdno[32 + 1];    // 证件账号
};
struct MATRADEAPI CRspStkQryRpcDepositoryCuacctField
{
    int           iServerid;         // 机器编码
    LONGLONG      llCustid;          // 客户代码
    LONGLONG      llFundid;          // 资金帐号
    char          chIdtype;          // 证件类型
    char          szIdno[32 + 1];    // 证件号码
    char          chStatus;          // 状态
    char          chMoneytype;       // 币种
    char          szBankid[32 + 1];  // 银行帐号
    char          szBankcode[4 + 1]; // 银行代码
};
//-------------------------------10101703:场内基金申赎失败查询--------------------------
struct MATRADEAPI CReqStkQryRpcFloorFundRedemptionFailureField
{
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
    int           iBegindate;       // 开始日期
    int           iEnddate;         // 结束日期
};
struct MATRADEAPI CRspStkQryRpcFloorFundRedemptionFailureField
{
    char          szOrgid[4 + 1];    // 机构代码
    char          chMarket;          // 市场
    LONGLONG      llCustid;          // 客户代码
    char          szSecuid[10 + 1];  // 股东代码
    char          chMoneytype;       // 币种
    int           iOrderdate;        // 委托日期
    char          szOrderid[10 + 1]; // 委托合同号
    char          szStkcode[6 + 1];  // 证券代码
    char          szStkname[16 + 1]; // 证券名称
    char          chStktype;         // 证券类别
    char          szRemark[32 + 1];  // 备注
};
//-------------------------------10101704:黑名单证券查询--------------------------
struct MATRADEAPI CReqStkQryRpcBlacklistStkField
{
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcBlacklistStkField
{
    char          chMarket;             // 市场
    char          szStkcode[6 + 1];     // 证券代码
    char          szStkname[16 + 1];    // 证券名称
    char          szBsflag[1024 + 1];   // 不允许的买卖类别
    char          szOperway[128 + 1];   // 不允许的操作方式
    char          szPromptmsg[128 + 1]; // 报错提示信息
};
//-------------------------------10101705:深圳大宗交易行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzBlockTradingMktField
{
    char          szStkcode[6 + 1];    // 证券代码
    char          szConfermnum[8 + 1]; // 约定号
    char          szBsflag[2 + 1];     // 业务类别
    char          chStatus;            // 记录状态
};
struct MATRADEAPI CRspStkQryRpcSzBlockTradingMktField
{
    int           iHqjlh;            // 记录号
    char          szHqzqdm[6 + 1];   // 证券代码
    LONGLONG      llHqsbsl;          // 申报数量（张）
    char          szHqsbjg[21 + 1];  // 申报价格
    char          szHqywlb[2 + 1];   // 业务类别
    char          szHqzllb[10 + 1];  // 指令类别
    char          szHqhtxh[22 + 1];  // 合同序号
    char          szHqlxr[12 + 1];   // 联系人
    char          szHqlxfs[30 + 1];  // 联系方式
    char          szHqjsjg[2 + 1];   // 结算机构代码
    char          szHqsbsj[8 + 1];   // 申报时间
    char          szHqydh[8 + 1];    // 约定号
    char          szHqsyl[21 + 1];   // 参考收益率
    LONGLONG      llHqsbsl2;         // 申报数量2
    char          szHqsbjg2[21 + 1]; // 申报价格2
    char          szHqsyl2[21 + 1];  // 参考收益率2
    char          chHqjlzt;          // 记录状态
    char          szHqbybz[2 + 1];   // 备用标志
    char          chHqqxlx;          // 期限类型
    int           iHqghqx;           // 回购期限
    char          szHqbywb[80 + 1];  // 备用文本
};
//-------------------------------10101706:上海大宗交易行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcShBlockTradingMktField
{
    char          szStkcode[6 + 1]; // 证券代码
    char          szBsflag[2 + 1];  // 业务类别
};
struct MATRADEAPI CRspStkQryRpcShBlockTradingMktField
{
    int           iHqjlh;           // 记录号
    char          szHqzqdm[6 + 1];  // 证券代码
    char          szHqhybh[10 + 1]; // 会员编号
    char          chHqbdlx;         // 报单类型
    char          chHqmmbz;         // 买卖标志
    char          szHqsbjg[21 + 1]; // 申报价格
    LONGLONG      llHqsbsl;         // 申报数量（张）
    char          szHqcdbh[10 + 1]; // 撤单编号
    char          szHqfqf[6 + 1];   // 发起方席位
    char          szHqlxfs[50 + 1]; // 联系方式
};
//-------------------------------10101707:深圳场内基金分红方式查询--------------------------
struct MATRADEAPI CReqStkQryRpcSzFloorFundDividendsField
{
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcSzFloorFundDividendsField
{
    char          chMarket;          // 市场
    char          szStkcode[6 + 1];  // 证券代码
    char          szStkname[16 + 1]; // 证券名称
    char          chDividpolicy;     // 分红方式
};
//-------------------------------10101708:受限股东减持额度查询--------------------------
struct MATRADEAPI CReqStkQryRpcLimitedTrdacctReductionLimitField
{
    LONGLONG      llFundid;         // 资金帐号
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[6 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcLimitedTrdacctReductionLimitField
{
    int           iImportdate;      // 导入日期
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 机构代码
    LONGLONG      llFundid;         // 资金帐号
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szSeat[6 + 1];    // 席位
    LONGLONG      llStkbal;         // 股份余额
    LONGLONG      llStkavl1;        // 竞价可用
    LONGLONG      llStkavl2;        // 大宗非特定减持可用
    LONGLONG      llStkavl3;        // 大宗特定减持可用
    LONGLONG      llStkavl4;        // ETF申购可用
};
//-------------------------------10101709:港股通资金资产查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkCuacctField
{
    LONGLONG      llFundid;          // 资金帐号
    char          szOrgid[4 + 1];    // 机构编码
    char          chMoneytype;       // 货币代码
    char          szFundcode[8 + 1]; // 资金科目
};
struct MATRADEAPI CRspStkQryRpcHkCuacctField
{
    char          szOrgid[4 + 1];          // 机构编码
    LONGLONG      llFundid;                // 资金帐号
    char          szFundcode[8 + 1];       // 资金科目
    char          chMoneytype;             // 货币代码
    char          szLasttotalamt[21 + 1];  // 上日总数
    char          szTotalamt[21 + 1];      // 当前总数
    char          szAvlamt[21 + 1];        // 可用数
    char          szFunduncomein[21 + 1];  // 在途入账
    char          szFunduncomeout[21 + 1]; // 在途出账
    char          szFundrealin[21 + 1];    // 实时入账
    char          szFundrealout[21 + 1];   // 实时出账
    char          szFundnightout[21 + 1];  // 夜市出账资金
    char          szFundtranin[21 + 1];    // 划入金额
    char          szFundtranout[21 + 1];   // 划出金额
    char          szOtheramt[21 + 1];      // 其他金额
    char          szRemark[128 + 1];       // 备注信息
};
//-------------------------------10101710:港股通客户负债查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkLiabilityField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMoneytype;      // 货币代码
    char          chDebtsid;        // 负债类型
    char          szOrgid[4 + 1];   // 机构编码
};
struct MATRADEAPI CRspStkQryRpcHkLiabilityField
{
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llCustid;             // 客户代码
    LONGLONG      llFundid;             // 资金帐号
    char          chDebtsid;            // 负债类型
    char          chMoneytype;          // 货币代码
    char          szUnpayamt[21 + 1];   // 未支付金额
    char          szSumpaidamt[21 + 1]; // 已支付总金额
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 股东代码
    char          szSumdebtamt[21 + 1]; // 应付总金额
};
//-------------------------------10101711:权益查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkEquityField
{
    char          szSecuid[11 + 1]; // 股东代码
    char          chMarket;         // 交易市场
    char          szStkcode[9 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcHkEquityField
{
    char          chMarket;           // 市场代码
    char          szSecuid[11 + 1];   // 证券帐号
    char          szSeat[7 + 1];      // 席位代码
    char          szStkcode[9 + 1];   // 证券代码
    char          szStkprop[2 + 1];   // 股份性质
    char          szEquittype[2 + 1]; // 权益类型
    char          szRightno[10 + 1];  // 权益编号
    char          chListstatus;       // 上市状态
    LONGLONG      llStkbal;           // 持有余额
    LONGLONG      llStkuncome;        // 未交收数量
    LONGLONG      llStkfrz;           // 冻结数量
    LONGLONG      llStkother;         // 其它数量
};
//-------------------------------10101712:港股通开户准备--------------------------
struct MATRADEAPI CReqStkRpcHkPreparationForOpeningAccountField
{
    char          chMarket;         // 市场代码
    char          szSecuid[10 + 1]; // 股东代码
};
struct MATRADEAPI CRspStkRpcHkPreparationForOpeningAccountField
{
    int           iSno;            // 流水号
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101713:港股通未交收明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkUndeliveryDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkUndeliveryDetailField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iOrderdate;           // 委托日期
    int           iOrdersno;            // 成交日期
    LONGLONG      llCustid;             // 客户代码
    char          szCustname[16 + 1];   // 客户姓名
    LONGLONG      llFundid;             // 资金账户
    char          chMoneytype;          // 货币
    char          szOrgid[4 + 1];       // 机构编码
    char          szSecuid[10 + 1];     // 股东代码
    char          szBusitype[4 + 1];    // 业务类别
    char          szOrderid[10 + 1];    // 合同序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    char          szOrderprice[21 + 1]; // 委托价格(港币)
    int           iOrderqty;            // 委托数量
    int           iMatchqty;            // 成交数量
    char          szMatchprice[21 + 1]; // 成交价格(人民币)
    char          szMatchamt[21 + 1];   // 成交金额
    char          szClearedamt[21 + 1]; // 清算金额
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeJsxf[21 + 1];    // 标准手续费
    char          szFeeSxf[21 + 1];     // 手续费
    char          szFeeQsf[21 + 1];     // 股份交收费
    char          szFeeJygf[21 + 1];    // 交易费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFeeJsf[21 + 1];     // 交易系统使用费费
    char          szFeeZgf[21 + 1];     // 交易征费
    char          szFeeOneYhs[21 + 1];  // 一级印花税
    char          szFeeOneGhf[21 + 1];  // 一级过户费
    char          szFeeOneQsf[21 + 1];  // 一级股份交收费
    char          szFeeOneJygf[21 + 1]; // 一级交易费
    char          szFeeOneJsf[21 + 1];  // 一级交易系统使用费
    char          szFeeOneZgf[21 + 1];  // 一级交易征费
    char          szFeeOneQtf[21 + 1];  // 一级其他费
    char          szFeeOneFxj[21 + 1];  // 一级风险金
    char          szSettrate[21 + 1];   // 结算汇率
    char          szRemark[128 + 1];    // 备注
};
//-------------------------------10101714:港股通证券组合费交收明细--------------------------
struct MATRADEAPI CReqStkRpcHkStkCombinationFeeDeliveryDetailField
{
    LONGLONG      llFundid;          // 资金帐号
    char          chMoneytype;       // 货币代码
    char          szOrgid[4 + 1];    // 机构编码
    int           iStrdate;          // 清算开始时间
    int           iEnddate;          // 清算结束时间
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
    char          chMarket;          // 交易市场
};
struct MATRADEAPI CRspStkRpcHkStkCombinationFeeDeliveryDetailField
{
    char          szPoststr[32 + 1];      // 定位串
    LONGLONG      llFundid;               // 资金帐号
    char          szOrgid[4 + 1];         // 机构编码
    char          chMoneytype;            // 货币代码
    char          chMarket;               // 交易市场
    char          szSecuid[10 + 1];       // 证券账号
    char          chDebtsid;              // 负债类型
    char          szSno[16 + 1];          // 清算流水号
    int           iQsdate;                // 清算日期
    int           iJsdate;                // 交收日期
    char          szLastmktvalue[21 + 1]; // T-1日持有市值
    char          szSettrate[21 + 1];     // 结算汇率
    char          szCombinfeeHk[21 + 1];  // 港币应收付金额
    char          szCombinfeeRmb[21 + 1]; // 人民币应收付金额
    char          szRemark[128 + 1];      // 备注
};
//-------------------------------10101715:港股通交割单查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkDeliveryField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkDeliveryField
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
    char          szFeeQsf[21 + 1];     // 股份交收费
    char          szFeeJygf[21 + 1];    // 交易费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFeeJsf[21 + 1];     // 交易系统使用费费
    char          szFeeZgf[21 + 1];     // 交易征费
    char          szFeeQtf[21 + 1];     // 其它费
    char          szFundbal[21 + 1];    // 资金本次余额
    int           iStkbal;              // 股份本次余额
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格(港币)
    char          szFeeOneYhs[21 + 1];  // 一级印花税
    char          szFeeOneGhf[21 + 1];  // 一级过户费
    char          szFeeOneQsf[21 + 1];  // 一级股份交收费
    char          szFeeOneJygf[21 + 1]; // 一级交易费
    char          szFeeOneJsf[21 + 1];  // 一级交易系统使用费
    char          szFeeOneZgf[21 + 1];  // 一级交易征费
    char          szFeeOneQtf[21 + 1];  // 一级其他费
    char          szFeeOneFxj[21 + 1];  // 一级风险金
    char          szSettrate[21 + 1];   // 结算汇率
};
//-------------------------------10101716:港股通对帐单查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkStatementField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chPrintflag;       // 重打标识
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkStatementField
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
    char          szSettrate[21 + 1];   // 结算汇率
};
//-------------------------------10101717:港股通最小价差查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkMinSpreadField
{
    char          chMarket;         // 交易市场
    char          chStktype;        // 证券类别
    char          szPrice[21 + 1];  // 委托价格
};
struct MATRADEAPI CRspStkQryRpcHkMinSpreadField
{
    char          chMarket;             // 交易市场
    char          chStktype;            // 证券类别
    char          szBeginprice[21 + 1]; // 价格下限
    char          szEndprice[21 + 1];   // 价格上限
    char          szPriceunit[21 + 1];  // 价位
};
//-------------------------------10101718:港股通参考汇率查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkExchangeRateField
{
    char          chMarket;         // 交易市场
    char          chMoneytype;      // 币种
};
struct MATRADEAPI CRspStkQryRpcHkExchangeRateField
{
    char          chMarket;                    // 交易市场
    char          chMoneytype;                 // 币种
    char          szBuyrate[21 + 1];           // 买入参考汇率
    char          szDaybuyriserate[21 + 1];    // 日间买入参考汇率浮动比例
    char          szNightbuyriserate[21 + 1];  // 夜市买入参考汇率浮动比例
    char          szSalerate[21 + 1];          // 卖出参考汇率
    char          szDaysaleriserate[21 + 1];   // 日间卖出参考汇率浮动比例
    char          szNightsaleriserate[21 + 1]; // 夜市卖出参考汇率浮动比例
    char          szMidrate[21 + 1];           // 中间参考汇率
    int           iSysdate;                    // 使用日期
    char          szSettrate[21 + 1];          // 结算汇率
    char          szRemark[128 + 1];           // 备注
};
//-------------------------------10101719:港股通市场交易状态查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkMarketStatusField
{
    char          chMarket;         // 交易市场
};
struct MATRADEAPI CRspStkQryRpcHkMarketStatusField
{
    char          chMarket;          // 交易市场
    LONGLONG      llQuotabal;        // 每日初始额度
    LONGLONG      llQuotaavl;        // 日中剩余额度
    char          chQuotastatus;     // 额度状态
    char          chMkttrdstatus;    // 市场交易状态
    int           iUpddate;          // 更新日期
    char          szRemark[128 + 1]; // 备注
};
//-------------------------------10101720:港股通标的证券查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkUnderlyingSecurityField
{
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcHkUnderlyingSecurityField
{
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szStkname[8 + 1];  // 证券名称
    char          chZstrdstatus;     // 整手交易状态
    char          chLgtrdstatus;     // 零股交易状态
    int           iUpddate;          // 更新日期
    char          szRemark[128 + 1]; // 备注
};
//-------------------------------10101721:港股通交易日历查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkCalendarField
{
    int           iPhydate;          // 物理日期
    int           iEnddate;          // 结束日期
    char          chMarket;          // 交易市场
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkCalendarField
{
    char          szPoststr[32 + 1]; // 定位串
    char          chMarket;          // 交易市场
    char          chMoneytype;       // 币种
    int           iPhydate;          // 物理日期
    char          chBusinessflag;    // 交易日标识
    char          chCommitflag;      // 交收日标识
};
//-------------------------------10101722:港股通委托买卖交易--------------------------
struct MATRADEAPI CReqStkRpcHkOrderField
{
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    LONGLONG      llFundid;         // 资金账户
    char          szStkcode[8 + 1]; // 证券代码
    char          szBsflag[2 + 1];  // 买卖类别
    char          szPrice[21 + 1];  // 价格(价格)
    int           iQty;             // 数量
    int           iOrdergroup;      // 委托批号
    char          chTimeinforce;    // 订单有效时间类型
    LONGLONG      llRisksignsno;    // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcHkOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
};
//-------------------------------10101723:港股通撤单委托--------------------------
struct MATRADEAPI CReqStkRpcHkCancelField
{
    int           iOrderdate;       // 委托日期
    LONGLONG      llFundid;         // 资金帐户
    int           iOrdersno;        // 委托序号
    char          szBsflag[2 + 1];  // 买卖类别
};
struct MATRADEAPI CRspStkRpcHkCancelField
{
    int           iOrdersno; // 撤单委托序号
};
//-------------------------------10101724:委托撤单查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkCancelField
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
struct MATRADEAPI CRspStkQryRpcHkCancelField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iOrdersno;            // 委托序号
    int           iOrdergroup;          // 委托批号
    char          szOrderid[10 + 1];    // 合同序号
    int           iOrderdate;           // 委托日期
    int           iOpertime;            // 委托时间
    LONGLONG      llFundid;             // 资金帐户
    char          chMarket;             // 市场代码
    char          szSecuid[10 + 1];     // 股东代码
    char          szStkcode[8 + 1];     // 证券名称
    char          szStkname[8 + 1];     // 证券代码
    char          szBsflag[2 + 1];      // 买卖类别
    char          szOrderprice[21 + 1]; // 委托价格(港币)
    int           iOrderqty;            // 委托数量
    int           iMatchqty;            // 成交数量
    char          chOrderstatus;        // 委托状态
    char          chCancelstatus;       // 撤单状态
};
//-------------------------------10101725:港股通最大可交易数量查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkMaxTradeQtyField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          szSecuid[10 + 1]; // 股东代码
    char          szBsflag[2 + 1];  // 买卖类型
    char          szPrice[21 + 1];  // 价格(港币)
};
struct MATRADEAPI CRspStkQryRpcHkMaxTradeQtyField
{
    int           iMaxstkqty; // 最大交易数量
};
//-------------------------------10101726:港股通交易委托明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkOrderDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    int           iOrdergroup;       // 委托批号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkOrderDetailField
{
    char          szPoststr[32 + 1];        // 定位串
    int           iOrderdate;               // 委托日期
    int           iOrdersno;                // 委托序号
    int           iOrdergroup;              // 委托批号
    LONGLONG      llCustid;                 // 客户代码
    char          szCustname[16 + 1];       // 客户姓名
    LONGLONG      llFundid;                 // 资金账户
    char          chMoneytype;              // 货币
    char          szOrgid[4 + 1];           // 机构编码
    char          szSecuid[10 + 1];         // 股东代码
    char          szBsflag[2 + 1];          // 买卖类别
    char          szOrderid[10 + 1];        // 申报合同序号
    int           iReporttime;              // 报盘时间
    int           iOpertime;                // 委托时间
    char          chMarket;                 // 交易市场
    char          szStkcode[8 + 1];         // 证券代码
    char          szStkname[8 + 1];         // 证券名称
    char          szOrderprice[21 + 1];     // 委托价格(港币)
    int           iOrderqty;                // 委托数量
    char          szOrderfrzamtRmb[21 + 1]; // 冻结金额(人民币)
    int           iMatchqty;                // 成交数量
    char          szMatchamt[21 + 1];       // 成交金额(港币)
    int           iCancelqty;               // 撤单数量
    char          chOrderstatus;            // 委托状态
    char          chCancelstatus;           // 撤单状态
    char          szSeat[6 + 1];            // 交易席位
    char          chCancelflag;             // 撤单标识
    int           iOperdate;                // 操作日期
    char          szReferrate[21 + 1];      // 参考汇率
    char          szRemark[128 + 1];        // 备注
    char          szAfundamt[21 + 1];       // A股资金变动金额
    char          szHfundamt[21 + 1];       // 港股资金变动金额
};
//-------------------------------10101727:港股通交易成交查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkMatchField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkMatchField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iTrddate;             // 成交日期
    char          szSecuid[10 + 1];     // 股东代码
    char          szBsflag[2 + 1];      // 买卖类别
    int           iOrdersno;            // 委托序号
    char          szOrderid[10 + 1];    // 申报合同序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格(港币)
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额(港币)
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格(港币)
};
//-------------------------------10101728:港股通交易历史委托查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkHisOrderField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐号
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    int           iOrdergroup;       // 委托批号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkHisOrderField
{
    char          szPoststr[32 + 1];        // 定位串
    int           iOrderdate;               // 委托日期
    LONGLONG      llCustid;                 // 客户代码
    char          szCustname[16 + 1];       // 客户姓名
    LONGLONG      llFundid;                 // 资金账户
    char          chMoneytype;              // 货币
    char          szOrgid[4 + 1];           // 机构编码
    char          szSecuid[10 + 1];         // 股东代码
    char          szBsflag[2 + 1];          // 买卖类别
    char          szOrderid[10 + 1];        // 申报合同序号
    int           iOrdergroup;              // 委托批号
    int           iReporttime;              // 报盘时间
    int           iOpertime;                // 委托时间
    char          chMarket;                 // 交易市场
    char          szStkcode[8 + 1];         // 证券代码
    char          szStkname[8 + 1];         // 证券名称
    char          szOrderprice[21 + 1];     // 委托价格(港币)
    int           iOrderqty;                // 委托数量
    char          szOrderfrzamtRmb[21 + 1]; // 冻结金额（人民币）
    int           iMatchqty;                // 成交数量
    char          szMatchamt[21 + 1];       // 成交金额(港币)
    int           iCancelqty;               // 撤单数量
    char          chCancelflag;             // 撤单标志
    char          chOrderstatus;            // 委托状态
    char          chCancelstatus;           // 撤单状态
    char          szSeat[6 + 1];            // 交易席位
    char          chOperway;                // 操作方式
    int           iOperdate;                // 操作日期
    char          szReferrate[21 + 1];      // 参考汇率
    char          szAfundamt[21 + 1];       // A股资金变动金额
    char          szHfundamt[21 + 1];       // 港股资金变动金额
};
//-------------------------------10101729:港股通交易成交历史查询--------------------------
struct MATRADEAPI CReqStkQryRpcHkHisMatchField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcHkHisMatchField
{
    char          szPoststr[32 + 1];    // 定位串
    int           iBizdate;             // 交收日期
    int           iCleardate;           // 成交日期
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
    char          szOrderprice[21 + 1]; // 委托价格(港币)
    int           iStkbal;              // 股份本次余额
    char          szFeeJsxf[21 + 1];    // 净手续费
    char          szFeeSxf[21 + 1];     // 手续费
    char          szFeeYhs[21 + 1];     // 印花税
    char          szFeeGhf[21 + 1];     // 过户费
    char          szFeeQsf[21 + 1];     // 清算费
    char          szFeeJygf[21 + 1];    // 交易规费
    char          szFeeJsf[21 + 1];     // 经手费
    char          szFeeZgf[21 + 1];     // 证管费
    char          szFeeQtf[21 + 1];     // 其他费
    char          szFeefront[21 + 1];   // 前台费用
    char          szFundeffect[21 + 1]; // 资金发生数
    char          szFundbal[21 + 1];    // 使用金额
    char          szSettrate[21 + 1];   // 结算汇率
};
//-------------------------------10101730:股转系统证券委托申报--------------------------
struct MATRADEAPI CReqStkRpcStkTranSysStkOrderField
{
    char          chMarket;               // 交易市场
    LONGLONG      llFundid;               // 资金账户
    char          szSecuid[10 + 1];       // 股东代码
    char          szBsflag[2 + 1];        // 买卖类别
    char          szStkcode[8 + 1];       // 证券代码
    char          szPrice[21 + 1];        // 委托价格
    int           iQty;                   // 委托数量
    int           iOrdergroup;            // 委托批号
    char          szLinkman[12 + 1];      // 联系人
    char          szLinkway[30 + 1];      // 联系方式
    char          szTargetseat[6 + 1];    // 对方席位
    char          szTargetsecuid[10 + 1]; // 对方股东账户
    char          szConfernum[21 + 1];    // 成交约定号
    LONGLONG      llRisksignsno;          // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcStkTranSysStkOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
    int           iOrdergroup;       // 委托批号
};
//-------------------------------10101731:股转系统协议转让行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkTranSysAgreementTransferMktField
{
    char          szStkcode[8 + 1];    // 证券代码
    char          szBsflag[2 + 1];     // 委托类别
    char          szConfernum[21 + 1]; // 成交约定号
    char          szOrdertime[6 + 1];  // 委托时间
};
struct MATRADEAPI CRspStkQryRpcStkTranSysAgreementTransferMktField
{
    char          szStkcode[8 + 1];     // 证券代码
    char          szSeat[6 + 1];        // 席位代码
    char          szBsflag[2 + 1];      // 委托类别
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szConfernum[21 + 1];  // 成交约定号
    char          szOrdertime[6 + 1];   // 委托时间
    int           iStatus;              // 记录状态
    char          chRemark;             // 备用标志
};
//-------------------------------10101732:股转系统取最大交易数量,支持三方交易--------------------------
struct MATRADEAPI CReqStkRpcStkTranSysMaxTradeQtyField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    LONGLONG      llFundid;          // 资金账户
    char          szStkcode[8 + 1];  // 证券代码
    char          szBsflag[2 + 1];   // 买卖类别
    char          szPrice[21 + 1];   // 价格
    char          szBankcode[4 + 1]; // 外部银行
    char          chHiqtyflag;       // 最大上限标志
    char          chCreditid;        // 信用产品标识
    char          chCreditflag;      // 特殊委托类型
};
struct MATRADEAPI CRspStkRpcStkTranSysMaxTradeQtyField
{
    int           iMaxstkqty; // 股份数量
};
//-------------------------------10101733:股转系统受限投资者查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkTranSysRestrictedInvestorField
{
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          szPosstr[64 + 1]; // 定位串
    int           iCount;           // 请求行数
};
struct MATRADEAPI CRspStkQryRpcStkTranSysRestrictedInvestorField
{
    int           iServerid;        // 节点编号
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户代码
    char          chMarket;         // 市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          szPosstr[64 + 1]; // 定位串
};
//-------------------------------10101734:股转市场非流通股份余额查询--------------------------
struct MATRADEAPI CReqStkQryRpcStkTranSysNonCirculatingSharesBalanceField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iCount;            // 请求行数
    char          szPoststr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcStkTranSysNonCirculatingSharesBalanceField
{
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    char          szSeat[6 + 1];     // 席位
    char          szStkprop[2 + 1];  // 股份性质
    LONGLONG      llStkbal;          // 股份余额
    LONGLONG      llStkavl;          // 股份可用
    char          szPoststr[64 + 1]; // 定位串
};
//-------------------------------10101735:固收平台交易委托明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcFixedIncomeSysOrderDetailField
{
    char          chMarket;          // 交易市场
    LONGLONG      llFundid;          // 资金帐户
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcFixedIncomeSysOrderDetailField
{
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
    char          szStkcode[8 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
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
    int           iConfernum;            // 约定号
    char          szTargettrader[6 + 1]; // 对方交易员
    char          szRemark[128 + 1];     // 备注
    char          szPoststr[32 + 1];     // 定位串
};
//-------------------------------10101736:固收平台交易历史委托明细查询--------------------------
struct MATRADEAPI CReqStkQryRpcFixedIncomeSysHisOrderDetailField
{
    int           iStrdate;          // 起始日期
    int           iEnddate;          // 终止日期
    LONGLONG      llFundid;          // 资金帐号
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcFixedIncomeSysHisOrderDetailField
{
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
    char          szStkcode[8 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
    char          szOrderprice[21 + 1];  // 委托价格
    int           iOrderqty;             // 委托数量
    char          szOrderfrzamt[21 + 1]; // 冻结金额
    int           iMatchqty;             // 成交数量
    char          szMatchamt[21 + 1];    // 成交金额
    int           iCancelqty;            // 撤单数量
    char          chCancelflag;          // 撤单标志
    char          chOrderstatus;         // 委托状态
    char          szSeat[6 + 1];         // 交易席位
    char          chOperway;             // 操作方式
    int           iOperdate;             // 操作日期
    int           iConfernum;            // 约定号
    char          szTargettrader[6 + 1]; // 对方交易员
    char          szPoststr[32 + 1];     // 定位串
};
//-------------------------------10101737:固收平台交易成交查询--------------------------
struct MATRADEAPI CReqStkQryRpcFixedIncomeSysMatchField
{
    LONGLONG      llFundid;          // 资金帐户
    char          chMarket;          // 交易市场
    char          szSecuid[10 + 1];  // 股东代码
    char          szStkcode[8 + 1];  // 证券代码
    int           iOrdersno;         // 委托序号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcFixedIncomeSysMatchField
{
    int           iTrddate;             // 成交日期
    char          szSecuid[10 + 1];     // 股东代码
    char          szBsflag[2 + 1];      // 买卖类别
    int           iOrdersno;            // 委托序号
    char          szOrderid[10 + 1];    // 申报合同序号
    char          chMarket;             // 交易市场
    char          szStkcode[8 + 1];     // 证券代码
    char          szStkname[8 + 1];     // 证券名称
    int           iMatchtime;           // 成交时间
    char          szMatchcode[20 + 1];  // 成交序号
    char          szMatchprice[21 + 1]; // 成交价格
    int           iMatchqty;            // 成交数量
    char          szMatchamt[21 + 1];   // 成交金额
    char          chMatchtype;          // 成交类型
    int           iOrderqty;            // 委托数量
    char          szOrderprice[21 + 1]; // 委托价格
    char          szPoststr[32 + 1];    // 定位串
};
//-------------------------------10101738:固收平台最大可交易数量查询--------------------------
struct MATRADEAPI CReqStkQryRpcFixedIncomeSysMaxTradeQtyField
{
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          szSecuid[10 + 1]; // 股东代码
    char          szBsflag[2 + 1];  // 买卖类型
    char          szPrice[21 + 1];  // 价格
};
struct MATRADEAPI CRspStkQryRpcFixedIncomeSysMaxTradeQtyField
{
    int           iMaxstkqty; // 最大交易数量
};
//-------------------------------10101739:固收平台委托买卖交易--------------------------
struct MATRADEAPI CReqStkRpcFixedIncomeSysOrderField
{
    char          chMarket;              // 交易市场
    char          szSecuid[10 + 1];      // 股东代码
    LONGLONG      llFundid;              // 资金账户
    char          szStkcode[8 + 1];      // 证券代码
    char          szBsflag[2 + 1];       // 买卖类别
    char          szPrice[21 + 1];       // 价格
    int           iQty;                  // 数量
    int           iConfernum;            // 约定号
    char          szTargettrader[6 + 1]; // 对方交易员编号
    LONGLONG      llRisksignsno;         // 风险揭示签署流水号
};
struct MATRADEAPI CRspStkRpcFixedIncomeSysOrderField
{
    int           iOrdersno;         // 委托序号
    char          szOrderid[10 + 1]; // 合同序号
};
//-------------------------------10101740:固收平台撤单委托--------------------------
struct MATRADEAPI CReqStkRpcFixedIncomeSysCancelField
{
    int           iOrderdate;       // 委托日期
    LONGLONG      llFundid;         // 资金帐户
    int           iOrdersno;        // 委托序号
    char          szBsflag[2 + 1];  // 买卖类别
};
struct MATRADEAPI CRspStkRpcFixedIncomeSysCancelField
{
    int           iOrdersno; // 撤单委托序号
};
//-------------------------------10101741:委托撤单查询--------------------------
struct MATRADEAPI CReqStkQryRpcFixedIncomeSysCancelField
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
struct MATRADEAPI CRspStkQryRpcFixedIncomeSysCancelField
{
    int           iOrdersno;             // 委托序号
    char          szOrderid[10 + 1];     // 合同序号
    int           iOrderdate;            // 委托日期
    int           iOpertime;             // 委托时间
    LONGLONG      llFundid;              // 资金帐户
    char          chMarket;              // 客户代码
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkcode[8 + 1];      // 证券名称
    char          szStkname[8 + 1];      // 证券代码
    char          szBsflag[2 + 1];       // 买卖类别
    char          szOrderprice[21 + 1];  // 委托价格
    int           iOrderqty;             // 委托数量
    int           iMatchqty;             // 成交数量
    int           iConfernum;            // 约定号
    char          szTargettrader[6 + 1]; // 对方交易员
    char          chOrderstatus;         // 委托状态
    char          szPoststr[32 + 1];     // 定位串
};
//-------------------------------10101742:债券基本信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondBasicInfoField
{
    char          chMarket;         // 市场
    char          szStkcode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcBondBasicInfoField
{
    int           iServerid;                // 机器编码
    char          chMarket;                 // 市场
    char          szStkcode[6 + 1];         // 证券代码
    char          szBondname[8 + 1];        // 债券简称
    char          szBondfullname[64 + 1];   // 债券全称
    char          chStktype;                // 证券类型
    char          szIssueqty[21 + 1];       // 发行量
    char          szTicketprice[21 + 1];    // 债券面值
    char          chOffertype;              // 报价方式
    char          szIssueprice[21 + 1];     // 发行价格
    char          chBaseintrtype;           // 基准利率类型
    char          chIntrtype;               // 利息种类
    char          szExchrate[21 + 1];       // 标准债券折合比例
    int           iIssuedate;               // 发行日期
    int           iListingdate;             // 上市日期
    int           iIntrcaldate;             // 计息日期
    int           iIntrpaydate;             // 兑息日期
    char          szIntrpayrate[21 + 1];    // 兑息税率
    int           iPayvaluedate;            // 修改日期
    int           iExerdate;                // 行权日期
    char          szBondintr[21 + 1];       // 应计利息
    char          szTaxrate[21 + 1];        // 本次利息比重
    char          szIntrrate[21 + 1];       // 债券年利率
    int           iPaytimes;                // 付息频率
    int           iIntrenddate;             // 债券到期日
    char          szBondterm[21 + 1];       // 债券期限
    char          szMaincreditlevel[6 + 1]; // 主体信用等级
    char          szBondcreditlevel[6 + 1]; // 债项信用等级
    char          szTicketpriceadj[21 + 1]; // 分期偿还调整面值
    int           iRepaydate;               // 分期偿还日期
    char          chRisklevel;              // 风险级别
    char          chProprietyflag;          // 债券投资者适当性要求类
};
//-------------------------------10101743:柜台债券行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondMktField
{
    char          chMarket;            // 市场
    char          szStkcode[6 + 1];    // 证券代码
    char          szSecuid[10 + 1];    // 股东代码
    char          szOrderid[24 + 1];   // 合同序号
    char          szBsflag[128 + 1];   // 买卖类别
    char          szConfernum[21 + 1]; // 约定号
    char          szRecordid[21 + 1];  // 记录号
    char          szRemark[2 + 1];     // 备用标志
};
struct MATRADEAPI CRspStkQryRpcBondMktField
{
    char          szRecordid[21 + 1];    // 记录号
    char          chMarket;              // 市场
    char          szSecuid[10 + 1];      // 股东代码
    char          szStkcode[6 + 1];      // 证券代码
    char          szStkname[8 + 1];      // 证券名称
    char          szOrderqty[21 + 1];    // 申报数量
    char          szOrderprice[21 + 1];  // 申报净价
    char          szAllprice[21 + 1];    // 申报全价
    char          szTicketprice[21 + 1]; // 挂单面值
    char          szBusitype[2 + 1];     // 业务类别
    char          chCmdtype;             // 指令类别
    char          szBsflag[2 + 1];       // 买卖类别
    char          szOrderid[24 + 1];     // 合同序号
    char          szLinkman[12 + 1];     // 联系人
    char          szLinkway[30 + 1];     // 联系方式
    char          szBranchid[5 + 1];     // 机构代码
    char          szSettleorg[2 + 1];    // 结算机构
    int           iReporttime;           // 申报时间
    int           iDeaddate;             // 失效日期
    char          szConfernum[21 + 1];   // 约定号
    char          szIncomerate[21 + 1];  // 参数收益率
    char          szOrderqty2[21 + 1];   // 申报数量2
    char          szOrderprice2[21 + 1]; // 申报价格2
    char          szIncomerate2[21 + 1]; // 参数收益率2
    int           iStatus;               // 记录状态
    char          szBondlevel[6 + 1];    // 债券评级
    char          szBondterm[21 + 1];    // 剩余期限
    char          szIntrrate[21 + 1];    // 票面利率
    char          szIntrendrate[21 + 1]; // 到期收益率
    char          szExerrate[21 + 1];    // 行权收益率
    char          szBondintr[21 + 1];    // 应付利息
    char          szRemark[2 + 1];       // 备用标志
    char          szText[80 + 1];        // 备用文本
};
//-------------------------------10101744:柜台理财产品份额转让撤单--------------------------
struct MATRADEAPI CReqStkRpcFinancialProductShareTransferCancelField
{
    LONGLONG      llFundid;          // 资金帐户
    char          szOrderid[24 + 1]; // 合同编号
};
struct MATRADEAPI CRspStkRpcFinancialProductShareTransferCancelField
{
    int           iSno; // 委托序号
};
//-------------------------------10101745:柜台理财产品份额转让业务--------------------------
struct MATRADEAPI CReqStkRpcFinancialProductShareTransferField
{
    char          chMarket;             // 交易市场
    LONGLONG      llFundid;             // 资金帐户
    int           iTacode;              // 基金公司
    char          szTaacc[12 + 1];      // 基金帐号
    char          szTransacc[17 + 1];   // 交易帐号
    char          szOfcode[6 + 1];      // 产品代码
    char          szBsflag[2 + 1];      // 买卖类型
    char          szOrderid[24 + 1];    // 合同编号
    char          szOrderprice[21 + 1]; // 成交价格
    char          szOrderqty[21 + 1];   // 委托份额
    char          szConfernum[32 + 1];  // 约定号
    int           iDeaddate;            // 失效日期
    char          szOldorderid[24 + 1]; // 原合同编号
    char          szLinkname[17 + 1];   // 联系人姓名
    char          szLinktelno[32 + 1];  // 联系人电话
    LONGLONG      llRisksignsno;        // 风险揭示签署流水号
    char          szRemark[64 + 1];     // 备注信息
};
struct MATRADEAPI CRspStkRpcFinancialProductShareTransferField
{
    int           iSno;              // 委托序号
    char          szOrderid[24 + 1]; // 合同编号
};
//-------------------------------10101746:柜台理财产品转让联系人信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcFinancialProductShareTransferContactInfoField
{
    char          szOrgid[4 + 1];   // 机构编码
    char          szName[16 + 1];   // 联系人姓名
    char          szTelno[32 + 1];  // 联系电话
    char          chDefaultflag;    // 默认标识
};
struct MATRADEAPI CRspStkQryRpcFinancialProductShareTransferContactInfoField
{
    char          szOrgid[4 + 1];  // 机构编码
    char          szName[16 + 1];  // 联系人姓名
    char          szTelno[32 + 1]; // 联系电话
    char          chDefaultflag;   // 默认标识
};
//-------------------------------10101747:柜台理财产品份额转让费用计算--------------------------
struct MATRADEAPI CReqStkRpcFinancialProductShareTransferFeeField
{
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llFundid;             // 资金帐户
    char          szOfcode[6 + 1];      // 产品代码
    char          szBsflag[2 + 1];      // 买卖类型
    char          szOrderprice[21 + 1]; // 成交价格
    char          szOrderqty[21 + 1];   // 委托份额
};
struct MATRADEAPI CRspStkRpcFinancialProductShareTransferFeeField
{
    char          szOrderfee[21 + 1]; // 委托佣金
};
//-------------------------------10101748:柜台债券参数信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondParamInfoField
{
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
    char          chOffermode;      // 报价方式
};
struct MATRADEAPI CRspStkQryRpcBondParamInfoField
{
    int           iServerid;        // 节点编号
    char          chMarket;         // 市场
    char          szStkcode[6 + 1]; // 证券代码
    char          chOffermode;      // 报价方式
    char          szBsflag[2 + 1];  // 买卖类别
    char          chCancelflag;     // 撤单标志
    int           iBuyunit;         // 买入单位
    int           iSaleunit;        // 卖出单位
    int           iMaxqty;          // 交易最高数量
    int           iMinqty;          // 交易最低数量
};
//-------------------------------10101749:意向委托流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcBondIntentionOrderStatementField
{
    char          szOrgid[4 + 1];   // 机构编码
    char          szBsflag[2 + 1];  // 买卖类别
    LONGLONG      llCustid;         // 客户代码
    LONGLONG      llFundid;         // 资金账号
    char          chMarket;         // 交易市场
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    char          chStatus;         // 状态
    char          chDatetype;       // 日期类型
    int           iBegindate;       // 起始日期
    int           iEnddate;         // 结束日期
    int           iCount;           // 请求行数
    char          szPosstr[64 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcBondIntentionOrderStatementField
{
    char          szPosstr[64 + 1];     // 定位串
    int           iServerid;            // 节点编号
    int           iOrdersno;            // 委托序号
    char          szOrderid[10 + 1];    // 合同序号
    int           iOrderdate;           // 委托日期
    char          szOrgid[4 + 1];       // 机构编码
    LONGLONG      llFundid;             // 资金账号
    LONGLONG      llCustid;             // 客户代码
    char          chMarket;             // 交易市场
    char          szSecuid[10 + 1];     // 证券账号
    char          szStkcode[8 + 1];     // 证券代码
    char          szOrderprice[21 + 1]; // 委托价格
    int           iOrderqty;            // 委托数量
    char          szLinkname[16 + 1];   // 联系人
    char          szLinktelno[32 + 1];  // 联系人电话
    char          szBsflag[2 + 1];      // 买卖类别
    int           iDeaddate;            // 失效日期
    char          chStatus;             // 状态
};
//-------------------------------10101750:柜台理财产品行情查询--------------------------
struct MATRADEAPI CReqStkQryRpcFinancialProductMarketField
{
    char          chMarket;            // 市场
    char          szOfcode[6 + 1];     // 产品代码
    char          szTaacc[12 + 1];     // 基金帐号
    char          szOrderid[24 + 1];   // 合同序号
    char          szBsflag[128 + 1];   // 买卖类别
    char          szConfernum[21 + 1]; // 约定号
    char          szRecordid[21 + 1];  // 记录号
    char          szRemark[2 + 1];     // 备用标志
};
struct MATRADEAPI CRspStkQryRpcFinancialProductMarketField
{
    char          szRecordid[21 + 1];   // 记录号
    char          chMarket;             // 市场
    char          szTaacc[12 + 1];      // 基金帐号
    char          szOfcode[6 + 1];      // 产品代码
    char          szOfname[20 + 1];     // 产品名称
    char          szOrderqty[21 + 1];   // 申报数量
    char          szOrderprice[21 + 1]; // 申报净价
    char          szBusitype[2 + 1];    // 业务类别
    char          chCmdtype;            // 指令类别
    char          szOrderid[24 + 1];    // 合同序号
    char          szLinkman[12 + 1];    // 联系人
    char          szLinkway[30 + 1];    // 联系方式
    char          szBranchid[5 + 1];    // 机构代码
    int           iReporttime;          // 申报时间
    int           iDeaddate;            // 失效日期
    char          szConfernum[21 + 1];  // 约定号
    int           iStatus;              // 记录状态
    char          szRemark[2 + 1];      // 备用标志
    char          szText[80 + 1];       // 备用文本
};
//-------------------------------10101751:柜台理财产品委托查询--------------------------
struct MATRADEAPI CReqStkQryRpcFinancialProductOrderField
{
    char          chOrgid;           // 机构编码
    LONGLONG      llFundid;          // 资金帐户
    int           iTacode;           // 基金公司
    char          szOfcode[6 + 1];   // 产品代码
    char          szBsflag[2 + 1];   // 买卖类型
    char          szOrderid[24 + 1]; // 合同编号
    int           iOrderdate;        // 委托日期
    char          szPoststr[21 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcFinancialProductOrderField
{
    int           iSno;                 // 委托序号
    char          szOrderid[24 + 1];    // 合同编号
    char          chMarket;             // 市场
    char          szTaacc[12 + 1];      // 基金帐号
    char          szOfcode[6 + 1];      // 产品代码
    char          szOfname[20 + 1];     // 产品名称
    char          szOrderqty[21 + 1];   // 申报数量
    char          szOrderprice[21 + 1]; // 申报净价
    char          szBsflag[2 + 1];      // 买卖类别
    char          szLinkman[12 + 1];    // 联系人
    char          szLinkway[30 + 1];    // 联系方式
    char          szBranchid[5 + 1];    // 机构代码
    int           iReporttime;          // 申报时间
    int           iDeaddate;            // 失效日期
    char          szConfernum[21 + 1];  // 约定号
    int           iOrderstatus;         // 委托状态
    char          chStatus;             // 状态
    char          szPoststr[21 + 1];    // 定位串
};
//-------------------------------10101752:证券出借委托外围接口--------------------------
struct MATRADEAPI CReqStkRpcRefinancingStkLendingOrderField
{
    char          chDataOri;            // 数据来源
    int           iOrgCode;             // 分支机构
    LONGLONG      llCustCode;           // 客户代码
    char          chMarket;             // 市场代码
    char          szSecuAcc[10 + 1];    // 证券账户
    char          szSecuCode[8 + 1];    // 证券代码
    LONGLONG      llLendQty;            // 出借数量
    int           iCreditDays;          // 转融通期限
    char          chReqSrc;             // 需求来源
    char          chReportType;         // 申报类型
    char          szOpSecuAcc[10 + 1];  // 对手方证券账号
    char          szOpTradeUnit[6 + 1]; // 对手方交易单元
    int           iAgreementId;         // 约定号
    char          szTradeUnit[6 + 1];   // 交易单元
    char          chChannel;            // 渠道
    char          chStockChkFlag;       // 股份检查标志
    char          chStockFrozenFlag;    // 股份冻结标志
    LONGLONG      llAccount;            // 资产账户
    char          chSendFlag;           // 发起方式
};
struct MATRADEAPI CRspStkRpcRefinancingStkLendingOrderField
{
    char          szOrderId[10 + 1]; // 委托编号
    LONGLONG      llReqId;           // 需求序号
};
//-------------------------------10101753:证券出借委托撤单外围接口--------------------------
struct MATRADEAPI CReqStkRpcRefinancingStkLendingCancelField
{
    char          chDataOri;           // 数据来源
    int           iOrgCode;            // 分支机构
    LONGLONG      llCustCode;          // 客户代码
    char          chMarket;            // 市场代码
    char          szSecuAcc[10 + 1];   // 证券账户
    LONGLONG      llReqId;             // 需求序号
    LONGLONG      llOrderSno;          // 委托序号
    char          chStockUnfrozenFlag; // 股份解冻标志
    LONGLONG      llAccount;           // 资产账户
};
struct MATRADEAPI CRspStkRpcRefinancingStkLendingCancelField
{
};
//-------------------------------10101754:委托查询外围接口--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingOrderField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    char          szSecuCode[8 + 1]; // 证券代码
    int           iCreditDays;       // 转融通期限
    char          szOrderId[10 + 1]; // 委托编号
    LONGLONG      llOrderSno;        // 委托序号
    char          szPosstr[256 + 1]; // 定位串
    int           iCount;            // 每页行数
};
struct MATRADEAPI CRspStkQryRpcRefinancingOrderField
{
    char          szOrderId[10 + 1];   // 委托编号
    int           iTrdDate;            // 交易日期
    char          chMarket;            // 市场代码
    char          szSecuAcc[10 + 1];   // 证券帐号
    char          szSecuCode[8 + 1];   // 证券代码
    char          szSecuName[16 + 1];  // 证券名称
    char          szPub[6 + 1];        // 交易单元
    LONGLONG      llLendQty;           // 出借数量
    int           iCreditDays;         // 转融通期限
    char          szBusiCode[4 + 1];   // 订单类型
    char          szOpSecuAcc[10 + 1]; // 对手方证券帐号
    char          szOpPub[6 + 1];      // 对手方交易单元
    char          szOpOrderId[10 + 1]; // 对手方委托编号
    char          chOrderStatus;       // 委托状态
    char          chValidFlag;         // 合法标识
    LONGLONG      llReqId;             // 需求编号
    char          szPosstr[256 + 1];   // 定位串
    int           iOrgCode;            // 机构代码
    LONGLONG      llCustCode;          // 客户代码
    LONGLONG      llAccount;           // 资金账号
    char          szAfrReason[2 + 1];  // 提前索还原因
};
//-------------------------------10101755:提前索还外围接口--------------------------
struct MATRADEAPI CReqStkRpcRefinancingAdvanceRevertField
{
    char          chMarket;           // 市场代码
    char          szSecuAcc[10 + 1];  // 证券帐号
    int           iOpeningDate;       // 开仓日期
    LONGLONG      llContractNo;       // 合约编号
    LONGLONG      llAfrQty;           // 索还数量
    char          szAfrReason[2 + 1]; // 提前索还原因
};
struct MATRADEAPI CRspStkRpcRefinancingAdvanceRevertField
{
    char          szOrderId[10 + 1]; // 委托编号
};
//-------------------------------10101756:同意展期外围接口--------------------------
struct MATRADEAPI CReqStkRpcRefinancingExtentionApproveField
{
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    int           iOpeningDate;      // 开仓日期
    LONGLONG      llContractNo;      // 合约编号
    LONGLONG      llOpContractNo;    // 对手方合约编号
    char          chOpFlag;          // 同意展期索还标志
    LONGLONG      llExQty;           // 展期数量
};
struct MATRADEAPI CRspStkRpcRefinancingExtentionApproveField
{
    char          szOrderId[10 + 1]; // 委托编号
};
//-------------------------------10101757:证券出借可撤记录查询外围接口--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingStkLendingCanWithdrawField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    char          szSecuCode[6 + 1]; // 证券代码
    char          szOrderId[10 + 1]; // 委托编号
    char          szPosstr[64 + 1];  // 定位串
    int           iCount;            // 每页行数
};
struct MATRADEAPI CRspStkQryRpcRefinancingStkLendingCanWithdrawField
{
    char          szPosstr[64 + 1];     // 定位串
    LONGLONG      llReqId;              // 需求编号
    char          szOrderId[10 + 1];    // 委托编号
    LONGLONG      llOrderSno;           // 委托序号
    int           iTrdDate;             // 交易日期
    char          chMarket;             // 市场代码
    char          szSecuAcc[10 + 1];    // 证券账号
    char          szSecuCode[6 + 1];    // 证券代码
    char          szSecuName[16 + 1];   // 证券名称
    char          szTradeUnit[6 + 1];   // 交易单元
    LONGLONG      llLendQty;            // 出借数量
    int           iCreditDays;          // 转融通期限
    char          szBusiCode[4 + 1];    // 订单类型
    char          szOpSecuAcc[10 + 1];  // 对手方证券账号
    char          szOpTradeUnit[6 + 1]; // 对手方交易单元
    char          szOpOrderId[10 + 1];  // 对手方委托编号
    int           iAgreementId;         // 约定号
};
//-------------------------------10101758:转融券未了结合约查询外围接口--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingOpenContractField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    char          szSecuCode[6 + 1]; // 证券代码
    char          chRfsClsdtFlag;    // 转融券业务类型
    int           iCreditDays;       // 转融通期限
    int           iBgnDate;          // 开始日期
    int           iEndDate;          // 结束日期
    char          szPosstr[64 + 1];  // 定位串
    int           iCount;            // 每页行数
};
struct MATRADEAPI CRspStkQryRpcRefinancingOpenContractField
{
    char          szPosstr[64 + 1];       // 定位串
    LONGLONG      llContractNo;           // 合约编号
    int           iOpeningDate;           // 开仓日期
    char          chMarket;               // 交易市场
    char          szSecuAcc[10 + 1];      // 证券帐号
    char          szSecuCode[8 + 1];      // 证券代码
    char          szPub[6 + 1];           // 交易单元
    char          chRfsClsdtFlag;         // 转融券业务类型
    int           iCreditDays;            // 转融通期限
    LONGLONG      llContractQty;          // 合约数量
    char          szIntendingFee[21 + 1]; // 合约预计利息
    int           iCloseDate;             // 合约到期日
    char          szContractAmt[21 + 1];  // 合约金额
    char          chExAfrFlag;            // 同意展期索还标志
    char          szSecuName[16 + 1];     // 证券名称
    char          szCreditRate[21 + 1];   // 转融通息费率
};
//-------------------------------10101759:可出借标的券查询外围接口--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingUnderlyingStkForLendingField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuCode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcRefinancingUnderlyingStkForLendingField
{
    char          chMarket;             // 市场代码
    char          chRfsCls;             // 转融券业务类型
    int           iCreditDays;          // 转融通期限
    char          szSecuCode[8 + 1];    // 证券代码
    char          szStdFeeRate[21 + 1]; // 标准费率
    char          szFeeRate[21 + 1];    // 费率
    char          chExFlag;             // 展期标志
    char          chAfrFlag;            // 索还标志
    char          chRfFlag;             // 转融通标志
};
//-------------------------------10101760:出借人信息查询外围接口--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingLenderInfoField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
};
struct MATRADEAPI CRspStkQryRpcRefinancingLenderInfoField
{
    char          chDataOri;          // 数据来源
    int           iOrgCode;           // 机构代码
    LONGLONG      llCustCode;         // 客户代码
    char          szCustName[32 + 1]; // 客户名称
    char          chCustCls;          // 客户类别
    char          chCustStatus;       // 客户状态
    char          chCurrency;         // 货币代码
    char          chMarket;           // 交易市场
    char          szSecuAcc[10 + 1];  // 证券账户
    char          chSecuStatus;       // 股东状态
    char          chSecuCls;          // 股东分类
    char          chLendFlag;         // 出借标志
    char          chRfsFlag;          // 融券预约标志
    char          szTradeUnit[6 + 1]; // 交易单元
    int           iContrBgnDate;      // 合同开始日期
    int           iContrEndDate;      // 合同结束日期
    char          chContrStatus;      // 合同状态
};
//-------------------------------10101761:历史出借委托查询--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingHisLendingOrderField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    int           iBgnDate;          // 开始日期
    int           iEndDate;          // 结束日期
    char          szSecuCode[8 + 1]; // 证券代码
    int           iCreditDays;       // 转融通期限
    char          szOrderId[10 + 1]; // 委托编号
    LONGLONG      llOrderSno;        // 委托序号
    char          szPosstr[256 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcRefinancingHisLendingOrderField
{
    char          szOrderId[10 + 1];   // 委托编号
    int           iOrderDate;          // 委托日期
    char          chMarket;            // 市场代码
    char          szSecuAcc[10 + 1];   // 证券帐号
    char          szSecuCode[8 + 1];   // 证券代码
    char          szSecuName[16 + 1];  // 证券名称
    char          szPub[6 + 1];        // 交易单元
    LONGLONG      llOrderQty;          // 出借数量
    int           iCreditDays;         // 转融通期限
    char          szBusiCode[4 + 1];   // 订单类型
    char          szOpSecuAcc[10 + 1]; // 对手方证券帐号
    char          szOpPub[6 + 1];      // 对手方交易单元
    char          szOpOrderId[10 + 1]; // 对手方委托编号
    char          chOrderStatus;       // 委托状态
    char          chValidFlag;         // 合法标识
    LONGLONG      llOrderSno;          // 委托序号
};
//-------------------------------10101762:证券出借已了结合约查询--------------------------
struct MATRADEAPI CReqStkQryRpcRefinancingStkLendingClosedContractField
{
    int           iOrgCode;          // 机构代码
    LONGLONG      llCustCode;        // 客户代码
    char          chMarket;          // 市场代码
    char          szSecuAcc[10 + 1]; // 证券帐号
    int           iBgnDate;          // 开始日期
    int           iEndDate;          // 结束日期
    char          szSecuCode[8 + 1]; // 证券代码
};
struct MATRADEAPI CRspStkQryRpcRefinancingStkLendingClosedContractField
{
    char          szSettDate[10 + 1]; // 清算日期
    int           iTrdDate;           // 成交日期
    LONGLONG      llContractNo;       // 合约编号
    int           iOpeningDate;       // 开仓日期
    char          chBusiType;         // 转融通类别
    char          chMarket;           // 交易市场
    char          szSecuAcc[10 + 1];  // 证券帐号
    char          szPub[6 + 1];       // 交易单元
    int           iCreditDays;        // 转融通期限
    LONGLONG      llContractQty;      // 合约数量
    char          szSecuCode[8 + 1];  // 证券代码
    LONGLONG      llSettQty;          // 交收数量
    LONGLONG      llSettAmt;          // 交收金额
    LONGLONG      llSettPrincipal;    // 交收本金
    LONGLONG      llSettInterest;     // 交收利息
    LONGLONG      llSettPenalty;      // 交收罚息
    LONGLONG      llSettPenalSum;     // 交收违约金
    LONGLONG      llSettOtherFee;     // 交收其他费
    LONGLONG      llClosePrice;       // 收盘价
    char          szRemark[64 + 1];   // 备注
    int           iClosedDate;        // 交收日期
};
//-------------------------------10101763:查询客户的预留信息--------------------------
struct MATRADEAPI CReqStkQryRpcSysSecurityCustReservedInfoField
{
    LONGLONG      llCustid;         // 客户代码
    char          szOrgid[4 + 1];   // 机构编码
    char          szBrhid[4 + 1];   // 机构分支
};
struct MATRADEAPI CRspStkQryRpcSysSecurityCustReservedInfoField
{
    char          szPresetinfo[255 + 1]; // 客户预留信息
    char          szRemark[64 + 1];      // 备注信息
};
//-------------------------------10101764:设置客户预留信息--------------------------
struct MATRADEAPI CReqStkRpcSysSecuritySetCustReservedInfoField
{
    char          szOrgid[4 + 1];        // 机构编码
    char          szBrhid[4 + 1];        // 机构分支
    LONGLONG      llCustid;              // 客户代码
    char          szPresetinfo[255 + 1]; // 客户预留信息
    char          szRemark[64 + 1];      // 备注信息
};
struct MATRADEAPI CRspStkRpcSysSecuritySetCustReservedInfoField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101765:同步客户动态令牌--------------------------
struct MATRADEAPI CReqStkRpcSysSecuritySyncDynamicTokenField
{
    char          szFuncid[16 + 1];   // 功能号
    char          szCustorgid[8 + 1]; // 客户开户分支
    char          szNetaddr[16 + 1];  // 登录机器的IP
    char          szOrgid[8 + 1];     // 登录所在分支
    char          chOperway;          // 操作渠道
    char          chExt;              // 扩展标识
    char          chInputtype;        // 用户账户类型
    char          szInputid[32 + 1];  // 用户账户
    char          chCustprop;         // 用户角色
    char          szDynpwd1[16 + 1];  // 第一个动态口令
    char          szDynpwd2[16 + 1];  // 第二个动态口令
};
struct MATRADEAPI CRspStkRpcSysSecuritySyncDynamicTokenField
{
    char          szEtokensn[32 + 1]; // 令牌序列号
    char          szUsercn[128 + 1];  // 用户CN
    char          szDynpwd1[16 + 1];  // 第一个动态口令
    char          szDynpwd2[16 + 1];  // 第二个动态口令
    char          szUnlocked[8 + 1];  // 客户帐户解锁标识
};
//-------------------------------10101766:客户证书获取随机码--------------------------
struct MATRADEAPI CReqStkRpcSysSecurityCertificateRandomCodeField
{
    char          chInputtype;       // 用户账户类型
    char          szInputid[32 + 1]; // 用户账户
    char          chUserrole;        // 用户角色
    char          chCertType;        // 证书类型
};
struct MATRADEAPI CRspStkRpcSysSecurityCertificateRandomCodeField
{
    LONGLONG      llUserCode;        // 用户代码
    char          chUserRole;        // 用户角色
    char          chCaType;          // 证书类型
    char          szRndcode[16 + 1]; // 随机码
};
//-------------------------------10101767:客户动态令牌状态设置--------------------------
struct MATRADEAPI CReqStkRpcSysSecuritySetDynamicTokenStatusField
{
    char          szOrgid[4 + 1];       // 机构编码
    char          chInputtype;          // 帐号类型
    char          szInputid[64 + 1];    // 帐号
    char          chCustprop;           // 用户角色
    char          chOperway;            // 操作方式
    char          szTokenstatus[8 + 1]; // 令牌状态
    char          szDynpwd[33 + 1];     // 动态口令
    char          szEtokenpin[33 + 1];  // 动态口令pin码
};
struct MATRADEAPI CRspStkRpcSysSecuritySetDynamicTokenStatusField
{
    LONGLONG      llUserCode; // 用户代码
    char          chUserRole; // 用户角色
    char          chStatus;   // 令牌状态
};
//-------------------------------10101768:客户获取一次性动态口令--------------------------
struct MATRADEAPI CReqStkRpcSysSecurityOneTimeDynamicPwdField
{
    char          szOrgid[4 + 1];    // 机构编码
    char          chInputtype;       // 帐号类型
    char          szInputid[64 + 1]; // 帐号
    char          chCustprop;        // 用户角色
    char          chOperway;         // 操作方式
};
struct MATRADEAPI CRspStkRpcSysSecurityOneTimeDynamicPwdField
{
    LONGLONG      llUserCode;          // 用户代码
    char          chUserRole;          // 用户角色
    char          szTmpdynpwd[16 + 1]; // 一次性动态口令
    char          szMobileno[32 + 1];  // 申请令牌时预留手机号
};
//-------------------------------10101769:客户登陆绑定信息设置--------------------------
struct MATRADEAPI CReqStkRpcSysSecuritySetLoginBindingInfoField
{
    char          szOrgid[4 + 1];         // 机构编码
    LONGLONG      llCustid;               // 客户代码
    char          szComputername[32 + 1]; // 计算机昵称
    char          chBindtype;             // 绑定类型
    char          szBindcontext[128 + 1]; // 绑定内容
    char          chBindstatus;           // 绑定状态
    char          szOtpcode[32 + 1];      // 手机口令
    char          szRemark[64 + 1];       // 备注信息
    char          chAction;               // 操作方式
};
struct MATRADEAPI CRspStkRpcSysSecuritySetLoginBindingInfoField
{
    char          szMsgok[32 + 1]; // 成功信息
};
//-------------------------------10101770:客户登陆绑定信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcSysSecurityLoginBindingInfoField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 客户代码
    char          chBindtype;       // 绑定类型
    char          chBindstatus;     // 绑定状态
};
struct MATRADEAPI CRspStkQryRpcSysSecurityLoginBindingInfoField
{
    int           iServerid;              // 机器编码
    LONGLONG      llCustid;               // 客户代码
    char          szOrgid[4 + 1];         // 机构编码
    char          chBindtype;             // 绑定类型
    char          szBindcontext[128 + 1]; // 绑定内容
    int           iBinddate;              // 绑定日期
    int           iBindtime;              // 绑定时间
    char          szComputername[32 + 1]; // 计算机昵称
    char          chBindstatus;           // 绑定状态
    char          szRemark[64 + 1];       // 备注信息
};
//-------------------------------10101771:校验客户交易密码--------------------------
struct MATRADEAPI CReqStkRpcSysSecurityVerifyTrdPwdField
{
    char          chInputtype;       // 用户账户类型
    char          szInputid[32 + 1]; // 用户账户
};
struct MATRADEAPI CRspStkRpcSysSecurityVerifyTrdPwdField
{
    char          szMsgok[32 + 1]; // 成功信息
    char          chStatus;        // 客户状态
    char          szOrgid[4 + 1];  // 机构编码
    char          chTokenuser;     // 令牌客户
    LONGLONG      llCustid;        // 客户代码
};
//-------------------------------10101772:客户获取手机一次性动态口令--------------------------
struct MATRADEAPI CReqStkRpcSysSecurityMobileOneTimeDynamicPwdField
{
    char          szOrgid[4 + 1];   // 机构编码
    LONGLONG      llCustid;         // 帐号类型
};
struct MATRADEAPI CRspStkRpcSysSecurityMobileOneTimeDynamicPwdField
{
    LONGLONG      llUserCode;        // 用户代码
    char          szRndcode[32 + 1]; // 用户角色
};
//-------------------------------10101773:查询客户ETF篮各成分股的持仓数量--------------------------
struct MATRADEAPI CReqStkQryRpcETFConstituentStkPosiField
{
    char          chMarket;         // 交易市场
    LONGLONG      llFundid;         // 资金账户
    char          szEtfcode[8 + 1]; // ETF代码
};
struct MATRADEAPI CRspStkQryRpcETFConstituentStkPosiField
{
    char          chMarket;                // 交易市场
    char          szEtfcode[8 + 1];        // ETF代码
    char          szStkcode[8 + 1];        // 成份股代码
    char          szStkname[8 + 1];        // 成份股简称
    char          chStkmarket;             // 成份股交易市场
    int           iAmount;                 // 成分股数量
    int           iStkavl;                 // 客户持仓数量
    char          chReplaceflag;           // 替代标志
    char          szOverpricerate[21 + 1]; // 溢价比例
    char          szReplaceamtSg[21 + 1];  // 申购替代金额
    char          szReplaceamtSh[21 + 1];  // 赎回替代金额
};
//-------------------------------10101774:普通节点和VIP系统间证券划拨--------------------------
struct MATRADEAPI CReqStkRpcCommVIPStkTransferField
{
    char          szOrgid[4 + 1];   // 机构编码
    char          chMarket;         // 交易市场
    LONGLONG      llFundid;         // 资金账号
    char          szSecuid[10 + 1]; // 股东代码
    char          szStkcode[8 + 1]; // 证券代码
    LONGLONG      llStkavl;         // 划拨数量
    char          chDirect;         // 划拨方向
};
struct MATRADEAPI CRspStkRpcCommVIPStkTransferField
{
    int           iSno;    // 操作流水号
    int           iVipsno; // VIP操作流水号
};
//-------------------------------10101775:现金资产转银行--------------------------
struct MATRADEAPI CReqStkRpcCashAssetBankTransferField
{
    int           iFundid;           // 资金账号
    char          chMoneytype;       // 货币代码
    char          szFundpwd[32 + 1]; // 资金密码
    char          szBankcode[4 + 1]; // 银行代码
    char          szBankpwd[32 + 1]; // 银行密码
    char          chBanktrantype;    // 转帐类型
    char          szTranamt[21 + 1]; // 转帐金额
    char          chPwdflag;         // 资金密码校验标志
    int           nEncodeType;       // 加密方式 1:W版集中交易 2:U版集中交易
};
struct MATRADEAPI CRspStkRpcCashAssetBankTransferField
{
    int           iSno;             // 委托序号
    int           iSyserrid;        // 错误代码
    char          szErrmsg[64 + 1]; // 错误信息
};
//-------------------------------10101776:资金划拨流水查询--------------------------
struct MATRADEAPI CReqStkQryRpcCapitalTransferStatementField
{
    LONGLONG      llFundid;          // 资金帐户
    LONGLONG      llExtsno;          // 外部流水号
    char          chQryflag;         // 查询方向
    int           iCount;            // 请求行数
    char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspStkQryRpcCapitalTransferStatementField
{
    char          szPoststr[32 + 1]; // 定位串
    int           iOperdate;         // 操作日期
    int           iSysdate;          // 操作的交易日期
    int           iSno;              // 系统流水号
    char          szOrgid[4 + 1];    // 机构编码
    char          szBrhid[4 + 1];    // 机构分支
    int           iOpertime;         // 发生时间
    int           iDigestid;         // 摘要代码
    LONGLONG      llCustid;          // 客户代码
    char          szName[16 + 1];    // 客户中文姓名
    LONGLONG      llFundid;          // 资金账号
    char          chMoneytype;       // 货币代码
    char          szRemark[64 + 1];  // 备注信息
    LONGLONG      llExtsno;          // 外部流水号
};
//-------------------------------10101777:Vip客户资金资产同步表查询--------------------------
struct MATRADEAPI CReqStkQryRpcVIPCuacctSyncTableField
{
    LONGLONG      llFundid;             // 资产账户
    char          szMoneytype[128 + 1]; // 货币类型
};
struct MATRADEAPI CRspStkQryRpcVIPCuacctSyncTableField
{
    int           iServerid;              // 机器编码
    char          szOrgid[4 + 1];         // 机构编码
    LONGLONG      llCustid;               // 客户代码
    char          chMoneytype;            // 货币代码
    LONGLONG      llFundid;               // 资金帐号
    char          szFundlastbal[21 + 1];  // 昨日余额
    char          szFundbal[21 + 1];      // 到帐余额
    char          szFundavl[21 + 1];      // 可用资金
    char          szFundbuy[21 + 1];      // 买入冻结
    char          szFundsale[21 + 1];     // 卖出解冻
    char          szOverdraw[21 + 1];     // 透支限额
    char          szFundfrz[21 + 1];      // 冻结总金额
    char          szFundunfrz[21 + 1];    // 解冻总金额
    char          szFundtrdfrz[21 + 1];   // 交易解冻金额
    char          szFundnightfrz[21 + 1]; // 夜市冻结资金
    char          szFundloan[21 + 1];     // 融资总金额
    char          szMarketvalue[21 + 1];  // 证券市值
};
//-------------------------------10101778:场内产品适当性信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcFloorTradingProductAppropriatenessInfoField
{
    char          chMarket;          // 交易市场
    char          szStkcode[8 + 1];  // 证券代码
    char          szBsflag[128 + 1]; // 买卖类别
};
struct MATRADEAPI CRspStkQryRpcFloorTradingProductAppropriatenessInfoField
{
    char          chMarket;                // 交易市场
    char          szStkcode[8 + 1];        // 证券代码
    char          szStkname[8 + 1];        // 证券名称
    char          chStktype;               // 证券类别
    char          szBsflag[2 + 1];         // 买卖类别
    char          chRisklevel;             // 风险等级
    char          chInvestmentperiod;      // 投资期限
    char          szInvestmenttype[2 + 1]; // 投资品种
    char          chExincometype;          // 预期收益
};
//-------------------------------10101779:客户适当性信息查询--------------------------
struct MATRADEAPI CReqStkQryRpcAppropriatenessInfoField
{
};
struct MATRADEAPI CRspStkQryRpcAppropriatenessInfoField
{
    char          chInvestortype;            // 投资者类型
    int           iTrdprofvaliddate;         // 专业投资者有效期
    char          chRisklevel;               // 风险等级
    int           iRisklevelvaliddate;       // 风险等级有效期
    char          chLowriskflag;             // 最低风险承受能力标识
    char          chInvestmentperiod;        // 投资期限
    char          szInvestmenttype[128 + 1]; // 投资品种
    char          chExincometype;            // 预期收益
    char          szRiskpropright[32 + 1];   // 相应风险等级购买协议
};
//-------------------------------10101780:客户与场内产品适当性匹配校验--------------------------
struct MATRADEAPI CReqStkRpcAppropriatenessMatchingCheckField
{
    int           iMarket;          // 交易市场
    char          szStkcode[8 + 1]; // 证券代码
    char          szBsflag[2 + 1];  // 买卖类别
};
struct MATRADEAPI CRspStkRpcAppropriatenessMatchingCheckField
{
    char          chResultcode;            // 匹配结果
    char          szResultmsg[128 + 1];    // 匹配结果描述
    char          chRisklevelinvalid;      // 客户风险等级过期标识
    int           iRisklevelvaliddate;     // 客户风险等级有效期
    char          chInvestortype;          // 投资者类型
    int           iTrdprofvaliddate;       // 专业投资者有效期
    char          chLowriskflag;           // 最低风险承受能力标识
    char          szRiskpropright[32 + 1]; // 相应风险等级购买协议
    char          chOrderctrlflag;         // 是否允许强制下单标志
};
//-------------------------------10101781:记录场内业务风险揭示书签署流水--------------------------
struct MATRADEAPI CReqStkRpcFloorTradingRiskDisclosureStatementSignedRecordFlowField
{
    char          szSigntype[64 + 1]; // 风险揭示书类型
    char          szVersion[64 + 1];  // 风险揭示书版本
    char          chMarket;           // 交易市场
    char          szStkcode[8 + 1];   // 证券代码
    char          szBsflag[2 + 1];    // 买卖类别
    char          chConfirmflag;      // 确认标识
};
struct MATRADEAPI CRspStkRpcFloorTradingRiskDisclosureStatementSignedRecordFlowField
{
    LONGLONG      llRisksignsno; // 风险揭示书签署流水序号
};

//-------------------------------10349001:系统探测--------------------------
struct MATRADEAPI CReqStkBizStatusField
{
  char            szStkbd[2 + 1];             // 交易板块 不送时为系统探测，返回第一结果集； 送具体板块时，返回该板块是否可以下单以及交易日期等信息; 送*或者@时返回所有板块是否可下单信息
};

struct MATRADEAPI CRspStkBizStatusField
{
  char            szStkbd[2 + 1];             // 交易板块 
  int             iTrdDate;                   // 当前交易日 
  int             iPhysicalDate;              // 物理日期 
  char            chTrdStatus;                // 交易状态 0:禁止交易  1：允许交易
  char            szRemark[128 + 1];          // 备注 不可下单的理由
};

//-------------------------------10302002:买卖委托(快速)--------------------------
struct MATRADEAPI CReqStkOrderExField
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

struct MATRADEAPI CRspStkOrderExField
{
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号
  int             iCuacctSn;                  // 账户序号
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
};

//-------------------------------10309007:股份流水查询--------------------------
struct MATRADEAPI CReqStkQryStockFlowField
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

struct MATRADEAPI CRspStkQryStockFlowField
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
struct MATRADEAPI CReqStkQrySettDateField
{
};

struct MATRADEAPI CRspStkQrySettDateField
{
  int             iTrdDate;                   // 交易日期 
  int             iSettDate;                  // 下一交易日 
  int             iSubsysSn;                  // 子系统编号 
};

//-------------------------------10301030:盘后基金行情查询--------------------------
struct MATRADEAPI CReqStkQryLofMktInfoField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
};

struct MATRADEAPI CRspStkQryLofMktInfoField
{
  char            chStkex;                    // 证券交易所 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkLofStatus;             // 基金状态 
  int             iNavDate;                   // 净值日期 
  char            szStkLofNav[21 + 1];        // 基金净值 
  char            szStkLofNavTotal[21 + 1];   // 累计净值 
  char            chStkMsStatus;              // 合并/分拆状态 
  char            szStkLofMainCode[8 + 1];    // 主基金代码 
  int             iStkMsRatio;                // 合并/分拆比例 
  LONGLONG        llStkMergeFloor;            // 最低合并数量 
  LONGLONG        llStkSplitFloor;            // 最低分拆数量 
  char            chStkChangeStatus;          // 转换状态 
  char            szRsvField[32 + 1];         // 备用字段 
  char            szAdminCode[8 + 1];         // 管理人代码 
  char            szTaCode[8 + 1];            // TA代码 
};

//-------------------------------10305015:资金划拨(集中交易)--------------------------
struct MATRADEAPI CReqStkFundTransferJzjyField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 ‘1’集中交易划出、订单系统划入 ‘2’集中交易划入、订单系统划出
  char            szBizAmt[21 + 1];           // 发生金额 
};

struct MATRADEAPI CRspStkFundTransferJzjyField
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

//-------------------------------10303131:可用资金查询(集中交易)--------------------------
struct MATRADEAPI CReqStkQryExpendableFundJzjyField
{
  LONGLONG        llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY] 与客户代码配合使用 与资产账户配合使用
  char            chBlnFlag;                  // 最大可取标志 0计算最大可取金额 1不计算最大可取金额 默认是0
  char            chCuacctAttr;               // 账户属性 字典[CUACCT_ATTR]
};

struct MATRADEAPI CRspStkQryExpendableFundJzjyField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFundBln[21 + 1];          // 资金余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szTranAvl[21 + 1];          // 最大可取金额 
};

//-------------------------------10304013:认证信息设置(密码录入/修改)--------------------------
struct MATRADEAPI CReqStkAuthDataSetJzjyField
{
  LONGLONG        llUserCode;                 // 用户代码 
  char            szAuthData[256 + 1];        // 认证数据 明文
  char            szSrcAuthData[256 + 1];     // 原认证数据 明文
  char            chUseScope;                 // 使用范围 0: 股票交易密码 2：信用交易密码
};

struct MATRADEAPI CRspStkAuthDataSetJzjyField
{
  char            szMsgOk[32 + 1];            // 成功信息 
};

//-------------------------------10305019:资金划拨(单账户多节点) 订单节点之间划拨--------------------------
struct MATRADEAPI CReqStkFundTransferNodeField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 ‘0’划入 ‘1’划出
  char            szTransFund[21 + 1];        // 划拨金额 
};

struct MATRADEAPI CRspStkFundTransferNodeField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szTransFund[21 + 1];        // 划拨金额 
  LONGLONG        llSerialNo;                 // 流水序号 
  char            chTransStatus;              // 划拨状态 0-已调用 1-成功 2-失败 3-超时
  char            szRemark[256 + 1];          // 备注 划拨状态为2/3时返回错误原因
};

//-------------------------------10305036:银证转账--------------------------
struct MATRADEAPI CReqStkBankStkTransferField
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

struct MATRADEAPI CRspStkBankStkTransferField
{
  LONGLONG        llSerialNo;                 // 流水序号 
  char            szErrorMess[128 + 1];       // 错误信息 
};

//-------------------------------10301506:要约收购收购人信息查询--------------------------
struct MATRADEAPI CReqStkQryTenderSgrInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 要约代码 证券代码
  char            szSgrCode[8 + 1];           // 收购人编码 
};

struct MATRADEAPI CRspStkQryTenderSgrInfoField
{
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 要约代码 
  char            szSgrCode[8 + 1];           // 收购人编码 
  char            szSgPrice[21 + 1];          // 收购价格 
  char            szPurchaserNane[32 + 1];    // 收购人名称 
  int             iTenderBegDate;             // 要约开始日期 
  int             iTenderEndDate;             // 要约结束日期 
};

//-------------------------------10303224:盘后行情查询(创业板)--------------------------
struct MATRADEAPI CReqStkQryClosingMktSecondboardField
{
  char            chStkex;                    // 市场 DD[STKEX]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向前取数据 1:向后取数据
  char            szQueryPos[32 + 1];         // 定位串 第1次查询送空，后续查询必送 与查询方向配合使用
  int             iQueryNum;                  // 查询行数 一次不超过1000
};

struct MATRADEAPI CRspStkQryClosingMktSecondboardField
{
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 市场 DD[STKEX]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 
  char            szClosingPrice[21 + 1];     // 今日收盘价 
  LONGLONG        llStkMatchedQty;            // 成交数量 
  char            szStkMatchedAmt[21 + 1];    // 成交金额 
  LONGLONG        llStkBuyLeftQty;            // 买盘剩余数量 
  LONGLONG        llStkSaleLeftQty;           // 卖盘剩余数量 
  char            chStkSuspended;             // 停牌标识 DD[STK_SUSPENDED]
  char            szUpdTime[32 + 1];          // 更新时间 
  char            szRemark[56 + 1];           // 备注 
};

//-------------------------------10302017:ETF网下认购申请撤单--------------------------
struct MATRADEAPI CReqStkEtfOfflineCancelOrderField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iTrdDate;                   // 交易日期 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  LONGLONG        llFortuneCode;              // 财富账户 
};

struct MATRADEAPI CRspStkEtfOfflineCancelOrderField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkSubCode[8 + 1];        // 认购代码 
  char            szStkSubName[16 + 1];       // 认购代码名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  int             iOrderSn;                   // 委托序号 
  LONGLONG        llFortuneCode;              // 财富账户 
};

//-------------------------------10303007:ETF网下认购申请查询--------------------------
struct MATRADEAPI CReqStkQryEtfOfflineOrderField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  LONGLONG        llTrdDate;                  // 交易日期 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkSubCode[8 + 1];        // 认购代码 
  int             iOrderSn;                   // 委托序号 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkBiz[1024 + 1];         // 证券业务 字典[STK_BIZ]
  char            chStkSubStatus;             // 处理状态 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szIntOrg[1024 + 1];         // 内部机构 
  LONGLONG        llFortuneCode;              // 财富账户 
};

struct MATRADEAPI CRspStkQryEtfOfflineOrderField
{
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szOrderType[32 + 1];        // 委托类型 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iSubsysSn;                  // 子系统编码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szOrderUfzAmt[21 + 1];      // 委托解冻金额 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chIsWithdrawn;              // 已撤单标志 字典[IS_WITHDRAWN]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            szEtfName[16 + 1];          // ETF名称 
  char            szStkSubCode[8 + 1];        // 认购代码 
  char            szStkSubName[16 + 1];       // 认购代码名称 
  char            szBillFee[21 + 1];          // 单费 
  char            szCommissionRate[21 + 1];   // 认购费用比例 
  char            chStkSubStatus;             // 处理状态 '0'未认购 '1'已认购 '2'已撤销 '3'已作废 '4'已处理
  char            szTrdacctLink[20 + 1];      // 关联交易账户 
  char            szStkbdLink[2 + 1];         // 关联交易板块 字典[STKBD]
  char            szStkpbuLink[8 + 1];        // 关联交易单元 
  LONGLONG        llStkAvlFrz;                // 证券可用数量 
  int             iUpdDate;                   // 更新日期 
  char            szCustName[32 + 1];         // 客户名称 
  char            szIdCode[48 + 1];           // 证件号码 
  char            chCuacctAttr;               // 资产账户属性 
  LONGLONG        llFortuneCode;              // 财富账户 
};

//-------------------------------10303206:新股日历查询--------------------------
struct MATRADEAPI CReqStkQryNewStkCalField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkCls[2 + 1];            // 证券类别 
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  char            szFxMode[6 + 1];            // 发行方式 
};

struct MATRADEAPI CRspStkQryNewStkCalField
{
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 发行证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szGbType[2 + 1];            // 广播类别 
  char            szStkCls[2 + 1];            // 证券类别 
  char            szFxMode[6 + 1];            // 发行方式 
  LONGLONG        llStkUplimitQty;            // 申购数量上限 
  LONGLONG        llStkLwlimitQty;            // 申购数量下限 
  LONGLONG        llBuyUnit;                  // 申购基数 
  int             iBgnDate;                   // 申购开始日期 
  int             iEndDate;                   // 申购结束日期 
  char            szDataRate[21 + 1];         // 认购比例 
  char            szStkUplimitPrice[21 + 1];  // 发行价格上限 
  char            szStkLwlimitPrice[21 + 1];  // 发行价格下限 
  char            szByzd[50 + 1];             // 备用字段 
  int             iAcceptDate;                // 数据接收日期 
  char            szRemark[128 + 1];          // 备注信息 
};

//-------------------------------10307313:共享内存品种信息查询--------------------------
struct MATRADEAPI CReqStkQryStkInfoShmField
{
  char            szStkbd[2 + 1];             // 证券板块 
  char            szStkCls[2 + 1];            // 证券类别 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向后取数据(默认)
  char            szQueryPos[32 + 1];         // 定位串 输入值为记录号 与查询方向配合使用
  int             iQueryNum;                  // 查询行数 最大值为1000
};

struct MATRADEAPI CRspStkQryStkInfoShmField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkCls[2 + 1];            // 证券类别 
  char            chStkSubCls;                // 证券子类别 字典[STK_SUB_CLS]
  char            chStkStatus;                // 证券状态 
  char            chStkLotFlag;               // 单位标志 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            chStkLevel;                 // 证券级别 
  char            chIsReg;                    // 是否注册制 
  int             iDlvyCycle;                 // 交收周期 
  char            chLofSubFlag;               // 子基金标志 
  char            chBondSuitable;             // 债券适当性 
  char            szStkUplmtPrice[21 + 1];    // 涨停价格 
  char            szStkLwlmtPrice[21 + 1];    // 跌停价格 
  LONGLONG        llStkUplmtQty;              // 限价数量上限 
  LONGLONG        llStkLwlmtQty;              // 限价数量下限 
  LONGLONG        llStkUplmtQtyMp;            // 市价数量上限 
  LONGLONG        llStkLwlmtQtyMp;            // 市价数量下限 
  char            szStkSpread[21 + 1];        // 价位价差 
  int             iBuyUnit;                   // 买入最小单位 
  int             iSaleUnit;                  // 卖出最小单位 
  char            szBondInt[21 + 1];          // 债券利息 
  char            szQryPos[32 + 1];           // 定位串 
};

//-------------------------------10302122:债券竞买业务预约委托撤单--------------------------
struct MATRADEAPI CReqStkReserveOrderCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOrderId[10 + 1];          // 合同序号 和委托批号不能同时为空 合同序号不为空，就按合同序号撤单； 否则，委托批号不为空，按委托批号撤单。
  int             iOrderBsn;                  // 委托批号 如果按委托批号撤单，如果定位到撤单记录大于１条，请使用批量撤单来实现。
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llFortuneCode;              // 财富账户 
  int             iStkBiz;                    // 证券业务 
  int             iTrdDate;                   // 交易日期  不能为空
};

struct MATRADEAPI CRspStkReserveOrderCancelField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 原委托的合同序号
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 业务行为 
  char            chCancelStatus;             // 内部撤单标志 1:内部撤单 非1:普通撤单
  char            szMsgOk[32 + 1];            // 成功信息 如果是内部撤单，修改返回信息为”撤单成功”
  char            szCancelList[256 + 1];      // 撤单列表 格式: 原委托序号#撤单委托序号#内部撤单标志#错误信息|…...| 撤单委托序号=-1 表示撤单失败
  char            szWtOrderId[10 + 1];        // 委托合同号 委托的合同序号
};

//-------------------------------10301579:深圳债券报价行情查询--------------------------
struct MATRADEAPI CReqStkQryBondQuoteField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQuoteType;                // 报价类型 0：点击成交报价 1：询价申报 2：询价成交报价
  char            chSide;                     // 买卖方向 1：买入 2：卖出
  char            chQuoteStatus;              // 状态 
};

struct MATRADEAPI CRspStkQryBondQuoteField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  int             iOfferStime;                // 申报时间 
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iIntOrg;                    // 内部机构 
  char            chSide;                     // 买卖方向 
  char            chQuoteType;                // 报价类型 
  char            szOfferMsgId[10 + 1];       // 消息编号 
  char            szOfferPrice[21 + 1];       // 报价价格 
  LONGLONG        llOfferQty;                 // 报价数量 
  char            szMemberId[6 + 1];          // 交易商代码 
  char            szInvestorType[2 + 1];      // 交易主体类型 
  char            szInvestorName[256 + 1];    // 交易主体名称 
  char            szTraderCode[8 + 1];        // 交易员代码 
  char            chQuoteStatus;              // 状态 
  char            chIsAnonymity;              // 是否匿名 
  int             iOwnerType;                 // 所有者类型 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szInvestorId[10 + 1];       // 交易主体代码 
  char            szQuoteReqId[10 + 1];       // 询价请求编号 
  char            szRemark[256 + 1];          // 备注信息 
  int             iSettlType;                 // 结算方式 
  int             iSettlPeriod;               // 结算周期 
  char            chCashMargin;               // 融资融券信息标识 
  char            szCtpMemberId[8 + 1];       // 对手方交易商代码 
  char            szCtpInvestorType[2 + 1];   // 对手方交易主体类型 
  char            szCtpInvestorId[10 + 1];    // 对手方交易主体代码 
  char            szCtpInvestorName[30 + 1];  // 对手方交易主体名称 
  char            szCtpTraderCode[8 + 1];     // 对手方交易员代码 
  char            szExecId[32 + 1];           // 执行编号 
  char            szTrdMatchId[32 + 1];       // 交易编号 
  char            szOfferCustName[128 + 1];   // 申报客户姓名 
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szExpireTime[32 + 1];       // 失效时间 
};

//-------------------------------10301583:协商成交回报行情信息查询--------------------------
struct MATRADEAPI CReqStkQryReportMktInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szMemberId[8 + 1];          // 交易商代码 
  char            szCtpMemberId[8 + 1];       // 对手方交易商代码 
  char            chReportType;               // 成交申报类型 0=提交成交申报 2=接受成交申报 3=拒绝成交申报
  char            chSide;                     // 买卖方向 
  char            chStatus;                   // 状态 
  char            szCtpTraderCode[8 + 1];     // 对手方交易员代码 
};

struct MATRADEAPI CRspStkQryReportMktInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  int             iTransactStime;             // 回报时间 
  int             iOwnerType;                 // 所有者类型 
  char            szReportId[10 + 1];         // 客户成交申报编号 
  char            chReportType;               // 成交申报类型 0=提交成交申报 2=接受成交申报 3=拒绝成交申报
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szConfirmId[8 + 1];         // 约定号 
  char            chSide;                     // 买卖方向 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iIntOrg;                    // 营业部代码 
  char            szMemberId[6 + 1];          // 交易商代码 
  char            szInvestorType[2 + 1];      // 交易主体类型 
  char            szInvestorName[256 + 1];    // 交易主体名称 
  char            szInvestorId[10 + 1];       // 交易主体代码 
  char            szTraderCode[8 + 1];        // 交易员代码 
  char            szCtpMemberId[8 + 1];       // 对手方交易商代码 
  char            szCtpInvestorType[2 + 1];   // 对手方交易主体类型 
  char            szCtpInvestorName[256 + 1]; // 对手方交易主体名称 
  char            szCtpInvestorId[10 + 1];    // 对手方交易主体代码 
  char            szCtpTraderCode[8 + 1];     // 对手方交易员代码 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chStatus;                   // 状态 
  char            szRemark[256 + 1];          // 备注信息 
  int             iSettlType;                 // 结算方式 
  char            chSettlPeriod;              // 结算周期 
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  int             iTrdSubtype;                // 成交申报子业务类别 
  LONGLONG        llOrderAmt;                 // 委托金额 
  LONGLONG        llSettAmt;                  // 清算金额 
  int             iExpDates;                  // 期限 
  char            szExecId[32 + 1];           // 执行编号 
  char            szTrdMatchId[32 + 1];       // 交易编号 
  char            chCashMargin;               // 融资融券信息标识 
  char            szOfferCustName[128 + 1];   // 申报客户姓名 
};

//-------------------------------10303352:债券现券交易竞买预约委托信息查询--------------------------
struct MATRADEAPI CReqStkQryCashBondReserveOrderInfoField
{
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 证券交易所 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iBidTrdDate;                // 竞买交易日期 
  char            szStkCode[8 + 1];           // 证券代码 
  int             iBidExecInstType;           // 竞买成交方式 
  char            chIsWithdraw;               // 撤单标志 
  char            chIsWithdrawn;              // 已撤单标志 
};

struct MATRADEAPI CRspStkQryCashBondReserveOrderInfoField
{
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 证券交易所 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szStkOrderId[14 + 1];       // 交易所合同序号 
  char            szBidId[16 + 1];            // 竞买场次编号 
  int             iIntOrg;                    // 内部机构 
  char            szMemberId[6 + 1];          // 本方交易商代码 
  char            szInvestorType[2 + 1];      // 本方交易主体类型 
  char            szInvestorId[10 + 1];       // 本方交易主体代码 
  char            szTraderCode[8 + 1];        // 本方交易员代码 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iBidTrdDate;                // 竞买交易日期 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llMinQty;                   // 最低成交数量 
  int             iBidExecInstType;           // 竞买成交方式 
  char            szPriceCelling[21 + 1];     // 价格上限 
  char            szPriceFloor[21 + 1];       // 价格下限 
  char            chAnonymity;                // 是否匿名 
  char            chIsWithdraw;               // 撤单标志 
  char            chIsWithdrawn;              // 已撤单标志 
  char            szRemark[320 + 1];          // 备注信息 
  char            chStkSubStatus;             // 处理状态 
};

//-------------------------------10309038:委托汇总查询--------------------------
struct MATRADEAPI CReqStkQryOrderSummaryField
{
  LONGLONG        llCuacctCode;               // 资金账户
  LONGLONG        llOpUser;                   // 操作用户
  char            szStkCode[32 + 1];          // 证券代码
  char            chStkex;                    // 交易市场
  char            szStkbd[2 + 1];             // 交易板块
  int             iStkBiz;                    // 交易行为
  int             iStkBizAction;              // 业务活动
  char            chCurrency;                 // 币种
  int             iBeginDate;                 // 开始日期
  int             iEndDate;                   // 结束日期
};

struct MATRADEAPI CRspStkQryOrderSummaryField
{
  char            szCustName[32 + 1];         // 客户名称
  LONGLONG        llCuacctCode;               // 资金账户
  LONGLONG        llOpUser;                   // 操作用户
  char            szStkCode[32 + 1];          // 证券代码
  char            szStkName[32 + 1];          // 证券名称
  int             iStkBiz;                    // 交易行为
  int             iStkBizAction;              // 业务活动
  char            szStkbd[2 + 1];             // 交易板块
  char            chStkex;                    // 交易市场
  LONGLONG        llOrderQty;                 // 委托数量
  LONGLONG        llMatchedQty;               // 成交数量
  char            szMatchedAmt[21 + 1];       // 成交金额
  char            szMatchPrice[21 + 1];       // 成交均价
  LONGLONG        llWithdrawnQty;             // 撤单数量
  char            szTrdsDate[32 + 1];         // 交易日期
  char            chCurrency;                 // 币种
};

//-------------------------------10309039:成交汇总查询--------------------------
struct MATRADEAPI CReqStkQryMatchSummaryField
{
  LONGLONG        llCuacctCode;               // 资金账户
  LONGLONG        llOpUser;                   // 操作用户
  char            szStkCode[32 + 1];          // 证券代码
  char            chStkex;                    // 交易市场
  char            szStkbd[2 + 1];             // 交易板块
  int             iStkBiz;                    // 交易行为
  int             iStkBizAction;              // 业务活动
  char            chCurrency;                 // 币种
  int             iBeginDate;                 // 开始日期
  int             iEndDate;                   // 结束日期
};

struct MATRADEAPI CRspStkQryMatchSummaryField
{
  char            szCustName[32 + 1];         // 客户名称
  LONGLONG        llCuacctCode;               // 资金账户
  LONGLONG        llOpUser;                   // 操作用户
  char            szStkCode[32 + 1];          // 证券代码
  char            szStkName[32 + 1];          // 证券名称
  int             iStkBiz;                    // 交易行为
  int             iStkBizAction;              // 业务活动
  char            szStkbd[2 + 1];             // 交易板块
  char            chStkex;                    // 交易市场
  LONGLONG        llMatchedQty;               // 成交数量
  char            szMatchedAmt[21 + 1];       // 成交金额
  char            szMatchPrice[21 + 1];       // 成交均价
  char            szTrdsDate[32 + 1];         // 交易日期
  char            chCurrency;                 // 币种
};

//-------------------------------10303353:债券现券交易竞买预约信息查询--------------------------
struct MATRADEAPI CReqStkQryCashBondTradeReservationInfoField
{
  char            szStkCode[8 + 1];           // 证券代码 
  char            szBidId[16 + 1];            // 竞买场次编号 
  int             iBidTrdDate;                // 竞买交易日期 
  int             iBidExecInstType;           // 竞买成交方式 
  char            chAnonymity;                // 是否匿名 
  char            szMemberId[6 + 1];          // 交易商代码 
  char            szInvestorType[2 + 1];      // 交易主体类型 
  int             iSettType;                  // 结算方式 
};

struct MATRADEAPI CRspStkQryCashBondTradeReservationInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szBidId[16 + 1];            // 竞买场次编号 
  char            szStkCode[8 + 1];           // 证券代码 
  int             iBidTrdDate;                // 预约竞买日期 
  LONGLONG        llBidQty;                   // 竞买预约数量 
  LONGLONG        llBidMinQty;                // 最低成交数量 
  int             iBidExecInstType;           // 竞买成交方式 
  char            chAnonymity;                // 是否匿名 
  char            szMemberId[6 + 1];          // 交易商代码 
  char            szInvestorType[2 + 1];      // 交易主体类型 
  char            szPriceCelling[21 + 1];     // 价格上限 
  char            szPriceFloor[21 + 1];       // 价格下限 
  int             iSettType;                  // 结算方式 
  int             iSettPeriod;                // 结算周期 
  char            szRemark[320 + 1];          // 备注信息 
};

//-------------------------------10301389:债券现券交易业务参考信息查询--------------------------
struct MATRADEAPI CReqStkQryCashBondTradeProfitInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQueryPos[32 + 1];         // 定位串 输入值为记录号 与查询方向配合使用
  int             iQueryNum;                  // 查询行数 最大值为1000
};

struct MATRADEAPI CRspStkQryCashBondTradeProfitInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chTraType;                  // 交易方式 
  char            szPriceTick[21 + 1];        // 价格档位 
  char            szPriceUpperLimit[21 + 1];  // 涨停价 
  char            szPriceLowerLimit[21 + 1];  // 跌停价 
  LONGLONG        llBuyQtyUpperLimit;         // 买数量上限 
  LONGLONG        llSellQtyUpperLimit;        // 卖数量上限 
  LONGLONG        llBuyQtyLowerLimit;         // 买数量下限 
  LONGLONG        llSellQtyLowerLimit;        // 卖数量下限 
  LONGLONG        llBuyQtyUnit;               // 买数量单位 
  LONGLONG        llSellQtyUnit;              // 卖数量单位 
  char            szQryPos[32 + 1];           // 定位串 
};

//-------------------------------10301391:债券通用质押式回购业务参考信息查询--------------------------
struct MATRADEAPI CReqStkQryBondPledgeRepchProfitInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQueryPos[32 + 1];         // 定位串 输入值为记录号 与查询方向配合使用
  int             iQueryNum;                  // 查询行数 最大值为1000
};
struct MATRADEAPI CRspStkQryBondPledgeRepchProfitInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chTraType;                  // 交易方式 
  char            szPriceTick[21 + 1];        // 价格档位 
  char            szPriceUpperLimit[21 + 1];  // 涨停价 
  char            szPriceLowerLimit[21 + 1];  // 跌停价 
  LONGLONG        llBuyQtyUpperLimit;         // 买数量上限 
  LONGLONG        llSellQtyUpperLimit;        // 卖数量上限 
  LONGLONG        llBuyQtyLowerLimit;         // 买数量下限 
  LONGLONG        llSellQtyLowerLimit;        // 卖数量下限 
  LONGLONG        llBuyQtyUnit;               // 买数量单位 
  LONGLONG        llSellQtyUnit;              // 卖数量单位 
  char            szQryPos[32 + 1];           // 定位串 
};

//////////////////////////////////////////////////////////////////////////
//-------------------------------10302102:买卖委托(极速)-------------------------
struct MATRADEAPI CReqStkOrderFlashField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  int             iOrderBsn;                  // 委托批号
  int             iCuacctSn;                  // 账户序号
  char            szStkpbu[8 + 1];            // 交易单元(未启用) 
  char            szClientInfo[256 + 1];      // 终端信息(未启用)
};

struct MATRADEAPI CRspStkOrderFlashField
{
  char            szOrderId[10 + 1];          // 合同序号
  int             iOrderBsn;                  // 委托批号
  int             iCuacctSn;                  // 账户序号
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
};

//-------------------------------10302104:委托撤单(极速)------------------------------------
struct MATRADEAPI CReqStkCancelOrderFlashField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szOrderId[10 + 1];            // 合同序号
  int           iStkBiz;                      // 证券业务
  int           iOrderBsn;                    // 委托批号(未启用)
  char          szClientInfo[256 + 1];        // 终端信息(未启用)
  LONGLONG      llCustCode;                   // 客户代码(未启用)
  LONGLONG      llFortuneCode;                // 财富账户(未启用)
};

struct MATRADEAPI CRspStkCancelOrderFlashField
{
  char          szOrderId[10 + 1];            // 合同序号 原委托合同序号
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  int           iStkBiz;                      // 证券业务
  char          szWtOrderId[10 + 1];          // 委托合同号 撤单委托合同序号
  int           iOrderBsn;                    // 委托批号(未启用)
  char          szOrderPrice[21 + 1];         // 委托价格(未启用)
  LONGLONG      llOrderQty;                   // 委托数量(未启用)
  char          szOrderAmt[21 + 1];           // 委托金额(未启用)
  char          szOrderFrzAmt[21 + 1];        // 冻结金额(未启用)
  char          szStkpbu[8 + 1];              // 交易单元(未启用)
  char          szStkCode[8 + 1];             // 证券代码(未启用)
  char          szStkName[16 + 1];            // 证券名称(未启用)
  int           iStkBizAction;                // 业务行为(未启用)
  char          chCancelStatus;               // 内部撤单标志(未启用)
  char          szTrdacct[20 + 1];            // 证券账户(未启用)
  char          szMsgOk[32 + 1];              // 内撤信息(未启用)
  char          szCancelList[256 + 1];        // 撤单列表(未启用)
  LONGLONG      llFortuneCode;                // 财富账户(未启用)
};

//-------------------------------10302103:组合委托(极速)------------------------------------
struct MATRADEAPI CReqStkCombOrderFlashField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 业务行为
  int           iOrderBsn;                    // 委托批号
  int           iCuacctSn;                    // 账户序号
  char          chErrorFlag;                  // 错误屏弊标志
  char          szStrategyText[32768 + 1];    // 策略信息
  LONGLONG      llCustCode;                   // 客户代码(未启用)
  char          szClientInfo[256 + 1];        // 终端信息(未启用)
  char          szTrdTermcode[20 + 1];        // 终端特征码(未启用)
  char          chSecurityLevel;              // 安全手段(未启用)
  char          szSecurityInfo[256 + 1];      // 安全信息(未启用)
  char          chPublishCtrlFlag;            // 股票风控标志(未启用)
};

struct MATRADEAPI CRspStkCombOrderFlashField
{
  int           iOrderBsn;                    // 委托批号
  char          szOrderId[10 + 1];            // 合同序号
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szStkCode[8 + 1];             // 证券代码
  int           iStkBiz;                      // 证券业务
  int           iCuacctSn;                    // 账户序号
  int           iOrderRet;                    // 委托结果
  char          szRetInfo[256 + 1];           // 返回信息
  char          szOrderPrice[21 + 1];         // 委托价格(未启用)
  LONGLONG      llOrderQty;                   // 委托数量(未启用)
  char          szOrderAmt[21 + 1];           // 委托金额(未启用)
  char          szOrderFrzAmt[21 + 1];        // 冻结金额(未启用)
  char          szStkpbu[8 + 1];              // 交易单元(未启用)
  char          szStkName[16 + 1];            // 证券名称(未启用)
  int           iStkBizAction;                // 业务行为(未启用)
  char          szTrdacct[20 + 1];            // 证券账户(未启用)
};

//-------------------------------00102023:成交回报推送------------------------------------
struct MATRADEAPI CRtnStkOrderFillField
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
struct MATRADEAPI CRtnStkPefEtfFillField
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
struct MATRADEAPI CRtnStkOrderConfirmField
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

//-------------------------------00102030:成交回报推送(极速)------------------------------------
struct MATRADEAPI CRtnStkOrderFillFlashField
{
  LONGLONG      llCuacctCode;               // 资产账户
  LONGLONG      llMatchedQty;               // 本次成交数量
  int           iOrderBsn;                  // 委托批号
  char          szMatchedSn[16 + 1];        // 成交编号
  char          szMatchedPrice[11 + 1];     // 本次成交价格
  char          szOrderId[10 + 1];          // 合同序号
  char          szStkCode[8 + 1];           // 证券代码
  char          szStkbd[2 + 1];             // 交易板块
  char          chIsWithdraw;               // 撤单标志
  char          chMatchedType;              // 成交类型
  char          chOrderStatus;              // 委托状态
};

//-------------------------------00102031:确认回报推送(极速)------------------------------------
struct MATRADEAPI CRtnStkOrderConfirmFlashField
{
  char          szStkCode[8 + 1];           // 证券代码
  char          szOrderId[10 + 1];          // 合同序号
  char          chIsWithdraw;               // 撤单标志
  LONGLONG      llCuacctCode;               // 资产账户
  int           iOrderBsn;                  // 委托批号
  char          szStkbd[2 + 1];             // 交易板块
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_STK_TRADE_API_STRUCT_H__