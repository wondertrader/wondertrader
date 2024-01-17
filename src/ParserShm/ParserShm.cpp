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
#include "../Includes/IBaseDataMgr.h"
#include "../Includes/WTSContractInfo.hpp"
#include "../Share/CpuHelper.hpp"
#include "../Share/StrUtil.hpp"

#include <boost/bind.hpp>

 //By Wesley @ 2022.01.05
#include "../Share/fmtlib.h"
template<typename... Args>
inline void write_log(IParserSpi* sink, WTSLogLevel ll, const char* format, const Args&... args) noexcept
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
	_gpsize = config->getUInt32("gpsize", 1000);
	_check_span = config->getUInt32("checkspan", 0);
	_cpu = config->getUInt32("cpu", 0);
	return true;
}

void ParserShm::release()
{
	disconnect();
	if (_thrd_parser)
		_thrd_parser->join();
}

bool ParserShm::connect()
{
	_thrd_parser.reset(new StdThread([this]() {

		write_log(_sink, LL_INFO, "[ParserShm] loading {} ...", _path);
		while (!StdFile::exists(_path.c_str()))
		{
			if (_stopped)
				return;

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

		/*
		 *	By Wesley @ 2023.12.28
		 *	新增一个绑核的逻辑
		 */
		if (_cpu > 0)
			CpuHelper::bind_core(_cpu - 1);

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
				WTSContractInfo* cInfo = _bd_mgr->getContract(item._tick.code, item._tick.exchg);
				if(cInfo == NULL)
					break;

				const char* fullCode = cInfo->getFullCode();
				bool isSubbed = !_subbed.empty() && _subbed[cInfo->getTotalIndex()];
				if (isSubbed)
				{
					WTSTickData* newData = WTSTickData::create(item._tick);
					newData->setContractInfo(cInfo);
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
				WTSContractInfo* cInfo = _bd_mgr->getContract(item._queue.code, item._queue.exchg);
				if (cInfo == NULL)
					break;

				const char* fullCode = cInfo->getFullCode();
				bool isSubbed = !_subbed.empty() && _subbed[cInfo->getTotalIndex()];
				if (isSubbed)
				{
					WTSOrdQueData* newData = WTSOrdQueData::create(item._queue);
					newData->setContractInfo(cInfo);
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
				WTSContractInfo* cInfo = _bd_mgr->getContract(item._order.code, item._order.exchg);
				if(cInfo == NULL)
					break;

				const char* fullCode = cInfo->getFullCode();
				bool isSubbed = !_subbed.empty() && _subbed[cInfo->getTotalIndex()];
				if (isSubbed)
				{
					WTSOrdDtlData* newData = WTSOrdDtlData::create(item._order);
					newData->setContractInfo(cInfo);
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
				WTSContractInfo* cInfo = _bd_mgr->getContract(item._trans.code, item._trans.exchg);
				if (cInfo == NULL)
					break;

				const char* fullCode = cInfo->getFullCode();
				bool isSubbed = !_subbed.empty() && _subbed[cInfo->getTotalIndex()];
				if (isSubbed)
				{
					WTSTransData* newData = WTSTransData::create(item._trans);
					newData->setContractInfo(cInfo);
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
	if (_subbed.empty())
		_subbed.resize(_bd_mgr->getGlobalSize(), false);
	
	for(auto cit = vecSymbols.begin(); cit != vecSymbols.end(); cit++)
	{
		const auto &code = *cit;
		auto ay = StrUtil::split(code, ".");
		WTSContractInfo* cInfo = _bd_mgr->getContract(ay[1].c_str(), ay[0].c_str());
		if(cInfo == NULL)
			continue;

		_subbed[cInfo->getTotalIndex()] = true;
	}
}

void ParserShm::unsubscribe(const CodeSet &setSymbols)
{
	for (auto cit = setSymbols.begin(); cit != setSymbols.end(); cit++)
	{
		const auto &code = *cit;
		auto ay = StrUtil::split(code, ".");
		WTSContractInfo* cInfo = _bd_mgr->getContract(ay[0].c_str(), ay[1].c_str());
		if (cInfo == NULL)
			continue;

		_subbed[cInfo->getTotalIndex()] = false;
	}
}

void ParserShm::registerSpi( IParserSpi* listener )
{
	bool bReplaced = (_sink!=NULL);
	_sink = listener;
	if(bReplaced && _sink)
	{
		write_log(_sink, LL_WARN, "Listener is replaced");
	}

	if (_sink)
	{
		_bd_mgr = _sink->getBaseDataMgr();
		_subbed.resize(_bd_mgr->getGlobalSize(), false);
	}
}