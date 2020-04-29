from wtpy import WtBtEngine

if __name__ == "__main__":
    #创建一个运行环境，并加入策略
    env = WtBtEngine()
    
    '''
    WonderTrader需要的基础数据周期，只有1分钟、5分钟和日线
    其他周期都是在这基础上拼接出来的
    3分钟线，不能被5整除，所以用1分钟线拼接
    15分钟线，可以被5整除，所以用5分钟线拼接
    '''
    # 将从MC导出的CSV格式的5分钟线，放到./csv_m5/下，然后批量转换
    env.trans_mc_bars("./csv_m5/","./bin_m5/", "m5")

    # 将从MC导出的CSV格式的日线线，放到./csv_d/下，然后批量转换
    env.trans_mc_bars("./csv_d/","./bin_d/", "d")
    
    kw = input('press any key to exit\n')