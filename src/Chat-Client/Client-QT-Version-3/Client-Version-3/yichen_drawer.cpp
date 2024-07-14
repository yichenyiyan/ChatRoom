#include "yichen_drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <iostream>



yichen_drawer::yichen_drawer(QWidget *parent, Qt::WindowFlags f)
    :QToolBox(parent, f) {
    setWindowTitle(tr("Chat Room"));
    setWindowIcon(QPixmap(":/yichen_images/ChatRoom.jpg"));

    addFriendsBtn = new QPushButton(tr("ADD"));
    connect(addFriendsBtn, SIGNAL(clicked()), this, SLOT(slotAddFriends()));

    toolBtn1 = new QToolButton;
    toolBtn1->setIcon(QPixmap(":/yichen_images/liu.png"));
    toolBtn1->setIconSize(QPixmap(":/yichen_images/liu.png").size());
    toolBtn1->setAutoRaise(true);
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn1, SIGNAL(clicked()), this, SLOT(showChatWidget1()));

    QGroupBox* groupBox = new QGroupBox;
    QVBoxLayout* layout = new QVBoxLayout(groupBox);

    layout->setMargin(20);
    layout->setAlignment(Qt::AlignLeft);
    layout->addWidget(toolBtn1);
    layout->addWidget(addFriendsBtn);
    layout->addStretch(); //插入一个占位符
    this->addItem((QWidget*)groupBox, tr("Member"));
}

void yichen_drawer::showChatWidget1()
{
    chat = new yichen_tcp();
    chat->username = toolBtn1->text();
    chat->setWindowTitle(toolBtn1->text());
    chat->show();
}

void yichen_drawer::slotAddFriends()
{
    addFriends = new yichen_addFriends();
    addFriends->show();
}






