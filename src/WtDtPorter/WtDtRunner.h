#pragma once
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
	void	initialize(const char* cfgFile, const char* logCfg);
	void	start();

private:
	void	initDataMgr(WTSVariant* config);
	void	initParsers(WTSVariant* config);

private:

	WTSBaseDataMgr	m_baseDataMgr;
	WTSHotMgr		m_hotMgr;
	boost::asio::io_service m_asyncIO;
	StateMonitor	m_stateMon;
	UDPCaster		m_udpCaster;
	DataManager		m_dataMgr;
};

