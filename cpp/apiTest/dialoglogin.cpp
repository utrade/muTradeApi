#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include <testStrategy.h>
#include "processapiresponses.h"
#define host "10.0.0.31"
#define port 8286
#include <Logger.h>
DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    ui->lineEditPswd->setEchoMode(QLineEdit::Password);
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::clearForm()
{
    ui->lineEditUserId->clear();
    ui->lineEditPswd->clear();
    uid =0;
    pswd="";
}

void DialogLogin::on_pushButtonLogin_clicked()
{

    uid = ui->lineEditUserId->text().toInt();
    pswd = ui->lineEditPswd->text().toStdString();
    this->accept();

}
void DialogLogin::reqLogin(mutrade::Int32 user, const std::string &pswd, bool restoreState)
{
    mutrade::String _host = host;
    LOGGER_INFO("Login Request Sent");
    mutrade::Context::getInstance()->login(user,pswd,_host, port,restoreState);
}

void DialogLogin::reqLogout()
{
    LOGGER_INFO("Logout Request Sent");
    mutrade::Context::getInstance()->logout();
}
