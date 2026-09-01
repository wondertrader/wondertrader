#include "../Includes/WTSVariant.hpp"
#include "gtest/gtest/gtest.h"

TEST(test_wtsvariant, real_format_handles_large_values)
{
	wtp::WTSVariant* root = wtp::WTSVariant::createObject();
	root->append("value", 6.022e23);

	EXPECT_EQ(
		root->getString("value"),
		"602200000000000027262976.0000000000");
	root->release();
}
