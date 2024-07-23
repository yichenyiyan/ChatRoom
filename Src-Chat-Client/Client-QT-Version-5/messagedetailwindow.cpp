#include "tcpclient.h"
#include "messagedetailwindow.h"
#include <QScrollBar>

MessageDetailWindow::MessageDetailWindow(const QString& _sendUsername, const QString& _friendName, QWidget *parent)
    : QWidget(parent), sendUsername(_sendUsername),friendUserName(_friendName) {
    sendSocket = new TcpClient();
    setupUI();

}

void MessageDetailWindow::setupUI() {
    setWindowTitle(friendUserName);
    setFixedSize(400, 600);
    setStyleSheet("background-color: white;");

    mainLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    messagesContainer = new QWidget();
    messagesLayout = new QVBoxLayout(messagesContainer);
    messagesContainer->setLayout(messagesLayout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(messagesContainer);
    mainLayout->addWidget(scrollArea);

    messageInput = new QTextEdit(this);
    messageInput->setFixedHeight(30);
    messageInput->setStyleSheet("border-radius: 15px; border: 1px solid gray;");

    sendButton = new QPushButton("发送", this);
    sendButton->setFixedSize(40, 30);
    sendButton->setStyleSheet("border-radius: 15px; background-color: darkblue; color: white;");

    backButton = new QPushButton("返回", this);
    backButton->setFixedSize(40, 30);
    backButton->setStyleSheet("border-radius: 15px; background-color: darkblue; color: white;");
    connect(backButton, &QPushButton::clicked, this, &MessageDetailWindow::hide);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(backButton);
    bottomLayout->addWidget(messageInput);
    bottomLayout->addWidget(sendButton);

    mainLayout->addLayout(bottomLayout);


    connect(sendButton, &QPushButton::clicked, this, [&]() {
        QString message = messageInput->toPlainText();
        if (!message.isEmpty()) {
            addMessage(friendUserName, message, true);
            messageInput->clear();
        }
        sendSocket->sendCommand("5 " + sendUsername + " " + friendUserName + " "+ message);

    });
}

void MessageDetailWindow::addMessage(const QString &username, const QString &message, bool isOwnMessage) {
    if (username != friendUserName) return;

    QLabel *messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    messageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  
    messageLabel->setStyleSheet(isOwnMessage ? "background-color: green; color: white; border-radius: 10px; border: 1px dashed black;" : "background-color: pink; color: black; border-radius: 10px; border: 1px dashed black;");
    messagesLayout->addWidget(messageLabel);
    messagesLayout->addStretch();
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
}
