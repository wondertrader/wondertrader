#include <iostream>

#include "../Includes/ITraderApi.h"
#include "../Includes/WTSVariant.hpp"
#include "../Includes/WTSTradeDef.hpp"
#include "../Includes/WTSError.hpp"
#include "../Includes/WTSCollection.hpp"

#include "../Share/TimeUtils.hpp"
#include "../Share/StdUtils.hpp"
#include "../Share/DLLHelper.hpp"
#include "../Share/StrUtil.hpp"
#include "../Share/charconv.hpp"

#include "../WTSTools/WTSBaseDataMgr.h"
#include "../WTSTools/WTSLogger.h"
#include "../WTSUtils/WTSCfgLoader.h"


WTSBaseDataMgr	g_bdMgr;
StdUniqueMutex	g_mtxOpt;
StdCondVariable	g_condOpt;
bool		g_exitNow = false;
bool		g_riskAct = false;
std::set<std::string>	g_blkList;

template<typename... Args>
inline void encoding_print(const char* format, const Args& ...args)
{
	fmt::print(format, args...);
	//printf(fmtutil::format(format, args...));
//#ifdef _MSC_VER
//	printf(UTF8toChar(s).c_str());
//#else
//	printf(s.c_str());
//#endif
}

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
			WTSLogger::info(u8"模块{}加载失败", moduleName);
			return false;
		}

		FuncCreateTrader pFunCreateTrader = (FuncCreateTrader)DLLHelper::get_symbol(hInst, "createTrader");
		if (NULL == pFunCreateTrader)
		{
			WTSLogger::info(u8"入口函数createTrader不存在");
			return false;
		}

		m_pTraderApi = pFunCreateTrader();
		if (NULL == m_pTraderApi)
		{
			WTSLogger::info(u8"交易接口创建失败");
			return false;
		}

		m_funcDelTrader = (FuncDeleteTrader)DLLHelper::get_symbol(hInst, "deleteTrader");
		return true;
	}

	bool qryFund()
	{
		WTSLogger::info(u8"Querying fund info...");
		m_pTraderApi->queryAccount();

		return true;
	}

	bool qryOrders()
	{
		WTSLogger::info(u8"正在查询订单...");
		m_pTraderApi->queryOrders();

		return true;
	}

	bool qryTrades()
	{
		WTSLogger::info(u8"正在查询成交...");
		m_pTraderApi->queryTrades();

		return true;
	}

	bool qryPosition()
	{
		WTSLogger::info(u8"正在查询持仓...");
		m_pTraderApi->queryPositions();

		return true;
	}

	bool qrySettle()
	{
		uint32_t uDate = TimeUtils::getNextDate(TimeUtils::getCurDate(), -1);
		WTSLogger::info(u8"正在查询日期为{}的结算信息...", uDate);
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
			encoding_print(u8"合约代码: ");
			std::cin >> code;

			encoding_print(u8"交易所代码: ");
			std::cin >> exchg;

			encoding_print(u8"委托价格: ");
			std::cin >> price;

			encoding_print(u8"委托数量: ");
			std::cin >> qty;

			if(isNet)
			{
				encoding_print(u8"方向: 0-买, 1-卖: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				encoding_print(u8"合约: {}.{},价格: {},数量: {},方向: {},确认(y/n)? ", exchg, code, price, qty, bs == 0 ? "买" : "卖");
			}
			else
			{
				encoding_print(u8"方向: 0-多, 1-空: ");
				std::cin >> bs;
				if (bs != 0 && bs != 1)
					continue;

				encoding_print(u8"开平: 0-开, 1-平,2-平今: ");
				std::cin >> offset;
				if (offset != 0 && offset != 1 && offset != 2)
					continue;

				encoding_print(u8"合约: {}.{},价格: {},数量: {},方向: {},开盘: {},确认(y/n)? ", exchg, code, price, qty, bs == 0 ? "多" : "空", offset == 0 ? "开" : "平");
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
				WTSLogger::info(u8"{}已被禁止交易", code);
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
		entrust->setUserTag("test");

		if(!isNet)
			WTSLogger::info(u8"[{}]下单中, 代码:{}.{}, 价格:{}, 数量:{}, 操作:{}{}", m_pParams->getCString("user"), exchg, code, price, qty, offset == 0 ? "Open" : "Close", bs == 0 ? "Long" : "Short");
		else
			WTSLogger::info(u8"[{}]下单中, 代码:{}.{}, 价格:{}, 数量:{}, 操作:{}", m_pParams->getCString("user"), exchg, code, price, qty, bs == 0 ? "buy" : "sell");

		entrust->setContractInfo(g_bdMgr.getContract(code, exchg));
		entrust->setUserTag("test_user_tag");
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
			encoding_print(u8"合约代码: ");
			std::cin >> code;

			encoding_print(u8"交易所代码: ");
			std::cin >> exchg;

			encoding_print(u8"委托数量: ");
			std::cin >> qty;

			encoding_print(u8"方向: 0-多, 1-空: ");
			std::cin >> bs;

			encoding_print(u8"开平: 0-开, 1-平: ");
			std::cin >> offset;

			encoding_print(u8"合约: {}.{},数量: {},方向: {},开盘: {},确认(y/n)? ", exchg, code, qty, bs == 0 ? "多" : "空", offset == 0 ? "开" : "平");
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
				WTSLogger::info(u8"{}已被禁止交易", code);
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

		WTSLogger::info(u8"[{}]下单中, 代码:{}.{}, 价格:市价, 数量:{}, 操作:{}{}", m_pParams->getCString("user"), exchg, code, qty, offset == 0 ? "Open" : "Close", bs == 0 ? "Long" : "Short");

		m_pTraderApi->orderInsert(entrust);
		entrust->release();

		return true;
	}

	bool cancel()
	{
		std::string orderid;

		for (;;)
		{
			encoding_print(u8"订单号: ");
			std::cin >> orderid;

			StrUtil::replace(orderid, "\"", "");

			encoding_print(u8"订单号: {},确认撤单(y/n)? ", orderid);
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
			WTSLogger::info(u8"订单不存在，请检查你的订单号或者先查询订单");
			return false;
		}


		WTSLogger::info(u8"[{}]正在撤单 [{}]...", m_pParams->getCString("user"), orderid);
		WTSEntrustAction* action = WTSEntrustAction::create(ordInfo->getCode(), ordInfo->getExchg());
		action->setEntrustID(ordInfo->getEntrustID());
		action->setOrderID(ordInfo->getOrderID());
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
				WTSLogger::info(u8"[{}] 已连接", m_pParams->getCString("user"));
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
			WTSLogger::info(u8"[{}] 登录成功" , m_pParams->getCString("user"));
			m_bLogined = true;
		}
		else
		{
			WTSLogger::info(u8"[{}] 登录失败: {}", m_pParams->getCString("user"), msg);
			g_exitNow = true;
		}

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspEntrust(WTSEntrust* entrust, WTSError *err)
	{
		if(err)
		{
			WTSLogger::info(u8"[{}] 下单失败: {}", m_pParams->getCString("user"), err->getMessage());
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
				WTSLogger::info(u8"[{}] 资金数据已更新，静态权益: {:.2f}", m_pParams->getCString("user"), accInfo->getBalance());
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

		WTSLogger::info(u8"[{}] 持仓数据已更新, 共{}条数据", m_pParams->getCString("user"), cnt);
		for(uint32_t i = 0; i < cnt; i++)
		{
			WTSPositionItem* posItem = (WTSPositionItem*)((WTSArray*)ayPositions)->at(i);
			if(posItem && posItem->getTotalPosition() > 0)
			{
				if(g_riskAct)
				{
					g_blkList.insert(posItem->getCode());
					WTSLogger::info(u8"{}持仓量超限,限制open", posItem->getCode());
				}
				
				WTSLogger::info(u8"持仓{}({})已更新, {}[{}]", posItem->getCode(), posItem->getDirection() == WDT_LONG ? "L" : "S", posItem->getTotalPosition(), posItem->getAvailPosition());
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
			{
				m_mapOrds->add(StrUtil::trim(ordInfo->getOrderID()), ordInfo, true);
				WTSLogger::info(u8"[{}] 未完成单, 代码: {}, 订单号: {}", m_pParams->getCString("user"), ordInfo->getCode(), ordInfo->getOrderID());
			}
		}

		WTSLogger::info(u8"[{}] 订单数据已更新, 共{}条订单, 其中未完成单{}条", m_pParams->getCString("user"), cnt, m_mapOrds->size());

		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspTrades(const WTSArray* ayTrades)
	{
		uint32_t cnt = 0;
		if (ayTrades != NULL)
			cnt = ayTrades->size();

		WTSLogger::info(u8"[{}] 成交数据已更新, 共{}条成交", m_pParams->getCString("user"), cnt);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onRspSettlementInfo(uint32_t uDate, const char* content)
	{
		WTSLogger::info(u8"[{}]{} 收到结算信息", m_pParams->getCString("user"), uDate);
		WTSLogger::info(content);
		StdUniqueLock lock(g_mtxOpt);
		g_condOpt.notify_all();
	}

	virtual void onPushOrder(WTSOrderInfo* orderInfo)
	{
		std::string orderid = StrUtil::trim(orderInfo->getOrderID());
		if(orderInfo->getOrderState() != WOS_Canceled)
		{
			if(!orderid.empty())
			{
				if (m_mapOrds == NULL)
					m_mapOrds = WTSObjectMap::create();

				if (m_mapOrds->find(orderid) == m_mapOrds->end())
				{
					WTSLogger::info(u8"[{}] 下单成功，订单号: {}, 用户标记: {}",  m_pParams->getCString("user"), orderid, orderInfo->getUserTag());
					m_mapOrds->add(orderid, orderInfo, true);
				}

				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
		}
		else 
		{
			if (m_mapOrds)
				m_mapOrds->remove(orderid);

			if (orderid.empty())
			{
				WTSLogger::info(u8"[{}] 订单{}下单失败并撤销: {}, 用户标记: {}", m_pParams->getCString("user"), orderInfo->getEntrustID(), orderInfo->getStateMsg(), orderInfo->getUserTag());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}
			else
			{
				WTSLogger::info(u8"[{}] 订单{}已撤销: {}, 用户标记: {}", m_pParams->getCString("user"), orderid, orderInfo->getStateMsg(), orderInfo->getUserTag());
				StdUniqueLock lock(g_mtxOpt);
				g_condOpt.notify_all();
			}			
		}
	}

	virtual void onPushTrade(WTSTradeInfo* tradeRecord)
	{
		WTSLogger::info(u8"[{}] 收到成交回报，代码:{}, 价格:{}, 数量: {}, 用户标记: {}", 
			m_pParams->getCString("user"), tradeRecord->getCode(), tradeRecord->getPrice(), tradeRecord->getVolume(), tradeRecord->getUserTag());

		if(g_riskAct)
		{
			WTSLogger::info(u8"[{}]{}超过最大持仓Volume,禁止open", m_pParams->getCString("user"), tradeRecord->getCode());

			g_blkList.insert(tradeRecord->getCode());
		}
	}

	virtual void onTraderError(WTSError*	err)
	{
		if(err && err->getErrorCode() == WEC_ORDERCANCEL)
		{
			WTSLogger::info(u8"[{}] 撤单出错: {}", m_pParams->getCString("user"), err->getMessage());
			StdUniqueLock lock(g_mtxOpt);
			g_condOpt.notify_all();
		}
		else if (err && err->getErrorCode() == WEC_ORDERINSERT)
		{
			WTSLogger::info(u8"[{}] 下单出错: {}", m_pParams->getCString("user"), err->getMessage());
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

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

int main()
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif // _WIN32
	WTSLogger::init("logcfg.yaml");

	WTSVariant* root = WTSCfgLoader::load_from_file("config.yaml");
	if(root == NULL)
	{
		WTSLogger::log_raw(LL_ERROR, u8"配置文件config.yaml加载失败");
		return 0;
	}

	WTSVariant* cfg = root->get("config");
	bool isUTF8 = cfg->getBoolean("utf8");
	if(cfg->has("session"))
		g_bdMgr.loadSessions(cfg->getCString("session"));

	if (cfg->has("commodity"))
		g_bdMgr.loadCommodities(cfg->getCString("commodity"));

	if (cfg->has("contract"))
		g_bdMgr.loadContracts(cfg->getCString("contract"));

	if (cfg->has("holiday"))
		g_bdMgr.loadHolidays(cfg->getCString("holiday"));

	g_riskAct = cfg->getBoolean("risk");
	WTSLogger::info("RiskMon: {}", g_riskAct ? "Open" : "Closed");

	std::string module = cfg->getCString("trader");
	std::string profile = cfg->getCString("profile");
	WTSVariant* params = root->get(profile.c_str());
	if(params == NULL)
	{
		WTSLogger::error(u8"配置项{}不存在", profile);
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
	
	while(!g_exitNow)
	{
		encoding_print(u8"请选择一个操作：\r\n");
		encoding_print(u8"1. 查询资金\r\n");
		encoding_print(u8"2. 查询订单\r\n");
		encoding_print(u8"3. 查询成交\r\n");
		encoding_print(u8"4. 查询持仓\r\n");
		encoding_print(u8"5. 查询结算单\r\n");
		encoding_print(u8"6. 限价委托\r\n");
		encoding_print(u8"7. 市价委托\r\n");
		encoding_print(u8"8. 撤单\r\n");
		encoding_print(u8"9. 净头寸交易\r\n");
		encoding_print(u8"0. 退出\r\n");

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