#include "studentwindow.h"
#include "ui_studentwindow.h"
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <QDebug>

//构造函数
StudentWindow::StudentWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentWindow)
{
    //赋值
    ui->setupUi(this);
    m_pProc = new StudentProcess(this);
    m_pAnsQuiz = new AnsQuiz(this);
    //注册自定义信号
    qRegisterMetaType<StudentQuiz>("StudentQuiz");
    qRegisterMetaType<StudentQuiz>("StudentQuiz&");
    qRegisterMetaType<string>("string");
    qRegisterMetaType<string>("string&");
    qRegisterMetaType<unsigned int>("unsigned int");
    qRegisterMetaType<unsigned int>("unsigned int&");
    qRegisterMetaType<vector<bool>>("vector<bool>");
    qRegisterMetaType<vector<bool>>("vector<bool>&");
    //建立信号和槽的连接关系
    //connect是建立联系，而非调用槽函数
    connect(m_pProc, SIGNAL(ClassEntered()), this, SLOT(ClassEntered()));
    connect(m_pProc, SIGNAL(ClassExited()), this, SLOT(ClassExited()));
    connect(m_pProc, SIGNAL(RecvStop()), m_pAnsQuiz, SLOT(RecvStop()));
    connect(m_pAnsQuiz, SIGNAL(closeWin()), this, SLOT(closeWin()));
    connect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));
    connect(m_pProc, SIGNAL(RecvScreen(QImage)), this, SLOT(RecvScreen(QImage)));
    connect(m_pProc, SIGNAL(RecvQuiz(StudentQuiz, string)), this, SLOT(RecvQuiz(StudentQuiz, string)));
    connect(m_pAnsQuiz, SIGNAL(RecvAns(StudentQuiz, vector<bool>, unsigned int, string)), this, SLOT(RecvAns(StudentQuiz, vector<bool>, unsigned int, string)));
    //设定计时器
    m_pTimer = new QTimer();
    m_pTimer->setInterval(1000);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    m_pTimer->start();
    //只显示关闭窗口按钮，没有？按钮
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    //设定控件是否可用
    UpdateControlEnabled(false);
}

StudentWindow::~StudentWindow()
{
    //终止计时器
    m_pTimer->stop();
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    delete m_pTimer;
    m_pTimer = nullptr;
    //先disconnected，再释放业务流程类
    disconnect(m_pProc, SIGNAL(ClassEntered()), this, SLOT(ClassEntered()));
    disconnect(m_pProc, SIGNAL(ClassExited()), this, SLOT(ClassExited()));
    disconnect(m_pAnsQuiz, SIGNAL(closeWin()), this, SLOT(closeWin()));
    disconnect(m_pProc, SIGNAL(RecvStop()), m_pAnsQuiz, SLOT(RecvStop()));
    disconnect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));
    disconnect(m_pProc, SIGNAL(RecvScreen(QImage)), this, SLOT(RecvScreen(QImage)));
    disconnect(m_pProc, SIGNAL(RecvQuiz(StudentQuiz, string)), this, SLOT(RecvQuiz(StudentQuiz, string)));
    disconnect(m_pAnsQuiz, SIGNAL(RecvAns(StudentQuiz, vector<bool>, unsigned int, string)), this, SLOT(RecvAns(StudentQuiz, vector<bool>, unsigned int, string)));
    delete m_pAnsQuiz;
    m_pAnsQuiz = nullptr;
    delete m_pProc;
    m_pProc = nullptr;
    delete ui;
}

//更新界面显示
void StudentWindow::UpdateControlEnabled(bool IsInClass) {
    //非上课状态，清空共享屏幕显示、清空聊天记录、清空聊天输入文本框
    if (!IsInClass) {
        ui->lblSharedScreen->clear();
        ui->cmbChatList->clear();
        ui->edtChat->clear();
    }
    //IP输入文本框在非上课状态可用
    ui->edtIP->setEnabled(!IsInClass);
    //进入课堂按钮在非上课状态可用
    ui->btnEnterClass->setEnabled(!IsInClass);
    //退出课堂按钮在上课状态可用
    ui->btnExitClass->setEnabled(IsInClass);
    //聊天输入框在上课状态可用
    ui->edtChat->setEnabled(IsInClass);
    //发送按钮在上课状态可用
    ui->btnSend->setEnabled(IsInClass);
}

//点击进入课堂
void StudentWindow::on_btnEnterClass_clicked() {
    //IP不为空在能进入课堂
    if (!ui->edtIP->text().isEmpty()) {
        //更新界面控件状态为非上课状态
        UpdateControlEnabled(false);
        //临时锁死IP输入框
        ui->edtIP->setEnabled(false);
        //临时锁死退出课堂按钮
        ui->btnExitClass->setEnabled(false);
        //尝试进入课堂
        m_pProc->EnterClass(ui->edtIP->text());
    }
    std::cout<< "enter";
}

//点击退出课堂
void StudentWindow::on_btnExitClass_clicked(){
    m_pProc->ExitClass();
}

//点击送出文字
void StudentWindow::on_btnSend_clicked(){
    //聊天输入框有文字才能发送
    if (!ui->edtChat->text().isEmpty()) {
        //发送结束则清空聊天输入框
        if (m_pProc->Send(ui->edtChat->text())) {
            ui->edtChat->clear();
        }
    }
}

//上课中禁止关闭窗口
void StudentWindow::closeEvent (QCloseEvent* e) {
    if (m_pProc->IsInClass) {
        e->ignore();
    }
}

//定时更新界面显示
void StudentWindow::TimerEvent() {
    if (m_pProc->IsInClass) {
        //上课中，窗口是激活状态
        if(this->isActiveWindow()) {
            this->setWindowTitle("学生 " + QString::fromStdString(User::GetLoginedUser()->GetName()) +  " 在线");
            m_pProc->dealConcen(true);
        }
        //上课中，窗口不是激活状态
        else {
            this->setWindowTitle("学生 " + QString::fromStdString(User::GetLoginedUser()->GetName()) +  " 离开");
            m_pProc->dealConcen(false);
        }
    }
    else {
        //非上课
        this->setWindowTitle("学生 " + QString::fromStdString(User::GetLoginedUser()->GetName()) +  " 未在课堂中");
    }
}

//进入课堂信号
void StudentWindow::ClassEntered(){
    UpdateControlEnabled(true);
}

//退出课堂信号
void StudentWindow::ClassExited(){
    UpdateControlEnabled(false);
    m_pProc->ExitClass();
}

//收到文字消息信号
void StudentWindow::RecvChat(QString Msg){
    ui->cmbChatList->insertItem(0, Msg);
    ui->cmbChatList->setCurrentIndex(0);
}

//收到屏幕共享消息信号
void StudentWindow::RecvScreen(QImage Img){
    //收到的图像尺寸和滚动区尺寸不一致，则调整滚动区尺寸
    if (ui->scrollAreaWidgetContents->minimumSize() != Img.size()) {
        ui->scrollAreaWidgetContents->setMinimumSize(Img.size());
    }
    //收到的图像尺寸和Label尺寸不一致，则调整Label尺寸
    if (ui->lblSharedScreen->minimumSize() != Img.size()) {
        ui->lblSharedScreen->setMinimumSize(Img.size());
    }
    //刷新Label显示
    ui->lblSharedScreen->setPixmap(QPixmap::fromImage(Img));
}


//收到题目
void StudentWindow::RecvQuiz(StudentQuiz aQuiz, string Name){
    //发出更新界面信号
    emit QuizInfo(aQuiz, Name);
    //跳出视窗
    m_pAnsQuiz->exec();
}

//收到回答
void StudentWindow::RecvAns(StudentQuiz aQuiz, vector<bool> isChe, unsigned int Time, string Owner){
    //关闭视窗
    m_pAnsQuiz->close();
    //送出回答内容
    m_pProc->RecvAns(aQuiz, isChe, Time, Owner);
}

//收到关闭视窗通知
void StudentWindow::closeWin(){
    m_pAnsQuiz->close();
}

