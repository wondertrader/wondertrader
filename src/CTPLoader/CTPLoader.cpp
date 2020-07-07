#include <string>
#include <map>
#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "TraderSpi.h"

#include "../Share/IniHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/DLLHelper.hpp"
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
bool		ISFOROPTION;	//期权

std::string COMM_FILE;		//输出的品种文件名
std::string CONT_FILE;		//输出的合约文件名

typedef std::map<std::string, std::string>	SymbolMap;
SymbolMap	MAP_NAME;
SymbolMap	MAP_SESSION;

// 请求编号
int iRequestID = 0;

std::string getBaseFolder()
{
	static std::string basePath;
	if(basePath.empty())
	{
		basePath = boost::filesystem::initial_path<boost::filesystem::path>().string();

		basePath = StrUtil::standardisePath(basePath);
	}

	return basePath;
}

int main()
{
#ifdef _WIN32
#else
	DLLHelper::load_library("libthosttraderapi_se.so");
#endif

	std::string cfg = getBaseFolder() + "config.ini";
	IniHelper ini;
	ini.load(cfg.c_str());

	FRONT_ADDR = ini.readString("ctp", "front", "");
	BROKER_ID	= ini.readString("ctp", "broker", "");
	INVESTOR_ID = ini.readString("ctp", "user", "");
	PASSWORD	= ini.readString("ctp", "pass", "");
	APPID = ini.readString("ctp", "appid", "");
	AUTHCODE = ini.readString("ctp", "authcode", "");

	SAVEPATH	= ini.readString("config", "path", "");
	ISFOROPTION = ini.readInt("config", "option", 0) == 1;

	COMM_FILE = ini.readString("config", "commfile", "commodities.json");
	CONT_FILE = ini.readString("config", "contfile", "contracts.json");

	if(FRONT_ADDR.empty() || BROKER_ID.empty() || INVESTOR_ID.empty() || PASSWORD.empty() || SAVEPATH.empty())
	{
		return 0;
	}

	SAVEPATH = StrUtil::standardisePath(SAVEPATH);

	FieldArray ayKeys, ayVals;
	int cout = ini.readSecKeyValArray("Name", ayKeys, ayVals);
	for (int i = 0; i < cout; i++)
	{
		MAP_NAME[ayKeys[i]] = ayVals[i];
		printf("品种名称映射：%s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
	}

	ayKeys.clear();
	ayVals.clear();
	cout = ini.readSecKeyValArray("Session", ayKeys, ayVals);
	for (int i = 0; i < cout; i++)
	{
		MAP_SESSION[ayKeys[i]] = ayVals[i];
		printf("交易时间映射：%s - %s\r\n", ayKeys[i].c_str(), ayVals[i].c_str());
	}

	// 初始化UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);					// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);					// 注册私有流
	pUserApi->RegisterFront((char*)FRONT_ADDR.c_str());				// connect
	pUserApi->Init();

	pUserApi->Join();
	return 0;
}