#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include "adminprocess.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    //构造函数
    explicit AdminWindow(QWidget *parent = nullptr);
    //析构函数
    ~AdminWindow();
private slots:
    //按下新增用户按钮
    void on_Add_clicked();
    //按下移除用户按钮
    void on_Delete_clicked();

private:
    Ui::AdminWindow *ui;
    AdminProcess* m_pProc;
    QTimer* m_pTimer;

signals:
    void addUser(QString, QString, QString);

};

#endif // ADMINWINDOW_H
