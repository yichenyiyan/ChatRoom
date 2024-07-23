#include "friendrequestwindow.h"
#include "tcpclient.h"

#include <QScreen>
#include <QVBoxLayout>
#include <QApplication>

FriendRequestWindow::FriendRequestWindow(const QString& _this_username, const QString& username, QWidget *parent)
    : QWidget(parent), username(username), this_username(_this_username) {
    this->setFixedSize(400, 200);
    this->setWindowTitle(this_username + "'s Add Friend Request");
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    messageLabel = new QLabel("Friend request from: " + username, this);

    acceptButton = new QPushButton("Accept", this);
    rejectButton = new QPushButton("Reject", this);

    QString buttonStyle = "QPushButton { border: 1px solid black; border-radius: 10px; background-color: rgba(0, 0, 0, 0); color: black; }";
    acceptButton->setStyleSheet("QPushButton { border: 2px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }");
    rejectButton->setStyleSheet("QPushButton { border: 2px solid #8f8f91; border-radius: 10px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); }");

    // 或者，如果你只想改变border-radius，可以单独设置
    // button->setStyleSheet("QPushButton { border-radius: 10px; }");


    layout->addWidget(messageLabel);
    layout->addWidget(acceptButton);
    layout->addWidget(rejectButton);

    setLayout(layout);

    connect(acceptButton, &QPushButton::clicked, this, &FriendRequestWindow::on_acceptButton_clicked);
    connect(rejectButton, &QPushButton::clicked, this, &FriendRequestWindow::on_rejectButton_clicked);
}

FriendRequestWindow::~FriendRequestWindow() {}

void FriendRequestWindow::on_acceptButton_clicked() {
    // Send accept request to the server
    QString command = "6 " + this_username + " " + username + " accept";
    TcpClient* sendSocket = new TcpClient();
    sendSocket->sendCommand(command);
    this->close();
}

void FriendRequestWindow::on_rejectButton_clicked() {
    // Send reject request to the server
    QString command = "6 " + this_username + " " + username + " reject";
    TcpClient* sendSocket = new TcpClient();
    sendSocket->sendCommand(command);
    this->close();
}

void FriendRequestWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}
