#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <string>

using namespace std;

class Message;

/*************************************************************************
【类名】Performance
【功能】学生表现类，用于记录学生专注度
【接口说明】
    构造函数Performance
    拷贝构造Performance
    赋值运算符operator=
    析构函数~ConcenTime
    函数addConcenData增加专注度数据
    函数getString得到文字型输出
    常引用name学生姓名
    常引用allTime总上课时常
    常饮用concenTime总专注时常
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽修改getString输出内容
*************************************************************************/

class Performance
{
public:
    //构造函数
    Performance(string Name = "", unsigned int AllTime = 0, unsigned int Concen = 0);
    //拷贝构造
    Performance(const Performance& aPer);
    //赋值运算
    Performance& operator = (const Performance& aPer);
    //析构函数
    ~Performance();
    //增加专注度数据
    void addConcenData(bool isConcen);
    //得到文字形式输出
    string getString()const;
    //友元类 Message
    friend Message;
private:
    //总上课时常
    unsigned int m_uiAllTime;
    //专注时长
    unsigned int m_uiConcenTime;
    //专注时间占比
    float m_fConcenRatio;
    //学生姓名
    string m_sName;
};

#endif // PERFORMANCE_H
