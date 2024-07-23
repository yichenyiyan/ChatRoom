#include "register.h"
#include "mainwindow.h"
#include "tcpclient.h"
#include "myMessage.h"
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>

Register::Register(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(400, 600);
    centerWindow();
    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("注册", this);
    backButton = new QPushButton("返回", this);
    registerButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    backButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);

    connect(registerButton, &QPushButton::clicked, this, &Register::on_registerButton_clicked);
    connect(backButton, &QPushButton::clicked, this, &Register::on_backButton_clicked);
    connect(this, &Register::back, this, &Register::on_backButton_clicked);

    tsocket = new TcpClient();
}

Register::~Register() {}

void Register::on_registerButton_clicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString command = "1 " + username + " " + password;

    tsocket->connectToServer("127.0.0.1", 8888);
    tsocket->sendCommand(command);
    QObject::connect(tsocket, &TcpClient::receivedMessage, [this](){
        QString msg = tsocket->getMessage();
        QString reason;
        if (msg == "0") {
            CustomMessageBox msgBox(this);
            msgBox.setText("注册成功！");
            msgBox.setStandardButtons(QMessageBox::Ok);
            if (msgBox.exec() == QMessageBox::Ok) {
                emit toBack();
                this->deleteLater();
            }
        } else if (msg == "1") {
            reason = "用户已存在";
        } else if (msg == "2") {
            reason = "服务器维护中";
        }
        if (!reason.isEmpty()) {
            CustomMessageBox msgBox(this);
            msgBox.setText("注册失败！\n" + reason);
            msgBox.setStandardButtons(QMessageBox::Ok);
            if (msgBox.exec() == QMessageBox::Ok) {
                emit toBack();
                this->deleteLater();
            }
        }
    });
}

void Register::on_backButton_clicked() {
    tsocket->destroySelf();
    this->close();
    emit toBack();
}
