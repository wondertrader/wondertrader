#include "../Share/TimeUtils.hpp"
#include "../Share/fmtlib.h"
#include "../WTSTools/WTSHotMgr.h"
#include "gtest/gtest/gtest.h"

TEST(test_utils, test_hotmgr)
{
	WTSHotMgr hotMgr;
	hotMgr.loadCustomRules("HOT", "common/hots.json");

	EXPECT_STREQ(hotMgr.getCustomRawCode("HOT", "CFFEX.IF", 0), "IF2206");
	EXPECT_STREQ(hotMgr.getCustomRawCode("HOT", "CFFEX.IF", 20220106), "IF2201");

	EXPECT_STREQ(hotMgr.getPrevCustomRawCode("HOT", "CFFEX.IF", 0), "IF2205");
	EXPECT_STREQ(hotMgr.getPrevCustomRawCode("HOT", "CFFEX.IF", 20220106), "IF2112");

	EXPECT_TRUE(hotMgr.isCustomHot("HOT", "CFFEX.IF2206", 0));
	EXPECT_FALSE(hotMgr.isCustomHot("HOT", "CFFEX.IF2205", 0));

	EXPECT_FALSE(hotMgr.isCustomHot("HOT", "CFFEX.IF2206", 20220518));
	EXPECT_TRUE(hotMgr.isCustomHot("HOT", "CFFEX.IF2205", 20220518));

	EXPECT_TRUE(hotMgr.isCustomHot("HOT", "CFFEX.IF2206", 20220519));
	EXPECT_FALSE(hotMgr.isCustomHot("HOT", "CFFEX.IF2205", 20220519));

	HotSections secs;
	hotMgr.splitCustomSections("HOT", "CFFEX.IF", 20200101, 20220526, secs);
	EXPECT_FALSE(secs.empty());
}
