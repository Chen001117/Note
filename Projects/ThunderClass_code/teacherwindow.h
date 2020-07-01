#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include <QShowEvent>
#include <QTimer>
#include "teacherprocess.h"
#include <QMetaType>
#include "quiz.h"
#include "makequiz.h"
#include "resultwindow.h"
#include "teacherquiz.h"

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit TeacherWindow(QWidget *parent = nullptr);
    //析构函数
    ~TeacherWindow();

private slots:
    //尝试关闭视窗触发
    void closeEvent (QCloseEvent* e) override;
    //是窗显示时触发
    void showEvent(QShowEvent* e) override;
    //按下开始课堂
    void on_btnBeginClass_clicked();
    //按下开始共享屏幕
    void on_ckbScreenShare_clicked();
    //按下分享音频
    void on_ckbVoiceShare_clicked();
    //按下结束课堂
    void on_btnEndClass_clicked();
    //按下发送文字
    void on_btnSend_clicked();
    //课堂开始
    void ClassBegined();
    //课堂结束
    void ClassEnded();
    //收到学生登入
    void RecvStudentLogin(QString Name);
    //收到学生登出
    void RecvStudentLogout(QString Name);
    //收到文本消息
    void RecvChat(QString Msg);
    //收到题目
    void RecvQuiz(QString ques, vector<QString> opts, vector<bool> isAns);
    //按下随机提问按钮
    void on_popquiz_clicked();
    //关闭创造题目是窗
    void CloseMakeQuiz();
    //按下出题钮
    void on_Quiz_clicked();
    //按下显示统计结果按钮
    void on_Result_clicked();
    //显示学生表现在视窗
    void Perfor(vector<Performance> aPer);

private:
    Ui::TeacherWindow *ui;
    //业务流程类
    TeacherProcess* m_pProc;
    //定时刷新控件是否可用
    QTimer* m_pTimer;
    //刷新控件是否可用
    void UpdateControlEnabled(bool IsInClass);
    //创建题目视窗
    MakeQuiz* m_pMakeQuiz;
    //显示结果视窗
    ResultWindow* m_pResWin;
};

#endif // TEACHERWINDOW_H
