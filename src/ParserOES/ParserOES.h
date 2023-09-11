/*!
 * \file ParserOES.h
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
#include "../Share/SpinMutex.hpp"
#include "../API/oesApi0.17.5.8/include/mds_api/mds_async_api.h"

#include <queue>

USING_NS_WTP;

class ParserOES : public IParserApi
{
public:
	ParserOES();
	~ParserOES();

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
	void	doSubscribe();

public:
	void	doOnConnected(MdsAsyncApiChannelT *pAsyncChannel);
	void	doOnDisconnected(MdsAsyncApiChannelT *pAsyncChannel);
	void	doOnMessage(SMsgHeadT *pMsgHead, void *pMsgItem);

private:
	uint32_t		_gpsize;

	std::string		_config;


	IParserSpi*		_sink;
	bool			_inited;
	bool			_udp;

	CodeSet			_set_subs;

	MdsAsyncApiContextT*	_context;

	IBaseDataMgr*	_bd_mgr;
};

