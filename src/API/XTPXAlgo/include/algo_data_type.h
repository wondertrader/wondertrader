
#ifndef _XTP_ALGO_DATA_TYPE_H_
#define _XTP_ALGO_DATA_TYPE_H_

///@brief XTPStrategyStateType����״̬����
typedef uint8_t XTPStrategyStateType;

///������
#define XTP_STRATEGY_STATE_CREATING		0
///�Ѵ���
#define XTP_STRATEGY_STATE_CREATED		1
///��ʼִ����
#define XTP_STRATEGY_STATE_STARTING		2
///��ִ��
#define XTP_STRATEGY_STATE_STARTED		3
///ֹͣ��
#define XTP_STRATEGY_STATE_STOPPING		4
///��ֹͣ
#define XTP_STRATEGY_STATE_STOPPED		5
///������
#define XTP_STRATEGY_STATE_DESTROYING	6
///������
#define XTP_STRATEGY_STATE_DESTROYED	7
///��������
#define XTP_STRATEGY_STATE_ERROR		8

#endif //_XTP_ALGO_DATA_TYPE_H_
