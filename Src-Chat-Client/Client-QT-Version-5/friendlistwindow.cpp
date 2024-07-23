#include "friendlistwindow.h"
#include "menu.h"
#include <QString>
#include "tcpclient.h"
#include "messagedetailwindow.h"
#include "messagelistwindow.h"
#include <QCoreApplication>
#include <QStringList>



FriendListWindow::FriendListWindow(const QString& _username, MessageListWindow* _msglist, QWidget *parent)
    : QMainWindow(parent),  msglist(_msglist), username(_username), tsocket(new TcpClient()) {
    this->setFixedSize(400, 600);
    centerWindow();
    setStyleSheet("background-color: lightblue;");
    setWindowTitle(username + "'s friends");
    friendListWidget = new QListWidget(this);
    setCentralWidget(friendListWidget);

    tsocket->sendCommand("7 "+username);
    QObject::connect(tsocket, &TcpClient::receivedMessage, [&](){
        QString msg = tsocket->getMessage();
        qDebug() << msg;
        // 使用空格作为分隔符来分割字符串
        QStringList words = msg.split(' ', QString::SkipEmptyParts);

        // 输出每个单词
        for (const QString& word : words) {
            qDebug() << word;
            friendListWidget->addItem(word);
        }

    });


    connect(friendListWidget, &QListWidget::itemClicked, this, &FriendListWindow::onFriendClicked);
}

void FriendListWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void FriendListWindow::onFriendClicked(QListWidgetItem *item) {
    QString friendName = item->text();
    qDebug() << "btn "<< friendName << " Clicked";
    this->msglist->toAddMessage(friendName, "");
    this->msglist->toOpenMessageDetailWindow(friendName);
    this->close();
}

void FriendListWindow::closeEvent(QCloseEvent *event) {

    event->accept();
}
FriendListWindow::~FriendListWindow(){}
