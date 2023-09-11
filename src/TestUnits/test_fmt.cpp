#include "gtest/gtest/gtest.h"
#include "../Share/fmtlib.h"


TEST(test_fmt, test_format)
{
	EXPECT_EQ(fmt::format("{:04d}",6), "0006");
	
	EXPECT_EQ(fmt::format("{:06d}", (uint32_t)-1), "4294967295");

	EXPECT_EQ(fmt::format("{:.2f}", 3.1415926), "3.14");
}