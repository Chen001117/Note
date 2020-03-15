# 导入函数库
import jqdata
import pandas as pd
import numpy as np
import math
import talib as tl
from mxnet import autograd, gluon, init, nd
from mxnet.gluon import data as gdata, loss as gloss, nn


# 初始化函数，设定基准等等
def initialize(context):
    # 设定500ETF作为基准
    set_benchmark('600105.XSHG')
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
    
    # 得到每个股票的模型
    getModel() 
    # 开盘前运行
    run_daily(pl_before_market_open, time='before_open', reference_security='000300.XSHG')
    # 交易
    run_daily(pl_trade, time='every_bar',reference_security='000300.XSHG')
    # 收盘后运行
    run_daily(pl_after_market_close, time='after_close', reference_security='000300.XSHG')

#参数放置函数
def getPara():# <---------------------这里更改参数
    tempList = []
    for i in [100]: # 迭代次数
        for j in [0.03,0.07]: # 学习率
            tempList.append([i,j])
    return tempList
        
# 初始化全局变量
def pl_init_global(context):
    # 股票池，500ETF
    g.pl_stock_pool = ['600105.XSHG'] # <---------------------------------在此处增加股票池内股票
    g.model = {}

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
    pl_position = (pl_total - pl_cash)/pl_total * 100
    record(position=pl_position)
    return



def pl_buy(context):
    '''
    买入逻辑
    '''
    pl_current_datas = get_current_data()
    for pl_code in g.pl_stock_pool:
        pl_current_data = pl_current_datas[pl_code]
        if pl_current_data == None:
            continue
        if pl_code in context.portfolio.positions.keys():
            # 已经有持仓，不再判断建仓信号
            continue
        if pl_is_high_limit(pl_code):
            continue

        day = 40 #需要获得的资料量 = 预测天数加月线
        base = 20
        Data = attribute_history(security=pl_code, count=day, unit='1d',fields=['close'],skip_paused=True, df=True, fq='pre')['close'] 
        # 标准化
        mean = pd.Series(Data).rolling(window=base).mean() # 计算月线
        Data = (Data - mean) / mean * 10000 #　标准化
        net = g.model[pl_code]
        try:
            result = net(nd.array([Data[20:]])).argmax(axis=1)
            if (result[0] == 1.0):
                # 计算头寸
                pl_position_value = pl_calc_position(context,pl_code)
                # 买入
                pl_order_ = order_value(security=pl_code, value=pl_position_value)
                if (pl_order_ is not None) and (pl_order_.filled > 0):
                    log.info("交易 买入",pl_code,"预期买入",pl_order_.amount,"实际买入",pl_order_.filled)
        except: 
            print('error')
            continue
    return


def pl_sell(context):
    '''
    卖出逻辑
    '''
    pl_current_datas = get_current_data()
    for pl_code in context.portfolio.positions.keys():
        pl_current_data = pl_current_datas[pl_code]
        if pl_current_data == None:
            continue
        if pl_is_low_limit(pl_code):
            continue
        pl_position = context.portfolio.positions[pl_code]
        if pl_position.closeable_amount <= 0:
            continue

        day = 40 #需要获得的资料量 = 预测天数加月线
        base = 20
        Data = attribute_history(security=pl_code, count=day, unit='1d',fields=['close'],skip_paused=True, df=True, fq='pre')['close'] 
        # 标准化
        mean = pd.Series(Data).rolling(window=base).mean() # 计算月线
        Data = (Data - mean) / mean * 10000 #　标准化
        
        net = g.model[pl_code]
        try:
            result = net(nd.array([Data[20:]])).argmax(axis=1)
            if (result[0] == 0.0):
                # 卖出
                pl_order_ = order_target(security=pl_code, amount=0)
                if (pl_order_ is not None) and (pl_order_.filled > 0):
                    log.info("交易 卖出",pl_code,"卖出数量",pl_order_.filled,"剩余数量",(pl_order_.amount - pl_order_.filled))
        except:
            print('error')
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
    
# 数据获得&预处理函数   
def getData(name,dataSize): 
    # 接取参数
    day = 20 # 要预测的天数
    base = 20 # 月线
    getSize = dataSize # 获取的数据大小
    interval = 10 # 每隔interval天采样一次
    preInter = 1 # 预测时是预测preInter天行情
    # 获得信息
    Data = attribute_history(security=name, count=getSize, unit='1d',fields=['close'],skip_paused=True, df=True, fq='pre')['close'] 
    # 标准化
    mean = pd.Series(Data).rolling(window=base).mean() # 计算月线
    Data = (Data - mean) / mean * 10000 #　标准化
    # 切割
    trainX = []
    trainY = []
    for i in range(base,getSize-day-preInter,interval):
        trainX.append(Data[i:day + i])
        temp1 = Data[day+i-1] # 输入值的最后一天
        temp2 = Data[day+i:day+i+preInter].mean() # 输出天的行情
        if(temp1 < temp2):
            trainY.append(1)
        else:
            trainY.append(0)
    return trainX,trainY
 
# 训练函数  
def train(trainX,trainY,para):
    # 一些参数
    epoch = para[0]
    batchSize = 10
    lr = para[1]
    newTrainX = [[],[],[],[],[]]
    newTrainY = [[],[],[],[],[]]
    # 切割为k = 5份
    for i,x in enumerate(trainX):
        newTrainX[i % 5].append(x)
    for i,y in enumerate(trainY):
        newTrainY[i % 5].append(y)
    # 训练&求准确率
    score = 0.0
    for i in range(5): # k = 5
        # 建构数据
        for j in range(5):
            if(j == i):
                testX = nd.array(newTrainX[j])
                testY = nd.array(newTrainY[j])
            else:
                if trainX is None:
                    trainX = nd.array(newTrainX[j])
                    trainY = nd.array(newTrainY[j])
                else:
                    trainX = nd.concat(nd.array(trainX),nd.array(newTrainX[j]),dim = 0)
                    trainY = nd.concat(nd.array(trainY),nd.array(newTrainY[j]),dim = 0)
        #训练
        net = nn.Sequential() 
        net.add(nn.Dense(64, activation='sigmoid'),nn.Dropout(0.2),nn.Dense(2)) 
        net.initialize(init.Normal(sigma=0.01)) # 初始化原始参数
        train_iter = gdata.DataLoader(gdata.ArrayDataset(trainX, trainY), batchSize, shuffle=True) 
        loss = gloss.SoftmaxCrossEntropyLoss() #交叉熵函数
        trainer = gluon.Trainer(net.collect_params('.*weight'), 'sgd', {'learning_rate': lr, 'wd': 0.2})
        for j in range(epoch):
            for x, y in train_iter:
                with autograd.record():
                    y_hat = net(x)
                    l = loss(y_hat, y).sum()
                l.backward()
                trainer.step(batchSize) # 更改参数
        #算分
        tempScore = (net(testX).argmax(axis=1)==testY.astype('float32')).sum().asscalar()
        tempScore = tempScore / testY.size
        score = score + tempScore
    return net,score / 5
    
# 得到每个股票的模型
def getModel():
    for name in g.pl_stock_pool:
        # 设定参数
        parameter = getPara() # 参数放置函数
        # 高分列表
        tempList = []
        for para in parameter:
            # 信息预处理
            trainX, trainY = getData(name,2000) # 获得&预处理函数
            # 训练&得到分数
            tempModel, tempScore = train(trainX,trainY,para) # 训练函数
            tempList.append([tempModel,tempScore])
        # 选取最高分&回传
        pdTempList = pd.DataFrame(tempList)
        print(pdTempList)
        pdTempList.sort_values(by = 1)
        g.model[name] = pdTempList[0][0]
