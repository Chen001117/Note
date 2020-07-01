#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    m_pProc = new AdminProcess();
    ui->Type->addItem(QString::fromStdString("Teacher"), 0);
    ui->Type->addItem(QString::fromStdString("Student"), 1);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

AdminWindow::~AdminWindow()
{
    delete m_pProc;
    m_pProc = nullptr;
    delete ui;
}

//创建帐户
void AdminWindow::on_Add_clicked()
{
    QString account = ui->Account->text();
    QString password = ui->Password->text();
    QString type = ui->Type->currentText();
    bool isAdd = m_pProc->addUser(account, password, type);
    if(isAdd == true){
        ui->Account->setText("");
        ui->Password->setText("");
        QMessageBox m_msgBox;
        m_msgBox.setText("创建成功");
        m_msgBox.exec();
    }else{
        QMessageBox m_msgBox;
        m_msgBox.setText("创建失败");
        m_msgBox.exec();
    }
}

//删除帐户
void AdminWindow::on_Delete_clicked()
{
    QString account = ui->Account->text();
    QString password = ui->Password->text();
    QString type = ui->Type->currentText();
    bool isAdd = m_pProc->delUser(account, password, type);
    if(isAdd == true){
        ui->Account->setText("");
        ui->Password->setText("");
        QMessageBox m_msgBox;
        m_msgBox.setText("删除成功");
        m_msgBox.exec();
    }else{
        QMessageBox m_msgBox;
        m_msgBox.setText("删除失败");
        m_msgBox.exec();
    }
}
