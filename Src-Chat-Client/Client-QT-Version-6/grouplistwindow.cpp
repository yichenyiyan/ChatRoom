#include "grouplistwindow.h"
#include "menu.h"
#include <QString>
#include "tcpclient.h"
#include "messagedetailwindow.h"
#include "messagelistwindow.h"
#include <QCoreApplication>
#include <QStringList>



GroupListWindow::GroupListWindow(const QString& _username, MessageListWindow* _msglist, QWidget *parent)
    : QMainWindow(parent),  msglist(_msglist), username(_username), tsocket(new TcpClient()) {
    this->setFixedSize(400, 600);
    centerWindow();
    setStyleSheet("background-color: lightblue;");
    setWindowTitle(username + "'s joined groups");
    friendListWidget = new QListWidget(this);
    setCentralWidget(friendListWidget);

    tsocket->sendCommand("b " + username);
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


    connect(friendListWidget, &QListWidget::itemClicked, this, &GroupListWindow::onFriendClicked);
}

void GroupListWindow::centerWindow() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void GroupListWindow::onFriendClicked(QListWidgetItem *item) {
    QString groupName = item->text();
    qDebug() << "btn "<< groupName << " Clicked";
    this->msglist->toAddGMessage(groupName, "");
    this->msglist->toOpenGMessageDetailWindow(groupName);
    this->close();
}

void GroupListWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}
GroupListWindow::~GroupListWindow(){
    delete tsocket;
}
