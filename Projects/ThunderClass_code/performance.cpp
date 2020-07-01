#include "performance.h"

/*************************************************************************
【函数名称】Performance::Performance
【函数功能】构造函数
【参数】
    Name 输入参数，学生姓名
    AllTime 输入参数，总花费时长
    Concen 输入参数，总专注时长
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-19 由陈文泽修改恒为0的bug
*************************************************************************/
Performance::Performance(string Name, unsigned int AllTime, unsigned int Concen)
{
    m_sName = Name;
    m_uiAllTime = AllTime;
    m_uiConcenTime = Concen;
    if(AllTime == 0){
        m_fConcenRatio = 0;
    }else{
        m_fConcenRatio = float(Concen) / float(AllTime);
    }
}


/*************************************************************************
【函数名称】Performance::Performance
【函数功能】拷贝构造函数
【参数】aPer 输入参数，表示源对象
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Performance::Performance(const Performance& aPer)
{
    m_sName = aPer.m_sName;
    m_uiAllTime = aPer.m_uiAllTime;
    m_uiConcenTime = aPer.m_uiConcenTime;
    m_fConcenRatio = aPer.m_fConcenRatio;
}


/*************************************************************************
【函数名称】operator =
【函数功能】赋值运算符
【参数】aPer 输入参数，表示源对象
【返回值】被赋值对象的引用
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Performance& Performance:: operator = (const Performance& aPer)
{
    if(this != &aPer){
        m_sName = aPer.m_sName;
        m_uiAllTime = aPer.m_uiAllTime;
        m_uiConcenTime = aPer.m_uiConcenTime;
        m_fConcenRatio = aPer.m_fConcenRatio;
    }
    return *this;
}


/*************************************************************************
【函数名称】~Performance
【函数功能】析构函数
【参数】无
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Performance::~Performance(){

}


/*************************************************************************
【函数名称】Performance::addConcenData
【函数功能】添加新的数据
【参数】isConcen 输入参数，表示是否有专心
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
void Performance::addConcenData(bool isConcen){
    if(isConcen == true){
        m_uiConcenTime++;
    }
    m_uiAllTime++;
    m_fConcenRatio = float(m_uiConcenTime) / float(m_uiAllTime);
}

/*************************************************************************
【函数名称】Performance::getString
【函数功能】得到字串型学生表现输出
【参数】无
【返回值】字符串，学生表现
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽修改字符串形式
*************************************************************************/
string Performance::getString()const{
    string ratio = to_string(m_fConcenRatio);
    string name = m_sName;
    string allTime = to_string(m_uiAllTime);
    string concenTime = to_string(m_uiConcenTime);
    return name + " 专注度" + ratio + " 总上课时长" + allTime + "s 总专心时长" + concenTime + "s";
}

