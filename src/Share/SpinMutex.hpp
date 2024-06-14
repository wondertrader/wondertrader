#pragma once

#if 0
/*
 *	By Wesley @ 2023.12.15
 *  boos的spinlock总是会死锁，不知道是不是我的用法
 */
#include <boost/smart_ptr/detail/spinlock.hpp>

typedef boost::detail::spinlock SpinMutex;
typedef boost::detail::spinlock::scoped_lock SpinLock;

#else

#include <atomic>
#ifdef _MSC_VER
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

class SpinMutex
{
private:
	std::atomic<bool> flag = { false };

public:
	inline void lock() noexcept
	{
		for (;;)
		{
			if (!flag.exchange(true, std::memory_order_acquire))
				break;

			while (flag.load(std::memory_order_relaxed))
			{
#ifdef _MSC_VER
				_mm_pause();
#else
				__builtin_ia32_pause();
#endif
			}
		}
	}

	inline void unlock() noexcept
	{
		flag.store(false, std::memory_order_release);
	}
};

class SpinLock
{
public:
	SpinLock(SpinMutex& mtx) noexcept
		:_mutex(mtx) { _mutex.lock(); }
	SpinLock(const SpinLock&) = delete;
	SpinLock& operator=(const SpinLock&) = delete;
	~SpinLock() noexcept { _mutex.unlock(); }

private:
	SpinMutex&	_mutex;
};
#endif
