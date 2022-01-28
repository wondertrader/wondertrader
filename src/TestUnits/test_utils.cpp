#include "../Share/TimeUtils.hpp"
#include "gtest/gtest/gtest.h"

TEST(test_utils, test_timeutils)
{
	auto a = TimeUtils::makeTime(20220128, 142348500, false);
	auto b = TimeUtils::makeTime(20220128, 142348500, true);

	EXPECT_EQ(a - b, 8 * 3600000);
}