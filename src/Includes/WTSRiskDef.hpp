/*!
 * \file WTSRiskDef.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief WT风控相关数据定义
 */
#pragma once
#include "WTSObject.hpp"

NS_WTP_BEGIN
typedef struct _TradeStatInfo
{
	char		_code[MAX_INSTRUMENT_LENGTH];
	//开平统计
	double	l_openvol;	//当日开多仓量
	double	l_closevol;	//当日平多仓量
	double	l_closetvol;//当日平今多仓量
	double	s_openvol;	//当日开空仓量
	double	s_closevol;	//当日平空仓量
	double	s_closetvol;//当日平今空仓量

	//挂单统计
	uint32_t	b_orders;	//委买笔数
	double		b_ordqty;	//委买数量
	uint32_t	s_orders;	//委卖笔数
	double		s_ordqty;	//委卖数量

	//撤单统计
	uint32_t	b_cancels;	//撤买笔数
	double		b_canclqty;	//撤买数量
	uint32_t	s_cancels;	//撤卖笔数
	double		s_canclqty;	//撤卖数量

	//自动撤单统计
	uint32_t	b_auto_cancels;		//撤买笔数
	double		b_auto_canclqty;	//撤买数量
	uint32_t	s_auto_cancels;		//撤卖笔数
	double		s_auto_canclqty;	//撤卖数量

	//错单统计
	uint32_t	b_wrongs;	//错单笔数
	double		b_wrongqty;	//错单数量
	uint32_t	s_wrongs;	//错单笔数
	double		s_wrongqty;	//错单数量

	uint32_t	_infos;		//信息量

	_TradeStatInfo()
	{
		memset(this, 0, sizeof(_TradeStatInfo));
	}
} TradeStatInfo;

class WTSTradeStateInfo : public WTSObject
{
protected:
	WTSTradeStateInfo(){}

public:
	static WTSTradeStateInfo* create(const char* code)
	{
		WTSTradeStateInfo* pRet = new WTSTradeStateInfo();
		wt_strcpy(pRet->_trd_stat_info._code, code);

		return pRet;
	}

	inline TradeStatInfo&	statInfo(){ return _trd_stat_info; }
	inline const TradeStatInfo& statInfo() const{ return _trd_stat_info; }

	inline const char* code() const{ return _trd_stat_info._code; }

	inline double open_volume_long() const{ return _trd_stat_info.l_openvol; }
	inline double close_volume_long() const{ return _trd_stat_info.l_closevol; }
	inline double closet_volume_long() const{ return _trd_stat_info.l_closetvol; }
	inline double open_volume_short() const{ return _trd_stat_info.s_openvol; }
	inline double close_volume_short() const{ return _trd_stat_info.s_closevol; }
	inline double closet_volume_short() const{ return _trd_stat_info.s_closetvol; }

	inline uint32_t orders_buy() const{ return _trd_stat_info.b_orders; }
	inline double ordqty_buy() const{ return _trd_stat_info.b_ordqty; }
	inline uint32_t orders_sell() const{ return _trd_stat_info.s_orders; }
	inline double ordqty_sell() const{ return _trd_stat_info.s_ordqty; }

	inline uint32_t cancels_buy() const{ return _trd_stat_info.b_cancels; }
	inline double cancelqty_buy() const{ return _trd_stat_info.b_canclqty; }
	inline uint32_t cancels_sell() const{ return _trd_stat_info.s_cancels; }
	inline double cancelqty_sell() const{ return _trd_stat_info.s_canclqty; }

	inline uint32_t total_cancels() const{ return _trd_stat_info.b_cancels + _trd_stat_info.s_cancels; }
	inline uint32_t total_orders() const { return _trd_stat_info.b_orders + _trd_stat_info.s_orders; }

	inline uint32_t infos() const { return _trd_stat_info._infos; }

private:
	TradeStatInfo	_trd_stat_info;
};

//组合资金数据
typedef struct _WTSFundStruct
{
	double		_predynbal;		//期初动态权益
	double		_prebalance;	//期初静态权益
	double		_balance;		//静态权益
	double		_profit;		//平仓盈亏
	double		_dynprofit;		//浮动盈亏
	double		_fees;			//佣金
	uint32_t	_last_date;		//上次结算交易日

	double		_max_dyn_bal;	//日内最大净值
	uint32_t	_max_time;		//日内高点产生时间
	double		_min_dyn_bal;	//日内最小净值
	uint32_t	_min_time;		//日内低点产生时间

	int64_t		_update_time;	//数据更新时间

	typedef struct _DynBalPair
	{
		uint32_t	_date;
		double		_dyn_balance;

		_DynBalPair()
		{
			memset(this, 0, sizeof(_DynBalPair));
		}
	} DynBalPair;

	DynBalPair	_max_md_dyn_bal;	//最大动态净值
	DynBalPair	_min_md_dyn_bal;	//最小动态净值

	_WTSFundStruct()
	{
		memset(this, 0, sizeof(_WTSFundStruct));
		_max_dyn_bal = DBL_MAX;
		_min_dyn_bal = DBL_MAX;
	}
} WTSFundStruct;


class WTSPortFundInfo : public WTSObject
{
protected:
	WTSPortFundInfo(){}

public:
	static WTSPortFundInfo* create()
	{
		WTSPortFundInfo* pRet = new WTSPortFundInfo();
		return pRet;
	}

	WTSFundStruct&	fundInfo(){ return _fund_info; }
	const WTSFundStruct& fundInfo() const{ return _fund_info; }

	double predynbalance() const{ return _fund_info._predynbal; }
	double balance() const{ return _fund_info._balance; }
	double profit() const{ return _fund_info._profit; }
	double dynprofit() const{ return _fund_info._dynprofit; }
	double fees() const{ return _fund_info._fees; }

	double max_dyn_balance() const{ return _fund_info._max_dyn_bal; }
	double min_dyn_balance() const{ return _fund_info._min_dyn_bal; }

	double max_md_dyn_balance() const{ return _fund_info._max_md_dyn_bal._dyn_balance; }
	double min_md_dyn_balance() const{ return _fund_info._min_md_dyn_bal._dyn_balance; }

	uint32_t max_dynbal_time() const{ return _fund_info._max_time; }
	uint32_t min_dynbal_time() const{ return _fund_info._min_time; }

	uint32_t last_settle_date() const{ return _fund_info._last_date; }

	uint32_t max_md_dynbal_date() const{ return _fund_info._max_md_dyn_bal._date; }
	uint32_t min_md_dynbal_date() const{ return _fund_info._min_md_dyn_bal._date; }


private:
	WTSFundStruct	_fund_info;
};

NS_WTP_END