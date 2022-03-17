#pragma once
#include <string.h>
#include <boost/container_hash/hash.hpp>
#include "WTSMarcos.h"

#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"

NS_WTP_BEGIN

#define faster_hashmap tsl::robin_map
#define faster_hashset tsl::robin_set

typedef struct _Longkey //char[32]
{
	uint64_t	_buf[4] = { 0 };

	_Longkey()
	{
	}

	_Longkey(const char* s)
	{
		auto len = strlen(s);
		memcpy(_buf , s, len);
	}

	_Longkey(const std::string& s)
	{
		memcpy(_buf, s.c_str(), s.size());
	}

	_Longkey(const _Longkey& rhs)
	{
		memcpy(_buf, rhs._buf, 32);
	}

	bool operator ==(const _Longkey& b) const
	{
		return (_buf[0] == b._buf[0] && _buf[1] == b._buf[1] && _buf[2] == b._buf[2] && _buf[3] == b._buf[3]);
	}

	const char* c_str() const { return (const char*)_buf; }

	inline std::size_t find(char ch) const
	{
		const char* s = (const char*)_buf;
		for (std::size_t i = 0; i < 32; i++)
		{
			if (s[i] == ch)
				return i;
		}

		return std::string::npos;
	}

	inline std::size_t size() const { return strlen((char*)_buf); }

} LongKey;

typedef faster_hashset<LongKey> CodeSet;

typedef struct _ShortKey //char[32]
{
	uint64_t	_buf[2] = { 0 };

	_ShortKey()
	{
	}

	_ShortKey(const char* s)
	{
		auto len = strlen(s);
		memcpy(_buf, s, len);
	}

	_ShortKey(const std::string& s)
	{
		memcpy(_buf, s.c_str(), s.size());
	}

	bool operator ==(const _ShortKey& b) const
	{
		return (_buf[0] == b._buf[0] && _buf[1] == b._buf[1]);
	}

	const char* c_str() const { return (const char*)_buf; }

	inline std::size_t find(char ch) const
	{
		const char* s = (const char*)_buf;
		for (std::size_t i = 0; i < 16; i++)
		{
			if (s[i] == ch)
				return i;
		}

		return std::string::npos;
	}

	inline std::size_t size() const { return strlen((char*)_buf); }
} ShortKey;

NS_WTP_END;

namespace std
{
	template<>
	struct hash<wtp::LongKey>
	{
		size_t operator()(const wtp::LongKey& key) const
		{
			size_t ret = 17;
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[0]);
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[1]);
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[2]);
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[3]);
			return ret;
		}
	};

	template<>
	struct hash<wtp::ShortKey>
	{
		size_t operator()(const wtp::ShortKey& key) const
		{
			size_t ret = 17;
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[0]);
			ret = ret * 31 + boost::hash<uint64_t>()(key._buf[1]);
			return ret;
		}
	};
};