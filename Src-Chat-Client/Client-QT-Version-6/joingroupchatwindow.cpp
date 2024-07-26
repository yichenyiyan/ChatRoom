#include "joingroupchatwindow.h"
#include "tcpclient.h"

#include <QScreen>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGuiApplication>

JoinGroupChatWindow::JoinGroupChatWindow(const QString& user, QWidget *parent) : QWidget(parent), JoinUser((user)) {
    this->setFixedSize(400, 400);
    this->setWindowTitle(JoinUser + " to join group");
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    groupNameLineEdit = new QLineEdit(this);
    groupNameLineEdit->setPlaceholderText("请输入群聊名称");
    connect(groupNameLineEdit, &QLineEdit::textChanged, this, &JoinGroupChatWindow::on_GroupLineEdit_textChanged);

    joinGroupButton = new QPushButton("加入群聊", this);
    joinGroupButton->setEnabled(false);

    layout->addWidget(groupNameLineEdit);
    layout->addWidget(joinGroupButton);

    connect(joinGroupButton, &QPushButton::clicked, this, &JoinGroupChatWindow::onJoinGroupButtonClicked);

    sendSocket = new TcpClient();
}

JoinGroupChatWindow::~JoinGroupChatWindow() {
    delete sendSocket;
}

void JoinGroupChatWindow::onJoinGroupButtonClicked() {
    QString groupName = groupNameLineEdit->text();
    if (groupName.isEmpty()) {
        QMessageBox::warning(this, "错误", "群聊名称不能为空！");
        return;
    }

    // 发送加入群聊的命令到服务器
    QString command = "a " + JoinUser + " " + groupName;
    sendSocket->sendCommand(command);

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
}

void JoinGroupChatWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void JoinGroupChatWindow::on_GroupLineEdit_textChanged(const QString &text) {
    joinGroupButton->setEnabled(!text.isEmpty());
}
