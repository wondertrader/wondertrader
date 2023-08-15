#pragma once
#include <string.h>
#include "WTSMarcos.h"
#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"


/*
 *	By Wesley @ 2023.08.15
 *	很遗憾，robin_map搭配std::string在数据量大的时候（经测试在13106条数据，不同测试机可能具体数值不同）
 *	会出现bad allocate的异常
 *	我猜测是std::string无法像string那样自动优化
 *	所以数据量大的时候，就会占用非常大的内存，当运行环境内存较小时，就会出现异常
 *	所以这次把LongKey和LongKey都注释掉，改成std::string
 */

NS_WTP_BEGIN

struct string_hash
{
	//BKDRHash算法
	size_t operator()(const std::string& key) const
	{
		size_t seed = 131; // 31 131 1313 13131 131313 etc..
		size_t hash = 0;

		char* str = (char*)key.c_str();
		while (*str)
		{
			hash = hash * seed + (*str++);
		}

		return (hash & 0x7FFFFFFF);
	}
};

template<class Key, class T>
class fastest_hashmap : public tsl::robin_map<Key, T>
{
public:
	typedef tsl::robin_map<Key, T>	Container;
	fastest_hashmap():Container(){}
};

template<class T>
class fastest_hashmap<std::string, T> : public tsl::robin_map<std::string, T, string_hash>
{
public:
	typedef tsl::robin_map<std::string, T, string_hash>	Container;
	fastest_hashmap() :Container() {}
};

template<class Key>
class fastest_hashset : public tsl::robin_set<Key>
{
public:
	typedef tsl::robin_set<Key>	Container;
	fastest_hashset() :Container() {}
};

template<>
class fastest_hashset<std::string> : public tsl::robin_set<std::string, string_hash>
{
public:
	typedef tsl::robin_set<std::string, string_hash>	Container;
	fastest_hashset() :Container() {}
};

class StringKey : public std::string
{
public:
	StringKey(const char* s):std::string(s){}
};

typedef fastest_hashset<std::string> CodeSet;

NS_WTP_END;
