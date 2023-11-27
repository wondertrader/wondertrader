#include "../Share/WtKVCache.hpp"
#include "../Share/IniHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"
#include "gtest/gtest/gtest.h"

USING_NS_WTP;

TEST(test_kvcache, test_usage)
{
	{
		WtKVCache cache;
		EXPECT_TRUE(cache.init("./simplecache.dat", 20220325, [](const char* msg) {
			printf(msg);
			printf("\n");
		}));
		cache.clear();
	
		EXPECT_STREQ(cache.get("test_null"), "");
	
		cache.put("test", "this is a test string", 0, [](const char* msg) {
			printf(msg);
			printf("\n");
		});

		EXPECT_STREQ(cache.get("test"), "this is a test string");
	
		EXPECT_EQ(cache.size(), 1);
	}

	{
		WtKVCache cache;
		EXPECT_TRUE(cache.init("./simplecache.dat", 20220326, [](const char* msg) {
			printf(msg);
			printf("\n");
		}));
		EXPECT_EQ(cache.size(), 0);
	}
}

TEST(test_kvcache, test_perform)
{
	WtKVCache cache;
	cache.init("./scperform.dat", 20220325, [](const char* msg) {
		printf(msg);
		printf("\n");
	});
	cache.clear();

	IniHelper ini;
	ini.load("./scperform.ini");
	ini.removeSection("test");
	ini.save();

	uint32_t times = 10000;

	TimeUtils::Ticker ticker;
	char buffer[16] = { 0 };

	//œ»≤‚ ‘simplecache
	for(uint32_t i = 0; i < times; i++)
	{
		char* s = fmt::format_to(buffer, "{}", i);
		s[0] = '\0';
		cache.put(buffer, buffer);
	}

	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmt::format_to(buffer, "{}", i);
		s[0] = '\0';
		cache.get(buffer);
	}
	uint64_t a = ticker.nano_seconds();

	//‘Ÿ≤‚ ‘ini
	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmt::format_to(buffer, "{}", i);
		s[0] = '\0';
		ini.writeString("test", buffer, buffer);
		//ini.save();
	}

	for (uint32_t i = 0; i < times; i++)
	{
		char* s = fmt::format_to(buffer, "{}", i);
		s[0] = '\0';
		ini.readString("test", buffer);
	}
	uint64_t b = ticker.nano_seconds();

	fmt::print("cache: {} - ini: {}\n", a, b);
}