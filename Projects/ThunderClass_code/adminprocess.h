/*************************************************************************
【文件名】adminprocess.h
【功能模块和目的】管理员业务流程类声明
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-10
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】2020-5-15 由范静涛增加注释
*************************************************************************/

#ifndef ADMINPROCESS_H
#define ADMINPROCESS_H
#include <QString>
#include <string>
#include "user.h"
using namespace std;

/*************************************************************************
【类名】AdminProcess
【功能】管理员业务流程类
【接口说明】
    构造函数AdminProcess()
    函数addUser添加用户
    函数delUser删除用户
【开发者及日期】
【更改记录】
    2020-5-15 由范静涛增加注释
    2020-06-19 由陈文泽增加adduser deluser函数
*************************************************************************/
class AdminProcess
{
public:
    //构造函数
    AdminProcess();
    //新增用户
    bool addUser(const QString, const QString, const QString);
    //删除用户
    bool delUser(const QString, const QString, const QString);
};

#endif // ADMINPROCESS_H
