#include "gtest/gtest/gtest.h"

#include <filesystem>

#include "../WtBtCore/HisDataReplayer.h"
#include "../Includes/WTSVariant.hpp"

namespace
{
std::filesystem::path fee_fixture(const char* name)
{
	return std::filesystem::path(__FILE__).parent_path() / "fixtures" / name;
}

std::filesystem::path repository_file(const char* relative_path)
{
	return std::filesystem::path(__FILE__).parent_path().parent_path().parent_path() /
		relative_path;
}
}

TEST(test_replayer_fee, applies_minimum_only_to_nonzero_rate_fills)
{
	EXPECT_DOUBLE_EQ(
		5.0,
		HisDataReplayer::apply_fee_minimum(0.01, 100.0, 0.0001, 5.0));
	EXPECT_DOUBLE_EQ(
		10.0,
		HisDataReplayer::apply_fee_minimum(10.0, 100000.0, 0.0001, 5.0));
	EXPECT_DOUBLE_EQ(
		5.0,
		HisDataReplayer::apply_fee_minimum(0.5, 1.0, 0.5, 5.0));
	EXPECT_DOUBLE_EQ(
		-0.1,
		HisDataReplayer::apply_fee_minimum(-0.1, 100.0, -0.001, 5.0));
}

TEST(test_replayer_fee, zero_quantity_zero_rate_and_default_are_unchanged)
{
	EXPECT_DOUBLE_EQ(
		0.0,
		HisDataReplayer::apply_fee_minimum(0.0, 0.0, 0.0001, 5.0));
	EXPECT_DOUBLE_EQ(
		0.0,
		HisDataReplayer::apply_fee_minimum(0.0, 100.0, 0.0, 5.0));
	EXPECT_DOUBLE_EQ(
		0.01,
		HisDataReplayer::apply_fee_minimum(0.01, 100.0, 0.0001, 0.0));
}

TEST(test_replayer_fee, loads_optional_minimum_from_json_template)
{
	const auto fixture = fee_fixture("fees_minfee.json");
	ASSERT_TRUE(std::filesystem::exists(fixture)) << fixture;

	WTSVariant* cfg = WTSVariant::createObject();
	cfg->append("mode", "csv");
	cfg->append("path", "/tmp");
	cfg->append("stime", static_cast<uint64_t>(0));
	cfg->append("etime", static_cast<uint64_t>(0));
	cfg->append("tick", false);
	cfg->append("adjust_flag", static_cast<uint32_t>(0));
	cfg->append("align_by_section", false);
	cfg->append("dont_simtick_if_notrade", false);
	cfg->append("fees", fixture.string().c_str());

	WTSVariant* basefiles = WTSVariant::createObject();
	const auto commodities = repository_file("dist/common/stk_comms.json");
	basefiles->append("commodity", commodities.string().c_str());
	cfg->append("basefiles", basefiles);
	basefiles->release();

	HisDataReplayer replayer;
	ASSERT_TRUE(replayer.init(cfg));
	cfg->release();

	EXPECT_DOUBLE_EQ(5.0,
		replayer.calc_fee("SSE.STK.000001", 1.0, 100.0, 0));
	EXPECT_DOUBLE_EQ(10000.0,
		replayer.calc_fee("SSE.STK.000001", 100000.0, 100.0, 0));
	EXPECT_DOUBLE_EQ(12000.0,
		replayer.calc_fee("SSE.STK.000001", 100000.0, 100.0, 1));
	EXPECT_DOUBLE_EQ(5.0,
		replayer.calc_fee("DCE.a", 1.0, 1.0, 0));
	EXPECT_DOUBLE_EQ(7.0,
		replayer.calc_fee("DCE.a", 1.0, 1.0, 1));
	const double negative_fee =
		replayer.calc_fee("SZSE.STK.000001", 1.0, 100.0, 0);
	EXPECT_DOUBLE_EQ(-0.09, negative_fee);
	EXPECT_DOUBLE_EQ(0.1,
		replayer.calc_fee("SSE.IDX.000001", 1.0, 100.0, 0));
	EXPECT_DOUBLE_EQ(0.0,
		replayer.calc_fee("SSE.STK.000001", 1.0, 100.0, 2));
	EXPECT_DOUBLE_EQ(0.0,
		replayer.calc_fee("SSE.STK.000001", 1.0, 0.0, 0));
}
