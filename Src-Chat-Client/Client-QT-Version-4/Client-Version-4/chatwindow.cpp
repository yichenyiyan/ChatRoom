#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent) {
    setFixedSize(400, 600);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet("background: #f5f5f5;");

    messageInput = new QTextEdit(this);
    messageInput->setFixedHeight(60);

    sendButton = new QPushButton("发送", this);
    sendButton->setStyleSheet("QPushButton { background: transparent; color: black; border: none; }");

    returnButton = new QPushButton("返回", this);
    returnButton->setStyleSheet("QPushButton { background: transparent; color: black; border: none; }");

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(returnButton);
    bottomLayout->addWidget(messageInput);
    bottomLayout->addWidget(sendButton);

    mainLayout->addWidget(chatDisplay);
    mainLayout->addLayout(bottomLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendButtonClicked);
    connect(returnButton, &QPushButton::clicked, this, &ChatWindow::onReturnButtonClicked);
}

void ChatWindow::setChatDetails(const QString &details) {
    chatDisplay->setPlainText(details);
}

void ChatWindow::onSendButtonClicked() {
    // Handle sending message
}

void ChatWindow::onReturnButtonClicked() {
    this->close();
}
