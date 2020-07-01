/*************************************************************************
【文件名】studentprocess.h
【功能模块和目的】学生业务流程类声明
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，用信号实现界面更新
*************************************************************************/
#ifndef STUDENTPROCESS_H
#define STUDENTPROCESS_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <string>
#include "client.h"
#include "Message.h"
#include "audio.h"
#include "user.h"
#include "performance.h"
#include "studentquiz.h"
using namespace std;

/*************************************************************************
【类名】StudentProcess
【功能】学生上课业务流程类
【接口说明】
    构造函数StudentProcess
    析构函数~StudentProcess
    函数EnterClass进入指定IP的课堂
    函数ExitClass退出课堂
    函数Send向教师发送文本消息
    函数setAsked将目前状态设定为"被提问"
    函数dealConcen处理专心度问题
    函数RecvAns收到回答
    常引用数据成员IsInClass表示是否在上课
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，用信号实现界面更新
    2020-06-19 由陈文泽增加setAsked函数
    2020-06-19 由陈文泽增加专心度函数
    2020-06-19 由陈文泽增加收到回答函数
    2020-06-20 由陈文泽将RecvChat与setAsked合并取代RecvPopQuiz
*************************************************************************/
class StudentProcess : public QObject{
    Q_OBJECT
public:
    //构造函数
    explicit StudentProcess(QObject* Parent = nullptr);

    //析构函数
    ~StudentProcess();

    //禁用拷贝构造函数
    StudentProcess(const StudentProcess&) = delete;

    //禁用赋值运算符
    StudentProcess& operator=(const StudentProcess&) = delete;

    //进入课堂
    bool EnterClass(const QString& IP);

    //退出课堂
    bool ExitClass();

    //发送文本消息给教师
    bool Send(const QString& Msg);

    //是否在上课
    const bool& IsInClass;

    //设为被提问状态
    void setAsked(bool isAsked);

    //处理专心度问题
    void dealConcen(bool isConcen);

    //收到回答
    void RecvAns(StudentQuiz aQuiz, vector<bool> isChe, unsigned int Time, string Owner);

signals:
    //进入课堂信号
    void ClassEntered();
    //退出课堂信号
    void ClassExited();
    //收到文字消息信号
    void RecvChat(QString Msg);
    //收到屏幕共享消息信号
    void RecvScreen(QImage Img);
    //收到题目
    void RecvQuiz(StudentQuiz aQues, string name);
    //提前收题
    void RecvStop();
private:
    //学生表现对象
    Performance* m_pStuPer;

    //是否在上课
    bool m_bIsInClass;

    //是否处于被随机抽考模式
    bool m_bIsAsked;

    //Socket客户端对象指针
    Client* m_pClient;

    //音频播放对象指针
    Audio* m_pVoicePlayer;

    //答題紀錄
    vector<StudentQuiz> m_QuizData;

    //接收消息并处理消息的线程函数
    static DWORD WINAPI MsgReceiveThread(LPVOID lpParameter);
    //对收到的消息进行执行的线程句柄
    HANDLE m_hMsgReceiveThread;
    Message m_RemoteLoginMsg;
};

#endif // STUDENTPROCESS_H
