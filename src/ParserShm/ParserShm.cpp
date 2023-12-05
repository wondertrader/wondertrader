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

#define NODATA_FLAG 0xfffffffffffffffe


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
	, _check_span(0)
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

	return true;
}

void ParserShm::release()
{
	
}

bool ParserShm::connect()
{
	_thrd_parser.reset(new StdThread([this]() {

		write_log(_sink, LL_INFO, "[ParserShm] loading {} ...", _path);
		while (!StdFile::exists(_path.c_str()))
		{
			write_log(_sink, LL_WARN, "[ParserShm] {} not exist yet, waiting for 2 seconds", _path);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			continue;
		}

		_mapfile.reset(new BoostMappingFile);
		_mapfile->map(_path.c_str());
		_queue = (CastQueue*)_mapfile->addr();
		uint32_t cast_pid = _queue->_pid;

		if (_sink)
		{
			_sink->handleEvent(WPE_Connect, 0);
			_sink->handleEvent(WPE_Login, 0);
		}
		write_log(_sink, LL_INFO, "[ParserShm] {} loaded, start to receiving", _path);

		uint64_t lastIdx = UINT64_MAX;
		while(!_stopped)
		{
			//如果pid不同，说明datakit重启了
			if(cast_pid != _queue->_pid)
			{
				lastIdx = UINT64_MAX;
				write_log(_sink, LL_WARN, "ShareMemory queue has been reset justnow");
				cast_pid = _queue->_pid;
			}
			
			if (_queue->_readable == UINT64_MAX)	//刚分配好，还没数据进来
			{
				lastIdx = NODATA_FLAG;
				if(_check_span != 0)
					std::this_thread::sleep_for(std::chrono::microseconds(_check_span));
				continue;
			}

			if (lastIdx == UINT64_MAX)	//有数据，第一次检查，则直接定位到最后一条数据
			{
				lastIdx = _queue->_readable;
				if (_check_span != 0)
					std::this_thread::sleep_for(std::chrono::microseconds(_check_span));
				continue;
			}
			else if (lastIdx == NODATA_FLAG)	//之前没数据的时候检查了一次，现在有数据了，从0开始读取
			{
				lastIdx = 0;
			}
			else if (lastIdx >= _queue->_readable)	//没有新的数据进来
			{
				if (_check_span != 0)
					std::this_thread::sleep_for(std::chrono::microseconds(_check_span));
				continue;
			}
			else
			{
				lastIdx++;	//普通情况，下标递增
			}

			DataItem& item = _queue->_items[lastIdx % _queue->_capacity];
			switch (item._type)
			{
			case 0:
			{
				const char* fullCode = fmtutil::format("{}.{}", item._tick.exchg, item._tick.code);
				auto it = _set_subs.find(fullCode);
				if (it != _set_subs.end())
				{
					WTSTickData* newData = WTSTickData::create(item._tick);
					if (_sink)
						_sink->handleQuote(newData, 0);
					newData->release();

					static uint32_t recv_cnt = 0;
					recv_cnt++;
					if (recv_cnt % _gpsize == 0)
						write_log(_sink, LL_DEBUG, "[ParserShm] {} ticks received in total", recv_cnt);
				}
			}
			break;
			case 1:
			{
				const char* fullCode = fmtutil::format("{}.{}", item._queue.exchg, item._queue.code);
				auto it = _set_subs.find(fullCode);
				if (it != _set_subs.end())
				{
					WTSOrdQueData* newData = WTSOrdQueData::create(item._queue);
					if (_sink)
						_sink->handleOrderQueue(newData);
					newData->release();

					static uint32_t recv_cnt = 0;
					recv_cnt++;
					if (recv_cnt % _gpsize == 0)
						write_log(_sink, LL_DEBUG, "[ParserShm] {} queues received in total", recv_cnt);
				}
			}
			break;
			case 2:
			{
				const char* fullCode = fmtutil::format("{}.{}", item._order.exchg, item._order.code);
				auto it = _set_subs.find(fullCode);
				if (it != _set_subs.end())
				{
					WTSOrdDtlData* newData = WTSOrdDtlData::create(item._order);
					if (_sink)
						_sink->handleOrderDetail(newData);
					newData->release();

					static uint32_t recv_cnt = 0;
					recv_cnt++;
					if (recv_cnt % _gpsize == 0)
						write_log(_sink, LL_DEBUG, "[ParserShm] {} orders received in total", recv_cnt);
				}
			}
			break;
			case 3:
			{
				const char* fullCode = fmtutil::format("{}.{}", item._trans.exchg, item._trans.code);
				auto it = _set_subs.find(fullCode);
				if (it != _set_subs.end())
				{
					WTSTransData* newData = WTSTransData::create(item._trans);
					if (_sink)
						_sink->handleTransaction(newData);
					newData->release();

					static uint32_t recv_cnt = 0;
					recv_cnt++;
					if (recv_cnt % _gpsize == 0)
						write_log(_sink, LL_DEBUG, "[ParserShm] {} transactions received in total", recv_cnt);
				}
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