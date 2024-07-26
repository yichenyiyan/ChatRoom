#include "creategroupchatwindow.h"
#include "tcpclient.h"

#include <QScreen>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGuiApplication>

CreateGroupChatWindow::CreateGroupChatWindow(const QString& user, QWidget *parent) : QWidget(parent), CreateUser(user) {
    this->setFixedSize(400, 400);
    this->setWindowTitle(CreateUser + " to create a group");
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    groupNameLineEdit = new QLineEdit(this);
    groupNameLineEdit->setPlaceholderText("群聊名称");

    //groupDescLineEdit = new QLineEdit(this);
    //groupDescLineEdit->setPlaceholderText("群聊描述");

    createGroupButton = new QPushButton("创建群聊", this);

    layout->addWidget(groupNameLineEdit);
    //layout->addWidget(groupDescLineEdit);
    layout->addWidget(createGroupButton);

    connect(createGroupButton, &QPushButton::clicked, this, &CreateGroupChatWindow::onCreateGroupButtonClicked);

    sendSocket = new TcpClient();
}

CreateGroupChatWindow::~CreateGroupChatWindow() {
    delete  sendSocket;
}

void CreateGroupChatWindow::onCreateGroupButtonClicked() {
    QString groupName = groupNameLineEdit->text();
    //QString groupDesc = groupDescLineEdit->text();

    if (groupName.isEmpty()) {
        QMessageBox::warning(this, "错误", "群聊名称不能为空！");
        return;
    }

    // 发送创建群聊请求的命令到服务器
    QString command = "9 " + CreateUser + " " + groupName;
    sendSocket->sendCommand(command);

    connect(sendSocket, &TcpClient::receivedMessage, [&](){
        QString response = sendSocket->getMessage();

        if (response == "0") {
            QMessageBox::information(this, "创建成功", "群聊创建成功");
        } else if (response == "1") {
            QMessageBox::information(this, "创建失败", "群聊已存在");
        } else if (response == "2"){
            QMessageBox::information(this, "创建失败", "群聊创建失败");
        } else {
            QMessageBox::information(this, "创建失败", "Unknown response: " + response);
        }
        close();
    });
}

void CreateGroupChatWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}
