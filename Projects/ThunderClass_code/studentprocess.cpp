/*************************************************************************
【文件名】studentprocess.cpp
【功能模块和目的】学生业务流程类定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，用信号实现界面更新
    2020-05-19 由范静涛增加了日志输出QDebug
*************************************************************************/

#include "studentprocess.h"
#include <windows.h>
#include <QDebug>

/*************************************************************************
【函数名称】StudentProcess::StudentProcess
【函数功能】构造函数
【参数】入口参数，父对象指针，默认nullptr
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛删除界面空间指针参数，用信号实现界面更新
    2020-05-19 由范静涛修正了未初始化消息接收处理线程句柄为nullptr的错误逻辑
    2020-06-19 由陈文泽增加不属于被抽考状态
*************************************************************************/
StudentProcess::StudentProcess(QObject* Parent): QObject(Parent), IsInClass(m_bIsInClass)
{
    //准备一个用于远程登录的消息，包含自己的用户名和密码
    //GetLoginedUser获取的是本地登录的账户指针
    m_RemoteLoginMsg = User::GetLoginedUser()->ToMessage();
    //用私有成员保存界面控件指针
    //初始化内部状态
    m_bIsInClass = false;
    //新建socket客户端
    m_pClient = new Client();
    //新建音频播放对象
    m_pVoicePlayer = new Audio();
    //初始化消息接收处理线程句柄为空
    m_hMsgReceiveThread = nullptr;
    //不属于被抽考状态
    m_bIsAsked = false;
    //初始化学生表现
    string Name = User::GetLoginedUser()->GetName();
    m_pStuPer = new Performance(Name);
    qDebug() << "SP: Created" << QString::fromStdString(Name);
}

/*************************************************************************
【函数名称】StudentProcess::~StudentProcess
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
StudentProcess::~StudentProcess() {
    //退出课堂
    ExitClass();
    //释放socket客户端和音频对象
    delete m_pClient;
    m_pClient = nullptr;
    delete m_pVoicePlayer;
    m_pVoicePlayer = nullptr;
    delete m_pStuPer;
    m_pStuPer = nullptr;
    qDebug("SP: Destroyed");
}

/*************************************************************************
【函数名称】StudentProcess::EnterClass
【函数功能】进入课堂
【参数】入口参数，教师端IP地址
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
*************************************************************************/
bool StudentProcess::EnterClass(const QString& IP) {
    //如果已经进入课堂，则不执行任何操作
    if (m_bIsInClass) {
        return m_bIsInClass;
    }

    //客户端连接到服务器
    string IpStr = IP.toStdString();
    m_pClient->ConnectTo(IpStr, 7908);

    //开始计时
    qDebug() << "SP: trying to connect to server @ " << QString::fromStdString(IpStr);

    DWORD BeginTime = GetTickCount();
    DWORD EndTime = BeginTime;
    do {
        //更新当前时间
        EndTime = GetTickCount();
        //30秒内未连接，则继续循环
    }while(EndTime - BeginTime <= 30000 && !m_pClient->IsConnected);

    //连接失败，返回
    if (!m_pClient->IsConnected) {
        qDebug() << "SP: connect fail after " << EndTime - BeginTime << " ms";
        //发出“退出课堂信号”
        emit ClassExited();
        return false;
    }

    //更新为以进入课堂状态
    m_bIsInClass = true;
    //发出“进入课堂信号”
    //qDebug() << "SP: connected after " << EndTime - BeginTime << " ms";
    emit ClassEntered();
    //创建消息接收和处理线程
    m_hMsgReceiveThread = CreateThread(nullptr, 0, MsgReceiveThread, (LPVOID)this, 0, nullptr);
    if (m_hMsgReceiveThread == nullptr) {
        qDebug() << "SP: failed to create a MessageRecv&Process thread!";
        throw(runtime_error("Failed to create a MessageRecv&Process thread!"));
    }
    //第一时间向教师端发送登录用的用户名密码消息
    qDebug() << "SP: send ID & Password";
    m_pClient->Send(m_RemoteLoginMsg);
    //开始播放声音(如果从教师端收到了)
    m_pVoicePlayer->StartPlay();

    return m_bIsInClass;
}

/*************************************************************************
【函数名称】StudentProcess::ExitClass
【函数功能】退出课堂
【参数】无
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
    2020-06-19 由陈文泽增加离开前送出学生表现
*************************************************************************/
bool StudentProcess::ExitClass() {
    //如果未在课堂中，检查是否消息接收处理线程自主退出了
    if (!m_bIsInClass) {
        if(m_hMsgReceiveThread != nullptr) {
            qDebug() << "SP: clean MsgRecv&Process thread handle";
            CloseHandle(m_hMsgReceiveThread);
            m_hMsgReceiveThread = nullptr;
        }
        return m_bIsInClass;
    }
    //离开前送出学生表现
    m_pClient->Send(Message::FromPerformance(*m_pStuPer));
    Sleep(500);
    //更新学生表现
    delete m_pStuPer;
    string Name = User::GetLoginedUser()->GetName();
    m_pStuPer = new Performance(Name);
    //断开与服务器连接
    m_pClient->DisConnect();
    //停止播放音频
    m_pVoicePlayer->StopPlay();
    //清空待播放音频队列
    m_pVoicePlayer->Clear();
    //更新为未上课状态
    m_bIsInClass = false;
    //更新答题记录
    m_QuizData = vector<StudentQuiz>();
    //更新是否被抽到答题
    m_bIsAsked = false;
    //等待消息接收和处理线程结束
    qDebug() << "SP: wait MsgRecv&Process thread end";
    WaitForSingleObject(m_hMsgReceiveThread, 2000);
    //清理线程句柄
    qDebug() << "SP: clean MsgRecv&Process thread handle";
    CloseHandle(m_hMsgReceiveThread);
    m_hMsgReceiveThread = nullptr;
    //发出“退出课堂信号”
    emit ClassExited();
    //返回当前是否在课堂中
    return m_bIsInClass;
}

/*************************************************************************
【函数名称】StudentProcess::setAsked
【函数功能】改变状态(是否被提问)
【参数】temp，输入参数，表示是否被提问
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
void StudentProcess::setAsked(bool isAsked){
    m_bIsAsked = isAsked;
}

/*************************************************************************
【函数名称】StudentProcess::dealConcen
【函数功能】改变专心度
【参数】isConcen，输入参数，表示是否专心
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
*************************************************************************/
void StudentProcess::dealConcen(bool isConcen){
    m_pStuPer->addConcenData(isConcen);
}

/*************************************************************************
【函数名称】StudentProcess::Send
【函数功能】向教师端发送文字消息
【参数】入口参数，QString字符串
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-06-19 由陈文泽更改若处于被提问状态则传送PopQuiz类对象
    2020-06-19 由陈文泽更改Send函数不为const类型
*************************************************************************/
bool StudentProcess::Send(const QString& Msg){
    //不在课堂
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //未连接教师端
    if (!m_pClient->IsConnected) {
        return m_pClient->IsConnected;
    }
    //转换格式并发送
    qDebug() << "SP: send Message: " << Msg;
    string aName = User::GetLoginedUser()->GetName();
    string MsgStr = aName + " " + Msg.toStdString();
    //若在被提问状态->传PopQuiz类对象
    if(m_bIsAsked == true){
        m_bIsAsked = false;
        return m_pClient->Send(Message::FromPopQuiz(MsgStr));
    }
    //若否，传一般文字对象
    else{
        return m_pClient->Send(Message::FromString(MsgStr));
    }
}

/*************************************************************************
【函数名称】StudentProcess::RecvAns
【函数功能】收到回答，回答问题后将带有答案的问题回传
【参数】
    aQuiz 入口参数，问题对象
    isChe 入口参数，我的答案
    Time 入口参数，答题时间
    Owner 入口参数，拥有者姓名
【返回值】无
【开发者及日期】陈文泽(cwz19@mails.tsinghua.edu.cn) 2020-06-19
【更改记录】
    2020-06-20 由陈文泽优化逻辑，让问题对象等到此函数才建构，而非在界面建构
*************************************************************************/
void StudentProcess::RecvAns(StudentQuiz aQuiz, vector<bool> isChe, unsigned int Time, string Owner){
    aQuiz.Ans(isChe, Time, Owner);
    m_QuizData.push_back(aQuiz);
    m_pClient->Send(Message::FromQuiz(aQuiz));
}

/*************************************************************************
【函数名称】StudentProcess::MsgRecieveThread
【函数功能】接收并处理消息的线程函数
【参数】入口参数，表示线程所属的StudentProcess对象指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
    2020-05-19 由范静涛修正了jpg空间未释放的错误逻辑
    2020-06-19 由陈文泽增加接收popquiz信息判断句
    2020-06-20 由陈文泽优化Popquiz类信息处理方式
*************************************************************************/
DWORD WINAPI StudentProcess::MsgReceiveThread(LPVOID lpParameter) {
    //因为实参是StudentProcess*，所以可以强制类型装换
    StudentProcess* pProcess = (StudentProcess*)lpParameter;
    //用于接收的临时消息存储对象
    Message TempMessage;
    //接收到的消息的类型
    MessageType Type;
    //在课堂且通信中，始终执行循环
    while (pProcess->IsInClass && pProcess->m_pClient->IsConnected) {
        //有待处理消息，则放入TempMessage里
        if (pProcess->m_pClient->GetAndRemoveFirstRecvMsg(TempMessage)) {
            //获得消息类型
            Type = TempMessage.Type();
            //如果是文本信息消息，转换成为本对象，发出信号要求界面更新
            if (Type == MSG_STRING) {
                qDebug() << "SP: recieved chat message: " << QString::fromStdString(Message::ToString(TempMessage));
                emit pProcess->RecvChat(QString::fromStdString(Message::ToString(TempMessage)));
            }
            //如果是音频帧消息，放入待播放音频队列中
            else if (Type == MSG_AUDIO) {
                qDebug() << "SP: recieved audio frame message " << TempMessage.Size << " bytes";
                pProcess->m_pVoicePlayer->AppendFrameToPlay(Message::ToAudioFrame(TempMessage));
            }
            //如果是Jpg图像消息，界面后刷到界面上
            else if (Type == MSG_JPG) {
                char* pJpgData = nullptr;
                unsigned int JpgSize = 0;
                Message::ToJpgData(TempMessage, &pJpgData, &JpgSize);
                QImage qImg;
                qImg.loadFromData((unsigned char*)pJpgData, JpgSize, "jpg");
                qDebug() << "SP: recieved jpg message " << JpgSize << " bytes";
                //2020-05-19 修正了逻辑，之前没有释放开辟的空间
                delete [] pJpgData;
                //发出收到图像信号，要求界面更新
                emit pProcess->RecvScreen(qImg);
            }
            //如果是PopQuiz类信息，显示并将是否被提问状态改为是
            else if(Type == MSG_POPQUIZ){
                emit pProcess->RecvChat(QString::fromStdString(Message::ToPopQuiz(TempMessage)));
                pProcess->setAsked(true);
            }
            //如果是Quiz类信息，送出RecvQuiz信号
            else if(Type == MSG_QUIZ){
                StudentQuiz aQuiz;
                Message::ToQuiz(TempMessage, aQuiz);
                string name = User::GetLoginedUser()->GetName();
                emit pProcess->RecvQuiz(aQuiz, name);
            }
            //如果是提前收提信息，送出RecvStop信号
            else if(Type == MSG_STOP){
                emit pProcess->RecvStop();
            }
            //如果是退出消息，跳出消息接收处理循环，相当退出课堂
            else if (Type == MSG_EXIT) {
                emit pProcess->ClassExited();
                break;
            }
        }
    }
    return 0;
}
