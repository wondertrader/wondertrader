/*!
* \file MfStraBaseCtx.h
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#pragma once
#include "../Includes/FasterDefs.h"
#include "../Includes/ISelStraCtx.h"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/BoostFile.hpp"
#include "../Share/fmtlib.h"

NS_WTP_BEGIN

class WtSelEngine;


class SelStraBaseCtx : public ISelStraCtx
{
public:
	SelStraBaseCtx(WtSelEngine* engine, const char* name, int32_t slippage);
	virtual ~SelStraBaseCtx();

private:
	void	init_outputs();
	inline void log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag = "");
	inline void	log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag = "", double fee = 0.0);
	inline void	log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
		double profit, double totalprofit = 0, const char* enterTag = "", const char* exitTag = "");

	void	save_data(uint32_t flag = 0xFFFFFFFF);
	void	load_data(uint32_t flag = 0xFFFFFFFF);

	void	load_userdata();
	void	save_userdata();

	void	update_dyn_profit(const char* stdCode, double price);

	void	do_set_position(const char* stdCode, double qty, const char* userTag = "", bool bTriggered = false);
	void	append_signal(const char* stdCode, double qty, const char* userTag = "");

protected:
	template<typename... Args>
	void log_debug(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_debug(buffer);
	}

	template<typename... Args>
	void log_info(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_info(buffer);
	}

	template<typename... Args>
	void log_error(const char* format, const Args& ...args)
	{
		const char* buffer = fmtutil::format(format, args...);
		stra_log_error(buffer);
	}

public:
	virtual uint32_t id() { return _context_id; }

	//�ص�����
	virtual void on_init() override;
	virtual void on_session_begin(uint32_t uTDate) override;
	virtual void on_session_end(uint32_t uTDate) override;
	virtual void on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy = true) override;
	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;
	virtual bool on_schedule(uint32_t curDate, uint32_t curTime, uint32_t fireTime) override;

	virtual void enum_position(FuncEnumSelPositionCallBack cb) override;


	//////////////////////////////////////////////////////////////////////////
	//���Խӿ�
	virtual double stra_get_position(const char* stdCode, bool bOnlyValid = false, const char* userTag = "") override;
	virtual void stra_set_position(const char* stdCode, double qty, const char* userTag = "") override;
	virtual double stra_get_price(const char* stdCode) override;

	virtual uint32_t stra_get_date() override;
	virtual uint32_t stra_get_time() override;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) override;
	virtual WTSSessionInfo* stra_get_sessinfo(const char* stdCode) override;
	virtual WTSKlineSlice*	stra_get_bars(const char* stdCode, const char* period, uint32_t count) override;
	virtual WTSTickSlice*	stra_get_ticks(const char* stdCode, uint32_t count) override;
	virtual WTSTickData*	stra_get_last_tick(const char* stdCode) override;

	/*
	 *	��ȡ���º�Լ����
	 */
	virtual std::string		stra_get_rawcode(const char* stdCode) override;

	virtual void stra_sub_ticks(const char* stdCode) override;

	virtual void stra_log_info(const char* message) override;
	virtual void stra_log_debug(const char* message) override;
	virtual void stra_log_warn(const char* message) override;
	virtual void stra_log_error(const char* message) override;

	virtual void stra_save_user_data(const char* key, const char* val) override;

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") override;

protected:
	uint32_t		_context_id;
	WtSelEngine*	_engine;
	int32_t			_slippage;

	uint64_t		_total_calc_time;	//�ܼ���ʱ��
	uint32_t		_emit_times;		//�ܼ������

	uint32_t		_schedule_date;
	uint32_t		_schedule_time;

	typedef struct _KlineTag
	{
		bool			_closed;

		_KlineTag() :_closed(false){}

	} KlineTag;
	typedef faster_hashmap<LongKey, KlineTag> KlineTags;
	KlineTags	_kline_tags;

	typedef faster_hashmap<LongKey, double> PriceMap;
	PriceMap		_price_map;

	typedef struct _DetailInfo
	{
		bool		_long;
		double		_price;
		double		_volume;
		uint64_t	_opentime;
		uint32_t	_opentdate;
		double		_max_profit;
		double		_max_loss;
		double		_profit;
		char		_opentag[32];

		_DetailInfo()
		{
			memset(this, 0, sizeof(_DetailInfo));
		}
	} DetailInfo;

	typedef struct _PosInfo
	{
		double		_volume;
		double		_closeprofit;
		double		_dynprofit;
		double		_frozen;
		uint32_t	_frozen_date;

		std::vector<DetailInfo> _details;

		_PosInfo()
		{
			_volume = 0;
			_closeprofit = 0;
			_dynprofit = 0;
			_frozen = 0;
			_frozen_date = 0;
		}
	} PosInfo;
	typedef faster_hashmap<LongKey, PosInfo> PositionMap;
	PositionMap		_pos_map;

	typedef struct _SigInfo
	{
		double		_volume;
		std::string	_usertag;
		double		_sigprice;
		bool		_triggered;
		uint64_t	_gentime;

		_SigInfo()
		{
			_volume = 0;
			_sigprice = 0;
			_triggered = false;
			_gentime = 0;
		}
	}SigInfo;
	typedef faster_hashmap<LongKey, SigInfo>	SignalMap;
	SignalMap		_sig_map;

	BoostFilePtr	_trade_logs;
	BoostFilePtr	_close_logs;
	BoostFilePtr	_fund_logs;
	BoostFilePtr	_sig_logs;
	BoostFilePtr	_pos_logs;

	//�Ƿ��ڵ����еı��
	bool			_is_in_schedule;	//�Ƿ����Զ�������

	//�û�����
	typedef faster_hashmap<LongKey, std::string> StringHashMap;
	StringHashMap	_user_datas;
	bool			_ud_modified;

	typedef struct _StraFundInfo
	{
		double	_total_profit;
		double	_total_dynprofit;
		double	_total_fees;

		_StraFundInfo()
		{
			memset(this, 0, sizeof(_StraFundInfo));
		}
	} StraFundInfo;

	StraFundInfo		_fund_info;

	//tick�����б�
	faster_hashset<LongKey> _tick_subs;
};


NS_WTP_END