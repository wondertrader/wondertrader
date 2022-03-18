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

class StrUtil
{
public:

	

	/** Removes any whitespace characters, be it standard space or
	TABs and so on.
	@remarks
	The user may specify wether they want to trim only the
	beginning or the end of the std::string ( the default action is
	to trim both).
	*/
	static inline void trim(std::string& str, const char* delims = " \t\r", bool left = true, bool right = true)
	{
		if(right)
			str.erase(str.find_last_not_of(delims)+1);
		if(left)
			str.erase(0, str.find_first_not_of(delims));
	}

	static inline std::string trim(const char* str, const char* delims = " \t\r", bool left = true, bool right = true)
	{
		std::string ret = str;
		if(right)
			ret.erase(ret.find_last_not_of(delims)+1);
		if(left)
			ret.erase(0, ret.find_first_not_of(delims));

		return std::move(ret);
	}

	//去掉所有空格
	static inline void trimAllSpace(std::string &str)
	{
		std::string::iterator destEnd = std::remove_if(str.begin(), str.end(), [](const char& c){
			return c == ' ';
		});
		str.resize(destEnd-str.begin());
	}

	//去除所有特定字符
	static inline void trimAll(std::string &str,char ch)
	{
		std::string::iterator destEnd=std::remove_if(str.begin(),str.end(),std::bind1st(std::equal_to<char>(),ch));
		str.resize(destEnd-str.begin());
	}

	static inline std::size_t findFirst(const char* str, char ch)
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

	/** Returns a std::stringVector that contains all the substd::strings delimited
	by the characters in the passed <code>delims</code> argument.
	@param
	delims A list of delimiter characters to split by
	@param
	maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
	parameters is > 0, the splitting process will stop after this many splits, left to right.
	*/
	static inline StringVector split( const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
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

	/** Returns a std::stringVector that contains all the substd::strings delimited
	by the characters in the passed <code>delims</code> argument.
	@param
	delims A list of delimiter characters to split by
	@param
	maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
	parameters is > 0, the splitting process will stop after this many splits, left to right.
	*/
	static inline void split(const std::string& str, StringVector& ret, const std::string& delims = "\t\n ", unsigned int maxSplits = 0)
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
	static inline void toLowerCase( std::string& str )
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			(int(*)(int))tolower);

	}

	/** Lower-cases all the characters in the std::string.
	*/
	static inline void toUpperCase( std::string& str )
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

	static inline std::string makeUpperCase(const char* str)
	{
		std::string strRet = str;
		std::transform(
			strRet.begin(),
			strRet.end(),
			strRet.begin(),
			(int(*)(int))toupper);
		return std::move(strRet);
	}

	/** Converts the contents of the std::string to a float.
	@remarks
	Assumes the only contents of the std::string are a valid parsable float. Defaults to  a
	value of 0.0 if conversion is not possible.
	*/
	static inline float toFloat( const std::string& str )
	{
		return (float)atof(str.c_str());
	}

	static inline double toDouble( const std::string& str )
	{
		return atof(str.c_str());
	}

	/** Returns whether the std::string begins with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the std::string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static inline bool startsWith(const std::string& str, const std::string& pattern, bool lowerCase = true)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		std::string startOfThis = str.substr(0, patternLen);
		if (lowerCase)
			toLowerCase(startOfThis);

		return (startOfThis == pattern);
	}

	/** Returns whether the std::string ends with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the std::string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static inline bool endsWith(const std::string& str, const std::string& pattern, bool lowerCase = true)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		std::string endOfThis = str.substr(thisLen - patternLen, patternLen);
		if (lowerCase)
			toLowerCase(endOfThis);

		return (endOfThis == pattern);
	}

	/** Method for standardising paths - use forward slashes only, end with slash.
	*/
	static inline std::string standardisePath( const std::string &init, bool bIsDir = true)
	{
		std::string path = init;

		std::replace( path.begin(), path.end(), '\\', '/' );
		if (path[path.length() - 1] != '/' && bIsDir)
			path += '/';

		return std::move(path);
	}

	/** Method for splitting a fully qualified filename into the base name
	and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static inline void splitFilename(const std::string& qualifiedName,std::string& outBasename, std::string& outPath)
	{
		std::string path = qualifiedName;
		// Replace \ with / first
		std::replace( path.begin(), path.end(), '\\', '/' );
		// split based on final /
		size_t i = path.find_last_of('/');

		if (i == std::string::npos)
		{
			outPath = "";
			outBasename = qualifiedName;
		}
		else
		{
			outBasename = path.substr(i+1, path.size() - i - 1);
			outPath = path.substr(0, i+1);
		}
	}

	/** Simple pattern-matching routine allowing a wildcard pattern.
	@param str std::string to test
	@param pattern Pattern to match against; can include simple '*' wildcards
	@param caseSensitive Whether the match is case sensitive or not
	*/
	static inline bool match(const std::string& str, const std::string& pattern, bool caseSensitive = true)
	{
		std::string tmpStr = str;
		std::string tmpPattern = pattern;
		if (!caseSensitive)
		{
			toLowerCase(tmpStr);
			toLowerCase(tmpPattern);
		}

		std::string::const_iterator strIt = tmpStr.begin();
		std::string::const_iterator patIt = tmpPattern.begin();
		std::string::const_iterator lastWildCardIt = tmpPattern.end();
		while (strIt != tmpStr.end() && patIt != tmpPattern.end())
		{
			if (*patIt == '*')
			{
				lastWildCardIt = patIt;
				// Skip over looking for next character
				++patIt;
				if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the string
					strIt = tmpStr.end();
				}
				else
				{
					// scan until we find next pattern character
					while(strIt != tmpStr.end() && *strIt != *patIt)
						++strIt;
				}
			}
			else
			{
				if (*patIt != *strIt)
				{
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
				}
				else
				{
					++patIt;
					++strIt;
				}
			}

		}
		// If we reached the end of both the pattern and the string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// Constant blank std::string, useful for returning by ref where local does not exist
	static inline const std::string BLANK()
	{
		static const std::string temp = std::string("");
		return std::move(temp);
	}

	//地球人都知道,恶心的std::string是没有CString的Format这个函数的,所以我们自己造
	static inline std::string printf(const char *pszFormat, ...)
	{
		va_list argptr;
		va_start(argptr, pszFormat);
		std::string result=printf(pszFormat,argptr);
		va_end(argptr);
		return std::move(result);
	}

	//地球人都知道,恶心的std::string是没有CString的Format这个函数的,所以我们自己造
	static inline std::string printf2(const char *pszFormat, ...)
	{
		va_list argptr;
		va_start(argptr, pszFormat);
		std::string result=printf2(pszFormat,argptr);
		va_end(argptr);
		return std::move(result);
	}

	//地球人都知道,恶心的std::string是没有CString的Format这个函数的,所以我们自己造
	static inline std::string printf2(const char *pszFormat,va_list argptr)
	{
		int         size   = 1024;
		char*       buffer = new char[size];

		while (1)
		{
#ifdef _MSC_VER
			int n = _vsnprintf(buffer, size, pszFormat, argptr);
#else
			int n = vsnprintf(buffer, size, pszFormat, argptr);
#endif

			// If that worked, return a string.
			if (n > -1 && n < size)
			{
				std::string s(buffer);
				delete [] buffer;
				return s;
			}

			if (n > -1)     size  = n+1; // ISO/IEC 9899:1999
			else            size *= 2;   // twice the old size

			delete [] buffer;
			buffer = new char[size];
		}
	}

	static inline std::string extend(const char* str, uint32_t length)
	{
		if(strlen(str) >= length)
			return str;

		std::string ret = str;
		uint32_t spaces = length - (uint32_t)strlen(str);
		uint32_t former = spaces/2;
		uint32_t after = spaces - former;
		for(uint32_t i = 0; i < former; i++)
		{
			ret.insert(0, " ");
		}

		for(uint32_t i = 0; i < after; i++)
		{
			ret += " ";
		}
		return std::move(ret);
	}

	//地球人都知道,恶心的std::string是没有CString的Format这个函数的,所以我们自己造
	static inline std::string printf(const char* pszFormat, va_list argptr)
	{
		int size = 1024;
		int len=0;
		std::string ret;
		for ( ;; )
		{
			ret.resize(size + 1,0);
			char *buf=(char *)ret.c_str();   
			if ( !buf )
			{
				return BLANK();
			}

			va_list argptrcopy;
			va_copy(argptrcopy, argptr);

#ifdef _MSC_VER
			len = _vsnprintf(buf, size, pszFormat, argptrcopy);
#else
			len = vsnprintf(buf, size, pszFormat, argptrcopy);
#endif
			va_end(argptrcopy);

			if ( len >= 0 && len <= size )
			{
				// ok, there was enough space
				break;
			}
			size *= 2;
		}
		ret.resize(len);
		return std::move(ret);
	}

	//取得右边的N个字符
	static inline std::string right(const std::string &src,size_t nCount)
	{
		if(nCount>src.length())
			return BLANK();
		return std::move(src.substr(src.length()-nCount,nCount));
	}

	//取左边的N个字符
	static inline std::string left(const std::string &src,size_t nCount)
	{
		return std::move(src.substr(0,nCount));
	}

	static inline size_t charCount(const std::string &src,char ch)
	{
		size_t result=0;
		for(size_t i=0;i<src.length();i++)
		{
			if(src[i]==ch)result++;
		}
		return result;
	}

	static inline void replace(std::string& str, const char* src, const char* des)
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

	static inline std::string fmtInt64(int64_t v)
	{
		char buf[64] = { 0 };
#ifdef _MSC_VER
		int pos = sprintf(buf, "%I64d", v);
#else
		int pos = sprintf(buf, "%lld", (long long)v);
#endif
		return buf;
	}

	static inline std::string fmtUInt64(uint64_t v)
	{
		char buf[64] = { 0 };
#ifdef _MSC_VER
		int pos = sprintf(buf, "%I64u", v);
#else
		int pos = sprintf(buf, "%llu", (unsigned long long)v);
#endif
		return buf;
	}
};
