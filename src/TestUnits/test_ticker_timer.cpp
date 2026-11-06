#include "../Includes/WTSSessionInfo.hpp"
#include "../Share/TimeUtils.hpp"
#include "gtest/gtest/gtest.h"

USING_NS_WTP;

/*
 *	Ticker本地闭合定时器的测试
 *	对应issue #253: 夜盘集合竞价(20:59)的tick会把21:01这根K线的本地闭合定时器定到21:00, 导致K线被提前闭合
 */

namespace
{
	//与dist/common/sessions.json中的FN2300一致
	WTSSessionInfo* makeFN2300()
	{
		WTSSessionInfo* sInfo = WTSSessionInfo::create("FN2300", "期货夜盘2300", 300);
		sInfo->setAuctionTime(2059, 2100);
		sInfo->addTradingSection(2100, 2300);
		sInfo->addTradingSection(900, 1015);
		sInfo->addTradingSection(1030, 1130);
		sInfo->addTradingSection(1330, 1500);
		return sInfo;
	}

	//与dist/common/sessions.json中的FN0230一致
	WTSSessionInfo* makeFN0230()
	{
		WTSSessionInfo* sInfo = WTSSessionInfo::create("FN0230", "期货夜盘0230", 300);
		sInfo->setAuctionTime(2059, 2100);
		sInfo->addTradingSection(2100, 230);
		sInfo->addTradingSection(900, 1015);
		sInfo->addTradingSection(1030, 1130);
		sInfo->addTradingSection(1330, 1500);
		return sInfo;
	}

	//与dist/common/sessions.json中的FD0915一致
	WTSSessionInfo* makeFD0915()
	{
		WTSSessionInfo* sInfo = WTSSessionInfo::create("FD0915", "期货白盘0915", 0);
		sInfo->setAuctionTime(929, 930);
		sInfo->addTradingSection(930, 1130);
		sInfo->addTradingSection(1300, 1515);
		return sInfo;
	}

	//与WtCtaRtTicker/WtHftRtTicker/WtSelRtTicker/WtUftRtTicker::on_tick中的分钟序号计算逻辑保持一致
	//返回tick所属K线的序号, 非交易时间返回INVALID_UINT32
	uint32_t tickToBarPos(WTSSessionInfo* sInfo, uint32_t curMin)
	{
		uint32_t minutes = sInfo->timeToMinutes(curMin);
		if (minutes == INVALID_UINT32)
			return INVALID_UINT32;

		if (sInfo->isLastOfSection(curMin))
			minutes--;
		minutes++;
		return minutes;
	}

	//修复前的定时公式: 到行情时间的下一个整分钟
	uint32_t oldLeftTicks(uint32_t curSecMs)
	{
		uint32_t sec = curSecMs / 1000;
		uint32_t msec = curSecMs % 1000;
		return (60 - sec) * 1000 - msec;
	}

	//修复后的定时公式: 到tick所属K线的闭合时刻
	uint32_t newLeftTicks(WTSSessionInfo* sInfo, uint32_t curMin, uint32_t curSecMs)
	{
		uint32_t barPos = tickToBarPos(sInfo, curMin);
		uint32_t barMin = sInfo->minuteToTime(barPos);
		return TimeUtils::getMillisToMinute(curMin, curSecMs, barMin);
	}
}

TEST(test_ticker_timer, test_millis_to_minute)
{
	//交易时段内的普通行情, 目标分钟就是下一分钟
	EXPECT_EQ(TimeUtils::getMillisToMinute(2100, 30500, 2101), 29500);
	EXPECT_EQ(TimeUtils::getMillisToMinute(2100, 0, 2101), 60000);
	EXPECT_EQ(TimeUtils::getMillisToMinute(2259, 59999, 2300), 1);

	//集合竞价的行情, 目标分钟是开盘后第一根K线的闭合时间
	EXPECT_EQ(TimeUtils::getMillisToMinute(2059, 0, 2101), 120000);
	EXPECT_EQ(TimeUtils::getMillisToMinute(2059, 500, 2101), 119500);

	//跨零点
	EXPECT_EQ(TimeUtils::getMillisToMinute(2359, 30000, 0), 30000);
	EXPECT_EQ(TimeUtils::getMillisToMinute(2359, 0, 1), 120000);

	//目标分钟就是当前分钟(小节最后一分钟的行情), 或者已经过了, 都返回0
	EXPECT_EQ(TimeUtils::getMillisToMinute(2300, 0, 2300), 0);
	EXPECT_EQ(TimeUtils::getMillisToMinute(2300, 300, 2300), 0);
}

TEST(test_ticker_timer, test_auction_tick_bar_pos)
{
	WTSSessionInfo* sInfo = makeFN2300();

	//导致bug的等价关系: 集合竞价20:59的tick和开盘21:00的tick, 得到的K线序号都是1(即21:01这根K线)
	EXPECT_TRUE(sInfo->isInAuctionTime(2059));
	EXPECT_EQ(sInfo->timeToMinutes(2059), 0);
	EXPECT_EQ(tickToBarPos(sInfo, 2059), 1);
	EXPECT_EQ(tickToBarPos(sInfo, 2100), 1);
	EXPECT_EQ(sInfo->minuteToTime(1), 2101);

	//Ticker线程在20:59就已经处于"交易时间"的判定分支, 本地定时器是会生效的
	EXPECT_TRUE(sInfo->isInTradingTime(2059, true));

	//非交易时间的tick(08:59、12:59), timeToMinutes返回INVALID_UINT32
	EXPECT_EQ(sInfo->timeToMinutes(859), INVALID_UINT32);
	EXPECT_EQ(sInfo->timeToMinutes(1259), INVALID_UINT32);
	EXPECT_EQ(tickToBarPos(sInfo, 859), INVALID_UINT32);

	sInfo->release();
}

TEST(test_ticker_timer, test_night_auction_fn2300)
{
	WTSSessionInfo* sInfo = makeFN2300();

	//20:59:00.000的集合竞价tick, 所属K线是21:01, 定时器应定到21:01:00, 即120秒
	//修复前按下一个整分钟定时只有60秒, 21:01这根K线会在21:00:00被闭合
	EXPECT_EQ(newLeftTicks(sInfo, 2059, 0), 120000);
	EXPECT_EQ(oldLeftTicks(0), 60000);

	//交易时段内的普通tick, 新旧公式结果一致
	EXPECT_EQ(newLeftTicks(sInfo, 2100, 30500), 29500);
	EXPECT_EQ(oldLeftTicks(30500), 29500);

	EXPECT_EQ(newLeftTicks(sInfo, 2259, 59999), 1);
	EXPECT_EQ(oldLeftTicks(59999), 1);

	EXPECT_EQ(newLeftTicks(sInfo, 1014, 15000), 45000);
	EXPECT_EQ(oldLeftTicks(15000), 45000);

	EXPECT_EQ(newLeftTicks(sInfo, 1459, 0), 60000);
	EXPECT_EQ(oldLeftTicks(0), 60000);

	//小节最后一分钟的tick(如23:00:00.300)所属K线就是23:00这一根, 已经到了闭合时刻
	EXPECT_EQ(tickToBarPos(sInfo, 2300), tickToBarPos(sInfo, 2259));
	EXPECT_EQ(newLeftTicks(sInfo, 2300, 300), 0);

	sInfo->release();
}

TEST(test_ticker_timer, test_cross_midnight_fn0230)
{
	WTSSessionInfo* sInfo = makeFN0230();

	//23:59:30.000的tick, 所属K线是00:00, 跨零点后还有30秒
	EXPECT_EQ(sInfo->minuteToTime(tickToBarPos(sInfo, 2359)), 0);
	EXPECT_EQ(newLeftTicks(sInfo, 2359, 30000), 30000);
	EXPECT_EQ(oldLeftTicks(30000), 30000);

	//00:00:10.000的tick, 所属K线是00:01
	EXPECT_EQ(sInfo->minuteToTime(tickToBarPos(sInfo, 0)), 1);
	EXPECT_EQ(newLeftTicks(sInfo, 0, 10000), 50000);

	//集合竞价同样是120秒
	EXPECT_EQ(newLeftTicks(sInfo, 2059, 0), 120000);

	sInfo->release();
}

TEST(test_ticker_timer, test_day_auction_fd0915)
{
	WTSSessionInfo* sInfo = makeFD0915();

	//白盘品种的集合竞价09:29的tick, 所属K线是09:31, 同样应定到09:31:00
	EXPECT_EQ(tickToBarPos(sInfo, 929), 1);
	EXPECT_EQ(sInfo->minuteToTime(1), 931);
	EXPECT_EQ(newLeftTicks(sInfo, 929, 0), 120000);
	EXPECT_EQ(oldLeftTicks(0), 60000);

	//开盘后的普通tick
	EXPECT_EQ(newLeftTicks(sInfo, 930, 500), 59500);
	EXPECT_EQ(oldLeftTicks(500), 59500);

	sInfo->release();
}
