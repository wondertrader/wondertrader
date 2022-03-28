#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"
#include "gtest/gtest/gtest.h"

TEST(test_utils, test_timeutils)
{
	uint32_t times = 100000000;
	TimeUtils::Ticker ticker;
	uint64_t t;
	for(uint32_t i = 0; i< times; i ++)
	{
		t = TimeUtils::getLocalTimeNowOld();
	}

	uint64_t a = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		t = TimeUtils::getLocalTimeNow();
	}

	uint64_t b = ticker.nano_seconds();

	fmt::print("getLocalTimeNowOld: {} - getLocalTimeNow: {}\n", a, b);
}