#include "tcpclient.h"
#include "joingrouprequestwindow.h"
#include <QMessageBox>

JoinGroupRequestWindow::JoinGroupRequestWindow(const QString& _user, QWidget *parent)
    : QWidget(parent), user(_user),
      groupnameEdit(new QLineEdit(this)),
      submitButton(new QPushButton("Submit", this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);


    layout->addWidget(new QLabel("Groupname:", this));
    layout->addWidget(groupnameEdit);

    layout->addWidget(submitButton);
    sendSocket = new TcpClient();

    connect(submitButton, &QPushButton::clicked, this, &JoinGroupRequestWindow::onSubmit);

    setLayout(layout);
    setWindowTitle("Join Group Request");
    resize(300, 200);
}

void JoinGroupRequestWindow::onSubmit()
{
    QString groupname = groupnameEdit->text();

    if (groupname.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both username and groupname.");
        return;
    }

    sendSocket->sendCommand("a " + user + " " + groupname);

    // 这里你可以添加发送申请给群主的逻辑，例如发送网络请求

    connect(sendSocket, &TcpClient::receivedMessage, [&](){
        QString response = sendSocket->getMessage();

        if (response == "3") {
            QMessageBox::information(this, "请求结果", "发送成功");
        } else if (response == "1") {
            QMessageBox::information(this, "请求结果", "已加入该群聊");
        } else if (response == "0"){
            QMessageBox::information(this, "请求错误", "群聊不存在");
        } else if (response == "2"){
            QMessageBox::information(this, "请求结果", "群主不在线");
        } else {
            QMessageBox::information(this, "请求结果", "Unknown response: " + response);
        }
        close();
    });

    // 清空输入框
    groupnameEdit->clear();
    close();
}
