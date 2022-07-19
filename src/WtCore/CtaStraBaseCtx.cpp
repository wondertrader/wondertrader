/*!
 * \file CtaStraBaseCtx.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaStraBaseCtx.h"
#include "WtCtaEngine.h"
#include "WtHelper.h"

#include <exception>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/IHotMgr.h"
#include "../Includes/WTSTradeDef.hpp"
#include "../Share/decimal.h"
#include "../Share/CodeHelper.hpp"

#include "../WTSTools/WTSLogger.h"

const char* CMP_ALG_NAMES[] =
{
	"��",
	">",
	"<",
	">=",
	"<="
};

const char* ACTION_NAMES[] =
{
	"OL",
	"CL",
	"OS",
	"CS",
	"SYN"
};


inline uint32_t makeCtaCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 1 };
	return _auto_context_id.fetch_add(1);
}


CtaStraBaseCtx::CtaStraBaseCtx(WtCtaEngine* engine, const char* name, int32_t slippage)
	: ICtaStraCtx(name)
	, _engine(engine)
	, _total_calc_time(0)
	, _emit_times(0)
	, _last_cond_min(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
	, _last_barno(0)
	, _slippage(slippage)
{
	_context_id = makeCtaCtxId();
}


CtaStraBaseCtx::~CtaStraBaseCtx()
{
}

void CtaStraBaseCtx::init_outputs()
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
			_trade_logs->write_file("code,time,direct,action,price,qty,tag,fee,barno\n");
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
			_close_logs->write_file("code,direct,opentime,openprice,closetime,closeprice,qty,profit,totalprofit,entertag,exittag,openbarno,closebarno\n");
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

void CtaStraBaseCtx::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	if (_sig_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
		_sig_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag /* = "" */, double fee /* = 0.0 */, uint32_t barNo /* = 0 */)
{
	if (_trade_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") << "," << price << "," << qty << "," << userTag << "," << fee << "," << barNo << "\n";
		_trade_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double totalprofit /* = 0 */, 
	const char* enterTag /* = "" */, const char* exitTag /* = "" */, uint32_t openBarNo /* = 0 */, uint32_t closeBarNo /* = 0 */)
{
	if (_close_logs)
	{
		std::stringstream ss;
		ss << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
			<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," 
			<< totalprofit << "," << enterTag << "," << exitTag << "," << openBarNo << "," << closeBarNo << "\n";
		_close_logs->write_file(ss.str());
	}
}

void CtaStraBaseCtx::save_userdata()
{
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

void CtaStraBaseCtx::load_userdata()
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

void CtaStraBaseCtx::load_data(uint32_t flag /* = 0xFFFFFFFF */)
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

	if(root.HasMember("fund"))
	{
		//��ȡ�ʽ�
		const rj::Value& jFund = root["fund"];
		if(!jFund.IsNull() && jFund.IsObject())
		{
			_fund_info._total_profit = jFund["total_profit"].GetDouble();
			_fund_info._total_dynprofit = jFund["total_dynprofit"].GetDouble();
			uint32_t tdate = jFund["tdate"].GetUint();
			_fund_info._total_fees = jFund["total_fees"].GetDouble();
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
				const char* ruleTag = _engine->get_hot_mgr()->getRuleTag(stdCode);
				if (strlen(ruleTag) == 0 && _engine->get_contract_info(stdCode) == NULL)
				{
					log_info("{} not exists or expired, position ignored", stdCode);
					continue;
				}
				PosInfo& pInfo = _pos_map[stdCode];
				pInfo._closeprofit = pItem["closeprofit"].GetDouble();
				pInfo._volume = pItem["volume"].GetDouble();
				pInfo._last_entertime = pItem["lastentertime"].GetUint64();
				pInfo._last_exittime = pItem["lastexittime"].GetUint64();
				if (pItem.HasMember("frozen"))
				{
					pInfo._frozen = pItem["frozen"].GetDouble();
					pInfo._frozen_date = pItem["frozendate"].GetUint();
				}

				if (pInfo._volume == 0)
				{
					pInfo._dynprofit = 0;
					pInfo._frozen = 0;
				}
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
					double vol = dItem["volume"].GetDouble();
					if(decimal::eq(vol, 0))
						continue;

					DetailInfo dInfo;
					dInfo._long = dItem["long"].GetBool();
					dInfo._price = dItem["price"].GetDouble();
					dInfo._volume = dItem["volume"].GetDouble();
					dInfo._opentime = dItem["opentime"].GetUint64();
					if(dItem.HasMember("opentdate"))
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
					if (dItem.HasMember("openbarno"))
						dInfo._open_barno = dItem["openbarno"].GetUint();
					else
						dInfo._open_barno = 0;

					pInfo._details.emplace_back(dInfo);
				}

				log_info("Position confirmed,{} -> {}", stdCode, pInfo._volume);
				stra_sub_ticks(stdCode);
			}
		}

		_fund_info._total_profit = total_profit;
		_fund_info._total_dynprofit = total_dynprofit;
	}

	{//��ȡ������
		uint32_t count = 0;
		const rj::Value& jCond = root["conditions"];
		if (!jCond.IsNull() && jCond.IsObject())
		{
			_last_cond_min = jCond["settime"].GetUint64();
			const rj::Value& jItems = jCond["items"];
			for (auto& m : jItems.GetObject())
			{
				const char* stdCode = m.name.GetString();
				const char* ruleTag = _engine->get_hot_mgr()->getRuleTag(stdCode);
				if (strlen(ruleTag) == 0 && _engine->get_contract_info(stdCode) == NULL)
				{
					log_info("{} not exists or expired, condition ignored", stdCode);
					continue;
				}

				const rj::Value& cListItem = m.value;

				CondList& condList = _condtions[stdCode];

				for(auto& cItem : cListItem.GetArray())
				{
					CondEntrust condInfo;
					strcpy(condInfo._code, stdCode);
					strcpy(condInfo._usertag, cItem["usertag"].GetString());

					condInfo._field = (WTSCompareField)cItem["field"].GetUint();
					condInfo._alg = (WTSCompareType)cItem["alg"].GetUint();
					condInfo._target = cItem["target"].GetDouble();
					condInfo._qty = cItem["qty"].GetDouble();
					condInfo._action = (char)cItem["action"].GetUint();

					condList.emplace_back(condInfo);

					log_info("{} condition recovered, {} {}, condition: newprice {} {}",
						stdCode, ACTION_NAMES[condInfo._action], condInfo._qty, CMP_ALG_NAMES[condInfo._alg], condInfo._target);
					count++;
				}
			}

			log_info("{} conditions recovered, setup time: {}", count, _last_cond_min);
		}
	}

	if (root.HasMember("signals"))
	{
		//��ȡ�ź�
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

	if (root.HasMember("utils"))
	{
		//��ȡ����
		const rj::Value& jUtils = root["utils"];
		if (!jUtils.IsNull() && jUtils.IsObject())
		{
			_last_barno = jUtils["lastbarno"].GetUint();
		}
	}
}

void CtaStraBaseCtx::save_data(uint32_t flag /* = 0xFFFFFFFF */)
{
	rj::Document root(rj::kObjectType);

	{//�ֲ����ݱ���
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
				dItem.AddMember("openbarno", dInfo._open_barno, allocator);

				details.PushBack(dItem, allocator);
			}

			pItem.AddMember("details", details, allocator);

			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	{//�ʽ𱣴�
		rj::Value jFund(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		jFund.AddMember("total_profit", _fund_info._total_profit, allocator);
		jFund.AddMember("total_dynprofit", _fund_info._total_dynprofit, allocator);
		jFund.AddMember("total_fees", _fund_info._total_fees, allocator);
		jFund.AddMember("tdate", _engine->get_trading_date(), allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//�źű���
		rj::Value jSigs(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto& m:_sig_map)
		{
			const char* stdCode = m.first.c_str();
			const SigInfo& sInfo = m.second;

			rj::Value jItem(rj::kObjectType);
			jItem.AddMember("usertag", rj::Value(sInfo._usertag.c_str(), allocator), allocator);

			jItem.AddMember("volume", sInfo._volume, allocator);
			jItem.AddMember("sigprice", sInfo._sigprice, allocator);
			jItem.AddMember("gentime", sInfo._gentime, allocator);

			jSigs.AddMember(rj::Value(stdCode, allocator), jItem, allocator);
		}

		root.AddMember("signals", jSigs, allocator);
	}

	{//����������
		rj::Value jCond(rj::kObjectType);
		rj::Value jItems(rj::kObjectType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _condtions.begin(); it != _condtions.end(); it++)
		{
			const char* code = it->first.c_str();
			const CondList& condList = it->second;

			rj::Value cArray(rj::kArrayType);
			for(auto& condInfo : condList)
			{
				rj::Value cItem(rj::kObjectType);
				cItem.AddMember("code", rj::Value(code, allocator), allocator);
				cItem.AddMember("usertag", rj::Value(condInfo._usertag, allocator), allocator);

				cItem.AddMember("field", (uint32_t)condInfo._field, allocator);
				cItem.AddMember("alg", (uint32_t)condInfo._alg, allocator);
				cItem.AddMember("target", condInfo._target, allocator);
				cItem.AddMember("qty", condInfo._qty, allocator);
				cItem.AddMember("action", (uint32_t)condInfo._action, allocator);

				cArray.PushBack(cItem, allocator);
			}

			jItems.AddMember(rj::Value(code, allocator), cArray, allocator);
		}
		jCond.AddMember("settime", _last_cond_min, allocator);
		jCond.AddMember("items", jItems, allocator);

		root.AddMember("conditions", jCond, allocator);
	}

	{//�����
		rj::Value jUtils(rj::kObjectType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		jUtils.AddMember("lastbarno", _last_barno, allocator);

		root.AddMember("utils", jUtils, allocator);
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
//�ص�����
void CtaStraBaseCtx::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
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

	if(key == _main_key)
		log_debug("Main KBars {} closed", key);
}

void CtaStraBaseCtx::on_init()
{
	init_outputs();

	//��ȡ����
	load_data();

	//�����û�����
	load_userdata();
}

void CtaStraBaseCtx::update_dyn_profit(const char* stdCode, double price)
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
	for(auto& v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		total_dynprofit += pInfo._dynprofit;
	}

	_fund_info._total_dynprofit = total_dynprofit;
}

void CtaStraBaseCtx::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	_price_map[stdCode] = newTick->price();

	//�ȼ���Ƿ�Ҫ�ź�Ҫ����
	{
		auto it = _sig_map.find(stdCode);
		if(it != _sig_map.end())
		{
			WTSSessionInfo* sInfo = _engine->get_session_info(stdCode, true);

			if (sInfo->isInTradingTime(_engine->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				do_set_position(stdCode, sInfo._volume, sInfo._usertag.c_str(), sInfo._triggered);

				//�������������������ص�on_condition_triggered
				if(sInfo._triggered)
					on_condition_triggered(stdCode, sInfo._volume, newTick->price(), sInfo._usertag.c_str());

				_sig_map.erase(it);
			}
			
		}
	}

	//���¸���ӯ��
	update_dyn_profit(stdCode, newTick->price());

	//////////////////////////////////////////////////////////////////////////
	//���������
	if(!_condtions.empty())
	{
		auto it = _condtions.find(stdCode);
		if (it == _condtions.end())
			return;

		const CondList& condList = it->second;
		for (const CondEntrust& entrust : condList)
		{
			double curPrice = newTick->price();

			bool isMatched = false;
			switch (entrust._alg)
			{
			case WCT_Equal:
				isMatched = decimal::eq(curPrice, entrust._target);
				break;
			case WCT_Larger:
				isMatched = decimal::gt(curPrice, entrust._target);
				break;
			case WCT_LargerOrEqual:
				isMatched = decimal::ge(curPrice, entrust._target);
				break;
			case WCT_Smaller:
				isMatched = decimal::lt(curPrice, entrust._target);
				break;
			case WCT_SmallerOrEqual:
				isMatched = decimal::le(curPrice, entrust._target);
				break;
			default:
				break;
			}

			if (isMatched)
			{
				log_info("Condition triggered[newprice {}{} targetprice {}], instrument: {}, {} {}",
					curPrice, CMP_ALG_NAMES[entrust._alg], entrust._target, stdCode, ACTION_NAMES[entrust._action], entrust._qty);

				switch (entrust._action)
				{
				case COND_ACTION_OL:
				{
					double curQty = stra_get_position(stdCode);
					double desQty = 0;
					if (decimal::lt(curQty, 0))
						desQty = entrust._qty;
					else
						desQty = curQty + entrust._qty;

					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_CL:
				{
					double curQty = stra_get_position(stdCode);
					if (decimal::gt(curQty, 0))
					{
						double maxQty = min(curQty, entrust._qty);
						double desQty = curQty - maxQty;
						append_signal(stdCode, desQty, entrust._usertag);
					}
				}
				break;
				case COND_ACTION_OS:
				{
					double curQty = stra_get_position(stdCode);
					double desQty = 0;
					if (decimal::gt(curQty, 0))
						desQty = -entrust._qty;
					else
						desQty = curQty - entrust._qty;

					append_signal(stdCode, desQty, entrust._usertag);
				}
				break;
				case COND_ACTION_CS:
				{
					double curQty = stra_get_position(stdCode);
					if (decimal::lt(curQty, 0))
					{
						double maxQty = min(abs(curQty), entrust._qty);
						double desQty = curQty + maxQty;
						append_signal(stdCode, desQty, entrust._usertag);
					}
				}
				break;
				case COND_ACTION_SP: 
				{
					append_signal(stdCode, entrust._qty, entrust._usertag);
				}
				break;
				default: break;
				}

				//ͬһ��bar�������ͬһ����Լ��������, ֻ���ܴ���һ��
				//��������ֱ�����������
				_condtions.erase(it);
				break;
			}
		}
	}

	if (bEmitStrategy)
		on_tick_updated(stdCode, newTick);

	if(_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

bool CtaStraBaseCtx::on_schedule(uint32_t curDate, uint32_t curTime)
{
	_is_in_schedule = true;//��ʼ����, �޸ı��

	//��Ҫ���ڱ��渡��ӯ����
	save_data();

	bool isMainUdt = false;
	bool emmited = false;

	for (auto it = _kline_tags.begin(); it != _kline_tags.end(); it++)
	{
		const char* key = it->first.c_str();
		KlineTag& marker = (KlineTag&)it->second;

		auto idx = StrUtil::findFirst(key, '#');

		std::string stdCode(key, idx);

		if (key == _main_key)
		{
			if (marker._closed)
			{
				isMainUdt = true;
				marker._closed = false;
			}
			else
			{
				isMainUdt = false;
				break;
			}
		}

		WTSSessionInfo* sInfo = _engine->get_session_info(stdCode.c_str(), true);

		if (isMainUdt || _kline_tags.empty())
		{	
			TimeUtils::Ticker ticker;

			uint32_t offTime = sInfo->offsetTime(curTime, true);
			if(offTime <= sInfo->getCloseTime(true))
			{
				_condtions.clear();
				on_calculate(curDate, curTime);
				log_debug("Strategy {} scheduled @ {}", _name, curTime);
				emmited = true;

				_emit_times++;
				_total_calc_time += ticker.micro_seconds();

				if (_emit_times % 20 == 0)
				{
					log_info("Strategy has been scheduled {} times, totally taking {} us, {:.3f} us each time",
						_emit_times, _total_calc_time, _total_calc_time*1.0 / _emit_times);
				}

				if (_ud_modified)
				{
					save_userdata();
					_ud_modified = false;
				}

				if(!_condtions.empty())
				{
					_last_cond_min = (uint64_t)curDate * 10000 + curTime;
					save_data();
				}
			}
			else
			{
				log_info("{} not in trading time, schedule canceled", curTime);
			}
			break;
		}
	}

	_is_in_schedule = false;//���Ƚ���, �޸ı��
	_last_barno++;	//ÿ�μ��㣬barno��1
	return emmited;
}

void CtaStraBaseCtx::on_session_begin(uint32_t uTDate)
{
	//ÿ�������տ�ʼ��Ҫ�Ѷ���ֲ�����
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosInfo& pInfo = (PosInfo&)it.second;
		if(pInfo._frozen_date!=0 && pInfo._frozen_date < uTDate && !decimal::eq(pInfo._frozen, 0))
		{
			log_debug("{} of %s frozen on {} released on {}", pInfo._frozen, stdCode, pInfo._frozen_date, uTDate);

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

void CtaStraBaseCtx::enum_position(FuncEnumCtaPosCallBack cb)
{
	std::unordered_map<std::string, double> desPos;
	for (auto& it:_pos_map)
	{
		const char* stdCode = it.first.c_str();
		const PosInfo& pInfo = it.second;
		//cb(stdCode, pInfo._volume);
		desPos[stdCode] = pInfo._volume;
	}

	for (auto sit:_sig_map)
	{
		const char* stdCode = sit.first.c_str();
		const SigInfo& sInfo = sit.second;
		desPos[stdCode] = sInfo._volume;
	}

	for(auto v:desPos)
	{
		cb(v.first.c_str(), v.second);
	}
}

void CtaStraBaseCtx::on_session_end(uint32_t uTDate)
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

		if(_pos_logs)
			_pos_logs->write_file(fmt::format("{},{},{},{:.2f},{:.2f}\n", curDate, stdCode,
				pInfo._volume, pInfo._closeprofit, pInfo._dynprofit));
	}

	//����Ҫ�ѵ��ս��������д����־�ļ���
	//��������ز��ʵ��д����ͬ, ������, ��������
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

CondList& CtaStraBaseCtx::get_cond_entrusts(const char* stdCode)
{
	CondList& ce = _condtions[stdCode];
	return ce;
}

//////////////////////////////////////////////////////////////////////////
//���Խӿ�
void CtaStraBaseCtx::stra_enter_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	_engine->sub_tick(id(), stdCode);
	
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ, ��ֱ�Ӵ���
	{
		double curQty = stra_get_position(stdCode);
		if (decimal::lt(curQty, 0))
		{
			//��ǰ�ֲ�С��0,�߼��Ƿ��ֵ�qty,���������ź�Ŀ���λΪqty
			append_signal(stdCode, qty, userTag);
		}
		else
		{
			//��ǰ�ֲִ��ڵ���0,��Ҫ���Ӷ��qty
			append_signal(stdCode, curQty + qty, userTag);
		}
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		wt_strcpy(entrust._code, stdCode);
		wt_strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if(!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		
		entrust._action = COND_ACTION_OL;

		condList.emplace_back(entrust);
	}
}

void CtaStraBaseCtx::stra_enter_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	if (!commInfo->canShort())
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	_engine->sub_tick(id(), stdCode);
	
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ, ��ֱ�Ӵ���
	{
		double curQty = stra_get_position(stdCode);
		if (decimal::gt(curQty, 0))
		{
			//��ǰ��λ����0,�߼��Ƿ��ֵ�qty��,���������ź�Ŀ���λΪ-qty��
			append_signal(stdCode, -qty, userTag);
		}
		else
		{
			//��ǰ��λС�ڵ���0,����׷�ӿշ�����
			append_signal(stdCode, curQty - qty, userTag);
		}
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		wt_strcpy(entrust._code, stdCode);
		wt_strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_OS;

		condList.emplace_back(entrust);
	}
}

void CtaStraBaseCtx::stra_exit_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	//��ȡ��ƽ�ֲ�
	double curQty = stra_get_position(stdCode, true);
	if (decimal::le(curQty, 0))
		return;
	
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ, ��ֱ�Ӵ���
	{
		double maxQty = min(curQty, qty);
		append_signal(stdCode, curQty - maxQty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		wt_strcpy(entrust._code, stdCode);
		wt_strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_CL;

		condList.emplace_back(entrust);
	}
}

void CtaStraBaseCtx::stra_exit_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _engine->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	if (!commInfo->canShort())
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	double curQty = stra_get_position(stdCode);
	//����ֲ��Ƕ�,����Ҫִ���˳���ͷ���߼���
	if (decimal::ge(curQty, 0))
		return;
	
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ, ��ֱ�Ӵ���
	{
		double maxQty = min(abs(curQty), qty);
		append_signal(stdCode, curQty + maxQty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		wt_strcpy(entrust._code, stdCode);
		wt_strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = WCT_SmallerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = WCT_LargerOrEqual;
		}

		entrust._action = COND_ACTION_CS;
		
		condList.emplace_back(entrust);
	}
}

double CtaStraBaseCtx::stra_get_price(const char* stdCode)
{
	auto it = _price_map.find(stdCode);
	if (it != _price_map.end())
		return it->second;

	if (_engine)
		return _engine->get_cur_price(stdCode);
	
	return 0.0;
}

double CtaStraBaseCtx::stra_get_day_price(const char* stdCode, int flag /* = 0 */)
{
	if (_engine)
		return _engine->get_day_price(stdCode, flag);

	return 0.0;
}

void CtaStraBaseCtx::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice /* = 0.0 */, double stopprice /* = 0.0 */)
{
	_engine->sub_tick(id(), stdCode);

	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//������Ƕ�̬�µ�ģʽ, ��ֱ�Ӵ���
	{
		append_signal(stdCode, qty, userTag);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		double curVol = stra_get_position(stdCode);
		//���Ŀ���λ�͵�ǰ��λ��һ�µģ���������������
		if (decimal::eq(curVol, qty))
			return;

		//����Ŀ���λ�͵�ǰ��λ,�ж���������
		bool isBuy = decimal::gt(qty, curVol);

		CondEntrust entrust;
		wt_strcpy(entrust._code, stdCode);
		wt_strcpy(entrust._usertag, userTag);

		entrust._qty = qty;
		entrust._field = WCF_NEWPRICE;
		if (!decimal::eq(limitprice))
		{
			entrust._target = limitprice;
			entrust._alg = isBuy ? WCT_SmallerOrEqual : WCT_LargerOrEqual;
		}
		else if (!decimal::eq(stopprice))
		{
			entrust._target = stopprice;
			entrust._alg = isBuy ? WCT_LargerOrEqual : WCT_SmallerOrEqual;
		}

		entrust._action = COND_ACTION_SP;

		condList.emplace_back(entrust);
	}
}

void CtaStraBaseCtx::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */)
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

void CtaStraBaseCtx::do_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, bool bTriggered /* = false */)
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

	//�ɽ���
	double trdPx = curPx;

	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))
	{//��ǰ�ֲֺͲ�λ�仯����һ��, ����һ����ϸ, ������������
		pInfo._volume = qty;

		//���T+1���򶳽��λҪ����
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
		dInfo._open_barno = _last_barno;
		wt_strcpy(dInfo._opentag, userTag);
		pInfo._details.emplace_back(dInfo);
		pInfo._last_entertime = curTm;

		double fee = _engine->calc_fee(stdCode, trdPx, abs(diff), 0);
		_fund_info._total_fees += fee;
		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(diff), userTag, fee, _last_barno);
	}
	else
	{//�ֲַ���Ͳ�λ�仯����һ��, ��Ҫƽ��
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

			if (decimal::eq(dInfo._volume, 0))
				count++;

			//����ƽ��ӯ��
			double profit = (trdPx - dInfo._price) * maxQty * commInfo->getVolScale();
			if (!dInfo._long)
				profit *= -1;
			pInfo._closeprofit += profit;

			//��ӯҲҪ���ȱ�����
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);
			pInfo._last_exittime = curTm;
			_fund_info._total_profit += profit;

			//����������
			double fee = _engine->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//����д�ɽ���¼
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, userTag, fee, _last_barno);
			//����дƽ�ּ�¼
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, pInfo._closeprofit, dInfo._opentag, userTag, dInfo._open_barno, _last_barno);

			if (decimal::eq(left,0))
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
		if (decimal::gt(left, 0))
		{
			left = left * qty / abs(qty);

			//���T+1���򶳽��λҪ����
			if (commInfo->isT1())
			{
				pInfo._frozen += left;
				pInfo._frozen_date = curTDate;
				log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
			}

			DetailInfo dInfo;
			dInfo._long = decimal::gt(qty, 0);
			dInfo._price = trdPx;
			dInfo._max_price = trdPx;
			dInfo._min_price = trdPx;
			dInfo._volume = abs(left);
			dInfo._opentime = curTm;
			dInfo._opentdate = curTDate;
			dInfo._open_barno = _last_barno;
			wt_strcpy(dInfo._opentag, userTag);
			pInfo._details.emplace_back(dInfo);
			pInfo._last_entertime = curTm;

			//���ﻹ��Ҫдһ�ʳɽ���¼
			double fee = _engine->calc_fee(stdCode, trdPx, abs(left), 0);
			_fund_info._total_fees += fee;
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), userTag, fee, _last_barno);
		}
	}


	//�洢����
	save_data();

	if (bTriggered)	//���������������, ���������ύ�仯��
	{
		_engine->handle_pos_change(_name.c_str(), stdCode, diff);
	}
}

WTSKlineSlice* CtaStraBaseCtx::stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain /* = false */)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);

	if (isMain)
	{
		if (_main_key.empty())
		{
			_main_key = key;
			log_debug("Main KBars confirmed: {}", key);
		}
		else if (_main_key != key)
			throw std::runtime_error("Main KBars already confirmed");
	}

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);

	WTSKlineSlice* kline = _engine->get_kline_slice(_context_id, stdCode, basePeriod, count, times);
	if(kline)
	{
		//���K�߻�ȡ����,˵��Ҳ�����бպ��¼�����,���Բ����±��ر��
		bool isFirst = (_kline_tags.find(key) == _kline_tags.end());	//���û�б�����,˵���ǵ�һ����ȡ��K��
		KlineTag& tag = _kline_tags[key];
		tag._closed = false;

		double lastClose = kline->at(-1)->close;
		_price_map[stdCode] = lastClose;

		if(isMain && isFirst && !_condtions.empty())
		{
			//����ǵ�һ����ȡ��K��,��������������ʱ��
			bool isDay = basePeriod[0] == 'd';
			uint64_t lastBartime = isDay ? kline->at(-1)->date : kline->at(-1)->time;
			if(!isDay)
				lastBartime += 199000000000;

			//������һ���ѱպϵ�K�ߵ�ʱ���������������ʱ�䣬˵���������Ѿ������ˣ�����Ҫ����
			if(lastBartime > _last_cond_min)
			{
				log_info("Conditions expired, setup time: {}, time of last bar of main kbars: {}, all cleared", _last_cond_min, lastBartime);
				_condtions.clear();
			}
		}

		_engine->sub_tick(id(), stdCode);

		//�������K�ߣ��������һ��bar�ı��Ϊ0
		//�����һ��bar�ı������Ϊ��K�ߵĳ���
		if(isMain && _last_barno == 0)
		{
			_last_barno = kline->size();
		}
	}

	return kline;
}

WTSTickSlice* CtaStraBaseCtx::stra_get_ticks(const char* stdCode, uint32_t count)
{
	WTSTickSlice* ret = _engine->get_tick_slice(_context_id, stdCode, count);
	if (ret)
		_engine->sub_tick(id(), stdCode);

	return ret;
}

WTSTickData* CtaStraBaseCtx::stra_get_last_tick(const char* stdCode)
{
	return _engine->get_last_tick(_context_id, stdCode);
}

void CtaStraBaseCtx::stra_sub_ticks(const char* code)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	��������tick���ڱ��ؼ�һ��
	 *	tick���ݻص���ʱ���ȼ��һ��
	 */
	_tick_subs.insert(code);

	_engine->sub_tick(_context_id, code);
	log_info("Market data subscribed: {}", code);
}

WTSCommodityInfo* CtaStraBaseCtx::stra_get_comminfo(const char* stdCode)
{
	return _engine->get_commodity_info(stdCode);
}

std::string CtaStraBaseCtx::stra_get_rawcode(const char* stdCode)
{
	return _engine->get_rawcode(stdCode);
}

uint32_t CtaStraBaseCtx::stra_get_tdate()
{
	return _engine->get_trading_date();
}

uint32_t CtaStraBaseCtx::stra_get_date()
{
	return _engine->get_date();
}

uint32_t CtaStraBaseCtx::stra_get_time()
{
	return _engine->get_min_time();
}

double CtaStraBaseCtx::stra_get_fund_data(int flag )
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

void CtaStraBaseCtx::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void CtaStraBaseCtx::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void CtaStraBaseCtx::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void CtaStraBaseCtx::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

const char* CtaStraBaseCtx::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void CtaStraBaseCtx::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

uint64_t CtaStraBaseCtx::stra_get_first_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[0]._opentime;
}

uint64_t CtaStraBaseCtx::stra_get_last_exittime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	return pInfo._last_exittime;
}

uint64_t CtaStraBaseCtx::stra_get_last_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_UINT64;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_UINT64;

	return pInfo._details[pInfo._details.size() - 1]._opentime;
}

double CtaStraBaseCtx::stra_get_last_enterprice(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return INVALID_DOUBLE;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return INVALID_DOUBLE;

	return pInfo._details[pInfo._details.size() - 1]._price;
}

double CtaStraBaseCtx::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, const char* userTag /* = "" */)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (strlen(userTag) == 0)
	{
		//ֻ��userTagΪ�յ�ʱ��ʱ�򣬲Ż���bOnlyValid
		if (bOnlyValid)
		{
			//���������ϣ�ֻ�ж�ͷ�Ż��������
			//�����ط�Ҫ��֤����ͷ�ֲֵĻ���_frozenҪΪ0
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

double CtaStraBaseCtx::stra_get_position_avgpx(const char* stdCode)
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

double CtaStraBaseCtx::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

uint64_t CtaStraBaseCtx::stra_get_detail_entertime(const char* stdCode, const char* userTag)
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

double CtaStraBaseCtx::stra_get_detail_cost(const char* stdCode, const char* userTag)
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

double CtaStraBaseCtx::stra_get_detail_profit(const char* stdCode, const char* userTag, int flag /* = 0 */)
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


