#ifndef FRIENDLISTWIDGET_H
#define FRIENDLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include "tcpclient.h"

class FriendListWidget : public QWidget {
    Q_OBJECT

public:
    explicit FriendListWidget(TcpClient* tsocket, QWidget *parent = nullptr);

private slots:
    void fetchFriends();
    void displayFriends(const QString &friendsData);

private:
    TcpClient* tsocket;
    QListWidget *friendListWidget;
    QVBoxLayout *layout;
};

#endif // FRIENDLISTWIDGET_H
