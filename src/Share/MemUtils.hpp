#pragma once
#include <boost/pool/pool.hpp>
#include <atomic>

template < typename T>
class ObjectPool
{
	boost::pool<> _pool;

public:
	ObjectPool() :_pool(sizeof(T)) {}
	virtual ~ObjectPool() {}

	T* construct()
	{
		void * mem = _pool.malloc();
		if (!mem)
			return nullptr;

		T* pobj = new(mem) T();
		return pobj;
	}

	void destroy(T* pobj)
	{
		pobj->~T();
		_pool.free(pobj);
	}

	//手动释放未使用的内存
	void release()
	{
		_pool.release_memory();
	}
};


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
