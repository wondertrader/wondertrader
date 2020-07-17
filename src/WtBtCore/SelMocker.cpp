/*!
* \file MfMocker.cpp
* \project	WonderTrader
*
* \author Wesley
* \date 2020/03/30
*
* \brief
*/
#include "SelMocker.h"
#include "WtHelper.h"
#include "HisDataReplayer.h"

#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "../Share/StdUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Share/decimal.h"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSLogger.h"

namespace rj = rapidjson;

inline uint32_t makeSelCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 3000 };
	return _auto_context_id.fetch_add(1);
}


SelMocker::SelMocker(HisDataReplayer* replayer, const char* name)
	: ISelStraCtx(name)
	, _replayer(replayer)
	, _total_calc_time(0)
	, _emit_times(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
	, _strategy(NULL)
{
	_context_id = makeSelCtxId();
}


SelMocker::~SelMocker()
{
}

void SelMocker::init_outputs()
{
	bool isBt = true;

	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());

	std::string filename = folder + "trades.csv";
	_trade_logs.reset(new BoostFile());
	if (isBt)
	{
		_trade_logs->create_new_file(filename.c_str());
		_trade_logs->write_file("code,time,direct,action,price,qty,tag,fee\n");
	}
	else
	{
		bool isNewFile = !StdFile::exists(filename.c_str());
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
	if (isBt)
	{
		_close_logs->create_new_file(filename.c_str());
		_close_logs->write_file("code,direct,opentime,openprice,closetime,closeprice,qty,profit,totalprofit,entertag,exittag\n");
	}
	else
	{
		bool isNewFile = !StdFile::exists(filename.c_str());
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
	if (isBt)
	{
		_fund_logs->create_new_file(filename.c_str());
		_fund_logs->write_file("date,closeprofit,positionprofit,dynbalance,fee\n");
	}
	else
	{
		bool isNewFile = !StdFile::exists(filename.c_str());
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
	if (isBt)
	{
		_sig_logs->create_new_file(filename.c_str());
		_sig_logs->write_file("code,target,sigprice,gentime,usertag\n");
	}
	else
	{
		bool isNewFile = !StdFile::exists(filename.c_str());
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

void SelMocker::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	//if (_sig_logs)
	//	_sig_logs->write_file(StrUtil::printf("%s,%f,%f,%s,%s\n", stdCode, target, price, StrUtil::fmtUInt64(gentime).c_str(), usertag));

	if (_sig_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
		_sig_logs->write_file(ss.str());
	}
}

void SelMocker::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag, double fee)
{
	//if (_trade_logs)
	//	_trade_logs->write_file(StrUtil::printf("%s,%s,%s,%s,%f,%f,%s,%.2f\n", stdCode, StrUtil::fmtUInt64(curTime).c_str(), isLong ? "LONG" : "SHORT", isOpen ? "OPEN" : "CLOSE", price, qty, userTag, fee));
	if (_trade_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") << "," << price << "," << qty << "," << userTag << "," << fee << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void SelMocker::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty,
	double profit, double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */)
{
	//if (_close_logs)
	//	_close_logs->write_file(StrUtil::printf("%s,%s,%s,%f,%s,%f,%f,%.2f,%.2f,%s,%s\n",
	//	stdCode, isLong ? "LONG" : "SHORT", StrUtil::fmtUInt64(openTime).c_str(), openpx, 
	//	StrUtil::fmtUInt64(closeTime).c_str(), closepx, qty, profit, totalprofit, enterTag, exitTag));

	if (_close_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << ","
			<< totalprofit << "," << enterTag << "," << exitTag << "\n";
		_close_logs->write_file(ss.str());
	}
}

bool SelMocker::initSelFactory(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	const char* module = cfg->getCString("module");

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateSelStraFact creator = (FuncCreateSelStraFact)DLLHelper::get_symbol(hInst, "createSelStrategyFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteSelStraFact)DLLHelper::get_symbol(hInst, "deleteSelStrategyFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgStra = cfg->get("strategy");
	if (cfgStra)
	{
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), cfgStra->getCString("id"));
		if (_strategy)
		{
			WTSLogger::info("策略%s.%s创建成功，策略ID：%s", _factory._fact->getName(), _strategy->getName(), _strategy->id());
		}
		_strategy->init(cfgStra->get("params"));
		_name = _strategy->id();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//IDataSink
void SelMocker::handle_init()
{
	this->on_init();
}

void SelMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	this->on_bar(stdCode, period, times, newBar);
}

void SelMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	uint32_t nextTime = TimeUtils::getNextMinute(uTime, 1);
	if (nextTime < uTime)
		uDate = TimeUtils::getNextDate(uDate);
	this->on_schedule(uDate, uTime, nextTime);
}

void SelMocker::handle_session_begin()
{
	this->on_session_begin();
}

void SelMocker::handle_session_end()
{
	this->on_session_end();
}

void SelMocker::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	this->on_tick(stdCode, curTick, true);
}


//////////////////////////////////////////////////////////////////////////
//回调函数
void SelMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	std::string realPeriod;
	if (period[0] == 'd')
		realPeriod = StrUtil::printf("%s%u", period, times);
	else
		realPeriod = StrUtil::printf("m%u", times);

	std::string key = StrUtil::printf("%s#%s", stdCode, realPeriod.c_str());
	KlineTag& tag = _kline_tags[key];
	tag._closed = true;

	on_bar_close(stdCode, realPeriod.c_str(), newBar);
}

void SelMocker::on_init()
{
	init_outputs();

	if (_strategy)
		_strategy->on_init(this);

	WTSLogger::info("策略初始化完成");
}

void SelMocker::update_dyn_profit(const char* stdCode, double price)
{
	auto it = _pos_map.find(stdCode);
	if (it != _pos_map.end())
	{
		PosInfo& pInfo = it->second;
		if (pInfo._volumn == 0)
		{
			pInfo._dynprofit = 0;
		}
		else
		{
			WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volumn*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

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

void SelMocker::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	_price_map[stdCode].first = newTick->price();
	_price_map[stdCode].second = (uint64_t)newTick->actiondate() * 1000000000 + newTick->actiontime();

	//先检查是否要信号要触发
	{
		auto it = _sig_map.find(stdCode);
		if (it != _sig_map.end())
		{
			//if (sInfo->isInTradingTime(_replayer->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				double price;
				if (decimal::eq(sInfo._desprice, 0.0))
					price = newTick->price();
				else
					price = sInfo._desprice;
				do_set_position(stdCode, sInfo._volumn, price, sInfo._usertag.c_str(), sInfo._triggered);
				_sig_map.erase(it);
			}

		}
	}

	update_dyn_profit(stdCode, newTick->price());

	if (bEmitStrategy)
		on_tick_updated(stdCode, newTick);
}

void SelMocker::on_bar_close(const char* code, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, newBar);
}

void SelMocker::on_tick_updated(const char* code, WTSTickData* newTick)
{
	if (_strategy)
		_strategy->on_tick(this, code, newTick);
}

void SelMocker::on_strategy_schedule(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}


bool SelMocker::on_schedule(uint32_t curDate, uint32_t curTime, uint32_t fireTime)
{
	_is_in_schedule = true;//开始调度，修改标记

	//主要用于保存浮动盈亏的
	//save_data();

	TimeUtils::Ticker ticker;
	on_strategy_schedule(curDate, curTime);
	stra_log_text("策略已重新调度 @ %u.%u[闭合时间%u]", curDate, fireTime, curTime);

	std::unordered_set<std::string> to_clear;
	for(auto& v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		const char* code = v.first.c_str();
		if(_sig_map.find(code) == _sig_map.end() && !decimal::eq(pInfo._volumn, 0.0))
		{
			//新的信号中没有该持仓，则要清空
			to_clear.insert(code);
		}
	}

	for(const std::string& code : to_clear)
	{
		append_signal(code.c_str(), 0, "autoexit");
	}

	_emit_times++;
	_total_calc_time += ticker.micro_seconds();

	if (_emit_times % 20 == 0)
		stra_log_text(fmt::format("策略共触发{}次, 共耗时{}微秒, 平均耗时{}微秒",
		_emit_times, _total_calc_time, _total_calc_time / _emit_times).c_str());

	_is_in_schedule = false;//调度结束，修改标记
	return true;
}

void SelMocker::on_session_begin()
{
}

void SelMocker::enum_position(FuncEnumSelPositionCallBack cb)
{
	std::unordered_map<std::string, double> desPos;
	for (auto it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		const PosInfo& pInfo = it.second;
		//cb(stdCode, pInfo._volumn);
		desPos[stdCode] = pInfo._volumn;
	}

	for (auto sit : _sig_map)
	{
		const char* stdCode = sit.first.c_str();
		const SigInfo& sInfo = sit.second;
		desPos[stdCode] = sInfo._volumn;
	}

	for (auto v : desPos)
	{
		cb(v.first.c_str(), v.second);
	}
}

void SelMocker::on_session_end()
{
	uint32_t curDate = _replayer->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;
	}

	//TODO:
	//这里要把当日结算的数据写到日志文件里
	//而且这里回测和实盘写法不同，先留着，后面来做
	if (_fund_logs)
		_fund_logs->write_file(StrUtil::printf("%d,%.2f,%.2f,%.2f,%.2f\n", curDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees));

	//save_data();
}

//////////////////////////////////////////////////////////////////////////
//策略接口
double SelMocker::stra_get_price(const char* stdCode)
{
	if (_replayer)
		return _replayer->get_cur_price(stdCode);

	return 0.0;
}

void SelMocker::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */)
{
	_replayer->sub_tick(id(), stdCode);
	append_signal(stdCode, qty, userTag);
}

void SelMocker::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */, double price/* = 0.0*/)
{
	double curPx = _price_map[stdCode].first;

	SigInfo& sInfo = _sig_map[stdCode];
	sInfo._volumn = qty;
	sInfo._sigprice = curPx;
	sInfo._desprice = price;
	sInfo._usertag = userTag;
	sInfo._gentime = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_raw_time() * 100000 + _replayer->get_secs();
	sInfo._triggered = !_is_in_schedule;

	log_signal(stdCode, qty, curPx, sInfo._gentime, userTag);

	//save_data();
}

void SelMocker::do_set_position(const char* stdCode, double qty, double price /* = 0.0 */, const char* userTag /* = "" */, bool bTriggered /* = false */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode].first;
	uint64_t curTm = (uint64_t)_replayer->get_date() * 10000 + _replayer->get_min_time();
	uint32_t curTDate = _replayer->get_trading_date();

	if (decimal::eq(pInfo._volumn, qty))
		return;

	double diff = qty - pInfo._volumn;

	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);

	if (decimal::gt(pInfo._volumn*qty, 0))//当前持仓和目标仓位方向一致，增加一条明细，增加数量即可
	{
		double diff = abs(qty - pInfo._volumn);
		pInfo._volumn = qty;

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = curPx;
		dInfo._volumn = diff;
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		strcpy(dInfo._opentag, userTag);
		pInfo._details.push_back(dInfo);

		double fee = _replayer->calc_fee(stdCode, curPx, abs(qty), 0);
		_fund_info._total_fees += fee;
		//_engine->mutate_fund(fee, FFT_Fee);
		log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(qty), userTag, fee);
	}
	else
	{//持仓方向和目标仓位方向不一致，需要平仓
		double left = abs(pInfo._volumn) + abs(qty);

		pInfo._volumn = qty;
		if (decimal::eq(pInfo._volumn, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			double maxQty = min(dInfo._volumn, left);
			if (decimal::eq(maxQty, 0))
				continue;

			dInfo._volumn -= maxQty;
			left -= maxQty;

			//if (dInfo._volumn == 0)
			if (decimal::eq(dInfo._volumn, 0))
				count++;

			double profit = (curPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volumn / (dInfo._volumn + maxQty);//浮盈也要做等比缩放
			_fund_info._total_profit += profit;
			//_engine->mutate_fund(profit, FFT_CloseProfit);

			double fee = _replayer->calc_fee(stdCode, curPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//_engine->mutate_fund(fee, FFT_Fee);
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, curPx, maxQty, userTag, fee);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, curPx, maxQty, profit, pInfo._closeprofit, dInfo._opentag, userTag);

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

		//最后，如果还有剩余的，则需要反手了
		if (left > 0)
		{
			left = left * qty / abs(qty);

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = curPx;
			dInfo._volumn = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			strcpy(dInfo._opentag, userTag);
			pInfo._details.push_back(dInfo);

			//TODO: 
			//这里还需要写一笔成交记录
			double fee = _replayer->calc_fee(stdCode, curPx, abs(qty), 0);
			_fund_info._total_fees += fee;
			//_engine->mutate_fund(fee, FFT_Fee);
			log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(left), userTag, fee);
		}
	}
}

WTSKlineSlice* SelMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count)
{
	std::string key = StrUtil::printf("%s#%s", stdCode, period);

	std::string basePeriod = "";
	uint32_t times = 1;
	if (strlen(period) > 1)
	{
		basePeriod.append(period, 1);
		times = strtoul(period + 1, NULL, 10);
	}
	else
	{
		basePeriod = period;
	}

	WTSKlineSlice* kline = _replayer->get_kline_slice(stdCode, basePeriod.c_str(), count, times, false);

	KlineTag& tag = _kline_tags[key];
	tag._closed = false;

	if (kline)
	{
		double lastClose = kline->close(-1);
		uint64_t lastTime = 0;
		if(basePeriod[0] == 'd')
		{
			lastTime = kline->date(-1);
			WTSSessionInfo* sInfo = _replayer->get_session_info(stdCode, true);
			lastTime *= 1000000000;
			lastTime += (uint64_t)sInfo->getCloseTime() * 100000;
		}
		else
		{
			lastTime = kline->time(-1);
			lastTime += 199000000000;
			lastTime *= 100000;
		}

		if(lastTime > _price_map[stdCode].second)
		{
			_price_map[stdCode].second = lastTime;
			_price_map[stdCode].first = lastClose;
		}
	}

	return kline;
}

WTSTickSlice* SelMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSTickData* SelMocker::stra_get_last_tick(const char* stdCode)
{
	return _replayer->get_last_tick(stdCode);
}

void SelMocker::sub_ticks(const char* code)
{
	_replayer->sub_tick(_context_id, code);
}

WTSCommodityInfo* SelMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

WTSSessionInfo* SelMocker::stra_get_sessinfo(const char* stdCode)
{
	return _replayer->get_session_info(stdCode, true);
}


uint32_t SelMocker::stra_get_date()
{
	return _replayer->get_date();
}

uint32_t SelMocker::stra_get_time()
{
	return _replayer->get_min_time();
}

void SelMocker::stra_log_text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog_dyn("strategy", _name.c_str(), LL_INFO, fmt, args);
	va_end(args);
}

const char* SelMocker::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void SelMocker::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

double SelMocker::stra_get_position(const char* stdCode, const char* userTag /* = "" */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (strlen(userTag) == 0)
		return pInfo._volumn;

	for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
	{
		const DetailInfo& dInfo = (*it);
		if (strcmp(dInfo._opentag, userTag) != 0)
			continue;

		return dInfo._volumn;
	}

	return 0;
}
