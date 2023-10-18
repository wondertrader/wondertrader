/*!
* \file MfStraBaseCtx.cpp
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#include "SelStraBaseCtx.h"
#include "WtSelEngine.h"
#include "WtHelper.h"

#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IHotMgr.h"
#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"

#include "../WTSTools/WTSLogger.h"

namespace rj = rapidjson;

inline uint32_t makeSelCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 3000 };
	return _auto_context_id.fetch_add(1);
}


SelStraBaseCtx::SelStraBaseCtx(WtSelEngine* engine, const char* name, int32_t slippage)
	: ISelStraCtx(name)
	, _engine(engine)
	, _total_calc_time(0)
	, _emit_times(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
	, _schedule_date(0)
	, _schedule_time(0)
	, _slippage(slippage)
{
	_context_id = makeSelCtxId();
}


SelStraBaseCtx::~SelStraBaseCtx()
{
}

void SelStraBaseCtx::init_outputs()
{
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

	filename = folder + "positions.csv";
	_pos_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_pos_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_pos_logs->write_file("date,code,volume,closeprofit,dynprofit\n");
		}
		else
		{
			_pos_logs->seek_to_end();
		}
	}
}

void SelStraBaseCtx::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	if (_sig_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
		_sig_logs->write_file(ss.str());
	}
}

void SelStraBaseCtx::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag, double fee)
{
	if (_trade_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") << "," << price << "," << qty << "," << userTag << "," << fee << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void SelStraBaseCtx::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
	double profit, double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */)
{
	if (_close_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << ","
			<< totalprofit << "," << enterTag << "," << exitTag << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void SelStraBaseCtx::save_userdata()
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

void SelStraBaseCtx::load_userdata()
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

void SelStraBaseCtx::load_data(uint32_t flag /* = 0xFFFFFFFF */)
{
	std::string filename = WtHelper::getStraDataDir();
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

	if (root.HasMember("fund"))
	{
		//读取资金
		const rj::Value& jFund = root["fund"];
		if (!jFund.IsNull() && jFund.IsObject())
		{
			_fund_info._total_profit = jFund["total_profit"].GetDouble();
			_fund_info._total_dynprofit = jFund["total_dynprofit"].GetDouble();
			uint32_t tdate = jFund["tdate"].GetUint();
			if (tdate == _engine->get_trading_date())
				_fund_info._total_fees = jFund["total_fees"].GetDouble();
		}
	}

	{//读取仓位
		double total_profit = 0;
		double total_dynprofit = 0;
		const rj::Value& jPos = root["positions"];
		if (!jPos.IsNull() && jPos.IsArray())
		{
			for (const rj::Value& pItem : jPos.GetArray())
			{
				const char* stdCode = pItem["code"].GetString();
				const char* ruleTag = _engine->get_hot_mgr()->getRuleTag(stdCode);
				bool isExpired = (strlen(ruleTag) == 0 && _engine->get_contract_info(stdCode) == NULL);

				if (isExpired)
					log_info("{} not exists or expired, position ignored", stdCode);

				PosInfo& pInfo = _pos_map[stdCode];
				pInfo._closeprofit = pItem["closeprofit"].GetDouble();
				pInfo._last_entertime = pItem["lastentertime"].GetUint64();
				pInfo._last_exittime = pItem["lastexittime"].GetUint64();
				pInfo._volume = isExpired ? 0 : pItem["volume"].GetDouble();
				if (pItem.HasMember("frozen") && !isExpired)
				{
					pInfo._frozen = pItem["frozen"].GetDouble();
					pInfo._frozen_date = pItem["frozendate"].GetUint();
				}

				if (pInfo._volume == 0 || isExpired)
				{
					pInfo._dynprofit = 0;
					pInfo._frozen = 0;
				}
				else
					pInfo._dynprofit = pItem["dynprofit"].GetDouble();				

				total_profit += pInfo._closeprofit;
				total_dynprofit += pInfo._dynprofit;

				const rj::Value& details = pItem["details"];
				if (details.IsNull() || !details.IsArray() || details.Size() == 0 || isExpired)
					continue;

				pInfo._details.resize(details.Size());

				for (uint32_t i = 0; i < details.Size(); i++)
				{
					const rj::Value& dItem = details[i];
					DetailInfo& dInfo = pInfo._details[i];
					dInfo._long = dItem["long"].GetBool();
					dInfo._price = dItem["price"].GetDouble();
					dInfo._volume = dItem["volume"].GetDouble();
					dInfo._opentime = dItem["opentime"].GetUint64();
					if (dItem.HasMember("opentdate"))
						dInfo._opentdate = dItem["opentdate"].GetUint();

					if (dItem.HasMember("maxprice"))
						dInfo._max_price = dItem["maxprice"].GetDouble();
					else
						dInfo._max_price = dInfo._price;

					if (dItem.HasMember("minprice"))
						dInfo._min_price = dItem["minprice"].GetDouble();
					else
						dInfo._min_price = dInfo._price;

					dInfo._profit = dItem["profit"].GetDouble();
					dInfo._max_profit = dItem["maxprofit"].GetDouble();
					dInfo._max_loss = dItem["maxloss"].GetDouble();

					strcpy(dInfo._opentag, dItem["opentag"].GetString());
				}

				if (!isExpired)
				{
					log_info("Position confirmed,{} -> {}", stdCode, pInfo._volume);
					stra_sub_ticks(stdCode);
				}
			}
		}

		_fund_info._total_profit = total_profit;
		_fund_info._total_dynprofit = total_dynprofit;
	}

	if (root.HasMember("signals"))
	{
		//读取信号
		const rj::Value& jSignals = root["signals"];
		if (!jSignals.IsNull() && jSignals.IsObject())
		{
			for (auto& m : jSignals.GetObject())
			{
				const char* stdCode = m.name.GetString();
				const char* ruleTag = _engine->get_hot_mgr()->getRuleTag(stdCode);
				if (strlen(ruleTag) == 0 && _engine->get_contract_info(stdCode) == NULL)
				{
					log_info("{} not exists or expired, signal ignored", stdCode);
					continue;
				}

				const rj::Value& jItem = m.value;

				SigInfo& sInfo = _sig_map[stdCode];
				sInfo._usertag = jItem["usertag"].GetString();
				sInfo._volume = jItem["volume"].GetDouble();
				sInfo._sigprice = jItem["sigprice"].GetDouble();
				sInfo._gentime = jItem["gentime"].GetUint64();

				log_info("{} untouched signal recovered, target pos: {}", stdCode, sInfo._volume);
				stra_sub_ticks(stdCode);
			}
		}
	}
}

void SelStraBaseCtx::save_data(uint32_t flag /* = 0xFFFFFFFF */)
{
	rj::Document root(rj::kObjectType);

	{//持仓数据保存
		rj::Value jPos(rj::kArrayType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosInfo& pInfo = it->second;

			rj::Value pItem(rj::kObjectType);
			pItem.AddMember("code", rj::Value(stdCode, allocator), allocator);
			pItem.AddMember("volume", pInfo._volume, allocator);
			pItem.AddMember("closeprofit", pInfo._closeprofit, allocator);
			pItem.AddMember("dynprofit", pInfo._dynprofit, allocator);
			pItem.AddMember("lastentertime", pInfo._last_entertime, allocator);
			pItem.AddMember("lastexittime", pInfo._last_exittime, allocator);
			pItem.AddMember("frozen", pInfo._frozen, allocator);
			pItem.AddMember("frozendate", pInfo._frozen_date, allocator);

			rj::Value details(rj::kArrayType);
			for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
			{
				const DetailInfo& dInfo = *dit;
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("long", dInfo._long, allocator);
				dItem.AddMember("price", dInfo._price, allocator);
				dItem.AddMember("maxprice", dInfo._max_price, allocator);
				dItem.AddMember("minprice", dInfo._min_price, allocator);
				dItem.AddMember("volume", dInfo._volume, allocator);
				dItem.AddMember("opentime", dInfo._opentime, allocator);
				dItem.AddMember("opentdate", dInfo._opentdate, allocator);

				dItem.AddMember("profit", dInfo._profit, allocator);
				dItem.AddMember("maxprofit", dInfo._max_profit, allocator);
				dItem.AddMember("maxloss", dInfo._max_loss, allocator);
				dItem.AddMember("opentag", rj::Value(dInfo._opentag, allocator), allocator);

				details.PushBack(dItem, allocator);
			}

			pItem.AddMember("details", details, allocator);

			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	{//资金保存
		rj::Value jFund(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		jFund.AddMember("total_profit", _fund_info._total_profit, allocator);
		jFund.AddMember("total_dynprofit", _fund_info._total_dynprofit, allocator);
		jFund.AddMember("total_fees", _fund_info._total_fees, allocator);
		jFund.AddMember("tdate", _engine->get_trading_date(), allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//信号保存
		rj::Value jSigs(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it : _sig_map)
		{
			const char* stdCode = it.first.c_str();
			const SigInfo& sInfo = it.second;

			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("usertag", rj::Value(sInfo._usertag.c_str(), allocator), allocator);

			jItem.AddMember("volume", sInfo._volume, allocator);
			jItem.AddMember("sigprice", sInfo._sigprice, allocator);
			jItem.AddMember("gentime", sInfo._gentime, allocator);

			jSigs.AddMember(rj::Value(stdCode, allocator), jItem, allocator);
		}

		root.AddMember("signals", jSigs, allocator);
	}

	{
		std::string filename = WtHelper::getStraDataDir();
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

//////////////////////////////////////////////////////////////////////////
//回调函数
void SelStraBaseCtx::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	thread_local static char realPeriod[8] = { 0 };
	fmtutil::format_to(realPeriod, "{}{}", period, times);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, realPeriod);

	KlineTag& tag = _kline_tags[key];
	tag._closed = true;

	on_bar_close(stdCode, realPeriod, newBar);
}

void SelStraBaseCtx::on_init()
{
	init_outputs();

	//读取数据
	load_data();

	load_userdata();
}

void SelStraBaseCtx::update_dyn_profit(const char* stdCode, double price)
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

				dInfo._max_price = std::max(dInfo._max_price, price);
				dInfo._min_price = std::min(dInfo._min_price, price);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}

	double total_dynprofit = 0;
	for (auto v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		total_dynprofit += pInfo._dynprofit;
	}

	_fund_info._total_dynprofit = total_dynprofit;
}

void SelStraBaseCtx::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	_price_map[stdCode] = newTick->price();

	//先检查是否要信号要触发
	{
		auto it = _sig_map.find(stdCode);
		if (it != _sig_map.end())
		{
			WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

			if (sInfo->isInTradingTime(_engine->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				do_set_position(stdCode, sInfo._volume, sInfo._usertag.c_str(), sInfo._triggered);
				_sig_map.erase(it);
			}

		}
	}

	update_dyn_profit(stdCode, newTick->price());

	if (bEmitStrategy)
		on_tick_updated(stdCode, newTick);

	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

bool SelStraBaseCtx::on_schedule(uint32_t curDate, uint32_t curTime, uint32_t fireTime)
{
	_schedule_date = curDate;
	_schedule_time = curTime;

	_is_in_schedule = true;//开始调度, 修改标记	

	//主要用于保存浮动盈亏的
	save_data();

	TimeUtils::Ticker ticker;
	on_strategy_schedule(curDate, fireTime);
	log_debug("Strategy {} scheduled @ {}", _context_id, curTime);

	wt_hashset<std::string> to_clear;
	for (auto& v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		const char* code = v.first.c_str();
		if (_sig_map.find(code) == _sig_map.end() && !decimal::eq(pInfo._volume, 0.0))
		{
			//新的信号中没有该持仓,则要清空
			to_clear.insert(code);
		}
	}

	for (const std::string& code : to_clear)
	{
		append_signal(code.c_str(), 0, "autoexit");
	}

	_emit_times++;
	_total_calc_time += ticker.micro_seconds();

	if (_emit_times % 20 == 0)
		log_info("Strategy has been scheduled {} times, totally taking {} us, {:.3f} us each time",
			_emit_times, _total_calc_time, _total_calc_time*1.0 / _emit_times);

	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}

	_is_in_schedule = false;//调度结束, 修改标记
	return true;
}

void SelStraBaseCtx::on_session_begin(uint32_t uTDate)
{
	//每个交易日开始，要把冻结持仓置零
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosInfo& pInfo = (PosInfo&)it.second;
		if (pInfo._frozen_date != 0 && pInfo._frozen_date < uTDate && !decimal::eq(pInfo._frozen, 0))
		{
			log_debug("{} of {} frozen on {} released on {}", pInfo._frozen, stdCode, pInfo._frozen_date, uTDate);

			pInfo._frozen = 0;
			pInfo._frozen_date = 0;
		}
	}

	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void SelStraBaseCtx::enum_position(FuncEnumSelPositionCallBack cb)
{
	wt_hashmap<std::string, double> desPos;
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		const PosInfo& pInfo = it.second;
		desPos[stdCode] = pInfo._volume;
	}

	for (auto sit : _sig_map)
	{
		const char* stdCode = sit.first.c_str();
		const SigInfo& sInfo = sit.second;
		desPos[stdCode] = sInfo._volume;
	}

	for (auto v : desPos)
	{
		cb(v.first.c_str(), v.second);
	}
}

void SelStraBaseCtx::on_session_end(uint32_t uTDate)
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

		if (decimal::eq(pInfo._volume, 0.0))
			continue;

		if (_pos_logs)
			_pos_logs->write_file(fmt::format("{},{},{},{:.2f},{:.2f}\n", curDate, stdCode,
				pInfo._volume, pInfo._closeprofit, pInfo._dynprofit));
	}

	if (_fund_logs)
		_fund_logs->write_file(fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees));

	save_data();

	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}


//////////////////////////////////////////////////////////////////////////
//策略接口
#pragma region "策略接口"
double SelStraBaseCtx::stra_get_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if (it != _price_map.end())
		return it->second;

	if (_engine)
		return _engine->get_cur_price(stdCode);

	return 0.0;
}

void SelStraBaseCtx::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */)
{
	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	//如果不能做空，则目标仓位不能设置负数
	if (!commInfo->canShort() && decimal::lt(qty, 0))
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	double total = stra_get_position(stdCode, false);
	//如果目标仓位和当前仓位是一致的，直接退出
	if (decimal::eq(total, qty))
		return;

	if (commInfo->isT1())
	{
		double valid = stra_get_position(stdCode, true);
		double frozen = total - valid;
		//如果是T+1规则，则目标仓位不能小于冻结仓位
		if (decimal::lt(qty, frozen))
		{
			log_error("New position of {} cannot be set to {} due to {} being frozen", stdCode, qty, frozen);
			return;
		}
	}

	append_signal(stdCode, qty, userTag);
}

void SelStraBaseCtx::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */)
{
	double curPx = _price_map[stdCode];

	SigInfo& sInfo = _sig_map[stdCode];
	sInfo._volume = qty;
	sInfo._sigprice = curPx;
	sInfo._usertag = userTag;
	sInfo._gentime = (uint64_t)_engine->get_date() * 1000000000 + (uint64_t)_engine->get_raw_time() * 100000 + _engine->get_secs();
	sInfo._triggered = !_is_in_schedule;

	log_signal(stdCode, qty, curPx, sInfo._gentime, userTag);

	save_data();
}

void SelStraBaseCtx::do_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, bool bTriggered /* = false */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_engine->get_date() * 10000 + _engine->get_min_time();
	uint32_t curTDate = _engine->get_trading_date();

	if (decimal::eq(pInfo._volume, qty))
		return;

	double diff = qty - pInfo._volume;

	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
		return;

	//成交价
	double trdPx = curPx;

	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))//当前持仓和目标仓位方向一致, 增加一条明细, 增加数量即可
	{
		pInfo._volume = qty;
		//如果T+1，则冻结仓位要增加
		if (commInfo->isT1())
		{
			//ASSERT(diff>0);
			pInfo._frozen += diff;
			pInfo._frozen_date = curTDate;
			log_debug("{} frozen position updated to {}", stdCode, pInfo._frozen);
		}

		if (_slippage != 0)
		{
			trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);
		}

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = trdPx;
		dInfo._max_price = trdPx;
		dInfo._min_price = trdPx;
		dInfo._volume = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		wt_strcpy(dInfo._opentag, userTag);
		pInfo._details.push_back(dInfo);
		pInfo._last_entertime = curTm;

		double fee = commInfo->calcFee(trdPx, abs(qty), 0);
		_fund_info._total_fees += fee;
		//_engine->mutate_fund(fee, FFT_Fee);
		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(qty), userTag, fee);
	}
	else
	{//持仓方向和目标仓位方向不一致, 需要平仓
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
			//if (maxQty == 0)
			if (decimal::eq(maxQty, 0))
				continue;

			dInfo._volume -= maxQty;
			left -= maxQty;

			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//浮盈也要做等比缩放
			pInfo._last_exittime = curTm;
			_fund_info._total_profit += profit;

			double fee = commInfo->calcFee(trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, userTag, fee);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, pInfo._closeprofit, dInfo._opentag, userTag);

			//if (left == 0)
			if (decimal::eq(left, 0))
				break;
		}

		//需要清理掉已经平仓完的明细
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//最后, 如果还有剩余的, 则需要反手了
		//if (left > 0)
		if (decimal::gt(left, 0))
		{
			left = left * qty / abs(qty);

			//如果T+1，则冻结仓位要增加
			if (commInfo->isT1())
			{
				//ASSERT(diff>0);
				pInfo._frozen += diff;
				pInfo._frozen_date = curTDate;
				log_debug("{} frozen position updated to {}", stdCode, pInfo._frozen);
			}

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = trdPx;
			dInfo._max_price = trdPx;
			dInfo._min_price = trdPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			wt_strcpy(dInfo._opentag, userTag);
			pInfo._details.push_back(dInfo);
			pInfo._last_entertime = curTm;

			//这里还需要写一笔成交记录
			double fee = commInfo->calcFee(trdPx, abs(qty), 0);
			_fund_info._total_fees += fee;
			//_engine->mutate_fund(fee, FFT_Fee);
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), userTag, fee);
		}
	}

	//存储数据
	save_data();

	_engine->handle_pos_change(_name.c_str(), stdCode, diff);
}

WTSKlineSlice* SelStraBaseCtx::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);
	
	uint64_t etime = 0;
	if (period[0] == 'd')
	{
		WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);
		etime = (uint64_t)_schedule_date * 10000 + sInfo->getCloseTime();
	}
	else
		etime = (uint64_t)_schedule_date * 10000 + _schedule_time;

	WTSKlineSlice* kline = _engine->get_kline_slice(_context_id, stdCode, basePeriod, count, times, etime);

	KlineTag& tag = _kline_tags[key];
	tag._closed = false;

	if (kline)
	{
		double lastClose = kline->at(-1)->close;
		_price_map[stdCode] = lastClose;
	}

	return kline;
}

WTSTickSlice* SelStraBaseCtx::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _engine->get_tick_slice(_context_id, stdCode, count);
}

WTSTickData* SelStraBaseCtx::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void SelStraBaseCtx::stra_sub_ticks(const char* stdCode)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	主动订阅tick会在本地记一下
	 *	tick数据回调的时候先检查一下
	 */
	_tick_subs.insert(stdCode);

	_engine->sub_tick(_context_id, stdCode);
	log_info("Market data subscribed: {}", stdCode);
}

WTSCommodityInfo* SelStraBaseCtx::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

std::string SelStraBaseCtx::stra_get_rawcode(const char* stdCode)
{
	return _engine->get_rawcode(stdCode);
}

WTSSessionInfo* SelStraBaseCtx::stra_get_sessinfo(const char* stdCode)
{
	return _engine->get_session_info(stdCode, true);
}

double SelStraBaseCtx::stra_get_day_price(const char* stdCode, int flag /* = 0 */)
{
	if (_engine)
		return _engine->get_day_price(stdCode, flag);

	return 0.0;
}

uint32_t SelStraBaseCtx::stra_get_tdate()
{
	return _engine->get_trading_date();
}

uint32_t SelStraBaseCtx::stra_get_date()
{
	return _is_in_schedule ? _schedule_date : _engine->get_date();
}

uint32_t SelStraBaseCtx::stra_get_time()
{
	return _is_in_schedule ? _schedule_time : _engine->get_min_time();
}

double SelStraBaseCtx::stra_get_fund_data(int flag)
{
	switch (flag)
	{
	case 0:
		return _fund_info._total_profit - _fund_info._total_fees + _fund_info._total_dynprofit;
	case 1:
		return _fund_info._total_profit;
	case 2:
		return _fund_info._total_dynprofit;
	case 3:
		return _fund_info._total_fees;
	default:
		return 0.0;
	}
}

void SelStraBaseCtx::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void SelStraBaseCtx::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void SelStraBaseCtx::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void SelStraBaseCtx::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

const char* SelStraBaseCtx::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void SelStraBaseCtx::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

uint64_t SelStraBaseCtx::stra_get_first_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[0]._opentime;
}

const char* SelStraBaseCtx::stra_get_last_entertag(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return "";

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return "";

	return pInfo._details[0]._opentag;
}


uint64_t SelStraBaseCtx::stra_get_last_exittime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._last_exittime;
}

uint64_t SelStraBaseCtx::stra_get_last_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[pInfo._details.size() - 1]._opentime;
}

double SelStraBaseCtx::stra_get_last_enterprice(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[pInfo._details.size() - 1]._price;
}

double SelStraBaseCtx::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, const char* userTag /* = "" */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (strlen(userTag) == 0)
	{
		//只有userTag为空的时候时候，才会用bOnlyValid
		if (bOnlyValid)
		{
			//这里理论上，只有多头才会进到这里
			//其他地方要保证，空头持仓的话，_frozen要为0
			return pInfo._volume - pInfo._frozen;
		}
		else
			return pInfo._volume;
	}

	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._volume;
	}

	return 0;
}

double SelStraBaseCtx::stra_get_position_avgpx(const char* stdCode)
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

double SelStraBaseCtx::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

uint64_t SelStraBaseCtx::stra_get_detail_entertime(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._opentime;
	}

	return 0;
}

double SelStraBaseCtx::stra_get_detail_cost(const char* stdCode, const char* userTag)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._price;
	}

	return 0.0;
}

double SelStraBaseCtx::stra_get_detail_profit(const char* stdCode, const char* userTag, int flag /* = 0 */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		switch (flag)
		{
		case 0:
			return dInfo._profit;
		case 1:
			return dInfo._max_profit;
		case -1:
			return dInfo._max_loss;
		case 2:
			return dInfo._max_price;
		case -2:
			return dInfo._min_price;
		}
	}

	return 0.0;
}

#pragma endregion 