#if !defined(__MA_CLI_KSPB_TRADE_API_STRUCT_H__)
#define __MA_CLI_KSPB_TRADE_API_STRUCT_H__

#include "maCliTradeApi.h"

BGN_NAMESPACE_MACLI

#if defined(OS_IS_LINUX)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

//-------------------------------10101530:用户登陆--------------------------
struct MATRADEAPI CReqKspbRpcLoginField
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
struct MATRADEAPI CRspKspbRpcLoginField
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

//-------------------------------10101573:买卖委托--------------------------
struct MATRADEAPI CReqKspbRpcOrderField
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
  int           iCuacctSn;            // 账户序号
};
struct MATRADEAPI CRspKspbRpcOrderField
{
  int           iOrdersno;         // 委托序号
  char          szOrderid[10 + 1]; // 合同序号
  int           iOrdergroup;       // 委托批号
};

//-------------------------------10101574:委托撤单--------------------------
struct MATRADEAPI CReqKspbRpcOrderCancelField
{
  int           iOrderdate;        // 委托日期
  LONGLONG      llFundid;          // 资金帐户
  int           iOrdersno;         // 委托序号
  char          szBsflag[2 + 1];   // 买卖类别
  int           iCuacctSn;         // 账户序号
};
struct MATRADEAPI CRspKspbRpcOrderCancelField
{
  char          szMsgok[32 + 1];   // 成功信息
  char          chCancelStatus;    // 内部撤单标志
  int           iOrdersno;         // 撤单委托序号
};

//-------------------------------10101679:股份查询--------------------------
struct MATRADEAPI CReqKspbRpcQryStkDetailField
{
  char          chMarket;          // 交易市场
  LONGLONG      llFundid;          // 资金帐户
  char          szSecuid[10 + 1];  // 股东代码
  char          szStkcode[8 + 1];  // 证券代码
  char          chQryflag;         // 查询方向
  int           iCount;            // 请求行数
  char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspKspbRpcQryStkDetailField
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

//-------------------------------10101521:资金查询--------------------------
struct MATRADEAPI CReqKspbRpcQryFundField
{
  LONGLONG      llFundid;         // 资金账号
  char          chMoneytype;      // 货币
};
struct MATRADEAPI CRspKspbRpcQryFundField
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

//-------------------------------10101684:委托查询--------------------------
struct MATRADEAPI CReqKspbRpcQryOrderDetailField
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
  int           iCuacctSn;         // 账户序号
};
struct MATRADEAPI CRspKspbRpcQryOrderDetailField
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

//-------------------------------10101686:成交查询--------------------------
struct MATRADEAPI CReqKspbRpcQryMatchDetailField
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
  int           iCuacctSn;         // 账户序号
};
struct MATRADEAPI CRspKspbRpcQryMatchDetailField
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

//-------------------------------10101527:证券信息查询--------------------------
struct MATRADEAPI CReqKspbRpcStkInfoField
{
  char          chMarket;          // 交易市场
  char          chStklevel;        // 证券级别
  char          szStkcode[8 + 1];  // 证券代码
  char          szPoststr[32 + 1]; // 定位串
  int           iRowcount;         // 查询行数
  char          chStktype;         // 证券类别
};
struct MATRADEAPI CRspKspbRpcStkInfoField
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

//-------------------------------10101676:股东查询--------------------------
struct MATRADEAPI CReqKspbRpcQryTrdacctField
{
  LONGLONG      llFundid;          // 资金帐户
  char          chMarket;          // 交易市场
  char          szSecuid[10 + 1];  // 股东代码
  char          chQryflag;         // 查询方向
  int           iCount;            // 请求行数
  char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspKspbRpcQryTrdacctField
{
  char          szPoststr[32 + 1]; // 定位串
  LONGLONG      llCustid;          // 客户代码
  char          chMarket;          // 交易市场
  char          szSecuid[10 + 1];  // 股东代码
  char          szName[16 + 1];    // 股东姓名
  int           iSecuseq;          // 股东序号
  char          chRegflag;         // 指定交易状态
};

//-------------------------------10101500:银行信息查询--------------------------
struct MATRADEAPI CReqKspbRpcBankTransBasicInfoField
{
  char          chCurrency;        // 货币代码
  char          szBankCode[4+1];   // 银行代码
};
struct MATRADEAPI CRspKspbRpcBankTransBasicInfoField
{
  char          szBankCode[4+1];   // 银行代码
  char          chSourceType;      // 发起方
  char          chBankTranType;    // 转帐方式
  char          chFundPwdFlag;     // 资金密码校验
  char          chBankPwdFlag;     // 银行密码校验
  char          chCheckBankId;     // 银行帐号校验
  char          chCheckIdNo;       // 证件校验标志
  int           iIntOrg;           // 操作机构
  char          chCurrency;        // 货币代码
  char          chStatus;          // 银行状态
};

//-------------------------------10101501:银证转账--------------------------
struct MATRADEAPI CReqKspbRpcBankStkTransField
{
  LONGLONG      llCuacctCode;      // 资金账号
  char          chCurrency;        // 货币代码
  char          szFundPwd[32+1];   // 资金密码
  char          szBankCode[4+1];   // 银行代码
  char          szBankPwd[32+1];   // 银行密码
  char          chBankTranType;    // 转帐类型
  char          szTransAmt[21+1];  // 转帐金额
  char          chPwdFlag;         // 资金密码校验标志
};
struct MATRADEAPI CRspKspbRpcBankStkTransField
{
  int           iSNo;              // 委托序号
  int           iSysErrorId;       // 错误代码
  char          szErrorMsg[64+1];  // 错误信息
};

//-------------------------------10101503:转账流水查询--------------------------
struct MATRADEAPI CReqKspbRpcBankStkTransInfoField
{
  LONGLONG      llCuacctCode;       // 资金账号
  char          chCurrency;         // 货币代码
  char          szSNo[32 + 1];      // 转帐序号
};
struct MATRADEAPI CRspKspbRpcBankStkTransInfoField
{
  int           iOperDate;           // 转帐日期
  int           iOperTime;           // 转帐时间
  LONGLONG      llCuacctCode;        // 资金账号
  char          chCurrency;          // 货币代码
  LONGLONG      llCustCode;          // 客户代码
  char          szBankCode[4 + 1];   // 银行代码
  char          chBankTranId;        // 业务类型
  int           iSNo;                // 合同序号
  LONGLONG      llFundEffect;        // 委托金额
  LONGLONG      llFundBal;           // 余额   
  char          szRemark[32 + 1];    // 备注信息
  char          chStatus;            // 处理结果
  char          chSourceType;        // 发起方向
  char          szBankMsgId[16 + 1]; // 外部信息代码
  char          szBankMsg[64 + 1];   // 外部信息内容
  char          szErrorMsg[64 + 1];  // 系统错误信息
  int           iSysErrId;           // 系统错误代码
  char          szFundEffect[23 + 1];// 委托金额
  char          szFundBal[23 + 1];   // 余额
};

//-------------------------------10101509:历史委托查询--------------------------
struct MATRADEAPI CReqKspbRpcQryHistoryOrderDetailField
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
struct MATRADEAPI CRspKspbRpcQryHistoryOrderDetailField
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

//-------------------------------10101510:历史成交查询--------------------------
struct MATRADEAPI CReqKspbRpcQryHistoryMatchDetailField
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
struct MATRADEAPI CRspKspbRpcQryHistoryMatchDetailField
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

//-------------------------------10101506:资金股份流水查询--------------------------
struct MATRADEAPI CReqKspbRpcStatementField
{
  int           iStrDate;              // 起始日期
  int           iEndDate;              // 终止日期
  LONGLONG      llCuacctCode;          // 资金帐户
  char          chCurrency;            // 货币代码
  char          chPrintFlag;           // 重打标识
  char          chQryFlag;             // 查询方向
  int           iQryNum;               // 请求行数
  char          szQryPos[32 + 1];      // 定位串
};
struct MATRADEAPI CRspKspbRpcStatementField
{
  char          szQryPos[32 + 1];      // 定位串  
  int           iClearDate;            // 清算日期
  int           iBizDate;              // 交收日期
  int           iOrderDate;            // 发生日期
  int           iOrderTime;            // 发生时间
  int           iDigestId;             // 业务代码
  char          szDigestName[16 + 1];  // 业务说明
  LONGLONG      llCustId;              // 客户代码
  char          szCustName[16 + 1];    // 客户姓名
  char          szIntOrg[4 + 1];       // 机构代码
  LONGLONG      llCuacctCode;          // 资金帐号
  char          chCurrency;            // 货币代码
  char          chStkex;               // 市场代码
  char          szTrdacct[32 + 1];     // 股东代码
  LONGLONG      llFundEffect;          // 资金发生数
  LONGLONG      llFundBal;             // 资金本次余额
  int           iStkBal;               // 股份本次余额
  char          szOrderId[10 + 1];     // 合同序号
  char          szStkCode[16 + 1];     // 证券代码
  char          szStkName[16 + 1];     // 证券名称
  char          szBsFlag[2 + 1];       // 买卖类别
  int           iMatchedQty;           // 成交数量
  LONGLONG      llMatchedPrice;        // 成交价格
  LONGLONG      llMatchedAmt;          // 成交金额
};

//-------------------------------10101517:交割单查询--------------------------
struct MATRADEAPI CReqKspbRpcQryDeliOrderField
{
  int           iStrdate;          // 起始日期
  int           iEnddate;          // 终止日期
  LONGLONG      llFundid;          // 资金帐户
  char          chPrintflag;       // 重打标识
  char          chQryflag;         // 查询方向
  int           iCount;            // 请求行数
  char          szPoststr[32 + 1]; // 定位串
};
struct MATRADEAPI CRspKspbRpcQryDeliOrderField
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

//-------------------------------00102023:成交回报推送------------------------------------
struct MATRADEAPI CRtnKspbRpcOrderFillField
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
struct MATRADEAPI CRtnKspbRpcOrderConfirmField
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

#endif  //__MA_CLI_KSPB_TRADE_API_STRUCT_H__