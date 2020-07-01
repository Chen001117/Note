#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include "teacherquiz.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit ResultWindow(QWidget *parent = nullptr);
    //析构函数
    ~ResultWindow();

private slots:
    //收到更新界面信号
    void changeRes(QString aStr);

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
