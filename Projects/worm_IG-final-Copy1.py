#!/usr/bin/env python
# coding: utf-8

# In[ ]:


get_ipython().run_line_magic('pip', 'install selenium')


# In[ ]:


from selenium import webdriver
from bs4 import BeautifulSoup
import time
from selenium.webdriver.common.action_chains import ActionChains
import pandas as pd


# In[ ]:


driver = webdriver.Chrome()
driver.get("https://www.instagram.com/explore/tags/%E5%8F%B0%E5%8C%97%E7%BE%8E%E9%A3%9F/")
time.sleep(10)
for j in range(4):
    driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
    time.sleep(0.3)
time.sleep(5)
driver.find_element_by_class_name("KPnG0").click()
driver.find_element_by_id("email").send_keys("676767@tp.edu.tw")
driver.find_element_by_id("pass").send_keys("kaokao0584")
driver.find_element_by_id("loginbutton").click()
time.sleep(10)


# In[ ]:


def count(dict,x,tempdata):
    for contest in [tempdata['contest'],tempdata['tag']]:
        for sentence in contest:
            for i in range(len(sentence)-x-1):
                isbreak = 0
                for j in range(x):
                    for k in [' ','#', '(' , ')' , '/', '\'','\"' , '_','-','+','"' , '{' , '}' , '.' , ',' , '!' , '?' , '[' , ']' ,'。' , '，' , ' )', '（', ':', '：', '！','？' , '）','、']:
                        if sentence[i+j] == k or (sentence[i+j] <= 'z' and sentence[i+j] >= 'a') or (sentence[i+j] <= 'Z' and sentence[i+j] >= 'A') or (sentence[i+j] <= '9' and sentence[i+j] >= '0'):
                            isbreak = 1
                            break
                if isbreak == 0:
                    try:
                        dict[sentence[i:i+x]] = dict[sentence[i:i+x]] + 1
                    except:
                        dict[sentence[i:i+x]] = 1


# In[ ]:


def count2(dict,x,tempdata):
    for contest in [tempdata['tag']]:
        for sentence in contest:
            for i in range(len(sentence)-x-1):
                isbreak = 0
                for j in range(x):
                    for k in [' ','#', '(' , ')' , '/', '\'','\"' , '_','-','+','"' , '{' , '}' , '.' , ',' , '!' , '?' , '[' , ']' ,'。' , '，' , ' )', '（', ':', '：', '！','？' , '）','、']:
                        if sentence[i+j] == k or (sentence[i+j] <= 'z' and sentence[i+j] >= 'a') or (sentence[i+j] <= 'Z' and sentence[i+j] >= 'A') or (sentence[i+j] <= '9' and sentence[i+j] >= '0'):
                            isbreak = 1
                            break
                if isbreak == 0:
                    try:
                        dict[sentence[i:i+x]] = dict[sentence[i:i+x]] + 1
                    except:
                        dict[sentence[i:i+x]] = 1


# In[ ]:


def storyProcess(n,data):
    tempList = {}
    html = BeautifulSoup(driver.page_source)
    story = html.find('div',class_ = ['C4VMK'])
    if(story == None):
        tempList['tag'] = []
        tempList["contest"] = []
    else:
        # hashtag
        hashtag = story.find_all('a',class_ = 'xil3i')
        tempTag = []
        for eachTag in hashtag:
            tempTag.append(eachTag.string)
        tempList['tag'] = tempTag
        # 正文
        contest = story.find('h1')
        try: [temp.extract() for temp in contest(['a','span'])]
        except: print('')
        contestList = list(contest)
        finalContest = []
        for x in contestList:
            try:
                _ = x.type
            except:
                if(len(x) > 2):
                    finalContest.append(x)
        tempList["contest"] = finalContest
    # 加入data
    data.loc[n] = tempList


# In[ ]:


def oneWeb(name,data):
    print('enter ' + name)
    #-----------更改疊代次數----------------------------------------------------------------------------------------------------
    infoScale = 100000000
    time.sleep(10)
    driver.find_element_by_class_name("_9AhH0").click()
    storyProcess(0,data)
    second = driver.find_elements_by_css_selector("a[class][tabindex]")[0]
    ActionChains(driver).click(second).perform()
    for i in range(infoScale):
        time.sleep(2.5)
        if(i % 100 == 0): print(i)
        try: storyProcess(i+1,data)
        except: print('')
        try:
            second = driver.find_elements_by_css_selector("a[class][tabindex]")[1]
            ActionChains(driver).click(second).perform()
        except:
            infoScale = i+1
            print('final scale = ' + infoScale)
            break
    dropList = []
    for i in range(infoScale - 1):
        if(data.iloc[i,:]['contest'] == data.iloc[i+1,:]['contest']):
            if(data.iloc[i,:]['tag'] == data.iloc[i+1,:]['tag']):
                dropList.append(i)
    Data = data.drop(dropList)
    path =  'C:\code\python_practice\worm\_' + name + '.csv'
    Data.to_csv(path)
    Data =  pd.read_csv(path)
    dic = {}
    count(dic,2,Data)
    count(dic,3,Data)
    count(dic,4,Data)
    count(dic,5,Data)
    dic = sorted(dic.items(), key = lambda item:item[1],reverse = True)
    dicData = pd.DataFrame(dic[:500])
    path =  'C:\code\python_practice\worm\_' + name + '_dic.csv'
    dicData.to_csv(path)
    dic2 = {}
    count2(dic2,2,Data)
    count2(dic2,3,Data)
    count2(dic2,4,Data)
    count2(dic2,5,Data)
    dic2 = sorted(dic2.items(), key = lambda item:item[1],reverse = True)
    dicData2 = pd.DataFrame(dic2[:100])
    path =  'C:\code\python_practice\worm\_' + name + '_tag_dic.csv'
    dicData2.to_csv(path)


# In[ ]:


time.sleep(5)  
data = pd.DataFrame(columns = ['contest','tag'])
#----------更改名單----------------------------------------------------------------------------
List = ['#花蓮美食'] 
for i,name in enumerate(List):
    driver.find_element_by_class_name("XTCLo").send_keys(name)
    time.sleep(5)
    driver.find_element_by_class_name("z556c").click()
    oneWeb(name,data)
    driver.find_element_by_css_selector("path[d = 'M41.8 9.8L27.5 24l14.2 14.2c.6.6.6 1.5 0 2.1l-1.4 1.4c-.6.6-1.5.6-2.1 0L24 27.5 9.8 41.8c-.6.6-1.5.6-2.1 0l-1.4-1.4c-.6-.6-.6-1.5 0-2.1L20.5 24 6.2 9.8c-.6-.6-.6-1.5 0-2.1l1.4-1.4c.6-.6 1.5-.6 2.1 0L24 20.5 38.3 6.2c.6-.6 1.5-.6 2.1 0l1.4 1.4c.6.6.6 1.6 0 2.2z']").click()
    time.sleep(50)


# In[ ]:


print('start')
name = '花蓮美食'
infoScale = 6407
dropList = []
for i in range(infoScale - 1):
    if(data.iloc[i,:]['contest'] == data.iloc[i+1,:]['contest']):
        print(data.iloc[i,:]['contest'])
        if(data.iloc[i,:]['tag'] == data.iloc[i+1,:]['tag']):
            dropList.append(i)
    elif '營業時間' in data['contest'].iloc[i] or  '地址' in data['contest'].iloc[i]:
        dropList.append(i)
Data = data.drop(dropList)
path =  'C:\code\python_practice\worm\_' + name + '.csv'
Data.to_csv(path)
Data =  pd.read_csv(path)
dic = {}
count(dic,2,Data)
count(dic,3,Data)
count(dic,4,Data)
count(dic,5,Data)
dic = sorted(dic.items(), key = lambda item:item[1],reverse = True)
dicData = pd.DataFrame(dic[:500])
path =  'C:\code\python_practice\worm\_' + name + '_dic.csv'
dicData.to_csv(path)
dic2 = {}
count2(dic2,2,Data)
count2(dic2,3,Data)
count2(dic2,4,Data)
count2(dic2,5,Data)
dic2 = sorted(dic2.items(), key = lambda item:item[1],reverse = True)
dicData2 = pd.DataFrame(dic2[:100])
path =  'C:\code\python_practice\worm\_' + name + '_tag_dic.csv'
dicData2.to_csv(path)


# In[ ]:


print(data)


# In[ ]:


'營業時間' in data.iloc[8,:]['contest']


# In[ ]:




