/*!
 * \file StrUtil.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 字符串处理的封装
 */
#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <cctype>

typedef std::vector<std::string> StringVector;

namespace StrUtil
{	

	/** Removes any whitespace characters, be it standard space or
	TABs and so on.
	@remarks
	The user may specify wether they want to trim only the
	beginning or the end of the std::string ( the default action is
	to trim both).
	*/
	static inline void trim(std::string& str, const char* delims = " \t\r", bool left = true, bool right = true) noexcept
	{
		if(right)
			str.erase(str.find_last_not_of(delims)+1);
		if(left)
			str.erase(0, str.find_first_not_of(delims));
	}

	static inline std::string trim(const char* str, const char* delims = " \t\r", bool left = true, bool right = true) noexcept
	{
		std::string ret = str;
		if(right)
			ret.erase(ret.find_last_not_of(delims)+1);
		if(left)
			ret.erase(0, ret.find_first_not_of(delims));

		return std::move(ret);
	}

	constexpr static inline std::size_t findFirst(const char* str, char ch) noexcept
	{
		std::size_t i = 0;
		for(;;)
		{
			if (str[i] == ch)
				return i;

			if(str[i] == '\0')
				break;

			i++;
		}

		return std::string::npos;
	}

	static inline std::size_t findLast(const char* str, char ch) noexcept
	{
		auto len = strlen(str);
		std::size_t i = 0;
		for (; i < len; i++)
		{
			if (str[len - 1 - i] == ch)
				return len - 1 - i;
		}

		return std::string::npos;
	}

	static inline StringVector split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0) noexcept
	{
		StringVector ret;
		unsigned int numSplits = 0;

		// Use STL methods
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				ret.emplace_back("");
				// Do nothing
				start = pos + 1;
			}
			else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the std::string
				ret.emplace_back( str.substr(start) );
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.emplace_back( str.substr(start, pos - start) );
				start = pos + 1;
			}
			// parse up to next real data
			//start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != std::string::npos);
		return std::move(ret);
	}

	static inline void split(const std::string& str, StringVector& ret, const std::string& delims = "\t\n ", unsigned int maxSplits = 0) noexcept
	{
		unsigned int numSplits = 0;

		// Use STL methods
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				ret.emplace_back("");
				// Do nothing
				start = pos + 1;
			}
			else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the std::string
				ret.emplace_back(str.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.emplace_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			//start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != std::string::npos);
	}

	/** Upper-cases all the characters in the std::string.
	*/
	static inline void toLowerCase( std::string& str ) noexcept
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			(int(*)(int))tolower);

	}

	/** Lower-cases all the characters in the std::string.
	*/
	static inline void toUpperCase( std::string& str ) noexcept
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			(int(*)(int))toupper);
	}

	static inline std::string makeLowerCase(const char* str)
	{
		std::string strRet = str;
		std::transform(
			strRet.begin(),
			strRet.end(),
			strRet.begin(),
			(int(*)(int))tolower);
		return std::move(strRet);
	}

	static inline std::string makeUpperCase(const char* str) noexcept
	{
		std::string strRet = str;
		std::transform(
			strRet.begin(),
			strRet.end(),
			strRet.begin(),
			(int(*)(int))toupper);
		return std::move(strRet);
	}

	/*
	 *	检查是否以指定的字符串开始
	 *	@str		要检查的字符串
	 *	@pattern	要匹配的模板
	 *	@ignroreCase是否忽略大小写
	 */
	static inline bool startsWith(const char* str, const char* pattern, bool ignoreCase = true) noexcept
	{
		size_t thisLen = strlen(str);
		size_t patternLen = strlen(pattern);
		if (thisLen < patternLen || patternLen == 0)
			return false;

		if(ignoreCase)
		{
#ifdef _MSC_VER
			return _strnicmp(str, pattern, patternLen) == 0;
#else
			return strncasecmp(str, pattern, patternLen) == 0;
#endif
		}
		else
		{
			return strncmp(str, pattern, patternLen) == 0;
		}
	}

	/*
	 *	检查是否以指定的字符串结束
	 *	@str		要检查的字符串
	 *	@pattern	要匹配的模板
	 *	@ignroreCase是否忽略大小写
	 */
	static inline bool endsWith(const char* str, const char* pattern, bool ignoreCase = true) noexcept
	{
		size_t thisLen = strlen(str);
		size_t patternLen = strlen(pattern);
		if (thisLen < patternLen || patternLen == 0)
			return false;

		const char* s = str + (thisLen - patternLen);

		if (ignoreCase)
		{
#ifdef _MSC_VER
			return _strnicmp(s, pattern, patternLen) == 0;
#else
			return strncasecmp(s, pattern, patternLen) == 0;
#endif
		}
		else
		{
			return strncmp(s, pattern, patternLen) == 0;
		}
	}

	/** Method for standardising paths - use forward slashes only, end with slash.
	*/
	static inline std::string standardisePath( const std::string &init, bool bIsDir = true) noexcept
	{
		std::string path = init;

		std::replace( path.begin(), path.end(), '\\', '/' );
		if (path[path.length() - 1] != '/' && bIsDir)
			path += '/';

		return std::move(path);
	}

	static inline void replace(std::string& str, const char* src, const char* des) noexcept
	{
		std::string ret = "";
		std::size_t srcLen = strlen(src);
		std::size_t lastPos = 0;
		std::size_t pos = str.find(src);
		while(pos != std::string::npos)
		{
			ret += str.substr(lastPos, pos-lastPos);
			ret += des;

			lastPos = pos + srcLen;
			pos = str.find(src, lastPos);
		}
		ret += str.substr(lastPos, pos);

		str = ret;
	}

	constexpr static inline bool isEmpty(const char* str) noexcept
	{
		return *str == '\0';
	}
};
