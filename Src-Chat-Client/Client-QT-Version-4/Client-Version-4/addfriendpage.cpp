#include "addfriendpage.h"
#include "menu.h"

AddFriendPage::AddFriendPage(Menu* _menu, QWidget *parent)
    : QWidget(parent), menu(_menu) {
    this->setFixedSize(400, 600);
    centerWindow();
    layout = new QVBoxLayout(this);

    friendNameInput = new QLineEdit(this);
    friendNameInput->setPlaceholderText("输入好友名称");
    layout->addWidget(friendNameInput);

    addButton = new QPushButton("添加好友", this);
    layout->addWidget(addButton);

    statusLabel = new QLabel(this);
    layout->addWidget(statusLabel);

    backButton = new QPushButton("返回", this);
    layout->addWidget(backButton);

    connect(addButton, &QPushButton::clicked, this, &AddFriendPage::onAddFriend);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        menu->show();
    });
}

AddFriendPage::~AddFriendPage() {}

void AddFriendPage::onAddFriend() {
    QString friendName = friendNameInput->text();
    if (!friendName.isEmpty()) {
        // 模拟添加好友到服务器
        // 在实际应用中，您将通过网络将好友请求发送到服务器
        statusLabel->setText("添加好友请求已发送: " + friendName);
    }
}
