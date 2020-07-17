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
#include "WtHftEngine.h"
#include "WtHelper.h"

#include "../Share/CodeHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/StdUtils.hpp"
#include "../WTSTools/WTSLogger.h"
#include "../WTSTools/WTSHotMgr.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

USING_NS_OTP;

extern uint32_t makeCtxId();

HftStraBaseCtx::HftStraBaseCtx(WtHftEngine* engine, const char* name)
	: IHftStraCtx(name)
	, _engine(engine)
{
	_context_id = makeCtxId();
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

void HftStraBaseCtx::on_init()
{
	
}

void HftStraBaseCtx::on_tick(const char* code, WTSTickData* newTick)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_bar(const char* code, const char* period, uint32_t times, WTSBarStruct* newBar)
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

OrderIDs HftStraBaseCtx::stra_cancel(const char* code, bool isBuy, double qty)
{
	return _trader->cancel(code, isBuy, qty);
}

const char* HftStraBaseCtx::get_inner_code(const char* stdCode)
{
	auto it = _code_map.find(stdCode);
	if (it == _code_map.end())
		return stdCode;

	return it->second.c_str();
}

OrderIDs HftStraBaseCtx::stra_buy(const char* stdCode, double price, double qty)
{
	if(CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdCode(stdCode, cInfo);
		std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
		std::string realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->buy(realCode.c_str(), price, qty);
	}
	else
	{
		return _trader->buy(stdCode, price, qty);
	}
}

OrderIDs HftStraBaseCtx::stra_sell(const char* stdCode, double price, double qty)
{
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdCode(stdCode, cInfo);
		std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
		std::string realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->sell(realCode.c_str(), price, qty);
	}
	else
	{
		return _trader->sell(stdCode, price, qty);
	}
}

WTSCommodityInfo* HftStraBaseCtx::stra_get_comminfo(const char* code)
{
	return _engine->get_commodity_info(code);
}

WTSKlineSlice* HftStraBaseCtx::stra_get_bars(const char* code, const char* period, uint32_t count)
{
	return _engine->get_kline_slice(_context_id, code, period, count);
}

WTSTickSlice* HftStraBaseCtx::stra_get_ticks(const char* code, uint32_t count)
{
	WTSTickSlice* ticks = _engine->get_tick_slice(_context_id, code, count);

	return ticks;
}

WTSTickData* HftStraBaseCtx::stra_get_last_tick(const char* code)
{
	return _engine->get_last_tick(_context_id, code);
}

void HftStraBaseCtx::stra_sub_ticks(const char* code)
{
	_engine->sub_tick(id(), code);
}

void HftStraBaseCtx::stra_log_text(const char* fmt, ...)
{
	char szBuf[256] = { 0 };
	uint32_t length = sprintf(szBuf, "[%s]", _name.c_str());
	strcat(szBuf, fmt);
	va_list args;
	va_start(args, fmt);
	WTSLogger::log2_direct("strategy", LL_INFO, szBuf, args);
	va_end(args);
}

void HftStraBaseCtx::on_trade(const char* stdCode, bool isBuy, double vol, double price)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_order(uint32_t localid, const char* stdCode, bool isBuy, double totalQty, double leftQty, double price, bool isCanceled /* = false */)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
	}
}

void HftStraBaseCtx::on_position(const char* stdCode, bool isLong, double prevol, double preavail, double newvol, double newavail)
{
	if (_ud_modified)
	{
		save_userdata();
		_ud_modified = false;
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

double HftStraBaseCtx::stra_get_position(const char* stdCode)
{
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdCode(stdCode, cInfo);
		std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
		std::string realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->getPosition(realCode.c_str());
	}
	else
	{
		return _trader->getPosition(stdCode);
	}
}

double HftStraBaseCtx::stra_get_undone(const char* stdCode)
{
	if (CodeHelper::isStdFutHotCode(stdCode))
	{
		CodeHelper::CodeInfo cInfo;
		CodeHelper::extractStdCode(stdCode, cInfo);
		std::string code = _engine->get_hot_mgr()->getRawCode(cInfo._exchg, cInfo._product, _engine->get_trading_date());
		std::string realCode = CodeHelper::bscFutCodeToStdCode(code.c_str(), cInfo._exchg);

		_code_map[realCode] = stdCode;

		return _trader->getUndoneQty(realCode.c_str());
	}
	else
	{
		return _trader->getUndoneQty(stdCode);
	}
}

double HftStraBaseCtx::stra_get_price(const char* code)
{
	return _engine->get_cur_price(code);
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