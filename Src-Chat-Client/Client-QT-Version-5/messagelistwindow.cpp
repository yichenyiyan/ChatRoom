#include "friendrequestwindow.h"
#include "friendlistwindow.h"
#include "messagelistwindow.h"
#include <QScrollArea>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QDate>

MessageListWindow::MessageListWindow(const QString& _user, QWidget *parent)
    : QWidget(parent), user(_user) {
    setupUI();

    tcpClientThread = new TcpClientThread(this);
    tcpClientThread->toUpdateServerFd(user);
    connect(tcpClientThread, &TcpClientThread::newMessageReceived, this, &MessageListWindow::addMessage);
    connect(tcpClientThread, &TcpClientThread::newMessageReceived2, this, &MessageListWindow::addFriendWin);

    tcpClientThread->start();
}

MessageListWindow::~MessageListWindow() {
    tcpClientThread->quit();
    tcpClientThread->wait();
}

void MessageListWindow::setupUI() {
    setFixedSize(400, 600);
    setStyleSheet("background-color: lightblue;");

    mainLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    container = new QWidget();
    containerLayout = new QVBoxLayout(container);
    container->setLayout(containerLayout);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(container);

    mainLayout->addWidget(scrollArea);
}

void MessageListWindow::addMessage(const QString &username, const QString &message) {
    QPushButton *messageButton;
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString displayDate = currentDate.toString("yyyy-MM-dd");
    QString displayTime = currentTime.toString("HH:mm");

    // 制作显示文本，并确保日期和时间位于最右边
    QString displayText = QString("%1%2\n%3%4")
                              .arg(username, -20) // 左对齐用户名
                              .arg(displayDate, 40 - username.length(), QLatin1Char(' ')) // 右对齐日期
                              .arg((message.length() > 20 ? message.left(20) + "..." : message), -20) // 左对齐消息
                              .arg(displayTime, 40 - message.length(), QLatin1Char(' ')); // 右对齐时间

    if (userButtons.contains(username)) {
        messageButton = userButtons[username];
        messageButton->setText(displayText);
        containerLayout->removeWidget(messageButton);
        containerLayout->insertWidget(0, messageButton);
    } else {
        messageButton = new QPushButton(displayText, this);
        messageButton->setFixedSize(scrollArea->width() - 20, 60);
        messageButton->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: 1px solid black; color: black; text-align: left; border-radius: 10px;");
        connect(messageButton, &QPushButton::clicked, this, [this, username]() {
            openMessageDetailWindow(username);
        });

        containerLayout->insertWidget(0, messageButton);
        userButtons[username] = messageButton;
    }

    if (userWindows.contains(username)) {
        userWindows[username]->addMessage(username, message);
    }
}

void MessageListWindow::toAddMessage(const QString &username, const QString &message) {
    QPushButton *messageButton;
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString displayDate = currentDate.toString("yyyy-MM-dd");
    QString displayTime = currentTime.toString("HH:mm");

    // 制作显示文本，并确保日期和时间位于最右边
    QString displayText = QString("%1%2\n%3%4")
                              .arg(username, -20) // 左对齐用户名
                              .arg(displayDate, 40 - username.length(), QLatin1Char(' ')) // 右对齐日期
                              .arg((message.length() > 20 ? message.left(20) + "..." : message), -20) // 左对齐消息
                              .arg(displayTime, 40 - message.length(), QLatin1Char(' ')); // 右对齐时间

    if (userButtons.contains(username)) {
        messageButton = userButtons[username];
        messageButton->setText(displayText);
        containerLayout->removeWidget(messageButton);
        containerLayout->insertWidget(0, messageButton);
    } else {
        messageButton = new QPushButton(displayText, this);
        messageButton->setFixedSize(scrollArea->width() - 20, 60);
        messageButton->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: 1px solid black; color: black; text-align: left; border-radius: 10px;");
        connect(messageButton, &QPushButton::clicked, this, [this, username]() {
            openMessageDetailWindow(username);
        });

        containerLayout->insertWidget(0, messageButton);
        userButtons[username] = messageButton;
    }

    if (userWindows.contains(username)) {
        userWindows[username]->addMessage(username, message);
    }
}

void MessageListWindow::openMessageDetailWindow(const QString &username) {
    MessageDetailWindow *detailWindow;
    if (userWindows.contains(username)) {
        detailWindow = userWindows[username];
    } else {
        detailWindow = new MessageDetailWindow(user, username, nullptr);
        userWindows[username] = detailWindow;
    }
    detailWindow->show();
}

void MessageListWindow::toOpenMessageDetailWindow(const QString &username) {
    MessageDetailWindow *detailWindow;
    if (userWindows.contains(username)) {
        detailWindow = userWindows[username];
    } else {
        detailWindow = new MessageDetailWindow(user, username, nullptr);
        userWindows[username] = detailWindow;
    }
    detailWindow->show();
}

void MessageListWindow::addFriendWin(const QString& req_user, const QString& tar_user) {
    if (tar_user != this->user)
            return;

    FriendRequestWindow* addfriend = new FriendRequestWindow(user, req_user, nullptr);
    addfriend->show();
}
