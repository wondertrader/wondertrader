from pandas import DataFrame as df
import pandas as pd
import numpy as np
import math
import xlsxwriter

def fmtNAN(val, defVal = 0):
    if math.isnan(val):
        return defVal

    return val

class WtBtAnalyst:

    def __init__(self):
        self.__strategies__ = dict()
        return


    def add_strategy(self,  sname:str, folder:str, init_capital:float, rf:float=0.02, annual_trading_days:int = 240):
        self.__strategies__[sname] = {
            "folder": folder,
            "cap":init_capital,
            "rf":rf,
            "atd":annual_trading_days
        }


    def run(self):
        if len(self.__strategies__.keys()) == 0:
            raise Exception("strategies is empty")

        for sname in self.__strategies__:
            sInfo = self.__strategies__[sname]
            folder = sInfo["folder"]
            print("start PnL analyzing for strategy %s……" % (sname))

            df_funds = pd.read_csv(folder + "funds.csv")
            print("fund logs loaded……")
            # df_closes = pd.read_csv(folder + "closes.csv")
            # df_signals = pd.read_csv(folder + "signals.csv")
            # df_trades = pd.read_csv(folder + "trades.csv")

            days = len(df_funds)
            init_capital = sInfo["cap"]
            annual_days = sInfo["atd"]
            rf = sInfo["rf"]

            #先做资金统计吧
            print("anayzing fund data……")
            df_funds["dynbalance"] += init_capital
            ayBal = df_funds["dynbalance"]              # 每日期末动态权益

            #生成每日期初动态权益
            ayPreBal = np.array(ayBal.tolist()[:-1])  
            ayPreBal = np.insert(ayPreBal, 0, init_capital)    #每日期初权益
            df_funds["prebalance"] = ayPreBal

            #统计期末权益大于期初权益的天数，即盈利天数
            windays = len(df_funds[df_funds["dynbalance"]>df_funds["prebalance"]])

            #每日净值
            ayNetVals = (ayBal/init_capital)
            
            ar = math.pow(ayNetVals.iloc[-1], annual_days/days) - 1       #年化收益率=总收益率^(年交易日天数/统计天数)
            ayDailyReturn = ayBal/ayPreBal-1 #每日收益率
            delta = fmtNAN(ayDailyReturn.std(axis=0)*math.pow(annual_days,0.5),0)       #年化标准差=每日收益率标准差*根号下(年交易日天数)
            down_delta = fmtNAN(ayDailyReturn[ayDailyReturn<0].std(axis=0)*math.pow(annual_days,0.5), 0)    #下行标准差=每日亏损收益率标准差*根号下(年交易日天数)

            #sharpe率
            if delta != 0.0:
                sr = (ar-rf)/delta
            else:
                sr = 9999.0

            #计算最大回撤和最大上涨
            maxub = ayNetVals[0]
            minub = maxub
            mdd = 0.0
            midd = 0.0
            mup = 0.0
            for idx in range(1,len(ayNetVals)):
                maxub = max(maxub, ayNetVals[idx])
                minub = min(minub, ayNetVals[idx])
                profit = (ayNetVals[idx] - ayNetVals[idx-1])/ayNetVals[idx-1]
                falldown = (ayNetVals[idx] - maxub)/maxub
                riseup = (ayNetVals[idx] - minub)/minub
                if profit <= 0:
                    midd = max(midd, abs(profit))
                    mdd = max(mdd, abs(falldown))
                else:
                    mup = max(mup, abs(riseup))
            #索提诺比率
            if down_delta != 0.0:
                sortino = (ar-rf)/down_delta
            else:
                sortino = 0.0
            if mdd != 0.0:
                calmar = ar/mdd
            else:
                calmar = 999999.0

            #输出到excel
            print("outputting results to excel……")
            workbook = xlsxwriter.Workbook('Strategy[%s]_PnLAnalyzing_%s_%s.xlsx' % (sname, df_funds['date'][0], df_funds['date'].iloc[-1]))
            sheetName = '策略绩效概览'
            worksheet = workbook.add_worksheet(sheetName)

            #   设置合并单元格及格式   #
            # ~~~~~~ 写入数据 ~~~~~~ #
            title_format = workbook.add_format({
                'bold':     True,
                'border': 1,
                'align':    'center',   # 水平居中
                'valign':   'vcenter',  # 垂直居中
                'fg_color': '#bcbcbc'
            })
            fund_data_format = workbook.add_format({
                'border': 1,
                'align':    'right',    # 右对齐
                'valign':   'vcenter',  # 垂直居中
            })
                
            fund_data_format_2 = workbook.add_format({
                'border': 1,
                'align':    'right',    # 右对齐
                'valign':   'vcenter',  # 垂直居中
                'num_format': '0.00'
            })

            fund_data_format_3 = workbook.add_format({
                'border': 1,
                'align':    'right',    # 右对齐
                'valign':   'vcenter',  # 垂直居中
                'num_format': '0.000'
            })

            fund_data_format_4 = workbook.add_format({
                'border': 1,
                'align':    'right',    # 右对齐
                'valign':   'vcenter',  # 垂直居中
                'num_format': '0.0000'
            })

            headings = ['日期', '累计净值']
            worksheet.write_row('A1', headings, title_format)
            worksheet.write_column('A2', df_funds['date'], fund_data_format)
            worksheet.write_column('B2', ayNetVals, fund_data_format_4)

            merge_format = workbook.add_format({
                'font_size': 16,
                'bold':     True,
                'align':    'center',   # 水平居中
                'valign':   'vcenter',  # 垂直居中
            })
            indicator_format = workbook.add_format({
                'font_size': 12,
                'bold':     True,
                'align':    'center',   # 水平居中
                'valign':   'vcenter',  # 垂直居中
            })
            worksheet.merge_range('F1:I1', '收益率统计指标', merge_format)
            worksheet.merge_range('J1:M1', '风险统计指标', merge_format)
            worksheet.merge_range('N1:P1', '综合指标', merge_format)

            key_indicator = ['交易天数', '累积收益（%）', '年化收益率（%）', '胜率（%）', '最大回撤（%）', '最大上涨（%）', '标准差（%）',
                 '下行波动率（%）', 'Sharpe比率', 'Sortino比率', 'Calmar比率']
            key_data = [(ayNetVals.iloc[-1]-1)*100, ar*100, (windays/days)*100, mdd*100, mup*100, delta*100, down_delta*100, sr, sortino, calmar]
            worksheet.write_row('F2', key_indicator, indicator_format)
            worksheet.write_column('F3', [days], fund_data_format)
            worksheet.write_row('G3', key_data, fund_data_format_3)

            #   画图   #
            chart_col = workbook.add_chart({'type': 'line'})
            length = days
            chart_col.add_series(                                   # 给图表设置格式，填充内容
                {
                    'name': '=%s!$B$1' % (sheetName),
                    'categories': '=%s!$A$2:$A$%s' % (sheetName, length+1),
                    'values':   '=%s!$B$2:$B$%s' % (sheetName, length+1),
                    'line': {'color': 'blue'},
                }
            )
            chart_col.set_title({'name': '累计净值'})
            worksheet.insert_chart('F8', chart_col)

            #  准备第二张表格  #
            sheetName = '策略绩效分析'
            worksheet = workbook.add_worksheet(sheetName)
            title_format2 = workbook.add_format({
                'border': 1,
                'align':    'center',   # 水平居中
                'valign':   'vcenter',  # 垂直居中
                'fg_color': '#D3D3D3',
                'text_wrap': 1
            })
            worksheet.merge_range('A1:A2', '日期', title_format2)
            worksheet.merge_range('B1:B2', '统计时间', title_format2)
            worksheet.merge_range('C1:C2', '初始资金', title_format2)
            worksheet.merge_range('D1:D2', '出入金', title_format2)
            worksheet.merge_range('E1:E2', '当前权益', title_format2)
            worksheet.merge_range('F1:F2', '累计盈亏', title_format2)
            worksheet.merge_range('G1:G2', '累计净值', title_format2)
            worksheet.merge_range('H1:I1', '当日盈亏', title_format2)
            indicator = ['数值', '比例']
            worksheet.write_row('H2', indicator, title_format2)
            worksheet.merge_range('J1:J2', '峰值', title_format2)
            worksheet.merge_range('K1:K2', '当日累计回撤', title_format2)
            worksheet.merge_range('L1:L2', '历史最大累计回撤', title_format2)
            worksheet.merge_range('M1:M2', '最大单日回撤', title_format2)
            worksheet.merge_range('N1:N2', '衰落时间', title_format2)
            # worksheet.merge_range('O1:O2', 'IF指数', title_format2)
            # worksheet.merge_range('P1:P2', 'IF净值', title_format2)

            #  写入内容  #
            profit_format = workbook.add_format({
                'border': 1,
                'align':    'right',    # 靠右
                'valign':   'vcenter',  # 垂直居中
                'fg_color': '#FAFAD2',
                'num_format': '0.00'
            })

            percent_format = workbook.add_format({
                'border': 1,
                'align':    'right',    # 右对齐
                'valign':   'vcenter',  # 垂直居中
                'num_format': '0.00%'
            })

            worksheet.write_column('A3', df_funds['date'], fund_data_format)
            worksheet.write_column('B3', range(len(df_funds)), fund_data_format)
            initial = [init_capital]*len(df_funds)
            worksheet.write_column('C3', initial, fund_data_format)
            worksheet.write_column('D3', '/', fund_data_format)
            worksheet.write_column('E3', ayBal, fund_data_format)
            worksheet.write_column('F3', ayBal-init_capital, fund_data_format_2)
            worksheet.write_column('G3', ayNetVals, fund_data_format_4)
            worksheet.write_column('H3', ayBal-ayPreBal, profit_format)
            worksheet.write_column('I3', ayDailyReturn, percent_format)
            #  计算峰值
            upper = np.maximum.accumulate(ayNetVals)
            worksheet.write_column('J3', upper, fund_data_format_4)
            #  回撤指标
            temp = 1-(ayNetVals)/(np.maximum.accumulate(ayNetVals))
            worksheet.write_column('K3', temp, percent_format)
            worksheet.write_column('L3', np.maximum.accumulate(temp), percent_format)
            worksheet.write_column('M3', np.minimum.accumulate(ayDailyReturn), percent_format)
            #  计算衰落时间
            down_time = [0]
            for i in range(1, len(upper)):
                if upper[i] > upper[i-1]:
                    down_time.append(0)
                else:
                    l = down_time[i-1]
                    down_time.append(l+1)
            worksheet.write_column('N3', down_time, fund_data_format)
            #  输出IF指数值和IF净值###
            # worksheet.write_column('O3', data_funds['close'], fund_data_format_2)
            # net = data_funds['close']/data_funds['close'][0]
            # worksheet.write_column('P3', net, fund_data_format_4)
            #  画图设定#
            chart_col = workbook.add_chart({'type': 'line'})
            #  配置第一个系列
            chart_col.add_series({
                'name': '=%s!$G$1' % (sheetName),
                'categories': '=%s!$A$3:$A$%s' % (sheetName, length+2),
                'values':   '=%s!$G$3:$G$%s' % (sheetName, length+2),
                'line': {'color': 'blue'},
            })
            #  配置第二个系列
            # chart_col.add_series({
            #     'name': '=组合盘绩效统计!$P$1',
            #     'categories':  '=组合盘绩效统计!$A$3:$A$%s' % ((length+2)),
            #     'values':   '=组合盘绩效统计!$P$3:$P$%s' % ((length+2)),
            #     'line': {'color': 'red'},
            # })    
            chart_col.set_title({'name': '策略净值图'})
            worksheet.insert_chart('B16', chart_col)
            workbook.close()

            print("PnL analyzing of strategy %s done" % (sname))

            

        