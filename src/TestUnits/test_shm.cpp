#include "gtest/gtest/gtest.h"
#include "../WtShareHelper/WtShareHelper.h"

TEST(test_shm, test_sharehelper)
{
	EXPECT_TRUE(init_slave("uft", "E:\\deploy_uft\\uft_test\\.share"));

	//EXPECT_TRUE(set_string("test", "section", "string", "value"));
	//EXPECT_TRUE(set_string("test", "section", "string", "value2"));
	//EXPECT_TRUE(set_int32("test", "section", "int32", -5));
	//EXPECT_TRUE(set_int32("test", "section", "int32", 5));
	//EXPECT_TRUE(set_int64("test", "section", "int64", -1968759));
	//EXPECT_TRUE(set_int64("test", "section", "int64", 1968759));
	//EXPECT_TRUE(set_uint32("test", "section", "uint32", 0xFFFF0000));
	//EXPECT_TRUE(set_uint32("test", "section", "uint32", 0xFFFF0002));
	//EXPECT_TRUE(set_uint64("test", "section", "uint64", 0xFFFFFFFF00000000));
	//EXPECT_TRUE(set_uint64("test", "section", "uint64", 0xFFFFFFFF00000003));
	//EXPECT_TRUE(set_double("test", "section", "double", DBL_MAX));
	//EXPECT_TRUE(set_double("test", "section", "double", FLT_MAX));


	EXPECT_EQ(get_int32("uft", "uft_demo", "offset"), 1);
	EXPECT_EQ(get_uint32("uft", "uft_demo", "freq"), 0);
	EXPECT_EQ(get_uint32("uft", "uft_demo", "second"), 10);
	EXPECT_EQ(get_double("uft", "uft_demo", "lots"), 1.0);
}
