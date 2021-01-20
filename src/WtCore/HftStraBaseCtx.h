/*!
 * \file HftStraBaseCtx.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <unordered_map>

#include "../Includes/IHftStraCtx.h"
#include "../Share/BoostFile.hpp"
#include "ITrdNotifySink.h"

NS_OTP_BEGIN

class WtHftEngine;
class TraderAdapter;

class HftStraBaseCtx : public IHftStraCtx, public ITrdNotifySink
{
public:
	HftStraBaseCtx(WtHftEngine* engine, const char* name);
	virtual ~HftStraBaseCtx();

	void setTrader(TraderAdapter* trader);

public:
	//////////////////////////////////////////////////////////////////////////
	//IHftStraCtx 接口
	virtual uint32_t id() override;

	virtual void on_init() override;

	virtual void on_tick(const char* stdCode, WTSTickData* newTick) override;

	virtual void on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar) override;

	virtual void on_session_begin() override;

	virtual void on_session_end() override;

	virtual bool stra_cancel(uint32_t localid) override;

	virtual OrderIDs stra_cancel(const char* stdCode, bool isBuy, double qty) override;

	virtual OrderIDs stra_buy(const char* stdCode, double price, double qty, const char* userTag) override;

	virtual OrderIDs stra_sell(const char* stdCode, double price, double qty, const char* userTag) override;

	virtual WTSCommodityInfo* stra_get_comminfo(const char* stdCode) override;

	virtual WTSKlineSlice* stra_get_bars(const char* stdCode, const char* period, uint32_t count) override;

	virtual WTSTickSlice* stra_get_ticks(const char* stdCode, uint32_t count) override;

	virtual WTSTickData* stra_get_last_tick(const char* stdCode) override;

	virtual void stra_log_text(const char* fmt, ...) override;

	virtual double stra_get_position(const char* stdCode) override;
	virtual double stra_get_position_profit(const char* stdCode) override;
	virtual double stra_get_price(const char* stdCode) override;
	virtual double stra_get_undone(const char* stdCode) override;

	virtual uint32_t stra_get_date() override;
	virtual uint32_t stra_get_time() override;
	virtual uint32_t stra_get_secs() override;

	virtual void stra_sub_ticks(const char* stdCode) override;

	virtual void stra_save_user_data(const char* key, const char* val) override;

	virtual const char* stra_load_user_data(const char* key, const char* defVal = "") override;

	//////////////////////////////////////////////////////////////////////////
	virtual void on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price) override;

	virtual void on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled) override;

	virtual void on_channel_ready() override;

	virtual void on_channel_lost() override;

	virtual void on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message) override;

protected:
	const char* get_inner_code(const char* stdCode);

	void	load_userdata();
	void	save_userdata();

	void	init_outputs();

	void	do_set_position(const char* stdCode, double qty, double price = 0.0, const char* userTag = "");
	void	update_dyn_profit(const char* stdCode, WTSTickData* newTick);

	inline void	log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee, const char* userTag);
	inline void	log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
		double profit, double maxprofit, double maxloss, double totalprofit, const char* enterTag, const char* exitTag);

	inline const char* getOrderTag(uint32_t localid);


protected:
	uint32_t		_context_id;
	WtHftEngine*	_engine;
	TraderAdapter*	_trader;

	std::unordered_map<std::string, std::string> _code_map;

	BoostFilePtr	_sig_logs;
	BoostFilePtr	_close_logs;
	BoostFilePtr	_trade_logs;
	BoostFilePtr	_fund_logs;

	//用户数据
	typedef std::unordered_map<std::string, std::string> StringHashMap;
	StringHashMap	_user_datas;
	bool			_ud_modified;

private:
	typedef struct _DetailInfo
	{
		bool		_long;
		double		_price;
		double		_volumn;
		uint64_t	_opentime;
		uint32_t	_opentdate;
		double		_max_profit;
		double		_max_loss;
		double		_profit;
		char		_usertag[32];

		_DetailInfo()
		{
			memset(this, 0, sizeof(_DetailInfo));
		}
	} DetailInfo;

	typedef struct _PosInfo
	{
		double		_volumn;
		double		_closeprofit;
		double		_dynprofit;

		std::vector<DetailInfo> _details;

		_PosInfo()
		{
			_volumn = 0;
			_closeprofit = 0;
			_dynprofit = 0;
		}
	} PosInfo;
	typedef std::unordered_map<std::string, PosInfo> PositionMap;
	PositionMap		_pos_map;

	typedef std::unordered_map<uint32_t, std::string> OrderMap;
	OrderMap		_orders;

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

	typedef std::unordered_map<std::string, double> PriceMap;
	PriceMap		_price_map;
};

NS_OTP_END
