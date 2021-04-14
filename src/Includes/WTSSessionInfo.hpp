/*!
 * \file WTSSessionInfo.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt交易时间模板对象定义
 */
#pragma once
#include <vector>

#include "WTSObject.hpp"
#include "../Share/TimeUtils.hpp"

NS_OTP_BEGIN

static const char* DEFAULT_SESSIONID = "TRADING";

class WTSSessionInfo : public WTSObject
{
public:
	//交易时段
	typedef std::pair<uint32_t, uint32_t>	TradingSection;
	typedef std::vector<TradingSection>		TradingTimes;

protected:
	TradingTimes	m_tradingTimes;
	TradingSection	m_auctionTime;
	int32_t			m_uOffsetMins;

	std::string		m_strID;
	std::string		m_strName;

protected:
	WTSSessionInfo(int32_t offset)
	{
		m_uOffsetMins = offset;
	}
	virtual ~WTSSessionInfo(){}

public:
	const char* id() const{ return m_strID.c_str(); }
	const char* name() const{ return m_strName.c_str(); }

	static WTSSessionInfo* create(const char* sid, const char* name, int32_t offset = 0)
	{
		WTSSessionInfo* pRet = new WTSSessionInfo(offset);
		pRet->m_strID = sid;
		pRet->m_strName = name;
		return pRet;
	}

public:
	int32_t	getOffsetMins() const{return m_uOffsetMins;}

	void addTradingSection(uint32_t sTime, uint32_t eTime)
	{
		sTime = offsetTime(sTime);
		eTime = offsetTime(eTime);
		m_tradingTimes.emplace_back(TradingSection(sTime, eTime));
	}

	void setAuctionTime(uint32_t sTime, uint32_t eTime)
	{
		m_auctionTime.first = offsetTime(sTime);
		m_auctionTime.second = offsetTime(eTime);
	}

	void setOffsetMins(int32_t offset){m_uOffsetMins = offset;}

	const TradingTimes& getTradingSections() const{ return m_tradingTimes; }
	const TradingSection&	getAuctionSection() const{ return m_auctionTime; }

	//需要导出到脚本的函数
public:
	uint32_t getSectionCount() const{ return (uint32_t)m_tradingTimes.size(); }

	/*
	 *	计算偏移以后的日期
	 *	主要用于各种日期比较
	 *	如夜盘的偏移日期都是下一日
	 */
	uint32_t getOffsetDate(uint32_t uDate = 0, uint32_t uTime = 0)
	{
		if(uDate == 0)
		{
			TimeUtils::getDateTime(uDate, uTime);
			uTime /= 100000;
		}

		int32_t curMinute = (uTime / 100) * 60 + uTime % 100;
		curMinute += m_uOffsetMins;

		if (curMinute >= 1440)
			return TimeUtils::getNextDate(uDate);

		if (curMinute < 0)
			return TimeUtils::getNextDate(uDate, -1);

		return uDate;
	}

	/*
	 *	将时间转换成分钟数
	 *	@uTime	当前时间,格式如0910
	 *	@autoAdjust	是否自动调整,如果开启,非交易时间内的行情,会自动对齐到下一个交易时间,如8点59分的行情,会自动算作9点00的行情
	 *				会不会有别的影响,暂时无法确定,主要是担心非交易时间里收到错误数据
	 *				但是有接收时间控制,应该没问题
	 */
	uint32_t timeToMinutes(uint32_t uTime, bool autoAdjust = false)
	{
		if(m_tradingTimes.empty())
			return INVALID_UINT32;

		if(isInAuctionTime(uTime))
			return 0;

		uint32_t offTime = offsetTime(uTime);

		uint32_t offset = 0;
		bool bFound = false;
		auto it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			if (section.first <= offTime && offTime <= section.second)
			{
				int32_t hour = offTime / 100 - section.first / 100;
				int32_t minute = offTime % 100 - section.first % 100;
				offset += hour*60 + minute;
				bFound = true;
				break;
			}
			else if(offTime > section.second)	//大于上边界
			{
				int32_t hour = section.second/100 - section.first/100;
				int32_t minute = section.second%100 - section.first%100;
				offset += hour*60 + minute;
				//offset += section.second - section.first;
			} 
			else //小于下边界
			{
				if(autoAdjust)
				{
					bFound = true;
				}
				break;
			}
		}

		//没找到就返回0
		if(!bFound)
			return INVALID_UINT32;

		return offset;
	}

	uint32_t minuteToTime(uint32_t uMinutes, bool bHeadFirst = false)
	{
		if(m_tradingTimes.empty())
			return INVALID_UINT32;

		uint32_t offset = uMinutes;
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			uint32_t startMin = section.first/100*60 + section.first%100;
			uint32_t stopMin = section.second/100*60 + section.second%100;

			if(!bHeadFirst)
			{
				if (startMin + offset >= stopMin)
				{
					offset -= (stopMin - startMin);
					if (offset == 0)
					{
						return originalTime(stopMin / 60 * 100 + stopMin % 60);
					}
				}
				else
				{
					//干好位于该区间
					uint32_t desMin = startMin + offset;
					if (desMin >= 1440)
						desMin -= 1440;

					return originalTime(desMin / 60 * 100 + desMin % 60);
				}
			}
			else
			{
				if (startMin + offset < stopMin)
				{
					//干好位于该区间
					uint32_t desMin = startMin + offset;
					if (desMin >= 1440)
						desMin -= 1440;

					return originalTime(desMin / 60 * 100 + desMin % 60);
				}
				else
				{
					offset -= (stopMin - startMin);
				}
			}
		}

		return getCloseTime();
	}

	uint32_t timeToSeconds(uint32_t uTime)
	{
		if(m_tradingTimes.empty())
			return INVALID_UINT32;

		//如果是集合竞价的价格,则认为是0秒价格
		if(isInAuctionTime(uTime/100))
			return 0;

		uint32_t sec = uTime%100;
		uint32_t h = uTime/10000;
		uint32_t m = uTime%10000/100;
		uint32_t offMin = offsetTime(h*100 + m);
		h = offMin/100;
		m = offMin%100;
		uint32_t seconds = h*60*60 + m*60 + sec;

		uint32_t offset = 0;
		bool bFound = false;
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			uint32_t startSecs = (section.first/100*60 + section.first%100)*60;
			uint32_t stopSecs = (section.second/100*60 + section.second%100)*60;
			//uint32_t s = section.first;
			//uint32_t e = section.second;
			//uint32_t hour = (e/100 - s/100);
			//uint32_t minute = (e%100 - s%100);
			if(startSecs <= seconds && seconds <= stopSecs)
			{
				offset += seconds-startSecs;
				if(seconds == stopSecs)
					offset--;
				bFound = true;
				break;
			}
			else
			{
				offset += stopSecs - startSecs;
			}
		}

		//没找到就返回0
		if(!bFound)
			return INVALID_UINT32;

		return offset;
	}

	uint32_t secondsToTime(uint32_t seconds)
	{
		if(m_tradingTimes.empty())
			return INVALID_UINT32;

		uint32_t offset = seconds;
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			uint32_t startSecs = (section.first/100*60 + section.first%100)*60;
			uint32_t stopSecs = (section.second/100*60 + section.second%100)*60;

			if(startSecs + offset >= stopSecs)
			{
				offset -= (stopSecs-startSecs);
				if(offset == 0)
				{
					uint32_t desMin = stopSecs/60;
					return originalTime((desMin/60*100 + desMin%60))*100 + stopSecs%60;
				}
			}
			else
			{
				//干好位于该区间
				uint32_t desSecs = startSecs+offset;
				if(desSecs >= 86400)
					desSecs -= 86400;

				uint32_t desMin = desSecs/60;
				return originalTime((desMin/60*100 + desMin%60))*100 + desSecs%60;
			}
		}

		return INVALID_UINT32;
	}

	inline uint32_t getOpenTime(bool bOffseted = false) const
	{
		if(m_tradingTimes.empty())
			return 0;

		if(bOffseted)
			return m_tradingTimes[0].first;
		else
			return originalTime(m_tradingTimes[0].first);
	}

	inline uint32_t getAuctionStartTime(bool bOffseted = false) const
	{
		if(m_auctionTime.first == 0 && m_auctionTime.second == 0)
			return -1;

		if(bOffseted)
			return m_auctionTime.first;
		else
			return originalTime(m_auctionTime.first);
	}

	inline uint32_t getCloseTime(bool bOffseted = false) const
	{
		if(m_tradingTimes.empty())
			return 0;

		if(bOffseted)
			return m_tradingTimes[m_tradingTimes.size()-1].second;
		else
			return originalTime(m_tradingTimes[m_tradingTimes.size()-1].second);
	}

	inline uint32_t getTradingSeconds()
	{
		uint32_t count = 0;
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			uint32_t s = section.first;
			uint32_t e = section.second;

			uint32_t hour = (e/100 - s/100);
			uint32_t minute = (e%100 - s%100);
			count += hour*60+minute;
		}
		return count*60;
	}

	inline uint32_t getTradingMins()
	{
		uint32_t count = 0;
		TradingTimes::iterator it = m_tradingTimes.begin();
		for (; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			uint32_t s = section.first;
			uint32_t e = section.second;

			uint32_t hour = (e / 100 - s / 100);
			uint32_t minute = (e % 100 - s % 100);
			count += hour * 60 + minute;
		}
		return count;
	}


	bool	isInTradingTime(uint32_t uTime, bool bStrict = false)
	{
		uint32_t count = timeToMinutes(uTime);
		if(count == INVALID_UINT32)
			return false;

		if (bStrict && isLastOfSection(uTime))
			return false;

		return true;
	}

	inline bool	isLastOfSection(uint32_t uTime)
	{
		uint32_t offTime = offsetTime(uTime);
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			if(section.second == offTime)
				return true;
		}

		return false;
	}

	inline bool	isFirstOfSection(uint32_t uTime)
	{
		uint32_t offTime = offsetTime(uTime);
		TradingTimes::iterator it = m_tradingTimes.begin();
		for(; it != m_tradingTimes.end(); it++)
		{
			TradingSection &section = *it;
			if(section.first == offTime)
				return true;
		}

		return false;
	}

	inline bool	isInAuctionTime(uint32_t uTime)
	{
		uint32_t offTime = offsetTime(uTime);

		if(m_auctionTime.first == 0 && m_auctionTime.second == 0)
			return false;

		if (m_auctionTime.first <= offTime && offTime < m_auctionTime.second)
			return true;

		return false;
	}

	const TradingTimes &getTradingTimes() const{return m_tradingTimes;}

	inline uint32_t	offsetTime(uint32_t uTime) const
	{
		int32_t curMinute = (uTime/100)*60 + uTime%100;
		curMinute += m_uOffsetMins;
		if(curMinute >= 1440)
			curMinute -= 1440;
		else if(curMinute < 0)
			curMinute += 1440;

		return (curMinute/60)*100 + curMinute%60;

		return uTime;
	}

	inline uint32_t	originalTime(uint32_t uTime) const
	{
		int32_t curMinute = (uTime/100)*60 + uTime%100;
		curMinute -= m_uOffsetMins;
		if(curMinute >= 1440)
			curMinute -= 1440;
		else if(curMinute < 0)
			curMinute += 1440;

		return (curMinute/60)*100 + curMinute%60;
	}
};

NS_OTP_END