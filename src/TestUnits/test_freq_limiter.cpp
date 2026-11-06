#include "../Share/FreqLimiter.hpp"
#include "gtest/gtest/gtest.h"

#include <vector>

typedef std::vector<uint64_t> TimeCacheList;

//模拟issue #157的场景：每秒撤单1次，窗口100秒，上限20次，第21次应该被拒绝
TEST(test_freq_limiter, test_issue157_scenario)
{
	TimeCacheList cache;
	const uint32_t spanSecs = 100;
	const uint32_t boundary = 20;
	uint64_t base = 1700000000000ULL;
	uint32_t times = 0;

	for (uint32_t i = 0; i < boundary; i++)
	{
		uint64_t now = base + (uint64_t)i * 1000;
		//前20次都应该放行
		EXPECT_FALSE(FreqLimiter::isExceeded(cache, now, spanSecs, boundary, times)) << "cancel #" << (i + 1);
		EXPECT_EQ(times, i);
		cache.emplace_back(now);
	}

	//第21次，窗口内已经有20次，应该被拒绝
	uint64_t now = base + (uint64_t)boundary * 1000;
	EXPECT_TRUE(FreqLimiter::isExceeded(cache, now, spanSecs, boundary, times));
	EXPECT_EQ(times, boundary);

	//之后无论再试多少次，只要窗口内还是20次，都应该被拒绝
	EXPECT_TRUE(FreqLimiter::isExceeded(cache, now + 500, spanSecs, boundary, times));
	EXPECT_EQ(times, boundary);
}

//窗口滑动以后，旧的时间戳过期，应该恢复放行
TEST(test_freq_limiter, test_window_sliding)
{
	TimeCacheList cache;
	const uint32_t spanSecs = 100;
	const uint32_t boundary = 20;
	uint64_t base = 1700000000000ULL;
	uint32_t times = 0;

	for (uint32_t i = 0; i < boundary; i++)
		cache.emplace_back(base + (uint64_t)i * 1000);

	//第一次撤单发生在base，窗口是(now-100s, now]，now = base+100s时第一次正好落在窗口外
	uint64_t now = base + (uint64_t)spanSecs * 1000;
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, now, spanSecs, boundary, times));
	EXPECT_EQ(times, boundary - 1);
	//裁剪以后，缓存里只剩下窗口内的19条
	EXPECT_EQ(cache.size(), boundary - 1);

	//now = base+100s-1ms时，第一次还在窗口内，应该被拒绝
	TimeCacheList cache2;
	for (uint32_t i = 0; i < boundary; i++)
		cache2.emplace_back(base + (uint64_t)i * 1000);
	EXPECT_TRUE(FreqLimiter::isExceeded(cache2, now - 1, spanSecs, boundary, times));
	EXPECT_EQ(times, boundary);
	EXPECT_EQ(cache2.size(), boundary);
}

//旧时间戳应该在每次检查时被裁剪，缓存不会无限增长
TEST(test_freq_limiter, test_prune_old_stamps)
{
	TimeCacheList cache;
	const uint32_t spanSecs = 10;
	uint64_t base = 1700000000000ULL;

	//1000秒内每秒一次，共1000条
	for (uint32_t i = 0; i < 1000; i++)
		cache.emplace_back(base + (uint64_t)i * 1000);

	uint64_t now = base + 1000 * 1000;
	//窗口是左开右闭的(now-10s, now]，base+990s正好落在窗口外，窗口内只有最后9条：base+991s ~ base+999s
	EXPECT_EQ(FreqLimiter::countInWindow(cache, now, spanSecs), 9u);
	EXPECT_EQ(cache.size(), 9u);
	EXPECT_EQ(cache.front(), base + 991 * 1000);
	EXPECT_EQ(cache.back(), base + 999 * 1000);

	//再过很久以后，全部过期
	EXPECT_EQ(FreqLimiter::countInWindow(cache, now + 3600 * 1000, spanSecs), 0u);
	EXPECT_TRUE(cache.empty());
}

//边界情况：空缓存、上限为0、窗口为0、now小于窗口长度
TEST(test_freq_limiter, test_edge_cases)
{
	TimeCacheList cache;
	uint32_t times = 123;

	//空缓存
	EXPECT_EQ(FreqLimiter::countInWindow(cache, 1000, 10), 0u);
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, 1000, 10, 1, times));
	EXPECT_EQ(times, 0u);

	//上限为0视为不限制，再多也不拒绝
	for (uint32_t i = 0; i < 50; i++)
		cache.emplace_back(1000 + i);
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, 1100, 10, 0, times));
	EXPECT_EQ(times, 50u);

	//窗口为0秒，窗口内没有任何事件，不会拒绝
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, 1100, 0, 1, times));
	EXPECT_EQ(times, 0u);
	EXPECT_TRUE(cache.empty());

	//now小于窗口长度（例如用相对时间），不能发生无符号下溢
	cache.emplace_back(5);
	cache.emplace_back(10);
	EXPECT_EQ(FreqLimiter::countInWindow(cache, 20, 1), 2u);
	EXPECT_TRUE(FreqLimiter::isExceeded(cache, 20, 1, 2, times));
	EXPECT_EQ(times, 2u);
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, 20, 1, 3, times));
}

//上限为1时，只有在窗口内没有事件的时候才放行
TEST(test_freq_limiter, test_boundary_one)
{
	TimeCacheList cache;
	uint32_t times = 0;
	uint64_t base = 1700000000000ULL;

	EXPECT_FALSE(FreqLimiter::isExceeded(cache, base, 10, 1, times));
	cache.emplace_back(base);
	EXPECT_TRUE(FreqLimiter::isExceeded(cache, base + 9999, 10, 1, times));
	EXPECT_FALSE(FreqLimiter::isExceeded(cache, base + 10000, 10, 1, times));
	EXPECT_TRUE(cache.empty());
}
