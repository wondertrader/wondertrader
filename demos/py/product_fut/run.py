from wtpy import WtEngine
from Strategies.DualThrust import StraDualThrust

if __name__ == "__main__":
    #创建一个运行环境，并加入策略
    env = WtEngine()
    env.init('.\\Common\\', "config.json")
    
    straInfo = StraDualThrust(name='pydt_IF', code="CFFEX.IF.HOT", barCnt=50, period="m5", days=30, k1=0.1, k2=0.1, isForStk=False)
    env.add_strategy(straInfo)
    
    env.run()

    kw = input('press any key to exit\n')