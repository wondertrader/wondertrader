/*!
 * \file TimeUtils.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 时间处理的封装
 */
#pragma once
#include <stdint.h>
#include <sys/timeb.h>
#ifdef _MSC_VER
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <string>
#include <string.h>
#include<chrono>
/*
#define CTIME_BUF_SIZE 64

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

typedef struct _KSYSTEM_TIME
{
	ULONG LowPart;
	LONG High1Time;
	LONG High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;

struct KUSER_SHARED_DATA
{
	ULONG TickCountLowDeprecated;
	ULONG TickCountMultiplier;
	volatile KSYSTEM_TIME InterruptTime;
	volatile KSYSTEM_TIME SystemTime;
	volatile KSYSTEM_TIME TimeZoneBias;
};

#define KI_USER_SHARED_DATA   0x7FFE0000
#define SharedUserData   ((KUSER_SHARED_DATA * const)KI_USER_SHARED_DATA)

#define TICKSPERSEC        10000000L
*/

class TimeUtils 
{
	
public:

	//static int64_t GetSysTime()
	//{
	//	LARGE_INTEGER SystemTime;
	//	do
	//	{
	//		SystemTime.HighPart = SharedUserData->SystemTime.High1Time;
	//		SystemTime.LowPart = SharedUserData->SystemTime.LowPart;
	//	} 
	//	while (SystemTime.HighPart != SharedUserData->SystemTime.High2Time);

	//	return SystemTime.QuadPart;
	//}

	//static uint64_t mtime()
	//{
	//	uint64_t t = GetSysTime();
	//	t = t - 11644473600L * TICKSPERSEC;
	//	return t;
	//}

	static inline int64_t getLocalTimeNow(void)
	{
		timeb now;
		ftime(&now);
		return now.time * 1000 + now.millitm;
		//return mtime() / 10000;
	}

	static inline int64_t getLocalTimeNano(void)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	static inline std::string getLocalTime(bool bIncludeMilliSec = true)
	{
		timeb now;
		ftime(&now);
		tm * tNow = localtime(&(now.time));

		char str[64] = {0};
		if(bIncludeMilliSec)
			sprintf(str, "%02d:%02d:%02d,%03d", tNow->tm_hour, tNow->tm_min, tNow->tm_sec, now.millitm);
		else
			sprintf(str, "%02d:%02d:%02d", tNow->tm_hour, tNow->tm_min, tNow->tm_sec);
		return str;
	}

	/*
	static inline int64_t getNowFreqCount(void){
		_LARGE_INTEGER count;    	
		QueryPerformanceCounter(&count);
		return count.QuadPart;
	}
	*/

	static inline std::string now(void) 
	{
		using namespace std; // For time_t, time and ctime;
		time_t now = time(0);
		std::string str = ctime(&now);
		return str;
	}

	/*
	static inline int64_t getFrequency(void){              
		LARGE_INTEGER f;              //计时器频率
		QueryPerformanceFrequency(&f);
		return (int64_t)f.QuadPart;
	}
	*/

	static inline std::string getYYYYMMDD(void)
	{
		std::string yyyymmdd;
		tm local;
		time_t now;
		time(&now);
#ifdef _WIN32
		localtime_s(&local, &now);
#else
		localtime_r(&now, &local);
#endif
		char year[5];
		sprintf(year, "%d", local.tm_year + 1900);
		char month[3];
		sprintf(month, "%02d",local.tm_mon+1);
		char day[3];
		sprintf(day, "%02d",local.tm_mday);
		std::string ofilename;
		yyyymmdd.append(year);
		yyyymmdd.append(month);
		yyyymmdd.append(day);
		return yyyymmdd;
	}

	static inline uint64_t getYYYYMMDDhhmmss()
	{
		timeb now;
		ftime(&now);

		tm * tNow = localtime(&(now.time));

		uint64_t date = (tNow->tm_year + 1900) * 10000 + (tNow->tm_mon + 1) * 100 + tNow->tm_mday;

		uint64_t time = tNow->tm_hour * 10000 + tNow->tm_min * 100 + tNow->tm_sec;
		return date * 1000000 + time;
	}

    /*
     * 读取当前时间
     * @date    当前日期，格式如20220309
     * @time    当前时间，精确到毫秒，格式如103029500
     */
	static inline void getDateTime(uint32_t &date, uint32_t &time)
	{
		timeb now;
		ftime(&now);

		tm * tNow = localtime(&(now.time));

		date = (tNow->tm_year+1900)*10000 + (tNow->tm_mon+1)*100 + tNow->tm_mday;
		
		time = tNow->tm_hour*10000 + tNow->tm_min*100 + tNow->tm_sec;
		time *= 1000;
		time += now.millitm;
	}

	static inline uint32_t getCurDate()
	{
		timeb now;
		ftime(&now);

		tm * tNow = localtime(&(now.time));

		uint32_t date = (tNow->tm_year+1900)*10000 + (tNow->tm_mon+1)*100 + tNow->tm_mday;

		return date;
	}

	static inline uint32_t getWeekDay(uint32_t uDate = 0)
	{
		time_t ts = 0;
		if(uDate == 0)
		{
			timeb now;
			ftime(&now);
			ts = now.time;
		}
		else
		{
			tm t;	
			memset(&t,0,sizeof(tm));
			t.tm_year = uDate/10000 - 1900;
			t.tm_mon = (uDate%10000)/100 - 1;
			t.tm_mday = uDate % 100;
			ts = mktime(&t);
		}

		tm * tNow = localtime(&ts);
	
		return tNow->tm_wday;
	}

	static inline uint32_t getCurMin()
	{
		timeb now;
		ftime(&now);

		tm * tNow = localtime(&(now.time));

		uint32_t time = tNow->tm_hour*10000 + tNow->tm_min*100 + tNow->tm_sec;

		return time;
	}

	static inline std::string getYYYYMMDD_hhmmss(void)
	{
		std::string datetime_fmt_str = "";
		tm local;
		time_t now;
		time(&now);
#ifdef _WIN32
		localtime_s(&local, &now);
#else
		localtime_r(&now, &local);
#endif
		char year[5] = {'\0'};
		sprintf(year, "%d", local.tm_year + 1900);
		char month[3]  = {'\0'};
		sprintf(month, "%02d",local.tm_mon+1);
		char day[3]  = {'\0'};
		sprintf(day, "%02d",local.tm_mday);
		char hh[3]  = {'\0'};
		sprintf(hh, "%02d", local.tm_hour);
		char mm[3]  = {'\0'};
		sprintf(mm, "%02d", local.tm_min);
		char ss[3]  = {'\0'};
		sprintf(ss, "%02d", local.tm_sec);
		std::string ofilename;
		datetime_fmt_str.append(year);
		datetime_fmt_str.append(month);
		datetime_fmt_str.append(day);
		datetime_fmt_str.append("_");
		datetime_fmt_str.append(hh);
		datetime_fmt_str.append(mm);
		datetime_fmt_str.append(ss);
		return datetime_fmt_str;
	}

	//20120512 09:15:00 -> 毫秒
	//支持如下格式的字符串: 
	// 20120512 09:15:00 or 20120512 09:15:00 999
	// 20120512091500 or 20120512091500999
	static inline int64_t makeTime(std::string time_str)
	{
	    //time_str = StringUtils::trim(time_str, ' ');
		//time_str = StringUtils::trim(time_str, ':');
		uint32_t len = (uint32_t)time_str.size();
		if (len < 14) return 0;
		tm t;	
		memset(&t,0,sizeof(tm));
		t.tm_year = atoi(time_str.substr(0, 4).c_str()) - 1900;
		t.tm_mon = atoi(time_str.substr(4,2).c_str()) - 1;
		t.tm_mday = atoi(time_str.substr(6,2).c_str());
		t.tm_hour = atoi(time_str.substr(8,2).c_str());
		t.tm_min = atoi(time_str.substr(10,2).c_str());
		t.tm_sec = atoi(time_str.substr(12,2).c_str());
		int millisec = 0;
		if ( len == 17){ //说明还有毫秒
			millisec = atoi(time_str.substr(14,3).c_str());
		}
		//t.tm_isdst 	
		time_t ts = mktime(&t);
		if (ts == -1) return 0;
		return ts * 1000+ millisec;
	}

	static inline int32_t getTZOffset()
	{
		static int32_t offset = 99;
		if(offset == 99)
		{
			time_t now = time(NULL);
			tm tm_ltm = *localtime(&now);
			tm tm_gtm = *gmtime(&now);

			time_t _gt = mktime(&tm_gtm);
			tm _gtm2 = *localtime(&_gt);

			offset = (uint32_t)(((now - _gt) + (_gtm2.tm_isdst ? 3600 : 0)) / 60);
			offset /= 60;
		}

		return offset;
	}

	static inline int64_t makeTime(long lDate, long lTimeWithMs, bool isGM = false)
	{
		tm t;	
		memset(&t,0,sizeof(tm));
		t.tm_year = lDate/10000 - 1900;
		t.tm_mon = (lDate%10000)/100 - 1;
		t.tm_mday = lDate % 100;
		t.tm_hour = lTimeWithMs/10000000;
		t.tm_min = (lTimeWithMs%10000000)/100000;
		t.tm_sec = (lTimeWithMs%100000)/1000;
		int millisec = lTimeWithMs%1000;
		//t.tm_isdst 	
		time_t ts = mktime(&t);
		if (isGM)
			ts -= getTZOffset() * 3600;
		if (ts == -1) return 0;
		return ts * 1000+ millisec;
	}

	static std::string timeToString(int64_t mytime)
	{
		if (mytime == 0) return "";
		int64_t sec = mytime/1000;
		int msec = (int) (mytime - sec * 1000);
		if (msec < 0) return "";
		time_t tt =  sec;
		struct tm t;
#ifdef _WIN32
		localtime_s(&t, &tt);
#else
		localtime_r(&tt, &t);
#endif
		char tm_buf[64] = {'\0'};
		if (msec > 0) //是否有毫秒
		   sprintf(tm_buf,"%4d%02d%02d%02d%02d%02d.%03d",t.tm_year+1900, t.tm_mon+1, t.tm_mday,
			t.tm_hour, t.tm_min, t.tm_sec, msec);
		else 
		   sprintf(tm_buf,"%4d%02d%02d%02d%02d%02d",t.tm_year+1900, t.tm_mon+1, t.tm_mday,
			t.tm_hour, t.tm_min, t.tm_sec);
		return tm_buf;
	};

	static uint32_t getNextDate(uint32_t curDate, int days = 1)
	{
		tm t;	
		memset(&t,0,sizeof(tm));
		t.tm_year = curDate/10000 - 1900;
		t.tm_mon = (curDate%10000)/100 - 1;
		t.tm_mday = curDate % 100;
		//t.tm_isdst 	
		time_t ts = mktime(&t);
		ts += days*86400;

		tm* newT = localtime(&ts);
		return (newT->tm_year+1900)*10000 + (newT->tm_mon+1)*100 + newT->tm_mday;
	}

	static uint32_t getNextMinute(int32_t curTime, int32_t mins = 1)
	{
		int32_t curHour = curTime / 100;
		int32_t curMin = curTime % 100;
		int32_t totalMins = curHour * 60 + curMin;
		totalMins += mins;

		if (totalMins >= 1440)
			totalMins -= 1440;
		else if (totalMins < 0)
			totalMins += 1440;

		int32_t ret = (totalMins / 60) * 100 + totalMins % 60;
		return (uint32_t)ret;
	}

	static uint32_t getNextMonth(uint32_t curMonth, int months = 1)
	{
		uint32_t uYear = curMonth/100;
		uint32_t uMonth = curMonth%100;

		uint32_t uAddYear = months/12;
		uint32_t uAddMon = months%12;

		uYear += uAddYear;
		uMonth += uAddMon;
		if(uMonth > 12)
		{
			uYear ++;
			uMonth -= 12;
		}
		else if(uMonth <= 0)
		{
			uYear --;
			uMonth = 12;
		}

		return uYear*100 + uMonth;
	}

	static inline uint64_t timeToMinBar(uint32_t uDate, uint32_t uTime)
	{
		return (uint64_t)((uDate-19900000)*10000) + uTime;
	}

	static inline uint32_t minBarToDate(uint64_t minTime)
	{
		return (uint32_t)(minTime/10000 + 19900000);
	}

	static inline uint32_t minBarToTime(uint64_t minTime)
	{
		return (uint32_t)(minTime%10000);
	}

	static inline bool isWeekends(uint32_t uDate)
	{
		tm t;	
		memset(&t,0,sizeof(tm));
		t.tm_year = uDate/1/10000 - 1900;
		t.tm_mon = (uDate/1%10000)/100 - 1;
		t.tm_mday = uDate/1 % 100;

		time_t tt = mktime(&t);
		tm* tmt = localtime(&tt);
		if(tmt == NULL)
			return true;
	
		if(tmt->tm_wday == 0 || tmt->tm_wday==6)
			return true;

		return false;
	}

public:
	class Time32
	{
	public:
		Time32():_msec(0){}

		Time32(time_t _time, uint32_t msecs = 0)
		{
#ifdef _WIN32
			localtime_s(&t, &_time);
#else
			localtime_r(&_time, &t);
#endif
			_msec = msecs;
		}

		Time32(uint64_t _time)
		{
			time_t _t = _time/1000;
			_msec = (uint32_t)_time%1000;
#ifdef _WIN32
			localtime_s(&t, &_t);
#else
			localtime_r(&_t, &t);
#endif
		}

		void from_local_time(uint64_t _time)
		{
			time_t _t = _time/1000;
			_msec = (uint32_t)_time%1000;
#ifdef _WIN32
			localtime_s(&t, &_t);
#else
			localtime_r(&_t, &t);
#endif
		}

		uint32_t date()
		{
			return (t.tm_year + 1900)*10000 + (t.tm_mon + 1)*100 + t.tm_mday;
		}

		uint32_t time()
		{
			return t.tm_hour*10000 + t.tm_min*100 + t.tm_sec;
		}

		uint32_t time_ms()
		{
			return t.tm_hour*10000000 + t.tm_min*100000 + t.tm_sec*1000 + _msec;
		}

		const char* fmt(const char* sfmt = "%Y.%m.%d %H:%M:%S", bool hasMilliSec = false) const
		{
			static char buff[1024];
			uint32_t length = (uint32_t)strftime(buff, 1023, sfmt, &t);
			if (hasMilliSec)
				sprintf(buff + length, ",%03u", _msec);
			return buff;
		}

	protected:
		struct tm t;
		uint32_t _msec;
	};

	class Ticker
	{
	public:
		Ticker()
		{
			_tick = std::chrono::high_resolution_clock::now();
		}

		void reset()
		{
			_tick = std::chrono::high_resolution_clock::now();
		}

		inline int64_t seconds() const 
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto td = now - _tick;
			return std::chrono::duration_cast<std::chrono::seconds>(td).count();
		}

		inline int64_t milli_seconds() const
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto td = now - _tick;
			return std::chrono::duration_cast<std::chrono::milliseconds>(td).count();
		}

		inline int64_t micro_seconds() const
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto td = now - _tick;
			return std::chrono::duration_cast<std::chrono::microseconds>(td).count();
		}

		inline int64_t nano_seconds() const
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto td = now - _tick;
			return std::chrono::duration_cast<std::chrono::nanoseconds>(td).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _tick;
	};
};
