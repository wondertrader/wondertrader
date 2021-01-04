#include "WtOrdMon.h"

void WtOrdMon::push_order(const uint32_t* ids, uint32_t cnt, uint64_t curTime)
{
	StdLocker<StdRecurMutex> lock(_mtx_ords);
	for (uint32_t idx = 0; idx < cnt; idx++)
	{
		uint32_t localid = ids[idx];
		_orders[localid] = curTime;
	}
}

void WtOrdMon::erase_order(uint32_t localid)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return;

	_orders.erase(it);
}

void WtOrdMon::check_orders(uint32_t expiresecs, uint64_t curTime, EnumOrderCallback callback)
{
	if (_orders.empty())
		return;

	StdLocker<StdRecurMutex> lock(_mtx_ords);
	for (auto& m : _orders)
	{
		uint32_t localid = m.first;
		uint64_t entertm = m.second;

		if (curTime - entertm < expiresecs * 1000)
			continue;

		callback(m.first);
	}
}

