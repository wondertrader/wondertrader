#pragma once
#include <string.h>
#include "WTSMarcos.h"
#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"

#include "../FasterLibs/ankerl/unordered_dense.h"

/*
 *	By Wesley @ 2023.08.15
 *	很遗憾，robin_map搭配std::string在数据量大的时候（经测试在13106条数据，不同测试机可能具体数值不同）
 *	会出现bad allocate的异常
 *	我猜测是std::string无法像string那样自动优化
 *	所以数据量大的时候，就会占用非常大的内存，当运行环境内存较小时，就会出现异常
 *	所以这次把LongKey和LongKey都注释掉，改成std::string
 */

 /*
  *	By Wesley @ 2023.08.16
  *	ankerl写入速度比robin好很多，大概快1/3，尤其数据量在40w以内的时候
  *	但是robin的读取速度比robin好，不过到了30w条数据以内，差别就不大
  *	按照wondertrader的场景，还是ankerl要好很多
  * 具体可以参考以下页面的性能对比
  * https://martin.ankerl.com/2022/08/27/hashmap-bench-01/#benchmark-results-table
  */

NS_WTP_BEGIN

struct string_hash
{
	//BKDRHash算法
	std::size_t operator()(const std::string& key) const
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

typedef fastest_hashset<std::string> CodeSet;

//////////////////////////////////////////////////////////////////////////
//下面使用unordered_dense

template<class Key, class T, class Hash = std::hash<Key>>
class wt_hashmap : public ankerl::unordered_dense::map<Key, T, Hash>
{
public:
	typedef ankerl::unordered_dense::map<Key, T, Hash>	Container;
	wt_hashmap() :Container() {}
};

template<class T>
class wt_hashmap<std::string, T, string_hash> : public ankerl::unordered_dense::map<std::string, T, string_hash>
{
public:
	typedef ankerl::unordered_dense::map<std::string, T, string_hash>	Container;
	wt_hashmap() :Container() {}
};

template<class Key, class Hash = std::hash<Key>>
class wt_hashset : public ankerl::unordered_dense::set<Key, Hash>
{
public:
	typedef ankerl::unordered_dense::set<Key, Hash>	Container;
	wt_hashset() :Container() {}
};

template<>
class wt_hashset<std::string, string_hash> : public ankerl::unordered_dense::set<std::string, string_hash>
{
public:
	typedef ankerl::unordered_dense::set<std::string, string_hash>	Container;
	wt_hashset() :Container() {}
};

NS_WTP_END
