#include <string>
#include <map>
//v6.3.15
#include "../API/CTP6.3.15/ThostFtdcTraderApi.h"
#include "TraderSpi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/ModuleHelper.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/StdUtils.hpp"

#ifdef _WIN32
#include "../Share/charconv.hpp"
#endif

#include <boost/filesystem.hpp>

// UserApi对象
CThostFtdcTraderApi* pUserApi;

// 配置参数
std::string	FRONT_ADDR;	// 前置地址
std::string	BROKER_ID;	// 经纪公司代码
std::string	INVESTOR_ID;// 投资者代码
std::string	PASSWORD;	// 用户密码
std::string SAVEPATH;	//保存位置
std::string APPID;
std::string AUTHCODE;
uint32_t	CLASSMASK;	//期权

std::string COMM_FILE;		//输出的品种文件名
std::string CONT_FILE;		//输出的合约文件名

std::string MODULE_NAME;	//外部模块名

typedef std::map<std::string, std::string>	SymbolMap;
SymbolMap	MAP_NAME;
SymbolMap	MAP_SESSION;

typedef CThostFtdcTraderApi* (*CTPCreator)(const char *);
CTPCreator		g_ctpCreator = NULL;

// 请求编号
int iRequestID = 0;

#ifdef _MSC_VER
#	define EXPORT_FLAG __declspec(dllexport)
#else
#	define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORT_FLAG int run(const char* cfgfile, bool bAsync);
#ifdef __cplusplus
}
#endif

int run(const char* cfgfile, bool bAsync = false)
{
	std::string cfg = cfgfile;
	IniHelper ini;
	ini.load(cfg.c_str());

	FRONT_ADDR = ini.readString("ctp", "front", "");
	BROKER_ID	= ini.readString("ctp", "broker", "");
	INVESTOR_ID = ini.readString("ctp", "user", "");
	PASSWORD	= ini.readString("ctp", "pass", "");
	APPID = ini.readString("ctp", "appid", "");
	AUTHCODE = ini.readString("ctp", "authcode", "");

	SAVEPATH	= ini.readString("config", "path", "");
	CLASSMASK = ini.readUInt("config", "mask", 1 | 2 | 4); //1-期货,2-期权,4-股票

	COMM_FILE = ini.readString("config", "commfile", "commodities.json");
	CONT_FILE = ini.readString("config", "contfile", "contracts.json");

#ifdef _WIN32
	MODULE_NAME = ini.readString("config", "module", "thosttraderapi_se.dll");
#else
	MODULE_NAME = ini.readString("config", "module", "thosttraderapi_se.so");
#endif
	if(!boost::filesystem::exists(MODULE_NAME.c_str()))
	{
		MODULE_NAME = getBinDir();
#ifdef _WIN32
		MODULE_NAME += "traders/thosttraderapi_se.dll";
#else
		MODULE_NAME += "traders/thosttraderapi_se.so";
#endif
	}

	if(FRONT_ADDR.empty() || BROKER_ID.empty() || INVESTOR_ID.empty() || PASSWORD.empty() || SAVEPATH.empty())
	{
		return 0;
	}

	SAVEPATH = StrUtil::standardisePath(SAVEPATH);

	std::string map_files = ini.readString("config", "mapfiles", "");
	if(!map_files.empty())
	{
		StringVector ayFiles = StrUtil::split(map_files, ",");
		for(const std::string& fName:ayFiles)
		{
			printf("Reading mapping file %s...\r\n", fName.c_str());
			IniHelper iniMap;
			if(!StdFile::exists(fName.c_str()))
				continue;

			iniMap.load(fName.c_str());
			FieldArray ayKeys, ayVals;
			int cout = iniMap.readSecKeyValArray("Name", ayKeys, ayVals);
			for (int i = 0; i < cout; i++)
			{
				MAP_NAME[ayKeys[i]] = ayVals[i];
#ifdef _WIN32
				printf("Commodity name mapping: %s - %s\r\n", ayKeys[i].c_str(), UTF8toChar(ayVals[i]).c_str());
#else
				printf("Commodity name mapping: %s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
#endif
			}

			ayKeys.clear();
			ayVals.clear();
			cout = iniMap.readSecKeyValArray("Session", ayKeys, ayVals);
			for (int i = 0; i < cout; i++)
			{
				MAP_SESSION[ayKeys[i]] = ayVals[i];
				printf("Trading session mapping: %s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
			}
		}
		
	}

	// 初始化UserApi
	DllHandle dllInst = DLLHelper::load_library(MODULE_NAME.c_str());
	if (dllInst == NULL)
		printf("Loading module %s failed\r\n", MODULE_NAME.c_str());
#ifdef _WIN32
#	ifdef _WIN64
	g_ctpCreator = (CTPCreator)DLLHelper::get_symbol(dllInst, "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPEAV1@PEBD@Z");
#	else
	g_ctpCreator = (CTPCreator)DLLHelper::get_symbol(dllInst, "?CreateFtdcTraderApi@CThostFtdcTraderApi@@SAPAV1@PBD@Z");
#	endif
#else
	g_ctpCreator = (CTPCreator)DLLHelper::get_symbol(dllInst, "_ZN19CThostFtdcTraderApi19CreateFtdcTraderApiEPKc");
#endif
	if (g_ctpCreator == NULL)
		printf("Loading CreateFtdcTraderApi failed\r\n");
	pUserApi = g_ctpCreator("");			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);					// 注册私有流
	pUserApi->RegisterFront((char*)FRONT_ADDR.c_str());				// connect
	pUserApi->Init();

    //如果不是异步，则等待API返回
    if(!bAsync)
	    pUserApi->Join();

	return 0;
}