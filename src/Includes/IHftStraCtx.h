/*!
 * \file IHftStraCtx.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>
#include <string>
#include "ExecuteDefs.h"

#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSCommodityInfo;
class WTSTickSlice;
class WTSKlineSlice;
class WTSTickData;
struct WTSBarStruct;

/*
 *	�������
 */
static const int HFT_OrderFlag_Nor = 0;
static const int HFT_OrderFlag_FAK = 1;
static const int HFT_OrderFlag_FOK = 2;

class IHftStraCtx
{
public:
	IHftStraCtx(const char* name) :_name(name) {}
	virtual ~IHftStraCtx() {}

	const char* name() const { return _name.c_str(); }

public:
	virtual uint32_t id() = 0;

	//�ص�����
	virtual void on_init() = 0;
	virtual void on_tick(const char* stdCode, WTSTickData* newTick) = 0;
	virtual void on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue) = 0;
	virtual void on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl) = 0;
	virtual void on_transaction(const char* stdCode, WTSTransData* newTrans) = 0;
	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) {}
	virtual void on_session_begin(uint32_t uTDate) {}
	virtual void on_session_end(uint32_t uTDate) {}
	/*
	 *	�ز�����¼�
	 *	ֻ�ڻز��²Żᴥ��
	 */
	virtual void on_bactest_end() {};

	virtual void on_tick_updated(const char* stdCode, WTSTickData* newTick) {}
	virtual void on_ordque_updated(const char* stdCode, WTSOrdQueData* newOrdQue) {}
	virtual void on_orddtl_updated(const char* stdCode, WTSOrdDtlData* newOrdDtl) {}
	virtual void on_trans_updated(const char* stdCode, WTSTransData* newTrans) {}

	//���Խӿ�
	virtual bool		stra_cancel(uint32_t localid) = 0;
	virtual OrderIDs	stra_cancel(const char* stdCode, bool isBuy, double qty) = 0;

	/*
	 *	�µ��ӿ�: ����
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 *	@bForceClose	ǿƽ��־�����Ϊtrue����ǿ������ƽ��
	 */
	virtual OrderIDs	stra_buy(const char* stdCode, double price, double qty, const char* userTag, int flag = 0, bool bForceClose = false) = 0;

	/*
	 *	�µ��ӿ�: ����
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 *	@bForceClose	ǿƽ��־�����Ϊtrue����ǿ������ƽ��
	 */
	virtual OrderIDs	stra_sell(const char* stdCode, double price, double qty, const char* userTag, int flag = 0, bool bForceClose = false) = 0;

	/*
	 *	�µ��ӿ�: ����
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok
	 */
	virtual uint32_t	stra_enter_long(const char* stdCode, double price, double qty, const char* userTag, int flag = 0) { return 0; }

	/*
	 *	�µ��ӿ�: ����
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok
	 */
	virtual uint32_t	stra_enter_short(const char* stdCode, double price, double qty, const char* userTag, int flag = 0) { return 0; }

	/*
	 *	�µ��ӿ�: ƽ��
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@isToday	�Ƿ��֣�Ĭ��false
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 */
	virtual uint32_t	stra_exit_long(const char* stdCode, double price, double qty, const char* userTag, bool isToday = false, int flag = 0) { return 0; }

	/*
	 *	�µ��ӿ�: ƽ��
	 *
	 *	@stdCode	��Լ����
	 *	@price		�µ��۸�0�����м۵�
	 *	@qty		�µ�����
	 *	@isToday	�Ƿ��֣�Ĭ��false
	 *	@flag		�µ���־: 0-normal��1-fak��2-fok��Ĭ��0
	 */
	virtual uint32_t	stra_exit_short(const char* stdCode, double price, double qty, const char* userTag, bool isToday = false, int flag = 0) { return 0; }

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) = 0;
	virtual WTSKlineSlice*	stra_get_bars(const char* stdCode, const char* period, uint32_t count) = 0;
	virtual WTSTickSlice*	stra_get_ticks(const char* stdCode, uint32_t count) = 0;
	virtual WTSOrdDtlSlice*	stra_get_order_detail(const char* stdCode, uint32_t count) = 0;
	virtual WTSOrdQueSlice*	stra_get_order_queue(const char* stdCode, uint32_t count) = 0;
	virtual WTSTransSlice*	stra_get_transaction(const char* stdCode, uint32_t count) = 0;
	virtual WTSTickData*	stra_get_last_tick(const char* stdCode) = 0;

	/*
	 *	��ȡ���º�Լ����
	 */
	virtual std::string		stra_get_rawcode(const char* stdCode) = 0;

	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false) = 0;
	virtual double stra_get_position_avgpx(const char* stdCode) = 0;
	virtual double stra_get_position_profit(const char* stdCode) = 0;
	virtual double stra_get_price(const char* stdCode) = 0;
	virtual double stra_get_undone(const char* stdCode) = 0;

	virtual uint32_t stra_get_date() = 0;
	virtual uint32_t stra_get_time() = 0;
	virtual uint32_t stra_get_secs() = 0;

	virtual void stra_sub_ticks(const char* stdCode) = 0;
	virtual void stra_sub_order_queues(const char* stdCode) = 0;
	virtual void stra_sub_order_details(const char* stdCode) = 0;
	virtual void stra_sub_transactions(const char* stdCode) = 0;

	virtual void stra_log_info(const char* message) = 0;
	virtual void stra_log_debug(const char* message) = 0;
	virtual void stra_log_error(const char* message) = 0;
	virtual void stra_log_warn(const char* message) {}

	virtual void stra_save_user_data(const char* key, const char* val) {}

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") { return defVal; }

protected:
	std::string _name;
};

NS_WTP_END