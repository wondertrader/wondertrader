#pragma once
#include <unordered_map>
#include <stdint.h>
#include <functional>

#include "../Share/StdUtils.hpp"

typedef std::function<void(uint32_t)> EnumOrderCallback;

/*
 *	订单管理器
 */
class WtOrdMon
{
public:
	/*
	 *	添加订单
	 *	
	 */
	void push_order(const uint32_t* ids, uint32_t cnt, uint64_t curTime, bool bCanCancel = true);

	void erase_order(uint32_t localid);

	/*
	 *	检查是否有订单
	 *	@localid	订单号,为0时检查是否有任意订单,不为0时检查是否有指定订单
	 */
	inline bool has_order(uint32_t localid = 0)
	{
		if (localid == 0)
			return !_orders.empty();

		auto it = _orders.find(localid);
		if (it == _orders.end())
			return false;

		return true;
	}

	void check_orders(uint32_t expiresecs, uint64_t curTime, EnumOrderCallback callback);

	inline void clear_orders()
	{
		_orders.clear();
	}

private:
	typedef std::pair<uint64_t, bool> OrderPair;	//uint64_t - entertime, bool - cancancel
	typedef std::unordered_map<uint32_t, OrderPair> IDMap;
	IDMap			_orders;
	StdRecurMutex	_mtx_ords;
};

