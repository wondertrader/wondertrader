#include "TraderMocker.h"

#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSError.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/decimal.h"
#include "../Share/StrUtil.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
namespace rj = rapidjson;

//By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(ITraderSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	std::string s = std::move(fmt::sprintf(format, args...));
	strcpy(buffer, s.c_str());

	sink->handleTraderLog(ll, buffer);
}

extern "C"
{
	EXPORT_FLAG ITraderApi* createTrader()
	{
		TraderMocker *instance = new TraderMocker();
		return instance;
	}

	EXPORT_FLAG void deleteTrader(ITraderApi* &trader)
	{
		if (NULL != trader)
		{
			delete trader;
			trader = NULL;
		}
	}
}

std::vector<uint32_t> splitVolume(uint32_t vol, uint32_t minQty = 1, uint32_t maxQty = 100)
{
	uint32_t length = maxQty - minQty + 1;
	std::vector<uint32_t> ret;
	if (vol <= minQty)
	{
		ret.emplace_back(vol);
	}
	else
	{
		uint32_t left = vol;
		srand((uint32_t)time(NULL));
		while (left > 0)
		{
			uint32_t curVol = minQty + (uint32_t)rand() % length;

			if (curVol >= left)
				curVol = left;

			if (curVol == 0)
				continue;

			ret.emplace_back(curVol);
			left -= curVol;
		}
	}

	return ret;
}
TraderMocker::TraderMocker()
	: _terminated(false)
	, _listener(NULL)
	, _ticks(NULL)
	, _orders(NULL)
	, _awaits(NULL)
	, _trades(NULL)
	, _b_socket(NULL)
	, _max_tick_time(0)
	, _last_match_time(0)
{
	_auto_order_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20200101, 0)) / 1000 * 100);
	_auto_trade_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20200101, 0)) / 1000 * 300);
	_auto_entrust_id = (uint32_t)((TimeUtils::getLocalTimeNow() - TimeUtils::makeTime(20200101, 0)) / 1000 * 100);
}


TraderMocker::~TraderMocker()
{
	if (_orders)
		_orders->release();

	if (_trades)
		_trades->release();

	if (_ticks)
		_ticks->release();
}

uint32_t TraderMocker::makeTradeID()
{
	return ++_auto_trade_id;
}

uint32_t TraderMocker::makeOrderID()
{
	return ++_auto_order_id;
}

bool TraderMocker::makeEntrustID(char* buffer, int length)
{
	if (buffer == NULL || length == 0)
		return false;

	try
	{
		memset(buffer, 0, length);
		sprintf(buffer, "me.%u.%u.%u", TimeUtils::getCurDate(), _mocker_id, _auto_entrust_id++);
		return true;
	}
	catch (...)
	{

	}

	return false;
}

int TraderMocker::orderInsert(WTSEntrust* entrust)
{	
	if (entrust == NULL)
	{
		return 0;
	}

	entrust->retain();
	_io_service.post([this, entrust](){
		StdUniqueLock lock(_mutex_api);

		WTSContractInfo* ct = entrust->getContractInfo();

		/*
		 *	1、开仓无需检查
		 *	2、平仓要先检查可平
		 *	3、检查通过了,平仓还要冻结持仓
		 *	4、还要考虑国际期货的问题
		 */

		bool bPass = false;
		std::string msg;
		do 
		{
			if (ct == NULL)
			{
				bPass = false;
				msg = "品种不存在";
				break;
			}
			WTSCommodityInfo* commInfo = ct->getCommInfo();

			//检查价格类型的合法性
			if (entrust->getPriceType() == WPT_ANYPRICE && commInfo->getPriceMode() == PM_Limit)
			{
				bPass = false;
				msg = "价格类型不合法";
				break;
			}

			//检查数量的合法性
			if ((commInfo->getCategoty() == CC_Stock) && (entrust->getOffsetType() == WOT_OPEN) && !decimal::eq(decimal::mod(entrust->getVolume(), 100), 0))
			{
				bPass = false;
				msg = "股票买入数量必须为100的整数倍";
				break;
			}

			//检查方向的合法性
			if(!commInfo->canShort() && entrust->getDirection() == WDT_SHORT)
			{
				bPass = false;
				msg = "股票不能做空";
				break;
			}

			//检查价格的合法性
			if(!decimal::eq(entrust->getPrice(), 0))
			{
				double pricetick = commInfo->getPriceTick();
				double v = entrust->getPrice() / pricetick;

				if (!decimal::eq(decimal::mod(entrust->getPrice(), pricetick), 0))	//整除的检查方式,先小数相除得到商,然后商取整以后,再跟原来的商相减,如果等于0,则是整除,否则是
				{
					bPass = false;
					msg = "委托价格不合法";
					break;
				}
			}


			//开仓直接通过,不检查资金
			if (entrust->getOffsetType() == WOT_OPEN)
			{
				bPass = true;
				break;
			}

			//如果不需要开平,则直接通过,主要针对国际期货
			if (commInfo->getCoverMode() == CM_None)
			{
				bPass = true;
				break;
			}
			
			//如果区分平昨平今,而委托的是平昨,则直接拒绝,因为mocker为了简化处理,不考虑昨仓
			if (commInfo->getCoverMode() == CM_CoverToday && (entrust->getOffsetType() == WOT_CLOSE || entrust->getOffsetType() == WOT_CLOSEYESTERDAY))
			{
				bPass = false;
				msg = "没有足够的可平仓位";
				break;
			}

			//如果没有持仓或者持仓不够,也要
			auto it = _positions.find(ct->getFullCode());
			if(it == _positions.end())
			{
				bPass = false;
				msg = "没有足够的可平仓位";
				break;
			}

			PosItem& pItem = (PosItem&)it->second;
			bool isLong = entrust->getDirection() == WDT_LONG;

			double validQty = isLong ? (pItem._long._volume - pItem._long._frozen) : (pItem._short._volume - pItem._short._frozen);
			if(decimal::lt(validQty, entrust->getVolume()))
			{
				bPass = false;
				msg = "没有足够的可平仓位";
				break;
			}

			//冻结持仓
			if(isLong)
			{
				pItem._long._frozen += entrust->getVolume();
			}
			else
			{
				pItem._short._frozen += entrust->getVolume();
			}

			bPass = true;
			msg = "下单成功";

		} while (false);
		
		if(bPass)
		{
			WTSOrderInfo* ordInfo = WTSOrderInfo::create();
			ordInfo->setCode(entrust->getCode());
			ordInfo->setExchange(entrust->getExchg());
			ordInfo->setDirection(entrust->getDirection());
			ordInfo->setOffsetType(entrust->getOffsetType());
			ordInfo->setUserTag(entrust->getUserTag());
			ordInfo->setPrice(entrust->getPrice());
			char str[64];
			sprintf(str, "mo.%u.%u", _mocker_id, makeOrderID());
			ordInfo->setOrderID(str);
			ordInfo->setStateMsg(msg.c_str());
			ordInfo->setOrderState(WOS_NotTraded_Queuing);
			ordInfo->setOrderTime(TimeUtils::getLocalTimeNow());
			ordInfo->setVolume(entrust->getVolume());
			ordInfo->setVolLeft(entrust->getVolume());
			ordInfo->setPriceType(entrust->getPriceType());
			ordInfo->setOrderFlag(entrust->getOrderFlag());

			if (_listener != NULL)
			{
				_listener->onRspEntrust(entrust, NULL);
				_listener->onPushOrder(ordInfo);
			}

			_codes.insert(ct->getFullCode());

			if(_listener)
			{
				write_log(_listener,LL_INFO, "共有%u个品种有待撮合订单", _codes.size());
			}

			if (_orders == NULL)
				_orders = WTSArray::create();
			_orders->append(ordInfo, false);

			if (_awaits == NULL)
				_awaits = OrderCache::create();

			_awaits->add(ordInfo->getOrderID(), ordInfo, true);

			save_positions();
		}
		else
		{
			WTSError* err = WTSError::create(WEC_ORDERINSERT, msg.c_str());
			if (_listener != NULL)
			{
				_listener->onRspEntrust(entrust, err);
			}
			err->release();
		}
		entrust->release();
	});

	return 0;
}

int32_t TraderMocker::match_once()
{
	if (_terminated || _orders == NULL || _orders->size() == 0 || _ticks == NULL)
		return 0;
	
	int32_t count = 0;
	for (const std::string& fullcode : _codes)
	{
		if (_terminated)
			return count;

		std::string code, exchg;
		auto pos = fullcode.find(".");
		exchg = fullcode.substr(0, pos);
		code = fullcode.substr(pos + 1);

		WTSContractInfo* ct = _bd_mgr->getContract(code.c_str(), exchg.c_str());
		if (ct == NULL)
			continue;

		WTSCommodityInfo* commInfo = ct->getCommInfo();

		WTSTickData* curTick = (WTSTickData*)_ticks->grab(fullcode);
		if (curTick && strcmp(curTick->code(), ct->getCode())==0)
		{
			StdUniqueLock lock(_mtx_awaits);
			uint64_t tickTime = (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime();
			if (decimal::gt(curTick->price(), 0) /*&& tickTime >= _last_match_time*/)
			{
				//开始处理订单
				//处理记录
				std::vector<std::string> to_erase;

				for (auto it = _awaits->begin(); it != _awaits->end(); it++)
				{
					WTSOrderInfo* ordInfo = (WTSOrderInfo*)it->second;
					if (ordInfo->getVolLeft() == 0 || strcmp(ordInfo->getCode(), curTick->code()) != 0)
						continue;

					bool isBuy = (ordInfo->getDirection() == WDT_LONG && ordInfo->getOffsetType() == WOT_OPEN) || (ordInfo->getDirection() != WDT_LONG && ordInfo->getOffsetType() != WOT_OPEN);

					double uPrice, uVolume;
					if (isBuy)
					{
						uPrice = curTick->askprice(0);
						uVolume = curTick->askqty(0);
					}
					else
					{
						uPrice = curTick->bidprice(0);
						uVolume = curTick->bidqty(0);
					}

					if (decimal::eq(uVolume, 0))
						continue;					

					if (_use_newpx)
					{
						uPrice = curTick->price();
					}

					if (decimal::eq(uPrice, 0))
						continue;

					double target = ordInfo->getPrice();
					//买入的时候,委托价格小于最新价则不成交,卖出的时候,委托价大于最新价则不成交
					if (ordInfo->getPriceType() == WPT_LIMITPRICE && ((isBuy && decimal::lt(target, uPrice)) || (!isBuy && decimal::gt(target, uPrice))))
						continue;

					count++;

					double maxVolume = min(uVolume, ordInfo->getVolLeft());
					std::vector<uint32_t> ayVol = splitVolume((uint32_t)maxVolume, (uint32_t)_min_qty, (uint32_t)_max_qty);
					for (uint32_t curVol : ayVol)
					{

						WTSTradeInfo* trade = WTSTradeInfo::create(curTick->code(), curTick->exchg());
						trade->setDirection(ordInfo->getDirection());
						trade->setOffsetType(ordInfo->getOffsetType());

						trade->setPrice(uPrice);
						trade->setVolume(curVol);

						trade->setRefOrder(ordInfo->getOrderID());

						char str[64];
						sprintf(str, "mt.%u.%u", _mocker_id, makeTradeID());
						trade->setTradeID(str);

						trade->setTradeTime(TimeUtils::getLocalTimeNow());
						trade->setUserTag(ordInfo->getUserTag());

						//更新订单数据
						ordInfo->setVolLeft(ordInfo->getVolLeft() - curVol);
						ordInfo->setVolTraded(ordInfo->getVolTraded() - curVol);
						if (decimal::eq(ordInfo->getVolLeft(), 0))
						{
							ordInfo->setOrderState(WOS_AllTraded);
							ordInfo->setStateMsg("全部成交");
							to_erase.emplace_back(ordInfo->getOrderID());
						}
						else
						{
							ordInfo->setOrderState(WOS_PartTraded_Queuing);
							ordInfo->setStateMsg("部分成交");
						}

						PosItem& pItem = _positions[ct->getFullCode()];
						//第一次的话要给代码和交易所赋值
						if(strlen(pItem._code) == 0)
						{
							strcpy(pItem._code, ct->getCode());
							strcpy(pItem._exchg, ct->getExchg());
						}

						if(commInfo->getCoverMode() == CM_None)
						{

						}
						else
						{
							if (ordInfo->getDirection() == WDT_LONG)
							{
								if (ordInfo->getOffsetType() == WOT_OPEN)
								{
									pItem._long._volume += curVol;
								}
								else
								{
									pItem._long._volume -= curVol;
									pItem._long._frozen -= curVol;
								}
							}
							else
							{
								if (ordInfo->getOffsetType() == WOT_OPEN)
								{
									pItem._short._volume += curVol;
								}
								else
								{
									pItem._short._volume -= curVol;
									pItem._short._frozen -= curVol;
								}
							}
						}
						

						if (_listener)
						{
							StdUniqueLock lock(_mutex_api);
							_listener->onPushOrder(ordInfo);
							_listener->onPushTrade(trade);
						}

						if (_trades == NULL)
							_trades = WTSArray::create();

						_trades->append(trade, false);
					}
				}

				if (count > 0)
				{
					//write_log(_listener,LL_INFO, "[TraderMocker]触发 %s.%s 开多 %u 条,价格:%u", tick->exchg(), tick->code(), iCount, uPrice);
					for (const std::string& oid : to_erase)
					{
						_awaits->remove(oid);
					}
				}
			}
		}

		if(curTick)
			curTick->release();
	}

	_last_match_time = _max_tick_time;

	_ticks->clear();

	if(count > 0)
		save_positions();

	return count;
}

//////////////////////////////////////////////////////////////////////////

bool TraderMocker::init(WTSVariant *params)
{
	_millisecs = params->getUInt32("span");
	_use_newpx = params->getBoolean("newpx");
	_mocker_id = params->getUInt32("mockerid");
	_max_qty = params->getDouble("maxqty");
	_min_qty = params->getDouble("minqty");

	_udp_port = params->getInt32("udp_port");

	boost::asio::ip::address addr = boost::asio::ip::address::from_string("0.0.0.0");
	_broad_ep = boost::asio::ip::udp::endpoint(addr, _udp_port);

	if (decimal::eq(_max_qty, 0))
		_max_qty = 100;

	if (decimal::eq(_min_qty, 0))
		_min_qty = 1;

	//加载持仓数据
	std::stringstream ss;
	ss << "./mocker_" << _mocker_id << "/";
	std::string path = ss.str();
	boost::filesystem::create_directories(path.c_str());

	_pos_file = path;
	_pos_file += "positions.json";

	return true;
}

void TraderMocker::load_positions()
{
	if (!boost::filesystem::exists(_pos_file.c_str()))
		return;

	std::string json;
	StdFile::read_file_content(_pos_file.c_str(), json);

	rj::Document root;
	root.Parse(json.c_str());
	if (root.HasParseError())
		return;

	if(root.HasMember("positions"))
	{//读取仓位
		double total_profit = 0;
		double total_dynprofit = 0;
		const rj::Value& jPos = root["positions"];
		if (!jPos.IsNull() && jPos.IsArray())
		{
			for (const rj::Value& pItem : jPos.GetArray())
			{
				const char* exchg = pItem["exchg"].GetString();
				const char* code = pItem["code"].GetString();
				WTSContractInfo* ct = _bd_mgr->getContract(code, exchg);
				if (ct == NULL)
					continue;

				PosItem& pInfo = _positions[ct->getFullCode()];
				strcpy(pInfo._code, ct->getCode());
				strcpy(pInfo._exchg, ct->getExchg());
				
				pInfo._long._volume = pItem["long"]["volume"].GetDouble();

				pInfo._short._volume = pItem["short"]["volume"].GetDouble();
			}
		}
	}

	if (_listener)
		write_log(_listener,LL_INFO, "[TraderMocker]共加载%u条持仓数据", _positions.size());
}

void TraderMocker::save_positions()
{
	rj::Document root(rj::kObjectType);

	{//持仓数据保存
		rj::Value jPos(rj::kArrayType);

		rj::Document::AllocatorType &allocator = root.GetAllocator();

		for (auto& v : _positions)
		{
			const char* fullcode = v.first.c_str();
			const PosItem& pInfo = v.second;

			rj::Value pItem(rj::kObjectType);
			pItem.AddMember("exchg", rj::Value(pInfo._exchg, allocator), allocator);
			pItem.AddMember("code", rj::Value(pInfo._code, allocator), allocator);

			{
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("volume", pInfo._long._volume, allocator);

				pItem.AddMember("long", dItem, allocator);
			}

			{
				rj::Value dItem(rj::kObjectType);
				dItem.AddMember("volume", pInfo._short._volume, allocator);

				pItem.AddMember("short", dItem, allocator);
			}


			jPos.PushBack(pItem, allocator);
		}

		root.AddMember("positions", jPos, allocator);
	}

	{
        rj::StringBuffer sb;
        rj::PrettyWriter<rj::StringBuffer> writer(sb);
        root.Accept(writer);
        StdFile::write_file_content(_pos_file.c_str(), sb.GetString());

	}
}

void TraderMocker::release()
{
	if (_terminated)
		return;

	_terminated = true;

	if (_thrd_match)
	{
		_thrd_match->join();
	}

	if (_thrd_worker)
	{
		_io_service.stop();
		_thrd_worker->join();
	}
}

void TraderMocker::registerSpi(ITraderSpi *listener)
{
	_listener = listener;

	_bd_mgr = listener->getBaseDataMgr();
}

void TraderMocker::reconn_udp()
{
	if (_b_socket != NULL)
	{
		_b_socket->close();
		delete _b_socket;
		_b_socket = NULL;
	}

	_b_socket = new boost::asio::ip::udp::socket(_io_service);

	_b_socket->open(_broad_ep.protocol());
	_b_socket->set_option(boost::asio::ip::udp::socket::reuse_address(true));
	_b_socket->set_option(boost::asio::ip::udp::socket::broadcast(true));
	_b_socket->bind(_broad_ep);


	_b_socket->async_receive_from(boost::asio::buffer(_b_buffer), _broad_ep,
		boost::bind(&TraderMocker::handle_read, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred, true));
}

void TraderMocker::connect()
{
	reconn_udp();

	_thrd_worker.reset(new StdThread(boost::bind(&boost::asio::io_service::run, &_io_service)));

	_io_service.post([this](){
		StdUniqueLock lock(_mutex_api);

		load_positions();

		if (_listener)
			_listener->handleEvent(WTE_Connect, 0);
	});
}

void TraderMocker::disconnect()
{
	if (_terminated)
		return;

	_terminated = true;

	if (_thrd_match)
	{
		_thrd_match->join();
	}
}

bool TraderMocker::isConnected()
{
	return _thrd_match != NULL;
}

int TraderMocker::login(const char* user, const char* pass, const char* productInfo)
{
	_thrd_match.reset(new StdThread([this]() {
		while (!_terminated)
		{
			match_once();

			//等待5毫秒
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}));

	_io_service.post([this](){
		StdUniqueLock lock(_mutex_api);

		if (_listener)
			_listener->onLoginResult(true, "", TimeUtils::getCurDate());
	});

	return 0;
}

int TraderMocker::logout()
{
	return 0;
}

int TraderMocker::orderAction(WTSEntrustAction* action)
{
	action->retain();
	
	_io_service.post([this, action](){
		StdUniqueLock lck(_mtx_awaits);	//一定要把awaits锁起来,不然可能会导致一边撮合一边撤单
		WTSOrderInfo* ordInfo = (WTSOrderInfo*)_awaits->grab(action->getOrderID());

		/*
		 *	撤单也要考虑几个问题
		 *	1、是否处于可以撤销的状态
		 *	2、如果是开仓,则直接撤销
		 *	3、如果是平仓,要释放冻结
		 */
		if(ordInfo == NULL)
		{
			write_log(_listener,LL_ERROR, "订单%s不存在或者已完成", action->getOrderID());
			WTSError* err = WTSError::create(WEC_ORDERCANCEL, "订单不存在或者处于不可撤销状态");
			if (_listener)
				_listener->onTraderError(err);
			err->release();
			return;
		}

		WTSContractInfo* ct = ordInfo->getContractInfo();
		WTSCommodityInfo* commInfo = ct->getCommInfo();

		bool bPass = false;
		do 
		{
			//开仓委托直接撤单
			if (ordInfo->getOffsetType() == WOT_OPEN)
			{
				bPass = true;
				break;
			}
			
			//不区分开平的,也直接撤销
			if (commInfo->getCoverMode() == CM_None)
			{
				bPass = true;
				break;
			}

			//释放冻结持仓
			PosItem& pItem = _positions[ct->getFullCode()];
			bool isLong = ordInfo->getDirection() == WDT_LONG;
			if(isLong)
			{
				pItem._long._frozen -= ordInfo->getVolLeft();
			}
			else
			{
				pItem._short._frozen -= ordInfo->getVolLeft();
			}
			bPass = true;

		} while (false);

		ordInfo->setStateMsg("撤单成功");
		ordInfo->setOrderState(WOS_Canceled);
		//ordInfo->setVolLeft(0);	

		if (_listener)
		{
			StdUniqueLock lock(_mutex_api);
			_listener->onPushOrder(ordInfo);
		}

		ordInfo->release();
		action->release();

		_awaits->remove(action->getOrderID());

		save_positions();
	});

	return 0;
}

int TraderMocker::queryAccount()
{
	_io_service.post([this](){
		WTSArray* ay = WTSArray::create();
		WTSAccountInfo* accountInfo = WTSAccountInfo::create();
		accountInfo->setCurrency("CNY");
		accountInfo->setBalance(0);
		accountInfo->setPreBalance(0);
		accountInfo->setCloseProfit(0);
		accountInfo->setMargin(0);
		accountInfo->setAvailable(0);
		accountInfo->setCommission(0);
		accountInfo->setFrozenMargin(0);
		accountInfo->setFrozenCommission(0);
		accountInfo->setDeposit(0);
		accountInfo->setWithdraw(0);
		accountInfo->setDescription("");
		accountInfo->setDynProfit(0);

		ay->append(accountInfo, false);

		if (_listener)
		{
			StdUniqueLock lock(_mutex_api);
			_listener->onRspAccount(ay);
		}

		ay->release();

	});
	return 0;
}

int TraderMocker::queryPositions()
{
	_io_service.post([this](){
		WTSArray* ayPos = WTSArray::create();

		for(auto& v : _positions)
		{
			const PosItem& pItem = v.second;

			WTSContractInfo* ct = _bd_mgr->getContract(pItem._code, pItem._exchg);
			if(ct == NULL)
				continue;

			WTSCommodityInfo* commInfo = ct->getCommInfo();

			if(pItem._long._volume > 0)
			{
				WTSPositionItem* pInfo = WTSPositionItem::create(pItem._code, commInfo->getCurrency(), pItem._exchg);
				pInfo->setDirection(WDT_LONG);
				pInfo->setNewPosition(pItem._long._volume);
				pInfo->setAvailNewPos(pItem._long._volume - pItem._long._frozen);

				ayPos->append(pInfo, false);
			}

			if (pItem._short._volume > 0)
			{
				WTSPositionItem* pInfo = WTSPositionItem::create(pItem._code, commInfo->getCurrency(), pItem._exchg);
				pInfo->setDirection(WDT_SHORT);
				pInfo->setNewPosition(pItem._short._volume);
				pInfo->setAvailNewPos(pItem._short._volume - pItem._short._frozen);

				ayPos->append(pInfo, false);
			}
		}

		if (_listener)
		{
			StdUniqueLock lock(_mutex_api);
			_listener->onRspPosition(ayPos);
		}
		ayPos->release();
	});

	return 0;
}

int TraderMocker::queryOrders()
{
	_io_service.post([this](){
		StdUniqueLock lock(_mutex_api);

		if (_listener)
			_listener->onRspOrders(_orders);
	});

	return 0;
}

int TraderMocker::queryTrades()
{
	_io_service.post([this](){
		StdUniqueLock lock(_mutex_api);

		if (_listener)
			_listener->onRspTrades(_trades);
	});

	return 0;
}

void TraderMocker::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred, bool isBroad /* = true */)
{
	if (e)
	{
		if (_listener)
			write_log(_listener,LL_ERROR, "[TraderMocker]UDP行情接收出错:%s(%d)", e.message().c_str(), e.value());

		if (!_terminated)
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			reconn_udp();
			return;
		}
	}

	if (_terminated || bytes_transferred <= 0)
		return;

	extract_buffer(bytes_transferred, isBroad);

	if (isBroad && _b_socket)
	{
		_b_socket->async_receive_from(boost::asio::buffer(_b_buffer), _broad_ep,
			boost::bind(&TraderMocker::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, true));
	}
}

#define UDP_MSG_PUSHTICK	0x200
#pragma pack(push,1)

typedef struct UDPPacketHead
{
	uint32_t		_type;
} UDPPacketHead;
//UDP请求包
typedef struct _UDPReqPacket : UDPPacketHead
{
	char			_data[1020];
} UDPReqPacket;

//UDPTick数据包
template <typename T>
struct UDPDataPacket : UDPPacketHead
{
	T			_data;
};
#pragma pack(pop)
typedef UDPDataPacket<WTSTickStruct>	UDPTickPacket;
void TraderMocker::extract_buffer(uint32_t length, bool isBroad /* = true */)
{
	UDPPacketHead* header = (UDPTickPacket*)_b_buffer.data();

	if (header->_type == UDP_MSG_PUSHTICK)
	{
		UDPTickPacket* packet = (UDPTickPacket*)header;
		std::string fullcode = StrUtil::printf("%s.%s", packet->_data.exchg, packet->_data.code);
		auto it = _codes.find(fullcode);
		if (it == _codes.end())
			return;

		WTSTickData* curTick = WTSTickData::create(packet->_data);
		
		if (_ticks == NULL)
			_ticks = TickCache::create();

		_ticks->add(fullcode, curTick, false);

		_max_tick_time = max(_max_tick_time, (uint64_t)curTick->actiondate() * 1000000000 + curTick->actiontime());
	}
}