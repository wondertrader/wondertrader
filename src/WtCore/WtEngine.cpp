/*!
 * \file WtEngine.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "WtEngine.h"
#include "WtDtMgr.h"
#include "WtHelper.h"

#include "../Share/TimeUtils.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"

#include "../Includes/IBaseDataMgr.h"
#include "../Includes/IHotMgr.h"

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSRiskDef.hpp"

#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;


USING_NS_WTP;

WtEngine::WtEngine()
	: _port_fund(NULL)
	, _risk_volscale(1.0)
	, _risk_date(0)
	, _terminated(false)
	, _evt_listener(NULL)
	, _adapter_mgr(NULL)
	, _notifier(NULL)
	, _fund_udt_span(0)
	, _ready(false)
{
	TimeUtils::getDateTime(_cur_date, _cur_time);
	_cur_secs = _cur_time % 100000;
	_cur_time /= 100000;
	_cur_raw_time = _cur_time;
	_cur_tdate = _cur_date;

	WtHelper::setTime(_cur_date, _cur_time, _cur_secs);
}

void WtEngine::set_date_time(uint32_t curDate, uint32_t curTime, uint32_t curSecs /* = 0 */, uint32_t rawTime /* = 0 */)
{
	_cur_date = curDate;
	_cur_time = curTime;
	_cur_secs = curSecs;

	if (rawTime == 0)
		rawTime = curTime;

	_cur_raw_time = rawTime;

	WtHelper::setTime(_cur_date, _cur_raw_time, _cur_secs);
}

void WtEngine::set_trading_date(uint32_t curTDate)
{
	_cur_tdate = curTDate; 

	WtHelper::setTDate(curTDate);
}

WTSCommodityInfo* WtEngine::get_commodity_info(const char* stdCode)
{
	return _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
}

WTSContractInfo* WtEngine::get_contract_info(const char* stdCode)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	return _base_data_mgr->getContract(cInfo._code, cInfo._exchg);
}

std::string WtEngine::get_rawcode(const char* stdCode)
{
	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
	if (cInfo.hasRule())
	{
		std::string code = _hot_mgr->getCustomRawCode(cInfo._ruletag, cInfo.stdCommID());
		return CodeHelper::rawMonthCodeToStdCode(code.c_str(), cInfo._exchg);
	}

	return "";
}

WTSSessionInfo* WtEngine::get_session_info(const char* sid, bool isCode /* = false */)
{
	if (!isCode)
		return _base_data_mgr->getSession(sid);

	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(sid).c_str());
	if (cInfo == NULL)
		return NULL;

	return _base_data_mgr->getSession(cInfo->getSession());
}

void WtEngine::on_tick(const char* stdCode, WTSTickData* curTick)
{
	_price_map[stdCode] = curTick->price();

	//�ȼ���Ƿ�Ҫ�ź�Ҫ����
	{
		bool bTriggered = false;
		auto it = _sig_map.find(stdCode);
		if (it != _sig_map.end())
		{
			WTSSessionInfo* sInfo = get_session_info(stdCode, true);

			if (sInfo->isInTradingTime(_cur_raw_time, true))
			{
				const SigInfo& sInfo = it->second;
				double pos = sInfo._volume;
				std::string code = stdCode;
				do_set_position(code.c_str(), pos, curTick->price());
				_sig_map.erase(it);
				bTriggered = true;
			}

		}

		if(bTriggered)
			save_datas();
	}

	//����ɽ���Ϊ0���۸�Ҳ�����б䶯
	if (curTick->volume() == 0)
		return;

	std::string code = stdCode;
	double price = curTick->price();
	push_task([this, code, price]{
		auto it = _pos_map.find(code);
		if (it == _pos_map.end())
			return;

		PosInfo* pInfo = (PosInfo*)&it->second;
		if (pInfo->_volume == 0)
		{
			pInfo->_dynprofit = 0;
		}
		else
		{
			WTSCommodityInfo* commInfo = get_commodity_info(code.c_str());
			double dynprofit = 0;
			for (auto pit = pInfo->_details.begin(); pit != pInfo->_details.end(); pit++)
			{
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				dynprofit += dInfo._profit;
			}

			pInfo->_dynprofit = dynprofit;
		}
	});

	push_task([this]() {
		update_fund_dynprofit();
	});
}

void WtEngine::update_fund_dynprofit()
{
	WTSFundStruct& fundInfo = _port_fund->fundInfo();
	if (fundInfo._last_date == _cur_tdate)
	{
		//�ϴν������ڵ��ڵ�ǰ������,˵���Ѿ�����,���ٸ�����
		return;
	}

	int64_t now = TimeUtils::getLocalTimeNow();
	if(_fund_udt_span != 0)
	{
		if (now - fundInfo._update_time < _fund_udt_span * 1000)
			return;
	}

	double profit = 0.0;
	for(auto v : _pos_map)
	{
		const PosInfo& pItem = v.second;
		profit += pItem._dynprofit;
	}

	fundInfo._dynprofit = profit;
	double dynbal = fundInfo._balance + profit;
	if (fundInfo._max_dyn_bal == DBL_MAX || decimal::gt(dynbal, fundInfo._max_dyn_bal))
	{
		fundInfo._max_dyn_bal = dynbal;
		fundInfo._max_time = _cur_raw_time * 100000 + _cur_secs;
	}

	if (fundInfo._min_dyn_bal == DBL_MAX || decimal::lt(dynbal, fundInfo._min_dyn_bal))
	{
		fundInfo._min_dyn_bal = dynbal;
		fundInfo._min_time = _cur_raw_time * 100000 + _cur_secs;;
	}

	double dynbalance = fundInfo._balance + profit;
	if (fundInfo._max_md_dyn_bal._date == 0 || decimal::gt(dynbalance, fundInfo._max_md_dyn_bal._dyn_balance))
	{
		fundInfo._max_md_dyn_bal._dyn_balance = dynbalance;
		fundInfo._max_md_dyn_bal._date = _cur_tdate;
	}

	if (fundInfo._min_md_dyn_bal._date == 0 || decimal::lt(dynbalance, fundInfo._min_md_dyn_bal._dyn_balance))
	{
		fundInfo._min_md_dyn_bal._dyn_balance = dynbalance;
		fundInfo._min_md_dyn_bal._date = _cur_tdate;
	}

	fundInfo._update_time = now;
}

void WtEngine::writeRiskLog(const char* message)
{
	static thread_local char szBuf[2048] = { 0 };
	auto len = wt_strcpy(szBuf, "[RiskControl] ");
	wt_strcpy(szBuf + len, message);
	WTSLogger::log_raw_by_cat("risk", LL_INFO, szBuf);
}

uint32_t WtEngine::getCurDate()
{
	return _cur_date;
}

uint32_t WtEngine::getCurTime()
{
	return _cur_time;
}

uint32_t WtEngine::getTradingDate()
{
	return _cur_tdate;
}

bool WtEngine::isInTrading()
{
	return false;
}

void WtEngine::setVolScale(double scale)
{
	double oldScale = _risk_volscale;
	_risk_volscale = scale;
	_risk_date = _cur_tdate;

	WTSLogger::log_by_cat("risk", LL_INFO, "Position risk scale updated: {} - > {}", oldScale, scale);
	save_datas();
}

WTSPortFundInfo* WtEngine::getFundInfo()
{
	update_fund_dynprofit();
	save_datas();

	return _port_fund;
}

void WtEngine::init(WTSVariant* cfg, IBaseDataMgr* bdMgr, WtDtMgr* dataMgr, IHotMgr* hotMgr, EventNotifier* notifier)
{
	_base_data_mgr = bdMgr;
	_data_mgr = dataMgr;
	_hot_mgr = hotMgr;
	_notifier = notifier;

	WTSLogger::info("Platform running mode: Production");

	_filter_mgr.set_notifier(notifier);

	_filter_mgr.load_filters(cfg->getCString("filters"));

	load_fees(cfg->getCString("fees"));

	load_datas();

	init_outputs();

	WTSVariant* cfgRisk = cfg->get("riskmon");
	if(cfgRisk)
	{
		init_riskmon(cfgRisk);
	}
	else
	{
		//���û�����÷���̣߳�����Ҫ�Լ����¸���ӯ��
		//�Ѹ���ʱ��������Ϊ5s
		_fund_udt_span = 5;
		WTSLogger::log_raw(LL_WARN, "RiskMon is not configured, portfilio fund will be updated every 5s");
	}
}

void WtEngine::on_session_end()
{
	//�ʽ����
	WTSFundStruct& fundInfo = _port_fund->fundInfo();
	if (fundInfo._last_date < _cur_tdate)
	{
		std::string filename = WtHelper::getPortifolioDir();
		filename += "funds.csv";
		BoostFilePtr fund_log(new BoostFile());
		{
			bool isNewFile = !BoostFile::exists(filename.c_str());
			fund_log->create_or_open_file(filename.c_str());
			if (isNewFile)
			{
				fund_log->write_file("date,predynbalance,prebalance,balance,closeprofit,dynprofit,fee,maxdynbalance,maxtime,mindynbalance,mintime,mdmaxbalance,mdmaxdate,mdminbalance,mdmindate\n");
			}
			else
			{
				fund_log->seek_to_end();
			}
		}

		//�������ﻹ��Ҫдһ���ʽ��¼
		//date,predynbalance,prebalance,balance,closeprofit,dynprofit,fee,maxdynbalance,maxtime,mindynbalance,mintime,mdmaxbalance,mdmaxdate,mdminbalance,mdmindate
		fund_log->write_file(fmt::format("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}\n", 
			_cur_tdate, fundInfo._predynbal, fundInfo._prebalance, fundInfo._balance, 
			fundInfo._profit, fundInfo._dynprofit, fundInfo._fees, fundInfo._max_dyn_bal,
			fundInfo._max_time, fundInfo._min_dyn_bal, fundInfo._min_time,
			fundInfo._max_md_dyn_bal._dyn_balance, fundInfo._max_md_dyn_bal._date,
			fundInfo._min_md_dyn_bal._dyn_balance, fundInfo._min_md_dyn_bal._date));

		fundInfo._last_date = _cur_tdate;
		fundInfo._predynbal = fundInfo._balance + fundInfo._dynprofit;
		fundInfo._prebalance = fundInfo._balance;
		fundInfo._profit = 0;
		fundInfo._fees = 0;
		fundInfo._max_dyn_bal = DBL_MAX;
		fundInfo._min_dyn_bal = DBL_MAX;
		fundInfo._max_time = 0;
		fundInfo._min_time = 0;
	}
	save_datas();
}

void WtEngine::on_session_begin()
{

}

void WtEngine::save_datas()
{
	rj::Document root(rj::kObjectType);
	rj::Document::AllocatorType &allocator = root.GetAllocator();

	if (_port_fund != NULL)
	{//�����ʽ�����
		const WTSFundStruct& fundInfo = _port_fund->fundInfo();
		rj::Value jFund(rj::kObjectType);
		jFund.AddMember("predynbal", fundInfo._predynbal, allocator);
		jFund.AddMember("balance", fundInfo._balance, allocator);
		jFund.AddMember("prebalance", fundInfo._prebalance, allocator);
		jFund.AddMember("profit", fundInfo._profit, allocator);
		jFund.AddMember("dynprofit", fundInfo._dynprofit, allocator);
		jFund.AddMember("fees", fundInfo._fees, allocator);

		jFund.AddMember("max_dyn_bal", fundInfo._max_dyn_bal, allocator);
		jFund.AddMember("max_time", fundInfo._max_time, allocator);
		jFund.AddMember("min_dyn_bal", fundInfo._min_dyn_bal, allocator);
		jFund.AddMember("min_time", fundInfo._min_time, allocator);

		jFund.AddMember("last_date", fundInfo._last_date, allocator);
		jFund.AddMember("date", _cur_tdate, allocator);

		rj::Value jMaxMD(rj::kObjectType);
		jMaxMD.AddMember("date", fundInfo._max_md_dyn_bal._date, allocator);
		jMaxMD.AddMember("dyn_balance", fundInfo._max_md_dyn_bal._dyn_balance, allocator);

		rj::Value jMinMD(rj::kObjectType);
		jMinMD.AddMember("date", fundInfo._min_md_dyn_bal._date, allocator);
		jMinMD.AddMember("dyn_balance", fundInfo._min_md_dyn_bal._dyn_balance, allocator);

		jFund.AddMember("maxmd", jMaxMD, allocator);
		jFund.AddMember("minmd", jMinMD, allocator);

		jFund.AddMember("update_time", fundInfo._update_time, allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//�ֲ����ݱ���
		rj::Value jPos(rj::kArrayType);

		for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
		{
			const char* stdCode = it->first.c_str();
			const PosInfo& pInfo = it->second;

			rj::Value pItem(rj::kObjectType);
			pItem.AddMember("code", rj::Value(stdCode, allocator), allocator);
			pItem.AddMember("volume", pInfo._volume, allocator);
			pItem.AddMember("closeprofit", pInfo._closeprofit, allocator);
			pItem.AddMember("dynprofit", pInfo._dynprofit, allocator);

			rj::Value details(rj::kArrayType);
			for (auto dit = pInfo._details.begin(); dit != pInfo._details.end(); dit++)
			{
				const DetailInfo& dInfo = *dit;
				if(decimal::eq(dInfo._volume, 0))
					continue;
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("long", dInfo._long, allocator);
				dItem.AddMember("price", dInfo._price, allocator);
				dItem.AddMember("volume", dInfo._volume, allocator);
				dItem.AddMember("opentime", dInfo._opentime, allocator);
				dItem.AddMember("opentdate", dInfo._opentdate, allocator);

				dItem.AddMember("profit", dInfo._profit, allocator);

				details.PushBack(dItem, allocator);
			}

			pItem.AddMember("details", details, allocator);

			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	//��ز�������
	{
		rj::Value jRisk(rj::kObjectType);

		jRisk.AddMember("scale", _risk_volscale, allocator);
		jRisk.AddMember("date", _risk_date, allocator);

		root.AddMember("riskmon", jRisk, allocator);
	}

	{
		std::string filename = WtHelper::getPortifolioDir();
		filename += "datas.json";

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

void WtEngine::load_datas()
{
	_port_fund = WTSPortFundInfo::create();

	std::string filename = WtHelper::getPortifolioDir();
	filename += "datas.json";

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

	//��ȡ�ʽ�
	{
		const rj::Value& jFund = root["fund"];
		if (!jFund.IsNull() && jFund.IsObject())
		{
			WTSFundStruct& fundInfo = _port_fund->fundInfo();
			fundInfo._predynbal = jFund["predynbal"].GetDouble();
			fundInfo._balance = jFund["balance"].GetDouble();
			fundInfo._prebalance = jFund["prebalance"].GetDouble();
			fundInfo._profit = jFund["profit"].GetDouble();
			fundInfo._dynprofit = jFund["dynprofit"].GetDouble();
			fundInfo._fees = jFund["fees"].GetDouble();
			fundInfo._last_date = jFund["last_date"].GetUint();
			fundInfo._max_dyn_bal = jFund["max_dyn_bal"].GetDouble();
			fundInfo._max_time = jFund["max_time"].GetUint();
			fundInfo._min_dyn_bal = jFund["min_dyn_bal"].GetDouble();
			fundInfo._min_time = jFund["min_time"].GetUint();

			const rj::Value& jMaxMD = jFund["maxmd"];
			fundInfo._max_md_dyn_bal._dyn_balance = jMaxMD["dyn_balance"].GetDouble();
			fundInfo._max_md_dyn_bal._date = jMaxMD["date"].GetUint();

			const rj::Value& jMinMD = jFund["minmd"];
			fundInfo._min_md_dyn_bal._dyn_balance = jMinMD["dyn_balance"].GetDouble();
			fundInfo._min_md_dyn_bal._date = jMinMD["date"].GetUint();

			if(jFund.HasMember("update_time"))
			{
				fundInfo._update_time = jFund["update_time"].GetInt64();
			}
		}
	}

	{//��ȡ��λ
		double total_profit = 0;
		double total_dynprofit = 0;
		const rj::Value& jPos = root["positions"];
		if (!jPos.IsNull() && jPos.IsArray())
		{
			for (const rj::Value& pItem : jPos.GetArray())
			{
				const char* stdCode = pItem["code"].GetString();
				PosInfo& pInfo = _pos_map[stdCode];
				pInfo._closeprofit = pItem["closeprofit"].GetDouble();
				pInfo._volume = pItem["volume"].GetDouble();
				if (pInfo._volume == 0)
					pInfo._dynprofit = 0;
				else
					pInfo._dynprofit = pItem["dynprofit"].GetDouble();

				total_profit += pInfo._closeprofit;
				total_dynprofit += pInfo._dynprofit;

				const rj::Value& details = pItem["details"];
				if (details.IsNull() || !details.IsArray() || details.Size() == 0)
					continue;

				for (uint32_t i = 0; i < details.Size(); i++)
				{
					const rj::Value& dItem = details[i];
					DetailInfo dInfo;
					dInfo._long = dItem["long"].GetBool();
					dInfo._price = dItem["price"].GetDouble();
					dInfo._volume = dItem["volume"].GetDouble();
					dInfo._opentime = dItem["opentime"].GetUint64();
					if (dItem.HasMember("opentdate"))
						dInfo._opentdate = dItem["opentdate"].GetUint();

					dInfo._profit = dItem["profit"].GetDouble();
					pInfo._details.emplace_back(dInfo);
				}

				WTSLogger::debug("Porfolio position confirmed,{} -> {}", stdCode, pInfo._volume);
			}
		}

		WTSFundStruct& fundInfo = _port_fund->fundInfo();
		fundInfo._dynprofit = total_dynprofit;

		WTSLogger::debug("{} position info of portfolio loaded", _pos_map.size());
	}

	if(root.HasMember("riskmon"))
	{
		//��ȡ��ز���
		const rj::Value& jRisk = root["riskmon"];
		if (!jRisk.IsNull() && jRisk.IsObject())
		{
			_risk_date = jRisk["date"].GetUint();
			_risk_volscale = jRisk["scale"].GetDouble();
		}
	}
}

WTSTickSlice* WtEngine::get_tick_slice(uint32_t sid, const char* code, uint32_t count)
{
	return _data_mgr->get_tick_slice(code, count);
}

WTSTickData* WtEngine::get_last_tick(uint32_t sid, const char* stdCode)
{
	return _data_mgr->grab_last_tick(stdCode);
}

WTSKlineSlice* WtEngine::get_kline_slice(uint32_t sid, const char* stdCode, const char* period, uint32_t count, uint32_t times /* = 1 */, uint64_t etime /* = 0 */)
{
	WTSCommodityInfo* cInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());
	if (cInfo == NULL)
		return NULL;

	//WTSSessionInfo* sInfo = cInfo->getSessionInfo();

	//std::string key = StrUtil::printf("%s-%s-%u", stdCode, period, times);
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}-{}-{}", stdCode, period, times);

	SubList& sids = _bar_sub_map[key];
	sids[sid] = std::make_pair(sid, 0);

	WTSKlinePeriod kp;
	if (period[0] == 'm')
	{
		if (times % 5 == 0)
		{
			kp = KP_Minute5;
			times /= 5;
		}
		else
			kp = KP_Minute1;
	}
	else
	{
		kp = KP_DAY;
	}

	return _data_mgr->get_kline_slice(stdCode, kp, times, count, etime);
}


void WtEngine::handle_push_quote(WTSTickData* curTick, uint32_t hotFlag)
{
	std::string stdCode = curTick->code();
	_data_mgr->handle_push_quote(stdCode.c_str(), curTick);
	on_tick(stdCode.c_str(), curTick);

	double price = curTick->price();

	if(hotFlag == 1)
	{
		std::string hotCode = CodeHelper::stdCodeToStdHotCode(stdCode.c_str());
		WTSTickData* hotTick = WTSTickData::create(curTick->getTickStruct());
		hotTick->setCode(hotCode.c_str());
		
		_data_mgr->handle_push_quote(hotCode.c_str(), hotTick);
		on_tick(hotCode.c_str(), hotTick);

		hotTick->release();
	}
	else if (hotFlag == 2)
	{
		std::string scndCode = CodeHelper::stdCodeToStd2ndCode(stdCode.c_str());
		WTSTickData* scndTick = WTSTickData::create(curTick->getTickStruct());
		scndTick->setCode(scndCode.c_str());

		_data_mgr->handle_push_quote(scndCode.c_str(), scndTick);
		on_tick(scndCode.c_str(), scndTick);

		scndTick->release();
	}
}

double WtEngine::get_cur_price(const char* stdCode)
{
	auto len = strlen(stdCode);
	char lastChar = stdCode[len - 1];
	//ǰ��Ȩֱ�Ӷ�ȡ��׼��Լ����
	bool bAdjusted = (lastChar == SUFFIX_QFQ || lastChar == SUFFIX_HFQ);
	//ǰ��Ȩ��Ҫȥ��������Ȩ��δ��Ȩ��ֱ�Ӳ���
	std::string sCode = (lastChar == SUFFIX_QFQ) ? std::string(stdCode, len - 1) : stdCode;
	auto it = _price_map.find(sCode);
	if(it == _price_map.end())
	{
		//�Ҳ�����ʱ���ȶ�ȡδ��Ȩ��tick����
		std::string fCode = bAdjusted ? std::string(stdCode, len - 1) : stdCode;
		WTSTickData* lastTick = _data_mgr->grab_last_tick(fCode.c_str());
		if (lastTick == NULL)
			return 0.0;

		WTSContractInfo* cInfo = lastTick->getContractInfo();

		double ret = lastTick->price();
		lastTick->release();

		//����Ǻ�Ȩ������и�Ȩ����
		if (lastChar == SUFFIX_HFQ)
		{
			ret *= get_exright_factor(stdCode, cInfo->getCommInfo());
		}

		_price_map[sCode] = ret;
		return ret;
	}
	else
	{
		return it->second;
	}
}

double WtEngine::get_day_price(const char* stdCode, int flag /* = 0 */)
{
	auto len = strlen(stdCode);
	char lastChar = stdCode[len - 1];
	//ǰ��Ȩֱ�Ӷ�ȡ��׼��Լ����
	bool bAdjusted = (lastChar == SUFFIX_QFQ || lastChar == SUFFIX_HFQ);
	//ǰ��Ȩ��Ҫȥ��������Ȩ��δ��Ȩ��ֱ�Ӳ���
	std::string sCode = (lastChar == SUFFIX_QFQ) ? std::string(stdCode, len - 1) : stdCode;

	//�Ҳ�����ʱ���ȶ�ȡδ��Ȩ��tick����
	std::string fCode = bAdjusted ? std::string(stdCode, len - 1) : stdCode;
	WTSTickData* lastTick = _data_mgr->grab_last_tick(fCode.c_str());
	if (lastTick == NULL)
		return 0.0;

	WTSCommodityInfo* commInfo = get_commodity_info(fCode.c_str());

	double ret = 0.0;
	switch (flag)
	{
	case 0:
		ret = lastTick->open(); break;
	case 1:
		ret = lastTick->high(); break;
	case 2:
		ret = lastTick->low(); break;
	case 3:
		ret = lastTick->price(); break;
	default:
		break;
	}
	lastTick->release();

	//����Ǻ�Ȩ������и�Ȩ����
	if (lastChar == SUFFIX_HFQ)
	{
		ret *= get_exright_factor(stdCode, commInfo);
	}

	return ret;
}

double WtEngine::get_exright_factor(const char* stdCode, WTSCommodityInfo* commInfo /* = NULL */)
{
	if (commInfo == NULL)
		commInfo = get_commodity_info(stdCode);

	if (commInfo == NULL)
		return 1.0;

	if (commInfo->isStock())
		return _data_mgr->get_adjusting_factor(stdCode, get_trading_date());
	else
	{
		const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
		if(strlen(ruleTag) > 0)
			return _hot_mgr->getRuleFactor(ruleTag, commInfo->getFullPid(), get_trading_date());
	}

	return 1.0;
}

void WtEngine::sub_tick(uint32_t sid, const char* stdCode)
{
	//�����������Լ����, ��SHFE.ag.HOT, ��ôҪת����ԭ��Լ����, SHFE.ag.1912
	//��Ϊִ����ֻʶ��ԭ��Լ����
	const char* ruleTag = _hot_mgr->getRuleTag(stdCode);
	if(strlen(ruleTag) > 0)
	{
		//SubList& sids = _tick_sub_map[stdCode];
		//sids[sid] = std::make_pair(sid, 0);

		std::size_t length = strlen(stdCode);
		uint32_t flag = 0;
		if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
		{
			length--;

			flag = (stdCode[length - 1] == SUFFIX_QFQ) ? 1 : 2;
		}

		SubList& sids = _tick_sub_map[std::string(stdCode, length)];
		sids[sid] = std::make_pair(sid, flag);

		CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _hot_mgr);
		std::string rawCode = _hot_mgr->getCustomRawCode(ruleTag, cInfo.stdCommID(), _cur_tdate);
		std::string stdRawCode = CodeHelper::rawMonthCodeToStdCode(rawCode.c_str(), cInfo._exchg);
	}
	//if (CodeHelper::isStdFutHotCode(stdCode))
	//{
	//	SubList& sids = _tick_sub_map[stdCode];
	//	sids[sid] = std::make_pair(sid, 0);

	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string rawCode = _hot_mgr->getRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
	//	std::string stdRawCode = CodeHelper::rawMonthCodeToStdCode(rawCode.c_str(), cInfo._exchg);
	//	//_ticksubed_raw_codes.insert(stdRawCode);
	//}
	//else if (CodeHelper::isStdFut2ndCode(stdCode))
	//{
	//	SubList& sids = _tick_sub_map[stdCode];
	//	sids[sid] = std::make_pair(sid, 0);

	//	CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode);
	//	std::string rawCode = _hot_mgr->getSecondRawCode(cInfo._exchg, cInfo._product, _cur_tdate);
	//	std::string stdRawCode = CodeHelper::rawMonthCodeToStdCode(rawCode.c_str(), cInfo._exchg);
	//	//_ticksubed_raw_codes.insert(stdRawCode);
	//}
	else
	{
		std::size_t length = strlen(stdCode);
		uint32_t flag = 0;
		if (stdCode[length - 1] == SUFFIX_QFQ || stdCode[length - 1] == SUFFIX_HFQ)
		{
			length--;

			flag = (stdCode[length - 1] == SUFFIX_QFQ) ? 1 : 2;
		}

		SubList& sids = _tick_sub_map[std::string(stdCode, length)];
		sids[sid] = std::make_pair(sid, flag);

		//_ticksubed_raw_codes.insert(std::string(stdCode, length));
	}
}

void WtEngine::load_fees(const char* filename)
{
	if (strlen(filename) == 0)
		return;

	if (!StdFile::exists(filename))
	{
		WTSLogger::error("Fee templates file {} not exists", filename);
		return;
	}

	WTSVariant* cfg = WTSCfgLoader::load_from_file(filename, true);
	if (cfg == NULL)
	{
		WTSLogger::error("Fee templates file {} loading failed", filename);
		return;
	}

	auto keys = cfg->memberNames();
	for (const std::string& key : keys)
	{
		WTSVariant* cfgItem = cfg->get(key.c_str());
		FeeItem& fItem = _fee_map[key];
		fItem._by_volume = cfgItem->getBoolean("byvolume");
		fItem._open = cfgItem->getDouble("open");
		fItem._close = cfgItem->getDouble("close");
		fItem._close_today = cfgItem->getDouble("closetoday");
	}

	cfg->release();

	WTSLogger::info("{} fee templates loaded", _fee_map.size());
}

double WtEngine::calc_fee(const char* stdCode, double price, double qty, uint32_t offset)
{
	std::string stdPID = CodeHelper::stdCodeToStdCommID(stdCode);
	auto it = _fee_map.find(stdPID);
	if (it == _fee_map.end())
	{
		WTSLogger::warn("Fee template of {} not found, return 0.0 as default", stdCode);
		return 0.0;
	}

	double ret = 0.0;
	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(stdPID.c_str());
	const FeeItem& fItem = it->second;
	if(fItem._by_volume)
	{
		switch (offset)
		{
		case 0: ret = fItem._open*qty; break;
		case 1: ret = fItem._close*qty; break;
		case 2: ret = fItem._close_today*qty; break;
		default: ret = 0.0; break;
		}
	}
	else
	{
		double amount = price*qty*commInfo->getVolScale();
		switch (offset)
		{
		case 0: ret = fItem._open*amount; break;
		case 1: ret = fItem._close*amount; break;
		case 2: ret = fItem._close_today*amount; break;
		default: ret = 0.0; break;
		}
	}

	return (int32_t)(ret * 100 + 0.5) / 100.0;
}

void WtEngine::append_signal(const char* stdCode, double qty, bool bStandBy /* = true */)
{
	/*
	 *	By Wesley @ 2021.12.16
	 *	���﷢��һ�����⣬������ϵ����۳ɽ��ۺͲ��Ե����۳ɽ��۲�һ��
	 *	����Ժ��֣����Ե����۳ɽ��ۻ�����һ��tick����
	 *	������ϵ����۳ɽ�����һ��tick��ֱ�Ӹ�����
	 *	��͵�����ϳɽ�����Զ�Ȳ�����ǰһ��tick
	 *	������һ������������һ��tick����������signal
	 *	�����bar�ڴ����ģ�bStandByΪfalse����ֱ���޸ĳֲ�
	 */
	double curPx = get_cur_price(stdCode);
	if(bStandBy || decimal::eq(curPx, 0.0))
	{
		SigInfo& sInfo = _sig_map[stdCode];
		sInfo._volume = qty;
		sInfo._gentime = (uint64_t)_cur_date * 1000000000 + (uint64_t)_cur_raw_time * 100000 + _cur_secs;
	}
	else
	{
		do_set_position(stdCode, qty);
	}

	/*
	double curPx = get_cur_price(stdCode);
	if(decimal::eq(curPx, 0.0))
	{
		SigInfo& sInfo = _sig_map[stdCode];
		sInfo._volume = qty;
		sInfo._gentime = (uint64_t)_cur_date * 1000000000 + (uint64_t)_cur_raw_time * 100000 + _cur_secs;
	}
	else
	{
		do_set_position(stdCode, qty);
	}
	*/
}

void WtEngine::do_set_position(const char* stdCode, double qty, double curPx /* = -1 */)
{
	PosInfo& pInfo = _pos_map[stdCode];

	if(decimal::lt(curPx, 0))
		curPx = get_cur_price(stdCode);

	uint64_t curTm = (uint64_t)_cur_date * 10000 + _cur_time;
	uint32_t curTDate = _cur_tdate;

	if (decimal::eq(pInfo._volume, qty))
		return;

	double diff = qty - pInfo._volume;

	WTSCommodityInfo* commInfo = _base_data_mgr->getCommodity(CodeHelper::stdCodeToStdCommID(stdCode).c_str());

	WTSFundStruct& fundInfo = _port_fund->fundInfo();

	if (decimal::gt(pInfo._volume*diff, 0))//��ǰ�ֲֺ�Ŀ���λ����һ��, ����һ����ϸ, ������������
	{
		pInfo._volume = qty;

		DetailInfo dInfo;
		dInfo._long = decimal::gt(qty, 0);
		dInfo._price = curPx;
		dInfo._volume = abs(diff);
		dInfo._opentime = curTm;
		dInfo._opentdate = curTDate;
		pInfo._details.emplace_back(dInfo);

		double fee = calc_fee(stdCode, curPx, abs(qty), 0);
		fundInfo._fees += fee;
		fundInfo._balance -= fee;

		log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(diff), fee);
	}
	else
	{//�ֲַ����Ŀ���λ����һ��, ��Ҫƽ��
		double left = abs(diff);

		pInfo._volume = qty;
		if (decimal::eq(pInfo._volume, 0))
			pInfo._dynprofit = 0;
		uint32_t count = 0;
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			DetailInfo& dInfo = *it;
			if (decimal::eq(dInfo._volume, 0))
			{
				count++;
				continue;
			}

			double maxQty = min(dInfo._volume, left);
			if (decimal::eq(maxQty, 0))
				continue;

			dInfo._volume -= maxQty;
			left -= maxQty;

			//if (dInfo._volume == 0)
			if (decimal::eq(dInfo._volume, 0))
				count++;

			double profit = (curPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//��ӯҲҪ���ȱ�����
			fundInfo._profit += profit;
			fundInfo._balance += profit;

			double fee = calc_fee(stdCode, curPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			fundInfo._fees += fee;
			fundInfo._balance -= fee;

			//����д�ɽ���¼
			log_trade(stdCode, dInfo._long, false, curTm, curPx, maxQty, fee);
			//����дƽ�ּ�¼
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, curPx, maxQty, profit, pInfo._closeprofit);

			if (left == 0)
				break;
		}

		//��Ҫ������Ѿ�ƽ�������ϸ
		while (count > 0)
		{
			auto it = pInfo._details.begin();
			pInfo._details.erase(it);
			count--;
		}

		//���, �������ʣ���, ����Ҫ������
		//if (left > 0)
		if(decimal::gt(left, 0))
		{
			left = left * qty / abs(qty);

			DetailInfo dInfo;
			dInfo._long = qty > 0;
			dInfo._price = curPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			pInfo._details.emplace_back(dInfo);

			//���ﻹ��Ҫдһ�ʳɽ���¼
			double fee = calc_fee(stdCode, curPx, abs(qty), 0);
			fundInfo._fees += fee;
			fundInfo._balance -= fee;

			log_trade(stdCode, dInfo._long, true, curTm, curPx, abs(left), fee);
		}
	}
}

void WtEngine::push_task(TaskItem task)
{
	{
		StdUniqueLock lock(_mtx_task);
		_task_queue.push(task);
	}
	

	if (_thrd_task == NULL)
	{
		_thrd_task.reset(new StdThread([this]{
			task_loop();
		}));
	}

	_cond_task.notify_all();
}

void WtEngine::task_loop()
{
	while (!_terminated)
	{
		TaskQueue temp;
		{
			StdUniqueLock lock(_mtx_task);
			if(_task_queue.empty())
			{
				_cond_task.wait(_mtx_task);
				continue;
			}

			temp.swap(_task_queue);
		}

		for (;;)
		{
			if(temp.empty())
				break;

			TaskItem& item = temp.front();
			item();
			temp.pop();
		}
	}
}

bool WtEngine::init_riskmon(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	if (!cfg->getBoolean("active"))
		return false;

	std::string module = DLLHelper::wrap_module(cfg->getCString("module"));
	//�ȿ�����Ŀ¼���Ƿ��ж�Ӧģ��
	std::string dllpath = WtHelper::getCWD() + module;
	//���û��,���ٿ�ģ��Ŀ¼,��dllͬĿ¼��
	if (!StdFile::exists(dllpath.c_str()))
		dllpath = WtHelper::getInstDir() + module;

	DllHandle hInst = DLLHelper::load_library(dllpath.c_str());
	if (hInst == NULL)
	{
		WTSLogger::log_by_cat("risk", LL_ERROR, "Riskmon module {} loading failed", dllpath.c_str());
		return false;
	}

	FuncCreateRiskMonFact creator = (FuncCreateRiskMonFact)DLLHelper::get_symbol(hInst, "createRiskMonFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		WTSLogger::log_by_cat("risk", LL_ERROR, "Riskmon module {} is not compatible", module.c_str());
		return false;
	}

	_risk_fact._module_inst = hInst;
	_risk_fact._module_path = module;
	_risk_fact._creator = creator;
	_risk_fact._remover = (FuncDeleteRiskMonFact)DLLHelper::get_symbol(hInst, "deleteRiskMonFact");
	_risk_fact._fact = _risk_fact._creator();

	const char* name = cfg->getCString("name");
	
	_risk_mon.reset(new WtRiskMonWrapper(_risk_fact._fact->createRiskMonotor(name), _risk_fact._fact));
	_risk_mon->self()->init(this, cfg);

	return true;
}

void WtEngine::init_outputs()
{
	std::string folder = WtHelper::getPortifolioDir();
	std::string filename = folder + "trades.csv";
	_trade_logs.reset(new BoostFile());
	{
		bool isNewFile = !BoostFile::exists(filename.c_str());
		_trade_logs->create_or_open_file(filename.c_str());
		if (isNewFile)
		{
			_trade_logs->write_file("code,time,direct,action,price,qty,fee\n");
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
			_close_logs->write_file("code,direct,opentime,openprice,closetime,closeprice,qty,profit,totalprofit\n");
		}
		else
		{
			_close_logs->seek_to_end();
		}
	}
}

void WtEngine::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, double fee /* = 0.0 */)
{
	if (_trade_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") << "," << price << "," << qty << "," << fee << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void WtEngine::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double totalprofit /* = 0 */)
{
	if (_close_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << ","
			<< totalprofit << "\n";
		_close_logs->write_file(ss.str());
	}
}