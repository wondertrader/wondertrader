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
#include <thread>

#ifdef _MSC_VER
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
#endif

class TimeUtils 
{
	
public:
	static inline int64_t getLocalTimeNowOld(void)
	{
		thread_local static timeb now;
		ftime(&now);
		return now.time * 1000 + now.millitm;
	}

	/*
	 *	获取本地时间，精确到毫秒
	 */
	static inline int64_t getLocalTimeNow(void)
	{
#ifdef _MSC_VER
		LARGE_INTEGER SystemTime;
		do
		{
			SystemTime.HighPart = SharedUserData->SystemTime.High1Time;
			SystemTime.LowPart = SharedUserData->SystemTime.LowPart;
		} while (SystemTime.HighPart != SharedUserData->SystemTime.High2Time);

		uint64_t t = SystemTime.QuadPart;
		t = t - 11644473600L * TICKSPERSEC;
		return t / 10000;
#else
		//timeb now;
		//ftime(&now);
		//return now.time * 1000 + now.millitm;
		/*
		 *	clock_gettime比ftime会提升约10%的性能
		 */
		thread_local static struct timespec now;
		clock_gettime(CLOCK_REALTIME, &now);
		return now.tv_sec * 1000 + now.tv_nsec / 1000000;
#endif
	}

	static inline std::string getLocalTime(bool bIncludeMilliSec = true)
	{
		uint64_t ltime = getLocalTimeNow();
		time_t now = ltime / 1000;
		uint32_t millitm = ltime % 1000;
		tm * tNow = localtime(&now);

		char str[64] = {0};
		if(bIncludeMilliSec)
			sprintf(str, "%02d:%02d:%02d,%03d", tNow->tm_hour, tNow->tm_min, tNow->tm_sec, millitm);
		else
			sprintf(str, "%02d:%02d:%02d", tNow->tm_hour, tNow->tm_min, tNow->tm_sec);
		return str;
	}

	static inline uint64_t getYYYYMMDDhhmmss()
	{
		uint64_t ltime = getLocalTimeNow();
		time_t now = ltime / 1000;

		tm * tNow = localtime(&now);

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
		uint64_t ltime = getLocalTimeNow();
		time_t now = ltime / 1000;
		uint32_t millitm = ltime % 1000;

		tm * tNow = localtime(&now);

		date = (tNow->tm_year+1900)*10000 + (tNow->tm_mon+1)*100 + tNow->tm_mday;
		
		time = tNow->tm_hour*10000 + tNow->tm_min*100 + tNow->tm_sec;
		time *= 1000;
		time += millitm;
	}

	static inline uint32_t getCurDate()
	{
		uint64_t ltime = getLocalTimeNow();
		time_t now = ltime / 1000;
		uint32_t millitm = ltime % 1000;

		tm * tNow = localtime(&now);

		uint32_t date = (tNow->tm_year+1900)*10000 + (tNow->tm_mon+1)*100 + tNow->tm_mday;

		return date;
	}

	static inline uint32_t getWeekDay(uint32_t uDate = 0)
	{
		time_t ts = 0;
		if(uDate == 0)
		{
			ts = getLocalTimeNow()/1000;
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
		uint64_t ltime = getLocalTimeNow();
		time_t now = ltime / 1000;
		uint32_t millitm = ltime % 1000;

		tm * tNow = localtime(&now);

		uint32_t time = tNow->tm_hour*10000 + tNow->tm_min*100 + tNow->tm_sec;

		return time;
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

	/*
	 *	生成带毫秒的timestamp
	 *	@lDate			日期，yyyymmdd
	 *	@lTimeWithMs	带毫秒的时间，HHMMSSsss
	 *	@isToUTC		是否转成UTC时间
	 */
	static inline int64_t makeTime(long lDate, long lTimeWithMs, bool isToUTC = false)
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
		//如果要转成UTC时间，则需要根据时区进行转换
		if (isToUTC)
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
			_msec = (uint32_t)(_time%1000);
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
