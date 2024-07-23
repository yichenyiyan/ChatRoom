#include "addfriendwindow.h"
#include "tcpclient.h"

#include <QScreen>
#include <QVBoxLayout>
#include <QApplication>
#include <QMessageBox>

AddFriendWindow::AddFriendWindow(const QString& _user, QWidget *parent) : QWidget(parent), user(_user) {
    this->setFixedSize(400, 600);
    this->setWindowTitle(user + " Add Friend");
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    friendUserLineEdit = new QLineEdit(this);
    friendUserLineEdit->setPlaceholderText("Friend's Username");
    connect(friendUserLineEdit, &QLineEdit::textChanged, this, &AddFriendWindow::on_friendUserLineEdit_textChanged);

    addFriendButton = new QPushButton("Add Friend", this);
    addFriendButton->setEnabled(false); // 初始状态为禁用
    connect(addFriendButton, &QPushButton::clicked, this, &AddFriendWindow::on_addFriendButton_clicked);

    layout->addWidget(friendUserLineEdit);
    layout->addWidget(addFriendButton);

    setLayout(layout);

    sendSocket = new TcpClient("127.0.0.1", 8888);

    connect(sendSocket, &TcpClient::receivedMessage, this, &AddFriendWindow::on_receiveResponse);
}

AddFriendWindow::~AddFriendWindow() {
    delete sendSocket;
}

void AddFriendWindow::on_addFriendButton_clicked() {
    QString friendUser = friendUserLineEdit->text();
    //qDebug() << "func on_addFriendButton_clicked: user: " << user <<" add friendName: " <<friendUser;
    if (!friendUser.isEmpty()) {
        QString command = "4 " + this->user + " " + friendUser;
        //qDebug() << command;
        sendSocket->sendCommand(command);
        //friendUserLineEdit->clear();
    }
}

void AddFriendWindow::on_friendUserLineEdit_textChanged(const QString &text) {
    addFriendButton->setEnabled(!text.isEmpty());
}

void AddFriendWindow::on_receiveResponse() {
    QString response = sendSocket->getMessage();

    if (response == "4") {
        QMessageBox::information(this, "Add Friend", "System error.");
    } else if (response == "2") {
        QMessageBox::information(this, "Add Friend", "You are already friends with this user.");
    } else if (response == "3") {
        QMessageBox::information(this, "Add Friend", "The user is not online.");
    } else if (response == "0") {
        QMessageBox::information(this, "Add Friend", "The user does not exist.");
    } else if (response == "1") {
        QMessageBox::information(this, "Add Friend", "Friend request sent.");
    } else {
        QMessageBox::information(this, "Add Friend", "Unknown response: " + response);
    }
    close();
}

void AddFriendWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}
