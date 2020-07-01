#include "teacherquiz.h"
#include <QDebug>

/*************************************************************************
【函数名称】TeacherQuiz
【函数功能】构造函数
【参数】
    Ques 问题描述 输入参数
    opts 选项描述 输入参数
    CorrAns 正确选项 输入参数
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
TeacherQuiz::TeacherQuiz(string Ques, vector<string> opts, vector<bool> CorrAns): Quiz(Ques, opts, CorrAns)
{
    m_uiAllNum = 0;
    m_uiCorrNum = 0;
    for(unsigned int i = 0; i < opts.size(); i++){
        m_optName.push_back(vector<string>());
    }
    m_Name.clear();
    m_Time.clear();
}

/*************************************************************************
【函数名称】TeacherQuiz
【函数功能】拷贝构造函数
【参数】aQuiz 输入参数，表示源对象
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
TeacherQuiz::TeacherQuiz(const TeacherQuiz& aQuiz): Quiz(aQuiz)
{
    m_optName = aQuiz.m_optName;
    m_Name = aQuiz.m_Name;
    m_Time = aQuiz.m_Time;
    m_uiCorrNum = aQuiz.m_uiCorrNum;
    m_uiAllNum = aQuiz.m_uiAllNum;
}

/*************************************************************************
【函数名称】~TeacherQuiz
【函数功能】析构函数
【参数】无
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
TeacherQuiz::~TeacherQuiz(){
    m_optName.clear();
    m_Name.clear();
    m_Time.clear();
}

/*************************************************************************
【函数名称】operator =
【函数功能】赋值运算符
【参数】aQuiz 输入参数，表示源对象
【返回值】被赋值对象的引用
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
TeacherQuiz& TeacherQuiz::operator=(const TeacherQuiz &aQuiz)
{
    if(this != &aQuiz){
        Quiz::operator=(aQuiz);
        m_optName = aQuiz.m_optName;
        m_Name = aQuiz.m_Name;
        m_Time = aQuiz.m_Time;
        m_uiCorrNum = aQuiz.m_uiCorrNum;
        m_uiAllNum = aQuiz.m_uiAllNum;
    }
    return *this;
}




/*************************************************************************
【函数名称】TeacherQuiz::getAns
【函数功能】收到回答
【参数】aAns 输入参数，表示收到的学生答案
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
void TeacherQuiz::getAns(const StudentQuiz& aAns){
    //统计答题名单
    m_Name.push_back(aAns.m_sName);
    m_uiAllNum++;
    //统计正确人数
    if(aAns.isCorr() == true){
        m_uiCorrNum++;
    }
    //统计各选项名单
    vector<bool> isChos = aAns.m_MyAns;
    for(unsigned int i = 0; i < isChos.size(); i++){
        if(isChos[i] == true){
            m_optName[i].push_back(aAns.m_sName);
        }
    }
    //统计答题者费时
    m_Time.push_back(aAns.m_uiUseTime);
}

/*************************************************************************
【函数名称】TeacherQuiz::getStr
【函数功能】得到文字形式输出
【参数】无
【返回值】字符串对象
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-21
【更改记录】
*************************************************************************/
string TeacherQuiz::getStr(){
    float corrRatio = (m_uiAllNum == 0 ? 0 : float(m_uiCorrNum) / float(m_uiAllNum));
    string out = this->Question + "(共" + to_string(m_uiAllNum) + "人回答, 正确率" + to_string(corrRatio) + ")\n";
    for(unsigned int i = 0; i < m_optName.size(); i++){
        out += ("(" + to_string(i) + ")" + m_option[i] + " (" + to_string(m_optName[i].size()) + "人)\n");
    }
    out += "\n";
    return out;
}
