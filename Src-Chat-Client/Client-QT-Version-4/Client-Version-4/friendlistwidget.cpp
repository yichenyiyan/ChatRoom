#include "friendlistwidget.h"

FriendListWidget::FriendListWidget(TcpClient* tsocket, QWidget *parent)
    : QWidget(parent), tsocket(tsocket) {
    layout = new QVBoxLayout(this);
    friendListWidget = new QListWidget(this);
    layout->addWidget(friendListWidget);
    setLayout(layout);

    connect(tsocket, &TcpClient::receivedMessage, this, &FriendListWidget::displayFriends);
    fetchFriends();
}

void FriendListWidget::fetchFriends() {
    // 向服务器请求好友列表
    QString command = "GET_FRIENDS";
    tsocket->sendCommand(command);
    displayFriends("yichen jiedan xvlhua qsm");
}

void FriendListWidget::displayFriends(const QString &friendsData) {
    QStringList friendList = friendsData.split(" ");
    for (int i = 0; i < friendList.size() && i < 100; ++i) {
        QListWidgetItem *item = new QListWidgetItem(friendList.at(i), friendListWidget);
        item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
}
