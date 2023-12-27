#include "gtest/gtest/gtest.h"
#include "../Share/fmtlib.h"
#include "../Share/TimeUtils.hpp"
#include "../Includes/WTSMarcos.h"
#include "../Share/Converter.hpp"

#include <charconv>


TEST(test_fmt, test_format)
{
	EXPECT_EQ(fmt::format("{:04d}",6), "0006");
	
	EXPECT_EQ(fmt::format("{:06d}", (uint32_t)-1), "4294967295");

	EXPECT_EQ(fmt::format("{:.2f}", 3.1415926), "3.14");

	int times = 1000000;
	char buffer[64] = { 0 };

	const char* tag = "logtag";

	//先做一个预热
	TimeUtils::Ticker ticker;
	for (int i = 0; i < times; i++)
	{
		buffer[0] = '[';
		wt_strcpy(buffer + 1, tag);
		auto len = strlen(tag);
		buffer[len + 1] = ']';
	}

	ticker.reset();
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

TEST(test_fmt, test_stoi)
{
	int times = 1000000;
	const char* str = "12346789";

	//先做一个预热
	TimeUtils::Ticker ticker;
	for (int i = 0; i < times; i++)
	{
		atoi(str);
	}
	
	uint64_t t1 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		strtol(str, NULL, 10);
	}
	uint64_t t2 = ticker.nano_seconds();

	int result{};
	std::string_view s(str);
	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		std::from_chars(s.data(), s.data() + s.size(), result);
	}
	uint64_t t3 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		convert::to_int32(str);
	}
	uint64_t t4 = ticker.nano_seconds();
	fmt::print("atoi: {} - strtul: {} - from_chars: {}, constexpr: {}\n", t1, t2, t3, t4);
}

TEST(test_fmt, test_itos)
{
	int times = 1000000;
	int num = 12346789;

	char buffer[32] = { 0 };

	TimeUtils::Ticker ticker;
#ifdef _MSVC	
	for (int i = 0; i < times; i++)
	{
		itoa(num, buffer, 10);
	}
#endif
	uint64_t t1 = ticker.nano_seconds();

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		sprintf(buffer, "%d", num);
	}
	uint64_t t2 = ticker.nano_seconds();

	buffer[9] = '0';
	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		std::to_chars(buffer, buffer + 64, num);
	}
	uint64_t t3 = ticker.nano_seconds();
	fmt::print(buffer);

	ticker.reset();
	for (int i = 0; i < times; i++)
	{
		fmt::format_to(buffer, "{}", num);
	}
	uint64_t t4 = ticker.nano_seconds();
	fmt::print("itoa: {} - sprintf: {} - to_chars: {} - fmt: {}\n", t1, t2, t3, t4);
}