#ifndef ANSQUIZ_H
#define ANSQUIZ_H

#include <QDialog>
#include <QTimer>
#include <QMetaType>
#include "studentquiz.h"

namespace Ui {
class AnsQuiz;
}

class AnsQuiz : public QDialog
{
    Q_OBJECT

public:
    explicit AnsQuiz(QWidget *parent = nullptr);
    ~AnsQuiz();

private slots:
    void on_send_clicked();
    void TimerEvent();
    //收到题目信息
    void QuizInfo(StudentQuiz aQuiz, string Name);
    //选择选项1
    void on_Opt1_stateChanged(int arg1);
    //选择选项2
    void on_Opt2_stateChanged(int arg1);
    //选择选项3
    void on_Opt3_stateChanged(int arg1);
    //选择选项4
    void on_Opt4_stateChanged(int arg1);
    //选择选项5
    void on_Opt5_stateChanged(int arg1);
    //收到提前收题信号
    void RecvStop();

signals:
    //将目前的答案传出去
    void RecvAns(StudentQuiz aQuiz, vector<bool> isChe, unsigned int Time, string Owner);
    //请求关闭此窗口
    void closeWin();

private:
    Ui::AnsQuiz *ui;
    //此窗口显示的题目
    StudentQuiz* m_aQuiz;
    //目前的费时
    unsigned int m_uiTime;
    //计时器
    QTimer* m_pTimer;
    //此题的回答者姓名
    string m_sOwner;
    //表示是否为多选题
    bool m_bIsMuti;
    //表示目前是否处于回答状态
    bool m_bIsAnsering;
};

#endif // ANSQUIZ_H
