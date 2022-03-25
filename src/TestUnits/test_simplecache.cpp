#include "../Share/SimpleCache.hpp"
#include "gtest/gtest/gtest.h"

USING_NS_WTP;

TEST(test_simplecache, test_simplecache)
{
	{
		SimpleCache cache;
		EXPECT_TRUE(cache.init("./simplecache.sc", 20220325, [](const char* msg) {
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
		SimpleCache cache;
		EXPECT_TRUE(cache.init("./simplecache.sc", 20220326, [](const char* msg) {
			printf(msg);
			printf("\n");
		}));
		EXPECT_EQ(cache.size(), 0);
	}
}