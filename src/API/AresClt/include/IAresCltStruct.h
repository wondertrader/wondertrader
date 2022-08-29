#pragma once

#ifndef SWESOME_CHAR	
#define SWESOME_CHAR
typedef char    char_8[8];
typedef char	char_10[10];
typedef char	char_20[20];
typedef char	char_31[31];
typedef char	char_32[32];
typedef char	char_64[64];
typedef char	char_128[128];
typedef char	char_256[256];
#endif

#pragma pack(1)

//市场定义(此定义仅限行情使用)
#define		ACLT_MARKET_UNKNOW			0
#define		ACLT_MARKET_SSE				1
#define		ACLT_MARKET_SZSE			2
#define		ACLT_MARKET_CFFEX			3
#define		ACLT_MARKET_CNF				4
typedef		char	AClt_Market;

//.................................................................................................................................................................................................................................
//市场时间
typedef struct
{
	int								Date;						//日期，形式为YYYYMMDD
	int								Time;						//时间, 形式为HHMMSS
}tagAClt_MarketField;

//一档
typedef struct
{
	double							Price;						//委托价格
	unsigned __int64				Volume;						//委托量[股]
}tagAClt_BuySell;

//行情
typedef struct
{
	AClt_Market						Exchange;
	char_31							Code;
	double		 					Open;						//开盘价
	double		 					High;						//最高价
	double		 					Low;						//最低价
	double		 					Now;						//最新

	unsigned __int64 				Volume;						//成交量
	double 							Amount;						//成交金额(元)

	unsigned __int64 				Position;					//持仓量
	double		 					SettlePrice;				//结算价

	tagAClt_BuySell					Buy[5];						//买5档
	tagAClt_BuySell					Sell[5];					//卖5档
	char_8 							TradingCode;				//交易状态(若有)[见文档]
}tagAClt_QuoteField;

//.................................................................................................................................................................................................................................
typedef struct
{
	AClt_Market						Exchange;
	char_31							Code;
}tagAClt_Instrument;

#define		AClt_INSTRTYPE_STOCK		1
#define		AClt_INSTRTYPE_OPTION		2
#define		AClt_INSTRTYPE_FUTURE		3
typedef     char		ACLT_INSTRUMENT_TYPE;

//股票、基金基础数据结构
typedef struct
{
	tagAClt_Instrument				Instrument;					//市场+代码
	char_32							Name;						//名称or简称(GBK编码)[注意:由于交易所可能会扩位,扩位后的名称，通过其他接口获取] 
	double							PreClose;					//昨收价
	double							UpperLimit;					//涨停价
	double							LowerLimit;					//跌停价

	unsigned char					SubType;					//股票or ETFFund
	char_10							ProductID;					//所属产品ID

	unsigned int					LotSize;					//手比率
	unsigned int					ContractMulti;				//(合约乘数*合约单位)
	double							PriceTick;					//价格变动价位
	unsigned char					ShowDot;					//显示小数位数
	bool							IsTrading;					//是否交易
}tagAClt_StockBaseData;

//.................................................................................................................................................................................................................................
//期权基础数据结构
typedef struct
{
	tagAClt_Instrument				Instrument;					//市场+代码(此代码是可以下单的代码)
	char_32							Name;					//名称or 简称(GBK)		//中金填代码 [注意:由于交易所可能会扩位,扩位后的名称，通过其他接口获取] 
	char_20							ContractID;				//沪深期权的合约代码，不能用来下单  //中金填空

	double							PreClose;					//昨收价
	double							PreSettle;					//昨结价
	unsigned __int64				PrePosition;				//昨持仓
	double							UpperLimit;					//涨停价
	double							LowerLimit;					//跌停价

	char							OptKind;					//期权种类('C' = call 'P'= put)
	char							ExecKind;					//行权种类('A' = 美式 'E' = 欧式)

	char_10							ProductID;					//所属产品ID
	tagAClt_Instrument				UnderlyingCode;				//标的市场+代码

	unsigned int					LotSize;					//手比率
	unsigned int					ContractMulti;				//(合约乘数*合约单位)
	double							PriceTick;					//价格变动价位

	double							ExecPrice;					//行权价格
	int								LastTradeDay;				//最后交易日
	int								EndDay;						//到期日

	unsigned char					ShowDot;					//显示小数位数
	double							reserved;				//reserved
	bool							IsTrading;					//是否交易
}tagAClt_OptionBaseData;

//期货基础数据结构
typedef struct
{
	tagAClt_Instrument				Instrument;					//市场+代码
	char_32							Name;					//名称or 简称(GBK) [注意:由于交易所可能会扩位,扩位后的名称，通过其他接口获取] 
	double							PreClose;					//昨收价
	double							PreSettle;					//昨结价
	unsigned __int64				PrePosition;				//昨持仓
	double							UpperLimit;					//涨停价
	double							LowerLimit;					//跌停价

	char_10							ProductID;				//所属产品ID
	tagAClt_Instrument				UnderlyingCode;				//标的市场+代码

	unsigned int					LotSize;					//手比率
	unsigned int					ContractMulti;				//(合约乘数*合约单位)
	double							PriceTick;					//价格变动价位

	int								LastTradeDay;				//最后交易日
	int								EndDay;						//到期日
	unsigned char					ShowDot;					//显示小数位数
	bool							IsTrading;					//是否交易
}tagAClt_FutureBaseData;


//基础数据三合一
typedef	struct
{
	ACLT_INSTRUMENT_TYPE			InstrType;					//商品类型
	union
	{
		tagAClt_StockBaseData			StockBase;					//股票型
		tagAClt_OptionBaseData			OptionBase;					//期权型
		tagAClt_FutureBaseData			FutureBase;					//期货型
	};
}tagAClt_CommBaseData;


//补充数据(1.14+)
typedef struct  
{
	AClt_Market						Exchange;
	char_31							Code;
	char_64							Name;
	
	char							Reserved[1024];
}tagAClt_SupplementData;


#pragma pack()
