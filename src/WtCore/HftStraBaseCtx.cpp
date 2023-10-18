/*!
 * \file HftStraBaseCtx.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 *
 * \brief
 */
#include "HftStraBaseCtx.h"
#include "WtHftEngine.h"
#include "TraderAdapter.h"
#include "WtHelper.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSHotMgr.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_WTP;

inline uint32_t makeHftCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 6000 };
	return _auto_context_id.fetch_add(1);
}

HftStraBaseCtx::HftStraBaseCtx(WtHftEngine* engine, const char* name, bool bAgent, int32_t slippage)
	: IHftStraCtx(name)
	, _engine(engine)
	, _data_agent(bAgent)
	, _slippage(slippage)
{
	_context_id = makeHftCtxId();
}


HftStraBaseCtx::~HftStraBaseCtx()
{
}

uint32_t HftStraBaseCtx::id()
{
	return _context_id;
}

void HftStraBaseCtx::setTrader(TraderAdapter* trader)
{
	_trader = trader;
}

void HftStraBaseCtx::init_outputs()
{
	if (!_data_agent)
		return;

	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "//";
	BoostFile::create_directories(folder.c_str());

	std::string filename = folder + "trades.csv";
	_trade_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_trade_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_trade_logs->write_file("code,time,direct,action,price,qty,tag,fee\n");
		}
		else
		{
			_trade_logs->seek_to_end();
		}
	}

	filename = folder + "closes.csv";
	_close_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_close_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_close_logs->write_file("code,direct,opentime,openprice,closetime,closeprice,qty,profit,totalprofit,entertag,exittag\n");
		}
		else
		{
			_close_logs->seek_to_end();
		}
	}

	filename = folder + "funds.csv";
	_fund_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_fund_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_fund_logs->write_file("date,closeprofit,positionprofit,dynbalance,fee\n");
		}
		else
		{
			_fund_logs->seek_to_end();
		}
	}

	filename = folder + "signals.csv";
	_sig_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_sig_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_sig_logs->write_file("code,target,sigprice,gentime,usertag\n");
		}
		else
		{
			_sig_logs->seek_to_end();
		}
	}
}

void HftStraBaseCtx::on_init()
{
	init_outputs();

	load_userdata();
}

void HftStraBaseCtx::on_tick(const char* stdCode, WTSTickData* newTick)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_order_queue(const char* stdCode, WTSOrdQueData* newOrdQue)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_order_detail(const char* stdCode, WTSOrdDtlData* newOrdDtl)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_transaction(const char* stdCode, WTSTransData* newTrans)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

bool HftStraBaseCtx::stra_cancel(uint32_t localid)
{
	return _trader->cancel(localid);
}

OrderIDs HftStraBaseCtx::stra_cancel(const char* stdCode, bool isBuy, double qty)
{
	//撤单频率检查
	if (!_trader->checkCancelLimits(stdCode))
		return OrderIDs();

	return _trader->cancel(stdCode, isBuy, qty);
}

const char* HftStraBaseCtx::get_inner_code(const char* stdCode)
{
	auto it = _code_map.find(stdCode);
	if (it == _code_map.end())
		return stdCode;

	return it->second.c_str();
}

OrderIDs HftStraBaseCtx::stra_buy(const char* stdCode, double price, double qty, const char* userTag, int flag /* = 0 */, bool bForceClose /* = false */)
{
	/*
	 *	By Wesley @ 2022.05.26
	 *	如果找到匹配自定义规则，则进行映射处理
	 */
	 //const char* ruleTag = _engine->get_hot_mgr()->getRuleTag(stdCode);
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		WTSContractInfo* ct = _engine->get_basedata_mgr()->getContract(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		if (_trader && !_trader->checkOrderLimits(realCode.c_str()))
		{
			log_info("{} is forbidden to trade", realCode.c_str());
			return OrderIDs();
		}

		auto ids = _trader->buy(realCode.c_str(), price, qty, flag, bForceClose, ct);
		for (auto localid : ids)
			setUserTag(localid, userTag);

		return ids;
	}
	else
	{
		WTSContractInfo* ct = _engine->get_basedata_mgr()->getContract(cInfo._code, cInfo._exchg);
		if (ct == NULL)
		{
			log_error("Cannot find corresponding contract info of {}", stdCode);
			return OrderIDs();
		}

		if (!_trader->checkOrderLimits(stdCode))
		{
			log_info("{} is forbidden to trade", stdCode);
			return OrderIDs();
		}

		auto ids = _trader->buy(stdCode, price, qty, flag, bForceClose, ct);
		for (auto localid : ids)
			setUserTag(localid, userTag);
		return ids;
	}
}

OrderIDs HftStraBaseCtx::stra_sell(const char* stdCode, double price, double qty, const char* userTag, int flag /* = 0 */, bool bForceClose /* = false */)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	WTSCommodityInfo* commInfo = _engine->get_basedata_mgr()->getCommodity(cInfo._exchg, cInfo._product);

	//如果不能做空，则要看可用持仓
	if (!commInfo->canShort())
	{
		double curPos = stra_get_position(stdCode, true);//只读可用持仓
		if (decimal::gt(qty, curPos))
		{
			log_error("No enough position of {} to sell", stdCode);
			return OrderIDs();
		}
	}

	/*
	 *	By Wesley @ 2022.05.26
	 *	如果找到匹配自定义规则，则进行映射处理
	 */
	
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		WTSContractInfo* ct = _engine->get_basedata_mgr()->getContract(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		if (_trader && !_trader->checkOrderLimits(realCode.c_str()))
		{
			log_info("{} is forbidden to trade", realCode.c_str());
			return OrderIDs();
		}

		auto ids = _trader->sell(realCode.c_str(), price, qty, flag, bForceClose, ct);
		for (auto localid : ids)
			setUserTag(localid, userTag);
		return ids;
	}
	else
	{
		WTSContractInfo* ct = _engine->get_basedata_mgr()->getContract(cInfo._code, cInfo._exchg);
		if (ct == NULL)
		{
			log_error("Cannot find corresponding contract info of {}", stdCode);
			return OrderIDs();
		}

		if (_trader && !_trader->checkOrderLimits(stdCode))
		{
			log_info("{} is forbidden to trade", stdCode);
			return OrderIDs();
		}

		auto ids = _trader->sell(stdCode, price, qty, flag, bForceClose, ct);
		for (auto localid : ids)
			setUserTag(localid, userTag);
		return ids;
	}
}

uint32_t HftStraBaseCtx::stra_enter_long(const char* stdCode, double price, double qty, const char* userTag, int flag/* = 0*/)
{
	std::string realCode = stdCode;
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
		_code_map[realCode] = stdCode;
	}

	return _trader->openLong(realCode.c_str(), price, qty, flag);
}

uint32_t HftStraBaseCtx::stra_exit_long(const char* stdCode, double price, double qty, const char* userTag, bool isToday/* = false*/, int flag/* = 0*/)
{
	std::string realCode = stdCode;
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;
	}

	return _trader->closeLong(realCode.c_str(), price, qty, isToday, flag);
}

uint32_t HftStraBaseCtx::stra_enter_short(const char* stdCode, double price, double qty, const char* userTag, int flag/* = 0*/)
{
	std::string realCode = stdCode;
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;
	}
	//else if (CodeHelper::isStdFutHotCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;
	//}
	//else if (CodeHelper::isStdFut2ndCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getSecondRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;
	//}

	return _trader->openShort(realCode.c_str(), price, qty, flag);
}

uint32_t HftStraBaseCtx::stra_exit_short(const char* stdCode, double price, double qty, const char* userTag, bool isToday/* = false*/, int flag/* = 0*/)
{
	std::string realCode = stdCode;
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;
	}
	//else if (CodeHelper::isStdFutHotCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;
	//}
	//else if (CodeHelper::isStdFut2ndCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getSecondRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;
	//}

	return _trader->closeShort(realCode.c_str(), price, qty, isToday, flag);
}

WTSCommodityInfo* HftStraBaseCtx::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

std::string HftStraBaseCtx::stra_get_rawcode(const char* stdCode)
{
	return _engine->get_rawcode(stdCode);
}

WTSKlineSlice* HftStraBaseCtx::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
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

WTSTickSlice* HftStraBaseCtx::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ticks = _engine->get_tick_slice(_context_id, stdCode, count);

	if (ticks)
		_engine->sub_tick(id(), stdCode);
	return ticks;
}

WTSOrdDtlSlice* HftStraBaseCtx::stra_get_order_detail(const char* stdCode, uint32_t count)
{
	WTSOrdDtlSlice* ret = _engine->get_order_detail_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_detail(id(), stdCode);
	return ret;
}

WTSOrdQueSlice* HftStraBaseCtx::stra_get_order_queue(const char* stdCode, uint32_t count)
{
	WTSOrdQueSlice* ret = _engine->get_order_queue_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_order_queue(id(), stdCode);
	return ret;
}


WTSTransSlice* HftStraBaseCtx::stra_get_transaction(const char* stdCode, uint32_t count)
{
	WTSTransSlice* ret = _engine->get_transaction_slice(_context_id, stdCode, count);

	if (ret)
		_engine->sub_transaction(id(), stdCode);
	return ret;
}


WTSTickData* HftStraBaseCtx::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void HftStraBaseCtx::stra_sub_ticks(const char* stdCode)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	主动订阅tick会在本地记一下
	 *	tick数据回调的时候先检查一下
	 */
	_tick_subs.insert(stdCode);

	_engine->sub_tick(id(), stdCode);
	log_info("Market Data subscribed: {}", stdCode);
}

void HftStraBaseCtx::stra_sub_order_details(const char* stdCode)
{
	_engine->sub_order_detail(id(), stdCode);
	log_info("Order details subscribed: {}", stdCode);
}

void HftStraBaseCtx::stra_sub_order_queues(const char* stdCode)
{
	_engine->sub_order_queue(id(), stdCode);
	log_info("Order queues subscribed: {}", stdCode);
}

void HftStraBaseCtx::stra_sub_transactions(const char* stdCode)
{
	_engine->sub_transaction(id(), stdCode);
	log_info("Transactions subscribed: {}", stdCode);
}

void HftStraBaseCtx::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void HftStraBaseCtx::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void HftStraBaseCtx::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void HftStraBaseCtx::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

void HftStraBaseCtx::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}

	if(_sig_logs && _data_agent)
	{
		double curPos = stra_get_position(stdCode);
		_sig_logs->write_file(fmt::format("{}.{}.{},{}{},{},{}\n", stra_get_date(), stra_get_time(), stra_get_secs(), isBuy ? "+" : "-", vol, curPos, price));
	}

	const PosInfo& posInfo = _pos_map[stdCode];
	double curPos = posInfo._volume + vol * (isBuy ? 1 : -1);
	do_set_position(stdCode, curPos, price, getOrderTag(localid));
}

void HftStraBaseCtx::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}

	if(isCanceled || decimal::eq(leftQty, 0))
	{
		//订单结束了，要把订单号清理掉，不然开销太大
	}
}

void HftStraBaseCtx::on_channel_ready()
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_channel_lost()
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{

}

double HftStraBaseCtx::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0.0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

double HftStraBaseCtx::stra_get_position_avgpx(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._volume == 0)
		return 0.0;

	double amount = 0.0;
	for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
	{
		const DetailInfo& dInfo = *dit;
		amount += dInfo._price*dInfo._volume;
	}

	return amount / pInfo._volume;
}

double HftStraBaseCtx::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, int flag /* = 3*/)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->getPosition(realCode.c_str(), bOnlyValid, flag);
	}
	else
	{
		return _trader->getPosition(stdCode, bOnlyValid, flag);
	}
}

double HftStraBaseCtx::stra_get_undone(const char* stdCode)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _engine->get_hot_mgr());
	if (strlen(cInfo._ruletag) > 0)
	{
		std::string code = _engine->get_hot_mgr()->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID(), _engine->get_trading_date());
		std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->getUndoneQty(realCode.c_str());
	}
	//else if (CodeHelper::isStdFutHotCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;

	//	return _trader->getUndoneQty(realCode.c_str());
	//}
	//else if (CodeHelper::isStdFut2ndCode(stdCode))
	//{
	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string code = _engine->get_hot_mgr()->getSecondRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
	//	std::string realCode = CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);

	//	_code_map[realCode] = stdCode;

	//	return _trader->getUndoneQty(realCode.c_str());
	//}
	else
	{
		return _trader->getUndoneQty(stdCode);
	}
}

double HftStraBaseCtx::stra_get_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if (it != _price_map.end())
		return it->second;

	return _engine->get_cur_price(stdCode);
}

uint32_t HftStraBaseCtx::stra_get_date()
{
	return _engine->get_date();
}

uint32_t HftStraBaseCtx::stra_get_time()
{
	return _engine->get_raw_time();
}

uint32_t HftStraBaseCtx::stra_get_secs()
{
	return _engine->get_secs();
}

const char* HftStraBaseCtx::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void HftStraBaseCtx::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

void HftStraBaseCtx::save_userdata()
{
	//ini.save(filename.c_str());
	rj::Document root(rj::kObjectType);
	rj::Document::AllocatorType &allocator = root.GetAllocator();
	for (auto it = _user_datas.begin(); it != _user_datas.end(); it++)
	{
		root.AddMember(rj::Value(it->first.c_str(), allocator), rj::Value(it->second.c_str(), allocator), allocator);
	}

	{
		std::string filename = WtHelper::getStraUsrDatDir();
		filename += "ud_";
		filename += _name;
		filename += ".json";

		BoostFile bf;
		if (bf.create_new_file(filename.c_str()))
		{
			rj::StringBuffer sb;
			rj::PrettyWriter<rj::StringBuffer> writer(sb);
			root.Accept(writer);
			bf.write_file(sb.GetString());
			bf.close_file();
		}
	}
}

void HftStraBaseCtx::load_userdata()
{
	std::string filename = WtHelper::getStraUsrDatDir();
	filename += "ud_";
	filename += _name;
	filename += ".json";

	if (!StdFile::exists(filename.c_str()))
	{
		return;
	}

	std::string content;
	StdFile::read_file_content(filename.c_str(), content);
	if (content.empty())
		return;

	rj::Document root;
	root.Parse(content.c_str());

	if (root.HasParseError())
		return;

	for (auto& m : root.GetObject())
	{
		const char* key = m.name.GetString();
		const char* val = m.value.GetString();
		_user_datas[key] = val;
	}
}

void HftStraBaseCtx::do_set_position(const char* stdCode, double qty, double price /* = 0.0 */, const char* userTag /*= ""*/)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
	uint32_t curTDate = _engine->get_trading_date();

	//手数相等则不用操作了
	if (decimal::eq(pInfo._volume, qty))
		return;

	log_info("Target position updated: {} -> {}", pInfo._volume, qty);

	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);

	//成交价
	double trdPx = curPx;

	double diff = qty - pInfo._volume;
	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))//当前持仓和仓位变化方向一致, 增加一条明细, 增加数量即可
	{
		pInfo._volume = qty;

		if (_slippage != 0)
		{
			trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);
		}

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = trdPx;
		dInfo._volume = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		wt_strcpy(dInfo._usertag, userTag);
		pInfo._details.emplace_back(dInfo);

		double fee = commInfo->calcFee(trdPx, abs(diff), 0);
		_fund_info._total_fees += fee;

		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(diff), fee, userTag);
	}
	else
	{//持仓方向和仓位变化方向不一致,需要平仓
		double left = abs(diff);

		if (_slippage != 0)
			trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);

		pInfo._volume = qty;
		if (decimal::eq(pInfo._volume, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			double maxQty = min(dInfo._volume, left);
			if (decimal::eq(maxQty, 0))
				continue;

			double maxProf = dInfo._max_profit * maxQty / dInfo._volume;
			double maxLoss = dInfo._max_loss * maxQty / dInfo._volume;

			dInfo._volume -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//浮盈也要做等比缩放
			_fund_info._total_profit += profit;

			double fee = commInfo->calcFee(trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, fee, userTag);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, pInfo._closeprofit, dInfo._usertag, userTag);

			if (left == 0)
				break;
		}

		//需要清理掉已经平仓完的明细
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//最后,如果还有剩余的,则需要反手了
		if (left > 0)
		{
			left = left * qty / abs(qty);

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = trdPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			wt_strcpy(dInfo._usertag, userTag);
			pInfo._details.emplace_back(dInfo);

			//这里还需要写一笔成交记录
			double fee = commInfo->calcFee(trdPx, abs(left), 0);
			_fund_info._total_fees += fee;
			//_engine->mutate_fund(fee, FFT_Fee);
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), fee, userTag);
		}
	}
}

void HftStraBaseCtx::update_dyn_profit(const char* stdCode, WTSTickData* newTick)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		PosInfo& pInfo = (PosInfo&)it->second;
		if (pInfo._volume == 0)
		{
			pInfo._dynprofit = 0;
		}
		else
		{
			bool isLong = decimal::gt(pInfo._volume, 0);
			double price = isLong ? newTick->bidprice(0) : newTick->askprice(0);

			WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{

				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = std::max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = std::min(dInfo._profit, dInfo._max_loss);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}
}

void HftStraBaseCtx::on_session_begin(uint32_t uTDate)
{

}

void HftStraBaseCtx::on_session_end(uint32_t uTDate)
{
	uint32_t curDate = uTDate;//_engine->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;
	}

	//这里要把当日结算的数据写到日志文件里
	//而且这里回测和实盘写法不同, 先留着, 后面来做
	if (_fund_logs && _data_agent)
		_fund_logs->write_file(fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curDate,
			_fund_info._total_profit, _fund_info._total_dynprofit,
			_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees));
}

void HftStraBaseCtx::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee, const char* userTag/* = ""*/)
{
	if(_trade_logs && _data_agent)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE")
			<< "," << price << "," << qty << "," << fee << "," << userTag << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void HftStraBaseCtx::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double maxprofit, double maxloss,
	double totalprofit /* = 0 */, const char* enterTag/* = ""*/, const char* exitTag/* = ""*/)
{
	if (_close_logs && _data_agent)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," << maxprofit << "," << maxloss << ","
			<< totalprofit << "," << enterTag << "," << exitTag << "\n";
		_close_logs->write_file(ss.str());
	}
}
