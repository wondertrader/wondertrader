#ifndef _XTP_ALGO_API_STRUCT_H_
#define _XTP_ALGO_API_STRUCT_H_

#include "algo_data_type.h"
#include "xtp_api_struct_common.h"

#pragma pack(8)

///������Ϣ�ṹ��
typedef struct XTPStrategyInfoStruct
{
	uint16_t				m_strategy_type;		///< ��������
	XTPStrategyStateType	m_strategy_state;		///< ����״̬
	uint64_t				m_client_strategy_id;	///< �ͻ�����id
	uint64_t				m_xtp_strategy_id;		///< xtp����id
} XTPStrategyInfoStruct;

///����״̬�ṹ��
typedef struct XTPStrategyStateReportStruct
{
	XTPStrategyInfoStruct		m_strategy_info;			///< ������Ϣ	
	int64_t						m_strategy_qty;				///< ��������
	int64_t						m_strategy_ordered_qty;		///< ������ί������
	int64_t						m_strategy_cancelled_qty;	///< �����ѳ�������
	int64_t						m_strategy_execution_qty;	///< �����ѳɽ�����
	int64_t						m_strategy_unclosed_qty;	///< ����δƽ������(T0��������-��������)
	double						m_strategy_asset;			///< �����ܽ��
	double						m_strategy_ordered_asset;	///< ������ί�н��
	double						m_strategy_execution_asset;	///< �����ѳɽ����
	double						m_strategy_execution_price;	///< ����ִ�м۸�
	double						m_strategy_market_price;	///< �����г���
	double						m_strategy_price_diff;		///< ����ִ�м۲�	
	double						m_strategy_asset_diff;		///< ����ִ�м�Ч(T0�ʽ�Ԥ������)
	XTPRI						m_error_info;				///< ������Ϣ
} XTPStrategyStateReport;

#pragma pack()

#endif //_XTP_ALGO_API_STRUCT_H_
