#include "addfriend.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AddFriend::AddFriend(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(400, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);

    requestUserLineEdit = new QLineEdit(this);
    requestUserLineEdit->setPlaceholderText("Your Username");

    friendUserLineEdit = new QLineEdit(this);
    friendUserLineEdit->setPlaceholderText("Friend's Username");

    addFriendButton = new QPushButton("Add Friend", this);

    layout->addWidget(requestUserLineEdit);
    layout->addWidget(friendUserLineEdit);
    layout->addWidget(addFriendButton);

    connect(addFriendButton, &QPushButton::clicked, this, &AddFriend::on_addFriendButton_clicked);

    socket = new QTcpSocket(this);
}

AddFriend::~AddFriend() {}

void AddFriend::on_addFriendButton_clicked() {
    QString requestUser = requestUserLineEdit->text();
    QString friendUser = friendUserLineEdit->text();
    QString command = "4 " + requestUser + " " + friendUser;

    socket->connectToHost("127.0.0.1", 12345);
    if (socket->waitForConnected()) {
        socket->write(command.toUtf8());
    }
}
