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

USING_NS_OTP;

class wxMainFrame;
class WTSBaseDataMgr;
class DataManager;

class ParserAdapter : public IParserSpi
{
public:
	ParserAdapter(WTSBaseDataMgr * bgMgr, DataManager* dtMgr);
	~ParserAdapter();

public:
	bool	initAdapter(WTSParams* params, FuncCreateParser funcCreate, FuncDeleteParser funcDelete);

	void	release();

public:
	virtual void handleSymbolList(const WTSArray* aySymbols) override;

	virtual void handleQuote(WTSTickData *quote, bool bNeedSlice) override;

	virtual void handleOrderQueue(WTSOrdQueData* ordQueData) override;

	virtual void handleTransaction(WTSTransData* transData) override;

	virtual void handleOrderDetail(WTSOrdDtlData* ordDetailData) override;

	virtual void handleParserLog(WTSLogLevel ll, const char* format, ...) override;

	virtual IBaseDataMgr* getBaseDataMgr() override;

private:
	IParserApi*			m_pParser;
	FuncCreateParser	m_funcCreate;
	FuncDeleteParser	m_funcDelete;
	WTSBaseDataMgr*		m_bdMgr;
	DataManager*		m_dtMgr;

	bool				m_bStopped;

	typedef std::set<std::string>	CodeFilter;
	CodeFilter		m_codeFilters;
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


