#ifndef STUDENTQUIZ_H
#define STUDENTQUIZ_H

#include "quiz.h"

class Message;
class TeacherQuiz;

/*************************************************************************
【类名】StudentQuiz
【功能】学生问题类(派生自Quiz)，用来记录每个学生的回答情况
【接口说明】
    构造函数StudentQuiz
    拷贝构造函数StudentQuiz
    赋值运算符operator
    析构函数~StudentQuiz
    函数isCorr判断此题是否正确
    函数Ans回答此问题
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-21 由陈文泽基于安全性考量，将StuQuiz设为Msg类友元，并删除部份函数
    2020-06-21 由陈文泽更正注释
*************************************************************************/

class StudentQuiz: public Quiz
{
public:
    //構造函數
    StudentQuiz(string Ques = "", vector<string> opts = vector<string>(), vector<bool> CorrAns = vector<bool>(), unsigned int usetime = 0, vector<bool> myans = vector<bool>());
    //拷贝构造函数
    StudentQuiz(const StudentQuiz& aQuiz);
    //赋值运算符
    StudentQuiz& operator = (const StudentQuiz& aQuiz);
    //析构函数
    ~StudentQuiz();
    //是否正确
    bool isCorr()const;
    //回答问题
    void Ans(const vector<bool> myAns, unsigned int useTime, string Name);
    //友元类
    friend Message;
    friend TeacherQuiz;
private:
    //答題時間
    unsigned int m_uiUseTime;
    //我的答案
    vector<bool> m_MyAns;
    //回答者姓名
    string m_sName;
};
#endif // STUDENTQUIZ_H
