#include <windows.h>
#include <iostream>
#include <mutex>

#include "IniFile.hpp"

#include "..\Includes\ITraderApi.h"
#include "..\Includes\WTSParams.hpp"
#include "..\Includes\WTSTradeDef.hpp"
#include "..\Includes\WTSError.hpp"
#include "..\Includes\WTSCollection.hpp"

#include "..\Share\TimeUtils.hpp"
#include "..\Share\StdUtils.hpp"

#include "..\WTSTools\WTSBaseDataMgr.h"
#include "..\WTSTools\WTSLogger.h"


WTSBaseDataMgr	g_bdMgr;
StdUniqueMutex	g_mtxOpt;
StdCondVariable	g_condOpt;
bool		g_exitNow = false;
bool		g_riskAct = false;
std::set<std::string>	g_blkList;

USING_NS_OTP;

typedef enum tagTradeAccountType
{
	TAT_CTP,		//CTP接口
	TAT_CTPTest,	//CTP测试
	TAT_Femas,
	TAT_CTPMini,
	TAT_CTPMiniTest
}TraderType;

void log(const char* fmt, ...)
{
	//char szBuf[512] = { 0 };
	va_list args;
	va_start(args, fmt);
	WTSLogger::vlog(LL_INFO, fmt, args);
	va_end(args);

	//printf(szBuf);
	//printf("\r\n");
	
}

void log_raw(const char* message)
{
	printf(message);
	printf("\r\n");
	//WTSLogger::info(message);
}

class TraderSpi : public ITraderSpi
{
public:
	TraderSpi() :m_bLogined(false), m_mapOrds(NULL){}

	bool init(WTSParams* params, const char* ttype)
	{
		m_pParams = params;
		if (m_pParams)
			m_pParams->retain();

		m_strModule = ttype;
		return true;
	}

	void release()
	{
		if (m_pTraderApi)
		{
			m_pTraderApi->release();
		}
	}

	void run(bool bRestart = false)
	{
		if (!createTrader(m_strModule.c_str()))
		{
			return;
		}

		m_pTraderApi->registerSpi(this);

		if (!m_pTraderApi->init(m_pParams))
		{
			return;
		}

		log("[%s]开始连接服务端: %s", m_pParams->getCString("user"), m_pParams->getCString("front"));
		m_pTraderApi->connect();
	}

	bool createTrader(const char* moduleName)
	{
		HINSTANCE hInst = LoadLibrary(moduleName);
		if (hInst == NULL)
		{
			log("交易模块%s加载失败", moduleName);
			return false;
		}

		FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)GetProcAddress(hInst, "createTrader");
		if (NULL == pFunCreateTrader)
		{
			log("交易接口创建函数读取失败");
			return false;
		}

		m_pTraderApi = pFunCreateTrader();
		if (NULL == m_pTraderApi)
		{
			log("交易接口创建失败");
			return false;
		}

		m_funcDelTrader = (FuncDeleteTrader)GetProcAddress(hInst, "deleteTrader");
		return true;
	}

	bool qryFund()
	{
		log("[%s]正在查询资金...", m_pParams->getCString("user"));
		m_pTraderApi->queryAccount();

		return true;
	}

	bool qryOrders()
	{
		log("[%s]正在查询当日委托...", m_pParams->getCString("user"));
		m_pTraderApi->queryOrders();

		return true;
	}

	bool qryTrades()
	{
		log("%s]正在查询当日成交...", m_pParams->getCString("user"));
		m_pTraderApi->queryTrades();

		return true;
	}

	bool qryPosition()
	{
		log("[%s]正在查询持仓...",  m_pParams->getCString("user"));
		m_pTraderApi->queryPositions();

		return true;
	}

	bool qrySettle()
	{
		uint32_t uDate = TimeUtils::getNextDate(TimeUtils::getCurDate(), -1);
		log("[%s]正在查询%u的结算单...", m_pParams->getCString("user"), uDate);
		m_pTraderApi->querySettlement(uDate);

		return true;
	}

	bool entrustLmt()
	{
		char code[32] = { 0 };
		char exchg[32] = { 0 };
		double price = 0.0;
		uint32_t qty = 0;
		uint32_t bs = 0;
		uint32_t offset = 0;

		for (;;)
		{
			printf("请输入品种代码: ");
			std::cin >> code;

			printf("请输入交易所代码: ");
			std::cin >> exchg;

			printf("请输入委托价格: ");
			std::cin >> price;

			printf("请输入数量: ");
			std::cin >> qty;

			printf("请输入方向,0-做多,1-做空: ");
			std::cin >> bs;
			if(bs != 0 && bs != 1)
				continue;

			printf("请输入开平,0-开仓,1-平仓: ");
			std::cin >> offset;
			if (offset != 0 && offset != 1)
				continue;

			printf("品种: %s.%s,价格: %f,数量: %u,方向: %s,开平: %s,确认y/n? ", exchg, code, price, qty, bs == 0 ? "做多" : "做空", offset == 0 ? "开仓" : "平仓");
			char c;
			std::cin >> c;
			if(c == 'y')
				break;
		}

		if(g_riskAct)
		{
			auto it = g_blkList.find(code);
			if (it != g_blkList.end())
			{
				log("%s已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);

		WTSEntrust* entrust = WTSEntrust::create(code, qty, price, exchg);
		entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
		entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday?WOT_CLOSETODAY:WOT_CLOSE));
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setTimeCondition(WTC_GFD);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		log("[%s]开始下单,品种: %s.%s,价格: %f,数量: %d,动作: %s%s", m_pParams->getCString("user"), exchg, code, price, qty, offset == 0 ? "开" : "平", bs == 0 ? "多" : "空");

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool entrustMkt()
	{
		char code[32] = { 0 };
		char exchg[32] = { 0 };
		uint32_t qty = 0;
		uint32_t bs = 0;
		uint32_t offset = 0;

		for (;;)
		{
			printf("请输入品种代码: ");
			std::cin >> code;

			printf("请输入交易所代码: ");
			std::cin >> exchg;

			printf("请输入数量: ");
			std::cin >> qty;

			printf("请输入方向,0-做多,1-做空: ");
			std::cin >> bs;

			printf("请输入开平,0-开仓,1-平仓: ");
			std::cin >> offset;

			printf("品种: %s.%s,数量: %u,方向: %s,开平: %s,确认y/n? ", exchg, code, qty, bs == 0 ? "做多" : "做空", offset == 0 ? "开仓" : "平仓");
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		if (g_riskAct)
		{
			auto it = g_blkList.find(code);
			if (it != g_blkList.end())
			{
				log("%s已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);
		WTSEntrust* entrust = WTSEntrust::create(code, qty, 0, exchg);
		entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
		entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday ? WOT_CLOSETODAY : WOT_CLOSE));
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setTimeCondition(WTC_IOC);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		log("[%s]开始下单,品种: %s.%s,价格: 市价,数量: %d,动作: %s%s", m_pParams->getCString("user"), exchg, code, qty, offset == 0 ? "开" : "平", bs == 0 ? "多" : "空");

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool cancel()
	{
		char orderid[128] = { 0 };

		for (;;)
		{
			printf("请输入订单ID: ");
			std::cin >> orderid;

			printf("订单ID: %s,确认y/n? ", orderid);
			char c;
			std::cin >> c;
			if (c == 'y')
				break;
		}

		if (m_mapOrds == NULL)
			m_mapOrds = WTSObjectMap::create();

		WTSOrderInfo* ordInfo = (WTSOrderInfo*)m_mapOrds->get(orderid);
		if (ordInfo == NULL)
		{
			printf("订单不存在,请检查订单号是否有误,或者先查询订单\r\n");
			return false;
		}


		log("[%s]开始撤单[%s]...", m_pParams->getCString("user"), orderid);
		WTSEntrustAction* action = WTSEntrustAction::create(ordInfo->getCode());
		action->setEntrustID(ordInfo->getEntrustID());
		action->setOrderID(orderid);
		action->setActionFlag(WAF_CANCEL);

		m_pTraderApi->orderAction(action);
		action->release();

		return true;
	}

public:
	virtual void handleEvent(WTSTraderEvent e, int32_t ec)
	{
		if(e == WTE_Connect)
		{
			if (ec == 0)
			{
				log("[%s]连接成功", m_pParams->getCString("user"));
				m_pTraderApi->login(m_pParams->getCString("user"), m_pParams->getCString("pass"), "");
			}
			else
			{
				g_exitNow = true;
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
	}

	virtual void handleTraderLog(WTSLogLevel ll, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		WTSLogger::vlog(LL_INFO, format, args);
		va_end(args);
	}

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
	{
		if(bSucc)
		{
			log("[%s]登录成功" , m_pParams->getCString("user"));	
			m_bLogined = true;
		}
		else
		{
			log("[%s]登录失败: %s", m_pParams->getCString("user"), msg);
			g_exitNow = true;
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err)
	{
		if(err)
		{
			log("[%s]下单失败: %s", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		
	}

	virtual void onRspAccount(WTSArray* ayAccounts)
	{
		if(ayAccounts != NULL)
		{
			WTSAccountInfo* accInfo = (WTSAccountInfo*)ayAccounts->at(0);
			if(accInfo)
			{
				log("[%s]资金数据更新, 当前静态权益: %.2f", m_pParams->getCString("user"), accInfo->getBalance());
			}
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspPosition(const WTSArray* ayPositions)
	{
		uint32_t cnt = 0;
		if (ayPositions != NULL)
			cnt = ayPositions->size();

		log("[%s]持仓数据已更新, 当日共有%u笔持仓", m_pParams->getCString("user"), cnt);
		for(uint32_t i = 0; i < cnt; i++)
		{
			WTSPositionItem* posItem = (WTSPositionItem*)((WTSArray*)ayPositions)->at(i);
			if(posItem && posItem->getTotalPosition() > 0 && g_riskAct)
			{
				g_blkList.insert(posItem->getCode());
				log("%s持仓量超限,限制开仓", posItem->getCode());
			}
		}
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspOrders(const WTSArray* ayOrders)
	{
		uint32_t cnt = 0;
		if (ayOrders != NULL)
			cnt = ayOrders->size();

		if (m_mapOrds == NULL)
			m_mapOrds = WTSObjectMap::create();

		m_mapOrds->clear();
		for (uint32_t i = 0; i < cnt; i++)
		{
			WTSOrderInfo* ordInfo = (WTSOrderInfo*)((WTSArray*)ayOrders)->at(i);
			if (ordInfo->isAlive())
				m_mapOrds->add(ordInfo->getOrderID(), ordInfo, true);
		}

		log("[%s]委托列表已更新, 当日共有%u笔委托, 未完成%u笔", m_pParams->getCString("user"), cnt, m_mapOrds->size());

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspTrades(const WTSArray* ayTrades)
	{
		uint32_t cnt = 0;
		if (ayTrades != NULL)
			cnt = ayTrades->size();

		log("[%s]成交明细已更新, 当日共有%u笔成交", m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspSettlementInfo(uint32_t uDate, const char* content)
	{
		log("[%s]%u结算单已接收", m_pParams->getCString("user"), uDate);
		log_raw(content);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onPushOrder(WTSOrderInfo* orderInfo)
	{
		if(orderInfo->getOrderState() != WOS_Canceled)
		{
			if(strlen(orderInfo->getOrderID()) > 0)
			{
				if (m_mapOrds == NULL)
					m_mapOrds = WTSObjectMap::create();

				if (m_mapOrds->find(orderInfo->getOrderID()) == m_mapOrds->end())
				{
					log("[%s]下单成功,订单ID: %s",  m_pParams->getCString("user"), orderInfo->getOrderID());
					m_mapOrds->add(orderInfo->getOrderID(), orderInfo, true);
				}

				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
		else 
		{
			if (m_mapOrds)
				m_mapOrds->remove(orderInfo->getOrderID());

			if (strlen(orderInfo->getOrderID()) == 0)
			{
				log("[%s]订单%s提交失败被撤销:%s", m_pParams->getCString("user"), orderInfo->getEntrustID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
			else
			{
				log("[%s]订单%s已撤销:%s", m_pParams->getCString("user"), orderInfo->getOrderID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}			
		}
	}

	virtual void onPushTrade(WTSTradeInfo* tradeRecord)
	{
		log("[%s]收到成交回报,合约%s,成交价: %.4f,成交数量: %.4f", m_pParams->getCString("user"), tradeRecord->getCode(), tradeRecord->getPrice(), tradeRecord->getVolume());

		if(g_riskAct)
		{
			log("[%s]%s超过最大持仓数量,禁止开仓", m_pParams->getCString("user"), tradeRecord->getCode());

			g_blkList.insert(tradeRecord->getCode());
		}
	}

	virtual void onTraderError(WTSError*	err)
	{
		if(err && err->getErrorCode() == WEC_ORDERCANCEL)
		{
			log("[%s]撤单失败: %s", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		else if (err && err->getErrorCode() == WEC_ORDERINSERT)
		{
			log("[%s]下单失败: %s", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
	}

public:
	virtual IBaseDataMgr*	getBaseDataMgr()
	{
		return &g_bdMgr;
	}
	

private:
	ITraderApi*			m_pTraderApi;
	FuncDeleteTrader	m_funcDelTrader;
	std::string			m_strModule;
	WTSParams*			m_pParams;

	typedef WTSHashMap<std::string>	WTSObjectMap;
	WTSObjectMap*		m_mapOrds;

	bool				m_bLogined;
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

void main()
{
	WTSLogger::init();

	WTSLogger::info("启动成功,当前系统版本号: v1.0");

	std::string cfg = getBaseFolder() + "config.ini";

	std::string file = IniFile::ReadConfigString("config", "session", "", cfg.c_str());
	if(!file.empty())
		g_bdMgr.loadSessions(file.c_str());

	file = IniFile::ReadConfigString("config", "commodity", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadCommodities(file.c_str());

	file = IniFile::ReadConfigString("config", "contract", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadContracts(file.c_str());

	file = IniFile::ReadConfigString("config", "holiday", "", cfg.c_str());
	if (!file.empty())
		g_bdMgr.loadHolidays(file.c_str());

	g_riskAct = IniFile::ReadConfigInt("config", "risk", 0, cfg.c_str()) == 1;
	WTSLogger::info("风控开关: %s", g_riskAct ? "开" : "关");


	WTSParams* params = WTSParams::createObject();

	std::string module = IniFile::ReadConfigString("config", "trader", "", cfg.c_str());
	std::string profile = IniFile::ReadConfigString("config", "profile", "", cfg.c_str());

	StringVector ayKeys, ayVals;
	IniFile::ReadConfigSectionKeyValueArray(ayKeys, ayVals, profile.c_str(), cfg.c_str());
	for (uint32_t i = 0; i < ayKeys.size(); i++)
	{
		const char* key = ayKeys[i].c_str();
		const char* val = ayVals[i].c_str();

		params->append(key, val);
	}

	TraderSpi* trader = new TraderSpi;
	trader->init(params, module.c_str());
	trader->run();
	params->release();

	{
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.wait(lock);
	}
	
	while(true)
	{
		printf("请选择操作\r\n");
		printf("1、查询资金\r\n");
		printf("2、查询订单\r\n");
		printf("3、查询成交\r\n");
		printf("4、查询持仓\r\n");
		printf("5、查询结算单\r\n");
		printf("6、限价下单\r\n");
		printf("7、市价下单\r\n");
		printf("8、撤单\r\n");
		printf("0、退出\r\n");

		char cmd;
		for (;;)
		{
			scanf("%c", &cmd);

			if(cmd >= '0' && cmd <= '8')
				break;
		}

		bool bSucc = false;
		switch (cmd)
		{
		case '1':
			bSucc = trader->qryFund();
			break;
		case '2': 
			bSucc = trader->qryOrders();
			break;
		case '3': 
			bSucc = trader->qryTrades();
			break;
		case '4': 
			bSucc = trader->qryPosition();
			break;
		case '5': 
			bSucc = trader->qrySettle();
			break;
		case '6': 
			bSucc = trader->entrustLmt();
			break;
		case '7': 
			bSucc = trader->entrustMkt();
			break;
		case '8': 
			bSucc = trader->cancel();
			break;
		case '0': break;
		default:
			cmd = 'X';
			break;
		}

		if (cmd != '0' && cmd != 'X' && bSucc)
		{
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.wait(lock);
		}
		else if(cmd == '0')
			break;
	}
	
	//exit(9);
	trader->release();
	delete trader;
}