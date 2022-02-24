#include "WtBtDtReader.h"

#include "../Includes/WTSVariant.hpp"
#include "../Share/StrUtil.hpp"
#include "../WTSUtils/WTSCmpHelper.hpp"

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void pipe_btreader_log(IBtDtReaderSink* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	memset(buffer, 0, 512);
	fmt::format_to(buffer, format, args...);

	sink->reader_log(ll, buffer);
}

extern bool proc_block_data(std::string& content, bool isBar, bool bKeepHead = true);

extern "C"
{
	EXPORT_FLAG IBtDtReader* createBtDtReader()
	{
		IBtDtReader* ret = new WtBtDtReader();
		return ret;
	}

	EXPORT_FLAG void deleteBtDtReader(IBtDtReader* reader)
	{
		if (reader != NULL)
			delete reader;
	}
};

/*
 *	处理块数据
 */
extern bool proc_block_data(std::string& content, bool isBar, bool bKeepHead);

WtBtDtReader::WtBtDtReader()
{
}


WtBtDtReader::~WtBtDtReader()
{

}

void WtBtDtReader::init(WTSVariant* cfg, IBtDtReaderSink* sink)
{
	_sink = sink;

	if (cfg == NULL)
		return;

	_base_dir = cfg->getCString("path");
	_base_dir = StrUtil::standardisePath(_base_dir);

	pipe_btreader_log(_sink, LL_INFO, "WtBtDtReader initialized, root data dir is {}", _base_dir);
}

bool WtBtDtReader::read_raw_bars(const char* exchg, const char* code, WTSKlinePeriod period, std::string& buffer)
{
	std::stringstream ss;
	ss << _base_dir << "his/" << PERIOD_NAME[period] << "/" << exchg << "/" << code << ".dsb";
	std::string filename = ss.str();
	if (!StdFile::exists(filename.c_str()))
	{
		pipe_btreader_log(_sink, LL_WARN, "Back {} data file {} not exists", PERIOD_NAME[period], filename);
		return false;
	}

	pipe_btreader_log(_sink, LL_DEBUG, "Reading back {} bars from file {}...", PERIOD_NAME[period], filename);
	StdFile::read_file_content(filename.c_str(), buffer);
	bool bSucc = proc_block_data(buffer, true, false);
	if(!bSucc)
		pipe_btreader_log(_sink, LL_ERROR, "Processing back {} data from file {} failed", PERIOD_NAME[period], filename);

	return bSucc;
}

bool WtBtDtReader::read_raw_ticks(const char* exchg, const char* code, uint32_t uDate, std::string& buffer)
{
	std::stringstream ss;
	ss << _base_dir << "his/ticks/" << exchg << "/" << uDate << "/" << code << ".dsb";
	std::string filename = ss.str();
	if (!StdFile::exists(filename.c_str()))
	{
		pipe_btreader_log(_sink, LL_WARN, "Back tick data file {} not exists", filename);
		return false;
	}

	StdFile::read_file_content(filename.c_str(), buffer);
	bool bSucc = proc_block_data(buffer, false, false);
	if (!bSucc)
		pipe_btreader_log(_sink, LL_ERROR, "Processing back tick data from file {} failed", filename);

	return bSucc;
}