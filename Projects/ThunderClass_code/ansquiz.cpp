#include "ansquiz.h"
#include "ui_ansquiz.h"
#include <QDebug>
AnsQuiz::AnsQuiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnsQuiz)
{
    ui->setupUi(this);
    m_aQuiz = new StudentQuiz();
    //设定计时器
    m_pTimer = new QTimer();
    m_pTimer->setInterval(1000);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    m_pTimer->start();
    //注册
    qRegisterMetaType<StudentQuiz>("StudentQuiz");
    qRegisterMetaType<StudentQuiz>("StudentQuiz&");
    qRegisterMetaType<string>("string");
    qRegisterMetaType<string>("string&");
    //连接信号
    connect(parent, SIGNAL(QuizInfo(StudentQuiz, string)), this, SLOT(QuizInfo(StudentQuiz, string)));
    //只显示关闭窗口按钮，没有？按钮
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    //初始化使用时间
    m_uiTime = 0;
    m_bIsMuti = true;
    m_bIsAnsering = false;
}

AnsQuiz::~AnsQuiz()
{
    m_pTimer->stop();
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    delete m_pTimer;
    if(m_aQuiz != nullptr){
        delete m_aQuiz;
        m_aQuiz = nullptr;
    }
    delete ui;
}

//送出你的回答
void AnsQuiz::on_send_clicked()
{
    //如果在回答状态才能送出
    if(m_bIsAnsering == true){
        QCheckBox* optList[5] = {ui->Opt1, ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
        //得到答题数据
        vector<bool> isChe;
        for(unsigned int i = 0; i < m_aQuiz->optNum; i++){
            isChe.push_back(optList[i]->isChecked());
        }
        //回传信号
        emit RecvAns(*m_aQuiz, isChe, m_uiTime, m_sOwner);
        //清空视窗
        m_bIsAnsering = false;
        ui->Ques->setText(QString::fromStdString(""));
        for(unsigned int i = 0; i < m_aQuiz->optNum; i++){
            optList[i]->setText(QString::fromStdString(""));
            optList[i]->setChecked(false);
        }
    }
}

//收到新的题目，更新界面
void AnsQuiz::QuizInfo(StudentQuiz aQuiz, string Owner){
    //初始化
    QCheckBox* optList[5] = {ui->Opt1, ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
    for(unsigned int i = 0; i < 5; i++){
        optList[i]->setText("");
        optList[i]->setEnabled(false);
    }
    m_bIsAnsering = true;
    m_uiTime = 0;
    //赋值
    m_sOwner = Owner;
    *m_aQuiz = aQuiz;
    m_bIsMuti = aQuiz.isMuti;
    //改题目名字
    string isMuti = (aQuiz.isMuti == true ? " (多选)" : " (单选)");
    ui->Ques->setText(QString::fromStdString(m_aQuiz->Question + isMuti));
    //改选项名字
    vector<string> strList;
    m_aQuiz->get_opts(strList);
    for(unsigned int i = 0; i < 5; i++){
        if(i < m_aQuiz->optNum){
            optList[i]->setEnabled(true);
            optList[i]->setText(QString::fromStdString(strList[i]));
        }else{
            optList[i]->setEnabled(false);
        }
    }
}

//如果在答题状态就开始计时
void AnsQuiz::TimerEvent() {
    if(m_bIsAnsering == true){
        m_uiTime++;
    }
}

//收到提前结束信息
void AnsQuiz::RecvStop(){
    //如果有尚未传出的答案，将他传出
    this->on_send_clicked();
    //如果还处于答题状态，清空视窗并关闭
    if(m_bIsAnsering == true){
        //清空视窗
        m_bIsAnsering = false;
        ui->Ques->setText(QString::fromStdString(""));
        for(unsigned int i = 0; i < m_aQuiz->optNum; i++){
            QCheckBox* optList[5] = {ui->Opt1, ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
            optList[i]->setText(QString::fromStdString(""));
            optList[i]->setChecked(false);
        }
        m_uiTime = 0;
        //关闭视窗
        emit closeWin();
    }
}

//以下函数处理单选题，一次只能选一个项目
void AnsQuiz::on_Opt1_stateChanged(int arg1)
{
    if(ui->Opt1->isChecked() == true  && m_bIsMuti == false){
        QCheckBox* optList[4] = {ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
        for(int i = 0; i < 4; i++){
            optList[i]->setChecked(false);
        }
    }
}

void AnsQuiz::on_Opt2_stateChanged(int arg1)
{
    if(ui->Opt2->isChecked() == true  && m_bIsMuti == false){
        QCheckBox* optList[4] = {ui->Opt1, ui->Opt3, ui->Opt4, ui->Opt5};
        for(int i = 0; i < 4; i++){
            optList[i]->setChecked(false);
        }
    }
}

void AnsQuiz::on_Opt3_stateChanged(int arg1)
{
    if(ui->Opt3->isChecked() == true  && m_bIsMuti == false){
        QCheckBox* optList[4] = {ui->Opt2, ui->Opt1, ui->Opt4, ui->Opt5};
        for(int i = 0; i < 4; i++){
            optList[i]->setChecked(false);
        }
    }
}

void AnsQuiz::on_Opt4_stateChanged(int arg1)
{
    if(ui->Opt4->isChecked() == true  && m_bIsMuti == false){
        QCheckBox* optList[4] = {ui->Opt2, ui->Opt3, ui->Opt1, ui->Opt5};
        for(int i = 0; i < 4; i++){
            optList[i]->setChecked(false);
        }
    }
}

void AnsQuiz::on_Opt5_stateChanged(int arg1)
{
    if(ui->Opt5->isChecked() == true  && m_bIsMuti == false){
        QCheckBox* optList[4] = {ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt1};
        for(int i = 0; i < 4; i++){
            optList[i]->setChecked(false);
        }
    }
}

