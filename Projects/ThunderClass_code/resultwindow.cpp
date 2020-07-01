#include "resultwindow.h"
#include "ui_resultwindow.h"
#include <QDebug>

ResultWindow::ResultWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

//收到更新界面信号
void ResultWindow::changeRes(QString aStr){
    ui->label->setText(aStr);
}
