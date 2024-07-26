#include "grouprequestwindow.h"
#include "tcpclient.h"

#include <QScreen>
#include <QVBoxLayout>
#include <QApplication>

GroupRequestWindow::GroupRequestWindow(const QString& _master_user, const QString& _req_user, const QString& _group_name, QWidget *parent)
    : QWidget(parent), req_user(_req_user), master_user(_master_user), group_name(_group_name) {
    this->setFixedSize(400, 200);
    this->setWindowTitle(master_user + "'s Group Request");
    centerWindow();
    setStyleSheet("background-color: lightblue;");

    QVBoxLayout *layout = new QVBoxLayout(this);

    messageLabel = new QLabel("Add Group " + group_name + "'s request from: " + req_user, this);

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

    connect(acceptButton, &QPushButton::clicked, this, &GroupRequestWindow::on_acceptButton_clicked);
    connect(rejectButton, &QPushButton::clicked, this, &GroupRequestWindow::on_rejectButton_clicked);
}

GroupRequestWindow::~GroupRequestWindow() {}

void GroupRequestWindow::on_acceptButton_clicked() {
    QString command = "d " + master_user +" " + req_user + " " + group_name + " accept";
    TcpClient* sendSocket = new TcpClient();
    sendSocket->sendCommand(command);
    this->close();
}

void GroupRequestWindow::on_rejectButton_clicked() {
    QString command = "d " + master_user +" " + req_user + " " + group_name + " reject";
    TcpClient* sendSocket = new TcpClient();
    sendSocket->sendCommand(command);
    this->close();
}

void GroupRequestWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}
