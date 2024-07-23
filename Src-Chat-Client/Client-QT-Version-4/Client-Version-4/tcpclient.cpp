#include "tcpclient.h"
#include <QHostAddress>
#include <QDebug>

TcpClient::TcpClient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)) {
    message = new QString();
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
}

TcpClient::~TcpClient() {}

void TcpClient::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
    if (!socket->waitForConnected(3000)) {
        //qDebug() << "Connection failed!";
    } else {
        //qDebug() << "Connected to server!";
    }
}

void TcpClient::sendCommand(const QString &command) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(command.toUtf8());
    } else {
        //qDebug() << "Not connected to server!";
    }
}

void TcpClient::onReadyRead() {
    while (socket->bytesAvailable()) {
        QByteArray data = socket->readAll();
        message->clear();
        *message = QString::fromUtf8(data);
        //qDebug() << "recv info from server:"<<message.toUtf8();
        emit receivedMessage(*message);
    }
}




