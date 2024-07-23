#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>

class TcpClient : public QObject {
    Q_OBJECT

public:
    explicit TcpClient(const QString &host = "127.0.0.1", quint16 port = 8888, QObject *parent = nullptr);
    ~TcpClient();

    void sendCommand(const QString &command);
    QString getMessage();

signals:
    void receivedMessage();

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
    QString messageBuffer;
};

#endif // TCPCLIENT_H
