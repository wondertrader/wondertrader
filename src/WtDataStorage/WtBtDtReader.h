#pragma once
#include <string>
#include <stdint.h>

#include "DataDefine.h"

#include "../Includes/FasterDefs.h"
#include "../Includes/IBtDtReader.h"

#include "../Share/BoostMappingFile.hpp"
#include "../Share/StdUtils.hpp"

NS_WTP_BEGIN
class WTSVariant;
class WTSTickSlice;
class WTSKlineSlice;
class WTSOrdDtlSlice;
class WTSOrdQueSlice;
class WTSTransSlice;
class WTSArray;

class IBaseDataMgr;
class IHotMgr;
typedef std::shared_ptr<BoostMappingFile> BoostMFPtr;

class WtBtDtReader : public IBtDtReader
{
public:
	WtBtDtReader();
	virtual ~WtBtDtReader();	

//////////////////////////////////////////////////////////////////////////
//IBtDtReader
public:
	virtual void init(WTSVariant* cfg, IBtDtReaderSink* sink);

	virtual bool read_raw_bars(const char* exchg, const char* code, WTSKlinePeriod period, std::string& buffer) override;
	virtual bool read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer) override;

private:
	std::string		_base_dir;
};

NS_WTP_END