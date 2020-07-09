/*!
 * \file ParserAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <unordered_set>
#include <unordered_map>
#include <boost/core/noncopyable.hpp>

#include "../Includes/IParserApi.h"
#include "../Share/BoostDefine.h"


NS_OTP_BEGIN
class WTSVariant;
class IHotMgr;

class IParserStub
{
public:
	virtual void			handle_push_quote(WTSTickData* curTick, bool isHot = false){}
};

class ParserAdapter : public IParserApiListener,
					private boost::noncopyable
{
public:
	ParserAdapter();
	~ParserAdapter();

public:
	bool	init(const char* id, WTSVariant* cfg, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr = NULL);

	void	release();

	bool	run();

	const char* id() const{ return _id.c_str(); }

public:
	virtual void handleSymbolList(const WTSArray* aySymbols) override {}

	virtual void handleQuote(WTSTickData *quote, bool bNeedSlice) override;

	virtual void handleParserLog(WTSLogLevel ll, const char* format, ...) override;

	virtual IBaseDataMgr* getBaseDataMgr() override { return _bd_mgr; }


private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;

	bool				_stopped;

	typedef std::unordered_set<std::string>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	IBaseDataMgr*		_bd_mgr;
	IHotMgr*			_hot_mgr;
	IParserStub*		_stub;
	WTSVariant*			_cfg;
	std::string			_id;
};

typedef std::shared_ptr<ParserAdapter>	ParserAdapterPtr;
typedef std::unordered_map<std::string, ParserAdapterPtr>	ParserAdapterMap;

class ParserAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();

	ParserAdapterPtr getAdapter(const char* id);

	bool	addAdapter(const char* id, ParserAdapterPtr& adapter);


public:
	ParserAdapterMap _adapters;
};

NS_OTP_END