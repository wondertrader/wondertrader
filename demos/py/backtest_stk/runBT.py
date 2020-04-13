from wtpy import WtBtEngine
from Strategies.DualThrust import StraDualThrust
from wtpy.backtest import WtBtAnalyst

if __name__ == "__main__":
    #创建一个运行环境，并加入策略
    engine = WtBtEngine()
    engine.init(folder='./common/', cfgfile="configbt.json", commfile="stk_comms.json", contractfile="stocks.json")
    engine.configBacktest(201901010930,201912151500)
    engine.configBTStorage(mode="csv", path="./storage/")
    engine.commitBTConfig()
    
    straInfo = StraDualThrust(name='pydt_SH600000', code="SSE.600000", barCnt=50, period="d1", days=30, k1=0.1, k2=0.1, isForStk=True)
    engine.set_strategy(straInfo)

    engine.run_backtest()

    #绩效分析
    analyst = WtBtAnalyst()
    analyst.add_strategy("pydt_SH600000", folder="./outputs_bt/pydt_SH600000/", init_capital=5000, rf=0.02, annual_trading_days=240)
    analyst.run()

    kw = input('press any key to exit\n')
    engine.release_backtest()