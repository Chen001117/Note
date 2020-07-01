/*************************************************************************
【文件名】adminprocess.cpp
【功能模块和目的】管理员业务流程类定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-10
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】2020-5-15 由范静涛增加注释
*************************************************************************/

#include "adminprocess.h"
#include "admin.h"

/*************************************************************************
【函数名称】AdminProcess::AdminProcess
【函数功能】管理员业务流程类构造函数，不允许在类外显性构造管理员对象（友元User类除外）
【参数】无
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-10
【更改记录】2020-5-15 由范静涛增加注释
*************************************************************************/
AdminProcess::AdminProcess() {

}

/*************************************************************************
【函数名称】AdminProcess::addUser
【函数功能】新增用户
【参数】
    acc 用户名 输入参数
    pas 用户密码 输入参数
    typ 用户类型 输入参数
【返回值】构造函数不可有返回值
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
bool AdminProcess::addUser(const QString acc, const QString pas, const QString typ){
    string account = acc.toStdString();
    string password = pas.toStdString();
    string type = typ.toStdString();
    //判断类型是否正确
    if(type != "Teacher" && type != "Student"){
        return false;
    }
    //判断是否已经存在该用户名
    const User* aUser = User::GetUser(account);
    if(aUser != nullptr){
        return false;
    }
    //新增用户
    User* curUser = User::GetLoginedUser();
    bool IsAdd = curUser->AddUser(account, password, type);
    User::SaveToFile("userinformation.txt");
    return IsAdd;
}

/*************************************************************************
【函数名称】AdminProcess::addUser
【函数功能】删除用户
【参数】
    acc 用户名 输入参数
    pas 用户密码 输入参数
    typ 用户类型 输入参数
【返回值】成功删除返回true
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽添加判断类型
*************************************************************************/
bool AdminProcess::delUser(const QString acc, const QString pas, const QString typ){
    string account = acc.toStdString();
    string password = pas.toStdString();
    string type = typ.toStdString();
    //类型错误不删除
    if(typ != "Teacher" && typ != "Student"){
        return false;
    }
    //移除账户
    User* curUser = User::GetLoginedUser();
    bool IsDel = curUser->delUser(account, password, type);
    User::SaveToFile("userinformation.txt");
    return IsDel;
}

