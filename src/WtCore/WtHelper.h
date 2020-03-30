#pragma once
#include <string>
#include <stdint.h>

class WtHelper
{
public:
	static std::string getCWD();

	static std::string getTraderModule(const char* moduleName);
	static std::string getParserModule(const char* moduleName);

	static const char* getBaseDir();
	static const char* getOutputDir();
	static const char* getStraDataDir();
	static const char* getStraUsrDatDir();

	static const char* getPortifolioDir();

	static void setTime(uint32_t date, uint32_t time, uint32_t secs = 0)
	{
		_cur_date = date;
		_cur_time = time;
		_cur_secs = secs;
	}

	static void setTDate(uint32_t tDate){ _cur_tdate = tDate; }

	static uint32_t getDate(){ return _cur_date; }
	static uint32_t getTime(){ return _cur_time; }
	static uint32_t getSecs(){ return _cur_secs; }
	static uint32_t getTradingDate(){ return _cur_tdate; }

private:
	static uint32_t		_cur_date;	//当前日期
	static uint32_t		_cur_time;	//当前时间, 以分钟为准
	static uint32_t		_cur_secs;	//当前秒数, 包含毫秒
	static uint32_t		_cur_tdate;	//当前交易日
};

