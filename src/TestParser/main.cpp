#include <iostream>
#include <boost/filesystem.hpp>

#include "../Includes/IParserApi.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSContractInfo.hpp"
#include "../Includes/WTSDataDef.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"

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
        DllHandle hInst = DLLHelper::load_library(moduleName);
		if (hInst == NULL)
		{
			log("模块{}加载失败", moduleName);
			return false;
		}

		FuncCreateParser pCreator = (FuncCreateParser)DLLHelper::get_symbol(hInst, "createParser");
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
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath.c_str();
}

int main()
{
	WTSLogger::init();

	WTSLogger::info("启动成功,当前系统版本号: v1.0");

	WTSVariant* root = WTSCfgLoader::load_from_file("config.yaml", true);
	if (root == NULL)
	{
		WTSLogger::log_raw(LL_ERROR, "配置文件config.yaml加载失败");
		return 0;
	}

	WTSVariant* cfg = root->get("config");
	bool isUTF8 = cfg->getBoolean("utf8");
	if (cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"), isUTF8);

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"), isUTF8);

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"), isUTF8);

	std::string module = cfg->getCString("parser");
	std::string profile = cfg->getCString("profile");
	WTSVariant* params = root->get(profile.c_str());
	if (params == NULL)
	{
		WTSLogger::error_f("配置项{}不存在", profile);
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