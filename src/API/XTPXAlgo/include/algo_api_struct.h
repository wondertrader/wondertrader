#ifndef _XTP_ALGO_API_STRUCT_H_
#define _XTP_ALGO_API_STRUCT_H_

#include "algo_data_type.h"
#include "xtp_api_struct_common.h"

#pragma pack(8)

///策略信息结构体
typedef struct XTPStrategyInfoStruct
{
	uint16_t				m_strategy_type;		///< 策略类型
	XTPStrategyStateType	m_strategy_state;		///< 策略状态
	uint64_t				m_client_strategy_id;	///< 客户策略id
	uint64_t				m_xtp_strategy_id;		///< xtp策略id
} XTPStrategyInfoStruct;

///策略状态结构体
typedef struct XTPStrategyStateReportStruct
{
	XTPStrategyInfoStruct		m_strategy_info;			///< 策略信息	
	int64_t						m_strategy_qty;				///< 策略总量
	int64_t						m_strategy_ordered_qty;		///< 策略已委托数量
	int64_t						m_strategy_cancelled_qty;	///< 策略已撤单数量
	int64_t						m_strategy_execution_qty;	///< 策略已成交数量
	int64_t						m_strategy_unclosed_qty;	///< 策略未平仓数量(T0卖出数量-买入数量)
	double						m_strategy_asset;			///< 策略总金额
	double						m_strategy_ordered_asset;	///< 策略已委托金额
	double						m_strategy_execution_asset;	///< 策略已成交金额
	double						m_strategy_execution_price;	///< 策略执行价格
	double						m_strategy_market_price;	///< 策略市场价
	double						m_strategy_price_diff;		///< 策略执行价差	
	double						m_strategy_asset_diff;		///< 策略执行绩效(T0资金预净收入)
	XTPRI						m_error_info;				///< 错误信息
} XTPStrategyStateReport;

#pragma pack()

#endif //_XTP_ALGO_API_STRUCT_H_
