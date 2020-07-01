#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QMetaType>
#include "teacherwindow.h"
#include "studentwindow.h"
#include "adminwindow.h"
#include "loginprocess.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    //构造函数
    LoginWindow(QWidget *parent = nullptr);
    //析构函数
    ~LoginWindow();

private slots:
    //按下登入按钮触发
    void on_btnLogin_clicked();
    //按下离开按钮触发
    void on_btnExit_clicked();
    //请求关闭视窗
    void closeWin();

private:
    Ui::LoginWindow *ui;
    //业务流程类
    LoginProcess* m_pProc;
};
#endif // LOGINWINDOW_H
