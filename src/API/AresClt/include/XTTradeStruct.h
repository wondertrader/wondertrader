
#pragma once

#pragma pack(1)


typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];

	char			Password[41];
	char			ExtraPassword[41];
	char			MachineInfo[2048];
	char			AuthToken[2048];
	char			ExtendAttrs[512];
}tagXTReqUserLoginField;

typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
}tagXTReqQryAccountField;

typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
}tagXTReqQryPositionField;

typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	int				TimeBegin;				//起始时间 HHMMSS
	int				TimeEnd;				//截止时间 HHMMSS
	int				Mode;					//查询报单模式类型 0=全部报单  1=可撤报单
}tagXTReqQryOrderField;

typedef struct
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	int				TimeBegin;				//起始时间 HHMMSS
	int				TimeEnd;				//截止时间 HHMMSS
}tagXTReqQryTradeField;

typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	char			Exchange[16];			//交易所
	char			Code[32];				//代码
	char			OrderRef[32];			//报单编号
	char			PriceType[2];			//报单价格条件
	char			Direction[2];			//方向
	char			Offset[2];				//开平标志
	char			Hedge[2];				//套保标志
	double			LimitPrice;				//委托价格
	int				VolumeOrigin;			//委托数量
}tagXTReqOrderInsertField;

typedef struct  
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	char			Exchange[16];			//交易所
	char			Code[32];				//代码
	char			ActionRef[32];			//撤单编号
	char			OrderSysID[32];			//由交易所或柜台生成的报单编号
}tagXTReqOrderCancelField;


typedef struct  
{

}tagXTRspUserLoginField;

typedef struct
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	char			Exchange[16];			//交易所
	char			Code[32];				//代码
	char			OrderRef[32];			//报单编号
	char			PriceType[2];			//报单价格条件
	char			Direction[2];			//方向
	char			Offset[2];				//开平标志
	char			Hedge[2];				//套保标志
	double			LimitPrice;				//委托价格
	int				VolumeOrigin;			//委托数量
	char			OrderSysID[32];			//由交易所或柜台生成的报单编号
	char			OrderStatus[2];			//报单状态
	int				VolumeTraded;			//已成交的数量
	int				VolumeRemain;			//剩余数量
	int				InsertDate;				//报单的日期
	int				InsertTime;				//报单的时间
	char			StatusMsg[256];			//
}tagXTOrderField;

typedef struct
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	char			Exchange[16];			//交易所
	char			Code[32];				//代码
	char			OrderRef[32];			//报单编号
	char			TradeID[32];			//成交编号
	char			Direction[2];			//方向
	char			OrderSysID[32];			//由交易所或柜台生成的报单编号
	char			Offset[2];				//开平标志
	char			Hedge[2];				//套保标志
	double			Price;					//成交价格
	int				Volume;					//成交数量
	int				TradeDate;				//交易日期
	int				TradeTime;				//交易时间
}tagXTTradeField;

typedef struct
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	double			FrozenMargin;			//冻结的保证金
	double			FrozenCash;				//冻结的资金
	double			Available;				//可用资金
}tagXTRspAccountField;


typedef struct
{
	int				Line;
	char			UserType[2];
	char			UserID[32];
	char			InvestorID[32];
	char			Exchange[16];			//交易所
	char			Code[32];				//代码
	char			PosiDirection[2];		//持仓多空方向：买入还是卖出 
	char			Hedge[2];				//套保标志
	int				YdPosition;				//上日持仓
	int				Position;				//今日持仓
	double			PositionCost;			//持仓成本价：该持仓的平均成本
	int				FrozenPosition;			//冻结的数量
	double			UseMargin;				//占用的保证金
}tagXTRspPositionField;

typedef struct  
{
	int				ErrorID;				//如果等于0，表示无错误
	char			ErrorMsg[256];
}tagXTRspInfoField;












#pragma pack()

