/*!
 * \file StdUtils.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief C++标准库一些定义的简单封装,方便调用
 */
#pragma once

#include <stdint.h>
#include <string>
#include <charconv>


//////////////////////////////////////////////////////////////////////////
//
namespace convert
{
	template <typename T>
	static const char* to_str(T val)
	{
		thread_local static char buffer[64] = { 0 };
		std::to_chars(buffer, buffer + 64, val);
		return buffer;
	}

	template <typename T>
	static const char* to_str(char* buffer, std::size_t len, T val)
	{
		std::to_chars(buffer, buffer + len, val);
		return buffer;
	}

	template <typename T>
	static T from_str(const char* str)
	{
		T ret{};
		std::from_chars(str, str + strlen(str), ret);
		return ret;
	}

	template <typename T>
	static T from_str(std::string& str)
	{
		T ret;
		std::from_chars(str.begin(), str.end(), ret);
		return ret;
	}

	constexpr int32_t to_int32(const char *str, int32_t h = 0) noexcept
	{
		return *str ? to_int32(str + 1, h * 10 + *str - '0') : h;
	}

	constexpr uint32_t to_uint32(const char *str, uint32_t h = 0) noexcept
	{
		return *str ? to_uint32(str + 1, h * 10 + *str - '0') : h;
	}

	constexpr int64_t to_int64(const char *str, int64_t h = 0) noexcept
	{
		return *str ? to_int64(str + 1, h * 10 + *str - '0') : h;
	}

	constexpr uint64_t to_uint64(const char *str, uint64_t h = 0) noexcept
	{
		return *str ? to_uint64(str + 1, h * 10 + *str - '0') : h;
	}
};
