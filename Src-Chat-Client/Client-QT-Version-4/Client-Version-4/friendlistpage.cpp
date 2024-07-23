#include "friendlistpage.h"
#include "privatechatpage.h"

FriendListPage::FriendListPage(Menu* _menu, TcpClient* _tsocket, const QString& _user, QWidget *parent)
    : QWidget(parent), menu(_menu), tsocket(_tsocket), user(_user) {
    this->setFixedSize(400, 600);
    centerWindow();

    layout = new QVBoxLayout(this);

    friendListWidget = new QListWidget(this);
    layout->addWidget(friendListWidget);
    /*
    friendListWidget->addItem(QString("jiedan"));
    friendListWidget->addItem(QString("yichen"));*/

    backButton = new QPushButton("返回", this);
    layout->addWidget(backButton);

    connect(friendListWidget, &QListWidget::itemClicked, this, &FriendListPage::onFriendClicked);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        this->close();
        menu->show();
    });

    loadFriendList();
}

FriendListPage::~FriendListPage() {}

void FriendListPage::onFriendClicked(QListWidgetItem *item) {
    QString friendName = item->text();
    this->hide();
    PrivateChatPage *privateChatPage = new PrivateChatPage(this, menu, tsocket, user, friendName);
    privateChatPage->show();
}

void FriendListPage::loadFriendList() {
    // 从服务器获取用户好友数据
    QString cmd = "";
    tsocket->sendCommand(cmd);
    QObject::connect(tsocket, &TcpClient::receivedMessage, [&]() {
        // 模拟从服务器获取到好友列表数据, 在这里添加一下模拟数据
        QStringList friends = {"jiedan", "yichen"};
        friendListWidget->clear();
        for (const QString& friendName : friends) {
            friendListWidget->addItem(friendName);
        }
    });
}
