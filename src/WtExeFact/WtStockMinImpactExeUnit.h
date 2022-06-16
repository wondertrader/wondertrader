/*!
 * \file WtStockMinImpactExeUnit.h
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * ��С���ִ�е�Ԫ
 */
#pragma once
#include "../Includes/ExecuteDefs.h"
#include "WtOrdMon.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"
#include "../Share/fmtlib.h"

USING_NS_WTP;

#define BESTPX -1 // ��������
#define LASTPX 0  // ���¼�
#define MARKET 1  // ���ּ�
#define AUTOPX 2  // �Զ�

class WtStockMinImpactExeUnit : public ExecuteUnit
{
private:
	std::vector<std::string> PriceModeNames = {
		"BESTPX",		//���ż�
		"LASTPX",		//���¼�
		"MARKET",		//���ּ�
		"AUTOPX"		//�Զ�
	};

public:
	WtStockMinImpactExeUnit();
	virtual ~WtStockMinImpactExeUnit();

private:
	void	do_calc();
	bool	is_clear();
public:
	/*
	 *	����ִ������������
	 */
	virtual const char* getFactName() override;

	/*
	 *	ִ�е�Ԫ����
	 */
	virtual const char* getName() override;

	/*
	 *	��ʼ��ִ�е�Ԫ
	 *	ctx		ִ�е�Ԫ���л���
	 *	code	����ĺ�Լ����
	 */
	virtual void init(ExecuteContext* ctx, const char* stdCode, WTSVariant* cfg) override;

	/*
	 *	�����ر�
	 *	localid	���ص���
	 *	code	��Լ����
	 *	isBuy	��or��
	 *	leftover	ʣ������
	 *	price	ί�м۸�
	 *	isCanceled	�Ƿ��ѳ���
	 */
	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled) override;

	/*
	 *	tick���ݻص�
	 *	newTick	���µ�tick����
	 */
	virtual void on_tick(WTSTickData* newTick) override;

	/*
	 *	�ɽ��ر�
	 *	code	��Լ����
	 *	isBuy	��or��
	 *	vol		�ɽ�����,����û������,ͨ��isBuyȷ�����뻹������
	 *	price	�ɽ��۸�
	 */
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	/*
	 *	�µ�����ر�
	 */
	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

	/*
	 *	�����µ�Ŀ���λ
	 *	code	��Լ����
	 *	newVol	�µ�Ŀ���λ
	 */
	virtual void set_position(const char* stdCode, double newVol) override;

	/*
	 *	�����µ�Ŀ���λ
	 *	code	��Լ����
	 *	newVol	�µ�Ŀ���λ
	 */
	virtual void set_amount(const char* stdCode, double newAmount);

	/*
	 *	����ȫ���ֲ�
	 *	stdCode	��Լ����
	 */
	virtual void clear_all_position(const char* stdCode) override;

	/*
	 *	����ͨ�������ص�
	 */
	virtual void on_channel_ready() override;

	/*
	 *	����ͨ����ʧ�ص�
	 */
	virtual void on_channel_lost() override;

private:
	WTSTickData* _last_tick;	//��һ������
	double		_target_pos;	//Ŀ���λ
	double		_target_amount;  // Ŀ����
	StdUniqueMutex	_mtx_calc;
	WTSCommodityInfo* _comm_info;
	WTSSessionInfo* _sess_info;

	//////////////////////////////////////////////////////////////////////////
	//ִ�в���
	int32_t		_price_offset;
	uint32_t	_expire_secs;
	int32_t		_price_mode;
	uint32_t	_entrust_span;
	bool		_by_rate;
	double		_order_lots;
	double		_qty_rate;
	double		_min_order;

	WtOrdMon	_orders_mon;
	uint32_t	_cancel_cnt;
	uint32_t	_cancel_times;

	uint64_t	_last_place_time;
	uint64_t	_last_tick_time;
	bool		_is_clear;
	bool		_is_target_pos;
	bool		_is_KC {false};			// �Ƿ��ǿƴ����Ʊ
public:
	inline int round_hands(double hands)
	{
		return (int)((hands + 50) / 100) * 100;
	}
};

