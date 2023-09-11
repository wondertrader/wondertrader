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

