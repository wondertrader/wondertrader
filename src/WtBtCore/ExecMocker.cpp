/*!
 * \file ExecMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ExecMocker.h"
#include "WtHelper.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../WTSTools/WTSLogger.h"

#include <boost/filesystem.hpp>

#define PRICE_DOUBLE_TO_INT_P(x) ((int32_t)((x)*10000.0 + 0.5))
#define PRICE_DOUBLE_TO_INT_N(x) ((int32_t)((x)*10000.0 - 0.5))
#define PRICE_DOUBLE_TO_INT(x) (((x)==DBL_MAX)?0:((x)>0?PRICE_DOUBLE_TO_INT_P(x):PRICE_DOUBLE_TO_INT_N(x)))

extern uint32_t makeLocalOrderID();

ExecMocker::ExecMocker(HisDataReplayer* replayer)
	: _replayer(replayer)
	, _position(0)
	, _undone(0)
	, _ord_qty(0)
	, _ord_cnt(0)
	, _cacl_cnt(0)
	, _cacl_qty(0)
	, _sig_cnt(0)
	, _last_tick(NULL)
{
}


ExecMocker::~ExecMocker()
{
	if (_last_tick)
		_last_tick->release();
}

bool ExecMocker::init(WTSVariant* cfg)
{
	const char* module = cfg->getCString("module");
	_code = cfg->getCString("code");
	_period = cfg->getCString("period");
	_volunit = cfg->getInt32("volunit");

	_matcher.regisSink(this);
	_matcher.init(cfg->get("matcher"));

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateExeFact creator = (FuncCreateExeFact)DLLHelper::get_symbol(hInst, "createExecFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteExeFact)DLLHelper::get_symbol(hInst, "deleteExecFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgExec = cfg->get("executer");
	if (cfgExec)
	{
		_exec_unit = _factory._fact->createExeUnit(cfgExec->getCString("name"));
		_exec_unit->init(this, _code.c_str(), cfgExec->get("params"));
		_id = cfgExec->getCString("id");
	}

	return true;
}

WTSCommodityInfo* ExecMocker::getCommodityInfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

WTSSessionInfo* ExecMocker::getSessionInfo(const char* stdCode)
{
	return _replayer->get_session_info(stdCode, true);
}

uint64_t ExecMocker::getCurTime()
{
	return TimeUtils::makeTime(_replayer->get_date(), _replayer->get_raw_time() * 100000 + _replayer->get_secs());
}

void ExecMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void ExecMocker::handle_session_begin(uint32_t curTDate)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void ExecMocker::handle_session_end(uint32_t curTDate)
{
	_matcher.clear();
	_undone = 0;

	WTSLogger::info(fmt::format("Total entrust:{}, total quantity:{}, total cancels:{}, total cancel quantity:{}, total signals:{}", _ord_cnt, _ord_qty, _cacl_cnt, _cacl_qty, _sig_cnt).c_str());
}

void ExecMocker::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	if (_last_tick)
	{
		_last_tick->release();
		_last_tick = NULL;
	}

	_last_tick = curTick;
	_last_tick->retain();
	
	_matcher.handle_tick(stdCode, curTick);

	if (_exec_unit)
		_exec_unit->on_tick(curTick);
}

void ExecMocker::handle_init()
{
	WTSKlineSlice* kline = _replayer->get_kline_slice(_code.c_str(), "m", 10, 1, true);
	if (kline)
		kline->release();

	_replayer->sub_tick(0, _code.c_str());

	std::string folder = WtHelper::getOutputDir();
	folder += "exec/";
	boost::filesystem::create_directories(folder.c_str());

	std::stringstream ss;
	ss << folder << "trades_" << _id << ".csv";
	std::string filename = ss.str();
	_trade_logs.open(filename, std::ios_base::trunc);
	_trade_logs << "localid,signaltime,ordertime,bs,sigprice,ordprice,lmtprice,tradetime,trdprice,qty,sigtimespan,exectime,cancel" << std::endl;

	_exec_unit->on_channel_ready();
}

void ExecMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	if (uTime == 1500)
		return;

	_sig_px = _last_tick->price();
	_sig_time = (uint64_t)uDate * 10000 + uTime;
	if (_position <= 0)
	{
		_exec_unit->set_position(_code.c_str(), _volunit);
		WTSLogger::info("Target position updated @%u.%u: %d", uDate, uTime, _volunit);
	}
	else
	{
		_exec_unit->set_position(_code.c_str(), -_volunit);
		WTSLogger::info("Target position updated @%u.%u: %d", uDate, uTime, -_volunit);
	}
	_sig_cnt++;
}

WTSTickSlice* ExecMocker::getTicks(const char* stdCode, uint32_t count, uint64_t etime /*= 0*/)
{
	return _replayer->get_tick_slice(stdCode, count, etime);
}

WTSTickData* ExecMocker::grabLastTick(const char* stdCode)
{
	return _replayer->get_last_tick(stdCode);
}

double ExecMocker::getPosition(const char* stdCode, int32_t flag /*= 3*/)
{
	return _position;
}

OrderMap* ExecMocker::getOrders(const char* stdCode)
{
	return NULL;
}

double ExecMocker::getUndoneQty(const char* stdCode)
{
	return _undone;
}

OrderIDs ExecMocker::buy(const char* stdCode, double price, double qty, bool bForceClose /* = false */)
{
	uint64_t curTime = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_raw_time() * 100000 + _replayer->get_secs();
	OrderIDs ret = _matcher.buy(stdCode, price, qty, curTime);

	_ord_cnt++;
	_ord_qty += qty;

	_undone += (int32_t)qty;
	WTSLogger::info("%s, undone orders updated: %d", __FUNCTION__,_undone);

	return ret;
}

OrderIDs ExecMocker::sell(const char* stdCode, double price, double qty, bool bForceClose /* = false */)
{
	uint64_t curTime = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_raw_time() * 100000 + _replayer->get_secs();
	OrderIDs ret = _matcher.sell(stdCode, price, qty, curTime);

	_ord_cnt++;
	_ord_qty += qty;

	_undone -= (int32_t)qty;
	WTSLogger::info("%s, undone orders updated: %d", __FUNCTION__, _undone);

	return ret;
}

bool ExecMocker::cancel(uint32_t localid)
{
	double change = _matcher.cancel(localid);
	if (decimal::eq(change, 0))
		return false;

	_undone -= change;
	_cacl_cnt++;
	_cacl_qty += abs(change);
	WTSLogger::info("%s, undone orders updated: %d", __FUNCTION__, _undone);

	return true;
}

OrderIDs ExecMocker::cancel(const char* stdCode, bool isBuy, double qty /*= 0*/)
{
	OrderIDs ret = _matcher.cancel(stdCode, isBuy, qty, [this](double change) {
		_undone -= change;

		_cacl_cnt++;
		_cacl_qty += abs(change);
	});
	WTSLogger::info("%s, undone orders updated: %d", __FUNCTION__, _undone);

	return ret;
}

void ExecMocker::writeLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog_dyn("executer", _id.c_str(), LL_INFO, fmt, args);
	va_end(args);
}

void ExecMocker::handle_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, uint64_t ordTime)
{
	_exec_unit->on_entrust(localid, stdCode, bSuccess, message);
}

void ExecMocker::handle_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled, uint64_t ordTime)
{
	uint64_t curTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	uint64_t curUnixTime = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());

	uint64_t sigUnixTime = TimeUtils::makeTime((uint32_t)(_sig_time / 10000), _sig_time % 10000 * 100000);
	uint64_t ordUnixTime = TimeUtils::makeTime((uint32_t)(ordTime / 1000000000), ordTime % 1000000000);

	_exec_unit->on_order(localid, stdCode, isBuy, leftover, price, isCanceled);
	if(isCanceled)
	{
		_trade_logs << localid << ","
			<< _sig_time << ","
			<< ordTime << ","
			<< (isBuy ? "B" : "S") << ","
			<< _sig_px << ","
			<< 0 << ","
			<< price << ","
			<< curTime << ","
			<< price << ","
			<< 0 << ","
			<< curUnixTime - sigUnixTime << ","
			<< curUnixTime - ordUnixTime << ","
			<< "true" << std::endl;
	}
}

void ExecMocker::handle_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double fireprice, double price, uint64_t ordTime)
{
	uint64_t curTime = (uint64_t)_last_tick->actiondate() * 1000000000 + _last_tick->actiontime();
	uint64_t curUnixTime = TimeUtils::makeTime(_last_tick->actiondate(), _last_tick->actiontime());

	uint64_t sigUnixTime = TimeUtils::makeTime((uint32_t)(_sig_time / 10000), _sig_time % 10000 * 100000);
	uint64_t ordUnixTime = TimeUtils::makeTime((uint32_t)(ordTime / 1000000000), ordTime % 1000000000);

	_exec_unit->on_trade(localid, stdCode, isBuy, vol, price);
	_trade_logs << localid << ","
		<< _sig_time << ","
		<< ordTime << ","
		<< (isBuy?"B":"S") << ","
		<< _sig_px << ","
		<< fireprice << ","
		<< price << ","
		<< curTime << ","
		<< price << ","
		<< vol << ","
		<< curUnixTime - sigUnixTime << ","
		<< curUnixTime - ordUnixTime << ","
		<< "false" << std::endl;

	_position += vol* (isBuy?1:-1);
	_undone -= vol * (isBuy ? 1 : -1);
	WTSLogger::info("%s, undone orders updated: %d", __FUNCTION__, _undone);
	WTSLogger::info("Position updated: %d", _position);
}