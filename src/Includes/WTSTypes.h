/*!
 * \file WTSTypes.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief WonderTrader基本数据类型定义文件
 */
#pragma once
#include "WTSMarcos.h"
#include <stdint.h>

NS_WTP_BEGIN

/*
 *	合约分类
 */
//从CTP复制过来的
typedef enum tagContractCategory
{
	CC_Stock,			//股票
	CC_Future,			//期货
	CC_FutOption,		//期货期权，商品期权是这个分类
	CC_Combination,		//组合
	CC_Spot,			//即期
	CC_EFP,				//期转现
	CC_SpotOption,		//现货期权，股指期权是这个分类
	CC_ETFOption,		//个股期权，ETF期权是这个分类

	CC_DC_Spot	= 20,	//数币现货
	CC_DC_Swap,			//数币永续
	CC_DC_Future,		//数币期货
	CC_DC_Margin,		//数币杠杆
	CC_DC_Option,		//数币期权

	CC_UserIndex = 90	//自定义指数
} ContractCategory;

/*
 *	期权类型
 */
typedef enum tagOptionType
{
	OT_None = 0,
	OT_Call = '1',		//看涨期权
	OT_Put	= '2'		//看跌期权
} OptionType;

/*
 *	平仓类型
 */
typedef enum tagCoverMode
{
	CM_OpenCover,		//开平
	CM_CoverToday,		//开平昨平今
	CM_UNFINISHED,		//平未了结的
	CM_None			//不区分开平
} CoverMode;

/*
 *	交易模式
 */
typedef enum tagTradingMode
{
	TM_Both,	//多空都支持
	TM_Long,	//只能做多
	TM_LongT1,	//做多T+1
	TM_None = 9	//不能交易
} TradingMode;

/*
*	价格模式
*/
typedef enum tagPriceMode
{
	PM_Both,		//市价限价都支持
	PM_Limit,		//只支持限价
	PM_Market,		//只支持市价
	PM_None	= 9		//不支持交易
} PriceMode;

/*
 *	K线数据类型
 *	开、高、低、收、量、额、日期、时间
 */
typedef enum tagKlineFieldType
{
	KFT_OPEN,
	KFT_HIGH,
	KFT_LOW,
	KFT_CLOSE,
	KFT_DATE,
	KFT_TIME,
	KFT_VOLUME,
	KFT_SVOLUME
} WTSKlineFieldType;

/*
 *	K线周期
 */
typedef enum tagKlinePeriod
{
	KP_Tick,
	KP_Minute1,
	KP_Minute5,
	KP_DAY,
	KP_Week,
	KP_Month
} WTSKlinePeriod;

static const char* PERIOD_NAME[] = 
{
	"tick",
	"min1",
	"min5",
	"day",
	"week",
	"month"
};

/*
 *	日志级别
 */
typedef enum tagLogLevel
{
	LL_ALL	= 100,
	LL_DEBUG,
	LL_INFO,
	LL_WARN,
	LL_ERROR,
	LL_FATAL,
	LL_NONE
} WTSLogLevel;

/*
 *	价格类型
 */
typedef enum tagPriceType
{
	WPT_ANYPRICE	= 0,			//市价单
	WPT_LIMITPRICE,					//限价单
	WPT_BESTPRICE,					//最优价
	WPT_LASTPRICE,					//最新价

	//////////////////////////////////////////////////////////////////////////
	//以下对标CTP的价格类型
	WPT_CTP_LASTPLUSONETICKS = 20,	//最新价+1ticks
	WPT_CTP_LASTPLUSTWOTICKS,		//最新价+2ticks
	WPT_CTP_LASTPLUSTHREETICKS,		//最新价+3ticks
	WPT_CTP_ASK1,					//卖一价
	WPT_CTP_ASK1PLUSONETICKS,		//卖一价+1ticks
	WPT_CTP_ASK1PLUSTWOTICKS,		//卖一价+2ticks
	WPT_CTP_ASK1PLUSTHREETICKS,		//卖一价+3ticks
	WPT_CTP_BID1,					//买一价
	WPT_CTP_BID1PLUSONETICKS,		//卖一价+1ticks
	WPT_CTP_BID1PLUSTWOTICKS,		//卖一价+2ticks
	WPT_CTP_BID1PLUSTHREETICKS,		//卖一价+3ticks
	WPT_CTP_FIVELEVELPRICE,			//五档价，中金所市价

	//////////////////////////////////////////////////////////////////////////
	//以下对标DC的价格类型
	WPT_DC_POSTONLY	= 100,			//只做maker单
	WPT_DC_FOK,						//全部成交或立即取消
	WPT_DC_IOC,						//立即成交并取消剩余
	WPT_DC_OPTLIMITIOC				//市价委托立即成交并取消剩余
} WTSPriceType;

/*
 *	时间条件
 */
typedef enum tagTimeCondition
{
	WTC_IOC		= '1',	//立即完成,否则撤销
	WTC_GFS,			//本节有效
	WTC_GFD,			//当日有效
} WTSTimeCondition;

/*
 *	订单标志
 */
typedef enum tagOrderFlag
{
	WOF_NOR = '0',		//普通订单
	WOF_FAK,			//fak
	WOF_FOK,			//fok
} WTSOrderFlag;

/*
 *	开平方向
 */
typedef enum tagOffsetType
{
	WOT_OPEN			= '0',	//开仓
	WOT_CLOSE,					//平仓,上期为平昨
	WOT_FORCECLOSE,				//强平
	WOT_CLOSETODAY,				//平今
	WOT_CLOSEYESTERDAY,			//平昨
} WTSOffsetType;

/*
 *	多空方向
 */
typedef enum tagDirectionType
{
	WDT_LONG			= '0',	//做多
	WDT_SHORT,					//做空
	WDT_NET						//净
} WTSDirectionType;

/*
 *	业务类型
 */
typedef enum tagBusinessType
{
	BT_CASH		= '0',	//普通买卖,
	BT_ETF		= '1',	//ETF申赎
	BT_EXECUTE	= '2',	//期权行权
	BT_QUOTE	= '3',	//期权报价
	BT_FORQUOTE = '4',	//期权询价
	BT_FREEZE	= '5',	//期权对锁
	BT_CREDIT	= '6',	//融资融券
	BT_UNKNOWN			//未知业务类型
} WTSBusinessType;

/*
 *	订单操作类型
 */
typedef enum tagActionFlag
{
	WAF_CANCEL			= '0',	//撤销
	WAF_MODIFY			= '3',	//修改
} WTSActionFlag;

/*
 *	订单状态
 */
typedef enum tagOrderState
{
	WOS_AllTraded				= '0',	//全部成交
	WOS_PartTraded_Queuing,				//部分成交,仍在队列中
	WOS_PartTraded_NotQueuing,			//部分成交,未在队列
	WOS_NotTraded_Queuing,				//未成交
	WOS_NotTraded_NotQueuing,			//未成交,未在队列
	WOS_Canceled,						//已撤销
	WOS_Submitting				= 'a',	//正在提交
	WOS_Cancelling,						//在撤
	WOS_Nottouched,						//未触发
} WTSOrderState;

/*
 *	订单类型
 */
typedef enum tagOrderType
{
	WORT_Normal			= 0,		//正常订单
	WORT_Exception,					//异常订单
	WORT_System,					//系统订单
	WORT_Hedge						//对冲订单
} WTSOrderType;

/*
 *	成交类型
 */
typedef enum tagTradeType
{
	WTT_Common				= '0',	//普通
	WTT_OptionExecution		= '1',	//期权执行
	WTT_OTC					= '2',	//OTC成交
	WTT_EFPDerived			= '3',	//期转现衍生成交
	WTT_CombinationDerived	= '4'	//组合衍生成交
} WTSTradeType;


/*
 *	错误代码
 */
typedef enum tagErrorCode
{
	WEC_NONE			=	0,		//没有错误
	WEC_ORDERINSERT,				//下单错误
	WEC_ORDERCANCEL,				//撤单错误
	WEC_EXECINSERT,					//行权指令错误
	WEC_EXECCANCEL,					//行权撤销错误
	WEC_UNKNOWN			=	9999	//未知错误
} WTSErroCode;

/*
 *	比较字段
 */
typedef enum tagCompareField
{
	WCF_NEWPRICE			=	0,	//最新价
	WCF_BIDPRICE,					//买一价
	WCF_ASKPRICE,					//卖一价
	WCF_PRICEDIFF,					//价差,止盈止损专用
	WCF_NONE				=	9	//不比较
} WTSCompareField;

/*
 *	比较类型
 */
typedef enum tagCompareType
{
	WCT_Equal			= 0,		//等于
	WCT_Larger,						//大于
	WCT_Smaller,					//小于
	WCT_LargerOrEqual,				//大于等于
	WCT_SmallerOrEqual				//小于等于
}WTSCompareType;

/*
 *	行情解析器事件
 */
typedef enum tagParserEvent
{
	WPE_Connect			= 0,		//连接事件
	WPE_Close,						//关闭事件
	WPE_Login,						//登录
	WPE_Logout						//注销
}WTSParserEvent;

/*
 *	交易模块事件
 */
typedef enum tagTraderEvent
{
	WTE_Connect			= 0,		//连接事件
	WTE_Close,						//关闭事件
	WTE_Login,						//登录
	WTE_Logout						//注销
}WTSTraderEvent;

/*
 *	交易状态
 */
typedef enum tagTradeStatus
{
	TS_BeforeTrading	= '0',	//开盘前
	TS_NotTrading		= '1',	//非交易
	TS_Continous		= '2',	//连续竞价
	TS_AuctionOrdering	= '3',	//集合竞价下单
	TS_AuctionBalance	= '4',	//集合竞价平衡
	TS_AuctionMatch		= '5',	//集合竞价撮合
	TS_Closed			= '6'	//收盘
}WTSTradeStatus;

/*
 *	买卖方向类型
 */
typedef uint32_t WTSBSDirectType;
#define BDT_Buy		'B'	//买入	
#define BDT_Sell	'S'	//卖出
#define BDT_Unknown ' '	//未知
#define BDT_Borrow	'G'	//借入
#define BDT_Lend	'F'	//借出

/*
 *	成交类型
 */
typedef uint32_t WTSTransType;
#define TT_Unknown	'U'	//未知类型
#define TT_Match	'M'	//撮合成交
#define TT_Cancel	'C'	//撤单

/*
 *	委托明细类型
 */
typedef uint32_t WTSOrdDetailType;
#define ODT_Unknown		0	//未知类型
#define ODT_BestPrice	'U'	//本方最优
#define ODT_AnyPrice	'1'	//市价
#define ODT_LimitPrice	'2'	//限价

NS_WTP_END