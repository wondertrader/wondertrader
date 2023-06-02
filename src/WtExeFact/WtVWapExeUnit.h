#pragma once
/*
23.5.23--zhaoyv--VWAP
*/
#include "WtOrdMon.h"
#include "../Includes/ExecuteDefs.h"
#include "../Share/StdUtils.hpp"
#include <fstream>
#include "rapidjson/document.h"
USING_NS_WTP;

class WtVWapExeUnit : public ExecuteUnit {

public:
	WtVWapExeUnit();
	virtual ~WtVWapExeUnit();

private:
	void	do_calc();
	void	fire_at_once(double qty);

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
	bool		_channel_ready;
	StdUniqueMutex	_mtx_calc;

	WTSCommodityInfo* _comm_info;
	WTSSessionInfo*	_sess_info;
	uint32_t	_cancel_times;//��������



	//////////////////////////////////////////////////////////////////////////
	//ִ�в���
	WtOrdMon		_orders_mon;
	uint32_t		_cancel_cnt;
	vector<double>	VwapAim;			//���Ӽǣ�Ŀ��VWapԤ�� �ܱ�����
	//////////////////////////////////////////////////////////////////////////
	//����
	uint32_t		_total_secs;	//ִ����ʱ��,��λs
	uint32_t		_total_times;	//��ִ�д���
	uint32_t		_tail_secs;		//ִ��β��ʱ��
	uint32_t		_ord_sticky;	//�ҵ�ʱ��,��λs
	uint32_t		_price_mode;	//�۸�ģʽ: 0-���¼�,1-���ż�,2-���ּ�
	uint32_t		_price_offset;	//�ҵ��۸�ƫ��,����ڼ����۸�ƫ��,��+��-
	uint32_t        _begin_time;	//��ʼʱ�� ��1000->10:00��
	uint32_t		_end_time;		//����ʱ�� ��1030->10:30��
	double			_min_open_lots; //��С��������
	double			_order_lots;	//���η�������

	//////////////////////////////////////////////////////////////////////////
	//��ʱ����
	double			_this_target;	//����Ŀ���λ
	uint32_t		_fire_span;		//�������//ms
	uint32_t		_fired_times;	//��ִ�д���
	uint64_t		_last_fire_time; //�ϴ���ִ�е�ʱ��
	uint64_t		_last_place_time;//�ϸ��µ�ʱ��
	uint64_t		_last_tick_time;//�ϸ�tickʱ��
	double			_Vwap_vol ;		//vwap��λʱ���µ���
	double			_Vwap_prz;		//vwap�۸�

	std::atomic<bool> _in_calc;

	typedef struct _CalcFlag
	{
		bool _result;
		std::atomic<bool>* _flag;
		_CalcFlag(std::atomic<bool>*flag) :_flag(flag) {
			_result = _flag->exchange(true, std::memory_order_acq_rel);
		}

		~_CalcFlag() {
			if (_flag)
				_flag->exchange(false, std::memory_order_acq_rel);
		}
		operator bool() const { return _result; }
	}CalcFlag;
};