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
	void	start();

	bool	createExtParser(const char* id);

//////////////////////////////////////////////////////////////////////////
//À©Õ¹Parser
public:
	void registerParserPorter(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	void parser_init(const char* id);
	void parser_connect(const char* id);
	void parser_release(const char* id);
	void parser_disconnect(const char* id);
	void parser_subscribe(const char* id, const char* code);
	void parser_unsubscribe(const char* id, const char* code);

	void on_parser_quote(const char* id, WTSTickStruct* curTick, bool bNeedSlice = true);

private:
	void	initDataMgr(WTSVariant* config);
	void	initParsers(const char* filename);

private:

	WTSBaseDataMgr	m_baseDataMgr;
	WTSHotMgr		m_hotMgr;
	boost::asio::io_service m_asyncIO;
	StateMonitor	m_stateMon;
	UDPCaster		m_udpCaster;
	DataManager		m_dataMgr;
	ParserAdapterMgr	m_parsers;

	FuncParserEvtCallback	_cb_parser_evt;
	FuncParserSubCallback	_cb_parser_sub;
};

