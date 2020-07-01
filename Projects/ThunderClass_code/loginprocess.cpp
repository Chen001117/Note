#include "loginprocess.h"
#include <windows.h>
#include <QDebug>

/*************************************************************************
【函数名称】LoginProcess::LoginProcess
【函数功能】构造函数
【参数】Prent，输入参数，表示父视窗
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-20
【更改记录】
*************************************************************************/
LoginProcess::LoginProcess(QObject* Parent): QObject(Parent)
{
    User::LoadFromFile("UserInformation.txt");
}


/*************************************************************************
【函数名称】LoginProcess::~LoginProcess
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-20
【更改记录】
*************************************************************************/
LoginProcess::~LoginProcess(){

}

/*************************************************************************
【函数名称】LoginProcess::Login
【函数功能】得到使用者的类型
【参数】
    UserName 使用者姓名
    Password 使用者密码
【返回值】使用者类型，错误返回空
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-20
【更改记录】
*************************************************************************/
LoginResult LoginProcess::Login(const QString& UserName, const QString& Password){
    const User* pCurrentUser = User::GetUser(UserName.toStdString(), Password.toStdString());
    if (pCurrentUser == nullptr) {
        //如果第三次错误就强制退出
        m_uiErrTime++;
        if(m_uiErrTime >= 3){
            emit closeWin();
        }
        return LR_NOUSER;
    }
    if (pCurrentUser->Type == "Admin") {
        m_uiErrTime = 0;
        return LR_ADMIN;
    }
    else if (pCurrentUser->Type == "Teacher") {
        m_uiErrTime = 0;
        return LR_TEACHER;
    }
    else {
        m_uiErrTime = 0;
        return LR_STUDENT;
    }
}

