//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maCliOptTradeApiStruct.h
// 模块名称：股票期权交易API C++ Class
// 模块描述：
// 开发作者：何万刚
// 创建日期：2012-12-23
// 模块版本：001.000.000
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2012-12-23    1.0          何万刚          初创
//----------------------------------------------------------------------------
#if !defined(__MA_CLI_OPT_TRADE_API_STRUCT_H__)
#define __MA_CLI_OPT_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10314001:用户登录(内部使用)------------------------------------
struct MATRADEAPI CReqOptUserLoginField
{
  char          szAcctType[2 + 1];          // 账户类型 'U':客户代码 'Z':资产账户
  char          szAcctId[32 + 1];           // 账户标识
  char          chUseScope;                 // 使用范围 '4':期权交易密码
  char          chAuthType;                 // 认证类型 '0':密码
  char          szAuthData[256 + 1];        // 认证数据(明文)
  char          chEncryptType;              // 加密方式 ‘0’:金证KBSS系统加密 ‘1’:金证Win版集中交易加密 ‘2’:金证Unix版集中交易加密 ‘3’:外部重加密 ‘4’ : 外部加密
  char          szEncryptKey[32 + 1];       // 加密因子
  char          chChannel;                  // 操作渠道
  char          szSerChannel[8 + 1];        // 服务渠道
};

struct MATRADEAPI CRspOptUserLoginField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          chStkex;                    // 交易市场
  char          szStkbd[2 + 1];             // 交易板块
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8+1];         // 证券账户子编码
  char          szOptTrdacct[28+1];         // 期权合约账户
  int           iIntOrg;                    // 内部机构
  char          chTrdacctStatus;            // 账户状态
  char          szStkpbu[8 + 1];            // 交易单元
  char          szAcctType[2 + 1];          // 账户类型 'U':客户代码 'Z':资产账户
  char          szAcctId[32 + 1];           // 账户标识
  char          szTrdacctName[32 + 1];      // 账户名称
  char          szSession[256 + 1];         // 会话凭证
  int           iFlashPort;                 // 软加速端口号
  char          szFlashStkBizes[64 + 1];    // 软加速业务
  char          chSzFlashMode;              // 软加速深圳市场加速模式 ‘0’-不加速 ‘1’-软件加速 ‘2’-FPGA加速
  char          chShFlashMode;              // 软加速上海市场加速模式 ‘0’-不加速 ‘1’-软件加速 ‘2’-FPGA加速
  char          chFlashEnableFlag;          // 加速启用标志 ‘0’-未启用 ‘1’-已启用
};

//-------------------------------10311001:期权基础信息查询------------------------------------
struct MATRADEAPI CReqOptBaseInfoField
{
  char          szStkbd[2 + 1];             // 交易板块
  char          szOptNum[32 + 1];           // 合约编码
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          chOptType;                  // 合约类型
  char          chIsRemind;                 // 附加提醒标志 1-返回提醒信息标签；非1-不返回（用于委托时进行提醒）
  char          chQueryFlag;                // 查询方向
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptBaseInfoField
{
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 账户标识 字典[STKBD]
  char          szOptNum[16 + 1];           // 合约编码
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          chOptType;                  // 合约类型
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  char          chOptUndlCls;               // 标的证券类别 字典[STK_CLS]
  char          chOptExeType;               // 履行方式
  LONGLONG      llOptUnit;                  // 合约单位
  char          szExepcisePrice[11 + 1];    // 行权价
  int           iStartDate;                 // 首个交易日
  int           iEndDate;                   // 最后交易日
  int           iExeRciseDate;              // 期权行权日
  int           iExpireDate;                // 期权到期日
  char          chUpdVersion;               // 合约版本号
  LONGLONG      llLeavesQty;                // 未平仓合约数
  char          szPreClosePx[11 + 1];       // 昨日收盘价
  char          szPreSettPrice[11 + 1];     // 昨日结算价
  char          szUndlClsPrice[11 + 1];     // 标的券前收盘
  char          chPriceLmtType;             // 涨跌幅限制类型
  char          szOptUplmtPrice[11 + 1];    // 涨停价格
  char          szOptLwlmtPrice[11 + 1];    // 跌停价格
  char          szMarginUnit[21 + 1];       // 单位保证金
  char          szMarginRatio1[11 + 1];     // 保证金计算比例参数一
  char          szMarginRatio2[11 + 1];     // 保证金计算比例参数二
  LONGLONG      llOptLotSize;               // 整手数
  LONGLONG      llOptLbuplmtQty;            // 限价买单笔申报上限
  LONGLONG      llOptLsuplmtQty;            // 限价卖单笔申报上限
  LONGLONG      llOptLlwlmtQty;             // 限价单笔申报下限
  LONGLONG      llOptMbuplmtQty;            // 市价买单笔申报上限
  LONGLONG      llOptMsuplmtQty;            // 市价卖单笔申报上限
  LONGLONG      llOptMlwlmtQty;             // 市价单笔申报下限
  char          chOpenFlag;                 // 开仓标志
  char          chSuspendedFlag;            // 停牌标志 0:正常 1:临时停牌 2:长期停牌
  char          chExpireFlag;               // 到期日标志
  char          chAdjustFlag;               // 调整标志
  char          chOptStatus;                // 合约状态
  char          chCombFlag;                 // 合约组合标志位 2015/8/3添加
  int           iAutoSplitDate;             // 组合策略到期解除日 2015/8/3添加
  int           iUpdDate;                   // 更新日期
  int           iDeliveryDate;              // 行权交割日
  char          szQryPos[32 + 1];           // 定位串
  char          szRemindFlags[16 + 1];      // 提醒信息标签 根据入参IS_REMIND进行返回，若IS_REMIND送’1’，该出参返回。格式为’0,1,2,3’， ’0’表示严重价外期权开仓委托时提醒 ’1’表示临近到期期权开仓委托时提醒 ’2’表示合约调整期权开仓委托时提醒 ‘3’表示虚值合约提醒
  char          szBasicStkName[80 + 1];     // 长证券简称 
};

//-------------------------------10312001:个股期权委托申报------------------------------------
struct MATRADEAPI CReqOptOrderField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOrderPrice[21 + 1];       // 委托价格 单位：元，精确到0.1厘
  LONGLONG        llOrderQty;                 // 委托数量 普通询价送0
  short           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  short           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]，认购行权、认沽行权、普通询价默认送100
  char            szStkpbu[8 + 1];            // 交易单元 
  int             iOrderBsn;                  // 委托批号 
  char            chEnforceFlag;              // 强制平仓标志
  char            chOrderType;                // 委托类型
  char            szClientInfo[256 + 1];      // 终端信息 1，通过mid接入时，值必须和固定入参F_OP_SITE(操作站点)的值保持一致； 2，通过其它方式接入时，值可以为空，如果送值，必须和固定入参F_OP_SITE(操作站点)的值保持一致
  LONGLONG        llCustCode;                 // 客户代码 
  int             iIntOrg;                    // 内部机构 
  char            chSecurityLevel;            // 安全手段 0:无安全 1:校验票据 2:校验密码
  char            szSecurityInfo[256 + 1];    // 安全信息 安全手段为校验票据则需填写票据 安全手段为密码则需填写交易密码密文
  char            szOrderIdEx[64 + 1];        // 外部合同序号 此序号为外部传入，期权系统不做任何处理
  char            chEncryptType;              // 加密方式 当安全手段为2：校验密码时，为必填入参。 加密方式: ‘0’:金证KBSS系统加密方式 ‘1’:金证Win版集中交易加密方式 ‘2’:金证Unix版集中交易加密方式 ‘3’:外部重加密方式 ‘4’:外部加密方式 说明：该入参根据接入券商要求填写，默认值为0。 外部重加密方式’3’：使用非金证加密函数将密码明文加密为密文后，然后使用金证公司提供的通信加密函数kbss_comencrypt对密文再一次加密。 外部加密方式’4’：使用非金证加密函数将密码明文加密为密文后作为入参。
  char            szEncryptKey[32 + 1];       // 加密因子 当安全手段为2：校验密码时，为必填入参。 加密因子可以填写为资产账户：CUACCT_CODE参数值
  char            szCombNum[16 + 1];          // 组合编码 组合策略单边平仓时送值，其它情形送空值
};

struct MATRADEAPI CRspOptOrderField
{
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[18 + 1];       // 期权合约账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptCode[40 + 1];          // 合约代码 
  char            szOptName[120 + 1];         // 合约简称 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOptUndlName[80 + 1];      // 标的证券名称 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 
  char            szOrderIdEx[64 + 1];        // 外部合同序号 
};

//-------------------------------10310502:股票期权委托撤单------------------------------------
struct MATRADEAPI CReqOptCancelOrderField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  char            chForceWth;                 // 强撤标志
  char            szRiskId[2 + 1];            // 风险标识
  char            szClientInfo[256 + 1];      // 客户端信息 1，通过mid接入时，值必须和固定入参F_OP_SITE(操作站点)的值保持一致； 2，通过其它方式接入时，值可以为空，如果送值，必须和固定入参F_OP_SITE(操作站点)的值保持一致
  short           siStkBiz;                   // 证券业务
  char            szOrderIdEx[64 + 1];        // 外部合同序号 此序号为外部传入，期权系统不做任何处理
  int             iIntOrg;                    // 内部机构 
};

struct MATRADEAPI CRspOptCancelOrderField
{
  char            szStkbd[2 + 1];             // 交易板块 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szOrderIdWtd[10 + 1];       // 撤单合同序号 
  int             iOrderBsn;                  // 委托批号 
  char            chCancelStatus;             // 内部撤单标志 1:内部撤单 非1:普通撤单
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[18 + 1];       // 期权合约账户 
  char            szStkCode[32 + 1];          // 证券代码 
  char            szStkName[80 + 1];          // 证券名称 
  char            szOrderIdEx[64 + 1];        // 外部合同序号 
};

//-------------------------------10312008:个股期权标的证券锁定解锁------------------------------------
struct MATRADEAPI CReqOptUndlStkLockField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iIntOrg;                    // 内部机构
  char          szStkbd[2 + 1];             // 交易板块
  char          szTrdacct[20 + 1];          // 证券账户
  char          szStkCode[8 + 1];           // 证券代码
  LONGLONG      llOrderQty;                 // 委托数量
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szStkpbu[8 + 1];            // 交易单元
  int           iOrderBsn;                  // 委托批号
  char          szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
  char          chSecurityLevel;            // 安全手段 0:无安全 1:校验票据 2:校验密码
  char          szSecurityInfo[256 + 1];    // 安全手段为校验票据则需填写票据,安全手段为密码则需填写交易密码密文
  char          szOrderIdEx[64 + 1];        // 此序号为外部传入，期权系统不做任何处理
  char          chEncryptType;              // 加密方式 2015/8/3添加
  char          szEncryptKey[32+1];         // 加密因子 2015/8/3添加
};

struct MATRADEAPI CRspOptUndlStkLockField
{
  int           iOrderBsn;                  // 委托批号
  char          szOrderId[10 + 1];          // 合同序号
  LONGLONG      llCuacctCode;               // 资产账户
  LONGLONG      llOrderQty;                 // 委托数量
  char          szStkpbu[8 + 1];            // 交易单元
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18+1];         // 期权合约账户 2015/8/3 
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szOrderIdEx[64 + 1];        // 外部合同序号
};

//-------------------------------10312006:计算个股期权最大可交易数量------------------------------------
struct MATRADEAPI CReqOptMaxTradeQtyField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szOptNum[16 + 1];           // 合约编码 若为开平仓、行权，合约编码不能为空；若为证券锁定解锁，证券代码不能为空；若为构建与拆分组合均不送；若为行权指令合并申报均不送
  char            szStkCode[8 + 1];           // 证券代码 若为开平仓、行权，合约编码不能为空；若为证券锁定解锁，证券代码不能为空；若为构建与拆分组合均不送；若为行权指令合并申报均不送
  char            szOrderPrice[21 + 1];       // 委托价格 开平仓必送，行权、证券锁定解锁、构建与拆分组合、行权指令合并申报非必送
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]，其中行权、证券锁定解锁、构建与拆分组合、行权指令合并申报送100
  char            szCombNum[16 + 1];          // 组合编码 构建组合策略时不需送入，组合策略单边平仓时送值
  char            szCombStraCode[16 + 1];     // 组合策略代码 构建与拆分组合必送，其他不送
  char            szLeg1Num[16 + 1];          // 成分一合约编码 1.认购普通义务仓转备兑仓成分一合约编码必送； 2.构建组合时，除认购普通义务仓转备兑仓之外，其他组合策略有几条腿就必须送入几个成分合约编码，并需按序送入； 3.解除组合时，若送入组合编码COMB_NUM，则成分合约不需送入。 4.行权指令合并申报成分一合约编码和成分二合约编码必送
  char            szLeg2Num[16 + 1];          // 成分二合约编码 1.认购普通义务仓转备兑仓成分一合约编码必送； 2.构建组合时，除认购普通义务仓转备兑仓之外，其他组合策略有几条腿就必须送入几个成分合约编码，并需按序送入； 3.解除组合时，若送入组合编码COMB_NUM，则成分合约不需送入。 4.行权指令合并申报成分一合约编码和成分二合约编码必送
  char            szLeg3Num[16 + 1];          // 成分三合约编码 1.认购普通义务仓转备兑仓成分一合约编码必送； 2.构建组合时，除认购普通义务仓转备兑仓之外，其他组合策略有几条腿就必须送入几个成分合约编码，并需按序送入； 3.解除组合时，若送入组合编码COMB_NUM，则成分合约不需送入。 4.行权指令合并申报成分一合约编码和成分二合约编码必送
  char            szLeg4Num[16 + 1];          // 成分四合约编码 1.认购普通义务仓转备兑仓成分一合约编码必送； 2.构建组合时，除认购普通义务仓转备兑仓之外，其他组合策略有几条腿就必须送入几个成分合约编码，并需按序送入； 3.解除组合时，若送入组合编码COMB_NUM，则成分合约不需送入。 4.行权指令合并申报成分一合约编码和成分二合约编码必送
};

struct MATRADEAPI CRspOptMaxTradeQtyField
{
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubAcctcode[8 + 1];       // 证券账户子编码
  char            szOptTrdacct[18 + 1];       // 期权合约账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szCombNum[16 + 1];          // 组合编码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  char            szLeg3Num[16 + 1];          // 成分三合约编码 
  char            szLeg4Num[16 + 1];          // 成分四合约编码 
  LONGLONG        llOrderQty;                 // 委托数量 最大可委托数量
  LONGLONG        llNoLmtOrderQty;            // 最大委托数量 去掉单手限制的最大可委托数量
};

//-------------------------------10313019:可用资金查询--------------------------
struct MATRADEAPI CReqOptExpendableFundField
{
  LONGLONG        llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iValueFlag;                 // 取值标志 字段用于标识，可复合取值 MARKET_VALUE、FUND_VALUE、 STK_VALUE、FUND_LOAN 四个字段是否取值 0：均不计算 1：取MARKET_VALUE值 2：取FUND_VALUE值 4：取STK_VALUE值 8：取FUND_LOAN值 可复合取值
};

struct MATRADEAPI CRspOptExpendableFundField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  int             iIntOrg;                    // 内部机构 
  char            szMarketValue[21 + 1];      // 资产总值 客户资产总额（实时）见文档
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
  char            chFundStatus;               // 资金状态 字典[FUND_STATUS]
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

//-------------------------------10313001:可用合约资产查询------------------------------------
struct MATRADEAPI CReqOptExpendableCuField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOptNum[16 + 1];           // 合约编码
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szStkpbu[8 + 1];            // 交易单元
  char          chOptSide;                  // 持仓方向 L-权力仓，S-义务仓，C-备兑策略持仓
  char          chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptExpendableCuField
{
  char            szQryPos[32 + 1];           // 定位串
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[18 + 1];       // 期权合约账户 
  char            chCurrency;                 // 货币代码 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptCode[40 + 1];          // 合约代码 
  char            szOptName[120 + 1];         // 合约简称 
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

//-------------------------------10313002:可用备兑股份查询------------------------------------
struct MATRADEAPI CReqOptExpendableCvdStkField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOptCode[8 + 1];           // 证券代码
  char          szStkpbu[8 + 1];            // 交易单元
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptExpendableCvdStkField
{
  char          szQryPos[32 + 1];           // 定位串
  int           iIntOrg;                    // 内部机构
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkpbu[8 + 1];            // 交易单元
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18+1];         // 期权合约账户 2015/8/5
  char          chCurrency;                 // 货币代码 字典[CURRENCY]
  char          szStkCode[8 + 1];           // 证券代码
  char          szStkName[80 + 1];          // 证券名称
  char          chStkCls;                   // 证券类别 字典[STK_CLS]
  LONGLONG      llCvdStkPrebln;             // 备兑股份昨日余额
  LONGLONG      llCvdStkBln;                // 备兑股份余额
  LONGLONG      llCvdStkAvl;                // 备兑股份可用
};

//-------------------------------10313003:个股期权当日委托查询------------------------------------
struct MATRADEAPI CReqOptCurrDayOrderField
{
  LONGLONG      llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOptNum[32 + 1];           // 合约编码
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szCombStraCode[16 + 1];     // 组合策略代码  2015/8/5
  char          szOrderId[10 + 1];          // 合同序号
  int           iOrderBsn;                  // 委托批号
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          chOrderStatus;              // 委托状态
  char          szOwnerType[3 + 1];         // 订单所有类型  
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptCurrDayOrderField
{
  char          szQryPos[32 + 1];           // 定位串
  int           iTrdDate;                   // 交易日期
  int           iOrderDate;                 // 委托日期
  char          szOrderTime[25 + 1];        // 委托时间
  int           iOrderBsn;                  // 委托批号
  char          szOrderId[10 + 1];          // 合同序号
  char          chOrderStatus;              // 委托状态
  char          chOrderValidFlag;           // 委托有效标志
  int           iIntOrg;                    // 内部机构
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkpbu[8 + 1];            // 交易单元
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18 + 1];       // 期权合约账户 2015/8/5
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // 订单所有类型
  char          szOptNum[16 + 1];           // 合约编码
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          szCombNum[16 + 1];          // 组合编码            2015/8/5       
  char          szCombStraCode[16 + 1];     // 组合策略代码        2015/8/5
  char          szLeg1Num[16 + 1];          // 成分一合约编码      2015/8/5
  char          szLeg2Num[16 + 1];          // 成分二合约编码      2015/8/5
  char          szLeg3Num[16 + 1];          // 成分三合约编码      2015/8/5
  char          szLeg4Num[16 + 1];          // 成分四合约编码      2015/8/5
  char          chCurrency;                 // 货币代码 字典[CURRENCY]
  char          szOrderPrice[11 + 1];       // 委托价格 CPRICE4
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
  char          szOptUndlName[80 + 1];      // 标的证券名称
  LONGLONG      llUndlFrzQty;               // 标的券委托冻结数量
  LONGLONG      llUndlUfzQty;               // 标的券委托解冻数量
  LONGLONG      llUndlWthQty;               // 标的券已撤单数量
  char          szOfferRetMsg[64 + 1];      // 申报返回信息
  char          szOrderIdEx[64 + 1];        // 外部合同序号
  int           iOrderSn;                   // 委托序号
  char          szRawOrderId[10 + 1];       // 外部合同序号
  char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
  char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
  char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
  char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
};

//-------------------------------10313004:个股期权当日成交查询------------------------------------
struct MATRADEAPI CReqOptCurrDayFillField
{
  LONGLONG      llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOptNum[32 + 1];           // 合约编码
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szCombStraCode[16 + 1];     // 组合策略代码    2015/8/5
  char          szOrderId[10 + 1];          // 合同序号
  int           iOrderBsn;                  // 委托批号
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // 订单所有类型    2015/8/5
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptCurrDayFillField
{
  char          szQryPos[32 + 1];           // 定位串
  int           iTrdDate;                   // 交易日期
  char          szMatchedTime[8 + 1];       // 成交时间
  int           iOrderDate;                 // 委托日期
  int           iOrderSn;                   // 委托序号
  int           iOrderBsn;                  // 委托批号
  char          szOrderId[10 + 1];          // 合同序号
  int           iIntOrg;                    // 内部机构
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkpbu[8 + 1];            // 交易单元
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18+1];         // 期权合约账户          2015/8/5
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // 订单所有类型
  char          szOptNum[16 + 1];           // 合约编码
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          szCombNum[16+1];            // 组合编码              2015/8/5
  char          szCombStraCode[16+1];       // 组合策略代码          2015/8/5
  char          szLeg1Num[16+1];            // 成分一合约编码        2015/8/5
  char          szLeg2Num[16+1];            // 成分二合约编码        2015/8/5
  char          szLeg3Num[16+1];            // 成分三合约编码        2015/8/5
  char          szLeg4Num[16+1];            // 成分四合约编码        2015/8/5
  char          chCurrency;                 // 货币代码 字典[CURRENCY]
  char          chOptUndlCls;               // 标的证券类别
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  char          szOrderPrice[11 + 1];       // 委托价格 CPRICE4
  LONGLONG      llOrderQty;                 // 委托数量
  char          szOrderAmt[21 + 1];         // 委托金额
  char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
  char          chIsWithdraw;               // 撤单标志
  char          chMatchedType;              // 成交类型
  char          szMatchedSn[32 + 1];        // 成交编号
  char          szMatchedPrice[11 + 1];     // 成交价格 CPRICE4
  LONGLONG      llMatchedQty;               // 已成交数量
  char          szMatchedAmt[21 + 1];       // 已成交金额
  char          szOrderIdEx[10 + 1];        // 外部合同序号
  char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
  char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
  char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
  char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
  char          szMatchedFee[21 + 1];       // 成交费用        2015/8/5    
};

//-------------------------------10313005:个股期权合约账户查询------------------------------------
struct MATRADEAPI CReqOptAcctField
{
  LONGLONG        llCustCode;                 // 客户代码 不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptAcctField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[18 + 1];       // 期权合约账户 
  char            chOptTrdacctLvl;            // 期权账户级别 
  int             iIntOrg;                    // 内部机构 
  int             iTrdacctSn;                 // 账户序号 
  char            szTrdacctExid[20 + 1];      // 报盘账户 
  char            chTrdacctStatus;            // 账户状态 
  char            chTregStatus;               // 指定状态 字典[TREG_STATUS]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szIdType[2 + 1];            // 证件类型 
  char            szIdCode[48 + 1];           // 证件号码 
  char            szCustName[32 + 1];         // 客户名称 
  //int             iOptMarginSn;               // 保证金比例模板编号 
  char            szQuotaVal[21 + 1];         // 买入额度 
  char            szQuotaValUsed[21 + 1];     // 占用买入额度 
  char            chOpFlag;                   // 操作标志 
  LONGLONG        llDailyEnqryCount;          // 当日询价次数 
};

//-------------------------------10313006:个股期权可撤委托查询------------------------------------
struct MATRADEAPI CReqOptCanWithdrawOrderField
{
  LONGLONG      llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG      llCuacctCode;               // 资产账户
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOptNum[32 + 1];           // 合约编码
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOrderId[10 + 1];          // 合同序号
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char          szQryPos[32 + 1];           // 定位串
  int           iQryNum;                    // 查询行数
};

struct MATRADEAPI CRspOptCanWithdrawOrderField
{
  char          szQryPos[32 + 1];           // 定位串
  int           iTrdDate;                   // 交易日期
  int           iOrderDate;                 // 委托日期
  char          szOrderTime[25 + 1];        // 委托时间
  int           iOrderBsn;                  // 委托批号
  char          szOrderId[10 + 1];          // 合同序号
  char          chOrderStatus;              // 委托状态
  char          chOrderValidFlag;           // 委托有效标志
  int           iIntOrg;                    // 内部机构
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkpbu[8 + 1];            // 交易单元
  char          szTrdacct[20 + 1];          // 证券账户
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18 + 1];       // 期权合约账户           2015/8/5
  int           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char          szOwnerType[3 + 1];         // 订单所有类型
  char          szOptNum[16 + 1];           // 合约编码
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          chCurrency;                 // 货币代码 字典[CURRENCY]
  char          szOrderPrice[11 + 1];       // 委托价格 CPRICE4
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
  char          szOptUndlName[80 + 1];      // 标的证券名称
  LONGLONG      llUndlFrzQty;               // 标的券委托冻结数量
  LONGLONG      llUndlUfzQty;               // 标的券委托解冻数量
  LONGLONG      llUndlWthQty;               // 标的券已撤单数量
  char          szMarginPreFrz[21 + 1];     // 预占用保证金 卖开委托时填写预冻结的保证金金额，其他情况填“0”
  char          szMarginFrz[21 + 1];        // 占用保证金 卖开成交时填写实际冻结的保证金金额，其他情况填“0”
  char          szMarginPreUfz[21 + 1];     // 预解冻保证金 买平委托时填写预解冻的保证金金额，其他情况填“0”
  char          szMarginUfz[21 + 1];        // 解冻保证金 买平成交时填写实际解冻的保证金金额，其他情况填“0”
};

//-------------------------------10102509:客户适当性匹配产品信息查询------------------------------------
struct MATRADEAPI CReqOptQryCustAppropriateMatchInfoField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
};

struct MATRADEAPI CRspOptQryCustAppropriateMatchInfoField
{
  LONGLONG      llCustCode;                 // 客户代码
  char          szStkbd[2 + 1];             // 交易板块
  char          szTrdacct[20 + 1];          // 证券账户
  char          szStkBizes[512 + 1];        // 证券业务(复数)
  char          szStkBizActions[512 + 1];   // 证券业务行为(复数)
};

//-------------------------------10102510:客户适当性信息查询------------------------------------
struct MATRADEAPI CReqOptQryCustAppropriateInfoField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
};

struct MATRADEAPI CRspOptQryCustAppropriateInfoField
{
  LONGLONG      llCustCode;                 // 客户代码
  char          chInvestorType;             // 投资者类型
  int           iProfInvestorExpDate;       // 专业投资者失效日期
  char          chLowestRiskFlag;           // 最低风险等级客户标志
  char          chRatingLvl;                // 测评等级
  int           iRatingDate;                // 测评日期
  int           iRatingExpDate;             // 测评失效日期
  char          szInvestPro[32 + 1];        // 投资品种
  char          chInvestLmt;                // 投资期限
  char          chExpectIncome;             // 预期收益
  char          chIsSign;                   // 是否签署风险警示书
};

//-------------------------------10102511:客户风险警示书签署------------------------------------
struct MATRADEAPI CReqOptCustRiskSignField
{
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  char          chIsSign;                   // 是否签署风险警示书
  char          szRemark[128 + 1];          // 备注
};

struct MATRADEAPI CRspOptCustRiskSignField
{
};

//-------------------------------00102023:成交回报推送------------------------------------
// 成交信息
struct MATRADEAPI CRtnOptOrderFillField
{
  char          szMatchedSn[32 + 1];        // 成交编号
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkCode[8 + 1];           // 证券代码
  char          szOrderId[10 + 1];          // 合同序号
  char          szTrdacct[20 + 1];          // 交易账户
  LONGLONG      llMatchedQty;               // 本次成交数量 撤单成交时数为负数
  char          szMatchedPrice[11 + 1];     // 本次成交价格
  char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
  char          szRltSettAmt[21 + 1];       // 实时清算金额
  char          szFundAvl[21 + 1];          // 资金可用金额（成交后）
  LONGLONG      llStkAvl;                   // 证券可用数量（成交后）
  //char          szOpptStkpbu[8 + 1];        // 对方席位
  //char          szOpptTrdacct[20 + 1];      // 对方交易账号
  int           iMatchedDate;               // 成交日期
  char          szMatchedTime[8 + 1];       // 成交时间
  char          chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iOrderBsn;                  // 委托批号
  int           iCuacctSn;                  // 账户序号
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          chMatchedType;              // 成交类型 ‘0’:内部撤单成交 ‘1’:非法委托撤单成交 ‘2’:交易所成交（含主动撤单成交与撮合委托成交）
  char          chOrderStatus;              // 委托状态 字典[ORDER_STATUS] 本次成交后委托的状态
  int           iStkBiz;                    // 证券业务
  int           iStkBizAction;              // 业务行为
  char          szOfferRetMsg[64 + 1];      // 申报信息 交易所废单时返回废单原因
  LONGLONG      llOrderQty;                 // 委托数量
  LONGLONG      llWithdrawnQty;             // 已撤单数量
  LONGLONG      llTotalMatchedQty;          // 累计成交数量
  //char          chCuacctAttr;               // 账户属性
  char          szTotalMatchedAmt[21 + 1];  // 累计成交金额
  //LONGLONG      llStkQty;                   // 拥股数
  char          szMatchedAmt[21 + 1];       // 已成交金额
  //char          szHFundAvl[21 + 1];         // 港股资金可用
  int           iOrderDate;                 // 委托日期
  int           iOrderSn;                   // 委托序号
  int           iIntOrg;                    // 内部机构
  char          szStkpbu[8 + 1];            // 交易单元
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18 + 1];       // 期权合约账户
  char          szOwnerType[3 + 1];         // 订单所有类型
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          chCurrency;                 // 货币代码
  char          chOptUndlCls;               // 标的证券类别
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  char          szOrderPrice[11 + 1];       // 委托价格
  char          szOrderAmt[21 + 1];         // 委托金额
  char          szMarginPreFrz[21 + 1];     // 预占用保证金  卖开委托时填写预冻结的保证金金额，其他情况填“0”
  char          szMarginFrz[21 + 1];        // 占用保证金    卖开成交时填写实际冻结的保证金金额，其他情况填“0”
  char          szMarginPreUfz[21 + 1];     // 预释放保证金  买平委托时填写预解冻的保证金金额，其他情况填“0”
  char          szMarginUfz[21 + 1];        // 释放保证金    买平成交时填写实际解冻的保证金金额，其他情况填“0”
  char          szCombNum[16 + 1];          // 组合编码       构建组合、解除组合时填写，其他情况填空
  char          szCombStraCode[16 + 1];     // 组合策略代码    构建组合、解除组合时填写，其他情况填空
  char          szLeg1Num[16 + 1];          // 成分一合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg2Num[16 + 1];          // 成分二合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg3Num[16 + 1];          // 成分三合约编码  构建组合、解除组合时填写，其他情况填空
  char          szLeg4Num[16 + 1];          // 成分四合约编码  构建组合、解除组合时填写，其他情况填空
  char          szOptBcostRltPrice[21 + 1]; // 实时成本价
  char          szOrderNo[10 + 1];          // 委托编号
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

// 委托信息
struct MATRADEAPI CRtnOptOrderField
{
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int           iOrderDate;                 // 委托日期
  char          szStkCode[8 + 1];           // 证券代码
  int           iOrderBsn;                  // 委托批号
  char          szOrderId[10 + 1];          // 合同序号
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iCuacctSn;                  // 账户序号
  char          szTrdacct[20 + 1];          // 交易账户
  char          szOrderPrice[11 + 1];       // 委托价格
  LONGLONG      llOrderQty;                 // 委托数量
  char          szOrderAmt[21 + 1];         // 委托金额
  char          szOrderFrzAmt[21 + 1];      // 委托冻结金额
  char          chOrderStatus;              // 委托状态 字典[ORDER_STATUS] 本次成交后委托的状态
  int           iStkBiz;                    // 证券业务
  int           iStkBizAction;              // 证券业务行为
  LONGLONG      llWithdrawnQty;             // 已撤单数量
  LONGLONG      llTotalMatchedQty;          // 累计成交数量
  char          szStkpbu[8 + 1];            // 交易单元
};

// 合约信息
struct MATRADEAPI CRtnOptContractField
{
  char          chStkex;                    // 交易市场 字典[STKEX]
  char          szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char          szStkCode[8 + 1];           // 证券代码
  char          szTrdacct[20 + 1];          // 交易账户
  LONGLONG      llCustCode;                 // 客户代码
  LONGLONG      llCuacctCode;               // 资产账户
  int           iStkBiz;                    // 证券业务
  int           iStkBizAction;              // 证券业务行为
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18 + 1];       // 期权合约账户
  char          szOwnerType[3 + 1];         // 订单所有类型
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          chCurrency;                 // 货币代码
  char          chOptUndlCls;               // 标的证券类别
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  char          szMarginPreFrz[21 + 1];     // 预占用保证金  卖开委托时填写预冻结的保证金金额，其他情况填“0”
  char          szMarginFrz[21 + 1];        // 占用保证金    卖开成交时填写实际冻结的保证金金额，其他情况填“0”
  char          szMarginPreUfz[21 + 1];     // 预释放保证金  买平委托时填写预解冻的保证金金额，其他情况填“0”
  char          szMarginUfz[21 + 1];        // 释放保证金    买平成交时填写实际解冻的保证金金额，其他情况填“0”
  char          szCombNum[16 + 1];          // 组合编码       构建组合、解除组合时填写，其他情况填空
  char          szCombStraCode[16 + 1];     // 组合策略代码    构建组合、解除组合时填写，其他情况填空
  char          szLeg1Num[16 + 1];          // 成分一合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg2Num[16 + 1];          // 成分二合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg3Num[16 + 1];          // 成分三合约编码  构建组合、解除组合时填写，其他情况填空
  char          szLeg4Num[16 + 1];          // 成分四合约编码  构建组合、解除组合时填写，其他情况填空
};

//-------------------------------00102024:确认回报推送------------------------------------
struct MATRADEAPI CRtnOptOrderConfirmField
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
  int           iOrderDate;                 // 委托日期
  int           iOrderSn;                   // 委托序号
  int           iIntOrg;                    // 内部机构
  char          szStkpbu[8 + 1];            // 交易单元
  char          szOrderPrice[21 + 1];       // 委托价格
  LONGLONG      llOrderQty;                 // 委托数量
  char          szSubacctCode[8 + 1];       // 证券账户子编码
  char          szOptTrdacct[18 + 1];       // 期权合约账户
  char          szOptCode[40 + 1];          // 合约代码
  char          szOptName[120 + 1];         // 合约简称
  char          chCurrency;                 // 货币代码
  char          chOptUndlCls;               // 标的证券类别
  char          szOptUndlCode[8 + 1];       // 标的证券代码
  char          szOptUndlName[80 + 1];      // 标的证券名称
  char          szCombNum[16 + 1];          // 组合编码       构建组合、解除组合时填写，其他情况填空
  char          szCombStraCode[16 + 1];     // 组合策略代码    构建组合、解除组合时填写，其他情况填空
  char          szLeg1Num[16 + 1];          // 成分一合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg2Num[16 + 1];          // 成分二合约编码  构建组合、解除组合、行权指令合并申报时填写，其他情况填空
  char          szLeg3Num[16 + 1];          // 成分三合约编码  构建组合、解除组合时填写，其他情况填空
  char          szLeg4Num[16 + 1];          // 成分四合约编码  构建组合、解除组合时填写，其他情况填空
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

//-------------------------------10311010:组合策略文件查询------------------------------------
struct MATRADEAPI CReqOptCombStraField
{
  char szStkbd[2+1];        //交易板块       625
  char szCombStraCode[16+1];//组合策略代码     8928
  char chQueryFlag;      //查询方向       8987
  char szQueryPos[32+1];    //定位串         8991
  int  iQueryNum;          //查询行数       8992
    
};
struct MATRADEAPI CRspOptCombStraField
{
  char szQueryPos[32+1];      //定位串               8991
  char chStkex;          // 交易市场               207
  char szStkbd[2+1];        // 交易板块               625
  char szCombStraCode[16+1];    // 组合策略代码             8928
  char szCombStraName[32+1];    // 组合策略名称           8936
  char chSameExpDate;        // 到期日是否相同           9095
  char chSameUndl;        //标的是否相同             9096
  char chAutoSplitDay;      // 自动解除组合日           326
  int  iLegCnt;          // 成分合约个数             9090
  char  chLeg1Type;        // 成分一合约类型           8930
  char  chLeg1Side;        //成分一持仓方向           8931
  int iLeg1ExePrcSeq;          //成分一行权价高低顺序         8932
  int iLeg1ExpDateSeq;      // 成分一合约到期日顺序         8933
  int iLeg1PosiUnit;          // 成分一单份持仓数量         8934
  char chLeg2Type;        // 成分二合约类型           8935
  char chLeg2Side;        //成分二持仓方向           9080
  int iLeg2ExePrcSeq;         //成分二行权价高低顺序         9081
  int iLeg2ExpDateSeq;      // 成分二合约到期日顺序         8929
  int iLeg2PosiUnit;         // 成分二单份持仓数量         9083
  char chLeg3Type;        // 成分三合约类型           9084
  char chLeg3Side;        //成分三持仓方向          9085
  int iLeg3ExePrcSeq;         //成分三行权价高低顺序         9086
  int iLeg3ExpDateSeq;      // 成分三合约到期日顺序         9087
  int iLeg3PosiUnit;          // 成分三单份持仓数量         9088
  char  chLeg4Type;        // 成分四合约类型           9089
  char  chLeg4Side;        // 成分四持仓方向           9091
  int iLeg4ExePrcSeq;          // 成分四行权价高低顺序         9092
  int iLeg4ExpDateSeq;      // 成分四合约到期日顺序         9093
  int iLeg4PosiUnit;          // 成分四单份持仓数量         9094
    

};

//-------------------------------10312016:组合保证金委托------------------------------------
struct MATRADEAPI CReqOptCombStraOrderField
{
  LONGLONG  llCustCode  ;    //客户代码          8902
  LONGLONG  llCuacctCode  ;  //资产账户          8920
  int    iIntOrg;        //内部机构          8911
  char  szStkbd[2+1];      //交易板块          625
  char  szTrdacct[20+1];    //证券账户          448
  char  szCombNum[16+1];    //组合编码          8900
  char  szCombStraCode[16+1];  //组合策略代码        8928
  LONGLONG  llOrderQty;      //委托数量          38
  char  szLeg1Num[16+1]  ;    //成分一合约编码      9090
  char  szLeg2Num[16+1]  ;    //成分二合约编码      9091
  char  szLeg3Num[16+1]  ;    //成分三合约编码      9092
  char  szLeg4Num[16+1];    //成分四合约编码      9094
  int    iStkBiz  ;        //证券业务          8842
  int    iStkBizAction;      //证券业务行为        40
  char  szStkpbu[8+1];      //交易单元          8843
  int    iOrderBsn  ;      //委托批号          66
  char  szClientInfo[256+1];  //终端信息          9039
  char  chSecurityLevel;    //安全手段          9080
  char  szSecurityInfo[256+1];  //安全信息          9081
  char  szOrderIdEx[64+1];    //外部合同序号        9093
  char  chEncryptType;      //加密方式          9085
  char    szEncryptKey[32+1];    //加密因子          9086
  
};
struct MATRADEAPI CRspOptCombStraOrderField
{
  int     iOrderBsn;            //委托批号       66
  char  szOrderId[10+1];        //合同序号       11
  LONGLONG  llCuacctCode;        //资产账户       8920
  LONGLONG  llOrderQty;          //委托数量       38
  char  szStkpbu[8+1];          //交易单元       8843
  char  szStkbd[2+1];          //交易板块       625
  char  szTrdacct[20+1];        //证券账户       448
  char  szSubAcctcode[8+1];        //证券账户子编码    9099
  char  szOptTrdacct[18+1];        //期权合约账户     9098
  char  szCombNum[16+1];        //组合编码       8900
  char  szCombStraCode[16+1];      //组合策略代码     8928
  char  szLeg1Num[16+1];        //成分一合约编码    9090
  char  szLeg2Num[16+1];        //成分二合约编码    9091
  char  szLeg3Num[16+1];        //成分三合约编码    9092
  char  szLeg4Num[16+1];        //成分四合约编码    9094
  int    iStkBiz;            //证券业务       8842
  int    iStkBizAction;          //证券业务行为     40
  char  szOrderIdEx[64+1];        //外部合同序号     9093
      

};

//-------------------------------10313023:可用组合持仓------------------------------------
struct MATRADEAPI CReqOptCombStraPosField
{
  LONGLONG    llCustCode;            //客户代码  CUST_CODE  BIGINT  X  8902
  LONGLONG  llCuacctCode;          //资产账户  CUACCT_CODE  BIGINT  X  8920
  char    szStkbd[2+1];          //交易板块  STKBD  CHAR(2)  X  625
  char    szTrdacct[20+1];        //交易账户  TRDACCT  VARCHAR(10)  X  448
  char    szCombStraCode[16+1];      //组合策略代码  COMB_STRA_CODE  VARCHAR(16)  X  8928
  char    szLeg1Num[16+1];        //成分一合约编码  LEG1_NUM  VARCHAR(16)  X  9090
  char    szLeg2Num[16+1];        //成分二合约编码  LEG2_NUM  VARCHAR(16)  X  9091
  char    szLeg3Num[16+1];        //成分三合约编码  LEG3_NUM  VARCHAR(16)  X  9092
  char    szLeg4Num[16+1];        //成分四合约编码  LEG4_NUM  VARCHAR(16)  X  9094
  char    chQueryFlag;          //查询方向  QUERY_FLAG  CHAR(1)  X  9080
  char    szQueryPos[32+1];        //定位串  QRY_POS  VARCHAR(32)  X  8991
  int      iQueryNum;            //查询行数  QRY_NUM  INTEGER  X  8992
  
  
};
struct MATRADEAPI CRspOptCombStraPosField
{
  char    szQueryPos[32+1];      //定位串           8991
  LONGLONG  llCustCode;          //客户代码        8902
  LONGLONG  llCuacctCode;        //资产账户        8920
  int      iIntOrg;          //内部机构        8911
  char    chStkex;          //交易市场        207
  char    szStkbd[2+1];        //交易板块        625
  char    szStkpbu[8+1];        //交易单元        8843
  char    szTrdacct[20+1];      //证券账户        448
  char    szSubacctCode[8+1];      //证券账户子编码       9099
  char    szOptTrdacct[18+1];      //期权合约账户        9098
  char    chCurrency;          //货币代码        15
  char    szCombStraCode[16+1];    //组合策略代码        8928
  char    szCombStraName[32+1];    //组合策略名称        8936
  LONGLONG  llCombPrebln;        //组合持仓昨日余额    8860
  LONGLONG  llCombBln;          //组合持仓余额        8861
  LONGLONG  llCombAvl;          //组合持仓可用        8869
  LONGLONG  llCombTrdFrz;        //组合持仓交易冻结    8872
  LONGLONG  llCombTrdUfz;        //组合持仓交易解冻    8873
  LONGLONG  llCombTrdOtd;        //组合持仓交易在途    8874
  LONGLONG  llCombTrdBln;        //组合持仓交易轧差    8867
  LONGLONG  llCombClrFrz;        //组合持仓清算冻结    8887
  LONGLONG  llCombClrUfz;        //组合持仓清算解冻    8888
  LONGLONG  llCombClrOtd;        //组合持仓清算在途    8889
  LONGLONG  llSecuCombMargin;      //组合占用保证金       8871
  int      iLegCnt;          //成分合约个数        8875
  char    szLeg1Num[16+1];      //成分一合约编码       9090
  char    chLeg1Type;          //成分一合约类型       8877
  char    chLeg1Side;          //成分一持仓方向       8878
  LONGLONG  llLeg1Posi;          //成分一持仓数量       8879
  char    szLeg2Num[16+1];      //成分二合约编码       9091
  char    chLeg2Type;          //成分二合约类型       9084
  char    chLeg2Side;          //成分二持仓方向       9086
  LONGLONG  llLeg2Posi;          //成分二持仓数量       9087
  char    szLeg3Num[16+1];      //成分三合约编码       9092
  char    chLeg3Type;          //成分三合约类型       9093
  char    chLeg3Side;          //成分三持仓方向       9089
  LONGLONG  llLeg3Posi;          //成分三持仓数量       9095
  char    szLeg4Num[16+1];      //成分四合约编码       9094
  char    chLeg4Type;          //成分四合约类型       9096
  char    chLeg4Side;          //成分四持仓方向       9097
  LONGLONG  llLeg4Posi;          //成分四持仓数量       8876
    
    
};

//-------------------------------10313024:可用组合持仓明细------------------------------------
struct MATRADEAPI CReqOptCombStraPosDetailField
{
  LONGLONG    llCustCode;             //客户代码  CUST_CODE  BIGINT  X  8902
  LONGLONG  llCuacctCode;           //资产账户  CUACCT_CODE  BIGINT  X  8920
  char    szStkbd[2+1];           //交易板块  STKBD  CHAR(2)  X  625
  char    szTrdacct[20+1];         //交易账户  TRDACCT  VARCHAR(10)  X  448
  char    szCombNum[16+1];         //组合编码  COMB_NUM  VARCHAR(16)  X  8900
  char    szCombStraCode[16+1];       //组合策略代码  COMB_STRA_CODE  VARCHAR(16)  X  8928
  char    szLeg1Num[16+1];         //成分一合约编码  LEG1_NUM  VARCHAR(16)  X  9090
  char    szLeg2Num[16+1];         //成分二合约编码  LEG2_NUM  VARCHAR(16)  X  9091
  char    szLeg3Num[16+1];         //成分三合约编码  LEG3_NUM  VARCHAR(16)  X  9092
  char    szLeg4Num[16+1];         //成分四合约编码  LEG4_NUM  VARCHAR(16)  X  9094
  char    chQueryFlag;           //查询方向  QUERY_FLAG  CHAR(1)  X  9080
  char    szQueryPos[32+1];         //定位串  QRY_POS  VARCHAR(32)  X  8991
  int      iQueryNum;             //查询行数  QRY_NUM  INTEGER  X  8992
  
};
struct MATRADEAPI CRspOptCombStraPosDetailField
{
  char    szQueryPos[32+1];     //定位串               8991
  LONGLONG  llCustCode;         //客户代码              8902
  LONGLONG  llCuacctCode;       //资产账户              8920
  int      iIntOrg;         //内部机构              8911
  char    chStkex;         //交易市场              207
  char    szStkbd[2+1];       //交易板块              625
  char    szStkpbu[8+1];       //交易单元              8843
  char    szTrdacct[20+1];     //证券账户              448
  char    szSubacctCode[8+1];     //证券账户子编码           9099
  char    szOptTrdacct[18+1];     //期权合约账户          9098
  char    chCurrency;         //货币代码              15
  char    szCombNum[16+1];     //组合编码              8900
  char    szCombStraCode[16+1];   //组合策略代码          8928
  char    szCombStraName[32+1];   //组合策略名称          8936
  LONGLONG  llCombPrebln;       //组合持仓昨日余额          8860
  LONGLONG  llCombBln;         //组合持仓余额          8861
  LONGLONG  llCombAvl;         //组合持仓可用          8869
  LONGLONG  llCombTrdFrz;       //组合持仓交易冻结          8872
  LONGLONG  llCombTrdUfz;       //组合持仓交易解冻          8873
  LONGLONG  llCombTrdOtd;       //组合持仓交易在途          8874
  LONGLONG  llCombTrdBln;       //组合持仓交易轧差          8867
  LONGLONG  llCombClrFrz;       //组合持仓清算冻结          8887
  LONGLONG  llCombClrUfz;       //组合持仓清算解冻          8888
  LONGLONG  llCombClrOtd;       //组合持仓清算在途          8889
  LONGLONG  llSecuCombMargin;     //组合占用保证金           8871
  int      iLegCnt;         //成分合约个数          8875
  char    szLeg1Num[16+1];     //成分一合约编码           9090
  char    chLeg1Type;         //成分一合约类型           8877
  char    chLeg1Side;         //成分一持仓方向           8878
  LONGLONG  llLeg1Posi;         //成分一持仓数量           8879
  char    szLeg2Num[16+1];     //成分二合约编码           9091
  char    chLeg2Type;         //成分二合约类型           9084
  char    chLeg2Side;         //成分二持仓方向           9086
  LONGLONG  llLeg2Posi;         //成分二持仓数量           9087
  char    szLeg3Num[16+1];     //成分三合约编码           9092
  char    chLeg3Type;         //成分三合约类型           9093
  char    chLeg3Side;         //成分三持仓方向           9089
  LONGLONG  llLeg3Posi;         //成分三持仓数量           9095
  char    szLeg4Num[16+1];     //成分四合约编码           9094
  char    chLeg4Type;         //成分四合约类型           9096
  char    chLeg4Side;         //成分四持仓方向           9097
  LONGLONG  llLeg4Posi;         //成分四持仓数量           8876
  
};

//-------------------------------10102500:客户结算单查询------------------------------------
struct MATRADEAPI CReqOptSettListField
{
  int     iTrdDate;    //  交易日期  TRD_DATE  INTEGER  √
  LONGLONG llCuacctCode;  //  资产帐号  CUACCT_CODE  BIGINT  √
  LONGLONG llCustCode;  //  客户代码  CUST_CODE  BIGINT  √
  char     chCurrency;    //  货币代码  CURRENCY  CHAR(1)  √

};
struct MATRADEAPI CRspOptSettListField
{
  int      iTrdDate;        //交易日期  TRD_DATE  INTEGER
  int      iIntOrg;        //内部机构  INT_ORG  SMALLINT
  LONGLONG llCustCode;        //客户代码  CUST_CODE  BIGINT
  char     szCustName[64+1];      //客户名称  CUST_NAME  VARCHAR(64)
  LONGLONG llCuacctCode;        //资产账户  CUACCT_CODE  BIGINT
  char     chCurrency;      //货币代码  CURRENCY  CHAR(1)
  char     szFundPrebln[21+1];  //期初结存  FUND_PREBLN  CMONEY
  char     szFundBln[21+1];    //期末结存  FUND_BLN  CMONEY
  char     szFundAvl[21+1];    //可用资金  FUND_AVL  CMONEY
  char     szFundExeFrz[21+1];  //行权资金冻结金额  FUND_EXE_FRZ  CMONEY
  char     szFundExeMargin[21+1];  //行权冻结维持保证金  FUND_EXE_MARGIN  CMONEY
  char     szFundUsedMargin[21+1];//占用保证金  FUND_USED_MARGIN  CMONEY
  char     szPaylater[21+1];    //垫付资金  PAYLATER  CMONEY
  char     szExpectPnint[21+1];  //预计垫资罚息  EXPECT_PNINT  CMONEY
  char     szRepayPaylater[21+1];  //归还垫资  REPAY_PAYLATER  CMONEY
  char     szRepayPnint[21+1];  //归还罚息  REPAY_PNINT  CMONEY
  char     szRemitPnint[21+1];  //减免罚息  REMIT_PNINT  CMONEY
  char     szInterstIncome[21+1];  //利息归本  INTERST_INCOME  CMONEY
  char     szInterstIncomeTax[21+1];//利息税  INTERST_INCOME_TAX  CMONEY
  char     szInAmt[21+1];      //银衍入金  IN_AMT  CMONEY
  char     szOutAmt[21+1];    //银衍出金  OUT_AMT  CMONEY
  char     szPremiumPay[21+1];  //权利金收付  PREMIUM_PAY  CMONEY
  char     szExecPay[21+1];    //行权收付  EXEC_PAY  CMONEY
  char     szSXFFee[21+1];    //手续费  SXF_FEE  CMONEY
  char     szQsfFee[21+1];    //结算费  QSF_FEE  CMONEY
  char     szJsfFee[21+1];    //经手费  JSF_FEE  CMONEY
  char     szJysJsfFee[21+1];    //交易所经手费  JYS_JSF_FEE  CMONEY
  char     szXqghFee[21+1];    //行权过户费  XQGH_FEE  CMONEY
  char     szXqjsfFee[21+1];    //行权结算费  XQJSF_FEE  CMONEY
  char     szXqsxfFee[21+1];    //行权手续费  XQSXF_FEE  CMONEY
  LONGLONG llOptFloatProfit;      //浮动盈亏  OPT_FLOAT_PROFIT  BIGINT
  LONGLONG llQuotaValUsed;      //占用买入额度  QUOTA_VAL_USED  BIGINT
  LONGLONG llQuotaVal;        //买入额度  QUOTA_VAL  BIGINT
  char     szRightMktVal[21+1];  //权利仓市值  RIGHT_MKT_VAL  CMONEY
  char     szDuteMktVal[21+1];  //义务仓市值  DUTE_MKT_VAL  CMONEY
  char     szMarginRiskRate[8+1];  //保证金风险率  MARGIN_RISK_RATE  CRATE
  char     szAddMarginAmt[21+1];  //应追加保证金  ADD_MARGIN_AMT  CMONEY
  char     szAddMarginNotice[2048+1];    //追保通知内容  ADD_MARGIN_NOTICE  VARCHAR(2048)
  char     chConfirmFlag;      //客户确认标志  CONFIRM_FLAG  CHAR(1)
  char     szConfirmTime[25+1];  //客户确认时间  CONFIRM_TIME  DATETIME
};

//-------------------------------10102501:客户结算单查询------------------------------------
struct MATRADEAPI CReqOptSettListConfirmField
{
  int           iTrdDate;                     // 交易日期
  LONGLONG     llCuacctCode;                 // 资产帐号
  LONGLONG     llCustCode;                   // 客户代码
  char          chCurrency;          // 货币代码
};
struct MATRADEAPI CRspOptSettListConfirmField
{
  //none
};

//-------------------------------10102502:银衍转账------------------------------------
struct MATRADEAPI CReqOptBankDeriTransField
{
  LONGLONG llCuacctCode;         //资产账户
  char     chCurrency;         //货币代码
  char     szFundPwd[32+1];       //资金密码
  char     szBankCode[4+1];       //银行代码
  char     szBankPwd[32+1];       //银行密码
  char     chTransType;         //转账类型
  char     szTransAmt[21+1];       //转账金额
  char     chEncryptType;         //加密方式
  char     szEncryptKey[32+1];     //加密因子
  char     szOpRemark[128+1];       //操作备注
  
};
struct MATRADEAPI CRspOptBankDeriTransField
{
  LONGLONG llSerialNo;        //流水序号
};

//-------------------------------10102503:银衍交易配置信息------------------------------------
struct MATRADEAPI CReqOptBankDeriInfoField
{
  char          chCurrency;          // 货币代码
  int           iExtOrg;                     // 外部机构
  int           iIntOrg;                     // 内部机构
  char          szCubsbTrdId[2 + 1];          // 银衍业务标识
};
struct MATRADEAPI CRspOptBankDeriInfoField
{
  int           iExtOrg;                      // 外部机构
  int           iIntOrg;                      // 内部机构
  char          chCurrency;          // 货币代码
  char          szCubsbTrdId[2 + 1];          // 银衍业务标识
  char          chChkId;            // 校验身份
  char          chChkFundPwd;          // 校验资金密码
  char          chChkBankPwd;          // 校验银行密码
  char          chChkBankAcct;        // 校验银行账户
  char          chSendToBank;          // 发送银行标志
  char          chSendCuacct;          // 发送资产账户
  char          szChannels[32 + 1];           // 操作渠道
  char          szUserTypes[16 + 1];          // 客户类型
  char          szCuacctAttrs[8 + 1];         // 资产账户属性
  char          szWeekDays[64 + 1];           // 周有效日
  int           iBgnStime;                    // 开始时间
  int           iEndStime;                    // 结束时间
};

//-------------------------------10102504:客户签约银行代码------------------------------------
struct MATRADEAPI CReqOptUserBankField
{
  char          chCurrency;             // 货币代码
  LONGLONG      llCuacctCode;           // 资产帐户
};
struct MATRADEAPI CRspOptUserBankField
{
  char          chCurrency;             // 货币代码
  int           iExtOrg;                // 外部机构
  char          szExtOrgName[32 + 1];   // 外部机构名称
};

//-------------------------------10102505:对账单行权交割流水------------------------------------
struct MATRADEAPI CReqOptStateDeliHisField
{
  LONGLONG     llCustCode;                   // 客户代码
  LONGLONG     llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szIntOrges[2048 + 1];         // 机构代码
  char          szStkBizs[2048 + 1];          // 证券业务
  int           iBgnDate;                     // 起始日期
  int           iEndDate;                     // 终止日期
  int           iQryPos;                      // 定位串
  int           iQryNum;                      // 查询行数
};
struct MATRADEAPI CRspOptStateDeliHisField
{
  int           iQryPos;                      // 定位串
  LONGLONG      llSerialNo;                   // 流水序号
  int           iSettDate;                    // 清算日期
  int           iOccurDate;                   // 发生日期
  char          szOccurTime[32 + 1];          // 发生时间
  int           iComeInDate;                  // 到账日期
  int           iIntOrg;                      // 内部机构
  char          szTrdacct[20 + 1];            // 交易账户
  char          szSubacctCode[8 + 1];         // 交易账户子编码
  char          szOptTrdacct[18 + 1];         // 期权合约账户
  char          chTrdacctExcls;               // 交易账户类别
  char          chStkex;                      // 交易市场
  char          szStkbd[2 + 1];               // 交易板块
  LONGLONG      llCustCode;                   // 客户代码
  char          szCustName[32 + 1];           // 客户姓名
  char          chCustCls;                    // 客户类别
  char          szCustType[2 + 1];            // 客户类型
  LONGLONG      llCuacctCode;                 // 资产账户
  char          chCurrency;                   // 货币代码
  int           iStkBiz;                      // 证券业务
  int           iStkBizAction;                // 证券业务行为
  char         szBizAmt[21 + 1];              // 业务金额LONGLONG->char[] 2015/10/21
  char         szFundBln[21 + 1];             // 资金当前余额LONGLONG->char[] 2015/10/21
  char         szFundAvl[21 + 1];             // 资金当前可用LONGLONG->char[] 2015/10/21
  char         szFundClrFrz[21 + 1];          // 冻结保证金LONGLONG->char[] 2015/10/21
  char         szFundClrUfz[21 + 1];          // 解冻保证金LONGLONG->char[] 2015/10/21
  LONGLONG     llOptUndlQty;                  // 交收标的数量
  char         szOptUndlCode[8 + 1];          // 标的代码
  char         szStampDuty[21+1];             // 印花税LONGLONG->char[] 2015/10/21
  char         szHandleFee[21+1];             // 经手费LONGLONG->char[] 2015/10/21
  char         szSecuReguFee[21+1];           // 证管费LONGLONG->char[] 2015/10/21
  char         szTrdRegFee[21+1];             // 交易规费LONGLONG->char[] 2015/10/21
  char         szClearingFee[21+1];           // 清算费LONGLONG->char[] 2015/10/21
  char         szVentureFee[21+1];            // 风险金LONGLONG->char[] 2015/10/21
  char         szTransFee[21+1];              // 过户费LONGLONG->char[] 2015/10/21
  char         szExeTrinFee[21+1];            // 行权过户费LONGLONG->char[] 2015/10/21
  char         szPaidinCom[21+1];             // 净手续费LONGLONG->char[] 2015/10/21
  char         szFeeAmt[21+1];                // 总费用LONGLONG->char[] 2015/10/21
  int           iBizCode;                     // 业务代码
  char          szBizName[32 + 1];            // 业务代码名称
  char          szRemark[128 + 1];            // 备注信息
};

//-------------------------------10102506:银衍转账流水------------------------------------
struct MATRADEAPI CReqOptBankDeriTransHisField
{
  LONGLONG     llSerialNo;                   // 流水序号
  char          chCurrency;          // 货币代码
  LONGLONG     llCuacctCode;                 // 资产帐号
  LONGLONG     llCustCode;                   // 客户代码
  int           iExtOrg;                     // 外部机构
  int           iIntOrg;                     // 内部机构
  char          szCubsbTrdIds[64 + 1];        // 银衍业务标识
  char          szCubsbTrdStatuss[16 + 1];    // 银衍业务状态
  char          szCancelStatuss[16 + 1];      // 冲销状态
};
struct MATRADEAPI CRspOptBankDeriTransHisField
{
  LONGLONG     llSerialNo;                   // 流水序列号
  int           iSettDate;                    // 清算日期
  int           iOccurDate;                   // 发生日期
  char          szOccurTime[32 + 1];          // 发生时间
  int           iExtOrg;                     // 外部机构
  int           iIntOrg;                     // 内部机构
  LONGLONG     llCustCode;                   // 客户代码
  char          chUserType;          // 用户类型
  char          szCustName[32 + 1];           // 客户姓名
  LONGLONG     llCuacctCode;                 // 资产账户
  char          chCuacctAttr;          // 资产账户属性
  char          szBankAcct[32 + 1];           // 银行账户
  char          szBdmfAcct[32 + 1];           // 存管账户
  char          szIdType[2 + 1];              // 证件类型
  char          szIdCode[32 + 1];             // 证件号码
  char          chCurrency;          // 货币代码
  char          szCubsbTrdId[2 + 1];          // 银衍业务标识
  char          szBizAmt[21 + 1];             // 业务金额 LONGLONG->char[] 2015/10/21
  char          szFundBln[21 + 1];            // 资金当前余额 LONGLONG->char[] 2015/10/21
  char          szAuthData[64 + 1];           // 认证数据
  char          chCubsbTrdStatus;        // 银衍业务状态
  char          chCancelStatus;         // 冲销标志
  char          chOriginalSn;          // 原流水序号
  LONGLONG     llCuacctLogSn;                // 记账流水序号
  int           iRpcSubsysSn;                // 子系统
  int           iIntMsgCode;                 // 内部调用信息代码
  char          szIntMsgText[256 + 1];        // 内部调用信息内容
  LONGLONG     llIntSerialNo;                // 内部调用流水序号
  int           iIntReqStime;                 // 内部调用请求时间
  int           iIntAnsStime;                 // 内部调用应答时间
  char          szExtMsgCode[256 + 1];        // 外部调用信息代码
  char          szExtMsgText[256 + 1];        // 外部调用信息内容
  char          szExtSerialNo[32 + 1];        // 外部调用流水序号
  int           iExtReqStime;                 // 外部调用请求时间
  int           iExtAnsStime;                 // 外部调用应答时间
  int           iCanceledTimes;              // 已冲销次数
  char          szLstCancelTime[256 + 1];     // 最后冲销时间
  LONGLONG     llOpUser;                     // 操作用户
  char          chOpRole;            // 操作用户角色
  char          szOpName[32 + 1];             // 操作用户姓名
  int           iOpOrg;                      // 操作机构
  char          szOpSite[32 + 1];             // 操作站点
  char          chChannel;          // 操作渠道
  char          szRemark[128 + 1];            // 备注信息
};

//-------------------------------10102507:银行余额------------------------------------
struct MATRADEAPI CReqOptBankBalaField
{
  char          szReqSn[32 + 1];              // 请求流水号
  char          chCurrency;                   // 货币代码
  LONGLONG      llCuacctCode;                 // 资产账户
  int           iExtOrg;                      // 外部机构
  char          szFundAuthData[32 + 1];       // 资金认证数据
  char          szBankAuthData[32 + 1];       // 银行认证数据
  char          chEncryptType;                // 加密方式
  char          szEncryptKey[32 + 1];         // 加密因子
  char          szBankCustNum[200 + 1];       // 银行客户号
  char          szOpRemark[128 + 1];          // 操作备注
};
struct MATRADEAPI CRspOptBankBalaField
{
  LONGLONG     llAnsSn;                      // 流水序号
};

//-------------------------------10102508:密码修改------------------------------------
struct MATRADEAPI CReqOptChangePwdField
{
  LONGLONG llUserCode;           //客户代码  USER_CODE  BIGINT  √  
  char   chUseScope;           //使用范围  USE_SCOPE  CHAR(1)  √  
  char     chAuthType;           //认证类型  AUTH_TYPE  VARCHAR(1)  √  
  char     szOldAuthData[256+1];       //旧密码  OLD_AUTH_DATA  VARCHAR(256)  √  
  char   szNewAuthData[256+1];       //新密码  NEW_AUTH_DATA  VARCHAR(256)  √  
  char     chEncryptType;           //加密方式  ENCRYPT_TYPE  CHAR(1)  √  
  char     szEncryptKey[32+1];       //加密因子  ENCRYPT_KEY  VARCHAR(32)  √  
  
};
struct MATRADEAPI CRspOptChangePwdField
{
};

//-------------------------------10312013:个股期权初始保证金------------------------------------
struct MATRADEAPI CReqOptIniMarginField
{
  LONGLONG llCuacctCode;    //资产账户  CUACCT_CODE  BIGINT  √  8920
  char     chCurrency;    //货币代码  CURRENCY  CHAR(1)  √  15
  char     szStkbd[2+1];    //交易板块  STKBD  CHAR(2)  √  625
  char     szTrdacct[20+1];  //证券账户  TRDACCT  VARCHAR(10)  √  448
  char     szOptNum[16+1];  //合约编码  OPT_NUM  VARCHAR(16)  √  9082
  LONGLONG llOrderQty;    //委托数量  ORDER_QTY  BIGINT  √  38

};
struct MATRADEAPI CRspOptIniMarginField
{
  LONGLONG llCustCode;      //客户代码  CUST_CODE  BIGINT  8902
  LONGLONG llCuacctCode;      //资产账户  CUACCT_CODE  BIGINT  8920
  char     chCurrency;      //货币代码  CURRENCY  CHAR(1)  15
  char     szStkbd[2+1];      //交易板块  STKBD  CHAR(2)  625
  char     szTrdacct[20+1];    //证券账户  TRDACCT  VARCHAR(10)  448
  char     szSubacctCode[8+1];  //证券账户子编码  SUBACCT_CODE  VARCHAR(8)  9099
  char     szOptTrdacct[18+1];  //期权合约账户  OPT_TRDACCT  VARCHAR(18)  9098
  char     szOptNum[16+1];    //合约编码  OPT_NUM  VARCHAR(16)  9082
  LONGLONG llOrderQty;      //委托数量  ORDER_QTY  BIGINT  38
  char     szSecuMargin[21+1];  //初始保证金(券商)  SECU_MARGIN  CMONEY  9086
  char     szStkexMargin[21+1];  //初始保证金(交易所)  STKEX_MARGIN  CMONEY  9087

};

//-------------------------------10313020:行权指派欠资------------------------------------
struct MATRADEAPI CReqOptExeDebtField
{
  LONGLONG llCustCode;  //客户代码  CUST_CODE  BIGINT  X  8902
  LONGLONG llCuacctCode;  //资产账户  CUACCT_CODE  BIGINT  X  8920

};
struct MATRADEAPI CRspOptExeDebtField
{
  LONGLONG llCustCode;    //客户代码  CUST_CODE  BIGINT  8902
  char     szCustName[32+1];  //客户姓名  CUST_NAME  VARCHAR(32)  8932
  LONGLONG llCuacctCode;    //资产账户  CUACCT_CODE  BIGINT  8920
  char     chCurrency;        //货币代码  CURRENCY  CHAR(1)  15
  int      iIntOrg;      //内部机构  INT_ORG  SMALLINT  8911
  char     szFundPrebln[21+1];  //资金昨日余额  FUND_PREBLN  CMONEY  8860
  char     szFundBln[21+1];    //资金余额  FUND_BLN  CMONEY  8984
  char     szFundAvl[21+1];    //资金可用金额  FUND_AVL  CMONEY  8861
  char   szExeDueAmt[21+1];    //行权应付资金  EXE_DUE_AMT  CMONEY  8862
  char   szExeLackAmt[21+1];  //行权欠资金额  EXE_LACK_AMT  CMONEY  9086
};

//-------------------------------10313021:行权指派欠券------------------------------------
 struct MATRADEAPI CReqOptExeStkDebtField
 {
  LONGLONG llCustCode;      //客户代码  CUST_CODE  BIGINT  X  8902
  LONGLONG llCuacctCode;      //资产账户  CUACCT_CODE  BIGINT  X  8920
  char     szStkbd[2+1];       //交易板块  STKBD  CHAR(2)  √  625
  char     szTrdacct[20+1];     //交易账户  TRDACCT  VARCHAR(10)  √  448
  char     szOptUndlCode[8+1];   //标的证券代码  OPT_UNDL_CODE  VARCHAR(8)  X  9090

 };
 struct MATRADEAPI CRspOptExeStkDebtField
 {
  LONGLONG llCustCode;     //客户代码  CUST_CODE  BIGINT  8902
  char     szCustName[32+1];   //客户姓名  CUST_NAME  VARCHAR(32)  8932
  LONGLONG llCuacctCode;     //资产账户  CUACCT_CODE  BIGINT  8920
  char     chCurrency;       //货币代码  CURRENCY  CHAR(1)  15
  int      iIntOrg;       //内部机构  INT_ORG  SMALLINT  8911
  char     szStkbd[2+1];     //交易板块  STKBD  CHAR(2)  625
  char     szTrdacct[20+1];   //交易账户  TRDACCT  VARCHAR(10)  448
  char     szOptUndlCode[8+1]; //标的证券代码  OPT_UNDL_CODE  VARCHAR(8)  9090
  char     szOptUndlName[80+1];//标的证券名称  OPT_UNDL_NAME  VARCHAR(80)  9083
  LONGLONG llOptUndlAvl;      //标的证券可用  OPT_UNDL_AVL  BIGINT  9091
  LONGLONG llExeDueQty;      //行权应付数量  EXE_DUE_QTY  BIGINT  8862
  LONGLONG llExeLackQty;     //行权欠券数量  EXE_LACK_QTY  BIGINT  9087

 };

 //-------------------------------10313011:客户风险度------------------------------------
 struct MATRADEAPI CReqOptRiskLvlField
 {
  LONGLONG llCuacctCode;     //资产账户  CUACCT_CODE  BIGINT  √  8920
  char   chCurrency;     //货币代码  CURRENCY  CHAR(1)  √  15
  char   szStkbd[2+1];        //交易板块  STKBD  CHAR(2)  X  625
  char   chValueFlag;      //取值标志  VALUE_FLAG  CHAR(1)  X  9080

 };
 struct MATRADEAPI CRspOptRiskLvlField
 {
  LONGLONG llCustCode;          //客户代码  CUST_CODE  BIGINT  8902
  LONGLONG llCuacctCode;          //资产账户  CUACCT_CODE  BIGINT  8920
  char     chCurrency;        //货币代码  CURRENCY  CHAR(1)  15
  int      iIntOrg;          //内部机构  INT_ORG  SMALLINT  8911
  char     szFundBln[21+1];      //资金余额  FUND_BLN  CMONEY  8984
  char     szDueAddAmt[21+1];      //应补资金  DUE_ADD_AMT  CMONEY  9095
  char     szFundAvl[21+1];      //资金可用金额  FUND_AVL  CMONEY  8861
  char     szFundAvlRlt[21+1];    //实时可用金额  FUND_AVL_RLT  CMONEY  8862
  char     szStkexFundAvl[21+1];    //可用金额(交易所)  STKEX_FUND_AVL  CMONEY  8863
  char     szStkexFundAvlRlt[21+1];   //实时可用金额(交易所)  STKEX_FUND_AVL_RLT  CMONEY  8864
  char     szRiskValue[21+1];      //风险率  RISK_VALUE  CRATE  9085
  char     szRiskValueRlt[21+1];     //实时风险率  RISK_VALUE_RLT  CRATE  9086
  char     szStkexRiskValue[21+1];     //风险率(交易所)  STKEX_RISK_VALUE  CRATE  9087
  char     szStkexRiskValueRlt[21+1];   //实时风险率(交易所)  STKEX_RISK_VALUE_RLT  CRATE  9089
  char     szMarginUsed[21+1];    //占用保证金  MARGIN_USED  CMONEY  9041
  char     szMarginUsedRlt[21+1];    //实时占用保证金  MARGIN_USED_RLT  CMONEY  9042
  char     szStkexMagUsed[21+1];    //占用保证金(交易所)  STKEX_MAG_USED  CMONEY  9043
  char     szStkexMagUsedRlt[21+1];   //实时占用保证金(交易所)  STKEX_MAG_USED_RLT  CMONEY  9044
  char     szHedgedMargin[21+1];    //对冲后保证金  HEDGED_MARGIN  CMONEY  9045
  char     szHedgedMarginRlt[21+1];   //对冲后实时保证金  HEDGED_MARGIN_RLT  CMONEY  9046
  char     szStkexHedgedMag[21+1];   //对冲后保证金(交易所)  STKEX_HEDGED_MAG  CMONEY  9047
  char     szStkexHedgedMagRlt[21+1];   //对冲后实时保证金(交易所)  STKEX_HEDGED_MAG_RLT  CMONEY  9048

 };

 //-------------------------------10312011:可锁定股份查询------------------------------------
 struct MATRADEAPI CReqOptCanLockStkField
 {
   LONGLONG      llCustCode;                 // 客户代码
   LONGLONG      llCuacctCode;               // 资产账户
   char          szStkbd[2 + 1];             // 交易板块
   char          szTrdacct[20 + 1];          // 证券账户
 };

 struct MATRADEAPI CRspOptCanLockStkField
 {
   char          chStkex;                    // 交易市场
   char          szStkbd[2 + 1];             // 交易板块
   char          szTrdacct[20 + 1];          // 证券账户
   char          szSubacctCode[8 + 1];       // 证券账户子编码
   char          szStkCode[8 + 1];           // 证券代码
   LONGLONG      llOrderQty;                 // 可委托数量
 };

//-------------------------------10312019:股票期权行权指令合并申报委托--------------------------
struct MATRADEAPI CReqOptCombExeField
{
  LONGLONG        llCustCode;                 // 客户代码
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]，默认送100
  char            szStkpbu[8 + 1];            // 交易单元 
  int             iOrderBsn;                  // 委托批号 
  char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
  char            chSecurityLevel;            // 安全手段 0:无安全 1:校验票据 2:校验密码
  char            szSecurityInfo[256 + 1];    // 安全信息 安全手段为校验票据则需填写票据 安全手段为密码则需填写交易密码密文
  char            szOrderIdEx[64 + 1];        // 外部合同序号 此序号为外部传入，期权系统不做任何处理
  char            chEncryptType;              // 加密方式 当安全手段为2：校验密码时，为必填入参。 加密方式: ‘0’:金证KBSS系统加密方式 ‘1’:金证Win版集中交易加密方式 ‘2’:金证Unix版集中交易加密方式 ‘3’:外部重加密方式 ‘4’:外部加密方式 说明：该入参根据接入券商要求填写，默认值为0。 外部重加密方式’3’：使用非金证加密函数将密码明文加密为密文后，然后使用金证公司提供的通信加密函数kbss_comencrypt对密文再一次加密。 外部加密方式’4’： 使用非金证加密函数将密码明文加密为密文后作为入参。
  char            szEncryptKey[32 + 1];       // 加密因子 当安全手段为2：校验密码时，为必填入参。 加密因子可以填写为资产账户：CUACCT_CODE参数值
};

struct MATRADEAPI CRspOptCombExeField
{
  int             iOrderBsn;                  // 委托批号
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szOrderIdEx[64 + 1];        // 外部合同序号 
};

//-------------------------------10313036:行权指令合并申报成份合约查询--------------------------
struct MATRADEAPI CReqOptQryCombExeLegNumField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkpbu[8 + 1];            // 交易单元 深圳市场必送，上海市场可不送
  char            szLeg1Num[16 + 1];          // 成份一合约编码 送合并申报的其中一个合约编码
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryCombExeLegNumField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szLeg1Num[16 + 1];          // 成份一合约编码 
  char            szLeg1Code[32 + 1];         // 成份一合约代码 
  char            szLeg1Name[120 + 1];        // 成份一合约简称 
  char            chLeg1Type;                 // 成份一合约类型 字典[OPT_TYPE]
  char            chLeg1Side;                 // 成份一持仓方向 字典[OPT_SIDE]
  LONGLONG        llLeg1Unit;                 // 成份一合约单位 
  int             iLeg1EndDate;               // 成份一最后交易日 
  int             iLeg1ExerciseDate;          // 成份一期权行权日 
  int             iLeg1ExpireDate;            // 成份一期权到期日 
  char            szLeg1ExercisePrice[21 + 1];// 成份一行权价 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 成分一、成分二可行权指令合并申报
  char            szLeg2Code[32 + 1];         // 成分二合约代码 
  char            szLeg2Name[120 + 1];        // 成分二合约简称 
  char            chLeg2Type;                 // 成分二合约类型 字典[OPT_TYPE]
  char            chLeg2Side;                 // 成分二持仓方向 字典[OPT_SIDE]
  LONGLONG        llLeg2Unit;                 // 成分二合约单位 
  int             iLeg2EndDate;               // 成份二最后交易日 
  int             iLeg2ExerciseDate;          // 成分二期权行权日 
  int             iLeg2ExpireDate;            // 成分二期权到期日 
  char            szLeg2ExercisePrice[21 + 1];// 成分二行权价 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOptUndlName[80 + 1];      // 标的证券名称 
};

//-------------------------------10313027:股票期权当日委托查询[优化]--------------------------
struct MATRADEAPI CReqOptQryCurrDayOrderExField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 首页查询，该字段不送值； 翻页查询送上次查询结果中最后一条记录的QRY_POS
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryCurrDayOrderExField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szOrderTime[25 + 1];        // 委托时间 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptName[120 + 1];         // 合约简称 
  char            szCombNum[16 + 1];          // 组合编码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  char            szLeg3Num[16 + 1];          // 成分三合约编码 
  char            szLeg4Num[16 + 1];          // 成分四合约编码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            chIsWithdraw;               // 撤单标志 
  char            chIsWithdrawn;              // 已撤单标志 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
};

//-------------------------------10313028:股票期权当日成交查询[优化]--------------------------
struct MATRADEAPI CReqOptQryCurrDayFillExField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 首页查询，该字段不送值； 翻页查询送上次查询结果中最后一条记录的QRY_POS
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryCurrDayFillExField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            szMatchedTime[8 + 1];       // 成交时间 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 
  char            szOwnerType[3 + 1];         // 订单所有类型 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptName[120 + 1];         // 合约简称 
  char            szCombNum[16 + 1];          // 组合编码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  char            szLeg3Num[16 + 1];          // 成分三合约编码 
  char            szLeg4Num[16 + 1];          // 成分四合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            chIsWithdraw;               // 撤单标志 
  char            chMatchedType;              // 成交类型 
  char            szMatchedSn[32 + 1];        // 成交编号 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  char            szMatchedQty[21 + 1];       // 成交数量 
  char            szMatchedAmt[21 + 1];       // 成交金额 
  char            szMatchedFee[21 + 1];       // 成交费用 
};

//-------------------------------10313032:可用资金查询[优化]--------------------------
struct MATRADEAPI CReqOptQryExpendableFundExField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspOptQryExpendableFundExField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szFundPrebln[21 + 1];       // 资金昨日余额 
  char            szFundBln[21 + 1];          // 资金余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szFundTrdBln[21 + 1];       // 资金交易轧差金额 
  char            chFundStatus;               // 资金状态 字典[FUND_STATUS]
};

//-------------------------------10313033:可取资金查询[优化]--------------------------
struct MATRADEAPI CReqOptQryDrawFundExField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspOptQryDrawFundExField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szFundBln[21 + 1];          // 资金余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szFundDraw[21 + 1];         // 资金可取金额 
};

//-------------------------------10313034:可用合约资产查询[优化]--------------------------
struct MATRADEAPI CReqOptQryExpendableCuExField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryExpendableCuExField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptName[120 + 1];         // 合约简称 
  char            chOptType;                  // 合约类型 字典[OPT_TYPE]
  char            chOptSide;                  // 持仓方向 
  char            chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
  LONGLONG        llOptPrebln;                // 合约昨日余额 
  LONGLONG        llOptBln;                   // 合约余额 
  LONGLONG        llOptAvl;                   // 合约可用数量 
  LONGLONG        llOptTrdBln;                // 合约交易轧差数量 
  LONGLONG        llCombedQty;                // 已组合合约数量 参与组合的期权合约持仓数量
  char            szCostPrice[21 + 1];        // 合约成本价 
};

//-------------------------------10102512:历史委托查询--------------------------
struct MATRADEAPI CReqOptQryHisOrderField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szStkBizs[2048 + 1];        // 证券业务 多个证券业务用逗号分隔
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  int             iBgnDate;                   // 起始日期 
  int             iEndDate;                   // 终止日期 
  LONGLONG        llPageRecnum;               // 分页起始记录 
  int             iPageReccnt;                // 分页记录数量 默认每页100行记录
};

struct MATRADEAPI CRspOptQryHisOrderField
{
  int             iRecNum;                    // 记录序号 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[23 + 1];        // 委托时间 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 
  char            chOrderValidFlag;           // 委托有效标志 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 
  char            szOwnerType[3 + 1];         // 订单所有类型 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptCode[40 + 1];          // 合约代码 
  char            szOptName[120 + 1];         // 合约简称 
  char            szCombNum[16 + 1];          // 组合编码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  char            szLeg3Num[16 + 1];          // 成分三合约编码 
  char            szLeg4Num[16 + 1];          // 成分四合约编码 
  char            chCurrency;                 // 货币代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szOrderUfzAmt[21 + 1];      // 委托解冻金额 
  LONGLONG        llOfferQty;                 // 申报数量 
  int             iOfferStime;                // 申报时间 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
  char            chIsWithdraw;               // 撤单标志 
  char            chIsWithdrawn;              // 已撤单标志 
  char            chOptUndlCls;               // 标的证券类别 字典[STK_CLS]
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOptUndlName[80 + 1];      // 标的证券名称 
  LONGLONG        llUndlFrzQty;               // 标的券委托冻结数量 
  LONGLONG        llUndlUfzQty;               // 标的券委托解冻数量 
  LONGLONG        llUndlWthQty;               // 标的券已撤单数量 
  char            szOfferRetMsg[64 + 1];      // 申报返回信息 
};

//-------------------------------10102513:历史成交查询--------------------------
struct MATRADEAPI CReqOptQryHisFillField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szStkBizs[2048 + 1];        // 证券业务 多个证券业务用逗号分隔
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
  int             iBgnDate;                   // 起始日期 
  int             iEndDate;                   // 终止日期 
  LONGLONG        llPageRecnum;               // 分页起始记录 
  int             iPageReccnt;                // 分页记录数量 默认每页100行记录
};

struct MATRADEAPI CRspOptQryHisFillField
{
  int             iRecNum;                    // 记录序号 
  int             iTrdDate;                   // 交易日期 
  char            szMatchedTime[9 + 1];       // 成交时间 
  int             iOrderDate;                 // 委托日期 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[32 + 1];          // 成交编号 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  int             iStkBiz;                    // 证券业务 
  int             iStkBizAction;              // 证券业务行为 详见附表说明
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptName[120 + 1];         // 合约简称 
  char            szCombNum[16 + 1];          // 组合编码 
  char            szCombStraCode[16 + 1];     // 组合策略代码 
  char            szLeg1Num[16 + 1];          // 成分一合约编码 
  char            szLeg2Num[16 + 1];          // 成分二合约编码 
  char            szLeg3Num[16 + 1];          // 成分三合约编码 
  char            szLeg4Num[16 + 1];          // 成分四合约编码 
  char            chCurrency;                 // 货币代码 
  char            chOptUndlCls;               // 标的证券类别 字典[STK_CLS]
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOptUndlName[80 + 1];      // 标的证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szMatchPrice[21 + 1];       // 成交价格 
  char            szMatchQty[21 + 1];         // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szOwnerType[3 + 1];         // 订单所有类型 
};

//-------------------------------10102514:客户结算单(文件)查询--------------------------
struct MATRADEAPI CReqOptQrySettListFileField
{
  int             iTrdDate;                   // 交易日期 当交易日期入参为非必输入参时，默认为前一交易日结算单，入参格式YYYYMMDD，例如20160203表示2016年2月3日的结算单
  int             iTrdMonth;                  // 交易月份 入参格式YYYYMM，例如201602表示2016年2月的结算单
  char            chSettListType;             // 结算单类型 字典[SETT_LIST_TYPE]，结算单类型为’D’日结单时，交易日TRD_DATE入参有效。当结算单类型为’M’月结单时，交易月份TRD_MONTH入参有效。
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQrySettListFileField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llTotalLines;               // 总计行数 
  int             iTrdDate;                   // 交易日期 
  int             iTrdMonth;                  // 交易月份 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chConfirmFlag;              // 客户确认标志 ‘0’-未确认，’1’-已确认
  char            szConfirmTime[23 + 1];      // 客户确认时间 
  char            szFileFragment[512 + 1];    // 文件数据片 
};

//-------------------------------10312020:股票期权委托申报(优化)--------------------------
struct MATRADEAPI CReqOptOrderExField
{
  LONGLONG        llCuacctCode;               // 资产账户
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户
  char            szOptNum[16 + 1];           // 合约编码
  char            szOrderPrice[21 + 1];       // 委托价格
  LONGLONG        llOrderQty;                 // 委托数量
  short           iStkBiz;                    // 证券业务 字典[STK_BIZ]
  short           iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // 交易单元
  int             iOrderBsn;                  // 委托批号
  char            chEnforceFlag;              // 强制平仓标志 0-非强平 1-强平
  char            chOrderType;                // 委托类型
  char            szClientInfo[256 + 1];      // 终端信息(未启用)
};

struct MATRADEAPI CRspOptOrderExField
{
  char            szOrderId[10 + 1];          // 合同序号
  int             iOrderBsn;                  // 委托批号 
};

//-------------------------------10312022:期权做市双边报价（深圳）--------------------------
struct MATRADEAPI CReqOptBilateralQuoteField
{
  LONGLONG        llCuacctCode;               // 资产账户
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户
  char            szOptNum[16 + 1];           // 合约编码
  int             iBidStkBiz;                 // 买证券业务 400-买入开仓；403-买入平仓
  char            szBidPrice[21 + 1];         // 买价格
  LONGLONG        llBidVolume;                // 买数量
  int             iAskStkBiz;                 // 卖证券业务 401-卖出平仓；402-卖出开仓
  char            szAskPrice[21 + 1];         // 卖价格 买入卖出至少有一边的数量价格都大于0
  LONGLONG        llAskVolume;                // 卖数量
};

struct MATRADEAPI CRspOptBilateralQuoteField
{
  char            szBidOrderId[10 + 1];       // 买合同序号
  char            szAskOrderId[10 + 1];       // 卖合同序号
  int             iOrderBsn;                  // 委托批号
  LONGLONG        llCustCode;                 // 客户代码
  LONGLONG        llCuacctCode;               // 资产账户
  char            szStkpbu[8 + 1];            // 交易单元
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户
  char            szSubacctCode[8 + 1];       // 证券账户子编码
  char            szQuoteId[10 + 1];          // 报价合同序号 
};

//-------------------------------10312023:期权做市双边报价撤单（深圳）--------------------------
struct MATRADEAPI CReqOptBilateralQuotationKillField
{
  LONGLONG        llCuacctCode;               // 资产账户
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户
  char            szOrderId[10 + 1];          // 合同序号
};

struct MATRADEAPI CRspOptBilateralQuotationKillField
{
  int             iOrderBsn;                  // 委托批号
  char            szBidOrderId[10 + 1];       // 买合同序号
  char            szRawBidOrderId[10 + 1];    // 原买合同序号
  char            szAskOrderId[10 + 1];       // 卖合同序号
  char            szRawAskOrderId[10 + 1];    // 原卖合同序号
  char            szQuoteIdWtd[10 + 1];       // 撤单合同序号 
  char            szQuoteIdRaw[10 + 1];       // 原报价合同序号 
};

//-------------------------------10311022:证券信息查询（期权）--------------------------
struct MATRADEAPI CReqOptQryStkInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryStkInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[80 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            chStkStatus;                // 证券状态 字典[STK_STATUS]
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkUplmtRatio[21 + 1];    // 涨停比例 
  char            szStkLwlmtRatio[21 + 1];    // 跌停比例 
  char            szStkUplmtPrice[21 + 1];    // 涨停价格 
  char            szStkLwlmtPrice[21 + 1];    // 跌停价格 
  LONGLONG        llStkUplmtQty;              // 数量上限 
  LONGLONG        llStkLwlmtQty;              // 数量下限 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            chStkLotFlag;               // 单位标志 0:股或张为单位 1:手为单位
  LONGLONG        llStkSpread;                // 价差（价位） 
  char            chStkCalMktval;             // 市值标志 0:不计算市值 1:计算市值
  char            chStkSuspended;             // 停牌标志 0:正常 1:临时停牌 2:长期停牌
  char            szStkIsin[16 + 1];          // 国际编码 
  char            chStkSubCls;                // 证券子类别 
  char            szStkBizes[512 + 1];        // 证券业务 
  char            chStkCustodyMode;           // 托管模式 
  char            szStkUndlCode[8 + 1];       // 正股代码 
  int             iBuyUnit;                   // 买入最小单位 
  int             iSaleUnit;                  // 卖出最小单位 
  char            szBondInt[21 + 1];          // 债券利息 
  char            chStkLevel;                 // 证券级别 
  int             iStkDeadline;               // 交易截止日期 
  char            szRemindMessage[128 + 1];   // 退市提示信息 
};

//-------------------------------10102515:期权客户登录--------------------------
struct MATRADEAPI CReqOptRpcUserLoginField
{
  char            szAcctType[2 + 1];          // 账户类型 U:客户代码 Z:资产账户 05:深A证券账户 15:沪A证券账户
  char            szAcctId[32 + 1];           // 账户标识 
  char            chUseScope;                 // 使用范围 ‘4’:期权交易密码 ‘5’:期权资金密码
  char            chAuthType;                 // 认证类型 ‘0’--密码
  char            szAuthData[256 + 1];        // 认证数据 明文
  char            chEncryptType;              // 加密方式 加密方式: ‘0’:金证KBSS系统加密方式 ‘1’:金证Win版集中交易加密方式 ‘2’:金证Unix版集中交易加密方式 ‘3’:外部重加密方式 ‘4’:外部加密方式 说明：该入参根据接入券商要求填写，默认值为0。 外部重加密方式’3’：使用非金证加密函数将密码明文加密为密文后，然后使用金证公司提供的通信加密函数kbss_comencrypt对密文再一次加密。 外部加密方式’4’： 使用非金证加密函数将密码明文加密为密文后作为入参。
  char            szEncryptKey[32 + 1];       // 加密因子 加密因子可以填写为账户标识：ACCT_ID参数值
  char            chChannel;                  // 操作渠道
  char            szSerChannel[8 + 1];        // 服务渠道
};

struct MATRADEAPI CRspOptRpcUserLoginField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 交易账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  int             iIntOrg;                    // 内部机构 
  char            chTrdacctStatus;            // 账户状态 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szAcctType[2 + 1];          // 登陆账户类型 U:客户代码 Z:资产账户 05:深A证券账户 15:沪A证券账户
  char            szAcctId[32 + 1];           // 登陆账户标识 
  char            chTrdacctExcls;             // 交易账户类别 
  char            szTrdacctName[128 + 1];     // 交易账户名称 
  char            szSessionId[256 + 1];       // 会话凭证 
  int             iSubsysSn;                  // 交易节点编号 期权订单交易结点编号，以支持多节点订单系统
  char            chPwdMdfFlag;               // 密码修改标识 0-不修改 1-修改 开户初始化密码后，首次登陆返回1； 重置密码后，首次登陆返回1； 密码修改后，登陆返回0。
  char            szLastLoginTime[23 + 1];    // 最后登录时间 
  char            szLastLoginSite[32 + 1];    // 最后登录站点 
  char            chLastLoginChnl;            // 最后登录渠道 
  int             iFlashPort;                 // 软加速端口号
  char            chFlashFlag;                // 加速启用标志 '0'-未启用 '1'-已启用
  char            szFlashStkBizes[768 + 1];   // 软加速业务
};

//-------------------------------10102516:快速订单资金划拨--------------------------
struct MATRADEAPI CReqOptRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码\资产账户至少要传一个
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 0-快速订单入金 1-快速订单出金
  char            szTransAmt[21 + 1];         // 划拨金额 不区分正负号
  char            szRemark[256 + 1];          // 备注信息 
};

struct MATRADEAPI CRspOptRpcFundingTransferField
{
  LONGLONG        llSerialNo;                 // 流水号 
};

//-------------------------------10102517:快速订单可划入资金查询--------------------------
struct MATRADEAPI CReqOptQryRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码\资产账户至少要传一个
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
};

struct MATRADEAPI CRspOptQryRpcFundingTransferField
{
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szTransAmt[21 + 1];         // 可划拨金额 
};

//-------------------------------10102518:快速订单资金划拨流水查询--------------------------
struct MATRADEAPI CReqOptQryRpcFundingTransferFlowField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码\资产账户至少要传一个
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            chDirect;                   // 划拨方向 
  char            chTransStatus;              // 划拨状态 
  char            chCancelStatus;             // 冲销状态 
  LONGLONG        llQryPos;                   // 定位串 
  LONGLONG        llQryNum;                   // 查询行数 
};

struct MATRADEAPI CRspOptQryRpcFundingTransferFlowField
{
  LONGLONG        llQryPos;                   // 定位串 
  int             iSettDate;                  // 清算日期 
  LONGLONG        llSerialNo;                 // 流水序号 
  int             iOccurDate;                 // 发生日期 
  char            szOccurTime[32 + 1];        // 发生时间 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szBizAmt[21 + 1];           // 业务金额 
  char            chDirect;                   // 划拨方向 
  char            chTransStatus;              // 划拨状态 
  int             iIntSubsysSn;               // 内部调用子系统编码 
  int             iIntMsgCode;                // 内部调用信息代码 
  char            szIntMsgText[256 + 1];      // 内部调用信息内容 
  LONGLONG        llIntSerialNo;              // 内部调用流水序号 
  int             iIntReqStime;               // 内部调用请求时间 
  int             iIntAnsStime;               // 内部调用应答时间 
  int             iExtMsgCode;                // 外部调用信息代码 
  char            szExtMsgText[256 + 1];      // 外部调用信息内容 
  LONGLONG        llExtSerialNo;              // 外部调用流水序号 
  int             iExtReqStime;               // 外部调用请求时间 
  int             iExtAnsStime;               // 外部调用应答时间 
  char            chCancelStatus;             // 冲销状态 
  LONGLONG        llOpUser;                   // 操作用户 
  char            chOpRole;                   // 操作用户角色 
  char            szOpName[32 + 1];           // 操作用户姓名 
  char            szRemark[256 + 1];          // 备注信息 
};

//-------------------------------10313018:临近到期合约资产查询--------------------------
struct MATRADEAPI CReqOptQryExpendableCuOfCloseToExpireDateField
{
  LONGLONG        llCustCode;                 // 客户代码 两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            chOptSide;                  // 持仓方向 L-权力仓，S-义务仓，C-备兑策略持仓
  char            chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
  int             iDueCloseDays;              // 距离到期天数 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryExpendableCuOfCloseToExpireDateField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  int             iIntOrg;                    // 内部机构 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            chCurrency;                 // 货币代码 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptCode[40 + 1];          // 合约代码 
  char            szOptName[120 + 1];         // 合约简称 
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
  LONGLONG        llOptRealPosi;              // 合约实际持仓 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szOptUndlName[32 + 1];      // 标的证券名称 
  char            chOptUndlCls;               // 标的证券类别 字典[STK_CLS]
  int             iExpireDate;                // 期权到期日 
  int             iExerciseDate;              // 期权行权日 
  int             iRealDueDays;               // 实际距离到期日天数 
};

//-------------------------------10102519:股票期权行权指派明细查询--------------------------
struct MATRADEAPI CReqOptQryRpcExeDetailField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户两项不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chOptType;                  // 合约类型 字典[OPT_TYPE]
  char            chOptCvdFlag;               // 备兑标志 0-非备兑合约 1-备兑合约
  char            chExecSide;                 // 行权方向 ‘B’-行权方  ‘S’-被行权方
  LONGLONG        llQryPos;                   // 定位串 
  int             iQryNum;                    // 查询行数 
  char            chCombFlag;                 // 组合行权标志 0:'非组合行权';1:'是组合行权'
};

struct MATRADEAPI CRspOptQryRpcExeDetailField
{
  LONGLONG        llQryPos;                   // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szUserName[32 + 1];         // 客户姓名 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iIntOrg;                    // 内部机构 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptName[120 + 1];         // 合约简称 
  char            chOptType;                  // 合约类型 字典[OPT_TYPE]
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chOptCvdFlag;               // 备兑标志 
  LONGLONG        llOptEffect;                // 指派合约数量(张) 
  LONGLONG        llStkEffect;                // 标的证券收付数量 
  char            szFundEffect[21 + 1];       // 收付金额 行权价*指派数量
  char            szExecPrice[21 + 1];        // 行权价 
  char            chExecSide;                 // 行权方向 ‘B’-行权方  ‘S’-被行权方
  int             iComeInDate;                // 过户日期 
  char            szFundExeMargin[21 + 1];    // 行权锁定保证金 
  char            szClearingFee[21 + 1];      // 行权结算费 
  char            szCommision[21 + 1];        // 手续费 
  char            szFundCombExeUfz[21 + 1];   // 组合行权解冻资金 
  LONGLONG        llCombQty;                  // 组合行权数量 
};

//-------------------------------10102520:股票期权当日风险通知查询--------------------------
struct MATRADEAPI CReqOptQryRpcCurrDayRiskNotifyField
{
  char            szRiskIds[512 + 1];         // 风险标识(复数) 
  LONGLONG        llNoticeBsn;                // 通知批号 
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  LONGLONG        llQryPos;                   // 定位串 1表示首页；从第二页开始（包括第二页），此值为上一页的最后一条记录中的“定位串”字段的值加1。
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryRpcCurrDayRiskNotifyField
{
  LONGLONG        llRecNum;                   // 记录序号 
  LONGLONG        llNoticeSn;                 // 通知序号 
  LONGLONG        llNoticeBsn;                // 通知批号 
  char            szRiskId[2 + 1];            // 风险标识 
  char            szSchemaId[4 + 1];          // 预案编号 
  int             iCreateDate;                // 创建日期 
  int             iCreateTime;                // 创建时间 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 交易账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szUserName[32 + 1];         // 客户姓名 
  char            szMobile[16 + 1];           // 移动电话 
  char            szEmail[254 + 1];           // 电子信箱 
  char            szTel[16 + 1];              // 联系电话 
  char            szContent[2048 + 1];        // 通知内容 
  char            chNoticeChnl;               // 通知渠道 短信 电子邮件 电话通知 系统通知
  char            chSendType;                 // 发送类型 0逐日盯市 1手工触发 2收盘触发
  int             iSendDate;                  // 发送日期 
  int             iSendTime;                  // 发送时间 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  char            szRespCont[256 + 1];        // 反馈内容 
  int             iRespDate;                  // 反馈日期 
  int             iRespTime;                  // 反馈时间 
};

//-------------------------------10102521:股票期权历史风险通知查询--------------------------
struct MATRADEAPI CReqOptQryRpcHisRiskNotifyField
{
  char            szRiskIds[512 + 1];         // 风险标识(复数) 
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llNoticeBsn;                // 通知批号 
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  LONGLONG        llQryPos;                   // 定位串 1表示首页；从第二页开始（包括第二页），此值为上一页的最后一条记录中的“定位串”字段的值加1。
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryRpcHisRiskNotifyField
{
  LONGLONG        llRecNum;                   // 记录序号 
  LONGLONG        llNoticeSn;                 // 通知序号 
  LONGLONG        llNoticeBsn;                // 通知批号 
  char            szRiskId[2 + 1];            // 风险标识 
  char            szSchemaId[4 + 1];          // 预案编号 
  int             iCreateDate;                // 创建日期 
  int             iCreateTime;                // 创建时间 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 交易账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szContent[2048 + 1];        // 通知内容 
  char            chNoticeChnl;               // 通知渠道 短信 电子邮件 电话通知 系统通知
  char            chSendType;                 // 发送类型 0逐日盯市 1手工触发 2收盘触发
  int             iSendDate;                  // 发送日期 
  int             iSendTime;                  // 发送时间 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  char            szRespCont[256 + 1];        // 反馈内容 
  int             iRespDate;                  // 反馈日期 
  int             iRespTime;                  // 反馈时间 
};

//-------------------------------10102522:股票期权当日风险通知查询（风控系统专用）--------------------------
struct MATRADEAPI CReqOptQryRpcCurrDayRiskSysNotifyField
{
  LONGLONG        llQryPos;                   // 定位串 1表示首页；从第二页开始（包括第二页），此值为上一页的最后一条记录中的“定位串”字段的值加1。
  int             iQryNum;                    // 查询行数 
  char            szNoticeId[512 + 1];        // 风险标识(复数) 字典[NOTICE_REASON]
  LONGLONG        llNoticeBsn;                // 通知批号 
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  char            chRespFlag;                 // 反馈标志 0未反馈 1已反馈
};

struct MATRADEAPI CRspOptQryRpcCurrDayRiskSysNotifyField
{
  LONGLONG        llRecNum;                   // 记录序号 
  LONGLONG        llNoticeSn;                 // 通知序号 
  LONGLONG        llNoticeBsn;                // 通知批号 
  char            szNoticeId[4 + 1];          // 风险标识 
  char            szSchemaId[6 + 1];          // 预案编号 
  int             iCreateDate;                // 创建日期 
  char            szCreateTime[32 + 1];       // 创建时间 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 交易账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szUserName[32 + 1];         // 客户姓名 
  char            szMobile[32 + 1];           // 移动电话 
  char            szEmail[254 + 1];           // 电子信箱 
  char            szTel[32 + 1];              // 联系电话 
  char            szContent[2048 + 1];        // 通知内容 
  char            szNoticeChnl[16 + 1];       // 通知渠道 短信 电子邮件 电话通知 系统通知
  char            chSendType;                 // 发送类型 0逐日盯市 1手工触发 2收盘触发
  int             iSendDate;                  // 发送日期 
  char            szSendTime[32 + 1];         // 发送时间 
  char            chRespFlag;                 // 反馈标志 0未反馈 1已反馈
};

//-------------------------------10102523:股票期权历史风险通知查询（风控系统专用）--------------------------
struct MATRADEAPI CReqOptQryRpcHisRiskSysNotifyField
{
  LONGLONG        llQryPos;                   // 定位串 1表示首页；从第二页开始（包括第二页），此值为上一页的最后一条记录中的“定位串”字段的值加1。
  int             iQryNum;                    // 查询行数 
  char            szNoticeId[512 + 1];        // 风险标识(复数) 字典[NOTICE_REASON]
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llNoticeBsn;                // 通知批号 
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chReadFlag;                 // 阅读标志 0未阅读 1已阅读
  char            chRespFlag;                 // 反馈标志 0未反馈 1已反馈
};

struct MATRADEAPI CRspOptQryRpcHisRiskSysNotifyField
{
  LONGLONG        llRecNum;                   // 记录序号 
  LONGLONG        llNoticeSn;                 // 通知序号 
  LONGLONG        llNoticeBsn;                // 通知批号 
  char            szNoticeId[4 + 1];          // 风险标识 
  char            szSchemaId[6 + 1];          // 预案编号 
  int             iCreateDate;                // 创建日期 
  char            szCreateTime[32 + 1];       // 创建时间 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSubacctCode[8 + 1];       // 交易账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            szContent[2048 + 1];        // 通知内容 
  char            szNoticeChnl[16 + 1];       // 通知渠道 短信 电子邮件 电话通知 系统通知
  char            chSendType;                 // 发送类型 0逐日盯市 1手工触发 2收盘触发
  int             iSendDate;                  // 发送日期 
  char            szSendTime[32 + 1];         // 发送时间 
  char            chRespFlag;                 // 反馈标志 0未反馈 1已反馈
};

//-------------------------------10314032:客户限仓额度查询--------------------------
struct MATRADEAPI CReqOptQryPositionLimitField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码与资产账户不能同时为空
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  char            chQueryFlag;                // 查询方向 0:向后取数据 1:向前取数据 其他全部返回
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspOptQryPositionLimitField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szCustName[32 + 1];         // 客户姓名 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szSubacctCode[8 + 1];       // 证券账户子编码 
  char            szOptTrdacct[28 + 1];       // 期权合约账户 
  char            chOptUndlCls;               // 标的证券类别 证券类别[STK_CLS]
  char            szOptUndlCode[8 + 1];       // 标的证券代码 
  LONGLONG        llTotalPosiLmt;             // 总持仓限额 
  LONGLONG        llPosiLmtLong;              // 权利仓持仓限额 
  LONGLONG        llDailyOpenLmt;             // 单日开仓限额 0表示不限制
  LONGLONG        llDailyBuyOpenLmt;          // 单日买入开仓限额 0表示不限制
  char            szOptUndlName[80 + 1];      // 标的证券名称 
  int             iIntOrg;                    // 机构 适配恒生外围接口
  LONGLONG        llLongPosiAvl;              // 可用权利仓限额 适配恒生外围接口
  LONGLONG        llDailyBuyOpenPosiAvl;      // 可用当日累计买入开仓限额 适配恒生外围接口
  LONGLONG        llDailyOpenPosiAvl;         // 可用当日开仓限额 适配恒生外围接口
  LONGLONG        llTotalPosiAvl;             // 可用总持仓限额 适配恒生外围接口
};

//////////////////////////////////////////////////////////////////////////
//-------------------------------10312041:期权委托申报(极速)-------------------------
struct MATRADEAPI CReqOptOrderFlashField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szOptNum[16 + 1];           // 合约编码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  short           siStkBiz;                   // 证券业务 字典[STK_BIZ]
  short           siStkBizAction;             // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // 交易单元(未启用)   
  int             iOrderBsn;                  // 委托批号
  char            chEnforceFlag;              // 强制平仓标志
  char            chOrderType;                // 委托类型
  char            szClientInfo[256 + 1];      // 终端信息(未启用)
};

struct MATRADEAPI CRspOptOrderFlashField
{
  char            szOrderId[10 + 1];          // 合同序号 
  int             iOrderBsn;                  // 委托批号 
};

//-------------------------------10312042:期权委托撤单(极速)------------------------------------
struct MATRADEAPI CReqOptCancelOrderFlashField
{
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szStkbd[2 + 1];               // 交易板块
  char          szOrderId[10 + 1];            // 合同序号
  int           iOrderBsn;                    // 委托批号(未启用)  
  char          chForceWth;                   // 强撤标志
  char          szRiskId[2 + 1];              // 风险标识(强制平仓时必送)
  char          szClientInfo[256 + 1];        // 终端信息(未启用)
  short         siStkBiz;                     // 证券业务
};

struct MATRADEAPI CRspOptCancelOrderFlashField
{
  char          szStkbd[2 + 1];               // 交易板块  
  LONGLONG      llCuacctCode;                 // 资产账户
  char          szOrderId[10 + 1];            // 合同序号 原委托合同序号
  char          szOrderIdWtd[10 + 1];         // 委托合同号 撤单委托合同序号  
  int           iOrderBsn;                    // 委托批号(未启用)
  char          chCancelStatus;               // 内部撤单标志(未启用)  
  short         siStkBiz;                     // 证券业务
};

//-------------------------------10312044:成交回报推送(极速）------------------------------------
struct MATRADEAPI CRtnOptOrderFillFlashField
{
  LONGLONG      llCuacctCode;               // 资产账户
  int           iMatchedDate;               // 成交日期
  char          szStkbd[2 + 1];             // 交易板块
  char          szOrderId[10 + 1];          // 合同序号
  char          szMatchedSn[32 + 1];        // 成交编号
  char          szFundAvl[21 + 1];          // 资金可用金额（成交后）
  LONGLONG      llOptAvl;                   // 合约可用数量（成交后）
  char          chOrderStatus;              // 委托状态
  LONGLONG      llMatchedQty;               // 本次成交数量
  char          szMatchedPrice[11 + 1];     // 本次成交价格
  char          szOptCode[16 + 1];          // 证券代码
  char          chIsWithdraw;               // 撤单标志
};

//-------------------------------10312045:确认回报推送（极速）------------------------------------
struct MATRADEAPI CRtnOptOrderConfirmFlashField
{
  LONGLONG      llCuacctCode;               // 资产账户
  int           iOrderDate;                 // 委托日期
  char          szOptCode[16 + 1];          // 合约代码
  char          szStkbd[2 + 1];             // 交易板块
  char          szOrderId[10 + 1];          // 合同序号
  char          chIsWithdraw;               // 撤单标志
  char          chOrderStatus;              // 委托状态
};

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_OPT_TRADE_API_STRUCT_H__