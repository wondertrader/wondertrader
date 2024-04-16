#include "../Share/WtKVCache.hpp"
#include "../Share/IniHelper.hpp"
#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"
#include "gtest/gtest/gtest.h"

USING_NS_WTP;

TEST(test_kvcache, test_perform)
{
	WtKVCache cache;
	cache.init("./scperform.dat", 20220325, [](const char* msg) {
		printf("%s\n", msg);
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