#include "../Share/CodeHelper.hpp"
#include "gtest/gtest/gtest.h"

TEST(test_codehelper, test_raw_to_std)
{
	EXPECT_EQ(CodeHelper::rawFlatCodeToStdCode("600000", "SSE", "STK"), "SSE.STK.600000");
	EXPECT_EQ(CodeHelper::rawFlatCodeToStdCode("000001", "SSE", "IDX"), "SSE.IDX.000001");
	EXPECT_EQ(CodeHelper::rawFlatCodeToStdCode("510300", "SSE", "ETF"), "SSE.ETF.510300");

	EXPECT_EQ(CodeHelper::rawMonthCodeToStdCode("IF2112", "CFFEX"), "CFFEX.IF.2112");
	EXPECT_EQ(CodeHelper::rawMonthCodeToStdCode("MA112", "CZCE"), "CZCE.MA.2112");
	EXPECT_EQ(CodeHelper::rawMonthCodeToStdCode("v2112", "DCE"), "DCE.v.2112");
	
	EXPECT_TRUE(CodeHelper::isMonthlyCode("MA221"));
	EXPECT_TRUE(CodeHelper::isMonthlyCode("rb2001"));
	EXPECT_TRUE(CodeHelper::isMonthlyCode("v2001"));

	EXPECT_FALSE(CodeHelper::isMonthlyCode("600000"));
	EXPECT_TRUE(CodeHelper::isMonthlyCode("BTC-USDT-220627"));
}

TEST(test_codehelper, test_extract)
{
	auto c = CodeHelper::extractStdCode("SSE.STK.600000-");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "600000");
	EXPECT_STREQ(c._product, "STK");
	EXPECT_TRUE(c.isExright());
	EXPECT_EQ(c._exright, 1);

	c = CodeHelper::extractStdCode("SSE.STK.600000+");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "600000");
	EXPECT_STREQ(c._product, "STK");
	EXPECT_TRUE(c.isExright());
	EXPECT_EQ(c._exright, 2);

	c = CodeHelper::extractStdCode("SSE.STK.600000");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "600000");
	EXPECT_STREQ(c._product, "STK");
	EXPECT_FALSE(c.isExright());
	EXPECT_EQ(c._exright, 0);

	c = CodeHelper::extractStdCode("SSE.ETF.1108600-");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "1108600");
	EXPECT_STREQ(c._product, "ETF");
	EXPECT_TRUE(c.isExright());
	EXPECT_EQ(c._exright, 1);

	c = CodeHelper::extractStdCode("SSE.ETF.1108600+");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "1108600");
	EXPECT_STREQ(c._product, "ETF");
	EXPECT_TRUE(c.isExright());
	EXPECT_EQ(c._exright, 2);

	c = CodeHelper::extractStdCode("SSE.ETF.1108600");
	EXPECT_STREQ(c._exchg, "SSE");
	EXPECT_STREQ(c._code, "1108600");
	EXPECT_STREQ(c._product, "ETF");
	EXPECT_FALSE(c.isExright());
	EXPECT_EQ(c._exright, 0);

	c = CodeHelper::extractStdCode("CFFEX.IF.2101");
	EXPECT_STREQ(c._exchg, "CFFEX");
	EXPECT_STREQ(c._code, "IF2101");
	EXPECT_STREQ(c._product, "IF");
	EXPECT_FALSE(c.isHot());
	EXPECT_FALSE(c.isSecond());

	c = CodeHelper::extractStdCode("CFFEX.IF.HOT");
	EXPECT_STREQ(c._exchg, "CFFEX");
	EXPECT_STREQ(c._code, "IF");
	EXPECT_STREQ(c._product, "IF");
	EXPECT_TRUE(c.isHot());
	EXPECT_FALSE(c.isSecond());

	c = CodeHelper::extractStdCode("CFFEX.IF.2ND");
	EXPECT_STREQ(c._exchg, "CFFEX");
	EXPECT_STREQ(c._code, "IF");
	EXPECT_STREQ(c._product, "IF");
	EXPECT_FALSE(c.isHot());
	EXPECT_TRUE(c.isSecond());

	c = CodeHelper::extractStdCode("BINANCE.DC.BTCUSDT");
	EXPECT_STREQ(c._exchg, "BINANCE");
	EXPECT_STREQ(c._code, "BTCUSDT");
	EXPECT_STREQ(c._product, "DC");
	EXPECT_FALSE(c.isHot());
	EXPECT_FALSE(c.isSecond());
}
