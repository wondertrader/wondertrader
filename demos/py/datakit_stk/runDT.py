from wtpy import WtDtEngine

if __name__ == "__main__":
    #创建一个运行环境，并加入策略
    env = WtDtEngine()
    env.initialize()
    
    env.run()

    kw = input('press any key to exit\n')