#ifndef TEACHERQUIZ_H
#define TEACHERQUIZ_H

#include <vector>
#include <string>
#include "quiz.h"
#include "studentquiz.h"

using namespace std;


/*************************************************************************
【类名】TeacherQuiz
【功能】教师用题目，用来记录各个题目统计数据
【接口说明】
    构造函数TeacherQuiz
    拷贝构造函数TeacherQuiz
    赋值运算符operator=
    析构函数~TeacherQuiz
    函数getOptNum获取各个选项人数
    函数getStr得到文本输出
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-21 由陈文泽基于安全考量，被StuQuiz设为友元
    2020-06-21 由陈文泽增加getStr函数
*************************************************************************/

class TeacherQuiz: public Quiz
{
public:
    //构造函数
    TeacherQuiz(string Ques = "", vector<string> opts = vector<string>(), vector<bool> CorrAns = vector<bool>());
    //拷贝构造函数
    TeacherQuiz(const TeacherQuiz& aQuiz);
    //赋值运算符
    TeacherQuiz& operator = (const TeacherQuiz& aQuiz);
    //析构函数
    ~TeacherQuiz();
    //接收回答
    void getAns(const StudentQuiz& aAns);
    //得到文本输出
    string getStr();
private:
    //各个选项名单
    vector<vector<string> > m_optName;
    //答题者名单
    vector<string> m_Name;
    //答题者费时
    vector<unsigned int> m_Time;
    //答对人数
    unsigned int m_uiCorrNum;
    //总答题人数
    unsigned int m_uiAllNum;
};

#endif // TEACHERQUIZ_H
