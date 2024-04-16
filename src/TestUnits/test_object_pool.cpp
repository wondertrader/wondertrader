#include <boost/pool/pool.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/null_mutex.hpp>

#include "../Share/fmtlib.h"

//线程安全 boost::mutex
//非线程使用 boost::null_mutex
template < class obj_type>
class enable_obj_pool
{
	boost::pool<> m_pool;

public:
	enable_obj_pool():m_pool(sizeof(obj_type)){}
	virtual ~enable_obj_pool() {}

	obj_type* construct()
	{
		void * mem = m_pool.malloc();
		if (!mem)
			return nullptr;

		obj_type* pobj = new(mem) obj_type();
		return pobj;
	}

	void destroy(obj_type* pobj)
	{
		pobj->~obj_type();
		m_pool.free(pobj);
	}

	//手动释放未使用的内存
	void release()
	{
		m_pool.release_memory();
	}
};




#include "gtest/gtest/gtest.h"
#include "../Share/TimeUtils.hpp"
#include <boost/pool/object_pool.hpp>

class A
{
private:
	uint32_t	_x;
	std::string _y;
	uint64_t	_z;
	bool		_w;
};

TEST(test_object_pool, test_object_pool)
{
	boost::object_pool<A> boost_pool;
	enable_obj_pool<A> opt_pool;
	uint32_t times = 100000000;
	TimeUtils::Ticker ticker;
	for (uint32_t i = 0; i < times; i++)
	{
		A* p = boost_pool.construct();
		boost_pool.destroy(p);
	}
	uint64_t time_a = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		A* p = opt_pool.construct();
		opt_pool.destroy(p);
	}
	uint64_t time_b = ticker.nano_seconds();

	ticker.reset();
	for (uint32_t i = 0; i < times; i++)
	{
		volatile A* p = new A;
		delete p;
	}
	uint64_t time_c = ticker.nano_seconds();
	fmt::print("construct and destroy object for {} times\n", times);
	fmt::print("boost::object_pool: {}ns \n", time_a);
	fmt::print("optimized_object_pool: {}ns\n", time_b);
	fmt::print("no_pool: {}ns\n", time_c);
}