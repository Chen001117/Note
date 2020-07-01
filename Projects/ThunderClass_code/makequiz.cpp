#include "makequiz.h"
#include "ui_makequiz.h"

MakeQuiz::MakeQuiz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeQuiz)
{
    ui->setupUi(this);
    m_pTimer = new QTimer();
    m_pTimer->setInterval(250);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    //只显示关闭窗口按钮，没有？按钮
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    m_pTimer->start();
}

MakeQuiz::~MakeQuiz()
{
    m_pTimer->stop();
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    delete m_pTimer;
    m_pTimer = nullptr;
    delete ui;
}

//按下送出问题按钮
void MakeQuiz::on_send_clicked()
{
    //得到问题描述
    QString ques = ui->Ques->text();
    //得到选项描述
    QLineEdit* optList[5] = {ui->Opt1, ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
    vector<QString> opts;
    for(unsigned int i = 0; i < 5; i++){
        opts.push_back(optList[i]->text());
    }
    //得到正确答案
    QCheckBox* cheList[5] = {ui->Che1, ui->Che2, ui->Che3, ui->Che4, ui->Che5};
    vector<bool> isAns;
    for(unsigned int i = 0; i < 5; i++){
        isAns.push_back(cheList[i]->isChecked());
    }
    //送出问题信号
    emit SendQuiz(ques, opts, isAns);
    //清空界面
    ui->Ques->setText(QString::fromStdString(""));
    for(unsigned int i = 0; i < 5; i++){
        optList[i]->setText(QString::fromStdString(""));
        cheList[i]->setChecked(false);
    }
}

//更新界面显示
void MakeQuiz::TimerEvent() {
    QLineEdit* optList[5] = {ui->Opt1, ui->Opt2, ui->Opt3, ui->Opt4, ui->Opt5};
    QCheckBox* cheList[5] = {ui->Che1, ui->Che2, ui->Che3, ui->Che4, ui->Che5};
    //选项为空->不能是答案
    for(int i = 0; i < 5; i++){
        if(optList[i]->text() == ""){
            cheList[i]->setEnabled(false);
        }
        else{
            cheList[i]->setEnabled(true);
        }
    }
    //上一个选项为空->无法输入选项描述
    for(int i = 1; i < 5; i++){
        if(optList[i-1]->text() == ""){
            optList[i]->setEnabled(false);
            cheList[i]->setChecked(false);
            cheList[i]->setEnabled(false);
        }else{
            optList[i]->setEnabled(true);
            cheList[i]->setEnabled(true);
        }
    }
    //没有答案或题目->无法输出
    bool haveAns = false;
    for(int i = 0; i < 5; i++){
        if(cheList[i]->isChecked() == true){
            haveAns = true;
            break;
        }
    }
    if(ui->Ques->text() == "" || haveAns == false){
        ui->send->setEnabled(false);
    }else{
        ui->send->setEnabled(true);
    }
}


