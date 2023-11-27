#include "gtest/gtest/gtest.h"
#include "../Includes/FasterDefs.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"

USING_NS_WTP;

/*
 *	By Wesley @ 2023.08.16
 *	ankerl写入速度比robin好很多，大概快1/3，尤其数据量在40w以内的时候
 *	但是robin的读取速度比robin好，不过到了30w条数据以内，差别就不大
 *	按照wondertrader的场景，还是ankerl要好很多
 */
TEST(test_fastestmap, test_perform)
{
	fastest_hashmap<std::string, std::string> a;
	wt_hashmap<std::string, std::string> b;

	uint32_t times = 300000;

	char buffer[16] = { 0 };
	TimeUtils::Ticker ticker;

	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		a[buffer] = buffer;
	}
	uint64_t t1 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		b[buffer] = buffer;
	}
	uint64_t t2 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		a[buffer];
	}
	uint64_t t3 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		b[buffer];
	}
	uint64_t t4 = ticker.nano_seconds();

	fmt::print("robin_write: {} - ankerl_write: {} - robin_read: {} - ankerl_read: {}\n", t1, t2, t3, t4);
}