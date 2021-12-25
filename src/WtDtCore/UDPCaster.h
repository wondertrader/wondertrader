/*!
 * \file UDPCaster.h
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
#include "../Share/StdUtils.hpp"

#include <boost/asio.hpp>
#include <queue>

NS_WTP_BEGIN
	class WTSTickData;
	class WTSQueue;
	class WTSVariant;
	class WTSOrdDtlData;
	class WTSOrdQueData;
	class WTSTransData;
NS_WTP_END

USING_NS_WTP;
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
	typedef std::shared_ptr<UDPReceiver>	UDPReceiverPtr;
	typedef std::vector<UDPReceiverPtr>		ReceiverList;

private:
	void handle_send_broad(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 
	void handle_send_multi(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred); 

	void do_receive();
	void do_send();

	void broadcast(WTSObject* data, uint32_t dataType);

public:
	bool	init(WTSVariant* cfg, WTSBaseDataMgr* bdMgr, DataManager* dtMgr);
	void	start(int bport);
	void	stop();

	bool	addBRecver(const char* remote, int port, int type = 0);
	bool	addMRecver(const char* remote, int port, int sendport, int type = 0);

	void	broadcast(WTSTickData* curTick);
	void	broadcast(WTSOrdQueData* curOrdQue);
	void	broadcast(WTSOrdDtlData* curOrdDtl);
	void	broadcast(WTSTransData* curTrans);

private:
	typedef boost::asio::ip::udp::socket	UDPSocket;
	typedef std::shared_ptr<UDPSocket>		UDPSocketPtr;

	enum 
	{ 
		max_length = 2048 
	};

	boost::asio::ip::udp::endpoint	m_senderEP;
	char			m_data[max_length];

	//广播
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
	StdThreadPtr	m_thrdIO;

	StdThreadPtr	m_thrdCast;
	StdCondVariable	m_condCast;
	StdUniqueMutex	m_mtxCast;
	bool			m_bTerminated;

	WTSBaseDataMgr*	m_bdMgr;
	DataManager*	m_dtMgr;

	typedef struct _CastData
	{
		uint32_t	_datatype;
		WTSObject*	_data;

		_CastData(WTSObject* obj = NULL, uint32_t dataType = 0)
			: _data(obj), _datatype(dataType)
		{
			if (_data)
				_data->retain();
		}

		_CastData(const _CastData& data)
			: _data(data._data), _datatype(data._datatype)
		{
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