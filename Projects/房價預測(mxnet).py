#!/usr/bin/env python
# coding: utf-8

# In[1]:


get_ipython().system('pip install pandas')
import d2lzh as d2l
from mxnet import autograd, gluon, init, nd
from mxnet.gluon import data as gdata, loss as gloss, nn
import numpy as np
import pandas as pd


# In[2]:


trainData = pd.read_csv('C:/code/d2l-zh/test_code/house_price_data/train.csv')
testData = pd.read_csv('C:/code/d2l-zh/test_code/house_price_data/test.csv')
trainData


# In[3]:


features = pd.concat((trainData.iloc[:,1:-1],testData.iloc[:,1:]))


# In[4]:


# 处理数据
t = features.dtypes[features.dtypes != 'object'].index # 取得每行的标头(list形势)
features[t] = features[t].apply(lambda x: (x - x.mean()) / (x.std())) # 标准化
features[t] = features[t].fillna(0) # 默认值为零
features = pd.get_dummies(features, dummy_na=True) # 将字符型变数转为0/1(如:alpha下面有A&B->创建两列alpha_A,alpha_B底下的值是0/1),na也算一类型
features.shape # 变多列了
# 转为Nd.array
trainSize = trainData.shape[0]
Xtrain = nd.array(features[:trainSize].values)
Xtest = nd.array(features[trainSize:].values)
Ytrain = nd.array(trainData.SalePrice.values).reshape(-1,1)


# In[5]:


loss = gloss.L2Loss()
def newNet():
    net = nn.Sequential()
    net.add(nn.Dense(1))
    net.initialize()
    return net


# ![%E6%93%B7%E5%8F%96.JPG](attachment:%E6%93%B7%E5%8F%96.JPG)

# In[6]:


def log_rmse(net, features, labels):  # 评价函数
    clipped_preds = nd.clip(net(features), 1, float('inf'))  # 将⼩于1的值设成1，使得取对数时数值更稳定
    rmse = nd.sqrt(2 * loss(clipped_preds.log(), labels.log()).mean())
    return rmse.asscalar()


# In[7]:


def train(net, train_features, train_labels, test_features, test_labels,num_epochs, learning_rate, weight_decay, batch_size):
    train_ls, test_ls = [], []
    train_iter = gdata.DataLoader(gdata.ArrayDataset(train_features, train_labels), batch_size, shuffle=True) 
    trainer = gluon.Trainer(net.collect_params(), 'adam', {'learning_rate': learning_rate, 'wd': weight_decay}) # 这⾥使⽤了Adam优化算法
    for epoch in range(num_epochs):
        for X, y in train_iter:
            with autograd.record():
                l = loss(net(X), y)
            l.backward()
            trainer.step(batch_size)
    train_ls.append(log_rmse(net, train_features, train_labels))
    if test_labels is not None:
        test_ls.append(log_rmse(net, test_features, test_labels))
    return train_ls, test_ls


# In[8]:


# 切分训练集与交叉验证集
def get_k_fold_data(k, i, X, y):
    assert k > 1
    fold_size = X.shape[0] // k 
    X_train, y_train = None, None
    for j in range(k):
        X_part, y_part = X[j * fold_size : (j + 1) * fold_size, :], y[j * fold_size : (j + 1) * fold_size]
        if j == i:
            X_valid, y_valid = X_part, y_part
        elif X_train is None:
            X_train, y_train = X_part, y_part
        else:
            X_train = nd.concat(X_train, X_part, dim=0)
            y_train = nd.concat(y_train, y_part, dim=0)
    return X_train, y_train, X_valid, y_valid


# In[9]:


def k_fold(k, X_train, y_train, num_epochs,learning_rate, weight_decay, batch_size):
    train_l_sum, valid_l_sum = 0, 0
    for i in range(k):
        Xtrain,Ytrain,Xtest,Ytest = get_k_fold_data(k,i,X_train,y_train)
        trainResult,testResult = train(newNet(), Xtrain, Ytrain, Xtest, Ytest,num_epochs, learning_rate, weight_decay, batch_size)
        train_l_sum += trainResult[-1]
        valid_l_sum += testResult[-1]
        #print('fold %d, train rmse %f, valid rmse %f' % (i, trainResult[-1], testResult[-1]))
    return train_l_sum / k, valid_l_sum / k


# In[10]:


temp = []
for i in [80,90,100,110,120]:
    for j in  [0,0.025,0.05]:
        for g in  [45,55,65,75]:
            for h in [15,20,25,30]:
                k = 5
                epochs = i # 100
                weight_decay = j # 0-0.05 
                batch_size = g # 50~80 (64)
                lr = h # 5~50 (20)
                train_l, valid_l = k_fold(k, Xtrain, Ytrain, epochs, lr,weight_decay, batch_size)
                print('lr',lr,'epochs',epochs,'weight_decay',weight_decay,'batchSize',batch_size,'%d-fold validation: avg train rmse %f, avg valid rmse %f' % (k, train_l, valid_l))
                temp.append([i,j,g,h,valid_l])


# In[20]:


result = pd.DataFrame(temp,columns = ['epoch','decay','batch','lr','score'])
result.sort_values(by = 'score').head(20)


# In[23]:


# 导出
def train_and_pred(Xtrain, Xtest, Ytrain, testData,num_epochs, lr, weight_decay, batch_size):
    net = newNet()
    train_ls, _ = train(net, Xtrain, Ytrain, None, None,num_epochs, lr, weight_decay, batch_size)
    pre = net(Xtest).asnumpy()
    testData['SalePrice'] = pd.Series(pre.reshape(1,-1)[0])
    sub = pd.concat([testData['Id'], testData['SalePrice']],axis = 1)
    sub.to_csv('result.csv',index = False)
train_and_pred(Xtrain, Xtest, Ytrain, testData,100, 15, 0, 45)


# In[ ]:


print(testData['Id'].shape)


# In[ ]:




