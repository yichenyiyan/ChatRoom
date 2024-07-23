#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QObject>

class TcpClient : public QObject {
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    void connectToServer(const QString &host = "127.0.0.1", quint16 port = 8888);
    void sendCommand(const QString &command);
    inline QString getMessage() { return *message; }
    inline void destroySelf() { delete this; }

signals:
    void receivedMessage(const QString &message);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
    QString* message;
};

#endif // TCPCLIENT_H
