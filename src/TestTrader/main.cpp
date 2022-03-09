#include <iostream>
#include <boost/filesystem.hpp>

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSCollection.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"


WTSBaseDataMgr	g_bdMgr;
StdUniqueMutex	g_mtxOpt;
StdCondVariable	g_condOpt;
bool		g_exitNow = false;
bool		g_riskAct = false;
std::set<std::string>	g_blkList;

USING_NS_WTP;

class TraderSpi : public ITraderSpi
{
public:
	TraderSpi() :m_bLogined(false), m_mapOrds(NULL){}

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

		m_pTraderApi->connect();
	}

	bool createTrader(const char* moduleName)
	{
		DllHandle hInst = DLLHelper::load_library(moduleName);
		if (hInst == NULL)
		{
			WTSLogger::info("Loading module %s failed", moduleName);
			return false;
		}

		FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
		if (NULL == pFunCreateTrader)
		{
			WTSLogger::info("Entry function createTrader not exists");
			return false;
		}

		m_pTraderApi = pFunCreateTrader();
		if (NULL == m_pTraderApi)
		{
			WTSLogger::info("Creating trader api failed");
			return false;
		}

		m_funcDelTrader = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");
		return true;
	}

	bool qryFund()
	{
		WTSLogger::info("Querying fund info...");
		m_pTraderApi->queryAccount();

		return true;
	}

	bool qryOrders()
	{
		WTSLogger::info("Querying orders...");
		m_pTraderApi->queryOrders();

		return true;
	}

	bool qryTrades()
	{
		WTSLogger::info("Querying trades...");
		m_pTraderApi->queryTrades();

		return true;
	}

	bool qryPosition()
	{
		WTSLogger::info("Querying positions...");
		m_pTraderApi->queryPositions();

		return true;
	}

	bool qrySettle()
	{
		uint32_t uDate = TimeUtils::getNextDate(TimeUtils::getCurDate(), -1);
		WTSLogger::info("Querying settlement info on %u...", uDate);
		m_pTraderApi->querySettlement(uDate);

		return true;
	}

	bool entrustLmt(bool isNet)
	{
		char code[32] = { 0 };
		char exchg[32] = { 0 };
		double price = 0.0;
		double qty = 0;
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

			if(isNet)
			{
				printf("请输入方向,0-买入,1-卖出: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				printf("品种: %s.%s,价格: %f,数量: %f,方向: %s,确认y/n? ", exchg, code, price, qty, bs == 0 ? "买入" : "卖出");
			}
			else
			{
				printf("请输入方向,0-做多,1-做空: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				printf("请输入开平,0-开仓,1-平仓: ");
				std::cin >> offset;
				if (offset != 0 && offset != 1)
					continue;

				printf("品种: %s.%s,价格: %f,数量: %f,方向: %s,开平: %s,确认y/n? ", exchg, code, price, qty, bs == 0 ? "做多" : "做空", offset == 0 ? "开仓" : "平仓");
			}
			
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
				WTSLogger::info("%s已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);

		WTSEntrust* entrust = WTSEntrust::create(code, qty, price, exchg);
		if(!isNet)
		{
			entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
			entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday ? WOT_CLOSETODAY : WOT_CLOSE));
		}
		else
		{
			entrust->setNetDirection(bs == 0);
		}
		
		entrust->setPriceType(WPT_LIMITPRICE);
		entrust->setOrderFlag(WOF_NOR);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		if(!isNet)
			WTSLogger::info("[%s]开始下单,品种: %s.%s,价格: %f,数量: %f,动作: %s%s", m_pParams->getCString("user"), exchg, code, price, qty, offset == 0 ? "开" : "平", bs == 0 ? "多" : "空");
		else
			WTSLogger::info("[%s]开始下单,品种: %s.%s,价格: %f,数量: %f,动作: %s", m_pParams->getCString("user"), exchg, code, price, qty, bs == 0 ? "买入" : "卖出");

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
				WTSLogger::info("%s已被禁止交易", code);
				return false;
			}
		}

		bool bNeedToday = (strcmp(exchg, "SHFE") == 0 || strcmp(exchg, "INE") == 0);
		WTSEntrust* entrust = WTSEntrust::create(code, qty, 0, exchg);
		entrust->setDirection(bs == 0 ? WDT_LONG : WDT_SHORT);
		entrust->setOffsetType(offset == 0 ? WOT_OPEN : (bNeedToday ? WOT_CLOSETODAY : WOT_CLOSE));
		entrust->setPriceType(WPT_ANYPRICE);
		entrust->setOrderFlag(WOF_NOR);

		char entrustid[64] = { 0 };
		m_pTraderApi->makeEntrustID(entrustid, 64);
		entrust->setEntrustID(entrustid);

		WTSLogger::info("[%s]开始下单,品种: %s.%s,价格: 市价,数量: %d,动作: %s%s", m_pParams->getCString("user"), exchg, code, qty, offset == 0 ? "开" : "平", bs == 0 ? "多" : "空");

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

			printf("订单ID: %s,确认y\n? ", orderid);
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
			WTSLogger::info("订单不存在,请检查订单号是否有误,或者先查询订单");
			return false;
		}


		WTSLogger::info("[%s]开始撤单[%s]...", m_pParams->getCString("user"), orderid);
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
				WTSLogger::info("[%s]连接成功", m_pParams->getCString("user"));
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

	virtual void handleTraderLog(WTSLogLevel ll, const char* message) override
	{
		WTSLogger::log_raw(ll, message);
	}

	virtual void onLoginResult(bool bSucc, const char* msg, uint32_t tradingdate)
	{
		if(bSucc)
		{
			WTSLogger::info("[%s]登录成功" , m_pParams->getCString("user"));	
			m_bLogined = true;
		}
		else
		{
			WTSLogger::info("[%s]登录失败: %s", m_pParams->getCString("user"), msg);
			g_exitNow = true;
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err)
	{
		if(err)
		{
			WTSLogger::info("[%s]下单失败: %s", m_pParams->getCString("user"), err->getMessage());
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
				WTSLogger::info("[%s]资金数据更新, 当前静态权益: %.2f", m_pParams->getCString("user"), accInfo->getBalance());
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

		WTSLogger::info("[%s]持仓数据已更新, 当日共有%u笔持仓", m_pParams->getCString("user"), cnt);
		for(uint32_t i = 0; i < cnt; i++)
		{
			WTSPositionItem* posItem = (WTSPositionItem*)((WTSArray*)ayPositions)->at(i);
			if(posItem && posItem->getTotalPosition() > 0 && g_riskAct)
			{
				g_blkList.insert(posItem->getCode());
				WTSLogger::info("%s持仓量超限,限制开仓", posItem->getCode());
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

		WTSLogger::info("[%s]委托列表已更新, 当日共有%u笔委托, 未完成%u笔", m_pParams->getCString("user"), cnt, m_mapOrds->size());

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspTrades(const WTSArray* ayTrades)
	{
		uint32_t cnt = 0;
		if (ayTrades != NULL)
			cnt = ayTrades->size();

		WTSLogger::info("[%s]成交明细已更新, 当日共有%u笔成交", m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspSettlementInfo(uint32_t uDate, const char* content)
	{
		WTSLogger::info("[%s]%u结算单已接收", m_pParams->getCString("user"), uDate);
		WTSLogger::info(content);
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
					WTSLogger::info("[%s]下单成功,订单ID: %s",  m_pParams->getCString("user"), orderInfo->getOrderID());
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
				WTSLogger::info("[%s]订单%s提交失败被撤销:%s", m_pParams->getCString("user"), orderInfo->getEntrustID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
			else
			{
				WTSLogger::info("[%s]订单%s已撤销:%s", m_pParams->getCString("user"), orderInfo->getOrderID(), orderInfo->getStateMsg());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}			
		}
	}

	virtual void onPushTrade(WTSTradeInfo* tradeRecord)
	{
		WTSLogger::info("[%s]收到成交回报,合约%s,成交价: %.4f,成交数量: %.4f", m_pParams->getCString("user"), tradeRecord->getCode(), tradeRecord->getPrice(), tradeRecord->getVolume());

		if(g_riskAct)
		{
			WTSLogger::info("[%s]%s超过最大持仓数量,禁止开仓", m_pParams->getCString("user"), tradeRecord->getCode());

			g_blkList.insert(tradeRecord->getCode());
		}
	}

	virtual void onTraderError(WTSError*	err)
	{
		if(err && err->getErrorCode() == WEC_ORDERCANCEL)
		{
			WTSLogger::info("[%s]撤单失败: %s", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		else if (err && err->getErrorCode() == WEC_ORDERINSERT)
		{
			WTSLogger::info("[%s]下单失败: %s", m_pParams->getCString("user"), err->getMessage());
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
	WTSVariant*			m_pParams;

	typedef WTSHashMap<std::string>	WTSObjectMap;
	WTSObjectMap*		m_mapOrds;

	bool				m_bLogined;
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
	if(root == NULL)
	{
		WTSLogger::log_raw(LL_ERROR, "配置文件config.yaml加载失败");
		return 0;
	}

	WTSVariant* cfg = root->get("config");
	bool isUTF8 = cfg->getBoolean("utf8");
	if(cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"), isUTF8);

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"), isUTF8);

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"), isUTF8);

	if (cfg->has("holiday"))
		g_bdMgr.loadHolidays(cfg->getCString("holiday"));

	g_riskAct = cfg->getBoolean("risk");
	WTSLogger::info("风控开关: %s", g_riskAct ? "开" : "关");

	std::string module = cfg->getCString("trader");
	std::string profile = cfg->getCString("profile");
	WTSVariant* params = root->get(profile.c_str());
	if(params == NULL)
	{
		WTSLogger::error_f("配置项{}不存在", profile);
		return 0;
	}

	TraderSpi* trader = new TraderSpi;
	trader->init(params, module.c_str());
	trader->run();

	root->release();

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
		printf("9、净持仓交易\r\n");
		printf("0、退出\r\n");

		char cmd;
		for (;;)
		{
			scanf("%c", &cmd);

			if(cmd >= '0' && cmd <= '9')
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
			bSucc = trader->entrustLmt(false);
			break;
		case '7': 
			bSucc = trader->entrustMkt();
			break;
		case '8': 
			bSucc = trader->cancel();
			break;
		case '9':
			bSucc = trader->entrustLmt(true);
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
	return 0;
}