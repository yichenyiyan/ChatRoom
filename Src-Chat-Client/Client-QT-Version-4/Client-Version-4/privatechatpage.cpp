#include "privatechatpage.h"
#include <QDebug>

PrivateChatPage::PrivateChatPage(FriendListPage* _prevpage, Menu* _menu, TcpClient* _tsocket, const QString& username, const QString &friendName, QWidget *parent)
    : QWidget(parent), friendName(friendName), menu(_menu), tsocket(_tsocket), user(username), prevpage(_prevpage) {
    this->setFixedSize(400, 600);
    centerWindow();

    layout = new QVBoxLayout(this);

    // 聊天展示区
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    // 消息输入区
    messageInput = new QLineEdit(this);
    layout->addWidget(messageInput);

    // 按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    sendButton = new QPushButton("发送", this);
    backButton = new QPushButton("返回", this);

    // 设置按钮透明度
    backButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");
    sendButton->setStyleSheet("background-color: rgba(255, 255, 255, 0.5); border: none;");

    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(sendButton);
    layout->addLayout(buttonLayout);

    connect(sendButton, &QPushButton::clicked, this, &PrivateChatPage::onSendMessage);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        prevpage->show();
    });

    connect(tsocket, &TcpClient::receivedMessage, this, &PrivateChatPage::onReceivedMessage);
}

PrivateChatPage::~PrivateChatPage() {}

void PrivateChatPage::onSendMessage() {
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        // 右边开始显示用户发送的消息
        chatDisplay->append(QString("<div style='text-align: right;'>Me: %1</div>").arg(message));
        messageInput->clear();

        QString cmd = "5 " + user + " " + friendName + " " + message;
        tsocket->sendCommand(cmd);
    }
}

void PrivateChatPage::onReceivedMessage() {
    QString msg = tsocket->getMessage();
    if (msg.left(1) == "0") {
        qDebug() << "He/She is not online";
    } else if (msg.left(1) == "1") {
        qDebug() << "He/She is not your friend";
    } else {
        // 左边开始显示接收到的好友消息
        chatDisplay->append(QString("<div style='text-align: left;'>%1: %2</div>").arg(friendName, msg));
    }
}
