#include "studentquiz.h"

/*************************************************************************
【函数名称】StudentQuiz::StudentQuiz
【函数功能】构造函数
【参数】
    Ques 输入参数 问题描述
    opt 输入参数 选项描述
    CorrAns 输入参数 正确解答
【返回值】构造函数不可有返回值 被赋值对象的引用
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
StudentQuiz::StudentQuiz(string Ques, vector<string> opts, vector<bool> CorrAns, unsigned int usetime, vector<bool> myans): Quiz(Ques, opts, CorrAns)
{
    //答题时间
    m_uiUseTime = usetime;
    //我的回答(若与正确答案长度不同添加到相同)
    m_MyAns = myans;
    while(m_MyAns.size() < m_CorrAns.size()){
        m_MyAns.push_back(false);
    }
    //答题人
    m_sName = "";
}

/*************************************************************************
【函数名称】StudentQuiz::StudentQuiz
【函数功能】拷贝构造函数
【参数】aQuiz 输入参数，表示源对象
【返回值】构造函数不可有返回值 被赋值对象的引用
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
StudentQuiz::StudentQuiz(const StudentQuiz& aQuiz): Quiz(aQuiz)
{
    this->m_uiUseTime = aQuiz.m_uiUseTime;
    this->m_MyAns = aQuiz.m_MyAns;
}

/*************************************************************************
【函数名称】tudentQuiz::operator =
【函数功能】=运算符
【参数】aQuiz 输入参数，表示源对象
【返回值】StudentQuiz对象
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽修改Quiz::operator=(aQuiz);
*************************************************************************/
StudentQuiz& StudentQuiz::operator = (const StudentQuiz& aQuiz){
    if(this != &aQuiz){
        Quiz::operator=(aQuiz);
        this->m_uiUseTime = aQuiz.m_uiUseTime;
        this->m_MyAns = aQuiz.m_MyAns;
    }
    return *this;
}

/*************************************************************************
【函数名称】StudentQuiz::~StudentQuiz()
【函数功能】析构函数
【参数】无
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
StudentQuiz::~StudentQuiz(){

}


/*************************************************************************
【函数名称】StudentQuiz::isCorr()
【函数功能】判断是否正确
【参数】无
【返回值】正确则输出true
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
bool StudentQuiz::isCorr()const{
    if(m_MyAns.size() != m_CorrAns.size()){
        return false;
    }
    for(unsigned int i = 0; i < m_MyAns.size(); i++){
        if(m_MyAns[i] != m_CorrAns[i]){
            return false;
        }
    }
    return true;
}

/*************************************************************************
【函数名称】StudentQuiz::Ans()
【函数功能】回答问题
【参数】myAns 输入参数 我的答案
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
void StudentQuiz::Ans(const vector<bool> myAns, unsigned int useTime, string Name){
    m_MyAns = myAns;
    m_uiUseTime = useTime;
    m_sName = Name;
}
