/*************************************************************************
【文件名】teacherprocess.h
【功能模块和目的】教师业务流程类声明
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，并添加了使用信号更新界面的机制
*************************************************************************/
#ifndef TEACHERPROCESS_H
#define TEACHERPROCESS_H

#include <QObject>
#include <QString>
#include "Server.h"
#include "Message.h"
#include "audio.h"
#include <vector>
#include <utility>
#include <QComboBox>
#include <QLineEdit>
#include <string>
#include "teacherquiz.h"
#include "quiz.h"
#include <iostream>
#include <fstream>

using namespace std;

/*************************************************************************
TeacherProcess
【功能】学生上课业务流程类
【接口说明】
    构造函数TeacherProcess
    析构函数~TeacherProcess
    函数BeginClass上课
    函数EndClass下课
    函数StartScreenShare开始屏幕共享
    函数StopScreenShare停止屏幕共享
    函数StartVoiceShare开始语音直播
    函数StopVoiceShare停止语音直播
    函数SendStrChat群发文本信息
    函数popQuiz向随机对象发送随机提问提示
    函数getQuiz得到新的问题
    函数outPut输出学生表现信息
    函数getAns得到答案
    函数stopAns收题目
    函数bool& IsInClass;
    函数bool& IsScreenSharing;
    函数bool& IsVoiceSharing;
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，并添加了使用信号更新界面的机制
    2020-06-19 由陈文泽增加popquiz函数
    2020-06-19 由陈文泽新增getQuiz函数
    2020-06-20 由陈文泽新增outPut函数
    2020-06-21 由陈文则更新getans函数
*************************************************************************/
class TeacherProcess : public QObject{
    Q_OBJECT
public:
    //构造函数
    explicit TeacherProcess(QObject* Parent = nullptr);

    //析构函数
    ~TeacherProcess();

    //禁用拷贝构造函数
    TeacherProcess(const TeacherProcess&) = delete;

    //禁用赋值运算符
    TeacherProcess& operator=(const TeacherProcess&) = delete;

    //上课
    bool BeginClass();

    //下课
    bool EndClass();

    //开始屏幕共享
    bool StartScreenShare();

    //停止屏幕共享
    bool StopScreenShare();

    //开始语音直播
    bool StartVoiceShare();

    //停止语音直播
    bool StopVoiceShare();

    //群发文字
    bool SendStrChat(const QString& ChatStr);

    //随机提问
    void popQuiz();

    //得到新的题目
    void getQuiz(QString ques, vector<QString> opts, vector<bool> isAns);

    //得到答案
    void getAns(StudentQuiz aQuiz);

    //输出学生表现
    void outPut(vector<Performance> aPerfor);

    //收题
    void stopAns();

    //在不在上课
    const bool& IsInClass;

    //在不在共享屏幕
    const bool& IsScreenSharing;

    //在不在语音直播
    const bool& IsVoiceSharing;

signals:
    void ClassBegined();
    void ClassEnded();
    void RecvBeginClass();
    void RecvStudentLogin(QString Name);
    void RecvStudentLogout(QString Name);
    void RecvChat(QString Msg);
    void CloseMakeQuiz();
    void changeRes(QString aStr);
    void Perfor(vector<Performance> aPer);

private:

    //上线一个学生
    bool OnlineStudent(const pair<unsigned int, Message>& Pair);

    //下线一个学生
    void OffLineStudent(unsigned int Idx);

    //在不在上课
    bool m_bIsInClass;

    //服务端
    Server* m_pServer;

    //音频播放对象
    Audio* m_pVoiceCapture;

    //屏幕共享线程函数
    static DWORD WINAPI ScreenShareThread(LPVOID lpParameter);

    //屏幕共享线程句柄
    HANDLE m_hScreeShareThread;

    //是否正在屏幕共享
    bool m_bIsScreenSharing;

    //语音直播线程函数
    static DWORD WINAPI VoiceShareThread(LPVOID lpParameter);

    //语音直播线程句柄
    HANDLE m_hVoiceShareThread;

    //是否正在语音直播
    bool m_bIsVoiceSharing;

    //对收到的消息进行执行的线程函数
    static DWORD WINAPI MsgReceiveThread(LPVOID lpParameter);

    //对收到的消息进行执行的线程句柄
    HANDLE m_hMsgReceiveThread;

    //保存用户名的数组*
    vector<string> m_Names;

    //保存密码的数组*
    vector<string> m_Passwords;

    //保存连接号的数组*
    vector<unsigned int> m_ConnectionIdx;

    //*标注星号，表示同一个学生的用户名、密码、连接号在三个数组中下标一致

    //教师题目数组
    vector<TeacherQuiz> m_TeaQuiz;

    //教师端统计学生表现数组
    vector<Performance> m_Perfor;
};

#endif // TEACHERPROCESS_H
