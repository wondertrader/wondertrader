/*!
 * \file EventCaster.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief UDP广播对象定义
 */
#pragma once

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSObject.hpp"
#include "../Share/BoostDefine.h"

#include <boost/asio.hpp>
#include <queue>

NS_OTP_BEGIN
class WTSTradeInfo;
class WTSOrderInfo;
class WTSVariant;

class EventCaster
{
public:
	EventCaster();
	~EventCaster();

	typedef boost::asio::ip::udp::endpoint	EndPoint;
	typedef std::vector<EndPoint>			ReceiverList;

private:
	void handle_send_broad(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 
	void handle_send_multi(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 

	void broadcast(const char* trader, uint32_t localid, const char* stdCode, WTSObject* data, uint32_t dataType);

	void tradeToJson(uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo, std::string& output);
	void orderToJson(uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo, std::string& output);

public:
	bool	init(WTSVariant* cfg);
	void	start(int bport);
	void	stop();

	bool	addBRecver(const char* remote, int port);
	bool	addMRecver(const char* remote, int port, int sendport);

	void	broadcast(const char* trader, uint32_t localid, const char* stdCode, WTSTradeInfo* trdInfo);
	void	broadcast(const char* trader, uint32_t localid, const char* stdCode, WTSOrderInfo* ordInfo);

private:
	typedef boost::asio::ip::udp::socket	UDPSocket;
	typedef boost::shared_ptr<UDPSocket>	UDPSocketPtr;

	enum 
	{ 
		max_length = 2048 
	};

	boost::asio::ip::udp::endpoint	m_senderEP;
	char			m_data[max_length];

	//广播
	ReceiverList	m_listRawRecver;
	UDPSocketPtr	m_sktBroadcast;

	typedef std::pair<UDPSocketPtr,EndPoint>	MulticastPair;
	typedef std::vector<MulticastPair>	MulticastList;
	MulticastList	m_listRawGroup;
	boost::asio::io_service		m_ioservice;
	BoostThreadPtr	m_thrdIO;

	BoostThreadPtr	m_thrdCast;
	BoostCondition	m_condCast;
	BoostUniqueMutex	m_mtxCast;
	bool			m_bTerminated;

	typedef struct _CastData
	{
		char		_trader[32];
		char		_code[32];
		uint32_t	_datatype;
		uint32_t	_localid;
		WTSObject*	_data;

		_CastData(const char* trader, uint32_t localid, const char* stdCode, WTSObject* obj = NULL, uint32_t dataType = 0)
			: _data(obj), _datatype(dataType), _localid(localid)
		{
			strcpy(_trader, trader);
			strcpy(_code, stdCode);

			if (_data)
				_data->retain();
		}

		~_CastData()
		{
			if (_data)
			{
				_data->release();
				_data = NULL;
			}
		}
	} CastData;

	std::queue<CastData>		m_dataQue;
};

NS_OTP_END