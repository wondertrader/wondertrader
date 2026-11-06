/*!
 * \file FreqLimiter.hpp
 * \project	WonderTrader
 *
 * \brief 滑动时间窗频率统计工具，供交易通道的下单/撤单流控使用
 *
 *	时间戳单位为毫秒，要求按时间递增写入（调用方在写入时取当前时间追加即可）
 *	统计语义：窗口为(now - spanSecs*1000, now]，窗口内已发生的次数达到boundary，则本次操作应被拒绝
 *	即任意spanSecs秒内最多允许boundary次，与配置说明"超过该次数后指令不会再发送"一致
 */
#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>

class FreqLimiter
{
public:
	/*
	 *	裁剪掉窗口之外的旧时间戳，并返回窗口内的事件数
	 *	@stamps		事件时间戳列表，单位毫秒，递增
	 *	@now		当前时间，单位毫秒
	 *	@spanSecs	统计窗口长度，单位秒
	 */
	static inline uint32_t countInWindow(std::vector<uint64_t>& stamps, uint64_t now, uint32_t spanSecs)
	{
		if (stamps.empty())
			return 0;

		uint64_t span = (uint64_t)spanSecs * 1000;
		uint64_t sTime = (now > span) ? (now - span) : 0;

		//窗口是左开右闭区间，所以用upper_bound定位第一个大于sTime的时间戳
		auto it = std::upper_bound(stamps.begin(), stamps.end(), sTime);
		if (it != stamps.begin())
			stamps.erase(stamps.begin(), it);

		return (uint32_t)stamps.size();
	}

	/*
	 *	检查窗口内的次数是否已经达到上限
	 *	@boundary	窗口内允许的最大次数，0表示不限制
	 *	@times		输出参数，窗口内已发生的次数
	 *	返回true表示已达上限，本次操作应被拒绝
	 */
	static inline bool isExceeded(std::vector<uint64_t>& stamps, uint64_t now, uint32_t spanSecs, uint32_t boundary, uint32_t& times)
	{
		times = countInWindow(stamps, now, spanSecs);
		if (boundary == 0)
			return false;

		return times >= boundary;
	}
};
