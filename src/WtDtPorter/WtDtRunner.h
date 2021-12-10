/*!
 * \file WtDtRunner.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"
#include "ExpDumper.h"

#include "../WtDtCore/DataManager.h"
#include "../WtDtCore/ParserAdapter.h"
#include "../WtDtCore/StateMonitor.h"
#include "../WtDtCore/UDPCaster.h"

#include "../WTSTools/WTSHotMgr.h"
#include "../WTSTools/WTSBaseDataMgr.h"

#include <boost/asio.hpp>

NS_OTP_BEGIN
class WTSVariant;
NS_OTP_END

class WtDtRunner
{
public:
	WtDtRunner();
	~WtDtRunner();

public:
	void	initialize(const char* cfgFile, const char* logCfg, const char* modDir = "");
	void	start(bool bAsync = false);

	bool	createExtParser(const char* id);


//////////////////////////////////////////////////////////////////////////
//扩展Parser
public:
	void registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	void parser_init(const char* id);
	void parser_connect(const char* id);
	void parser_release(const char* id);
	void parser_disconnect(const char* id);
	void parser_subscribe(const char* id, const char* code);
	void parser_unsubscribe(const char* id, const char* code);

	void on_parser_quote(const char* id, WTSTickStruct* curTick, bool bNeedSlice = true);

//////////////////////////////////////////////////////////////////////////
//扩展Dumper
public:
	bool createExtDumper(const char* id);

	void registerExtDumper(FuncDumpBars barDumper, FuncDumpTicks tickDumper);

	void registerExtHftDataDumper(FuncDumpOrdQue ordQueDumper, FuncDumpOrdDtl ordDtlDumper, FuncDumpTrans transDumper);

	bool dumpHisBars(const char* id, const char* stdCode, const char* period, WTSBarStruct* bars, uint32_t count);

	bool dumpHisTicks(const char* id, const char* stdCode, uint32_t uDate, WTSTickStruct* ticks, uint32_t count);

	bool dumpHisOrdQue(const char* id, const char* stdCode, uint32_t uDate, WTSOrdQueStruct* item, uint32_t count);

	bool dumpHisOrdDtl(const char* id, const char* stdCode, uint32_t uDate, WTSOrdDtlStruct* items, uint32_t count);

	bool dumpHisTrans(const char* id, const char* stdCode, uint32_t uDate, WTSTransStruct* items, uint32_t count);

private:
	void initDataMgr(WTSVariant* config);
	void initParsers(const char* filename);

private:

	WTSBaseDataMgr	_bd_mgr;
	WTSHotMgr		_hot_mgr;
	boost::asio::io_service _async_io;
	StateMonitor	_state_mon;
	UDPCaster		_udp_caster;
	DataManager		_data_mgr;
	ParserAdapterMgr	_parsers;

	FuncParserEvtCallback	_cb_parser_evt;
	FuncParserSubCallback	_cb_parser_sub;

	FuncDumpBars	_dumper_for_bars;
	FuncDumpTicks	_dumper_for_ticks;

	FuncDumpOrdQue	_dumper_for_ordque;
	FuncDumpOrdDtl	_dumper_for_orddtl;
	FuncDumpTrans	_dumper_for_trans;

	typedef std::shared_ptr<ExpDumper> ExpDumperPtr;
	typedef std::map<std::string, ExpDumperPtr>  ExpDumpers;
	ExpDumpers		_dumpers;
};

