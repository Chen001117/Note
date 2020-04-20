# 导入函数库
import jqdata
import pandas as pd
import numpy as np
import math
import talib as tl
from sklearn.tree import DecisionTreeClassifier



# 初始化函数，设定基准等等
def initialize(context):
    # 设定500ETF作为基准
    set_benchmark('600030.XSHG')
    # 开启动态复权模式(真实价格)
    set_option('use_real_price', True)
    # 输出内容到日志 log.info()
    #log.info('初始函数开始运行且全局只运行一次')
    # 过滤掉order系列API产生的比error级别低的log
    log.set_level('order', 'error')

    ### 股票相关设定 ###
    # 设定滑点为0
    set_slippage(FixedSlippage(0))
    # ETF类每笔交易时的手续费是：买入时佣金万分之三，卖出时佣金万分之三，没有印花税, 每笔交易佣金最低扣5块钱
    set_order_cost(OrderCost(close_tax=0, open_commission=0.0003, close_commission=0.0003, min_commission=5), type='fund')

    
    pl_init_global(context)
    # 开盘前运行
    run_daily(pl_before_market_open, time='before_open', reference_security='000300.XSHG')
    # 交易
    run_daily(pl_trade, time='every_bar',reference_security='000300.XSHG')
    # 收盘后运行
    run_daily(pl_after_market_close, time='after_close', reference_security='000300.XSHG')
    

# 初始化全局变量
def pl_init_global(context):
    # 股票池，500ETF
    g.pl_stock_pool = ['600030.XSHG']
    # 获得历史信息
    g.SVM = []
    for t,name in enumerate(g.pl_stock_pool):
        Data = get_price(name, start_date='2005-01-01', end_date='2018-12-31', frequency='daily', fields=None, skip_paused=False, fq='pre', panel=True)
        newData = Data['close']
        # 数据分类
        interval = 5 # 预测后interval天行情
        maxi = 240 # jk中最大元素
        X_train = []
        Y_train = []
        for i in range (maxi, newData.shape[0] - interval):
            temp = []
            for j in [5,10,20,60,120]:
                for k in [5,10,20,60,120,240]:
                    if(j < k):
                        temp.append(pl_cross(min(j,k),max(j,k),newData[:i]))
            X_train.append(temp)
            if(newData[i-1] < newData[i : i+interval].mean() ):
                Y_train.append(1)
            elif(newData[i-1] > newData[i : i+interval].mean()):
                Y_train.append(-1)
            else:
                Y_train.append(0)
        # 训练
        g.SVM.append( DecisionTreeClassifier(max_depth = 5) )
        g.SVM[t].fit(X_train,Y_train)

## 开盘前运行函数
def pl_before_market_open(context):
    pass


def pl_trade(context):
    pl_sell(context)
    pl_buy(context)   # 建仓

## 收盘后运行函数
def pl_after_market_close(context):
    # 绘制当天的仓位
    pl_cash = context.portfolio.available_cash
    pl_total = context.portfolio.total_value
    if(pl_total == 0):
        pl_total = 0.01
    pl_position = (pl_total - pl_cash) / pl_total * 100
    record(position=pl_position)
    return


def pl_buy(context):
    '''
    买入逻辑
    '''
    pl_current_datas = get_current_data()
    for i,name in enumerate (g.pl_stock_pool):
        pl_current_data = pl_current_datas[name]
        if pl_current_data == None:
            continue
        if name in context.portfolio.positions.keys():
            # 已经有持仓，不再判断建仓信号
            continue
        if pl_is_high_limit(name):
            continue

        day = 240 #jk中最大元素
        Data = attribute_history(security=name, count=day, unit='1d',fields=['close'],skip_paused=True, df=True, fq='pre')['close']
        temp = []
        for j in [5,10,20,60,120]:
            for k in [5,10,20,60,120,240]:
                if(j < k):
                    temp.append(pl_cross(min(j,k),max(j,k),Data))
        try:
            if ( int(g.SVM[i].predict([temp])) == 1):
                # 计算头寸
                pl_position_value = pl_calc_position(context,name)
                # 买入
                pl_order_ = order_value(security=name, value=pl_position_value)
                if (pl_order_ is not None) and (pl_order_.filled > 0):
                    log.info("交易 买入",name,"预期买入",pl_order_.amount,"实际买入",pl_order_.filled)
        except:
            print("error")
            continue
    return


def pl_sell(context):
    '''
    卖出逻辑
    '''
    pl_current_datas = get_current_data()
    for name in context.portfolio.positions.keys():
        
        pl_current_data = pl_current_datas[name]
        if pl_current_data == None:
            continue
        if pl_is_low_limit(name):
            continue
        pl_position = context.portfolio.positions[name]
        if pl_position.closeable_amount <= 0:
            continue

        day = 240 # j,k中最大元素
        index = -1 # SVM的编号
        for i,tempName in enumerate (g.pl_stock_pool):
            if(name == tempName):
                index = i
        Data = attribute_history(security=name, count=day, unit='1d',fields=['close'],skip_paused=True, df=True, fq='pre')['close']
        temp = []
        for j in [5,10,20,60,120]:
            for k in [5,10,20,60,120,240]:
                if(j < k):
                    temp.append(pl_cross(min(j,k),max(j,k),Data))
        try:
            if (int(g.SVM[index].predict([temp])) == -1):
                # 卖出
                pl_order_ = order_target(security=name, amount=0)
                if (pl_order_ is not None) and (pl_order_.filled > 0):
                    log.info("交易 卖出",name,"卖出数量",pl_order_.filled,"剩余数量",(pl_order_.amount - pl_order_.filled))
        except:
            print("error")
            continue    
    return


def pl_calc_position(context,pl_code):
    '''
    计算仓位。

    全仓买入
    '''
    return context.portfolio.available_cash


def pl_is_high_limit(pl_code):
    '''
    判断标的是否已经涨停或停牌

    Args:
        pl_code 标的的代码。如要检测平安银行需要传入参数 000001.XSHE
    Returns:
        True 表示要检测的标的涨停或停牌，这时不能进行买入操作
        False 表示要检测的标的没有涨停或停牌，可以进行买入操作
    '''
    pl_current_data = get_current_data()[pl_code]
    if pl_current_data.last_price >= pl_current_data.high_limit:
        return True
    if pl_current_data.paused:
        return True
    return False



def pl_is_low_limit(pl_code):
    '''
    判断标的是否已经跌停或停牌

    Args:
        pl_code 标的的代码。
    Returns:
        True 表示要检测的标的涨停或停牌，这时不能进行卖出操作
        False 表示要检测的标的没有涨停或停牌，可以进行卖出操作
    '''
    pl_current_data = get_current_data()[pl_code]
    if pl_current_data.last_price <= pl_current_data.low_limit:
        return True
    if pl_current_data.paused:
        return True
    return False
    
def pl_cross(shortNum, longNum, Data):
    DataPd = pd.Series(Data)
    pl_series1 = DataPd.rolling(window=shortNum).mean()
    pl_series2  = DataPd.rolling(window=longNum).mean()
    '''
    判断 pl_series1 和 pl_series2 的交叉情况

    Args:
        pl_series1 系列1 最少包含3个数据
        pl_series2 系列2 最少包含3个数据
    Returns:
        1  pl_series1 上穿 pl_series2
        0  pl_series1 没有和 pl_series2 发生交叉
        -1 pl_series1 下穿 pl_series2
    '''
    if pl_series1[-1] > pl_series2[-1]:
        if pl_series1[-2] < pl_series2[-2]:
            return 1
        elif pl_series1[-2] == pl_series2[-2]:
            if pl_series1[-3] < pl_series2[-3]:
                return 1

    elif pl_series1[-1] < pl_series2[-1]:
        if pl_series1[-2] > pl_series2[-2]:
            return -1
        elif pl_series1[-2] == pl_series2[-2]:
            if pl_series1[-3] > pl_series2[-3]:
                return -1
    return 0


    
