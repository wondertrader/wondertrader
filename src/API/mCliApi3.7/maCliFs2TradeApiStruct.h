#if !defined(__MA_CLI_FS_TRADE_API_STRUCT_H__)
#define __MA_CLI_FS_TRADE_API_STRUCT_H__

#include "maCliFs2Api.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10308011:机构信息查询--------------------------
struct MATRADEAPI CReqFsQryOrgInfoField
{
  int             iIntOrg;                    // 内部机构 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryOrgInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iIntOrg;                    // 内部机构 
  char            szOrgName[32 + 1];          // 机构名称 
};

//-------------------------------10308013:证券信息查询--------------------------
struct MATRADEAPI CReqFsQryTrdInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryTrdInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            chStkStatus;                // 证券状态 字典[STK_STATUS]
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkUplmtRatio[21 + 1];    // 涨停比例 
  char            szStkLwlmtRatio[21 + 1];    // 跌停比例 
  char            szStkUplmtPrice[21 + 1];    // 涨停价格 
  char            szStkLwlmtPrice[21 + 1];    // 跌停价格 
  LONGLONG        llStkUplmtQty;              // 数量上限(限价) 
  LONGLONG        llStkLwlmtQty;              // 数量下限(限价) 
  LONGLONG        llStkLwlmtQtyMp;            // 数量下限(市价) 
  LONGLONG        llStkUplmtQtyMp;            // 数量上限(市价) 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            chStkLotFlag;               // 单位标志 
  LONGLONG        llStkSpread;                // 价位价差 
  char            chStkCalMktval;             // 市值标志 字典[STK_CAL_MKTVAL]
  char            chStkSuspended;             // 停牌标志 字典[STK_SUSPENDED]
  char            szStkIsin[16 + 1];          // 国际编码 
  char            chStkSubCls;                // 证券子类别 字典[STK_SUB_CLS]
  char            szStkBizes[512 + 1];        // 证券业务 
  char            chStkCustodyMode;           // 托管模式 字典[STK_CUSTODY_MODE]
  char            szStkUndlCode[8 + 1];       // 正股代码 
  char            szStkFaceVal[21 + 1];       // 证券面值 
  char            szBondInt[21 + 1];          // 债券利息 
  char            chStkLevel;                 // 证券级别 字典[STK_LEVEL]
};

//-------------------------------10308012:字典信息查询--------------------------
struct MATRADEAPI CReqFsQryDictInfoField
{
  char            szDdId[32 + 1];             // 字典标识 
  char            szDdItem[32 + 1];           // 字典项目 
};

struct MATRADEAPI CRspFsQryDictInfoField
{
  char            szDdName[32 + 1];           // 字典名称 
  char            szDdId[32 + 1];             // 字典标识 
  char            szDdItem[32 + 1];           // 字典项 
  char            szDdItemName[128 + 1];      // 字典项名称 
  int             iIntOrg;                    // 内部机构 
};

//-------------------------------10308014:系统状态查询--------------------------
struct MATRADEAPI CReqFsQrySysStatField
{
};

struct MATRADEAPI CRspFsQrySysStatField
{
  int             iTrdDate;                   // 交易日期 
  int             iSettDate;                  // 交收日期 
  char            chBizStatus;                // 系统状态 字典[BIZ_STATUS]
};

//-------------------------------10510605:银证转账--------------------------
struct MATRADEAPI CReqFsBankStkTransferField
{
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  LONGLONG        llCuacctCode;               // 资金账户 
  int             iExtOrg;                    // 银行代码 
  char            szFundAuthData[32 + 1];     // 资金密码 
  char            szBankAuthData[32 + 1];     // 银行密码 
  char            szBizAmt[21 + 1];           // 业务金额 
  char            szOpRemark[128 + 1];        // 操作备注 
  int             iIntOrg;                    // 机构代码 
  char            chEncryptType;              // 密码加密类型 0：AES再加密密文1：未再加密密文2：明文
  char            szBankCustCode[200 + 1];    // 银行客户号 
  char            chBanktrantype;             // 转账类型 1：银行转证券2：证券转银行
};

struct MATRADEAPI CRspFsBankStkTransferField
{
  char            szSerialNo[64 + 1];         // 流水序号 
};

//-------------------------------10510606:券商发起查银行余额--------------------------
struct MATRADEAPI CReqFsQryInitiatorOrderBankBalField
{
  LONGLONG        llCuacctCode;               // 资金账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iExtOrg;                    // 外部机构 
  char            szFundAuthData[32 + 1];     // 资金密码据 
  char            szBankAuthData[32 + 1];     // 银行密码 
  char            szOpRemark[128 + 1];        // 备注信息 
  char            chEncryptType;              // 密码加密类型 0：AES再加密密文1：未再加密密文2：明文
  int             iIntOrg;                    // 机构代码 
};

struct MATRADEAPI CRspFsQryInitiatorOrderBankBalField
{
  char            szSerialNo[64 + 1];         // 流水序号 
};

//-------------------------------10510608:查询银证业务流水--------------------------
struct MATRADEAPI CReqFsQryBankStkDetailField
{
  int             iOccurDate;                 // 发生日期 
  LONGLONG        llSerialNo;                 // 流水序号 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  LONGLONG        llCuacctCode;               // 资产帐户 
  int             iExtOrg;                    // 银行代码 
  LONGLONG        llCustCode;                 // 客户代码 
  int             iIntOrg;                    // 内部机构 
  char            chParamType;                // 出参类型 字典[PARAM_TYPE]
};

struct MATRADEAPI CRspFsQryBankStkDetailField
{
  LONGLONG        llSerialNo;                 // 流水序号 
  int             iOccurDate;                 // 发生日期 
  char            szOccurTime[32 + 1];        // 发生时间 
  int             iExtOrg;                    // 银行代码 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szCubsbTrdId[2 + 1];        // 银证业务标识 字典[CUBSB_TRD_ID]
  char            szBizAmt[21 + 1];           // 业务金额 
  char            szFudnBln[21 + 1];          // 资金当前余额 
  char            chCubsbTrdStatus;           // 银证业务状态 字典[CUBSB_TRD_STATUS]
  char            chSourceType;               // 发起方向 0－证券发起1－银行发起2－双边发起
  LONGLONG        llIntMsgCode;               // 内部信息代码 
  char            szIntMsgText[32 + 1];       // 内部错误信息 
  LONGLONG        llExtMsgCode;               // 外部信息代码 
  char            szExtMsgText[256 + 1];      // 外部信息内容 
  LONGLONG        llExtSerialNo;              // 外部流水号 
  char            szExtAnsStime[32 + 1];      // 成交时间 
  char            szRemark[32 + 1];           // 备注信息 
};

//-------------------------------10510620:客户内现金划转--------------------------
struct MATRADEAPI CReqFsCustCashTransferField
{
  int             iIntOrg;                    // 机构代码 
  LONGLONG        llOutfundid;                // 转出资金帐号 
  LONGLONG        llOutpassword;              // 转出资金密码 
  LONGLONG        llInfundid;                 // 转入资金帐号 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFundeffect[21 + 1];       // 转帐金额 
  char            chCheckpwdflag;             // 检查密码标志 0：不校验1：校验
  char            chCashcheck;                // 现金支票类型 0：现金1：支票
  char            chEncryptType;              // 密码加密类型 0：AES再加密密文1：未再加密密文2：明文
};

struct MATRADEAPI CRspFsCustCashTransferField
{
  LONGLONG        llOutSno;                   // 转出流水号 
  LONGLONG        llInSno;                    // 转入流水号 
};

//-------------------------------10510621:账户资金归集--------------------------
struct MATRADEAPI CReqFsQryAcctConcentrationField
{
  int             iOrgid;                     // 机构代码 
  LONGLONG        llCuacctCode;               // 资金账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            chCashcheck;                // 现金支票类型 0：现金1：支票
};

struct MATRADEAPI CRspFsQryAcctConcentrationField
{
  char            szFundeffect[21 + 1];       // 归集金额 
  int             iRenum;                     // 返回笔数 
};

//-------------------------------10510601:查询转帐银行帐户--------------------------
struct MATRADEAPI CReqFsQryTransferAcctField
{
  int             iExtOrg;                    // 银行代码 不送值查询全部
  char            chCurrency;                 // 货币代码 不送值查询全部
  LONGLONG        llCuacctCode;               // 资金账户 
};

struct MATRADEAPI CRspFsQryTransferAcctField
{
  LONGLONG        llCustCode;                 // 客户代码 
  int             iIntOrg;                    // 机构编码 
  int             iExtOrg;                    // 银行代码 
  char            szOrgName[32 + 1];          // 银行名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBankAcct[32 + 1];         // 银行账户 
  char            szBdmfAcct[32 + 1];         // 存管账户 
  LONGLONG        llCuacctCode;               // 资金账号 
};

//-------------------------------10510468:财富账户资金划拨--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctTransferField
{
  LONGLONG        llCuacctCode;               // 资金账户 
  LONGLONG        llFortuneCodeOut;           // 转出财富账户 
  LONGLONG        llFortuneCodeIn;            // 转入财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBizAmt[21 + 1];           // 调拨金额 正数
};

struct MATRADEAPI CRspFsQryFortuneAcctTransferField
{
  int             iOutSno;                    // 转出流水号 
  int             iInSno;                     // 转入流水号 
};

//-------------------------------10510473:财富账户划拨流水查询--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctTransferDetailField
{
  int             iBeginDate;                 // 开始日期 
  int             iEndDate;                   // 结束日期 
  LONGLONG        llCuacctCode;               // 资金账户 
  LONGLONG        llFortuneCode;              // 财富账户 -1表示查全部
  char            chDirection;                // 划拨方向 S：划出方(BIZ_CODE = 20040054)D：划入方(BIZ_CODE = 20040054)不送查全部
  LONGLONG        llSerialNo;                 // 流水号 不送查全部
  char            chQryflag;                  // 查询方向 0：向下查询1：向上查询
  int             iCount;                     // 请求行数 每次取的行数
  char            szQryPos[32 + 1];           // 定位串 第一次填空(查询返回的流水号)
};

struct MATRADEAPI CRspFsQryFortuneAcctTransferDetailField
{
  char            szQryPos[32 + 1];           // 定位串 查询返回定位值(最后一条记录的流水号)
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llSerialNo;                 // 流水号 
  char            chDirection;                // 划拨方向 字典[DIRECTION]
  int             iOperDate;                  // 操作日期 
  int             iOperTime;                  // 操作时间 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[16 + 1];         // 客户姓名 
  int             iIntOrg;                    // 机构代码 
  LONGLONG        llCuacctCode;               // 资金账号 
  LONGLONG        llFortuneCode;              // 财富账户 
  char            szFortuneAlias[32 + 1];     // 财富账户别名 
  LONGLONG        llOtherFortuneCode;         // 对方财富账户 
  char            szOtherFortuneAlias[32 + 1];// 对方财富账户别名 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFundeffect[21 + 1];       // 资金发生数 
  char            chStatus;                   // 划拨状态 0:进行中1:成功
};

//-------------------------------10301105:客户登录--------------------------
struct MATRADEAPI CReqFsQryCustLoginField
{
  char            szAcctType[2 + 1];          // 账户类型 U:客户代码 Z:资产账户 00:深A股东 01:深B股东 02:三板股东 10:沪A股东 11:沪B股东
  char            szAcctId[32 + 1];           // 账户标识 
  char            chUseScope;                 // 使用范围 字典[USE_SCOPE]
  char            szEncryptKey[32 + 1];       // 加密因子 
  char            chAuthType;                 // 认证类型 字典[AUTH_TYPE]
  char            szAuthData[256 + 1];        // 认证数据 
  char            szThirdParty[1024 + 1];     // 第三方 
};

struct MATRADEAPI CRspFsQryCustLoginField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkTrdacct[20 + 1];       // 证券账户 
  int             iTrdacctSn;                 // 账户序号 
  char            szTrdacctExid[20 + 1];      // 报盘账户 
  char            chTrdacctStatus;            // 账户状态 
  char            chTregStatus;               // 指定状态 
  char            chBregStatus;               // 回购状态 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szAcctType[2 + 1];          // 账户类型 U:客户代码 Z:资产账户 00:深A股东 01:深B股东 02:三板股东 10:沪A股东 11:沪B股东
  char            szAcctId[32 + 1];           // 账户标识 
  char            szTrdacctName[32 + 1];      // 交易账户名称 
  char            szChannelId[2 + 1];         // 通道号 
  char            szSessionId[128 + 1];       // 会话凭证 
  int             iIntOrg;                    // 内部机构 
  char            chCuacctAttr;               // 资产账户属性 字典[CUACCT_ATTR]
  int             iSubsysSn;                  // 远程子系统编号 指资产管理对应的系统编码 对接集中交易系统时是集中交易系统的节点编码 对接资产托管系统时,指的是簿记托管系统
  char            szThirdParty[1024 + 1];     // 第三方 
};

//-------------------------------10308112:资产账户查询--------------------------
struct MATRADEAPI CReqFsQryAssetsAcctField
{
  LONGLONG        llUserCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
};

struct MATRADEAPI CRspFsQryAssetsAcctField
{
  LONGLONG        llUserCode;                 // 用户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCuacctAttr;               // 资产账户属性 字典[CUACCT_ATTR]
  char            szCuacctCls[4 + 1];         // 资产账户类别 
  char            szCuacctLvl[4 + 1];         // 资产账户级别 
  char            szCuacctGrp[4 + 1];         // 资产账户组别 
  char            chCuacctStatus;             // 资产账户状态 字典[CUACCT_STATUS]
  char            chMainFlag;                 // 主账户标识 字典[TRDACCT_MAIN_FLAG]
  int             iIntOrg;                    // 内部机构 
  int             iOpenDate;                  // 开户日期 
  int             iCloseDate;                 // 销户日期 
};

//-------------------------------10308113:财富账户查询--------------------------
struct MATRADEAPI CReqFsQryFortuneAcctField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
};

struct MATRADEAPI CRspFsQryFortuneAcctField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chFortuneStatus;            // 财富账户状态 字典[CUACCT_STATUS]
  char            szFortuneAlias[32 + 1];     // 财富账户别名 
  int             iOpenDate;                  // 开户日期 
  int             iCloseDate;                 // 销户日期 
};

//-------------------------------10308001:股东查询--------------------------
struct MATRADEAPI CReqFsQrySecuField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
};

struct MATRADEAPI CRspFsQrySecuField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iIntOrg;                    // 内部机构 
  char            chTrdacctStatus;            // 账户状态 字典[TRDACCT_STATUS]
  char            chTregStatus;               // 指定状态 字典[TREG_STATUS]
  char            szCustName[8 + 1];          // 客户名称 
  char            chTrdacctExcls;             // 交易账户类别 字典[TRDACCT_EXCLS]
  char            chTrdacctMainFlag;          // 主交易账户标志 字典[TRDACCT_MAIN_FLAG]
  char            chLimitFlag;                // 受限股东标志 字典[LIMIT_FLAG]
};

//-------------------------------10308010:股东交易协议查询--------------------------
struct MATRADEAPI CReqFsQrySecuTransferAgreementField
{
  LONGLONG        llCustCode;                 // 客户代码 
};

struct MATRADEAPI CRspFsQrySecuTransferAgreementField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szCustAgmtType[128 + 1];    // 协议类型 字典[CUST_AGMT_TYPE]
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iEftDate;                   // 生效日期 
  int             iExpDate;                   // 失效日期 
};

//-------------------------------10308611:适当性匹配检查--------------------------
struct MATRADEAPI CReqFsPropriatenceCheckField
{
  LONGLONG        llCustCode;                 // 客户代码 CUST_CODE和CUACCT_CODE不能同时为空
  LONGLONG        llCuacctCode;               // 资产账号 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
};

struct MATRADEAPI CRspFsPropriatenceCheckField
{
  LONGLONG        llPropRiskNo;               // 适当性流水号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  char            chCheckResult;              // 适当性匹配结果 0 校验通过1 不匹配2 未测评3 客户风险等级已过期4 专业投资者已过期
  char            chCustRiskLevel;            // 客户风险等级 字典[CUST_RISK_LEVEL]
  char            chCustInvestLmt;            // 客户投资期限 字典[INVEST_LMT]
  char            szCustInvestPro[128 + 1];   // 客户投资品种 字典[INVEST_PRO]
  char            chCustExpectIncome;         // 客户预期收益 字典[EXINCOME_TYPE]
  char            chStkRiskLevel;             // 产品风险等级 字典[STK_RISK_LEVEL]
  char            chStkInvestPeriod;          // 投资期限 字典[INVEST_LMT]
  char            szStkInvestType[2 + 1];     // 投资品种 字典[INVEST_PRO]
  char            chStkExincomeType;          // 预期收益 字典[EXINCOME_TYPE]
  char            chRiskLevelFitFlag;         // 风险等级匹配结果 字典[FIT_FLAG]
  char            chInvestPeriodFitFlag;      // 投资期限匹配结果 字典[FIT_FLAG]
  char            chInvestTypeFitFlag;        // 投资品种匹配结果 字典[FIT_FLAG]
  char            chExincomeTypeFitFlag;      // 预期收益匹配结果 字典[FIT_FLAG]
  char            szCheckDesc[512 + 1];       // 校验结果描述 
  char            chCustLastriskFlag;         // 最低客户风险类别标示 字典[CUST_LASTRISK_FLAG]
  int             iCustRiskLevelDate;         // 风险等级有效期 
};

//-------------------------------10308068:客户适当性查询--------------------------
struct MATRADEAPI CReqFsQryCustPropriatenceCheckField
{
  LONGLONG        llCuacctCode;               // 资产账号 
};

struct MATRADEAPI CRspFsQryCustPropriatenceCheckField
{
  LONGLONG        llCustCode;                 // 客户代码 
  char            chProfInvestorType;         // 专业投资者分类 字典[PROF_INVESTOR_TYPE]
  int             iProfExpDate;               // 专业投资者到期日期 
  char            chInvestLmt;                // 投资期限 字典[INVEST_LMT]
  char            szInvestPro[32 + 1];        // 投资品种 字典[INVEST_PRO]
  char            chExpectIncome;             // 预期收益 字典[EXINCOME_TYPE]
  char            chCustLastriskFlag;         // 最低客户风险类别标示 字典[CUST_LASTRISK_FLAG]
  char            chCustRiskLevel;            // 客户风险等级 字典[CUST_RISK_LEVEL]
  int             iCustRiskLevelDate;         // 风险等级有效期 
};

//-------------------------------10308069:产品适当性查询--------------------------
struct MATRADEAPI CReqFsQryProdPropriatenceCheckField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
};

struct MATRADEAPI CRspFsQryProdPropriatenceCheckField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkRiskLevel;             // 产品风险等级 字典[STK_RISK_LEVEL]
  char            chInvestPeriod;             // 投资期限 字典[INVEST_LMT]
  char            szInvestType[2 + 1];        // 投资品种 字典[INVEST_PRO]
  char            chExincomeType;             // 预期收益 字典[EXINCOME_TYPE]
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
};

//-------------------------------10308048:资金查询--------------------------
struct MATRADEAPI CReqFsQryFundField
{
  LONGLONG        llCuacctCode;               // 资产账户 资产账户和客户代码不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  LONGLONG        llCustCode;                 // 客户代码 
};

struct MATRADEAPI CRspFsQryFundField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFundValue[21 + 1];        // 资金资产 
  char            szFundPrebln[21 + 1];       // 资金昨日余额 
  char            szFundBln[21 + 1];          // 资金余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            szLwlmtFund[21 + 1];        // 资金下限 
  char            chFundStatus;               // 资金状态 
  char            chCuacctAttr;               // 资产账户属性 字典[CUACCT_ATTR]
  char            szFundAle[21 + 1];          // 资金可取 
  char            szFundTrdUfz[21 + 1];       // 资金交易解冻金额 
  char            szFundBorrowBln[21 + 1];    // 资金内部拆借金额 
  int             iIntOrg;                    // 内部机构 
  char            szFundFrz[21 + 1];          // 资金冻结金额 
  char            szFundTrdFrz[21 + 1];       // 资金交易冻结金额 
  char            szFundTrdOtd[21 + 1];       // 资金交易在途金额 
  int             iExtOrg;                    // 外部机构 
};

//-------------------------------10308002:总资产查询--------------------------
struct MATRADEAPI CReqFsQryTotalAssetsField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFortuneCode[20 + 1];      // 财富账户 
};

struct MATRADEAPI CRspFsQryTotalAssetsField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iIntOrg;                    // 内部机构 
  char            szMarketValue[21 + 1];      // 资产总值 
  char            szFundValue[21 + 1];        // 资金资产 
  char            szStkValue[21 + 1];         // 市值 
  char            szFundPrebln[21 + 1];       // 资金昨日余额 
  char            szFundBln[21 + 1];          // 资金余额 
  char            szFundAvl[21 + 1];          // 资金可用金额 
  char            chFundStatus;               // 资金状态 
  char            chCuacctAttr;               // 资产账户属性 字典[CUACCT_ATTR]
  char            szIncome[21 + 1];           // 总盈亏 
  char            szFortuneCode[20 + 1];      // 财富账户 
};

//-------------------------------10308003:股份资产查询--------------------------
struct MATRADEAPI CReqFsQryShareAssetsField
{
  LONGLONG        llCuacctCode;               // 资产账户 资产账户和客户代码不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 支持多选，格式：值1，值2，值3；例如：00,01,03,10,11
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
  LONGLONG        llCustCode;                 // 客户代码 
};

struct MATRADEAPI CRspFsQryShareAssetsField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szStkpbu[8 + 1];            // 交易单元 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  LONGLONG        llStkPrebln;                // 证券昨日余额 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkAvl;                   // 证券可用数量 
  LONGLONG        llStkQty;                   // 当前拥股数 
  char            szCurrentPrice[21 + 1];     // 最新价格 
  char            szStkPcostRlt[21 + 1];      // 持仓成本 
  char            szMktVal[21 + 1];           // 市值 
  char            szCostPrice[21 + 1];        // 成本价格 
  char            szIncome[21 + 1];           // 盈亏 
  char            szProfitRate[21 + 1];       // 盈亏比例 
  LONGLONG        llStkTrdFrz;                // 证券交易冻结数量 
  LONGLONG        llStkTrdUfz;                // 证券交易解冻数量 
  char            szAveragePrice[21 + 1];     // 买入均价 
  char            chStkCls;                   // 证券类别 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llStkSale;                  // 卖出冻结数量 
  LONGLONG        llStkTrdOtd;                // 证券交易在途数量 
  LONGLONG        llStkFrz;                   // 证券冻结数量 
};

//-------------------------------10308004:当日委托查询--------------------------
struct MATRADEAPI CReqFsQryCurrDayOrderField
{
  LONGLONG        llCuacctCode;               // 资产账户 资产账户和客户代码不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chFlag;                     // 查询标志 
  char            szStkBiz[1024 + 1];         // 业务过滤 字典[STK_BIZ]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
  LONGLONG        llCustCode;                 // 客户代码 
  char            chOrderMode;                // 委托模式 
};

struct MATRADEAPI CRspFsQryCurrDayOrderField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
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
  char            szMatchedAmt[21 + 1];       // 成交金额 
  char            szOfferRetMsg[64 + 1];      // 申报返回信息 
  char            szReferRate[21 + 1];        // 参考汇率 
  char            szAfundAmt[21 + 1];         // A股资金变动金额 
  char            szHfundAmt[21 + 1];         // 港股资金变动金额 
  char            szBizParam[512 + 1];        // 业务参数 
  char            szCustName[32 + 1];         // 客户姓名 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  char            chOrderValidFlag;           // 委托有效标志 
};

//-------------------------------10308006:当日成交查询--------------------------
struct MATRADEAPI CReqFsQryCurrDayMatchField
{
  LONGLONG        llCuacctCode;               // 资产账户 资产账户和客户代码不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chFlag;                     // 查询标志 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
  LONGLONG        llCustCode;                 // 客户代码 
};

struct MATRADEAPI CRspFsQryCurrDayMatchField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szMatchedTime[8 + 1];       // 成交时间 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szMatchedSn[16 + 1];        // 成交编号 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chMatchedType;              // 成交类型 字典[MARCHED_TYPE]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
};

//-------------------------------10308053:当日成交统计--------------------------
struct MATRADEAPI CReqFsQryCurrDayMatchCountField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[32 + 1];            // 交易板块 支持多选，格式：值1，值2，值3；例如：00,01,03,10,11
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryCurrDayMatchCountField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkName[16 + 1];          // 证券名称 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  LONGLONG        llMatchedQty;               // 成交数量 
  char            szMatchedAmt[21 + 1];       // 成交金额 
  char            chMatchedType;              // 成交类型 字典[MATCHED_TYPE]
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
};

//-------------------------------10308056:最大可交易数量计算--------------------------
struct MATRADEAPI CReqFsQryMaxLimitTransferCountField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szOrderPrice[21 + 1];       // 委托价格 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkbdLink[2 + 1];         // 关联板块 
  char            szTrdacctLink[20 + 1];      // 关联股东 
  int             iTrdDate;                   // 交易日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szStkpbu[8 + 1];            // 交易席位 
  char            szStkprop[2 + 1];           // 股份性质 字典[STKPROP]
  char            szSdStkType[2 + 1];         // 上登证券类别 字典[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // 权益类型 字典[EQUIT_TYPE]
  char            chCirculType;               // 流通类型 字典[CIRCUL_TYPE]
  char            szProdcode[12 + 1];         // 产品编码 
  char            chStkBizCtvFlag;            // 业务转换标志 
  char            szOrderText[256 + 1];       // 委托扩展 
};

struct MATRADEAPI CRspFsQryMaxLimitTransferCountField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llStkQty;                   // 成分股数量 
  LONGLONG        llMaxOrerQty;               // 最大可委托数量 
};

//-------------------------------10308601:证券交易买卖委托申报--------------------------
struct MATRADEAPI CReqFsTrdTransferOrderField
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
  int             iOrderBsn;                  // 委托批号 
  char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
  char            chSecurityLevel;            // 安全手段 0:无安全1:校验票据2:校验密码
  char            szSecurityInfo[256 + 1];    // 安全信息 安全手段为校验票据则需填写票据安全手段为密码则需填写交易密码密文
  char            szOrderText[256 + 1];       // 委托扩展 该字段需根据业务场景填写格式如下：Key1:Value1,key2:Value2,key3:Value3,...
  char            chStkBizCtvFlag;            // 业务转换标志 
  char            szFortuneCode[20 + 1];      // 财富账户 
};

struct MATRADEAPI CRspFsTrdTransferOrderField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  int             iOrderSn;                   // 委托序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  char            szFortuneCode[20 + 1];      // 财富账户 
};

//-------------------------------10308607:证券交易委托撤单--------------------------
struct MATRADEAPI CReqFsTrdTransferPurchaseCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
};

struct MATRADEAPI CRspFsTrdTransferPurchaseCancelField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
};

//-------------------------------10308054:可撤单委托查询--------------------------
struct MATRADEAPI CReqFsQryAvlTrdTransferPurchaseCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 支持多选，格式：值1，值2，值3；例如：00,01,03,10,11
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkBiz[1024 + 1];         // 证券业务 该字段用于业务过滤支持多选，格式：值1，值2，值3；例如：100,101,206
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryAvlTrdTransferPurchaseCancelField
{
  char            szQryPos[32 + 1];           // 定位串查询 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szOrderId[10 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
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
  int             iOfferStime;                // 申报时间 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chIsWithdrawn;              // 已撤单标志 字典[IS_WITHDRAWN]
  int             iOrderBsn;                  // 委托批号 
  char            szMatchedAmt[21 + 1];       // 成交金额 
};

//-------------------------------10308606:证券交易批量撤单--------------------------
struct MATRADEAPI CReqFsTrdQuantityCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iOrderBsn;                  // 委托批号 
};

struct MATRADEAPI CRspFsTrdQuantityCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  int             iCancelRet;                 // 撤单结果 返回值0：表示成功-1：表示失败
  char            szRetInfo[256 + 1];         // 返回信息 返回委托结果信息
};

//-------------------------------10308602:重置成本--------------------------
struct MATRADEAPI CReqFsResetCostField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szCostPrice[21 + 1];        // 成本价格 
  char            szFortuneCode[20 + 1];      // 财富账户 
};

struct MATRADEAPI CRspFsResetCostField
{
  char            szCostPrice[21 + 1];        // 修改后成本价 
  char            szCurrentPrice[21 + 1];     // 最新价格 
  char            szMktVal[21 + 1];           // 市值 
};

//-------------------------------10308015:国债预发行持仓明细查询--------------------------
struct MATRADEAPI CReqFsQryBondsPreIssuePosiField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBondsPreIssuePosiField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  char            chBidMode;                  // 招标方式 字典[BID_MODE]
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iTrdDate;                   // 开仓日期 
  int             iMatchTime;                 // 成交时间 
  char            szMatchedSn[16 + 1];        // 成交编号 
  LONGLONG        llPositionQty;              // 原始持仓数量 
  char            chPositionType;             // 持仓类别 字典[POSITION_TYPE]
  char            szPositionPrice[21 + 1];    // 持仓价格 
  char            szDepositAmt[21 + 1];       // 原始履约保证金 
  char            szMarginAmt[21 + 1];        // 价差保证金 
  char            szProfitAmt[21 + 1];        // 收益金额 
  char            szCustRate[21 + 1];         // 原始履约保证金比例 
  LONGLONG        llCloseQty;                 // 平仓数量 
};

//-------------------------------10308016:国债预发行客户平仓明细查询--------------------------
struct MATRADEAPI CReqFsQryCustBondsPreIssueClosePosiField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryCustBondsPreIssueClosePosiField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 开仓合同序号 
  int             iSettDate;                  // 清算日期 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iOpenDate;                  // 开仓日期 
  int             iCloseDate;                 // 平仓日期 
  char            szCloseOrderId[10 + 1];     // 平仓合同序号 
  char            chCloseType;                // 平仓类别 字典[CLOSE_TYPE]
  char            szClosePrice[21 + 1];       // 平仓价格 
  char            szBackDeposit[21 + 1];      // 返还履约保证金 
  char            szMarginAmt[21 + 1];        // 价差保证金 
  char            szProfitAmt[21 + 1];        // 收益金额 
  LONGLONG        llCloseQty;                 // 平仓数量 
};

//-------------------------------10308017:国债预发行产品信息查询--------------------------
struct MATRADEAPI CReqFsQryBondsPreProdInfoField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBondsPreProdInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iOpenDate;                  // 开户日期 
  int             iLastDate;                  // 最后交易日期 
  char            chBidMode;                  // 招标方式 字典[BID_MODE]
  char            szFacePrice[21 + 1];        // 面值价格 
  char            szBenchPrice[21 + 1];       // 基准价格 
  char            szDepositRate[21 + 1];      // 最低履约保证金比例 
  LONGLONG        llRefDuration;              // 参考久期 
  LONGLONG        llIssueAmount;              // 当期发行量 
  char            chIssueStatus;              // 发行状态 字典[ISSUE_STATUS]
};

//-------------------------------10418004:新股配号查询--------------------------
struct MATRADEAPI CReqFsQryNewShareDistributionField
{
  LONGLONG        llCuacctCode;               // 资金账号 资金账户及证券账户不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 资金账户及证券账户不能同时为空
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
  int             iBgnDate;                   // 开始日期 清算日期SETT_DATE
  int             iEndDate;                   // 结束日期 清算日期SETT_DATE
};

struct MATRADEAPI CRspFsQryNewShareDistributionField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iSettDate;                  // 清算日期 
  LONGLONG        llSerialNo;                 // 流水序号 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资金账号 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 币种 字典[CURRENCY]
  char            szOrderId[32 + 1];          // 合同序号 
  char            chAssignFlag;               // 配号中签标志 字典[ASSIGN_FLAG]
  char            szBeginAssignNo[32 + 1];    // 起始配号 
  LONGLONG        llAssignNoNum;              // 配号个数 
  LONGLONG        llOrderQty;                 // 委托数量 
};

//-------------------------------10418005:新股中签查询--------------------------
struct MATRADEAPI CReqFsQryNewShareWinningField
{
  LONGLONG        llCuacctCode;               // 资金账号 资金账户及证券账户不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 资金账户及证券账户不能同时为空
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
  int             iBgnDate;                   // 交易日期 交易日期SETT_DATE
  int             iEndDate;                   // 中签日期 清算日期SETT_DATE
  LONGLONG        llCustCode;                 // 客户代码 
  char            szOrderId[32 + 1];          // 合同序号 
};

struct MATRADEAPI CRspFsQryNewShareWinningField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iSuccessDate;               // 中签日期 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llCuacctCode;               // 资金账号 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  int             iIntOrg;                    // 内部机构 
  char            chCurrency;                 // 币种 字典[CURRENCY]
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkCodeLink[10 + 1];      // 证券代码（关联） 
  char            szOrderId[32 + 1];          // 合同序号 
  LONGLONG        llSuccessNo;                // 中签号码 
  LONGLONG        llSuccessQty;               // 中签数量 
  char            szSubscribePrice[21 + 1];   // 认购价格 
};

//-------------------------------10418006:新股申购市值额度查询--------------------------
struct MATRADEAPI CReqFsQryNewShareSubscriptionLimitField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码及证券账户不能同时为空
  char            szTrdacct[20 + 1];          // 证券账户 客户代码及证券账户不能同时为空
  int             iIntOrg;                    // 内部机构 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryNewShareSubscriptionLimitField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  int             iIntOrg;                    // 机构编码 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szTrdacct[20 + 1];          // 证券账户 
  LONGLONG        llCustQuota;                // 客户市值额度 
  LONGLONG        llKcCustQuota;              // 科创板申购额度 
  int             iReceiveDate;               // 接收日期 
  char            szRemark[32 + 1];           // 备注 
};

//-------------------------------10418007:新股信息查询--------------------------
struct MATRADEAPI CReqFsQryNewShareInfoField
{
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
};

struct MATRADEAPI CRspFsQryNewShareInfoField
{
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szIssuePrice[21 + 1];       // 发行价格 
  int             iTrdDate;                   // 发行日期 
  LONGLONG        llStkUplmtQty;              // 最高数量 
  LONGLONG        llStkLwlmtQty;              // 最低数量 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iStkBiz;                    // 交易标识 字典[STK_BIZ]
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
};

//-------------------------------10308019:报价回购品种信息查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductInfoField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szProdcode[12 + 1];         // 产品编码 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductInfoField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szRepchCode[8 + 1];         // 债券回购代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szQuoteYearRate[21 + 1];    // 报价回购年收益率 
  char            szQuoteBreakRate[21 + 1];   // 报价提前购回收益率 
  char            chQuoteAutoBuyFlag;         // 报价自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  char            chQuoteTradeFlag;           // 报价回购交易标志 字典[QUOTE_TRADE_FLAG]
  char            szProdcode[12 + 1];         // 产品编码 
  char            chProdmode;                 // 产品模式 字典[PROD_MODE]
  char            chIsvalid;                  // 产品是否有效 字典[PROD_IS_VALID]
  int             iRepchDays;                 // 债券回购天数 
  int             iBegintDate;                // 发行日期 
  int             iEndDate;                   // 结束日期 
  int             iNextBeginDate;             // 下轮发行日期 
  int             iNextEndDate;               // 下轮到期日期 
  int             iNextPreBookDate;           // 下轮预约开始日期 
  char            szNextYearRate[21 + 1];     // 下轮到期年收益率 
  char            szNextBreakRate[21 + 1];    // 下轮提前终止收益率 
};

//-------------------------------10308020:报价回购质押物折算率信息查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductInfoConversionRateField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductInfoConversionRateField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szCodeCvtrate[21 + 1];      // 折算率 
  char            szStkLofNav[21 + 1];        // 基金净值 
};

//-------------------------------10308021:报价回购质押物查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseProductField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 股东代码 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseProductField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llStkPrebln;                // 证券昨日余额 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkAvl;                   // 证券可用数量 
};

//-------------------------------10308608:报价回购合约展期--------------------------
struct MATRADEAPI CReqFsQuoteRepurchaseContflagField
{
  int             iTrdDate;                   // 交易日期 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szMatchId[16 + 1];          // 成交编号 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szProdcode[12 + 1];         // 产品编码 
  LONGLONG        llContQty;                  // 展期数量 
};

struct MATRADEAPI CRspFsQuoteRepurchaseContflagField
{
};

//-------------------------------10308022:报价回购质押资金查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchasePledgeFundField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 股东代码 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchasePledgeFundField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szBondCashPrebln[21 + 1];   // 昨日余额 
  char            szBondCashBln[21 + 1];      // 担保库余额 
  char            szBondCashAvl[21 + 1];      // 担保库可用余额 
};

//-------------------------------10308023:报价回购预约流水查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseReservationDetailField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseReservationDetailField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iPreExecDate;               // 提前终止执行日期 
  LONGLONG        llMatchQty;                 // 提前终止数量 
  char            szMatchAmt[21 + 1];         // 提前终止额度 
  LONGLONG        llSerialNo;                 // 预约流水号 
  int             iRepchDate;                 // 购回日期 
  int             iOccurDate;                 // 发生日期 
  char            szOccurTime[32 + 1];        // 发生时间 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
  char            szMatchId[16 + 1];          // 成交编号 
  int             iMatchDate;                 // 成交日期 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            chStatus;                   // 状态 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308610:报价回购预约--------------------------
struct MATRADEAPI CReqFsQuoteRepurchaseReservationField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
  char            chActType;                  // 操作类型 字典[ACT_TYPE]
  int             iPreExecDate;               // 预约执行日期 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llMatchQty;                 // 提前终止数量 
  char            szMatchAmt[21 + 1];         // 提前终止额度 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  int             iIntOrg;                    // 内部机构 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iRepchDate;                 // 购回日期 
  char            szMatchId[16 + 1];          // 成交编号 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  int             iMatchDate;                 // 成交日期 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szProdcode[12 + 1];         // 产品编码 
  LONGLONG        llSerialNo;                 // 预约流水号 
};

struct MATRADEAPI CRspFsQuoteRepurchaseReservationField
{
};

//-------------------------------10308024:报价回购可提前购回合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValPreRepurchaseContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValPreRepurchaseContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            szAdvRepchAmt[21 + 1];      // 提前购回金额 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308025:报价回购可展期取消合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValFlagCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValFlagCancelField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            szAdvRepchAmt[21 + 1];      // 提前购回金额 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308026:报价回购可调整自动展期合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAutoValFlagContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAutoValFlagContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            szAdvRepchAmt[21 + 1];      // 提前购回金额 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308027:报价回购可取消提前终止预约合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlCancelAdvanceEndPreContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            szAdvRepchAmt[21 + 1];      // 提前购回金额 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
  LONGLONG        llRemainQty;                // 可提前终止数量 
  LONGLONG        llSerialNo;                 // 预约流水号 
};

//-------------------------------10308028:报价回购可取消初始交易预约合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlCancelStaTransferPreContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlCancelStaTransferPreContField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iPreExecDate;               // 提前终止执行日期 
  LONGLONG        llMatchQty;                 // 提前终止数量 
  char            szMatchAmt[21 + 1];         // 提前终止额度 
  LONGLONG        llSerialNo;                 // 预约流水号 
  int             iRepchDate;                 // 购回日期 
  int             iOccurDate;                 // 发生日期 
  char            szOccurTime[32 + 1];        // 发生时间 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chPreBookType;              // 预约类型 字典[PRE_BOOK_TYPE]
  char            szMatchId[16 + 1];          // 成交编号 
  int             iMatchDate;                 // 成交日期 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            chStatus;                   // 状态 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308029:报价回购可展期合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseValFlagContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseValFlagContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  LONGLONG        llContQtyReal;              // 可展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
};

//-------------------------------10308074:报价回购可提前终止预约合约查询--------------------------
struct MATRADEAPI CReqFsQryQuoteRepurchaseAvlAdvanceEndPreContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryQuoteRepurchaseAvlAdvanceEndPreContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  char            chStkCls;                   // 证券类别 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_B]
  char            szQuoteYearRate[21 + 1];    // 到期年收益率 
  char            szQuoteBreakRate[21 + 1];   // 提前终止收益率 
  char            chQuoteAutoBuyFlag;         // 自动展期标志 字典[QUOTE_AUTO_BUY_FLAG]
  int             iQuoteContStopDate;         // 展期截止日期 
  char            szAdvRepchAmt[21 + 1];      // 提前购回金额 
  int             iSubContractRepchDate;      // 展期后子合约购回日期 
  LONGLONG        llContQty;                  // 自动展期数量 
  char            szProdcode[12 + 1];         // 产品编码 
  LONGLONG        llRemainQty;                // 可提前终止数量 
};

//-------------------------------10308615:融资行权直接还款--------------------------
struct MATRADEAPI CReqFsFinancingRightDirectRepaymentField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 证券业务行为 字典[STK_BIZ_ACTION]
  char            szRepayContractAmt[21 + 1]; // 偿还金额 
  char            szRemark[128 + 1];          // 备注 
};

struct MATRADEAPI CRspFsFinancingRightDirectRepaymentField
{
  char            szRealRepayAmt[21 + 1];     // 实际还款金额 
};

//-------------------------------10308030:融资行权合约查询--------------------------
struct MATRADEAPI CReqFsQryFinancingRightcontractField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chContractStatus;           // 合约状态 字典[CONTRACT_STATUS]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryFinancingRightcontractField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            szContractSno[32 + 1];      // 合约编号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户名称 
  LONGLONG        llCuacctCode;               // 资金账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llWrntQty;                  // 行权数量 
  char            szWrntAmt[21 + 1];          // 行权金额 
  char            chContractStatus;           // 合约状态 字典[CONTRACT_STATUS]
  char            szTaxAmt[21 + 1];           // 扣税金额 
  int             iExpiringDate;              // 到期日期 
  int             iClosingDate;               // 了结日期 
  char            szRepayAmt[21 + 1];         // 已归还本金 
  char            szRltRepayAmt[21 + 1];      // 实时归还本金 
  int             iIntCalDate;                // 上次计息日期 
  char            szInitIntRate[21 + 1];      // 初始利息年利率 
  char            szIntRate[21 + 1];          // 利息年利率 
  char            szAwardInt[21 + 1];         // 已记利息 
  char            szDueInt[21 + 1];           // 应付利息 
  char            szRepayInt[21 + 1];         // 已还利息 
  char            szInitPuniRate[21 + 1];     // 初始违约日利率 
  char            szPuniRate[21 + 1];         // 违约日利率 
  char            szAwardPuniAmt[21 + 1];     // 已记违约金 
  char            szDuePuniAmt[21 + 1];       // 应付违约金 
  char            szRepayPuniAmt[21 + 1];     // 已还违约金 
  char            chCycleType;                // 利息自动扣收周期 
  int             iCycleValue;                // 利息自动扣收周期日 
};

//-------------------------------10308085:融资行权可还负债查询--------------------------
struct MATRADEAPI CReqFsQryFinancingRightCanrePayDebtField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspFsQryFinancingRightCanrePayDebtField
{
  LONGLONG        llDebt;                     // 可还负债 
  LONGLONG        llRltDebt;                  // 实时已还负债 
};

//-------------------------------10308087:融资行权偿还明细查询--------------------------
struct MATRADEAPI CReqFsQryFinancingRightPayDetailField
{
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chRepayType;                // 偿还类型 字典[WRNT_REPAY_TYPE]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryFinancingRightPayDetailField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            szContractSno[32 + 1];      // 合约编号 
  char            chRepayType;                // 偿还类型 字典[WRNT_REPAY_TYPE]
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            szRepayAmt[21 + 1];         // 合约本金还款 
  char            szRepayInt[21 + 1];         // 合约利息还款 
  char            szRepayPuniAmt[21 + 1];     // 违约金还款 
};

//-------------------------------10308031:网络投票股东大会信息查询--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingTrdacctsConferenceInfoField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szCompanyCode[8 + 1];       // 公司代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryNetworkVotingTrdacctsConferenceInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szCompanyCode[8 + 1];       // 公司代码 
  char            szCompanyInfo[8 + 1];       // 公司信息 
  int             iMeetingDateBegin;          // 股东大会开始日 
  int             iMeetingDateEnd;            // 股东大会结束日 
  char            chMeetingType;              // 股东大会类型 字典[MEETING_TYPE]
  char            szMeetingDesc[512 + 1];     // 股东大会描述 
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
};

//-------------------------------10308032:网络投票投票结果查询--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingResultField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryNetworkVotingResultField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iVoteDate;                  // 投票日期 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szVoteStkCode[8 + 1];       // 产品代码 
  char            szVoteResult[128 + 1];      // 投票结果 
  char            szVoteId[10 + 1];           // 议案编号 
  char            szVoteInfo[256 + 1];        // 议案信息 
  char            chVoteType;                 // 议案类型 字典[VOTE_TYPE]
};

//-------------------------------10308033:网络投票议案信息查询--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingMotionInfoField
{
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szVoteId[10 + 1];           // 议案编号 
  char            chVoteType;                 // 议案类型 字典[VOTE_TYPE]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryNetworkVotingMotionInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szVoteId[10 + 1];           // 议案编号 
  char            szVoteInfo[256 + 1];        // 议案信息 
  char            chVoteType;                 // 议案类型 字典[VOTE_TYPE]
  LONGLONG        llVoteLeijino;              // 累积投票应选数 
  char            szVoteRole[64 + 1];         // 股东身份 
  char            szVoteRelation[128 + 1];    // 议案关系 
  char            szVoteStkCodes[512 + 1];    // 投票产品(复) 
};

//-------------------------------10308059:网络投票产品信息查询--------------------------
struct MATRADEAPI CReqFsQryNetworkVotingProdInfoField
{
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szVoteStkCode[8 + 1];       // 产品代码 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryNetworkVotingProdInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szMeetingSeq[10 + 1];       // 股东大会编码 
  char            szVoteStkCode[8 + 1];       // 产品代码 
  int             iRegDate;                   // 股权登记日 
  int             iLastTrdDate;               // 最后交易日 
  char            chVoteStkType;              // 股份类别 字典[VOTE_STK_TYPE]
  char            szStkSabbr[8 + 1];          // 股票简称 
};

//-------------------------------10308051:港股通价差信息查询--------------------------
struct MATRADEAPI CReqFsQryHkCuaccDifferencetField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szPrice[21 + 1];            // 价格 
};

struct MATRADEAPI CRspFsQryHkCuaccDifferencetField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            chStkCls;                   // 证券类别 字典[CURRENCY]
  char            szBgnPrice[21 + 1];         // 价格下限 
  char            szEndPrice[21 + 1];         // 价格上限 
  char            szStkSpread[21 + 1];        // 价差（价位） 
  char            szMinBgnPrice[21 + 1];      // 最低价格下限 
  char            szMaxEndPrice[21 + 1];      // 最高价格上限 
  int             iPriceUnitNum;              // 价差数量 
};

//-------------------------------10308050:港股通汇率信息查询--------------------------
struct MATRADEAPI CReqFsQryHkRationInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspFsQryHkRationInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBuyRate[21 + 1];          // 买入参考汇率 
  char            szDayBuyRiseRate[21 + 1];   // 日间买入参考汇率浮动比例 
  char            szNightBuyRiseRate[21 + 1]; // 夜市买入参考汇率浮动比例 
  char            szSaleRate[21 + 1];         // 卖出参考汇率 
  char            szDaySaleRiseRate[21 + 1];  // 日间卖出参考汇率浮动比例 
  char            szNightSaleRiseRate[21 + 1];// 夜市卖出参考汇率浮动比例 
  char            szMidRate[21 + 1];          // 中间参考汇率 
  char            szBuySettRate[21 + 1];      // 买入结算汇率 
};

//-------------------------------10308061:港股通市场状态信息查询--------------------------
struct MATRADEAPI CReqFsQryHkShareMarketInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
};

struct MATRADEAPI CRspFsQryHkShareMarketInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szThresholdQuota[21 + 1];   // 每日初始额度 
  char            szPosQuota[21 + 1];         // 日中剩余额度 
  char            chQuotaStatus;              // 额度状态 字典[QUOTA_STATUS]
  char            chMktTrdStatus;             // 市场交易状态 字典[MKT_TRD_STATUS]
};

//-------------------------------10308034:港股通标的证券信息查询--------------------------
struct MATRADEAPI CReqFsQryHkShareCommTrdInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryHkShareCommTrdInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chLotTrdStatus;             // 整手交易状态 字典[LOT_TRD_STATUS]
  char            chOddTrdStatus;             // 零股交易状态 字典[ODD_TRD_STATUS]
  char            chObjectFlag;               // 标的标志 字典[OBJECT_FLAG]
  int             iUpdDate;                   // 更新日期 
};

//-------------------------------10308035:港股通交易日历查询--------------------------
struct MATRADEAPI CReqFsQryHkShareTransferDateField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryHkShareTransferDateField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iPhysicalDate;              // 物理日期 
  char            chTrdDateFlag;              // 交易日标志 字典[TRD_DATE_FLAG]
  char            chSettDateFlag;             // 清算日标志 字典[SETT_DATE_FLAG]
};

//-------------------------------10308047:港股通资金查询--------------------------
struct MATRADEAPI CReqFsQryHkShareFundField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
};

struct MATRADEAPI CRspFsQryHkShareFundField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szFundAvl[21 + 1];          // 港股通资金可用 
  char            szFundTrdFrz[21 + 1];       // 港股通资金交易冻结 
  char            szFundTrdUfz[21 + 1];       // 港股通资金交易解冻 
  char            szFundTrdOtd[21 + 1];       // 港股通资金交易在途 
  char            szFundTrdBln[21 + 1];       // 港股通资金交易轧差 
};

//-------------------------------10308036:大宗交易行情信息查询--------------------------
struct MATRADEAPI CReqFsQryBlockTradingMktField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBlockTradingMktField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chMkCls;                    // 行情类别 字典[MK_CLS]
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            chSide;                     // 买卖方向 字典[SIDE]
  char            szConfirmId[8 + 1];         // 约定号 
  int             iOrderTime;                 // 委托时间 
  char            szMatchedTime[8 + 1];       // 成交时间 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  LONGLONG        llMatchedQty;               // 成交数量 
  char            szContactInfo[50 + 1];      // 联系信息 
  char            chExecType;                 // 执行类型 字典[EXEC_TYPE]
};

//-------------------------------10308108:盘后行情查询--------------------------
struct MATRADEAPI CReqFsQrylosingMktSecondboardField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkPhQuoteType;           // 盘后报价类型 字典[PH_QUOTE_TYPE]
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQrylosingMktSecondboardField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkPhhqlb[5 + 1];         // 行情类别 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            chStkPhQuoteType;           // 报价类型 字典[PH_QUOTE_TYPE]
  char            szStkPhQuotePrice[21 + 1];  // 价格 
  LONGLONG        llStkMatchedQty;            // 成交数量 
  char            szStkMatchedAmt[21 + 1];    // 成交金额 
  LONGLONG        llStkMatchedCount;          // 成交笔数 
  LONGLONG        llStkBuyLeftQty;            // 买盘剩余数量 
  LONGLONG        llStkSaleLeftQty;           // 卖盘剩余数量 
  char            szUpdTime[32 + 1];          // 更新时间 
  char            szRemark[56 + 1];           // 备注 
};

//-------------------------------10308037:受限投资者可交易证券查询--------------------------
struct MATRADEAPI CReqFsQryRestrictedInvestorValTransferTrdField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryRestrictedInvestorValTransferTrdField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 证券账户 
};

//-------------------------------10308067:股转分层信息查询--------------------------
struct MATRADEAPI CReqFsQryStkTranLayeringInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryStkTranLayeringInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szStkLayerInfor[16 + 1];    // 分层信息 
};

//-------------------------------10308109:股转确认信息查询--------------------------
struct MATRADEAPI CReqFsQryStkTranInquiryConfirmInfoField
{
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            chQnqrStatus;               // 确认状态 字典[QNQR_STATUS]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryStkTranInquiryConfirmInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szMatchPrice[21 + 1];       // 成交价格 
  LONGLONG        llMatchQty;                 // 成交数量 
  LONGLONG        llMatchQty2;                // 成交数量2 
  char            chQnqrStatus;               // 确认状态 字典[QNQR_STATUS]
  int             iMatchDate;                 // 成交日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szRemark[1024 + 1];         // 备注信息 
};

//-------------------------------10308110:股转发行业务信息查询--------------------------
struct MATRADEAPI CReqFsQryStkTranIssuingBusinessInfoField
{
  char            chIssuStatus;               // 发行状态 字典[ISSU_STATUS]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryStkTranIssuingBusinessInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szStkCode[8 + 1];           // 发行代码 
  char            szStkName[32 + 1];          // 证券名称 
  char            szStkUndlCode[8 + 1];       // 标的代码 
  char            chIssuStatus;               // 发行状态 字典[ISSU_STATUS]
  char            szStkSpread[21 + 1];        // 价差（价位） 
  LONGLONG        llStkLotSize;               // 每手数量 
  char            szLwlmtPrice[21 + 1];       // 最低申报价 
  char            szUplmtPrice[21 + 1];       // 最高申报价 
  LONGLONG        llLwlmtQty;                 // 最低申报数量 
  LONGLONG        llUplmtQty;                 // 最高申报数量 
  LONGLONG        llPublishCount;             // 本次发行量 
  int             iPublishDate;               // 挂牌日期 
  int             iRfqBgnDate;                // 询价开始日期 
  int             iRfqEndDate;                // 询价截止日期 
};

//-------------------------------10308038:股票质押合约总表查询--------------------------
struct MATRADEAPI CReqFsQryStkPledgeContractField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            chRepchFlag;                // 购回标志 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryStkPledgeContractField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szGpzysno[32 + 1];          // 合约编号 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  int             iLoanerIntOrg;              // 出借人营业部代码 
  char            szLoanerStkpbu[8 + 1];      // 出借人席位 
  char            szLoanerTrdacct[20 + 1];    // 出借人股东 
  char            szLoanerName[40 + 1];       // 出借人姓名 
  char            chLoanerKind;               // 出借人类型 字典[LOANER_KIND]
  int             iRepchDate;                 // 回购日期 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szBackAmt[21 + 1];          // 已还金额 
  char            szBackAmtReal[21 + 1];      // 实时购回金额 
  char            szDueIntr[21 + 1];          // 应付利息 
  char            szPayInterest[21 + 1];      // 已还利息 
  char            szGpzybzRate[21 + 1];       // 履约保障比例 
  char            szWarningRate[21 + 1];      // 预警线比例 
  char            szLowestRate[21 + 1];       // 最低线比例 
  char            szEndingRate[21 + 1];       // 终止线比例 
  char            szSafeRate[21 + 1];         // 风险比例 
  char            chSafeStatus;               // 风险状态 字典[SAFE_STATUS]
  char            szFundusedo[32 + 1];        // 资金用途 字典[FUND_PROC_METHOD]
  char            chRiskStatus;               // 违约状态 字典[RISK_STATUS]
  char            chStatus;                   // 状态 字典[STATUS]
};

//-------------------------------10308039:股票质押合约明细查询--------------------------
struct MATRADEAPI CReqFsQryStkPledgeContractDetailField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szGpzysno[32 + 1];          // 合约编号 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryStkPledgeContractDetailField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szGpzysno[32 + 1];          // 合约编号 
  int             iItemno;                    // 合约序号 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szTrdacct[20 + 1];          // 交易账户 
  LONGLONG        llMatchQty;                 // 成交数量 
  LONGLONG        llBonusqtyUncome;           // 抵押在途数量 
  LONGLONG        llBonusqty;                 // 红股数量 
  LONGLONG        llBackqty;                  // 解押数量 
  LONGLONG        llBackqtyReal;              // 当天解押数量 
  LONGLONG        llAvlBackQty;               // 可解押数量 
  char            szBonusAmt[21 + 1];         // 红利金额 
  char            szBackbonusAmt[21 + 1];     // 红利解押金额 
  char            szBackbonusAmtReal[21 + 1]; // 当天解押金额 
  char            szAvlBonusAmt[21 + 1];      // 可解押金额 
  LONGLONG        llWysbqty;                  // 违约已申报股份数量 
  LONGLONG        llWyczqty;                  // 违约处置委托数量 
  LONGLONG        llWyczmatchqty;             // 违约处置卖出成交数量 
  int             iOrderDate;                 // 委托日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  int             iStartorderdate;            // 初始质押日期 
  char            szStartOrderId[10 + 1];     // 初始合同序号 
  char            szStartMatchCode[16 + 1];   // 初始成交编号 
  char            szStkprop[2 + 1];           // 股份性质 字典[STKPROP]
  char            szSdStkType[2 + 1];         // 上登证券类别 字典[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // 权益类型 字典[EQUIT_TYPE]
  char            szInitYear[4 + 1];          // 挂牌年份 
  char            chCirculType;               // 流通类型 字典[CIRCUL_TYPE]
  char            chStatus;                   // 状态 字典[STATUS]
};

//-------------------------------10308062:股票质押初始交易参考金额查询--------------------------
struct MATRADEAPI CReqFsQryStkPledgeInitTrdReferAmtField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szStkprop[2 + 1];           // 股份性质 字典[STKPROP]
  char            chCirculType;               // 流通类型 字典[CIRCUL_TYPE]
};

struct MATRADEAPI CRspFsQryStkPledgeInitTrdReferAmtField
{
  char            szReferenceAtm[21 + 1];     // 参考金额 
};

//-------------------------------10308063:股票质押股份额度信息查询--------------------------
struct MATRADEAPI CReqFsQryStkPledgeQuotaInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkpbu[8 + 1];            // 交易单元 
};

struct MATRADEAPI CRspFsQryStkPledgeQuotaInfoField
{
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llStkAvl;                   // 证券可用数量 
  char            szStkprop[2 + 1];           // 股份性质 字典[STKPROP]
  char            szSdStkType[2 + 1];         // 上登证券类别 字典[SD_STK_TYPE]
  char            szEquitType[2 + 1];         // 权益类型 字典[EQUIT_TYPE]
  char            chCirculType;               // 流通类型 字典[CIRCUL_TYPE]
};

//-------------------------------10308076:股票质押标的证券查询--------------------------
struct MATRADEAPI CReqFsQryStkPledgeTrdField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryStkPledgeTrdField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szCodeCvtrate[21 + 1];      // 折算率 
  char            szLimitCvtrate[21 + 1];     // 限售股折算率 
  LONGLONG        llTrdMinQty;                // 交易最低数量 
  char            szMinAmt[21 + 1];           // 最少初始金额 
  char            chPriceType;                // 初始价格类型 0: 昨收盘1: 今开盘2: 最新价3: 面值，股票1元4: 前20个交易日收盘平均价
  LONGLONG        llCountLimit;               // 总额度 
};

//-------------------------------10308077:出借人股票质押额度查询--------------------------
struct MATRADEAPI CReqFsQryLenderStkPledgeQuotaField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szLoanerTrdacct[20 + 1];    // 出借人股东 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryLenderStkPledgeQuotaField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szLoanerTrdacct[20 + 1];    // 出借人股东 
  char            szLoanerName[40 + 1];       // 出借人姓名 
  char            chLoanerKind;               // 出借人类型 字典[LOANER_KIND]
  char            szCountLimit[21 + 1];       // 总额度 
  char            chLoanerStatus;             // 出借状态 0: 正常 1: 禁止
};

//-------------------------------10308040:深圳债券质押式协议回购合约查询--------------------------
struct MATRADEAPI CReqFsQryszBondPledgeRepurchaseContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chRepchStatus;              // 合约状态 字典[REPCH_STATUS_Z]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryszBondPledgeRepurchaseContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户名称 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchPrice[21 + 1];       // 成交价格 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szStkprop[2 + 1];           // 股份性质 字典[STKPROP]
  char            szBonusAmt[21 + 1];         // 红利金额 
  int             iRepchDate;                 // 购回日期 
  char            szRepchAmt[21 + 1];         // 购回金额 
  char            szDueRate[21 + 1];          // 年利率 
  int             iRealRepchDate;             // 实际购回日期 
  char            szRealRepchAmt[21 + 1];     // 实际购回金额 
  char            szRealDueRate[21 + 1];      // 实际年利率 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 合约状态 字典[REPCH_STATUS_Z]
  char            szOpptStkpbu[8 + 1];        // 对手方交易单元 
  char            szOpptTrdacct[20 + 1];      // 对手方股东账户 
  char            szOriginalContractId[32 + 1];// 初始合约编号 
};

//-------------------------------10308041:债券质押式协议回购标的证券查询--------------------------
struct MATRADEAPI CReqFsQryBondPledgeRepurchaseTrdField
{
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBondPledgeRepurchaseTrdField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chPriceType;                // 价格类型 字典[PRICE_TYPE]
  char            szCodeCvtrate[21 + 1];      // 折算率 
  LONGLONG        llTrdMinQty;                // 最少交易数量 
  char            szOrderPrice[21 + 1];       // 初始交易价格 
  char            szMinAmt[21 + 1];           // 最小交易金额 
  int             iOccupedDate;               // 存续截止日 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
};

//-------------------------------10308613:上海协议回购行情查询申报--------------------------
struct MATRADEAPI CReqFsshContRepurchaseMarketField
{
  char            chHqCls;                    // 行情类别 0 – 未结算协议1-非公开报价2-公开报价
  char            chQueryType;                // 查询类型 HQ_CLS为0 时有效1 到期结算查询申请2 到期续做查询申请3解除质押查询申请4 换券申报查询申请5 提前终止查询申请
  char            chRepchStatus;              // 回购状态 HQ_CLS为0 时有效3 当日到期(查询类型到期结算和到期续做时填写3)4 未到期 (查询类型为换券申报和提前终止时填写4)5 已过期 (查询类型为解除质押时填写5)
  LONGLONG        llBeginSn;                  // 起始序号 起始序号（全市场），单调递增不连续，最小值为0，最大值为1000000000
  char            szCompanyId[3 + 1];         // 交易商代码 HQ_CLS为0 时为正回购方其他为查询方交易商代码
  char            szOperCode[6 + 1];          // 交易员代码 HQ_CLS为0 时为正回购方其他为查询方交易员代码
  char            szOpptCompanyId[3 + 1];     // 逆回购方交易商代码 HQ_CLS为0 时有效
};

struct MATRADEAPI CRspFsshContRepurchaseMarketField
{
};

//-------------------------------10308072:上海协议回购非公开报价行情查询--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseClsMarketField
{
  char            szQuoteRefId[10 + 1];       // 交易所报价请求编号 
  char            chMdUpdateAction;           // 行情更新类型 字典[MD_UPDATE_ACTION]
  char            chOrdType;                  // 报价申报类型 字典[ORD_TYPE]
  char            chSide;                     // 买卖方向 字典[SIDE_R]
  char            szSecurityId[6 + 1];        // 质押券代码 
  char            chOrderRestrictions;        // 到期续做类型 字典[ORDER_RESTRICTIONS]
  char            szCompanyId[3 + 1];         // 本方交易商代码 
  char            szOpptTrdacctName[10 + 1];  // 对手方交易账户名称 
  char            szOpptOperCode[6 + 1];      // 对手方交易员代码 
  char            szApplReqId[10 + 1];        // 查询请求编号 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryshContRepurchaseClsMarketField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            szMatchedTime[8 + 1];       // 成交时间 
  char            szApplReqId[10 + 1];        // 查询请求编号 
  int             iRecSn;                     // 记录号 
  int             iEndSeqNo;                  // 结束行情序号 
  char            szQuoteRefId[10 + 1];       // 交易所报价请求编号 
  char            chMdUpdateAction;           // 行情更新类型 字典[MD_UPDATE_ACTION]
  char            chOrdType;                  // 报价申报类型 字典[ORD_TYPE]
  char            szPrice[21 + 1];            // 回购利率 
  int             iRepurchaseTerm;            // 回购期限 
  int             iUaInterestAccrualDays;     // 实际占款天数 
  int             iSettlDate;                 // 首次结算日 
  int             iMaturityDate;              // 回购到期日 
  int             iSettlDate2;                // 到期结算日 
  char            chSide;                     // 买卖方向 字典[SIDE_R]
  char            szSecurityId[6 + 1];        // 质押券代码 
  char            szSecurityName[8 + 1];      // 质押券简称 
  char            szUnderlyingSecurityId[6 + 1];// 原质押券代码 
  LONGLONG        llOrderQty;                 // 质押券数量 
  char            szContractMultiplier[21 + 1];// 折算比例 
  char            szTotalValueTraded[21 + 1]; // 成交金额 
  char            szUnderlyingQty[21 + 1];    // 原成交申报成交金额 
  char            szAccruedInterestAmt[21 + 1];// 回购利息 
  char            chOrderRestrictions;        // 到期续做类型 字典[ORDER_RESTRICTIONS]
  int             iExecRefId;                 // 首次申报时交易所成交编号 
  char            szCompanyId[3 + 1];         // 本方交易商代码 
  char            szOrigTradeDate[8 + 1];     // 成交申报时交易所成交日期 
  char            szOpptCompanyName[16 + 1];  // 对手方交易商简称 
  char            szOpptTrdacctName[10 + 1];  // 对手方交易账户名称 
  char            szOpptOperCode[6 + 1];      // 对手方交易员代码 
  char            szSettlCurrAmt[21 + 1];     // 到期结算金额 
  char            szLastQty[21 + 1];          // 质押券面总额合计 
};

//-------------------------------10308073:上海协议回购公开报价行情查询--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseOpenMarketField
{
  char            szQuoteRefId[10 + 1];       // 交易所报价请求编号 
  char            chMdUpdateAction;           // 行情更新类型 字典[MD_UPDATE_ACTION]
  char            chOrdType;                  // 报价申报类型 字典[ORD_TYPE]
  char            chSide;                     // 买卖方向 字典[SIDE_R]
  char            szSecurityId[6 + 1];        // 质押券代码 
  char            szOperCode[6 + 1];          // 发起方交易员代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryshContRepurchaseOpenMarketField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  char            szMatchedTime[8 + 1];       // 成交时间 
  char            szApplReqId[10 + 1];        // 查询请求编号 
  int             iRecSn;                     // 记录号 
  int             iEndSeqNo;                  // 结束行情序号 
  char            szQuoteRefId[10 + 1];       // 交易所报价请求编号 
  char            chMdUpdateAction;           // 行情更新类型 字典[MD_UPDATE_ACTION]
  char            chOrdType;                  // 报价申报类型 字典[ORD_TYPE]
  char            chSide;                     // 买卖方向 字典[SIDE_R]
  char            szSecurityId[6 + 1];        // 质押券代码 
  char            szSecurityName[8 + 1];      // 质押券简称 
  LONGLONG        llOrderQty;                 // 质押券数量 
  char            szContractMultiplier[21 + 1];// 折算比例 
  char            szPrice[21 + 1];            // 回购利率 
  int             iRepurchaseTerm;            // 回购期限 
  char            szTotalValueTraded[21 + 1]; // 成交金额 
  int             iSettlDate;                 // 首次结算日 
  char            szCompanyName[16 + 1];      // 发起方交易商简称 
  char            szOperCode[6 + 1];          // 发起方交易员代码 
};

//-------------------------------10308075:上海协议回购未结算协议查询--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseUnsettledBillField
{
  char            szCompanyCode[3 + 1];       // 交易商代码 
  char            szOperCode[6 + 1];          // 交易员代码 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_C]
  char            chOptFlag;                  // 处理标志 字典[OPT_FLAG]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szOpptCompanyCode[3 + 1];   // 交易商代码(对手) 
  char            szOpptCompanyName[10 + 1];  // 交易商简称(对手) 
  char            szOpptOperCode[6 + 1];      // 交易员代码(对手) 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryshContRepurchaseUnsettledBillField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iMatchDate;                 // 成交日期 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iIntOrg;                    // 内部机构 
  char            szCompanyCode[3 + 1];       // 交易商代码 
  char            szCompanyName[10 + 1];      // 交易商简称 
  char            szOperCode[6 + 1];          // 交易员代码 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkPbu[8 + 1];            // 交易单元 
  char            szMatchId[16 + 1];          // 成交编号 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  int             iRepchDate;                 // 回购日期 
  int             iEndDate;                   // 到期结算日期 
  int             iRepchLimitDates;           // 回购期限 
  char            szRepchAmt[21 + 1];         // 购回金额 
  char            szInterest[21 + 1];         // 应计利息 
  char            szDueRate[21 + 1];          // 年利率 
  char            szConversionRate[21 + 1];   // 折算率 
  char            szSumFaceAmt[21 + 1];       // 质押券面总额 
  int             iRealHoldDates;             // 实际占款天数 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_C]
  char            chOptFlag;                  // 处理标志 字典[OPT_FLAG]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szOpptCompanyCode[3 + 1];   // 交易商代码(对手) 
  char            szOpptCompanyName[10 + 1];  // 交易商简称(对手) 
  char            szOpptOperCode[6 + 1];      // 交易员代码(对手) 
  char            szPawneeName[30 + 1];       // 质权人名称 
};

//-------------------------------10308078:上海协议回购合约查询--------------------------
struct MATRADEAPI CReqFsQryshContRepurchaseContractField
{
  char            szCompanyCode[3 + 1];       // 交易商代码 
  char            szOperCode[6 + 1];          // 交易员代码 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_H]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szOpptCompanyCode[3 + 1];   // 交易商代码(对手) 
  char            szOpptCompanyName[10 + 1];  // 交易商简称(对手) 
  char            szOpptOperCode[6 + 1];      // 交易员代码(对手) 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryshContRepurchaseContractField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iSettDate;                  // 清算日期 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  int             iIntOrg;                    // 内部机构 
  char            szCompanyCode[3 + 1];       // 交易商代码 
  char            szCompanyName[10 + 1];      // 交易商简称 
  char            szOperCode[6 + 1];          // 交易员代码 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkPbu[8 + 1];            // 交易单元 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchPrice[21 + 1];       // 成交价格 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkProp[2 + 1];           // 股份性质 字典[STKPROP]
  char            szBonusAmt[21 + 1];         // 红利金额 
  int             iRepchDate;                 // 回购日期 
  int             iEndDate;                   // 到期结算日期 
  int             iRepchLimitDates;           // 回购期限 
  char            szRepchAmt[21 + 1];         // 购回金额 
  char            szDueRate[21 + 1];          // 年利率 
  char            szConversionRate[21 + 1];   // 折算率 
  char            szSumFaceAmt[21 + 1];       // 质押券面总额 
  int             iRealHoldDates;             // 实际占款天数 
  int             iRealRepchDate;             // 实际购回日期 
  char            szRealRepchAmt[21 + 1];     // 实际购回金额 
  char            szRealDueRate[21 + 1];      // 实际购回年利率 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_H]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szOpptCompanyCode[3 + 1];   // 交易商代码(对手) 
  char            szOpptCompanyName[10 + 1];  // 交易商简称(对手) 
  char            szOpptOperCode[6 + 1];      // 交易员代码(对手) 
  char            szPawneeName[30 + 1];       // 质权人名称 
  char            chCustCls;                  // 客户类别 
  char            szCuacctCls[4 + 1];         // 资产账户类别 
  char            szRemark[1024 + 1];         // 备注 
};

//-------------------------------10308086:深圳债券协议回购购回金额查询--------------------------
struct MATRADEAPI CReqFsQryszBondContRepurchaseQuotaField
{
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iRepayOpeningDate;          // 初始交易日期 
  char            szInitTrdAmt[21 + 1];       // 初始交易金额 
  char            szRepchRate[21 + 1];        // 购回利率 
};

struct MATRADEAPI CRspFsQryszBondContRepurchaseQuotaField
{
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iRepayOpeningDate;          // 初始交易日期 
  char            szInitTrdAmt[21 + 1];       // 初始交易金额 
  char            szRepchRate[21 + 1];        // 购回利率 
  char            szRepchAmt[21 + 1];         // 购回金额 
};

//-------------------------------10308042:债券质押股份明细查询--------------------------
struct MATRADEAPI CReqFsQryBondPledgeStkDetailField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBondPledgeStkDetailField
{
  char            szQryPos[32 + 1];           // 定位串 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llStkPrebln;                // 证券昨日余额 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkAvl;                   // 证券可用数量 
};

//-------------------------------10308043:债券质押式回购合约查询--------------------------
struct MATRADEAPI CReqFsQryBondPledgeStkRepurchaseContField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryBondPledgeStkRepurchaseContField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iRepchDate;                 // 到期日期 
  int             iTrdDate;                   // 交易日期 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCustCode;                 // 客户代码 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szOrderPrice[21 + 1];       // 委托价格 
  char            szMatchedPrice[21 + 1];     // 成交价格 
  LONGLONG        llRepchQty;                 // 回购数量 
  char            szRepchAmt[21 + 1];         // 到期金额 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llOrderQty;                 // 委托数量 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_Z]
};

//-------------------------------10308604:ETF网下认购申请撤单--------------------------
struct MATRADEAPI CReqFsETFOfflineApplicationCancelField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  int             iTrdDate;                   // 交易日期 
  int             iOrderSn;                   // 委托序号 
};

struct MATRADEAPI CRspFsETFOfflineApplicationCancelField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkSubCode[8 + 1];        // 认购代码 
  char            szStkSubName[16 + 1];       // 认购代码名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  int             iOrderSn;                   // 委托序号 
};

//-------------------------------10308605:ETF篮子委托--------------------------
struct MATRADEAPI CReqFsETFOrderField
{
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szEtfCode[8 + 1];           // ETF代码 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szPriceInfo[1024 + 1];      // 价格信息 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            chBizType;                  // 交易类型 'B'：买篮, 'P'：补篮, 'S'：卖篮，'4'：留篮，'5'：清篮
  char            szClientInfo[256 + 1];      // 终端信息 包括硬盘号\CPU\机器名等
  char            chSecurityLevel;            // 安全手段 0:无安全1:校验票据2:校验密码
  char            szSecurityInfo[256 + 1];    // 安全信息 安全手段为校验票据则需填写票据安全手段为密码则需填写交易密码密文
  char            szTrdacctLink[20 + 1];      // 关联股东 深圳跨市场ETF时启用
  char            chPublishCtrlFlag;          // 持仓比例控制 
  int             iOrderBsn;                  // 委托批号 
};

struct MATRADEAPI CRspFsETFOrderField
{
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 冻结金额 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  int             iStkBizAction;              // 业务行为 字典[STK_BIZ_ACTION]
};

//-------------------------------10308057:ETF网下认购申请查询--------------------------
struct MATRADEAPI CReqFsQryETFOfflineApplicationField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  LONGLONG        llTrdDate;                  // 交易日期 
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkSubCode[8 + 1];        // 认购代码 
  int             iOrderSn;                   // 委托序号 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkBiz[1024 + 1];         // 证券业务 字典[STK_BIZ]
  char            chStkSubStatus;             // 处理状态 字典[STK_SUB_STATUS]
  char            szOrderId[10 + 1];          // 合同序号 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryETFOfflineApplicationField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  int             iOrderSn;                   // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szOrderType[32 + 1];        // 委托类型 字典[ORDER_TYPE]
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
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
};

//-------------------------------10308064:ETF成分股持仓查询--------------------------
struct MATRADEAPI CReqFsQryETFCotPositionField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            chEtfMode;                  // ETF模式 字典[ETF_MODE]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryETFCotPositionField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            szStkCode[8 + 1];           // 成分股代码 
  char            szStkName[8 + 1];           // 成分股简称 
  char            chStkMarket;                // 成分股交易市场 
  LONGLONG        llStkQty;                   // 成分股数量 
  LONGLONG        llStkAvl;                   // 客户持仓数量 
  char            chEtfInsteadFlag;           // 替代标志 0'表示禁止现金替代（必须有股票）1'表示可以进行现金替代（先用股票，股票不足的话用现金替代）2'表示必须用现金替代3'表示深市股票退补现金替代4'表示深市股票必须现金替代5'表示非沪深退补现金替代6'表示非沪深必须现金替代
  char            szEtfOverflowRate[21 + 1];  // 溢价比例 
  char            szCreationInsteadAmt[21 + 1];// 申购替代金额 
  char            szRedemptionInsteadAmt[21 + 1];// 赎回替代金额 
};

//-------------------------------10308052:ETF信息查询--------------------------
struct MATRADEAPI CReqFsQryEtfInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            szQryPos[32 + 1];           // 定位串 
  int             iQryNum;                    // 查询行数 
};

struct MATRADEAPI CRspFsQryEtfInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szEtfCode[8 + 1];           // ETF代码 
  char            szEtfName[32 + 1];          // ETF名称 
  char            chEtfPublish;               // ETF发布标志 0--不发布1--发布
  char            chEtfCrFlag;                // 申购赎回标志 0-不允许申购赎回1-允许申购赎回2-允许申购，不允许赎回3-允许赎回，不允许申购
  LONGLONG        llEtfCrUnit;                // 申购赎回单位 
  char            szEtfCashRatio[21 + 1];     // 现金替代比例 
  int             iEtfStkNum;                 // ETF股票篮股票数 
  char            szEtfEstmCash[21 + 1];      // ETF股票篮现金差 
  char            szEtfCashComp[21 + 1];      // 基准单位现金 
  char            szEtfNavPercu[21 + 1];      // 基准单位净值 
  char            szEtfNav[21 + 1];           // ETF单位净值 
  char            chEtfType;                  // ETF类型 字典[ETF_TYPE]
  char            chEtfMode;                  // ETF模式 字典[ETF_MODE]
  char            chQsEtfCrFlag;              // 券商申购赎回标志 0-不允许申购赎回1-允许申购赎回2-允许申购，不允许赎回3-允许赎回，不允许申购
  int             iTrdDate;                   // 交易日期 
  char            szRgCode[8 + 1];            // 发行认购代码 
  char            szEjscCode[8 + 1];          // 二级市场基金代码 
};

//-------------------------------10308044:上海LOF基金分拆合并信息查询--------------------------
struct MATRADEAPI CReqFsQryshLOFFundSplitMergeField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkLofCode[8 + 1];        // LOF代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryshLOFFundSplitMergeField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkLofCode[8 + 1];        // 上证LOF代码 
  char            szStkLofName[10 + 1];       // 上证LOF名称 
  char            szStkLofStatusSh[8 + 1];    // 上证LOF状态 
  int             iStkLofRatio;               // 基金转换系数 
  char            chStkLofType;               // 基金类型 
  char            chStkLofSubType;            // 基金子类型 
  LONGLONG        llStkLofPmUnit;             // 拆分合并单位 
  LONGLONG        llStkLofMergeFloor;         // 最小合并数 
  LONGLONG        llStkLofPartitionFloor;     // 最小拆分数 
  char            szStkLofMainCode[8 + 1];    // 母基金代码 
  char            szStkLofMainName[10 + 1];   // 母基金名称 
  int             iStkLofMainRatio;           // 母份额转换系数 
  char            szStkLofMainStatus[8 + 1];  // 母基金状态 
};

//-------------------------------10308045:深圳LOF基金分拆合并信息查询--------------------------
struct MATRADEAPI CReqFsQryszLOFFundSplitMergeField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkLofCode[8 + 1];        // LOF代码 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQryszLOFFundSplitMergeField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 证券交易所 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chStkLofStatus;             // 基金状态 字典[STK_LOF_STATUS]
  int             iStkMsRatio;                // 合并/分拆比例 
  LONGLONG        llStkMergeFloor;            // 最小合并数 
  LONGLONG        llStkSplitFloor;            // 最小拆分数 
  char            szStkLofMainCode[8 + 1];    // 母基金代码 
};

//-------------------------------10308046:股权人员信息查询--------------------------
struct MATRADEAPI CReqFsQrySecurInfoField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  int             iQryNum;                    // 查询行数 每次取的行数
};

struct MATRADEAPI CRspFsQrySecurInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szWrntStkCode[8 + 1];       // 标的代码 
  char            szWrntExeCode[8 + 1];       // 行权代码 
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  char            chCustType;                 // 客户类型 字典[CUST_TYPE_F]
  int             iProrateTime;               // 境内工作时间 
  char            chProcTax;                  // 是否处理所得税 字典[PROC_TAX]
  char            szBegIncomeTax[21 + 1];     // 所得税起征金额 
  char            chStatus;                   // 状态 字典[STATUS_S]
};

//-------------------------------10308065:所得税策略信息维护--------------------------
struct MATRADEAPI CReqFsQryTaxInfoMaintenanceField
{
  int             iSno;                       // 流水号 
};

struct MATRADEAPI CRspFsQryTaxInfoMaintenanceField
{
  int             iSno;                       // 流水号 
  char            szBeginAmt[21 + 1];         // 起始金额 
  char            szEndAmt[21 + 1];           // 结束金额 
  char            szTaxRate[21 + 1];          // 个人所得税率 
  char            szFreeAmt[21 + 1];          // 速算扣除数 
};

//-------------------------------10308058:要约收购转换关系表查询--------------------------
struct MATRADEAPI CReqFsQryTenderSgConverRelatField
{
  char            szStkbd[32 + 1];            // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 要约代码 
  char            szSgrCode[8 + 1];           // 收购人编码 
};

struct MATRADEAPI CRspFsQryTenderSgConverRelatField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 要约代码 
  char            szSgrCode[8 + 1];           // 收购人编码 
  char            szSgPrice[21 + 1];          // 收购价格 
  char            szPurchaserNane[32 + 1];    // 收购人名称 
  int             iTenderBegDate;             // 要约开始日期 
  int             iTenderEndDate;             // 要约结束日期 
};

//-------------------------------10308070:上海减持股份额度查询--------------------------
struct MATRADEAPI CReqFsQryshReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
};

struct MATRADEAPI CRspFsQryshReduceStkQuotaField
{
  int             iTrdDate;                   // 交易日期 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  LONGLONG        llMaxReductQty;             // 竞价最大减持数量 
  LONGLONG        llDzjyMaxReductQty;         // 大宗最大减持数量 
  LONGLONG        llTotalReductQty;           // 竞价累计减持数量 
  LONGLONG        llDzjyTotalReductQty;       // 大宗交易累计减持数量 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llStkConfirmQty;            // 证券认定数量1 
  LONGLONG        llStkConfirmQty2;           // 证券认定数量2 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llDzjyLimitQty;             // 大宗减持受让方限制数量 
  LONGLONG        llStkFrz;                   // 证券冻结数量 
  LONGLONG        llLimitAvlQty;              // 竞价减持剩余额度 
  LONGLONG        llDzjyNoLimitAvlQty;        // 大宗减持非受限剩余额度 
  LONGLONG        llDzjyLimitAvlQty;          // 大宗减持受限剩余额度 
};

//-------------------------------10308071:深圳减持股份额度查询--------------------------
struct MATRADEAPI CReqFsQryszReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkpbu[8 + 1];            // 交易单元 
};

struct MATRADEAPI CRspFsQryszReduceStkQuotaField
{
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szStkpbu[8 + 1];            // 交易单元 
  LONGLONG        llStkBln;                   // 证券余额 
  LONGLONG        llStkFrz;                   // 证券冻结数量 
  LONGLONG        llDzjyLimitQty;             // 受让方限制数量 
  LONGLONG        llDzjyReductQty;            // 仅能通过大宗减持 
  LONGLONG        llReductQty;                // 可竞价和大宗减持 
  LONGLONG        llStkBuyQty;                // 二级市场买入 
  LONGLONG        llReductLimit;              // 竞价减持额度 
  LONGLONG        llDzjyReductLimit;          // 大宗减持额度 
  LONGLONG        llInitDzjyLimitQty;         // 初始受让方限制数量 
  LONGLONG        llInitDzjyReductQty;        // 初始仅能通过大宗减持 
  LONGLONG        llInitReductQty;            // 初始可竞价和大宗减持 
  LONGLONG        llInitStkBuyQty;            // 初始二级市场买入 
  LONGLONG        llInitReductLimit;          // 初始竞价减持额度 
  LONGLONG        llInitDzjyReductLimit;      // 初始大宗减持额度 
  LONGLONG        llDzjyLimitQtyInclfrz;      // 受让方限制数量(含冻结) 
  LONGLONG        llDzjyReductQtyInclfrz;     // 仅能通过大宗减持(含冻结) 
  LONGLONG        llReductQtyInclfrz;         // 可竞价和大宗减持(含冻结) 
  LONGLONG        llStkBuyQtyInclfrz;         // 二级市场买入(含冻结) 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llLimitAvlQty;              // 竞价减持剩余额度 
  LONGLONG        llDzjyNoLimitAvlQty;        // 大宗减持非受限剩余额度 
  LONGLONG        llDzjyLimitAvlQty;          // 大宗减持受限剩余额度 
};

//-------------------------------10308084:权证信息查询--------------------------
struct MATRADEAPI CReqFsQryWarrantInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szWrntCode[8 + 1];          // 权证代码 
  char            szWrntExeCode[8 + 1];       // 行权代码 
};

struct MATRADEAPI CRspFsQryWarrantInfoField
{
  char            chStkex;                    // 交易市场 
  char            szStkbd[2 + 1];             // 交易板块 
  char            szWrntCode[8 + 1];          // 权证代码 
  char            chWrntCls;                  // 权证类别 字典[WRNT_CLS]
  char            szWrntStkCode[8 + 1];       // 标的代码 
  char            szWrntExeCode[8 + 1];       // 行权代码 
  char            chWrntExeMode;              // 行权模式 字典[WRNT_EXE_MODE]
  char            szWrntExePrice[21 + 1];     // 行权价格 
  char            szWrntExeRatio[21 + 1];     // 行权比例 
  int             iWrntExeBgnDate;            // 行权开始日期 
  int             iWrntExeEndDate;            // 行权结束日期 
  char            chWrntSettMode;             // 结算方式 字典[WRNT_SETT_MODE]
  char            szWrntSettPrice[21 + 1];    // 结算价格 
  char            szWrntTrdbleVol[21 + 1];    // 流通总额 
  int             iWrntExpireDate;            // 到期日期 
  char            szWrntCreateCode[8 + 1];    // 创设代码 
};

//-------------------------------10308079:三方回购篮子信息查询--------------------------
struct MATRADEAPI CReqFsQryThreeRePurchaseInfoField
{
  char            szBasketId[8 + 1];          // 篮子编号 
  char            szBondCode[8 + 1];          // 债券代码 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 输入值为记录号与查询方向配合使用
  int             iQryNum;                    // 查询行数 最大值为1000
};

struct MATRADEAPI CRspFsQryThreeRePurchaseInfoField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szBasketId[8 + 1];          // 篮子编号 
  char            szBasketName[20 + 1];       // 篮子名称 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szBondCode[8 + 1];          // 债券代码 
  char            szBasketRate[21 + 1];       // 折扣率 
  int             iUpdDate;                   // 更新日期 
};

//-------------------------------10308080:三方回购质押表查询--------------------------
struct MATRADEAPI CReqFsQryThreeRePurchasepledgeField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szQryPos[32 + 1];           // 定位串 输入值为记录号与查询方向配合使用
  int             iQryNum;                    // 查询行数 最大值为1000
};

struct MATRADEAPI CRspFsQryThreeRePurchasepledgeField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  LONGLONG        llStkTotal;                 // 已入库数量 
  LONGLONG        llStkUse;                   // 入库已质押数量 
  LONGLONG        llStkAvl;                   // 入库拟质押数量 
  LONGLONG        llStkTrdUse;                // 当日已用 
  LONGLONG        llStkRealIn;                // 当日入库数量 
  LONGLONG        llStkRealOut;               // 当日出库数量 
  char            szRemark[128 + 1];          // 备注信息 
};

//-------------------------------10308081:深圳三方回购转发成交信息表查询--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseMatchInfoField
{
  int             iTrdDate;                   // 交易日期 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szSfcontractNo[32 + 1];     // 合约编号 
  char            szOpptStkpbu[8 + 1];        // 对手方席位 
  char            chStatus;                   // 状态 字典[STATUS_F]
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseMatchInfoField
{
  int             iTrdDate;                   // 交易日期 
  char            szOpptStkpbu[8 + 1];        // 交易单元（对手） 
  int             iSfTrdType;                 // 申报业务类别 字典[SF_TRD_TYPE]
  char            szOrderId[10 + 1];          // 合同序号 
  char            szSfcontractNo[32 + 1];     // 合约编号 
  char            szDueRate[21 + 1];          // 年利率 
  char            szRepchAmt[21 + 1];         // 购回金额 
  int             iRepchLimitDates;           // 回购期限 
  int             iBaskets;                   // 篮子数量 
  char            szBaskrtid[32 + 1];         // 篮子编号 
  int             iCodes;                     // 债券数量 
  char            szBondCodes[256 + 1];       // 换入证券 
  char            szCodeQty[256 + 1];         // 换入证券数量 
  char            szBondCode1[8 + 1];         // 质押债券1 
  char            szBondCodeQty1[20 + 1];     // 质押债券1数量 
  char            szBondCode2[8 + 1];         // 质押债券2 
  char            szBondCodeQty2[20 + 1];     // 质押债券2数量 
  char            szBondCode3[8 + 1];         // 质押债券3 
  char            szBondCodeQty3[20 + 1];     // 质押债券3数量 
  char            szBondCode4[8 + 1];         // 质押债券4 
  char            szBondCodeQty4[20 + 1];     // 质押债券4数量 
  char            szBondCode5[8 + 1];         // 质押债券5 
  char            szBondCodeQty5[20 + 1];     // 质押债券5数量 
  char            szHcBondCode[8 + 1];        // 解押债券 
  char            szHcCodeQty[20 + 1];        // 解押债券数量 
  char            chStatus;                   // 状态 字典[STATUS_F]
  char            szRemark[1024 + 1];         // 备注 
};

//-------------------------------10308082:深圳三方回购合约总表查询--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseContTotalField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_C]
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseContTotalField
{
  int             iTrdDate;                   // 交易日期 
  int             iMatchDate;                 // 成交日期 
  char            szSfcontractNo[32 + 1];     // 合约编号 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkPbu[8 + 1];            // 交易单元 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  int             iRepchDate;                 // 回购日期 
  int             iEndDate;                   // 到期结算日期 
  int             iRepchLimitDates;           // 回购期限 
  char            szMatchAmt[21 + 1];         // 初始交易金额 
  char            szRepchAmt[21 + 1];         // 回购金额 
  char            szDueRate[21 + 1];          // 回购利率 
  char            szDueIntr[21 + 1];          // 回购利息 
  int             iRealRepchDate;             // 实际购回日期 
  char            szRealRepchAmt[21 + 1];     // 实际购回金额 
  int             iRealHoldDates;             // 实际占款天数 
  char            szRealDueRate[21 + 1];      // 实际购回利率 
  char            szRealDueIntr[21 + 1];      // 实际购回利息 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_F]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szRemark[1024 + 1];         // 备注 
};

//-------------------------------10308083:深圳三方回购合约明细查询--------------------------
struct MATRADEAPI CReqFsQryszThreeRePurchaseContDetailField
{
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szSfcontractNo[32 + 1];     // 合约编号 
  char            szQryPos[32 + 1];           // 定位串 输入值为记录号与查询方向配合使用
  int             iQryNum;                    // 查询行数 最大值为1000
};

struct MATRADEAPI CRspFsQryszThreeRePurchaseContDetailField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iTrdDate;                   // 交易日期 
  int             iMatchDate;                 // 成交日期 
  char            szSfcontractNo[32 + 1];     // 合约编号 
  int             iItemno;                    // 合约序号 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkPbu[8 + 1];            // 交易单元 
  char            szOrderId[10 + 1];          // 合同序号 
  char            szMatchId[16 + 1];          // 成交编号 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szBasketId[8 + 1];          // 篮子编号 
  char            szBasketRate[21 + 1];       // 篮子折扣率 
  char            szStkCode[8 + 1];           // 证券代码 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkProp[2 + 1];           // 股份性质 
  char            szBonusAmt[21 + 1];         // 红利金额 
  LONGLONG        llMatchQty;                 // 质押数量 
  LONGLONG        llBackqty;                  // 解押数量 
  LONGLONG        llBackqtyReal;              // 当天解押数量 
  char            chContractType;             // 合约类型 字典[CONTRACT_TYPE]
  char            chRepchStatus;              // 回购状态 字典[REPCH_STATUS_F]
  char            szOpptStkpbu[8 + 1];        // 交易单元(对手) 
  char            szOpptTrdacct[20 + 1];      // 交易账户(对手) 
  char            szRemark[1024 + 1];         // 备注 
};

//-------------------------------10308104:H股全流通股东账户信息查询--------------------------
struct MATRADEAPI CReqFsQryHStkFullCirAcctInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
};

struct MATRADEAPI CRspFsQryHStkFullCirAcctInfoField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szTrdacct[20 + 1];          // 交易账户 
  char            szStkCode[8 + 1];           // 证券代码 
  char            szSellApprovalSn[50 + 1];   // 减持外汇批文号 
  char            szBuyApprovalSn[50 + 1];    // 增持外汇批文号 
  LONGLONG        llAssetJcInit;              // 减持股份初始额度 
  LONGLONG        llAssetZcInit;              // 增持股份初始额度 
  LONGLONG        llAssetJcBln;               // 减持股份额度余额 
  LONGLONG        llAssetZcBln;               // 增持股份额度余额 
  LONGLONG        llAssetJcAvl;               // 减持股份额度可用 
  LONGLONG        llAssetZcAvl;               // 增持股份额度可用 
  int             iEffectDateJc;              // 减持额度生效时间 
  int             iEffectDateZc;              // 增持额度生效时间 
};

//-------------------------------10308105:H股境外证券代码对应关系查询--------------------------
struct MATRADEAPI CReqFsQryHStkForTrdIdCorrespondField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkCodeI[8 + 1];          // 境内证券代码 
  char            szStkCodeO[8 + 1];          // 境外证券代码 
};

struct MATRADEAPI CRspFsQryHStkForTrdIdCorrespondField
{
  char            chStkex;                    // 交易市场 字典[STKEX]
  char            szStkCodeI[8 + 1];          // 境内证券代码 
  char            szStkCodeO[8 + 1];          // 境外证券代码 
};

//-------------------------------10308106:询价交易非公开报价行情查询--------------------------
struct MATRADEAPI CReqFsQryInquireTransferCloseMarketInfoField
{
  char            szBjType[4 + 1];            // 报价类型 字典[INQUIRY_QUOTE_TYPE]
  char            chQuoteStatus;              // 状态 字典[INQUIRY_QUOTE_STATUS]
  char            chSide;                     // 买卖方向 字典[INQUIRY_SIDE]
};

struct MATRADEAPI CRspFsQryInquireTransferCloseMarketInfoField
{
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szBjType[4 + 1];            // 报价类型 字典[INQUIRY_QUOTE_TYPE]
  char            szJysOrderId[16 + 1];       // 交易所委托编号 
  char            szConferNum[16 + 1];        // 要约号 
  char            chQuoteStatus;              // 状态 字典[INQUIRY_QUOTE_STATUS]
  char            szCompanyName[16 + 1];      // 报价方交易商简称 
  char            chSide;                     // 买卖方向 字典[INQUIRY_SIDE]
  char            szPrice[21 + 1];            // 价格 
  int             iInquiryTime;               // 询价时间 
  char            szStkCode[8 + 1];           // 债券代码 
  LONGLONG        llStkQty;                   // 债券数量 
};

//-------------------------------10418001:历史成交查询--------------------------
struct MATRADEAPI CReqFsQryHisFillField
{
  LONGLONG        llCustCode;                 // 客户代码 客户代码及证券账户不能同时为空
  LONGLONG        llCuacctCode;               // 资金账号 客户代码及证券账户不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[10 + 1];          // 证券代码 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
};

struct MATRADEAPI CRspFsQryHisFillField
{
  char            szQryPos[32 + 1];           // 定位串 
  char            szMatchedTime[8 + 1];       // 成交时间 
  LONGLONG        llOrderSn;                  // 委托序号 
  int             iOrderBsn;                  // 委托批号 
  char            szOrderId[32 + 1];          // 合同序号 
  int             iIntOrg;                    // 内部机构 
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chDlvyCurrency;             // 交收币种 字典[CURRENCY]
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szMatchSn[20 + 1];          // 成交编号 
  char            szMatchPrice[21 + 1];       // 成交价格 
  LONGLONG        llMatchQty;                 // 已成交数量 
  char            szMatchAmt[21 + 1];         // 已成交金额 
  LONGLONG        llMatchCnt;                 // 成交笔数 
};

//-------------------------------10418002:历史委托查询--------------------------
struct MATRADEAPI CReqFsQryHisOrderField
{
  LONGLONG        llCuacctCode;               // 资金账号 资金账户及证券账户不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 资金账户及证券账户不能同时为空
  char            szStkCode[10 + 1];          // 证券代码 
  int             iIntOrg;                    // 内部机构 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
  int             iBgnDate;                   // 开始日期 交易日期TRD_DATE
  int             iEndDate;                   // 结束日期 交易日期TRD_DATE
};

struct MATRADEAPI CRspFsQryHisOrderField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iIntOrg;                    // 内部机构 
  int             iTrdDate;                   // 交易日期 
  int             iOrderDate;                 // 委托日期 
  char            szOrderId[32 + 1];          // 合同序号 
  char            chOrderStatus;              // 委托状态 字典[ORDER_STATUS]
  LONGLONG        llCustCode;                 // 客户代码 
  LONGLONG        llCuacctCode;               // 资产账户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szStkpbu[8 + 1];            // 交易单元 
  char            szTrdacct[20 + 1];          // 证券账户 
  int             iStkBiz;                    // 证券业务 字典[STK_BIZ]
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            chCurrency;                 // 货币代码 字典[CURRENCY]
  char            szOrderPrice[21 + 1];       // 委托价格 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderAmt[21 + 1];         // 委托金额 
  char            szOrderFrzAmt[21 + 1];      // 委托冻结金额 
  char            szOrderUfzAmt[21 + 1];      // 委托解冻金额 
  LONGLONG        llWithdrawnQty;             // 已撤单数量 
  LONGLONG        llMatchedQty;               // 已成交数量 
  char            chIsWithdraw;               // 撤单标志 字典[IS_WITHDRAW]
  char            chIsWithdrawn;              // 已撤单标志 字典[IS_WITHDRAWN]
  int             iOrderBsn;                  // 委托批号 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szMatchedAmt[21 + 1];       // 已成交金额 
  char            szSettAmt[21 + 1];          // 清算金额 
};

//-------------------------------10418003:历史交割单查询--------------------------
struct MATRADEAPI CReqFsQryHisDeliOrderField
{
  LONGLONG        llCustCode;                 // 客户代码 资金账户及证券账户不能同时为空
  LONGLONG        llCuacctCode;               // 资金账号 资金账户及证券账户不能同时为空
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            szTrdacct[20 + 1];          // 证券账户 
  char            szStkCode[10 + 1];          // 证券代码 
  char            chStkCls;                   // 证券类别 字典[STK_CLS]
  char            szStkbd[2 + 1];             // 交易板块 字典[STKBD]
  char            szQryPos[32 + 1];           // 定位串 第一次填空
  LONGLONG        llQryNum;                   // 查询行数 每次取的行数
  int             iBgnDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
};

struct MATRADEAPI CRspFsQryHisDeliOrderField
{
  char            szQryPos[32 + 1];           // 定位串 
  int             iSettDate;                  // 清算日期 
  int             iShareDlvyDate;             // 股份交收日期 
  char            szFundDlvyDate[21 + 1];     // 资金交收日期 
  char            szOrderTime[32 + 1];        // 委托时间 
  char            szStkBiz[10 + 1];           // 证券业务 
  LONGLONG        llCuacctCode;               // 资金帐户 
  char            szFortuneCode[20 + 1];      // 财富账户 
  char            chDlvyCurrency;             // 交收币种 字典[DLVY_CURRENCY]
  char            chStkex;                    // 市场 字典[STKEX]
  LONGLONG        llCustCode;                 // 客户代码 
  char            szCustName[32 + 1];         // 客户姓名 
  LONGLONG        llOrderId;                  // 合同序号 
  char            szStkCode[10 + 1];          // 证券代码 
  char            szStkName[16 + 1];          // 证券名称 
  char            szMatchTime[8 + 1];         // 成交时间 
  char            szMatchId[20 + 1];          // 成交编号 
  LONGLONG        llMatchCnt;                 // 成交笔数 
  LONGLONG        llMatchQty;                 // 成交数量 
  char            szMatchPrice[21 + 1];       // 成交价格 
  char            szMatchAmt[21 + 1];         // 成交金额 
  char            szSettAmt[21 + 1];          // 清算金额 
  char            szStampDuty[21 + 1];        // 印花税 
  char            szCommision[21 + 1];        // 手续费 
  char            szTransferFee[21 + 1];      // 过户费 
  char            szClearingFee[21 + 1];      // 清算费 
  char            szTrdRegFee[21 + 1];        // 交易规费 
  char            szHandleFee[21 + 1];        // 经手费 
  char            szSecuReguFee[21 + 1];      // 证管费 
  char            szFundBln[21 + 1];          // 资金余额 
  LONGLONG        llStkBln;                   // 股份余额 
  LONGLONG        llOrderQty;                 // 委托数量 
  char            szOrderPrice[21 + 1];       // 委托价格 
  char            szVentureFee[21 + 1];       // 风险金 
};

//-------------------------------00102023:成交回报推送------------------------------------
struct MATRADEAPI CRtnFsOrderFillField
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

//-------------------------------00102024:确认回报推送------------------------------------
struct MATRADEAPI CRtnFsOrderConfirmField
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

#if defined(OS_IS_LINUX)
#pragma pack()
#else
#pragma pack(pop)
#endif

END_NAMESPACE_MACLI

#endif  //__MA_CLI_FS_TRADE_API_STRUCT_H__