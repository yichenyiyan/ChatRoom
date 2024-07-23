#include "tcpclient.h"
#include <QDebug>

TcpClient::TcpClient(const QString &host, quint16 port, QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this)) {

    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &TcpClient::onErrorOccurred);

    socket->connectToHost(QHostAddress(host), port);

    if (!socket->waitForConnected(3000)) {
        qDebug() << "Connection failed:" << socket->errorString();
    } else {
        qDebug() << "Connected to server.";
    }
}

TcpClient::~TcpClient() {
    qDebug() << "delete a TcpClient!";
}

void TcpClient::sendCommand(const QString &command) {
    if (socket->isOpen()) {
        socket->write(command.toUtf8());
        socket->flush();
        qDebug() <<"tcp socket send: " << command;
    } else {
        qDebug() << "Socket is not connected.";
    }
}

QString TcpClient::getMessage() {
    return messageBuffer;
}

void TcpClient::onReadyRead() {
    messageBuffer = socket->readAll();
    emit receivedMessage();
}

void TcpClient::onDisconnected() {
    qDebug() << "Disconnected from server.";
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError;
}
