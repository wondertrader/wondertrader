/*!
 * \file ParserUDP.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserUDP.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSDataDef.hpp"

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

#define UDP_MSG_SUBSCRIBE	0x100
#define UDP_MSG_PUSHTICK	0x200
#define UDP_MSG_PUSHORDQUE	0x201	//委托队列
#define UDP_MSG_PUSHORDDTL	0x202	//委托明细
#define UDP_MSG_PUSHTRANS	0x203	//逐笔成交

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
typedef UDPDataPacket<WTSOrdQueStruct>	UDPOrdQuePacket;
typedef UDPDataPacket<WTSOrdDtlStruct>	UDPOrdDtlPacket;
typedef UDPDataPacket<WTSTransStruct>	UDPTransPacket;


extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserUDP* parser = new ParserUDP();
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



ParserUDP::ParserUDP()
	: _b_socket(NULL)
	, _s_socket(NULL)
	, _strand(_io_service)
	, _stopped(false)
	, _sink(NULL)
	, _connecting(false)
{
}


ParserUDP::~ParserUDP()
{
}

bool ParserUDP::init( WTSVariant* config )
{
	_hots = config->getCString("host");
	_bport = config->getInt32("bport");
	_sport = config->getInt32("sport");
	_gpsize = config->getUInt32("gpsize");
	if (_gpsize == 0)
		_gpsize = 1000;

	ip::address addr = ip::address::from_string(_hots);
	_server_ep = ip::udp::endpoint(addr, _sport);

	_broad_ep = ip::udp::endpoint(ip::address_v4::any(), _bport);

	return true;
}

void ParserUDP::release()
{
	
}

bool ParserUDP::reconnect()
{
	{//建立广播通道
		if (_b_socket != NULL)
		{
			_b_socket->close();
			delete _b_socket;
			_b_socket = NULL;
		}

		_b_socket = new ip::udp::socket(_io_service);

		_b_socket->open(_broad_ep.protocol());
		_b_socket->set_option(ip::udp::socket::reuse_address(true));
		_b_socket->set_option(ip::udp::socket::broadcast(true));
		_b_socket->set_option(ip::udp::socket::receive_buffer_size(8 * 1024 * 1024));
		_b_socket->bind(_broad_ep);


		_b_socket->async_receive_from(buffer(_b_buffer), _broad_ep,
			boost::bind(&ParserUDP::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, true));
	}

	{
		//建立订阅通道
		if (_s_socket != NULL)
		{
			_s_socket->close();
			delete _s_socket;
			_s_socket = NULL;
		}

		_s_socket = new ip::udp::socket(_io_service, ip::udp::endpoint(ip::udp::v4(), 0));

		_s_socket->async_receive_from(buffer(_s_buffer), _server_ep,
			boost::bind(&ParserUDP::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, false));

		subscribe();
	}
	return true;
}

void ParserUDP::subscribe()
{
	std::string data;
	data.resize(sizeof(UDPReqPacket), 0);
	UDPReqPacket* req = (UDPReqPacket*)data.data();
	req->_type = UDP_MSG_SUBSCRIBE;
	uint32_t length = 0;
	for (auto& code : _set_subs)
	{
		if (length > 0)
		{
			req->_data[length] = ',';
			length++;
		}

		std::size_t pos = code.find('.');
		if (pos != std::string::npos)
			strcpy(req->_data + length, (char*)code.c_str() + pos + 1);
		else
			strcpy(req->_data + length, code.c_str());

		length += code.size();

		if (length > 1000)
		{
			_send_queue.push(data);
			
			data.resize(sizeof(UDPReqPacket), 0);
			req = (UDPReqPacket*)data.data();
			req->_type = UDP_MSG_SUBSCRIBE;
			length = 0;
		}
	}

	do_send();

	write_log(_sink, LL_INFO, "[ParserUDP] Ticks subscribing sent");
}

void ParserUDP::do_send()
{
	if (_send_queue.empty())
		return;

	std::string& data = _send_queue.front();

	_s_socket->async_send_to(boost::asio::buffer(data, data.size()), _server_ep,
		boost::bind(&ParserUDP::handle_write, this, boost::asio::placeholders::error));
}

void ParserUDP::handle_write(const boost::system::error_code& e)
{
	if (e)
	{
		write_log(_sink, LL_ERROR, "[ParserUDP] Error occured while receiving: {}({})", e.message().c_str(), e.value());
	}
	else
	{
		_send_queue.pop();
	}
	
	do_send();
}

bool ParserUDP::connect()
{
	if(reconnect())
	{
		_thrd_parser.reset(new StdThread(boost::bind(&io_service::run, &_io_service)));
	}
	else
	{
		return false;
	}

	return true;
}

bool ParserUDP::disconnect()
{
	if(_b_socket != NULL)
	{
		_b_socket->close();
		delete _b_socket;
		_b_socket = NULL;
	}

	_stopped = true;
	_strand.post(boost::bind(&ParserUDP::doOnDisconnected, this));

	return true;
}

bool ParserUDP::isConnected()
{
	return _b_socket!=NULL;
}


void ParserUDP::subscribe( const CodeSet &vecSymbols )
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

void ParserUDP::unsubscribe(const CodeSet &setSymbols)
{

}

void ParserUDP::registerSpi( IParserSpi* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		write_log(_sink, LL_WARN, "Listener is replaced");
	}
}


void ParserUDP::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred, bool isBroad /* = true */)
{
	if(e)
	{
		if(_sink)
			_sink->handleEvent(WPE_Close, 0);

		write_log(_sink, LL_ERROR, "[ParserUDP] Error occured while receiving: {}({})", e.message().c_str(), e.value());

		if (!_stopped && !_connecting)
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			reconnect();
			return;
		}
	}

	if(_stopped || bytes_transferred<=0)
		return;

	extract_buffer(bytes_transferred, isBroad);

	if (isBroad && _b_socket)
	{
		_b_socket->async_receive_from(buffer(_b_buffer), _broad_ep,
			boost::bind(&ParserUDP::handle_read, this, 
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, true));
	} 
	else if(!isBroad && _s_socket)
	{
		_s_socket->async_receive_from(buffer(_s_buffer), _server_ep,
			boost::bind(&ParserUDP::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred, false));

	}
}

void ParserUDP::extract_buffer(uint32_t length, bool isBroad /* = true */)
{
	UDPPacketHead* header = NULL;
	if(isBroad)
		header = (UDPTickPacket*)_b_buffer.data();
	else
		header = (UDPTickPacket*)_s_buffer.data();

	if (header->_type == UDP_MSG_PUSHTICK || header->_type == UDP_MSG_SUBSCRIBE)
	{
		UDPTickPacket* packet = (UDPTickPacket*)header;
		WTSTickData* curTick = WTSTickData::create(packet->_data);
		if (_sink)
			_sink->handleQuote(curTick, 0);

		curTick->release();

		static uint32_t recv_cnt = 0;
		recv_cnt++;
		if (recv_cnt % _gpsize == 0)
			write_log(_sink, LL_DEBUG, "[ParserUDP] {} ticks received in total", recv_cnt);
	}
	else if (header->_type == UDP_MSG_PUSHORDDTL)
	{
		UDPOrdDtlPacket* packet = (UDPOrdDtlPacket*)header;
		WTSOrdDtlData* curData = WTSOrdDtlData::create(packet->_data);
		if (_sink)
			_sink->handleOrderDetail(curData);

		curData->release();

		static uint32_t recv_cnt = 0;
		recv_cnt++;
		if (recv_cnt % _gpsize == 0)
			write_log(_sink, LL_DEBUG, "[ParserUDP] {} order details received in total", recv_cnt);
	}
	else if (header->_type == UDP_MSG_PUSHORDQUE)
	{
		UDPOrdQuePacket* packet = (UDPOrdQuePacket*)header;
		WTSOrdQueData* curData = WTSOrdQueData::create(packet->_data);
		if (_sink)
			_sink->handleOrderQueue(curData);

		curData->release();

		static uint32_t recv_cnt = 0;
		recv_cnt++;
		if (recv_cnt % _gpsize == 0)
			write_log(_sink, LL_DEBUG, "[ParserUDP] {} order queues received in total", recv_cnt);
	}
	else if (header->_type == UDP_MSG_PUSHTRANS)
	{
		UDPTransPacket* packet = (UDPTransPacket*)header;
		WTSTransData* curData = WTSTransData::create(packet->_data);
		if (_sink)
			_sink->handleTransaction(curData);

		curData->release();

		static uint32_t recv_cnt = 0;
		recv_cnt++;
		if (recv_cnt % _gpsize == 0)
			write_log(_sink, LL_DEBUG, "[ParserUDP] {} transactions received in total", recv_cnt);
	}
}

void ParserUDP::doOnConnected()
{
	if(_sink)
	{
		_sink->handleEvent(WPE_Connect, 0);
		_sink->handleEvent(WPE_Login, 0);
	}
}

void ParserUDP::doOnDisconnected()
{
	if(_sink)
	{
		_sink->handleEvent(WPE_Close, 0);
	}
}