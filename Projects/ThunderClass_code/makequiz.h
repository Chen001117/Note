#ifndef MAKEQUIZ_H
#define MAKEQUIZ_H

#include <QDialog>
#include <QTimer>
#include "quiz.h"

namespace Ui {
class MakeQuiz;
}

class MakeQuiz : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit MakeQuiz(QWidget *parent = nullptr);
    //析构函数
    ~MakeQuiz();

private slots:
    //按下送出问题按钮
    void on_send_clicked();
    //更新窗口显示
    void TimerEvent();

signals:
    //送出问题信号
    void SendQuiz(QString, vector<QString>, vector<bool>);

private:
    Ui::MakeQuiz *ui;
    //计时器，处理更新窗口问题
    QTimer* m_pTimer;
};

#endif // MAKEQUIZ_H
