#include "gtest/gtest/gtest.h"
#include "../Includes/FasterDefs.h"
#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"

USING_NS_WTP;

/*
 *	By Wesley @ 2023.08.16
 *	ankerl写入速度比robin好很多，大概快1/3，尤其数据量在40w以内的时候
 *	但是robin的读取速度比ankerl好，不过到了30w条数据以内，差别就不大
 *	按照wondertrader的场景，还是ankerl要好很多
 */
TEST(test_fastestmap, test_perform)
{
	fastest_hashmap<std::string, std::string> a;
	wt_hashmap<std::string, std::string> b;
	std::map<std::string, std::string> c;
	std::unordered_map<std::string, std::string> d;

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
		c[buffer] = buffer;
	}
	uint64_t t3 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		d[buffer] = buffer;
	}
	uint64_t t4 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		a[buffer];
	}
	uint64_t t5 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		b[buffer];
	}
	uint64_t t6 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		c[buffer];
	}
	uint64_t t7 = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmtutil::format_to(buffer, "{:09d}", i);
		d[buffer];
	}
	uint64_t t8 = ticker.nano_seconds();

	fmt::print("write and read {} time with different maps\n", times);
	fmt::print("robin_write: {}ns\n", t1);
	fmt::print("ankerl_write: {}ns\n", t2);
	fmt::print("stdmap_write: {}ns\n", t3);
	fmt::print("stdunorder_write: {}ns\n", t4);

	fmt::print("robin_read: {}ns\n", t5);
	fmt::print("ankerl_read: {}ns\n", t6);
	fmt::print("stdmap_read: {}ns\n", t7);
	fmt::print("stdunorder_read: {}ns\n", t8);
}