#pragma once
#include "../Includes/IParserApi.h"

USING_NS_WTP;

class ExpParser : public IParserApi
{
public:
	ExpParser(const char* id):_id(id){}
	virtual ~ExpParser(){}

public:
	virtual bool init(WTSVariant* config) override;


	virtual void release() override;


	virtual bool connect() override;


	virtual bool disconnect() override;


	virtual bool isConnected() override { return true; }


	virtual void subscribe(const CodeSet& setCodes) override;


	virtual void unsubscribe(const CodeSet& setCodes) override;


	virtual void registerSpi(IParserSpi* listener) override;

private:
	std::string			_id;
	IParserSpi*			m_sink;
	IBaseDataMgr*		m_pBaseDataMgr;
};

