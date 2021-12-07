/*!
 * \file UDPCaster.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "UDPCaster.h"
#include "DataManager.h"

#include "../Share/StrUtil.hpp"
#include "../Includes/WTSDataDef.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSVariant.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"


#define UDP_MSG_SUBSCRIBE	0x100
#define UDP_MSG_PUSHTICK	0x200
#define UDP_MSG_PUSHORDQUE	0x201	//委托队列
#define UDP_MSG_PUSHORDDTL	0x202	//委托明细
#define UDP_MSG_PUSHTRANS	0x203	//逐笔成交

#pragma pack(push,1)
//UDP请求包
typedef struct _UDPReqPacket
{
	uint32_t		_type;
	char			_data[1020];
} UDPReqPacket;

//UDPTick数据包
template <typename T>
struct UDPDataPacket
{
	uint32_t	_type;
	T			_data;
};
#pragma pack(pop)
typedef UDPDataPacket<WTSTickStruct>	UDPTickPacket;
typedef UDPDataPacket<WTSOrdQueStruct>	UDPOrdQuePacket;
typedef UDPDataPacket<WTSOrdDtlStruct>	UDPOrdDtlPacket;
typedef UDPDataPacket<WTSTransStruct>	UDPTransPacket;

UDPCaster::UDPCaster()
	: m_bTerminated(false)
	, m_bdMgr(NULL)
	, m_dtMgr(NULL)
{
	
}


UDPCaster::~UDPCaster()
{
}

bool UDPCaster::init(WTSVariant* cfg, WTSBaseDataMgr* bdMgr, DataManager* dtMgr)
{
	m_bdMgr = bdMgr;
	m_dtMgr = dtMgr;

	if (!cfg->getBoolean("active"))
		return false;

	WTSVariant* cfgBC = cfg->get("broadcast");
	if (cfgBC)
	{
		for (uint32_t idx = 0; idx < cfgBC->size(); idx++)
		{
			WTSVariant* cfgItem = cfgBC->get(idx);
			addBRecver(cfgItem->getCString("host"), cfgItem->getInt32("port"), cfgItem->getUInt32("type"));
		}
	}

	WTSVariant* cfgMC = cfg->get("multicast");
	if (cfgMC)
	{
		for (uint32_t idx = 0; idx < cfgMC->size(); idx++)
		{
			WTSVariant* cfgItem = cfgMC->get(idx);
			addMRecver(cfgItem->getCString("host"), cfgItem->getInt32("port"), cfgItem->getInt32("sendport"), cfgItem->getUInt32("type"));
		}
	}

	start(cfg->getInt32("bport"));

	return true;
}

void UDPCaster::start(int bport)
{
	if (!m_listFlatRecver.empty() || !m_listJsonRecver.empty() || !m_listRawRecver.empty())
	{
		m_sktBroadcast.reset(new UDPSocket(m_ioservice, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)));
		boost::asio::socket_base::broadcast option(true);
		m_sktBroadcast->set_option(option);
	}

	m_sktSubscribe.reset(new UDPSocket(m_ioservice, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), bport)));

	do_receive();

	m_thrdIO.reset(new StdThread([this](){
		try
		{
			m_ioservice.run();
		}
		catch(...)
		{
			m_ioservice.stop();
		}
	}));
}

void UDPCaster::stop()
{
	m_bTerminated = true;
	m_ioservice.stop();
	if (m_thrdIO)
		m_thrdIO->join();

	m_condCast.notify_all();
	if (m_thrdCast)
		m_thrdCast->join();
}

void UDPCaster::do_receive()
{
	m_sktSubscribe->async_receive_from(boost::asio::buffer(m_data, max_length), m_senderEP,
		[this](boost::system::error_code ec, std::size_t bytes_recvd)
	{
		if(ec)
		{
			do_receive();
			return;
		}

		if (bytes_recvd == sizeof(UDPReqPacket))
		{
			UDPReqPacket* req = (UDPReqPacket*)m_data;

			std::string data;
			//处理请求
			if (req->_type == UDP_MSG_SUBSCRIBE)
			{
				const StringVector& ay = StrUtil::split(req->_data, ",");
				std::string code, exchg;
				for(const std::string& fullcode : ay)
				{
					auto pos = fullcode.find(".");
					if (pos == std::string::npos)
						code = fullcode;
					else
					{
						code = fullcode.substr(pos + 1);
						exchg = fullcode.substr(0, pos);
					}
					WTSContractInfo* ct = m_bdMgr->getContract(code.c_str(), exchg.c_str());
					if (ct == NULL)
						continue;

					WTSTickData* curTick = m_dtMgr->getCurTick(code.c_str(), exchg.c_str());
					if(curTick == NULL)
						continue;

					std::string* data = new std::string();
					data->resize(sizeof(UDPTickPacket), 0);
					UDPTickPacket* pkt = (UDPTickPacket*)data->data();
					pkt->_type = req->_type;
					memcpy(&pkt->_data, &curTick->getTickStruct(), sizeof(WTSTickStruct));
					curTick->release();
					m_sktSubscribe->async_send_to(
						boost::asio::buffer(*data, data->size()), m_senderEP,
						[this, data](const boost::system::error_code& ec, std::size_t /*bytes_sent*/)
					{
						delete data;
						if (ec)
						{
							WTSLogger::error("Sending data on UDP failed: %s", ec.message().c_str());
						}
					});
				}
			}			
		}
		else
		{
			std::string* data = new std::string("Can not indentify the command");
			m_sktSubscribe->async_send_to(
				boost::asio::buffer(*data, data->size()), m_senderEP,
				[this, data](const boost::system::error_code& ec, std::size_t /*bytes_sent*/)
			{
				delete data;
				if (ec)
				{
					WTSLogger::error("Sending data on UDP failed: %s", ec.message().c_str());
				}
			});
		}

		do_receive();
	});
}

bool UDPCaster::addBRecver(const char* remote, int port, int type /* = 0 */)
{
	try
	{
		boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(remote);
		UDPReceiverPtr item(new UDPReceiver(EndPoint(addr, port), type));
		if(type == 0)
			m_listFlatRecver.emplace_back(item);
		else if(type == 1)
			m_listJsonRecver.emplace_back(item);
		else if(type == 2)
			m_listRawRecver.emplace_back(item);
	}
	catch(...)
	{
		return false;
	}

	return true;
}


bool UDPCaster::addMRecver(const char* remote, int port, int sendport, int type /* = 0 */)
{
	try
	{
		boost::asio::ip::address_v4 addr = boost::asio::ip::address_v4::from_string(remote);
		UDPReceiverPtr item(new UDPReceiver(EndPoint(addr, port), type));
		UDPSocketPtr sock(new UDPSocket(m_ioservice, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), sendport)));
		boost::asio::ip::multicast::join_group option(item->_ep.address());
		sock->set_option(option);
		if(type == 0)
			m_listFlatGroup.emplace_back(std::make_pair(sock, item));
		else if(type == 1)
			m_listJsonGroup.emplace_back(std::make_pair(sock, item));
		else if(type == 2)
			m_listRawGroup.emplace_back(std::make_pair(sock, item));
	}
	catch(...)
	{
		return false;
	}

	return true;
}

void UDPCaster::broadcast(WTSTickData* curTick)
{
	broadcast(curTick, UDP_MSG_PUSHTICK);
}

void UDPCaster::broadcast(WTSOrdDtlData* curOrdDtl)
{
	broadcast(curOrdDtl, UDP_MSG_PUSHORDDTL);
}

void UDPCaster::broadcast(WTSOrdQueData* curOrdQue)
{
	broadcast(curOrdQue, UDP_MSG_PUSHORDQUE);
}

void UDPCaster::broadcast(WTSTransData* curTrans)
{
	broadcast(curTrans, UDP_MSG_PUSHTRANS);
}

void UDPCaster::broadcast(WTSObject* data, uint32_t dataType)
{
	if(m_sktBroadcast == NULL || data == NULL || m_bTerminated)
		return;

	{
		StdUniqueLock lock(m_mtxCast);
		m_dataQue.push(CastData(data, dataType));
	}

	if(m_thrdCast == NULL)
	{
		m_thrdCast.reset(new StdThread([this](){

			while (!m_bTerminated)
			{
				if(m_dataQue.empty())
				{
					StdUniqueLock lock(m_mtxCast);
					m_condCast.wait(lock);
					continue;
				}	

				std::queue<CastData> tmpQue;
				{
					StdUniqueLock lock(m_mtxCast);
					tmpQue.swap(m_dataQue);
				}
				
				while(!tmpQue.empty())
				{
					const CastData& castData = tmpQue.front();

					if (castData._data == NULL)
						break;

					//直接广播
					if (!m_listRawGroup.empty() || !m_listRawRecver.empty())
					{
						std::string buf_raw;
						if (castData._datatype == UDP_MSG_PUSHTICK)
						{
							buf_raw.resize(sizeof(UDPTickPacket));
							UDPTickPacket* pack = (UDPTickPacket*)buf_raw.data();
							pack->_type = castData._datatype;
							WTSTickData* curObj = (WTSTickData*)castData._data;
							memcpy(&pack->_data, &curObj->getTickStruct(), sizeof(WTSTickStruct));
						}
						else if (castData._datatype == UDP_MSG_PUSHORDDTL)
						{
							buf_raw.resize(sizeof(UDPOrdDtlPacket));
							UDPOrdDtlPacket* pack = (UDPOrdDtlPacket*)buf_raw.data();
							pack->_type = castData._datatype;
							WTSOrdDtlData* curObj = (WTSOrdDtlData*)castData._data;
							memcpy(&pack->_data, &curObj->getOrdDtlStruct(), sizeof(WTSOrdDtlStruct));
						}
						else if (castData._datatype == UDP_MSG_PUSHORDQUE)
						{
							buf_raw.resize(sizeof(UDPOrdQuePacket));
							UDPOrdQuePacket* pack = (UDPOrdQuePacket*)buf_raw.data();
							pack->_type = castData._datatype;
							WTSOrdQueData* curObj = (WTSOrdQueData*)castData._data;
							memcpy(&pack->_data, &curObj->getOrdQueStruct(), sizeof(WTSOrdQueStruct));
						}
						else if (castData._datatype == UDP_MSG_PUSHTRANS)
						{
							buf_raw.resize(sizeof(UDPTransPacket));
							UDPTransPacket* pack = (UDPTransPacket*)buf_raw.data();
							pack->_type = castData._datatype;
							WTSTransData* curObj = (WTSTransData*)castData._data;
							memcpy(&pack->_data, &curObj->getTransStruct(), sizeof(WTSTransStruct));
						}
						else
						{
							break;
						}

						//广播
						for (auto it = m_listRawRecver.begin(); it != m_listRawRecver.end(); it++)
						{
							const UDPReceiverPtr& receiver = (*it);
							m_sktBroadcast->send_to(boost::asio::buffer(buf_raw), receiver->_ep);
						}

						//组播
						for (auto it = m_listRawGroup.begin(); it != m_listRawGroup.end(); it++)
						{
							const MulticastPair& item = *it;
							it->first->send_to(boost::asio::buffer(buf_raw), item.second->_ep);
						}
					}

					tmpQue.pop();
				} 
			}
		}));
	}
	else
	{
		m_condCast.notify_all();
	}

	//纯文本格式
	/*
	if(!m_listFlatRecver.empty() || !m_listFlatGroup.empty())
	{
		uint32_t curTime = curTick->actiontime()/1000;
		char buf_flat[2048] = {0};
		char *str = buf_flat;
		//日期,时间,买价,卖价,代码,最新价,开,高,低,今结,昨结,总手,现手,总持,增仓,档位[买x价,买x量,卖x价,卖x量]
		str += sprintf(str, "%04d.%02d.%02d,", 
			curTick->actiondate()/10000, curTick->actiondate()%10000/100, curTick->actiondate()%100);
		str += sprintf(str, "%02d:%02d:%02d,", 
			curTime/10000, curTime%10000/100, curTime%100);
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->bidprice(0)));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->askprice(0)));
		str += sprintf(str, "%s,", curTick->code());

		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->price()));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->open()));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->high()));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->low()));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->settlepx()));
		str += sprintf(str, "%.2f,", PRICE_INT_TO_DOUBLE(curTick->preclose()));

		str += sprintf(str, "%u,", curTick->totalvolume());
		str += sprintf(str, "%u,", curTick->volume());
		str += sprintf(str, "%u,", curTick->openinterest());
		str += sprintf(str, "%d,", curTick->additional());

		for(int i = 0; i < 5; i++)
		{
			str += sprintf(str, "%.2f,%u,", PRICE_INT_TO_DOUBLE(curTick->bidprice(i)), curTick->bidqty(i));
			str += sprintf(str, "%.2f,%u,", PRICE_INT_TO_DOUBLE(curTick->askprice(i)), curTick->askqty(i));
		}

		for(auto it = m_listFlatRecver.begin(); it != m_listFlatRecver.end(); it++)
		{
			const UDPReceiverPtr& receiver = (*it);
			m_sktBroadcast->send_to(boost::asio::buffer(buf_flat, strlen(buf_flat)), receiver->_ep);
			sendTicks++;
			sendBytes += strlen(buf_flat);
		}

		//组播
		for(auto it = m_listFlatGroup.begin(); it != m_listFlatGroup.end(); it++)
		{
			const MulticastPair& item = *it;
			it->first->send_to(boost::asio::buffer(buf_flat, strlen(buf_flat)), item.second->_ep);
			sendTicks++;
			sendBytes += strlen(buf_flat);
		}
	}
	

	//json格式
	if(!m_listJsonRecver.empty() || !m_listJsonGroup.empty())
	{
		datasvr::TickData newTick;
		newTick.set_market(curTick->market());
		newTick.set_code(curTick->code());

		newTick.set_price(curTick->price());
		newTick.set_open(curTick->open());
		newTick.set_high(curTick->high());
		newTick.set_low(curTick->low());
		newTick.set_preclose(curTick->preclose());
		newTick.set_settlepx(curTick->settlepx());

		newTick.set_totalvolume(curTick->totalvolume());
		newTick.set_volume(curTick->volume());
		newTick.set_totalmoney(curTick->totalturnover());
		newTick.set_money(curTick->turnover());
		newTick.set_openinterest(curTick->openinterest());
		newTick.set_additional(curTick->additional());

		newTick.set_tradingdate(curTick->tradingdate());
		newTick.set_actiondate(curTick->actiondate());
		newTick.set_actiontime(curTick->actiontime());

		for(int i = 0; i < 10; i++)
		{
			if(curTick->bidprice(i) == 0 && curTick->askprice(i) == 0)
				break;

			newTick.add_bidprices(curTick->bidprice(i));
			newTick.add_bidqty(curTick->bidqty(i));

			newTick.add_askprices(curTick->askprice(i));
			newTick.add_askqty(curTick->askqty(i));
		}
		const std::string& buf_json =  pb2json(newTick);

		//广播
		for(auto it = m_listJsonRecver.begin(); it != m_listJsonRecver.end(); it++)
		{
			const UDPReceiverPtr& receiver = (*it);
			m_sktBroadcast->send_to(boost::asio::buffer(buf_json), receiver->_ep);
			sendTicks++;
			sendBytes += buf_json.size();
		}

		//组播
		for(auto it = m_listJsonGroup.begin(); it != m_listJsonGroup.end(); it++)
		{
			const MulticastPair& item = *it;
			it->first->send_to(boost::asio::buffer(buf_json), item.second->_ep);
			sendTicks++;
			sendBytes += buf_json.size();
		}
	}
	*/
}

void UDPCaster::handle_send_broad(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(error)
	{
		WTSLogger::error("Broadcasting of market data failed, remote addr: %s, error message: %s", ep.address().to_string().c_str(), error.message().c_str());
	}
}

void UDPCaster::handle_send_multi(const EndPoint& ep, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(error)
	{
		WTSLogger::error("Multicasting of market data failed, remote addr: %s, error message: %s", ep.address().to_string().c_str(), error.message().c_str());
	}
}

