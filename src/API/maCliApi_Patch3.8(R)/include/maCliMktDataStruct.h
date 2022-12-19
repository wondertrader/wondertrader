#if !defined(__MA_CLI_MKT_DATA_STRUCT_H__)
#define __MA_CLI_MKT_DATA_STRUCT_H__

#include "maCliTradeApi.h"

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

BGN_NAMESPACE_MACLI

// 行情回放
struct MACLIAPI CReqPlayBackField
{
  char chPlayCtrl;        //播放控制  PLAY_CTRL CHAR(1) √  9080  ‘0’—停止播放 ‘1’—开始播放
  char szTrdDate[8+1];    //TRD_DATE              8844
  int  iBgnTime;          //BGN_TIME              916
  int  iEndTime;          //END_TIME              917
  char chPlayBackType;    //PLAY_BACK_TYPE        8996
  int  iPlayInterval;     //回放时间间隔  PLAY_INTERVAL INTEGER X 9081  播放单位行情时间后的间隔时间(PLAY_INTERVAL以毫秒为单位)
  char szStkCode[128+1];  //股票代码  STK_CODE  VARCHAR(128)  X 48  播放行情代码用逗号（，）分割，如000001,000002,000003….
};

struct MACLIAPI CRspPlayBackField
{
};

//自选股编辑
struct MACLIAPI CReqMktDataPortfolioField
{
  char     szUserCode[16+1]; //用户代码 USER_CODE VARCHAR(16) Y 8902 cos用户代码
  char     chStkex;          //交易所   STKEX     CHAR(1)     Y 207
  char     szStkbd[2+1];     //板块     STKBD     CHAR(2)     Y 625
  char     szTrdCodes[512+1];//品种代码 TRD_CODES VARCHAR(512)X 48  竖线分割(逐条添加或删除时必传)
  char     chOperType;       //操作类型 OPER_TYPE CHAR(1)     Y 8844;0:添加；1:逐条删除；2:范围删除；其它为参数错误
  char     szBegCode[8+1];   //开始代码 BEG_CODE  VARCHAR(8)  X 9000范围删除时必传(此范围包含开始代码和结束代码本身)
  char     szEndCode[8+1];   //结束代码 END_CODE  VARCHAR(8)  X 9001范围删除时必传
};

struct MACLIAPI CRspMktDataPortfolioField
{
  int      iAmount;          //数量AMOUNTINTEGER8900返回插入或删除的数量
};

//自选股查询
struct MACLIAPI CReqMktDataPortfolioInfoField
{
  char     szUserCode[16+1]; //用户代码 USER_CODE  VARCHAR(16) Y 8902 cos用户代码
  char     chStkex;         //交易所    STKEX      CHAR(1)     Y 207
  char     szStkbd[2+1];    //板块      STKBD      CHAR(2)     Y 625
  char     chQueryFlag;     //查询方式  QUERY_FLAG CHAR(1)     Y 9080;0：按位置向后查询1：按位置向前查询2：按范围查询其它参数错误
  char     szStkCode[8+1];  //查询位置  STK_CODE   VARCHAR(8)  X 8991按位置查询时必传
  int      iQueryNum;       //查询行数  QUERY_NUM  INTEGER     X 8992按位置查询时必传，最多1000条
  char     szBegCode[8+1];  //开始代码  BEG_CODE   VARCHAR(8)  X 9000范围查询时必传(此范围包含开始代码和结束代码本身)
  char     szEndCode[8+1];  //结束代码  END_CODE   VARCHAR(8)  X 9001范围查询时必传
};

struct MACLIAPI CRspMktDataPortfolioInfoField
{
  char     szUserCode[16+1]; //用户代码 USER_CODE   VARCHAR(16)   8902 cos用户代码
  char     chStkex;           //交易所   STKEX      CHAR(1)       207
  char     szStkbd[2+1];      //板块    STKBD       CHAR(2)       625
  int      iAmount;           //数量    AMOUNT      INTEGER       8900
  char     szStkCodes[1536+1];//代码(复数)STK_CODES VARCHAR(512)  48  |分割多个代码
};

//-------------------------------10388210:k线历史数据--------------------------
struct MACLIAPI CReqMktDataKDataField
{
  char            chMarket;                   // 市场 
  char            szCode[8 + 1];              // 代码 
  char            chCqFlag;                   // 除权标志 0：不复权（仅支持） 1：向前复权（从现在向过去） 2：向后复权（从过去向现在）
  int             iCqDate;                    // 复权日期 定点复权日期,默认当前日期
  char            chQjFlag;                   // 全价标志 0:净价；1：全价
  char            szCycType[2 + 1];           // 数据周期 0：秒（不支持） 1：分 2：日 3：周 4：月 5：季 6：半年 7：年 8：tickbar（不支持） 9：5分 10：15分 11：30分 12：60分
  int             iCycDef;                    // 周期数量 0 不限制
  char            chAutoComplete;             // 自动补齐 0：不补齐 其它补齐
  int             iBegDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  int             iBegTime;                   // 开始时间 日线及以上非必送
  int             iEndTime;                   // 结束时间 日线及以上非必送
  char            chOrder;                    // 数据顺序 非0为逆序； 其它为顺序
  int             iNodeId;                    // 节点ID 内部使用
};

struct MACLIAPI CRspMktDataKDataField
{
  char            chMarket;                   // 市场 
  char            szCode[8 + 1];              // 代码 
  int             iAmount;                    // 数量 数据条数
  int             iBegDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  int             iBegTime;                   // 开始时间 日、周、月、季、半年、年线没有开始、结束时间
  int             iEndTime;                   // 结束时间 
  LONGLONG        llMarktDataTime;            // 接收时间 查询时刻最新tick接收时间
  LONGLONG        llMatchedVol;               // 成交总量 查询时刻最新tick成交总量
  LONGLONG        llMatchedAmtTot;            // 成交总金额 查询时刻最新tick成交总金额
  char            szData[256000 + 1];         // 数据 {日期|时间|开盘价|最高|最低|收盘价|成交量|成交总金额/持仓量|除权除息日|昨收/昨结}{}... 其中，日期YYYYMMDD 时间HHMMSSmmm 价格金额为实际价格10000倍 现货为昨结、成交总金额 期权期货为昨结、持仓量 逐笔为： {日期|时间|类型|方向|成交价|持仓增量|成交笔数|成交量|成交金额|iopv} 类型： '0' 多换 '1' 空换 '2' 双换 '3' 多开 '4' 空开 '5' 双开 '6' 多平 '7' 空平 '8' 双平 方向： 0:买 1:卖 其它: 股票只有（0，1，2）
};

//tick快照
struct MACLIAPI CReqMktDataTickSnapshotField
{
  char     chStkex;          //交易所    STKEX     CHAR(1)     Y 8899
  char     szStkCodes[512+1];//代码(复数)STK_CODES VARCHAR(512)Y 8900 |分割多只代码,最多支持单次40只代码，超过40只，多余部分不再处理
};

struct MACLIAPI CRspMktDataTickSnapshotField
{
  char     chStkex;       //交易所STKEX CHAR(1)         8899
  int      iAmount;       //数量  AMOUNTINTEGER         8901返回的数据条数
  char     szData[40960+1];//数据 DATA  VARCHAR(40960)  8902{..|..|..}{..|..|..}...其中，日期YYYYMMDD时间HHMMSSmmm价格金额为实际价格10000倍,整数；具体格式为ST_MKT_DATA结构体字段
};

//千档行情查询
struct MACLIAPI CReqMktDataThounsandDataField
{
  char     chStkex;           // 交易所     STKEX         CHAR(1)       √  8899
  char     szStkCodes[512+1]; // 代码(复数) STK_CODES     VARCHAR(512)  √  8900  |分割多只代码
  int      iCategory;         // 交易分类   STK_CATEGORY  INTEGER       √  625  
};

struct MACLIAPI CRspMktDataThounsandDataField
{
  char     chStkex;           // 交易所 STKEX    CHAR(1)       8899
  int      iAmount;           // 数量	 AMOUNT	INTEGER	        8901	返回的数据条数
  char     szData[2048 + 1];  // 数据   DATA     VARCHAR(2048) 8902
};

//-------------------------------10388226:指数估值查询--------------------------
struct MATRADEAPI CReqMktDataQryIndexValField
{
  char            szCode[16 + 1];             // 代码 指数代码
  int             iDate;                      // 日期 请求日期(格式:20210720)
};

struct MATRADEAPI CRspMktDataQryIndexValField
{
  char            szData[512 + 1];            // 数据 数据 格式: indexcode|indexname|trd_date|pe|pepercentiles|pb|pbpercentiles|ispe 示例: SZ399814|大农业|20210715|27.1828|20.8578|4.9515|87.074|1 
};

//-------------------------------10388232:云指标计算请求--------------------------
struct MATRADEAPI CReqMktDataQryCloudIndexField
{
  char            chMarket;                   // 市场 
  char            szCode[8 + 1];              // 代码 
  char            chCqFlag;                   // 除权标志 0：不复权 1：向前复权（从现在向过去） 2：向后复权（从过去向现在）
  int             iCqDate;                    // 复权日期 定点复权日期,默认当前日期
  char            chQjFlag;                   // 全价标志 不支持
  char            szCycType[2 + 1];           // 数据周期 0：秒 1：分 2：日 3：周 4：月 5：季 6：半年 7：年 9：5分 10：15分 11：30分 12：60分 13:3秒 14：5秒 15：10秒 16：15秒 17：20秒 18：30秒
  int             iCycDef;                    // 周期数量 0 不限制
  char            chAutoComplete;             // 自动补齐 不支持
  int             iBegDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  int             iBegTime;                   // 开始时间 日线及以上非必送
  int             iEndTime;                   // 结束时间 日线及以上非必送
  char            chOrder;                    // 数据顺序 非0为逆序； 其它为顺序
  char            szName[20 + 1];             // 指标名称 指标名称,大小写敏感
  char            szParams[20 + 1];           // 指标参数 指标参数,如果空,使用默认参数计算指标,非空则按传入的参数值计算,多个参数以”~”分隔,示例: “100~200~300”
  int             iVolunit;                   // 成交量倍数 必须大于零
};

struct MATRADEAPI CRspMktDataQryCloudIndexField
{
  char            chMarket;                   // 市场 
  char            szCode[8 + 1];              // 代码 
  int             iAmount;                    // 数量 数据条数
  int             iBegDate;                   // 开始日期 
  int             iEndDate;                   // 结束日期 
  int             iBegTime;                   // 开始时间 日、周、月、季、半年、年线没有开始、结束时间
  int             iEndTime;                   // 结束时间 
  char            szName[20 + 1];             // 指标名称 指标名称
  char            szData[256000 + 1];         // 数据 // 先返回所有指标线的名称,线性,画法,颜色等数据 指标线1: 名称,指标线类型,指标线扩展类型,画法类型,线型粗度,指标线颜色,额外数据个数,额外数据内容| 指标线2: 名称,指标线类型,指标线扩展类型,画法类型,线型粗度,指标线颜色,额外数据个数,额外数据内容| 指标线N: 名称,指标线类型,指标线扩展类型,画法类型,线型粗度,指标线颜色,额外数据个数,额外数据内容| 结束符: "~" // 再返回具体的指标数值数据, 从第一个时间点到最后一个时间点(总数等于K 线个数), 每一个时间点对应一个 ST_MKT_INDEX结构体保存所有数据, 调用DeSerialize() 解开 每个时间点数据之间的分隔符 "~"
};

END_NAMESPACE_MACLI

#endif  //__MA_CLI_MKT_DATA_STRUCT_H__