/*!
 * \file DataManager.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据管理器定义
 */
#pragma once

#include "../Includes/IDataWriter.h"
#include "../Share/StdUtils.hpp"
#include "../Share/BoostMappingFile.hpp"

NS_WTP_BEGIN
class WTSTickData;
class WTSOrdQueData;
class WTSOrdDtlData;
class WTSTransData;
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

class WTSBaseDataMgr;
class StateMonitor;
class UDPCaster;

class DataManager : public IDataWriterSink
{
public:
	DataManager();
	~DataManager();

public:
	bool init(WTSVariant* params, WTSBaseDataMgr* bdMgr, StateMonitor* stMonitor, UDPCaster* caster = NULL);

	void add_ext_dumper(const char* id, IHisDataDumper* dumper);

	void release();

	bool writeTick(WTSTickData* curTick, uint32_t procFlag);

	bool writeOrderQueue(WTSOrdQueData* curOrdQue);

	bool writeOrderDetail(WTSOrdDtlData* curOrdDetail);

	bool writeTransaction(WTSTransData* curTrans);

	void transHisData(const char* sid);
	
	bool isSessionProceeded(const char* sid);

	WTSTickData* getCurTick(const char* code, const char* exchg = "");

public:
	//////////////////////////////////////////////////////////////////////////
	//IDataWriterSink
	virtual IBaseDataMgr* getBDMgr() override;

	virtual bool canSessionReceive(const char* sid) override;

	virtual void broadcastTick(WTSTickData* curTick) override;

	virtual void broadcastOrdQue(WTSOrdQueData* curOrdQue) override;

	virtual void broadcastOrdDtl(WTSOrdDtlData* curOrdDtl) override;

	virtual void broadcastTrans(WTSTransData* curTrans) override;

	virtual CodeSet* getSessionComms(const char* sid) override;

	virtual uint32_t getTradingDate(const char* pid) override;

	/*
	*	处理解析模块的日志
	*	@ll			日志级别
	*	@message	日志内容
	*/
	virtual void outputLog(WTSLogLevel ll, const char* message) override;

private:
	IDataWriter*		_writer;
	FuncDeleteWriter	_remover;
	WTSBaseDataMgr*		_bd_mgr;
	StateMonitor*		_state_mon;
	UDPCaster*			_udp_caster;
};

