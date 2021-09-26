#include "ExpParser.h"
#include "WtDtRunner.h"

extern WtDtRunner& getRunner();

bool ExpParser::init(WTSParams* config)
{
	getRunner().parser_init(_id.c_str());
	return true;
}

void ExpParser::release()
{
	getRunner().parser_release(_id.c_str());
}

bool ExpParser::connect()
{
	getRunner().parser_connect(_id.c_str());
	return true;
}

bool ExpParser::disconnect()
{
	getRunner().parser_disconnect(_id.c_str());
	return true;
}

void ExpParser::subscribe(const CodeSet& setCodes)
{
	for(const std::string& code : setCodes)
		getRunner().parser_subscribe(_id.c_str(), code.c_str());
}

void ExpParser::unsubscribe(const CodeSet& setCodes)
{
	for (const std::string& code : setCodes)
		getRunner().parser_unsubscribe(_id.c_str(), code.c_str());
}

void ExpParser::registerSpi(IParserSpi* listener)
{
	m_sink = listener;

	if (m_sink)
		m_pBaseDataMgr = m_sink->getBaseDataMgr();
}
