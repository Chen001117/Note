#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QMetaType>
#include "studentprocess.h"
#include "ansquiz.h"

namespace Ui {
class StudentWindow;
}

class StudentWindow : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit StudentWindow(QWidget *parent = nullptr);
    //析构函数
    ~StudentWindow();
    //确保上课中无法关闭视窗
    void closeEvent (QCloseEvent* e) override;
    //更新界面的显示
    void UpdateControlEnabled(bool IsInClass);
private slots:
    //点击进入课堂
    void on_btnEnterClass_clicked();
    //点击退出课堂
    void on_btnExitClass_clicked();
    //点击送出文字信息
    void on_btnSend_clicked();
    //计时器触发事件
    void TimerEvent();
    //进入课堂信号
    void ClassEntered();
    //退出课堂信号
    void ClassExited();
    //收到文字消息信号
    void RecvChat(QString Msg);
    //收到屏幕共享消息信号
    void RecvScreen(QImage Img);
    //收到题目
    void RecvQuiz(StudentQuiz aQues, string Name);
    //收到回答
    void RecvAns(StudentQuiz aQuiz, vector<bool> isChe, unsigned int Time, string Owner);
    //收到关闭视窗通知
    void closeWin();

private:
    Ui::StudentWindow *ui;
    //业务流程类
    StudentProcess* m_pProc;
    //计时器对象
    QTimer* m_pTimer;
    //回答问题界面
    AnsQuiz* m_pAnsQuiz;

signals:
    //得到新的问题，提示更新界面
    void QuizInfo(StudentQuiz aQuiz, string Name);
};

#endif // STUDENTWINDOW_H
