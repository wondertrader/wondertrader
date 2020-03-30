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
#include "../Share/WTSParams.hpp"
#include "../Share/WTSDataDef.hpp"


#define UDP_MSG_SUBSCRIBE	0x100
#define UDP_MSG_PUSHTICK	0x200

#pragma pack(push,1)
//UDP请求包
typedef struct _UDPReqPacket
{
	uint32_t		_type;
	char			_data[1020];
} UDPReqPacket;

//UDPTick数据包
typedef struct _UDPTickPacket
{
	uint32_t		_type;
	WTSTickStruct	_tick;
} UDPTickPacket;
#pragma pack(pop)


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

bool ParserUDP::init( WTSParams* config )
{
	_hots = config->getCString("host");
	_bport = config->getInt32("bport");
	_sport = config->getInt32("sport");

	ip::address addr = ip::address::from_string(_hots);
	_server_ep = ip::udp::endpoint(addr, _sport);

	addr = ip::address::from_string("0.0.0.0");
	_broad_ep = ip::udp::endpoint(addr, _bport);

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
	CodeSet::iterator it = _set_subs.begin();
	for (; it != _set_subs.end(); it++)
	{
		if (length > 0)
		{
			req->_data[length] = ',';
			length++;
		}

		strcpy(req->_data + length, (*it).c_str());
		length += (*it).size();

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
		if (_sink)
			_sink->handleParserLog(LL_ERROR, "[ParserUDP]发送出错:%s(%d)", e.message().c_str(), e.value());
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
		_thrd_parser.reset(new BoostThread(boost::bind(&io_service::run, &_io_service)));
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
		const std::string &code = *cit;
		if(_set_subs.find(code) == _set_subs.end())
		{
			_set_subs.insert(code);
		}
	}
}

void ParserUDP::unsubscribe(const CodeSet &setSymbols)
{

}

void ParserUDP::registerListener( IParserApiListener* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		_sink->handleParserLog(LL_WARN, "Listener is replaced");
	}
}


void ParserUDP::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred, bool isBroad /* = true */)
{
	if(e)
	{
		if(_sink)
			_sink->handleEvent(WPE_Close, 0);

		if(_sink)
			_sink->handleParserLog(LL_ERROR, "[ParserUDP]接收出错:%s(%d)", e.message().c_str(), e.value());

		if (!_stopped && !_connecting)
		{		
			boost::this_thread::sleep(boost::posix_time::seconds(2));
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
	if (length != sizeof(UDPTickPacket))
		return;

	UDPTickPacket* packet = NULL;
	if(isBroad)
		packet = (UDPTickPacket*)_b_buffer.data();
	else
		packet = (UDPTickPacket*)_s_buffer.data();
	WTSTickStruct& ts = packet->_tick;
	bool isPush = false;
	if (packet->_type == UDP_MSG_PUSHTICK)
		isPush = true;
		
	WTSTickData* curTick = WTSTickData::create(packet->_tick);
	if (_sink)
		_sink->handleQuote(curTick, false);

	curTick->release();
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