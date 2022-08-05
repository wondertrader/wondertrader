/*!
 * \file WtExecuter.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtDiffExecuter.h"
#include "TraderAdapter.h"
#include "WtEngine.h"
#include "WtHelper.h"

#include "../Share/CodeHelper.hpp"
#include "../Includes/IDataManager.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/IHotMgr.h"
#include "../Share/decimal.h"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_WTP;


WtDiffExecuter::WtDiffExecuter(WtExecuterFactory* factory, const char* name, IDataManager* dataMgr)
	: IExecCommand(name)
	, _factory(factory)
	, _data_mgr(dataMgr)
	, _channel_ready(false)
	, _scale(1.0)
	, _avaliable(-1)
	, _fix_capital(0)
	, _use_fix_capital(false)
{
}


WtDiffExecuter::~WtDiffExecuter()
{
	if (_pool)
		_pool->wait();
}

bool WtDiffExecuter::init(WTSVariant* params)
{
	if (params == NULL)
		return false;

	_config = params;
	_config->retain();

	_scale = params->getDouble("scale");
	if (params->has("fix_capital"))
	{
		_use_fix_capital = true;
		_fix_capital = params->getDouble("fix_capital");
	}
	uint32_t poolsize = params->getUInt32("poolsize");
	if(poolsize > 0)
	{
		_pool.reset(new boost::threadpool::pool(poolsize));
	}

	load_data();

	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Diff executer inited, scale: {}, thread poolsize: {}", _scale, poolsize);

	return true;
}

void WtDiffExecuter::load_data()
{
	//读取执行器的理论部位，以及待执行的差量
	std::string filename = WtHelper::getExecDataDir();
	filename += _name + ".json";

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

	if(root.HasMember("targets"))
	{
		const rj::Value& jTargets = root["targets"];
		for (const rj::Value& jItem : jTargets.GetArray())
		{
			const char* stdCode = jItem["code"].GetString();
			double pos = jItem["target"].GetDouble();
			_target_pos[stdCode] = pos;
		}
	}

	if (root.HasMember("diffs"))
	{
		const rj::Value& jDiffs = root["diffs"];
		for (const rj::Value& jItem : jDiffs.GetArray())
		{
			const char* stdCode = jItem["code"].GetString();
			double pos = jItem["diff"].GetDouble();
			_diff_pos[stdCode] = pos;
		}
	}
}

void WtDiffExecuter::save_data()
{
	std::string filename = WtHelper::getExecDataDir();
	filename += _name + ".json";

	rj::Document root(rj::kObjectType);
	rj::Document::AllocatorType &allocator = root.GetAllocator();

	{//目标持仓数据保存
		rj::Value jTarget(rj::kArrayType);

		for (auto& v : _target_pos)
		{
			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("code", rj::Value(v.first.c_str(), allocator), allocator);
			jItem.AddMember("target", v.second, allocator);

			jTarget.PushBack(jItem, allocator);
		}

		root.AddMember("targets", jTarget, allocator);
	}

	{//差量持仓数据保存
		rj::Value jDiff(rj::kArrayType);

		for (auto& v : _diff_pos)
		{
			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("code", rj::Value(v.first.c_str(), allocator), allocator);
			jItem.AddMember("diff", v.second, allocator);

			jDiff.PushBack(jItem, allocator);
		}

		root.AddMember("diffs", jDiff, allocator);
	}

	{
		std::string filename = WtHelper::getExecDataDir();
		filename += _name + ".json";

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

ExecuteUnitPtr WtDiffExecuter::getUnit(const char* stdCode, bool bAutoCreate /* = true */)
{
	std::string commID = CodeHelper::stdCodeToStdCommID(stdCode);

	WTSVariant* policy = _config->get("policy");
	std::string des = commID;
	if (!policy->has(commID.c_str()))
		des = "default";

	SpinLock lock(_mtx_units);

	auto it = _unit_map.find(stdCode);
	if(it != _unit_map.end())
	{
		return it->second;
	}

	if (bAutoCreate)
	{
		WTSVariant* cfg = policy->get(des.c_str());

		const char* name = cfg->getCString("name");
		ExecuteUnitPtr unit = _factory->createDiffExeUnit(name);
		if (unit != NULL)
		{
			unit->self()->init(this, stdCode, cfg);
			_unit_map[stdCode] = unit;
		}
		else
		{
			WTSLogger::error("Creating ExecUnit {} failed", name);
		}
		return unit;
	}
	else
	{
		return ExecuteUnitPtr();
	}
}


//////////////////////////////////////////////////////////////////////////
//ExecuteContext
#pragma region Context回调接口
WTSTickSlice* WtDiffExecuter::getTicks(const char* stdCode, uint32_t count, uint64_t etime /* = 0 */)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->get_tick_slice(stdCode, count);
}

WTSTickData* WtDiffExecuter::grabLastTick(const char* stdCode)
{
	if (_data_mgr == NULL)
		return NULL;

	return _data_mgr->grab_last_tick(stdCode);
}

double WtDiffExecuter::getPosition(const char* stdCode, bool validOnly /* = true */, int32_t flag /* = 3 */)
{
	return _trader->getPosition(stdCode, validOnly, flag);
}

double WtDiffExecuter::getUndoneQty(const char* stdCode)
{
	return _trader->getUndoneQty(stdCode);
}

OrderMap* WtDiffExecuter::getOrders(const char* stdCode)
{
	return _trader->getOrders(stdCode);
}

OrderIDs WtDiffExecuter::buy(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();
	return _trader->buy(stdCode, price, qty, 0, bForceClose);
}

OrderIDs WtDiffExecuter::sell(const char* stdCode, double price, double qty, bool bForceClose/* = false*/)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->sell(stdCode, price, qty, 0, bForceClose);
}

bool WtDiffExecuter::cancel(uint32_t localid)
{
	if (!_channel_ready)
		return false;

	return _trader->cancel(localid);
}

OrderIDs WtDiffExecuter::cancel(const char* stdCode, bool isBuy, double qty)
{
	if (!_channel_ready)
		return OrderIDs();

	return _trader->cancel(stdCode, isBuy, qty);
}

void WtDiffExecuter::writeLog(const char* message)
{
	static thread_local char szBuf[2048] = { 0 };
	fmtutil::format_to(szBuf, "[{}]{}", _name.c_str(), message);
	WTSLogger::log_dyn_raw("executer", _name.c_str(), LL_INFO, szBuf);
}

WTSCommodityInfo* WtDiffExecuter::getCommodityInfo(const char* stdCode)
{
	return _stub->get_comm_info(stdCode);
}

WTSSessionInfo* WtDiffExecuter::getSessionInfo(const char* stdCode)
{
	return _stub->get_sess_info(stdCode);
}

uint64_t WtDiffExecuter::getCurTime()
{
	return _stub->get_real_time();
	//return TimeUtils::makeTime(_stub->get_date(), _stub->get_raw_time() * 100000 + _stub->get_secs());
}

bool WtDiffExecuter::getMarketValue(double& market_value)
{
	if (!_channel_ready)
		return false;
	std::map<std::string, double> real_pos_map{};
	// TODO 增加保证金比例的转换
	_trader->enumPosition([&real_pos_map](const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail) {
		if (real_pos_map.find(stdCode) == real_pos_map.end())
			real_pos_map[stdCode] = 0;
		double real_pos = (prevol + newvol);
		real_pos_map[stdCode] += real_pos;
	});

	market_value = 0;
	for (auto& it : real_pos_map)
	{
		std::string stdCode = it.first;
		double real_pos = it.second;
		WTSTickData* tick = grabLastTick(stdCode.c_str());
		if (tick)
		{
			market_value += real_pos * tick->price();
			tick->release();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool WtDiffExecuter::amountToPos(const char* stdCode, double amount, double& pos)
{
	WTSTickData* tick = grabLastTick(stdCode);
	if (tick)
	{
		double last_price = tick->price();
		pos = amount / last_price;
		writeLog(fmtutil::format("[{}] amount:{} -> pos:{} with price:{}", stdCode, amount, pos, last_price));
		tick->release();
		return true;
	}
	else
		return false;
	return false;
}

bool WtDiffExecuter::ratioToPos(const char* stdCode, double ratio, double& pos)
{
	if (decimal::le(_avaliable))
		return false;
	double market_value = 0;
	if (!getMarketValue(market_value))
		return false;
	double total_captaial = _fix_capital;
	if (!_use_fix_capital)
		total_captaial = _avaliable + market_value;
	double amount = total_captaial * ratio;
	writeLog(fmtutil::format("[{}] ratio:{} -> amount:{} with total_captaial:{}", stdCode, ratio, amount, total_captaial));
	return amountToPos(stdCode, amount, pos);
}

inline void WtDiffExecuter::checkTarget()
{
	faster_hashset<LongKey> finish_target_amount{};
	for (auto& it : _target_amount)
	{
		double pos;
		std::string stdCode = it.first.c_str();
		double amount = it.second;
		if (amountToPos(stdCode.c_str(), amount, pos))
		{
			on_position_changed(stdCode.c_str(), pos);
			finish_target_amount.insert(stdCode.c_str());
		}
	}
	for (auto& finish_stdCode : finish_target_amount)
	{
		_target_amount.erase(finish_stdCode);
	}

	faster_hashset<LongKey> finish_target_ratio{};
	for (auto& it : _target_ratio)
	{
		double pos;
		std::string stdCode = it.first.c_str();
		double ratio = it.second;
		if (ratioToPos(stdCode.c_str(), ratio, pos))
		{
			on_position_changed(stdCode.c_str(), pos);
			finish_target_ratio.insert(stdCode.c_str());
		}
	}
	for (auto& finish_stdCode : finish_target_ratio)
	{
		_target_ratio.erase(finish_stdCode);
	}
}

#pragma endregion Context回调接口
//ExecuteContext
//////////////////////////////////////////////////////////////////////////


#pragma region 外部接口
void WtDiffExecuter::on_position_changed(const char* stdCode, double targetPos)
{
	ExecuteUnitPtr unit = getUnit(stdCode, true);
	if (unit == NULL)
		return;

	targetPos = round(targetPos*_scale);

	double oldVol = _target_pos[stdCode];
	_target_pos[stdCode] = targetPos;

	if(decimal::eq(oldVol, targetPos))
		return;

	//更新差量
	double& thisDiff = _diff_pos[stdCode];
	double prevDiff = thisDiff;
	thisDiff += (targetPos - oldVol);

	//WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target position of {} changed: {} -> {}", stdCode, oldVol, targetPos);
	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target position of {} changed: {} -> {}, diff postion changed: {} -> {}", stdCode, oldVol, targetPos, prevDiff, thisDiff);

	if (_trader && !_trader->checkOrderLimits(stdCode))
	{
		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "{} is disabled", stdCode);
		return;
	}

	//TODO 差量执行还要再看一下
	if (_pool)
	{
		std::string code = stdCode;
		_pool->schedule([unit, code, thisDiff]() {
			unit->self()->set_position(code.c_str(), thisDiff);
		});
	}
	else
	{
		unit->self()->set_position(stdCode, thisDiff);
	}
}

void WtDiffExecuter::on_amount_changed(const char* stdCode, double targetAmount)
{
	ExecuteUnitPtr unit = getUnit(stdCode, true);
	if (unit == NULL)
		return;

	double oldAmount = _target_amount[stdCode];
	_target_amount[stdCode] = targetAmount;

	if (decimal::eq(oldAmount, targetAmount))
		return;

	//更新差量
	double& thisDiff = _diff_pos[stdCode];
	double prevDiff = thisDiff;
	thisDiff += (targetAmount - oldAmount);

	//WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target amount of {} changed: {} -> {}", stdCode, oldAmount, targetAmount);
	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target amount of {} changed: {} -> {}, diff amount changed: {} -> {}", stdCode, oldAmount, targetAmount, prevDiff, thisDiff);

	double pos{ 0 };
	if (amountToPos(stdCode, targetAmount, pos))
	{
		on_position_changed(stdCode, pos);
		_target_amount.erase(stdCode);
	}
}

void WtDiffExecuter::on_ratio_changed(const char* stdCode, double targetRatio)
{
	ExecuteUnitPtr unit = getUnit(stdCode, true);
	if (unit == NULL)
		return;

	double oldRatio = _target_ratio[stdCode];
	_target_ratio[stdCode] = targetRatio;

	if (decimal::eq(oldRatio, targetRatio))
		return;

	//更新差量
	double& thisDiff = _diff_pos[stdCode];
	double prevDiff = thisDiff;
	thisDiff += (targetRatio - oldRatio);

	//WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target ratio of {} changed: {} -> {}", stdCode, oldRatio, targetRatio);
	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target ratio of {} changed: {} -> {}, diff ratio changed: {} -> {}", stdCode, oldRatio, targetRatio, prevDiff, thisDiff);

	double pos{ 0 };
	if (ratioToPos(stdCode, targetRatio, pos))
	{
		on_position_changed(stdCode, pos);
		_target_ratio.erase(stdCode); 
	}
}

void WtDiffExecuter::set_position(const faster_hashmap<LongKey, double>& targets)
{
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		const char* stdCode = it->first.c_str();		
		double newVol = it->second;
		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;

		newVol = round(newVol*_scale);
		double oldVol = _target_pos[stdCode];
		_target_pos[stdCode] = newVol;
		if (decimal::eq(oldVol, newVol))
			continue;

		//差量更新
		double& thisDiff = _diff_pos[stdCode];
		double prevDiff = thisDiff;
		thisDiff += (newVol - oldVol);

		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Target position of {} changed: {} -> {}, diff postion changed: {} -> {}", stdCode, oldVol, newVol, prevDiff, thisDiff);

		if (_trader && !_trader->checkOrderLimits(stdCode))
		{
			WTSLogger::log_dyn("executer", _name.c_str(), LL_WARN, "{} is disabled due to entrust limit control ", stdCode);
			continue;
		}

		//TODO 差量执行还要再看一下
		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code, thisDiff](){
				unit->self()->set_position(code.c_str(), thisDiff);
			});
		}
		else
		{
			unit->self()->set_position(stdCode, thisDiff);
		}
	}

	//在原来的目标头寸中，但是不在新的目标头寸中，则需要自动设置为0
	for (auto it = _target_pos.begin(); it != _target_pos.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		double& pos = (double&)it->second;
		auto tit = targets.find(stdCode);
		if(tit != targets.end())
			continue;

		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "{} is not in target, set to 0 automatically", stdCode);

		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;

		//差量更新
		//更新差量
		double& thisDiff = _diff_pos[stdCode];
		double prevDiff = thisDiff;
		thisDiff -= -pos;
		pos = 0;

		//TODO 差量执行还要再看一下
		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code, thisDiff](){
				unit->self()->set_position(code.c_str(), thisDiff);
			});
		}
		else
		{
			unit->self()->set_position(stdCode, thisDiff);
		}

		pos = 0;
	}

	save_data();
}

void WtDiffExecuter::on_tick(const char* stdCode, WTSTickData* newTick)
{
	checkTarget();
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	//unit->self()->on_tick(newTick);
	if (_pool)
	{
		newTick->retain();
		_pool->schedule([unit, newTick](){
			unit->self()->on_tick(newTick);
			newTick->release();
		});
	}
	else
	{
		unit->self()->on_tick(newTick);
	}
}

void WtDiffExecuter::on_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double price)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	if (localid == 0)
		return;

	//如果localid不为0，则更新差量
	double& curDiff = _diff_pos[stdCode];
	double prevDiff = curDiff;
	curDiff -= vol * (isBuy ? 1 : -1);

	WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Diff of {} updated by trade: {} -> {}", stdCode, prevDiff, curDiff);
	save_data();

	if (_pool)
	{
		std::string code = stdCode;
		_pool->schedule([localid, unit, code, isBuy, vol, price]() {
			unit->self()->on_trade(localid, code.c_str(), isBuy, vol, price);
		});
	}
	else
	{
		unit->self()->on_trade(localid, stdCode, isBuy, vol, price);
		}
}

void WtDiffExecuter::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	if (_pool)
	{
		std::string code = stdCode;
		_pool->schedule([localid, unit, code, isBuy, leftQty, price, isCanceled](){
			unit->self()->on_order(localid, code.c_str(), isBuy, leftQty, price, isCanceled);
		});
	}
	else
	{
		unit->self()->on_order(localid, stdCode, isBuy, leftQty, price, isCanceled);
	}
}

void WtDiffExecuter::on_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message)
{
	ExecuteUnitPtr unit = getUnit(stdCode, false);
	if (unit == NULL)
		return;

	if (_pool)
	{
		std::string code = stdCode;
		std::string msg = message;
		_pool->schedule([unit, localid, code, bSuccess, msg](){
			unit->self()->on_entrust(localid, code.c_str(), bSuccess, msg.c_str());
		});
	}
	else
	{
		unit->self()->on_entrust(localid, stdCode, bSuccess, message);
	}
}

void WtDiffExecuter::on_channel_ready()
{
	_channel_ready = true;
	SpinLock lock(_mtx_units);
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			if (_pool)
			{
				_pool->schedule([unitPtr](){
					unitPtr->self()->on_channel_ready();
				});
			}
			else
			{
				unitPtr->self()->on_channel_ready();
			}
		}
	}

	for(auto& v : _diff_pos)
	{
		const char* stdCode = v.first.c_str();
		ExecuteUnitPtr unit = getUnit(stdCode);
		if (unit == NULL)
			continue;
		double thisDiff = _diff_pos[stdCode];

		if (_pool)
		{
			std::string code = stdCode;
			_pool->schedule([unit, code, thisDiff]() {
				unit->self()->set_position(code.c_str(), thisDiff);
			});
		}
		else
		{
			unit->self()->set_position(stdCode, thisDiff);
		}

		WTSLogger::log_dyn("executer", _name.c_str(), LL_INFO, "Diff of {} recovered to {}", stdCode, thisDiff);
	}
}

void WtDiffExecuter::on_channel_lost()
{
	_channel_ready = false;
	SpinLock lock(_mtx_units);
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			if (_pool)
			{
				_pool->schedule([unitPtr](){
					unitPtr->self()->on_channel_lost();
				});
			}
			else
			{
				unitPtr->self()->on_channel_lost();
			}
		}
	}
}

void WtDiffExecuter::on_account(const char* currency, double prebalance, double balance, double dynbalance,
	double avaliable, double closeprofit, double dynprofit, double margin, double fee, double deposit, double withdraw)
{
	if (std::strcmp(currency, "CNY") == 0)
		_avaliable = avaliable;
	SpinLock lock(_mtx_units);
	for (auto it = _unit_map.begin(); it != _unit_map.end(); it++)
	{
		ExecuteUnitPtr& unitPtr = (ExecuteUnitPtr&)it->second;
		if (unitPtr)
		{
			if (_pool)
			{
				std::string strCur = currency;
				_pool->schedule([unitPtr, strCur, prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw]() {
					unitPtr->self()->on_account(strCur.c_str(), prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw);
				});
			}
			else
			{
				unitPtr->self()->on_account(currency, prebalance, balance, dynbalance, avaliable, closeprofit, dynprofit, margin, fee, deposit, withdraw);
			}
		}
	}
}

void WtDiffExecuter::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail, uint32_t tradingday)
{

}

#pragma endregion 外部接口