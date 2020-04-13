from wtpy import WtBtEngine
from wtpy.backtest import WtBtAnalyst

from Strategies.DualThrust import StraDualThrust

# from Strategies.XIM import XIM

if __name__ == "__main__":
    #创建一个运行环境，并加入策略
    engine = WtBtEngine()
    engine.init('.\\Common\\', "configbt.json")
    engine.configBacktest(201909100930,201912011500)
    engine.configBTStorage(mode="csv", path=".\\storage\\")
    engine.commitBTConfig()

    straInfo = StraDualThrust(name='pydt_IF', code="CFFEX.IF.HOT", barCnt=50, period="m5", days=30, k1=0.1, k2=0.1, isForStk=False)
    engine.set_strategy(straInfo)

    engine.run_backtest()

    analyst = WtBtAnalyst()
    analyst.add_strategy("pydt_IF", folder="./outputs_bt/pydt_IF/", init_capital=500000, rf=0.02, annual_trading_days=240)
    analyst.run()

    kw = input('press any key to exit\n')
    engine.release_backtest()