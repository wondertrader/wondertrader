/*!
 * \file ParserXeleSkt.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/IParserApi.h"
#include "../Includes/WTSCollection.hpp"
#include "../Share/StdUtils.hpp"

#include <queue>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>

USING_NS_WTP;
using namespace boost::asio;

class ParserXeleSkt : public IParserApi
{
public:
	ParserXeleSkt();
	~ParserXeleSkt();

	//IQuoteParser ½Ó¿Ú
public:
	virtual bool init(WTSVariant* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerSpi(IParserSpi* listener) override;


private:
	void	handle_udp_read(const boost::system::error_code& e, std::size_t bytes_transferred);

	bool	prepare();

	bool	reconnect();

	void	extract_buffer(uint32_t length);

private:
	void	doOnConnected();
	void	doOnDisconnected();

private:
	std::string	_tcp_host;
	int			_tcp_port;
	std::string	_mcast_host;
	int			_mcast_port;
	std::string	_local_host;
	uint32_t	_gpsize;

	ip::udp::endpoint	_mcast_ep;
	ip::udp::endpoint	_udp_ep;
	ip::tcp::endpoint	_tcp_ep;
	io_service			_io_service;

	io_service::strand	_strand;

	ip::udp::socket*	_udp_socket;

	boost::array<char, 4096> _udp_buffer;

	IParserSpi*		_sink;
	IBaseDataMgr*	_bd_mgr;
	bool			_stopped;
	bool			_prepared;

	CodeSet			_set_subs;
	StdThreadPtr	_thrd_parser;

	typedef WTSHashMap<int>	TickCache;
	TickCache*		_tick_cache;

	wt_hashmap<int, double> _price_scales;
};

