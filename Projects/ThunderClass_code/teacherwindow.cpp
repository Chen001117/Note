#include "teacherwindow.h"
#include "ui_teacherwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QSize>

//构造函数
TeacherWindow::TeacherWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeacherWindow){
    //初始化
    ui->setupUi(this);
    m_pProc = new TeacherProcess(this);
    m_pMakeQuiz = new MakeQuiz(this);
    m_pResWin = new ResultWindow(this);
    //注册
    qRegisterMetaType<Performance>("Performance");
    qRegisterMetaType<Performance>("Performance&");
    qRegisterMetaType<vector<Performance> >("vector<Performance>");
    qRegisterMetaType<vector<Performance> >("vector<Performance>&");
    qRegisterMetaType<vector<QString> >("vector<QString>");
    qRegisterMetaType<vector<QString> >("vector<QString>&");
    qRegisterMetaType<vector<bool> >("vector<bool>");
    qRegisterMetaType<vector<bool> >("vector<bool>&");
    qRegisterMetaType<unsigned int>("unsigned int");
    qRegisterMetaType<unsigned int>("unsigned int&");
    qRegisterMetaType<TeacherQuiz>("TeacherQuiz");
    qRegisterMetaType<TeacherQuiz>("TeacherQuiz&");
    //建立信号和槽的连接关系
    //一定要在m_pProc构造后才能建立
    //connect是建立联系，而非调用槽函数
    connect(m_pProc, SIGNAL(ClassBegined()), this, SLOT(ClassBegined()));
    connect(m_pProc, SIGNAL(ClassEnded()), this, SLOT(ClassEnded()));
    connect(m_pProc, SIGNAL(CloseMakeQuiz()), this, SLOT(CloseMakeQuiz()));
    connect(m_pProc, SIGNAL(RecvStudentLogin(QString)), this, SLOT(RecvStudentLogin(QString)));
    connect(m_pProc, SIGNAL(RecvStudentLogout(QString)), this, SLOT(RecvStudentLogout(QString)));
    connect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));
    connect(m_pProc, SIGNAL(changeRes(QString)), m_pResWin, SLOT(changeRes(QString)));
    connect(m_pMakeQuiz, SIGNAL(SendQuiz(QString, vector<QString>, vector<bool>)), this, SLOT(RecvQuiz(QString, vector<QString>, vector<bool>)));
    connect(m_pProc, SIGNAL(Perfor(vector<Performance>)), this, SLOT(Perfor(vector<Performance>)));
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

TeacherWindow::~TeacherWindow(){
    //先disconnected，再释放业务流程类
    disconnect(m_pProc, SIGNAL(ClassBegined()), this, SLOT(ClassBegined()));
    disconnect(m_pProc, SIGNAL(CloseMakeQuiz()), this, SLOT(CloseMakeQuiz()));
    disconnect(m_pProc, SIGNAL(ClassEnded()), this, SLOT(ClassEnded()));
    disconnect(m_pProc, SIGNAL(RecvStudentLogin(QString)), this, SLOT(RecvStudentLogin(QString)));
    disconnect(m_pProc, SIGNAL(RecvStudentLogout(QString)), this, SLOT(RecvStudentLogout(QString)));
    disconnect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));
    disconnect(m_pMakeQuiz, SIGNAL(SendQuiz(QString, vector<QString>, vector<bool>)), this, SLOT(RecvQuiz(QString, vector<QString>, vector<bool>)));
    disconnect(m_pProc, SIGNAL(changeRes(QString)), m_pResWin, SLOT(changeRes(QString)));
    disconnect(m_pProc, SIGNAL(Perfor(vector<Performance>)), this, SLOT(Perfor(vector<Performance>)));
    delete m_pProc;
    delete m_pMakeQuiz;
    delete m_pResWin;
    delete ui;
}

//定时设定控件是否可用
void TeacherWindow::UpdateControlEnabled(bool IsInClass){
    if (!IsInClass) {
        ui->cmbStudent->clear();
        ui->cmbChatList->clear();
        ui->edtChat->clear();
        ui->ckbScreenShare->setChecked(false);
        ui->ckbVoiceShare->setChecked(false);
    }
    ui->btnBeginClass->setEnabled(!IsInClass);
    ui->btnEndClass->setEnabled(IsInClass);
    ui->btnSend->setEnabled(IsInClass);
    ui->ckbVoiceShare->setEnabled(IsInClass);
    ui->ckbScreenShare->setEnabled(IsInClass);
    ui->edtChat->setEnabled(IsInClass);
    ui->popquiz->setEnabled(IsInClass);
    ui->Quiz->setEnabled(IsInClass);
    ui->Result->setEnabled(IsInClass);
}

//视窗显示时触发(将是窗移动到指定位置)
void TeacherWindow::showEvent(QShowEvent* e) {
    int iWidth = QGuiApplication::screens()[0]->size().width();
    this->move((iWidth - this->width()) / 2, 0);
    e->accept();
}

//无法点击关闭视窗
void TeacherWindow::closeEvent (QCloseEvent* e) {
    if (m_pProc->IsInClass) {
        e->ignore();
    }
}

//按下开始上课
void TeacherWindow::on_btnBeginClass_clicked() {
    if (m_pProc->BeginClass()) {
        ui->cmbStudent->clear();
        ui->cmbChatList->clear();
    }
}

//按下共享屏幕
void TeacherWindow::on_ckbScreenShare_clicked()
{
    if (ui->ckbScreenShare->isChecked()) {
        ui->ckbScreenShare->setChecked(m_pProc->StartScreenShare());
    }
    else{
        ui->ckbScreenShare->setChecked(m_pProc->StopScreenShare());
    }
}

//按下分享语音
void TeacherWindow::on_ckbVoiceShare_clicked()
{
    if (ui->ckbVoiceShare->isChecked()) {
        ui->ckbVoiceShare->setChecked(m_pProc->StartVoiceShare());
    }
    else{
        ui->ckbVoiceShare->setChecked(m_pProc->StopVoiceShare());
    }
}

//按下下课
void TeacherWindow::on_btnEndClass_clicked(){
    m_pProc->EndClass();
}

//按下送出文本
void TeacherWindow::on_btnSend_clicked(){
    if (m_pProc->SendStrChat(ui->edtChat->text())) {
        ui->edtChat->clear();
    }
}

//课堂开始信号
void TeacherWindow::ClassBegined(){
    UpdateControlEnabled(true);
}

//课堂结束信号
void TeacherWindow::ClassEnded() {
    UpdateControlEnabled(false);
}

//接收到用户登入消息
void TeacherWindow::RecvStudentLogin(QString Name){
    int idx = ui->cmbStudent->findText(Name);
    if (idx == -1) {
        ui->cmbStudent->insertItem(0, Name);
        idx  = 0;
    }
    ui->cmbStudent->setCurrentIndex(idx);
}

//接收到用户登出消息
void TeacherWindow::RecvStudentLogout(QString Name) {
    int idx = ui->cmbStudent->findText(Name);
    if (idx != -1) {
        ui->cmbStudent->removeItem(idx);
    }
}

//收到文本信号
void TeacherWindow::RecvChat(QString Msg){
    ui->cmbChatList->insertItem(0, Msg);
    ui->cmbChatList->setCurrentIndex(0);
}

//按下随机抽考
void TeacherWindow::on_popquiz_clicked()
{
    m_pProc->popQuiz();
}

//收到问题
void TeacherWindow::RecvQuiz(QString ques, vector<QString> opts, vector<bool> isAns){
    m_pProc->getQuiz(ques,opts,isAns);
    ui->Quiz->setText(QString::fromStdString("收题"));
}

//关闭问题设计界面
void TeacherWindow::CloseMakeQuiz(){
    m_pMakeQuiz->close();
}

//按下出题
void TeacherWindow::on_Quiz_clicked()
{
    if(ui->Quiz->text() == "出题"){
        m_pMakeQuiz->setModal(false);
        m_pMakeQuiz->exec();
    }else{
        m_pProc->stopAns();
        ui->Quiz->setText(QString::fromStdString("出题"));
    }
}

//按下显示结果
void TeacherWindow::on_Result_clicked()
{
    m_pResWin->exec();
}

//显示学生表现
void TeacherWindow::Perfor(vector<Performance> aPer){
    qDebug() << "aPer Size " << aPer.size();
    for(unsigned int i = 0; i < aPer.size(); i++){
        string msg = aPer[i].getString();
        ui->cmbChatList->insertItem(0, QString::fromStdString(msg));
        ui->cmbChatList->setCurrentIndex(0);
    }
}

