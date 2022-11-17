//----------------------------------------------------------------------------
// 版权声明：本程序模块属于金证微内核架构平台(KMAP)的一部分
//           金证科技股份有限公司  版权所有
//
// 文件名称：maMktData.h
// 模块名称：微架构客户端(Client)的行情结构定义
// 模块描述：
// 开发作者：何万刚
// 创建日期：2015-07-17
// 模块版本：1.3
//----------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//----------------------------------------------------------------------------
// 2015-07-17    1.0          何万刚          初创
// 2016-03-18    1.0          高剑            新增ST_MKT_DATA2，ST_MKT_DATA3成交量，成交金额字段，
//                                            出现空字符串时，无法反序列化，重写反序列化实现
// 2016-08-16    1.0          高剑            ST_MKT_DATA2增加昨日收盘价，以及4个预留字段
// 2016-12-16    1.1          高剑            增加ST_MKT_DATA字段的描述
// 2018-03-08    1.2          高剑            增加ST_MKT_MATCH逐笔、ST_MKT_ORDER委托结构体,ST_MKT_INDEX指标结构体
//                                            行情订阅启用DATA_SET入参,2(原有行情订阅),8(成交订阅),16(委托订阅),32(指标订阅),入参为空向前兼容,订阅多种取值用或
// 2019-02-12    1.3          张东方          支持L2行情源数据
// 2019-09-06    1.4          林振益          ST_MKT_DATA2增加开盘价uiOpenPrice，启用uiReserved1~4，并新增uiReserved5~8
// 2020-02-28    1.5          范国政          行情指标ST_MKT_INDEX的uiValues和uiExtra数据类型从unsigned int改为unsigned long long
// 2021-05-10    1.6          卜文军          修改ST_MKT_DATA,ST_MKT_DATA2涨停价跌停价类型从unsigned int改为long long;
// 2021-12-17    1.7          卜文军          修改ST_MKT_DATA预留字段llReserved增加到10个;
//----------------------------------------------------------------------------
#if !defined(__MA_MKT_DATA_H__)
#define __MA_MKT_DATA_H__

#include <stdio.h>

#if defined(WIN32) || defined(WIN64) || defined(OS_IS_WINDOWS)
  #if defined(MA_CLIAPI_EXPORTS)
    #define MACLIAPI __declspec(dllexport)
  #else
    #define MACLIAPI __declspec(dllimport)
  #endif
  #define MACLI_STDCALL __stdcall
  #define MACLI_EXPORTS __declspec(dllexport)
#else
  #define MACLIAPI
  #define MACLI_STDCALL
  #define MACLI_EXPORTS
#endif

#if defined (_MSC_VER) && (_MSC_VER == 1200)
  #define FORMAT_LONGLONG "%I64d"
  #if defined(WIN32) || defined(WIN64)
  typedef __int64 LONGLONG;
  #endif
#else
  #define FORMAT_LONGLONG "%lld"
  typedef long long LONGLONG;
#endif

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
// llMktDataTime:行情服务器的接收时间,格式YYYYMMDDHHMMSSsss
// chExchange   :交易所，0上交所、1深交所、A上期所、B大商所、C郑商所、G中金所
// iTrdDate     :行情数据本身的日期,格式YYYYMMDD
// iDataTime    :行情数据本身的时间,格式HHMMSSsss
// uiMatchedCnt :笔数,无小数部分
// szStatus     :品种状态
//               '0' 未设置
//               'Y' 新产品
//               'R' 交易间，禁止任何交易活动
//               'P' 休市，例如，午餐休市。无撮合市场内部信息披露。
//               'B' 停牌
//               'C' 收市
//               'D' 停牌
//               'Z' 产品待删除
//               'V' 结束交易
//               'T' 固定价格集合竞价
//               'W' 暂停，除了自有订单和交易的查询之外，任何交易活动都被禁止。
//               'X' 停牌,( 'X'和'W'的区别在于'X'时可以撤单)
//               'I' 盘中集合竞价。
//               'L' 盘中集合竞价PreOBB
//               'I' 开市集合竞价
//               'K' 开市集合竞价订单溥平衡(OBB)前期时段
//               'S' 非交易服务支持
//               'U' 盘后处理
//               'F' 盘前处理
//               'E' 启动
//               'O' 连续撮合
//               'Q' 连续交易和集合竞价的波动性中断
//               'J' 收盘集合竞价
//               'M' 可恢复交易熔断时段(盘中集合竞价)
//               'N' 不可恢复交易的熔断时段(暂停交易至闭市)
//
// 价格         :元,4位小数,实际价格10000倍
// 量、成交量   :股票为股数,期权期货为手数,均无小数部分
// 金额         :元,无小数部分
// 市盈率等其它字段,格式待定
//////////////////////////////////////////////////////////////////////////
//行情数据(快照)
struct MACLIAPI ST_MKT_DATA
{
  bool      bInitialized;
  LONGLONG  llMktDataTime;                    // 接收时间(YYYYMMDDHHMMSSmmm)
  char      chExchange;                       // 交易所
  char      szTrdCode[16 + 1];                // 品种代码
  int       iTrdDate;                         // 行情日期
  int       iDataTime;                        // 行情时间(HHMMSSmmm)
  char      szStatus[1+1];                    // 品种状态

  unsigned int    uiPreClosePrice;            // 昨收盘价
  unsigned int    uiOpenPrice;                // 开盘价
  unsigned int    uiHighPrice;                // 最高价
  unsigned int    uiLowPrice;                 // 最低价
  unsigned int    uiCurPrice;                 // 最新价
  unsigned int    uiAnsPrice[10];             // 申卖价
  LONGLONG        llAnsVol[10];               // 申卖量
  unsigned int    uiBidPrice[10];             // 申买价
  LONGLONG        llBidVol[10];               // 申买量
  unsigned int    uiMatchedCnt;               // 成交笔数
  LONGLONG        llMatchedVol;               // 成交总量
  LONGLONG        llMatchedAmtTot;            // 成交总金额
  LONGLONG        llBidVolTot;                // 买入总量
  LONGLONG        llAnsVolTot;                // 卖出总量
  unsigned int    uiBidPriceAvg;              // 平均委买价格
  unsigned int    uiAnsPriceAvg;              // 平均委卖价格
  int             iIopv;                      // IOPV
  int             iYield;                     // 到期收益率
  long long       uiHighLmtPrice;             // 涨停价
  long long       uiLowLmtPrice;              // 跌停价
  char            szStkPrefix[16 + 1];        // 证券前缀
  LONGLONG        llPreOpenVol;               // 昨持仓量
  unsigned int    uiSettPrice;                // 今结算价
  int             iPreDelta;                  // 昨虚实度
  int             iCurrDelta;                 // 今虚实度
  int             iNsyl1;                     // 市盈率1
  int             iNsyl2;                     // 市盈率2
  int             iNsd2;                      // 升跌2
  LONGLONG        llOpenVol;                  // 持仓总量
  unsigned int    uiPreSettPrice;             // 昨结算价
  unsigned int    uiClosePrice;               // 今收盘
  char            szContractID[32 +1];        // 期权合约代码
  char            szUnderlyingSecurityID[32]; // 标的证券代码
  char            szCallOrPut[1+1];           // 认购认沽C1
  int             iExerciseDate;              // 期权行权日，YYYYMMDD
  char            szCallOrPutStatus[1+1];     //

  long long       uiAuctionPrice;             // 上海动态参考价格，深圳熔断价格（期权）
  unsigned long long ullAuctionQty;           // 上海虚拟匹配数量（期权）
  unsigned long long ullSecuritySwitchType;   // 深圳证券实时状态
  char            szTradingPhaseCode[8+1];    // 产品实时阶段及标志
  long long       llReserved[10];             // 预留字段

  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//行情数据(推送)
struct MACLIAPI ST_MKT_DATA2
{
  bool            bInitialized;
  LONGLONG        llMktDataTime;              // 接收时间(YYYYMMDDHHMMSSmmm)
  char            chExchange;                 // 交易所
  char            szTrdCode[16 + 1];          // 品种代码
  int             iTrdDate;                   // 行情日期
  int             iDataTime;                  // 行情时间(HHMMSSmmm)
  char            szStatus[1+1];              // 品种状态

  unsigned int    uiCurPrice;                 // 最新价（价格*10000)
  unsigned int    uiAnsPrice[10];             // 申卖价（价格*10000)
  LONGLONG        llAnsVol[10];               // 申卖量
  unsigned int    uiBidPrice[10];             // 申买价（价格*10000)
  LONGLONG        llBidVol[10];               // 申买量
  unsigned int    uiMatchedCnt;               // 成交笔数
  LONGLONG        llMatchedVol;               // 成交量
  LONGLONG        llMatchedValue;             // 成交金额

  unsigned int    uiPreClosePrice;            // 昨收盘价
  unsigned int    uiReserved1;                // 最高价
  unsigned int    uiReserved2;                // 最低价
  long long       uiReserved3;                // 涨停价
  long long       uiReserved4;                // 跌停价
  unsigned int    uiReserved5;                // 预留5
  unsigned int    uiReserved6;                // 预留6
  unsigned int    uiReserved7;                // 预留7
  unsigned int    uiReserved8;                // 预留8
  unsigned int    uiOpenPrice;                // 开盘价

  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//行情数据(预留)
struct MACLIAPI ST_MKT_DATA3
{
  bool            bInitialized;
  LONGLONG        llMktDataTime;              // 接收时间(YYYYMMDDHHMMSSmmm)
  char            chExchange;                 // 交易所
  char            szTrdCode[16 + 1];          // 品种代码
  int             iTrdDate;                   // 行情日期
  int             iDataTime;                  // 行情时间(HHMMSSmmm)
  char            szStatus[1+1];              // 品种状态

  unsigned int    uiCurPrice;                 // 最新价（价格*10000)
  unsigned int    uiAnsPrice[5];              // 申卖价（价格*10000)
  LONGLONG        llAnsVol[5];                // 申卖量
  unsigned int    uiBidPrice[5];              // 申买价（价格*10000)
  LONGLONG        llBidVol[5];                // 申买量
  unsigned int    uiMatchedCnt;               // 成交笔数
  LONGLONG        llMatchedVol;               // 成交量
  LONGLONG        llMatchedValue;             // 成交金额
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//分时成交
struct MACLIAPI ST_MKT_MATCH
{
  bool           bInitialized;
  char           chExchange;             // 交易所
  char           szTrdCode[16 + 1];      // 品种代码
  int            iTrdDate;               // 行情日期
  char           chType;                 // 类型:0多换1空换2双换;3多开4空开5双开;6多平7空平8双平
  char           chBSSide;               // 买卖方向:0:买 1:卖 其它:
  int            iDataDate;              // 数据日期
  int            iDataTime;              // 行情时间
  int            iIopv;                  // (Indicative Optimized Portfolio Value)ETF的基金份额参考净值,由交易所每15秒计算并公告一次,实际价格10000倍
  unsigned int   uiPrice;                // 成交价
  int            iOpenVol;               // 持仓增量
  unsigned int   uiMatchedCnt;           // 成交笔数
  unsigned int   uiMatchedVol;           // 成交量
  LONGLONG       llMatchedAmtTot;        // 成交金额
  int            iTimediff;              // 计算此笔成交的两笔快照时间差 越小越准确
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//逐笔委托
struct MACLIAPI ST_MKT_ORDER
{
  bool           bInitialized;
  char           chExchange;             // 交易所
  char           szTrdCode[16 + 1];      // 品种代码
  int            iTrdDate;               // 行情日期
  char           chOrderDirection;       // 委托方向 B, S, C
  int            iDataDate;              // 委托日期
  int            iDataTime;              // 委托时间
  int            iOrderNo;               // 委托号
  unsigned int   uiOrderPrice;           // 委托价格
  int            iOrderVol;              // 委托数量
  int            iIndex;                 // 委托编号，从1开始，递增1
  char           chOrderKind;            // 委托类型, 1=市价 2=限价 U=本方最优
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//最多10 条指标线
#define INDEXLINE_NUM 10

//行情指标
struct MACLIAPI ST_MKT_INDEX
{
  bool                bInitialized;
  char                chExchange;         // 交易所
  char                szTrdCode[16 + 1];  // 品种代码
  int                 iTrdDate;           // 行情日期
  int                 iDataDate;          // 指标日期
  int                 iDataTime;          // 指标时间
  int                 iIndexID;           // 指标ID
  int                 iKLinePeriod;       // K线周期
  long long           llValues[INDEXLINE_NUM];    // 指标线的值
  char                szExtra[20*INDEXLINE_NUM+1];// 可能用的额外字符串
  long long           llExtra[5*INDEXLINE_NUM];   // 可能的额外数据
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//委托队列
struct MACLIAPI ST_MKT_ORDER_QUEUE
{
  bool           bInitialized;
  char           chExchange;             // 交易所
  char           szTrdCode[16 + 1];      // 品种代码
  int            iTrdDate;               // 行情日期
  int            iDataDate;              // 委托日期
  int            iDataTime;              // 委托时间
  int            iSide;                  // 买卖方向
  unsigned int   iPrice;                 // 委托价格
  int            iOrderItems;            // 订单数量（该档位上的总委托数）
  int            iABItems;               // 明细个数（该档位上揭示的委托数）
  int            iABVolum[256];          // 订单明细
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//逐笔成交
struct MACLIAPI ST_MKT_TRANSACTION
{
  bool           bInitialized;
  char           chExchange;             // 交易所
  char           szTrdCode[16 + 1];      // 品种代码
  int            iTrdDate;               // 行情日期
  int            iDataDate;              // 数据日期
  int            iDataTime;              // 行情时间
  int            iIndex;                 // 成交编号(从1开始，递增1)
  char           chFunctionCode;         // 成交代码: 'C', 0
  char           chOrderKind;            // 成交类型
  char           chBSSide;               // 买卖方向:0:买 1:卖 其它:
  unsigned int   uiPrice;                // 成交价
  unsigned int   uiMatchedVol;           // 成交量
  int            iAskOrderNo;            // 叫卖序号
  int            iBidOrderNo;            // 叫买序号
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char * p_szFmtMktDataStr);
};

//千档行情
#define THOUNSAND_DATA_NUM    500
#define TSD_SINGLE_QUEUE_SIZE 200

//千档中一档的数据
struct MACLIAPI ST_MKT_DATA_TSD_SINGLE
{
  unsigned int uiPrice;     // 价
  int          iVol;        // 量
  int          iQueueSize;  // 队列长度
  int          iQueues[TSD_SINGLE_QUEUE_SIZE];  // 委托队列
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char *p_szFmtMktDataStr, int p_iPos);
};

//千档行情
struct MACLIAPI ST_MKT_THOUNSAND_DATA
{
  bool         bInitialized;
  char         chExchange;             // 交易所
  char         szTrdCode[16 + 1];      // 品种代码
  long long    llMktDataTime;          // 行情时间
  int          iDataDate;              // 数据日期
  int          iDataTime;              // 数据时间
  int          iBidNum;                // 买的档数
  ST_MKT_DATA_TSD_SINGLE stBid[THOUNSAND_DATA_NUM]; // 数据
  int          iOffNum;                // 卖的档数
  ST_MKT_DATA_TSD_SINGLE stOff[THOUNSAND_DATA_NUM]; // 数据
  int Serialize(char *p_szFmtMktDataStr, int p_iFmtMktDataStrLen);
  int DeSerialize(char *p_szFmtMktDataStr);
};

struct ST_EXCHANGE_CVT
{
  char chExchange;
  char szHongHuiType[3+1];
  char szTsType[2+1];
};

#pragma pack()

const ST_EXCHANGE_CVT g_stExchangeCvt[] =
{
  {'0', "SZ",  "SZ"},   // 深圳证券交易所
  {'1', "SH",  "SH"},   // 上海证券交易所
  {'A', "SHF", "SQ"},   // 上海期货交易所
  {'B', "DCE", "DL"},   // 大连期货交易所
  {'C', "CZC", "ZZ"},   // 郑州期货交易所
  {'G', "CF",  "ZJ"},   // 中国金融期货交易所
  {'7', "HK",  "HK"},   // 香港交易所
  {'2', "BJ",  "BJ"},   // 股转证券交易所
  {'I', "NY",  "NY"},   // 能源行情交易所
  {'3', "BJ",  "BJ"},   // 北京证券交易所
};

#endif  // __MA_MKT_DATA_H__
