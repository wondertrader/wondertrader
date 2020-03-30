#pragma once

#include "../WTSUtils/pugixml/pugixml.hpp"

#include "../Share/WTSMarcos.h"
#include "../Share/BoostDefine.h"

#include <boost/asio.hpp>

NS_OTP_BEGIN
	class WTSTickData;
	class WTSQueue;
	class WTSVariant;
NS_OTP_END

USING_NS_OTP;
class WTSBaseDataMgr;
class DataManager;

class UDPCaster
{
public:
	UDPCaster();
	~UDPCaster();

	typedef boost::asio::ip::udp::endpoint EndPoint;
	typedef struct tagUDPReceiver
	{
		EndPoint	_ep;
		uint32_t	_type;


		tagUDPReceiver(EndPoint ep, uint32_t t)
		{
			_ep = ep;
			_type = t;
		}

	} UDPReceiver;
	typedef boost::shared_ptr<UDPReceiver>	UDPReceiverPtr;
	typedef std::vector<UDPReceiverPtr>		ReceiverList;

private:
	void handle_send_broad(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 
	void handle_send_multi(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 

	void do_receive();
	void do_send();

public:
	bool	init(WTSVariant* cfg, WTSBaseDataMgr* bdMgr, DataManager* dtMgr);
	void	start(int bport);
	void	stop();

	bool	addBRecver(const char* remote, int port, int type = 0);
	bool	addMRecver(const char* remote, int port, int sendport, int type = 0);

	void	broadcast(WTSTickData* curTick);

private:
	typedef boost::asio::ip::udp::socket	UDPSocket;
	typedef boost::shared_ptr<UDPSocket>	UDPSocketPtr;

	enum 
	{ 
		max_length = 2048 
	};

	boost::asio::ip::udp::endpoint	m_senderEP;
	char			m_data[max_length];

	//¹ã²¥
	ReceiverList	m_listFlatRecver;
	ReceiverList	m_listJsonRecver;
	ReceiverList	m_listRawRecver;
	UDPSocketPtr	m_sktBroadcast;
	UDPSocketPtr	m_sktSubscribe;

	typedef std::pair<UDPSocketPtr,UDPReceiverPtr>	MulticastPair;
	typedef std::vector<MulticastPair>	MulticastList;
	MulticastList	m_listFlatGroup;
	MulticastList	m_listJsonGroup;
	MulticastList	m_listRawGroup;
	boost::asio::io_service		m_ioservice;
	BoostThreadPtr	m_thrdIO;

	WTSQueue*		m_tickQue;
	BoostThreadPtr	m_thrdCast;
	BoostCondition	m_condCast;
	BoostUniqueMutex	m_mtxCast;
	bool			m_bTerminated;

	WTSBaseDataMgr*	m_bdMgr;
	DataManager*	m_dtMgr;
};