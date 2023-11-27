#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"
#include "gtest/gtest/gtest.h"
#include "../Includes/WTSMarcos.h"

void run_test(uint32_t times, uint32_t len)
{
	char buffer[512] = { 0 };

	char fmt[32] = { 0 };
	sprintf(fmt, "{:0%dd}", len);

	TimeUtils::Ticker ticker;
	for (int i = 0; i < times; i++)
	{
		wt_strcpy(buffer, fmtutil::format(fmt, i));
	}
	uint64_t t1 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		strcpy(buffer, fmtutil::format(fmt, i));
	}
	uint64_t t2 = ticker.nano_seconds();
	fmt::print("{}-bytes string compare, wt_strcpy: {} - strcpy: {}\n", len, t1, t2);
}

TEST(test_utils, copy_str)
{
	run_test(1000000, 16);
	run_test(1000000, 32);
	run_test(1000000, 64);
	run_test(1000000, 128);
}