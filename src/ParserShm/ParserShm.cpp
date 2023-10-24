/*!
 * \file ParserShm.cpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#include "ParserShm.h"
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
	fmtutil::format_to(buffer, format, args...);

	sink->handleParserLog(ll, buffer);
}

#define UDP_MSG_SUBSCRIBE	0x100
#define UDP_MSG_PUSHTICK	0x200
#define UDP_MSG_PUSHORDQUE	0x201	//委托队列
#define UDP_MSG_PUSHORDDTL	0x202	//委托明细
#define UDP_MSG_PUSHTRANS	0x203	//逐笔成交


extern "C"
{
	EXPORT_FLAG IParserApi* createParser()
	{
		ParserShm* parser = new ParserShm();
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



ParserShm::ParserShm()
	: _stopped(false)
	, _sink(NULL)
	, _queue(NULL)
{
}


ParserShm::~ParserShm()
{
}

bool ParserShm::init( WTSVariant* config )
{
	_path = config->getCString("path");
	_gpsize = config->getUInt32("gpsize");
	if (_gpsize == 0)
		_gpsize = 1000;
	_check_span = config->getUInt32("checkspan");
	if (_check_span == 0)
		_check_span = 2;

	return true;
}

void ParserShm::release()
{
	
}

bool ParserShm::connect()
{
	if (!StdFile::exists(_path.c_str()))
		return false;

	_mapfile.reset(new BoostMappingFile);
	_mapfile->map(_path.c_str());
	_queue = (CastQueue*)_mapfile->addr();
	if (_queue->_capacity == 0)
		new(_mapfile->addr()) CastQueue();

	//启动的时候都做一下偏移
	_queue->_writable %= _queue->_capacity;
	if (_queue->_readable != UINT64_MAX)
	{
		_queue->_readable %= _queue->_capacity;
		if (_queue->_readable > _queue->_writable)
			_queue->_writable += _queue->_capacity;
	}

	_thrd_parser.reset(new StdThread([this]() {
		if (_sink)
		{
			_sink->handleEvent(WPE_Connect, 0);
			_sink->handleEvent(WPE_Login, 0);
		}

		uint64_t lastIdx = UINT64_MAX;
		while(!_stopped)
		{
			//说明刚启动，之前的命令全部作废
			if (_queue->_readable == UINT64_MAX)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(_check_span));
				continue;
			}

			if (lastIdx == UINT64_MAX && _queue->_readable != UINT64_MAX)
			{
				lastIdx = _queue->_readable;
				continue;
			}
			else if (lastIdx == 999 && _queue->_readable != UINT64_MAX)
			{
				lastIdx = 0;
			}
			else if (lastIdx >= _queue->_readable)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(_check_span));
				continue;
			}
			else
			{
				lastIdx++;
			}

			DataItem& item = _queue->_items[lastIdx % _queue->_capacity];
			switch (item._type)
			{
			case 0:
			{
				WTSTickData* newData = WTSTickData::create(item._tick);
				if (_sink)
					_sink->handleQuote(newData, 0);
				newData->release();
			}
				break;
			case 1:
			{
				WTSOrdQueData* newData = WTSOrdQueData::create(item._queue);
				if (_sink)
					_sink->handleOrderQueue(newData);
				newData->release();
			}
				break;
			case 2:
			{
				WTSOrdDtlData* newData = WTSOrdDtlData::create(item._order);
				if (_sink)
					_sink->handleOrderDetail(newData);
				newData->release();
			}
				break;
			case 3:
			{
				WTSTransData* newData = WTSTransData::create(item._trans);
				if (_sink)
					_sink->handleTransaction(newData);
				newData->release();
			}
				break;
			default:
				break;
			}
		}
	}));

	return true;
}

bool ParserShm::disconnect()
{
	_stopped = true;

	return true;
}

bool ParserShm::isConnected()
{
	return _queue!=NULL;
}


void ParserShm::subscribe( const CodeSet &vecSymbols )
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

void ParserShm::unsubscribe(const CodeSet &setSymbols)
{

}

void ParserShm::registerSpi( IParserSpi* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		write_log(_sink, LL_WARN, "Listener is replaced");
	}
}