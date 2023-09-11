#include <iostream>
#include <boost/filesystem.hpp>

#include "../Includes/IParserApi.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

WTSBaseDataMgr	g_bdMgr;

USING_NS_WTP;

class ParserSpi : public IParserSpi
{
public:
	ParserSpi(){}

	bool init(WTSVariant* params, const char* ttype)
	{
		m_pParams = params;
		if (m_pParams)
			m_pParams->retain();

		m_strModule = ttype;
		return true;
	}

	void release()
	{
		if (_api)
		{
			_api->release();
		}
	}

	void run(bool bRestart = false)
	{
		if (!createParser(m_strModule.c_str()))
		{
			return;
		}

		_api->registerSpi(this);

		if (!_api->init(m_pParams))
		{
			return;
		}

		ContractSet contractSet;
		WTSArray* ayContract = g_bdMgr.getContracts();
		WTSArray::Iterator it = ayContract->begin();
		for (; it != ayContract->end(); it++)
		{
			WTSContractInfo* contract = STATIC_CONVERT(*it, WTSContractInfo*);
			contractSet.insert(contract->getFullCode());
		}

		ayContract->release();
		_api->subscribe(contractSet);
		_api->connect();
	}

	bool createParser(const char* moduleName)
	{
        DllHandle hInst = DLLHelper::load_library(moduleName);
		if (hInst == NULL)
		{
			WTSLogger::error("Loading module {} failed", moduleName);
			return false;
		}

		FuncCreateParser pCreator = (FuncCreateParser)DLLHelper::get_symbol(hInst, "createParser");
		if (NULL == pCreator)
		{
			WTSLogger::error("Entry function createParser not found");
			return false;
		}

		_api = pCreator();
		if (NULL == _api)
		{
			WTSLogger::error("Creating parser api failed");
			return false;
		}

		m_funcRemover = (FuncDeleteParser)DLLHelper::get_symbol(hInst, "deleteParser");
		return true;
	}

public:
	virtual void handleParserLog(WTSLogLevel ll, const char* message) override
	{
		WTSLogger::log_raw(ll, message);
	}

	virtual void handleQuote(WTSTickData *quote, uint32_t procFlag) override
	{
		WTSLogger::info("{}@{}.{}, price:{}, voume:{}", quote->code(), quote->actiondate(), quote->actiontime(), quote->price(), quote->totalvolume());
	}

	virtual void handleSymbolList(const WTSArray* aySymbols) override
	{

	}

public:
	virtual IBaseDataMgr*	getBaseDataMgr()
	{
		return &g_bdMgr;
	}
	

private:
	IParserApi*			_api;
	FuncDeleteParser	m_funcRemover;
	std::string			m_strModule;
	WTSVariant*			m_pParams;
};

std::string getBaseFolder()
{
	static std::string basePath;
	if (basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}

int main()
{
	WTSLogger::init("logcfg.yaml");

	WTSVariant* root = WTSCfgLoader::load_from_file("config.yaml");
	if (root == NULL)
	{
		WTSLogger::log_raw(LL_ERROR, "Loading config.yaml failed");
		return 0;
	}

	WTSVariant* cfg = root->get("config");
	if (cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"));

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"));

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"));

	std::string module = cfg->getCString("parser");
	std::string profile = cfg->getCString("profile");
	WTSVariant* params = root->get(profile.c_str());
	if (params == NULL)
	{
		WTSLogger::error("Configure {} not exist", profile);
		return 0;
	}

	ParserSpi* parser = new ParserSpi;
	parser->init(params, module.c_str());
	parser->run();

	root->release();

	getchar();
	
	//exit(9);
	parser->release();
	delete parser;

	return 0;
}