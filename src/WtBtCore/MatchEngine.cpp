#include "MatchEngine.h"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../WTSTools/WTSLogger.h"

#define PRICE_DOUBLE_TO_INT_P(x) ((int32_t)((x)*10000.0 + 0.5))
#define PRICE_DOUBLE_TO_INT_N(x) ((int32_t)((x)*10000.0 - 0.5))
#define PRICE_DOUBLE_TO_INT(x) (((x)==DBL_MAX)?0:((x)>0?PRICE_DOUBLE_TO_INT_P(x):PRICE_DOUBLE_TO_INT_N(x)))

extern uint32_t makeLocalOrderID();

void MatchEngine::init(WTSVariant* cfg)
{
	if (cfg == NULL)
		return;

	_cancelrate = cfg->getDouble("cancelrate");
}

void MatchEngine::clear()
{
	_orders.clear();
}

void MatchEngine::fire_orders(const char* stdCode, OrderIDs& to_erase)
{
	for (auto& v : _orders)
	{
		uint32_t localid = v.first;
		OrderInfo& ordInfo = (OrderInfo&)v.second;

		if (ordInfo._state == 0)	//��Ҫ����
		{
			_sink->handle_entrust(localid, stdCode, true, "", ordInfo._time);
			_sink->handle_order(localid, stdCode, ordInfo._buy, ordInfo._left, ordInfo._limit, false, ordInfo._time);
			ordInfo._state = 1;
		}
	}
}

void MatchEngine::match_orders(WTSTickData* curTick, OrderIDs& to_erase)
{
	uint64_t curTime = (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime();
	uint64_t curUnixTime = TimeUtils::makeTime(curTick->actiondate(), curTick->actiontime());

	for (auto& v : _orders)
	{
		uint32_t localid = v.first;
		OrderInfo& ordInfo = (OrderInfo&)v.second;

		if (ordInfo._state == 9)//Ҫ����
		{
			_sink->handle_order(localid, ordInfo._code, ordInfo._buy, 0, ordInfo._limit, true, ordInfo._time);
			ordInfo._state = 99;

			to_erase.emplace_back(localid);

			WTSLogger::info("����{}�ѳ���, ʣ������: {}", localid, ordInfo._left*(ordInfo._buy ? 1 : -1));
			ordInfo._left = 0;
			continue;
		}

		if (ordInfo._state != 1 || curTick->volume() == 0)
			continue;

		if (ordInfo._buy)
		{
			double price;
			double volume;

			//���������Ͱ��ն��ּ�
			if (ordInfo._positive)
			{
				price = curTick->askprice(0);
				volume = curTick->askqty(0);
			}
			else
			{
				price = curTick->price();
				volume = curTick->volume();
			}

			if (decimal::le(price, ordInfo._limit))
			{
				//����۸����,��Ҫ�ȿ��Ŷ�λ��,����۸񲻵�˵���Ѿ�ȫ�����󵥳Ե���
				if (!ordInfo._positive && decimal::eq(price, ordInfo._limit))
				{
					double& quepos = ordInfo._queue;

					//����ɽ���С���Ŷ�λ��,���ܳɽ�
					if (volume <= quepos)
					{
						quepos -= volume;
						continue;
					}
					else if (quepos != 0)
					{
						//����ɽ��������Ŷ�λ��,����Գɽ�
						volume -= quepos;
						quepos = 0;
					}
				}
				else if (!ordInfo._positive)
				{
					volume = ordInfo._left;
				}

				double qty = min(volume, ordInfo._left);
				if (decimal::eq(qty, 0.0))
					qty = 1;

				_sink->handle_trade(localid, ordInfo._code, ordInfo._buy, qty, ordInfo._price, price, ordInfo._time);

				ordInfo._traded += qty;
				ordInfo._left -= qty;

				_sink->handle_order(localid, ordInfo._code, ordInfo._buy, ordInfo._left, price, false, ordInfo._time);

				if (ordInfo._left == 0)
					to_erase.emplace_back(localid);
			}
		}

		if (!ordInfo._buy)
		{
			double price;
			double volume;

			//���������Ͱ��ն��ּ�
			if (ordInfo._positive)
			{
				price = curTick->bidprice(0);
				volume = curTick->bidqty(0);
			}
			else
			{
				price = curTick->price();
				volume = curTick->volume();
			}

			if (decimal::ge(price, ordInfo._limit))
			{
				//����۸����,��Ҫ�ȿ��Ŷ�λ��,����۸񲻵�˵���Ѿ�ȫ�����󵥳Ե���
				if (!ordInfo._positive && decimal::eq(price, ordInfo._limit))
				{
					double& quepos = ordInfo._queue;

					//����ɽ���С���Ŷ�λ��,���ܳɽ�
					if (volume <= quepos)
					{
						quepos -= volume;
						continue;
					}
					else if (quepos != 0)
					{
						//����ɽ��������Ŷ�λ��,����Գɽ�
						volume -= quepos;
						quepos = 0;
					}
				}
				else if (!ordInfo._positive)
				{
					volume = ordInfo._left;
				}

				double qty = min(volume, ordInfo._left);
				if (decimal::eq(qty, 0.0))
					qty = 1;

				_sink->handle_trade(localid, ordInfo._code, ordInfo._buy, qty, ordInfo._price, price, ordInfo._time);
				ordInfo._traded += qty;
				ordInfo._left -= qty;

				_sink->handle_order(localid, ordInfo._code, ordInfo._buy, ordInfo._left, price, false, ordInfo._time);

				if (ordInfo._left == 0)
					to_erase.emplace_back(localid);
			}

		}
	}
}

void MatchEngine::update_lob(WTSTickData* curTick)
{
	LmtOrdBook& curBook = _lmt_ord_books[curTick->code()];
	curBook._cur_px = PRICE_DOUBLE_TO_INT(curTick->price());
	curBook._ask_px = PRICE_DOUBLE_TO_INT(curTick->askprice(0));
	curBook._bid_px = PRICE_DOUBLE_TO_INT(curTick->bidprice(0));

	for (uint32_t i = 0; i < 10; i++)
	{
		if (PRICE_DOUBLE_TO_INT(curTick->askprice(i)) == 0 && PRICE_DOUBLE_TO_INT(curTick->bidprice(i)) == 0)
			break;

		uint32_t px = PRICE_DOUBLE_TO_INT(curTick->askprice(i));
		if (px != 0)
		{
			double& volume = curBook._items[px];
			volume = curTick->askqty(i);
		}

		px = PRICE_DOUBLE_TO_INT(curTick->bidprice(i));
		if (px != 0)
		{
			double& volume = curBook._items[px];
			volume = curTick->askqty(i);
		}
	}

	//��һ����һ֮��ı��۱���ȫ�������
	if (!curBook._items.empty())
	{
		auto sit = curBook._items.lower_bound(curBook._bid_px);
		if (sit->first == curBook._bid_px)
			sit++;

		auto eit = curBook._items.lower_bound(curBook._ask_px);

		if (sit->first <= eit->first)
			curBook._items.erase(sit, eit);
	}
}


OrderIDs MatchEngine::buy(const char* stdCode, double price, double qty, uint64_t curTime)
{
	WTSTickData* lastTick = grab_last_tick(stdCode);
	if (lastTick == NULL)
		return OrderIDs();

	uint32_t localid = makeLocalOrderID();
	OrderInfo& ordInfo = _orders[localid];
	strcpy(ordInfo._code, stdCode);
	ordInfo._buy = true;
	ordInfo._limit = price;
	ordInfo._qty = qty;
	ordInfo._left = qty;
	ordInfo._price = lastTick->price();

	//�����Ŷ�,����Ƕ��ּ�,���ն��ּ۵Ĺҵ������Ŷ�
	//��������¼�,������һ��һ�ļ�Ȩƽ��
	if (decimal::ge(price, lastTick->askprice(0)))
		ordInfo._positive = true;
	else if (decimal::eq(price, lastTick->bidprice(0)))
		ordInfo._queue = lastTick->bidqty(0);
	if (decimal::eq(price, lastTick->price()))
		ordInfo._queue = (uint32_t)round((lastTick->askqty(0)*lastTick->askprice(0) + lastTick->bidqty(0)*lastTick->bidprice(0)) / (lastTick->askprice(0) + lastTick->bidprice(0)));

	//�Ŷ�λ�ð���ƽ��������,����������
	ordInfo._queue -= (uint32_t)round(ordInfo._queue*_cancelrate);
	ordInfo._time = curTime;

	lastTick->release();

	OrderIDs ret;
	ret.emplace_back(localid);
	return ret;
}

OrderIDs MatchEngine::sell(const char* stdCode, double price, double qty, uint64_t curTime)
{
	WTSTickData* lastTick = grab_last_tick(stdCode);
	if (lastTick == NULL)
		return OrderIDs();

	uint32_t localid = makeLocalOrderID();
	OrderInfo& ordInfo = _orders[localid];
	strcpy(ordInfo._code, stdCode);
	ordInfo._buy = false;
	ordInfo._limit = price;
	ordInfo._qty = qty;
	ordInfo._left = qty;
	ordInfo._price = lastTick->price();

	//�����Ŷ�,����Ƕ��ּ�,���ն��ּ۵Ĺҵ������Ŷ�
	//��������¼�,������һ��һ�ļ�Ȩƽ��
	if (decimal::eq(price, lastTick->askprice(0)))
		ordInfo._queue = lastTick->askqty(0);
	else if (decimal::le(price, lastTick->bidprice(0)))
		ordInfo._positive = true;
	if (decimal::eq(price, lastTick->price()))
		ordInfo._queue = (uint32_t)round((lastTick->askqty(0)*lastTick->askprice(0) + lastTick->bidqty(0)*lastTick->bidprice(0)) / (lastTick->askprice(0) + lastTick->bidprice(0)));

	ordInfo._queue -= (uint32_t)round(ordInfo._queue*_cancelrate);
	ordInfo._time = curTime;

	lastTick->release();

	OrderIDs ret;
	ret.emplace_back(localid);
	return ret;
}

OrderIDs MatchEngine::cancel(const char* stdCode, bool isBuy, double qty, FuncCancelCallback cb)
{
	OrderIDs ret;
	for (auto& v : _orders)
	{
		OrderInfo& ordInfo = (OrderInfo&)v.second;
		if (ordInfo._state != 1)
			continue;

		double left = qty;
		if (ordInfo._buy == isBuy)
		{
			uint32_t localid = v.first;
			ret.emplace_back(localid);
			ordInfo._state = 9;
			cb(ordInfo._left*(ordInfo._buy ? 1 : -1));

			if (qty != 0)
			{
				if ((int32_t)left <= ordInfo._left)
					break;

				left -= ordInfo._left;
			}
		}
	}

	return ret;
}

double MatchEngine::cancel(uint32_t localid)
{
	auto it = _orders.find(localid);
	if (it == _orders.end())
		return 0.0;

	OrderInfo& ordInfo = (OrderInfo&)it->second;
	ordInfo._state = 9;

	return ordInfo._left*(ordInfo._buy ? 1 : -1);
}

void MatchEngine::handle_tick(const char* stdCode, WTSTickData* curTick)
{
	if (NULL == curTick)
		return;

	if (NULL == _tick_cache)
		_tick_cache = WTSTickCache::create();

	_tick_cache->add(stdCode, curTick, true);

	update_lob(curTick);

	OrderIDs to_erase;
	//��鶩��״̬
	fire_orders(stdCode, to_erase);

	//���
	match_orders(curTick, to_erase);

	for (uint32_t localid : to_erase)
	{
		auto it = _orders.find(localid);
		if (it != _orders.end())
			_orders.erase(it);
	}
}

WTSTickData* MatchEngine::grab_last_tick(const char* stdCode)
{
	if (NULL == _tick_cache)
		return NULL;

	return (WTSTickData*)_tick_cache->grab(stdCode);
}