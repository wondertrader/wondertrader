/*!
 * \file ParserAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 行情解析模块适配类定义
 */
#pragma once
#include <set>
#include <vector>
#include <memory>
#include <boost/core/noncopyable.hpp>
#include "../Includes/IParserApi.h"

NS_OTP_BEGIN
class WTSVariant;
NS_OTP_END

USING_NS_OTP;
class wxMainFrame;
class WTSBaseDataMgr;
class DataManager;

class ParserAdapter : public IParserSpi, private boost::noncopyable
{
public:
	ParserAdapter(WTSBaseDataMgr * bgMgr, DataManager* dtMgr);
	~ParserAdapter();

public:
	bool	init(const char* id, WTSVariant* cfg);

	bool	initExt(const char* id, IParserApi* api);

	void	release();

	bool	run();

	const char* id() const { return _id.c_str(); }

public:
	virtual void handleSymbolList(const WTSArray* aySymbols) override;

	virtual void handleQuote(WTSTickData *quote, bool bNeedSlice) override;

	virtual void handleOrderQueue(WTSOrdQueData* ordQueData) override;

	virtual void handleTransaction(WTSTransData* transData) override;

	virtual void handleOrderDetail(WTSOrdDtlData* ordDetailData) override;

	virtual void handleParserLog(WTSLogLevel ll, const char* format, ...) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;
	WTSBaseDataMgr*		_bd_mgr;
	DataManager*		_dt_mgr;

	bool				_stopped;

	typedef faster_hashset<std::string>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	ExchgFilter			_code_filter;
	WTSVariant*			_cfg;
	std::string			_id;
};

typedef std::shared_ptr<ParserAdapter>	ParserAdapterPtr;
typedef faster_hashmap<std::string, ParserAdapterPtr>	ParserAdapterMap;

class ParserAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();

	ParserAdapterPtr getAdapter(const char* id);

	bool	addAdapter(const char* id, ParserAdapterPtr& adapter);

	uint32_t size() const { return _adapters.size(); }

public:
	ParserAdapterMap _adapters;
};


