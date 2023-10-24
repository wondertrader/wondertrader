/*!
 * \file ParserShm.h
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
#include "../Includes/WTSStruct.h"
#include "../Share/BoostMappingFile.hpp"

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>

USING_NS_WTP;
using namespace boost::asio;

class ParserShm : public IParserApi
{
public:
	ParserShm();
	~ParserShm();

#pragma pack(push, 8)
	typedef struct _DataItem
	{
		uint32_t	_type;	//数据类型， 0-tick,1-委托队列,2-逐笔委托,3-逐笔成交
		union
		{
			WTSTickStruct	_tick;
			WTSOrdQueStruct _queue;
			WTSOrdDtlStruct	_order;
			WTSTransStruct	_trans;
		};

		_DataItem() { memset(this, 0, sizeof(_DataItem)); }
	} DataItem;

	template <int N = 8 * 1024>
	struct _DataQueue
	{
		uint64_t	_capacity = N;
		volatile uint64_t	_readable;
		volatile uint64_t	_writable;
		uint32_t	_pid;
		DataItem	_items[N];

		_DataQueue() :_readable(UINT64_MAX), _writable(0), _pid(0) {}
	};

	typedef _DataQueue<8 * 1024>	CastQueue;

#pragma pack(pop)

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
	std::string		_path;
	typedef std::shared_ptr<BoostMappingFile> MappedFilePtr;
	MappedFilePtr	_mapfile;
	CastQueue*		_queue;
	uint32_t		_gpsize;
	uint32_t		_check_span;

	IParserSpi*		_sink;
	bool			_stopped;

	CodeSet			_set_subs;

	StdThreadPtr	_thrd_parser;
};

