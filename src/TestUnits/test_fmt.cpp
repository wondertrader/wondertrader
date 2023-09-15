#include "gtest/gtest/gtest.h"
#include "../Share/fmtlib.h"
#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSMarcos.h"


TEST(test_fmt, test_format)
{
	EXPECT_EQ(fmt::format("{:04d}",6), "0006");
	
	EXPECT_EQ(fmt::format("{:06d}", (uint32_t)-1), "4294967295");

	EXPECT_EQ(fmt::format("{:.2f}", 3.1415926), "3.14");

	int times = 1000000;
	char buffer[512] = { 0 };

	const char* tag = "logtag";

	TimeUtils::Ticker ticker;
	for( int i = 0; i < times; i++)
	{
		buffer[0] = '[';
		wt_strcpy(buffer+1, tag);
		auto len = strlen(tag);
		buffer[len + 1] = ']';
	}
	uint64_t t1 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		buffer[0] = '[';
		strcpy(buffer + 1, tag);
		auto len = strlen(tag);
		buffer[len + 1] = ']';
	}
	uint64_t t2 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		fmt::format_to(buffer, "[{}]", tag);
	}
	uint64_t t3 = ticker.nano_seconds();
	fmt::print("wt_strcpy: {} - strcpy: {} - format: {}\n", t1, t2, t3);
}