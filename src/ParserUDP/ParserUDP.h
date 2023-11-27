/*!
 * \file ParserUDP.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "../Includes/IParserApi.h"
#include "../Share/StdUtils.hpp"

#include <queue>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>

USING_NS_WTP;
using namespace boost::asio;

class ParserUDP : public IParserApi
{
public:
	ParserUDP();
	~ParserUDP();

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
	void	handle_read(const boost::system::error_code& e, std::size_t bytes_transferred, bool isBroad);
	void	handle_write(const boost::system::error_code& e);

	bool	reconnect(uint32_t flag = 3);

	void	subscribe();

	void	extract_buffer(uint32_t length, bool isBroad);

private:
	void	doOnConnected();
	void	doOnDisconnected();

	void	do_send();

private:
	std::string	_hots;
	int			_bport;
	int			_sport;
	uint32_t	_gpsize;

	ip::udp::endpoint	_broad_ep;
	ip::udp::endpoint	_server_ep;
	io_service			_io_service;

	io_service::strand	_strand;

	ip::udp::socket*	_b_socket;
	ip::udp::socket*	_s_socket;
	bool				_s_inited;

	boost::array<char, 1024> _b_buffer;
	boost::array<char, 1024> _s_buffer;

	IParserSpi*				_sink;
	bool					_stopped;
	bool					_connecting;

	CodeSet					_set_subs;

	StdThreadPtr			_thrd_parser;

	StdUniqueMutex			_mtx_queue;
	std::queue<std::string>	_send_queue;
};

