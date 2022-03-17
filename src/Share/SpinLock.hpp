#pragma once
#include <atomic>

class SpinLock
{
private:
	std::atomic<bool> flag = { false };

public:
	void lock()
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

	void unlock()
	{
		flag.store(false, std::memory_order_release);
	}
};
