#ifndef YICHEN_DRAWER_H
#define YICHEN_DRAWER_H

#include <QToolButton>
#include <QPushButton>
#include <QToolBox>

#include "yichen_tcp.h"
#include "yichen_addfriends.h"



class yichen_drawer : public QToolBox {
    Q_OBJECT
public:
    yichen_drawer(QWidget* parent = 0, Qt::WindowFlags f = 0);


public:
    QString username;
    QToolButton* toolBtn1;
    QPushButton* addFriendsBtn;

private slots:
    void showChatWidget1();
    void slotAddFriends();

private:
    yichen_tcp* chat;
    yichen_addFriends* addFriends;
};

#endif // YICHEN_DRAWER_H
