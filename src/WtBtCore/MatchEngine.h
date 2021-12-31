#pragma once
#include <stdint.h>
#include <map>
#include <vector>
#include <functional>
#include <string.h>

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSCollection.hpp"
#include "../Includes/FasterDefs.h"

NS_WTP_BEGIN
class WTSTickData;
class WTSVariant;
NS_WTP_END

USING_NS_WTP;

typedef std::vector<uint32_t> OrderIDs;

typedef WTSHashMap<std::string>	WTSTickCache;

class IMatchSink
{
public:
	/*
	 *	成交回报
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	vol		成交数量, 这里没有正负, 通过isBuy确定买入还是卖出
	 *	price	成交价格
	 */
	virtual void handle_trade(uint32_t localid, const char* stdCode, bool isBuy, double vol, double fireprice, double price, uint64_t ordTime) = 0;

	/*
	 *	订单回报
	 *	localid	本地单号
	 *	code	合约代码
	 *	isBuy	买or卖
	 *	leftover	剩余数量
	 *	price	委托价格
	 *	isCanceled	是否已撤销
	 */
	virtual void handle_order(uint32_t localid, const char* stdCode, bool isBuy, double leftover, double price, bool isCanceled, uint64_t ordTime) = 0;

	/*
	 *
	 */
	virtual void handle_entrust(uint32_t localid, const char* stdCode, bool bSuccess, const char* message, uint64_t ordTime) = 0;
};

typedef std::function<void(double)> FuncCancelCallback;

class MatchEngine
{
public:
	MatchEngine() : _tick_cache(NULL),_cancelrate(0), _sink(NULL)
	{

	}
private:
	void	fire_orders(const char* stdCode, OrderIDs& to_erase);
	void	match_orders(WTSTickData* curTick, OrderIDs& to_erase);
	void	update_lob(WTSTickData* curTick);

	inline WTSTickData*	grab_last_tick(const char* stdCode);

public:
	void	init(WTSVariant* cfg);

	void	regisSink(IMatchSink* sink) { _sink = sink; }

	void	clear();

	void	handle_tick(const char* stdCode, WTSTickData* curTick);

	OrderIDs	buy(const char* stdCode, double price, double qty, uint64_t curTime);
	OrderIDs	sell(const char* stdCode, double price, double qty, uint64_t curTime);
	double		cancel(uint32_t localid);
	virtual OrderIDs cancel(const char* stdCode, bool isBuy, double qty, FuncCancelCallback cb);

private:
	typedef struct _OrderInfo
	{
		char		_code[32];
		bool		_buy;
		double		_qty;
		double		_left;
		double		_traded;
		double		_limit;
		double		_price;
		uint32_t	_state;
		uint64_t	_time;
		double		_queue;
		bool		_positive;

		_OrderInfo()
		{
			memset(this, 0, sizeof(_OrderInfo));
		}
	} OrderInfo;

	typedef faster_hashmap<uint32_t, OrderInfo> Orders;
	Orders	_orders;

	typedef std::map<uint32_t, double>	LOBItems;

	typedef struct _LmtOrdBook
	{
		LOBItems	_items;
		uint32_t	_cur_px;
		uint32_t	_ask_px;
		uint32_t	_bid_px;

		void clear()
		{
			_items.clear();
			_cur_px = 0;
			_ask_px = 0;
			_bid_px = 0;
		}

		_LmtOrdBook()
		{
			_cur_px = 0;
			_ask_px = 0;
			_bid_px = 0;
		}
	} LmtOrdBook;
	typedef faster_hashmap<std::string, LmtOrdBook> LmtOrdBooks;
	LmtOrdBooks	_lmt_ord_books;

	IMatchSink*	_sink;

	double			_cancelrate;
	WTSTickCache*	_tick_cache;
};

