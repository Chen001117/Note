#ifndef LOGINPROCESS_H
#define LOGINPROCESS_H
#include <QObject>
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <string>
#include <vector>
#include "user.h"
using namespace std;

enum LoginResult {LR_NOUSER, LR_ADMIN, LR_TEACHER, LR_STUDENT};


/*************************************************************************
【类名】LoginProcess
【功能】业务流程类，处理登入相关信息
【接口说明】
    构造函数LoginProcess
    函数Login 执行用户登入，并回传登入者类型
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽添加注释
    2020-06-20 由陈文泽添加判断三次登出
    2020-06-20 将此来改为继承自QObject类
*************************************************************************/

class LoginProcess: public QObject
{
    Q_OBJECT;

public:
    //构造函数
    LoginProcess(QObject* Parent = nullptr);
    //禁用拷贝构造，赋值运算
    LoginProcess(const LoginProcess&) = delete;
    LoginProcess& operator = (const LoginProcess&) = delete;
    //析构函数
    ~LoginProcess();
    //登入函数(回传登入者类型)
    LoginResult Login(const QString& UserName, const QString& Password);

private:
    //登入失败次数
    unsigned int m_uiErrTime;

signals:
    //请求强制跳出程序
    void closeWin();

};

#endif // LOGINPROCESS_H
