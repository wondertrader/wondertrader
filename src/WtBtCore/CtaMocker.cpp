/*!
 * \file CtaMocker.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "CtaMocker.h"
#include "WtHelper.h"
#include "EventNotifier.h"

#include <exception>
#include <boost/filesystem.hpp>

#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSSessionInfo.hpp"
#include "../Includes/WTSVariant.hpp"
#include "../Share/CodeHelper.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSLogger.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include "rapidjson/filereadstream.h"
#include <fstream>
namespace rj = rapidjson;

const char* CMP_ALG_NAMES[] =
{
	"＝",
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


inline uint32_t makeCtxId()
{
	static std::atomic<uint32_t> _auto_context_id{ 1 };
	return _auto_context_id.fetch_add(1);
}


CtaMocker::CtaMocker(HisDataReplayer* replayer, const char* name, int32_t slippage /* = 0 */, bool persistData /* = true */, EventNotifier* notifier /* = NULL */, bool isRatioSlp /* = false */)
	: ICtaStraCtx(name)
	, _replayer(replayer)
	, _total_calc_time(0)
	, _emit_times(0)
	, _is_in_schedule(false)
	, _ud_modified(false)
	, _strategy(NULL)
	, _slippage(slippage)
	, _ratio_slippage(isRatioSlp)
	, _schedule_times(0)
	, _total_closeprofit(0)
	, _notifier(notifier)
	, _has_hook(false)
	, _hook_valid(true)
	, _cur_step(0)
	, _wait_calc(false)
	, _in_backtest(false)
	, _persist_data(persistData)
{
	_context_id = makeCtxId();
}


CtaMocker::~CtaMocker()
{
}

void CtaMocker::dump_stradata()
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
		jFund.AddMember("tdate", _cur_tdate, allocator);

		root.AddMember("fund", jFund, allocator);
	}

	{//信号保存
		rj::Value jSigs(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto& m : _sig_map)
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

	{//条件单保存
		rj::Value jCond(rj::kObjectType);
		rj::Value jItems(rj::kObjectType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto it = _condtions.begin(); it != _condtions.end(); it++)
		{
			const char* code = it->first.c_str();
			const CondList& condList = it->second;

			rj::Value cArray(rj::kArrayType);
			for (auto& condInfo : condList)
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

	if(_persist_data)
	{
		std::string folder = WtHelper::getOutputDir();
		folder += _name;
		folder += "/";

		if (!StdFile::exists(folder.c_str()))
			boost::filesystem::create_directories(folder.c_str());

		std::string filename = folder;
		filename += _name;
		filename += ".json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}
}

void CtaMocker::dump_chartdata()
{
	rj::Document root(rj::kObjectType);
	rj::Document::AllocatorType &allocator = root.GetAllocator();

	rj::Value klineItem(rj::kObjectType);
	if(_chart_code.empty())
	{
		//如果没有设置主K线，就用主K线落地
		klineItem.AddMember("code", rj::Value(_main_code.c_str(), allocator), allocator);
		klineItem.AddMember("period", rj::Value(_main_period.c_str(), allocator), allocator);
	}
	else
	{
		klineItem.AddMember("code", rj::Value(_chart_code.c_str(), allocator), allocator);
		klineItem.AddMember("period", rj::Value(_chart_period.c_str(), allocator), allocator);
	}

	root.AddMember("kline", klineItem, allocator);

	if (!_chart_indice.empty())
	{
		rj::Value jIndice(rj::kArrayType);
		for (const auto& v : _chart_indice)
		{
			const ChartIndex& cIndex = v.second;
			rj::Value jIndex(rj::kObjectType);
			jIndex.AddMember("name", rj::Value(cIndex._name.c_str(), allocator), allocator);
			jIndex.AddMember("index_type", cIndex._indexType, allocator);

			rj::Value jLines(rj::kArrayType);
			for(const auto& v2 : cIndex._lines)
			{
				const ChartLine& cLine = v2.second;
				rj::Value jLine(rj::kObjectType);
				jLine.AddMember("name", rj::Value(cLine._name.c_str(), allocator), allocator);
				jLine.AddMember("line_type", cLine._lineType, allocator);

				//rj::Value jVals(rj::kArrayType);
				//for(const double& val : cLine._values)
				//{
				//	jVals.PushBack(val, allocator);
				//}

				//jLine.AddMember("values", jVals, allocator);

				jLines.PushBack(jLine, allocator);
			}

			jIndex.AddMember("lines", jLines, allocator);

			rj::Value jBaseLines(rj::kObjectType);
			for (const auto& v3 : cIndex._base_lines)
			{
				jBaseLines.AddMember(rj::Value(v3.first.c_str(), allocator), rj::Value(v3.second), allocator);
			}

			jIndex.AddMember("baselines", jBaseLines, allocator);

			jIndice.PushBack(jIndex, allocator);
		}

		root.AddMember("index", jIndice, allocator);
	}

	//if(!_chart_marks.empty())
	//{
	//	rj::Value jMarks(rj::kArrayType);
	//	for(const ChartMark& mark : _chart_marks)
	//	{
	//		rj::Value jMark(rj::kObjectType);
	//		jMark.AddMember("bartime", mark._bartime, allocator);
	//		jMark.AddMember("price", mark._price, allocator);
	//		jMark.AddMember("icon", rj::Value(mark._icon.c_str(), allocator), allocator);
	//		jMark.AddMember("tag", rj::Value(mark._tag.c_str(), allocator), allocator);

	//		jMarks.PushBack(jMark, allocator);
	//	}

	//	root.AddMember("marks", jMarks, allocator);
	//}

	if(_persist_data)
	{
		std::string folder = WtHelper::getOutputDir();
		folder += _name;
		folder += "/";

		if(!StdFile::exists(folder.c_str()))
			boost::filesystem::create_directories(folder.c_str());

		std::string filename = folder;
		filename += "btchart.json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());

		filename = folder;
		filename += "indice.csv";
		std::string content = "bartime,index_name,line_name,value\n";
		if (!_index_logs.str().empty()) content += _index_logs.str();
		StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

		filename = folder;
		filename += "marks.csv";
		content = "bartime,price,icon,tag\n";
		if (!_mark_logs.str().empty()) content += _mark_logs.str();
		StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());
	}
}

void CtaMocker::dump_outputs()
{
	if (!_persist_data)
		return;

	std::string folder = WtHelper::getOutputDir();
	folder += _name;
	folder += "/";
	boost::filesystem::create_directories(folder.c_str());

	std::string filename = folder + "trades.csv";
	std::string content = "code,time,direct,action,price,qty,tag,fee,barno\n";
	if(!_trade_logs.str().empty()) content += _trade_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "closes.csv";
	content = "code,direct,opentime,openprice,closetime,closeprice,qty,profit,maxprofit,maxloss,totalprofit,entertag,exittag,openbarno,closebarno\n";
	if (!_close_logs.str().empty()) content += _close_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "funds.csv";
	content = "date,closeprofit,positionprofit,dynbalance,fee\n";
	if (!_fund_logs.str().empty()) content += _fund_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "signals.csv";
	content = "code,target,sigprice,gentime,usertag\n";
	if (!_sig_logs.str().empty()) content += _sig_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	filename = folder + "positions.csv";
	content = "date,code,volume,closeprofit,dynprofit\n";
	if (!_pos_logs.str().empty()) content += _pos_logs.str();
	StdFile::write_file_content(filename.c_str(), (void*)content.c_str(), content.size());

	{
		rj::Document root(rj::kObjectType);
		rj::Document::AllocatorType &allocator = root.GetAllocator();
		for (auto it = _user_datas.begin(); it != _user_datas.end(); it++)
		{
			root.AddMember(rj::Value(it->first.c_str(), allocator), rj::Value(it->second.c_str(), allocator), allocator);
		}

		filename = folder;
		filename += "ud_";
		filename += _name;
		filename += ".json";

		rj::StringBuffer sb;
		rj::PrettyWriter<rj::StringBuffer> writer(sb);
		root.Accept(writer);
		StdFile::write_file_content(filename.c_str(), sb.GetString());
	}
}

void CtaMocker::log_signal(const char* stdCode, double target, double price, uint64_t gentime, const char* usertag /* = "" */)
{
	_sig_logs << stdCode << "," << target << "," << price << "," << gentime << "," << usertag << "\n";
}

void CtaMocker::log_trade(const char* stdCode, bool isLong, bool isOpen, uint64_t curTime, double price, double qty, const char* userTag, double fee, uint32_t barNo)
{
	_trade_logs << stdCode << "," << curTime << "," << (isLong ? "LONG" : "SHORT") << "," << (isOpen ? "OPEN" : "CLOSE") 
		<< "," << price << "," << qty << "," << userTag << "," << fee << "," << barNo << "\n";
}

void CtaMocker::log_close(const char* stdCode, bool isLong, uint64_t openTime, double openpx, uint64_t closeTime, double closepx, double qty, double profit, double maxprofit, double maxloss, 
	double totalprofit /* = 0 */, const char* enterTag /* = "" */, const char* exitTag /* = "" */, uint32_t openBarNo /* = 0 */, uint32_t closeBarNo /* = 0 */)
{
	_close_logs << stdCode << "," << (isLong ? "LONG" : "SHORT") << "," << openTime << "," << openpx
		<< "," << closeTime << "," << closepx << "," << qty << "," << profit << "," << maxprofit << "," << maxloss << ","
		<< totalprofit << "," << enterTag << "," << exitTag << "," << openBarNo << "," << closeBarNo << "\n";
}

bool CtaMocker::init_cta_factory(WTSVariant* cfg)
{
	if (cfg == NULL)
		return false;

	const char* module = cfg->getCString("module");

	DllHandle hInst = DLLHelper::load_library(module);
	if (hInst == NULL)
		return false;

	FuncCreateStraFact creator = (FuncCreateStraFact)DLLHelper::get_symbol(hInst, "createStrategyFact");
	if (creator == NULL)
	{
		DLLHelper::free_library(hInst);
		return false;
	}

	_factory._module_inst = hInst;
	_factory._module_path = module;
	_factory._creator = creator;
	_factory._remover = (FuncDeleteStraFact)DLLHelper::get_symbol(hInst, "deleteStrategyFact");
	_factory._fact = _factory._creator();

	WTSVariant* cfgStra = cfg->get("strategy");
	if (cfgStra)
	{
		_strategy = _factory._fact->createStrategy(cfgStra->getCString("name"), cfgStra->getCString("id"));
		if(_strategy)
		{
			WTSLogger::info("Strategy {}.{} is created,strategy ID: {}", _factory._fact->getName(), _strategy->getName(), _strategy->id());
		}
		_strategy->init(cfgStra->get("params"));
		_name = _strategy->id();
	}

	return true;
}

void CtaMocker::load_incremental_data(const char* incremental_backtest_base)
{
	std::string folder = WtHelper::getOutputDir();
	folder += incremental_backtest_base;
	folder += "/";
	WTSLogger::info("loading incremental data from: {}", folder);

	std::string tradesFilename = folder + "trades.csv";
	if (boost::filesystem::exists(tradesFilename))
	{
		std::ifstream tradesFile(tradesFilename);
		std::string str;
		// 跳过标题行
		std::getline(tradesFile, str);
		while (std::getline(tradesFile, str))
		{
			_trade_logs << str << "\n";
		}
	}

	std::string closesFilename = folder + "closes.csv";
	if (boost::filesystem::exists(closesFilename))
	{
		std::ifstream closesFile(closesFilename);
		std::string str;
		// 跳过标题行
		std::getline(closesFile, str);
		while (std::getline(closesFile, str))
		{
			_close_logs << str << "\n";
		}
	}

	std::string fundsFilename = folder + "funds.csv";
	if (boost::filesystem::exists(fundsFilename))
	{
		std::ifstream fundsFile(fundsFilename);
		std::string str;
		// 跳过标题行
		std::getline(fundsFile, str);
		while (std::getline(fundsFile, str))
		{
			_fund_logs << str << "\n";
		}
	}

	std::string positionsFilename = folder + "positions.csv";
	if (boost::filesystem::exists(positionsFilename))
	{
		std::ifstream positionsFile(positionsFilename);
		std::string str;
		// 跳过标题行
		std::getline(positionsFile, str);
		while (std::getline(positionsFile, str))
		{
			_pos_logs << str << "\n";
		}
	}

	std::string signalsFilename = folder + "signals.csv";
	if (boost::filesystem::exists(signalsFilename))
	{
		std::ifstream signalsFile(signalsFilename);
		std::string str;
		// 跳过标题行
		std::getline(signalsFile, str);
		while (std::getline(signalsFile, str))
		{
			_sig_logs << str << "\n";
		}
	}

	std::string strategyDumpFilename = folder + fmtutil::format("{}.json", incremental_backtest_base);
	if (boost::filesystem::exists(strategyDumpFilename))
	{
		WTSLogger::info("load incremental data json: {}", strategyDumpFilename);
		FILE* fp = fopen(strategyDumpFilename.c_str(), "rb");
		char readBuffer[65536];
		rj::FileReadStream strategyDumpFile(fp, readBuffer, sizeof(readBuffer));
		rj::Document d;
		d.ParseStream(strategyDumpFile);
		fclose(fp);
		if (d.HasMember("positions"))
		{
			const rj::Value& positions = d["positions"];
			for (rj::SizeType i = 0; i < positions.Size(); i++)
			{
				const rj::Value& positionEntry = positions[i];
				const char* positionEntry_code = positionEntry["code"].GetString();
				PosInfo& pInfo = _pos_map[positionEntry_code];
				pInfo._volume = positionEntry["volume"].GetDouble();
				pInfo._closeprofit = positionEntry["closeprofit"].GetDouble();
				pInfo._dynprofit = positionEntry["dynprofit"].GetDouble();
				pInfo._last_entertime = positionEntry["lastentertime"].GetUint64();
				pInfo._last_exittime = positionEntry["lastexittime"].GetUint64();

				if (positionEntry.HasMember("details"))
				{
					const rj::Value& details = positionEntry["details"];
					for (rj::SizeType j = 0; j < details.Size(); j++)
					{
						const rj::Value& positionDetailEntry = details[j];
						DetailInfo curPosDetail;
						curPosDetail._long = positionDetailEntry["long"].GetBool();
						curPosDetail._price = positionDetailEntry["price"].GetDouble();
						curPosDetail._max_price = positionDetailEntry["maxprice"].GetDouble();
						curPosDetail._min_price = positionDetailEntry["minprice"].GetDouble();
						curPosDetail._volume = positionDetailEntry["volume"].GetDouble();
						curPosDetail._opentime = positionDetailEntry["opentime"].GetUint64();
						curPosDetail._opentdate = positionDetailEntry["opentdate"].GetInt();
						curPosDetail._profit = positionDetailEntry["profit"].GetDouble();
						curPosDetail._max_profit = positionDetailEntry["maxprofit"].GetDouble();
						curPosDetail._max_loss = positionDetailEntry["maxloss"].GetDouble();
						strcpy(curPosDetail._opentag, positionDetailEntry["opentag"].GetString());
						pInfo._details.push_back(curPosDetail);
					}
				}
			}
		}

		if (d.HasMember("fund"))
		{
			_fund_info._total_profit = d["fund"]["total_profit"].GetDouble();
			_fund_info._total_dynprofit = d["fund"]["total_dynprofit"].GetDouble();
			_fund_info._total_fees = d["fund"]["total_fees"].GetDouble();
		}

		if (d.HasMember("signals"))
		{
			for (rj::Value::ConstMemberIterator itr = d["signals"].MemberBegin(); itr != d["signals"].MemberEnd(); ++itr)
			{
				std::string stkCode = itr->name.GetString();
				SigInfo& sInfo = _sig_map[stkCode];
				sInfo._usertag = itr->value["usertag"].GetString();
				sInfo._volume = itr->value["volume"].GetDouble();
				sInfo._sigprice = itr->value["sigprice"].GetDouble();
				sInfo._gentime = itr->value["gentime"].GetUint64();
			}
		}

		if (d.HasMember("conditions") && d["conditions"].HasMember("items"))
		{
			// conditions -> items 下面的内容是两层嵌套   items[CODE] is a list
			rj::Value& conditionItemsEntry = d["conditions"]["items"];

			for (rj::Value::ConstMemberIterator itr = conditionItemsEntry.MemberBegin(); itr != conditionItemsEntry.MemberEnd(); ++itr)
			{
				std::string stkCode = itr->name.GetString();
				for (rj::SizeType i = 0; i < itr->value.Size(); i++)
				{
					const rj::Value& conditionItemStkCondEntry = itr->value[i];
					CondEntrust condEntrust;
					strcpy(condEntrust._usertag, conditionItemStkCondEntry["usertag"].GetString());
					condEntrust._field = (WTSCompareField)conditionItemStkCondEntry["field"].GetInt();
					condEntrust._alg = (WTSCompareType)conditionItemStkCondEntry["alg"].GetInt();
					condEntrust._target = conditionItemStkCondEntry["target"].GetDouble();
					condEntrust._qty = conditionItemStkCondEntry["qty"].GetDouble();
					condEntrust._action = (char)conditionItemStkCondEntry["action"].GetUint();

					_condtions[stkCode].push_back(condEntrust);
				}
			}
		}
	}
	else
	{
		WTSLogger::warn("fail load incremental data json: {}", strategyDumpFilename);
	}
}

//////////////////////////////////////////////////////////////////////////
//IDataSink
void CtaMocker::handle_init()
{
	this->on_init();
}

void CtaMocker::handle_bar_close(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	this->on_bar(stdCode, period, times, newBar);
}

void CtaMocker::handle_schedule(uint32_t uDate, uint32_t uTime)
{
	this->on_schedule(uDate, uTime);
}

void CtaMocker::handle_session_begin(uint32_t curTDate)
{
	this->on_session_begin(curTDate);
}

void CtaMocker::handle_session_end(uint32_t curTDate)
{
	this->on_session_end(curTDate);
}

void CtaMocker::handle_section_end(uint32_t curTDate, uint32_t curTime)
{
	/*
	 *	By Wesley @ 2022.05.16
	 *	如果小节结束，也需要清理掉价格缓存，防止小节跳空
	 *	这种主要是针对夜盘交易
	 */
	_price_map.clear();
}

void CtaMocker::handle_replay_done()
{
	_in_backtest = false;

	if(_emit_times > 0)
	{
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, 
			"Strategy has been scheduled {} times, totally taking {} us, {:.3f} us each time",
			_emit_times, _total_calc_time, _total_calc_time*1.0 / _emit_times);
	}
	else
	{
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, 
			"Strategy has been scheduled for {} times", _emit_times);
	}

	dump_outputs();

	dump_stradata();

	dump_chartdata();

	if (_has_hook && _hook_valid)
	{
		WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, "Replay done, notify control thread");
		while(_wait_calc)
			_cond_calc.notify_all();
		WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, "Notify control thread the end done");
	}

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Notify strategy the end of backtest");
	this->on_bactest_end();
}

void CtaMocker::proc_tick(const char* stdCode, double last_px, double cur_px)
{
	{
		auto it = _sig_map.find(stdCode);
		if (it != _sig_map.end())
		{
			//if (sInfo->isInTradingTime(_replayer->get_raw_time(), true))
			{
				const SigInfo& sInfo = it->second;
				double price;
				if (decimal::eq(sInfo._desprice, 0.0))
					price = cur_px;
				else
					price = sInfo._desprice;
				do_set_position(stdCode, sInfo._volume, price, sInfo._usertag.c_str());

				//如果是条件单触发，则回调on_condition_triggered
				if (sInfo._sigtype == 2)
					on_condition_triggered(stdCode, sInfo._volume, cur_px, sInfo._usertag.c_str());
				_sig_map.erase(it);
			}
		}
	}

	update_dyn_profit(stdCode, cur_px);

	//////////////////////////////////////////////////////////////////////////
	//检查条件单
	if (!_condtions.empty())
	{
		auto it = _condtions.find(stdCode);
		if (it == _condtions.end())
			return;

		const CondList& condList = it->second;
		double curPrice = cur_px;
		const CondEntrust* matchedEntrust = NULL;
		for (const CondEntrust& entrust : condList)
		{
			/*
			 * 如果开启了tick模式，就正常比较
			 * 但是如果没有开启tick模式，逻辑就非常复杂
			 * 因为不开回测的时候tick是用开高低收模拟出来的，如果直接按照目标价格触发，可能是有问题的
			 * 首先要拿到上一笔价格，和当前最新价格做一个比价，得到左边界和右边界
			 * 这里只能假设前后两笔价格之间是连续的，这样需要将两笔价格都加入判断
			 * 当条件是等于时，如果目标价格在左右边界之间，说明目标价格在这期间是出现过的，则认为价格匹配
			 * 当条件是大于的时候，我们需要判断右边界，即稍大的值是否满足条件，并取左边界与目标价中稍大的作为当前价
			 * 当条件是小于的时候，我们需要判断左边界，即稍小的值是否满足条件，并取右边界与目标价中稍小的作为当前价
			 */

			double left_px = min(last_px, cur_px);
			double right_px = max(last_px, cur_px);

			bool isMatched = false;
			if (!_replayer->is_tick_simulated())
			{
				//如果tick数据不是模拟的，则使用最新价格
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
					matchedEntrust = &entrust;
					break;
				}
			}
			else
			{
				//如果tick数据是模拟的，则要处理一下
				switch (entrust._alg)
				{
				case WCT_Equal:
					isMatched = decimal::le(left_px, entrust._target) && decimal::ge(right_px, entrust._target);
					break;
				case WCT_Larger:
					isMatched = decimal::gt(right_px, entrust._target);
					break;
				case WCT_LargerOrEqual:
					isMatched = decimal::ge(right_px, entrust._target);
					break;
				case WCT_Smaller:
					isMatched = decimal::lt(left_px, entrust._target);
					break;
				case WCT_SmallerOrEqual:
					isMatched = decimal::le(left_px, entrust._target);
					break;
				default:
					break;
				}

				if (isMatched)
				{
					/*
					* By HeJ @ 2023.02.27
					* 在bar回测中，经常会出现同一个价格触发了多个条件单时，要选出一个作为最终的触发价，遵循以下规则：
					* 1 alg不同的条件单，或者alg为WCT_Equal，以最先设置的那个为准
					* 2 alg一样的调价单，如果是WCT_Larger与WCT_LargerOrEqual，取触发价较小的，WCT_Smaller与WCT_SmallerOrEqual，取触发价较大的
					*/
					if (matchedEntrust == NULL)
					{
						matchedEntrust = &entrust;
						if (entrust._alg == WCT_Larger || entrust._alg == WCT_LargerOrEqual)
							curPrice = max(left_px, entrust._target);
						else if (entrust._alg == WCT_Smaller || entrust._alg == WCT_SmallerOrEqual)
							curPrice = min(right_px, entrust._target);
						else
							curPrice = entrust._target;
					}
					else if (matchedEntrust->_alg == entrust._alg)
					{
						if (entrust._alg == WCT_Larger || entrust._alg == WCT_LargerOrEqual)
						{
							if (entrust._target < matchedEntrust->_target)
							{
								matchedEntrust = &entrust;
								curPrice = max(left_px, entrust._target);
							}
						}
						else if (entrust._alg == WCT_Smaller || entrust._alg == WCT_SmallerOrEqual)
						{
							if (entrust._target > matchedEntrust->_target)
							{
								matchedEntrust = &entrust;
								curPrice = min(right_px, entrust._target);
							}
						}
					}
				}
			}
		}

		if (matchedEntrust != NULL)
		{
			const CondEntrust& entrust = *matchedEntrust;
			double price = curPrice;
			double curQty = stra_get_position(stdCode);
			//_replayer->is_tick_enabled() ? newTick->price() : entrust._target;	//如果开启了tick回测,则用tick数据的价格,如果没有开启,则只能用条件单价格
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO,
				"Condition order triggered[newprice: {}{}{}], instrument: {}, {} {}",
				curPrice, CMP_ALG_NAMES[entrust._alg], entrust._target, stdCode, ACTION_NAMES[entrust._action], entrust._qty);
			switch (entrust._action)
			{
			case COND_ACTION_OL:
			{
				if (decimal::lt(curQty, 0))
					append_signal(stdCode, entrust._qty, entrust._usertag, price, 2);
				else
					append_signal(stdCode, curQty + entrust._qty, entrust._usertag, price, 2);
			}
			break;
			case COND_ACTION_CL:
			{
				double maxQty = min(curQty, entrust._qty);
				append_signal(stdCode, curQty - maxQty, entrust._usertag, price, 2);
			}
			break;
			case COND_ACTION_OS:
			{
				if (decimal::gt(curQty, 0))
					append_signal(stdCode, -entrust._qty, entrust._usertag, price, 2);
				else
					append_signal(stdCode, curQty - entrust._qty, entrust._usertag, price, 2);
			}
			break;
			case COND_ACTION_CS:
			{
				double maxQty = min(abs(curQty), entrust._qty);
				append_signal(stdCode, curQty + maxQty, entrust._usertag, price, 2);
			}
			break;
			case COND_ACTION_SP:
			{
				append_signal(stdCode, entrust._qty, entrust._usertag, price, 2);
			}
			default: break;
			}

			//同一个bar设置针对同一个合约的条件单,只可能触发一条
			//所以这里直接清理掉即可
			_condtions.erase(it);
		}
	}
}


void CtaMocker::handle_tick(const char* stdCode, WTSTickData* newTick, uint32_t pxType /* = 0 */)
{
	double cur_px = newTick->price();

	/*
	 *	By Wesley @ 2022.04.19
	 *	这里的逻辑改了一下
	 *	如果缓存的价格不存在，则上一笔价格就用最新价
	 *	这里主要是为了应对跨日价格跳空的情况
	 */
	double last_px = cur_px;
	if(pxType != 0)
	{
		auto it = _price_map.find(stdCode);
		if (it != _price_map.end())
			last_px = it->second;
		else
			last_px = cur_px;
	}
	
	
	_price_map[stdCode] = cur_px;
	_ticks[stdCode] = newTick->getTickStruct();

	//先检查是否要信号要触发
	//By Wesley @ 2022.04.19
	//虽然这段逻辑下面也根据isBarEnd复制了一段
	//但是这一段还是要保留
	proc_tick(stdCode, last_px, cur_px);

	on_tick_updated(stdCode, newTick);

	/*
	 *	By Wesley @ 2022.04.19
	 *	isBarEnd，如果是逐tick回放，这个永远都是true，永远也不会触发下面这段逻辑
	 *	如果是模拟的tick数据，用收盘价模拟tick的时候，isBarEnd才会为true
	 *	如果不是收盘价模拟的tick，那么直接在当前tick触发撮合逻辑
	 *	这样做的目的是为了让在模拟tick触发的ontick中下单的信号能够正常处理
	 *	而不至于在回测的时候成交价偏离太远
	 */
	if(pxType != 3)
		proc_tick(stdCode, last_px, cur_px);
}


//////////////////////////////////////////////////////////////////////////
//回调函数
void CtaMocker::on_bar(const char* stdCode, const char* period, uint32_t times, WTSBarStruct* newBar)
{
	if (newBar == NULL)
		return;

	thread_local static char realPeriod[8] = { 0 };
	fmtutil::format_to(realPeriod, "{}{}", period, times);

	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, realPeriod);

	KlineTag& tag = _kline_tags[key];
	tag._closed = true;

	if(tag._notify)
		on_bar_close(stdCode, realPeriod, newBar);
}

void CtaMocker::on_init()
{
	_ticks.clear();
	_in_backtest = true;
	if (_strategy)
		_strategy->on_init(this);

	WTSLogger::info("CTA Strategy initialized with {} slippage: {}", _ratio_slippage?"ratio":"absolute", _slippage);
}

void CtaMocker::update_dyn_profit(const char* stdCode, double price)
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
			WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
			double dynprofit = 0;
			for (auto pit = pInfo._details.begin(); pit != pInfo._details.end(); pit++)
			{
				DetailInfo& dInfo = *pit;
				dInfo._profit = dInfo._volume*(price - dInfo._price)*commInfo->getVolScale()*(dInfo._long ? 1 : -1);
				if (dInfo._profit > 0)
					dInfo._max_profit = max(dInfo._profit, dInfo._max_profit);
				else if (dInfo._profit < 0)
					dInfo._max_loss = min(dInfo._profit, dInfo._max_loss);

				dInfo._max_price = std::max(dInfo._max_price, price);
				dInfo._min_price = std::min(dInfo._min_price, price);

				dynprofit += dInfo._profit;
			}

			pInfo._dynprofit = dynprofit;
		}
	}

	double total_dynprofit = 0;
	for (auto& v : _pos_map)
	{
		const PosInfo& pInfo = v.second;
		total_dynprofit += pInfo._dynprofit;
	}

	_fund_info._total_dynprofit = total_dynprofit;
}

void CtaMocker::on_tick(const char* stdCode, WTSTickData* newTick, bool bEmitStrategy /* = true */)
{
	//这个逻辑全部迁移到handle_tick里去了
}

void CtaMocker::on_bar_close(const char* code, const char* period, WTSBarStruct* newBar)
{
	if (_strategy)
		_strategy->on_bar(this, code, period, newBar);
}

void CtaMocker::on_tick_updated(const char* code, WTSTickData* newTick)
{
	auto it = _tick_subs.find(code);
	if (it == _tick_subs.end())
		return;

	if (_strategy)
		_strategy->on_tick(this, code, newTick);
}

void CtaMocker::on_calculate(uint32_t curDate, uint32_t curTime)
{
	if (_strategy)
		_strategy->on_schedule(this, curDate, curTime);
}

void CtaMocker::enable_hook(bool bEnabled /* = true */)
{
	_hook_valid = bEnabled;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calculating hook {}", bEnabled?"enabled":"disabled");
}

void CtaMocker::install_hook()
{
	_has_hook = true;

	WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "CTA hook installed");
}

bool CtaMocker::step_calc()
{
	if (!_has_hook)
	{
		return false;
	}

	//总共分为4个状态
	//0-初始状态，1-oncalc，2-oncalc结束，3-oncalcdone
	//所以，如果出于0/2，则说明没有在执行中，需要notify
	bool bNotify = false;
	while (_in_backtest && (_cur_step == 0 || _cur_step == 2))
	{
		_cond_calc.notify_all();
		bNotify = true;
	}

	if(bNotify)
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Notify calc thread, wait for calc done");

	if(_in_backtest)
	{
		_wait_calc = true;
		StdUniqueLock lock(_mtx_calc);
		_cond_calc.wait(_mtx_calc);
		_wait_calc = false;
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done notified");
		_cur_step = (_cur_step + 1) % 4;

		return true;
	}
	else
	{
		_hook_valid = false;
		WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Backtest exit automatically");
		return false;
	}
}

bool CtaMocker::on_schedule(uint32_t curDate, uint32_t curTime)
{
	_is_in_schedule = true;//开始调度,修改标记

	_schedule_times++;

	bool isMainUdt = false;
	bool emmited = false;

	for (auto it = _kline_tags.begin(); it != _kline_tags.end(); it++)
	{
		const std::string& key = it->first;
		KlineTag& marker = (KlineTag&)it->second;

		StringVector ay = StrUtil::split(key, "#");
		const char* stdCode = ay[0].c_str();

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

		WTSSessionInfo* sInfo = _replayer->get_session_info(stdCode, true);

		if (isMainUdt || _kline_tags.empty())
		{
			TimeUtils::Ticker ticker;

			uint32_t offTime = sInfo->offsetTime(curTime, true);
			if (offTime <= sInfo->getCloseTime(true))
			{
				_condtions.clear();
				if(_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
					StdUniqueLock lock(_mtx_calc);
					_cond_calc.wait(_mtx_calc);
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
					_cur_step = 1;
				}

				on_calculate(curDate, curTime);

				if (_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done, notify control thread");
					while (_cur_step==1)
						_cond_calc.notify_all();

					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Waiting for resume notify");
					StdUniqueLock lock(_mtx_calc);
					_cond_calc.wait(_mtx_calc);
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc resumed");
					_cur_step = 3;
				}

				if(_has_hook)
					on_calculate_done(curDate, curTime);
				emmited = true;

				if (_condtions.empty())
					_last_cond_min = (uint64_t)curDate * 10000 + curTime;

				_emit_times++;
				_total_calc_time += ticker.micro_seconds();


				/*
				 *	By Wesley @ 2022.07.16
				 *	策略计算完成，需要把指标数据做一个检查
				 *	如果策略在本轮没有设置指标值，则用上一个数据补齐
				 *	如果是开始，则用默认值补齐
				 */
				//for(auto& v : _chart_indice)
				//{
				//	ChartIndex& cIndex = v.second;
				//	for(auto& line : cIndex._lines)
				//	{
				//		ChartLine& cLine = line.second;
				//		if(cLine._values.size() < _emit_times)
				//		{
				//			double lastVal = DBL_MAX;
				//			if (!cLine._values.empty())
				//				lastVal = cLine._values.back();

				//			cLine._values.emplace_back(lastVal);
				//		}
				//	}
				//}

				if (_has_hook && _hook_valid)
				{
					WTSLogger::log_dyn("strategy", _name.c_str(), LL_DEBUG, "Calc done, notify control thread");
					while(_cur_step == 3)
						_cond_calc.notify_all();
				}
			}
			else
			{
				WTSLogger::log_dyn("strategy", _name.c_str(), LL_INFO, "{} is not trading time,strategy will not be scheduled", curTime);
			}
			break;
		}
	}

	_is_in_schedule = false;//调度结束,修改标记
	return emmited;
}


void CtaMocker::on_session_begin(uint32_t curTDate)
{
	_cur_tdate = curTDate;

	//每个交易日开始，要把冻结持仓置零
	for (auto& it : _pos_map)
	{
		const char* stdCode = it.first.c_str();
		PosInfo& pInfo = (PosInfo&)it.second;
		if (!decimal::eq(pInfo._frozen, 0))
		{
			log_debug("{} of {} frozen released on {}", pInfo._frozen, stdCode, curTDate);
			pInfo._frozen = 0;
		}
	}

	/*
	 *	By Wesley @ 2022.04.19
	 *	新交易日开始的时候，价格缓存清掉，要重新处理
	 */
	_price_map.clear();

	if (_strategy)
		_strategy->on_session_begin(this, curTDate);
}

void CtaMocker::enum_position(FuncEnumCtaPosCallBack cb, bool bForExecute)
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

void CtaMocker::on_session_end(uint32_t curTDate)
{
	if (_strategy)
		_strategy->on_session_end(this, curTDate);

	uint32_t curDate = curTDate;//_replayer->get_trading_date();

	double total_profit = 0;
	double total_dynprofit = 0;

	for (auto it = _pos_map.begin(); it != _pos_map.end(); it++)
	{
		const char* stdCode = it->first.c_str();
		const PosInfo& pInfo = it->second;
		total_profit += pInfo._closeprofit;
		total_dynprofit += pInfo._dynprofit;

		if(decimal::eq(pInfo._volume, 0.0))
			continue;

		_pos_logs << fmt::format("{},{},{},{:.2f},{:.2f}\n", curDate, stdCode,
			pInfo._volume, pInfo._closeprofit, pInfo._dynprofit);
	}

	_fund_logs << fmt::format("{},{:.2f},{:.2f},{:.2f},{:.2f}\n", curDate,
		_fund_info._total_profit, _fund_info._total_dynprofit,
		_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);
	
	if (_notifier)
		_notifier->notifyFund("BT_FUND", curDate, _fund_info._total_profit, _fund_info._total_dynprofit,
			_fund_info._total_profit + _fund_info._total_dynprofit - _fund_info._total_fees, _fund_info._total_fees);
}

CondList& CtaMocker::get_cond_entrusts(const char* stdCode)
{
	CondList& ce = _condtions[stdCode];
	return ce;
}

//////////////////////////////////////////////////////////////////////////
//策略接口
void CtaMocker::stra_enter_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if(commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式,则直接触发
	{
		double curQty = stra_get_position(stdCode);
		if(decimal::lt(curQty, 0))
			append_signal(stdCode, qty, userTag, 0.0, _is_in_schedule ? 0 : 1);
		else
			append_signal(stdCode, curQty + qty, userTag, 0.0, _is_in_schedule ? 0 : 1);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

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

		entrust._action = COND_ACTION_OL;

		condList.emplace_back(entrust);
	}
}

void CtaMocker::stra_enter_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	if(!commInfo->canShort())
	{
		log_error("Cannot short on {}", stdCode);
		return;
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式,则直接触发
	{
		double curQty = stra_get_position(stdCode);
		if(decimal::gt(curQty, 0))
			append_signal(stdCode, -qty, userTag, 0.0, _is_in_schedule ? 0 : 1);
		else
			append_signal(stdCode, curQty - qty, userTag, 0.0, _is_in_schedule ? 0 : 1);

	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

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

void CtaMocker::stra_exit_long(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
	{
		log_error("Cannot find corresponding commodity info of {}", stdCode);
		return;
	}

	WTSSessionInfo* sInfo = commInfo->getSessionInfo();
	uint32_t offTime = sInfo->offsetTime(_replayer->get_min_time(), true);
	bool isLastBarOfDay = (offTime == sInfo->getCloseTime(true));

	//读取可平持仓,如果是收盘那根bar，则直接读取全部持仓
	double curQty = stra_get_position(stdCode, !isLastBarOfDay);
	if (decimal::le(curQty, 0))
		return;

	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式,则直接触发
	{
		double maxQty = min(curQty, qty);
		double totalQty = stra_get_position(stdCode, false);
		append_signal(stdCode, totalQty - maxQty, userTag, 0.0, _is_in_schedule ? 0 : 1);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

		entrust._qty = min(curQty, qty);
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

void CtaMocker::stra_exit_short(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice, double stopprice)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
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
	if (decimal::ge(curQty, 0))
		return;

	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//如果不是动态下单模式,则直接触发
	{
		double maxQty = min(abs(curQty), qty);
		append_signal(stdCode, curQty + maxQty, userTag, 0.0, _is_in_schedule ? 0 : 1);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);

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

double CtaMocker::stra_get_price(const char* stdCode)
{
	if (_replayer)
		return _replayer->get_cur_price(stdCode);

	return 0.0;
}

double CtaMocker::stra_get_day_price(const char* stdCode, int flag /* = 0 */)
{
	if (_replayer)
		return _replayer->get_day_price(stdCode, flag);

	return 0.0;
}

void CtaMocker::stra_set_position(const char* stdCode, double qty, const char* userTag /* = "" */, double limitprice /* = 0.0 */, double stopprice /* = 0.0 */)
{
	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
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

	if(commInfo->isT1())
	{
		double valid = stra_get_position(stdCode, true);
		double frozen = total - valid;
		//如果是T+1规则，则目标仓位不能小于冻结仓位
		if(decimal::lt(qty, frozen))
		{
			WTSLogger::log_dyn("strategy", _name.c_str(), LL_ERROR, "New position of {} cannot be set to {} due to {} being frozen", stdCode, qty, frozen);
			return;
		}
	}

	_replayer->sub_tick(_context_id, stdCode);
	if (decimal::eq(limitprice, 0.0) && decimal::eq(stopprice, 0.0))	//没有设置触发条件，则直接添加信号
	{
		append_signal(stdCode, qty, userTag, 0.0, _is_in_schedule ? 0 : 1);
	}
	else
	{
		CondList& condList = get_cond_entrusts(stdCode);

		bool isBuy = decimal::gt(qty, total);

		CondEntrust entrust;
		strcpy(entrust._code, stdCode);
		strcpy(entrust._usertag, userTag);
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

void CtaMocker::append_signal(const char* stdCode, double qty, const char* userTag /* = "" */, double price /* = 0.0 */, uint32_t sigType /* = 0 */)
{
	double curPx = _price_map[stdCode];

	SigInfo& sInfo = _sig_map[stdCode];
	sInfo._volume = qty;
	sInfo._sigprice = curPx;
	sInfo._desprice = price;
	sInfo._usertag = userTag;
	sInfo._gentime = (uint64_t)_replayer->get_date() * 1000000000 + (uint64_t)_replayer->get_raw_time() * 100000 + _replayer->get_secs();
	sInfo._sigtype = sigType;

	log_signal(stdCode, qty, curPx, sInfo._gentime, userTag);

	//save_data();
}

void CtaMocker::do_set_position(const char* stdCode, double qty, double price /* = 0.0 */, const char* userTag /* = "" */)
{
	PosInfo& pInfo = _pos_map[stdCode];
	double curPx = price;
	if (decimal::eq(price, 0.0))
		curPx = _price_map[stdCode];
	uint64_t curTm = (uint64_t)_replayer->get_date() * 10000 + _replayer->get_min_time();
	uint32_t curTDate = _replayer->get_trading_date();

	//手数相等则不用操作了
	if (decimal::eq(pInfo._volume, qty))
		return;

	WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
	if (commInfo == NULL)
		return;

	//成交价
	double trdPx = curPx;

	double diff = qty - pInfo._volume;
	bool isBuy = decimal::gt(diff, 0.0);
	if (decimal::gt(pInfo._volume*diff, 0))//当前持仓和仓位变化方向一致, 增加一条明细, 增加数量即可
	{
		pInfo._volume = qty;

		//如果T+1，则冻结仓位要增加
		if (commInfo->isT1())
		{
			//ASSERT(diff>0);
			pInfo._frozen += diff;
			log_debug("{} frozen position up to {}", stdCode, pInfo._frozen);
		}
		
		if (_slippage != 0)
		{
			if (_ratio_slippage)
			{
				//By Wesley @ 2023.05.05
				//如果是比率滑点，则要根据目标成交价计算
				//得到滑点以后，再根据pricetick做一个修正
				double slp = (_slippage * trdPx / 10000.0);
				slp = round(slp / commInfo->getPriceTick())*commInfo->getPriceTick();

				trdPx += slp * (isBuy ? 1 : -1);
			}
			else
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
		strcpy(dInfo._opentag, userTag);
		dInfo._open_barno = _schedule_times;
		pInfo._details.emplace_back(dInfo);
		pInfo._last_entertime = curTm;

		double fee = _replayer->calc_fee(stdCode, trdPx, abs(diff), 0);
		_fund_info._total_fees += fee;

		log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(diff), userTag, fee, _schedule_times);
	}
	else
	{//持仓方向和仓位变化方向不一致,需要平仓
		double left = abs(diff);
		if (_slippage != 0)
		{
			if (_ratio_slippage)
			{
				//By Wesley @ 2023.05.05
				//如果是比率滑点，则要根据目标成交价计算
				//得到滑点以后，再根据pricetick做一个修正
				double slp = (_slippage * trdPx / 10000.0);
				slp = round(slp / commInfo->getPriceTick())*commInfo->getPriceTick();

				trdPx += slp * (isBuy ? 1 : -1);
			}
			else
				trdPx += _slippage * commInfo->getPriceTick()*(isBuy ? 1 : -1);
		}

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
			_total_closeprofit += profit;
			pInfo._dynprofit = pInfo._dynprofit*dInfo._volume / (dInfo._volume + maxQty);//浮盈也要做等比缩放
			pInfo._last_exittime = curTm;
			_fund_info._total_profit += profit;

			double fee = _replayer->calc_fee(stdCode, trdPx, maxQty, dInfo._opentdate == curTDate ? 2 : 1);
			_fund_info._total_fees += fee;
			//这里写成交记录
			log_trade(stdCode, dInfo._long, false, curTm, trdPx, maxQty, userTag, fee, _schedule_times);
			//这里写平仓记录
			log_close(stdCode, dInfo._long, dInfo._opentime, dInfo._price, curTm, trdPx, maxQty, profit, maxProf, maxLoss, 
				_total_closeprofit - _fund_info._total_fees, dInfo._opentag, userTag, dInfo._open_barno, _schedule_times);

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

			//如果T+1，则冻结仓位要增加
			if (commInfo->isT1())
			{
				pInfo._frozen += left;
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
			dInfo._open_barno = _schedule_times;
			strcpy(dInfo._opentag, userTag);
			pInfo._details.emplace_back(dInfo);
 
			//这里还需要写一笔成交记录
			double fee = _replayer->calc_fee(stdCode, trdPx, abs(left), 0);
			_fund_info._total_fees += fee;
			log_trade(stdCode, dInfo._long, true, curTm, trdPx, abs(left), userTag, fee, _schedule_times);

			pInfo._last_entertime = curTm;
		}
	}
}

WTSKlineSlice* CtaMocker::stra_get_bars(const char* stdCode, const char* period, uint32_t count, bool isMain /* = false */)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);

	thread_local static char basePeriod[2] = { 0 };
	basePeriod[0] = period[0];
	uint32_t times = 1;
	if (strlen(period) > 1)
		times = strtoul(period + 1, NULL, 10);
	else
		strcat(key, "1");

	if (isMain)
	{
		if (_main_key.empty())
			_main_key = key;
		else if (_main_key != key)
		{
			WTSLogger::error("Main k bars can only be setup once");
			return NULL;
		}

		/*
		 *	By Wesley @ 2022.07.16
		 */
		_main_code = stdCode;
		_main_period = period;
	}

	WTSKlineSlice* kline = _replayer->get_kline_slice(stdCode, basePeriod, count, times, isMain);

	KlineTag& tag = _kline_tags[key];
	tag._closed = false;

	if (kline)
	{
		//double lastClose = kline->close(-1);
		CodeHelper::CodeInfo cInfo = CodeHelper::extractStdCode(stdCode, _replayer->get_hot_mgr());
		WTSCommodityInfo* commInfo = _replayer->get_commodity_info(stdCode);
		std::string realCode = stdCode;
		if(cInfo.isExright())
			realCode = realCode.substr(0, realCode.size()-1);
		_replayer->sub_tick(id(), realCode.c_str());
	}

	return kline;
}

WTSTickSlice* CtaMocker::stra_get_ticks(const char* stdCode, uint32_t count)
{
	return _replayer->get_tick_slice(stdCode, count);
}

WTSTickData* CtaMocker::stra_get_last_tick(const char* stdCode)
{
	auto it = _ticks.find(stdCode);
	if (it != _ticks.end())
	{
		WTSTickData* lastTick = WTSTickData::create((WTSTickStruct&)it->second);
		return lastTick;
	}

	return _replayer->get_last_tick(stdCode);
}

void CtaMocker::stra_sub_ticks(const char* code)
{
	/*
	 *	By Wesley @ 2022.03.01
	 *	主动订阅tick会在本地记一下
	 *	tick数据回调的时候先检查一下
	 */
	_tick_subs.insert(code);

	_replayer->sub_tick(_context_id, code);
}

void CtaMocker::stra_sub_bar_events(const char* stdCode, const char* period)
{
	thread_local static char key[64] = { 0 };
	fmtutil::format_to(key, "{}#{}", stdCode, period);

	KlineTag& tag = _kline_tags[key];
	tag._notify = true;
}

WTSCommodityInfo* CtaMocker::stra_get_comminfo(const char* stdCode)
{
	return _replayer->get_commodity_info(stdCode);
}

std::string CtaMocker::stra_get_rawcode(const char* stdCode)
{
	return _replayer->get_rawcode(stdCode);
}

uint32_t CtaMocker::stra_get_tdate()
{
	return _replayer->get_trading_date();
}

uint32_t CtaMocker::stra_get_date()
{
	return _replayer->get_date();
}

uint32_t CtaMocker::stra_get_time()
{
	return _replayer->get_min_time();
}

double CtaMocker::stra_get_fund_data(int flag)
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

void CtaMocker::stra_log_info(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_INFO, message);
}

void CtaMocker::stra_log_debug(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_DEBUG, message);
}

void CtaMocker::stra_log_warn(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_WARN, message);
}

void CtaMocker::stra_log_error(const char* message)
{
	WTSLogger::log_dyn_raw("strategy", _name.c_str(), LL_ERROR, message);
}

const char* CtaMocker::stra_load_user_data(const char* key, const char* defVal /*= ""*/)
{
	auto it = _user_datas.find(key);
	if (it != _user_datas.end())
		return it->second.c_str();

	return defVal;
}

void CtaMocker::stra_save_user_data(const char* key, const char* val)
{
	_user_datas[key] = val;
	_ud_modified = true;
}

uint64_t CtaMocker::stra_get_first_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[0]._opentime;
}

uint64_t CtaMocker::stra_get_last_entertime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[pInfo._details.size() - 1]._opentime;
}

const char* CtaMocker::stra_get_last_entertag(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return "";

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return "";

	return pInfo._details[pInfo._details.size() - 1]._opentag;
}

uint64_t CtaMocker::stra_get_last_exittime(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._last_exittime;
}

double CtaMocker::stra_get_last_enterprice(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	if (pInfo._details.empty())
		return 0;

	return pInfo._details[pInfo._details.size() - 1]._price;
}

double CtaMocker::stra_get_position(const char* stdCode, bool bOnlyValid /* = false */, const char* userTag /* = "" */)
{
	//By Wesley @ 2022.05.22
	//如果有信号，说明刚下了指令，还没等到下一个tick进来，用户就在读取仓位
	double totalPos = 0;
	auto sit = _sig_map.find(stdCode);
	if (sit != _sig_map.end())
	{
		totalPos = sit->second._volume;
	}

	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return totalPos;

	const PosInfo& pInfo = it->second;
	totalPos = pInfo._volume;

	if (strlen(userTag) == 0)
	{
		if (bOnlyValid)
		{
			//只有userTag为空的时候时候，才会用bOnlyValid
			//这里理论上，只有多头才会进到这里
			//其他地方要保证，空头持仓的话，_frozen要为0
			return totalPos - pInfo._frozen;
		}
		else
			return totalPos;
	}
	else
	{
		for (auto it = pInfo._details.begin(); it != pInfo._details.end(); it++)
		{
			const DetailInfo& dInfo = (*it);
			if (strcmp(dInfo._opentag, userTag) != 0)
				continue;

			return dInfo._volume;
		}
	}

	return 0;
}

double CtaMocker::stra_get_position_avgpx(const char* stdCode)
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

double CtaMocker::stra_get_position_profit(const char* stdCode)
{
	auto it = _pos_map.find(stdCode);
	if (it == _pos_map.end())
		return 0;

	const PosInfo& pInfo = it->second;
	return pInfo._dynprofit;
}

uint64_t CtaMocker::stra_get_detail_entertime(const char* stdCode, const char* userTag)
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

double CtaMocker::stra_get_detail_cost(const char* stdCode, const char* userTag)
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

double CtaMocker::stra_get_detail_profit(const char* stdCode, const char* userTag, int flag /* = 0 */)
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

void CtaMocker::set_chart_kline(const char* stdCode, const char* period)
{
	_chart_code = stdCode;
	_chart_period = period;
}

void CtaMocker::add_chart_mark(double price, const char* icon, const char* tag)
{
	if (!_is_in_schedule)
	{
		WTSLogger::error("Marks can be added only during schedule");
		return;
	}

	uint64_t curTime = _replayer->get_date();
	curTime = curTime*10000 + _replayer->get_min_time();

	_mark_logs << curTime << "," << price << "," << icon << "," << tag << std::endl;
}

void CtaMocker::register_index(const char* idxName, uint32_t indexType)
{
	ChartIndex& cIndex = _chart_indice[idxName];
	cIndex._name = idxName;
	cIndex._indexType = indexType;
}

bool CtaMocker::register_index_line(const char* idxName, const char* lineName, uint32_t lineType)
{
	auto it = _chart_indice.find(idxName);
	if (it == _chart_indice.end())
	{
		WTSLogger::error("Index {} not registered", idxName);
		return false;
	}

	ChartIndex& cIndex = it->second;
	ChartLine& cLine = cIndex._lines[lineName];
	cLine._name = lineName;
	cLine._lineType = lineType;
	return true;
}

bool CtaMocker::add_index_baseline(const char* idxName, const char* lineName, double val)
{
	auto it = _chart_indice.find(idxName);
	if (it == _chart_indice.end())
	{
		WTSLogger::error("Index {} not registered", idxName);
		return false;
	}

	ChartIndex& cIndex = it->second;
	cIndex._base_lines[lineName] = val;
	return true;
}

bool CtaMocker::set_index_value(const char* idxName, const char* lineName, double val)
{
	if (!_is_in_schedule)
	{
		WTSLogger::error("Marks can be added only during schedule");
		return false;
	}

	auto ait = _chart_indice.find(idxName);
	if (ait == _chart_indice.end())
	{
		WTSLogger::error("Index {} not registered", idxName);
		return false;
	}

	ChartIndex& cIndex = ait->second;
	auto bit = cIndex._lines.find(lineName);
	if (bit == cIndex._lines.end())
	{
		WTSLogger::error("Line {} of index {} not registered", lineName, idxName);
		return false;
	}

	uint64_t curTime = _replayer->get_date();
	curTime = curTime * 10000 + _replayer->get_min_time();
	_index_logs << curTime << "," << idxName << "," << lineName << "," << val << std::endl;
	return true;
}

