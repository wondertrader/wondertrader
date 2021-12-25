#pragma once
#include "../Includes/IDataWriter.h"

USING_NS_WTP;

class ExpDumper : public IHisDataDumper
{
public:
	ExpDumper(const char* id) :_id(id) {}
	virtual ~ExpDumper() {}

public:
	virtual bool dumpHisBars(const char* stdCode, const char* period, WTSBarStruct* bars, uint32_t count) override;

	virtual bool dumpHisTicks(const char* stdCode, uint32_t uDate, WTSTickStruct* ticks, uint32_t count) override;

	virtual bool dumpHisOrdQue(const char* stdCode, uint32_t uDate, WTSOrdQueStruct* items, uint32_t count) override;

	virtual bool dumpHisOrdDtl(const char* stdCode, uint32_t uDate, WTSOrdDtlStruct* items, uint32_t count) override;

	virtual bool dumpHisTrans(const char* stdCode, uint32_t uDate, WTSTransStruct* items, uint32_t count) override;

private:
	std::string	_id;
};

