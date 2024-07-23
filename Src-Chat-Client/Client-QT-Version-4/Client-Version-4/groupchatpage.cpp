#include "groupchatpage.h"
#include "menu.h"

GroupChatPage::GroupChatPage(const QString &groupName, Menu* _menu, QWidget *parent)
    : QWidget(parent), groupName(groupName) , menu(_menu) {
    this->setFixedSize(400, 600);
    centerWindow();

    layout = new QVBoxLayout(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    messageInput = new QLineEdit(this);
    layout->addWidget(messageInput);

    sendButton = new QPushButton("发送", this);
    layout->addWidget(sendButton);

    backButton = new QPushButton("返回", this);
    layout->addWidget(backButton);

    connect(sendButton, &QPushButton::clicked, this, &GroupChatPage::onSendMessage);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        menu->show();
    });
}

GroupChatPage::~GroupChatPage() {}

void GroupChatPage::onSendMessage() {
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        chatDisplay->append("我: " + message);
        messageInput->clear();

        // 模拟发送消息到服务器
        // 在实际应用中，您将通过网络将消息发送到服务器
    }
}
