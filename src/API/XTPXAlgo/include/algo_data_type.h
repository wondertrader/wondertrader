
#ifndef _XTP_ALGO_DATA_TYPE_H_
#define _XTP_ALGO_DATA_TYPE_H_

///@brief XTPStrategyStateType策略状态类型
typedef uint8_t XTPStrategyStateType;

///创建中
#define XTP_STRATEGY_STATE_CREATING		0
///已创建
#define XTP_STRATEGY_STATE_CREATED		1
///开始执行中
#define XTP_STRATEGY_STATE_STARTING		2
///已执行
#define XTP_STRATEGY_STATE_STARTED		3
///停止中
#define XTP_STRATEGY_STATE_STOPPING		4
///已停止
#define XTP_STRATEGY_STATE_STOPPED		5
///销毁中
#define XTP_STRATEGY_STATE_DESTROYING	6
///已销毁
#define XTP_STRATEGY_STATE_DESTROYED	7
///发生错误
#define XTP_STRATEGY_STATE_ERROR		8

#endif //_XTP_ALGO_DATA_TYPE_H_
