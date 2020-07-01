#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <vector>
using namespace std;


/*************************************************************************
【类名】Quiz
【功能】问题基类，用来派生出StudentQuiz与TeacherQuiz
【接口说明】
    构造函数Quiz
    拷贝构造函数Quiz
    赋值运算符operator=
    析构函数~Quiz
    函数get_opts得到选项描述
    常引用optNum选项个数
    常引用Question问题描述
    常引用isMuti表示该题是否为多选题
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽增加isMuti
*************************************************************************/

class Quiz
{
public:
    //构造函数
    Quiz(string Ques = "", vector<string> opts = vector<string>(), vector<bool> CorrAns = vector<bool>());
    //拷贝构造函数
    Quiz(const Quiz& aQuiz);
    //赋值运算符
    Quiz& operator = (const Quiz& aQuiz);
    //析构函数
    virtual ~Quiz();
    //得到选项描述
    void get_opts(vector<string>& temp)const;
    //常引用选项个数
    const unsigned int& optNum;
    //问题描述
    const string& Question;
    //是否为多选
    const bool& isMuti;
protected:
    //type
    bool m_bIsMuti;
    //问题描述
    string m_sQuestion;
    //选项描述
    vector<string> m_option;
    //正确答案
    vector<bool> m_CorrAns;
    //选项个数
    unsigned int m_uiOptNum;
};

#endif // QUIZ_H
