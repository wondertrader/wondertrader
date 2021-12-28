#include "../WTSUtils/WtLMDB.hpp"
#include "../Share/StrUtil.hpp"
#include "gtest/gtest/gtest.h"

USING_NS_WTP;

TEST(test_lmdb, test_constructor)
{
	WtLMDB* db = new WtLMDB();
	EXPECT_TRUE(db->open("./testdb"));
	delete db;
}

TEST(test_lmdb, test_query)
{
	{	
		//–¥≤È—Ø
		WtLMDB db(false);
		EXPECT_TRUE(db.open("./testdb"));

		WtLMDBQuery query(db);
		EXPECT_FALSE(db.has_error());
		for(int i = 0; i < 100; i++)
		{
			std::string key = StrUtil::printf("key-%03d", i + 1);
			std::string val = StrUtil::printf("value-%03d", i + 1);
			EXPECT_TRUE(query.put(key, val));
			if (db.has_error())
				printf("%s\n", db.errmsg());
		}
		
	}

	{
		//∂¡≤È—Ø
		WtLMDB db(true);
		EXPECT_TRUE(db.open("./testdb"));

		WtLMDBQuery query(db);
		EXPECT_FALSE(db.has_error());
		EXPECT_FALSE(query.put(std::string("key1"), std::string("value1")));

		EXPECT_EQ(query.get(std::string("key-001")), "value-001");

		query.get_lowers(std::string("key-030"), 10, [](const ValueArray& ayKeys, const ValueArray& ayVals) {
			EXPECT_EQ(ayKeys.size(), 10);
		});

		query.get_lowers(std::string("key-005"), 10, [](const ValueArray& ayKeys, const ValueArray& ayVals) {
			EXPECT_EQ(ayKeys.size(), 5);
		});

		query.get_uppers(std::string("key-030"), 10, [](const ValueArray& ayKeys, const ValueArray& ayVals) {
			EXPECT_EQ(ayKeys.size(), 10);
		});

		query.get_uppers(std::string("key-095"), 10, [](const ValueArray& ayKeys, const ValueArray& ayVals) {
			EXPECT_EQ(ayKeys.size(), 6);
		});

		query.get_range(std::string("key-030"), std::string("key-050"), [](const ValueArray& ayKeys, const ValueArray& ayVals) {
			EXPECT_EQ(ayKeys.size(), 21);
		});
	}
}