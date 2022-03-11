#include "gtest/gtest/gtest.h"
#include "../WTSUtils/WTSCfgLoader.h"
#include "../Includes/WTSVariant.hpp"

USING_NS_WTP;

TEST(test_cfg_loader, test_yaml)
{
	WTSVariant* cfg = WTSCfgLoader::load_from_file("test.yaml", true);
	EXPECT_TRUE(cfg != NULL);
	if (cfg == NULL)
		return;

	EXPECT_TRUE(cfg->has("dict"));
	WTSVariant* dict = cfg->get("dict");
	EXPECT_TRUE(dict->isObject());

	EXPECT_TRUE(dict->has("str"));
	EXPECT_STREQ(dict->getCString("str"), "csv");
	EXPECT_TRUE(dict->has("int"));
	EXPECT_EQ(dict->getUInt64("int"), 201909010900);
	EXPECT_TRUE(dict->has("bigint"));
	EXPECT_EQ(dict->getUInt64("bigint"), 201909010900000000);
	EXPECT_TRUE(dict->has("float"));
	EXPECT_EQ(dict->getDouble("float"), 3.1415927);
	EXPECT_TRUE(dict->has("bool"));
	EXPECT_TRUE(dict->getBoolean("bool"));

	EXPECT_TRUE(dict->has("aynum"));
	EXPECT_TRUE(dict->get("aynum")->isArray());
	EXPECT_EQ(dict->get("aynum")->size(), 3);
	EXPECT_EQ(dict->get("aynum")->get(1)->asDouble(), 0.37);

	EXPECT_TRUE(dict->has("aystr"));
	EXPECT_TRUE(dict->get("aystr")->isArray());
	EXPECT_EQ(dict->get("aystr")->size(), 4);
	EXPECT_STREQ(dict->get("aystr")->get((uint32_t)0)->asCString(), "a");
	EXPECT_STREQ(dict->get("aystr")->get(3)->asCString(), "d");

	EXPECT_TRUE(cfg->has("aystr_L1"));
	EXPECT_TRUE(cfg->get("aystr_L1")->isArray());
	EXPECT_EQ(cfg->get("aystr_L1")->size(), 4);
	EXPECT_STREQ(cfg->get("aystr_L1")->get((uint32_t)0)->asCString(), "a");
	EXPECT_STREQ(cfg->get("aystr_L1")->get(3)->asCString(), "d");

	EXPECT_TRUE(cfg->has("aynum_L1"));
	EXPECT_TRUE(cfg->get("aynum_L1")->isArray());
	EXPECT_EQ(cfg->get("aynum_L1")->size(), 3);
	EXPECT_EQ(cfg->get("aynum_L1")->get(1)->asDouble(), 0.7219);

	cfg->release();
}

TEST(test_cfg_loader, test_json)
{
	WTSVariant* cfg = WTSCfgLoader::load_from_file("test.json", true);
	EXPECT_TRUE(cfg != NULL);
	if (cfg == NULL)
		return;

	EXPECT_TRUE(cfg->has("dict"));
	WTSVariant* dict = cfg->get("dict");
	EXPECT_TRUE(dict->isObject());

	EXPECT_TRUE(dict->has("str"));
	EXPECT_STREQ(dict->getCString("str"), "csv");
	EXPECT_TRUE(dict->has("int"));
	EXPECT_EQ(dict->getUInt64("int"), 201909010900);
	EXPECT_TRUE(dict->has("bigint"));
	EXPECT_EQ(dict->getUInt64("bigint"), 201909010900000000);
	EXPECT_TRUE(dict->has("float"));
	EXPECT_EQ(dict->getDouble("float"), 3.1415927);
	EXPECT_TRUE(dict->has("bool"));
	EXPECT_TRUE(dict->getBoolean("bool"));

	EXPECT_TRUE(dict->has("aynum"));
	EXPECT_TRUE(dict->get("aynum")->isArray());
	EXPECT_EQ(dict->get("aynum")->size(), 3);
	EXPECT_EQ(dict->get("aynum")->get(1)->asDouble(), 0.37);

	EXPECT_TRUE(dict->has("aystr"));
	EXPECT_TRUE(dict->get("aystr")->isArray());
	EXPECT_EQ(dict->get("aystr")->size(), 4);
	EXPECT_STREQ(dict->get("aystr")->get((uint32_t)0)->asCString(), "a");
	EXPECT_STREQ(dict->get("aystr")->get(3)->asCString(), "d");

	EXPECT_TRUE(cfg->has("aystr_L1"));
	EXPECT_TRUE(cfg->get("aystr_L1")->isArray());
	EXPECT_EQ(cfg->get("aystr_L1")->size(), 4);
	EXPECT_STREQ(cfg->get("aystr_L1")->get((uint32_t)0)->asCString(), "a");
	EXPECT_STREQ(cfg->get("aystr_L1")->get(3)->asCString(), "d");

	EXPECT_TRUE(cfg->has("aynum_L1"));
	EXPECT_TRUE(cfg->get("aynum_L1")->isArray());
	EXPECT_EQ(cfg->get("aynum_L1")->size(), 3);
	EXPECT_EQ(cfg->get("aynum_L1")->get(1)->asDouble(), 0.7219);

	cfg->release();
}
