/*!
 * \file HftStraContext.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UftStraContext.h"
#include "WtUftEngine.h"
#include "TraderAdapter.h"
#include "WtHelper.h"
#include "ShareManager.h"

#include "../Includes/UftStrategyDefs.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/decimal.h"
#include "../Share/TimeUtils.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

static const uint32_t DATA_SIZE_STEP = 8000;	//信息量每天最多4000

USING_NS_WTP;

inline uint32_t makeUftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

UftStraContext::UftStraContext(WtUftEngine* engine, const char* name)
	: IUftStraCtx(name)
	, _engine(engine)
	, _strategy(NULL)
	, _tradingday(0)
{
	_context_id = makeUftCtxId();
}


UftStraContext::~UftStraContext()
{
}

void UftStraContext::setTrader(TraderAdapter* trader)
{
	_trader = trader;
}

void UftStraContext::on_init()
{
	if (_strategy)
		_strategy->on_init(this);
}


void UftStraContext::on_tick(const char* stdCode, WTSTickData* newTick)
{
	auto it = _positions.find(stdCode);
	if(it != _positions.end())
	{
		WTSCommodityInfo* commInfo = newTick->getContractInfo()->getCommInfo();
		uint32_t volscale = commInfo->getVolScale();
		PosInfo& pInfo = it->second;

		for(auto i = pInfo._valid_idx; i < pInfo._details.size(); i++)
		{
			auto& ds = pInfo._details[i];
			if (ds->_volume == 0)
				ds->_position_profit = 0;
			else
				ds->_position_profit = (newTick->price() - ds->_open_price)*ds->_volume*volscale*(ds->_direct == 0 ? 1 : -1);
		}

		if (decimal::gt(pInfo._volume, 0.0))
			pInfo._dynprofit = newTick->price()*pInfo._volume*volscale - pInfo._opencost;
		else if (decimal::lt(pInfo._volume, 0.0))
			pInfo._dynprofit = newTick->price()*pInfo._volume*volscale + pInfo._opencost;
		else
			pInfo._dynprofit = 0;
	}

	if (_strategy)
		_strategy->on_tick(this, stdCode, newTick);
}

void UftStraContext::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_strategy)
		_strategy->on_order_queue(this, stdCode, newOrdQue);
}

void UftStraContext::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_strategy)
		_strategy->on_order_detail(this, stdCode, newOrdDtl);
}

void UftStraContext::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	if (_strategy)
		_strategy->on_transaction(this, stdCode, newTrans);
}

void UftStraContext::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, times, newBar);
}

void UftStraContext::on_trade(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double vol, double price)
{
	if (!is_my_order(localid))
		return;

	WTSContractInfo* cInfo = _engine->get_contract_info(stdCode);

	uint32_t volscale = cInfo->getCommInfo()->getVolScale();

	PosInfo& pItem = _positions[stdCode];

	uint64_t now = TimeUtils::getLocalTimeNow();

	/*
	 *	By Wesley
	 *	这里要考虑多个策略在相同合约上开相反头寸的情况，开仓可能变成平仓，所以本地持仓只能是净头寸持仓
	 */
	bool isBuy = (isLong && offset==0) || (!isLong && offset!=0);

	if(isBuy)
	{
		double unhandle = vol;

		//买入的时候，如果有空头，就先平空
		if(decimal::lt(pItem._volume, 0))
		{
			double thisQty = min(abs(pItem._volume), vol);

			pItem._volume += thisQty;
			unhandle -= thisQty;

			double left = thisQty;
			for (uint32_t idx = pItem._valid_idx; idx < pItem._details.size(); idx++)
			{
				uft::DetailStruct* pDS = pItem._details[idx];
				//只有索引递增，才递进，不递增就不递进
				if (decimal::eq(pDS->_volume, 0.0) && (idx == pItem._valid_idx + 1))
				{
					pItem._valid_idx++;
					continue;
				}

				//只平空头
				if (pDS->_direct != 1)
				{
					continue;
				}

				if(decimal::eq(left, 0))
					break;

				//计算明细最大平仓量
				double maxQty = std::min(left, pDS->_volume);

				//生成回合明细
				{
					SpinLock lock(_rnd_blk._mutex);
					uint32_t ridx = _rnd_blk._block->_size;
					_rnd_blk._block->_size++;
					uft::RoundStruct& rs = _rnd_blk._block->_rounds[ridx];
					wt_strcpy(rs._code, cInfo->getCode());
					wt_strcpy(rs._exchg, cInfo->getExchg());

					rs._open_price = pDS->_open_price;
					rs._open_time = pDS->_open_time;
					rs._close_price = price;
					rs._close_time = now;
					rs._direct = 1;
					rs._volume = maxQty;
					rs._profit = (rs._open_price - rs._close_price)*maxQty*volscale;

					pItem._total_profit += rs._profit;
					pDS->_closed_profit += rs._profit;
				}

				//落地成交明细
				{
					SpinLock lock(_trd_blk._mutex);
					uint32_t tidx = _trd_blk._block->_size;
					_trd_blk._block->_size++;
					uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
					wt_strcpy(ts._code, cInfo->getCode());
					wt_strcpy(ts._exchg, cInfo->getExchg());
					ts._direct = 1;
					ts._offset = 1;
					ts._price = price;
					ts._volume = maxQty;
					ts._trading_date = _tradingday;
					ts._trading_time = now;
				}

				pDS->_position_profit *= (1 - maxQty / pDS->_volume);
				pDS->_volume -= maxQty;
				pDS->_closed_volume += maxQty;
				pItem._opencost -= maxQty * volscale*pDS->_open_price;
				left -= maxQty;
			}
		}

		//如果还有剩余的没处理，则开多仓
		if (decimal::gt(unhandle, 0))
		{
			SpinLock lock(_pos_blk._mutex);
			uint32_t idx = _pos_blk._block->_size;
			_pos_blk._block->_size++;
			uft::DetailStruct& ds = _pos_blk._block->_details[idx];
			wt_strcpy(ds._code, cInfo->getCode());
			wt_strcpy(ds._exchg, cInfo->getExchg());
			ds._direct = 0;
			ds._open_price = price;
			ds._open_time = now;
			ds._position_profit = 0;
			ds._open_tdate = _tradingday;
			ds._volume = unhandle;

			ds._position_profit = 0;
			ds._closed_volume = 0;
			ds._closed_profit = 0;

			pItem._details.emplace_back(&ds);
			pItem._opencost += unhandle * volscale*price;
			pItem._volume += unhandle;
		}

		//落地开多成交明细
		{
			SpinLock lock(_trd_blk._mutex);
			uint32_t tidx = _trd_blk._block->_size;
			_trd_blk._block->_size++;
			uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
			wt_strcpy(ts._code, cInfo->getCode());
			wt_strcpy(ts._exchg, cInfo->getExchg());
			ts._direct = 0;
			ts._offset = 0;
			ts._price = price;
			ts._volume = unhandle;
			ts._trading_date = _tradingday;
			ts._trading_time = now;
		}
	}
	else
	{
		double unhandle = vol;

		//卖出的时候，有多头就先平多
		if (decimal::gt(pItem._volume, 0))
		{
			double thisQty = min(pItem._volume, vol);

			pItem._volume -= thisQty;
			unhandle -= thisQty;

			double left = thisQty;
			for (uint32_t idx = pItem._valid_idx; idx < pItem._details.size(); idx++)
			{
				uft::DetailStruct* pDS = pItem._details[idx];
				//只有索引递增，才递进，不递增就不递进
				if (decimal::eq(pDS->_volume, 0.0) && (idx == pItem._valid_idx + 1))
				{
					pItem._valid_idx++;
					continue;
				}

				//只平多头
				if (pDS->_direct != 0)
				{
					continue;
				}

				if (decimal::eq(left, 0))
					break;

				double maxQty = std::min(left, pDS->_volume);

				//生成回合明细
				{
					SpinLock lock(_rnd_blk._mutex);
					uint32_t ridx = _rnd_blk._block->_size;
					_rnd_blk._block->_size++;
					uft::RoundStruct& rs = _rnd_blk._block->_rounds[ridx];
					wt_strcpy(rs._code, cInfo->getCode());
					wt_strcpy(rs._exchg, cInfo->getExchg());

					rs._open_price = pDS->_open_price;
					rs._open_time = pDS->_open_time;
					rs._close_price = price;
					rs._close_time = now;
					rs._direct = 0;
					rs._volume = maxQty;
					rs._profit = (rs._close_price - rs._open_price)*maxQty*volscale;

					pItem._total_profit += rs._profit;
					pDS->_closed_profit += rs._profit;
				}

				//落地平多的成交明细
				{
					SpinLock lock(_trd_blk._mutex);
					uint32_t tidx = _trd_blk._block->_size;
					_trd_blk._block->_size++;
					uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
					wt_strcpy(ts._code, cInfo->getCode());
					wt_strcpy(ts._exchg, cInfo->getExchg());
					ts._direct = 0;
					ts._offset = 1;
					ts._price = price;
					ts._volume = maxQty;
					ts._trading_date = _tradingday;
					ts._trading_time = now;
				}

				pDS->_position_profit *= (1 - maxQty / pDS->_volume);
				pDS->_volume -= maxQty;
				pDS->_closed_volume += maxQty;
				pItem._opencost -= maxQty * volscale*pDS->_open_price;
				left -= maxQty;
			}
		}

		//如果还有剩余的没处理，则开空仓
		if (decimal::gt(unhandle, 0))
		{
			SpinLock lock(_pos_blk._mutex);
			uint32_t idx = _pos_blk._block->_size;
			_pos_blk._block->_size++;
			uft::DetailStruct& ds = _pos_blk._block->_details[idx];
			wt_strcpy(ds._code, cInfo->getCode());
			wt_strcpy(ds._exchg, cInfo->getExchg());
			ds._direct = 1;
			ds._open_price = price;
			ds._open_time = now;
			ds._position_profit = 0;
			ds._open_tdate = _tradingday;
			ds._volume = unhandle;

			ds._position_profit = 0;
			ds._closed_volume = 0;
			ds._closed_profit = 0;

			pItem._details.emplace_back(&ds);
			pItem._opencost += unhandle * volscale*price;
			pItem._volume -= unhandle;
		}

		//生成开空成交明细
		{
			SpinLock lock(_trd_blk._mutex);
			uint32_t tidx = _trd_blk._block->_size;
			_trd_blk._block->_size++;
			uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
			wt_strcpy(ts._code, cInfo->getCode());
			wt_strcpy(ts._exchg, cInfo->getExchg());
			ts._direct = 1;
			ts._offset = 0;
			ts._price = price;
			ts._volume = vol;
			ts._trading_date = _tradingday;
			ts._trading_time = now;
		}
	}

	/*
	if (isLong)
	{
		if (isOpen)
		{
			pItem.l_volume += vol;

			{
				SpinLock lock(_pos_blk._mutex);
				uint32_t idx = _pos_blk._block->_size;
				_pos_blk._block->_size++;
				uft::DetailStruct& ds = _pos_blk._block->_details[idx];
				wt_strcpy(ds._code, cInfo->getCode());
				wt_strcpy(ds._exchg, cInfo->getExchg());
				ds._direct = 0;
				ds._open_price = price;
				ds._open_time = now;
				ds._position_profit = 0;
				ds._open_tdate = _tradingday;
				ds._volume = vol;

				ds._position_profit = 0;
				ds._closed_volume = 0;
				ds._closed_profit = 0;

				pItem._details.emplace_back(&ds);
				pItem.l_opencost += vol * volscale*price;
			}

			{
				SpinLock lock(_trd_blk._mutex);
				uint32_t tidx = _trd_blk._block->_size;
				_trd_blk._block->_size++;
				uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
				wt_strcpy(ts._code, cInfo->getCode());
				wt_strcpy(ts._exchg, cInfo->getExchg());
				ts._direct = 0;
				ts._offset = offset;
				ts._price = price;
				ts._volume = vol;
				ts._trading_date = _tradingday;
				ts._trading_time = now;
			}
		}
		else
		{
			//处理平仓
			pItem.l_volume -= vol;
			
			double left = vol;
			for(uint32_t idx = pItem._valid_idx; idx < pItem._details.size(); idx++)
			{
				uft::DetailStruct* pDS = pItem._details[idx];
				//只有索引递增，才递进，不递增就不递进
				if(decimal::eq(pDS->_volume, 0.0) && (idx == pItem._valid_idx+1))
				{
					pItem._valid_idx++;
					continue;
				}

				if (pDS->_direct != 0)
				{
					continue;
				}

				double maxQty = std::min(left, pDS->_volume);
				{
					SpinLock lock(_rnd_blk._mutex);
					uint32_t ridx = _rnd_blk._block->_size;
					_rnd_blk._block->_size++;
					uft::RoundStruct& rs = _rnd_blk._block->_rounds[ridx];
					wt_strcpy(rs._code, cInfo->getCode());
					wt_strcpy(rs._exchg, cInfo->getExchg());

					rs._open_price = pDS->_open_price;
					rs._open_time = pDS->_open_time;
					rs._close_price = price;
					rs._close_time = now;
					rs._direct = 0;
					rs._volume = maxQty;
					rs._profit = (rs._close_price - rs._open_price)*maxQty*cInfo->getCommInfo()->getVolScale();
					pItem.total_profit += rs._profit;
				}	

				pDS->_volume -= maxQty;
				pItem.l_opencost -= maxQty * volscale*price;
				left -= maxQty;
			}

			{
				SpinLock lock(_trd_blk._mutex);
				uint32_t tidx = _trd_blk._block->_size;
				_trd_blk._block->_size++;
				uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
				wt_strcpy(ts._code, cInfo->getCode());
				wt_strcpy(ts._exchg, cInfo->getExchg());
				ts._direct = 0;
				ts._offset = offset;
				ts._price = price;
				ts._volume = vol;
				ts._trading_date = _tradingday;
				ts._trading_time = now;
			}
		}
	}
	else
	{
		if (isOpen)
		{
			pItem.s_volume += vol;

			{
				SpinLock lock(_pos_blk._mutex);
				uint32_t idx = _pos_blk._block->_size;
				_pos_blk._block->_size++;
				uft::DetailStruct& ds = _pos_blk._block->_details[idx];
				wt_strcpy(ds._code, cInfo->getCode());
				wt_strcpy(ds._exchg, cInfo->getExchg());
				ds._direct = 1;
				ds._open_price = price;
				ds._open_time = TimeUtils::getLocalTimeNow();
				ds._position_profit = 0;
				ds._open_tdate = _tradingday;
				ds._volume = vol;

				ds._position_profit = 0;
				ds._closed_volume = 0;
				ds._closed_profit = 0;

				pItem._details.emplace_back(&ds);
				pItem.s_opencost += vol * volscale*price;
			}

			{
				SpinLock lock(_trd_blk._mutex);
				uint32_t tidx = _trd_blk._block->_size;
				_trd_blk._block->_size++;
				uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
				wt_strcpy(ts._code, cInfo->getCode());
				wt_strcpy(ts._exchg, cInfo->getExchg());
				ts._direct = 1;
				ts._offset = offset;
				ts._price = price;
				ts._volume = vol;
				ts._trading_date = _tradingday;
				ts._trading_time = now;
			}
		}
		else
		{
			pItem.s_volume -= vol;

			double left = vol;
			for (uint32_t idx = pItem._valid_idx; idx < pItem._details.size(); idx++)
			{
				uft::DetailStruct* pDS = pItem._details[idx];
				//只有索引递增，才递进，不递增就不递进
				if (decimal::eq(pDS->_volume, 0.0) && (idx == pItem._valid_idx + 1))
				{
					pItem._valid_idx++;
					continue;
				}

				if (pDS->_direct != 1)
					continue;

				double maxQty = std::min(left, pDS->_volume);
				{
					SpinLock lock(_rnd_blk._mutex);
					uint32_t ridx = _rnd_blk._block->_size;
					_rnd_blk._block->_size++;
					uft::RoundStruct& rs = _rnd_blk._block->_rounds[ridx];
					wt_strcpy(rs._code, cInfo->getCode());
					wt_strcpy(rs._exchg, cInfo->getExchg());

					rs._open_price = pDS->_open_price;
					rs._open_time = pDS->_open_time;
					rs._close_price = price;
					rs._close_time = now;
					rs._direct = 1;
					rs._volume = maxQty;
					rs._profit = -1*(rs._close_price - rs._open_price)*maxQty*cInfo->getCommInfo()->getVolScale();
					pItem.total_profit += rs._profit;
				}

				pDS->_volume -= maxQty;
				left -= maxQty;
				pItem.s_opencost -= maxQty * volscale*price;
			}

			{
				SpinLock lock(_trd_blk._mutex);
				uint32_t tidx = _trd_blk._block->_size;
				_trd_blk._block->_size++;
				uft::TradeStruct& ts = _trd_blk._block->_trades[tidx];
				wt_strcpy(ts._code, cInfo->getCode());
				wt_strcpy(ts._exchg, cInfo->getExchg());
				ts._direct = 1;
				ts._offset = offset;
				ts._price = price;
				ts._volume = vol;
				ts._trading_date = _tradingday;
				ts._trading_time = now;
			}
		}
	}
	*/

	if (_strategy)
		_strategy->on_trade(this, localid, stdCode, isLong, offset, vol, price);
}

void UftStraContext::on_order(uint32_t localid, const char* stdCode, bool isLong, uint32_t offset, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	auto it = _order_ids.find(localid);
	if (it == _order_ids.end())
		return;

	WTSContractInfo* cInfo = _engine->get_contract_info(stdCode);
	uft::OrderStruct*& curOrd = it->second;
	if(curOrd == NULL)
	{
		SpinLock lock(_ord_blk._mutex);
		uint32_t idx = _ord_blk._block->_size;
		_ord_blk._block->_size++;
		uft::OrderStruct& os = _ord_blk._block->_orders[idx];
		wt_strcpy(os._code, cInfo->getCode());
		wt_strcpy(os._exchg, cInfo->getExchg());
		os._direct = 0;
		os._offset = offset;
		os._volume = totalQty;
		os._price = price;
		os._left = leftQty;
		os._oder_time = TimeUtils::getLocalTimeNow();

		if (isCanceled)
			os._state = 2;
		else
			os._state = leftQty == 0 ? 1 : 0;

		curOrd = &os;
	}

	if (_strategy)
		_strategy->on_order(this, localid, stdCode, isLong, offset, totalQty, leftQty, price, isCanceled);
}

void UftStraContext::on_channel_ready(uint32_t tradingday)
{
	if (_tradingday != tradingday)
	{
		_tradingday = tradingday;
		load_local_data();
	}

	if (_strategy)
	{
		for (const auto& v : _positions)
		{
			const char* stdCode = v.first.c_str();
			const PosInfo& pInfo = v.second;
			if (decimal::gt(pInfo._volume, 0))
			{
				_strategy->on_position(this, stdCode, true, pInfo._volume, pInfo._volume, 0, 0);
			}
			else if (decimal::lt(pInfo._volume, 0))
			{
				_strategy->on_position(this, stdCode, false, pInfo._volume, pInfo._volume, 0, 0);
			}
		}

		_strategy->on_channel_ready(this);
	}
}

void UftStraContext::on_channel_lost()
{
	if (_strategy)
		_strategy->on_channel_lost(this);
}

void UftStraContext::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (!is_my_order(localid))
		return;

	if (_strategy)
		_strategy->on_entrust(localid, bSuccess, message);
}

void UftStraContext::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{
	//账户的持仓通知不给策略了
	//if (_strategy)
	//	_strategy->on_position(this, stdCode, isLong, prevol, preavail, newvol, newavail);
}

void UftStraContext::on_session_begin(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_begin(this, uTDate);
}

void UftStraContext::on_session_end(uint32_t uTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, uTDate);
}

void UftStraContext::on_params_updated()
{
	if (_strategy)
		_strategy->on_params_updated();
}

const char* UftStraContext::watch_param(const char* name, const char* val)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

int64_t UftStraContext::watch_param(const char* name, int64_t val)
{
	return *ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

int32_t UftStraContext::watch_param(const char* name, int32_t val)
{
	return *ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

uint64_t UftStraContext::watch_param(const char* name, uint64_t val)
{
	return *ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

uint32_t UftStraContext::watch_param(const char* name, uint32_t val)
{
	return *ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

double UftStraContext::watch_param(const char* name, double val)
{
	return *ShareManager::self().allocate_value(_name.c_str(), name, val, false, true);
}

void UftStraContext::commit_param_watcher()
{
	ShareManager::self().commit_param_watcher(_name.c_str());
}

const char* UftStraContext::read_param(const char* name, const char* defVal /* = "" */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int32_t UftStraContext::read_param(const char* name, int32_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

uint32_t UftStraContext::read_param(const char* name, uint32_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int64_t UftStraContext::read_param(const char* name, int64_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

uint64_t UftStraContext::read_param(const char* name, uint64_t defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

double UftStraContext::read_param(const char* name, double defVal /* = 0 */)
{
	return ShareManager::self().get_value(_name.c_str(), name, defVal);
}

int32_t* UftStraContext::sync_param(const char* name, int32_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite, false);
}

uint32_t* UftStraContext::sync_param(const char* name, uint32_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite, false);
}

int64_t* UftStraContext::sync_param(const char* name, int64_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite, false);
}

uint64_t* UftStraContext::sync_param(const char* name, uint64_t initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite, false);
}

double* UftStraContext::sync_param(const char* name, double initVal /* = 0 */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite, false);
}

const char* UftStraContext::sync_param(const char* name, const char* initVal /* = "" */, bool bForceWrite/* = false*/)
{
	return ShareManager::self().allocate_value(_name.c_str(), name, initVal, bForceWrite);
}

double UftStraContext::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, int32_t iFlag /* = 0 */)
{
	return _trader->getPosition(stdCode, bOnlyValid, iFlag);
}

double UftStraContext::stra_get_local_position(const char* stdCode)
{
	auto it = _positions.find(stdCode);
	if (it == _positions.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	return pInfo._volume;
}

double UftStraContext::stra_get_local_posprofit(const char* stdCode)
{
	auto it = _positions.find(stdCode);
	if (it == _positions.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

double UftStraContext::stra_get_local_closeprofit(const char* stdCode)
{
	auto it = _positions.find(stdCode);
	if (it == _positions.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	return pInfo._total_profit;
}


double UftStraContext::stra_enum_position(const char* stdCode)
{
	return _trader->enumPosition(stdCode);
}

double UftStraContext::stra_get_undone(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

uint32_t UftStraContext::stra_get_infos(const char* stdCode)
{
	return _trader->getInfos(stdCode);
}

double UftStraContext::stra_get_price(const char* stdCode)
{
	return _engine->get_cur_price(stdCode);
}

uint32_t UftStraContext::stra_get_date()
{
	return _engine->get_date();
}

uint32_t UftStraContext::stra_get_time()
{
	return _engine->get_raw_time();
}

uint32_t UftStraContext::stra_get_secs()
{
	return _engine->get_secs();
}

bool UftStraContext::stra_cancel(uint32_t localid)
{
	return _trader->cancel(localid);
}

OrderIDs UftStraContext::stra_cancel_all(const char* stdCode)
{
	//撤单频率检查
	//if (!_trader->checkCancelLimits(stdCode))
	//	return OrderIDs();

	return _trader->cancelAll(stdCode);
}

OrderIDs UftStraContext::stra_buy(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	auto ids = _trader->buy(stdCode, price, qty, flag, false);

	for(uint32_t localid : ids)
	{
		_order_ids[localid] = NULL;
	}
	return std::move(ids);
}

OrderIDs UftStraContext::stra_sell(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	auto ids = _trader->sell(stdCode, price, qty, flag, false);
	for (uint32_t localid : ids)
	{
		_order_ids[localid] = NULL;
	}
	return std::move(ids);
}

uint32_t UftStraContext::stra_enter_long(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	uint32_t localid = _trader->openLong(stdCode, price, qty, flag);
	_order_ids[localid] = NULL;
	return localid;
}

uint32_t UftStraContext::stra_exit_long(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	uint32_t localid = _trader->closeLong(stdCode, price, qty, isToday, flag);
	_order_ids[localid] = NULL;
	return localid;
}

uint32_t UftStraContext::stra_enter_short(const char* stdCode, double price, double qty, int flag /* = 0 */)
{
	uint32_t localid = _trader->openShort(stdCode, price, qty, flag);
	_order_ids[localid] = NULL;
	return localid;
}

uint32_t UftStraContext::stra_exit_short(const char* stdCode, double price, double qty, bool isToday /* = false */, int flag /* = 0 */)
{
	uint32_t localid = _trader->closeShort(stdCode, price, qty, isToday, flag);
	_order_ids[localid] = NULL;
	return localid;
}

WTSCommodityInfo* UftStraContext::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

WTSKlineSlice* UftStraContext::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlineSlice* ret = _engine->get_kline_slice(_context_id, stdCode, basePeriod, count, times);

	if (ret)
		_engine->sub_tick(id(), stdCode);

	return ret;
}

WTSTickSlice* UftStraContext::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ticks = _engine->get_tick_slice(_context_id, stdCode, count);

	if (ticks)
		_engine->sub_tick(id(), stdCode);
	return ticks;
}

WTSOrdDtlSlice* UftStraContext::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	WTSOrdDtlSlice* ret = _engine->get_order_detail_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_detail(id(), stdCode);
	return ret;
}

WTSOrdQueSlice* UftStraContext::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	WTSOrdQueSlice* ret = _engine->get_order_queue_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_queue(id(), stdCode);
	return ret;
}


WTSTransSlice* UftStraContext::stra_get_transaction(const char* stdCode, uint32_t count)
{
	WTSTransSlice* ret = _engine->get_transaction_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_transaction(id(), stdCode);
	return ret;
}


WTSTickData* UftStraContext::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void UftStraContext::stra_sub_ticks(const char* stdCode)
{
	_engine->sub_tick(id(), stdCode);
	log_info("Market Data subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_order_details(const char* stdCode)
{
	_engine->sub_order_detail(id(), stdCode);
	log_info("Order details subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_order_queues(const char* stdCode)
{
	_engine->sub_order_queue(id(), stdCode);
	log_info("Order queues subscribed: {}", stdCode);
}

void UftStraContext::stra_sub_transactions(const char* stdCode)
{
	_engine->sub_transaction(id(), stdCode);
	log_info("Transactions subscribed: {}", stdCode);
}

void UftStraContext::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void UftStraContext::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void UftStraContext::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

void UftStraContext::load_local_data()
{
	if (_tradingday == 0)
		return;

	std::string folder = fmtutil::format("{}{}/", WtHelper::getOutputDir(), _name);
	if (!StdFile::exists(folder.c_str()))
		BoostFile::create_directories(folder.c_str());

	/*
	 *	By Wesley @ 2023.09.08
	 *	这里增加一个逻辑，从yaml文件读取手动生成的持仓
	 */
	std::string mannualfile = folder + "mannual.yaml";
	do
	{
		if (!StdFile::exists(mannualfile.c_str()))
			break;

		WTSLogger::log_dyn("strategy", _name.c_str(), LL_WARN, "{} detected, positions will be overwrited", mannualfile);

		WTSVariant* manual = WTSCfgLoader::load_from_file(mannualfile);
		if (manual == NULL)
		{
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_ERROR, "parsing mannual file {} failed", mannualfile);
			break;
		}

		WTSVariant* ayDetails = manual->get("details");
		if(ayDetails == NULL)
			break;

		//解析成功，开始处理持仓
		//char		_exchg[MAX_EXCHANGE_LENGTH];
		//char		_code[MAX_INSTRUMENT_LENGTH];
		//uint32_t	_direct;	//方向0-多，1-空
		//double		_volume;
		//double		_open_price;

		SpinLock lock(_pos_blk._mutex);
		std::string filename = folder + "position.membin";

		//强制新建
		{
			std::size_t uSize = sizeof(uft::PositionBlock) + sizeof(uft::DetailStruct) * DATA_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file(uSize);
			bf.close_file();
		}

		_pos_blk._file.reset(new BoostMappingFile);
		if (_pos_blk._file->map(filename.c_str()))
		{
			_pos_blk._block = (uft::PositionBlock*)_pos_blk._file->addr();
			strcpy(_pos_blk._block->_blk_flag, uft::BLK_FLAG);
			_pos_blk._block->_date = _tradingday;
			_pos_blk._block->_capacity = DATA_SIZE_STEP;

			for (uint32_t i = 0; i < ayDetails->size(); i++)
			{
				WTSVariant* objDetail = ayDetails->get(i);
				const char* exchg = objDetail->getCString("exchg");
				const char* code = objDetail->getCString("code");
				WTSContractInfo* cInfo = _engine->get_basedata_mgr()->getContract(code, exchg);
				if(cInfo == NULL)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_ERROR, "{}.{} not exist, skip this details", exchg, code);
					continue;
				}

				uft::DetailStruct& ds = _pos_blk._block->_details[i];
				wt_strcpy(ds._exchg, exchg);
				wt_strcpy(ds._code, code);
				ds._direct = objDetail->getUInt32("direct");
				ds._volume = objDetail->getDouble("volume");
				ds._open_price = objDetail->getDouble("openprice");
				ds._open_time = TimeUtils::getLocalTimeNow();
				ds._open_tdate = _tradingday;

				_pos_blk._block->_size++;
			}
		}

		WTSLogger::log_dyn("strategy", _name.c_str(), LL_WARN, "loading mannual file {} done, {} details imported", mannualfile, _pos_blk._block->_size);

		//把mmap释放掉，不影响后面的逻辑
		{
			_pos_blk._file.reset();
			_pos_blk._block = NULL;
		}
	} while (false);

	//不管前面解析的情况如何，文件都重命名
	if (StdFile::exists(mannualfile.c_str()))
		boost::filesystem::rename(boost::filesystem::path(mannualfile), boost::filesystem::path(fmtutil::format("{}.{}", mannualfile, TimeUtils::getYYYYMMDDhhmmss())));

	if(_pos_blk._block == NULL || _pos_blk._block->_date != _tradingday)
	{
		SpinLock lock(_pos_blk._mutex);
		std::string filename = folder + "position.membin";
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "loading local positions from {}", filename);
		bool isNew = false;
		if(!StdFile::exists(filename.c_str()))
		{
			std::size_t uSize = sizeof(uft::PositionBlock) + sizeof(uft::DetailStruct) * DATA_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file(uSize);
			bf.close_file();

			isNew = true;
		}

		_pos_blk._file.reset(new BoostMappingFile);
		if (_pos_blk._file->map(filename.c_str()))
		{
			_pos_blk._block = (uft::PositionBlock*)_pos_blk._file->addr();
			if(isNew)
			{
				strcpy(_pos_blk._block->_blk_flag, uft::BLK_FLAG);
				_pos_blk._block->_date = _tradingday;
				_pos_blk._block->_capacity = DATA_SIZE_STEP;
			}

			//复用原文件的好处就是，mmap文件大小会满足历史出现过的单日最高数据量，以后再扩的概率就很低了
			if(_pos_blk._block->_date != 0 && _pos_blk._block->_date != _tradingday)
			{	
				WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, "Clearing local position of {}", _pos_blk._block->_date);
				//如果日期不同，先读进来未完成的持仓，再清理掉原始数据
				std::vector<uft::DetailStruct> details;
				for(uint32_t i = 0; i < _pos_blk._block->_size; i++)
				{
					uft::DetailStruct& ds = _pos_blk._block->_details[i];
					ds._closed_profit = 0;	//交易日切换以后，平仓盈亏置为0
					if(decimal::eq(ds._volume, 0))
						continue;

					WTSContractInfo* cInfo = _engine->get_basedata_mgr()->getContract(ds._code, ds._exchg);
					if (cInfo == NULL)
						continue;

					details.emplace_back(ds);
				}

				memset(_pos_blk._block->_details, 0, sizeof(uft::DetailStruct)*_pos_blk._block->_size);

				if (!details.empty())
					memcpy(_pos_blk._block->_details, details.data(), sizeof(uft::DetailStruct)*details.size());
				_pos_blk._block->_size = details.size();
				_pos_blk._block->_date = _tradingday;
				
			}

			{
				//把剩余数量不为0的持仓读进来
				for (uint32_t i = 0; i < _pos_blk._block->_size; i++)
				{
					uft::DetailStruct& ds = _pos_blk._block->_details[i];

					WTSContractInfo* cInfo = _engine->get_basedata_mgr()->getContract(ds._code, ds._exchg);
					if (cInfo == NULL)
						continue;

					PosInfo& posInfo = _positions[cInfo->getFullCode()];
					posInfo._total_profit += ds._closed_profit;

					if (decimal::eq(ds._volume, 0))
						continue;

					posInfo._dynprofit += ds._position_profit;
					posInfo._opencost += ds._volume*ds._open_price*cInfo->getCommInfo()->getVolScale();
					posInfo._volume += ds._volume*(ds._direct == 0 ? 1 : -1);

					posInfo._details.emplace_back(&ds);
				}
			}
		}
		else
		{
			_pos_blk._file.reset();
			_pos_blk._block = NULL;
		}
	}

	if (_ord_blk._block == NULL || _ord_blk._block->_date != _tradingday)
	{
		SpinLock lock(_ord_blk._mutex);
		std::string filename = folder + "order.membin";
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "loading local orders from {}", filename);
		bool isNew = false;
		if (!StdFile::exists(filename.c_str()))
		{
			std::size_t uSize = sizeof(uft::OrderBlock) + sizeof(uft::OrderStruct) * DATA_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file(uSize);
			bf.close_file();

			isNew = true;
		}

		_ord_blk._file.reset(new BoostMappingFile);
		if (_ord_blk._file->map(filename.c_str()))
		{
			_ord_blk._block = (uft::OrderBlock*)_ord_blk._file->addr();
			if (isNew)
			{
				strcpy(_ord_blk._block->_blk_flag, uft::BLK_FLAG);
				_ord_blk._block->_date = _tradingday;
				_ord_blk._block->_capacity = DATA_SIZE_STEP;
			}

			//交易日不一致就把数据清掉
			//复用原文件的好处就是，mmap文件大小会满足历史出现过的单日最高数据量，以后再扩的概率就很低了
			if (_ord_blk._block->_date != 0 && _ord_blk._block->_date != _tradingday)
			{
				memset(_ord_blk._block->_orders, 0, sizeof(uft::OrderStruct)*_ord_blk._block->_size);
				_ord_blk._block->_size = 0;
				_ord_blk._block->_date = _tradingday;
			}
			else
			{
				//把未完成单读到内存里来
			}
		}
		else
		{
			_ord_blk._file.reset();
			_ord_blk._block = NULL;
		}
	}

	if (_trd_blk._block == NULL || _trd_blk._block->_date != _tradingday)
	{
		SpinLock lock(_trd_blk._mutex);
		std::string filename = folder + "trade.membin";
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "loading local trades from {}", filename);
		bool isNew = false;
		if (!StdFile::exists(filename.c_str()))
		{
			std::size_t uSize = sizeof(uft::TradeBlock) + sizeof(uft::TradeStruct) * DATA_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file(uSize);
			bf.close_file();

			isNew = true;
		}

		_trd_blk._file.reset(new BoostMappingFile);
		if (_trd_blk._file->map(filename.c_str()))
		{
			_trd_blk._block = (uft::TradeBlock*)_trd_blk._file->addr();
			if (isNew)
			{
				strcpy(_trd_blk._block->_blk_flag, uft::BLK_FLAG);
				_trd_blk._block->_date = _tradingday;
				_trd_blk._block->_capacity = DATA_SIZE_STEP;
			}

			//交易日不一致就把数据清掉
			//复用原文件的好处就是，mmap文件大小会满足历史出现过的单日最高数据量，以后再扩的概率就很低了
			if (_trd_blk._block->_date != 0 && _trd_blk._block->_date != _tradingday)
			{
				memset(_trd_blk._block->_trades, 0, sizeof(uft::TradeStruct)*_trd_blk._block->_size);
				_trd_blk._block->_size = 0;
				_trd_blk._block->_date = _tradingday;
			}
			else
			{
				//成交数据不用读进来了
			}			
		}
		else
		{
			_trd_blk._file.reset();
			_trd_blk._block = NULL;
		}
	}

	if (_rnd_blk._block == NULL || _rnd_blk._block->_date != _tradingday)
	{
		SpinLock lock(_rnd_blk._mutex);
		std::string filename = folder + "round.membin";
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "loading local rouds from {}", filename);
		bool isNew = false;
		if (!StdFile::exists(filename.c_str()))
		{
			std::size_t uSize = sizeof(uft::RoundBlock) + sizeof(uft::RoundStruct) * DATA_SIZE_STEP;
			BoostFile bf;
			bf.create_new_file(filename.c_str());
			bf.truncate_file(uSize);
			bf.close_file();

			isNew = true;
		}

		_rnd_blk._file.reset(new BoostMappingFile);
		if (_rnd_blk._file->map(filename.c_str()))
		{
			_rnd_blk._block = (uft::RoundBlock*)_rnd_blk._file->addr();
			if (isNew)
			{
				strcpy(_rnd_blk._block->_blk_flag, uft::BLK_FLAG);
				_rnd_blk._block->_date = _tradingday;
				_rnd_blk._block->_capacity = DATA_SIZE_STEP;
			}

			//交易日不一致就把数据清掉
			//复用原文件的好处就是，mmap文件大小会满足历史出现过的单日最高数据量，以后再扩的概率就很低了
			if (_rnd_blk._block->_date != 0 && _rnd_blk._block->_date != _tradingday)
			{
				memset(_rnd_blk._block->_rounds, 0, sizeof(uft::RoundStruct)*_rnd_blk._block->_size);
				_rnd_blk._block->_size = 0;
				_rnd_blk._block->_date = _tradingday;
			}
			else
			{
				//回合数据不用读到进来了
			}
		}
		else
		{
			_rnd_blk._file.reset();
			_rnd_blk._block = NULL;
		}
	}
}