#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <QDebug>


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    //初始化
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    m_pProc = new LoginProcess(this);
    //连接信号
    connect(m_pProc, SIGNAL(closeWin()), this, SLOT(closeWin()));
    //设定图片
    QImage image(QString::fromStdString("login.jpg"));
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->resize(image.width(), image.height());
}

LoginWindow::~LoginWindow()
{
    disconnect(m_pProc, SIGNAL(closeWin()), this, SLOT(closeWin()));
    delete m_pProc;
    m_pProc = nullptr;
    delete ui;
}

void LoginWindow::on_btnLogin_clicked()
{
    //初始化
    QMessageBox msgBox;
    TeacherWindow* pTeacherWindow;
    StudentWindow* pStudentWindow;
    AdminWindow* pAdminWindow;
    //取得使用者类型
    LoginResult res = m_pProc->Login(ui->edtName->text(), ui->edtPassword->text());
    switch (res) {
    case LR_NOUSER :
        //如果是空指针，登入失败
        msgBox.setText("登录失败");
        msgBox.exec();
        break;
    case LR_ADMIN :
        //如果非空指针，弹出相关视窗
        this->setWindowOpacity(0.0);
        pAdminWindow = new AdminWindow(this);
        pAdminWindow->exec();
        this->setWindowOpacity(1.0);
        delete pAdminWindow;
        pAdminWindow = nullptr;
        break;
    case LR_TEACHER :
        //如果非空指针，弹出相关视窗
        this->setWindowOpacity(0.0);
        pTeacherWindow = new TeacherWindow(this);
        pTeacherWindow->setWindowTitle("教师 " + QString(User::GetLoginedUser()->GetName().c_str()));
        pTeacherWindow->exec();
        this->setWindowOpacity(1.0);
        delete  pTeacherWindow;
        pTeacherWindow = nullptr;
        break;
    default:
        //如果非空指针，弹出相关视窗
        this->setWindowOpacity(0.0);
        pStudentWindow = new StudentWindow(this);
        pStudentWindow->exec();
        this->setWindowOpacity(1.0);
        delete pStudentWindow;
        pStudentWindow = nullptr;
    }
}

//按下离开，关闭视窗
void LoginWindow::on_btnExit_clicked()
{
    this->close();
}

//收到请求关闭信号，关闭视窗
void LoginWindow::closeWin()
{
    this->close();
}
