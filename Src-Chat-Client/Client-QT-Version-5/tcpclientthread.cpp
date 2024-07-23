#include "tcpclientthread.h"
#include <QDebug>

TcpClientThread::TcpClientThread(QObject *parent) : QThread(parent) {
    tcpClient = new TcpClient("127.0.0.1", 8888);
    connect(tcpClient, &TcpClient::receivedMessage, this, &TcpClientThread::handleMessage);
}

void TcpClientThread::run() {
    exec();
}

void TcpClientThread::handleMessage() {
    QString rawMessage = tcpClient->getMessage();
    if (rawMessage.isEmpty()) {
        return;
    }

    qDebug() << "handleMessage raw data : " <<rawMessage ;
    QChar messageType = rawMessage[0];
    QStringList parts = rawMessage.mid(1).trimmed().split(" ");

    if (messageType == '5' && parts.size() >= 2) {
        QString username = parts[0];
        QString message = parts.mid(1).join(" ");
        emit newMessageReceived(username, message);
    } else if (messageType == '9' && parts.size() >= 2) {
        QString username1 = parts[0];
        QString username2 = parts[1];
        emit newMessageReceived2(username1, username2);
    }
}

void TcpClientThread::toUpdateServerFd(const QString& username) {
    this->tcpClient->sendCommand("8 " + username);
}

void TcpClientThread::toSend(const QString& username, const QString& target_user,const QString& msg) {
    this->tcpClient->sendCommand("5 " + username +" " +target_user + " " + msg);
}
