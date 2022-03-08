#include <windows.h>
#include <iostream>
#include <mutex>

#include "IniFile.hpp"

#include "../Includes/IParserApi.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"


WTSBaseDataMgr	g_bdMgr;

USING_NS_WTP;

template<typename... Args>
void log(const char* format, const Args& ...args)
{
	WTSLogger::log_raw(LL_INFO, fmt::format(format, args...).c_str());
}

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
			WTSCommodityInfo* pCommInfo = g_bdMgr.getCommodity(contract);
			contractSet.insert(contract->getFullCode());
		}

		ayContract->release();
		_api->subscribe(contractSet);
		_api->connect();
	}

	bool createParser(const char* moduleName)
	{
		HINSTANCE hInst = LoadLibrary(moduleName);
		if (hInst == NULL)
		{
			log("模块{}加载失败", moduleName);
			return false;
		}

		FuncCreateParser pCreator = (FuncCreateParser)GetProcAddress(hInst, "createParser");
		if (NULL == pCreator)
		{
			log("接口创建函数读取失败");
			return false;
		}

		_api = pCreator();
		if (NULL == _api)
		{
			log("接口创建失败");
			return false;
		}

		m_funcRemover = (FuncDeleteParser)GetProcAddress(hInst, "deleteParser");
		return true;
	}

public:
	virtual void handleParserLog(WTSLogLevel ll, const char* message) override
	{
		WTSLogger::log_raw(ll, message);
	}

	virtual void handleQuote(WTSTickData *quote, uint32_t procFlag) override
	{
		log("{}@{}.{}, price:{}, voume:{}", quote->code(), quote->actiondate(), quote->actiontime(), quote->price(), quote->totalvolume());
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
		char path[MAX_PATH] = { 0 };
		GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);

		basePath = path;
		auto pos = basePath.find_last_of('\\');
		basePath = basePath.substr(0, pos + 1);
	}

	return basePath;
}

int main()
{
	WTSLogger::init();

	WTSLogger::info("启动成功,当前系统版本号: v1.0");

	std::string cfg = getBaseFolder() + "config.ini";

	bool isUTF8 = IniFile::ReadConfigInt("config", "utf8", 0, cfg.c_str())==1;
	std::string file = IniFile::ReadConfigString("config", "session", "", cfg.c_str());
	if(!file.empty())
		g_bdMgr.loadSessions(file.c_str(), isUTF8);

	file = IniFile::ReadConfigString("config", "commodity", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadCommodities(file.c_str(), isUTF8);

	file = IniFile::ReadConfigString("config", "contract", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadContracts(file.c_str(), isUTF8);

	file = IniFile::ReadConfigString("config", "holiday", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadHolidays(file.c_str());


	WTSVariant* params = WTSVariant::createObject();

	std::string module = IniFile::ReadConfigString("config", "parser", "", cfg.c_str());
	std::string profile = IniFile::ReadConfigString("config", "profile", "", cfg.c_str());

	StringVector ayKeys, ayVals;
	IniFile::ReadConfigSectionKeyValueArray(ayKeys, ayVals, profile.c_str(), cfg.c_str());
	for (uint32_t i = 0; i < ayKeys.size(); i++)
	{
		const char* key = ayKeys[i].c_str();
		const char* val = ayVals[i].c_str();

		params->append(key, val);
	}

	ParserSpi* parser = new ParserSpi;
	parser->init(params, module.c_str());
	parser->run();
	params->release();

	getchar();
	
	//exit(9);
	parser->release();
	delete parser;

	return 0;
}