#include "quiz.h"

/*************************************************************************
【函数名称】Quiz::Quiz
【函数功能】构造函数
【参数】
    Ques 输入参数 问题描述
    opt 输入参数 选项描述
    CorrAns 输入参数 正确解答
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽增加判断是否是多选题
*************************************************************************/
Quiz::Quiz(string Ques, vector<string> opts, vector<bool> CorrAns): optNum(m_uiOptNum), Question(m_sQuestion), isMuti(m_bIsMuti)
{
    //赋值
    m_sQuestion = Ques;
    m_option = opts;
    m_CorrAns = CorrAns;
    m_uiOptNum = m_CorrAns.size();
    //判断是否多选
    unsigned int ansNum = 0;
    for(unsigned int i = 0; i < CorrAns.size(); i++){
        if(CorrAns[i] == true){
            ansNum++;
        }
    }
    m_bIsMuti = (ansNum == 1 ? false : true);
}


/*************************************************************************
【函数名称】Quiz::Quiz
【函数功能】拷贝构造函数
【参数】aQuiz 输入参数，表示源对象
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Quiz::Quiz(const Quiz& aQuiz): optNum(m_uiOptNum), Question(m_sQuestion), isMuti(m_bIsMuti)
{
    m_bIsMuti = aQuiz.m_bIsMuti;
    m_sQuestion = aQuiz.m_sQuestion;
    m_option = aQuiz.m_option;
    m_CorrAns = aQuiz.m_CorrAns;
    m_uiOptNum = aQuiz.m_uiOptNum;
}

/*************************************************************************
【函数名称】operator=
【函数功能】赋值运算符
【参数】aQuiz 输入参数，表示源对象
【返回值】被赋值对象的引用
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Quiz& Quiz::operator = (const Quiz& aQuiz){
    if(this != &aQuiz){
        m_bIsMuti = aQuiz.m_bIsMuti;
        m_sQuestion = aQuiz.m_sQuestion;
        m_option = aQuiz.m_option;
        m_CorrAns = aQuiz.m_CorrAns;
        m_uiOptNum = aQuiz.m_uiOptNum;
    }
    return *this;
}

/*************************************************************************
【函数名称】Quiz::~Quiz
【函数功能】析构函数
【参数】无
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
Quiz::~Quiz(){
    m_option.clear();
    m_CorrAns.clear();
}

/*************************************************************************
【函数名称】Quiz::get_opts
【函数功能】得到选项描述
【参数】temp，输出参数，要获取信息的vector
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/

void Quiz::get_opts(vector<string>& temp)const{
    temp = m_option;
}

