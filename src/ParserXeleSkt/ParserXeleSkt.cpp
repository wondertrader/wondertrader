/*!
 * \file ParserXeleSkt.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserXeleSkt.h"
#include "md_struct.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"

#include <boost/bind.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(IParserSpi* sink, WTSLogLevel ll, const char* format, const Args&... args)
{
	if (sink == NULL)
		return;

	static thread_local char buffer[512] = { 0 };
	memset(buffer, 0, 512);
	fmt::format_to(buffer, format, args...);

	sink->handleParserLog(ll, buffer);
}

inline uint32_t strToTime(const char* strTime)
{
	static char str[10] = { 0 };
	const char *pos = strTime;
	int idx = 0;
	auto len = strlen(strTime);
	for (std::size_t i = 0; i < len; i++)
	{
		if (strTime[i] != ':')
		{
			str[idx] = strTime[i];
			idx++;
		}
	}
	str[idx] = '\0';

	return strtoul(str, NULL, 10);
}

extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserXeleSkt* parser = new ParserXeleSkt();
		return parser;
	}

	EXPORT_FLAG void deleteParser(IParserApi* &parser)
	{
		if (NULL != parser)
		{
			delete parser;
			parser = NULL;
		}
	}
};



ParserXeleSkt::ParserXeleSkt()
	: _udp_socket(NULL)
	, _strand(_io_service)
	, _stopped(false)
	, _sink(NULL)
	, _prepared(false)
	, _tick_cache(NULL)
{
}


ParserXeleSkt::~ParserXeleSkt()
{
}

bool ParserXeleSkt::init( WTSVariant* config )
{
	_tcp_host = config->getCString("tcp_host");
	_tcp_port = config->getInt32("tcp_port");
	_mcast_host = config->getCString("mcast_host");
	_mcast_port = config->getInt32("mcast_port");
	_local_host = config->getCString("local_host");
	_gpsize = config->getUInt32("gpsize");
	if (_gpsize == 0)
		_gpsize = 1000;

	ip::address addr = ip::address::from_string(_tcp_host);
	_tcp_ep = ip::tcp::endpoint(addr, _tcp_port);

	addr = ip::address::from_string("0.0.0.0");
	_mcast_ep = ip::udp::endpoint(addr, _mcast_port);

	return true;
}

void ParserXeleSkt::release()
{
	
}

bool ParserXeleSkt::reconnect()
{
	if(!_prepared)
	{
		prepare();
		_prepared = true;
	}

	{//建立广播通道
		if (_udp_socket != NULL)
		{
			_udp_socket->close();
			delete _udp_socket;
			_udp_socket = NULL;
		}

		_udp_socket = new ip::udp::socket(_io_service);

		_udp_socket->open(_mcast_ep.protocol());
		_udp_socket->set_option(ip::udp::socket::reuse_address(true));
		_udp_socket->bind(_mcast_ep);
		_udp_socket->set_option(ip::multicast::join_group(ip::make_address_v4(_mcast_host.c_str()), ip::make_address_v4(_local_host.c_str())));

		_udp_socket->async_receive_from(buffer(_udp_buffer), _udp_ep,
			boost::bind(&ParserXeleSkt::handle_udp_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	write_log(_sink, LL_INFO, "[ParserXeleSkt] Ready to receive from multicast tunnel {}:{}...", _mcast_host, _mcast_port);
	return true;
}

bool ParserXeleSkt::prepare()
{
	boost::asio::io_context io_context;
	ip::tcp::socket s(io_context);
	boost::system::error_code ec;
	s.connect(_tcp_ep, ec);
	if(ec)
	{
		write_log(_sink, LL_ERROR, "[ParserXeleSkt] Preparing snapshots failed, connecting tcp tunnel {}:{} error: {}", _tcp_host, _tcp_port, ec.message());
		return false;
	}

	if (_tick_cache == NULL)
		_tick_cache = TickCache::create();
	
	write_log(_sink, LL_INFO, "[ParserXeleSkt] Preparing snapshots via tcp tunnel {}:{}...", _tcp_host, _tcp_port);

	boost::array<char, 4096> buffer;
	std::string content;
	auto snap_size = sizeof(CXeleShfeSnapShot) + sizeof(CXeleShfeMarketHead);
	for (;;)
	{
		std::size_t reply_length = boost::asio::read(s, boost::asio::buffer(buffer), ec);
		if (ec)
		{
			//接收错误
			break;
		}

		if (reply_length > 0)
			content.append(buffer.data(), reply_length);
	}

	while (content.size() > snap_size)
	{
		CXeleShfeMarketHead *mh = (CXeleShfeMarketHead *)(content.data());
		int8_t version = mh->Version;
		if (version != XELE_MD_DATA_VERSION)
			break;

		int8_t type = mh->Type;
		if (type != MESSAGE_SNAP_SHOT)
			break;

		CXeleShfeSnapShot *p = (CXeleShfeSnapShot *)(content.data() + sizeof(CXeleShfeMarketHead));
		int instrumentNo = p->InstrumentNo;
		write_log(_sink, LL_DEBUG, "[ParserXeleSkt] {}-{}", p->InstrumentID, instrumentNo);
		WTSContractInfo* ct = _bd_mgr->getContract(p->InstrumentID);
		if (ct != NULL)
		{
			_price_scales[instrumentNo] = p->PriceTick;

			WTSTickData* tick = WTSTickData::create(p->InstrumentID);
			tick->setContractInfo(ct);

			WTSTickStruct& quote = tick->getTickStruct();
			wt_strcpy(quote.exchg, ct->getExchg());

			quote.action_date = strToTime(p->ActionDay);
			quote.action_time = strToTime(p->UpdateTime) * 1000 + p->UpdateMilliSec;

			quote.price = p->LastPrice;
			quote.open = p->OpenPrice;
			quote.high = p->HighestPrice;
			quote.low = p->LowestPrice;
			quote.total_volume = p->Volume;
			quote.trading_date = quote.action_date;
			quote.settle_price = p->SettlementPrice;
			quote.total_turnover = p->Turnover;

			quote.open_interest = p->OpenInterest;

			quote.upper_limit = p->UpperLimitPrice;
			quote.lower_limit = p->LowerLimitPrice;

			quote.pre_close = p->PreClosePrice;
			quote.pre_settle = p->PreSettlementPrice;
			quote.pre_interest = p->PreOpenInterest;

			//委卖价格
			quote.ask_prices[0] = p->AskPrice;
			//委买价格
			quote.bid_prices[0] = p->BidPrice;
			//委卖量
			quote.ask_qty[0] = p->AskVolume;
			//委买量
			quote.bid_qty[0] = p->BidVolume;

			_tick_cache->add(instrumentNo, tick, false);

			if (_sink)
				_sink->handleQuote(tick, 1);
		}

		content.erase(0, snap_size);
	}
	
	s.close(ec);

	write_log(_sink, LL_INFO, "[ParserXeleSkt] All snapshots synced");

	return true;
}


bool ParserXeleSkt::connect()
{
	if(reconnect())
	{
		_io_service.run();
	}
	else
	{
		return false;
	}

	return true;
}

bool ParserXeleSkt::disconnect()
{
	if(_udp_socket != NULL)
	{
		_udp_socket->close();
		delete _udp_socket;
		_udp_socket = NULL;
	}

	_stopped = true;
	_strand.post(boost::bind(&ParserXeleSkt::doOnDisconnected, this));

	return true;
}

bool ParserXeleSkt::isConnected()
{
	return _udp_socket!=NULL;
}


void ParserXeleSkt::subscribe( const CodeSet &vecSymbols )
{
	auto cit = vecSymbols.begin();
	for(; cit != vecSymbols.end(); cit++)
	{
		const auto &code = *cit;
		if(_set_subs.find(code) == _set_subs.end())
		{
			_set_subs.insert(code);
		}
	}
}

void ParserXeleSkt::unsubscribe(const CodeSet &setSymbols)
{

}

void ParserXeleSkt::registerSpi( IParserSpi* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		write_log(_sink, LL_DEBUG, "[ParserXeleSkt] Listener is replaced");
	}

	if (_sink)
		_bd_mgr = _sink->getBaseDataMgr();
}


void ParserXeleSkt::handle_udp_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	if(e)
	{
		if(_sink)
			_sink->handleEvent(WPE_Close, 0);

		write_log(_sink, LL_ERROR, "[ParserXeleSkt] Error occured while receiving: {}({})", e.message().c_str(), e.value());

		if (!_stopped)
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			reconnect();
			return;
		}
	}

	if(_stopped || bytes_transferred<=0)
		return;

	extract_buffer(bytes_transferred);

	_udp_socket->async_receive_from(buffer(_udp_buffer), _udp_ep,
		boost::bind(&ParserXeleSkt::handle_udp_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

#define XELE_MD_EPOCH 1546272000 // unix timestamp of 2019/1/1 0:0:0
#define FOUR_BIT 4
#define FIVE_HUNDRED_MS 500
#define ZERO_MS 0
#define XELE_MD_MS_MASK 0x000f

void ParserXeleSkt::extract_buffer(uint32_t length)
{
	std::size_t len = 0;
	char * data = _udp_buffer.data();
	while (length > len)
	{
		char * data = _udp_buffer.data() + len;
		CXeleShfeMarketHead *mh = reinterpret_cast<CXeleShfeMarketHead *>(data);
		int8_t version = mh->Version;
		if (version != XELE_MD_DATA_VERSION)
			return;

		int8_t type = mh->Type;
		if (type == MESSAGE_MARKET_DATA)
		{
			CXeleShfeMarketData *p = reinterpret_cast<CXeleShfeMarketData *>(data + sizeof(CXeleShfeMarketHead));
			static std::size_t packLen = sizeof(CXeleShfeMarketHead) + sizeof(CXeleShfeMarketData);
			len += packLen;

			//write_log(_sink, LL_DEBUG, "[ParserXeleSkt] Receiving market data, length: {}", packLen);

			WTSTickData* tick = (WTSTickData*)_tick_cache->get(p->InstrumentNo);
			if (tick != NULL)
			{
				uint64_t SnapTime = ((p->SnapDateTime) >> FOUR_BIT) + XELE_MD_EPOCH;
				struct tm *tNow;
				tNow = localtime((time_t*)&SnapTime);
				uint32_t SnapMillisec = (p->SnapDateTime & XELE_MD_MS_MASK) ? FIVE_HUNDRED_MS : ZERO_MS;
				uint32_t actDate = (tNow->tm_year + 1900) * 10000 + (tNow->tm_mon + 1) * 100 + tNow->tm_mday;
				uint32_t actTime = tNow->tm_hour * 10000 + tNow->tm_min * 100 + tNow->tm_sec;
				actTime = actTime * 1000 + SnapMillisec;

				double scale = _price_scales[p->InstrumentNo];
				WTSTickStruct& quote = tick->getTickStruct();
				quote.action_date = actDate;
				quote.action_time = actTime;
				quote.price = p->LastPrice*scale;
				quote.high = std::max(quote.high, p->LastPrice*scale);
				quote.low = std::min(quote.low, p->LastPrice*scale);

				quote.total_volume = p->Volume;
				quote.total_turnover = (double)p->Turnover;
				quote.open_interest = (double)p->OpenInterest;

				quote.volume = 0;
				quote.turn_over = 0;
				quote.diff_interest = 0;

				//委卖价格
				quote.ask_prices[0] = p->AskPrice*scale;
				//委买价格
				quote.bid_prices[0] = p->BidPrice*scale;
				//委卖量
				quote.ask_qty[0] = p->AskVolume;
				//委买量
				quote.bid_qty[0] = p->BidVolume;

				if (_sink)
					_sink->handleQuote(tick, 1);

				static uint32_t recv_cnt = 0;
				recv_cnt++;
				if (recv_cnt % _gpsize == 0)
					write_log(_sink, LL_DEBUG, "[ParserXeleSkt] {} ticks received in total", recv_cnt);
			}
		}
		else if (type == MESSAGE_DEPTH)
		{
			CXeleShfeDepthMarketData *p = (CXeleShfeDepthMarketData *)(data + sizeof(CXeleShfeMarketHead));
			static std::size_t packLen = sizeof(CXeleShfeMarketHead) + sizeof(CXeleShfeDepthMarketData);
			len += packLen;

			//write_log(_sink, LL_DEBUG, "[ParserXeleSkt] Receiving depth market data of {}, length: {}", p->InstrumentNo, packLen);

			WTSTickData* tick = (WTSTickData*)_tick_cache->get(p->InstrumentNo);
			if (tick != NULL)
			{
				uint64_t SnapTime = ((p->SnapDateTime) >> FOUR_BIT) + XELE_MD_EPOCH;
				struct tm *tNow;
				tNow = localtime((time_t*)&SnapTime);
				uint32_t SnapMillisec = (p->SnapDateTime & XELE_MD_MS_MASK) ? FIVE_HUNDRED_MS : ZERO_MS;
				uint32_t actDate = (tNow->tm_year + 1900) * 10000 + (tNow->tm_mon + 1) * 100 + tNow->tm_mday;
				uint32_t actTime = tNow->tm_hour * 10000 + tNow->tm_min * 100 + tNow->tm_sec;
				actTime = actTime * 1000 + SnapMillisec;

				double scale = _price_scales[p->InstrumentNo];
				WTSTickStruct& quote = tick->getTickStruct();
				quote.action_date = actDate;
				quote.action_time = actTime;
				quote.price = p->LastPrice*scale;
				quote.high = std::max(quote.high, p->LastPrice*scale);
				quote.low = std::min(quote.low, p->LastPrice*scale);

				quote.total_volume = p->Volume;
				quote.total_turnover = (double)p->Turnover;
				quote.open_interest = (double)p->OpenInterest;

				quote.volume = 0;
				quote.turn_over = 0;
				quote.diff_interest = 0;

				//委卖价格
				quote.ask_prices[0] = p->AskPrice1*scale;
				quote.ask_prices[1] = p->AskPrice2*scale;
				quote.ask_prices[2] = p->AskPrice3*scale;
				quote.ask_prices[3] = p->AskPrice4*scale;
				quote.ask_prices[4] = p->AskPrice5*scale;

				//委买价格
				quote.bid_prices[0] = p->BidPrice1*scale;
				quote.bid_prices[1] = p->BidPrice2*scale;
				quote.bid_prices[2] = p->BidPrice3*scale;
				quote.bid_prices[3] = p->BidPrice4*scale;
				quote.bid_prices[4] = p->BidPrice5*scale;

				//委卖量
				quote.ask_qty[0] = p->AskVolume1;
				quote.ask_qty[1] = p->AskVolume2;
				quote.ask_qty[2] = p->AskVolume3;
				quote.ask_qty[3] = p->AskVolume4;
				quote.ask_qty[4] = p->AskVolume5;

				//委买量
				quote.bid_qty[0] = p->BidVolume1;
				quote.bid_qty[1] = p->BidVolume2;
				quote.bid_qty[2] = p->BidVolume3;
				quote.bid_qty[3] = p->BidVolume4;
				quote.bid_qty[4] = p->BidVolume5;

				if (_sink)
					_sink->handleQuote(tick, 1);

				static uint32_t recv_cnt = 0;
				recv_cnt++;
				if (recv_cnt % _gpsize == 0)
					write_log(_sink, LL_DEBUG, "[ParserXeleSkt] {} depth ticks received in total", recv_cnt);
			}
		}
		else if (type == MESSAGE_TYPE_HEART_BEAT) {
			CXeleShfeHeartBeat *p = reinterpret_cast<CXeleShfeHeartBeat *>(data + sizeof(CXeleShfeMarketHead));
			static std::size_t packLen = sizeof(CXeleShfeMarketHead) + sizeof(CXeleShfeHeartBeat);
			len += packLen;
			write_log(_sink, LL_DEBUG, "[ParserXeleSkt] Receiving heartbeat packet, length: %d\n", packLen);
		}
	}
}

void ParserXeleSkt::doOnConnected()
{
	if(_sink)
	{
		_sink->handleEvent(WPE_Connect, 0);
		_sink->handleEvent(WPE_Login, 0);
	}
}

void ParserXeleSkt::doOnDisconnected()
{
	if(_sink)
	{
		_sink->handleEvent(WPE_Close, 0);
	}
}