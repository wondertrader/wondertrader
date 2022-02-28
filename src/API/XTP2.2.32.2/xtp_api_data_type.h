/////////////////////////////////////////////////////////////////////////
///@author 中泰证券股份有限公司
///@file xtp_api_data_type.h
///@brief 定义兼容数据基本类型
/////////////////////////////////////////////////////////////////////////
#ifndef _XTP_API_DATA_TYPE_H_
#define _XTP_API_DATA_TYPE_H_

#pragma pack(8)

/// 每个PBU最多被10个TGW使用。
#define MAX_TGW_CNT_PER_PBU             10

/// 存放版本号的字符串长度
#define XTP_VERSION_LEN 16
/// 版本号类型
typedef char XTPVersionType[XTP_VERSION_LEN];
/// 可交易日字符串长度
#define XTP_TRADING_DAY_LEN 9
/// 存放证券代码的字符串长度
#define XTP_TICKER_LEN 16
/// 存放证券名称的字符串长度
#define XTP_TICKER_NAME_LEN 64
/// 本地报单编号的字符串长度
#define XTP_LOCAL_ORDER_LEN         11
/// 交易所单号的字符串长度
#define XTP_ORDER_EXCH_LEN          17
/// 成交执行编号的字符串长度
#define XTP_EXEC_ID_LEN             18
/// 交易所交易员代码字符串长度
#define XTP_BRANCH_PBU_LEN          7
/// 用户资金账户的字符串长度
#define XTP_ACCOUNT_NAME_LEN        16
/// 信用业务合约负债编号长度
#define XTP_CREDIT_DEBT_ID_LEN      33
/// IP地址的字符串长度
#define XTP_INET_ADDRESS_STR_LEN 64
/// MAC地址的字符串长度
#define XTP_MAC_ADDRESS_LEN 16
/// 硬盘序列号的字符串长度
#define XTP_HARDDISK_SN_LEN 24
/// MacOS系统序列号的字符串长度
#define XTP_MACOS_SNO_LEN 21

/// 期权组合策略最多腿数
#define XTP_STRATEGE_LEG_NUM        4
/// 期权组合策略代码字符串长度
#define XTP_STRATEGY_ID_LEN         10
/// 期权组合策略名称字符串长度
#define XTP_STRATEGY_NAME_LEN       32
/// 期权组合策略组合编码字符串长度
#define XTP_SECONDARY_ORDER_ID_LEN  18

/// 期权合约可支持的组合策略列表字符串长度
#define XTP_CNTRT_COMB_STRA_LIST_LEN         2048

/// 期权行权合并最多成分合约数量
#define XTP_COMBINED_EXECUTION_LEG_NUM       2

/////////////////////////////////////////////////////////////////////////
///@brief XTP_LOG_LEVEL是日志输出级别类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_LOG_LEVEL {
	XTP_LOG_LEVEL_FATAL, ///<严重错误级别
	XTP_LOG_LEVEL_ERROR, ///<错误级别
	XTP_LOG_LEVEL_WARNING, ///<警告级别
	XTP_LOG_LEVEL_INFO,   ///<info级别
	XTP_LOG_LEVEL_DEBUG,  ///<debug级别
	XTP_LOG_LEVEL_TRACE   ///<trace级别
}XTP_LOG_LEVEL;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_PROTOCOL_TYPE是通讯传输协议方式
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_PROTOCOL_TYPE
{
	XTP_PROTOCOL_TCP = 1,	///<采用TCP方式传输
	XTP_PROTOCOL_UDP		///<采用UDP方式传输(仅行情接口支持)
}XTP_PROTOCOL_TYPE;



/////////////////////////////////////////////////////////////////////////
///@brief XTP_EXCHANGE_TYPE是交易所类型，行情里使用
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_EXCHANGE_TYPE
{
	XTP_EXCHANGE_SH = 1,	///<上证
	XTP_EXCHANGE_SZ,		///<深证
    XTP_EXCHANGE_UNKNOWN	///<不存在的交易所类型
}XTP_EXCHANGE_TYPE;

//////////////////////////////////////////////////////////////////////////
///@brief XTP_MARKET_TYPE市场类型，交易里使用
//////////////////////////////////////////////////////////////////////////
typedef enum XTP_MARKET_TYPE
{
    XTP_MKT_INIT = 0,///<初始化值或者未知
    XTP_MKT_SZ_A = 1,///<深圳A股
    XTP_MKT_SH_A,    ///<上海A股
    XTP_MKT_UNKNOWN   ///<未知交易市场类型
}XTP_MARKET_TYPE;


/////////////////////////////////////////////////////////////////////////
///@brief XTP_PRICE_TYPE是价格类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_PRICE_TYPE
{
	XTP_PRICE_LIMIT = 1,           ///<限价单-沪 / 深 / 沪期权 / 深期权 （除普通股票业务外，其余未特指的业务均使用此种类型）
	XTP_PRICE_BEST_OR_CANCEL,      ///<即时成交剩余转撤销，市价单-深 / 沪期权 / 深期权
	XTP_PRICE_BEST5_OR_LIMIT,      ///<最优五档即时成交剩余转限价，市价单-沪
	XTP_PRICE_BEST5_OR_CANCEL,     ///<最优5档即时成交剩余转撤销，市价单-沪深 / 深期权
	XTP_PRICE_ALL_OR_CANCEL,       ///<全部成交或撤销,市价单-深 / 沪期权 / 深期权
	XTP_PRICE_FORWARD_BEST,        ///<本方最优，市价单-深 / 深期权 / 沪科创板
	XTP_PRICE_REVERSE_BEST_LIMIT,  ///<对方最优剩余转限价，市价单-深 / 沪期权 / 深期权 / 沪科创板
	XTP_PRICE_LIMIT_OR_CANCEL,	   ///<期权限价申报FOK
	XTP_PRICE_TYPE_UNKNOWN,		   ///<未知或者无效价格类型
}XTP_PRICE_TYPE;



/////////////////////////////////////////////////////////////////////////
///@brief XTP_SIDE_TYPE是买卖方向类型
/////////////////////////////////////////////////////////////////////////
typedef uint8_t XTP_SIDE_TYPE;

///买（新股申购，ETF买，配股，信用交易中担保品买）
#define XTP_SIDE_BUY            1
///卖（逆回购，ETF卖，信用交易中担保品卖）
#define XTP_SIDE_SELL           2
///申购
#define XTP_SIDE_PURCHASE       7
///赎回
#define XTP_SIDE_REDEMPTION     8
///拆分
#define XTP_SIDE_SPLIT          9
///合并
#define XTP_SIDE_MERGE          10
///改版之后的side的备兑，暂不支持
#define XTP_SIDE_COVER          11
///改版之后的side锁定（对应开平标识为开）/解锁（对应开平标识为平）
#define XTP_SIDE_FREEZE         12
/// 融资买入
#define XTP_SIDE_MARGIN_TRADE	21
/// 融券卖出
#define XTP_SIDE_SHORT_SELL		22
/// 卖券还款
#define XTP_SIDE_REPAY_MARGIN	23
/// 买券还券
#define XTP_SIDE_REPAY_STOCK	24
/// 现金还款（不放在普通订单协议，另加请求和查询协议）
//#define XTP_SIDE_CASH_REPAY_MARGIN	25
/// 现券还券
#define XTP_SIDE_STOCK_REPAY_STOCK	26
/// 余券划转
#define XTP_SIDE_SURSTK_TRANS       27
/// 担保品转入
#define XTP_SIDE_GRTSTK_TRANSIN     28
/// 担保品转出
#define XTP_SIDE_GRTSTK_TRANSOUT    29

///组合策略的组合
#define XTP_SIDE_OPT_COMBINE        31 
///组合策略的拆分
#define XTP_SIDE_OPT_SPLIT          32 
///组合策略的管理员强制拆分
#define XTP_SIDE_OPT_SPLIT_FORCE    33 
///组合策略的交易所强制拆分
#define XTP_SIDE_OPT_SPLIT_FORCE_EXCH    34

///未知或者无效买卖方向
#define XTP_SIDE_UNKNOWN        50



/////////////////////////////////////////////////////////////////////////
///@brief XTP_POSITION_EFFECT_TYPE是开平标识类型
/////////////////////////////////////////////////////////////////////////
typedef uint8_t XTP_POSITION_EFFECT_TYPE;

/// 初始值或未知值开平标识，除期权外，均使用此值
#define XTP_POSITION_EFFECT_INIT                0
/// 开
#define XTP_POSITION_EFFECT_OPEN                1
/// 平
#define XTP_POSITION_EFFECT_CLOSE               2
/// 强平
#define XTP_POSITION_EFFECT_FORCECLOSE          3
/// 平今
#define XTP_POSITION_EFFECT_CLOSETODAY          4
/// 平昨
#define XTP_POSITION_EFFECT_CLOSEYESTERDAY      5
/// 强减
#define XTP_POSITION_EFFECT_FORCEOFF            6
/// 本地强平
#define XTP_POSITION_EFFECT_LOCALFORCECLOSE     7
/// 信用业务追保强平
#define XTP_POSITION_EFFECT_CREDIT_FORCE_COVER  8
/// 信用业务清偿强平
#define XTP_POSITION_EFFECT_CREDIT_FORCE_CLEAR  9
/// 信用业务合约到期强平
#define XTP_POSITION_EFFECT_CREDIT_FORCE_DEBT   10
/// 信用业务无条件强平
#define XTP_POSITION_EFFECT_CREDIT_FORCE_UNCOND 11
/// 未知的开平标识类型
#define XTP_POSITION_EFFECT_UNKNOWN             12


/////////////////////////////////////////////////////////////////////////
///@brief XTP_ORDER_ACTION_STATUS_TYPE是报单操作状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_ORDER_ACTION_STATUS_TYPE
{
	XTP_ORDER_ACTION_STATUS_SUBMITTED = 1,	///<已经提交
	XTP_ORDER_ACTION_STATUS_ACCEPTED,		///<已经接受
	XTP_ORDER_ACTION_STATUS_REJECTED		///<已经被拒绝
}XTP_ORDER_ACTION_STATUS_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_ORDER_STATUS_TYPE是报单状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_ORDER_STATUS_TYPE
{
    XTP_ORDER_STATUS_INIT = 0,///<初始化
    XTP_ORDER_STATUS_ALLTRADED = 1,           ///<全部成交
    XTP_ORDER_STATUS_PARTTRADEDQUEUEING,  ///<部分成交
    XTP_ORDER_STATUS_PARTTRADEDNOTQUEUEING, ///<部分撤单
    XTP_ORDER_STATUS_NOTRADEQUEUEING,   ///<未成交
    XTP_ORDER_STATUS_CANCELED,  ///<已撤单
    XTP_ORDER_STATUS_REJECTED,  ///<已拒绝
    XTP_ORDER_STATUS_UNKNOWN  ///<未知订单状态
}XTP_ORDER_STATUS_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_ORDER_SUBMIT_STATUS_TYPE是报单提交状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_ORDER_SUBMIT_STATUS_TYPE
{
    XTP_ORDER_SUBMIT_STATUS_INSERT_SUBMITTED = 1, ///<订单已经提交
    XTP_ORDER_SUBMIT_STATUS_INSERT_ACCEPTED,///<订单已经被接受
    XTP_ORDER_SUBMIT_STATUS_INSERT_REJECTED,///<订单已经被拒绝
    XTP_ORDER_SUBMIT_STATUS_CANCEL_SUBMITTED,///<撤单已经提交
    XTP_ORDER_SUBMIT_STATUS_CANCEL_REJECTED,///<撤单已经被拒绝
    XTP_ORDER_SUBMIT_STATUS_CANCEL_ACCEPTED ///<撤单已经被接受
}XTP_ORDER_SUBMIT_STATUS_TYPE;


/////////////////////////////////////////////////////////////////////////
///@brief XTP_TE_RESUME_TYPE是公有流（订单响应、成交回报）重传方式
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_TE_RESUME_TYPE
{
	XTP_TERT_RESTART = 0,	///<从本交易日开始重传
	XTP_TERT_RESUME,		///<从从上次收到的续传（暂未支持）
	XTP_TERT_QUICK			///<只传送登录后公有流（订单响应、成交回报）的内容
}XTP_TE_RESUME_TYPE;


//////////////////////////////////////////////////////////////////////////
///@brief ETF_REPLACE_TYPE现金替代标识定义
//////////////////////////////////////////////////////////////////////////
typedef enum ETF_REPLACE_TYPE
{
    ERT_CASH_FORBIDDEN = 0,             ///<禁止现金替代
    ERT_CASH_OPTIONAL,                  ///<可以现金替代
    ERT_CASH_MUST,                      ///<必须现金替代
    ERT_CASH_RECOMPUTE_INTER_SZ,        ///<深市退补现金替代
    ERT_CASH_MUST_INTER_SZ,             ///<深市必须现金替代
    ERT_CASH_RECOMPUTE_INTER_OTHER,     ///<非沪深市场成分证券退补现金替代（不适用于跨沪深港ETF产品）
    ERT_CASH_MUST_INTER_OTHER,          ///<表示非沪深市场成份证券必须现金替代（不适用于跨沪深港ETF产品）
    ERT_CASH_RECOMPUTE_INTER_HK,	    ///港市退补现金替代（仅适用于跨沪深港ETF产品）
    ERT_CASH_MUST_INTER_HK,		        ///港市必须现金替代（仅适用于跨沪深港ETF产品）
    EPT_INVALID                         ///<无效值
}ETF_REPLACE_TYPE;


//////////////////////////////////////////////////////////////////////////
///@brief XTP_TICKER_TYPE证券类型
//////////////////////////////////////////////////////////////////////////
typedef enum XTP_TICKER_TYPE
{
	XTP_TICKER_TYPE_STOCK = 0,            ///<普通股票
	XTP_TICKER_TYPE_INDEX,                ///<指数
	XTP_TICKER_TYPE_FUND,                 ///<基金
	XTP_TICKER_TYPE_BOND,                 ///<债券
	XTP_TICKER_TYPE_OPTION,               ///<期权
    XTP_TICKER_TYPE_TECH_STOCK,           ///<科创板股票（上海）
	XTP_TICKER_TYPE_UNKNOWN               ///<未知类型
	
}XTP_TICKER_TYPE;

//////////////////////////////////////////////////////////////////////////
///@brief XTP_BUSINESS_TYPE证券业务类型
//////////////////////////////////////////////////////////////////////////
typedef enum XTP_BUSINESS_TYPE
{
	XTP_BUSINESS_TYPE_CASH = 0,            ///<普通股票业务（股票买卖，ETF买卖，沪市交易型货币基金等）
	XTP_BUSINESS_TYPE_IPOS,                ///<新股申购业务（对应的price type需选择限价类型）
	XTP_BUSINESS_TYPE_REPO,                ///<回购业务（国债逆回购业务对应的price type填为限价，side填为卖）
	XTP_BUSINESS_TYPE_ETF,                 ///<ETF申赎业务
	XTP_BUSINESS_TYPE_MARGIN,              ///<融资融券业务
	XTP_BUSINESS_TYPE_DESIGNATION,         ///<转托管（未支持）
	XTP_BUSINESS_TYPE_ALLOTMENT,		   ///<配股业务（对应的price type需选择限价类型,side填为买）
	XTP_BUSINESS_TYPE_STRUCTURED_FUND_PURCHASE_REDEMPTION,	   ///<分级基金申赎业务
	XTP_BUSINESS_TYPE_STRUCTURED_FUND_SPLIT_MERGE,	   ///<分级基金拆分合并业务
	XTP_BUSINESS_TYPE_MONEY_FUND,		   ///<货币基金申赎业务（暂未支持，沪市交易型货币基金的买卖请使用普通股票业务）
    XTP_BUSINESS_TYPE_OPTION,              ///<期权业务
    XTP_BUSINESS_TYPE_EXECUTE,             ///<行权
    XTP_BUSINESS_TYPE_FREEZE,              ///<锁定解锁，暂不支持
    XTP_BUSINESS_TYPE_OPTION_COMBINE,      ///<期权组合策略 组合和拆分业务
    XTP_BUSINESS_TYPE_EXECUTE_COMBINE,     ///<期权行权合并业务
    XTP_BUSINESS_TYPE_UNKNOWN,             ///<未知类型
} XTP_BUSINESS_TYPE;

//////////////////////////////////////////////////////////////////////////
///@brief XTP_ACCOUNT_TYPE账户类型
//////////////////////////////////////////////////////////////////////////
typedef enum XTP_ACCOUNT_TYPE
{
    XTP_ACCOUNT_NORMAL = 0,	///<普通账户
    XTP_ACCOUNT_CREDIT,		///<信用账户
    XTP_ACCOUNT_DERIVE,		///<衍生品账户
    XTP_ACCOUNT_UNKNOWN		///<未知账户类型
}XTP_ACCOUNT_TYPE;


/////////////////////////////////////////////////////////////////////////
///@brief XTP_FUND_TRANSFER_TYPE是资金流转方向类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_FUND_TRANSFER_TYPE
{
    XTP_FUND_TRANSFER_OUT = 0,		///<转出 从XTP转出到柜台
    XTP_FUND_TRANSFER_IN,	        ///<转入 从柜台转入XTP
    XTP_FUND_INTER_TRANSFER_OUT,    ///<跨节点转出 从本XTP节点1，转出到对端XTP节点2，XTP服务器之间划拨，只能“一账号两中心”跨节点用户使用
    XTP_FUND_INTER_TRANSFER_IN,     ///<跨节点转入 从对端XTP节点2，转入到本XTP节点1，XTP服务器之间划拨，只能“一账号两中心”跨节点用户使用
    XTP_FUND_TRANSFER_UNKNOWN		///<未知类型
}XTP_FUND_TRANSFER_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_FUND_QUERY_TYPE是柜台资金查询类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_FUND_QUERY_TYPE
{
	XTP_FUND_QUERY_JZ = 0,		///<查询金证主柜台可转资金
	XTP_FUND_QUERY_INTERNAL,	///<查询一账号两中心设置时，对方节点的资金
	XTP_FUND_QUERY_UNKNOWN		///<未知类型
}XTP_FUND_QUERY_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_FUND_OPER_STATUS柜台资金操作结果
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_FUND_OPER_STATUS {
    XTP_FUND_OPER_PROCESSING = 0,	///<XTP已收到，正在处理中
    XTP_FUND_OPER_SUCCESS,			///<成功
    XTP_FUND_OPER_FAILED,			///<失败
    XTP_FUND_OPER_SUBMITTED,		///<已提交到集中柜台处理
    XTP_FUND_OPER_UNKNOWN			///<未知
}XTP_FUND_OPER_STATUS;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_DEBT_EXTEND_OPER_STATUS柜台负债展期操作状态
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_DEBT_EXTEND_OPER_STATUS {
	XTP_DEBT_EXTEND_OPER_PROCESSING = 0,	///<XTP已收到，正在处理中
	XTP_DEBT_EXTEND_OPER_SUBMITTED,			///<已提交到集中柜台处理
	XTP_DEBT_EXTEND_OPER_SUCCESS,			///<成功
	XTP_DEBT_EXTEND_OPER_FAILED,			///<失败
	XTP_DEBT_EXTEND_OPER_UNKNOWN			///<未知
}XTP_DEBT_EXTEND_OPER_STATUS;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_SPLIT_MERGE_STATUS是一个基金当天拆分合并状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_SPLIT_MERGE_STATUS {
	XTP_SPLIT_MERGE_STATUS_ALLOW = 0,	///<允许拆分和合并
	XTP_SPLIT_MERGE_STATUS_ONLY_SPLIT,	///<只允许拆分，不允许合并
	XTP_SPLIT_MERGE_STATUS_ONLY_MERGE,	///<只允许合并，不允许拆分
	XTP_SPLIT_MERGE_STATUS_FORBIDDEN	///<不允许拆分合并
}XTP_SPLIT_MERGE_STATUS;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_TBT_TYPE是一个逐笔回报类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_TBT_TYPE {
	XTP_TBT_ENTRUST = 1,	///<逐笔委托
	XTP_TBT_TRADE = 2,		///<逐笔成交
	XTP_TBT_STATE = 3,      ///<逐笔状态订单，2.2.32版本新增字段，为上海新债券Level2行情中独有
}XTP_TBT_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_OPT_CALL_OR_PUT_TYPE是一个认沽或认购类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_OPT_CALL_OR_PUT_TYPE {
	XTP_OPT_CALL = 1,	    ///<认购
	XTP_OPT_PUT = 2,		///<认沽
}XTP_OPT_CALL_OR_PUT_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_OPT_EXERCISE_TYPE_TYPE是一个行权方式类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_OPT_EXERCISE_TYPE_TYPE {
	XTP_OPT_EXERCISE_TYPE_EUR = 1,	    ///<欧式
	XTP_OPT_EXERCISE_TYPE_AME = 2,		///<美式
}XTP_OPT_EXERCISE_TYPE_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_POSITION_DIRECTION_TYPE是一个持仓方向类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_POSITION_DIRECTION_TYPE {
	XTP_POSITION_DIRECTION_NET = 0,	    ///<净
	XTP_POSITION_DIRECTION_LONG,		///<多（期权则为权利方）
    XTP_POSITION_DIRECTION_SHORT,       ///<空（期权则为义务方）
    XTP_POSITION_DIRECTION_COVERED,     ///<备兑（期权则为备兑义务方）
}XTP_POSITION_DIRECTION_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_OPT_COVERED_OR_UNCOVERED是否备兑的标签
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_OPT_COVERED_OR_UNCOVERED {
    XTP_POSITION_UNCOVERED = 0,     ///<非备兑
    XTP_POSITION_COVERED,           ///<备兑
}XTP_OPT_COVERED_OR_UNCOVERED;


/////////////////////////////////////////////////////////////////////////
///@brief XTP_CRD_CASH_REPAY_STATUS是一个融资融券直接还款状态类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_CRD_CR_STATUS {
    XTP_CRD_CR_INIT = 0,        ///< 初始、未处理状态
    XTP_CRD_CR_SUCCESS,         ///< 已成功处理状态
    XTP_CRD_CR_FAILED,          ///< 处理失败状态
} XTP_CRD_CR_STATUS;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_OPT_POSITION_TYPE是一个期权持仓类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_OPT_POSITION_TYPE
{
	XTP_OPT_POSITION_TYPE_CONTRACT = 0,     ///< 单合约持仓
	XTP_OPT_POSITION_TYPE_COMBINED = 1      ///< 组合策略持仓
}XTP_OPT_POSITION_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_ORDER_TYPE是一个订单的类型
/////////////////////////////////////////////////////////////////////////
enum XTP_ORDER_DETAIL_TYPE
{
	XTP_ORDER_DETAIL_TYPE_NEW_ORDER = 0,				///< 新订单
	XTP_ORDER_DETAIL_TYPE_CANCEL_ORDER = 1,				///< 新订单撤单
	XTP_ORDER_DETAIL_TYPE_OPT_COMB_NEW_ORDER = 2,		///< 组合订单
	XTP_ORDER_DETAIL_TYPE_OPT_COMB_CANCEL_ORDER = 3     ///< 组合订单撤单
};

/////////////////////////////////////////////////////////////////////////
///TXTPTradeTypeType是成交类型类型
/////////////////////////////////////////////////////////////////////////
typedef char TXTPTradeTypeType;

///普通成交
#define XTP_TRDT_COMMON '0'
///现金替代
#define XTP_TRDT_CASH '1'
///一级市场成交
#define XTP_TRDT_PRIMARY '2'
///跨市场资金成交
#define XTP_TRDT_CROSS_MKT_CASH '3'


/////////////////////////////////////////////////////////////////////////
///TXTPOrderTypeType是报单类型类型
/////////////////////////////////////////////////////////////////////////
typedef char TXTPOrderTypeType;

///正常
#define XTP_ORDT_Normal '0'
///报价衍生
#define XTP_ORDT_DeriveFromQuote '1'
///组合衍生
#define XTP_ORDT_DeriveFromCombination '2'
///组合报单
#define XTP_ORDT_Combination '3'
///条件单
#define XTP_ORDT_ConditionalOrder '4'
///互换单
#define XTP_ORDT_Swap '5'


//////////////////////////////////////////////////////////////////////////
///@brief XTPTerminalType是一种终端类型枚举，仅供授权系统使用
//////////////////////////////////////////////////////////////////////////
enum XTPTerminalType
{
	XTP_TERMINAL_PC = 1,            ///<"PC",PC-windows及MacOS
	XTP_TERMINAL_ANDROID,           ///<"MA",Mobile-Android
	XTP_TERMINAL_IOS,               ///<"MI",Mobile-Ios
	XTP_TERMINAL_WP,                ///<"MW",Mobile-Windows Phone
	XTP_TERMINAL_STATION,           ///<"WP",无盘站
	XTP_TERMINAL_TEL,               ///<"TO",电话委托
	XTP_TERMINAL_PC_LINUX           ///<"OH",PC-linux及其他终端
};

/////////////////////////////////////////////////////////////////////////
///@brief XTP_EXPIRE_DATE_TYPE是一个期权组合策略合约到期日要求类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_EXPIRE_DATE_TYPE {
	XTP_EXP_DATE_SAME = 0,   ///< 相同到期日
	XTP_EXP_DATE_DIFF,      ///< 不同到期日
	XTP_EXP_DATE_NON         ///< 无到期日要求
}XTP_EXPIRE_DATE_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_UNDERLYING_TYPE是一个期权组合策略标的要求类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_UNDERLYING_TYPE {
	XTP_UNDERLYING_SAME = 0,	///<相同标的
	XTP_UNDERLYING_DIFF,		///<不同标的
	XTP_UNDERLYING_NON			///<无标的要求
}XTP_UNDERLYING_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_AUTO_SPLIT_TYPE是一个期权组合策略自动解除枚举类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_AUTO_SPLIT_TYPE {
	XTP_AUTO_SPLIT_EXPDAY = 0,	///<到期日自动解除
	XTP_AUTO_SPLIT_PREDAY,		///<E-1日自动解除
	XTP_AUTO_SPLIT_PRE2DAY,		///<E-2日自动解除
	XTP_AUTO_SPLIT_NON			///<无效值
}XTP_AUTO_SPLIT_TYPE;


///行权价顺序类型， 从1开始，1表示行权价最高，2次之。如果行权价相同，则填写相同数字，用A表示行权价大于等于B，B大于等于C依次类推（C、D）
typedef char TXTPExerciseSeqType;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_QUALIFICATION_TYPE是一个证券适当性枚举类型
/////////////////////////////////////////////////////////////////////////
typedef enum  XTP_QUALIFICATION_TYPE
{
	XTP_QUALIFICATION_PUBLIC = 0,			///<公众投资者，合格投资者与机构投资者均可
	XTP_QUALIFICATION_COMMON = 1,			///<仅合格投资者与公众投资者
	XTP_QUALIFICATION_ORGANIZATION = 2,		///<仅限机构投资者
	XTP_QUALIFICATION_UNKNOWN = 3		///<未知，期权等可能为此种类型
}XTP_QUALIFICATION_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_SECURITY_TYPE是一个证券详细分类枚举类型
/////////////////////////////////////////////////////////////////////////
typedef enum XTP_SECURITY_TYPE {
	/// 主板股票
	XTP_SECURITY_MAIN_BOARD = 0,
	/// 中小板股票
	XTP_SECURITY_SECOND_BOARD,
	/// 创业板股票
	XTP_SECURITY_STARTUP_BOARD,
	/// 指数
	XTP_SECURITY_INDEX,
	/// 科创板股票(上海)
	XTP_SECURITY_TECH_BOARD = 4,
	/// 国债
	XTP_SECURITY_STATE_BOND = 5,
	/// 企业债
	XTP_SECURITY_ENTERPRICE_BOND = 6,
	/// 公司债
	XTP_SECURITY_COMPANEY_BOND = 7,
	/// 转换债券
	XTP_SECURITY_CONVERTABLE_BOND = 8,
	/// 国债逆回购
	XTP_SECURITY_NATIONAL_BOND_REVERSE_REPO = 12,
	/// 本市场股票 ETF
	XTP_SECURITY_ETF_SINGLE_MARKET_STOCK = 14,
	/// 跨市场股票 ETF
	XTP_SECURITY_ETF_INTER_MARKET_STOCK,
	// 跨境股票 ETF
	XTP_SECURITY_ETF_CROSS_BORDER_STOCK = 16,
	/// 本市场实物债券 ETF
	XTP_SECURITY_ETF_SINGLE_MARKET_BOND = 17,
	/// 黄金 ETF
	XTP_SECURITY_ETF_GOLD = 19,
	/// 分级基金子基金
	XTP_SECURITY_STRUCTURED_FUND_CHILD = 24,
	/// 深交所仅申赎基金
	XTP_SECURITY_SZSE_RECREATION_FUND = 26,
	/// 个股期权
	XTP_SECURITY_STOCK_OPTION = 29,
	/// ETF期权
	XTP_SECURITY_ETF_OPTION = 30,
	/// 配股
	XTP_SECURITY_ALLOTMENT = 100,

	/// 上交所申赎型货币基金
	XTP_SECURITY_MONETARY_FUND_SHCR = 110,
	/// 上交所交易型货币基金
	XTP_SECURITY_MONETARY_FUND_SHTR = 111,
	/// 深交所货币基金
	XTP_SECURITY_MONETARY_FUND_SZ = 112,

	/// 其他
	XTP_SECURITY_OTHERS = 255
}XTP_SECURITY_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_POSITION_SECURITY_TYPE是一个持仓证券枚举类型
/////////////////////////////////////////////////////////////////////////
typedef enum  XTP_POSITION_SECURITY_TYPE
{
	XTP_POSITION_SECURITY_NORMAL = 0,			///<普通持仓
	XTP_POSITION_SECURITY_PLACEMENT = 1,		///<配售类型的持仓，包含配股、配债等
	XTP_POSITION_SECURITY_UNKNOWN = 2			///<未知类型
}XTP_POSITION_SECURITY_TYPE;

/////////////////////////////////////////////////////////////////////////
///@brief XTP_SECURITY_STATUS是一个证券状态枚举类型
/////////////////////////////////////////////////////////////////////////
typedef enum  XTP_SECURITY_STATUS
{
	XTP_SECURITY_STATUS_ST = 0,         ///< 风险警示板
	XTP_SECURITY_STATUS_N_IPO,          ///< 首日上市
	XTP_SECURITY_STATUS_COMMON,         ///< 普通
	XTP_SECURITY_STATUS_RESUME,         ///< 恢复上市
	XTP_SECURITY_STATUS_DELISTING = 10, ///< 退市整理期
	XTP_SECURITY_STATUS_OTHERS = 255    ///< 其他
}XTP_SECURITY_STATUS;

#pragma pack()

#endif
